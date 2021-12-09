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
 * @file main.c
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
#include "ble_stack_common.h"
#include "app.h"
#include "prf.h"
#include "ns_delay.h"
#include "app_gpio.h"
#include "platform.h"

#if  (CFG_APP_NS_IUS)
	#include "ns_dfu_boot.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void main_handler(uevt_t* evt)
{
	switch(evt->evt_id) {
		case UEVT_APP_SETUP:
			LOG_RAW("UEVT_APP_SETUP\r\n");
			platform_init();
			app_init();
			prf_init(RWIP_INIT);
			uevt_bc_e(UEVT_APP_START);
			break;
		case UEVT_APP_START:
			LOG_RAW("UEVT_APP_START\r\n");
			break;
	}
}

void module_init(void)
{
	// 模块的初始化应当只进行handler的注册
	// xxx_init();
}

/**
 * @brief  main function
 * @param
 * @return
 * @note   Note
 */
int main(void)
{
	btn_to_start_up();

	NS_BLE_STACK_INIT();
#if (CFG_APP_NS_IUS)
	if(CURRENT_APP_START_ADDRESS == NS_APP1_START_ADDRESS) {
		LOG_RAW("application 1 start new ...\r\n");
	} else if(CURRENT_APP_START_ADDRESS == NS_APP2_START_ADDRESS) {
		LOG_RAW("application 2 start new ...\r\n");
	}
#endif
#if NS_LOG_ENABLED==1
	LOG_INIT();
	LOG_RAW("LOG INIT Succeed\r\n");
#endif
	app_sched_init();
	user_event_init();
	user_event_handler_regist(main_handler);
	module_init();

	uevt_bc_e(UEVT_APP_SETUP);
	while (1) {
		platform_scheduler();
	}
}

/**
 * @brief  user handle before enter sleep mode
 * @param
 * @return
 * @note
 */
void app_sleep_prepare_proc(void)
{

}

/**
 * @brief  user handle after wake up from sleep mode
 * @param
 * @return
 * @note
 */
void app_sleep_resume_proc(void)
{


}

/**
 * @brief  ble connected
 * @param
 * @return
 * @note
 */
void app_ble_connected(void)
{
}

/**
 * @brief  ble disconnected
 * @param
 * @return
 * @note
 */
void app_ble_disconnected(void)
{
}

/**
 * @}
 */

