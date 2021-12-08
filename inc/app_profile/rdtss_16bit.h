/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file rdtss_16bit.h
 * @author Nations Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */

#ifndef _RDTSS_16BIT_H_
#define _RDTSS_16BIT_H_

#include "rwip_config.h"              // SW configuration

#if (BLE_RDTSS_16BIT_SERVER)

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "prf_types.h"
#include "prf.h"
#include "attm.h"

#include "rdts_common.h"

#define RDTSS_16BIT_IDX_MAX        (1)

/* Public typedef -----------------------------------------------------------*/

/// Value element
struct rdtss_16bit_val_elmt {
	/// list element header
	struct co_list_hdr hdr;
	/// value identifier
	uint8_t att_idx;
	/// value length
	uint8_t length;
	/// value data
	uint8_t data[__ARRAY_EMPTY];
};

/// rdtss environment variable
struct rdtss_16bit_env_tag {
	/// profile environment
	prf_env_t prf_env;
	/// Service Start Handle
	uint16_t shdl;
	/// To store the DB max number of attributes
	uint8_t max_nb_att;
	/// On-going operation
	struct ke_msg* operation;

	/// Cursor on connection used to notify peer devices
	uint8_t cursor;
	/// CCC handle index, used during notification/indication busy state
	uint8_t ccc_idx;

	/// List of values set by application
	struct co_list values;
	/// rdtss_16bit task state
	ke_state_t state[RDTSS_16BIT_IDX_MAX];
};


/* Private functions ---------------------------------------------------------*/

/**
 * @brief Disable actions grouped in getting back to IDLE and sending configuration to requester task
 * @param[in] conhdl    Connection Handle
 */
void rdtss_16bit_disable(uint16_t conhdl);

const struct prf_task_cbs* rdtss_16bit_prf_itf_get(void);

/**
 * Initialize task handler
 *
 * @param p_task_desc Task descriptor to fill
 */
void rdtss_16bit_task_init(struct ke_task_desc* p_task_desc);

#endif // (BLE_RDTSS_16BIT_SERVER)

#endif // _RDTSS_16BIT_H_
