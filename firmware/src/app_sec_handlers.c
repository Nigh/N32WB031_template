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
 * @file app_sec_handlers.c
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
#include "app_user_callback_config.h"
#include "app_callback.h"
#include "app_user_config.h"
#include "app_sec_handlers.h"
#include "gapc_task.h"
#include "co_math.h"
#include "rwip.h"
#include "co_bt_defines.h"
#include "app.h"
#include "string.h"
#include "app_fsm.h"
#include "ns_sec.h"
#include "rwapp_config.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

extern struct app_env_tag app_env;
extern struct app_sec_env_tag app_sec_env;
extern struct app_sec_bond_data_env_tag app_sec_bond_data;

/**
 * @brief  default_app_sec_init
 * @param
 * @return
 * @note
 */
void default_app_sec_init(void)
{

}

/**
 * @brief  default_app_pairing_request
 * @param
 * @return
 * @note
 */
void default_app_pairing_request(struct gapc_bond_cfm* cfm)
{
	NS_LOG_INFO("GAPC_PAIRING_REQ\r\n");

	cfm->accept  = true;

	cfm->data.pairing_feat.auth      = ( SEC_PARAM_BOND | (SEC_PARAM_MITM << 2) | (SEC_PARAM_LESC << 3) | (SEC_PARAM_KEYPRESS << 4) );
	cfm->data.pairing_feat.iocap     = SEC_PARAM_IO_CAPABILITIES;
	cfm->data.pairing_feat.key_size  = SEC_PARAM_KEY_SIZE;
	cfm->data.pairing_feat.oob       = SEC_PARAM_OOB;
	cfm->data.pairing_feat.ikey_dist = SEC_PARAM_IKEY;
	cfm->data.pairing_feat.rkey_dist = SEC_PARAM_RKEY;
	cfm->data.pairing_feat.sec_req   = SEC_PARAM_SEC_MODE_LEVEL;
}

/**
 * @brief  default_app_tk_exch_nomitm
 * @param
 * @return
 * @note
 */
void default_app_tk_exch_nomitm(struct gapc_bond_cfm* cfm)
{
	NS_LOG_INFO("KEY_DISPLAY\r\n");

	cfm->accept  = true;

	rwip_time_t current_time;

	// Get current time
	current_time = rwip_time_get();

	// Generate a PIN Code- (Between 100000 and 999999)
	uint32_t pin_code = ((100000 + (co_rand_word() % 900000) + current_time.hs) % 999999);
	pin_code = 123456; //fix pin code

	// Set the TK value
	memset(cfm->data.tk.key, 0, KEY_LEN);

	cfm->data.tk.key[0] = (uint8_t)((pin_code & 0x000000FF) >>  0);
	cfm->data.tk.key[1] = (uint8_t)((pin_code & 0x0000FF00) >>  8);
	cfm->data.tk.key[2] = (uint8_t)((pin_code & 0x00FF0000) >> 16);
	cfm->data.tk.key[3] = (uint8_t)((pin_code & 0xFF000000) >> 24);

	NS_LOG_INFO("pin_code = %d\r\n", pin_code);
}

/**
 * @brief  default_app_ltk_exch
 * @param
 * @return
 * @note
 */
void default_app_ltk_exch(struct gapc_bond_cfm* cfm)
{
	NS_LOG_INFO("LTK EXCH\r\n");

	cfm->accept  = true;

	// Counter
	uint8_t counter;
	rwip_time_t current_time;

	// Get current time
	current_time = rwip_time_get();

	// Generate all the values
	cfm->data.ltk.ediv = (uint16_t)(co_rand_word() + current_time.hs);

	for (counter = 0; counter < RAND_NB_LEN; counter++) {
		cfm->data.ltk.ltk.key[counter]    = (uint8_t)(co_rand_word() + current_time.hs);
		cfm->data.ltk.randnb.nb[counter]  = (uint8_t)(co_rand_word() + current_time.hus);
	}

	for (counter = RAND_NB_LEN; counter < KEY_LEN; counter++) {
		cfm->data.ltk.ltk.key[counter]    = (uint8_t)(co_rand_word() + current_time.hus + current_time.hs);
	}
	cfm->data.ltk.key_size = SEC_PARAM_KEY_SIZE;

	memcpy(&app_sec_bond_data.ltk.key, &cfm->data.ltk.ltk.key, sizeof(struct gapc_ltk));
	memcpy(&app_sec_bond_data.rand_nb, &cfm->data.ltk.randnb, sizeof(rand_nb_t));
	app_sec_bond_data.ediv      = cfm->data.ltk.ediv;
	app_sec_bond_data.key_size  = cfm->data.ltk.key_size;
}

/**
 * @brief  default_app_pairing_succeded
 * @param
 * @return
 * @note
 */
void default_app_pairing_succeded(struct gapc_bond_ind const* bond_ind_data)
{
	NS_LOG_INFO("GAPC_PAIRING_SUCCEED\r\n");

#if (BOND_STORE_ENABLE)
	app_fsm_set_async_task(ASYNC_TASK_WRITE_BONDING_DATA);
#endif
}

/**
 * @brief  default_app_encrypt_req_ind
 * @param
 * @return
 * @note
 */
void default_app_encrypt_req_ind(struct gapc_encrypt_cfm* cfm, struct gapc_encrypt_req_ind const* param)
{
	NS_LOG_INFO("Func:[%s]\r\n", __func__);

	cfm->found    = false;

	if (app_sec_env.bonded) {

#if (BOND_STORE_ENABLE)

		uint8_t peer_num = 0;
		struct app_sec_bond_data_env_tag  bond_data;

		CALLBACK_ARGS_1(user_bond_callbacks.app_bdb_get_size, &peer_num);

		for(uint8_t i = 0; i < peer_num; i++) {
			CALLBACK_ARGS_2(user_bond_callbacks.app_bdb_load, i, &bond_data);

			if( (bond_data.ediv == param->ediv) &&
			        !memcmp(&bond_data.rand_nb, &param->rand_nb.nb, sizeof(struct rand_nb)) ) {
				NS_LOG_INFO("Found\r\n");

				cfm->found    = true;
				cfm->key_size = bond_data.key_size;
				memcpy(cfm->ltk.key, bond_data.ltk.key, sizeof(struct gap_sec_key));

				break;
			}
		}

#else

		if( (app_sec_bond_data.ediv == param->ediv) &&
		        !memcmp(&app_sec_bond_data.rand_nb, &param->rand_nb.nb, sizeof(struct rand_nb)) ) {
			NS_LOG_INFO("Found\r\n");

			cfm->found    = true;
			cfm->key_size = app_sec_bond_data.key_size;
			memcpy(cfm->ltk.key, app_sec_bond_data.ltk.key, sizeof(struct gap_sec_key));
		}

#endif //BOND_STORE_ENABLE


	}
	if(cfm->found == false) {
		NS_LOG_INFO("Not found\r\n");
		//disconnect if key miss
		struct gapc_disconnect_cmd* p_cmd = KE_MSG_ALLOC(GAPC_DISCONNECT_CMD,
		                                    KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
		                                    gapc_disconnect_cmd);

		NS_LOG_DEBUG("Func:[%s]\r\n", __func__);
		p_cmd->operation = GAPC_DISCONNECT;
		p_cmd->reason    = CO_ERROR_PIN_MISSING;//CO_ERROR_REMOTE_USER_TERM_CON;

		// Send the message
		ke_msg_send(p_cmd);
	}
}


/**
 * @}
 */

