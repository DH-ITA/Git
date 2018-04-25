/**
 ******************************************************************************
 * \file secr.h
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Softek Global eDesign S. de R.L. de C.V. All rights reserved.</b><br><br>
 * Softek Global eDesign Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup 
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all specific user configuration for mm100 Library.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 * 
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *     - CFE: Specification CFE G0100-05. March 2010.
 *
 *   - Applicable Standards:
 *     - ANSI C12.18 - 2005
 *     - ANSI C12.19 - 2008
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *     - None
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - Format: F-P06-DH-07
 *   - Revision: 02
 *   - Date: 2015-08-10
 *   - Author: Rodrigo Hurtado Rodriguez.
 ******************************************************************************
 */
#ifndef SECR_H_
#define SECR_H_

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "compiler.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
 */


/*===========================================================================*\
 * Bit Mask definitions
\*===========================================================================*/

/* TABLE_IDC_BFLD BIT MASK */
#define TBL_PROC_NBR_bm										(uint16_t) 0b0000011111111111
#define TBL_PROC_NBR_bp										(uint8_t)  0
#define STD_VS_MFG_FLAG_bm									(uint16_t) 0b0000100000000000
#define STD_VS_MFG_FLAG_bp									(uint8_t)  11
#define PROC_FLAG_bm										(uint16_t) 0b0001000000000000
#define PROC_FLAG_bp										(uint8_t)  12
#define UNRESTRICTED_READ_bm								(uint16_t) 0b0010000000000000
#define UNRESTRICTED_READ_bp								(uint8_t)  13
#define UNRESTRICTED_WRITE_bm								(uint16_t) 0b0100000000000000
#define UNRESTRICTED_WRITE_bp								(uint8_t)  14

/* ACCESS_PERMISSION_BFLD BIT MASK */
#define GROUP_PERM_0_FLAG_bm								(uint8_t) 0b00000001
#define GROUP_PERM_1_FLAG_bm								(uint8_t) 0b00000010
#define GROUP_PERM_2_FLAG_bm								(uint8_t) 0b00000100
#define GROUP_PERM_3_FLAG_bm								(uint8_t) 0b00001000
#define GROUP_PERM_4_FLAG_bm								(uint8_t) 0b00010000
#define GROUP_PERM_5_FLAG_bm								(uint8_t) 0b00100000
#define GROUP_PERM_6_FLAG_bm								(uint8_t) 0b01000000
#define GROUP_PERM_7_FLAG_bm								(uint8_t) 0b10000000

/*===========================================================================*\
 * ST_40: Security Dimension Limits Table (max values)
\*===========================================================================*/
#define MAX_NBR_PASSWORDS									(uint8_t)	 6
#define MAX_PASSWORD_LEN									(uint8_t)	20
#define MAX_NBR_KEYS										(uint8_t)	 0
#define MAX_KEY_LEN											(uint8_t)	 0
#define MAX_NBR_PERM_USED									(uint16_t)	64

/* ST_40 Elements size */
#define ST_40_NBR_PASSWORDS_SIZE							1
#define ST_40_PASSWORD_LEN_SIZE								1
#define ST_40_NBR_KEYS_SIZE									1
#define ST_40_KEY_LEN_SIZE									1
#define ST_40_NBR_PERM_USED_SIZE							2

/*===========================================================================*\
 * ST_41: Actual Security Limiting Table (default values)
\*===========================================================================*/
#define DEFAULT_NBR_PASSWORDS								(uint8_t)	 5
#define DEFAULT_PASSWORD_LEN								(uint8_t)	20
#define DEFAULT_NBR_KEYS									(uint8_t)	 0
#define DEFAULT_KEY_LEN										(uint8_t)	 0
#define DEFAULT_NBR_PERM_USED								(uint16_t)   0

/* ST_41 Elements size */
#define ST_41_NBR_PASSWORDS_SIZE							1
#define ST_41_PASSWORD_LEN_SIZE								1
#define ST_41_NBR_KEYS_SIZE									1
#define ST_41_KEY_LEN_SIZE									1
#define ST_41_NBR_PERM_USED_SIZE							2

/*===========================================================================*\
 * ST_42: Security Table (default values)
\*===========================================================================*/
#define DEFAULT_PASSWORD_0									"00000000000000000000"
#define	DEFAULT_PASSWORD_1									"00000000000000000000"
#define	DEFAULT_PASSWORD_2									"00000000000000000000"
#define	DEFAULT_PASSWORD_3									"00000000000000000000"
#define	DEFAULT_PASSWORD_4									"00000000000000000000"

#define DEFAULT_LOG_ON_ACCESS_PERM							0 //GROUP_PERM_0_FLAG_bm
#define DEFAULT_PASS_0_ACCESS_PERM							GROUP_PERM_0_FLAG_bm
#define DEFAULT_PASS_1_ACCESS_PERM							GROUP_PERM_0_FLAG_bm
#define DEFAULT_PASS_2_ACCESS_PERM							GROUP_PERM_0_FLAG_bm
#define DEFAULT_PASS_3_ACCESS_PERM							GROUP_PERM_0_FLAG_bm
#define DEFAULT_PASS_4_ACCESS_PERM							GROUP_PERM_0_FLAG_bm

#define	MASTER_PASSWORD										"cel8!#ma6$%to9&$er1/"
#define MASTER_PASS_ACCESS_PERM								(uint8_t)(GROUP_PERM_0_FLAG_bm | GROUP_PERM_1_FLAG_bm | GROUP_PERM_2_FLAG_bm | GROUP_PERM_3_FLAG_bm | GROUP_PERM_4_FLAG_bm | GROUP_PERM_5_FLAG_bm | GROUP_PERM_6_FLAG_bm | GROUP_PERM_7_FLAG_bm )

/* ST_42 Elements size */
#define ST_42_PASSWORD_SIZE									(MAX_PASSWORD_LEN * MAX_NBR_PASSWORDS)
#define ST_42_ACCESS_PERMISSION_BFLD_SIZE					MAX_NBR_PASSWORDS

/*===========================================================================*\
 * ST_43: Default Access Control Table - Default
\*===========================================================================*/
/* DEFAULT_TABLE_INFO SETTINGS */
#define TABLE_ANY_READ_FLAG									0	// 0 = FALSE: Only approved group members may access the associated table or procedure --- 1 = TRUE: Any user may read the associated table or procedure
#define TABLE_ANY_WRITE_FLAG								0	// 0 = FALSE: Only approved group members may access the associated table or procedure --- 1 = TRUE: Any user may read the associated table or procedure

#define DEFAULT_TABLE_INFO									(uint16_t) ( (TABLE_ANY_WRITE_FLAG << 14) | (TABLE_ANY_READ_FLAG << 13) )
#define DEFAULT_TABLE_ACCESS_CTRL_READ						(uint8_t) ( GROUP_PERM_0_FLAG_bm )
#define DEFAULT_TABLE_ACCESS_CTRL_WRITE						(uint8_t) ( GROUP_PERM_0_FLAG_bm )

/* DEFAULT_PROCEDURE_INFO SETTINGS */
#define	PROCEDURE_ANY_READ_FLAG								0	// 0 = FALSE: Only approved group members may access the associated table or procedure --- 1 = TRUE: Any user may read the associated table or procedure
#define	PROCEDURE_ANY_WRITE_FLAG							0	// 0 = FALSE: Only approved group members may access the associated table or procedure --- 1 = TRUE: Any user may read the associated table or procedure

#define DEFAULT_PROCEDURE_INFO								(uint16_t) ( (PROCEDURE_ANY_WRITE_FLAG << 14) | (PROCEDURE_ANY_READ_FLAG << 13) )
#define DEFAULT_PROCEDURE_ACCESS_CTRL_READ					(uint8_t) ( GROUP_PERM_0_FLAG_bm )
#define DEFAULT_PROCEDURE_ACCESS_CTRL_WRITE					(uint8_t) ( GROUP_PERM_0_FLAG_bm )

/* ST_43 Elements size */
#define ST_43_ACCESS_TABLE_DEFAULT_SIZE						2
#define ST_43_TABLE_ACCESS_PERM_BFLD_READ_SIZE				1
#define ST_43_TABLE_ACCESS_PERM_BFLD_WRITE_SIZE				1
#define ST_43_ACCESS_PROCEDURE_DEFAULT_SIZE					2
#define ST_43_PROCEDURE_ACCESS_PERM_BFLD_READ_SIZE			1
#define ST_43_PROCEDURE_ACCESS_PERM_BFLD_WRITE_SIZE			1

/*===========================================================================*\
 * ST_44: Access Control Table - Default
\*===========================================================================*/
/* DEFAULT_ACCESS_TABLE_DEF SETTINGS */
#define TABLE_PROC_NBR										0b11111111111
#define	STD_VS_MFG_FLAG										0					// The table or procedure is standard or manufacturer
#define NUMERIC_ID											0					// The numeric ID is table number or procedure number
#define UNRESTRICTED_READ									0					// 0 = FALSE: Only approved group members may access the associated table or procedure --- 1 = TRUE: Any user may read the associated table or procedure
#define UNRESTRICTED_WRITE									0					// 0 = FALSE: Only approved group members may access the associated table or procedure --- 1 = TRUE: Any user may read the associated table or procedure

#define DEFAULT_ACCESS_TABLE_DEF							(uint16_t) ( (UNRESTRICTED_WRITE << 14) | (UNRESTRICTED_READ << 13) | (NUMERIC_ID << 12) | (STD_VS_MFG_FLAG << 11) | (TABLE_PROC_NBR << 0) )
#define DEFAULT_DEF_CTRL_ACCESS_READ						(uint8_t) ( 0 )
#define DEFAULT_DEF_CTRL_ACCESS_WRITE						(uint8_t) ( 0 )

/* ST_44 Elements size */
#define ST_44_ACCESS_TABLE_DEFAULT_SIZE						(2 * MAX_NBR_PERM_USED)
#define ST_44_TABLE_ACCESS_PERMISSION_BFLD_READ_SIZE		MAX_NBR_PERM_USED
#define ST_44_TABLE_ACCESS_PERMISSION_BFLD_WRITE_SIZE		MAX_NBR_PERM_USED

/*===========================================================================*\
 * C12.19 Decade 4 Tables Size
\*===========================================================================*/
#define ST_40_MAX_SIZE										(ST_40_NBR_PASSWORDS_SIZE + ST_40_PASSWORD_LEN_SIZE + ST_40_NBR_KEYS_SIZE + ST_40_KEY_LEN_SIZE + ST_40_NBR_PERM_USED_SIZE)
#define ST_41_MAX_SIZE										(ST_41_NBR_PASSWORDS_SIZE + ST_41_PASSWORD_LEN_SIZE + ST_41_NBR_KEYS_SIZE + ST_41_KEY_LEN_SIZE + ST_41_NBR_PERM_USED_SIZE)
#define ST_42_MAX_SIZE										(ST_42_PASSWORD_SIZE + ST_42_ACCESS_PERMISSION_BFLD_SIZE)
#define ST_43_MAX_SIZE										(ST_43_ACCESS_TABLE_DEFAULT_SIZE + ST_43_TABLE_ACCESS_PERM_BFLD_READ_SIZE + ST_43_TABLE_ACCESS_PERM_BFLD_WRITE_SIZE + ST_43_ACCESS_PROCEDURE_DEFAULT_SIZE + ST_43_PROCEDURE_ACCESS_PERM_BFLD_READ_SIZE + ST_43_PROCEDURE_ACCESS_PERM_BFLD_WRITE_SIZE)
#define ST_44_MAX_SIZE										(ST_44_ACCESS_TABLE_DEFAULT_SIZE + ST_44_TABLE_ACCESS_PERMISSION_BFLD_READ_SIZE + ST_44_TABLE_ACCESS_PERMISSION_BFLD_WRITE_SIZE)

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs
 ******************************************************************************
 */

/*===========================================================================*\
 * Security user information
\*===========================================================================*/
typedef struct {
	
	uint16_t tbl_proc_id;
	bool mfg_flag;
	char password[MAX_PASSWORD_LEN];
	uint8_t access_permissions;
	
} SECURITY_USER_INFO_TYPE;

/*===========================================================================*\
 * ST_40 & ST_41 structure
\*===========================================================================*/
typedef struct {
	
	uint8_t nbr_passwords;
	uint8_t password_len;
	uint8_t nbr_keys;
	uint8_t key_len;
	uint16_t nbr_perm_used;
	
} ST_40_TYPE;

/*===========================================================================*\
 * ST_42 structures
\*===========================================================================*/

typedef struct {
	
	char password[MAX_PASSWORD_LEN];
	uint8_t access_permissions;
	
	bool is_pass_backed;
	char password_backup[MAX_PASSWORD_LEN];
	uint8_t access_permissions_backup;
	uint16_t user_id_backup;
	uint16_t incorrect_password;
} SECURITY_ENTRY_RCD_TYPE;

typedef	struct {
	
	SECURITY_ENTRY_RCD_TYPE security_entries[MAX_NBR_PASSWORDS];
	
} ST_42_TYPE;

/*===========================================================================*\
 * ST_43 structures
\*===========================================================================*/

typedef struct {
	
	uint16_t access_table_default;
	uint8_t read;
	uint8_t write;
	
} DEFAULT_ACCESS_CONTROL_RCD_TYPE;

typedef struct {
	
	DEFAULT_ACCESS_CONTROL_RCD_TYPE table_default;
	DEFAULT_ACCESS_CONTROL_RCD_TYPE procedure_default;
	
} ST_43_TYPE;

/*===========================================================================*\
 * ST_44 structures
\*===========================================================================*/
typedef struct {
	
	uint16_t access_table_def;
	uint8_t read;
	uint8_t write;
	
} ACCESS_CONTROL_ENTRY_RCD_TYPE;

typedef struct {
	
	ACCESS_CONTROL_ENTRY_RCD_TYPE access_control[MAX_NBR_PERM_USED];

} ST_44_TYPE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
*/
extern ST_40_TYPE st_41;
extern ST_42_TYPE st_42;

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
 */
extern void SECR_APP_vInitialize(void);

extern bool SECR_APP_bCheck_access_to_read_table(SECURITY_USER_INFO_TYPE user_info);
extern bool SECR_APP_bCheck_access_to_write_table(SECURITY_USER_INFO_TYPE user_info);
extern bool SECR_APP_bCheck_access_to_execute_procedure(SECURITY_USER_INFO_TYPE user_info);

extern bool SECR_APP_bSt_40_execute_table_reading(uint8_t *table_buffer);
extern uint16_t SECR_APP_usSt_40_get_size(void);

extern bool SECR_APP_bLoad_st_41(void);
extern bool SECR_APP_bSt_41_restore_default_values(void);
extern bool SECR_APP_bSt_41_refresh_ram(void);
extern bool SECR_APP_bSt_41_execute_table_reading(uint8_t *table_buffer);
extern bool SECR_APP_bSt_41_execute_table_writing(uint8_t *table_buffer);
extern void SECR_APP_vCopy_st_41_nvm(uint8_t* destination);
extern void SECR_APP_vCopy_st_41_ram(uint8_t* destination);
extern void SECR_APP_vSt_41_recover_nvm(void);
extern void SECR_APP_vSt_41_recover_ram(void);
extern uint16_t SECR_APP_usSt_41_get_size(void);

extern bool SECR_APP_bLoad_st_42(void);
extern bool SECR_APP_bSt_42_restore_default_values(void);
extern bool SECR_APP_bSt_42_refresh_ram(void);
extern bool SECR_APP_bSt_42_execute_table_reading(uint8_t *table_buffer);
extern bool SECR_APP_bSt_42_execute_table_writing(uint8_t *table_buffer);
extern void SECR_APP_vCopy_st_42_nvm(uint8_t* destination);
extern void SECR_APP_vCopy_st_42_ram(uint8_t* destination);
extern void SECR_APP_vSt_42_recover_nvm(void);
extern void SECR_APP_vSt_42_recover_ram(void);
extern uint16_t SECR_APP_usSt_42_get_size(void);

extern bool SECR_APP_bLoad_st_43(void);
extern bool SECR_APP_bSt_43_restore_default_values(void);
extern bool SECR_APP_bSt_43_refresh_ram(void);
extern bool SECR_APP_bSt_43_execute_table_writing(uint8_t *table_buffer);
extern bool SECR_APP_bSt_43_execute_table_reading(uint8_t *table_buffer);
extern void SECR_APP_vCopy_st_43_nvm(uint8_t* destination);
extern void SECR_APP_vCopy_st_43_ram(uint8_t* destination);
extern void SECR_APP_vSt_43_recover_nvm(void);
extern void SECR_APP_vSt_43_recover_ram(void);
extern uint16_t SECR_APP_usSt_43_get_size(void);

extern bool SECR_APP_bLoad_st_44(void);
extern bool SECR_APP_bSt_44_restore_default_values(void);
extern bool SECR_APP_bSt_44_refresh_ram(void);
extern bool SECR_APP_bSt_44_execute_table_reading(uint8_t *table_buffer);
extern bool SECR_APP_bSt_44_execute_table_writing(uint8_t *table_buffer);
extern void SECR_APP_vCopy_st_44_nvm(uint8_t* destination);
extern void SECR_APP_vCopy_st_44_ram(uint8_t* destination);
extern void SECR_APP_vSt_44_recover_nvm(void);
extern void SECR_APP_vSt_44_recover_ram(void);
extern uint16_t SECR_APP_usSt_44_get_size(void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-06-09
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */

#endif /* SECR_H_ */