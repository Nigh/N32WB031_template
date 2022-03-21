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
 * @file app_custom_srv.h
 * @author Nations Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */


#ifndef APP_RDTSS_16BIT_H_
#define APP_RDTSS_16BIT_H_

/**
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Battery Application Module entry point
 *
 * @{
 **/

/* Includes ------------------------------------------------------------------*/

#include "rwip_config.h"     // SW configuration

#include "platform.h"


/// Manufacturer Name Value
#define APP_CUSTOM_SERVICE_MANUFACTURER_NAME       ("CUST1_16")
#define APP_CUSTOM_SERVICE_MANUFACTURER_NAME_LEN   (8)



#define ATT_SERVICE_AM_SPEED_16          0xCC00           /*!< Service UUID */
#define ATT_CHAR_AM_SPEED_WRITE_16       0xCC01           /*!< Characteristic value UUID */
#define ATT_CHAR_AM_SPEED_NTF_16         0xCC02           /*!< Characteristic value UUID */

/// app custom uuid Service Attributes Indexes
enum {
	APP_CUSTOM_IDX_SVC,

	APP_CUSTOM_IDX_WRITE_CHAR,
	APP_CUSTOM_IDX_WRITE_VAL,
	APP_CUSTOM_IDX_WRITE_CFG,

	APP_CUSTOM_IDX_NTF_CHAR,
	APP_CUSTOM_IDX_NTF_VAL,
	APP_CUSTOM_IDX_NTF_CFG,

	APP_CUSTOM_IDX_NB,
};

/**
 * @brief Initialize Device Information Service Application
 **/
void app_custom_srv_init(void);

/**
 * @brief Add a Device Information Service instance in the DB
 **/
void app_custom_srv_add(void);

void custom_srv_send_notify(const uint8_t* data, uint16_t length);

typedef struct app_custom_srv_raw_data {
	uint16_t length;
	const uint8_t* data;
} s_raw_data;

#define UEVT_BLE_AIR_BASE (0x1100)
#define UEVT_BLE_AIR_RAW_RECV (UEVT_BLE_AIR_BASE | 0x10)

#define UEVT_BLE_AIR_CTL_RAW_SEND (UEVT_BLE_AIR_BASE | 0x90)

void ble_air_on_uevt_handler(uevt_t* evt);
void ble_air_init(void);

/// @} APP

#endif // APP_RDTSS_16BIT_H_
