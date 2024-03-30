use std::{
    error::Error,
    net::Ipv4Addr,
    sync::Arc,
    thread::{spawn, JoinHandle},
};

use crossbeam_queue::SegQueue;

use self::{
    logic::{BlueRDMALogic, BlueRdmaLogicError},
    net_agent::udp_agent::{UDPReceiveAgent, UDPSendAgent},
};

use super::{
    scheduler::{round_robin::RoundRobinStrategy, DescriptorScheduler},
    DeviceAdaptor, DeviceError, ToCardCtrlRbDesc, ToCardRb, ToCardWorkRbDesc, ToHostCtrlRbDesc,
    ToHostRb, ToHostWorkRbDesc,
};

mod logic;
mod net_agent;
mod packet;
mod packet_processor;
#[cfg(test)]
pub(crate) mod tests;
mod types;

/// An software device implementation of the device.
///
/// # Examples:
/// ```
/// let device = SoftwareDevice::init().unwrap();
/// let ctrl_rb = device.to_card_ctrl_rb();
// // ctrl_rb.push(desc) // create mr or qp
/// let data_send_rb = device.to_card_work_rb();
/// // data_rb.push(desc) // send data
/// let data_recv_rb = device.to_host_work_rb();
/// // data_recv_rb.pop() // recv data
/// ```
#[allow(dead_code)]
pub(crate) struct SoftwareDevice {
    recv_agent: UDPReceiveAgent,
    polling_thread: JoinHandle<()>,
    to_card_ctrl_rb: ToCardCtrlRb,
    to_host_ctrl_rb: ToHostCtrlRb,
    to_card_work_rb: ToCardWorkRb,
    to_host_work_rb: ToHostWorkRb,
}

#[derive(Clone)]
struct ToCardCtrlRb(Arc<BlueRDMALogic>);
#[derive(Clone)]
struct ToHostCtrlRb;
#[derive(Clone)]
struct ToCardWorkRb(Arc<DescriptorScheduler>);
#[derive(Clone)]
struct ToHostWorkRb(Arc<SegQueue<ToHostWorkRbDesc>>);

impl SoftwareDevice {
    /// Initializing an software device.
    pub(crate) fn init(addr: Ipv4Addr, port: u16) -> Result<Self, Box<dyn Error>> {
        let send_agent = UDPSendAgent::new()?;
        let device = Arc::new(BlueRDMALogic::new(Arc::new(send_agent)));
        // The strategy is a global singleton, so we leak it
        let round_robin = Arc::new(RoundRobinStrategy::new());
        let scheduler = DescriptorScheduler::new(round_robin);
        let scheduler = Arc::new(scheduler);
        let to_host_queue = device.get_to_host_descriptor_queue();
        let recv_agent = UDPReceiveAgent::new(Arc::<BlueRDMALogic>::clone(&device), addr, port)?;

        let this_scheduler = Arc::<DescriptorScheduler>::clone(&scheduler);
        let this_device = Arc::<BlueRDMALogic>::clone(&device);
        let polling_thread = spawn(move || loop {
            if let Some(to_card_ctrl_rb_desc) = this_scheduler.pop() {
                let _: Result<(), BlueRdmaLogicError> = this_device.send(to_card_ctrl_rb_desc);
            }
        });
        let to_card_work_rb = ToCardWorkRb(scheduler);
        Ok(Self {
            recv_agent,
            polling_thread,
            to_card_ctrl_rb: ToCardCtrlRb(device),
            to_host_ctrl_rb: ToHostCtrlRb,
            to_card_work_rb,
            to_host_work_rb: ToHostWorkRb(to_host_queue),
        })
    }
}

impl DeviceAdaptor for SoftwareDevice {
    fn to_card_ctrl_rb(&self) -> Arc<dyn ToCardRb<ToCardCtrlRbDesc>> {
        Arc::new(self.to_card_ctrl_rb.clone())
    }

    fn to_host_ctrl_rb(&self) -> Arc<dyn ToHostRb<ToHostCtrlRbDesc>> {
        todo!()
    }

    fn to_card_work_rb(&self) -> Arc<dyn ToCardRb<ToCardWorkRbDesc>> {
        Arc::new(self.to_card_work_rb.clone())
    }

    fn to_host_work_rb(&self) -> Arc<dyn ToHostRb<ToHostWorkRbDesc>> {
        Arc::new(self.to_host_work_rb.clone())
    }

    fn read_csr(&self, _addr: usize) -> Result<u32, DeviceError> {
        todo!()
    }

    fn write_csr(&self, _addr: usize, _data: u32) -> Result<(), DeviceError> {
        todo!()
    }

    fn get_phys_addr(&self, virt_addr: usize) -> Result<usize, DeviceError> {
        Ok(virt_addr)
    }
}

impl ToCardRb<ToCardCtrlRbDesc> for ToCardCtrlRb {
    fn push(&self, desc: ToCardCtrlRbDesc) -> Result<(), DeviceError> {
        self.0.update(desc).unwrap();
        Ok(())
    }
}

impl ToHostRb<ToHostCtrlRbDesc> for ToHostCtrlRb {
    fn pop(&self) -> Result<ToHostCtrlRbDesc, DeviceError> {
        todo!()
    }
}

impl ToHostRb<ToHostWorkRbDesc> for ToHostWorkRb {
    fn pop(&self) -> Result<ToHostWorkRbDesc, DeviceError> {
        self.0.pop().ok_or(DeviceError::Device(
            "Failed to read from ringbuf".to_owned(),
        ))
    }
}

impl ToCardRb<ToCardWorkRbDesc> for ToCardWorkRb {
    fn push(&self, desc: ToCardWorkRbDesc) -> Result<(), DeviceError> {
        self.0.push(desc)
    }
}
