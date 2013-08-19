/**
 * \file datastore.h
 * \author Radek Krejci <rkrejci@cesnet.cz>
 * \brief NETCONF datastore handling function prototypes and structures.
 *
 * Copyright (C) 2012 CESNET, z.s.p.o.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Company nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * ALTERNATIVELY, provided that this notice is retained in full, this
 * product may be distributed under the terms of the GNU General Public
 * License (GPL) version 2 or later, in which case the provisions
 * of the GPL apply INSTEAD OF those given above.
 *
 * This software is provided ``as is, and any express or implied
 * warranties, including, but not limited to, the implied warranties of
 * merchantability and fitness for a particular purpose are disclaimed.
 * In no event shall the company or contributors be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential
 * damages (including, but not limited to, procurement of substitute
 * goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether
 * in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even
 * if advised of the possibility of such damage.
 *
 */

#ifndef DATASTORE_H_
#define DATASTORE_H_

#include "netconf.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup store
 * @brief Datastore implementation types provided by libnetconf
 */
typedef enum {
	NCDS_TYPE_EMPTY, /**< No datastore. For read-only devices. */
	NCDS_TYPE_FILE /**< Datastores implemented as files */
} NCDS_TYPE;

/**
 * @ingroup store
 * @brief Datastore ID.
 *
 * Each datastore gets its ID after initialisation (ncds_init()). Only
 * initialised datastores can be used to access the configuration data.
 */
typedef int ncds_id;

/**
 * @ingroup store
 * @brief Datastore ID to access libnetconf's internal datastores such as
 * notifications, monitoring, etc.
 */
#define NCDS_INTERNAL_ID 0

/**
 * @ingroup store
 * @brief Datastore structure
 */
struct ncds_ds;

/**
 * @ingroup store
 * @def NCDS_RPC_NOT_APPLICABLE
 * @brief Return value of ncds_apply_rpc() when the requested operation is
 * not applicable to the specified datastore.
 */
extern char error_area;
#define NCDS_RPC_NOT_APPLICABLE ((void*)(&error_area))

/**
 * @ingroup store
 * @brief Create a new datastore structure of the specified implementation type.
 * @param[in] type Datastore implementation type for the new datastore structure.
 * @param[in] model_path Path to the YIN configuration data model.
 * @param[in] get_state Pointer to a callback function that returns a serialized
 * XML document containing the state configuration data of the device. The parameters
 * it receives are a serialized configuration data model in YIN format and the current
 * content of the running datastore. If NULL is set, \<get\> operation is
 * performed in the same way as \<get-config\>.
 * @return Prepared (not configured) datastore structure. To configure the
 * structure, caller must use the parameter setters of the specific datastore
 * implementation type. Then, the datastore can be initiated (ncds_init()) and
 * used to access the configuration data.
 */
struct ncds_ds* ncds_new(NCDS_TYPE type, const char* model_path, char* (*get_state)(const char* model, const char* running, struct nc_err ** e));

/**
 * @ingroup transapi
 * @brief Create new datastore structure with transaction API support
 * @param[in] type Datastore implementation type for the new datastore structure.
 * @param[in] model_path Path to the YIN configuration data model.
 * @param[in] callbacks_path Path to shared library with callbacks and other functions for transaction API.
 *
 * @return Prepared (not configured) datastore structure. To configure the
 * structure, caller must use the parameter setters of the specific datastore
 * implementation type. Then, the datastore can be initiated (ncds_init()) and
 * used to access the configuration data.
 */
struct ncds_ds* ncds_new_transapi(NCDS_TYPE type, const char* model_path, const char* callbacks_path);

/**
 * @ingroup store
 * @brief Assign the path of the datastore file into the datastore structure.
 *
 * Checks if the file exist and is accessible for reading and writing.
 * If the file does not exist, it is created. The file is opened and the file
 * descriptor is stored in the structure.
 *
 * @param[in] datastore Datastore structure to be configured.
 * @param[in] path File path to the file storing configuration datastores.
 * @return
 * - 0 on success
 * - -1 Invalid datastore
 * - -2 Invalid path ((does not exist && can not be created) || insufficient rights)
 */
int ncds_file_set_path (struct ncds_ds* datastore, const char* path);

/**
 * @ingroup store
 * @brief Activate datastore structure for use.
 *
 * The datastore configuration is checked and if everything is correct,
 * datastore gets its unique ID to be used for datastore operations
 * (ncds_apply_rpc()).
 *
 * @param[in] datastore Datastore to be initiated.
 * @return Positive integer with the datastore ID on success, negative value on
 * error.
 * - -1 Invalid datastore
 * - -2 Type-specific initialization failed
 * - -3 Unsupported datastore type
 * - -4 Memory allocation problem
 */
ncds_id ncds_init(struct ncds_ds* datastore);

/**
 * @brief Initialize transAPI module(s) (if present) and copy startup configuration to running
 *
 * @param id Pointer to ncds_id of device to initialize, if NULL all found transapi-capable devices will be initialized
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int ncds_device_init (ncds_id * id);

/**
 * @ingroup store
 * @brief Close the specified datastore and free all the resources.
 *
 * Equivalent function to ncds_free2().
 *
 * @param[in] datastore Datastore to be closed.
 */
void ncds_free(struct ncds_ds* datastore);

/**
 * @ingroup store
 * @brief Close specified datastore and free all the resources.
 *
 * Equivalent function to ncds_free().
 *
 * @param[in] datastore_id ID of the datastore to be closed.
 */
void ncds_free2(ncds_id datastore_id);

/**
 * @ingroup store
 * @brief Perform the requested RPC operation on the datastore.
 * @param[in] id Datastore ID. Use #NCDS_INTERNAL_ID (0) to apply request
 * (typically \<get\>) onto the libnetconf's internal datastore.
 * @param[in] session NETCONF session (a dummy session is acceptable) where the
 * \<rpc\> came from. Capabilities checks are done according to this session.
 * @param[in] rpc NETCONF \<rpc\> message specifying requested operation.
 * @return NULL in case of a non-NC_RPC_DATASTORE_* operation type or invalid
 * parameter session or rpc, else \<rpc-reply\> with \<ok\>, \<data\> or
 * \<rpc-error\> according to the type and the result of the requested
 * operation. When the requested operation is not applicable to the specified
 * datastore (e.g. the namespace does not match), NCDS_RPC_NOT_APPLICABLE
 * is returned.
 */
nc_reply* ncds_apply_rpc(ncds_id id, const struct nc_session* session, const nc_rpc* rpc);

/**
 * @ingroup store
 * @brief Perform the requested RPC operation on the all datastores controlled
 * by the libnetconf (created by ncdsd_new() and ncds_init()).
 *
 * **This function IS NOT thread safety.**
 *
 * @param[in] session NETCONF session (a dummy session is acceptable) where the
 * \<rpc\> came from. Capabilities checks are done according to this session.
 * @param[in] rpc NETCONF \<rpc\> message specifying requested operation.
 * @param[out] ids Pointer to a static array containing list of datastore IDs
 * where the RPC was successfully applied. The list is terminated by value a
 * (ncds_id)(-1). The list is rewritten by any following call to
 * ncds_apply_rpc2all().
 * @return NULL in case of a non-NC_RPC_DATASTORE_* operation type or invalid
 * parameter session or rpc, else \<rpc-reply\> with \<ok\>, \<data\> or
 * \<rpc-error\> according to the type and the result of the requested
 * operation. When the requested operation is not applicable to any datastore
 * (e.g. the namespace does not match no of the controlled datstores),
 * NCDS_RPC_NOT_APPLICABLE is returned.
 *
 */
nc_reply* ncds_apply_rpc2all(struct nc_session* session, const nc_rpc* rpc, ncds_id* ids[]);

/**
 * @ingroup store
 * @brief Undo the last change performed on the specified datastore.
 * @param[in] id ID of the datastore where the rollback will be performed.
 * @return 0 on success, non-zero on error.
 */
int ncds_rollback(ncds_id id);

/**
 * @ingroup store
 * @brief Remove all the locks that the given session is holding.
 *
 * @param[in] session Session holding locks to remove
 */
void ncds_break_locks (const struct nc_session* session);

/**
 * @ingroup store
 * @brief Return a serialized XML containing the data model in the YIN format
 *
 * @param[in] id ID of the datastore whose data model we want
 * @param[in] base Set 1 to get only base model without any modification. Use 0
 * value to get complete data model including augmentation, substituted uses
 * statements and removed disabled features of the model. In this case, returned
 * string contains modified YIN format - there are \<augment\> elements inside
 * the model including information about its namespace and module name.
 *
 * @return String containing YIN model. Caller must free the memory after use.
 */
char* ncds_get_model (ncds_id id, int base);

/**
 * @ingroup store
 * @brief Return path to the file containing the datastore datamodel
 *
 * @param[in] id ID of the datastore whose data model we want
 *
 * @return String containing the path to the file containing the datastore datamodel.
 * The caller must NOT free the memory.
 */
const char * ncds_get_model_path (ncds_id id);


/**
 * @ingroup store
 * @brief Specify a directory path to the location where the required (imported
 * or included) data models can be found. This function can be called repeatedly
 * to specify multiple locations.
 *
 * @param[in] path Directory path
 * @return 0 on success, non-zero on error.
 */
int ncds_add_models_path(const char* path);

/**
 * @ingroup store
 * @brief Add an configuration data model to the internal list of models. Such
 * model is used to resolve imports, includes and uses statements in base models.
 *
 * @param[in] path Path to the YIN format of the configuration data model.
 * @return 0 on success, non-zero on error.
 */
int ncds_add_model(const char* path);

/**
 * @ingroup store
 * @brief Enable usage of the specified feature defined in the specified module.
 * By default, all features are disabled.
 * @param[in] module Name of the module where the feature is defined. Module
 * must be accessible - added via ncds_add_model() or present in a directory
 * specified via ncds_add_models_path() function.
 * @param[in] feature Name of the feature to be enabled.
 * @return 0 on success, non-zero on error.
 */
int ncds_feature_enable(const char* module, const char* feature);

/**
 * @ingroup store
 * @brief Disable usage of the specified feature defined in the specified module
 * By default, all features are disabled.
 * @param[in] module Name of the module where the feature is defined. Module
 * must be accessible - added via ncds_add_model() or present in a directory
 * specified via ncds_add_models_path() function.
 * @param[in] feature Name of the feature to be disabled.
 * @return 0 on success, non-zero on error.
 */
int ncds_feature_disable(const char* module, const char* feature);

/**
 * @ingroup store
 * @brief Enable usage of all features defined in the specified module. By
 * default, all features are disabled. To enable only the specific feature(s),
 * use ncds_feature_enable().
 * @param[in] module Name of the module where the feature is defined. Module
 * must be accessible - added via ncds_add_model() or present in a directory
 * specified via ncds_add_models_path() function.
 * @return 0 on success, non-zero on error.
 */
int ncds_features_enableall(const char* module);

/**
 * @ingroup store
 * @brief Disable usage of all features defined in the specified module. By
 * default, all features are disabled. To disable only the specific feature(s),
 * use ncds_feature_disable().
 * @param[in] module Name of the module where the feature is defined. Module
 * must be accessible - added via ncds_add_model() or present in a directory
 * specified via ncds_add_models_path() function.
 * @return 0 on success, non-zero on error.
 */
int ncds_features_disableall(const char* module);

/**
 * @ingroup store
 * @brief Check if the feature of the specified module is currently enabled or
 * disabled.
 * @return
 * - negative value in case of error
 * - 0 if feature is disabled
 * - 1 if feature is enabled
 */
int ncds_feature_isenabled(const char* module, const char* feature);

/**
 * @ingroup store
 * @brief Consolidate all internal structures of created data stores and all
 * data models. This function especially solves all YANG's `uses` and `augment`
 * statements.
 *
 * @return 0 on success, non-zero on error.
 */
int ncds_consolidate(void);

#ifdef __cplusplus
}
#endif

#endif /* DATASTORE_H_ */
