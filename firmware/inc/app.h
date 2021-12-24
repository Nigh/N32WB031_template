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
 * @file app.h
 * @author Nations Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */


#ifndef APP_H_
#define APP_H_

/**
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Application entry point.
 *
 * @{
 **/

/* Includes ------------------------------------------------------------------*/

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_PRESENT)

#include <stdint.h>          // Standard Integer Definition
#include "arch.h"            // Platform Definitions
#include <co_bt.h>           // Common BT Definitions


#include "gapc_task.h"       // GAPC Definitions
#include "gattc_task.h"       // GATTC Definitions


/* Define ------------------------------------------------------------*/



#define APP_ADV_DURATION_MAX           (655)

#define SECS_UNIT_1_25_MS              (800)
#define SECS_UNIT_10MS                 (100)
#define SECS_TO_UNIT(sec,uint)         ((sec)*(uint))

#define MSECS_UNIT_1_25_MS              (1250)
#define MSECS_UNIT_10_MS                (10000)
#define MSECS_TO_UNIT(msec,uint)        ((msec*1000)/(uint))

/// Maximal length of the Device Name value
#define APP_DEVICE_NAME_MAX_LEN      (18)
#define APP_MESH_DEMO_TYPE_LEN        (1)

/*
 * MACROS
 **/

#define APP_HANDLERS(subtask)    {&subtask##_msg_handler_list[0], ARRAY_LEN(subtask##_msg_handler_list)}

/* Typedef -----------------------------------------------------------*/


/// Advertising state machine
enum app_adv_state {
	/// Advertising activity does not exists
	APP_ADV_STATE_IDLE = 0,
#if BLE_APP_PRF
	/// Creating advertising activity
	APP_ADV_STATE_CREATING,
	/// Setting advertising data
	APP_ADV_STATE_SETTING_ADV_DATA,
	/// Setting scan response data
	APP_ADV_STATE_SETTING_SCAN_RSP_DATA,

	/// Advertising activity created
	APP_ADV_STATE_CREATED,
	/// Starting advertising activity
	APP_ADV_STATE_STARTING,
	/// Advertising activity started
	APP_ADV_STATE_STARTED,
	/// Stopping advertising activity
	APP_ADV_STATE_STOPPING,
#endif //(BLE_APP_PRF)
};

enum app_adv_mode {
	APP_ADV_MODE_IDLE = 0,
	APP_ADV_MODE_DIRECTED_HIGH_DUTY,
	APP_ADV_MODE_DIRECTED,
	APP_ADV_MODE_FAST,
	APP_ADV_MODE_SLOW,

};

/* Public define ------------------------------------------------------------*/

/// Structure containing information about the handlers for an application subtask
struct app_subtask_handlers {
	/// Pointer to the message handler table
	const struct ke_msg_handler* p_msg_handler_tab;
	/// Number of messages handled
	uint16_t msg_cnt;
};

/// Application environment structure
struct app_env_tag {
	/// Connection handle
	uint16_t conhdl;
	/// Connection Index
	uint8_t  conidx;

	/// Advertising activity index
	uint8_t adv_actv_idx;
	/// Current advertising state (@see enum app_adv_state)
	uint8_t adv_state;
	/// Next expected operation completed event
	uint8_t adv_op;

	/// Last initialized profile
	uint8_t next_svc;

	/// Bonding status
	bool bonded;

	/// Last paired peer address type
	uint8_t peer_addr_type;

	/// Last paired peer address
	struct bd_addr peer_addr;

	/// Device Name length
	uint8_t dev_name_len;
	/// Device Name
	uint8_t dev_name[APP_DEVICE_NAME_MAX_LEN];

	/// Local device IRK
	uint8_t loc_irk[KEY_LEN];

	/// Secure Connections on current link
	bool sec_con_enabled;

	struct gapc_ltk ltk;

	/// Counter used to generate IRK
	uint8_t rand_cnt;

	/// Advertising mode
	uint8_t adv_mode;
	/// Maximum device MTU size
	uint16_t max_mtu;

	/// Demonstration type length
	uint8_t demo_type_len;
	/// Demonstration type
	uint8_t demo_type;
#if (BLE_APP_NS_IUS)
	uint8_t manual_conn_param_update;
	uint8_t manual_mtu_update;
#endif
};

/* Public variables ---------------------------------------------------------*/

/// Application environment
extern struct app_env_tag app_env;

/* Public function prototypes -----------------------------------------------*/

/**
 * @brief Initialize the BLE demo application.
 **/
void app_init(void);

/**
 * @brief Add a required service in the database
 **/
bool app_add_svc(void);

/**
 * @brief Retrieve device name
 *
 * @param[out] Pointer at which device name will be returned
 *
 * @return Name length
 **/
uint8_t app_get_dev_name(uint8_t* p_name);

extern uint16_t ble_att_mtu;

#if (BLE_APP_PRF)
	/**
	* @brief
	**/
	void app_adv_fsm_next(void);

	/**
	* @brief Send to request to update the connection parameters
	**/
	void app_update_param(struct gapc_conn_param* conn_param);

	/**
	* @brief Send a disconnection request
	**/
	void app_disconnect(void);

	/**
	* @brief Start/stop advertising
	*
	* @param[in] start     True if advertising has to be started, else false
	**/

	void app_update_adv_state(bool start);

	/**
	* @brief delete advertising
	*
	* @param[in] none
	**/

	void app_delete_advertising(void);
	/**
	* @brief Return if the device is currently bonded
	**/
	bool app_sec_get_bond_status(void);

	/**
	* @brief set mtu
	*
	* @param[in] none
	**/
	void app_mtu_set(uint16_t mtu);

	/// @} APP
	///
#endif //(BLE_APP_PRF)
#endif //(BLE_APP_PRESENT)

#endif // APP_H_
