/**
 ******************************************************************************
 * \file regs.c
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
#include <asf.h>
#include "stdint.h"
#include "math.h"
#include "target.h"
#include "regs.h"
#include "util.h"
#include "tou.h"
#include "evnt.h"
#include "rtc.h"
#include "sm.h"
#include "watchdog.h"
#include "nic.h"
#include "c1218.h"
#include "crc.h"

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
static DEMAND_TYPE demand;
static ST_10_TYPE st_11;
static ST_13_TYPE st_13;
static ST_20_TYPE st_21;
static ST_22_TYPE st_22;
static ST_23_TYPE st_23;
static ST_27_TYPE st_27;

static ST_23_TYPE st_23_struct_copy;
static uint8_t st_23_copy[ST_23_MAX_SIZE];
static uint8_t table_11[20];

/*
 ************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ************************************************************************
 */
REGISTERS_OS_TYPE registers_os;

/**
 ************************************************************************
 * Function Prototypes for Private Functions with File Level Scope
 ************************************************************************
 */
static void REGS_APP_vStorage_rtos_task(void *pvParameters);
static void REGS_APP_vStorage_task(void);

static void REGS_APP_vProcess_demand_interval(PRESENT_DEMAND_INTERVAL_TYPE* demand_interval);
static uint16_t REGS_APP_vGet_present_demand_time_remining(DEMAND_INTERVAL_ID_TYPE int_nbr);

static void REGS_APP_vReset_demands_intervals(void);
static void REGS_APP_vLoad_demand_configuration(void);

static bool REGS_APP_bSt_11_validate_data(ST_10_TYPE *st_11_structure);
static void REGS_APP_vSt_11_read_eeprom_values(ST_10_TYPE *st_11_structure);
static void REGS_APP_vSt_11_write_eeprom_values(ST_10_TYPE *st_11_structure);
static void REGS_APP_vSt_11_to_array(ST_10_TYPE st_11_source, uint8_t *table_buffer);

static bool REGS_APP_bSt_21_validate_data(ST_20_TYPE *st_21_structure);
static void REGS_APP_vSt_21_read_eeprom_values(ST_20_TYPE *st_21_structure);
static void REGS_APP_vSt_21_write_eeprom_values(ST_20_TYPE *st_21_structure);
static void REGS_APP_vSt_21_to_array(ST_20_TYPE st_21_source, uint8_t *table_buffer);

static bool REGS_APP_bSt_22_validate_data(ST_22_TYPE *st_22_structure);
static void REGS_APP_vSt_22_read_eeprom_values(ST_22_TYPE *st_22_structure);
static void REGS_APP_vSt_22_write_eeprom_values(ST_22_TYPE *st_22_structure);
static void REGS_APP_vSt_22_to_array(ST_22_TYPE st_22_source, uint8_t *table_buffer);

static bool REGS_APP_vSt_23_read_eeprom_values(void);
static void REGS_APP_vSt_23_write_eeprom_values(ST_23_TYPE *st_23_structure);

static bool REGS_APP_bSt_27_validate_data(ST_27_TYPE *st_27_structure);
static void REGS_APP_vSt_27_read_eeprom_values(ST_27_TYPE *st_27_structure);
static void REGS_APP_vSt_27_write_eeprom_values(ST_27_TYPE *st_27_structure);
static void REGS_APP_vSt_27_to_array(ST_27_TYPE st_27_source, uint8_t *table_buffer);

/*
 ******************************************************************************
 * ROM Const Variables With File Level Scope
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */
/** 
 ******************************************************************************
 * \fn void REGS_APP_vInitialize(void)
 * \brief REGS Library Initialization
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vInitialize(void) {
	
	registers_os.task_execute = false;
	registers_os.time_after_last_backup = 0;
	memset( &demand, 0, sizeof(demand) );
	
	REGS_APP_vReset_demands_intervals();
	REGS_APP_vLoad_demand_configuration();
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_10_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_10_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;

	table_buffer[0] = PERM_SOURCE_FLAGS;
	table_buffer[1] = MAX_NBR_UOM_ENTRIES;
	table_buffer[2] = MAX_NBR_DEMAND_CTRL_ENTRIES;
	table_buffer[3] = MAX_DATA_CTRL_LENGTH;
	table_buffer[4] = MAX_NBR_DATA_CTRL_ENTRIES;
	table_buffer[5] = MAX_NBR_CONSTANTS_ENTRIES;
	table_buffer[6] = MAX_CONSTANTS_SELECTOR;
	table_buffer[7] = MAX_NBR_SOURCES;
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_10_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_10_get_size(void) {

	return (uint16_t) ST_10_SIZE;
}

/**
 ******************************************************************************
 * \fn boll REGS_APP_bLoad_st_11(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bLoad_st_11(void) {
		
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_11_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = REGS_APP_bSt_11_restore_default_values();
	}
	else {
		
		is_ok = REGS_APP_bSt_11_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_11_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_11_restore_default_values(void) {
	
	bool is_ok = false;
	ST_10_TYPE st_11_temp;
	
	st_11_temp.source_flags = DEFAULT_SOURCE_FLAGS;
	st_11_temp.nbr_oum_entries = DEFAULT_NBR_UOM_ENTRIES;
	st_11_temp.nbr_demand_ctrl_entries = DEFAULT_NBR_DEMAND_CTRL_ENTRIES;
	st_11_temp.data_ctrl_length = DEFAULT_DATA_CTRL_LENGTH;
	st_11_temp.nbr_data_ctrl_entries = DEFAULT_NBR_DATA_CTRL_ENTRIES;
	st_11_temp.nbr_constants_entries = DEFAULT_NBR_CONSTANTS_ENTRIES;
	st_11_temp.constants_selector = DEFAULT_CONSTANTS_SELECTOR;
	st_11_temp.nbr_sources = DEFAULT_NBR_SOURCES;
	
	is_ok = REGS_APP_bSt_11_validate_data(&st_11_temp);
	
	st_11 = st_11_temp;
	REGS_APP_vSt_11_write_eeprom_values(&st_11);
	uint16_t crc = SM_APP_usSt_11_ram_calculate_crc();
	SM_APP_vSt_11_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_11_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_11_refresh_ram(void) {
	
	ST_10_TYPE st_11_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		REGS_APP_vSt_11_read_eeprom_values(&st_11_temp);
		is_ok = REGS_APP_bSt_11_validate_data(&st_11_temp);
		
		if ( is_ok ) {
			st_11 = st_11_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_11 = st_11_temp;
		REGS_APP_vSt_11_write_eeprom_values(&st_11);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  REGS_APP_vCopy_st_11_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vCopy_st_11_ram(uint8_t* destination) {
	
	REGS_APP_vSt_11_to_array(st_11, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vCopy_st_11_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vCopy_st_11_nvm(uint8_t* destination) {
	
	ST_10_TYPE st_11_aux;
	
	REGS_APP_vSt_11_read_eeprom_values(&st_11_aux);
	REGS_APP_vSt_11_to_array(st_11_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_11_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_11_recover_nvm(void) {
	
	REGS_APP_vSt_11_write_eeprom_values(&st_11);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_11_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_11_recover_ram(void) {
	
	REGS_APP_vSt_11_read_eeprom_values(&st_11);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_11_to_array(ST_10_TYPE st_11_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_11_to_array(ST_10_TYPE st_11_source, uint8_t *table_buffer) {
	
	table_buffer[0] = st_11_source.source_flags;
	table_buffer[1] = st_11_source.nbr_oum_entries;
	table_buffer[2] = st_11_source.nbr_demand_ctrl_entries;
	table_buffer[3] = st_11_source.data_ctrl_length;
	table_buffer[4] = st_11_source.nbr_data_ctrl_entries;
	table_buffer[5] = st_11_source.nbr_constants_entries;
	table_buffer[6] = st_11_source.constants_selector;
	table_buffer[7] = st_11_source.nbr_sources;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_11_validate_data(ST_10_TYPE *st_11_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_11_validate_data(ST_10_TYPE *st_11_structure) {
	
	bool is_ok = true;
	
	for ( uint8_t i = 0; i < 8; i++) {
		if ( ( ( (PERM_SOURCE_FLAGS >> i) & 0x01 ) == 0 ) && ( ( (st_11_structure->source_flags >> i) & 0x01 ) == 1 ) ) {
			is_ok = false;
		}
	}
	st_11_structure->source_flags &= PERM_SOURCE_FLAGS;
	
	if (st_11_structure->nbr_oum_entries > MAX_NBR_UOM_ENTRIES) {
		st_11_structure->nbr_oum_entries = MAX_NBR_UOM_ENTRIES;
		is_ok = false;
	}
	
	if (st_11_structure->nbr_demand_ctrl_entries > MAX_NBR_DEMAND_CTRL_ENTRIES) {
		st_11_structure->nbr_demand_ctrl_entries = MAX_NBR_DEMAND_CTRL_ENTRIES;
		is_ok = false;
	}
	
	if (st_11_structure->data_ctrl_length > MAX_DATA_CTRL_LENGTH) {
		st_11_structure->data_ctrl_length = MAX_DATA_CTRL_LENGTH;
		is_ok = false;
	}
	
	if (st_11_structure->nbr_data_ctrl_entries > MAX_NBR_DATA_CTRL_ENTRIES) {
		st_11_structure->nbr_data_ctrl_entries = MAX_NBR_DATA_CTRL_ENTRIES;
		is_ok = false;
	}
	
	if (st_11_structure->nbr_constants_entries > MAX_NBR_CONSTANTS_ENTRIES) {
		st_11_structure->nbr_constants_entries = MAX_NBR_CONSTANTS_ENTRIES;
		is_ok = false;
	}
	
	if (st_11_structure->constants_selector > MAX_CONSTANTS_SELECTOR) {
		st_11_structure->constants_selector = MAX_CONSTANTS_SELECTOR;
		is_ok = false;
	}
	
	if (st_11_structure->nbr_sources > MAX_NBR_SOURCES) {
		st_11_structure->nbr_sources = MAX_NBR_SOURCES;
		is_ok = false;
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_11_read_eeprom_values(ST_10_TYPE *st_11_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_11_read_eeprom_values(ST_10_TYPE *st_11_structure) {	
	
	EEPROM_reader( ST_11_SOURCE_FLAGS_ADDR, &st_11_structure->source_flags, 1 );
	EEPROM_reader( ST_11_NBR_UOM_ENTRIES_ADDR, &st_11_structure->nbr_oum_entries, 1 );
	EEPROM_reader( ST_11_NBR_DEMAND_CTRL_ENTRIES_ADDR, &st_11_structure->nbr_demand_ctrl_entries, 1 );
	EEPROM_reader( ST_11_DATA_CTRL_LENGTH_ADDR, &st_11_structure->data_ctrl_length, 1 );
	EEPROM_reader( ST_11_NBR_DATA_CTRL_ENTRIES_ADDR, &st_11_structure->nbr_data_ctrl_entries, 1 );
	EEPROM_reader( ST_11_NBR_CONSTANTS_ENTRIES_ADDR, &st_11_structure->nbr_constants_entries, 1 );
	EEPROM_reader( ST_11_CONSTANTS_SELECTOR_ADDR, &st_11_structure->constants_selector, 1 );
	EEPROM_reader( ST_11_NBR_SOURCES_ADDR, &st_11_structure->nbr_sources, 1 );
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_11_write_eeprom_values(ST_10_TYPE *st_11_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_11_write_eeprom_values(ST_10_TYPE *st_11_structure) {
	
	SM_APP_vChange_table_state(ST_11_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( ST_11_SOURCE_FLAGS_ADDR, &st_11_structure->source_flags, 1 );
	EEPROM_writer( ST_11_NBR_UOM_ENTRIES_ADDR, &st_11_structure->nbr_oum_entries, 1 );
	EEPROM_writer( ST_11_NBR_DEMAND_CTRL_ENTRIES_ADDR, &st_11_structure->nbr_demand_ctrl_entries, 1 );
	EEPROM_writer( ST_11_DATA_CTRL_LENGTH_ADDR, &st_11_structure->data_ctrl_length, 1 );
	EEPROM_writer( ST_11_NBR_DATA_CTRL_ENTRIES_ADDR, &st_11_structure->nbr_data_ctrl_entries, 1 );
	EEPROM_writer( ST_11_NBR_CONSTANTS_ENTRIES_ADDR, &st_11_structure->nbr_constants_entries, 1 );
	EEPROM_writer( ST_11_CONSTANTS_SELECTOR_ADDR, &st_11_structure->constants_selector, 1 );
	EEPROM_writer( ST_11_NBR_SOURCES_ADDR, &st_11_structure->nbr_sources, 1 );
	
	SM_APP_vChange_table_state(ST_11_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_11_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_11_execute_table_reading(uint8_t *table_buffer) {
	
	REGS_APP_vSt_11_to_array(st_11, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_11_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_11_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	ST_10_TYPE st_11_temp;
	
	st_11_temp.source_flags = table_buffer[k++];
	st_11_temp.nbr_oum_entries = table_buffer[k++];
	st_11_temp.nbr_demand_ctrl_entries = table_buffer[k++];
	st_11_temp.data_ctrl_length = table_buffer[k++];
	st_11_temp.nbr_data_ctrl_entries = table_buffer[k++];
	st_11_temp.nbr_constants_entries = table_buffer[k++];
	st_11_temp.constants_selector = table_buffer[k++];
	st_11_temp.nbr_sources = table_buffer[k++];
	
	is_ok = REGS_APP_bSt_11_validate_data(&st_11_temp);
	
	if ( is_ok ) {
		st_11 = st_11_temp;
		REGS_APP_vSt_11_write_eeprom_values(&st_11);
		uint16_t crc = SM_APP_usSt_11_ram_calculate_crc();
		SM_APP_vSt_11_write_crc(crc);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_11_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_11_get_size(void) {

	return (uint16_t) ST_11_SIZE;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_12_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_12_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;
	uint8_t i = 0;
	uint32_t st_12[145] = { 
							// Summations
							UOM_SUM_Wh_Del, UOM_SUM_Wh_Rec,	UOM_SUM_Wh_Net,	UOM_SUM_Wh_Sum,		
						  	UOM_SUM_Wh_Del_A, UOM_SUM_Wh_Del_B,	UOM_SUM_Wh_Del_C,
							UOM_SUM_Wh_Rec_A, UOM_SUM_Wh_Rec_B, UOM_SUM_Wh_Rec_C, 
							UOM_SUM_VARh_Q1, UOM_SUM_VARh_Q2, UOM_SUM_VARh_Q3, UOM_SUM_VARh_Q4,		
						  	UOM_SUM_VARh_Del, UOM_SUM_VARh_Rec,	UOM_SUM_VARh_Net, UOM_SUM_VARh_Sum,	
						  	UOM_SUM_VARh_Q1_Q4,	UOM_SUM_VARh_Q2_Q3,	
						  	UOM_SUM_VAh_Del, UOM_SUM_VAh_Rec, UOM_SUM_VAh_Net, UOM_SUM_VAh_Sum,		
						  	UOM_SUM_VAh_Q1,	UOM_SUM_VAh_Q2,	UOM_SUM_VAh_Q3,	UOM_SUM_VAh_Q4,		
							
							// Load Profile	
							UOM_LP_Wh_Del, UOM_LP_Wh_Rec, UOM_LP_Wh_Net, UOM_LP_Wh_Sum,
							UOM_LP_Wh_Del_A, UOM_LP_Wh_Del_B, UOM_LP_Wh_Del_C,
							UOM_LP_Wh_Rec_A, UOM_LP_Wh_Rec_B, UOM_LP_Wh_Rec_C,
							UOM_LP_VARh_Q1,	UOM_LP_VARh_Q2,	UOM_LP_VARh_Q3,	UOM_LP_VARh_Q4,
							UOM_LP_VARh_Del, UOM_LP_VARh_Rec, UOM_LP_VARh_Net, UOM_LP_VARh_Sum,
							UOM_LP_VARh_Q1_Q4, UOM_LP_VARh_Q2_Q3,
							UOM_LP_VAh_Del,	UOM_LP_VAh_Rec,	UOM_LP_VAh_Net,	UOM_LP_VAh_Sum,
							UOM_LP_VAh_Q1, UOM_LP_VAh_Q2, UOM_LP_VAh_Q3, UOM_LP_VAh_Q4,
							
							// Average Demand
							UOM_AD_W_Del, UOM_AD_W_Rec,	UOM_AD_W_Net, UOM_AD_W_Sum,
							UOM_AD_W_Del_A,	UOM_AD_W_Del_B,	UOM_AD_W_Del_C,
							UOM_AD_W_Rec_A,	UOM_AD_W_Rec_B,	UOM_AD_W_Rec_C,
							UOM_AD_VAR_Q1, UOM_AD_VAR_Q2, UOM_AD_VAR_Q3, UOM_AD_VAR_Q4,
							UOM_AD_VAR_Del, UOM_AD_VAR_Rec, UOM_AD_VAR_Net,	UOM_AD_VAR_Sum,
							UOM_AD_VAR_Q1_Q4, UOM_AD_VAR_Q2_Q3,
							UOM_AD_VA_Del, UOM_AD_VA_Rec, UOM_AD_VA_Net, UOM_AD_VA_Sum,
							UOM_AD_VA_Q1, UOM_AD_VA_Q2,	UOM_AD_VA_Q3, UOM_AD_VA_Q4,
							
							// Instantaneous Demand						  
							UOM_ID_W_Inst, UOM_ID_W_Inst_A,	UOM_ID_W_Inst_B, UOM_ID_W_Inst_C,
							UOM_ID_VAR_inst, UOM_ID_VAR_inst_A,	UOM_ID_VAR_inst_B, UOM_ID_VAR_inst_C,
							UOM_ID_VA_inst, UOM_ID_VA_inst_A, UOM_ID_VA_inst_B,	UOM_ID_VA_inst_C,

							// Instantaneous Values
							UOM_IV_V_A,	UOM_IV_V_B,	UOM_IV_V_C,
							UOM_IV_I_A, UOM_IV_I_B,	UOM_IV_I_C,
							UOM_IV_V_A_angle, UOM_IV_V_B_angle,	UOM_IV_V_C_angle,
							UOM_IV_I_A_angle, UOM_IV_I_B_angle,	UOM_IV_I_C_angle,
							UOM_IV_PF_angle, UOM_IV_PF_A_angle,	UOM_IV_PF_B_angle, UOM_IV_PF_C_angle,
							UOM_IV_PF, UOM_IV_PF_A,	UOM_IV_PF_B, UOM_IV_PF_C,
							UOM_IV_f,
							
							// Present Demand
							UOM_PD_W_Del, UOM_PD_W_Rec, UOM_PD_W_Net, UOM_PD_W_Sum,
							UOM_PD_W_Del_A, UOM_PD_W_Del_B, UOM_PD_W_Del_C,
							UOM_PD_W_Rec_A,	UOM_PD_W_Rec_B,	UOM_PD_W_Rec_C,
							UOM_PD_VAR_Q1, UOM_PD_VAR_Q2, UOM_PD_VAR_Q3, UOM_PD_VAR_Q4,
							UOM_PD_VAR_Del,	UOM_PD_VAR_Rec,	UOM_PD_VAR_Net,	UOM_PD_VAR_Sum,
							UOM_PD_VAR_Q1_Q4, UOM_PD_VAR_Q2_Q3,
							UOM_PD_VA_Del, UOM_PD_VA_Rec, UOM_PD_VA_Net, UOM_PD_VA_Sum,
							UOM_PD_VA_Q1, UOM_PD_VA_Q2,	UOM_PD_VA_Q3, UOM_PD_VA_Q4
						  };
	
	for ( i = 0; i < 145; i++) {
		convert_32_bit_to_byte_array(st_12[i], &table_buffer[k]);
		k += 4;
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_12_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_12_get_size(void) {

	return (uint16_t) 4*145;			// PARAMTERS_SIZE * NBR_PARAMETERS
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bLoad_st_13(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bLoad_st_13(void) {
		
	if ( SM_APP_ucGet_table_state(ST_13_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		REGS_APP_bSt_13_restore_default_values();
	}
	else {
		// There is no shadow ram for this table
	}
	
	return true;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_bSt_13_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_bSt_13_restore_default_values(void) {
	
	uint8_t data[ST_13_MAX_SIZE];
	uint8_t null_data = 0;
	
	data[0] = (uint8_t)(DEFAULT_RESET_EXCLUSION);
	data[1] = (uint8_t)(DEFAULT_P_FAIL_RECOGNTN_TM);
	data[2] = (uint8_t)(DEFAULT_P_FAIL_EXCLUSION);
	data[3] = (uint8_t)(DEFAULT_COLD_LOAD_PICKUP);
	data[4] = (uint8_t)(DEFAULT_SUB_INT);
	data[5] = (uint8_t)(DEFAULT_INT_MULTIPLIER);
	data[6] = (uint8_t)(DEFAULT_INT_LENGTH);
	data[7] = (uint8_t)(DEFAULT_INT_LENGTH >> 8);
	
	SM_APP_vChange_table_state(ST_13_PROG_STATE_ADDR, C1219_TABLE_EMPTY);

	EEPROM_writer( ST_13_RESET_EXCLUSION_ADDR, &data[0], 1 );
	EEPROM_writer( ST_13_P_FAIL_RECOGNTN_TM_ADDR, &data[1], 1 );
	EEPROM_writer( ST_13_P_FAIL_EXCLUSION_ADDR, &data[2], 1 );
	EEPROM_writer( ST_13_COLD_LOAD_PICKUP_ADDR, &data[3], 1 );
	EEPROM_writer( ST_13_SUB_INT_ADDR, &data[4], 1 );
	EEPROM_writer( ST_13_INT_MULTIPLIER_ADDR, &data[5], 1 );
	EEPROM_writer( ST_13_INT_LENGTH_ADDR, &data[6], 2 );
	
	SM_APP_vChange_table_state(ST_13_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_13_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_13_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;
	uint8_t i = 0;
	
	if ( (st_11.source_flags & RESET_EXCLUDE_FLAG_bm) == RESET_EXCLUDE_FLAG_bm ) {
		
		EEPROM_reader( ST_13_RESET_EXCLUSION_ADDR, &table_buffer[k], 1 );
		k += 1;
	}
	
	if ( (st_11.source_flags & PF_EXCLUDE_FLAG_bm) == PF_EXCLUDE_FLAG_bm ) {
		
		EEPROM_reader( ST_13_P_FAIL_RECOGNTN_TM_ADDR, &table_buffer[k], 1 );
		k += 1;
		
		EEPROM_reader( ST_13_P_FAIL_EXCLUSION_ADDR, &table_buffer[k], 1 );
		k += 1;
		
		EEPROM_reader( ST_13_COLD_LOAD_PICKUP_ADDR, &table_buffer[k], 1 );
		k += 1;
	}
	
	for ( i = 0; i < st_11.nbr_demand_ctrl_entries; i++ ) {
		
		if ( (st_11.source_flags & SLIDING_DEMAND_FLAG_bm) == SLIDING_DEMAND_FLAG_bm ) {
			
			EEPROM_reader( ST_13_SUB_INT_ADDR, &table_buffer[k], 1 );
			k += 1;
			
			EEPROM_reader( ST_13_INT_MULTIPLIER_ADDR, &table_buffer[k], 1 );
			k += 1;
		}
		else {
			
			EEPROM_reader( ST_13_INT_LENGTH_ADDR, &table_buffer[k], 2 );
			k += 2;
		}
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_13_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_13_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t i = 0;
	uint8_t k = 0;
	
	if ( (st_11.source_flags & RESET_EXCLUDE_FLAG_bm) == RESET_EXCLUDE_FLAG_bm ) {

		EEPROM_writer( ST_13_RESET_EXCLUSION_ADDR, &table_buffer[k], 1 );
		k += 1;
	}
	
	if ( (st_11.source_flags & PF_EXCLUDE_FLAG_bm) == PF_EXCLUDE_FLAG_bm ) {
		
		EEPROM_writer( ST_13_P_FAIL_RECOGNTN_TM_ADDR, &table_buffer[k], 1 );
		k += 1;
		
		EEPROM_writer( ST_13_P_FAIL_EXCLUSION_ADDR, &table_buffer[k], 1 );
		k += 1;
		
		EEPROM_writer( ST_13_COLD_LOAD_PICKUP_ADDR, &table_buffer[k], 1 );
		k += 1;
	}
	
	//for ( i = 0; i < st_11.nbr_demand_ctrl_entries; i++ ) {
		
		if ( (st_11.source_flags & SLIDING_DEMAND_FLAG_bm) == SLIDING_DEMAND_FLAG_bm ) {
			
			EEPROM_writer( ST_13_SUB_INT_ADDR, &table_buffer[k], 1 );
			k += 1;
			
			EEPROM_writer( ST_13_INT_MULTIPLIER_ADDR, &table_buffer[k], 1 );
			k += 1;
		}
		else {
			EEPROM_writer( ST_13_INT_LENGTH_ADDR, &table_buffer[k], 2 );
			k += 2;
		}
	//}
	
	//is_ok = REGS_APP_bSt_13_validate_data(&st_13_temp);
	//
	//if ( is_ok ) {
	//	st_13 = st_13_temp;
	//	REGS_APP_bSt_13_write_eeprom_values(&st_13);
	//}
	
	is_ok = true;
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_13_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_13_get_size(void) {

	uint16_t table_size = 0;
	
	if ( (st_11.source_flags & RESET_EXCLUDE_FLAG_bm) == RESET_EXCLUDE_FLAG_bm ) {
		table_size += 1;
	}
	
	if ( (st_11.source_flags & PF_EXCLUDE_FLAG_bm) == PF_EXCLUDE_FLAG_bm ) {
		table_size += 3;
	}
	
	table_size += ( 2 * st_11.nbr_demand_ctrl_entries);
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_15_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_15_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;
	uint8_t i = 0;
	
	for ( i = 0; i < st_11.nbr_constants_entries; i++) {
		convert_64_bit_to_byte_array(DEFAULT_MULTIPLIER, &table_buffer[k]);
		k += NI_FMAT1_SIZE;
		
		convert_64_bit_to_byte_array(DEFAULT_OFFSET, &table_buffer[k]);
		k += NI_FMAT1_SIZE;
		
		if ( (st_11.source_flags & SET1_PRESENT_FLAG_bm) == SET1_PRESENT_FLAG_bm ) {
			table_buffer[k++] = DEFAULT_SET_1_FLAGS;
			
			convert_64_bit_to_byte_array(DEFAULT_SET_1_RATIO_F1, &table_buffer[k]);
			k += NI_FMAT1_SIZE;
			
			convert_64_bit_to_byte_array(DEFAULT_SET_1_RATIO_P1, &table_buffer[k]);
			k += NI_FMAT1_SIZE;
		}
		
		if ( (st_11.source_flags & SET2_PRESENT_FLAG_bm) == SET2_PRESENT_FLAG_bm ) {
			table_buffer[k++] = DEFAULT_SET_2_FLAGS;
			
			convert_64_bit_to_byte_array(DEFAULT_SET_2_RATIO_F1, &table_buffer[k]);
			k += NI_FMAT1_SIZE;
			
			convert_64_bit_to_byte_array(DEFAULT_SET_2_RATIO_P1, &table_buffer[k]);
			k += NI_FMAT1_SIZE;
		}
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_15_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_15_get_size(void) {

	uint8_t i;
	uint16_t table_size;
	
	table_size = 0;
	table_size += (2*NI_FMAT1_SIZE);
	
	if ( (st_11.source_flags & SET1_PRESENT_FLAG_bm) == SET1_PRESENT_FLAG_bm ) {
		table_size += (2*NI_FMAT1_SIZE + 1);
	}
	
	if ( (st_11.source_flags & SET2_PRESENT_FLAG_bm) == SET2_PRESENT_FLAG_bm ) {
		table_size += (2*NI_FMAT1_SIZE + 1);
	}
	
	table_size *= st_11.nbr_constants_entries;
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_16_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_16_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;
	uint16_t i = 0;
	
	for ( i = 0; i < 28; i++ ) {
		table_buffer[k++] = DEFAULT_SOURCE_SUMMATIONS;
	}
	
	for ( i = 0; i < 22; i++ ) {
		table_buffer[k++] = 0;
	}
	
	for ( i = 0; i < 28; i++ ) {
		table_buffer[k++] = DEFAULT_SOURCE_LOAD_PROFILE;
	}
	
	for ( i = 0; i < 22; i++ ) {
		table_buffer[k++] = 0;
	}
	
	for ( i = 0; i < 28; i++ ) {
		table_buffer[k++] = DEFAULT_SOURCE_AVERAGE_DEMAND;
	}
	
	for ( i = 0; i < 22; i++ ) {
		table_buffer[k++] = 0;
	}
	
	for ( i = 0; i < 12; i++ ) {
		table_buffer[k++] = DEFAULT_SOURCE_INSTANTANEOUS_DEMAND;
	}
	
	for ( i = 0; i < 18; i++ ) {
		table_buffer[k++] = 0;
	}

	for ( i = 0; i < 21; i++ ) {
		table_buffer[k++] = DEFAULT_SOURCE_INSTANTANEOUS_VALUE;
	}
	
	for ( i = 0; i < 9; i++ ) {
		table_buffer[k++] = 0;
	}
	
	for ( i = 0; i < 28; i++ ) {
		table_buffer[k++] = DEFAULT_SOURCE_PRESENT_DEMAND;
	}
	
	for ( i = 0; i < 18; i++ ) {
		table_buffer[k++] = 0;
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_16_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_16_get_size(void) {
	
	return (uint16_t) ST_16_SIZE;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_20_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_20_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;

	table_buffer[0] = PERM_REG_FUNC1_FLAGS;
	table_buffer[1] = PERM_REG_FUNC2_FLAGS;
	table_buffer[2] = MAX_NBR_SELF_READS;
	table_buffer[3] = MAX_NBR_SUMMATIONS;
	table_buffer[4] = MAX_NBR_DEMANDS;
	table_buffer[5] = MAX_NBR_COIN_VALUES;
	table_buffer[6] = MAX_NBR_OCCUR;
	table_buffer[7] = MAX_NBR_TIERS;
	table_buffer[8] = MAX_NBR_PRESENT_DEMANDS;
	table_buffer[9] = MAX_NBR_PRESENT_VALUES;
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_20_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_20_get_size(void) {

	return (uint16_t) ST_20_SIZE;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bLoad_st_21(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bLoad_st_21(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_21_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = REGS_APP_bSt_21_restore_default_values();
	}
	else {
		
		is_ok = REGS_APP_bSt_21_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_21_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_21_restore_default_values(void) {
	
	bool is_ok = false;
	ST_20_TYPE st_21_temp;
	
	st_21_temp.reg_func1_flags = DEFAULT_REG_FUNC1_FLAGS;
	st_21_temp.reg_func2_flags = DEFAULT_REG_FUNC2_FLAGS;
	st_21_temp.nbr_self_reads = DEFAULT_NBR_SELF_READS;
	st_21_temp.nbr_summations = DEFAULT_NBR_SUMMATIONS;
	st_21_temp.nbr_demands = DEFAULT_NBR_DEMANDS;
	st_21_temp.nbr_coin_values = DEFAULT_NBR_COIN_VALUES;
	st_21_temp.nbr_occur = DEFAULT_NBR_OCCUR;
	st_21_temp.nbr_tiers = DEFAULT_NBR_TIERS;
	st_21_temp.nbr_present_demands = DEFAULT_NBR_PRESENT_DEMANDS;
	st_21_temp.nbr_present_values = DEFAULT_NBR_PRESENT_VALUES;
	
	is_ok = REGS_APP_bSt_21_validate_data(&st_21_temp);
	
	st_21 = st_21_temp;
	REGS_APP_vSt_21_write_eeprom_values(&st_21);
	uint16_t crc = SM_APP_usSt_21_ram_calculate_crc();
	SM_APP_vSt_21_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_21_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_21_refresh_ram(void) {
	
	ST_20_TYPE st_21_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		REGS_APP_vSt_21_read_eeprom_values(&st_21_temp);
		is_ok = REGS_APP_bSt_21_validate_data(&st_21_temp);
		
		if ( is_ok ) {
			st_21 = st_21_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_21 = st_21_temp;
		REGS_APP_vSt_21_write_eeprom_values(&st_21);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  REGS_APP_vCopy_st_21_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vCopy_st_21_ram(uint8_t* destination) {
	
	REGS_APP_vSt_21_to_array(st_21, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vCopy_st_21_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vCopy_st_21_nvm(uint8_t* destination) {
	
	ST_20_TYPE st_21_aux;
	
	REGS_APP_vSt_21_read_eeprom_values(&st_21_aux);
	REGS_APP_vSt_21_to_array(st_21_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_21_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_21_recover_nvm(void) {
	
	REGS_APP_vSt_21_write_eeprom_values(&st_21);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_21_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_21_recover_ram(void) {
	
	REGS_APP_vSt_21_read_eeprom_values(&st_21);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_21_to_array(ST_20_TYPE st_21_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_21_to_array(ST_20_TYPE st_21_source, uint8_t *table_buffer) {
	
	table_buffer[0] = st_21_source.reg_func1_flags;
	table_buffer[1] = st_21_source.reg_func2_flags;
	table_buffer[2] = st_21_source.nbr_self_reads;
	table_buffer[3] = st_21_source.nbr_summations;
	table_buffer[4] = st_21_source.nbr_demands;
	table_buffer[5] = st_21_source.nbr_coin_values;
	table_buffer[6] = st_21_source.nbr_occur;
	table_buffer[7] = st_21_source.nbr_tiers;
	table_buffer[8] = st_21_source.nbr_present_demands;
	table_buffer[9] = st_21_source.nbr_present_values;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_21_validate_data(ST_20_TYPE *st_21_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_21_validate_data(ST_20_TYPE *st_21_structure) {
	
	bool is_ok = true;
	uint8_t i;

	for ( i = 0; i < 8; i++) {
		if ( ( ( (PERM_REG_FUNC1_FLAGS >> i) & 0x01 ) == 0 ) && ( ( (st_21_structure->reg_func1_flags >> i) & 0x01 ) == 1 ) ) {
			is_ok = false;	
		}
	}
	st_21_structure->reg_func1_flags &= PERM_REG_FUNC1_FLAGS;
	
	for ( i = 0; i < 8; i++) {
		if ( ( ( (PERM_REG_FUNC2_FLAGS >> i) & 0x01 ) == 0 ) && ( ( (st_21_structure->reg_func2_flags >> i) & 0x01 ) == 1 ) ) {
			is_ok = false;
		}
	}
	st_21_structure->reg_func2_flags &= PERM_REG_FUNC2_FLAGS;
	
	if ( st_21_structure->nbr_self_reads > MAX_NBR_SELF_READS ) {
		st_21_structure->nbr_self_reads = MAX_NBR_SELF_READS;
		is_ok = false;
	}
	
	if ( st_21_structure->nbr_summations > MAX_NBR_SUMMATIONS ) {
		st_21_structure->nbr_summations = MAX_NBR_SUMMATIONS;
		is_ok = false;
	}

	if ( st_21_structure->nbr_demands > MAX_NBR_DEMANDS ) {
		st_21_structure->nbr_demands = MAX_NBR_DEMANDS;
		is_ok = false;
	}

	if ( st_21_structure->nbr_coin_values > MAX_NBR_COIN_VALUES ) {
		st_21_structure->nbr_coin_values = MAX_NBR_COIN_VALUES;
		is_ok = false;
	}
	
	if ( st_21_structure->nbr_occur > MAX_NBR_OCCUR ) {
		st_21_structure->nbr_occur = MAX_NBR_OCCUR;
		is_ok = false;
	}
	
	if ( st_21_structure->nbr_tiers > MAX_NBR_TIERS ){
		st_21_structure->nbr_tiers = MAX_NBR_TIERS;
		is_ok = false;
	}
	
	if ( st_21_structure->nbr_present_demands > MAX_NBR_PRESENT_DEMANDS ) {
		st_21_structure->nbr_present_demands = MAX_NBR_PRESENT_DEMANDS;
		is_ok = false;
	}
	
	if ( st_21_structure->nbr_present_values > MAX_NBR_PRESENT_VALUES ) {
		st_21_structure->nbr_present_values = MAX_NBR_PRESENT_VALUES;
		is_ok = false;
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_21_read_eeprom_values(ST_20_TYPE *st_21_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_21_read_eeprom_values(ST_20_TYPE *st_21_structure) {	
	
	EEPROM_reader( ST_21_REG_FUNC1_BFLD_ADDR, &st_21_structure->reg_func1_flags, 1 );
	EEPROM_reader( ST_21_REG_FUNC2_BFLD_ADDR, &st_21_structure->reg_func2_flags, 1 );
	EEPROM_reader( ST_21_NBR_SELF_READS_ADDR, &st_21_structure->nbr_self_reads, 1 );
	EEPROM_reader( ST_21_NBR_SUMMATIONS_ADDR, &st_21_structure->nbr_summations, 1 );
	EEPROM_reader( ST_21_NBR_DEMANDS_ADDR, &st_21_structure->nbr_demands, 1 );
	EEPROM_reader( ST_21_NBR_COIN_VALUES_ADDR, &st_21_structure->nbr_coin_values, 1 );
	EEPROM_reader( ST_21_NBR_OCCUR_ADDR, &st_21_structure->nbr_occur, 1 );
	EEPROM_reader( ST_21_NBR_TIERS_ADDR, &st_21_structure->nbr_tiers, 1 );
	EEPROM_reader( ST_21_NBR_PRESENT_DEMANDS_ADDR, &st_21_structure->nbr_present_demands, 1 );
	EEPROM_reader( ST_21_NBR_PRESENT_VALUES_ADDR, &st_21_structure->nbr_present_values, 1 );
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_21_write_eeprom_values(ST_20_TYPE *st_21_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_21_write_eeprom_values(ST_20_TYPE *st_21_structure)
{
	SM_APP_vChange_table_state(ST_21_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( ST_21_REG_FUNC1_BFLD_ADDR, &st_21_structure->reg_func1_flags, 1 );
	EEPROM_writer( ST_21_REG_FUNC2_BFLD_ADDR, &st_21_structure->reg_func2_flags, 1 );
	EEPROM_writer( ST_21_NBR_SELF_READS_ADDR, &st_21_structure->nbr_self_reads, 1 );
	EEPROM_writer( ST_21_NBR_SUMMATIONS_ADDR, &st_21_structure->nbr_summations, 1 );
	EEPROM_writer( ST_21_NBR_DEMANDS_ADDR, &st_21_structure->nbr_demands, 1 );
	EEPROM_writer( ST_21_NBR_COIN_VALUES_ADDR, &st_21_structure->nbr_coin_values, 1 );
	EEPROM_writer( ST_21_NBR_OCCUR_ADDR, &st_21_structure->nbr_occur, 1 );
	EEPROM_writer( ST_21_NBR_TIERS_ADDR, &st_21_structure->nbr_tiers, 1 );
	EEPROM_writer( ST_21_NBR_PRESENT_DEMANDS_ADDR, &st_21_structure->nbr_present_demands, 1 );
	EEPROM_writer( ST_21_NBR_PRESENT_VALUES_ADDR, &st_21_structure->nbr_present_values, 1 );
	
	SM_APP_vChange_table_state(ST_21_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_21_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_21_execute_table_reading(uint8_t *table_buffer) {
	
	REGS_APP_vSt_21_to_array(st_21, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_21_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_21_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	ST_20_TYPE st_21_temp;
	
	st_21_temp.reg_func1_flags = table_buffer[k++];
	st_21_temp.reg_func2_flags = table_buffer[k++];
	st_21_temp.nbr_self_reads = table_buffer[k++];
	st_21_temp.nbr_summations = table_buffer[k++];
	st_21_temp.nbr_demands = table_buffer[k++];
	st_21_temp.nbr_coin_values = table_buffer[k++];
	st_21_temp.nbr_occur = table_buffer[k++];
	st_21_temp.nbr_tiers = table_buffer[k++];
	st_21_temp.nbr_present_demands = table_buffer[k++];
	st_21_temp.nbr_present_values = table_buffer[k++];
	
	is_ok = REGS_APP_bSt_21_validate_data(&st_21_temp);
	
	if ( is_ok ) {
		st_21 = st_21_temp;
		REGS_APP_vSt_21_write_eeprom_values(&st_21);
		uint16_t crc = SM_APP_usSt_21_ram_calculate_crc();
		SM_APP_vSt_21_write_crc(crc);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_21_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_21_get_size(void) {

	return (uint16_t)ST_21_SIZE;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bLoad_st_22(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bLoad_st_22(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_22_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = REGS_APP_bSt_22_restore_default_values();
	}
	else {
		
		is_ok = REGS_APP_bSt_22_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_22_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_22_restore_default_values(void) {
	
	bool is_ok = false;
	ST_22_TYPE st_22_temp;
	uint8_t i;
	
	memset( &st_22_temp, 0, sizeof(st_22_temp) );

	for (i = 0; i < st_21.nbr_summations; i++) {
		st_22_temp.summation_select[i] = SM_WH_DEL_ID + i;
	}
	
	for (i = 0; i < st_21.nbr_demands; i++) {
		st_22_temp.min_or_max_flags |= (uint8_t)(1 << i);
		st_22_temp.demand_select[i] = AD_W_DEL_ID + i;
	}
	
	is_ok = REGS_APP_bSt_22_validate_data(&st_22_temp);
	
	st_22 = st_22_temp;
	REGS_APP_vSt_22_write_eeprom_values(&st_22);
	uint16_t crc = SM_APP_usSt_22_ram_calculate_crc();
	SM_APP_vSt_22_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_22_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_22_refresh_ram(void) {
	
	ST_22_TYPE st_22_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		REGS_APP_vSt_22_read_eeprom_values(&st_22_temp);
		is_ok = REGS_APP_bSt_22_validate_data(&st_22_temp);
		
		if ( is_ok ) {
			st_22 = st_22_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_22 = st_22_temp;
		REGS_APP_vSt_22_write_eeprom_values(&st_22);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  REGS_APP_vCopy_st_22_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vCopy_st_22_ram(uint8_t* destination) {
	
	REGS_APP_vSt_22_to_array(st_22, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vCopy_st_22_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vCopy_st_22_nvm(uint8_t* destination) {
	
	ST_22_TYPE st_22_aux;
	
	REGS_APP_vSt_22_read_eeprom_values(&st_22_aux);
	REGS_APP_vSt_22_to_array(st_22_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_22_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_22_recover_nvm(void) {
	
	REGS_APP_vSt_22_write_eeprom_values(&st_22);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_22_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_22_recover_ram(void) {
	
	REGS_APP_vSt_22_read_eeprom_values(&st_22);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_22_to_array(ST_22_TYPE st_22_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_22_to_array(ST_22_TYPE st_22_source, uint8_t *table_buffer) {

	uint8_t k = 0;
	uint8_t i = 0;
	
	for ( i = 0; i < st_21.nbr_summations; i++ ) {
		table_buffer[k++] = st_22_source.summation_select[i];
	}
	
	for ( i = 0; i < st_21.nbr_demands; i++ ) {
		table_buffer[k++] = st_22_source.demand_select[i];
	}
	
	table_buffer[k++] = st_22_source.min_or_max_flags;
	
	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
		table_buffer[k++] = st_22_source.coincident_select[i];
	}
	
	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
		table_buffer[k++] = st_22_source.coin_demand_assoc[i];
	}
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_22_validate_data(ST_22_TYPE *st_22_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_22_validate_data(ST_22_TYPE *st_22_structure) {
	
	bool is_ok = true;
	uint8_t i;
	
	for ( i = 0; i < st_21.nbr_summations; i++ ) {
		
		if ( (st_22_structure->summation_select[i] < SUMMATIONS_INITIAL_ID) || (st_22_structure->summation_select[i] >= SM_NUMBER_OF_IDS) ) {
			is_ok = false;
			st_22_structure->summation_select[i] = SUMMATIONS_INITIAL_ID;
		}
	}
	
	for ( i = 0; i < st_21.nbr_demands; i++ ) {
		
		if ( (st_22_structure->demand_select[i] < AVERAGE_DEMAND_INITIAL_ID) || (st_22_structure->demand_select[i] >= AD_NUMBER_OF_IDS) ) {
			is_ok = false;
			st_22_structure->demand_select[i] = AVERAGE_DEMAND_INITIAL_ID;
		}
	}
	
	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
		
		if ( (st_22_structure->coincident_select[i] < AVERAGE_DEMAND_INITIAL_ID) || (st_22_structure->coincident_select[i] >= AD_NUMBER_OF_IDS) ) {
			is_ok = false;
			st_22_structure->coincident_select[i] = AVERAGE_DEMAND_INITIAL_ID;
		}
		
		if ( (st_22_structure->coin_demand_assoc[i] < AVERAGE_DEMAND_INITIAL_ID) || (st_22_structure->coin_demand_assoc[i] >= AD_NUMBER_OF_IDS) ) {
			is_ok = false;
			st_22_structure->coin_demand_assoc[i] = AVERAGE_DEMAND_INITIAL_ID;
		}
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_22_read_eeprom_values(ST_22_TYPE *st_22_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_22_read_eeprom_values(ST_22_TYPE *st_22_structure) {
	
	uint8_t i;
	
	for ( i = 0; i < st_21.nbr_summations; i++ ) {
		EEPROM_reader( ST_22_SUMMATION_SELECT_ADDR + i, &st_22_structure->summation_select[i], 1 );
	}
	
	for ( i = 0; i < st_21.nbr_demands ; i++ ) {
		EEPROM_reader( ST_22_DEMAND_SELECT_ADDR + i, &st_22_structure->demand_select[i], 1 );
	}

	EEPROM_reader( ST_22_MIN_OR_MAX_FLAGS_ADDR, &st_22_structure->min_or_max_flags, 1 );
	
	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
		EEPROM_reader( ST_22_COINCIDENT_SELECT_ADDR + i, &st_22_structure->coincident_select[i], 1 );
	}
	
	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
		EEPROM_reader( ST_22_COIN_DEMAND_ASSOC_ADDR + i, &st_22_structure->coin_demand_assoc[i], 1 );
	}
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_22_write_eeprom_values(ST_22_TYPE *st_22_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_22_write_eeprom_values(ST_22_TYPE *st_22_structure)
{
	uint8_t i;
	
	SM_APP_vChange_table_state(ST_22_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	for ( i = 0; i < st_21.nbr_summations; i++ ) {
		EEPROM_writer( ST_22_SUMMATION_SELECT_ADDR + i, &st_22_structure->summation_select[i], 1 );
	}
	
	for ( i = 0; i < st_21.nbr_demands; i++ ) {
		EEPROM_writer( ST_22_DEMAND_SELECT_ADDR + i, &st_22_structure->demand_select[i], 1 );
	}

	EEPROM_writer( ST_22_MIN_OR_MAX_FLAGS_ADDR, &st_22_structure->min_or_max_flags, 1 );
	
	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
		EEPROM_writer( ST_22_COINCIDENT_SELECT_ADDR + i, &st_22_structure->coincident_select[i], 1 );
	}
	
	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
		EEPROM_writer( ST_22_COIN_DEMAND_ASSOC_ADDR + i, &st_22_structure->coin_demand_assoc[i], 1 );
	}
	
	SM_APP_vChange_table_state(ST_22_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_22_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_22_execute_table_reading(uint8_t *table_buffer) {
	
	REGS_APP_vSt_22_to_array(st_22, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_22_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_22_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	uint8_t i = 0;
	ST_22_TYPE st_22_temp;
	
	for ( i = 0; i < st_21.nbr_summations; i++ ) {
		st_22_temp.summation_select[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < st_21.nbr_demands; i++ ) {
		st_22_temp.demand_select[i] = table_buffer[k++];
	}

	st_22_temp.min_or_max_flags = table_buffer[k++];
	
	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
		st_22_temp.coincident_select[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
		st_22_temp.coin_demand_assoc[i] = table_buffer[k++];
	}
	
	is_ok = REGS_APP_bSt_22_validate_data(&st_22_temp);
	
	if ( is_ok ) {
		st_22 = st_22_temp;
		REGS_APP_vSt_22_write_eeprom_values(&st_22);
		uint16_t crc = SM_APP_usSt_22_ram_calculate_crc();
		SM_APP_vSt_22_write_crc(crc);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_22_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_22_get_size(void) {

	uint16_t table_size;
	
	table_size = st_21.nbr_summations + st_21.nbr_demands + 1 + st_21.nbr_coin_values + st_21.nbr_coin_values;
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bLoad_st_23(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bLoad_st_23(void) {
	
	static bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		is_ok = REGS_APP_vSt_23_read_eeprom_values();
	
		if ( is_ok ) {
			break;
		}
	}
		
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_vSt_23_read_eeprom_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_vSt_23_read_eeprom_values(void) {
	
	bool is_ok = true;
	uint8_t i, j;
	static uint64_t ull_auxvar;
	
	memset( &st_23, 0, sizeof(st_23) );
	
	/* NBR_DEMAND_RESETS */
	EEPROM_reader( ST_23_NBR_DEMAND_RESETS_ADDR, (uint8_t*)&st_23.nbr_demand_resets, 1 );
	
	if ( st_23.nbr_demand_resets == 0xFF ) {
		st_23.nbr_demand_resets = 0;
		is_ok = false;
	}
	
	/* SUMMATIONS */
	for ( i = 0; i < st_21.nbr_summations; i++ ) {
		
		ull_auxvar = 0;
		EEPROM_reader( ST_23_TOT_SUMMATIONS_ADDR + i*NI_FMAT1_SIZE, (uint8_t*)&ull_auxvar, NI_FMAT1_SIZE );
		st_23.tot_data_block.summations[i] = (double)(ull_auxvar);
		
		if ( (uint64_t)st_23.tot_data_block.summations[i] >= (uint64_t)MAX_REGISTER_VALUE) {
			st_23.tot_data_block.summations[i] = 0;
			is_ok = false;
		}
	}
	
	/* DEMANDS */
	for ( i = 0; i < st_21.nbr_demands; i++ ) {
		
		EEPROM_reader( ST_23_TOT_DEMANDS_EVENT_TIME_ADDR + i*TM_FMAT_SIZE, (uint8_t*)&st_23.tot_data_block.demands[i].event_time[0], TM_FMAT_SIZE );
		
		if (st_23.tot_data_block.demands[i].event_time[0] >= (uint32_t)0xFFFFFFFF) {
			st_23.tot_data_block.demands[i].event_time[0] = 0;
			is_ok = false;
		}
		
		EEPROM_reader( ST_23_TOT_DEMANDS_CUM_DEMAND_ADDR + i*NI_FMAT1_SIZE, (uint8_t*)&st_23.tot_data_block.demands[i].cum_demand, NI_FMAT1_SIZE );
			
		if (st_23.tot_data_block.demands[i].cum_demand >= (uint64_t)0x0000FFFFFFFFFFFF) {
			st_23.tot_data_block.demands[i].cum_demand = 0;
			is_ok = false;
		}
		
		EEPROM_reader( ST_23_TOT_DEMANDS_CONT_CUM_DEMAND_ADDR + i*NI_FMAT1_SIZE, (uint8_t*)&st_23.tot_data_block.demands[i].cont_cum_demand, NI_FMAT1_SIZE );
			
		if (st_23.tot_data_block.demands[i].cont_cum_demand >= (uint64_t)0x0000FFFFFFFFFFFF) {
			st_23.tot_data_block.demands[i].cont_cum_demand = 0;
			is_ok = false;
		}

		EEPROM_reader( ST_23_TOT_DEMANDS_DEMAND_ADDR + i*NI_FMAT2_SIZE, (uint8_t*)&st_23.tot_data_block.demands[i].demand[0], NI_FMAT2_SIZE );
		
		if (st_23.tot_data_block.demands[i].demand[0] >= (uint64_t)0x000000FFFFFFFFFF) {
			st_23.tot_data_block.demands[i].demand[0] = 0;
			is_ok = false;
		}
	}
	
	/* COINCIDENTS */
	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {

		EEPROM_reader( ST_23_TOT_COINCIDENTS_ADDR + i*NI_FMAT2_SIZE, (uint8_t*)&st_23.tot_data_block.coincidents[i].coincident_values[0], NI_FMAT2_SIZE );
		
		if (st_23.tot_data_block.coincidents[i].coincident_values[0] >= (uint64_t)0x000000FFFFFFFFFF) {
			st_23.tot_data_block.coincidents[i].coincident_values[0] = 0;
			is_ok = false;
		}
	}
	
	/* TIERS */
	for ( i = 0; i < st_21.nbr_tiers; i++ ) {
		
		/* SUMMATIONS */
		for ( j = 0; j < st_21.nbr_summations; j++ ) {
			
			ull_auxvar = 0;
			EEPROM_reader( ST_23_TIER_SUMMATIONS_ADDR + (i * NI_FMAT1_SIZE * MAX_NBR_SUMMATIONS) + (j * NI_FMAT1_SIZE), (uint8_t*)&ull_auxvar, NI_FMAT1_SIZE );
			st_23.tier_data_block[i].summations[j] = (double)(ull_auxvar);
			
			if ( (uint64_t)st_23.tier_data_block[i].summations[j] >= (uint64_t)MAX_REGISTER_VALUE) {
				st_23.tier_data_block[i].summations[j] = 0;
				is_ok = false;
			}
		}
		
		/* DEMANDS */
		for ( j = 0; j < st_21.nbr_demands; j++ ) {

			EEPROM_reader( ST_23_TIER_DEMANDS_EVENT_TIME_ADDR + (i * TM_FMAT_SIZE * MAX_NBR_DEMANDS) + (j * TM_FMAT_SIZE),(uint8_t*) &st_23.tier_data_block[i].demands[j].event_time[0], TM_FMAT_SIZE );
				
			if (st_23.tier_data_block[i].demands[j].event_time[0] >= (uint32_t)0xFFFFFFFF) {
				st_23.tier_data_block[i].demands[j].event_time[0] = 0;
				is_ok = false;
			}
			
			EEPROM_reader( ST_23_TIER_DEMANDS_CUM_DEMAND_ADDR + (i * NI_FMAT1_SIZE * MAX_NBR_DEMANDS) + (j * NI_FMAT1_SIZE), (uint8_t*)&st_23.tier_data_block[i].demands[j].cum_demand, NI_FMAT1_SIZE );
				
			if (st_23.tier_data_block[i].demands[j].cum_demand >= (uint64_t)0x0000FFFFFFFFFFFF) {
				st_23.tier_data_block[i].demands[j].cum_demand = 0;
				is_ok = false;
			}
			
			EEPROM_reader( ST_23_TIER_DEMANDS_CONT_CUM_DEMAND_ADDR + (i * NI_FMAT1_SIZE * MAX_NBR_DEMANDS) + (j * NI_FMAT1_SIZE), (uint8_t*)&st_23.tier_data_block[i].demands[j].cont_cum_demand, NI_FMAT1_SIZE );
				
			if (st_23.tier_data_block[i].demands[j].cont_cum_demand >= (uint64_t)0x0000FFFFFFFFFFFF) {
				st_23.tier_data_block[i].demands[j].cont_cum_demand = 0;
				is_ok = false;
			}
			
			EEPROM_reader( ST_23_TIER_DEMANDS_DEMAND_ADDR + (i * NI_FMAT2_SIZE * MAX_NBR_DEMANDS) + (j * NI_FMAT2_SIZE), (uint8_t*)&st_23.tier_data_block[i].demands[j].demand[0], NI_FMAT2_SIZE );
			
			if (st_23.tier_data_block[i].demands[j].demand[0] >= (uint64_t)0x000000FFFFFFFFFF) {
				st_23.tier_data_block[i].demands[j].demand[0] = 0;
				is_ok = false;
			}
		}
		
		/* COINCIDENTS */
		for ( j = 0; j < st_21.nbr_coin_values; j++ ) {
			
			EEPROM_reader( ST_23_TIER_COINCIDENTS_ADDR + (i * NI_FMAT2_SIZE * MAX_NBR_COIN_VALUES) + (j * NI_FMAT2_SIZE), (uint8_t*)&st_23.tier_data_block[i].coincidents[j].coincident_values[0], NI_FMAT2_SIZE );
			
			if (st_23.tier_data_block[i].coincidents[j].coincident_values[0] >= (uint64_t)0x000000FFFFFFFFFF) {
				st_23.tier_data_block[i].coincidents[j].coincident_values[0] = 0;
				is_ok = false;
			}
		}
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_23_write_eeprom_values(ST_23_TYPE *st_23_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_23_write_eeprom_values(ST_23_TYPE *st_23_structure) {
	
	uint8_t i, j;
	
	EEPROM_writer( ST_23_NBR_DEMAND_RESETS_ADDR, &st_23_structure->nbr_demand_resets, 1 );
		
	for ( i = 0; i < st_21.nbr_summations; i++ ) {
		
		EEPROM_writer( ST_23_TOT_SUMMATIONS_ADDR + i*NI_FMAT1_SIZE, &st_23_structure->tot_data_block.summations[i], NI_FMAT1_SIZE );
	}
	
	for ( i = 0; i < st_21.nbr_demands; i++ ) {
		
		EEPROM_writer( ST_23_TOT_DEMANDS_EVENT_TIME_ADDR + i*TM_FMAT_SIZE, &st_23_structure->tot_data_block.demands[i].event_time[0], TM_FMAT_SIZE );
		EEPROM_writer( ST_23_TOT_DEMANDS_CUM_DEMAND_ADDR + i*NI_FMAT1_SIZE, &st_23_structure->tot_data_block.demands[i].cum_demand, NI_FMAT1_SIZE );
		EEPROM_writer( ST_23_TOT_DEMANDS_CONT_CUM_DEMAND_ADDR + i*NI_FMAT1_SIZE, &st_23_structure->tot_data_block.demands[i].cont_cum_demand, NI_FMAT1_SIZE );
		EEPROM_writer( ST_23_TOT_DEMANDS_DEMAND_ADDR + i*NI_FMAT2_SIZE, &st_23_structure->tot_data_block.demands[i].demand[0], NI_FMAT2_SIZE );
	}

	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {

		EEPROM_writer( ST_23_TOT_COINCIDENTS_ADDR + i*NI_FMAT2_SIZE, &st_23_structure->tot_data_block.coincidents[i].coincident_values[0], NI_FMAT2_SIZE );
	}
	
	for ( i = 0; i < st_21.nbr_tiers; i++ ) {
		
		for ( j = 0; j < st_21.nbr_summations; j++ ) {

			EEPROM_writer( ST_23_TIER_SUMMATIONS_ADDR + (i * NI_FMAT1_SIZE * MAX_NBR_SUMMATIONS) + (j * NI_FMAT1_SIZE), &st_23_structure->tier_data_block[i].summations[j], NI_FMAT1_SIZE );
		}
		
		for ( j = 0; j < st_21.nbr_demands; j++ ) {

			EEPROM_writer( ST_23_TIER_DEMANDS_EVENT_TIME_ADDR + (i * TM_FMAT_SIZE * MAX_NBR_DEMANDS) + (j * TM_FMAT_SIZE), &st_23_structure->tier_data_block[i].demands[j].event_time[0], TM_FMAT_SIZE );
			EEPROM_writer( ST_23_TIER_DEMANDS_CUM_DEMAND_ADDR + (i * NI_FMAT1_SIZE * MAX_NBR_DEMANDS) + (j * NI_FMAT1_SIZE), &st_23_structure->tier_data_block[i].demands[j].cum_demand, NI_FMAT1_SIZE );
			EEPROM_writer( ST_23_TIER_DEMANDS_CONT_CUM_DEMAND_ADDR + (i * NI_FMAT1_SIZE * MAX_NBR_DEMANDS) + (j * NI_FMAT1_SIZE), &st_23_structure->tier_data_block[i].demands[j].cont_cum_demand, NI_FMAT1_SIZE );
			EEPROM_writer( ST_23_TIER_DEMANDS_DEMAND_ADDR + (i * NI_FMAT2_SIZE * MAX_NBR_DEMANDS) + (j * NI_FMAT2_SIZE), &st_23_structure->tier_data_block[i].demands[j].demand[0], NI_FMAT2_SIZE );
		}
		
		for ( j = 0; j < st_21.nbr_coin_values; j++ ) {
			
			EEPROM_writer( ST_23_TIER_COINCIDENTS_ADDR + (i * NI_FMAT2_SIZE * MAX_NBR_COIN_VALUES) + (j * NI_FMAT2_SIZE), &st_23_structure->tier_data_block[i].coincidents[j].coincident_values[0], NI_FMAT2_SIZE );
		}
	}
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vClear_st_23(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vClear_st_23(void) {
	
	memset( &st_23, 0, sizeof(st_23) );
	
	if ( !meter.ed_mode.test_mode.enabled )
	{
		REGS_APP_vSt_23_write_eeprom_values(&st_23);
	}
		
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_23_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_23_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t i, j = 0;
	uint16_t k = 0;
	uint64_t data;

	if ( (st_21.reg_func1_flags & DEMAND_RESET_CTR_FLAG_bm) == DEMAND_RESET_CTR_FLAG_bm) {
			
		table_buffer[k++] = st_23.nbr_demand_resets;
	}
 	
	for ( i = 0; i < st_21.nbr_summations; i++ ) {
				 
		data = (uint64_t)st_23.tot_data_block.summations[i];
		memcpy(&table_buffer[k], &data, NI_FMAT1_SIZE);
		k += NI_FMAT1_SIZE;
 	}
	
 	for ( i = 0; i < st_21.nbr_demands; i++ ) {				 
			
		if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm ) {
			
			memcpy(&table_buffer[k], &st_23.tot_data_block.demands[i].event_time[0], TM_FMAT_SIZE);
			k += TM_FMAT_SIZE;
		}
		
		if ( (st_21.reg_func1_flags & CUM_DEMAND_FLAG_bm) == CUM_DEMAND_FLAG_bm ) {
			
			memcpy(&table_buffer[k], &st_23.tot_data_block.demands[i].cum_demand, NI_FMAT1_SIZE);															
			k += NI_FMAT1_SIZE;
		}

		if ( (st_21.reg_func1_flags & CONT_CUM_DEMAND_FLAG_bm) == CONT_CUM_DEMAND_FLAG_bm ) {
			
			memcpy(&table_buffer[k], &st_23.tot_data_block.demands[i].cont_cum_demand, NI_FMAT1_SIZE);															
			k += NI_FMAT1_SIZE;
		}
		
		memcpy(&table_buffer[k], &st_23.tot_data_block.demands[i].demand[0], NI_FMAT2_SIZE);	
		k += NI_FMAT2_SIZE;
	}
			
	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
		
		memcpy(&table_buffer[k], &st_23.tot_data_block.coincidents[i], NI_FMAT2_SIZE);
		k += NI_FMAT2_SIZE;
	}

	for ( j = 0; j < st_21.nbr_tiers; j++ ) {

		for ( i = 0; i < st_21.nbr_summations; i++ ) {
			
			data = (uint64_t)(st_23.tier_data_block[j].summations[i]);
			memcpy(&table_buffer[k], &data, NI_FMAT1_SIZE);								
			k += NI_FMAT1_SIZE;
		}

		for ( i = 0; i < st_21.nbr_demands; i++ ) {
			
			if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm ) {
				
				memcpy(&table_buffer[k], &st_23.tier_data_block[j].demands[i].event_time[0], TM_FMAT_SIZE);						  
				k += TM_FMAT_SIZE;
			}
			
			if ( (st_21.reg_func1_flags & CUM_DEMAND_FLAG_bm) == CUM_DEMAND_FLAG_bm ) {
				
				memcpy(&table_buffer[k], &st_23.tier_data_block[j].demands[i].cum_demand, NI_FMAT1_SIZE);													
				k += NI_FMAT1_SIZE;
			}
			
			if ( (st_21.reg_func1_flags & CONT_CUM_DEMAND_FLAG_bm) == CONT_CUM_DEMAND_FLAG_bm ) {
				
				memcpy(&table_buffer[k], &st_23.tier_data_block[j].demands[i].cont_cum_demand, NI_FMAT1_SIZE);													
				k += NI_FMAT1_SIZE;
			}
			
			memcpy(&table_buffer[k], &st_23.tier_data_block[j].demands[i].demand[0], NI_FMAT2_SIZE);															
			k += NI_FMAT2_SIZE;
		}
		
		for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
			
			memcpy(&table_buffer[k], &st_23.tier_data_block[j].coincidents[i], NI_FMAT2_SIZE);														
			k += NI_FMAT2_SIZE;
		}
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_23_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_23_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t i, j;
	uint16_t k = 0;
	uint64_t data;
	ST_23_TYPE st_23_temp;
	
	memset( &st_23_temp, 0, sizeof(st_23_temp) ); 

	if ( (st_21.reg_func1_flags & DEMAND_RESET_CTR_FLAG_bm) == DEMAND_RESET_CTR_FLAG_bm) {
		
		st_23_temp.nbr_demand_resets = table_buffer[k++];
	}

	for ( i = 0; i < st_21.nbr_summations; i++ ) {
		
		data = 0;
		memcpy(&data, &table_buffer[k], NI_FMAT1_SIZE);
		st_23_temp.tot_data_block.summations[i] = (double)data;
		k += NI_FMAT1_SIZE;
	}

	for ( i = 0; i < st_21.nbr_demands; i++ ) {				 
			
		if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm ) {	
			
			memcpy(&st_23_temp.tot_data_block.demands[i].event_time[0], &table_buffer[k], TM_FMAT_SIZE);
			k += TM_FMAT_SIZE;
		}
	 
		if ( (st_21.reg_func1_flags & CUM_DEMAND_FLAG_bm) == CUM_DEMAND_FLAG_bm ) {
			
			memcpy(&st_23_temp.tot_data_block.demands[i].cum_demand, &table_buffer[k], NI_FMAT1_SIZE);
			k += NI_FMAT1_SIZE;
		}
	
		if ( (st_21.reg_func1_flags & CONT_CUM_DEMAND_FLAG_bm) == CONT_CUM_DEMAND_FLAG_bm ) {
			
			memcpy(&st_23_temp.tot_data_block.demands[i].cont_cum_demand, &table_buffer[k], NI_FMAT1_SIZE);
			k += NI_FMAT1_SIZE;
		}

		memcpy(&st_23_temp.tot_data_block.demands[i].demand[0], &table_buffer[k], NI_FMAT2_SIZE);
		k += NI_FMAT2_SIZE;
	}
	
	for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
		
		memcpy(&st_23_temp.tot_data_block.coincidents[i], &table_buffer[k], NI_FMAT2_SIZE);
		k += NI_FMAT2_SIZE;
	}
	
	for ( j = 0; j < st_21.nbr_tiers; j++ ) {
	
		for ( i = 0; i < st_21.nbr_summations; i++ ) {
			
			data = 0;
			memcpy(&data, &table_buffer[k], NI_FMAT1_SIZE);
			st_23_temp.tier_data_block[j].summations[i] = (double)data;
			k += NI_FMAT1_SIZE;
		}
		
		for ( i = 0; i < st_21.nbr_demands; i++ ) {
		
			if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm ) {
				
				memcpy(&st_23_temp.tier_data_block[j].demands[i].event_time[0], &table_buffer[k], TM_FMAT_SIZE);
				k += TM_FMAT_SIZE;
			}	
			
			if ( (st_21.reg_func1_flags & CUM_DEMAND_FLAG_bm) == CUM_DEMAND_FLAG_bm ) {
				
				memcpy(&st_23_temp.tier_data_block[j].demands[i].cum_demand, &table_buffer[k], NI_FMAT1_SIZE);
				k += NI_FMAT1_SIZE;
			}
			
			if ( (st_21.reg_func1_flags & CONT_CUM_DEMAND_FLAG_bm) == CONT_CUM_DEMAND_FLAG_bm ) {
				
				memcpy(&st_23_temp.tier_data_block[j].demands[i].cont_cum_demand, &table_buffer[k], NI_FMAT1_SIZE);
				k += NI_FMAT1_SIZE;
			}
			
			memcpy(&st_23_temp.tier_data_block[j].demands[i].demand[0], &table_buffer[k], NI_FMAT2_SIZE);
			k += NI_FMAT2_SIZE;			
		}
		
		for ( i = 0; i < st_21.nbr_coin_values; i++ ) {
			
			memcpy(&st_23_temp.tier_data_block[j].coincidents[i], &table_buffer[k], NI_FMAT2_SIZE);
			k += NI_FMAT2_SIZE;
		}
	}
	
	is_ok = true;
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_23_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_23_get_size(void) {

	uint16_t table_size = 0;
	uint8_t nbrDemandsResets_size = 0;
	uint16_t dataBlock_size = 0;
	uint16_t totDataBlock_size = 0;
	uint16_t tierDataBlock_size = 0;
	uint8_t eventTime_size = 0;
	uint8_t cumDemand_size = 0;
	uint8_t contCumDemand_size = 0;
	
	if ( (st_21.reg_func1_flags & DEMAND_RESET_CTR_FLAG_bm) == DEMAND_RESET_CTR_FLAG_bm) {
		nbrDemandsResets_size = 1;
	}
	
	if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm ){
		eventTime_size = 4;
	}
	
	if ( (st_21.reg_func1_flags & CUM_DEMAND_FLAG_bm) == CUM_DEMAND_FLAG_bm ) {
		cumDemand_size = 6;
	}
	
	if ( (st_21.reg_func1_flags & CONT_CUM_DEMAND_FLAG_bm) == CONT_CUM_DEMAND_FLAG_bm ) {
		contCumDemand_size = 6;
	}
	
	dataBlock_size = 6*st_21.nbr_summations + st_21.nbr_demands*(eventTime_size + cumDemand_size + contCumDemand_size + 5) + 5*st_21.nbr_coin_values;
	
	totDataBlock_size = dataBlock_size;
	tierDataBlock_size = st_21.nbr_tiers * dataBlock_size;
	
	table_size = nbrDemandsResets_size + totDataBlock_size + tierDataBlock_size;
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vClear_st_24(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vClear_st_24(void) {
	
	uint64_t default_value = 0;
	
	EEPROM_writer( ST_24_END_DATE_TIME_ADDR, (uint8_t *)&default_value, TM_FMAT_SIZE );
	EEPROM_writer( ST_24_SEASON_ADDR, (uint8_t *)&default_value, 1 );
	
	memset( &st_23_copy, 0, sizeof(st_23_copy) );
	EEPROM_writer( ST_24_PREV_SEASON_REG_DATA_ADDR, st_23_copy, ST_23_MAX_SIZE );
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_24_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_24_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;
	uint16_t st_23_size;
	
	st_23_size = REGS_APP_usSt_23_get_size();
	
	/*---------------------------------------------------------------------------*\
	 * REGISTER_INFO
	\*---------------------------------------------------------------------------*/
	if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm ) {
		EEPROM_reader( ST_24_END_DATE_TIME_ADDR, &table_buffer[k], TM_FMAT_SIZE );
		k += TM_FMAT_SIZE;
	}
	
	if ( (st_21.reg_func1_flags & SEASON_INFO_FIELD_FLAG_bm) == SEASON_INFO_FIELD_FLAG_bm ) {
		EEPROM_reader( ST_24_SEASON_ADDR, &table_buffer[k], 1 );
		k += 1;
	}
		
	/*---------------------------------------------------------------------------*\
	 * PREV_SEASON_REG_DATA
	\*---------------------------------------------------------------------------*/
	EEPROM_reader( ST_24_PREV_SEASON_REG_DATA_ADDR, &table_buffer[k], st_23_size );
	k += st_23_size;
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_24_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_24_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;
	uint16_t st_23_size;
	
	st_23_size = REGS_APP_usSt_23_get_size();
	
	/*---------------------------------------------------------------------------*\
	 * REGISTER_INFO
	\*---------------------------------------------------------------------------*/
	if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm ) {
		EEPROM_writer( ST_24_END_DATE_TIME_ADDR, &table_buffer[k], TM_FMAT_SIZE );
		k += TM_FMAT_SIZE;
	}
	
	if ( (st_21.reg_func1_flags & SEASON_INFO_FIELD_FLAG_bm) == SEASON_INFO_FIELD_FLAG_bm ) {
		EEPROM_writer( ST_24_SEASON_ADDR, &table_buffer[k], 1 );
		k += 1;
	}
		
	/*---------------------------------------------------------------------------*\
	 * PREV_SEASON_REG_DATA
	\*---------------------------------------------------------------------------*/
	 EEPROM_writer( ST_24_PREV_SEASON_REG_DATA_ADDR, &table_buffer[k], st_23_size );
	
	is_ok = true;
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_24_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_24_get_size(void) {

	uint16_t table_size = 0;
	
	if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm) {
		table_size += 4;
	}
	
	if ( (st_21.reg_func1_flags & SEASON_INFO_FIELD_FLAG_bm) == SEASON_INFO_FIELD_FLAG_bm) {
		table_size += 1;
	}
	
	table_size += REGS_APP_usSt_23_get_size();
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vClear_st_25(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vClear_st_25(void) {
	
	uint64_t default_value = 0;
	
	EEPROM_writer( ST_25_END_DATE_TIME_ADDR, (uint8_t *)&default_value, TM_FMAT_SIZE );
	EEPROM_writer( ST_25_SEASON_ADDR, (uint8_t *)&default_value, 1 );
	
	memset( &st_23_copy, 0, sizeof(st_23_copy) );
	EEPROM_writer( ST_25_PREV_DEMAND_RESET_DATA_ADDR, st_23_copy, ST_23_MAX_SIZE );
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_25_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_25_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;
	uint16_t st_23_size;
	
	st_23_size = REGS_APP_usSt_23_get_size();
	
	/*---------------------------------------------------------------------------*\
	 * REGISTER_INFO
	\*---------------------------------------------------------------------------*/
	if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm ) {
		EEPROM_reader( ST_25_END_DATE_TIME_ADDR, &table_buffer[k], TM_FMAT_SIZE );
		k += TM_FMAT_SIZE;
	}
	
	if ( (st_21.reg_func1_flags & SEASON_INFO_FIELD_FLAG_bm) == SEASON_INFO_FIELD_FLAG_bm ) {
		EEPROM_reader( ST_25_SEASON_ADDR, &table_buffer[k], 1 );
		k += 1;
	}
		
	/*---------------------------------------------------------------------------*\
	 * PREV_DEMAND_RESET_DATA
	\*---------------------------------------------------------------------------*/
	EEPROM_reader( ST_25_PREV_DEMAND_RESET_DATA_ADDR, &table_buffer[k], st_23_size );
	k += st_23_size;
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_25_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_25_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;
	uint16_t st_23_size;
	
	st_23_size = REGS_APP_usSt_23_get_size();
	
	/*---------------------------------------------------------------------------*\
	 * REGISTER_INFO
	\*---------------------------------------------------------------------------*/
	if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm ) {
		EEPROM_writer( ST_25_END_DATE_TIME_ADDR, &table_buffer[k], TM_FMAT_SIZE );
		k += TM_FMAT_SIZE;
	}
	
	if ( (st_21.reg_func1_flags & SEASON_INFO_FIELD_FLAG_bm) == SEASON_INFO_FIELD_FLAG_bm ) {
		EEPROM_writer( ST_25_SEASON_ADDR, &table_buffer[k], 1 );
		k += 1;
	}
		
	/*---------------------------------------------------------------------------*\
	 * PREV_DEMAND_RESET_DATA
	\*---------------------------------------------------------------------------*/
	 EEPROM_writer( ST_25_PREV_DEMAND_RESET_DATA_ADDR, &table_buffer[k], st_23_size );
	
	is_ok = true;
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_25_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_25_get_size(void) {
	
	return REGS_APP_usSt_24_get_size();
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bLoad_st_26(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bLoad_st_26(void) {
		
	if ( SM_APP_ucGet_table_state(ST_26_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		REGS_APP_bSt_26_restore_default_values();
	}
	else {
		// There is no shadow ram for this table
	}
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_26_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_26_restore_default_values(void) {
	
	uint8_t data[6];
	
	data[0] = (uint8_t)(ST_26_DEFAULT_LIST_STATUS);
	data[1] = (uint8_t)(ST_26_DEFAULT_NBR_VALID_ENTRIES);
	data[2] = (uint8_t)(ST_26_DEFAULT_LAST_ENTRY_ELEMENT);
	data[3] = (uint8_t)(ST_26_DEFAULT_LAST_ENTRY_SEQ_NBR);
	data[4] = (uint8_t)(ST_26_DEFAULT_LAST_ENTRY_SEQ_NBR >> 8);
	data[5] = (uint8_t)(ST_26_DEFAULT_NBR_UNREAD_ENTRIES);
	
	SM_APP_vChange_table_state(ST_26_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( ST_26_LIST_STATUS_ADDR, &data[0], 1 );
	EEPROM_writer( ST_26_NBR_VALID_ENTRIES_ADDR, &data[1], 1 );
	EEPROM_writer( ST_26_LAST_ENTRY_ELEMENT_ADDR, &data[2], 1 );
	EEPROM_writer( ST_26_LAST_ENTRY_SEQ_NBR_ADDR, &data[3], 2 );
	EEPROM_writer( ST_26_NBR_UNREAD_ENTRIES_ADDR, &data[5], 1 );
	
	SM_APP_vChange_table_state(ST_26_PROG_STATE_ADDR, C1219_TABLE_FULL);
	
	return true;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vClear_st_26(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vClear_st_26(void) {
	
	uint8_t i;
	uint64_t default_value = 0;

	REGS_APP_bSt_26_restore_default_values();
	
	for ( i = 0; i < MAX_NBR_SELF_READS; i++ ) {
		
		EEPROM_writer( ST_26_SELF_READ_SEQ_NBR_ADDR + 2*i, &default_value, 2 );
		EEPROM_writer( ST_26_END_DATE_TIME_ADDR + 4*i, &default_value, 4 );
		EEPROM_writer( ST_26_SEASON_ADDR + i, &default_value, 1 );
		
		memset( &st_23_copy, 0, sizeof(st_23_copy) );
		EEPROM_writer( ST_26_SELF_READ_REGISTER_DATA_ADDR + i*ST_23_MAX_SIZE, st_23_copy, ST_23_MAX_SIZE );
	}
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_26_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_26_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t i = 0;
	uint16_t k = 0;
	uint16_t st_23_size;
	
	st_23_size = REGS_APP_usSt_23_get_size();
	
	/*---------------------------------------------------------------------------*\
	 * LIST_STATUS
	\*---------------------------------------------------------------------------*/
		EEPROM_reader( ST_26_LIST_STATUS_ADDR, &table_buffer[k], 1 );
		table_buffer[k] &= ~ORDER_FLAG_bm;
		table_buffer[k] |=	LIST_TYPE_FLAG_bm;
		table_buffer[k] &= ~INHIBIT_OVERFLOW_FLAG_bm;
		k += 1;
	
	/*---------------------------------------------------------------------------*\
	 * NBR_VALID_ENTRIES
	\*---------------------------------------------------------------------------*/
		EEPROM_reader( ST_26_NBR_VALID_ENTRIES_ADDR, &table_buffer[k], 1 );
		k += 1;
	
	/*---------------------------------------------------------------------------*\
	 * LAST_ENTRY_ELEMENT
	\*---------------------------------------------------------------------------*/
		EEPROM_reader( ST_26_LAST_ENTRY_ELEMENT_ADDR, &table_buffer[k], 1 );
		k += 1;
	
	/*---------------------------------------------------------------------------*\
	 * LAST_ENTRY_SEQ_NBR
	\*---------------------------------------------------------------------------*/
		EEPROM_reader( ST_26_LAST_ENTRY_SEQ_NBR_ADDR, &table_buffer[k], 2 );
		k += 2;	
	
	/*---------------------------------------------------------------------------*\
	 * NBR_UNREAD_ENTRIES
	\*---------------------------------------------------------------------------*/
		EEPROM_reader( ST_26_NBR_UNREAD_ENTRIES_ADDR, &table_buffer[k], 1 );
		k += 1;

	/*---------------------------------------------------------------------------*\
	 * SELF_READS_ENTRIES
	\*---------------------------------------------------------------------------*/
	for ( i = 0; i < st_21.nbr_self_reads; i++ ) {
		
		/* SELF_READ_SEQ_NBR */
		if ( (st_21.reg_func2_flags & SELF_READ_SEQ_NBR_FLAG_bm) == SELF_READ_SEQ_NBR_FLAG_bm ) {
			EEPROM_reader( ST_26_SELF_READ_SEQ_NBR_ADDR + 2*i, &table_buffer[k], 2 );
			k += 2;
		}
		
		/* REGISTER_INFO */
		if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm ) {
			EEPROM_reader( ST_26_END_DATE_TIME_ADDR + 4*i, &table_buffer[k], 4 );
			k += 4;
		}

		if ( (st_21.reg_func1_flags & SEASON_INFO_FIELD_FLAG_bm) == SEASON_INFO_FIELD_FLAG_bm ) {
			EEPROM_reader( ST_26_SEASON_ADDR + i, &table_buffer[k], 1 );
			k += 1;
		}
		
		/* SELF_READ_REGISTER_DATA */
		EEPROM_reader( ST_26_SELF_READ_REGISTER_DATA_ADDR + i*ST_23_MAX_SIZE, &table_buffer[k], st_23_size );
		k += st_23_size;
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_26_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_26_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;
	uint16_t i = 0;
	uint16_t st_23_size;
	
	st_23_size = REGS_APP_usSt_23_get_size();
	
	/*---------------------------------------------------------------------------*\
	 * LIST_STATUS
	\*---------------------------------------------------------------------------*/
		EEPROM_writer( ST_26_LIST_STATUS_ADDR, &table_buffer[k], 1 );
		k += 1;
	
	/*---------------------------------------------------------------------------*\
	 * NBR_VALID_ENTRIES
	\*---------------------------------------------------------------------------*/
		EEPROM_writer( ST_26_NBR_VALID_ENTRIES_ADDR, &table_buffer[k], 1 );
		k += 1;
	
	/*---------------------------------------------------------------------------*\
	 * LAST_ENTRY_ELEMENT
	\*---------------------------------------------------------------------------*/
		EEPROM_writer( ST_26_LAST_ENTRY_ELEMENT_ADDR, &table_buffer[k], 1 );
		k += 1;
	
	/*---------------------------------------------------------------------------*\
	 * LAST_ENTRY_SEQ_NBR
	\*---------------------------------------------------------------------------*/
		EEPROM_writer( ST_26_LAST_ENTRY_SEQ_NBR_ADDR, &table_buffer[k], 2 );
		k += 2;	
	
	/*---------------------------------------------------------------------------*\
	 * NBR_UNREAD_ENTRIES
	\*---------------------------------------------------------------------------*/
		EEPROM_writer( ST_26_NBR_UNREAD_ENTRIES_ADDR, &table_buffer[k], 1 );
		k += 1;

	/*---------------------------------------------------------------------------*\
	 * SELF_READS_ENTRIES
	\*---------------------------------------------------------------------------*/
	for ( i = 0; i < st_21.nbr_self_reads; i++ ) {
		/* SELF_READ_SEQ_NBR */
		if ( (st_21.reg_func2_flags & SELF_READ_SEQ_NBR_FLAG_bm) == SELF_READ_SEQ_NBR_FLAG_bm ) {
			EEPROM_writer( ST_26_SELF_READ_SEQ_NBR_ADDR + 2*i, &table_buffer[k], 2 );
			k += 2;
		}
		
		/* REGISTER_INFO */
		if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm ) {
			EEPROM_writer( ST_26_END_DATE_TIME_ADDR + 4*i, &table_buffer[k], 4 );
			k += 4;
		}

		if ( (st_21.reg_func1_flags & SEASON_INFO_FIELD_FLAG_bm) == SEASON_INFO_FIELD_FLAG_bm ) {
			EEPROM_writer( ST_26_SEASON_ADDR + i, &table_buffer[k], 1 );
			k += 1;
		}
		
		/* SELF_READ_REGISTER_DATA */
		EEPROM_writer( ST_26_SELF_READ_REGISTER_DATA_ADDR + i*ST_23_MAX_SIZE, &table_buffer[k], st_23_size );
		k += st_23_size;
	}
	
	is_ok = true;
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_26_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_26_get_size(void) {
	
	uint8_t i;
	uint16_t table_size = 0;
	
	table_size += 6;
	
	for ( i = 0; i < st_21.nbr_self_reads ; i++ ) {
		
		/* SELF_READ_SEQ_NBR */
		if ( (st_21.reg_func2_flags & SELF_READ_SEQ_NBR_FLAG_bm) == SELF_READ_SEQ_NBR_FLAG_bm ) {
			table_size += 2;
		}
		
		/* REGISTER_INFO */
		if ( (st_21.reg_func1_flags & DATE_TIME_FIELD_FLAG_bm) == DATE_TIME_FIELD_FLAG_bm ) {
			table_size += 4;
		}

		if ( (st_21.reg_func1_flags & SEASON_INFO_FIELD_FLAG_bm) == SEASON_INFO_FIELD_FLAG_bm ) {
			table_size += 1;
		}
		
		/* SELF_READ_REGISTER_DATA */
		table_size += REGS_APP_usSt_23_get_size();
	}
	
	return table_size;	
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bLoad_st_27(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bLoad_st_27(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_27_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = REGS_APP_bSt_27_restore_default_values();
	}
	else {
		
		is_ok = REGS_APP_bSt_27_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_27_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_27_restore_default_values(void) {
	
	bool is_ok = false;
	ST_27_TYPE st_27_temp;
	uint8_t i;
	
	memset( &st_27_temp, (uint8_t)PD_W_DEL_ID, sizeof(st_27_temp) );
		
	for ( i = 0; i < st_21.nbr_present_demands; i++ ) {
		
		switch (st_22.demand_select[i]) {
			
			case AD_W_DEL_ID:
				st_27_temp.present_demand_select[i] = PD_W_DEL_ID;
				break;

			case AD_W_REC_ID:	
				st_27_temp.present_demand_select[i] = PD_W_REC_ID;
				break;

			case AD_W_NET_ID:	
				st_27_temp.present_demand_select[i] = PD_W_NET_ID;
				break;

			case AD_W_SUM_ID:	
				st_27_temp.present_demand_select[i] = PD_W_SUM_ID;
				break;

			case AD_VAR_Q1_ID:	
				st_27_temp.present_demand_select[i] = PD_VAR_Q1_ID;
				break;

			case AD_VAR_Q2_ID:	
				st_27_temp.present_demand_select[i] = PD_VAR_Q2_ID;
				break;

			case AD_VAR_Q3_ID:	
				st_27_temp.present_demand_select[i] = PD_VAR_Q3_ID;
				break;

			case AD_VAR_Q4_ID:	
				st_27_temp.present_demand_select[i] = PD_VAR_Q4_ID;
				break;

			case AD_VAR_DEL_ID:
				st_27_temp.present_demand_select[i] = PD_VAR_DEL_ID;
				break;

			case AD_VAR_REC_ID:
				st_27_temp.present_demand_select[i] = PD_VAR_REC_ID;
				break;

			case AD_VAR_NET_ID:
				st_27_temp.present_demand_select[i] = PD_VAR_NET_ID;
				break;

			case AD_VAR_SUM_ID:
				st_27_temp.present_demand_select[i] = PD_VAR_SUM_ID;
				break;

			case AD_VAR_Q1_Q4_ID:
				st_27_temp.present_demand_select[i] = PD_VAR_Q1_Q4_ID;
				break;

			case AD_VAR_Q2_Q3_ID:
				st_27_temp.present_demand_select[i] = PD_VAR_Q2_Q3_ID;
				break;

			case AD_VA_DEL_ID:	
				st_27_temp.present_demand_select[i] = PD_VA_DEL_ID;
				break;

			case AD_VA_REC_ID:
				st_27_temp.present_demand_select[i] = PD_VA_REC_ID;
				break;

			case AD_VA_NET_ID:	
				st_27_temp.present_demand_select[i] = PD_VA_NET_ID;
				break;

			case AD_VA_SUM_ID:	
				st_27_temp.present_demand_select[i] = PD_VA_SUM_ID;
				break;

			case AD_VA_Q1_ID:
				st_27_temp.present_demand_select[i] = PD_VA_Q1_ID;
				break;

			case AD_VA_Q2_ID:	
				st_27_temp.present_demand_select[i] = PD_VA_Q2_ID;
				break;

			case AD_VA_Q3_ID:	
				st_27_temp.present_demand_select[i] = PD_VA_Q3_ID;
				break;

			case AD_VA_Q4_ID:	
				st_27_temp.present_demand_select[i] = PD_VA_Q4_ID;
				break;

			default:
				break;
		}
	}
	
	i = 0;
	
	st_27_temp.present_value_select[i++] = IV_W_TOTAL_ID;

#if ( NBR_PHASES > 1 )
		st_27_temp.present_value_select[i++] = IV_W_A_ID;
#endif

#if ( NBR_PHASES >= 2 )
		st_27_temp.present_value_select[i++] = IV_W_B_ID;
#endif

#if ( NBR_PHASES >= 3 )
		st_27_temp.present_value_select[i++] = IV_W_C_ID;
#endif

	st_27_temp.present_value_select[i++] = IV_VAR_TOTAL_ID;

#if ( NBR_PHASES > 1 )
		st_27_temp.present_value_select[i++] = IV_VAR_A_ID;
#endif

#if ( NBR_PHASES >= 2 )
		st_27_temp.present_value_select[i++] = IV_VAR_B_ID;
#endif

#if ( NBR_PHASES >= 3 )
		st_27_temp.present_value_select[i++] = IV_VAR_C_ID;
#endif

	st_27_temp.present_value_select[i++] = IV_VA_TOTAL_ID;
	
#if ( NBR_PHASES > 1 )
		st_27_temp.present_value_select[i++] = IV_VA_A_ID;
#endif

#if ( NBR_PHASES >= 2 )
		st_27_temp.present_value_select[i++] = IV_VA_B_ID;
#endif

#if ( NBR_PHASES >= 3 )
		st_27_temp.present_value_select[i++] = IV_VA_C_ID;
#endif

#if ( NBR_PHASES > 1 )
		st_27_temp.present_value_select[i++] = IV_V_A_ID;
#endif

#if ( NBR_PHASES >= 2 )
		st_27_temp.present_value_select[i++] = IV_V_B_ID;
#endif

#if ( NBR_PHASES >= 3 )
		st_27_temp.present_value_select[i++] = IV_V_C_ID;
#endif

#if ( NBR_PHASES > 1 )
		st_27_temp.present_value_select[i++] = IV_I_A_ID;
#endif

#if ( NBR_PHASES >= 2 )
		st_27_temp.present_value_select[i++] = IV_I_B_ID;
#endif

#if ( NBR_PHASES >= 3 )
		st_27_temp.present_value_select[i++] = IV_I_C_ID;
#endif

#if ( NBR_PHASES > 1 )
		st_27_temp.present_value_select[i++] = IV_V_A_ANGLE_ID;
#endif

#if ( NBR_PHASES >= 2 )
		st_27_temp.present_value_select[i++] = IV_V_B_ANGLE_ID;
#endif

#if ( NBR_PHASES >= 3 )
		st_27_temp.present_value_select[i++] = IV_V_C_ANGLE_ID;
#endif

#if ( NBR_PHASES > 1 )
		st_27_temp.present_value_select[i++] = IV_I_A_ANGLE_ID;
#endif

#if ( NBR_PHASES >= 2 )
		st_27_temp.present_value_select[i++] = IV_I_B_ANGLE_ID;
#endif

#if ( NBR_PHASES >= 3 )
		st_27_temp.present_value_select[i++] = IV_I_C_ANGLE_ID;
#endif

	st_27_temp.present_value_select[i++] = IV_PF_ANGLE_ID;

#if ( NBR_PHASES > 1 )
		st_27_temp.present_value_select[i++] = IV_PF_A_ANGLE_ID;
#endif

#if ( NBR_PHASES >= 2 )
		st_27_temp.present_value_select[i++] = IV_PF_B_ANGLE_ID;
#endif

#if ( NBR_PHASES >= 3 )
		st_27_temp.present_value_select[i++] = IV_PF_C_ANGLE_ID;
#endif

	st_27_temp.present_value_select[i++] = IV_PF_ID;

#if ( NBR_PHASES > 1 )
		st_27_temp.present_value_select[i++] = IV_PF_A_ID;
#endif

#if ( NBR_PHASES >= 2 )
		st_27_temp.present_value_select[i++] = IV_PF_B_ID;
#endif

#if ( NBR_PHASES >= 3 )
		st_27_temp.present_value_select[i++] = IV_PF_C_ID;
#endif

	st_27_temp.present_value_select[i++] = IV_F_ID;
	
	is_ok = REGS_APP_bSt_27_validate_data(&st_27_temp);
	
	st_27 = st_27_temp;
	REGS_APP_vSt_27_write_eeprom_values(&st_27);
	uint16_t crc = SM_APP_usSt_27_ram_calculate_crc();
	SM_APP_vSt_27_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_27_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_27_refresh_ram(void) {
	
	ST_27_TYPE st_27_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		REGS_APP_vSt_27_read_eeprom_values(&st_27_temp);
		is_ok = REGS_APP_bSt_27_validate_data(&st_27_temp);
		
		if ( is_ok ) {
			st_27 = st_27_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_27 = st_27_temp;
		REGS_APP_vSt_27_write_eeprom_values(&st_27);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  REGS_APP_vCopy_st_27_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vCopy_st_27_ram(uint8_t* destination) {
	
	REGS_APP_vSt_27_to_array(st_27, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vCopy_st_27_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vCopy_st_27_nvm(uint8_t* destination) {
	
	ST_27_TYPE st_27_aux;
	
	REGS_APP_vSt_27_read_eeprom_values(&st_27_aux);
	REGS_APP_vSt_27_to_array(st_27_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_27_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_27_recover_nvm(void) {
	
	REGS_APP_vSt_27_write_eeprom_values(&st_27);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_27_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_27_recover_ram(void) {
	
	REGS_APP_vSt_27_read_eeprom_values(&st_27);
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_27_to_array(ST_27_TYPE st_27_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_27_to_array(ST_27_TYPE st_27_source, uint8_t *table_buffer) {

	uint8_t k = 0;
	uint8_t i = 0;
	
	for ( i = 0; i < st_21.nbr_present_demands; i++ ) {
		table_buffer[k++] = (uint8_t)st_27_source.present_demand_select[i];
	}
	
	for ( i = 0; i < st_21.nbr_present_values; i++ ) {
		table_buffer[k++] = (uint8_t)st_27_source.present_value_select[i];
	}
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_27_validate_data(ST_27_TYPE *st_27_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_27_validate_data(ST_27_TYPE *st_27_structure) {
	
	bool is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_27_read_eeprom_values(ST_27_TYPE *st_27_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_27_read_eeprom_values(ST_27_TYPE *st_27_structure) {
	
	uint8_t i;
	
	for ( i = 0; i < st_21.nbr_present_demands; i++ ) {
		EEPROM_reader( ST_27_PRESENT_DEMAND_SELECT_ADDR + i, &st_27_structure->present_demand_select[i], 1 );
	}
	
	for ( i = 0; i < st_21.nbr_present_values; i++ ) {
		EEPROM_reader( ST_27_PRESENT_VALUE_SELECT_ADDR + i, &st_27_structure->present_value_select[i], 1 );
	}
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vSt_27_write_eeprom_values(ST_27_TYPE *st_27_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSt_27_write_eeprom_values(ST_27_TYPE *st_27_structure) {
	
	uint8_t i;
	
	SM_APP_vChange_table_state(ST_27_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	for ( i = 0; i < st_21.nbr_present_demands; i++ ) {
		EEPROM_writer( ST_27_PRESENT_DEMAND_SELECT_ADDR + i, &st_27_structure->present_demand_select[i], 1 );
	}
	
	for ( i = 0; i < st_21.nbr_present_values; i++ ) {
		EEPROM_writer( ST_27_PRESENT_VALUE_SELECT_ADDR + i, &st_27_structure->present_value_select[i], 1 );
	}
	
	SM_APP_vChange_table_state(ST_27_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_27_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_27_execute_table_reading(uint8_t *table_buffer) {
	
	REGS_APP_vSt_27_to_array(st_27, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_27_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_27_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	uint8_t i = 0;
	ST_27_TYPE st_27_temp;
	
	for ( i = 0; i < st_21.nbr_present_demands; i++ ) {
		st_27_temp.present_demand_select[i] = (PRESENT_DEMAND_ID_TYPE)table_buffer[k++];
	}
	
	for ( i = 0; i < st_21.nbr_present_values; i++ ) {
		st_27_temp.present_value_select[i] = (INSTANTANEOUS_VALUE_ID_TYPE)table_buffer[k++];
	}
	
	is_ok = REGS_APP_bSt_27_validate_data(&st_27_temp);
	
	if ( is_ok ) {
		st_27 = st_27_temp;
		REGS_APP_vSt_27_write_eeprom_values(&st_27);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_27_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_27_get_size(void) {

	return (uint8_t)(st_21.nbr_present_demands + st_21.nbr_present_values);
}

/**
 ******************************************************************************
 * \fn bool REGS_APP_bSt_28_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bSt_28_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t i, j, k = 0;
	static uint64_t inst_value;
	uint32_t rem_time;
	
	for ( j = 0; j < st_21.nbr_present_demands; j++ ) {
		
		REGS_APP_bCopy_present_demand(INTERVAL_0, j, &inst_value);
		rem_time = (uint32_t)REGS_APP_vGet_present_demand_time_remining(INTERVAL_0);
		
		if ( (st_21.reg_func1_flags & TIME_REMAINING_FLAG_bm) == TIME_REMAINING_FLAG_bm )
		{
			convert_32_bit_to_byte_array(rem_time, &table_buffer[k]);
			k += TM_FMAT_SIZE;
		}
		
		convert_64_bit_to_byte_array(inst_value, &table_buffer[k]);
		k += NI_FMAT2_SIZE;
	}
	
	INSTANTANEOUS_VALUES_TYPE st_28_instantaneous;
	METROLOGY_APP_vCopy_instantaneous_struct(&st_28_instantaneous);

	for ( i = 0; i < st_21.nbr_present_values; i++ )
	{
		switch ( st_27.present_value_select[i] )
		{
#if ( NBR_PHASES >= 1 )
			case IV_V_A_ID:
				inst_value = (uint64_t)st_28_instantaneous.V_a;
				break;
#endif
		
#if ( NBR_PHASES >= 2 )	
			case IV_V_B_ID:
				inst_value = (uint64_t)st_28_instantaneous.V_b;
				break;
#endif

#if ( NBR_PHASES >= 3 )	
			case IV_V_C_ID:
				inst_value = (uint64_t)st_28_instantaneous.V_c;
				break;
#endif
			
#if ( NBR_PHASES >= 1 )
			case IV_I_A_ID:
				inst_value = (uint64_t)st_28_instantaneous.I_a;
				break;
#endif
		
#if ( NBR_PHASES >= 2 )	
			case IV_I_B_ID:
				inst_value = (uint64_t)st_28_instantaneous.I_b;
				break;
#endif

#if ( NBR_PHASES >= 3 )			
			case IV_I_C_ID:
				inst_value = (uint64_t)st_28_instantaneous.I_c;
				break;
#endif
		
#if ( NBR_PHASES > 1 )	
			case IV_V_A_ANGLE_ID:
				inst_value = (uint64_t)st_28_instantaneous.Angle_V_a;
				break;
#endif

#if ( NBR_PHASES >= 2 )			
			case IV_V_B_ANGLE_ID:
				inst_value = (uint64_t)st_28_instantaneous.Angle_V_b;
				break;
#endif

#if ( NBR_PHASES >= 3 )
			case IV_V_C_ANGLE_ID:
				inst_value = (uint64_t)st_28_instantaneous.Angle_V_c;
				break;
#endif
		
#if ( NBR_PHASES > 1 )	
			case IV_I_A_ANGLE_ID:
				inst_value = (uint64_t)st_28_instantaneous.Angle_I_a;
				break;
#endif
		
#if ( NBR_PHASES >= 2 )	
			case IV_I_B_ANGLE_ID:
				inst_value = (uint64_t)st_28_instantaneous.Angle_I_b;
				break;
#endif
		
#if ( NBR_PHASES >= 3 )	
			case IV_I_C_ANGLE_ID:
				inst_value = (uint64_t)st_28_instantaneous.Angle_I_c;
				break;
#endif
			
			case IV_PF_ANGLE_ID:
				inst_value = (uint64_t)st_28_instantaneous.Angle_PF_t;
				break;

#if ( NBR_PHASES > 1 )
			case IV_PF_A_ANGLE_ID:
				inst_value = (uint64_t)st_28_instantaneous.Angle_PF_a;
				break;
#endif

#if ( NBR_PHASES >= 2 )			
			case IV_PF_B_ANGLE_ID:
				inst_value = (uint64_t)st_28_instantaneous.Angle_PF_b;
				break;
#endif
		
#if ( NBR_PHASES >= 3 )	
			case IV_PF_C_ANGLE_ID:
				inst_value = (uint64_t)st_28_instantaneous.Angle_PF_c;
				break;
#endif
			
			case IV_PF_ID:
				inst_value = (uint64_t)st_28_instantaneous.PF_t;
				break;

#if ( NBR_PHASES > 1 )
			case IV_PF_A_ID:
				inst_value = (uint64_t)st_28_instantaneous.PF_a;
				break;
#endif
		
#if ( NBR_PHASES >= 2 )	
			case IV_PF_B_ID:
				inst_value = (uint64_t)st_28_instantaneous.PF_b;
				break;
#endif

#if ( NBR_PHASES >= 3 )			
			case IV_PF_C_ID:
				inst_value = (uint64_t)st_28_instantaneous.PF_c;
				break;
#endif
			
			case IV_F_ID:
				inst_value = (uint64_t)st_28_instantaneous.Freq_t;
				break;
			
			case IV_W_TOTAL_ID:
				inst_value = (uint64_t)fabs(st_28_instantaneous.P_t);
				break;

#if ( NBR_PHASES > 1 )
			case IV_W_A_ID:
				inst_value = (uint64_t)fabs(st_28_instantaneous.P_a);
				break;
#endif
	
#if ( NBR_PHASES >= 2 )	
			case IV_W_B_ID:
				inst_value = (uint64_t)fabs(st_28_instantaneous.P_b);
				break;
#endif
		
#if ( NBR_PHASES >= 3 )	
			case IV_W_C_ID:
				inst_value = (uint64_t)fabs(st_28_instantaneous.P_c);
				break;
#endif
			
			case IV_VAR_TOTAL_ID:
				inst_value = (uint64_t)fabs(st_28_instantaneous.Q_t);
				break;
			
#if ( NBR_PHASES > 1 )
			case IV_VAR_A_ID:
				inst_value = (uint64_t)fabs(st_28_instantaneous.Q_a);
				break;
#endif

#if ( NBR_PHASES >= 2 )			
			case IV_VAR_B_ID:
				inst_value = (uint64_t)fabs(st_28_instantaneous.Q_b);
				break;
#endif
		
#if ( NBR_PHASES >= 3 )	
			case IV_VAR_C_ID:
				inst_value = (uint64_t)fabs(st_28_instantaneous.Q_c);
				break;
#endif

			case IV_VA_TOTAL_ID:
				inst_value = (uint64_t)st_28_instantaneous.S_t;
				break;
			
#if ( NBR_PHASES > 1 )
			case IV_VA_A_ID:
				inst_value = (uint64_t)st_28_instantaneous.S_a;
				break;
#endif
		
#if ( NBR_PHASES >= 2 )	
			case IV_VA_B_ID:
				inst_value = (uint64_t)st_28_instantaneous.S_b;
				break;
#endif

#if ( NBR_PHASES >= 3 )			
			case IV_VA_C_ID:
				inst_value = (uint64_t)st_28_instantaneous.S_c;
				break;
#endif
			
			default:
				inst_value = 0;
				break;
		}
		
		convert_64_bit_to_byte_array(inst_value, &table_buffer[k]);
		k += NI_FMAT1_SIZE;
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t REGS_APP_usSt_28_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_usSt_28_get_size(void) {

	uint16_t table_size = 0;
	
	table_size += st_21.nbr_present_demands * NI_FMAT2_SIZE;
	table_size += st_21.nbr_present_values * NI_FMAT1_SIZE;
	
	if ( (st_21.reg_func1_flags & TIME_REMAINING_FLAG_bm) == TIME_REMAINING_FLAG_bm ) {
		table_size += st_21.nbr_present_demands * TM_FMAT_SIZE;
	}
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_v1sec_handler(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_v1sec_handler(void) {
	
	if ( !meter.system_state.changing_configuration ) {

		registers_os.task_execute = true;
	
		if ( registers_os.rtos_task_active ) {

			portBASE_TYPE xYieldRequired;
			xYieldRequired = xTaskResumeFromISR(registers_os.storage_task_id);
		
			if ( xYieldRequired == pdTRUE ) {
				portYIELD();
			}
		}
	}
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vCreate_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vCreate_task(void) {
	
	registers_os.rtos_task_active = false;

	/* Create Metering task */
	xTaskCreate( REGS_APP_vStorage_rtos_task, (const signed char * const ) "Metering_Task", METERING_STACK_SIZE, NULL, METERING_TASK_PRIO, &registers_os.storage_task_id );

	registers_os.rtos_task_active = true;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vStorage_rtos_task(void *pvParameters)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vStorage_rtos_task(void *pvParameters) {
		
	UNUSED( pvParameters );
	
	/* Metering module task Loop. */
	for ( ; ; ) {
		
		if ( registers_os.rtos_task_active ) {
		
			REGS_APP_vStorage_task();
			WATCHDOG_APP_vRestart();
		}
		
		/* Suspend Metering Task */
		vTaskSuspend(registers_os.storage_task_id);
	}
	
	/* Should never go there */
	vTaskDelete(NULL);
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vSuspend_storage_rtos_task(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSuspend_storage_rtos_task(void) {

	registers_os.rtos_task_active = false;

	if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) {

		vTaskSuspend(registers_os.storage_task_id);
	}
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vResume_storage_rtos_task(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vResume_storage_rtos_task(void) {

	registers_os.rtos_task_active = true;
}	

/** 
 ******************************************************************************
 * \fn void REGS_APP_vStorage_task(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vStorage_task(void) {
	
	if ( registers_os.task_execute ) {
		
		registers_os.task_execute = false;
	
		if ( ++registers_os.time_after_last_backup >= (10 * SECONDS_PER_MINUTE) ) {
			
			registers_os.time_after_last_backup = 0;
			REGS_APP_vStore_summations();
			WATCHDOG_APP_vRestart();
		}
		
		REGS_APP_vStore_demands();
		WATCHDOG_APP_vRestart();
		
		LP_APP_vStore_data_task();
		WATCHDOG_APP_vRestart();
	}
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vSummation_task(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSummation_task(void) {

	if ( !meter.system_state.changing_configuration ) {

		static uint8_t i;
		static volatile double inst_value;
		uint8_t curr_tier;
	
		curr_tier = TOU_APP_ucGet_current_tier();
	
		for ( i = 0; i < st_21.nbr_summations; i++ ) {
		
			inst_value = REGS_APP_dDetermine_instantaneous_power( (uint8_t)st_22.summation_select[i] ); 
		
			/* Accumulate in Total Summations */
			st_23.tot_data_block.summations[i] += (double)(inst_value/(double)SECONDS_PER_HOUR); 
		
			if (st_23.tot_data_block.summations[i] >= MAX_REGISTER_VALUE) {
				st_23.tot_data_block.summations[i] -= MAX_REGISTER_VALUE;
			}
			else if ( st_23.tot_data_block.summations[i] < 0 ) {
				st_23.tot_data_block.summations[i] += MAX_REGISTER_VALUE;
			}
				
			/* Accumulate in Tier Summations */
			st_23.tier_data_block[curr_tier].summations[i] += (double)(inst_value/(double)SECONDS_PER_HOUR); 
		
			if (st_23.tier_data_block[curr_tier].summations[i] >= MAX_REGISTER_VALUE) {
				st_23.tier_data_block[curr_tier].summations[i] -= MAX_REGISTER_VALUE;
			}
			else if ( st_23.tier_data_block[curr_tier].summations[i] < 0 ) {
				st_23.tier_data_block[curr_tier].summations[i] += MAX_REGISTER_VALUE;
			}	
		}
	}
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vDemand_task(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vDemand_task(void) {
	
	if ( !meter.system_state.changing_configuration ) {

 		if ( demand.config.is_sliding_demand ) {
		 
			REGS_APP_vProcess_demand_interval( &demand.interval[0] );
		
			for ( uint8_t i = 0; i < demand.config.nbr_intervals; i++ ) {
			
				if ( demand.interval[i].run_next_interval == true ) {
				
					REGS_APP_vProcess_demand_interval( &demand.interval[i + 1] );
				}
			}
 		}
		else {
			REGS_APP_vProcess_demand_interval( &demand.interval[0] );
		}
	}
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vProcess_demand_interval(PRESENT_DEMAND_INTERVAL_TYPE* demand_interval)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vProcess_demand_interval(PRESENT_DEMAND_INTERVAL_TYPE* demand_interval) {
		
	static uint8_t i;
	static double inst_value;
	uint32_t timestamp = rtc.utc_timestamp;
	uint8_t curr_tier;
	
	demand_interval->current_seconds++;
	
	if (demand_interval->current_seconds >= demand.config.interval_length ) {
		demand_interval->current_seconds = 0;
		demand_interval->is_int_expiried = true;
	}
	
	if (demand_interval->current_seconds >= (demand.config.subinterval_length + 1) ) {
		demand_interval->run_next_interval = true;
	}
	
	curr_tier = TOU_APP_ucGet_current_tier();

	uint64_t demand_value;

	for ( i = 0; i < st_21.nbr_demands; i++ ) {
		
		inst_value = REGS_APP_dDetermine_instantaneous_power( (uint8_t)st_22.demand_select[i] );
		demand_interval->present_demand[i] += ( inst_value / (double)demand.config.interval_length );
		
		demand_value = (uint64_t)( fabs(demand_interval->present_demand[i]) );
		
		if ( demand_value > st_23.tot_data_block.demands[i].demand[0] ) {
			
			st_23.tot_data_block.demands[i].demand[0] = demand_value;
			st_23.tot_data_block.demands[i].cont_cum_demand += demand_value;
			st_23.tot_data_block.demands[i].event_time[0] = timestamp;

			registers_os.max_demand = true;
		}
		
		if ( demand_value > st_23.tier_data_block[curr_tier].demands[i].demand[0] ) {
			
			st_23.tier_data_block[curr_tier].demands[i].demand[0] = demand_value;
			st_23.tier_data_block[curr_tier].demands[i].cont_cum_demand += demand_value;
			st_23.tier_data_block[curr_tier].demands[i].event_time[0] = timestamp;

			registers_os.max_demand = true;
		}
	}
	
	if ( demand_interval->is_int_expiried ) {
		memset( &demand_interval->present_demand, 0, sizeof(demand_interval->present_demand) );
		demand_interval->is_int_expiried = false;
	}
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vStore_demands(void)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vStore_demands(void) {
		
	static uint8_t i, j;

	if ( (!meter.ed_mode.test_mode.enabled) && (registers_os.max_demand == true) ){

		st_23_struct_copy = st_23;
		
		registers_os.max_demand = false;

		for ( i = 0; i < st_21.nbr_demands; i++ ) {				 
				
			EEPROM_writer( ST_23_TOT_DEMANDS_DEMAND_ADDR + i*NI_FMAT2_SIZE, &st_23_struct_copy.tot_data_block.demands[i].demand[0], NI_FMAT2_SIZE );
			EEPROM_writer( ST_23_TOT_DEMANDS_CONT_CUM_DEMAND_ADDR + i*NI_FMAT1_SIZE, &st_23_struct_copy.tot_data_block.demands[i].cont_cum_demand, NI_FMAT1_SIZE );
			EEPROM_writer( ST_23_TOT_DEMANDS_EVENT_TIME_ADDR + i*TM_FMAT_SIZE, &st_23_struct_copy.tot_data_block.demands[i].event_time[0], TM_FMAT_SIZE );
		}
		
		for ( j = 0; j < st_21.nbr_tiers; j++ ) {
			
			for ( i = 0; i < st_21.nbr_demands; i++ ) {
			
				EEPROM_writer( ST_23_TIER_DEMANDS_DEMAND_ADDR + (j * NI_FMAT2_SIZE * MAX_NBR_DEMANDS) + (i * NI_FMAT2_SIZE), &st_23_struct_copy.tier_data_block[j].demands[i].demand[0], NI_FMAT2_SIZE );
				EEPROM_writer( ST_23_TIER_DEMANDS_CONT_CUM_DEMAND_ADDR + (j * NI_FMAT1_SIZE * MAX_NBR_DEMANDS) + (i * NI_FMAT1_SIZE), &st_23_struct_copy.tier_data_block[j].demands[i].cont_cum_demand, NI_FMAT1_SIZE );
				EEPROM_writer( ST_23_TIER_DEMANDS_EVENT_TIME_ADDR + (j * TM_FMAT_SIZE * MAX_NBR_DEMANDS) + (i * TM_FMAT_SIZE), &st_23_struct_copy.tier_data_block[j].demands[i].event_time[0], TM_FMAT_SIZE );		
			}
		}
	}
}


/** 
 ******************************************************************************
 * \fn double REGS_APP_dDetermine_instantaneous_power(uint8_t source)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
double REGS_APP_dDetermine_instantaneous_power(uint8_t source) {
	
	double inst_value = 0;
	double rms_val = 0;
	uint8_t quadrant;

	switch ( source ) {

		/*===========================================================================*\
		 * Total Values
		\*===========================================================================*/
		
		/* Total Instantaneous Delivered Active Power */
		case 0:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(P_T_ID);
			if ( rms_val > 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
		
		/* Total Instantaneous Received Active Power */
		case 1:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(P_T_ID);
			if ( rms_val < 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
	
		/* Total Instantaneous Net Active Power */
		case 2:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(P_T_ID);
			break;
	
		/* Total Instantaneous Sum Active Power */
		case 3:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(P_T_ID);		
			inst_value = fabs(rms_val);
			break;
	
		/* Total Instantaneous Q1 Reactive Power */
		case 4:	
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_T_ID);
			if ( quadrant == (uint8_t)QUADRANT_1 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_T_ID) );
			}
			break;

		/* Total Instantaneous Q2 Reactive Power */
		case 5:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_T_ID);
			if ( quadrant == (uint8_t)QUADRANT_2 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_T_ID) );
			}
			break;
	
		/* Total Instantaneous Q3 Reactive Power */
		case 6:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_T_ID);
			if ( quadrant == (uint8_t)QUADRANT_3 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_T_ID) );
			}
			break;
	
		/* Total Instantaneous Q4 Reactive Power */
		case 7:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_T_ID);
			if ( quadrant == (uint8_t)QUADRANT_4 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_T_ID) );
			}
			break;
	
		/* Total Instantaneous Delivered Reactive Power */
		case 8:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(Q_T_ID);
			if ( rms_val > 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
	
		/* Total Instantaneous Received Reactive Power */
		case 9:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(Q_T_ID);
			if ( rms_val < 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
	
		/* Total Instantaneous Net Reactive Power */
		case 10:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(Q_T_ID);
			break;
	
		/* Total Instantaneous Sum Reactive Power */
		case 11:
			inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_T_ID) );
			break;
	
		/* Total Instantaneous (Q1+Q4) Reactive Power */
		case 12:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_T_ID);
			if ( (quadrant == (uint8_t)QUADRANT_1) || (quadrant == (uint8_t)QUADRANT_4) ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_T_ID) );
			}
			break;
	
		/* Total Instantaneous (Q2+Q3) Reactive Power */
		case 13:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_T_ID);
			if ( (quadrant == (uint8_t)QUADRANT_2) || (quadrant == (uint8_t)QUADRANT_3) ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_T_ID) );
			}
			break;
	
		/* Total Instantaneous Delivered Apparent Power */
		case 14:
			if ( METROLOGY_APP_dGet_instantaneous_value(P_T_ID) > 0 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_T_ID);
			}
			break;

		/* Total Instantaneous Received Apparent Power */
		case 15:
			if ( METROLOGY_APP_dGet_instantaneous_value(P_T_ID) < 0 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_T_ID);
			}
			break;
	
		/* Total Instantaneous Net Apparent Power */
		case 16:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(S_T_ID);
			break;
			
		/* Total Instantaneous Net Apparent Power */
		case 17:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(S_T_ID);
			break;
	
		/* Total Instantaneous Q1 Apparent Power */
		case 18:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_T_ID);
			if ( quadrant == (uint8_t)QUADRANT_1 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_T_ID);
			}
			break;

		/* Total Instantaneous Q2 Apparent Power */
		case 19:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_T_ID);
			if ( quadrant == (uint8_t)QUADRANT_2 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_T_ID);
			}
			break;

		/* Total Instantaneous Q3 Apparent Power */
		case 20:	
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_T_ID);
			if ( quadrant == (uint8_t)QUADRANT_3 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_T_ID);
			}
			break;

		/* Total Instantaneous Q4 Apparent Power */
		case 21:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_T_ID);
			if ( quadrant == (uint8_t)QUADRANT_4 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_T_ID);
			}
			break;

		/*===========================================================================*\
		 * Phase A Values
		\*===========================================================================*/
#if (NBR_PHASES > 1)
	
		/* Phase A Instantaneous Delivered Active Power */
		case 22:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(P_A_ID);
			if ( rms_val > 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
		
		/* Phase A Instantaneous Received Active Power */
		case 23:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(P_A_ID);
			if ( rms_val < 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
	
		/* Phase A Instantaneous Net Active Power */
		case 24:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(P_A_ID);
			break;
	
		/* Phase A Instantaneous Sum Active Power */
		case 25:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(P_A_ID);		
			inst_value = fabs(rms_val);
			break;
	
		/* Phase A Instantaneous Q1 Reactive Power */
		case 26:	
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_A_ID);
			if ( quadrant == (uint8_t)QUADRANT_1 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_A_ID) );
			}
			break;

		/* Phase A Instantaneous Q2 Reactive Power */
		case 27:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_A_ID);
			if ( quadrant == (uint8_t)QUADRANT_2 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_A_ID) );
			}
			break;
	
		/* Phase A Instantaneous Q3 Reactive Power */
		case 28:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_A_ID);
			if ( quadrant == (uint8_t)QUADRANT_3 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_A_ID) );
			}
			break;
	
		/* Phase A Instantaneous Q4 Reactive Power */
		case 29:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_A_ID);
			if ( quadrant == (uint8_t)QUADRANT_4 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_A_ID) );
			}
			break;
	
		/* Phase A Instantaneous Delivered Reactive Power */
		case 30:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(Q_A_ID);
			if ( rms_val > 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
	
		/* Phase A Instantaneous Received Reactive Power */
		case 31:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(Q_A_ID);
			if ( rms_val < 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
	
		/* Phase A Instantaneous Net Reactive Power */
		case 32:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(Q_A_ID);
			break;
	
		/* Phase A Instantaneous Sum Reactive Power */
		case 33:
			inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_A_ID) );
			break;
	
		/* Phase A Instantaneous (Q1+Q4) Reactive Power */
		case 34:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_A_ID);
			if ( (quadrant == (uint8_t)QUADRANT_1) || (quadrant == (uint8_t)QUADRANT_4) ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_A_ID) );
			}
			break;
	
		/* Phase A Instantaneous (Q2+Q3) Reactive Power */
		case 35:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_A_ID);
			if ( (quadrant == (uint8_t)QUADRANT_2) || (quadrant == (uint8_t)QUADRANT_3) ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_A_ID) );
			}
			break;
	
		/* Phase A Instantaneous Delivered Apparent Power */
		case 36:
			if ( METROLOGY_APP_dGet_instantaneous_value(P_A_ID) > 0 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_A_ID);
			}
			break;

		/* Phase A Instantaneous Received Apparent Power */
		case 37:
			if ( METROLOGY_APP_dGet_instantaneous_value(P_A_ID) < 0 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_A_ID);
			}
			break;
	
		/* Phase A Instantaneous Net Apparent Power */
		case 38:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(S_A_ID);
			break;
			
		/* Phase A Instantaneous Net Apparent Power */
		case 39:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(S_A_ID);
			break;
	
		/* Phase A Instantaneous Q1 Apparent Power */
		case 40:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_A_ID);
			if ( quadrant == (uint8_t)QUADRANT_1 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_A_ID);
			}
			break;

		/* Phase A Instantaneous Q2 Apparent Power */
		case 41:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_A_ID);
			if ( quadrant == (uint8_t)QUADRANT_2 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_A_ID);
			}
			break;

		/* Phase A Instantaneous Q3 Apparent Power */
		case 42:	
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_A_ID);
			if ( quadrant == (uint8_t)QUADRANT_3 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_A_ID);
			}
			break;

		/* Phase A Instantaneous Q4 Apparent Power */
		case 43:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_A_ID);
			if ( quadrant == (uint8_t)QUADRANT_4 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_A_ID);
			}
			break;
#endif

		/*===========================================================================*\
		 * Phase B Values
		\*===========================================================================*/
#if (NBR_PHASES >= 2)

		/* Phase B Instantaneous Delivered Active Power */
		case 44:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(P_B_ID);
			if ( rms_val > 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
		
		/* Phase B Instantaneous Received Active Power */
		case 45:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(P_B_ID);
			if ( rms_val < 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
	
		/* Phase B Instantaneous Net Active Power */
		case 46:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(P_B_ID);
			break;
	
		/* Phase B Instantaneous Sum Active Power */
		case 47:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(P_B_ID);		
			inst_value = fabs(rms_val);
			break;
	
		/* Phase B Instantaneous Q1 Reactive Power */
		case 48:	
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_B_ID);
			if ( quadrant == (uint8_t)QUADRANT_1 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_B_ID) );
			}
			break;

		/* Phase B Instantaneous Q2 Reactive Power */
		case 49:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_B_ID);
			if ( quadrant == (uint8_t)QUADRANT_2 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_B_ID) );
			}
			break;
	
		/* Phase B Instantaneous Q3 Reactive Power */
		case 50:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_B_ID);
			if ( quadrant == (uint8_t)QUADRANT_3 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_B_ID) );
			}
			break;
	
		/* Phase B Instantaneous Q4 Reactive Power */
		case 51:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_B_ID);
			if ( quadrant == (uint8_t)QUADRANT_4 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_B_ID) );
			}
			break;
	
		/* Phase B Instantaneous Delivered Reactive Power */
		case 52:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(Q_B_ID);
			if ( rms_val > 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
	
		/* Phase B Instantaneous Received Reactive Power */
		case 53:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(Q_B_ID);
			if ( rms_val < 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
	
		/* Phase B Instantaneous Net Reactive Power */
		case 54:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(Q_B_ID);
			break;
	
		/* Phase B Instantaneous Sum Reactive Power */
		case 55:
			inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_B_ID) );
			break;
	
		/* Phase B Instantaneous (Q1+Q4) Reactive Power */
		case 56:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_B_ID);
			if ( (quadrant == (uint8_t)QUADRANT_1) || (quadrant == (uint8_t)QUADRANT_4) ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_B_ID) );
			}
			break;
	
		/* Phase B Instantaneous (Q2+Q3) Reactive Power */
		case 57:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_B_ID);
			if ( (quadrant == (uint8_t)QUADRANT_2) || (quadrant == (uint8_t)QUADRANT_3) ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_B_ID) );
			}
			break;
	
		/* Phase B Instantaneous Delivered Apparent Power */
		case 58:
			if ( METROLOGY_APP_dGet_instantaneous_value(P_B_ID) > 0 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_B_ID);
			}
			break;

		/* Phase B Instantaneous Received Apparent Power */
		case 59:
			if ( METROLOGY_APP_dGet_instantaneous_value(P_B_ID) < 0 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_B_ID);
			}
			break;
	
		/* Phase B Instantaneous Net Apparent Power */
		case 60:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(S_B_ID);
			break;
			
		/* Phase B Instantaneous Net Apparent Power */
		case 61:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(S_B_ID);
			break;
	
		/* Phase B Instantaneous Q1 Apparent Power */
		case 62:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_B_ID);
			if ( quadrant == (uint8_t)QUADRANT_1 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_B_ID);
			}
			break;

		/* Phase B Instantaneous Q2 Apparent Power */
		case 63:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_B_ID);
			if ( quadrant == (uint8_t)QUADRANT_2 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_B_ID);
			}
			break;

		/* Phase B Instantaneous Q3 Apparent Power */
		case 64:	
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_B_ID);
			if ( quadrant == (uint8_t)QUADRANT_3 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_B_ID);
			}
			break;

		/* Phase B Instantaneous Q4 Apparent Power */
		case 65:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_B_ID);
			if ( quadrant == (uint8_t)QUADRANT_4 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_B_ID);
			}
			break;
#endif

		/*===========================================================================*\
		 * Phase C Values
		\*===========================================================================*/
#if (NBR_PHASES == 3)

	/* Phase C Instantaneous Delivered Active Power */
		case 66:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(P_C_ID);
			if ( rms_val > 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
		
		/* Phase C Instantaneous Received Active Power */
		case 67:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(P_C_ID);
			if ( rms_val < 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
	
		/* Phase C Instantaneous Net Active Power */
		case 68:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(P_C_ID);
			break;
	
		/* Phase C Instantaneous Sum Active Power */
		case 69:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(P_C_ID);		
			inst_value = fabs(rms_val);
			break;
	
		/* Phase C Instantaneous Q1 Reactive Power */
		case 70:	
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_C_ID);
			if ( quadrant == (uint8_t)QUADRANT_1 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_C_ID) );
			}
			break;

		/* Phase C Instantaneous Q2 Reactive Power */
		case 71:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_C_ID);
			if ( quadrant == (uint8_t)QUADRANT_2 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_C_ID) );
			}
			break;
	
		/* Phase C Instantaneous Q3 Reactive Power */
		case 72:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_C_ID);
			if ( quadrant == (uint8_t)QUADRANT_3 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_C_ID) );
			}
			break;
	
		/* Phase C Instantaneous Q4 Reactive Power */
		case 73:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_C_ID);
			if ( quadrant == (uint8_t)QUADRANT_4 ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_C_ID) );
			}
			break;
	
		/* Phase C Instantaneous Delivered Reactive Power */
		case 74:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(Q_C_ID);
			if ( rms_val > 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
	
		/* Phase C Instantaneous Received Reactive Power */
		case 75:
			rms_val = METROLOGY_APP_dGet_instantaneous_value(Q_C_ID);
			if ( rms_val < 0 ) {
				inst_value = fabs(rms_val);
			}
			break;
	
		/* Phase C Instantaneous Net Reactive Power */
		case 76:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(Q_C_ID);
			break;
	
		/* Phase C Instantaneous Sum Reactive Power */
		case 77:
			inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_C_ID) );
			break;
	
		/* Phase C Instantaneous (Q1+Q4) Reactive Power */
		case 78:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_C_ID);
			if ( (quadrant == (uint8_t)QUADRANT_1) || (quadrant == (uint8_t)QUADRANT_4) ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_C_ID) );
			}
			break;
	
		/* Phase C Instantaneous (Q2+Q3) Reactive Power */
		case 79:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_C_ID);
			if ( (quadrant == (uint8_t)QUADRANT_2) || (quadrant == (uint8_t)QUADRANT_3) ) {
				inst_value = fabs( METROLOGY_APP_dGet_instantaneous_value(Q_C_ID) );
			}
			break;
	
		/* Phase C Instantaneous Delivered Apparent Power */
		case 80:
			if ( METROLOGY_APP_dGet_instantaneous_value(P_C_ID) > 0 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_C_ID);
			}
			break;

		/* Phase C Instantaneous Received Apparent Power */
		case 81:
			if ( METROLOGY_APP_dGet_instantaneous_value(P_C_ID) < 0 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_C_ID);
			}
			break;
	
		/* Phase C Instantaneous Net Apparent Power */
		case 82:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(S_C_ID);
			break;
			
		/* Phase C Instantaneous Net Apparent Power */
		case 83:
			inst_value = METROLOGY_APP_dGet_instantaneous_value(S_C_ID);
			break;
	
		/* Phase C Instantaneous Q1 Apparent Power */
		case 84:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_C_ID);
			if ( quadrant == (uint8_t)QUADRANT_1 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_C_ID);
			}
			break;

		/* Phase C Instantaneous Q2 Apparent Power */
		case 85:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_C_ID);
			if ( quadrant == (uint8_t)QUADRANT_2 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_C_ID);
			}
			break;

		/* Phase C Instantaneous Q3 Apparent Power */
		case 86:	
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_C_ID);
			if ( quadrant == (uint8_t)QUADRANT_3 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_C_ID);
			}
			break;

		/* Phase C Instantaneous Q4 Apparent Power */
		case 87:
			quadrant = (uint8_t)METROLOGY_APP_dGet_instantaneous_value(QUADRANT_C_ID);
			if ( quadrant == (uint8_t)QUADRANT_4 ) {
				inst_value = METROLOGY_APP_dGet_instantaneous_value(S_C_ID);
			}
			break;
#endif
	
		default:
			inst_value = 0;
			break;
	}
	
	if (meter_os.metrology_ready == false) {
		inst_value = 0;
	}
	
	return inst_value;
}

/** 
 ******************************************************************************
 * \fn bool REGS_APP_bCopy_summation(DATA_BLOCK_TYPE data_block, SUMMATION_ID_TYPE summation_id, uint64_t *value_ptr)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bCopy_summation(DATA_BLOCK_TYPE data_block, SUMMATION_ID_TYPE summation_id, uint64_t *value_ptr) {
	
	bool id_exist = false;
	uint64_t summation = 0;
	
	for ( uint8_t i = 0; i < st_21.nbr_summations; i++ ) {
		
		if ( st_22.summation_select[i] == summation_id ) {
			
			switch (data_block) {
				
				case TOTAL_DATA:
					summation = (uint64_t)st_23.tot_data_block.summations[i];
					id_exist = true;
					break;
				
				case TIER_A_DATA:
					if (st_21.nbr_tiers >= 0) {
						summation = (uint64_t)st_23.tier_data_block[0].summations[i];
						id_exist = true;
					}
					break;
					
				case TIER_B_DATA:
					if (st_21.nbr_tiers >= 1) {
						summation = (uint64_t)st_23.tier_data_block[1].summations[i];
						id_exist = true;
					}
					break;
				
				case TIER_C_DATA:
					if (st_21.nbr_tiers >= 2) {
						summation = (uint64_t)st_23.tier_data_block[2].summations[i];
						id_exist = true;
					}
					break;
				
				case TIER_D_DATA:
					if (st_21.nbr_tiers >= 3) {
						summation = (uint64_t)st_23.tier_data_block[3].summations[i];
						id_exist = true;
					}
					break;
					
				default:
					summation = 0;
					break;
			}
			
			break;
		}
	}
	
	*value_ptr = summation;
	
	return id_exist;
}

/** 
 ******************************************************************************
 * \fn bool REGS_APP_bCopy_average_demand(DATA_BLOCK_TYPE data_block, AVERAGE_DEMAND_ID_TYPE demand_id, uint64_t *value_ptr)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bCopy_average_demand(DATA_BLOCK_TYPE data_block, AVERAGE_DEMAND_ID_TYPE demand_id, uint64_t *value_ptr) {
	
	bool id_exist = false;
	uint64_t demand = 0;
	
	for ( uint8_t i = 0; i < st_21.nbr_demands; i++ ) {
		
		if ( st_22.demand_select[i] == demand_id ) {
			
			switch (data_block) {
				
				case TOTAL_DATA:
					demand = (uint64_t)st_23.tot_data_block.demands[i].demand[0];
					id_exist = true;
					break;
				
				case TIER_A_DATA:
					if (st_21.nbr_tiers >= 0) {
						demand = (uint64_t)st_23.tier_data_block[0].demands[i].demand[0];
						id_exist = true;
					}
					break;
					
				case TIER_B_DATA:
					if (st_21.nbr_tiers >= 1) {
						demand = (uint64_t)st_23.tier_data_block[1].demands[i].demand[0];
						id_exist = true;
					}
					break;
				
				case TIER_C_DATA:
					if (st_21.nbr_tiers >= 2) {
						demand = (uint64_t)st_23.tier_data_block[2].demands[i].demand[0];
						id_exist = true;
					}
					break;
				
				case TIER_D_DATA:
					if (st_21.nbr_tiers >= 3) {
						demand = (uint64_t)st_23.tier_data_block[3].demands[i].demand[0];
						id_exist = true;
					}
					break;
					
				default:
					demand = 0;
					break;
			}
			
			break;
		}
	}
	
	*value_ptr = demand;
	
	return id_exist;
}

/** 
 ******************************************************************************
 * \fn bool REGS_APP_bCopy_present_demand(DEMAND_INTERVAL_ID_TYPE int_nbr, PRESENT_DEMAND_ID_TYPE demand_id, uint64_t value_ptr)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
bool REGS_APP_bCopy_present_demand(DEMAND_INTERVAL_ID_TYPE int_nbr, PRESENT_DEMAND_ID_TYPE demand_id, uint64_t *value_ptr) {
		
	bool id_eixst = false;
	uint64_t present_demand = 0;
	
	for ( uint8_t i = 0; i < st_21.nbr_demands; i++ ) {
		
		if ( (uint8_t)(st_22.demand_select[i] - AD_W_DEL_ID) == (uint8_t)(demand_id - PD_W_DEL_ID)) {
			
			if ( demand.interval[int_nbr].present_demand[i] < 0 ) {
				
				present_demand = (uint64_t)( demand.interval[int_nbr].present_demand[i] * (-1) );
			}
			else {
				present_demand = (uint64_t)(demand.interval[int_nbr].present_demand[i]);
			}
			
			id_eixst = true;
		}
	}
	
	*value_ptr = present_demand;
	
	return true;
}
/** 
 ******************************************************************************
 * \fn void REGS_APP_vUpdate_demand_registers(PRESENT_DEMAND_INTERVAL_TYPE* demand_interval)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vUpdate_demand_registers(PRESENT_DEMAND_INTERVAL_TYPE* demand_interval) {
	
	uint64_t cont_cum_demand;
	uint32_t timestamp = rtc.utc_timestamp;
	uint8_t curr_tier;
	
	curr_tier = TOU_APP_ucGet_current_tier();
	
	for ( uint8_t i = 0; i < st_21.nbr_demands; i++ ) {
		
		uint64_t demand = (uint64_t)( fabs(demand_interval->present_demand[i]) );
			
		if ( demand > st_23.tot_data_block.demands[i].demand[0] ) {
			
			/* Update Total Max Demand */
			st_23.tot_data_block.demands[i].demand[0] = demand;
		
			if ( meter.ed_mode.test_mode.enabled == false ) {						
				EEPROM_writer( ST_23_TOT_DEMANDS_DEMAND_ADDR + i*NI_FMAT2_SIZE, &demand, NI_FMAT2_SIZE );	
			}															
			
			/* Total Continuous Cumulative Demand */
			cont_cum_demand = 0;
			EEPROM_reader( ST_23_TOT_DEMANDS_CONT_CUM_DEMAND_ADDR + i*NI_FMAT1_SIZE, &cont_cum_demand, NI_FMAT1_SIZE );
			cont_cum_demand += demand;
			st_23.tot_data_block.demands[i].cont_cum_demand = cont_cum_demand;
				
			if ( meter.ed_mode.test_mode.enabled == false ) {
				EEPROM_writer( ST_23_TOT_DEMANDS_CONT_CUM_DEMAND_ADDR + i*NI_FMAT1_SIZE, &cont_cum_demand, NI_FMAT1_SIZE );
			}
			
			/* Store Event Time Total Demands */
			st_23.tot_data_block.demands[i].event_time[0] = timestamp;
			if ( meter.ed_mode.test_mode.enabled == false ) {										
				EEPROM_writer( ST_23_TOT_DEMANDS_EVENT_TIME_ADDR + i*TM_FMAT_SIZE, &timestamp, TM_FMAT_SIZE );
			}																			
			
			WATCHDOG_APP_vRestart();																			
		}
		
		if ( (demand > st_23.tier_data_block[curr_tier].demands[i].demand[0]) && (meter.ed_mode.test_mode.enabled == false) ) {
			
			/* Update Total Tier Demand */
			st_23.tier_data_block[curr_tier].demands[i].demand[0] = demand;
			
			if ( meter.ed_mode.test_mode.enabled == false ) {
				EEPROM_writer( ST_23_TIER_DEMANDS_DEMAND_ADDR + (curr_tier * NI_FMAT2_SIZE * MAX_NBR_DEMANDS) + (i * NI_FMAT2_SIZE), &demand, NI_FMAT2_SIZE );
			}
				
			/* Tier Continuous Cumulative Demand */
			cont_cum_demand = 0;
			EEPROM_reader( ST_23_TIER_DEMANDS_CONT_CUM_DEMAND_ADDR + (curr_tier * NI_FMAT1_SIZE * MAX_NBR_DEMANDS) + (i * NI_FMAT1_SIZE), &cont_cum_demand, NI_FMAT1_SIZE );
			cont_cum_demand += demand;
			st_23.tier_data_block[curr_tier].demands[i].cont_cum_demand = cont_cum_demand;
			
			if ( meter.ed_mode.test_mode.enabled == false ) {
				EEPROM_writer( ST_23_TIER_DEMANDS_CONT_CUM_DEMAND_ADDR + (curr_tier * NI_FMAT1_SIZE * MAX_NBR_DEMANDS) + (i * NI_FMAT1_SIZE), &cont_cum_demand, NI_FMAT1_SIZE );
			}
			
			/* Store Event Time Tier Demands */ 
			st_23.tier_data_block[curr_tier].demands[i].event_time[0] = timestamp;
			EEPROM_writer( ST_23_TIER_DEMANDS_EVENT_TIME_ADDR + (curr_tier * TM_FMAT_SIZE * MAX_NBR_DEMANDS) + (i * TM_FMAT_SIZE), &timestamp, TM_FMAT_SIZE );
			
			WATCHDOG_APP_vRestart();
		}
	}
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vLoad_demand_configuration(void)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vLoad_demand_configuration(void) {
	
	if ( (st_11.source_flags & BLOCK_DEMAND_FLAG_bm) == BLOCK_DEMAND_FLAG_bm ) {
		
		/* Block Demand */
		
		uint16_t int_len_min;
		
		EEPROM_reader( ST_13_INT_LENGTH_ADDR, &int_len_min, 2 );
		
		demand.config.is_sliding_demand = false;
		demand.config.nbr_intervals = 0;
		demand.config.interval_length = int_len_min * SECONDS_PER_MINUTE;
 		demand.config.subinterval_length = 0;
	}
	else { 
		
		/* Sliding demand */
		
		uint8_t sub_int_min;
		uint8_t int_multiplier;
		
		EEPROM_reader( ST_13_SUB_INT_ADDR, &sub_int_min, 1 );
		EEPROM_reader( ST_13_INT_MULTIPLIER_ADDR, &int_multiplier, 1 );
		
		demand.config.is_sliding_demand = true;
		demand.config.nbr_intervals = int_multiplier;
		demand.config.interval_length = sub_int_min * int_multiplier * SECONDS_PER_MINUTE;
		demand.config.subinterval_length = sub_int_min * SECONDS_PER_MINUTE;
	}
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vStore_summations(void)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vStore_summations(void) {
	
	uint8_t i, j;
	uint64_t summation;

	st_23_struct_copy = st_23;
	
	if ( (!meter.ed_mode.test_mode.enabled) ) {
		
		for ( i = 0; i < st_21.nbr_summations; i++ ){
			
			summation = (uint64_t)(st_23_struct_copy.tot_data_block.summations[i]);
			EEPROM_writer( ST_23_TOT_SUMMATIONS_ADDR + i*NI_FMAT1_SIZE, &summation, NI_FMAT1_SIZE );
		}
		
		for ( i = 0; i < st_21.nbr_tiers; i++ ) {
			
			for ( j = 0; j < st_21.nbr_summations; j++ ) {
				
				summation = (uint64_t)(st_23_struct_copy.tier_data_block[i].summations[j]);
				EEPROM_writer( ST_23_TIER_SUMMATIONS_ADDR + (i * NI_FMAT1_SIZE * MAX_NBR_SUMMATIONS) + (j * NI_FMAT1_SIZE), &summation, NI_FMAT1_SIZE );
			}
		}
	}
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vReset_summations(void)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vReset_summations(void)
{
	uint8_t i, j;
	uint64_t summation;
	
	if ( meter.ed_mode.test_mode.enabled == false ) {
		
		for ( i = 0; i < MAX_NBR_SUMMATIONS; i++) {
			
			summation = 0;
			st_23.tot_data_block.summations[i] = 0;
			EEPROM_writer( ST_23_TOT_SUMMATIONS_ADDR + i*NI_FMAT1_SIZE, &summation, NI_FMAT1_SIZE );
		}
	
		for ( i = 0; i < MAX_NBR_TIERS; i++ ) {
			
			for ( j = 0; j < MAX_NBR_SUMMATIONS; j++ ) {
				
				summation = 0;
				st_23.tier_data_block[i].summations[j] = 0;
				EEPROM_writer( ST_23_TIER_SUMMATIONS_ADDR + (i * NI_FMAT1_SIZE * MAX_NBR_SUMMATIONS) + (j * NI_FMAT1_SIZE), &summation, NI_FMAT1_SIZE );
			}
		}
	}
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vEnable_test_mode(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vEnable_test_mode(void) {
	
	REGS_APP_vStore_summations();
	
	st_21.nbr_summations = 3;
	st_22.summation_select[0] = SM_WH_SUM_ID;
	st_22.summation_select[1] = SM_VARH_SUM_ID;
	st_22.summation_select[2] = SM_VAH_SUM_ID;
	
	st_21.nbr_demands = 2;
	st_22.demand_select[0] = AD_W_SUM_ID;
	st_22.demand_select[1] = AD_VAR_SUM_ID;
	
	memset( &st_23, 0, sizeof(st_23) );
	
	REGS_APP_vInitialize();
	meter.ed_mode.test_mode.enabled = true;
}

/**
 ******************************************************************************
 * \fn void REGS_APP_vEnable_test_mode(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vDisable_test_mode(void) {
		
	REGS_APP_bLoad_st_21();
	REGS_APP_bLoad_st_22();
	REGS_APP_bLoad_st_23();
	
	REGS_APP_vInitialize();
	meter.ed_mode.test_mode.enabled = false;
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vDemand_reset(uint16_t user, uint8_t season, uint32_t timestamp)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vDemand_reset(uint16_t user, uint8_t season, uint32_t timestamp) {
	
	uint8_t i, j;

	REGS_APP_vReset_demands_intervals();
	
	if ( !meter.ed_mode.test_mode.enabled ) {
		
		/* Copy current registers ST-23 before demand reset */ 
		REGS_APP_vTake_snapshot_st_23((uint8_t)DEMAND_RESET, season, timestamp);

		st_23.nbr_demand_resets++;
		EEPROM_writer( ST_23_NBR_DEMAND_RESETS_ADDR, &st_23.nbr_demand_resets, 1 );
	}
	
	/* Update Total Data Block Cumulative Demand and then Reset  */ 
	for ( i = 0; i < st_21.nbr_demands; i++) {
			
		st_23.tot_data_block.demands[i].cum_demand += st_23.tot_data_block.demands[i].demand[0];
		st_23.tot_data_block.demands[i].demand[0] = 0;
		st_23.tot_data_block.demands[i].event_time[0] = 0;
		
		if ( !meter.ed_mode.test_mode.enabled ) {
			EEPROM_writer( ST_23_TOT_DEMANDS_CUM_DEMAND_ADDR + i*NI_FMAT1_SIZE, &st_23.tot_data_block.demands[i].cum_demand, NI_FMAT1_SIZE );
			EEPROM_writer( ST_23_TOT_DEMANDS_DEMAND_ADDR + i*NI_FMAT2_SIZE, &st_23.tot_data_block.demands[i].demand[0], NI_FMAT2_SIZE );
			EEPROM_writer( ST_23_TOT_DEMANDS_EVENT_TIME_ADDR + i*TM_FMAT_SIZE, &st_23.tot_data_block.demands[i].event_time[0], TM_FMAT_SIZE );
		}
	}
	
	/* Update Tier Data Block Cumulative Demand and then Reset */ 
	for ( i = 0; i < st_21.nbr_tiers; i++ ) {
		
		for ( j = 0; j < st_21.nbr_demands; j++) {
			
			st_23.tier_data_block[i].demands[j].cum_demand += st_23.tier_data_block[i].demands[j].demand[0];
			st_23.tier_data_block[i].demands[j].demand[0] = 0;
			st_23.tier_data_block[i].demands[j].event_time[0] = 0;
			
			if ( !meter.ed_mode.test_mode.enabled ) {
				EEPROM_writer( ST_23_TIER_DEMANDS_CUM_DEMAND_ADDR + (i * NI_FMAT1_SIZE * MAX_NBR_DEMANDS) + (j * NI_FMAT1_SIZE), &st_23.tier_data_block[i].demands[j].cum_demand, NI_FMAT1_SIZE );
				EEPROM_writer( ST_23_TIER_DEMANDS_DEMAND_ADDR + (i * NI_FMAT2_SIZE * MAX_NBR_DEMANDS) + (j * NI_FMAT2_SIZE), &st_23.tier_data_block[i].demands[j].demand[0], NI_FMAT2_SIZE );
				EEPROM_writer( ST_23_TIER_DEMANDS_EVENT_TIME_ADDR + (i * TM_FMAT_SIZE * MAX_NBR_DEMANDS) + (j * TM_FMAT_SIZE), &st_23.tier_data_block[i].demands[j].event_time[0], TM_FMAT_SIZE );
			}
		}
	}

	REGS_APP_vReset_demands_intervals();	
	EVNT_APP_vRegister_event(DEMAND_RESET_OCCURRED, 0, true, user, timestamp);
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vSelf_read(uint16_t user_attempt)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vSelf_read(uint16_t user, uint8_t season, uint32_t timestamp) {
	
	uint8_t list_status, nbr_valid_entries, last_entry_element, nbr_unread_entries;
	uint16_t last_entry_seq_nbr;
	uint32_t address;
	
	/*---------------------------------------------------------------------------*\
	 * LIST_STATUS
	\*---------------------------------------------------------------------------*/
		EEPROM_reader( ST_26_LIST_STATUS_ADDR, &list_status, 1 );
	
	/*---------------------------------------------------------------------------*\
	 * NBR_VALID_ENTRIES
	\*---------------------------------------------------------------------------*/
		EEPROM_reader( ST_26_NBR_VALID_ENTRIES_ADDR, &nbr_valid_entries, 1 );
		if ( nbr_valid_entries < st_21.nbr_self_reads ) {
			nbr_valid_entries++;
		}
		EEPROM_writer( ST_26_NBR_VALID_ENTRIES_ADDR, &nbr_valid_entries, 1 );
	
	/*---------------------------------------------------------------------------*\
	 * LAST_ENTRY_ELEMENT
	\*---------------------------------------------------------------------------*/
		EEPROM_reader( ST_26_LAST_ENTRY_ELEMENT_ADDR, &last_entry_element, 1 );
		last_entry_element++;
		if ( last_entry_element > st_21.nbr_self_reads ){
			last_entry_element = 1;
		}
		EEPROM_writer( ST_26_LAST_ENTRY_ELEMENT_ADDR, &last_entry_element, 1 );
	
	/*---------------------------------------------------------------------------*\
	 * LAST_ENTRY_SEQ_NBR
	\*---------------------------------------------------------------------------*/
		EEPROM_reader( ST_26_LAST_ENTRY_SEQ_NBR_ADDR, &last_entry_seq_nbr, 2 );
		last_entry_seq_nbr++;
		EEPROM_writer( ST_26_LAST_ENTRY_SEQ_NBR_ADDR, &last_entry_seq_nbr, 2 );

	/*---------------------------------------------------------------------------*\
	 * SELF_READS_ENTRIES
	\*---------------------------------------------------------------------------*/
		
		/* SELF_READ_SEQ_NBR */
		if ( (st_21.reg_func2_flags & SELF_READ_SEQ_NBR_FLAG_bm) == SELF_READ_SEQ_NBR_FLAG_bm ) {
			address = ST_26_SELF_READ_SEQ_NBR_ADDR + ( (last_entry_element - 1) * 2 );
			EEPROM_writer( (uint32_t)address, &last_entry_seq_nbr, 2 );
		}
		
		REGS_APP_vTake_snapshot_st_23((uint8_t)SELF_READ, season, timestamp);
		EVNT_APP_vRegister_event(SELF_READ_OCCURRED, 0, true, user, timestamp);
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vGet_interval_time_remining(DEMAND_INTERVAL_ID_TYPE int_nbr, uint8_t* minute, uint8_t* second)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vGet_interval_time_remining(DEMAND_INTERVAL_ID_TYPE int_nbr, uint8_t* minute, uint8_t* second) {
	
	uint16_t currIntTime;
	
	currIntTime = demand.config.interval_length - demand.interval[int_nbr].current_seconds;
	
	if (currIntTime == demand.config.interval_length) {
		currIntTime = 0;
	}
	
	*second = currIntTime % 60;
	currIntTime /= 60;
	*minute = currIntTime % 60;
}

/** 
 ******************************************************************************
 * \fn uint16_t REGS_APP_vGet_present_demand_time_remining(DEMAND_INTERVAL_ID_TYPE int_nbr)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t REGS_APP_vGet_present_demand_time_remining(DEMAND_INTERVAL_ID_TYPE int_nbr) {
	
	uint16_t remaining_time;
	
	remaining_time = demand.config.interval_length - demand.interval[int_nbr].current_seconds;
	
	return remaining_time;
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vReset_demands_intervals(void)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vReset_demands_intervals(void) {
	
	uint8_t i;
	
	for ( i = 0; i < MAX_NBR_DEMANDS; i++ ) {
		demand.interval[i].current_seconds = 0;
		demand.interval[i].is_int_expiried = 0;
		demand.interval[i].run_next_interval = false;
		
		memset( &demand.interval[i].present_demand, 0, sizeof(demand.interval[i].present_demand) );
	}
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vReset_present_demand(void)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vReset_present_demand(void) {
	
	uint8_t i;
	
	for ( i = 0; i < MAX_NBR_DEMANDS; i++ ) {
		memset( &demand.interval[i].present_demand, 0, sizeof(demand.interval[i].present_demand) );
	}
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vTake_snapshot_st_23(uint8_t cause, uint8_t season, uint32_t timestamp)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vTake_snapshot_st_23(uint8_t cause, uint8_t season, uint32_t timestamp) {
	
	uint16_t data_length;
	uint8_t last_entry_element;
	uint32_t addr[3];
	
	if (meter.ed_mode.test_mode.enabled == false) {
		
		switch (cause) {
			
			case SEASON_CHANGE:
				addr[0] = ST_24_END_DATE_TIME_ADDR;
				addr[1] = ST_24_SEASON_ADDR;
				addr[2] = ST_24_PREV_SEASON_REG_DATA_ADDR;					
				break;
			
			case DEMAND_RESET:
				addr[0] = ST_25_END_DATE_TIME_ADDR;
				addr[1] = ST_25_SEASON_ADDR;
				addr[2] = ST_25_PREV_DEMAND_RESET_DATA_ADDR;					
				break;
			
			case SELF_READ:
				EEPROM_reader( ST_26_LAST_ENTRY_ELEMENT_ADDR, &last_entry_element, 1 );
			
				addr[0] = ST_26_END_DATE_TIME_ADDR + ( (last_entry_element - 1) * 4 );
				addr[1] = ST_26_SEASON_ADDR + ( (last_entry_element - 1) * 1 );
				addr[2] = ST_26_SELF_READ_REGISTER_DATA_ADDR + (last_entry_element - 1)*ST_23_MAX_SIZE;	
				break;
			
			default:
				break;	
		}
	
		// REGISTER_INFO
		EEPROM_writer( addr[0], &timestamp, TM_FMAT_SIZE );
		EEPROM_writer( addr[1], &season, 1 );
	
		// PREV_SEASON_REG_DATA | PREV_DEMAND_RESET_DATA | SELF_READ_REGISTER_DATA
		data_length = REGS_APP_usSt_23_get_size();
		EEPROM_writer( addr[2], &st_23_copy, data_length );
	}
}

/** 
 ******************************************************************************
 * \fn void REGS_APP_vCopy_st_23(void)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void REGS_APP_vCopy_st_23(void) {
	
	REGS_APP_bSt_23_execute_table_reading(&st_23_copy[0]);
}

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-06-22
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */