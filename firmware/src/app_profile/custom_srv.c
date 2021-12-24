
/**
 * @file custom_srv.c
 * @author
 * @version v1.0.0
 */

/* Includes ------------------------------------------------------------------*/
#include "rwip_config.h"              // SW configuration

#include "custom_srv.h"
#include "custom_srv_task.h"
#include "attm_db.h"
#include "gapc.h"
#include "prf.h"

#include "ke_mem.h"

#include "app_custom_srv.h"


/* Private functions ---------------------------------------------------------*/

/**
 * @brief Initialization of the RDTSS 16bit uuid module.
 * This function performs all the initializations of the Profile module.
 *  - Creation of database (if it's a service)
 *  - Allocation of profile required memory
 *  - Initialization of task descriptor to register application
 *      - Task State array
 *      - Number of tasks
 *      - Default task handler
 *
 * @param[out]    env        Collector or Service allocated environment data.
 * @param[in|out] start_hdl  Service start handle (0 - dynamically allocated), only applies for services.
 * @param[in]     app_task   Application task number.
 * @param[in]     sec_lvl    Security level (AUTH, EKS and MI field of @see enum attm_value_perm_mask)
 * @param[in]     param      Configuration parameters of profile collector or service (32 bits aligned)
 *
 * @return status code to know if profile initialization succeed or not.
 */
static uint8_t custom_srv_init(struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task, uint8_t sec_lvl, struct custom_srv_db_cfg* params)
{
	//------------------ create the attribute database for the profile -------------------

	// DB Creation Status
	uint8_t status = ATT_ERR_NO_ERROR;

	uint16_t uuid_16 = ATT_SERVICE_AM_SPEED_16;
	uint32_t cfg_flag = ((1 << CUSTOM_SRV_IDX_NB) - 1);

	status = attm_svc_create_db(start_hdl, uuid_16, (uint8_t*)&cfg_flag,
	                            CUSTOM_SRV_IDX_NB, NULL, env->task, custom_srv_att_db,
	                            (sec_lvl & PERM_MASK_SVC_AUTH) | (sec_lvl & PERM_MASK_SVC_EKS) | PERM(SVC_SECONDARY, DISABLE) );

	//-------------------- allocate memory required for the profile  ---------------------
	if (status == ATT_ERR_NO_ERROR) {
		struct custom_srv_env_tag* custom_srv_env =
		    (struct custom_srv_env_tag*) ke_malloc(sizeof(struct custom_srv_env_tag), KE_MEM_ATT_DB);

		// allocate CUSTOMS_16BITrequired environment variable
		env->env = (prf_env_t*)custom_srv_env;
		custom_srv_env->shdl = *start_hdl;
		custom_srv_env->max_nb_att = CUSTOM_SRV_IDX_NB;
		custom_srv_env->prf_env.app_task = app_task
		                                   | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
		custom_srv_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

		// initialize environment variable
		env->id                     = TASK_ID_CUSTOM_SRV;

		custom_srv_task_init(&(env->desc));

		co_list_init(&(custom_srv_env->values));
		custom_srv_init_ccc_values(custom_srv_att_db, CUSTOM_SRV_IDX_NB);

		// service is ready, go into an Idle state
		ke_state_set(env->task, CUSTOMS_16BIT_IDLE);
	}

	return status;
}
/**
 * @brief Destruction of the CUSTOMS_16BITmodule - due to a reset for instance.
 * This function clean-up allocated memory (attribute database is destroyed by another
 * procedure)
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 */
static void custom_srv_destroy(struct prf_task_env* env)
{
	struct custom_srv_env_tag* custom_srv_env = (struct custom_srv_env_tag*)env->env;

	// remove all values present in list
	while (!co_list_is_empty(&(custom_srv_env->values))) {
		struct co_list_hdr* hdr = co_list_pop_front(&(custom_srv_env->values));
		ke_free(hdr);
	}

	// free profile environment variables
	env->env = NULL;
	ke_free(custom_srv_env);
}

/**
 * @brief Handles Connection creation
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index
 */
static void custom_srv_create(struct prf_task_env* env, uint8_t conidx)
{
	int att_idx;
	ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

	// Find all ccc fields and clean them
	for (att_idx = 1; att_idx < CUSTOM_SRV_IDX_NB; att_idx++) {
		// Find only CCC characteristics
		if( PERM_GET(custom_srv_att_db[att_idx].perm, UUID_LEN) == 0 &&
		        (custom_srv_att_db[att_idx].uuid) == ATT_DESC_CLIENT_CHAR_CFG
		  ) {
			// Clear CCC value
			custom_srv_set_ccc_value(conidx, att_idx, 0);
		}
	}
}

/**
 * @brief Handles Disconnection
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index
 * @param[in]        reason     Detach reason
 */
static void custom_srv_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
	int att_idx;

	struct custom_srv_env_tag* custom_srv_env = (struct custom_srv_env_tag*)env->env;

	ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

	// Find all ccc fields and clean them
	for (att_idx = 1; att_idx < CUSTOM_SRV_IDX_NB; att_idx++) {
		// Find only CCC characteristics
		if( PERM_GET(custom_srv_att_db[att_idx].perm, UUID_LEN) == 0 &&
		        (custom_srv_att_db[att_idx].uuid) == ATT_DESC_CLIENT_CHAR_CFG
		  ) {
			// Clear CCC value
			custom_srv_set_ccc_value(conidx, att_idx, 0);
		}
	}

	ke_state_set(prf_src_task_get(&(custom_srv_env->prf_env), conidx), CUSTOMS_16BIT_IDLE);
}

/* Public variables ---------------------------------------------------------*/

/// CUSTOMS_16BITTask interface required by profile manager
const struct prf_task_cbs custom_srv_itf = {
	(prf_init_fnct) custom_srv_init,
	custom_srv_destroy,
	custom_srv_create,
	custom_srv_cleanup,
};

/* Private functions ---------------------------------------------------------*/

const struct prf_task_cbs* custom_srv_prf_itf_get(void)
{
	return &custom_srv_itf;
}

