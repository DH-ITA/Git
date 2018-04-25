/**
 ******************************************************************************
 * \file mm.c
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

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "sm.h"
#include "crc.h"
#include "gral.h"
#include "regs.h"
#include "secr.h"
#include "tou.h"
#include "lp.h"
#include "evnt.h"
#include "rtc_driver.h"
#include "c1218.h"
#include "op.h"
#include "nic.h"
#include "timer.h"
#include "flash.h"
#include "flash_efc.h"
#include "eeprom.h"
#include "led.h"
#include "rst.h"
#include "pfail.h"
#include "watchdog.h"
#include "rst.h"
#include "c1219.h"
#include "led.h"
#include "system.h"
#include "i2c.h"

/*
 ******************************************************************************
 * File level pragmas                                                
 ******************************************************************************
 */ 

/*
 ******************************************************************************
 * Constant and Macro Definitions using #define
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
 */
static DIAGNOSTIC_TABLE_TYPE cfg_table;
static uint8_t tbl_array[MAX_CONFIGURATION_TABLE_SIZE];
static uint8_t temp_table_backup[MAX_CONFIGURATION_TABLE_SIZE];

/*
 ************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ************************************************************************
 */
ED_STATE_TYPE meter;
METER_OS_TYPE meter_os;

/*
 ******************************************************************************
 * ROM Const Variables With File Level Scope
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Static Function Prototypes for Private Functions with File Level Scope
 ******************************************************************************
 */
static void SM_APP_vTask(void *pvParameters);
static void SM_APP_vIo_reconfig_task(void);
static void SM_APP_vDiagnostics_task(void);
static void SM_APP_vVoltage_monitor_task(void);

static void SM_APP_vIo_configure(void);
static void SM_APP_vCheck_firmware_version(void);

static uint8_t SM_APP_ucCheck_table_ram_vs_nvm(DIAGNOSTIC_TABLE_TYPE table);

static bool SM_APP_bIs_ed_unprogrammed(void);
static bool SM_APP_bIs_ed_configuration_error(void);
static bool SM_APP_bIs_ed_self_chk_error(void);
static bool SM_APP_bIs_ed_ram_failure(void);
static bool SM_APP_bIs_ed_rom_failure(void);
static bool SM_APP_bIs_ed_nonvol_mem_failure(void);
static bool SM_APP_bIs_ed_clock_error(void);
static bool SM_APP_bIs_ed_measurement_error(void);
static bool SM_APP_bIs_ed_low_battery(void);
static bool SM_APP_bIs_ed_low_loss_potential(void);
static bool SM_APP_bIs_ed_demand_overload(void);
static bool SM_APP_bIs_ed_power_failure(void);
static bool SM_APP_bIs_ed_tamper_detect(void);
static bool SM_APP_bIs_ed_reverse_rotation(void);

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */

/**
 ******************************************************************************
 * \fn void SM_APP_vInitialize(void)
 * \brief Modular Meter initialization
 *
 * \param 
 * \retval 
 ******************************************************************************
 */
void SM_APP_vInitialize(void)
{	
	SM_APP_vIo_configure();
	
	meter.phase_a.voltage_state = VOLTAGE_NORMAL;
	meter.phase_b.voltage_state = VOLTAGE_NORMAL;
	meter.phase_c.voltage_state = VOLTAGE_NORMAL;

	meter.phase_a.prev_voltage_state = VOLTAGE_NORMAL;
	meter.phase_b.prev_voltage_state = VOLTAGE_NORMAL;
	meter.phase_c.prev_voltage_state = VOLTAGE_NORMAL;
	
	/* Initialize Metering Mode */
	meter.ed_mode.metering.enabled = true;
	meter.ed_mode.metering.active_period = 0;											// Active period to metering mode is undefined
	
	/* Initialize Meter Shop Mode */
	meter.ed_mode.meter_shop.enabled = false;
	meter.ed_mode.meter_shop.active_period = 0;											// Meter shop mode is not supported
	
	/* Initialize Test Mode */
	meter.ed_mode.test_mode.enabled = false;
	meter.ed_mode.test_mode.active_period = (uint32_t)TIMER_ms_to_tic(1800000);			// Active period to test mode
	
	/* Initialize Factory Mode */
	meter.ed_mode.factory.enabled = false;
	meter.ed_mode.factory.active_period = (uint32_t)TIMER_ms_to_tic(1800000);			// Active period to factory or manufacturer mode
	
	meter.detect_voltage_change_period = (uint32_t)TIMER_ms_to_tic(60000);				// 1 min
	
	meter_os.test_mode_expired = false;
}

/**
 ******************************************************************************
 * \fn void SM_APP_vInitialize_system_state(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vInitialize_system_state(void) {

	meter.system_state.changing_configuration = false;
	meter.system_state.busy = false;
	meter.system_state.initialiting = true;
}

/**
 ******************************************************************************
 * \fn void SM_APP_vCheck_configuration_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vTrigger_timers(void) {
	
	/* Initialize timer to auto diagnostic */
	TIMER_start_ms_timer( DIAGNOSTICS_EXECUTION_TIMER, (uint32_t)TIMER_ms_to_tic(20000), null_action );
	
	/* Initialize timer to check configuration */
	TIMER_start_ms_timer( CONFIGURATION_ERROR_TIMER, (uint32_t)TIMER_ms_to_tic(20000), null_action );
	
	/* Initialize io reconfigure timer */
	TIMER_start_ms_timer( IO_MCU_CONFIG_TIMER, TIMER_ms_to_tic(5000), null_action );

	/* Metrology check */
	TIMER_start_ms_timer( METROLOGY_CHECK_TIMER, TIMER_ms_to_tic(1000), null_action );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vIo_configure(void)
 * \brief MM I/O Port Registers Configuration
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vIo_configure(void)
{	
	
}

/**
 ******************************************************************************
 * \fn void SM_APP_vCreate_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vCreate_task(void)
{
	/* Create MM refresh task */
	xTaskCreate( SM_APP_vTask, (const signed char * const ) "SM_APP_vTask", SM_STACK_SIZE, NULL, SM_APP_vTask_PRIO, NULL );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vTask(void *pvParameters)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vTask(void *pvParameters) {
	
	UNUSED( pvParameters );
	
	for ( ;; ) {

		if ( !meter.power_failure ) {
				
			/* Check if test mode is finished */
			if ( meter_os.test_mode_expired ) {
				
				meter_os.test_mode_expired = false;
				Uart *uart = NULL;
				GRAL_APP_bEnable_metering_mode(END_DEVICE, uart);
			}
			
			/* Register abnormally events associated with User Session by rear optical port */
			if ( nic_c1218_apl.logon_session_expired ) {
				
				nic_c1218_apl.logon_session_expired = false;
				
				EVNT_APP_vRegister_event(COMMUNICATION_TERMINATED_ABNORMALLY, 0, true, END_DEVICE, rtc.utc_timestamp);
				EVNT_APP_vRegister_event(COMMUNICATION_TERMINATED_ABNORMALLY_NIC, 0, false, END_DEVICE, rtc.utc_timestamp);
				
				if ( nic_security.incorrect_password > 0 ) {
					
					EVNT_APP_vRegister_event(INCORRECT_PASSWORD_NIC, 0, false, nic_c1218_apl.user_id, rtc.utc_timestamp);
					nic_security.incorrect_password = 0;
				}
			}
			
			/* Register abnormally events associated with User Session by frontal optical port */
			if ( op_c1218_apl.logon_session_expired ) {
				
				op_c1218_apl.logon_session_expired = false;
				
				EVNT_APP_vRegister_event(COMMUNICATION_TERMINATED_ABNORMALLY, 0, true, END_DEVICE, rtc.utc_timestamp);
				EVNT_APP_vRegister_event(COMMUNICATION_TERMINATED_ABNORMALLY_OP, 0, false, END_DEVICE, rtc.utc_timestamp);
				
				if ( op_security.incorrect_password > 0 ) {
					
					EVNT_APP_vRegister_event(INCORRECT_PASSWORD_OP, 0, false, op_c1218_apl.user_id, rtc.utc_timestamp);
					op_security.incorrect_password = 0;
				}
			}
			
			/* IO Reconfigure Task */
			SM_APP_vIo_reconfig_task();
			WATCHDOG_APP_vRestart();
			
			/* Check voltage levels */
			SM_APP_vVoltage_monitor_task();
			WATCHDOG_APP_vRestart();
			
			/* Diagnostics & Self Check Task */
			SM_APP_vDiagnostics_task();
			WATCHDOG_APP_vRestart();

			/* Metrology Check Task */
			METROLOGY_APP_vCheck_task();
			WATCHDOG_APP_vRestart();

			/* Meter Phases Status Check Task */
			PFAIL_APP_v3ph_failure_detection();
			WATCHDOG_APP_vRestart();
		}
	}
}

/**
 ******************************************************************************
 * \fn void SM_APP_vLoad_c1219_tables(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vLoad_c1219_tables(void) {
	
	/* Initialize and load C1219 tables */
	SM_APP_vCheck_firmware_version();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	GRAL_APP_bLoad_st_1();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	GRAL_APP_bLoad_st_3();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	GRAL_APP_bLoad_st_6();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vLoad_mt_1();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vLoad_mt_3();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vLoad_mt_23();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vLoad_mt_4();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vLoad_mt_5();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vLoad_mt_53();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	 
	REGS_APP_bLoad_st_11();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_11_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	REGS_APP_bLoad_st_13();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	REGS_APP_bLoad_st_21();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_21_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	REGS_APP_bLoad_st_22();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_22_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	REGS_APP_bLoad_st_23();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
 	REGS_APP_bLoad_st_26();
 	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	 
	REGS_APP_bLoad_st_27();
 	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_27_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SECR_APP_bLoad_st_41();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_41_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SECR_APP_bLoad_st_42();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_42_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SECR_APP_bLoad_st_43();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_43_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SECR_APP_bLoad_st_44();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_44_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	TOU_APP_bLoad_st_51();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_51_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	TOU_APP_bLoad_st_53();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	LP_APP_bLoad_st_61();	
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_61_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	LP_APP_bLoad_st_62();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_62_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	LP_APP_bLoad_st_63();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	LP_APP_vLoad_st_64_eeprom_addr();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	EVNT_APP_bLoad_st_71();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_71_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	EVNT_APP_bLoad_st_73();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_73_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	EVNT_APP_bLoad_st_74();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	EVNT_APP_bLoad_st_75();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	SM_APP_vIs_st_75_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	EVNT_APP_bLoad_st_76();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();

	EVNT_APP_bLoad_mt_73();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();

	SM_APP_vIs_mt_73_corrupted_data();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
}

/**
 ******************************************************************************
 * \fn void SM_APP_vExecute_diagnostics_procedure(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vExecute_diagnostics_procedure(void) {
	
	if ( SM_APP_bIs_ed_unprogrammed() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(UNPROGRAMMED_FLAG);
	}
	
	if ( SM_APP_bIs_ed_configuration_error() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(CONFIGURATION_ERROR_FLAG);
	}
	
	if ( SM_APP_bIs_ed_self_chk_error() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(SELF_CHK_ERROR_FLAG);
	}
	
	if ( SM_APP_bIs_ed_ram_failure() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(RAM_FAILURE_FLAG);
	}
	
	if ( SM_APP_bIs_ed_rom_failure() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(ROM_FAILURE_FLAG);
	}
	
	if ( SM_APP_bIs_ed_nonvol_mem_failure() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(NONVOL_MEM_FAILURE_FLAG);
	}
	
	if ( SM_APP_bIs_ed_clock_error() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(CLOCK_ERROR_FLAG);
	}
	
	if ( SM_APP_bIs_ed_measurement_error() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(MEASUREMENT_ERROR_FLAG);
	}
	
	if ( SM_APP_bIs_ed_low_battery() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(LOW_BATTERY_FLAG);
	}
	
	if ( SM_APP_bIs_ed_low_loss_potential() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(LOW_LOSS_POTENTIAL_FLAG);
	}
	
	if ( SM_APP_bIs_ed_demand_overload() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(DEMAND_OVERLOAD_FLAG);
	}
	
	if ( SM_APP_bIs_ed_power_failure() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(POWER_FAILURE_FLAG);
	}
	
	if ( SM_APP_bIs_ed_tamper_detect() ) {
		GRAL_APP_vSet_flag_st_3_ed_std_status1(TAMPER_DETECT_FLAG);
	}
	
	if ( SM_APP_bIs_ed_reverse_rotation() && ( (GRAL_APP_usGet_st_3_ed_std_status1() & REVERSE_ROTATION_FLAG_bm) == 0 ) ) {
		
		EVNT_APP_vRegister_event(REVERSE_ROTATION_DETECTED, 0, true, END_DEVICE, rtc.utc_timestamp);
		GRAL_APP_vSet_flag_st_3_ed_std_status1(REVERSE_ROTATION_FLAG);
	}
	
	GRAL_APP_vStore_st_3_ed_std_status1();
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_unprogrammed(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device is programmed.
 *              true  : End Device is not programmed or it is in a 
 *                      factory default state.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_unprogrammed(void) {
	
	return false;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_configuration_error(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect a configuration error.
 *              true  : End Device did detect a configuration error.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_configuration_error(void) {
	
	bool error = false;
	
	if ( !meter.ed_mode.test_mode.enabled ) {

		if ( SM_APP_vIs_st_11_corrupted_data() ||
			 SM_APP_vIs_st_21_corrupted_data() ||
			 SM_APP_vIs_st_22_corrupted_data() ||
			 SM_APP_vIs_st_27_corrupted_data() ||
			 SM_APP_vIs_st_41_corrupted_data() ||
			 SM_APP_vIs_st_42_corrupted_data() ||
			 SM_APP_vIs_st_43_corrupted_data() ||
			 SM_APP_vIs_st_44_corrupted_data() ||
			 SM_APP_vIs_st_51_corrupted_data() ||
			 SM_APP_vIs_st_61_corrupted_data() ||
			 SM_APP_vIs_st_62_corrupted_data() ||
			 SM_APP_vIs_st_71_corrupted_data() ||
			 SM_APP_vIs_st_73_corrupted_data() ||
			 SM_APP_vIs_st_75_corrupted_data() ||
			 SM_APP_vIs_mt_73_corrupted_data() ){
		
			error = true;
		}
	}
	
	return error;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_self_chk_error(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect a self check error.
 *              true  : End Device did detect a self check error.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_self_chk_error(void) {
	
	return false;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_ram_failure(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect a RAM Memory failure.
 *              true  : End Device did detect a RAM Memory failure.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_ram_failure(void) {
	
	return false;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_rom_failure(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect a ROM memory failure.
 *              true  : End Device did detect a ROM Memory failure.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_rom_failure(void) {
	
	bool error = false;
	static volatile uint32_t crc_total;
	static volatile uint16_t crc_calculated, crc_on_flash;
	
	crc_total = CRC_app_crc16_calc();
	crc_calculated = (uint32_t)crc_total >> 16;
	crc_on_flash   = (uint32_t)crc_total;
	
	if ( crc_calculated != crc_on_flash ) {
		
		error = true;
	}
	
	return error;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_nonvol_mem_failure(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect a nonvolatile memory failure.
 *              true  : End Device did detect a nonvolatile memory failure.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_nonvol_mem_failure(void) {
	
	bool error = false;
	
	if ( SM_APP_ucGet_nonvol_mem_state() != (uint8_t)3 ) {
		error = true;
	}
	
	return error;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_clock_error(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect a clock error.
 *              true  : End Device did detect a clock error.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_clock_error(void) {
	
	return false;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_measurement_error(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect a measurement element error.
 *              true  : End Device did detect a measurement element error.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_measurement_error(void) {
	
	bool error = false;
	uint32_t dsp_is_running = VMetrology.DSP_ST.STATUS.WORD;
	uint32_t metrology_running = VMetrology.DSP_CTRL.STATE_CTRL.WORD;
	uint32_t atsense_ok = VMetrology.DSP_ST.STATE_FLAG.WORD;
	
	atsense_ok &= ATSENSE_FAIL_BIT_bm;
	
	if ( (dsp_is_running != DSP_IS_RUNNING) || (metrology_running != METROLOGY_IS_RUNNING) || (atsense_ok != ATSENSE_IS_OK) ) {
		
		error = true;
	}
	
	return error;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_low_battery(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect a low battery error.
 *              true  : End Device did detect a low battery error.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_low_battery(void) {
	
	return false;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_low_loss_potential(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect a potential that is below a predetermined value.
 *              true  : End Device did detect a device potential that is below a predetermined value.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_low_loss_potential(void) {
	
	return false;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_demand_overload(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect a demand threshold overload.
 *              true  : End Device did detect a demand threshold overload.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_demand_overload(void) {
	
	return false;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_power_failure(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect a power failure.
 *              true  : End Device did detect a power failure.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_power_failure(void) {
	
	return false;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_tamper_detect(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect tamper activity.
 *              true  : End Device did detect tamper activity.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_tamper_detect(void) {
	
	return false;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bIs_ed_reverse_rotation(void)
 * \brief
 *
 * \param void
 * \retval bool false : End Device did not detect reverse rotation.
 *              true  : End Device did detect reverse rotation.
 ******************************************************************************
 */
bool SM_APP_bIs_ed_reverse_rotation(void) {
	
	bool error = false;
	int32_t energy = (int32_t)METROLOGY_APP_dGet_instantaneous_value(P_T_ID);

	if ( energy < 0 ) {
		error = true;
	}
	
	return error;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_ucCheck_table_ram_vs_nvm(DIAGNOSTIC_TABLE_TYPE table)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t SM_APP_ucCheck_table_ram_vs_nvm(DIAGNOSTIC_TABLE_TYPE table) {
	
	TABLE_STATE_TYPE error_code = 0;	
	
	if ( table.size > 0 ) {
	
		if ( (table.crc_stored == table.crc_ram) && (table.crc_stored == table.crc_nvm) ) {
			
			/* Configuration OK - There is not nothing to do here, Return Error Code */
			error_code = TBL_DATA_OK;	
		}
		else if ( (table.crc_stored != table.crc_ram) && (table.crc_stored == table.crc_nvm) ) {
			
			/* RAM Corrupted Configuration - Read NVM values to RAM and Return Error Code*/
			table.recover_ram_table();
			error_code = TBL_RAM_DATA_CORRUPTED;
		}
		else if ( (table.crc_stored == table.crc_ram) && (table.crc_stored != table.crc_nvm) ) {
			
			/* NVM Corrupted Configuration - Write RAM values to NVM and Return Error Code */
			table.recover_nvm_table();
			error_code = TBL_NVM_DATA_CORRUPTED;
		}
		else if ( (table.crc_stored != table.crc_ram) && (table.crc_stored != table.crc_nvm) && (table.crc_ram == table.crc_nvm) ) {
			
			/* Corrupted Configuration - Return Error Code */
			error_code = TBL_NVM_CRC_CORRUPTED;
		}
		else if ( (table.crc_stored != table.crc_ram) && (table.crc_stored != table.crc_nvm) && (table.crc_ram != table.crc_nvm) ) {
			
			/* Corrupted Configuration - Return Error Code  */
			error_code = TBL_NVM_RAM_DATA_CORRUPTED;
		}
	}
	
	return (uint8_t)error_code;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_11_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_11_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = REGS_APP_usSt_11_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_11_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_11_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_11_read_crc();
	cfg_table.recover_nvm_table = REGS_APP_vSt_11_recover_nvm;
	cfg_table.recover_ram_table = REGS_APP_vSt_11_recover_ram;
	meter.config_state.st_11_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST11_ERROR_CODE_ADDR, &meter.config_state.st_11_state, 1 );
	
	if ( meter.config_state.st_11_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_11_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_11_nvm_calculate_crc(void) {
	
	REGS_APP_vCopy_st_11_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], REGS_APP_usSt_11_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_11_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_11_ram_calculate_crc(void) {
	
	REGS_APP_vCopy_st_11_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], REGS_APP_usSt_11_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_11_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_11_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_11_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_11_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_11_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_11_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_21_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_21_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = REGS_APP_usSt_21_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_21_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_21_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_21_read_crc();
	cfg_table.recover_nvm_table = REGS_APP_vSt_21_recover_nvm;
	cfg_table.recover_ram_table = REGS_APP_vSt_21_recover_ram;
	meter.config_state.st_21_state = (TABLE_STATE_TYPE)SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST21_ERROR_CODE_ADDR, &meter.config_state.st_21_state, 1 );
	
	if ( meter.config_state.st_21_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_21_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_21_nvm_calculate_crc(void) {
	
	REGS_APP_vCopy_st_21_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], REGS_APP_usSt_21_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_21_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_21_ram_calculate_crc(void) {
	
	REGS_APP_vCopy_st_21_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], REGS_APP_usSt_21_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_21_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_21_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_21_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_21_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_21_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_21_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_22_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_22_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = REGS_APP_usSt_22_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_22_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_22_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_22_read_crc();
	cfg_table.recover_nvm_table = REGS_APP_vSt_22_recover_nvm;
	cfg_table.recover_ram_table = REGS_APP_vSt_22_recover_ram;
	meter.config_state.st_22_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST22_ERROR_CODE_ADDR, &meter.config_state.st_22_state, 1 );
	
	if ( meter.config_state.st_22_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_22_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_22_nvm_calculate_crc(void) {
	
	REGS_APP_vCopy_st_22_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], REGS_APP_usSt_22_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_22_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_22_ram_calculate_crc(void) {
	
	REGS_APP_vCopy_st_22_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], REGS_APP_usSt_22_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_22_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_22_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_22_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_22_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_22_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_22_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_27_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_27_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = REGS_APP_usSt_27_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_27_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_27_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_27_read_crc();
	cfg_table.recover_nvm_table = REGS_APP_vSt_27_recover_nvm;
	cfg_table.recover_ram_table = REGS_APP_vSt_27_recover_ram;
	meter.config_state.st_27_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST27_ERROR_CODE_ADDR, &meter.config_state.st_27_state, 1 );
	
	if ( meter.config_state.st_27_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_27_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_27_nvm_calculate_crc(void) {
	
	REGS_APP_vCopy_st_27_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], REGS_APP_usSt_27_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_27_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_27_ram_calculate_crc(void) {
	
	REGS_APP_vCopy_st_27_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], REGS_APP_usSt_27_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_27_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_27_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_27_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_27_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_27_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_27_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_41_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_41_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = SECR_APP_usSt_41_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_41_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_41_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_41_read_crc();
	cfg_table.recover_nvm_table = SECR_APP_vSt_41_recover_nvm;
	cfg_table.recover_ram_table = SECR_APP_vSt_41_recover_ram;
	meter.config_state.st_41_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST41_ERROR_CODE_ADDR, &meter.config_state.st_41_state, 1 );
	
	if ( meter.config_state.st_41_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_41_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_41_nvm_calculate_crc(void) {
	
	SECR_APP_vCopy_st_41_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], SECR_APP_usSt_41_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_41_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_41_ram_calculate_crc(void) {
	
	SECR_APP_vCopy_st_41_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], SECR_APP_usSt_41_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_41_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_41_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_41_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_41_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_41_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_41_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_42_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_42_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = SECR_APP_usSt_42_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_42_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_42_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_42_read_crc();
	cfg_table.recover_nvm_table = SECR_APP_vSt_42_recover_nvm;
	cfg_table.recover_ram_table = SECR_APP_vSt_42_recover_ram;
	meter.config_state.st_42_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST42_ERROR_CODE_ADDR, &meter.config_state.st_42_state, 1 );
	
	if ( meter.config_state.st_42_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_42_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_42_nvm_calculate_crc(void) {
	
	SECR_APP_vCopy_st_42_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], SECR_APP_usSt_42_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_42_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_42_ram_calculate_crc(void) {
	
	SECR_APP_vCopy_st_42_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], SECR_APP_usSt_42_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_42_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_42_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_42_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_42_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_42_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_42_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_43_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_43_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = SECR_APP_usSt_43_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_43_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_43_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_43_read_crc();
	cfg_table.recover_nvm_table = SECR_APP_vSt_43_recover_nvm;
	cfg_table.recover_ram_table = SECR_APP_vSt_43_recover_ram;
	meter.config_state.st_43_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST43_ERROR_CODE_ADDR, &meter.config_state.st_43_state, 1 );
	
	if ( meter.config_state.st_43_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_43_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_43_nvm_calculate_crc(void) {
	
	SECR_APP_vCopy_st_43_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], SECR_APP_usSt_43_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_43_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_43_ram_calculate_crc(void) {
	
	SECR_APP_vCopy_st_43_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], SECR_APP_usSt_43_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_43_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_43_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_43_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_43_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_43_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_43_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_44_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_44_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = SECR_APP_usSt_44_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_44_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_44_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_44_read_crc();
	cfg_table.recover_nvm_table = SECR_APP_vSt_44_recover_nvm;
	cfg_table.recover_ram_table = SECR_APP_vSt_44_recover_ram;
	meter.config_state.st_44_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST44_ERROR_CODE_ADDR, &meter.config_state.st_44_state, 1 );
	
	if ( meter.config_state.st_44_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_44_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_44_nvm_calculate_crc(void) {
	
	memset(&tbl_array[0], 0, sizeof(tbl_array));
	SECR_APP_vCopy_st_44_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], SECR_APP_usSt_44_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_44_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_44_ram_calculate_crc(void) {
	
	memset(&tbl_array[0], 0, sizeof(tbl_array));
	SECR_APP_vCopy_st_44_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], SECR_APP_usSt_44_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_44_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_44_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_44_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_44_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_44_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_44_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_51_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_51_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = TOU_APP_usSt_51_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_51_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_51_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_51_read_crc();
	cfg_table.recover_nvm_table = TOU_APP_vSt_51_recover_nvm;
	cfg_table.recover_ram_table = TOU_APP_vSt_51_recover_ram;
	meter.config_state.st_51_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST51_ERROR_CODE_ADDR, &meter.config_state.st_51_state, 1 );
	
	if ( meter.config_state.st_51_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_51_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_51_nvm_calculate_crc(void) {
	
	TOU_APP_vCopy_st_51_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], TOU_APP_usSt_51_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_51_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_51_ram_calculate_crc(void) {
	
	TOU_APP_vCopy_st_51_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], TOU_APP_usSt_51_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_51_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_51_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_51_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_51_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_51_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_51_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_61_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_61_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = LP_APP_usSt_61_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_61_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_61_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_61_read_crc();
	cfg_table.recover_nvm_table = LP_APP_vSt_61_recover_nvm;
	cfg_table.recover_ram_table = LP_APP_vSt_61_recover_ram;
	meter.config_state.st_61_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST61_ERROR_CODE_ADDR, &meter.config_state.st_61_state, 1 );
	
	if ( meter.config_state.st_61_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_61_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_61_nvm_calculate_crc(void) {
	
	LP_APP_vCopy_st_61_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], LP_APP_usSt_61_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_61_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_61_ram_calculate_crc(void) {
	
	LP_APP_vCopy_st_61_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], LP_APP_usSt_61_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_61_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_61_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_61_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_61_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_61_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_61_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_62_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_62_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = LP_APP_usSt_62_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_62_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_62_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_62_read_crc();
	cfg_table.recover_nvm_table = LP_APP_vSt_62_recover_nvm;
	cfg_table.recover_ram_table = LP_APP_vSt_62_recover_ram;
	meter.config_state.st_62_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST62_ERROR_CODE_ADDR, &meter.config_state.st_62_state, 1 );
	
	if ( meter.config_state.st_62_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_62_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_62_nvm_calculate_crc(void) {
	
	LP_APP_vCopy_st_62_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], LP_APP_usSt_62_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_62_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_62_ram_calculate_crc(void) {
	
	LP_APP_vCopy_st_62_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], LP_APP_usSt_62_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_62_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_62_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_62_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_62_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_62_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_62_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_71_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_71_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = EVNT_APP_usSt_71_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_71_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_71_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_71_read_crc();
	cfg_table.recover_nvm_table = EVNT_APP_vSt_71_recover_nvm;
	cfg_table.recover_ram_table = EVNT_APP_vSt_71_recover_ram;
	meter.config_state.st_71_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST71_ERROR_CODE_ADDR, &meter.config_state.st_71_state, 1 );
	
	if ( meter.config_state.st_71_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_71_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_71_nvm_calculate_crc(void) {
	
	EVNT_APP_vCopy_st_71_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], EVNT_APP_usSt_71_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_71_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_71_ram_calculate_crc(void) {
	
	EVNT_APP_vCopy_st_71_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], EVNT_APP_usSt_71_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_71_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_71_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_71_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_71_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_71_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_71_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_73_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_73_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = EVNT_APP_usSt_73_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_73_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_73_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_73_read_crc();
	cfg_table.recover_nvm_table = EVNT_APP_vSt_73_recover_nvm;
	cfg_table.recover_ram_table = EVNT_APP_vSt_73_recover_ram;
	meter.config_state.st_73_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST73_ERROR_CODE_ADDR, &meter.config_state.st_73_state, 1 );
	
	if ( meter.config_state.st_73_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_73_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_73_nvm_calculate_crc(void) {
	
	EVNT_APP_vCopy_st_73_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], EVNT_APP_usSt_73_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_73_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_73_ram_calculate_crc(void) {
	
	EVNT_APP_vCopy_st_73_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], EVNT_APP_usSt_73_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_73_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_73_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_73_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_73_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_73_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_73_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_st_75_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_st_75_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = EVNT_APP_usSt_75_get_size();
	cfg_table.crc_nvm = SM_APP_usSt_75_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usSt_75_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usSt_75_read_crc();
	cfg_table.recover_nvm_table = EVNT_APP_vSt_75_recover_nvm;
	cfg_table.recover_ram_table = EVNT_APP_vSt_75_recover_ram;
	meter.config_state.st_75_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_ST75_ERROR_CODE_ADDR, &meter.config_state.st_75_state, 1 );
	
	if ( meter.config_state.st_75_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_75_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_75_nvm_calculate_crc(void) {
	
	EVNT_APP_vCopy_st_75_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], EVNT_APP_usSt_75_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_75_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_75_ram_calculate_crc(void) {
	
	EVNT_APP_vCopy_st_75_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], EVNT_APP_usSt_75_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSt_75_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSt_75_write_crc(uint16_t crc) {
	
	EEPROM_writer( ST_75_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usSt_75_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usSt_75_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( ST_75_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_vIs_mt_73_corrupted_data(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_vIs_mt_73_corrupted_data(void) {
	
	bool corrupted = false;
	
	cfg_table.size = EVNT_APP_usMt_73_get_size();
	cfg_table.crc_nvm = SM_APP_usMt_73_nvm_calculate_crc();
	cfg_table.crc_ram = SM_APP_usMt_73_ram_calculate_crc();
	cfg_table.crc_stored = SM_APP_usMt_73_read_crc();
	cfg_table.recover_nvm_table = EVNT_APP_vMt_73_recover_nvm;
	cfg_table.recover_ram_table = EVNT_APP_vMt_73_recover_ram;
	meter.config_state.mt_73_state = SM_APP_ucCheck_table_ram_vs_nvm(cfg_table);
	EEPROM_writer( MT_5_MT73_ERROR_CODE_ADDR, &meter.config_state.mt_73_state, 1 );
	
	if ( meter.config_state.mt_73_state != TBL_DATA_OK ) {
		corrupted = true;
	}
	
	return corrupted;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_73_nvm_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_73_nvm_calculate_crc(void) {
	
	EVNT_APP_vCopy_mt_73_nvm(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], EVNT_APP_usMt_73_get_size() );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_73_ram_calculate_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_73_ram_calculate_crc(void) {
	
	EVNT_APP_vCopy_mt_73_ram(&tbl_array[0]);
	return CRC_crc16( &tbl_array[0], EVNT_APP_usMt_73_get_size() );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vMt_73_write_crc(uint16_t crc)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vMt_73_write_crc(uint16_t crc) {
	
	EEPROM_writer( MT_73_NVM_CRC_ADDR, &crc, 2 );
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_73_read_crc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_73_read_crc(void) {
	
	uint16_t crc;
	EEPROM_reader( MT_73_NVM_CRC_ADDR, &crc, 2 );
	
	return crc;
}

/**
 ******************************************************************************
 * \fn void void SM_APP_vFwup_mcu_restart(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vFwup_mcu_restart(void) {
	
	if ( (op_c1218_apl.communication_state == BASE_STATE) && (nic_c1218_apl.communication_state == BASE_STATE) ) {
		
		SYSTEM_RESTART_TYPE mcu;
		mcu.restart_cause = FIRMWARE_UPDATE;
		mcu.save_information = true;
		RST_APP_vExecute_mcu_reset(mcu);
	}
	
	TIMER_start_ms_timer(SM_START_RESET_TIMER, START_RESET_MS, SM_APP_vFwup_mcu_restart);
}

/**
 ******************************************************************************
 * \fn void SM_APP_vIo_reconfig_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vIo_reconfig_task(void) {
	
	if ( TIMER_is_ms_timer_expired(IO_MCU_CONFIG_TIMER) ) {
		
		Disable_global_interrupt();
		
		/* Unused I/O pins */
		SYSTEM_initialize_io();
		
		if ( !meter.power_failure ) {
			
			/* Reset Rear Optical Port */
			NIC_io_reset();
			
			/* Reset Frontal Optical Port */
			OP_io_reset();
		}
		
		Enable_global_interrupt();
		
		TIMER_start_ms_timer(IO_MCU_CONFIG_TIMER, TIMER_ms_to_tic(5000), null_action);
	}
}

/**
 ******************************************************************************
 * \fn void SM_APP_vIo_reconfig_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vDiagnostics_task(void) {
	
	uint16_t std_status_1 = GRAL_APP_usGet_st_3_ed_std_status1();
	
	if ( TIMER_is_ms_timer_expired(DIAGNOSTICS_EXECUTION_TIMER) ) {
		
		if ( SM_APP_bIs_ed_unprogrammed() && ( (std_status_1 & UNPROGRAMMED_FLAG_bm) == 0 ) ) {
			GRAL_APP_vSet_flag_st_3_ed_std_status1(UNPROGRAMMED_FLAG);
		}
		
		//if ( SM_APP_bIs_ed_self_chk_error() && ( (std_status_1 & SELF_CHK_ERROR_FLAG_bm) == 0 ) ) {
		//	GRAL_APP_vSet_flag_st_3_ed_std_status1(SELF_CHK_ERROR_FLAG);
		//}
		
		//if ( SM_APP_bIs_ed_ram_failure() && ( (std_status_1 & RAM_FAILURE_FLAG_bm) == 0 ) ) {
		//	GRAL_APP_vSet_flag_st_3_ed_std_status1(RAM_FAILURE_FLAG);
		//}
		
		if ( SM_APP_bIs_ed_rom_failure() && ( (std_status_1 & ROM_FAILURE_FLAG_bm) == 0 ) ) {
			GRAL_APP_vSet_flag_st_3_ed_std_status1(ROM_FAILURE_FLAG);
		}
		
		if ( SM_APP_bIs_ed_nonvol_mem_failure() && ( (std_status_1 & NONVOL_MEM_FAILURE_FLAG_bm) == 0 ) ) {
			GRAL_APP_vSet_flag_st_3_ed_std_status1(NONVOL_MEM_FAILURE_FLAG);
		}
		
		//if ( SM_APP_bIs_ed_clock_error() && ( (std_status_1 & CLOCK_ERROR_FLAG_bm) == 0 ) ) {
		//	GRAL_APP_vSet_flag_st_3_ed_std_status1(CLOCK_ERROR_FLAG);
		//}
		
		if ( SM_APP_bIs_ed_measurement_error() && ( (std_status_1 & MEASUREMENT_ERROR_FLAG_bm) == 0 ) ) {
			GRAL_APP_vSet_flag_st_3_ed_std_status1(MEASUREMENT_ERROR_FLAG);
		}
		
		TIMER_start_ms_timer(DIAGNOSTICS_EXECUTION_TIMER, DIAGNOSTIC_PROC_MS, null_action);
	}
	
	if ( TIMER_is_ms_timer_expired(CONFIGURATION_ERROR_TIMER) ) {
		
		if ( SM_APP_bIs_ed_configuration_error() && ( (std_status_1 & CONFIGURATION_ERROR_FLAG_bm) == 0 ) ) {
			GRAL_APP_vSet_flag_st_3_ed_std_status1(CONFIGURATION_ERROR_FLAG);
		}
		
		TIMER_start_ms_timer(CONFIGURATION_ERROR_TIMER, CHECK_CONFIGURATION_MS_PERIOD, null_action);
	}
	
	//if ( SM_APP_bIs_ed_low_battery() && ( (std_status_1 & LOW_BATTERY_FLAG_bm) == 0 ) ) {
	//	GRAL_APP_vSet_flag_st_3_ed_std_status1(LOW_BATTERY_FLAG);
	//}
	
	//if ( SM_APP_bIs_ed_low_loss_potential() && ( (std_status_1 & LOW_LOSS_POTENTIAL_FLAG_bm) == 0 ) ) {
	//	GRAL_APP_vSet_flag_st_3_ed_std_status1(LOW_LOSS_POTENTIAL_FLAG);
	//}
	
	//if ( SM_APP_bIs_ed_demand_overload() && ( (std_status_1 & DEMAND_OVERLOAD_FLAG_bm) == 0 ) ) {
	//	GRAL_APP_vSet_flag_st_3_ed_std_status1(DEMAND_OVERLOAD_FLAG);
	//}
	
	//if ( SM_APP_bIs_ed_power_failure() && ( (std_status_1 & POWER_FAILURE_FLAG_bm) == 0 ) ) {
	//	GRAL_APP_vSet_flag_st_3_ed_std_status1(POWER_FAILURE_FLAG);
	//}
	
	//if ( SM_APP_bIs_ed_tamper_detect() && ( (std_status_1 & TAMPER_DETECT_FLAG_bm) == 0 ) ) {
	//	GRAL_APP_vSet_flag_st_3_ed_std_status1(TAMPER_DETECT_FLAG);
	//}
	
	if ( (SM_APP_bIs_ed_reverse_rotation() == true) && ( (std_status_1 & REVERSE_ROTATION_FLAG_bm) == 0 ) ) {
		
		EVNT_APP_vRegister_event(REVERSE_ROTATION_DETECTED, 0, true, END_DEVICE, rtc.utc_timestamp);
		GRAL_APP_vSet_flag_st_3_ed_std_status1(REVERSE_ROTATION_FLAG);
	}
	
	/* Store new Standard Status 1 if any change */
	if ( std_status_1 != GRAL_APP_usGet_st_3_ed_std_status1() ) {
		GRAL_APP_vStore_st_3_ed_std_status1();
	}		
}

/**
 ******************************************************************************
 * \fn void SM_APP_vVoltage_monitor_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vVoltage_monitor_task(void) {
	
	uint32_t voltage = 0;

#if NBR_PHASES >= 1

	voltage = (uint32_t)METROLOGY_APP_dGet_instantaneous_value(V_A_ID);
	
	if ( voltage > 0 ) {

		if ( voltage < meter.voltage_lower_limit ) {

			if ( meter.phase_a.prev_voltage_state != VOLTAGE_LOW ) {
				TIMER_reset_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER);
			}

			if ( meter.phase_a.voltage_state != VOLTAGE_LOW ) {

				if ( TIMER_is_ms_timer_expired(SM_PHA_VOLTAGE_MONITOR_TIMER) ) {

					TIMER_reset_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER);
					EVNT_APP_vRegister_event( (uint8_t)PHASE_A_LOW_VOLTAGE, voltage/1000, false, END_DEVICE, rtc.utc_timestamp );
					meter.phase_a.voltage_state = VOLTAGE_LOW;
				}

				if ( !TIMER_is_ms_timer_running(SM_PHA_VOLTAGE_MONITOR_TIMER) ) {
					TIMER_start_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER, meter.detect_voltage_change_period, null_action);
				}
			}
			else {
				TIMER_reset_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER);
			}

			meter.phase_a.prev_voltage_state = VOLTAGE_LOW;
		}
		else if ( voltage > meter.voltage_upper_limit ) {

			if ( meter.phase_a.prev_voltage_state != VOLTAGE_HIGH ) {
				TIMER_reset_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER);
			}
			
			if ( meter.phase_a.voltage_state != VOLTAGE_HIGH ) {

				if ( TIMER_is_ms_timer_expired(SM_PHA_VOLTAGE_MONITOR_TIMER) ) {

					TIMER_reset_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER);
					EVNT_APP_vRegister_event( (uint8_t)PHASE_A_HIGH_VOLTAGE, voltage/1000, false, END_DEVICE, rtc.utc_timestamp );
					meter.phase_a.voltage_state = VOLTAGE_HIGH;
				}

				if (!TIMER_is_ms_timer_running(SM_PHA_VOLTAGE_MONITOR_TIMER) ) {
					TIMER_start_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER, meter.detect_voltage_change_period, null_action);
				}
			}
			else {
				TIMER_reset_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER);
			}

			meter.phase_a.prev_voltage_state = VOLTAGE_HIGH;
		}
		else {
			
			if ( meter.phase_a.prev_voltage_state != VOLTAGE_NORMAL ) {
				TIMER_reset_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER);
			}

			if ( meter.phase_a.voltage_state != VOLTAGE_NORMAL ) {

				if ( TIMER_is_ms_timer_expired(SM_PHA_VOLTAGE_MONITOR_TIMER) ) {

					TIMER_reset_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER);
					EVNT_APP_vRegister_event( (uint8_t)PHASE_A_NORMAL_VOLTAGE, voltage/1000, false, END_DEVICE, rtc.utc_timestamp );
					meter.phase_a.voltage_state = VOLTAGE_NORMAL;
				}

				if (!TIMER_is_ms_timer_running(SM_PHA_VOLTAGE_MONITOR_TIMER) ) {
					TIMER_start_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER, meter.detect_voltage_change_period, null_action);
				}
			}
			else {
				TIMER_reset_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER);
			}

			meter.phase_a.prev_voltage_state = VOLTAGE_NORMAL;
		}
	}
	else {
		meter.phase_a.voltage_state = VOLTAGE_LOW;
	}

#endif

#if NBR_PHASES >= 2

	voltage = (uint32_t)METROLOGY_APP_dGet_instantaneous_value(V_B_ID);
	
	if ( voltage > 0 ) {

		if ( voltage < meter.voltage_lower_limit ) {

			if ( meter.phase_b.prev_voltage_state != VOLTAGE_LOW ) {
				TIMER_reset_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER);
			}

			if ( meter.phase_b.voltage_state != VOLTAGE_LOW ) {

				if ( TIMER_is_ms_timer_expired(SM_PHB_VOLTAGE_MONITOR_TIMER) ) {

					TIMER_reset_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER);
					EVNT_APP_vRegister_event( (uint8_t)PHASE_B_LOW_VOLTAGE, voltage/1000, false, END_DEVICE, rtc.utc_timestamp );
					meter.phase_b.voltage_state = VOLTAGE_LOW;
				}

				if ( !TIMER_is_ms_timer_running(SM_PHB_VOLTAGE_MONITOR_TIMER) ) {
					TIMER_start_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER, meter.detect_voltage_change_period, null_action);
				}
			}
			else {
				TIMER_reset_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER);
			}

			meter.phase_b.prev_voltage_state = VOLTAGE_LOW;
		}
		else if ( voltage > meter.voltage_upper_limit ) {

			if ( meter.phase_b.prev_voltage_state != VOLTAGE_HIGH ) {
				TIMER_reset_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER);
			}
			
			if ( meter.phase_b.voltage_state != VOLTAGE_HIGH ) {

				if ( TIMER_is_ms_timer_expired(SM_PHB_VOLTAGE_MONITOR_TIMER) ) {

					TIMER_reset_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER);
					EVNT_APP_vRegister_event( (uint8_t)PHASE_B_HIGH_VOLTAGE, voltage/1000, false, END_DEVICE, rtc.utc_timestamp );
					meter.phase_b.voltage_state = VOLTAGE_HIGH;
				}

				if (!TIMER_is_ms_timer_running(SM_PHB_VOLTAGE_MONITOR_TIMER) ) {
					TIMER_start_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER, meter.detect_voltage_change_period, null_action);
				}
			}
			else {
				TIMER_reset_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER);
			}

			meter.phase_b.prev_voltage_state = VOLTAGE_HIGH;
		}
		else {
			
			if ( meter.phase_b.prev_voltage_state != VOLTAGE_NORMAL ) {
				TIMER_reset_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER);
			}

			if ( meter.phase_b.voltage_state != VOLTAGE_NORMAL ) {

				if ( TIMER_is_ms_timer_expired(SM_PHB_VOLTAGE_MONITOR_TIMER) ) {

					TIMER_reset_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER);
					EVNT_APP_vRegister_event( (uint8_t)PHASE_B_NORMAL_VOLTAGE, voltage/1000, false, END_DEVICE, rtc.utc_timestamp );
					meter.phase_b.voltage_state = VOLTAGE_NORMAL;
				}

				if (!TIMER_is_ms_timer_running(SM_PHB_VOLTAGE_MONITOR_TIMER) ) {
					TIMER_start_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER, meter.detect_voltage_change_period, null_action);
				}
			}
			else {
				TIMER_reset_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER);
			}

			meter.phase_b.prev_voltage_state = VOLTAGE_NORMAL;
		}
	}
	else {
		meter.phase_b.voltage_state = VOLTAGE_LOW;
	}

#endif

#if NBR_PHASES >= 3

	voltage = (uint32_t)METROLOGY_APP_dGet_instantaneous_value(V_C_ID);
	
	if ( voltage > 0 ) {

		if ( voltage < meter.voltage_lower_limit ) {

			if ( meter.phase_c.prev_voltage_state != VOLTAGE_LOW ) {
				TIMER_reset_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER);
			}

			if ( meter.phase_c.voltage_state != VOLTAGE_LOW ) {

				if ( TIMER_is_ms_timer_expired(SM_PHC_VOLTAGE_MONITOR_TIMER) ) {

					TIMER_reset_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER);
					EVNT_APP_vRegister_event( (uint8_t)PHASE_C_LOW_VOLTAGE, voltage/1000, false, END_DEVICE, rtc.utc_timestamp );
					meter.phase_c.voltage_state = VOLTAGE_LOW;
				}

				if ( !TIMER_is_ms_timer_running(SM_PHC_VOLTAGE_MONITOR_TIMER) ) {
					TIMER_start_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER, meter.detect_voltage_change_period, null_action);
				}
			}
			else {
				TIMER_reset_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER);
			}

			meter.phase_c.prev_voltage_state = VOLTAGE_LOW;
		}
		else if ( voltage > meter.voltage_upper_limit ) {

			if ( meter.phase_c.prev_voltage_state != VOLTAGE_HIGH ) {
				TIMER_reset_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER);
			}
			
			if ( meter.phase_c.voltage_state != VOLTAGE_HIGH ) {

				if ( TIMER_is_ms_timer_expired(SM_PHC_VOLTAGE_MONITOR_TIMER) ) {

					TIMER_reset_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER);
					EVNT_APP_vRegister_event( (uint8_t)PHASE_C_HIGH_VOLTAGE, voltage/1000, false, END_DEVICE, rtc.utc_timestamp );
					meter.phase_c.voltage_state = VOLTAGE_HIGH;
				}

				if (!TIMER_is_ms_timer_running(SM_PHC_VOLTAGE_MONITOR_TIMER) ) {
					TIMER_start_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER, meter.detect_voltage_change_period, null_action);
				}
			}
			else {
				TIMER_reset_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER);
			}

			meter.phase_c.prev_voltage_state = VOLTAGE_HIGH;
		}
		else {
			
			if ( meter.phase_c.prev_voltage_state != VOLTAGE_NORMAL ) {
				TIMER_reset_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER);
			}

			if ( meter.phase_c.voltage_state != VOLTAGE_NORMAL ) {

				if ( TIMER_is_ms_timer_expired(SM_PHC_VOLTAGE_MONITOR_TIMER) ) {

					TIMER_reset_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER);
					EVNT_APP_vRegister_event( (uint8_t)PHASE_C_NORMAL_VOLTAGE, voltage/1000, false, END_DEVICE, rtc.utc_timestamp );
					meter.phase_c.voltage_state = VOLTAGE_NORMAL;
				}

				if (!TIMER_is_ms_timer_running(SM_PHC_VOLTAGE_MONITOR_TIMER) ) {
					TIMER_start_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER, meter.detect_voltage_change_period, null_action);
				}
			}
			else {
				TIMER_reset_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER);
			}

			meter.phase_c.prev_voltage_state = VOLTAGE_NORMAL;
		}
	}
	else {
		meter.phase_c.voltage_state = VOLTAGE_LOW;
	}

#endif
}

/**
 ******************************************************************************
 * \fn void SM_APP_vSet_test_mode_expired_flag(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vSet_test_mode_expired_flag(void) {
	
	meter_os.test_mode_expired = true;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_ucGet_table_state(uint32_t table_status_addr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t SM_APP_ucGet_table_state(uint32_t table_state_addr) {
	
	uint8_t state;
	
	EEPROM_reader( table_state_addr, &state, 1 );
	
	return state;
}

/**
 ******************************************************************************
 * \fn void SM_APP_vChange_table_state(uint32_t table_status_addr, uint8_t status)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vChange_table_state(uint32_t table_state_addr, uint8_t state) {
		
	EEPROM_writer( table_state_addr, &state, 1 );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vCheck_firmware_version(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vCheck_firmware_version(void) {
	
	uint8_t old_firmware;
	uint8_t new_firmware;
	
	// Check FW version
	new_firmware = FW_VERSION_NUMBER;
	EEPROM_reader( ST_1_FW_VERSION_NUMBER_ADDR, &old_firmware, 1);
	
	if ( new_firmware != old_firmware) {
		
		EEPROM_writer( ST_1_FW_VERSION_NUMBER_ADDR, &new_firmware, 1);
		
		// Check FW revision
		new_firmware = FW_REVISION_NUMBER;
		EEPROM_reader( ST_1_FW_REVISION_NUMBER_ADDR, &old_firmware, 1);

		if ( new_firmware != old_firmware) {

			EEPROM_writer( ST_1_FW_REVISION_NUMBER_ADDR, &new_firmware, 1);
		}	
	}
}

/**
 ******************************************************************************
 * \fn void SM_APP_vLoad_mt_1(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vLoad_mt_1(void) {
	
	if ( SM_APP_ucGet_table_state(MT_1_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		SM_APP_vMt_1_restore_default_values();
	}
	else {
		// There is no shadow ram for this table
	}

}
/**
 ******************************************************************************
 * \fn void SM_APP_vMt_1_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vMt_1_restore_default_values(void) {
	
	uint8_t data_1[16];
	uint8_t temp = 0;
	
	SM_APP_vChange_table_state(MT_1_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	memset(&data_1, 0, sizeof(data_1));
	strcpy((char*)&data_1, FW_PART_NUMBER);														// FW Part number
	EEPROM_writer( MT_1_FW_PART_NUMBER_ADDR, &data_1, 8);
	strcpy((char*)&data_1, HW_PART_NUMBER);														// HW Part number
	EEPROM_writer( MT_1_HW_PART_NUMBER_ADDR, &data_1, 8);
	memset((void*)&data_1, temp, 1);															// Identity
	EEPROM_writer( MT_1_ED_IDENTITY_ADDR, &data_1, 1);
	strcpy((char*)&data_1, "0000000000000000");													// Board assembly part number
	EEPROM_writer( MT_1_BOARD_ASSEMBLY_SERIAL_NUMBER_ADDR, &data_1, 16);
	strcpy((char*)&data_1, "0000000000000000");													// MFG module serial number
	EEPROM_writer( MT_1_MFG_MODULE_SERIAL_NUMBER_ADDR, &data_1, 16);
	
	SM_APP_vChange_table_state(MT_1_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_1_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_1_execute_table_reading(uint8_t *table_buffer) {
	
	delay_ms(10);
	EEPROM_reader( MT_1_START_ADDR, &table_buffer[0], MT_1_SIZE);
	memcpy((char*)&table_buffer[0], FW_PART_NUMBER, 8);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_1_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_1_execute_table_writing(uint8_t *table_buffer) {
	
	EEPROM_writer(MT_1_START_ADDR, &table_buffer[0], MT_1_SIZE);
	
	return true;	
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_1_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_1_get_size(void) {

	return (uint16_t)MT_1_SIZE;
}

/**
 ******************************************************************************
 * \fn void SM_APP_vLoad_mt_3(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vLoad_mt_3(void) {
	
	if ( SM_APP_ucGet_table_state(MT_3_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		SM_APP_vMt_3_restore_default_values();
	}
	else {
		// There is no shadow ram for this table
	}
}

/**
 ******************************************************************************
 * \fn void SM_APP_vMt_3_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vMt_3_restore_default_values(void) {
	
	uint32_t nbr_power_outages;
	
	SM_APP_vChange_table_state(MT_3_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_reader(MT_3_NBR_POWER_OUTAGES_ADDR, &nbr_power_outages, 4);
	
	if (nbr_power_outages == 0xFFFFFFFF) {
		nbr_power_outages = 0;
		EEPROM_writer(MT_3_NBR_POWER_OUTAGES_ADDR, &nbr_power_outages, 4);
	}
	
	SM_APP_vChange_table_state(MT_3_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_3_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_3_execute_table_reading(uint8_t *table_buffer) {
	
	delay_ms(10);
	EEPROM_reader( MT_3_START_ADDR, &table_buffer[0], MT_3_SIZE);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_3_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_3_execute_table_writing(uint8_t *table_buffer) {
	
	EEPROM_writer(MT_3_START_ADDR, &table_buffer[0], MT_3_SIZE);
	
	return true;	
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_3_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_3_get_size(void) {

	return (uint16_t)MT_3_SIZE;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vLoad_mt_23(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vLoad_mt_23(void) {
		
	if ( SM_APP_ucGet_table_state(MT_23_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		SM_APP_vMt_23_load_default_values();
		SM_APP_vMt_23_write_eeprom_values();
	}
	else {
		SM_APP_vMt_23_read_eeprom_values();
	}
}

/**
 ******************************************************************************
 * \fn void SM_APP_vMt_23_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vMt_23_restore_default_values(void) {
	
	SM_APP_vMt_23_load_default_values();
	SM_APP_vMt_23_write_eeprom_values();
}

/**
 ******************************************************************************
 * \fn void SM_APP_vMt_23_load_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vMt_23_load_default_values(void) {
	
	meter.voltage_lower_limit = 108000;	// 108,000 mV
	meter.voltage_upper_limit = 132000;	// 132,000 mV
}

/**
 ******************************************************************************
 * \fn void SM_APP_vMt_23_read_eeprom_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vMt_23_read_eeprom_values(void) {
	
	delay_ms(10);
	EEPROM_reader( MT_23_VOLT_LOW_LIMIT_ADDR, &meter.voltage_lower_limit, 4 );
	EEPROM_reader( MT_23_VOLT_HIGH_LIMIT_ADDR, &meter.voltage_upper_limit, 4 );
}

/**
 ******************************************************************************
 * \fn void SM_APP_vMt_23_write_eeprom_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vMt_23_write_eeprom_values(void) {
	
	SM_APP_vChange_table_state(MT_23_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( MT_23_VOLT_LOW_LIMIT_ADDR, &meter.voltage_lower_limit, 4 );
	EEPROM_writer( MT_23_VOLT_HIGH_LIMIT_ADDR, &meter.voltage_upper_limit, 4 );
	
	SM_APP_vChange_table_state(MT_23_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_23_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_23_execute_table_reading(uint8_t *table_buffer) {
	
	uint8_t k = 0;
	uint32_t phase_voltage;
	
	table_buffer[k++] = (uint8_t)(meter.voltage_lower_limit >> 0);
	table_buffer[k++] = (uint8_t)(meter.voltage_lower_limit >> 8);
	table_buffer[k++] = (uint8_t)(meter.voltage_lower_limit >> 16);
	table_buffer[k++] = (uint8_t)(meter.voltage_lower_limit >> 24);
	
	table_buffer[k++] = (uint8_t)(meter.voltage_upper_limit >> 0);
	table_buffer[k++] = (uint8_t)(meter.voltage_upper_limit >> 8);
	table_buffer[k++] = (uint8_t)(meter.voltage_upper_limit >> 16);
	table_buffer[k++] = (uint8_t)(meter.voltage_upper_limit >> 24);

#if NBR_PHASES >= 1
	table_buffer[k++] = (uint8_t)(meter.phase_a.failure);
	table_buffer[k++] = (uint8_t)(meter.phase_a.voltage_state);
	phase_voltage = (uint32_t)METROLOGY_APP_dGet_instantaneous_value(V_A_ID);
	table_buffer[k++] = (uint8_t)(phase_voltage >> 0);
	table_buffer[k++] = (uint8_t)(phase_voltage >> 8);
	table_buffer[k++] = (uint8_t)(phase_voltage >> 16);
	table_buffer[k++] = (uint8_t)(phase_voltage >> 24);
#endif

#if NBR_PHASES >= 2
	table_buffer[k++] = (uint8_t)(meter.phase_b.failure);
	table_buffer[k++] = (uint8_t)(meter.phase_b.voltage_state);
	phase_voltage = (uint32_t)METROLOGY_APP_dGet_instantaneous_value(V_B_ID);
	table_buffer[k++] = (uint8_t)(phase_voltage >> 0);
	table_buffer[k++] = (uint8_t)(phase_voltage >> 8);
	table_buffer[k++] = (uint8_t)(phase_voltage >> 16);
	table_buffer[k++] = (uint8_t)(phase_voltage >> 24);
#endif

#if NBR_PHASES >= 3
	table_buffer[k++] = (uint8_t)(meter.phase_c.failure);
	table_buffer[k++] = (uint8_t)(meter.phase_c.voltage_state);
	phase_voltage = (uint32_t)METROLOGY_APP_dGet_instantaneous_value(V_C_ID);
	table_buffer[k++] = (uint8_t)(phase_voltage >> 0);
	table_buffer[k++] = (uint8_t)(phase_voltage >> 8);
	table_buffer[k++] = (uint8_t)(phase_voltage >> 16);
	table_buffer[k++] = (uint8_t)(phase_voltage >> 24);
#endif

	return true;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_23_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_23_execute_table_writing(uint8_t *table_buffer) {
	
	EEPROM_writer(MT_23_START_ADDR, &table_buffer[0], MT_23_MAX_SIZE);
	SM_APP_vMt_23_read_eeprom_values();
	
	return true;	
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_23_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_23_get_size(void) {

	return (uint16_t)MT_23_SIZE;
}

/**
 ******************************************************************************
 * \fn void SM_APP_vLoad_mt_4(void)
 * \brief MCU Trouble Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vLoad_mt_4(void) {
	
	if ( SM_APP_ucGet_table_state(MT_4_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		SM_APP_vMt_4_restore_default_values();
	}
	else {
		// There is no shadow ram for this table
	}
	
	// Increment CPU reset counter
	uint16_t mcu_reset_total_counter;
	EEPROM_reader(MT_4_MCU_RESET_TOTAL_COUNTER_ADDR, (uint8_t*)&mcu_reset_total_counter, sizeof(mcu_reset_total_counter));
	mcu_reset_total_counter++;
	EEPROM_writer(MT_4_MCU_RESET_TOTAL_COUNTER_ADDR, (uint8_t*)&mcu_reset_total_counter, sizeof(mcu_reset_total_counter));
}

/**
 ******************************************************************************
 * \fn void SM_APP_vMt_4_restore_default_values(void)
 * \brief MCU Trouble Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vMt_4_restore_default_values(void) {
	
	SM_APP_vChange_table_state(MT_4_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	uint8_t buffer[MT_4_MAX_SIZE];
	memset(&buffer, 0, sizeof(buffer));
	EEPROM_writer(MT_4_START_ADDR, (uint8_t*)&buffer, MT_4_MAX_SIZE);
	
	SM_APP_vChange_table_state(MT_4_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_4_execute_table_reading(uint8_t *table_buffer)
 * \brief MCU Trouble Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_4_execute_table_reading(uint8_t *table_buffer) {
	
	uint8_t k = 0;
	
	table_buffer[k++] = (uint8_t)(meter.time_running >> 0);
	table_buffer[k++] = (uint8_t)(meter.time_running >> 8);
	table_buffer[k++] = (uint8_t)(meter.time_running >> 16);
	table_buffer[k++] = (uint8_t)(meter.time_running >> 24);
	
	EEPROM_reader(MT_4_MCU_RESET_TOTAL_COUNTER_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_reader(MT_4_MCU_RESET_POWER_FAILURE_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_reader(MT_4_MCU_RESET_FIRMWARE_UPDATE_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_reader(MT_4_MCU_RESET_WATCH_DOG_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_reader(MT_4_MCU_RESET_COLD_START_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_reader(MT_4_MCU_RESET_WARM_START_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_reader(MT_4_MCU_RESET_KT_CONSTANT_CHANGE_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_reader(MT_4_MCU_RESET_SYSTEM_FAULT_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_reader(MT_4_MCU_RESET_VLISTINSERT_COUNTER_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_reader(MT_4_MCU_RESET_DUMMY_HANDLER_COUNTER_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_reader(MT_4_MCU_RESET_ASSERT_TRIGGERED_COUNTER_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_reader(MT_4_MCU_RESET_STACK_OVERFLOW_COUNTER_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_reader(MT_4_RESERVED_ADDR, (uint8_t*)&table_buffer[k], MT_4_RESERVED_SIZE);
	k += MT_4_RESERVED_SIZE;
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_4_execute_table_writing(uint8_t *table_buffer)
 * \brief MCU Trouble Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_4_execute_table_writing(uint8_t *table_buffer) {
	
	uint8_t k = 0;
	
	meter.time_running  = (uint32_t)(table_buffer[k++] << 0);
	meter.time_running |= (uint32_t)(table_buffer[k++] << 8);
	meter.time_running |= (uint32_t)(table_buffer[k++] << 16);
	meter.time_running |= (uint32_t)(table_buffer[k++] << 24);
	
	EEPROM_writer(MT_4_MCU_RESET_TOTAL_COUNTER_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_writer(MT_4_MCU_RESET_POWER_FAILURE_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_writer(MT_4_MCU_RESET_FIRMWARE_UPDATE_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_writer(MT_4_MCU_RESET_WATCH_DOG_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_writer(MT_4_MCU_RESET_COLD_START_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_writer(MT_4_MCU_RESET_WARM_START_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_writer(MT_4_MCU_RESET_KT_CONSTANT_CHANGE_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_writer(MT_4_MCU_RESET_SYSTEM_FAULT_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_writer(MT_4_MCU_RESET_VLISTINSERT_COUNTER_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_writer(MT_4_MCU_RESET_DUMMY_HANDLER_COUNTER_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_writer(MT_4_MCU_RESET_ASSERT_TRIGGERED_COUNTER_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_writer(MT_4_MCU_RESET_STACK_OVERFLOW_COUNTER_ADDR, (uint8_t*)&table_buffer[k], 2);
	k += 2;
	EEPROM_writer(MT_4_RESERVED_ADDR, (uint8_t*)&table_buffer[k], MT_4_RESERVED_SIZE);
	k += MT_4_RESERVED_SIZE;
	
	return true;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_4_get_size(void)
 * \brief MCU Trouble Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_4_get_size(void) {

	return (uint16_t)MT_4_SIZE;
}

/**
 ******************************************************************************
 * \fn void SM_APP_vLoad_mt_5(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vLoad_mt_5(void) {
	
	if ( SM_APP_ucGet_table_state(Mt_5_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		SM_APP_vMt_5_restore_default_values();
	}
	else {
		// There is no shadow ram for this table
	}
}

/**
 ******************************************************************************
 * \fn void SM_APP_vMt_5_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vMt_5_restore_default_values(void) {
	
	SM_APP_vChange_table_state(Mt_5_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	uint8_t buffer[MT_5_SIZE];
	memset(&buffer, 0, sizeof(buffer));
	EEPROM_writer(MT_5_START_ADDR, (uint8_t*)&buffer[0], MT_5_SIZE);
	
	SM_APP_vChange_table_state(Mt_5_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_5_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_5_execute_table_reading(uint8_t *table_buffer) {
	
	EEPROM_reader(MT_5_START_ADDR, (uint8_t*)&table_buffer[0], MT_5_SIZE);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_5_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_5_execute_table_writing(uint8_t *table_buffer) {
	
	EEPROM_writer(MT_5_START_ADDR, (uint8_t*)&table_buffer[0], MT_5_SIZE);
	
	return true;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_5_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_5_get_size(void) {

	return (uint16_t)MT_5_SIZE;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_22_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_22_execute_table_reading(uint8_t *table_buffer) {
	
	uint8_t k = 0;
	uint64_t var_temp;
	uint8_t bcd_value[8];

	/* Phase A Instantaneous Values*/

#if ( NBR_PHASES >= 1 )
	/* Voltage */
	var_temp = 0;
	var_temp = (uint64_t)METROLOGY_APP_dGet_instantaneous_value(V_A_ID);
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Current */
	var_temp = 0;
	var_temp = (uint64_t)METROLOGY_APP_dGet_instantaneous_value(I_A_ID);
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Active Power */
	var_temp = 0;
	var_temp = (uint64_t)( fabs(METROLOGY_APP_dGet_instantaneous_value(P_A_ID)) );
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 5;
	
	/* Reactive Power */
	var_temp = 0;
	var_temp = (uint64_t)( fabs(METROLOGY_APP_dGet_instantaneous_value(Q_A_ID)) );
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 5;
#endif

	/* Phase B Instantaneous Values*/
#if ( NBR_PHASES >= 2 )
	/* Voltage */
	var_temp = 0;
	var_temp = (uint64_t)METROLOGY_APP_dGet_instantaneous_value(V_B_ID);
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Current */
	var_temp = 0;
	var_temp = (uint64_t)METROLOGY_APP_dGet_instantaneous_value(I_B_ID);
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Active Power */
	var_temp = 0;
	var_temp = (uint64_t)( fabs(METROLOGY_APP_dGet_instantaneous_value(P_B_ID)) );
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 5;
	
	/* Reactive Power */
	var_temp = 0;
	var_temp = (uint64_t)( fabs(METROLOGY_APP_dGet_instantaneous_value(Q_B_ID)) );
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 5;
#endif

	/* Phase C Instantaneous Values*/
#if ( NBR_PHASES >= 3 )
	/* Voltage */
	var_temp = 0;
	var_temp = (uint64_t)METROLOGY_APP_dGet_instantaneous_value(V_C_ID);
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Current */
	var_temp = 0;
	var_temp = (uint64_t)METROLOGY_APP_dGet_instantaneous_value(I_C_ID);
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Active Power */
	var_temp = 0;
	var_temp = (uint64_t)( fabs(METROLOGY_APP_dGet_instantaneous_value(P_C_ID)) );
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 5;
	
	/* Reactive Power */
	var_temp = 0;
	var_temp = (uint64_t)( fabs(METROLOGY_APP_dGet_instantaneous_value(Q_C_ID)) );
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 5;
#endif

	return true;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_22_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_22_get_size(void) {

	return (uint16_t)MT_22_SIZE;
}

/**
 ******************************************************************************
 * \fn void SM_APP_vLoad_mt_53(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vLoad_mt_53(void) {
	
	if ( SM_APP_ucGet_table_state(MT_53_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		SM_APP_vMt_53_restore_default_values();
	}
	else {
		// There is no shadow ram for this table
	}

}
/**
 ******************************************************************************
 * \fn void SM_APP_vMt_53_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vMt_53_restore_default_values(void) {
	
	uint8_t data_1[70];
	
	SM_APP_vChange_table_state(MT_53_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	memset(&data_1, (int)0, sizeof(data_1));
	EEPROM_writer( MT_53_WIN_TIMEZONE_INDEX_ADDR, &data_1, 2);
	memset(&data_1, (int)0x20, sizeof(data_1));														
	memcpy((char*)&data_1, "Central Standard Time (Mexico)", 30);
	EEPROM_writer( MT_53_WIN_TIMEZONE_NAME_ADDR, &data_1, 70);
	
	SM_APP_vChange_table_state(MT_53_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_53_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_53_execute_table_reading(uint8_t *table_buffer) {
	
	delay_ms(10);
	EEPROM_reader( MT_53_START_ADDR, &table_buffer[0], MT_53_SIZE);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_53_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_53_execute_table_writing(uint8_t *table_buffer) {
	
	EEPROM_writer(MT_53_START_ADDR, &table_buffer[0], MT_53_SIZE);
	
	return true;	
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_53_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_53_get_size(void) {

	return (uint16_t)MT_53_SIZE;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_230_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_230_execute_table_reading(uint8_t *table_buffer) {
	
	uint8_t k = 0;
	uint8_t bcd_value[8];
	static uint64_t var_temp;
	int64_t var_temp_sign;
	uint8_t minute, second;
	
	/* Voltage */
	var_temp = 0;
	var_temp = (uint64_t)METROLOGY_APP_dGet_instantaneous_value(V_A_ID);
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Current */
	var_temp = 0;
	var_temp = (uint64_t)METROLOGY_APP_dGet_instantaneous_value(I_A_ID);
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Power Factor */
	var_temp = 0;
	double p = METROLOGY_APP_dGet_instantaneous_value(P_T_ID);
	double q = METROLOGY_APP_dGet_instantaneous_value(Q_T_ID);
	double s = METROLOGY_APP_dGet_instantaneous_value(S_T_ID);
	var_temp = (int64_t)( ( fabs(p) / fabs(s) ) * 1000 );
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Angle */
	var_temp = 0;
	var_temp = (uint64_t)METROLOGY_APP_dGet_instantaneous_value(ANGLE_PF_T_ID);
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Frequency */
	var_temp = 0;
	var_temp = (uint64_t)METROLOGY_APP_dGet_instantaneous_value(FREQ_T_ID);
	if (var_temp == 0) {
		var_temp = 0;
		k += 0;
		UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	}
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Instantaneous active power */
	var_temp_sign = 0;
	var_temp_sign = (int64_t)p;
	if (var_temp_sign < 0) {
		table_buffer[k++] = 0x80;	// - sign
		var_temp = (-1) * var_temp_sign;
	}
	else {
		table_buffer[k++] = 0;		// + sign
		var_temp = var_temp_sign;
	}
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Instantaneous reactive power */
	var_temp_sign = 0;
	var_temp_sign = (int64_t)q;
	if (var_temp_sign < 0) {
		table_buffer[k++] = 0x80;	// - sign
		var_temp = (-1) * var_temp_sign;
	}
	else {
		table_buffer[k++] = 0;		// + sign
		var_temp = var_temp_sign;
	}
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;
	
	/* Instantaneous apparent power */
	var_temp_sign = 0;
	var_temp_sign = (int64_t)s;
	if (var_temp_sign < 0) {
		table_buffer[k++] = 0x80;	// - sign
		var_temp = (-1) * var_temp_sign;
	}
	else {
		table_buffer[k++] = 0;		// + sign
		var_temp = var_temp_sign;
	}
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[4], 4);
	k += 4;

	/* Total Sum Active Energy */
	table_buffer[k++] = 0;
	if ( !REGS_APP_bCopy_summation(TOTAL_DATA, SM_WH_SUM_ID, &var_temp ) ) { var_temp = 0; }
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[0], 8);
	k += 8;
	
	/* Total Sum Reactive Energy */
	table_buffer[k++] = 0;
	if ( !REGS_APP_bCopy_summation(TOTAL_DATA, SM_VARH_SUM_ID, &var_temp ) ) { var_temp = 0; }
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[0], 8);
	k += 8;
	
	/* Total Sum Apparent Energy */
	table_buffer[k++] = 0;
	if ( !REGS_APP_bCopy_summation(TOTAL_DATA, SM_VAH_SUM_ID, &var_temp ) ) { var_temp = 0; }
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[0], 8);
	k += 8;

	/* Max Sum Active Demand */
	if ( !REGS_APP_bCopy_average_demand(TOTAL_DATA, AD_W_SUM_ID, &var_temp ) ) { var_temp = 0; }
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[3], 5);
	k += 5;

	/* Max Sum Reactive Demand */
	if ( !REGS_APP_bCopy_average_demand(TOTAL_DATA, AD_VAR_SUM_ID, &var_temp ) ) { var_temp = 0; }
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[3], 5);
	k += 5;

	/* Sum Active Present Demand */
	if ( !REGS_APP_bCopy_present_demand(INTERVAL_0, PD_W_SUM_ID, &var_temp ) ) { var_temp = 0; }
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[3], 5);
	k += 5;
	
	/* Sum Reactive Present Demand */
	if ( !REGS_APP_bCopy_present_demand(INTERVAL_0, PD_VAR_SUM_ID, &var_temp ) ) { var_temp = 0; }
	UTIL_hex_to_bcd( (uint64_t)(var_temp), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[3], 5);
	k += 5;
	
	/* Demand Interval Time Remaining */
	REGS_APP_vGet_interval_time_remining(INTERVAL_0, &minute, &second);
	
	UTIL_hex_to_bcd( (uint64_t)(minute), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[7], 1);
	k += 1;
	
	UTIL_hex_to_bcd( (uint64_t)(second), &bcd_value[0]);
	memcpy(&table_buffer[k], &bcd_value[7], 1);
	k += 1;
	
	return true;
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_230_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_230_get_size(void) {

	return (uint16_t)MT_230_SIZE;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_24_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_24_execute_table_reading(uint8_t *table_buffer) {
	
	uint8_t k = 0;
	uint64_t acc_quantity;

#if NBR_PHASES >= 1

	acc_quantity = VMetrology.DSP_ACC.I_A;
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000000000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000000000FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000FF000000) >> 24 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000FF00000000) >> 32 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000FF0000000000) >> 40 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00FF000000000000) >> 48 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0xFF00000000000000) >> 56 );

	acc_quantity = VMetrology.DSP_ACC.V_A;
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000000000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000000000FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000FF000000) >> 24 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000FF00000000) >> 32 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000FF0000000000) >> 40 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00FF000000000000) >> 48 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0xFF00000000000000) >> 56 );

	acc_quantity = VMetrology.DSP_ACC.P_A;
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000000000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000000000FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000FF000000) >> 24 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000FF00000000) >> 32 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000FF0000000000) >> 40 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00FF000000000000) >> 48 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0xFF00000000000000) >> 56 );

	acc_quantity = VMetrology.DSP_ACC.Q_A;
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000000000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000000000FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000FF000000) >> 24 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000FF00000000) >> 32 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000FF0000000000) >> 40 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00FF000000000000) >> 48 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0xFF00000000000000) >> 56 );

#endif


#if NBR_PHASES >= 2
	
	acc_quantity = VMetrology.DSP_ACC.I_B;
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000000000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000000000FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000FF000000) >> 24 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000FF00000000) >> 32 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000FF0000000000) >> 40 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00FF000000000000) >> 48 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0xFF00000000000000) >> 56 );

	acc_quantity = VMetrology.DSP_ACC.V_B;
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000000000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000000000FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000FF000000) >> 24 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000FF00000000) >> 32 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000FF0000000000) >> 40 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00FF000000000000) >> 48 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0xFF00000000000000) >> 56 );

	acc_quantity = VMetrology.DSP_ACC.P_B;
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000000000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000000000FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000FF000000) >> 24 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000FF00000000) >> 32 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000FF0000000000) >> 40 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00FF000000000000) >> 48 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0xFF00000000000000) >> 56 );

	acc_quantity = VMetrology.DSP_ACC.Q_B;
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000000000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000000000FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000FF000000) >> 24 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000FF00000000) >> 32 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000FF0000000000) >> 40 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00FF000000000000) >> 48 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0xFF00000000000000) >> 56 );

#endif


#if NBR_PHASES >= 3
	
	acc_quantity = VMetrology.DSP_ACC.I_C;
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000000000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000000000FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000FF000000) >> 24 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000FF00000000) >> 32 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000FF0000000000) >> 40 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00FF000000000000) >> 48 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0xFF00000000000000) >> 56 );

	acc_quantity = VMetrology.DSP_ACC.V_C;
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000000000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000000000FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000FF000000) >> 24 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000FF00000000) >> 32 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000FF0000000000) >> 40 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00FF000000000000) >> 48 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0xFF00000000000000) >> 56 );

	acc_quantity = VMetrology.DSP_ACC.P_C;
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000000000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000000000FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000FF000000) >> 24 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000FF00000000) >> 32 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000FF0000000000) >> 40 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00FF000000000000) >> 48 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0xFF00000000000000) >> 56 );

	acc_quantity = VMetrology.DSP_ACC.Q_C;
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000000000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000000000FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00000000FF000000) >> 24 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x000000FF00000000) >> 32 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x0000FF0000000000) >> 40 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0x00FF000000000000) >> 48 );
	table_buffer[k++] = (uint8_t)( (acc_quantity & 0xFF00000000000000) >> 56 );

#endif	
	
	return true;	
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_24_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_24_get_size(void) {

	return (uint16_t)MT_24_SIZE;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bMt_25_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bMt_25_execute_table_reading(uint8_t *table_buffer) {
	
	uint8_t k = 0;
	uint32_t register_value;
	
#if NBR_PHASES >= 1

	EEPROM_reader(DSP_CTRL_CAL_M_IA_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_M_VA_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_PH_IA_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_M_IA_COPY_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_M_VA_COPY_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_PH_IA_COPY_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

#endif

#if NBR_PHASES >= 2
	
	EEPROM_reader(DSP_CTRL_CAL_M_IB_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_M_VB_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_PH_IB_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_M_IB_COPY_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_M_VB_COPY_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_PH_IB_COPY_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );
	 
#endif

#if NBR_PHASES >= 3
	
	EEPROM_reader(DSP_CTRL_CAL_M_IC_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_M_VC_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_PH_IC_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_M_IC_COPY_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_M_VC_COPY_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

	EEPROM_reader(DSP_CTRL_CAL_PH_IC_COPY_ADDR, &register_value, 4);
	table_buffer[k++] = (uint8_t)( (register_value & 0x000000FF) >> 0 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x0000FF00) >> 8 );
	table_buffer[k++] = (uint8_t)( (register_value & 0x00FF0000) >> 16 );
	table_buffer[k++] = (uint8_t)( (register_value & 0xFF000000) >> 24 );

#endif

	return true;	
}

/**
 ******************************************************************************
 * \fn uint16_t SM_APP_usMt_25_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SM_APP_usMt_25_get_size(void) {

	return (uint16_t)MT_25_SIZE;
}

/**
 ******************************************************************************
 * \fn void SM_full_read_service(C1218_DLL_TYPE *c1218_dll_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SM_APP_vExecute_mfg_service(C1218_DLL_TYPE *c1218_dll_ptr) {
	
	uint16_t task_id;
	
	if ( c1218_dll_ptr->rx_eop == true ) {
		
		if ( (c1218_dll_ptr->rx_buffer[C1218_IDENTITY_INDEX] == 0) && (c1218_dll_ptr->rx_error == 0) ) {
		
			switch ( c1218_dll_ptr->rx_data[SERVICE_INDEX] ) {
		
				 case 0xFF:
					
					task_id  = (uint16_t)(c1218_dll_ptr->rx_data[SERVICE_INDEX + 1] << 0);
					task_id |= (uint16_t)(c1218_dll_ptr->rx_data[SERVICE_INDEX + 2] << 8);
					
					switch ( task_id ) {
						
						case 0x00:
							RST_HAL_vExecute_software_reset();
							c1218_dll_ptr->rx_eop = false;
							break;
							
						case 0x01:
							NIC_initialize();
							c1218_dll_ptr->rx_eop = false;
							break;
							
						case 0x02:
							OP_initialize();
							c1218_dll_ptr->rx_eop = false;
							break;
							
						case 0x03:
							I2C_HAL_vInitialize();
							c1218_dll_ptr->rx_eop = false;
							break;
						
						default:
							break;
					}
				
				default:
					break;
			}
		}
	}
}

/**
 ******************************************************************************
 * \fn void SM_nonvol_mem_failure(void)
 * \brief
 *
 * \param void
 * \retval uint8_t 3 : EEPROM 1 & 2 Ok
 *                 2 : EEPROM 1 Failure, EPPROM 2 Ok 
 *                 1 : EEPROM 1 Ok, EPPROM 2 Failure 					
 *                 0 : Failure
 ******************************************************************************
 */
uint8_t SM_APP_ucGet_nonvol_mem_state(void) {
	
	uint8_t test_byte = 0;
	uint8_t byte_compare = 0;
	uint8_t eeprom_ok = 0;
	
	test_byte = TEST_BYTE;
			
	EEPROM_writer( LAST_EEPROM_1_ADDR, &test_byte, 1);
	EEPROM_reader( LAST_EEPROM_1_ADDR, &byte_compare, 1);
			
	if ( byte_compare == TEST_BYTE ) {
		eeprom_ok |= EEPROM_1_BM;
	}
	else {
		eeprom_ok |= 0;
	}
			
	byte_compare = 0;
	EEPROM_writer( LAST_EEPROM_2_ADDR, &test_byte, 1);
	EEPROM_reader( LAST_EEPROM_2_ADDR, &byte_compare, 1);
			
	if ( byte_compare == TEST_BYTE ) {
		eeprom_ok |= EEPROM_2_BM;
	}
	else {
		eeprom_ok |= 0;
	}
				
	if ( eeprom_ok != EEPROM_TEST_SUCCESFULL ) {
		
	}
	
	return eeprom_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_1(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_1(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = GRAL_APP_usSt_1_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			GRAL_APP_bSt_1_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					SM_APP_vChange_table_state(MCP_ST_1_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_1_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_1_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_1_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_1_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_1_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_1_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_1_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_1_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_1_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = GRAL_APP_usSt_1_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_1_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_1_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_1_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_1_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_1_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_1_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? GRAL_APP_bSt_1_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_6(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_6(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = GRAL_APP_usSt_6_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
			
			GRAL_APP_bSt_6_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:

				SM_APP_vChange_table_state(MCP_ST_6_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
				is_ok = EEPROM_writer(MCP_ST_6_START_ADDR, &temp_table_backup, table_size);

				if ( is_ok ) {
					SM_APP_vChange_table_state(MCP_ST_6_PROG_STATE_ADDR, C1219_TABLE_FULL);
					EEPROM_writer(MCP_ST_6_NVM_CRC_ADDR, &calc_crc, 2);
				}
				break;

				case MP_120_CCP_AREA:

				SM_APP_vChange_table_state(CCP_ST_6_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
				is_ok = EEPROM_writer(CCP_ST_6_START_ADDR, &temp_table_backup, table_size);

				if ( is_ok ) {
					SM_APP_vChange_table_state(CCP_ST_6_PROG_STATE_ADDR, C1219_TABLE_FULL);
					EEPROM_writer(CCP_ST_6_NVM_CRC_ADDR, &calc_crc, 2);
				}
				break;

				default:
				break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_6_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_6_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = GRAL_APP_usSt_6_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_6_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_6_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_6_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_6_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_6_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_6_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;		
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? GRAL_APP_bSt_6_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_11(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_11(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = REGS_APP_usSt_11_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_11_corrupted_data() ) ) {
			
			REGS_APP_bSt_11_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:

					SM_APP_vChange_table_state(MCP_ST_11_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_11_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_11_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_11_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_11_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_11_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_11_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_11_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_11_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_11_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = REGS_APP_usSt_11_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_11_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_11_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_11_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_11_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_11_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_11_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? REGS_APP_bSt_11_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_13(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_13(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = ST_13_MAX_SIZE;

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size )  {
			
			REGS_APP_bSt_13_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:

					SM_APP_vChange_table_state(MCP_ST_13_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_13_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_13_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_13_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_13_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_13_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_13_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_13_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_13_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_13_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = ST_13_MAX_SIZE;

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_13_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_13_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_13_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_13_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_13_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_13_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? REGS_APP_bSt_13_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_21(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_21(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = REGS_APP_usSt_21_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_21_corrupted_data() ) ) {
	
			REGS_APP_bSt_21_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_ST_21_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_21_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_21_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_21_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_21_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_21_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_21_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_21_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_21_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_21_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = REGS_APP_usSt_21_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
			
			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:

				if ( SM_APP_ucGet_table_state(MCP_ST_21_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
					EEPROM_reader(MCP_ST_21_NVM_CRC_ADDR, &backup_crc, 2);
					EEPROM_reader(MCP_ST_21_START_ADDR, &temp_table_backup, table_size);
				}
				break;

				case MP_120_CCP_AREA:
				
				if ( SM_APP_ucGet_table_state(CCP_ST_21_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
					EEPROM_reader(CCP_ST_21_NVM_CRC_ADDR, &backup_crc, 2);
					EEPROM_reader(CCP_ST_21_START_ADDR, &temp_table_backup, table_size);
				}
				break;

				default:
				break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? REGS_APP_bSt_21_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_22(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_22(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = REGS_APP_usSt_22_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_22_corrupted_data() ) ) {
	
			REGS_APP_bSt_22_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_ST_22_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_22_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_22_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_22_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_22_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_22_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_22_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_22_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_22_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_22_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = REGS_APP_usSt_22_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_22_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_22_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_22_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_22_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_22_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_22_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? REGS_APP_bSt_22_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_27(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_27(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = REGS_APP_usSt_27_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_27_corrupted_data() ) ) {
	
			REGS_APP_bSt_27_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_ST_27_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_27_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_27_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_27_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_27_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_27_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_27_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_27_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_27_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_27_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = REGS_APP_usSt_27_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_27_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_27_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_27_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_27_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_27_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_27_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? REGS_APP_bSt_27_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_41(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_41(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = SECR_APP_usSt_41_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_41_corrupted_data() ) ) {
	
			SECR_APP_bSt_41_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_ST_41_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_41_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_41_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_41_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_41_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_41_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_41_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_41_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_41_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_41_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = SECR_APP_usSt_41_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_41_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_41_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_41_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_41_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_41_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_41_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? SECR_APP_bSt_41_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_42(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_42(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = SECR_APP_usSt_42_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_42_corrupted_data() ) ) {
	
			SECR_APP_bSt_42_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_ST_42_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_42_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_42_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_42_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_42_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_42_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_42_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_42_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_42_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_42_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = SECR_APP_usSt_42_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_42_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_42_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_42_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_42_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_42_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_42_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? SECR_APP_bSt_42_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_43(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_43(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = SECR_APP_usSt_43_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_43_corrupted_data() ) ) {
	
			SECR_APP_bSt_43_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_ST_43_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_43_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_43_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_43_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_43_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_43_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_43_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_43_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_43_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_43_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = SECR_APP_usSt_43_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_43_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_43_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_43_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_43_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_43_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_43_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? SECR_APP_bSt_43_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_44(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_44(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = SECR_APP_usSt_44_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_44_corrupted_data() ) ) {
	
			SECR_APP_bSt_44_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_ST_44_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_44_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_44_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_44_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_44_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_44_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_44_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_44_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_44_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_44_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = SECR_APP_usSt_44_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_44_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_44_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_44_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_44_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_44_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_44_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? SECR_APP_bSt_44_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_51(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_51(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = TOU_APP_usSt_51_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_51_corrupted_data() ) ) {
	
			TOU_APP_bSt_51_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_ST_51_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_51_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_51_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_51_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_51_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_51_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_51_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_51_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_51_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_51_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = TOU_APP_usSt_51_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_51_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_51_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_51_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_51_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_51_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_51_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? TOU_APP_bSt_51_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_53(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_53(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = TOU_APP_usSt_53_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
	
			TOU_APP_bSt_53_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_ST_53_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_53_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_53_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_53_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_53_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_53_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_53_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_53_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_53_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_53_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = TOU_APP_usSt_53_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_53_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_53_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_53_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_53_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_53_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_53_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? TOU_APP_bSt_53_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_54(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_54(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = TOU_APP_usSt_54_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
			
			TOU_APP_bSt_54_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:
				
					SM_APP_vChange_table_state(MCP_ST_54_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_54_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_54_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_54_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_54_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_54_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_54_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_54_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_54_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_54_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = TOU_APP_usSt_54_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_54_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_54_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_54_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_54_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_54_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_54_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? TOU_APP_bSt_54_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_61(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_61(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = LP_APP_usSt_61_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_61_corrupted_data() ) ) {
	
			LP_APP_bSt_61_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_ST_61_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_61_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_61_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_61_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_61_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_61_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_61_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_61_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_61_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_61_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = LP_APP_usSt_61_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
			
			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_61_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
						
						EEPROM_reader(MCP_ST_61_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_61_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
				
					if ( SM_APP_ucGet_table_state(CCP_ST_61_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
						
						EEPROM_reader(CCP_ST_61_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_61_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? LP_APP_bSt_61_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_62(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_62(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = LP_APP_usSt_62_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_62_corrupted_data() ) ) {
			
			LP_APP_bSt_62_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:
				
					SM_APP_vChange_table_state(MCP_ST_62_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_62_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_62_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_62_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_62_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_62_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_62_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_62_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_62_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_62_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = LP_APP_usSt_62_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_62_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_62_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_62_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_62_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_62_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_62_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? LP_APP_bSt_62_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_71(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_71(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = EVNT_APP_usSt_71_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_71_corrupted_data() ) ) {
	
			EVNT_APP_bSt_71_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_ST_71_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_71_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_71_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_71_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_71_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_71_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_71_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_71_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_71_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_71_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = EVNT_APP_usSt_71_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
			
			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_71_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_71_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_71_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
				
					if ( SM_APP_ucGet_table_state(CCP_ST_71_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
						
						EEPROM_reader(CCP_ST_71_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_71_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? EVNT_APP_bSt_71_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_73(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_73(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = EVNT_APP_usSt_73_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_73_corrupted_data() ) ) {
			
			EVNT_APP_bSt_73_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:
				
					SM_APP_vChange_table_state(MCP_ST_73_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_73_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_73_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_73_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_73_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_73_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_73_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_73_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_73_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_73_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = EVNT_APP_usSt_73_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
			
			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_73_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
						
						EEPROM_reader(MCP_ST_73_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_73_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
				
					if ( SM_APP_ucGet_table_state(CCP_ST_73_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
						
						EEPROM_reader(CCP_ST_73_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_73_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? EVNT_APP_bSt_73_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_st_75(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_st_75(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = EVNT_APP_usSt_75_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_st_75_corrupted_data() ) ) {
	
			EVNT_APP_bSt_75_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_ST_75_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_ST_75_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_ST_75_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_ST_75_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_ST_75_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_ST_75_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_ST_75_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_ST_75_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_st_75_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_st_75_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = EVNT_APP_usSt_75_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_ST_75_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_ST_75_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_ST_75_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_ST_75_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_ST_75_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_ST_75_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? EVNT_APP_bSt_75_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_mt_1(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_mt_1(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = SM_APP_usMt_1_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
	
			SM_APP_bMt_1_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_MT_1_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_MT_1_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_MT_1_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_MT_1_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_MT_1_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_MT_1_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_MT_1_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_MT_1_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_mt_1_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_mt_1_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = SM_APP_usMt_1_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_MT_1_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_MT_1_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_MT_1_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_MT_1_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_MT_1_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_MT_1_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? SM_APP_bMt_1_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_mt_3(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_mt_3(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = SM_APP_usMt_3_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
			
			SM_APP_bMt_3_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:
				
					SM_APP_vChange_table_state(MCP_MT_3_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_MT_3_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_MT_3_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_MT_3_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_MT_3_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_MT_3_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_MT_3_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_MT_3_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_mt_3_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_mt_3_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = SM_APP_usMt_3_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
			
			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_MT_3_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_MT_3_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_MT_3_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
				
					if ( SM_APP_ucGet_table_state(CCP_MT_3_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
						
						EEPROM_reader(CCP_MT_3_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_MT_3_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? SM_APP_bMt_3_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_mt_23(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_mt_23(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;

	table_size = MT_23_MAX_SIZE;

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
			
			SM_APP_bMt_23_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:
				
					SM_APP_vChange_table_state(MCP_MT_23_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_MT_23_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_MT_23_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_MT_23_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_MT_23_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_MT_23_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_MT_23_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_MT_23_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_mt_23_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_mt_23_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = MT_23_MAX_SIZE;

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_MT_23_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_MT_23_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_MT_23_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_MT_23_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_MT_23_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_MT_23_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? SM_APP_bMt_23_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_mt_53(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_mt_53(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = SM_APP_usMt_53_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
			
			SM_APP_bMt_53_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:
				
					SM_APP_vChange_table_state(MCP_MT_53_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_MT_53_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_MT_53_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_MT_53_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_MT_53_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_MT_53_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_MT_53_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_MT_53_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_mt_53_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_mt_53_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = SM_APP_usMt_53_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
		
			switch (backup_area_id) {
			
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_MT_53_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
					
						EEPROM_reader(MCP_MT_53_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_MT_53_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
			
					if ( SM_APP_ucGet_table_state(CCP_MT_53_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
				
						EEPROM_reader(CCP_MT_53_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_MT_53_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? SM_APP_bMt_53_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bBack_up_mt_73(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bBack_up_mt_73(uint16_t backup_area_id) {
	
	bool is_ok = false;
	uint16_t table_size;
	uint16_t calc_crc;
	
	table_size = EVNT_APP_usMt_73_get_size();

	if ( table_size > 0 ) {

		if ( ( sizeof(temp_table_backup) > table_size ) && ( !SM_APP_vIs_mt_73_corrupted_data() ) ) {
	
			EVNT_APP_bMt_73_execute_table_reading(&temp_table_backup[0]);
			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);

			switch (backup_area_id) {
		
				case MP_120_MCP_AREA:
		
					SM_APP_vChange_table_state(MCP_MT_73_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(MCP_MT_73_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(MCP_MT_73_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(MCP_MT_73_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				case MP_120_CCP_AREA:

					SM_APP_vChange_table_state(CCP_MT_73_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
					is_ok = EEPROM_writer(CCP_MT_73_START_ADDR, &temp_table_backup, table_size);

					if ( is_ok ) {
						SM_APP_vChange_table_state(CCP_MT_73_PROG_STATE_ADDR, C1219_TABLE_FULL);
						EEPROM_writer(CCP_MT_73_NVM_CRC_ADDR, &calc_crc, 2);
					}
					break;

				default:
					break;
			}
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SM_APP_bRestore_mt_73_from_backup(uint16_t backup_area_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SM_APP_bRestore_mt_73_from_backup(uint16_t backup_area_id) {

	bool is_ok = false;
	uint16_t table_size;
	uint16_t backup_crc = 0, calc_crc = 1;

	table_size = EVNT_APP_usMt_73_get_size();

	if ( table_size > 0 ) {

		if ( sizeof(temp_table_backup) > table_size ) {
			
			switch (backup_area_id) {
				
				case MP_120_MCP_AREA:

					if ( SM_APP_ucGet_table_state(MCP_MT_73_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
						
						EEPROM_reader(MCP_MT_73_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(MCP_MT_73_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				case MP_120_CCP_AREA:
				
					if ( SM_APP_ucGet_table_state(CCP_MT_73_PROG_STATE_ADDR) == C1219_TABLE_FULL ) {
						
						EEPROM_reader(CCP_MT_73_NVM_CRC_ADDR, &backup_crc, 2);
						EEPROM_reader(CCP_MT_73_START_ADDR, &temp_table_backup, table_size);
					}
					break;

				default:
					break;
			}

			calc_crc = CRC_crc16(&temp_table_backup[0], table_size);
			is_ok = (calc_crc == backup_crc) ? EVNT_APP_bMt_73_execute_table_writing(&temp_table_backup[0]) : false;
		}
	}
	else {
		is_ok = true;
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-04-10
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
