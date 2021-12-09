
#include "platform.h"
#include <stdint.h>
#include <stdio.h>
#include "ns_log.h"


void LOG_HEX_RAW_IMP(uint8_t* array, uint16_t length)
{
	static char buffer[193];
	uint8_t* pa = array;
	char* pb;
	uint8_t len;	// 当次处理长度
	while(length > 0) {
		pb = buffer;
		if(length > 64) {
			len = 64;
		} else {
			len = length;
		}
		for (uint8_t i = 0; i < len; i++) {
			sprintf(pb, "%02X ", *pa);
			pb += 3;
			pa += 1;
		}
		length -= len;
		*pb = 0;
		LOG_RAW("%s\n", buffer);
	}
}

__WEAK void rtc_8hz_isr(uint8_t tick)
{
	LOG_RAW("(WEAK HANDLER)isr - %d\r\n", tick);
}
__WEAK void rtc_1hz_handler(void)
{
	static uint32_t sec = 0;
	LOG_RAW("(WEAK HANDLER)tick - %d\r\n", sec);
	sec += 1;
}

// TODO: int type
static void rtc_handler(int int_type)
{
	static unsigned char tick = 0;
	if (int_type == 1) {
	} else if (int_type == 2) {
		rtc_8hz_isr(tick);
		tick += 1;
		if(tick & 0x8) {
			tick &= 0x7;
			platform_simple_evt_put(rtc_1hz_handler);
		}
	}
}

static void rtc_config(void)
{
	
}

uint8_t button_chatter = 0;
uint8_t button_press_time = 0;
void btn_on_uevt_handler(uevt_t* evt)
{
	switch(evt->evt_id) {
		case UEVT_RTC_8HZ:
			if(button_chatter > 0) {
				button_press_time += 1;
				if(button_press_time == 9) {
					uevt_bc_e(UEVT_BTN_LONG);
				}
				if(button_press_time > 0x10) {
					button_press_time = 0x10;
					uevt_bc_e(UEVT_BTN_REPEAT);
				}
			}
			break;
	}
}
void button_handler(uint8_t* data, uint16_t length)
{
	// 使用单按钮逻辑，同时只允许单个按钮按下
	static uint8_t current_down = 0xFF;
	switch(data[1]) {
		case 0x1:
			button_chatter = 2;
			button_press_time = 0;
			uevt_bc_e(UEVT_BTN_DOWN);
			break;
		case 0x2:
			button_chatter = 0;
			button_press_time = 0;
			uevt_bc_e(UEVT_BTN_RELEASE);
			break;
	}
}

void button_handler_ISR(uint8_t pin_no, uint8_t button_action)
{
	static uint8_t data[2];
	data[0] = pin_no;
	data[1] = button_action;
	platform_evt_put(data, 2, button_handler);
}

bool is_button_release(uint32_t btn)
{
	// return (nrf_gpio_pin_read(btn) == (uint32_t)BUTTON_POL - 1) ? true : false;
}

void button_config(void)
{
	uint32_t err;

	user_event_handler_regist(btn_on_uevt_handler);
}

void leds_config(void)
{

}

void io_config(void)
{
	button_config();
	leds_config();
}

static void log_init(void)
{
	LOG_INIT();
	LOG_RAW("LOG INIT Succeed\r\n");
}

void platform_init(void)
{
#if NS_LOG_ENABLED==1
	log_init();
#endif
	LOG_RAW("LOG INIT\r\n");
	user_event_init();
	rtc_config();
	io_config();
}

void platform_reboot(void)
{
	// TODO:
}

bool is_going_to_shutdown = false;
void platform_powerdown(bool flag)
{
	is_going_to_shutdown = flag;
}

void shutdown_routine(void)
{
	// TODO:
	// nrf_pwr_mgmt_shutdown(NRF_PWR_MGMT_SHUTDOWN_GOTO_SYSOFF);
	while(1);
}

void platform_scheduler(void)
{
	rwip_schedule();
	if(is_going_to_shutdown) {
		shutdown_routine();
	}
	ns_sleep();
}
