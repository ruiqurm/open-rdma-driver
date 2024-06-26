use std::{
    collections::HashMap,
    fmt::Debug,
    sync::{
        atomic::{AtomicBool, Ordering},
        Arc,
    },
    thread::sleep,
    time::{Duration, SystemTime, UNIX_EPOCH},
    u128,
};

use flume::{Receiver, Sender};

use crate::{
    device::ToCardWorkRbDesc,
    op_ctx::OpCtx,
    types::{Msn, Qpn},
    Error, ThreadSafeHashmap, WorkDescriptorSender,
};

#[derive(Debug)]
pub(crate) struct RetryContext {
    descriptor: Box<ToCardWorkRbDesc>,
    retry_counter: u32,
    next_timeout: u128,
}

/// Typically the checking_interval should at most 1% of retry_timeout
/// So that the retrying won't drift too much
#[derive(Debug, Clone, Copy)]
pub struct RetryConfig {
    is_enable : bool,
    max_retry: u32,
    retry_timeout: u128,
    checking_interval: Duration,
}

impl RetryConfig {
    /// Create a new retry config
    pub fn new(
        is_enable: bool,
        max_retry: u32,
        retry_timeout: Duration,
        checking_interval: Duration,
    ) -> Self {
        Self {
            is_enable,
            max_retry,
            retry_timeout: retry_timeout.as_millis(),
            checking_interval,
        }
    }
}

// Main thread will send a retry record to retry monitor
pub(crate) struct RetryRecord {
    descriptor: Box<ToCardWorkRbDesc>,
    qpn: Qpn,
    msn: Msn,
}

impl RetryRecord {
    pub(crate) fn new(descriptor: Box<ToCardWorkRbDesc>, qpn: Qpn, msn: Msn) -> Self {
        Self {
            descriptor,
            qpn,
            msn,
        }
    }
}

pub(crate) struct RetryCancel {
    qpn: Qpn,
    msn: Msn,
}

pub(crate) enum RetryEvent {
    Retry(RetryRecord),
    Cancel(RetryCancel),
}

pub(crate) struct RetryMonitorContext {
    pub(crate) map: HashMap<(Qpn, Msn), RetryContext>,
    pub(crate) receiver: Receiver<RetryEvent>,
    pub(crate) device: Arc<dyn WorkDescriptorSender>,
    pub(crate) user_op_ctx_map: ThreadSafeHashmap<(Qpn, Msn), OpCtx<()>>,
    pub(crate) config: RetryConfig,
}

/// get current time in ms
#[allow(clippy::unwrap_used)]
fn get_current_time() -> u128 {
    SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap()
        .as_millis()
}

#[derive(Debug)]
pub(crate) struct RetryMonitor {
    sender: Sender<RetryEvent>,
    stop_flag: Arc<AtomicBool>,
    thread: Option<std::thread::JoinHandle<()>>,
}

impl RetryMonitor {
    pub(crate) fn new(sender: Sender<RetryEvent>, mut context: RetryMonitorContext) -> Self {
        let stop_flag = Arc::new(AtomicBool::new(false));
        let stop_flag_clone = Arc::<AtomicBool>::clone(&stop_flag);
        let thread = std::thread::spawn(move || {
            retry_monitor_working_thread(&stop_flag_clone, &mut context);
        });
        Self {
            sender,
            stop_flag,
            thread: Some(thread),
        }
    }

    pub(crate) fn subscribe(&self, event: RetryEvent) -> Result<(), Error> {
        self.sender
            .send(event)
            .map_err(|e| Error::ResourceNoAvailable(e.to_string()))
    }
}

impl RetryMonitorContext {
    fn check_receive(&mut self) {
        while let Ok(record) = self.receiver.try_recv() {
            if self.config.is_enable{
                match record {
                    RetryEvent::Retry(record) => self.handle_retry(record),
                    RetryEvent::Cancel(cancel) => self.handle_cancel(&cancel),
                }
            }
        }
    }

    #[allow(clippy::arithmetic_side_effects)]
    fn handle_retry(&mut self, record: RetryRecord) {
        let key = (record.qpn, record.msn);
        let ctx = RetryContext {
            descriptor: record.descriptor,
            retry_counter: self.config.max_retry,
            next_timeout: get_current_time() + self.config.retry_timeout,
        };
        if self.map.insert(key, ctx).is_some() {
            // receive same record more than once
            log::warn!("Receive same retry record more than once");
        }
    }

    fn handle_cancel(&mut self, cancel: &RetryCancel) {
        let key = (cancel.qpn, cancel.msn);
        if self.map.remove(&key).is_none() {
            // remove failed
            log::warn!("Remove retry record failed.");
        }
    }

    #[allow(clippy::arithmetic_side_effects)]
    fn check_timeout(&mut self) {
        let now = get_current_time();
        let mut has_removed = false;
        for (key, ctx) in self.map.iter_mut() {
            if ctx.next_timeout <= now {
                if ctx.retry_counter > 0 {
                    ctx.retry_counter -= 1;
                    ctx.next_timeout = now + self.config.retry_timeout;
                    if self.device.send_work_desc(ctx.descriptor.clone()).is_err() {
                        log::error!("Retry send work descriptor failed")
                    }
                } else {
                    // Encounter max retry, remove it and tell user the error
                    has_removed = true;
                    let guard = self.user_op_ctx_map.write();
                    if let Some(user_op_ctx) = guard.get(key) {
                        user_op_ctx.set_error("exceed max retry count");
                    } else {
                        log::warn!("Remove retry record failed: Can not find {key:?}");
                    }
                }
            }
        }
        if has_removed {
            self.map.retain(|_, ctx| ctx.retry_counter != 0);
        }
    }
}

fn retry_monitor_working_thread(stop_flag: &AtomicBool, monitor: &mut RetryMonitorContext) {
    while !stop_flag.load(Ordering::Relaxed) {
        monitor.check_receive();
        monitor.check_timeout();
        // sleep for an interval
        sleep(monitor.config.checking_interval);
    }
}

#[cfg(test)]
mod test {
    use std::{collections::HashMap, sync::Arc, time::Duration};

    use parking_lot::{lock_api::RwLock, Mutex, RawRwLock};

    use crate::{
        device::{DescSge, ToCardWorkRbDesc, ToCardWorkRbDescCommon, ToCardWorkRbDescWrite},
        op_ctx::{self, CtxStatus},
        types::{Key, Msn, Qpn, ThreeBytesStruct},
        Error, WorkDescriptorSender,
    };

    use super::{RetryConfig, RetryEvent, RetryMonitorContext};
    struct MockDevice(Mutex<Vec<ToCardWorkRbDesc>>);

    impl WorkDescriptorSender for MockDevice {
        fn send_work_desc(&self, desc: Box<ToCardWorkRbDesc>) -> Result<(), Error> {
            self.0.lock().push(*desc);
            Ok(())
        }
    }

    #[test]
    fn test_retry_monitor() {
        let map = Arc::new(RwLock::new(HashMap::new()));
        let (sender, receiver) = flume::unbounded();
        let device = Arc::new(MockDevice(Vec::new().into()));
        let context = RetryMonitorContext {
            map: HashMap::new(),
            receiver,
            device: Arc::<MockDevice>::clone(&device),
            user_op_ctx_map: Arc::<
                RwLock<RawRwLock, HashMap<(ThreeBytesStruct, Msn), op_ctx::OpCtx<()>>>,
            >::clone(&map),
            config: RetryConfig::new(
                true,
                3,
                Duration::from_millis(1000),
                std::time::Duration::from_millis(10),
            ),
        };
        map.write().insert(
            (Qpn::default(), Msn::default()),
            op_ctx::OpCtx::new_running(),
        );
        let _monitor = super::RetryMonitor::new(sender.clone(),context);
        let desc = Box::new(ToCardWorkRbDesc::Write(ToCardWorkRbDescWrite {
            common: ToCardWorkRbDescCommon {
                ..Default::default()
            },
            is_last: true,
            is_first: true,
            sge0: DescSge {
                addr: 0x1000,
                len: 512,
                key: Key::new(0x1234_u32),
            },
            sge1: None,
            sge2: None,
            sge3: None,
        }));
        for _i in 0..4 {
            sender
                .send(RetryEvent::Retry(super::RetryRecord {
                    descriptor: desc.clone(),
                    qpn: Qpn::default(),
                    msn: Msn::default(),
                }))
                .unwrap();
            // should send first retry
            std::thread::sleep(std::time::Duration::from_millis(1020));
            assert_eq!(device.0.lock().len(), 1);
            // should send second retry
            std::thread::sleep(std::time::Duration::from_millis(1020));
            assert_eq!(device.0.lock().len(), 2);
            // should send last retry
            std::thread::sleep(std::time::Duration::from_millis(1020));
            assert_eq!(device.0.lock().len(), 3);

            std::thread::sleep(std::time::Duration::from_millis(1020));
            // should remove the record
            matches!(
                map.read()
                    .get(&(Qpn::default(), Msn::default()))
                    .unwrap()
                    .status(),
                CtxStatus::Failed(_)
            );
            device.0.lock().clear();
            std::thread::sleep(std::time::Duration::from_millis(1000));
        }

        // sender
        //     .send(RetryEvent::Retry(super::RetryRecord {
        //         descriptor: desc,
        //         qpn: Qpn::default(),
        //         msn: Msn::default(),
        //     }))
        //     .unwrap();
        // std::thread::sleep(std::time::Duration::from_millis(120));

        // // should send first retry
        // assert_eq!(device.0.lock().len(), 1);
        // sender
        //     .send(RetryEvent::Cancel(super::RetryCancel {
        //         qpn: Qpn::default(),
        //         msn: Msn::default(),
        //     }))
        //     .unwrap();
        // std::thread::sleep(std::time::Duration::from_millis(105));
        // assert_eq!(device.0.lock().len(), 1);
    }
}
