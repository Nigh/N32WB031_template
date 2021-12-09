
#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#include "compiler_abstraction.h"
#include <stdint.h>
#include <string.h>
#include "scheduler.h"
#include "uevent.h"
#include "rwip_config.h"
#include "rwip.h"
#include "arch.h"


#define platform_simple_evt_put(handler) app_sched_event_put(NULL,(app_sched_event_handler_t)handler)
#define platform_evt_put(p_evt,handler) app_sched_event_put(p_evt,(app_sched_event_handler_t)handler)

#define LOG_INIT(x) NS_LOG_INIT()

#if NS_LOG_ENABLED==1
	#define LOG_HEX_RAW LOG_HEX_RAW_IMP
	#define LOG_RAW NS_LOG_INFO
#else
	#define LOG_HEX_RAW(x...)
	#define LOG_RAW(x...)
#endif

void LOG_HEX_RAW_IMP(uint8_t* array, uint16_t length);

#include "uevent.h"

void platform_init(void);
void platform_scheduler(void);

void btn_on_uevt_handler(uevt_t* evt);

#define UEVT_APP_BASE (0xAE00)
// 上电信号
#define UEVT_APP_POWERUP (UEVT_APP_BASE|0x01)
// 线性执行段
#define UEVT_APP_SETUP (UEVT_APP_BASE|0x02)
// 多任务开始
#define UEVT_APP_START (UEVT_APP_BASE|0x03)
// 准备休眠信号
#define UEVT_APP_BEFORE_SLEEP (UEVT_APP_BASE|0x0E)

#define UEVT_RTC_BASE (0x0000)
#define UEVT_RTC_8HZ (0x0001)
#define UEVT_RTC_1HZ (0x0002)
#define UEVT_RTC_NEWDAY (0x0003)

#define UEVT_BTN_BASE (0x0100)
#define UEVT_BTN_DOWN (0x0101)
#define UEVT_BTN_RELEASE (0x0102)
#define UEVT_BTN_LONG (0x0103)
#define UEVT_BTN_REPEAT (0x0104)

#define UEVT_DTIME_BASE (0x0500)
#define UEVT_DTIME_UPDATE (UEVT_DTIME_BASE|0x01)

#endif
