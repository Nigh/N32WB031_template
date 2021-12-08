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
 * @file app_bond_handlers.h
 * @author Nations Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */

/** @addtogroup
 * @{
 */
#ifndef __APP_BOND_HANDLERS_H__
#define __APP_BOND_HANDLERS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "gapc_task.h"
#include "co_bt_defines.h"
#include "app_user_config.h"
/* Public typedef -----------------------------------------------------------*/
/* Public define ------------------------------------------------------------*/
#define BOND_DATA_BASE_ADDR             0x01020000

#define BOND_SPACE_VALID_FLAG           0x1234

/* bonding structure
 * --------------------------------------------------------------------------------
 * |         ADDR            |                 FIELD                   |   SIZE   |
 * --------------------------|-----------------------------------------|----------|
 * | BOND_DATA_BASE_ADDR+0   | Bond space valid flag                   |    2     |
 * --------------------------|-----------------------------------------|----------|
 * | BOND_DATA_BASE_ADDR+2   | Bonded peer device numbers              |    1     |
 * --------------------------------------------------------------------------------
 * | BOND_DATA_BASE_ADDR+4   | Bond data0(@ app_sec_bond_data_env_tag) |          |
 * --------------------------------------------------------------------------------
 * |                         | Bond data1(@ app_sec_bond_data_env_tag) |          |
 * --------------------------------------------------------------------------------
 * |                         |               ......                    |          |
 * --------------------------------------------------------------------------------
 */

//BOND_DATA_BASE_ADDR


/// Application Security Bond Data environment structure
struct app_sec_bond_data_env_tag {
	// LTK
	struct gap_sec_key ltk;

	// Random Number
	struct rand_nb rand_nb;

	// EDIV
	uint16_t ediv;

	// Remote IRK
	struct gapc_irk irk;

	// LTK key size
	uint8_t key_size;

	// Last paired peer address type
	uint8_t peer_addr_type;

	// Last paired peer address
	struct bd_addr peer_addr;

	// authentication level
	uint8_t auth;
};

struct local_device_bond_data_tag {
	uint16_t valid_flag;
	uint8_t num;
	struct app_sec_bond_data_env_tag single_peer_bond_data[MAX_BOND_PEER];
};

/* Public constants ---------------------------------------------------------*/
/* Public function prototypes -----------------------------------------------*/

void default_app_bdb_init(void);
void default_app_bdb_get_size(uint8_t* size);
void default_app_bdb_add_entry(struct app_sec_bond_data_env_tag* bond_data_param);
void default_app_bdb_load(uint8_t index, struct app_sec_bond_data_env_tag* single_bond_data);

#ifdef __cplusplus
}
#endif

#endif /* __APP_BOND_HANDLERS_H__ */
/**
 * @}
 */
