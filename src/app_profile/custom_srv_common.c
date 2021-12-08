
/**
 * @file custom_srv_common.c
 * @author
 * @version v1.0.0
 */

/* Includes ------------------------------------------------------------------*/
#include "rwip_config.h"              // SW configuration

#include "custom_srv_common.h"
#include "custom_srv.h"

#include "gattc_task.h"
#include "att.h"
#include "attm_db.h"
#include "prf_types.h"
#include "prf_utils.h"
/* Private define ------------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

int check_client_char_cfg(bool is_notification, const struct gattc_write_req_ind* param)
{
	uint8_t status = GAP_ERR_NO_ERROR;
	uint16_t ntf_cfg = 0;

	if (param->length != sizeof(uint16_t)) {
		status =  ATT_ERR_INVALID_ATTRIBUTE_VAL_LEN;
	} else {
		ntf_cfg = *((uint16_t*)param->value);

		if (is_notification) {
			if ( (ntf_cfg != PRF_CLI_STOP_NTFIND) && (ntf_cfg != PRF_CLI_START_NTF) ) {
				status =  PRF_ERR_INVALID_PARAM;
			}
		} else {
			if ( (ntf_cfg != PRF_CLI_STOP_NTFIND) && (ntf_cfg != PRF_CLI_START_IND) ) {
				status =  PRF_ERR_INVALID_PARAM;
			}
		}
	}

	return status;
}

uint16_t get_value_handle(uint16_t cfg_handle)
{
	uint8_t uuid[ATT_UUID_128_LEN];
	uint8_t uuid_len;
	uint16_t handle = cfg_handle;
	struct attm_svc* srv;

	srv = attmdb_get_service(handle);

	while ((handle >= srv->svc.start_hdl) && (handle <= srv->svc.end_hdl)) {
		struct attm_elmt elmt;

		// Retrieve UUID
		attmdb_get_attribute(handle, &elmt);
		attmdb_get_uuid(&elmt, &uuid_len, uuid, false, false);

		// check for Characteristic declaration
		if (*(uint16_t*)&uuid[0] == ATT_DECL_CHARACTERISTIC) {
			return handle + 1;
		}

		handle--;
	}

	return 0;  //Should not reach this point. something is wrong with the database
}

uint16_t get_cfg_handle(uint16_t value_handle)
{
	uint8_t uuid[ATT_UUID_128_LEN];
	uint8_t uuid_len;
	uint16_t handle = value_handle;
	struct attm_svc* srv;

	srv = attmdb_get_service(handle);

	/* Iterate the database to find the client characteristic configuration.
	*/
	while ((handle >= srv->svc.start_hdl) && (handle <= srv->svc.end_hdl)) {
		struct attm_elmt elmt;

		// Retrieve UUID
		attmdb_get_attribute(handle, &elmt);
		attmdb_get_uuid(&elmt, &uuid_len, uuid, false, false);

		// check for Client Characteristic Configuration
		if (*(uint16_t*)&uuid[0] == ATT_DESC_CLIENT_CHAR_CFG && uuid_len == sizeof(uint16_t)) {
			return handle;
		} else if (*(uint16_t*)&uuid[0] == ATT_DECL_CHARACTERISTIC && uuid_len == sizeof(uint16_t)) {
			break;    // found the next Characteristic declaration without findig a CC CFG,
		}

		handle++;
	}

	return 0;  //Should not reach this point. something is wrong with the database
}

uint16_t custom_srv_get_att_handle(uint8_t att_idx)
{
	struct custom_srv_env_tag* custom_srv_env = PRF_ENV_GET(CUSTOM_SRV, custom_srv);
	uint16_t handle = ATT_INVALID_HDL;

	if (att_idx < custom_srv_env->max_nb_att) {
		handle = custom_srv_env->shdl + att_idx;
	}

	return handle;
}

uint8_t custom_srv_get_att_idx(uint16_t handle, uint8_t* att_idx)
{
	struct custom_srv_env_tag* custom_srv_env = PRF_ENV_GET(CUSTOM_SRV, custom_srv);
	uint8_t status = PRF_APP_ERROR;

	if ((handle >= custom_srv_env->shdl) && (handle < custom_srv_env->shdl + custom_srv_env->max_nb_att)) {
		*att_idx = handle - custom_srv_env->shdl;
		status = ATT_ERR_NO_ERROR;
	}

	return status;
}

