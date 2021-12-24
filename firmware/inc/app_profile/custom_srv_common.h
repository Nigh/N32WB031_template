/**
 * @file custom_srv_common.h
 * @author
 * @version v1.0.0
 */

#ifndef __CUSTOM_SRV_COMMON_H__
#define __CUSTOM_SRV_COMMON_H__

#include "gattc_task.h"

/**
 * @brief Validate the value of the Client Characteristic CFG.
 * @param[in] is_notification indicates whether the CFG is Notification (true) or Indication (false)
 * @param[in] param Pointer to the parameters of the message.
 * @return status.
 */
int check_client_char_cfg(bool is_notification, const struct gattc_write_req_ind* param);

/**
 * @brief Find the handle of the Characteristic Value having as input the Client Characteristic CFG handle
 * @param[in] cfg_handle the Client Characteristic CFG handle
 * @return the corresponding value handle
 */
uint16_t get_value_handle(uint16_t cfg_handle);

/**
 * @brief Find the handle of Client Characteristic CFG having as input the Characteristic value handle
 * @param[in] value_handle the Characteristic value handle
 * @return the corresponding Client Characteristic CFG handle
 */
uint16_t get_cfg_handle(uint16_t value_handle);

/**
* @brief Compute the handle of a given attribute based on its index
* @details Specific to raw data transfer server in 16bit uuid
* @param[in] att_idx attribute index
* @return the corresponding handle
*/
uint16_t custom_srv_get_att_handle(uint8_t att_idx);

/**
* @brief Compute the handle of a given attribute based on its index
* @details Specific to raw data transfer server in 16bit uuid
* @param[in] handle attribute handle
* @param[out] att_idx attribute index
* @return high layer error code
*/
uint8_t custom_srv_get_att_idx(uint16_t handle, uint8_t* att_idx);

#endif // __CUSTOM_SRV_COMMON_H__
