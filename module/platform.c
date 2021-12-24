
#include "platform.h"
#include <stdint.h>
#include <stdio.h>
#include "ns_log.h"
#include "ns_sleep.h"
#include "scheduler.h"

#include "n32wb03x.h"
#include "global_func.h"

typedef void (*void_handler_t)(void);
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
		LOG_RAW("%s\r\n", buffer);
	}
}

__WEAK void rtc_16hz_isr(uint8_t tick)
{
	LOG_RAW("(WEAK HANDLER)isr16 - %d\r\n", tick);
}
__WEAK void rtc_8hz_isr(uint8_t tick)
{
	LOG_RAW("(WEAK HANDLER)isr8 - %d\r\n", tick);
}
__WEAK void rtc_1hz_handler(void)
{
	static uint32_t sec = 0;
	LOG_RAW("(WEAK HANDLER)tick - %d\r\n", sec);
	sec += 1;
}
static void rtc_handler(void)
{
	static unsigned char tick = 0;
	// rtc_8hz_isr(tick);
	rtc_16hz_isr(tick);
	tick += 1;
	if(tick & 0x10) {
		tick &= 0xF;
		platform_simple_evt_put(rtc_1hz_handler);
	}
}

static void button_setup(void);
static void leds_setup(void);

#include "n32wb03x_rtc.h"
uint32_t SynchPrediv, AsynchPrediv;
#define VRTC_SynchPrediv (0xFF)
#define VRTC_AsynchPrediv (0x7F)
void RTC_EXTI9_WKUP_Configuration(void)
{
    EXTI_InitType EXTI_InitStructure;

    EXTI_ClrITPendBit(EXTI_LINE9);
    EXTI_InitStructure.EXTI_Line = EXTI_LINE9;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

}

static void RTC_IRQHandler(void);
void NVIC_RTC_WKUP_IRQ_Config(void)
{
    NVIC_InitType NVIC_InitStructure;

    /* Enable the RTC WakeUp Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    ModuleIrqRemoval(RTC_IRQn);
    ModuleIrqRegister(RTC_IRQn, RTC_IRQHandler);
}

static void RTC_PrescalerConfig(void)
{
	RTC_InitType RTC_InitStructure;
    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = VRTC_AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv  = VRTC_SynchPrediv;
    RTC_InitStructure.RTC_HourFormat   = RTC_24HOUR_FORMAT;
    if (RTC_Init(&RTC_InitStructure) == ERROR)
    {
        LOG_RAW("\r\n//******* RTC Prescaler Config failed **********//\r\n");
    }
}

void RTC_CLKSourceConfig(void)
{
    /* Enable the PWR clock */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);

    /* Disable RTC clock */
    RCC_EnableRtcClk(DISABLE);

    LOG_RAW("RTC_ClkSrc Set LSE! \r\n");
    RCC_ConfigLse(RCC_LSE_ENABLE);
    while (RCC_GetFlagStatus(RCC_LSCTRL_FLAG_LSERD) == RESET) { }
    
    RCC_ConfigLSXSEL(RCC_RTCCLK_SRC_LSE);
    RCC_EnableLsi(DISABLE);

    /* Enable the RTC Clock */
    RCC_EnableRtcClk(ENABLE);
    RTC_WaitForSynchro();
    LOG_RAW("RTC_ClkSrc Set LSE DONE! \r\n");
}

static void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_INT_WUT) != RESET) {
        RTC_ClrIntPendingBit(RTC_INT_WUT);
        EXTI_ClrITPendBit(EXTI_LINE9);
		rtc_handler();
    }
}

static void rtc_setup(void)
{
	LOG_RAW("RTC SETUP\r\n");
	RTC_CLKSourceConfig();
    RTC_PrescalerConfig();
	RTC_ConfigWakeUpClock(RTC_WKUPCLK_RTCCLK_DIV16);
    RTC_EnableWakeUp(DISABLE);
    RTC_SetWakeUpCounter(0x80);

    RTC_EXTI9_WKUP_Configuration();
	NVIC_RTC_WKUP_IRQ_Config();
    RTC_EnableWakeUp(ENABLE);
    RTC_ConfigInt(RTC_INT_WUT, ENABLE);
	LOG_RAW("RTC SETUP DONE\r\n");
}
void rtc_on_uevt_handler(uevt_t* evt)
{
	switch(evt->evt_id) {
		case UEVT_APP_SETUP:
			rtc_setup();
			break;
	}
}
void rtc_init(void)
{
	user_event_handler_regist(rtc_on_uevt_handler);
}


#define LED1 GPIOB,GPIO_PIN_0
#define LED2 GPIOA,GPIO_PIN_6


#define KEY_NUMBER 2

#define KEY1 GPIOB,GPIO_PIN_1
#define KEY2 GPIOB,GPIO_PIN_2

#define KEY1_INPUT_EXTI_LINE   EXTI_LINE1
#define KEY1_INPUT_IRQn        EXTI0_1_IRQn

#define KEY2_INPUT_EXTI_LINE   EXTI_LINE2
#define KEY2_INPUT_IRQn        EXTI2_3_IRQn

#define KEY1_PORT_SOURCE GPIOB_PORT_SOURCE
#define KEY1_PIN_SOURCE  GPIO_PIN_SOURCE1
#define KEY2_PORT_SOURCE GPIOB_PORT_SOURCE
#define KEY2_PIN_SOURCE  GPIO_PIN_SOURCE2

static void nvic_set(IRQn_Type irq_channel, uint8_t priority, FunctionalState en)
{
	NVIC_InitType NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = irq_channel;
    NVIC_InitStructure.NVIC_IRQChannelPriority           = priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = en;
    NVIC_Init(&NVIC_InitStructure);
}
static void key_exti_set(uint32_t exti_line, FunctionalState en)
{
	EXTI_InitType EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line    = exti_line;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = en;
    EXTI_InitPeripheral(&EXTI_InitStructure);
}

#define key1_int_enable() key_exti_set(KEY1_INPUT_EXTI_LINE, ENABLE)
#define key1_int_disable() key_exti_set(KEY1_INPUT_EXTI_LINE, DISABLE)
#define key2_int_enable() key_exti_set(KEY2_INPUT_EXTI_LINE, ENABLE)
#define key2_int_disable() key_exti_set(KEY2_INPUT_EXTI_LINE, DISABLE)

GPIO_Module* const key_port[KEY_NUMBER] = {
	GPIOB,GPIOB
};
const uint16_t key_pin[KEY_NUMBER] = {
	GPIO_PIN_1,GPIO_PIN_2
};
const uint32_t key_exti_line[KEY_NUMBER] = {
	KEY1_INPUT_EXTI_LINE,
	KEY2_INPUT_EXTI_LINE
};
const uint16_t key_long_event[KEY_NUMBER] = {
	UEVT_BTN1_LONG,
	UEVT_BTN2_LONG
};
const uint16_t key_repeat_event[KEY_NUMBER] = {
	UEVT_BTN1_REPEAT,
	UEVT_BTN2_REPEAT
};
const uint16_t key_release_event[KEY_NUMBER] = {
	UEVT_BTN1_RELEASE,
	UEVT_BTN2_RELEASE
};
// TODO: button程序化初始化 使用button命名替换key命名
bool is_button_release(GPIO_Module* GPIOx, uint16_t Pin)
{
	return GPIO_ReadInputDataBit(GPIOx,Pin) ? true : false;
}

void gpio_setup_out_pp(GPIO_Module* GPIOx, uint16_t Pin)
{
	GPIO_InitType GPIO_InitStructure;
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

	if (GPIOx == GPIOA) {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
	} else if (GPIOx == GPIOB) {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
	} else {
		return;
	}
	if (Pin <= GPIO_PIN_ALL) {
		GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin = Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
	}
}

void gpio_setup_in_pu(GPIO_Module* GPIOx, uint16_t Pin)
{
	GPIO_InitType GPIO_InitStructure;
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

	if (GPIOx == GPIOA) {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_AFIO, ENABLE);
	} else if (GPIOx == GPIOB) {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);
	} else {
		return;
	}
	GPIO_InitStruct(&GPIO_InitStructure);
	GPIO_InitStructure.Pin = Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.GPIO_Pull = GPIO_PULL_UP;
	GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
}
void gpio_setup_int_falling(uint8_t port_src, uint8_t pin_src, uint32_t exti_line, IRQn_Type irq_channel, void_handler_t handler)
{
	GPIO_ConfigEXTILine(port_src, pin_src);

	key_exti_set(exti_line, ENABLE);
	nvic_set(irq_channel, 3, ENABLE);

    ModuleIrqRemoval(irq_channel);
    ModuleIrqRegister(irq_channel, handler);
}

static void key1_press(void){
	if ( EXTI_GetITStatus(KEY1_INPUT_EXTI_LINE)!= RESET) {
		key1_int_disable();
		EXTI_ClrITPendBit(KEY1_INPUT_EXTI_LINE);
		uevt_bc_e(UEVT_BTN1_DOWN);
	}
}
static void key2_press(void){
	if ( EXTI_GetITStatus(KEY2_INPUT_EXTI_LINE)!= RESET) {
		key2_int_disable();
		EXTI_ClrITPendBit(KEY2_INPUT_EXTI_LINE);
		uevt_bc_e(UEVT_BTN2_DOWN);
	}
}

static uint8_t button_current_valid = 0x00;
static uint8_t button_press_time[KEY_NUMBER] = {0};
void btn_on_uevt_handler(uevt_t* evt)
{
	switch(evt->evt_id) {
		case UEVT_APP_SETUP:
			button_setup();
			break;
		case UEVT_BTN1_DOWN:
			button_current_valid |= 0x01;
			break;
		case UEVT_BTN2_DOWN:
			button_current_valid |= 0x02;
			break;
		case UEVT_RTC_8HZ:
			if(button_current_valid > 0) {
				for (uint8_t i = 0; i < KEY_NUMBER; i++)
				{
					if(button_current_valid&(0x1<<i)){
						button_press_time[i] += 1;
						if(button_press_time[i] == 9){
							uevt_bc_e(key_long_event[i]);
						}
						if(button_press_time[i] > 16){
							button_press_time[i] = 16;
							uevt_bc_e(key_repeat_event[i]);
						}
						if(is_button_release(key_port[i],key_pin[i])){
							button_current_valid &= (0xFF^(0x01<<i));
							button_press_time[i] = 0;
							key_exti_set(key_exti_line[i], ENABLE);
							uevt_bc_e(key_release_event[i]);
						}
					}
				}
			}
			break;
	}
}

void button_init(void)
{
	user_event_handler_regist(btn_on_uevt_handler);
}

static void button_setup(void)
{
	LOG_RAW("BTN SETUP\r\n");
	gpio_setup_in_pu(KEY1);
	gpio_setup_int_falling(KEY1_PORT_SOURCE,KEY1_PIN_SOURCE,KEY1_INPUT_EXTI_LINE, KEY1_INPUT_IRQn, key1_press);
	gpio_setup_in_pu(KEY2);
	gpio_setup_int_falling(KEY2_PORT_SOURCE,KEY2_PIN_SOURCE,KEY2_INPUT_EXTI_LINE, KEY2_INPUT_IRQn, key2_press);
}

void led1_on(void) { GPIO_SetBits(LED1); }
void led1_off(void) { GPIO_ResetBits(LED1); }
void led1_toggle(void) { GPIO_TogglePin(LED1); }
void led2_on(void) { GPIO_SetBits(LED2); }
void led2_off(void) { GPIO_ResetBits(LED2); }
void led2_toggle(void) { GPIO_TogglePin(LED2); }
static void leds_setup(void)
{
	gpio_setup_out_pp(LED1);
	gpio_setup_out_pp(LED2);
}
void led_on_uevt_handler(uevt_t* evt)
{
	switch(evt->evt_id) {
		case UEVT_APP_SETUP:
			leds_setup();
			break;
		case UEVT_LED1_ON:
			led1_on();
			break;
		case UEVT_LED1_OFF:
			led1_off();
			break;
		case UEVT_LED2_ON:
			led2_on();
			break;
		case UEVT_LED2_OFF:
			led2_off();
			break;
	}
}

void led_init(void)
{
	user_event_handler_regist(led_on_uevt_handler);
}

void platform_wakeup(void)
{
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);
}
void io_config(void)
{
}

void btn_to_start_up(void)
{
	gpio_setup_in_pu(KEY1);
	gpio_setup_in_pu(KEY2);
	gpio_setup_out_pp(LED1);
	gpio_setup_out_pp(LED2);
	
	while(1) {
		if((GPIO_ReadInputDataBit(KEY1) == 0) || (GPIO_ReadInputDataBit(KEY2) == 0)) {
			break;
		}
		delay_n_ms(131);
		led1_toggle();
		if((GPIO_ReadInputDataBit(KEY1) == 0) || (GPIO_ReadInputDataBit(KEY2) == 0)) {
			break;
		}
		delay_n_ms(131);
		led2_toggle();
	}
	led1_off();
	led2_off();
}

void platform_init(void)
{
}

void platform_reboot(void)
{
}

bool is_going_to_shutdown = false;
void platform_powerdown(bool flag)
{
	is_going_to_shutdown = flag;
}

void shutdown_routine(void)
{
	// nrf_pwr_mgmt_shutdown(NRF_PWR_MGMT_SHUTDOWN_GOTO_SYSOFF);
	while(1);
}

void platform_scheduler(void)
{
	app_sched_execute();

	// NOTE:BLE SWITCH
	rwip_schedule();

	if(is_going_to_shutdown) {
		shutdown_routine();
	}
	// ns_sleep();
}
