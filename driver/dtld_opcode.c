// SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB
/*
 * Copyright (c) 2016 Mellanox Technologies Ltd. All rights reserved.
 * Copyright (c) 2015 System Fabric Works, Inc. All rights reserved.
 */

#include <rdma/ib_pack.h>
#include "dtld_opcode.h"
#include "dtld_hdr.h"

/* useful information about work request opcodes and pkt opcodes in
 * table form
 */
struct dtld_wr_opcode_info dtld_wr_opcode_info[] = {
	[IB_WR_RDMA_WRITE]				= {
		.name	= "IB_WR_RDMA_WRITE",
		.mask	= {
			[IB_QPT_RC]	= WR_INLINE_MASK | WR_WRITE_MASK,
			[IB_QPT_UC]	= WR_INLINE_MASK | WR_WRITE_MASK,
		},
	},
	[IB_WR_RDMA_WRITE_WITH_IMM]			= {
		.name	= "IB_WR_RDMA_WRITE_WITH_IMM",
		.mask	= {
			[IB_QPT_RC]	= WR_INLINE_MASK | WR_WRITE_MASK,
			[IB_QPT_UC]	= WR_INLINE_MASK | WR_WRITE_MASK,
		},
	},
	[IB_WR_SEND]					= {
		.name	= "IB_WR_SEND",
		.mask	= {
			[IB_QPT_GSI]	= WR_INLINE_MASK | WR_SEND_MASK,
			[IB_QPT_RC]	= WR_INLINE_MASK | WR_SEND_MASK,
			[IB_QPT_UC]	= WR_INLINE_MASK | WR_SEND_MASK,
			[IB_QPT_UD]	= WR_INLINE_MASK | WR_SEND_MASK,
		},
	},
	[IB_WR_SEND_WITH_IMM]				= {
		.name	= "IB_WR_SEND_WITH_IMM",
		.mask	= {
			[IB_QPT_GSI]	= WR_INLINE_MASK | WR_SEND_MASK,
			[IB_QPT_RC]	= WR_INLINE_MASK | WR_SEND_MASK,
			[IB_QPT_UC]	= WR_INLINE_MASK | WR_SEND_MASK,
			[IB_QPT_UD]	= WR_INLINE_MASK | WR_SEND_MASK,
		},
	},
	[IB_WR_RDMA_READ]				= {
		.name	= "IB_WR_RDMA_READ",
		.mask	= {
			[IB_QPT_RC]	= WR_READ_MASK,
		},
	},
	[IB_WR_ATOMIC_CMP_AND_SWP]			= {
		.name	= "IB_WR_ATOMIC_CMP_AND_SWP",
		.mask	= {
			[IB_QPT_RC]	= WR_ATOMIC_MASK,
		},
	},
	[IB_WR_ATOMIC_FETCH_AND_ADD]			= {
		.name	= "IB_WR_ATOMIC_FETCH_AND_ADD",
		.mask	= {
			[IB_QPT_RC]	= WR_ATOMIC_MASK,
		},
	},
	[IB_WR_LSO]					= {
		.name	= "IB_WR_LSO",
		.mask	= {
			/* not supported */
		},
	},
	[IB_WR_SEND_WITH_INV]				= {
		.name	= "IB_WR_SEND_WITH_INV",
		.mask	= {
			[IB_QPT_RC]	= WR_INLINE_MASK | WR_SEND_MASK,
			[IB_QPT_UC]	= WR_INLINE_MASK | WR_SEND_MASK,
			[IB_QPT_UD]	= WR_INLINE_MASK | WR_SEND_MASK,
		},
	},
	[IB_WR_RDMA_READ_WITH_INV]			= {
		.name	= "IB_WR_RDMA_READ_WITH_INV",
		.mask	= {
			[IB_QPT_RC]	= WR_READ_MASK,
		},
	},
	[IB_WR_LOCAL_INV]				= {
		.name	= "IB_WR_LOCAL_INV",
		.mask	= {
			[IB_QPT_RC]	= WR_LOCAL_OP_MASK,
		},
	},
	[IB_WR_REG_MR]					= {
		.name	= "IB_WR_REG_MR",
		.mask	= {
			[IB_QPT_RC]	= WR_LOCAL_OP_MASK,
		},
	},
	[IB_WR_BIND_MW]					= {
		.name	= "IB_WR_BIND_MW",
		.mask	= {
			[IB_QPT_RC]	= WR_LOCAL_OP_MASK,
			[IB_QPT_UC]	= WR_LOCAL_OP_MASK,
		},
	},
};

struct dtld_opcode_info dtld_opcode[DTLD_NUM_OPCODE] = {
	[IB_OPCODE_RC_SEND_FIRST]			= {
		.name	= "IB_OPCODE_RC_SEND_FIRST",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_REQ_MASK | DTLD_RWR_MASK |
			  DTLD_SEND_MASK | DTLD_START_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_RC_SEND_MIDDLE]		= {
		.name	= "IB_OPCODE_RC_SEND_MIDDLE",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_REQ_MASK | DTLD_SEND_MASK |
			  DTLD_MIDDLE_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_RC_SEND_LAST]			= {
		.name	= "IB_OPCODE_RC_SEND_LAST",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_REQ_MASK | DTLD_COMP_MASK |
			  DTLD_SEND_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_RC_SEND_LAST_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_RC_SEND_LAST_WITH_IMMEDIATE",
		.mask	= DTLD_IMMDT_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_COMP_MASK | DTLD_SEND_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},
	[IB_OPCODE_RC_SEND_ONLY]			= {
		.name	= "IB_OPCODE_RC_SEND_ONLY",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_REQ_MASK | DTLD_COMP_MASK |
			  DTLD_RWR_MASK | DTLD_SEND_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_RC_SEND_ONLY_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_RC_SEND_ONLY_WITH_IMMEDIATE",
		.mask	= DTLD_IMMDT_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_COMP_MASK | DTLD_RWR_MASK | DTLD_SEND_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},
	[IB_OPCODE_RC_RDMA_WRITE_FIRST]		= {
		.name	= "IB_OPCODE_RC_RDMA_WRITE_FIRST",
		.mask	= DTLD_RETH_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_WRITE_MASK | DTLD_START_MASK,
		.length = DTLD_BTH_BYTES + DTLD_RETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RETH_BYTES,
		}
	},
	[IB_OPCODE_RC_RDMA_WRITE_MIDDLE]		= {
		.name	= "IB_OPCODE_RC_RDMA_WRITE_MIDDLE",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_REQ_MASK | DTLD_WRITE_MASK |
			  DTLD_MIDDLE_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_RC_RDMA_WRITE_LAST]			= {
		.name	= "IB_OPCODE_RC_RDMA_WRITE_LAST",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_REQ_MASK | DTLD_WRITE_MASK |
			  DTLD_END_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_RC_RDMA_WRITE_LAST_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_RC_RDMA_WRITE_LAST_WITH_IMMEDIATE",
		.mask	= DTLD_IMMDT_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_WRITE_MASK | DTLD_COMP_MASK | DTLD_RWR_MASK |
			  DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},
	[IB_OPCODE_RC_RDMA_WRITE_ONLY]			= {
		.name	= "IB_OPCODE_RC_RDMA_WRITE_ONLY",
		.mask	= DTLD_RETH_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_WRITE_MASK | DTLD_START_MASK |
			  DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_RETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RETH_BYTES,
		}
	},
	[IB_OPCODE_RC_RDMA_WRITE_ONLY_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_RC_RDMA_WRITE_ONLY_WITH_IMMEDIATE",
		.mask	= DTLD_RETH_MASK | DTLD_IMMDT_MASK | DTLD_PAYLOAD_MASK |
			  DTLD_REQ_MASK | DTLD_WRITE_MASK |
			  DTLD_COMP_MASK | DTLD_RWR_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES + DTLD_RETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RETH]	= DTLD_BTH_BYTES,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES +
					  DTLD_RETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RETH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},
	[IB_OPCODE_RC_RDMA_READ_REQUEST]			= {
		.name	= "IB_OPCODE_RC_RDMA_READ_REQUEST",
		.mask	= DTLD_RETH_MASK | DTLD_REQ_MASK | DTLD_READ_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_RETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RETH_BYTES,
		}
	},
	[IB_OPCODE_RC_RDMA_READ_RESPONSE_FIRST]		= {
		.name	= "IB_OPCODE_RC_RDMA_READ_RESPONSE_FIRST",
		.mask	= DTLD_AETH_MASK | DTLD_PAYLOAD_MASK | DTLD_ACK_MASK |
			  DTLD_START_MASK,
		.length = DTLD_BTH_BYTES + DTLD_AETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_AETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_AETH_BYTES,
		}
	},
	[IB_OPCODE_RC_RDMA_READ_RESPONSE_MIDDLE]		= {
		.name	= "IB_OPCODE_RC_RDMA_READ_RESPONSE_MIDDLE",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_ACK_MASK | DTLD_MIDDLE_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_RC_RDMA_READ_RESPONSE_LAST]		= {
		.name	= "IB_OPCODE_RC_RDMA_READ_RESPONSE_LAST",
		.mask	= DTLD_AETH_MASK | DTLD_PAYLOAD_MASK | DTLD_ACK_MASK |
			  DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_AETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_AETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_AETH_BYTES,
		}
	},
	[IB_OPCODE_RC_RDMA_READ_RESPONSE_ONLY]		= {
		.name	= "IB_OPCODE_RC_RDMA_READ_RESPONSE_ONLY",
		.mask	= DTLD_AETH_MASK | DTLD_PAYLOAD_MASK | DTLD_ACK_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_AETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_AETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_AETH_BYTES,
		}
	},
	[IB_OPCODE_RC_ACKNOWLEDGE]			= {
		.name	= "IB_OPCODE_RC_ACKNOWLEDGE",
		.mask	= DTLD_AETH_MASK | DTLD_ACK_MASK | DTLD_START_MASK |
			  DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_AETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_AETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_AETH_BYTES,
		}
	},
	[IB_OPCODE_RC_ATOMIC_ACKNOWLEDGE]			= {
		.name	= "IB_OPCODE_RC_ATOMIC_ACKNOWLEDGE",
		.mask	= DTLD_AETH_MASK | DTLD_ATMACK_MASK | DTLD_ACK_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_ATMACK_BYTES + DTLD_AETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_AETH]	= DTLD_BTH_BYTES,
			[DTLD_ATMACK]	= DTLD_BTH_BYTES +
					  DTLD_AETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_ATMACK_BYTES +
					  DTLD_AETH_BYTES,
		}
	},
	[IB_OPCODE_RC_COMPARE_SWAP]			= {
		.name	= "IB_OPCODE_RC_COMPARE_SWAP",
		.mask	= DTLD_ATMETH_MASK | DTLD_REQ_MASK | DTLD_ATOMIC_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_ATMETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_ATMETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_ATMETH_BYTES,
		}
	},
	[IB_OPCODE_RC_FETCH_ADD]			= {
		.name	= "IB_OPCODE_RC_FETCH_ADD",
		.mask	= DTLD_ATMETH_MASK | DTLD_REQ_MASK | DTLD_ATOMIC_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_ATMETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_ATMETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_ATMETH_BYTES,
		}
	},
	[IB_OPCODE_RC_SEND_LAST_WITH_INVALIDATE]		= {
		.name	= "IB_OPCODE_RC_SEND_LAST_WITH_INVALIDATE",
		.mask	= DTLD_IETH_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_COMP_MASK | DTLD_SEND_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_IETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_IETH_BYTES,
		}
	},
	[IB_OPCODE_RC_SEND_ONLY_WITH_INVALIDATE]		= {
		.name	= "IB_OPCODE_RC_SEND_ONLY_INV",
		.mask	= DTLD_IETH_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_COMP_MASK | DTLD_RWR_MASK | DTLD_SEND_MASK |
			  DTLD_END_MASK  | DTLD_START_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_IETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_IETH_BYTES,
		}
	},

	/* UC */
	[IB_OPCODE_UC_SEND_FIRST]			= {
		.name	= "IB_OPCODE_UC_SEND_FIRST",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_REQ_MASK | DTLD_RWR_MASK |
			  DTLD_SEND_MASK | DTLD_START_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_UC_SEND_MIDDLE]		= {
		.name	= "IB_OPCODE_UC_SEND_MIDDLE",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_REQ_MASK | DTLD_SEND_MASK |
			  DTLD_MIDDLE_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_UC_SEND_LAST]			= {
		.name	= "IB_OPCODE_UC_SEND_LAST",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_REQ_MASK | DTLD_COMP_MASK |
			  DTLD_SEND_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_UC_SEND_LAST_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_UC_SEND_LAST_WITH_IMMEDIATE",
		.mask	= DTLD_IMMDT_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_COMP_MASK | DTLD_SEND_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},
	[IB_OPCODE_UC_SEND_ONLY]			= {
		.name	= "IB_OPCODE_UC_SEND_ONLY",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_REQ_MASK | DTLD_COMP_MASK |
			  DTLD_RWR_MASK | DTLD_SEND_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_UC_SEND_ONLY_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_UC_SEND_ONLY_WITH_IMMEDIATE",
		.mask	= DTLD_IMMDT_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_COMP_MASK | DTLD_RWR_MASK | DTLD_SEND_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},
	[IB_OPCODE_UC_RDMA_WRITE_FIRST]		= {
		.name	= "IB_OPCODE_UC_RDMA_WRITE_FIRST",
		.mask	= DTLD_RETH_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_WRITE_MASK | DTLD_START_MASK,
		.length = DTLD_BTH_BYTES + DTLD_RETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RETH_BYTES,
		}
	},
	[IB_OPCODE_UC_RDMA_WRITE_MIDDLE]		= {
		.name	= "IB_OPCODE_UC_RDMA_WRITE_MIDDLE",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_REQ_MASK | DTLD_WRITE_MASK |
			  DTLD_MIDDLE_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_UC_RDMA_WRITE_LAST]			= {
		.name	= "IB_OPCODE_UC_RDMA_WRITE_LAST",
		.mask	= DTLD_PAYLOAD_MASK | DTLD_REQ_MASK | DTLD_WRITE_MASK |
			  DTLD_END_MASK,
		.length = DTLD_BTH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES,
		}
	},
	[IB_OPCODE_UC_RDMA_WRITE_LAST_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_UC_RDMA_WRITE_LAST_WITH_IMMEDIATE",
		.mask	= DTLD_IMMDT_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_WRITE_MASK | DTLD_COMP_MASK | DTLD_RWR_MASK |
			  DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},
	[IB_OPCODE_UC_RDMA_WRITE_ONLY]			= {
		.name	= "IB_OPCODE_UC_RDMA_WRITE_ONLY",
		.mask	= DTLD_RETH_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_WRITE_MASK | DTLD_START_MASK |
			  DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_RETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RETH_BYTES,
		}
	},
	[IB_OPCODE_UC_RDMA_WRITE_ONLY_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_UC_RDMA_WRITE_ONLY_WITH_IMMEDIATE",
		.mask	= DTLD_RETH_MASK | DTLD_IMMDT_MASK | DTLD_PAYLOAD_MASK |
			  DTLD_REQ_MASK | DTLD_WRITE_MASK |
			  DTLD_COMP_MASK | DTLD_RWR_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES + DTLD_RETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RETH]	= DTLD_BTH_BYTES,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES +
					  DTLD_RETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RETH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},

	/* RD */
	[IB_OPCODE_RD_SEND_FIRST]			= {
		.name	= "IB_OPCODE_RD_SEND_FIRST",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_PAYLOAD_MASK |
			  DTLD_REQ_MASK | DTLD_RWR_MASK | DTLD_SEND_MASK |
			  DTLD_START_MASK,
		.length = DTLD_BTH_BYTES + DTLD_DETH_BYTES + DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
		}
	},
	[IB_OPCODE_RD_SEND_MIDDLE]		= {
		.name	= "IB_OPCODE_RD_SEND_MIDDLE",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_PAYLOAD_MASK |
			  DTLD_REQ_MASK | DTLD_SEND_MASK |
			  DTLD_MIDDLE_MASK,
		.length = DTLD_BTH_BYTES + DTLD_DETH_BYTES + DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
		}
	},
	[IB_OPCODE_RD_SEND_LAST]			= {
		.name	= "IB_OPCODE_RD_SEND_LAST",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_PAYLOAD_MASK |
			  DTLD_REQ_MASK | DTLD_COMP_MASK | DTLD_SEND_MASK |
			  DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_DETH_BYTES + DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
		}
	},
	[IB_OPCODE_RD_SEND_LAST_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_RD_SEND_LAST_WITH_IMMEDIATE",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_IMMDT_MASK |
			  DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_COMP_MASK | DTLD_SEND_MASK |
			  DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES + DTLD_DETH_BYTES +
			  DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},
	[IB_OPCODE_RD_SEND_ONLY]			= {
		.name	= "IB_OPCODE_RD_SEND_ONLY",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_PAYLOAD_MASK |
			  DTLD_REQ_MASK | DTLD_COMP_MASK | DTLD_RWR_MASK |
			  DTLD_SEND_MASK | DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_DETH_BYTES + DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
		}
	},
	[IB_OPCODE_RD_SEND_ONLY_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_RD_SEND_ONLY_WITH_IMMEDIATE",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_IMMDT_MASK |
			  DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_COMP_MASK | DTLD_RWR_MASK | DTLD_SEND_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES + DTLD_DETH_BYTES +
			  DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},
	[IB_OPCODE_RD_RDMA_WRITE_FIRST]		= {
		.name	= "IB_OPCODE_RD_RDMA_WRITE_FIRST",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_RETH_MASK |
			  DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_WRITE_MASK | DTLD_START_MASK,
		.length = DTLD_BTH_BYTES + DTLD_RETH_BYTES + DTLD_DETH_BYTES +
			  DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_RETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES +
					  DTLD_RETH_BYTES,
		}
	},
	[IB_OPCODE_RD_RDMA_WRITE_MIDDLE]		= {
		.name	= "IB_OPCODE_RD_RDMA_WRITE_MIDDLE",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_PAYLOAD_MASK |
			  DTLD_REQ_MASK | DTLD_WRITE_MASK |
			  DTLD_MIDDLE_MASK,
		.length = DTLD_BTH_BYTES + DTLD_DETH_BYTES + DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
		}
	},
	[IB_OPCODE_RD_RDMA_WRITE_LAST]			= {
		.name	= "IB_OPCODE_RD_RDMA_WRITE_LAST",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_PAYLOAD_MASK |
			  DTLD_REQ_MASK | DTLD_WRITE_MASK |
			  DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_DETH_BYTES + DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
		}
	},
	[IB_OPCODE_RD_RDMA_WRITE_LAST_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_RD_RDMA_WRITE_LAST_WITH_IMMEDIATE",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_IMMDT_MASK |
			  DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_WRITE_MASK | DTLD_COMP_MASK | DTLD_RWR_MASK |
			  DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES + DTLD_DETH_BYTES +
			  DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},
	[IB_OPCODE_RD_RDMA_WRITE_ONLY]			= {
		.name	= "IB_OPCODE_RD_RDMA_WRITE_ONLY",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_RETH_MASK |
				DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
				DTLD_WRITE_MASK | DTLD_START_MASK |
				DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_RETH_BYTES + DTLD_DETH_BYTES +
			  DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_RETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES +
					  DTLD_RETH_BYTES,
		}
	},
	[IB_OPCODE_RD_RDMA_WRITE_ONLY_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_RD_RDMA_WRITE_ONLY_WITH_IMMEDIATE",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_RETH_MASK |
			  DTLD_IMMDT_MASK | DTLD_PAYLOAD_MASK |
			  DTLD_REQ_MASK | DTLD_WRITE_MASK |
			  DTLD_COMP_MASK | DTLD_RWR_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES + DTLD_RETH_BYTES +
			  DTLD_DETH_BYTES + DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_RETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES +
					  DTLD_RETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES +
					  DTLD_RETH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},
	[IB_OPCODE_RD_RDMA_READ_REQUEST]			= {
		.name	= "IB_OPCODE_RD_RDMA_READ_REQUEST",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_RETH_MASK |
			  DTLD_REQ_MASK | DTLD_READ_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_RETH_BYTES + DTLD_DETH_BYTES +
			  DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_RETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RETH_BYTES +
					  DTLD_DETH_BYTES +
					  DTLD_RDETH_BYTES,
		}
	},
	[IB_OPCODE_RD_RDMA_READ_RESPONSE_FIRST]		= {
		.name	= "IB_OPCODE_RD_RDMA_READ_RESPONSE_FIRST",
		.mask	= DTLD_RDETH_MASK | DTLD_AETH_MASK |
			  DTLD_PAYLOAD_MASK | DTLD_ACK_MASK |
			  DTLD_START_MASK,
		.length = DTLD_BTH_BYTES + DTLD_AETH_BYTES + DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_AETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_AETH_BYTES,
		}
	},
	[IB_OPCODE_RD_RDMA_READ_RESPONSE_MIDDLE]		= {
		.name	= "IB_OPCODE_RD_RDMA_READ_RESPONSE_MIDDLE",
		.mask	= DTLD_RDETH_MASK | DTLD_PAYLOAD_MASK | DTLD_ACK_MASK |
			  DTLD_MIDDLE_MASK,
		.length = DTLD_BTH_BYTES + DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
		}
	},
	[IB_OPCODE_RD_RDMA_READ_RESPONSE_LAST]		= {
		.name	= "IB_OPCODE_RD_RDMA_READ_RESPONSE_LAST",
		.mask	= DTLD_RDETH_MASK | DTLD_AETH_MASK | DTLD_PAYLOAD_MASK |
			  DTLD_ACK_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_AETH_BYTES + DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_AETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_AETH_BYTES,
		}
	},
	[IB_OPCODE_RD_RDMA_READ_RESPONSE_ONLY]		= {
		.name	= "IB_OPCODE_RD_RDMA_READ_RESPONSE_ONLY",
		.mask	= DTLD_RDETH_MASK | DTLD_AETH_MASK | DTLD_PAYLOAD_MASK |
			  DTLD_ACK_MASK | DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_AETH_BYTES + DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_AETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_AETH_BYTES,
		}
	},
	[IB_OPCODE_RD_ACKNOWLEDGE]			= {
		.name	= "IB_OPCODE_RD_ACKNOWLEDGE",
		.mask	= DTLD_RDETH_MASK | DTLD_AETH_MASK | DTLD_ACK_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_AETH_BYTES + DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_AETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
		}
	},
	[IB_OPCODE_RD_ATOMIC_ACKNOWLEDGE]			= {
		.name	= "IB_OPCODE_RD_ATOMIC_ACKNOWLEDGE",
		.mask	= DTLD_RDETH_MASK | DTLD_AETH_MASK | DTLD_ATMACK_MASK |
			  DTLD_ACK_MASK | DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_ATMACK_BYTES + DTLD_AETH_BYTES +
			  DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_AETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_ATMACK]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_AETH_BYTES,
		}
	},
	[IB_OPCODE_RD_COMPARE_SWAP]			= {
		.name	= "RD_COMPARE_SWAP",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_ATMETH_MASK |
			  DTLD_REQ_MASK | DTLD_ATOMIC_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_ATMETH_BYTES + DTLD_DETH_BYTES +
			  DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_ATMETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_ATMETH_BYTES +
					  DTLD_DETH_BYTES +
					  DTLD_RDETH_BYTES,
		}
	},
	[IB_OPCODE_RD_FETCH_ADD]			= {
		.name	= "IB_OPCODE_RD_FETCH_ADD",
		.mask	= DTLD_RDETH_MASK | DTLD_DETH_MASK | DTLD_ATMETH_MASK |
			  DTLD_REQ_MASK | DTLD_ATOMIC_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_ATMETH_BYTES + DTLD_DETH_BYTES +
			  DTLD_RDETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_RDETH]	= DTLD_BTH_BYTES,
			[DTLD_DETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES,
			[DTLD_ATMETH]	= DTLD_BTH_BYTES +
					  DTLD_RDETH_BYTES +
					  DTLD_DETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_ATMETH_BYTES +
					  DTLD_DETH_BYTES +
					  DTLD_RDETH_BYTES,
		}
	},

	/* UD */
	[IB_OPCODE_UD_SEND_ONLY]			= {
		.name	= "IB_OPCODE_UD_SEND_ONLY",
		.mask	= DTLD_DETH_MASK | DTLD_PAYLOAD_MASK | DTLD_REQ_MASK |
			  DTLD_COMP_MASK | DTLD_RWR_MASK | DTLD_SEND_MASK |
			  DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_DETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_DETH]	= DTLD_BTH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_DETH_BYTES,
		}
	},
	[IB_OPCODE_UD_SEND_ONLY_WITH_IMMEDIATE]		= {
		.name	= "IB_OPCODE_UD_SEND_ONLY_WITH_IMMEDIATE",
		.mask	= DTLD_DETH_MASK | DTLD_IMMDT_MASK | DTLD_PAYLOAD_MASK |
			  DTLD_REQ_MASK | DTLD_COMP_MASK | DTLD_RWR_MASK |
			  DTLD_SEND_MASK | DTLD_START_MASK | DTLD_END_MASK,
		.length = DTLD_BTH_BYTES + DTLD_IMMDT_BYTES + DTLD_DETH_BYTES,
		.offset = {
			[DTLD_BTH]	= 0,
			[DTLD_DETH]	= DTLD_BTH_BYTES,
			[DTLD_IMMDT]	= DTLD_BTH_BYTES +
					  DTLD_DETH_BYTES,
			[DTLD_PAYLOAD]	= DTLD_BTH_BYTES +
					  DTLD_DETH_BYTES +
					  DTLD_IMMDT_BYTES,
		}
	},

};