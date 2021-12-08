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
 * @file app_user_callback_config.h
 * @author Nations Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */

/** @addtogroup
 * @{
 */

#ifndef _APP_USER_CALLBACK_CONFIG_H_
#define _APP_USER_CALLBACK_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rwapp_config.h"
#include "app_callback.h"
#include "app_sec_handlers.h"
#include "app_bond_handlers.h"

static const struct app_callbacks user_app_callbacks = {

#if (BLE_APP_SEC)

	.app_sec_init                   = NULL,//default_app_sec_init,
	.app_pairing_request            = default_app_pairing_request,
	.app_tk_exch_nomitm             = default_app_tk_exch_nomitm,
	.app_irk_exch                   = NULL,
	.app_csrk_exch                  = NULL,
	.app_ltk_exch                   = default_app_ltk_exch,
	.app_pairing_succeded           = default_app_pairing_succeded,
	.app_encrypt_ind                = NULL,
	.app_mitm_passcode_req          = NULL,
	.app_encrypt_req_ind            = default_app_encrypt_req_ind,
	.app_security_req_ind           = NULL,

#endif
};

static const struct bond_callbacks user_bond_callbacks = {

#if (BLE_APP_SEC)

	.app_bdb_init                   = default_app_bdb_init,
	.app_bdb_get_size               = default_app_bdb_get_size,
	.app_bdb_add_entry              = default_app_bdb_add_entry,
	.app_bdb_load                   = default_app_bdb_load,

#endif
};



#ifdef __cplusplus
}
#endif

#endif // _APP_USER_CALLBACK_CONFIG_H_

/**
 * @}
 */
