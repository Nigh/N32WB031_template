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
 * @file app_bond_handlers.c
 * @author Nations Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */

/** @addtogroup
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "n32wb03x.h"
#include "app_bond_handlers.h"
#include "app_sec_handlers.h"
#include "string.h"
#include "n32wb03x_qflash.h"
#include "ns_sec.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct app_sec_bond_data_env_tag app_sec_bond_data;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

extern struct app_sec_env_tag app_sec_env;

/**
 * @brief  default_app_bdb_init
 */
void default_app_bdb_init(void)
{
	uint8_t peer_num = 0;

	Qflash_Init();

	default_app_bdb_get_size(&peer_num);

	if(peer_num != 0) {
		app_sec_env.bonded = true;
	} else {
		app_sec_env.bonded = false;
	}
}

/**
 * @brief  default_app_bdb_get_size
 * @param
 * @return
 * @note
 */
void default_app_bdb_get_size(uint8_t* size)
{
	uint8_t bond_num = 0;
	uint16_t bond_space_valid;

	bond_space_valid = *((uint32_t*)(BOND_DATA_BASE_ADDR));
	bond_num = *((uint32_t*)(BOND_DATA_BASE_ADDR + 2));

	if(bond_space_valid == BOND_SPACE_VALID_FLAG) {
		if(bond_num > MAX_BOND_PEER) {
			bond_num = 0;

			// Erase full sector
			Qflash_Erase_Sector_Raw(BOND_DATA_BASE_ADDR);
		}
	} else {
		bond_num = 0;
	}

	*size = bond_num;
}

/**
 * @brief  default_app_bdb_add_entry
 * @param
 * @return
 * @note
 */
void default_app_bdb_add_entry(struct app_sec_bond_data_env_tag* bond_data_param)
{
	// Load bond data from flash
	struct local_device_bond_data_tag local_device_bond_data;

	default_app_bdb_get_size(&local_device_bond_data.num);

	// Update bond data
	if(local_device_bond_data.num < MAX_BOND_PEER) {
		memcpy(&local_device_bond_data.single_peer_bond_data, (uint32_t*)(4 + BOND_DATA_BASE_ADDR),
		       sizeof(struct app_sec_bond_data_env_tag) * local_device_bond_data.num );

		local_device_bond_data.num ++;
	} else {
		memcpy(&local_device_bond_data.single_peer_bond_data, (uint32_t*)(4 + BOND_DATA_BASE_ADDR + sizeof(struct app_sec_bond_data_env_tag)),
		       sizeof(struct app_sec_bond_data_env_tag) * local_device_bond_data.num );

	}

	memcpy(&local_device_bond_data.single_peer_bond_data[local_device_bond_data.num - 1], bond_data_param,
	       sizeof(struct app_sec_bond_data_env_tag) );

	local_device_bond_data.valid_flag = BOND_SPACE_VALID_FLAG;

	// Erase full sector
	Qflash_Erase_Sector_Raw(BOND_DATA_BASE_ADDR);

	// Re-write data
	Qflash_Write_Raw(BOND_DATA_BASE_ADDR, (uint8_t*)(&local_device_bond_data), sizeof(struct local_device_bond_data_tag) );

}

/**
 * @brief  default_app_bdb_load
 * @param
 * @return
 * @note
 */
void default_app_bdb_load(uint8_t index, struct app_sec_bond_data_env_tag* single_bond_data)
{
	uint32_t length = sizeof(struct app_sec_bond_data_env_tag);

	memcpy(single_bond_data, (uint8_t*)(4 + BOND_DATA_BASE_ADDR + index * length ), length);
}

/**
 * @}
 */

