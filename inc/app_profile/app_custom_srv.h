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
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */


#ifndef APP_CUSTOM_SRV_H_
#define APP_CUSTOM_SRV_H_

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
#include "custom_srv.h"

/// Manufacturer Name Value
#define APP_RDTSS_16BIT_MANUFACTURER_NAME       ("CUST1_16")
#define APP_RDTSS_16BIT_MANUFACTURER_NAME_LEN   (8)



#define ATT_SERVICE_AM_SPEED_16          0xfffa           /*!< Service UUID */
#define ATT_CHAR_AM_SPEED_WRITE_16       0xfffb           /*!< Characteristic value UUID */
#define ATT_CHAR_AM_SPEED_NTF_16         0xfffc           /*!< Characteristic value UUID */

/// rdtss 16bit uuid Service Attributes Indexes
enum {
	CUSTOM_SRV_IDX_SVC,

	CUSTOM_SRV_IDX_WRITE_CHAR,
	CUSTOM_SRV_IDX_WRITE_VAL,
	CUSTOM_SRV_IDX_WRITE_CFG,

	CUSTOM_SRV_IDX_NTF_CHAR,
	CUSTOM_SRV_IDX_NTF_VAL,
	CUSTOM_SRV_IDX_NTF_CFG,

	CUSTOM_SRV_IDX_NB,
};

extern struct attm_desc custom_srv_att_db[CUSTOM_SRV_IDX_NB];

/// Table of message handlers
extern const struct app_subtask_handlers app_custom_srv_handlers;

/**
 * @brief Initialize Device Information Service Application
 **/
void app_custom_srv_init(void);

/**
 * @brief Add a Device Information Service instance in the DB
 **/
void app_custom_srv_add_profile(void);

void custom_srv_send_notify(uint8_t* data, uint16_t length);


/// @} APP

#endif // APP_CUSTOM_SRV_H_
