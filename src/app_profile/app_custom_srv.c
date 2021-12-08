/**
 * @file app_custom_srv.c
 * @author
 * @version v1.0.0
 */

/**
 * @addtogroup APP
 * @{
 */

#include "rwip_config.h"     // SW configuration

/* Includes ------------------------------------------------------------------*/
#include "co_utils.h"
#include "app_custom_srv.h"                    // rdtss 16bit uuid Application Module Definitions
#include "custom_srv_task.h"                   // Device Information Profile Functions
#include "gapm_task.h"                   // GAP Manager Task API
#include "app.h"
#include "prf.h"
#include "ke_timer.h"

#include "stdio.h"
#include "app_task.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


struct attm_desc custom_srv_att_db[CUSTOM_SRV_IDX_NB] = {
	/* Service Declaration */
	[0] = {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE),                                              0,                                          0       },

	/* Characteristic Declaration */
	[1] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE),                     0,                                          0       },
	/* Characteristic Value */
	[2] = {ATT_CHAR_AM_SPEED_WRITE_16, PERM(WRITE_COMMAND, ENABLE),                                 PERM(RI, ENABLE) | PERM_VAL(UUID_LEN, 0), 0x200   },
	/* Client Characteristic Configuration Descriptor */
	[3] = {ATT_DESC_CHAR_USER_DESCRIPTION, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE),              PERM(RI, ENABLE),                           20      },

	/* Characteristic Declaration */
	[4] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE),                     0,                                          0       },
	/* Characteristic Value */
	[5] = {ATT_CHAR_AM_SPEED_NTF_16, PERM(NTF, ENABLE),                                              PERM(RI, ENABLE) | PERM_VAL(UUID_LEN, 0), 0x200   },
	/* Client Characteristic Configuration Descriptor */
	[6] = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE),                      PERM(RI, ENABLE),                           20      },
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void app_custom_srv_init(void)
{
	// Nothing to do
}

/**
 * @brief Create raw data transfer server profile database.
 * @return void
 */
void app_custom_srv_add_profile(void)
{
	struct custom_srv_db_cfg* db_cfg;

	struct gapm_profile_task_add_cmd* req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
	                                        TASK_GAPM,
	                                        TASK_APP,
	                                        gapm_profile_task_add_cmd,
	                                        sizeof(struct custom_srv_db_cfg));

	// Fill message
	req->operation = GAPM_PROFILE_TASK_ADD;
	req->sec_lvl = PERM(SVC_AUTH, NO_AUTH);
	req->prf_task_id = TASK_ID_CUSTOM_SRV;
	req->app_task = TASK_APP;
	req->start_hdl = 0;

	// Set parameters
	db_cfg = (struct custom_srv_db_cfg*) req->param;
	// Attribute table. In case the handle offset needs to be saved
	db_cfg->att_tbl = NULL;
	db_cfg->cfg_flag = 0;
	db_cfg->features = 0;

	// Send the message
	ke_msg_send(req);
}

/**
 * @brief  rdtss value require indicate handler
 * @param
 * @return
 * @note   Note
 */
static int custom_srv_value_req_ind_handler(ke_msg_id_t const msgid,
        struct custom_srv_value_req_ind const* req_value,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
	NS_LOG_DEBUG("Func:[%s]\r\n", __func__);

	// Initialize length
	uint8_t len = 0;
	// Pointer to the data
	uint8_t* data = NULL;

	len = APP_CUSTOMS_16BIT_MANUFACTURER_NAME_LEN;
	data = (uint8_t*)APP_CUSTOMS_16BIT_MANUFACTURER_NAME;


	// Allocate confirmation to send the value
	struct custom_srv_value_req_rsp* rsp_value = KE_MSG_ALLOC_DYN(CUSTOMS_16BIT_VALUE_REQ_RSP,
	        src_id, dest_id,
	        custom_srv_value_req_rsp,
	        len);

	rsp_value->length = len;
	rsp_value->att_idx = req_value->att_idx;
	if (len) {
		// Copy data
		memcpy(&rsp_value->value, data, len);
	}
	// Send message
	ke_msg_send(rsp_value);

	return (KE_MSG_CONSUMED);
}

/**
 * @brief  cuusts write indicate handler
 * @param
 * @return
 * @note
 */
static int custom_srv_val_write_ind_handler(ke_msg_id_t const msgid,
        struct custom_srv_val_write_ind const* ind_value,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
	NS_LOG_DEBUG("Func:[%s]\r\n\twrite handle = %x\r\n\tlength = %x\r\n\t", __func__, ind_value->handle, ind_value->length);

	for(uint16_t i = 0; i < ind_value->length; i++) {
		NS_LOG_INFO("%x ", ind_value->value[i]);
	}
	NS_LOG_INFO("\r\n");

	uint16_t handle = ind_value->handle;
	uint16_t length = ind_value->length;

	switch (handle) {
		case CUSTOM_SRV_IDX_NTF_CFG:

			NS_LOG_DEBUG("CUSTOM_SRV_IDX_NTF_CFG\r\n");

			if(length == 2) {
				uint16_t cfg_value = ind_value->value[0] + ind_value->value[1];

				if(cfg_value == PRF_CLI_START_NTF) {
					//enabled notify
				} else if(cfg_value == PRF_CLI_STOP_NTFIND) {
				}
			}

			break;
		case CUSTOM_SRV_IDX_WRITE_VAL:
			NS_LOG_DEBUG("CUSTOM_SRV_IDX_WRITE_VAL\r\n");
			// TODO: Receive
			// app_usart_tx_fifo_enter(ind_value->value, ind_value->length);
			break;
		default:
			break;

	}

	return (KE_MSG_CONSUMED);
}

/**
 * @brief  Functions
 * @param
 * @return
 * @note   Note
 */
static int custom_srv_val_ntf_cfm_handler(ke_msg_id_t const msgid,
        struct custom_srv_val_ntf_cfm const* cfm_value,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
	NS_LOG_DEBUG("Func:[%s]\r\n\tntf cfm handle = %x\r\n\tstatus = %x\r\n", __func__, cfm_value->handle, cfm_value->status);
	// usart_forward_to_ble_loop();
	return (KE_MSG_CONSUMED);
}

/**
 * @brief  custom_srv send notify
 * @param
 * @return
 * @note
 */
void custom_srv_send_notify(uint8_t* data, uint16_t length)
{
	struct custom_srv_val_ntf_ind_req* req = KE_MSG_ALLOC_DYN(CUSTOMS_16BIT_VAL_NTF_REQ,
	        prf_get_task_from_id(TASK_ID_CUSTOM_SRV),
	        TASK_APP,
	        custom_srv_val_ntf_ind_req,
	        length);

	req->conidx = app_env.conidx;
	req->notification = true;
	req->handle = CUSTOM_SRV_IDX_NTF_VAL;
	req->length = length;
	memcpy(&req->value[0], data, length);

	ke_msg_send(req);
}


/// Default State handlers definition
const struct ke_msg_handler app_custom_srv_msg_handler_list[] = {
	{CUSTOMS_16BIT_VALUE_REQ_IND,                    (ke_msg_func_t)custom_srv_value_req_ind_handler},
	{CUSTOMS_16BIT_VAL_WRITE_IND,                    (ke_msg_func_t)custom_srv_val_write_ind_handler},
	{CUSTOMS_16BIT_VAL_NTF_CFM,                      (ke_msg_func_t)custom_srv_val_ntf_cfm_handler},

};

const struct app_subtask_handlers app_custom_srv_handlers = APP_HANDLERS(app_custom_srv);

