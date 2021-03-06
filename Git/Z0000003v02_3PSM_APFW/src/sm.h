/**
 ******************************************************************************
 * \file sm.h
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

#ifndef SM_H_
#define SM_H_

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "stdint.h"
#include "compiler.h"
#include "util.h"
#include "c1218.h"
#include "gral.h"
#include "secr.h"
#include "pfail.h"
#include "tou.h"
#include "regs.h"
#include "lp.h"
#include "evnt.h"
#include "config_meter.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */
#define NI_FMAT1_SIZE									(uint8_t) 6	//(UINT48)
#define NI_FMAT2_SIZE									(uint8_t) 5	//(UINT40)
#define TM_FMAT_SIZE									(uint8_t) 4	//(UINT32)

#define C1219_TABLE_FULL								0x45
#define C1219_TABLE_EMPTY								0xFF

/*==========================================================================*\
 * IO pin definition
\*===========================================================================*/
#define SM_TEST_BUTTON_PIN							PIO_PA18_IDX
#define SM_TEST_BUTTON_PIO							PIOA
#define SM_TEST_BUTTON_PIO_ID						ID_PIOA
#define SM_TEST_BUTTON_PIN_BIT_MASK					PIO_PA18
#define SM_TEST_BUTTON_IRQn							PIOA_IRQn

#define SM_RESET_BUTTON_PIN							PIO_PB15_IDX
#define SM_RESET_BUTTON_PIO							PIOB
#define SM_RESET_BUTTON_PIO_ID						ID_PIOB
#define SM_RESET_BUTTON_PIN_BIT_MASK				PIO_PB15
#define SM_RESET_BUTTON_IRQn						PIOB_IRQn

#define SM_RESET_BUTTON_PIN							PIO_PB12_IDX
#define SM_RESET_BUTTON_PIO							PIOB
#define SM_RESET_BUTTON_PIO_ID						ID_PIOB
#define SM_RESET_BUTTON_PIN_BIT_MASK				PIO_PB12
#define SM_RESET_BUTTON_IRQn						PIOB_IRQn

/*===========================================================================*\
 * IO reconfigure period
\*===========================================================================*/
#define IO_RECONFIG_PERIOD								(5000/portTICK_RATE_MS)	// 5 sec

/*===========================================================================*\
 * MM task config
\*===========================================================================*/
#define SM_STACK_SIZE									configMINIMAL_STACK_SIZE * 5
#define SM_APP_vTask_PRIO								tskIDLE_PRIORITY												
														
/*---------------------------------------------------------------------------*\
 * C12.19 MANUFACTURER TABLES
 *
 * Tables whose specific content is determined by the device manufacturer. 
 * Active manufacturers tables have a 1 in bit 11 and a 0 in bit 12 of the table
 * identifier. 
 * Manufacturers tables are referenced in the form �MT_n� where �n� is the table 
 * number (not including bits 11 and 12).
\*---------------------------------------------------------------------------*/
										
/*===========================================================================*\
 * MT_1: EOS Specific Product Identification
\*===========================================================================*/
#define MT_1_SIZE										49

#define MT_1_FW_PART_NUMBER_SIZE						8 
#define MT_1_HW_PART_NUMBER_SIZE						8 
#define MT_1_ED_IDENTITY_SIZE							1 
#define MT_1_BOARD_ASSEMBLY_SERIAL_NUMBER_SIZE			16
#define MT_1_MFG_MODULE_SERIAL_NUMBER_SIZE				16
#define MT_1_MAX_SIZE								   (MT_1_FW_PART_NUMBER_SIZE				+ \
														MT_1_HW_PART_NUMBER_SIZE				+ \
														MT_1_ED_IDENTITY_SIZE					+ \
														MT_1_BOARD_ASSEMBLY_SERIAL_NUMBER_SIZE	+ \
														MT_1_MFG_MODULE_SERIAL_NUMBER_SIZE		)

/*===========================================================================*\
 * MT_3: Auxiliary Configuration Table
\*===========================================================================*/
#define MT_3_SIZE										150

#define MT_3_NBR_POWER_OUTAGES_SIZE						4
#define MT_3_RESERVED_SIZE								146
#define MT_3_MAX_SIZE								   (MT_3_NBR_POWER_OUTAGES_SIZE	+ MT_3_RESERVED_SIZE)

/*===========================================================================*\
 * MT_23: Operating Voltage Limits
\*===========================================================================*/
#define MT_23_SIZE										( 8 + (6 * NBR_PHASES) )

#define MT_23_VOLT_LOW_LIMIT_SIZE						4
#define MT_23_VOLT_HIGH_LIMIT_SIZE						4
#define MT_23_MAX_SIZE									(MT_23_VOLT_LOW_LIMIT_SIZE + MT_23_VOLT_HIGH_LIMIT_SIZE)
														
/*===========================================================================*\
 * MT_4: MCU Diagnostics Table
\*===========================================================================*/
#define MT_4_SIZE										40

#define MT_4_MM100_TIME_RUNNING_SIZE					4
#define MT_4_MCU_RESET_TOTAL_COUNTER_SIZE				2
#define MT_4_MCU_RESET_POWER_FAILURE_SIZE				2
#define MT_4_MCU_RESET_FIRMWARE_UPDATE_SIZE			2
#define MT_4_MCU_RESET_WATCH_DOG_SIZE					2
#define MT_4_MCU_RESET_COLD_START_SIZE					2
#define MT_4_MCU_RESET_WARM_START_SIZE					2
#define MT_4_MCU_RESET_KT_CONSTANT_CHANGE_SIZE			2
#define MT_4_MCU_RESET_SYSTEM_FAULT_SIZE				2
#define MT_4_MCU_RESET_VLISTINSERT_COUNTER_SIZE		2
#define MT_4_MCU_RESET_DUMMY_HANDLER_COUNTER_SIZE		2
#define MT_4_MCU_RESET_ASSERT_TRIGGERED_COUNTER_SIZE	2
#define MT_4_MCU_RESET_STACK_OVERFLOW_COUNTER_SIZE		2
#define MT_4_RESERVED_SIZE								12

#define MT_4_MAX_SIZE								    MT_4_SIZE

/*===========================================================================*\
 * Mt_5: Table Integrity Diagnostic
\*===========================================================================*/
#define MT_5_SIZE										16

/*===========================================================================*\
 * MT_22: BCD Format - Phase A Instantaneous Values
\*===========================================================================*/
#define MT_22_SIZE										18 * NBR_PHASES

/*===========================================================================*\
 * MT_53: Time Zone Offset Description
\*===========================================================================*/
#define MT_53_SIZE										72

#define MT_53_WIN_TIMEZONE_INDEX_SIZE					2
#define MT_53_WIN_TIMEZONE_NAME_SIZE					70
#define MT_53_MAX_SIZE									(MT_53_WIN_TIMEZONE_INDEX_SIZE + MT_53_WIN_TIMEZONE_NAME_SIZE)

/*===========================================================================*\
 * MT_230: mm100 Registers
\*===========================================================================*/
#define MT_230_SIZE										84

/*===========================================================================*\
 * MT_24: Accumulated Output Quantities
\*===========================================================================*/
#define MT_24_SIZE										32 * NBR_PHASES

/*===========================================================================*\
 * MT_25: Voltage Register To Calibration
\*===========================================================================*/
#define MT_25_SIZE										24 * NBR_PHASES

/*===========================================================================*\
 * MT_235: Service Relay State
\*===========================================================================*/
#define MT_235_SIZE										7

#define MT_235_RELAY_STATE_SIZE							1
#define MT_235_IS_PENDING_ACTION_SIZE					1
#define MT_235_RELAY_CONTROL_FLAGS_PROG_SIZE			1
#define MT_235_SCHED_TIMESTAMP_PROG_SIZE				4
#define MT_235_DELAY_TIME_PROG_SIZE						1
#define MT_235_RETRIES_PROG_SIZE						1
#define MT_235_MAX_SIZE								   (MT_235_RELAY_STATE_SIZE					+ \
														MT_235_IS_PENDING_ACTION_SIZE			+ \
														MT_235_RELAY_CONTROL_FLAGS_PROG_SIZE	+ \
														MT_235_SCHED_TIMESTAMP_PROG_SIZE		+ \
														MT_235_DELAY_TIME_PROG_SIZE				+ \
														MT_235_RETRIES_PROG_SIZE				)

/*===========================================================================*\
 * MT_202 definition
\*===========================================================================*/
#define MT_202_SIZE										255

/*===========================================================================*\
 * MT-271: Voltage Register To Calibration
\*===========================================================================*/
#define MT_271_SIZE										3

/*===========================================================================*\
 * Definitions
\*===========================================================================*/
#define START_RESET_MS						(uint32_t)TIMER_ms_to_tic(2000) 		// 2 sec
#define DIAGNOSTIC_PROC_MS					(uint32_t)TIMER_ms_to_tic(86400000)		// 24 hrs
#define CHECK_CONFIGURATION_MS_PERIOD		(uint32_t)TIMER_ms_to_tic(86400000)		// 1 hr

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs                          
 ******************************************************************************
 */

/*===========================================================================*\
 * Voltage State Types
\*===========================================================================*/
typedef enum {
	
	VOLTAGE_LOW = 0,
	VOLTAGE_NORMAL,
	VOLTAGE_HIGH
	
} VOLTAGE_STATE_TYPE;

/*===========================================================================*\
 * Meter State Types
\*===========================================================================*/
// typedef enum {
// 	
// 	METER_INITIALIZING = 0,
// 	METER_RUNNING,
// 	METER_LOW_POWER,
// 	METER_BUSY	
// 	
// } SYSTEM_STATE_TYPE;

/*===========================================================================*\
 * Configuration Table State States Type
\*===========================================================================*/
typedef enum {
	
	TBL_DATA_OK = 0,
	TBL_RAM_DATA_CORRUPTED,
	TBL_NVM_DATA_CORRUPTED,
	TBL_NVM_CRC_CORRUPTED,	
	TBL_NVM_RAM_DATA_CORRUPTED

} TABLE_STATE_TYPE;

/*===========================================================================*\
 * Meter Status Type
\*===========================================================================*/
typedef struct {
	
	bool initialiting;
	bool busy;
	bool changing_configuration;
	
} SYSTEM_STATE_TYPE;

/*===========================================================================*\
 * CCG Comm Status Type
\*===========================================================================*/
typedef struct {
	
	bool register_loss_link;
	uint32_t loss_link_timestamp;
	uint8_t comm_status;
	uint16_t comm_timer;
	
} CCG_COSM_STATUS_TYPE;

/*===========================================================================*\
 * Meter Mode Type
\*===========================================================================*/
typedef struct {
	
	bool enabled;
	uint32_t active_period;
	
} MODE_CONFIG_TYPE;

/*===========================================================================*\
 * Meter Mode Type
\*===========================================================================*/
typedef struct {
	
	MODE_CONFIG_TYPE test_mode;
	MODE_CONFIG_TYPE metering;
	MODE_CONFIG_TYPE meter_shop;
	MODE_CONFIG_TYPE factory;
	
} ED_MODES_TYPE;

/*===========================================================================*\
 * Phases State Type
\*===========================================================================*/
typedef struct {
	
	volatile bool failure;
	volatile bool delayed_failure;
	VOLTAGE_STATE_TYPE voltage_state;
	VOLTAGE_STATE_TYPE prev_voltage_state;
	
} PHASE_STATE_TYPE;

/*===========================================================================*\
 * Meter Structure Type
\*===========================================================================*/
typedef struct {
	
	TABLE_STATE_TYPE st_1_state;
	TABLE_STATE_TYPE st_6_state;
	TABLE_STATE_TYPE st_11_state;
	TABLE_STATE_TYPE st_13_state;
	TABLE_STATE_TYPE st_21_state;
	TABLE_STATE_TYPE st_22_state;
	TABLE_STATE_TYPE st_27_state;
	TABLE_STATE_TYPE st_41_state;
	TABLE_STATE_TYPE st_42_state;
	TABLE_STATE_TYPE st_43_state;
	TABLE_STATE_TYPE st_44_state;
	TABLE_STATE_TYPE st_51_state;
	TABLE_STATE_TYPE st_53_state;
	TABLE_STATE_TYPE st_54_state;
	TABLE_STATE_TYPE st_61_state;
	TABLE_STATE_TYPE st_62_state;
	TABLE_STATE_TYPE st_71_state;
	TABLE_STATE_TYPE st_73_state;
	TABLE_STATE_TYPE st_75_state;
	TABLE_STATE_TYPE mt_1_state;
	TABLE_STATE_TYPE mt_3_state;
	TABLE_STATE_TYPE mt_23_state;
	TABLE_STATE_TYPE mt_53_state;
	TABLE_STATE_TYPE mt_73_state;
	
} CONF_TABLE_STATUS_TYPE;

/*===========================================================================*\
 * End Device State Structure Type
\*===========================================================================*/
typedef struct {
	
	uint8_t metrology_over_timer;
	uint32_t time_running;
	CONF_TABLE_STATUS_TYPE config_state;
	SYSTEM_STATE_TYPE system_state;
	ED_MODES_TYPE ed_mode;

	uint32_t voltage_lower_limit;
	uint32_t voltage_upper_limit;
	uint32_t detect_voltage_change_period;
	
	volatile bool power_failure;

	PHASE_STATE_TYPE phase_a;
	PHASE_STATE_TYPE phase_b;
	PHASE_STATE_TYPE phase_c;
	
} ED_STATE_TYPE;

/*===========================================================================*\
 * Meter Structure Type
\*===========================================================================*/
typedef struct {
	
	bool metrology_ready;
	bool test_mode_expired;
	
} METER_OS_TYPE;

/*===========================================================================*\
 * OTA Image Types
\*===========================================================================*/
typedef struct {
	
	uint8_t initiation;
	uint8_t activation;
	uint8_t type;
	uint32_t size;
	char part_number[8];
	uint32_t offset;
	uint32_t ee_segment_start;
	uint32_t ee_segment_end;
	uint32_t ee_segment_size;
	uint8_t new_version;
	uint8_t new_revision;
	uint8_t memory_busy;
	
} OTA_IMAGE_TYPE;

/*===========================================================================*\
 * Diagnostic Data Table Type
\*===========================================================================*/
typedef struct {
	
	uint16_t size;
	uint16_t crc_stored;
	uint16_t crc_ram;
	uint16_t crc_nvm;
	void (*recover_nvm_table)(void);
	void (*recover_ram_table)(void);
	
} DIAGNOSTIC_TABLE_TYPE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
extern ED_STATE_TYPE meter;
extern METER_OS_TYPE meter_os;

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
 */
extern void SM_APP_vCreate_task(void);
extern void SM_APP_vInitialize(void);
extern void SM_APP_vInitialize_system_state(void);
extern void SM_APP_vLoad_c1219_tables(void);

extern void SM_APP_vTrigger_timers(void);
extern void SM_APP_vExecute_diagnostics_procedure(void);

extern void SM_APP_vFwup_mcu_restart(void);
extern void SM_APP_vSet_test_mode_expired_flag(void);

extern void SM_APP_vExecute_mfg_service(C1218_DLL_TYPE *c1218_dll_ptr);

extern uint8_t SM_APP_ucGet_nonvol_mem_state(void);

extern uint8_t SM_APP_ucGet_table_state(uint32_t table_state_addr);
extern void SM_APP_vChange_table_state(uint32_t table_state_addr, uint8_t status);

extern void SM_APP_vLoad_mt_1(void);
extern void SM_APP_vMt_1_restore_default_values(void);
extern bool SM_APP_bMt_1_execute_table_reading(uint8_t *table_buffer);
extern bool SM_APP_bMt_1_execute_table_writing(uint8_t *table_buffer);
extern uint16_t SM_APP_usMt_1_get_size(void);

extern void SM_APP_vLoad_mt_3(void);
extern void SM_APP_vMt_3_restore_default_values(void);
extern bool SM_APP_bMt_3_execute_table_reading(uint8_t *table_buffer);
extern bool SM_APP_bMt_3_execute_table_writing(uint8_t *table_buffer);
extern uint16_t SM_APP_usMt_3_get_size(void);

extern void SM_APP_vLoad_mt_23(void);
extern void SM_APP_vMt_23_load_default_values(void);
extern void SM_APP_vMt_23_read_eeprom_values(void);
extern void SM_APP_vMt_23_write_eeprom_values(void);
extern void SM_APP_vMt_23_restore_default_values(void);
extern bool SM_APP_bMt_23_execute_table_reading(uint8_t *table_buffer);
extern bool SM_APP_bMt_23_execute_table_writing(uint8_t *table_buffer);
extern uint16_t SM_APP_usMt_23_get_size(void);

extern void SM_APP_vLoad_mt_4(void);
extern void SM_APP_vMt_4_restore_default_values(void);
extern bool SM_APP_bMt_4_execute_table_reading(uint8_t *table_buffer);
extern bool SM_APP_bMt_4_execute_table_writing(uint8_t *table_buffer);
extern uint16_t SM_APP_usMt_4_get_size(void);

extern void SM_APP_vLoad_mt_5(void);
extern void SM_APP_vMt_5_restore_default_values(void);
extern bool SM_APP_bMt_5_execute_table_reading(uint8_t *table_buffer);
extern bool SM_APP_bMt_5_execute_table_writing(uint8_t *table_buffer);
extern uint16_t SM_APP_usMt_5_get_size(void);

extern bool SM_APP_bMt_22_execute_table_reading(uint8_t *table_buffer);
extern uint16_t SM_APP_usMt_22_get_size(void);

extern void SM_APP_vLoad_mt_53(void);
extern void SM_APP_vMt_53_restore_default_values(void);
extern bool SM_APP_bMt_53_execute_table_reading(uint8_t *table_buffer);
extern bool SM_APP_bMt_53_execute_table_writing(uint8_t *table_buffer);
extern uint16_t SM_APP_usMt_53_get_size(void);

extern bool SM_APP_bMt_230_execute_table_reading(uint8_t *table_buffer);
extern uint16_t SM_APP_usMt_230_get_size(void);

extern bool SM_APP_bMt_24_execute_table_reading(uint8_t *table_buffer);
extern uint16_t SM_APP_usMt_24_get_size(void);

extern bool SM_APP_bMt_25_execute_table_reading(uint8_t *table_buffer);
extern uint16_t SM_APP_usMt_25_get_size(void);

extern bool SM_APP_vIs_st_11_corrupted_data(void);
extern uint16_t SM_APP_usSt_11_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_11_ram_calculate_crc(void);
extern void SM_APP_vSt_11_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_11_read_crc(void);

extern bool SM_APP_vIs_st_21_corrupted_data(void);
extern uint16_t SM_APP_usSt_21_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_21_ram_calculate_crc(void);
extern void SM_APP_vSt_21_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_21_read_crc(void);

extern bool SM_APP_vIs_st_22_corrupted_data(void);
extern uint16_t SM_APP_usSt_22_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_22_ram_calculate_crc(void);
extern void SM_APP_vSt_22_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_22_read_crc(void);

extern bool SM_APP_vIs_st_27_corrupted_data(void);
extern uint16_t SM_APP_usSt_27_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_27_ram_calculate_crc(void);
extern void SM_APP_vSt_27_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_27_read_crc(void);

extern bool SM_APP_vIs_st_41_corrupted_data(void);
extern uint16_t SM_APP_usSt_41_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_41_ram_calculate_crc(void);
extern void SM_APP_vSt_41_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_41_read_crc(void);

extern bool SM_APP_vIs_st_42_corrupted_data(void);
extern uint16_t SM_APP_usSt_42_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_42_ram_calculate_crc(void);
extern void SM_APP_vSt_42_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_42_read_crc(void);

extern bool SM_APP_vIs_st_43_corrupted_data(void);
extern uint16_t SM_APP_usSt_43_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_43_ram_calculate_crc(void);
extern void SM_APP_vSt_43_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_43_read_crc(void);

extern bool SM_APP_vIs_st_44_corrupted_data(void);
extern uint16_t SM_APP_usSt_44_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_44_ram_calculate_crc(void);
extern void SM_APP_vSt_44_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_44_read_crc(void);

extern bool SM_APP_vIs_st_51_corrupted_data(void);
extern uint16_t SM_APP_usSt_51_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_51_ram_calculate_crc(void);
extern void SM_APP_vSt_51_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_51_read_crc(void);

extern bool SM_APP_vIs_st_61_corrupted_data(void);
extern uint16_t SM_APP_usSt_61_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_61_ram_calculate_crc(void);
extern void SM_APP_vSt_61_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_61_read_crc(void);

extern bool SM_APP_vIs_st_62_corrupted_data(void);
extern uint16_t SM_APP_usSt_62_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_62_ram_calculate_crc(void);
extern void SM_APP_vSt_62_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_62_read_crc(void);

extern bool SM_APP_vIs_st_71_corrupted_data(void);
extern uint16_t SM_APP_usSt_71_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_71_ram_calculate_crc(void);
extern void SM_APP_vSt_71_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_71_read_crc(void);

extern bool SM_APP_vIs_st_73_corrupted_data(void);
extern uint16_t SM_APP_usSt_73_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_73_ram_calculate_crc(void);
extern void SM_APP_vSt_73_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_73_read_crc(void);

extern bool SM_APP_vIs_st_75_corrupted_data(void);
extern uint16_t SM_APP_usSt_75_nvm_calculate_crc(void);
extern uint16_t SM_APP_usSt_75_ram_calculate_crc(void);
extern void SM_APP_vSt_75_write_crc(uint16_t crc);
extern uint16_t SM_APP_usSt_75_read_crc(void);

extern bool SM_APP_vIs_mt_73_corrupted_data(void);
extern uint16_t SM_APP_usMt_73_nvm_calculate_crc(void);
extern uint16_t SM_APP_usMt_73_ram_calculate_crc(void);
extern void SM_APP_vMt_73_write_crc(uint16_t crc);
extern uint16_t SM_APP_usMt_73_read_crc(void);

extern bool SM_APP_bBack_up_st_1(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_1_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_6(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_6_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_11(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_11_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_13(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_13_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_21(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_21_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_22(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_22_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_27(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_27_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_41(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_41_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_42(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_42_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_43(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_43_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_44(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_44_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_51(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_51_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_53(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_53_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_54(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_54_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_61(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_61_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_62(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_62_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_71(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_71_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_73(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_73_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_st_75(uint16_t backup_area_id);
extern bool SM_APP_bRestore_st_75_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_mt_1(uint16_t backup_area_id);
extern bool SM_APP_bRestore_mt_1_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_mt_3(uint16_t backup_area_id);
extern bool SM_APP_bRestore_mt_3_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_mt_23(uint16_t backup_area_id);
extern bool SM_APP_bRestore_mt_23_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_mt_53(uint16_t backup_area_id);
extern bool SM_APP_bRestore_mt_53_from_backup(uint16_t backup_area_id);

extern bool SM_APP_bBack_up_mt_73(uint16_t backup_area_id);
extern bool SM_APP_bRestore_mt_73_from_backup(uint16_t backup_area_id);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */

#endif /* SM_H_ */
