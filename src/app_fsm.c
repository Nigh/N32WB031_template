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
 * @file app_fsm.c
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
#include "app_fsm.h"
#include "ns_log.h"
#include "app_callback.h"
#include "app_user_callback_config.h"
#include "app_bond_handlers.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t non_ble_async_tasks;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

extern struct app_sec_bond_data_env_tag app_sec_bond_data;

/**
 * @brief  app_fsm_set_async_task
 * @param
 * @return
 * @note
 */
void app_fsm_set_async_task(uint8_t taskbm)
{
	non_ble_async_tasks |= taskbm;
}

/**
 * @brief  app_fsm_clr_async_task
 * @param
 * @return
 * @note
 */
void app_fsm_clr_async_task(uint8_t taskbm)
{
	non_ble_async_tasks &= ~taskbm;
}

/**
 * @brief  app_fsm_on_process
 * @param
 * @return
 * @note
 */
void app_fsm_on_process(void)
{
	if(non_ble_async_tasks & ASYNC_TASK_WRITE_BONDING_DATA) {
		app_fsm_clr_async_task(ASYNC_TASK_WRITE_BONDING_DATA);

		NS_LOG_INFO("Writing bonding data\r\n");

#if (BLE_APP_SEC)
		CALLBACK_ARGS_1(user_bond_callbacks.app_bdb_add_entry, &app_sec_bond_data);
#endif
	}
}

/**
 * @}
 */
