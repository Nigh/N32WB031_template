
/**
 * @file app_custom_srv.h
 * @author
 * @version v1.0.0
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
#define APP_CUSTOMS_16BIT_MANUFACTURER_NAME       ("CUST1_16")
#define APP_CUSTOMS_16BIT_MANUFACTURER_NAME_LEN   (8)



#define ATT_SERVICE_AM_SPEED_16          0xCC00           /*!< Service UUID */
#define ATT_CHAR_AM_SPEED_WRITE_16       0xCC01           /*!< Characteristic value UUID */
#define ATT_CHAR_AM_SPEED_NTF_16         0xCC02           /*!< Characteristic value UUID */

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
