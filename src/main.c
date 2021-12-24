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
	static uint32_t sec = 0;
	switch(evt->evt_id) {
		case UEVT_RTC_1HZ:
			LOG_RAW("\r\n[%08d]: ", sec++);
			break;
		case UEVT_APP_SETUP:
			LOG_RAW("UEVT_APP_SETUP\r\n");
			// NOTE:BLE SWITCH
			app_init();
			prf_init(RWIP_INIT);

			uevt_bc_e(UEVT_APP_START);
			break;
		case UEVT_APP_START:
			LOG_RAW("UEVT_APP_START\r\n");
			break;
	}
}

#include "app_custom_srv.h"
#define case_evt_log(XXX) case XXX:LOG_RAW("\r\nEVT:[" #XXX "]\r\n");break
static uint8_t raw_buffer[256];
static uint8_t raw_buffer_ptr = 0;
void raw_send_notify(const uint8_t* data, uint8_t len)
{
	static s_raw_data raw;
	static const uevt_t event = {
		UEVT_BLE_AIR_CTL_RAW_SEND, &raw
	};
	uint8_t wp;
	if(raw_buffer_ptr + len < raw_buffer_ptr) {
		wp = 0;
		raw_buffer_ptr = len;
	} else {
		wp = raw_buffer_ptr;
		raw_buffer_ptr += len;
	}
	memcpy(&raw_buffer[wp], data, len);
	raw.length = len;
	raw.data = &raw_buffer[wp];
	user_event_send(event, ble_air_on_uevt_handler);
	return;
}

static void led1_blink_8hz_routine(uint8_t en)
{
	// en: 0=stop 1=start 2=run
	static bool enable = false;
	static uint8_t t = 0;
	if(en == 0) {
		uevt_bc_e(UEVT_LED1_OFF);
		enable = false;
	}
	if(en == 1) {
		uevt_bc_e(UEVT_LED1_ON);
		enable = true;
		t = 0;
	}
	if(!enable) {
		return;
	}
	if((t & 0x7) == 0x0) {
		uevt_bc_e(UEVT_LED1_ON);
	} else if((t & 0x7) == 0x4) {
		uevt_bc_e(UEVT_LED1_OFF);
	}
	t += 1;
}
void test_handler(uevt_t* evt)
{
	static uint8_t io_status_old = 0;
	static uint8_t io_status = 0;
	switch(evt->evt_id) {
		// case_evt_log(UEVT_BTN1_DOWN);
		// case_evt_log(UEVT_BTN1_RELEASE);
		// case_evt_log(UEVT_BTN1_LONG);
		// case_evt_log(UEVT_BTN1_REPEAT);
		// case_evt_log(UEVT_BTN2_DOWN);
		// case_evt_log(UEVT_BTN2_RELEASE);
		// case_evt_log(UEVT_BTN2_LONG);
		// case_evt_log(UEVT_BTN2_REPEAT);
		case UEVT_LED1_ON:
			io_status |= 0x04;
			break;
		case UEVT_LED1_OFF:
			io_status &= 0xFF ^ 0x04;
			break;
		case UEVT_LED2_ON:
			io_status |= 0x08;
			break;
		case UEVT_LED2_OFF:
			io_status &= 0xFF ^ 0x08;
			break;
		case UEVT_BTN1_DOWN:
			io_status |= 0x01;
			break;
		case UEVT_BTN1_RELEASE:
			io_status &= 0xFF ^ 0x01;
			break;
		case UEVT_BTN2_DOWN:
			io_status |= 0x02;
			break;
		case UEVT_BTN2_RELEASE:
			io_status &= 0xFF ^ 0x02;
			break;
		case UEVT_RTC_8HZ:
			led1_blink_8hz_routine(2);
			break;
		case UEVT_RTC_16HZ: {
			static uint8_t cmd[] = {0xB1, 0xBF, 0x00, 0x00};
			if(io_status != io_status_old) {
				cmd[3] = io_status;
				raw_send_notify(cmd, 4);
				io_status_old = io_status;
			}
		}
		break;
		case UEVT_BLE_AIR_RAW_RECV: {
			s_raw_data* raw = (s_raw_data*)evt->content;
			LOG_RAW("Recv RAW: ");
			LOG_HEX_RAW((uint8_t*)raw->data, raw->length);
			if(raw->length == 4) {
				if(raw->data[0] == 0xB1 &&
				        raw->data[1] == 0xBF) {
					switch (raw->data[2]) {
						case 0x20:
							if(raw->data[3] > 0) {
								uevt_bc_e(UEVT_LED2_ON);
							} else {
								uevt_bc_e(UEVT_LED2_OFF);
							}
							break;
						case 0x11:
							if(raw->data[3] > 0) {
								led1_blink_8hz_routine(1);
							} else {
								led1_blink_8hz_routine(0);
							}
							break;
					}
				}
			}
		}
		break;
	}
}
void rtc_1hz_handler(void)
{
	uevt_bc_e(UEVT_RTC_1HZ);
}
void rtc_8hz_isr(void)
{
	uevt_bc_e(UEVT_RTC_8HZ);
}
void rtc_16hz_isr(void)
{
	static bool v = true;
	uevt_bc_e(UEVT_RTC_16HZ);
	if(v = !v) {
		uevt_bc_e(UEVT_RTC_8HZ);
	}
}
void module_init(void)
{
	user_event_handler_regist(main_handler);
	user_event_handler_regist(test_handler);
	button_init();
	led_init();
	rtc_init();
	ble_air_init();
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

	// NOTE:BLE SWITCH
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
	LOG_RAW("\r\n\n\n\n\nLOG INIT Succeed\r\n");
	LOG_RAW("============================\r\n");
#endif
	platform_init();
	app_sched_init();
	user_event_init();
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
	// LOG_RAW("WAKE\r\n");
	// TODO: reinit GPIO
	platform_wakeup();
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

