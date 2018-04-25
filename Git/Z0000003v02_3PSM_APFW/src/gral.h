/**
 ******************************************************************************
 * \file gral.h
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

#ifndef GRAL_H_
#define GRAL_H_

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "stdint.h"
#include "compiler.h"
#include "evnt.h"
   
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/

/*===========================================================================*\
 * Maximum number of octets 
\*===========================================================================*/
#define MAX_NBR_OCTETS_SET							40

/*===========================================================================*\
 * ST_0: General Configuration Table (default values)
\*===========================================================================*/
/* FORMAT_CONTROL_1 SETTINGS */
#define DATA_ORDER									0
#define CHAR_FORMAT									1
#define MODEL_SELECT								0

/* FORMAT_CONTROL_2 SETTINGS */
#define TM_FORMAT									4
#define DATA_ACCESS_METHOD							1
#define ID_FORM										0
#define INT_FORMAT									0

/* FORMAT_CONTROL_3 SETTINGS */
#define NI_FORMAT1									10
#define NI_FORMAT2									9

#define FORMAT_CONTROL_1							(uint8_t) ( (MODEL_SELECT << 4) | (CHAR_FORMAT << 1) | (DATA_ORDER << 0) )
#define FORMAT_CONTROL_2							(uint8_t) ( (INT_FORMAT << 6) | (ID_FORM << 5) | (DATA_ACCESS_METHOD << 3) | (TM_FORMAT << 0) )
#define FORMAT_CONTROL_3							(uint8_t) ( (NI_FORMAT2 << 4) | (NI_FORMAT1 << 0) )
#define MANUFACTURER_GRAL							MENUFACTURER_ID
#define NAMEPLATE_TYPE								(uint8_t) 2
#define DEFAULT_SET_USED							(uint8_t) 0
#define MAX_PROC_PARM_LENGTH						(uint8_t) 19
#define MAX_RESP_DATA_LEN							(uint8_t) 24
#define STD_VERSION_NO								(uint8_t) 1
#define STD_REVISION_NO								(uint8_t) 0
#define DIM_STD_TBLS_USED							( GRAL_APP_ucGet_dim_std_tbls_used() )
#define DIM_MFG_TBLS_USED							( GRAL_APP_ucGet_dim_mfg_tbls_used() )
#define DIM_STD_PROC_USED							( GRAL_APP_ucGet_dim_std_proc_used() )
#define DIM_MFG_PROC_USED							( GRAL_APP_ucGet_dim_mfg_proc_used() )
#define DIM_MFG_STATUS_USED							(uint8_t) MAX_NBR_STD_EVENTS + MAX_NBR_MFG_EVENTS
#define NBR_PENDING									(uint8_t) 6

/*===========================================================================*\
 * ST_2: Device Nameplate Table (default values)
\*===========================================================================*/
/*E_ELEMENTS SETTINGS*/
#define E_FREQ										4
#define E_NO_OF_ELEMENTS							1
#define E_BASE_TYPE									9
#define E_ACCURACY_CLASS							0

/*E_VOLTS SETTINGS*/
#define E_ELEMENTS_VOLTS							3
#define E_ED_SUPPLY_VOLTS							3

#define E_KT_01										"0.3   "
#define E_KT_1										"3.0   "
#define E_KT_EMPTY									"      "

#define E_INPUT_SCALAR								(uint8_t) 1
#define E_ED_CONFIG									"     "
#define E_ELEMENTS									(uint16_t) ( (E_ACCURACY_CLASS << 10) | (E_BASE_TYPE << 6) | (E_NO_OF_ELEMENTS << 3) | (E_FREQ << 0) )
#define E_VOLTS										(uint8_t) ( (E_ED_SUPPLY_VOLTS << 4) | (E_ELEMENTS_VOLTS << 0) )
#define E_CLASS_MAX_AMPS							"200   "
#define E_TA										"30    "

/*===========================================================================*\
 * ST_3: End Device Mode Status Table
\*===========================================================================*/

/* ED_MODE BIT MASK */
#define METERING_FLAG_bm							0b00000001
#define METERING_FLAG_bp							0	
#define TEST_MODE_FLAG_bm							0b00000010
#define TEST_MODE_FLAG_bp							1	
#define METER_SHOP_MODE_FLAG_bm						0b00000100
#define METER_SHOP_MODE_FLAG_bp						2
#define FACTORY_FLAG_bm								0b00001000
#define FACTORY_FLAG_bp								3

/* ED_STD_STATUS1 BIT MASK */
#define UNPROGRAMMED_FLAG_bm						0b0000000000000001
#define UNPROGRAMMED_FLAG_bp						0
#define CONFIGURATION_ERROR_FLAG_bm					0b0000000000000010
#define CONFIGURATION_ERROR_FLAG_bp					1
#define SELF_CHK_ERROR_FLAG_bm						0b0000000000000100
#define SELF_CHK_ERROR_FLAG_bp						2
#define RAM_FAILURE_FLAG_bm							0b0000000000001000
#define RAM_FAILURE_FLAG_bp							3
#define ROM_FAILURE_FLAG_bm							0b0000000000010000
#define ROM_FAILURE_FLAG_bp							4
#define NONVOL_MEM_FAILURE_FLAG_bm					0b0000000000100000
#define NONVOL_MEM_FAILURE_FLAG_bp					5
#define CLOCK_ERROR_FLAG_bm							0b0000000001000000
#define CLOCK_ERROR_FLAG_bp							6
#define MEASUREMENT_ERROR_FLAG_bm					0b0000000010000000
#define MEASUREMENT_ERROR_FLAG_bp					7
#define LOW_BATTERY_FLAG_bm							0b0000000100000000
#define LOW_BATTERY_FLAG_bp							8
#define LOW_LOSS_POTENTIAL_FLAG_bm					0b0000001000000000
#define LOW_LOSS_POTENTIAL_FLAG_bp					9
#define DEMAND_OVERLOAD_FLAG_bm						0b0000010000000000
#define DEMAND_OVERLOAD_FLAG_bp						10
#define POWER_FAILURE_FLAG_bm						0b0000100000000000
#define POWER_FAILURE_FLAG_bp						11
#define TAMPER_DETECT_FLAG_bm						0b0001000000000000
#define TAMPER_DETECT_FLAG_bp						12
#define REVERSE_ROTATION_FLAG_bm					0b0010000000000000
#define REVERSE_ROTATION_FLAG_bp					13

#define ED_STD_STATUS1_SUPPORTED_FLAGS_bm			( UNPROGRAMMED_FLAG_bm | CONFIGURATION_ERROR_FLAG_bm | ROM_FAILURE_FLAG_bm | NONVOL_MEM_FAILURE_FLAG_bm | MEASUREMENT_ERROR_FLAG_bm | DEMAND_OVERLOAD_FLAG_bm | POWER_FAILURE_FLAG_bm | REVERSE_ROTATION_FLAG_bm )

#define ATSENSE_FAIL_BIT_bm							0x00000100
#define DSP_IS_RUNNING								0x00000008
#define METROLOGY_IS_RUNNING						0x00000002
#define ATSENSE_IS_OK								0x00
#define ATSENSE_IS_FAILURE							0x00000080

/*===========================================================================*\
 * ST_4
\*===========================================================================*/
/*EVEN_SELECTOR Bit Mask*/
#define EVENT_CODE_bm								0b00001111
#define	SELF_READ_FLAG_ST4_bm						0b00010000
#define DEMAND_RESET_FLAG_ST4_bm					0b00100000

/*TABLE_SELECTOR Bit Mask*/
#define TBL_PROC_NUMBER_bm							0b0000011111111111
#define MFG_FLAFG_bm								0b0000100000000000
#define PENDING_FLAG_bm								0b0001000000000000
#define EUDT_FLAG_bm								0b0010000000000000
#define FLAG_2_bm									0b0100000000000000
#define FLAG_3_bm									0b1000000000000000

/*===========================================================================*\
 * C12.19 Decade 0 Tables Size
\*===========================================================================*/

/* ST_0 Elements size */
#define ST_0_SIZE									124

/* ST_1 Elements size */
#define ST_1_MANUFACTURER_SIZE						4
#define ST_1_ED_MODEL_SIZE							8
#define ST_1_HW_VERSION_NUMBER_SIZE					1
#define ST_1_HW_REVISION_NUMBER_SIZE				1
#define ST_1_FW_VERSION_NUMBER_SIZE					1
#define ST_1_FW_REVISION_NUMBER_SIZE				1
#define ST_1_SERIAL_NUMBER_SIZE						16
#define ST_1_SIZE									32

/* ST_2 Elements size */
#define ST_2_SIZE									33

/* ST_3 Elements size */
#define ST_3_ED_MODE_SIZE							1
#define ST_3_ED_STD_STATUS1_SIZE					2
#define ST_3_ED_STD_STATUS2_SIZE					1
#define ST_3_ED_MFG_STATUS_SIZE						DIM_MFG_STATUS_USED
#define ST_3_SIZE									ST_3_ED_MODE_SIZE						+ \
													ST_3_ED_STD_STATUS1_SIZE				+ \
													ST_3_ED_STD_STATUS2_SIZE				+ \
													ST_3_ED_MFG_STATUS_SIZE

/* ST_6 Elements size */
#define ST_6_OWNER_NAME_SIZE						20
#define ST_6_UTILITY_DIV_SIZE						20
#define ST_6_SERVICE_POINT_ID_SIZE					20
#define ST_6_ELECT_ADDR_SIZE						20
#define ST_6_DEVICE_ID_SIZE							20
#define ST_6_UTIL_SER_NO_SIZE						20
#define ST_6_CUSTOMER_ID_SIZE						20
#define ST_6_COORDINATE_1_SIZE						10
#define ST_6_COORDINATE_2_SIZE						10
#define ST_6_COORDINATE_3_SIZE						10
#define ST_6_TARIFF_ID_SIZE							8
#define ST_6_EX1_SW_VENDOR_SIZE						4
#define ST_6_EX1_SW_VERSION_NUMBER_SIZE				1
#define ST_6_EX1_SW_REVISION_NUMBER_SIZE			1
#define ST_6_EX2_SW_VENDOR_SIZE						4
#define ST_6_EX2_SW_VERSION_NUMBER_SIZE				1
#define ST_6_EX2_SW_REVISION_NUMBER_SIZE			1
#define ST_6_PROGRAMMER_NAME_SIZE					10
#define ST_6_MISC_ID_SIZE							30
#define ST_6_FW_LOAD_DATE_SIZE						TM_FMAT_SIZE
#define ST_6_SIZE									ST_6_OWNER_NAME_SIZE					+ \
													ST_6_UTILITY_DIV_SIZE					+ \
													ST_6_SERVICE_POINT_ID_SIZE				+ \
													ST_6_ELECT_ADDR_SIZE					+ \
													ST_6_DEVICE_ID_SIZE						+ \
													ST_6_UTIL_SER_NO_SIZE					+ \
													ST_6_CUSTOMER_ID_SIZE					+ \
													ST_6_COORDINATE_1_SIZE					+ \
													ST_6_COORDINATE_2_SIZE					+ \
													ST_6_COORDINATE_3_SIZE					+ \
													ST_6_TARIFF_ID_SIZE						+ \
													ST_6_EX1_SW_VENDOR_SIZE					+ \
													ST_6_EX1_SW_VERSION_NUMBER_SIZE			+ \
													ST_6_EX1_SW_REVISION_NUMBER_SIZE		+ \
													ST_6_EX2_SW_VENDOR_SIZE					+ \
													ST_6_EX2_SW_VERSION_NUMBER_SIZE			+ \
													ST_6_EX2_SW_REVISION_NUMBER_SIZE		+ \
													ST_6_PROGRAMMER_NAME_SIZE				+ \
													ST_6_MISC_ID_SIZE						

/* ST_7 Elements size */
#define ST_7_MAX_SIZE		32

/* ST_8 Elements size */
#define ST_8_MAX_SIZE		32

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
*/

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs
 ******************************************************************************
*/
/*===========================================================================*\
 * ST-3: End Device Mode Status Table Structure
\*===========================================================================*/
typedef enum {
	
	UNPROGRAMMED_FLAG = 0,					
	CONFIGURATION_ERROR_FLAG,		
	SELF_CHK_ERROR_FLAG,					
	RAM_FAILURE_FLAG,							
	ROM_FAILURE_FLAG,							
	NONVOL_MEM_FAILURE_FLAG,					
	CLOCK_ERROR_FLAG,					
	MEASUREMENT_ERROR_FLAG,					
	LOW_BATTERY_FLAG,					
	LOW_LOSS_POTENTIAL_FLAG,					
	DEMAND_OVERLOAD_FLAG,						
	POWER_FAILURE_FLAG,						
	TAMPER_DETECT_FLAG,						
	REVERSE_ROTATION_FLAG	
					
} ED_STD_STATUS1_FLAGS_TYPE;

/*===========================================================================*\
 * ST-3: End Device Mode Status Table Structure
\*===========================================================================*/
typedef struct {
	
	uint8_t ed_mode;
	uint16_t ed_std_status1;
	uint8_t ed_std_status2;
	uint8_t ed_mfg_status[DIM_MFG_STATUS_USED];
	
} ST_3_TYPE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
 extern ST_3_TYPE st_3;

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/
extern bool GRAL_APP_bSt_0_execute_table_reading(uint8_t *table_buffer);
extern uint16_t GRAL_APP_usSt_0_get_size(void);

extern bool GRAL_APP_bLoad_st_1(void);
extern bool GRAL_APP_bSt_1_restore_default_values(void);
extern bool GRAL_APP_bSt_1_execute_table_writing(uint8_t *table_buffer);
extern bool GRAL_APP_bSt_1_execute_table_reading(uint8_t *table_buffer);
extern uint16_t GRAL_APP_usSt_1_get_size(void);

extern bool GRAL_APP_bSt_2_execute_table_reading(uint8_t *table_buffer);
extern uint16_t GRAL_APP_usSt_2_get_size(void);

extern bool GRAL_APP_bLoad_st_3(void);
extern bool GRAL_APP_bSt_3_restore_default_values(void);
extern bool GRAL_APP_bSt_3_refresh_ram(void);
extern bool GRAL_APP_bSt_3_execute_table_reading(uint8_t *table_buffer);
extern uint16_t GRAL_APP_usSt_3_get_size(void);

extern bool GRAL_APP_bLoad_st_6(void);
extern bool GRAL_APP_bSt_6_restore_default_values(void);
extern bool GRAL_APP_bSt_6_execute_table_writing(uint8_t *table_buffer);
extern bool GRAL_APP_bSt_6_execute_table_reading(uint8_t *table_buffer);
extern uint16_t GRAL_APP_usSt_6_get_size(void);

extern bool GRAL_APP_bEnable_metering_mode(uint16_t user_id, Uart* uart);
extern bool GRAL_APP_bEnable_test_mode(uint16_t user_id, Uart* uart);
extern bool GRAL_APP_bEnable_meter_shop_mode(uint16_t user_id);
extern bool GRAL_APP_bEnable_factory_mode(void);
extern bool GRAL_APP_bDisable_factory_mode(void);

extern uint8_t GRAL_APP_ucGet_st_3_ed_mode(void);
extern uint16_t GRAL_APP_usGet_st_3_ed_std_status1(void);
extern uint8_t GRAL_APP_ucGet_st_3_ed_std_status2(void);
extern void GRAL_APP_vCopy_st_3_ed_mfg_status(uint8_t *buffer);

extern void GRAL_APP_vClear_st_3_ed_std_status1(void);
extern void GRAL_APP_vClear_st_3_ed_std_status2(void);
extern void GRAL_APP_vClear_st_3_ed_mfg_status(void);

extern void GRAL_APP_vStore_st_3_ed_std_status1(void);
extern void GRAL_APP_vStore_st_3_ed_std_status2(void);
extern void GRAL_APP_vStore_st_3_ed_mfg_status(void);

extern void GRAL_APP_vSet_flag_st_3_ed_std_status1(ED_STD_STATUS1_FLAGS_TYPE flag);
extern void GRAL_APP_vReset_flag_st_3_ed_std_status1(ED_STD_STATUS1_FLAGS_TYPE flag);

extern void GRAL_APP_vLoad_std_tbls_used(uint8_t* set_buff);
extern void GRAL_APP_vLoad_mfg_tbls_used(uint8_t* set_buff);
extern void GRAL_APP_vLoad_std_proc_used(uint8_t* set_buff);
extern void GRAL_APP_vLoad_mfg_proc_used(uint8_t* set_buff);
extern void GRAL_APP_vLoad_std_tbls_write(uint8_t* set_buff);
extern void GRAL_APP_vLoad_mfg_tbls_write(uint8_t* set_buff);
extern inline uint8_t GRAL_APP_ucGet_dim_std_tbls_used(void);
extern inline uint8_t GRAL_APP_ucGet_dim_mfg_tbls_used(void);
extern inline uint8_t GRAL_APP_ucGet_dim_std_proc_used(void);
extern inline uint8_t GRAL_APP_ucGet_dim_mfg_proc_used(void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-08-13
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
#endif /* GRAL_H_ */