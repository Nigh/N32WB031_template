/**
 * @file custom_srv_task.h
 * @author
 * @version v1.0.0
 */

#ifndef __CUSTOM_SRV_TASK_PRF_H
#define __CUSTOM_SRV_TASK_PRF_H

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include "ke_task.h"
#include "prf_types.h"
#include "compiler.h"        // compiler definition
#include "att.h"
#include "attm.h"

#include "rwip_task.h"
#include "custom_srv.h"
/* Public typedef -----------------------------------------------------------*/

/// Possible states of the custom_srv task
enum custom_srv_state {
	/// Idle state
	CUSTOMS_16BIT_IDLE,
	/// Busy state
	CUSTOMS_16BIT_BUSY,
	/// Number of defined states.
	CUSTOMS_16BIT_STATE_MAX,
};

/// Messages for CUSTOM_SRV
enum {
	/// Add a custom_srv instance into the database
	CUSTOMS_16BIT_CREATE_DB_REQ = TASK_FIRST_MSG(TASK_ID_CUSTOM_SRV),
	/// Inform APP of database creation status
	CUSTOMS_16BIT_CREATE_DB_CFM,

	/// Start the Custom Service Task - at connection
	CUSTOMS_16BIT_ENABLE_REQ,
	/// Set/update characteristic value
	CUSTOMS_16BIT_VAL_SET_REQ,
	/// Peer device request to get a non-database value (RI enabled)
	CUSTOMS_16BIT_VALUE_REQ_IND,
	/// Response to non-database value request
	CUSTOMS_16BIT_VALUE_REQ_RSP,
	/// Set/update characteristic value and trigger a notification
	CUSTOMS_16BIT_VAL_NTF_REQ,
	/// Response after receiving a CUSTOMS_16BIT_VAL_NTF_REQ message and a notification is triggered
	CUSTOMS_16BIT_VAL_NTF_CFM,
	/// Set/update characteristic value and trigger an indication
	CUSTOMS_16BIT_VAL_IND_REQ,
	///Response after receiving a CUSTOMS_16BIT_VAL_IND_REQ message and an indication is triggered
	CUSTOMS_16BIT_VAL_IND_CFM,
	/// Indicate that the characteristic value has been written
	CUSTOMS_16BIT_VAL_WRITE_IND,
	/// Inform the application that the profile service role task has been disabled after a disconnection
	CUSTOMS_16BIT_DISABLE_IND,
	/// Profile error report
	CUSTOMS_16BIT_ERROR_IND,
	/// Inform the application that there is an attribute info request that shall be processed
	CUSTOMS_16BIT_ATT_INFO_REQ,
	/// Inform back that the attribute info request has been processed
	CUSTOMS_16BIT_ATT_INFO_RSP,
};



/// Parameters for the database creation
struct custom_srv_db_cfg {
	///max number of casts1 service characteristics
	uint8_t max_nb_att;
	uint8_t* att_tbl;
	uint8_t* cfg_flag;

	///Database configuration
	uint16_t features;
};

/// Parameters of the @ref CUSTOMS_16BIT_CREATE_DB_CFM message
struct custom_srv_create_db_cfm {
	///Status
	uint8_t status;
};

/// Parameters of the @ref CUSTOMS_16BIT_ENABLE_REQ message
struct custom_srv_enable_req {
	/// Connection index
	uint8_t conidx;
	/// security level: b0= nothing, b1=unauthenticated, b2=authenticated, b3=authorized; b1 or b2 and b3 can go together
	uint8_t sec_lvl;
	/// Type of connection
	uint8_t con_type;
};

/// Parameters of the @ref CUSTOMS_16BIT_DISABLE_IND message
struct custom_srv_disable_ind {
	/// Connection index
	uint8_t conidx;
};

/// Parameters of the @ref CUSTOMS_16BIT_VAL_WRITE_IND massage
struct custom_srv_val_write_ind {
	/// Connection index
	uint8_t  conidx;
	/// Handle of the attribute that has to be written
	uint16_t handle;
	/// Data length to be written
	uint16_t length;
	/// Data to be written in attribute database
	uint8_t  value[__ARRAY_EMPTY];
};

/// Parameters of the @ref CUSTOMS_16BIT_VAL_NTF_CFM massage
struct custom_srv_val_ntf_cfm {
	/// Connection index
	//    uint8_t  conidx;
	/// Handle of the attribute that has been updated
	uint16_t handle;
	/// Confirmation status
	uint8_t status;
};

/// Parameters of the @ref CUSTOMS_16BIT_VAL_IND_CFM massage
struct custom_srv_val_ind_cfm {
	/// Connection index
	uint8_t  conidx;
	/// Handle of the attribute that has been updated
	uint16_t handle;
	/// Confirmation status
	uint8_t status;
};

/// Parameters of the @ref CUSTOMS_16BIT_VAL_SET_REQ massage
struct custom_srv_val_set_req {
	/// Connection index
	uint8_t  conidx;
	/// Handle of the attribute that has to be written
	uint16_t handle;
	/// Data length to be written
	uint16_t length;
	/// Data to be written in attribute database
	uint8_t  value[__ARRAY_EMPTY];
};

/// Parameters of the @ref CUSTOMS_16BIT_VAL_REQ_IND message
struct custom_srv_value_req_ind {
	/// Connection index
	uint8_t  conidx;
	/// Index of the attribute for which value has been requested
	uint16_t att_idx;
};

/// Parameters of the @ref CUSTOMS_16BIT_VAL_REQ_RSP message
struct custom_srv_value_req_rsp {
	/// Connection index
	uint8_t  conidx;
	/// Index of the attribute for which value has been requested
	uint16_t att_idx;
	/// Current length of that attribute
	uint16_t length;
	/// ATT error code
	uint8_t  status;
	/// Data value
	uint8_t  value[__ARRAY_EMPTY];
};

/// Parameters of the @ref CUSTOMS_16BIT_VAL_NTF_REQ massage
struct custom_srv_val_ntf_ind_req {
	/// Connection index
	uint8_t  conidx;
	/// Notificatin/indication
	bool     notification;
	/// Handle of the attribute that has to be written
	uint16_t handle;
	/// Data length to be written
	uint16_t length;
	/// Data to be written in attribute database
	uint8_t  value[__ARRAY_EMPTY];
};

/// Parameters of the @ref CUSTOMS_16BIT_VAL_IND_REQ massage
struct custom_srv_val_ind_req {
	/// Connection index
	uint8_t  conidx;
	/// Handle of the attribute that has to be written
	uint16_t handle;
	/// Data length to be written
	uint16_t length;
	/// Data to be written in attribute database
	uint8_t  value[__ARRAY_EMPTY];
};

/// Parameters of the @ref CUSTOMS_16BIT_ATT_INFO_REQ message
struct custom_srv_att_info_req {
	/// Connection index
	uint8_t  conidx;
	/// Index of the attribute for which info has been requested
	uint16_t att_idx;
};

/// Parameters of the @ref CUSTOMS_16BIT_ATT_INFO_RSP message
struct custom_srv_att_info_rsp {
	/// Connection index
	uint8_t  conidx;
	/// Index of the attribute for which info has been requested
	uint16_t att_idx;
	/// Current length of that attribute
	uint16_t length;
	/// ATT error code
	uint8_t  status;
};

/**
 * @brief Initialize Client Characteristic Configuration fields.
 * @details Function initializes all CCC fields to default value.
 * @param[in] att_db         Id of the message received.
 * @param[in] max_nb_att     Pointer to the parameters of the message.
 */
void custom_srv_init_ccc_values(const struct attm_desc* att_db, int max_nb_att);

/**
 * @brief Set per connection CCC value for attribute
 * @details Function sets CCC for specified connection.
 * @param[in] conidx         Connection index.
 * @param[in] att_idx        Attribute index.
 * @param[in] ccc            Value of ccc.
 */
void custom_srv_set_ccc_value(uint8_t conidx, uint8_t att_idx, uint16_t ccc);


#endif // __CUSTOM_SRV_TASK_PRF_H