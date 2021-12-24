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
 * @file app_callback.h
 * @author Nations Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */

#ifndef _APP_CALLBACK_H_
#define _APP_CALLBACK_H_

/* Includes ------------------------------------------------------------------*/

#include "rwapp_config.h"
#include "gapc_task.h"
#include "app_bond_handlers.h"
#include "stdint.h"

/* Public define ------------------------------------------------------------*/

struct app_callbacks {
#if 1//(BLE_APP_SEC)

	void (*app_sec_init)(void);
	void (*app_pairing_request)(struct gapc_bond_cfm*);
	void (*app_tk_exch_nomitm)(struct gapc_bond_cfm*);
	void (*app_irk_exch)(struct gapc_bond_cfm*);
	void (*app_csrk_exch)(struct gapc_bond_cfm*);
	void (*app_ltk_exch)(struct gapc_bond_cfm*);
	void (*app_pairing_succeded)(struct gapc_bond_ind const*);
	void (*app_encrypt_ind)(uint8_t);
	void (*app_mitm_passcode_req)(struct gapc_bond_cfm*);
	void (*app_encrypt_req_ind)(struct gapc_encrypt_cfm*, struct gapc_encrypt_req_ind const*);
	void (*app_security_req_ind)(uint8_t);

#endif
};

struct bond_callbacks {
#if 1//(BLE_APP_SEC)

	void (*app_bdb_init)(void);
	void (*app_bdb_get_size)(uint8_t*);
	void (*app_bdb_add_entry)(struct app_sec_bond_data_env_tag*);
	void (*app_bdb_load)(uint8_t index, struct app_sec_bond_data_env_tag*);

#endif
};

/* Define ------------------------------------------------------------*/

#define CALLBACK_ARGS_0(cb)                      {if (cb != NULL) cb();}
#define CALLBACK_ARGS_1(cb, arg1)                {if (cb != NULL) cb(arg1);}
#define CALLBACK_ARGS_2(cb, arg1, arg2)          {if (cb != NULL) cb(arg1, arg2);}
#define CALLBACK_ARGS_3(cb, arg1, arg2, arg3)    {if (cb != NULL) cb(arg1, arg2, arg3);}


#endif // _APP_CALLBACK_H_
