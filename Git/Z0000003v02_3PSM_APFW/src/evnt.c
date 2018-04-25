/**
 ******************************************************************************
 * \file hist_evnt.c
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
#include "gral.h"
#include "evnt.h"
#include "regs.h"
#include "sm.h"
#include "compiler.h"
#include "eeprom.h"
#include "nic.h"

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
static uint16_t event_number;
static ST_74_TYPE st_74;
static ST_74_TYPE st_76;

/*
 ******************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ******************************************************************************
 */
ST_70_TYPE st_71;
ST_73_TYPE st_73;
ST_73_TYPE st_75;
MT_73_TYPE mt_73;

/*
 ******************************************************************************
 * ROM Const Variables With File Level Scope
 ******************************************************************************
 */

/**
 ******************************************************************************
 * Function Prototypes for Private Functions with File Level Scope
 ******************************************************************************
 */
static void EVNT_APP_vLoad_event_number(void);

static void EVNT_APP_vRegister_event_in_st_74(EVENT_ALARM_TYPE xEvent);
static void EVNT_APP_vRegister_event_in_st_76(EVENT_ALARM_TYPE xEvent);

static bool EVNT_APP_bIs_std_event_monitored_by_st_73(uint8_t code);
static bool EVNT_APP_bIs_mfg_event_monitored_by_st_73(uint8_t code);
static bool EVNT_APP_bIs_table_monitored_by_st_73(uint16_t table_ida_bfld);
static bool EVNT_APP_bIs_procedure_monitored_by_st_73(uint16_t table_idc_bfld);

static bool EVNT_APP_bIs_std_event_monitored_by_st_75(uint8_t code);
static bool EVNT_APP_bIs_mfg_event_monitored_by_st_75(uint8_t code);
static bool EVNT_APP_bIs_table_monitored_by_st_75(uint16_t table_ida_bfld);
static bool EVNT_APP_bIs_procedure_monitored_by_st_75(uint16_t table_idc_bfld);

static bool EVNT_APP_bSt_71_validate_data(ST_70_TYPE *st_71_structure);
static void EVNT_APP_vSt_71_read_eeprom_values(ST_70_TYPE *st_71_structure);
static void EVNT_APP_vSt_71_write_eeprom_values(ST_70_TYPE *st_71_structure);
static void EVNT_APP_vSt_71_to_array(ST_70_TYPE st_71_source, uint8_t *table_buffer);

static bool EVNT_APP_bSt_73_validate_data(ST_73_TYPE *st_73_structure);
static void EVNT_APP_vSt_73_read_eeprom_values(ST_73_TYPE *st_73_structure);
static void EVNT_APP_vSt_73_write_eeprom_values(ST_73_TYPE *st_73_structure);
static void EVNT_APP_vSt_73_to_array(ST_73_TYPE st_73_source, uint8_t *table_buffer);

static bool EVNT_APP_bMt_73_validate_data(MT_73_TYPE *mt_73_structure);
static void EVNT_APP_vMt_73_read_eeprom_values(MT_73_TYPE *mt_73_structure);
static void EVNT_APP_vMt_73_write_eeprom_values(MT_73_TYPE *mt_73_structure);
static void EVNT_APP_vMt_73_to_array(MT_73_TYPE mt_73_source, uint8_t *table_buffer);

static bool EVNT_APP_bSt_74_validate_data(ST_74_TYPE *st_74_structure);
static void EVNT_APP_vSt_74_read_eeprom_values(ST_74_TYPE *st_74_structure);
static void EVNT_APP_vSt_74_write_eeprom_values(ST_74_TYPE *st_74_structure);

static bool EVNT_APP_bSt_75_validate_data(ST_73_TYPE *st_75_structure);
static void EVNT_APP_vSt_75_read_eeprom_values(ST_73_TYPE *st_75_structure);
static void EVNT_APP_vSt_75_write_eeprom_values(ST_73_TYPE *st_75_structure);
static void EVNT_APP_vSt_75_to_array(ST_73_TYPE st_75_source, uint8_t *table_buffer);

static bool EVNT_APP_bSt_76_validate_data(ST_74_TYPE *st_76_structure);
static void EVNT_APP_vSt_76_read_eeprom_values(ST_74_TYPE *st_76_structure);
static void EVNT_APP_vSt_76_write_eeprom_values(ST_74_TYPE *st_76_structure);

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */

/**
 ******************************************************************************
 * \fn bool EVNT_APP_ubSt_70_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_ubSt_70_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;

	table_buffer[k++] = (uint8_t)(MAX_LOG_FLAGS);
	table_buffer[k++] = (uint8_t)(MAX_NBR_STD_EVENTS);
	table_buffer[k++] = (uint8_t)(MAX_NBR_MFG_EVENTS);
	table_buffer[k++] = (uint8_t)(MAX_HIST_DATA_LENGTH);
	table_buffer[k++] = (uint8_t)(MAX_EVENT_DATA_LENGTH);
	table_buffer[k++] = (uint8_t)((MAX_NBR_HISTORY_ENTRIES & 0x00FF) >> 0);
	table_buffer[k++] = (uint8_t)((MAX_NBR_HISTORY_ENTRIES & 0xFF00) >> 8);
	table_buffer[k++] = (uint8_t)((MAX_NBR_EVENT_ENTRIES & 0x00FF) >> 0);
	table_buffer[k++] = (uint8_t)((MAX_NBR_EVENT_ENTRIES & 0xFF00) >> 8);
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t EVNT_APP_usSt_70_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t EVNT_APP_usSt_70_get_size(void) {

	return (uint16_t) ST_70_SIZE;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bLoad_st_71(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bLoad_st_71(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_71_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = EVNT_APP_bSt_71_restore_default_values();
	}
	else {
		
		is_ok = EVNT_APP_bSt_71_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_71_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_71_restore_default_values(void) {
	
	bool is_ok = false;
	ST_70_TYPE st_71_temp;
	
	st_71_temp.log_flags = DEFAULT_LOG_FLAGS;
	st_71_temp.nbr_std_events = DEFAULT_NBR_STD_EVENTS;
	st_71_temp.nbr_mfg_events = DEFAULT_NBR_MFG_EVENTS;
	st_71_temp.hist_data_length = DEFAULT_HIST_DATA_LENGTH;
	st_71_temp.event_data_length = DEFAULT_EVENT_DATA_LENGTH;
	st_71_temp.nbr_history_entries = DEFAULT_NBR_HISTORY_ENTRIES;
	st_71_temp.nbr_event_entries = DEFAULT_NBR_EVENT_ENTRIES;
	
	is_ok = EVNT_APP_bSt_71_validate_data(&st_71_temp);
	
	st_71 = st_71_temp;
	EVNT_APP_vSt_71_write_eeprom_values(&st_71);
	uint16_t crc = SM_APP_usSt_71_ram_calculate_crc();
	SM_APP_vSt_71_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_71_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_71_refresh_ram(void) {
	
	ST_70_TYPE st_71_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		EVNT_APP_vSt_71_read_eeprom_values(&st_71_temp);
		is_ok = EVNT_APP_bSt_71_validate_data(&st_71_temp);
		
		if ( is_ok ) {
			st_71 = st_71_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_71 = st_71_temp;
		EVNT_APP_vSt_71_write_eeprom_values(&st_71);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vCopy_st_71_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vCopy_st_71_ram(uint8_t* destination) {
	
	EVNT_APP_vSt_71_to_array(st_71, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vCopy_st_71_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vCopy_st_71_nvm(uint8_t* destination) {
	
	ST_70_TYPE st_71_aux;
	
	EVNT_APP_vSt_71_read_eeprom_values(&st_71_aux);
	EVNT_APP_vSt_71_to_array(st_71_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_71_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_71_recover_nvm(void) {
	
	EVNT_APP_vSt_71_write_eeprom_values(&st_71);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_71_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_71_recover_ram(void) {
	
	EVNT_APP_vSt_71_read_eeprom_values(&st_71);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_71_to_array(ST_70_TYPE st_71_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_71_to_array(ST_70_TYPE st_71_source, uint8_t *table_buffer) {
	
	table_buffer[0] = st_71_source.log_flags;
	table_buffer[1] = st_71_source.nbr_std_events;
	table_buffer[2] = st_71_source.nbr_mfg_events;
	table_buffer[3] = st_71_source.hist_data_length;
	table_buffer[4] = st_71_source.event_data_length;
	table_buffer[5] = (uint8_t)((st_71_source.nbr_history_entries & 0x00FF) >> 0);
	table_buffer[6] = (uint8_t)((st_71_source.nbr_history_entries & 0xFF00) >> 8);
	table_buffer[7] = (uint8_t)((st_71_source.nbr_event_entries & 0x00FF) >> 0);
	table_buffer[8] = (uint8_t)((st_71_source.nbr_event_entries & 0xFF00) >> 8);
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_71_validate_data(ST_70_TYPE *st_71_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_71_validate_data(ST_70_TYPE *st_71_structure) {
	
	bool is_ok = true;
	uint8_t i;
	
	for ( i = 0; i < 8; i++) {
		if ( ( ( (MAX_LOG_FLAGS >> i) & 0x01 ) == 0 ) && ( ( (st_71_structure->log_flags >> i) & 0x01 ) == 1 ) ) {
			is_ok = false;	
		}
	}
	st_71_structure->log_flags &= MAX_LOG_FLAGS;
	
	if (st_71_structure->nbr_std_events > MAX_NBR_STD_EVENTS) {
		st_71_structure->nbr_std_events = MAX_NBR_STD_EVENTS;
		is_ok = false;
	}
	
	if (st_71_structure->nbr_mfg_events > MAX_NBR_MFG_EVENTS) {
		st_71_structure->nbr_mfg_events = MAX_NBR_MFG_EVENTS;
		is_ok = false;
	}
	
	if (st_71_structure->hist_data_length > MAX_HIST_DATA_LENGTH) {
		st_71_structure->hist_data_length = MAX_HIST_DATA_LENGTH;
		is_ok = false;
	}
	
	if (st_71_structure->event_data_length > MAX_EVENT_DATA_LENGTH ) {
		st_71_structure->event_data_length = MAX_EVENT_DATA_LENGTH;
		is_ok = false;
	}
	
	if (st_71_structure->nbr_history_entries > MAX_NBR_HISTORY_ENTRIES) {
		st_71_structure->nbr_history_entries = MAX_NBR_HISTORY_ENTRIES;
		is_ok = false;
	}
	
	if (st_71_structure->nbr_event_entries > MAX_NBR_EVENT_ENTRIES) {
		st_71_structure->nbr_event_entries = MAX_NBR_EVENT_ENTRIES;
		is_ok = false;
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_71_read_eeprom_values(ST_70_TYPE *st_71_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_71_read_eeprom_values(ST_70_TYPE *st_71_structure) {	
	
	EEPROM_reader( ST_71_LOG_FLAGS_ADDR, &st_71_structure->log_flags, 1 );
	EEPROM_reader( ST_71_NBR_STD_EVENTS_ADDR, &st_71_structure->nbr_std_events, 1 );
	EEPROM_reader( ST_71_NBR_MFG_EVENTS_ADDR, &st_71_structure->nbr_mfg_events, 1 );
	EEPROM_reader( ST_71_HIST_DATA_LENGTH_ADDR, &st_71_structure->hist_data_length, 1 );
	EEPROM_reader( ST_71_EVENT_DATA_LENGTH_ADDR, &st_71_structure->event_data_length, 1 );
	EEPROM_reader( ST_71_NBR_HISTORY_ENTRIES_ADDR, &st_71_structure->nbr_history_entries, 2 );
	EEPROM_reader( ST_71_NBR_EVENT_ENTRIES_ADDR, &st_71_structure->nbr_event_entries, 2 );
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_71_write_eeprom_values(ST_70_TYPE *st_71_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_71_write_eeprom_values(ST_70_TYPE *st_71_structure) {
	
	SM_APP_vChange_table_state(ST_71_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( ST_71_LOG_FLAGS_ADDR, &st_71_structure->log_flags, 1 );
	EEPROM_writer( ST_71_NBR_STD_EVENTS_ADDR, &st_71_structure->nbr_std_events, 1 );
	EEPROM_writer( ST_71_NBR_MFG_EVENTS_ADDR, &st_71_structure->nbr_mfg_events, 1 );
	EEPROM_writer( ST_71_HIST_DATA_LENGTH_ADDR, &st_71_structure->hist_data_length, 1 );
	EEPROM_writer( ST_71_EVENT_DATA_LENGTH_ADDR, &st_71_structure->event_data_length, 1 );
	EEPROM_writer( ST_71_NBR_HISTORY_ENTRIES_ADDR, &st_71_structure->nbr_history_entries, 2 );
	EEPROM_writer( ST_71_NBR_EVENT_ENTRIES_ADDR, &st_71_structure->nbr_event_entries, 2 );
	
	SM_APP_vChange_table_state(ST_71_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_71_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_71_execute_table_reading(uint8_t *table_buffer) {
	
	EVNT_APP_vSt_71_to_array(st_71, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_71_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_71_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	ST_70_TYPE st_71_temp;
	
	st_71_temp.log_flags = table_buffer[k++];
	st_71_temp.nbr_std_events = table_buffer[k++];
	st_71_temp.nbr_mfg_events = table_buffer[k++];
	st_71_temp.hist_data_length = table_buffer[k++];
	st_71_temp.event_data_length = table_buffer[k++];
	st_71_temp.nbr_history_entries  = (uint16_t)(table_buffer[k++] << 0);
	st_71_temp.nbr_history_entries |= (uint16_t)(table_buffer[k++] << 8);
	st_71_temp.nbr_event_entries  = (uint16_t)(table_buffer[k++] << 0);
	st_71_temp.nbr_event_entries |= (uint16_t)(table_buffer[k++] << 8);
	
	is_ok = EVNT_APP_bSt_71_validate_data(&st_71_temp);
	
	if ( is_ok ) {
		st_71 = st_71_temp;
		EVNT_APP_vSt_71_write_eeprom_values(&st_71);
		uint16_t crc = SM_APP_usSt_71_ram_calculate_crc();
		SM_APP_vSt_71_write_crc(crc);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t EVNT_APP_usSt_71_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t EVNT_APP_usSt_71_get_size(void) {

	return 	(uint16_t)ST_71_SIZE;
}

/**
 ******************************************************************************
 * \fn uint8_t EVNT_APP_ucGet_st_72_std_events(uint8_t set_nbr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t EVNT_APP_ucGet_st_72_std_events(uint8_t set_nbr) {
	
	uint8_t set_val;
	
	switch (set_nbr) {
		case 0:		set_val = ST_72_STD_EVENTS_SUPPORTED_SET_0;		break;
		case 1:		set_val = ST_72_STD_EVENTS_SUPPORTED_SET_1;		break;
		case 2:		set_val = ST_72_STD_EVENTS_SUPPORTED_SET_2;		break;
		case 3:		set_val = ST_72_STD_EVENTS_SUPPORTED_SET_3;		break;
		case 4:		set_val = ST_72_STD_EVENTS_SUPPORTED_SET_4;		break;
		case 5:		set_val = ST_72_STD_EVENTS_SUPPORTED_SET_5;		break;
		case 6:		set_val = ST_72_STD_EVENTS_SUPPORTED_SET_6;		break;
		case 7:		set_val = ST_72_STD_EVENTS_SUPPORTED_SET_7;		break;
		case 8:		set_val = ST_72_STD_EVENTS_SUPPORTED_SET_8;		break;
		case 9:		set_val = ST_72_STD_EVENTS_SUPPORTED_SET_9;		break;
		default:	set_val = 0;	break;
	}
	
	return set_val;
}

/**
 ******************************************************************************
 * \fn uint8_t EVNT_APP_ucGet_st_72_mfg_events(uint8_t set_nbr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t EVNT_APP_ucGet_st_72_mfg_events(uint8_t set_nbr) {
	
	uint8_t set_val;
	
	switch (set_nbr) {
		case 0:		set_val = ST_72_MFG_EVENTS_SUPPORTED_SET_0;		break;
		case 1:		set_val = ST_72_MFG_EVENTS_SUPPORTED_SET_1;		break;
		case 2:		set_val = ST_72_MFG_EVENTS_SUPPORTED_SET_2;		break;
		case 3:		set_val = ST_72_MFG_EVENTS_SUPPORTED_SET_3;		break;
		case 4:		set_val = ST_72_MFG_EVENTS_SUPPORTED_SET_4;		break;
		case 5:		set_val = ST_72_MFG_EVENTS_SUPPORTED_SET_5;		break;
		case 6:		set_val = ST_72_MFG_EVENTS_SUPPORTED_SET_6;		break;
		case 7:		set_val = ST_72_MFG_EVENTS_SUPPORTED_SET_7;		break;
		case 8:		set_val = ST_72_MFG_EVENTS_SUPPORTED_SET_8;		break;
		case 9:		set_val = ST_72_MFG_EVENTS_SUPPORTED_SET_9;		break;
		default:	set_val = 0;	break;
	}
	
	return set_val;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_72_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_72_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;
	uint16_t i = 0;

	for ( i = 0; i < st_71.nbr_std_events; i++ )
	{
		table_buffer[k++] = ST_72_STD_EVENTS_SUPPORTED(i);
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ )
	{
		table_buffer[k++] = ST_72_MFG_EVENTS_SUPPORTED(i);
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t EVNT_APP_usSt_72_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t EVNT_APP_usSt_72_get_size(void) {

	uint16_t table_size;
	
	table_size = st_71.nbr_std_events + st_71.nbr_mfg_events;
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn uint8_t EVNT_APP_ucGet_mt_72_std_alarms(uint8_t set_nbr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t EVNT_APP_ucGet_mt_72_std_alarms(uint8_t set_nbr) {
	
	uint8_t set_val;
	
	switch (set_nbr) {
		case 0:		set_val = MT_72_STD_ALARMS_SUPPORTED_SET_0;		break;
		case 1:		set_val = MT_72_STD_ALARMS_SUPPORTED_SET_1;		break;
		case 2:		set_val = MT_72_STD_ALARMS_SUPPORTED_SET_2;		break;
		case 3:		set_val = MT_72_STD_ALARMS_SUPPORTED_SET_3;		break;
		case 4:		set_val = MT_72_STD_ALARMS_SUPPORTED_SET_4;		break;
		case 5:		set_val = MT_72_STD_ALARMS_SUPPORTED_SET_5;		break;
		case 6:		set_val = MT_72_STD_ALARMS_SUPPORTED_SET_6;		break;
		case 7:		set_val = MT_72_STD_ALARMS_SUPPORTED_SET_7;		break;
		case 8:		set_val = MT_72_STD_ALARMS_SUPPORTED_SET_8;		break;
		case 9:		set_val = MT_72_STD_ALARMS_SUPPORTED_SET_9;		break;
		default:	set_val = 0;	break;
	}
	
	return set_val;
}

/**
 ******************************************************************************
 * \fn uint8_t EVNT_APP_ucGet_mt_72_mfg_alarms(uint8_t set_nbr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t EVNT_APP_ucGet_mt_72_mfg_alarms(uint8_t set_nbr) {
	
	uint8_t set_val;
	
	switch (set_nbr) {
		case 0:		set_val = MT_72_MFG_ALARMS_SUPPORTED_SET_0;		break;
		case 1:		set_val = MT_72_MFG_ALARMS_SUPPORTED_SET_1;		break;
		case 2:		set_val = MT_72_MFG_ALARMS_SUPPORTED_SET_2;		break;
		case 3:		set_val = MT_72_MFG_ALARMS_SUPPORTED_SET_3;		break;
		case 4:		set_val = MT_72_MFG_ALARMS_SUPPORTED_SET_4;		break;
		case 5:		set_val = MT_72_MFG_ALARMS_SUPPORTED_SET_5;		break;
		case 6:		set_val = MT_72_MFG_ALARMS_SUPPORTED_SET_6;		break;
		case 7:		set_val = MT_72_MFG_ALARMS_SUPPORTED_SET_7;		break;
		case 8:		set_val = MT_72_MFG_ALARMS_SUPPORTED_SET_8;		break;
		case 9:		set_val = MT_72_MFG_ALARMS_SUPPORTED_SET_9;		break;
		default:	set_val = 0;	break;
	}
	
	return set_val;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bMt_72_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bMt_72_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;
	uint16_t i = 0;

	for ( i = 0; i < st_71.nbr_std_events; i++ )
	{
		table_buffer[k++] = MT_72_STD_ALARMS_SUPPORTED(i);
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ )
	{
		table_buffer[k++] = MT_72_MFG_ALARMS_SUPPORTED(i);
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t EVNT_APP_usMt_72_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t EVNT_APP_usMt_72_get_size(void) {

	uint16_t table_size;
	
	table_size = st_71.nbr_std_events + st_71.nbr_mfg_events;
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bLoad_st_73(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bLoad_st_73(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_73_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = EVNT_APP_bSt_73_restore_default_values();
	}
	else {
		
		is_ok = EVNT_APP_bSt_73_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_73_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_73_restore_default_values(void) {
	
	bool is_ok = false;
	ST_73_TYPE st_73_temp;
	uint8_t i;

	memset( &st_73_temp, 0, sizeof(st_73_temp) );
	
	for ( i = 0; i < st_71.nbr_std_events; i++ ) {
		st_73_temp.std_events_monitored_flags[i] = ST_73_DEFAULT_STD_EVENTS_MONITORED(i);
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ ) {
		st_73_temp.mfg_events_monitored_flags[i] = ST_73_DEFAULT_MFG_EVENTS_MONITORED(i);
	}
	
	GRAL_APP_vLoad_std_tbls_write(&st_73_temp.std_tbls_monitored_flags[0]);
	GRAL_APP_vLoad_mfg_tbls_write(&st_73_temp.mfg_tbls_monitored_flags[0]);
	GRAL_APP_vLoad_std_proc_used(&st_73_temp.std_proc_monitored_flags[0]);
	GRAL_APP_vLoad_mfg_proc_used(&st_73_temp.mfg_proc_monitored_flags[0]);
	
	is_ok = EVNT_APP_bSt_73_validate_data(&st_73_temp);
	
	st_73 = st_73_temp;
	EVNT_APP_vSt_73_write_eeprom_values(&st_73);
	uint16_t crc = SM_APP_usSt_73_ram_calculate_crc();
	SM_APP_vSt_73_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_73_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_73_refresh_ram(void) {
	
	ST_73_TYPE st_73_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		EVNT_APP_vSt_73_read_eeprom_values(&st_73_temp);
		is_ok = EVNT_APP_bSt_73_validate_data(&st_73_temp);
		
		if ( is_ok ) {
			st_73 = st_73_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_73 = st_73_temp;
		EVNT_APP_vSt_73_write_eeprom_values(&st_73);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  EVNT_APP_vCopy_st_73_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vCopy_st_73_ram(uint8_t* destination) {
	
	EVNT_APP_vSt_73_to_array(st_73, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vCopy_st_73_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vCopy_st_73_nvm(uint8_t* destination) {
	
	ST_73_TYPE st_73_aux;
	
	EVNT_APP_vSt_73_read_eeprom_values(&st_73_aux);
	EVNT_APP_vSt_73_to_array(st_73_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_73_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_73_recover_nvm(void) {
	
	EVNT_APP_vSt_73_write_eeprom_values(&st_73);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_73_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_73_recover_ram(void) {
	
	EVNT_APP_vSt_73_read_eeprom_values(&st_73);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_73_to_array(ST_73_TYPE st_73_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_73_to_array(ST_73_TYPE st_73_source, uint8_t *table_buffer) {
		
	uint16_t i = 0;
	uint16_t k = 0;

	for ( i = 0; i < st_71.nbr_std_events; i++ ) {
		table_buffer[k++] = st_73_source.std_events_monitored_flags[i];
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ ) {
		table_buffer[k++] = st_73_source.mfg_events_monitored_flags[i];
	}
	
	for ( i = 0; i < DIM_STD_TBLS_USED; i++ ) {
		table_buffer[k++] = st_73_source.std_tbls_monitored_flags[i];
	}
	
	for ( i = 0; i < DIM_MFG_TBLS_USED; i++ ) {
		table_buffer[k++] = st_73_source.mfg_tbls_monitored_flags[i];
	}
	
	for ( i = 0; i < DIM_STD_PROC_USED; i++ ) {
		table_buffer[k++] = st_73_source.std_proc_monitored_flags[i];
	}
	
	for ( i = 0; i < DIM_MFG_PROC_USED; i++ ) {
		table_buffer[k++] = st_73_source.mfg_proc_monitored_flags[i];
	}
}

/**
 ******************************************************************************
 * \fn uint8_t EVNT_APP_ucGet_default_st_73_mfg_events(uint8_t set_nbr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t EVNT_APP_ucGet_default_st_73_std_events(uint8_t set_nbr) {
	
	uint8_t set_val;
	
	switch (set_nbr) {
		case 0:		set_val = ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_0;		break;
		case 1:		set_val = ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_1;		break;
		case 2:		set_val = ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_2;		break;
		case 3:		set_val = ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_3;		break;
		case 4:		set_val = ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_4;		break;
		case 5:		set_val = ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_5;		break;
		case 6:		set_val = ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_6;		break;
		case 7:		set_val = ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_7;		break;
		case 8:		set_val = ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_8;		break;
		case 9:		set_val = ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_9;		break;
		default:	set_val = 0;	break;
	}
	
	return set_val;
}

/**
 ******************************************************************************
 * \fn uint8_t EVNT_APP_ucGet_default_st_73_mfg_events(uint8_t set_nbr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t EVNT_APP_ucGet_default_st_73_mfg_events(uint8_t set_nbr) {
	
	uint8_t set_val;
	
	switch (set_nbr) {
		case 0:		set_val = ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_0;		break;
		case 1:		set_val = ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_1;		break;
		case 2:		set_val = ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_2;		break;
		case 3:		set_val = ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_3;		break;
		case 4:		set_val = ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_4;		break;
		case 5:		set_val = ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_5;		break;
		case 6:		set_val = ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_6;		break;
		case 7:		set_val = ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_7;		break;
		case 8:		set_val = ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_8;		break;
		case 9:		set_val = ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_9;		break;
		default:	set_val = 0;	break;
	}
	
	return set_val;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_73_validate_data(ST_71_TYPE *st_71_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_73_validate_data(ST_73_TYPE *st_73_structure) {
	
	bool is_ok = true;
	uint8_t i;
	uint8_t aux_set[MAX_NBR_OCTETS_SET];
	
	for ( i = 0; i < st_71.nbr_std_events; i++ ) {
		st_73_structure->std_events_monitored_flags[i] &= ST_72_STD_EVENTS_SUPPORTED(i);
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ ) {
		st_73_structure->mfg_events_monitored_flags[i] &= ST_72_MFG_EVENTS_SUPPORTED(i);
	}

	memset( &aux_set, 0, sizeof(aux_set) );
	GRAL_APP_vLoad_std_tbls_write(&aux_set[0]);
	for ( i = 0; i < DIM_STD_TBLS_USED; i++ ) {
		st_73_structure->std_tbls_monitored_flags[i] &= aux_set[i];
	}
	
	memset( &aux_set, 0, sizeof(aux_set) );
	GRAL_APP_vLoad_mfg_tbls_write(&aux_set[0]);
	for ( i = 0; i < DIM_MFG_TBLS_USED; i++ ) {
		st_73_structure->mfg_tbls_monitored_flags[i] &= aux_set[i];
	}
	
	memset( &aux_set, 0, sizeof(aux_set) );
	GRAL_APP_vLoad_std_proc_used(&aux_set[0]);
	for ( i = 0; i < DIM_STD_PROC_USED; i++ ) {
		st_73_structure->std_proc_monitored_flags[i] &= aux_set[i];
	}
	
	memset( &aux_set, 0, sizeof(aux_set) );
	GRAL_APP_vLoad_mfg_proc_used(&aux_set[0]);
	for ( i = 0; i < DIM_MFG_PROC_USED; i++ ) {
		st_73_structure->mfg_proc_monitored_flags[i] &= aux_set[i];
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_73_read_eeprom_values(ST_73_TYPE *st_73_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_73_read_eeprom_values(ST_73_TYPE *st_73_structure) { 	
	
	EEPROM_reader( ST_73_STD_EVENTS_MONITORED_FLAGS_ADDR, &st_73_structure->std_events_monitored_flags, st_71.nbr_std_events );
	EEPROM_reader( ST_73_MFG_EVENTS_MONITORED_FLAGS_ADDR, &st_73_structure->mfg_events_monitored_flags, st_71.nbr_mfg_events );
	EEPROM_reader( ST_73_STD_TBLS_MONITORED_FLAGS_ADDR, &st_73_structure->std_tbls_monitored_flags, DIM_STD_TBLS_USED );
	EEPROM_reader( ST_73_MFG_TBLS_MONITORED_FLAGS_ADDR, &st_73_structure->mfg_tbls_monitored_flags, DIM_MFG_TBLS_USED );
	EEPROM_reader( ST_73_STD_PROC_MONITORED_FLAGS_ADDR, &st_73_structure->std_proc_monitored_flags, DIM_STD_PROC_USED );
	EEPROM_reader( ST_73_MFG_PROC_MONITORED_FLAGS_ADDR, &st_73_structure->mfg_proc_monitored_flags, DIM_MFG_PROC_USED );
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_73_write_eeprom_values(ST_73_TYPE *st_73_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_73_write_eeprom_values(ST_73_TYPE *st_73_structure) {
	
	SM_APP_vChange_table_state(ST_73_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( ST_73_STD_EVENTS_MONITORED_FLAGS_ADDR, &st_73_structure->std_events_monitored_flags, st_71.nbr_std_events );
	EEPROM_writer( ST_73_MFG_EVENTS_MONITORED_FLAGS_ADDR, &st_73_structure->mfg_events_monitored_flags, st_71.nbr_mfg_events );
	EEPROM_writer( ST_73_STD_TBLS_MONITORED_FLAGS_ADDR, &st_73_structure->std_tbls_monitored_flags, DIM_STD_TBLS_USED );
	EEPROM_writer( ST_73_MFG_TBLS_MONITORED_FLAGS_ADDR, &st_73_structure->mfg_tbls_monitored_flags, DIM_MFG_TBLS_USED );
	EEPROM_writer( ST_73_STD_PROC_MONITORED_FLAGS_ADDR, &st_73_structure->std_proc_monitored_flags, DIM_STD_PROC_USED );
	EEPROM_writer( ST_73_MFG_PROC_MONITORED_FLAGS_ADDR, &st_73_structure->mfg_proc_monitored_flags, DIM_MFG_PROC_USED );
	
	SM_APP_vChange_table_state(ST_73_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_73_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_73_execute_table_reading(uint8_t *table_buffer) {
	
	EVNT_APP_vSt_73_to_array(st_73, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_73_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_73_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t i = 0;
	uint16_t k = 0;
	ST_73_TYPE st_73_temp;
	
	for ( i = 0; i < st_71.nbr_std_events; i++ ) {
		st_73_temp.std_events_monitored_flags[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ ) {
		st_73_temp.mfg_events_monitored_flags[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < DIM_STD_TBLS_USED; i++ ) {
		st_73_temp.std_tbls_monitored_flags[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < DIM_MFG_TBLS_USED; i++ ) {
		st_73_temp.mfg_tbls_monitored_flags[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < DIM_STD_PROC_USED; i++ ) {
		st_73_temp.std_proc_monitored_flags[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < DIM_MFG_PROC_USED; i++ ) {
		st_73_temp.mfg_proc_monitored_flags[i] = table_buffer[k++];
	}
	
	is_ok = EVNT_APP_bSt_73_validate_data(&st_73_temp);
	
	if ( is_ok ) {
		st_73 = st_73_temp;
		EVNT_APP_vSt_73_write_eeprom_values(&st_73);
		uint16_t crc = SM_APP_usSt_73_ram_calculate_crc();
		SM_APP_vSt_73_write_crc(crc);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t EVNT_APP_usSt_73_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t EVNT_APP_usSt_73_get_size(void) {

	uint16_t table_size;
	
	table_size = st_71.nbr_std_events + st_71.nbr_mfg_events + DIM_STD_TBLS_USED + DIM_MFG_TBLS_USED + DIM_STD_PROC_USED + DIM_MFG_PROC_USED;
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bLoad_mt_73(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bLoad_mt_73(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(MT_73_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = EVNT_APP_bMt_73_restore_default_values();
	}
	else {
		
		is_ok = EVNT_APP_bMt_73_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bMt_73_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bMt_73_restore_default_values(void) {
	
	bool is_ok = false;
	MT_73_TYPE mt_73_temp;
	uint8_t i;

	memset( &mt_73_temp, 0, sizeof(mt_73_temp) );
	
	for ( i = 0; i < st_71.nbr_std_events; i++ ) {
		mt_73_temp.std_alarms_monitored_flags[i] = MT_73_DEFAULT_STD_ALARMS_MONITORED(i);
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ ) {
		mt_73_temp.mfg_alarms_monitored_flags[i] = MT_73_DEFAULT_MFG_ALARMS_MONITORED(i);
	}
	
	is_ok = EVNT_APP_bMt_73_validate_data(&mt_73_temp);
	
	mt_73 = mt_73_temp;
	EVNT_APP_vMt_73_write_eeprom_values(&mt_73);
	uint16_t crc = SM_APP_usMt_73_ram_calculate_crc();
	SM_APP_vMt_73_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bMt_73_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bMt_73_refresh_ram(void) {
	
	MT_73_TYPE mt_73_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		EVNT_APP_vMt_73_read_eeprom_values(&mt_73_temp);
		is_ok = EVNT_APP_bMt_73_validate_data(&mt_73_temp);
		
		if ( is_ok ) {
			mt_73 = mt_73_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		mt_73 = mt_73_temp;
		EVNT_APP_vMt_73_write_eeprom_values(&mt_73);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  EVNT_APP_vCopy_mt_73_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vCopy_mt_73_ram(uint8_t* destination) {
	
	EVNT_APP_vMt_73_to_array(mt_73, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vCopy_mt_73_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vCopy_mt_73_nvm(uint8_t* destination) {
	
	MT_73_TYPE mt_73_aux;
	
	EVNT_APP_vMt_73_read_eeprom_values(&mt_73_aux);
	EVNT_APP_vMt_73_to_array(mt_73_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vMt_73_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vMt_73_recover_nvm(void) {
	
	EVNT_APP_vMt_73_write_eeprom_values(&mt_73);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vMt_73_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vMt_73_recover_ram(void) {
	
	EVNT_APP_vMt_73_read_eeprom_values(&mt_73);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vMt_73_to_array(MT_73_TYPE mt_73_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vMt_73_to_array(MT_73_TYPE mt_73_source, uint8_t *table_buffer) {
		
	uint16_t i = 0;
	uint16_t k = 0;

	for ( i = 0; i < st_71.nbr_std_events; i++ ) {
		table_buffer[k++] = mt_73_source.std_alarms_monitored_flags[i];
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ ) {
		table_buffer[k++] = mt_73_source.mfg_alarms_monitored_flags[i];
	}	
}

/**
 ******************************************************************************
 * \fn uint8_t EVNT_APP_ucGet_default_mt_73_std_alarms(uint8_t set_nbr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t EVNT_APP_ucGet_default_mt_73_std_alarms(uint8_t set_nbr) {
	
	uint8_t set_val;
	
	switch (set_nbr) {
		case 0:		set_val = MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_0;		break;
		case 1:		set_val = MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_1;		break;
		case 2:		set_val = MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_2;		break;
		case 3:		set_val = MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_3;		break;
		case 4:		set_val = MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_4;		break;
		case 5:		set_val = MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_5;		break;
		case 6:		set_val = MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_6;		break;
		case 7:		set_val = MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_7;		break;
		case 8:		set_val = MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_8;		break;
		case 9:		set_val = MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_9;		break;
		default:	set_val = 0;	break;
	}
	
	return set_val;
}

/**
 ******************************************************************************
 * \fn uint8_t EVNT_APP_ucGet_default_mt_73_mfg_alarms(uint8_t set_nbr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t EVNT_APP_ucGet_default_mt_73_mfg_alarms(uint8_t set_nbr) {
	
	uint8_t set_val;
	
	switch (set_nbr) {
		case 0:		set_val = MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_0;		break;
		case 1:		set_val = MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_1;		break;
		case 2:		set_val = MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_2;		break;
		case 3:		set_val = MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_3;		break;
		case 4:		set_val = MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_4;		break;
		case 5:		set_val = MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_5;		break;
		case 6:		set_val = MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_6;		break;
		case 7:		set_val = MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_7;		break;
		case 8:		set_val = MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_8;		break;
		case 9:		set_val = MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_9;		break;
		default:	set_val = 0;	break;
	}
	
	return set_val;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bMt_73_validate_data(MT_73_TYPE *mt_73_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bMt_73_validate_data(MT_73_TYPE *mt_73_structure) {
	
	bool is_ok = true;
	uint8_t i;
	uint8_t aux_set[MAX_NBR_OCTETS_SET];
	
	for ( i = 0; i < st_71.nbr_std_events; i++ ) {
		mt_73_structure->std_alarms_monitored_flags[i] &= MT_72_STD_ALARMS_SUPPORTED(i);
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ ) {
		mt_73_structure->mfg_alarms_monitored_flags[i] &= MT_72_MFG_ALARMS_SUPPORTED(i);
	}

	return is_ok;
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vMt_73_read_eeprom_values(MT_73_TYPE *mt_73_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vMt_73_read_eeprom_values(MT_73_TYPE *mt_73_structure) { 	
	
	EEPROM_reader( MT_73_STD_ALARMS_MONITORED_FLAGS_ADDR, &mt_73_structure->std_alarms_monitored_flags, st_71.nbr_std_events );
	EEPROM_reader( MT_73_MFG_ALARMS_MONITORED_FLAGS_ADDR, &mt_73_structure->mfg_alarms_monitored_flags, st_71.nbr_mfg_events );
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vMt_73_write_eeprom_values(MT_73_TYPE *mt_73_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vMt_73_write_eeprom_values(MT_73_TYPE *mt_73_structure) {
	
	SM_APP_vChange_table_state(MT_73_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( MT_73_STD_ALARMS_MONITORED_FLAGS_ADDR, &mt_73_structure->std_alarms_monitored_flags, st_71.nbr_std_events );
	EEPROM_writer( MT_73_MFG_ALARMS_MONITORED_FLAGS_ADDR, &mt_73_structure->mfg_alarms_monitored_flags, st_71.nbr_mfg_events );
	
	SM_APP_vChange_table_state(MT_73_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bMt_73_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bMt_73_execute_table_reading(uint8_t *table_buffer) {
	
	EVNT_APP_vMt_73_to_array(mt_73, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bMt_73_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bMt_73_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t i = 0;
	uint16_t k = 0;
	MT_73_TYPE mt_73_temp;
	
	for ( i = 0; i < st_71.nbr_std_events; i++ ) {
		mt_73_temp.std_alarms_monitored_flags[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ ) {
		mt_73_temp.mfg_alarms_monitored_flags[i] = table_buffer[k++];
	}
	
	is_ok = EVNT_APP_bMt_73_validate_data(&mt_73_temp);
	
	if ( is_ok ) {
		mt_73 = mt_73_temp;
		EVNT_APP_vMt_73_write_eeprom_values(&mt_73);
		uint16_t crc = SM_APP_usMt_73_ram_calculate_crc();
		SM_APP_vMt_73_write_crc(crc);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t EVNT_APP_usMt_73_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t EVNT_APP_usMt_73_get_size(void) {

	uint16_t table_size;
	
	table_size = st_71.nbr_std_events + st_71.nbr_mfg_events;
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bLoad_st_74(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bLoad_st_74(void) {	
	
	bool is_ok;
	
	if ( SM_APP_ucGet_table_state(ST_74_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = EVNT_APP_bSt_74_restore_default_values();
	}
	else {
		is_ok = EVNT_APP_bSt_74_refresh_ram();
	}
	
	if ( (SM_APP_ucGet_table_state(ST_74_PROG_STATE_ADDR) == C1219_TABLE_FULL) && (SM_APP_ucGet_table_state(ST_76_PROG_STATE_ADDR) == C1219_TABLE_FULL) ) {
		EVNT_APP_vLoad_event_number();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_74_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_74_restore_default_values(void) {
	
	bool is_ok = false;
	static ST_74_TYPE st_74_temp;
	
	st_74_temp.event_flags = ST_74_DEFAULT_LIST_STATUS;
	st_74_temp.nbr_valid_entries = ST_74_DEFAULT_NBR_VALID_ENTRIES;
	st_74_temp.last_entry_element = ST_74_DEFAULT_LAST_ENTRY_ELEMENT;
	st_74_temp.last_entry_seq_nbr = ST_74_DEFAULT_LAST_ENTRY_SEQ_NBR;
	st_74_temp.nbr_unread_entries = ST_74_DEFAULT_NBR_UNREAD_ENTRIES;

	is_ok = EVNT_APP_bSt_74_validate_data(&st_74_temp);
	event_number = 0;
	
	st_74 = st_74_temp;
	EVNT_APP_vSt_74_write_eeprom_values(&st_74);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_74_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_74_refresh_ram(void) {
	
	static ST_74_TYPE st_74_temp;
	static bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		EVNT_APP_vSt_74_read_eeprom_values(&st_74_temp);
		is_ok = EVNT_APP_bSt_74_validate_data(&st_74_temp);
		
		if ( is_ok ) {
			st_74 = st_74_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_74 = st_74_temp;
		EVNT_APP_vSt_74_write_eeprom_values(&st_74);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_74_validate_data(ST_74_TYPE *st_74_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_74_validate_data(ST_74_TYPE *st_74_structure) {
	
	bool is_ok = true;
	
	st_74_structure->event_flags &= ~(uint8_t)(ORDER_FLAG_bm);
	st_74_structure->event_flags |= (uint8_t)(LIST_TYPE_FLAG_bm);
	st_74_structure->event_flags &= ~(uint8_t)(INHIBIT_OVERFLOW_FLAG_bm);
	
	if (st_74_structure->nbr_valid_entries >= st_71.nbr_history_entries) {
		
		st_74_structure->nbr_valid_entries = (st_71.nbr_history_entries - 1);
		is_ok = false;
		
		if (st_71.nbr_history_entries == 0) {
			st_74_structure->nbr_valid_entries = 0;
			is_ok = true;
		}
	}
	
	if (st_74_structure->last_entry_element >= st_74_structure->nbr_valid_entries) {
		
		st_74_structure->last_entry_element = (st_74_structure->nbr_valid_entries - 1);
		is_ok = false;
		
		if (st_74_structure->nbr_valid_entries == 0) {
			st_74_structure->last_entry_element = 0;
			is_ok = true;
		}
	}
	
	if (st_74_structure->nbr_unread_entries >= MAX_NBR_HISTORY_ENTRIES) {
		st_74_structure->nbr_unread_entries = MAX_NBR_HISTORY_ENTRIES;
		is_ok = false;
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_74_read_eeprom_values(ST_74_TYPE *st_74_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_74_read_eeprom_values(ST_74_TYPE *st_74_structure) { 	
	
	EEPROM_reader( ST_74_HIST_FLAGS_ADDR, &st_74_structure->event_flags, 1 );
	EEPROM_reader( ST_74_NBR_VALID_ENTRIES_ADDR, &st_74_structure->nbr_valid_entries, 2 );
	EEPROM_reader( ST_74_LAST_ENTRY_ELEMENT_ADDR, &st_74_structure->last_entry_element, 2 );
	EEPROM_reader( ST_74_LAST_ENTRY_SEQ_NBR_ADDR, &st_74_structure->last_entry_seq_nbr, 4 );
	EEPROM_reader( ST_74_NBR_UNREAD_ENTRIES_ADDR, &st_74_structure->nbr_unread_entries, 2 );
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_74_write_eeprom_values(ST_74_TYPE *st_74_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_74_write_eeprom_values(ST_74_TYPE *st_74_structure) {
	
	SM_APP_vChange_table_state(ST_74_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( ST_74_HIST_FLAGS_ADDR, &st_74_structure->event_flags, 1 );
	EEPROM_writer( ST_74_NBR_VALID_ENTRIES_ADDR, &st_74_structure->nbr_valid_entries, 2 );
	EEPROM_writer( ST_74_LAST_ENTRY_ELEMENT_ADDR, &st_74_structure->last_entry_element, 2 );
	EEPROM_writer( ST_74_LAST_ENTRY_SEQ_NBR_ADDR, &st_74_structure->last_entry_seq_nbr, 4 );
	EEPROM_writer( ST_74_NBR_UNREAD_ENTRIES_ADDR, &st_74_structure->nbr_unread_entries, 2 );
	
	SM_APP_vChange_table_state(ST_74_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vClear_st_74(uint16_t user_attempt)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vClear_st_74(uint16_t user_attempt) {
	
	uint16_t i;
	uint32_t clear_data = 0xFFFFFFFF;
	
	EVNT_APP_bSt_74_restore_default_values();
	
	for ( i = 0; i < st_71.nbr_history_entries; i++ ) {
		EEPROM_writer( (uint32_t)(ST_74_HISTORY_TIME_ADDR + i*4), &clear_data, 4 );
		EEPROM_writer( (uint32_t)(ST_74_EVENT_NUMBER_ADDR + i*2), &clear_data, 2 );
		EEPROM_writer( (uint32_t)(ST_74_HISTORY_SEQ_NBR_ADDR + i*2), &clear_data, 2 );		
		EEPROM_writer( (uint32_t)(ST_74_USER_ID_ADDR + i*2), &clear_data, 2 );
		EEPROM_writer( (uint32_t)(ST_74_HISTORY_CODE_ADDR + i*2), &clear_data, 2 );
		EEPROM_writer( (uint32_t)(ST_74_HISTORY_ARGUMENT_ADDR + i*2), &clear_data, 2 );
	}
	
	EVNT_APP_vRegister_event(HISTORY_LOG_CLEARED, 0, true, user_attempt, rtc.utc_timestamp);
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_74_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_74_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t i = 0;
	uint16_t k = 0;
	
	delay_ms(10);
	
	EEPROM_reader( (uint32_t)(ST_74_HIST_FLAGS_ADDR), &table_buffer[k], 1 );
	table_buffer[k] &= ~ORDER_FLAG_bm;
	table_buffer[k] |=	LIST_TYPE_FLAG_bm;
	table_buffer[k] &= ~INHIBIT_OVERFLOW_FLAG_bm;
	k += 1;
	
	EEPROM_reader( (uint32_t)(ST_74_NBR_VALID_ENTRIES_ADDR), &table_buffer[k], 2 );
	k += 2;
	
	EEPROM_reader( (uint32_t)(ST_74_LAST_ENTRY_ELEMENT_ADDR), &table_buffer[k], 2 );
	k += 2;
	
	EEPROM_reader( (uint32_t)(ST_74_LAST_ENTRY_SEQ_NBR_ADDR), &table_buffer[k], 4 );
	k += 4;
	
	EEPROM_reader( (uint32_t)(ST_74_NBR_UNREAD_ENTRIES_ADDR), &table_buffer[k], 2 );
	k += 2;
	
	for ( i = 0; i < st_71.nbr_history_entries; i++ ) {
		if ( (st_71.log_flags & HIST_DATE_TIME_FLAG_bm) == HIST_DATE_TIME_FLAG_bm ) {
			EEPROM_reader( (uint32_t)(ST_74_HISTORY_TIME_ADDR + i*4), &table_buffer[k], 4 );
			k += 4;
		}
		
		if ( (st_71.log_flags & EVENT_NUMBER_FLAG_bm) == EVENT_NUMBER_FLAG_bm ) {
			EEPROM_reader( (uint32_t)(ST_74_EVENT_NUMBER_ADDR + i*2), &table_buffer[k], 2 );
			k += 2;	
		}
		
		if ( (st_71.log_flags & HIST_SEQ_NBR_FLAG_bm) == HIST_SEQ_NBR_FLAG_bm ) {
			EEPROM_reader( (uint32_t)(ST_74_HISTORY_SEQ_NBR_ADDR + i*2), &table_buffer[k], 2 );
			k += 2;
		}
		
		EEPROM_reader( (uint32_t)(ST_74_USER_ID_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
		
		EEPROM_reader( (uint32_t)(ST_74_HISTORY_CODE_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
		
		EEPROM_reader( (uint32_t)(ST_74_HISTORY_ARGUMENT_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_74_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_74_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t i = 0;
	uint16_t k = 0;
	
	EEPROM_writer( (uint32_t)(ST_74_HIST_FLAGS_ADDR), &table_buffer[k], 1 );
	k += 1;
	
	EEPROM_writer( (uint32_t)(ST_74_NBR_VALID_ENTRIES_ADDR), &table_buffer[k], 2 );
	k += 2;
	
	EEPROM_writer( (uint32_t)(ST_74_LAST_ENTRY_ELEMENT_ADDR), &table_buffer[k], 2 );
	k += 2;
	
	EEPROM_writer( (uint32_t)(ST_74_LAST_ENTRY_SEQ_NBR_ADDR), &table_buffer[k], 4 );
	k += 4;
	
	EEPROM_writer( (uint32_t)(ST_74_NBR_UNREAD_ENTRIES_ADDR), &table_buffer[k], 2 );
	k += 2;
	
	for ( i = 0; i < st_71.nbr_history_entries; i++ ) {
		
		if ( (st_71.log_flags & HIST_DATE_TIME_FLAG_bm) == HIST_DATE_TIME_FLAG_bm ) {
			EEPROM_writer( (uint32_t)(ST_74_HISTORY_TIME_ADDR + i*4), &table_buffer[k], 4 );
			k += 4;
		}
		
		if ( (st_71.log_flags & EVENT_NUMBER_FLAG_bm) == EVENT_NUMBER_FLAG_bm ) {
			EEPROM_writer( (uint32_t)(ST_74_EVENT_NUMBER_ADDR + i*2), &table_buffer[k], 2 );
			k += 2;
		}
		
		if ( (st_71.log_flags & HIST_SEQ_NBR_FLAG_bm) == HIST_SEQ_NBR_FLAG_bm ){
			EEPROM_writer( (uint32_t)(ST_74_HISTORY_SEQ_NBR_ADDR + i*2), &table_buffer[k], 2 );
			k += 2;
		}
		
		EEPROM_writer( (uint32_t)(ST_74_USER_ID_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
		
		EEPROM_writer( (uint32_t)(ST_74_HISTORY_CODE_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
		
		EEPROM_writer( (uint32_t)(ST_74_HISTORY_ARGUMENT_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
	}
	
	is_ok = true;
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t EVNT_APP_usSt_74_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t EVNT_APP_usSt_74_get_size(void) {

	uint16_t i;
	uint16_t table_size = 11;
	
	for ( i = 0; i < st_71.nbr_history_entries; i++ ) {
		
		if ( (st_71.log_flags & HIST_DATE_TIME_FLAG_bm) == HIST_DATE_TIME_FLAG_bm ) {
			table_size += 4;
		}
		
		if ( (st_71.log_flags & EVENT_NUMBER_FLAG_bm) == EVENT_NUMBER_FLAG_bm ) {
			table_size += 2;
		}
		
		if ( (st_71.log_flags & HIST_SEQ_NBR_FLAG_bm) == HIST_SEQ_NBR_FLAG_bm ) {
			table_size += 2;
		}

		table_size += 6;
	}
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bLoad_st_75(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bLoad_st_75(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_75_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = EVNT_APP_bSt_75_restore_default_values();
	}
	else {
		
		is_ok = EVNT_APP_bSt_75_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_75_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_75_restore_default_values(void) {
	
	bool is_ok = false;
	ST_73_TYPE st_75_temp;
	uint8_t i;

	memset( &st_75_temp, 0, sizeof(st_75_temp) );
	
	for ( i = 0; i < st_71.nbr_std_events; i++ ) {
		st_75_temp.std_events_monitored_flags[i] = ST_75_DEFAULT_STD_EVENTS_MONITORED(i);
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ ) {
		st_75_temp.mfg_events_monitored_flags[i] = ST_75_DEFAULT_MFG_EVENTS_MONITORED(i);
	}
	
	GRAL_APP_vLoad_std_tbls_write(&st_75_temp.std_tbls_monitored_flags[0]);
	GRAL_APP_vLoad_mfg_tbls_write(&st_75_temp.mfg_tbls_monitored_flags[0]);
	GRAL_APP_vLoad_std_proc_used(&st_75_temp.std_proc_monitored_flags[0]);
	GRAL_APP_vLoad_mfg_proc_used(&st_75_temp.mfg_proc_monitored_flags[0]);
	
	is_ok = EVNT_APP_bSt_75_validate_data(&st_75_temp);
	
	st_75 = st_75_temp;
	EVNT_APP_vSt_75_write_eeprom_values(&st_75);
	uint16_t crc = SM_APP_usSt_75_ram_calculate_crc();
	SM_APP_vSt_75_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_75_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_75_refresh_ram(void) {
	
	ST_73_TYPE st_75_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		EVNT_APP_vSt_75_read_eeprom_values(&st_75_temp);
		is_ok = EVNT_APP_bSt_75_validate_data(&st_75_temp);
		
		if ( is_ok ) {
			st_75 = st_75_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_75 = st_75_temp;
		EVNT_APP_vSt_75_write_eeprom_values(&st_75);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  EVNT_APP_vCopy_st_75_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vCopy_st_75_ram(uint8_t* destination) {
	
	EVNT_APP_vSt_75_to_array(st_75, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vCopy_st_75_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vCopy_st_75_nvm(uint8_t* destination) {
	
	ST_73_TYPE st_75_aux;
	
	EVNT_APP_vSt_75_read_eeprom_values(&st_75_aux);
	EVNT_APP_vSt_75_to_array(st_75_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_75_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_75_recover_nvm(void) {
	
	EVNT_APP_vSt_75_write_eeprom_values(&st_75);
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_75_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_75_recover_ram(void) {
	
	EVNT_APP_vSt_75_read_eeprom_values(&st_75);
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
void EVNT_APP_vSt_75_to_array(ST_73_TYPE st_75_source, uint8_t *table_buffer) {
	
	uint16_t i = 0;
	uint16_t k = 0;

	for ( i = 0; i < st_71.nbr_std_events; i++ ) {
		table_buffer[k++] = st_75_source.std_events_monitored_flags[i];
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ ) {
		table_buffer[k++] = st_75_source.mfg_events_monitored_flags[i];
	}
	
	for ( i = 0; i < DIM_STD_TBLS_USED; i++ ) {
		table_buffer[k++] = st_75_source.std_tbls_monitored_flags[i];
	}
	
	for ( i = 0; i < DIM_MFG_TBLS_USED; i++ ) {
		table_buffer[k++] = st_75_source.mfg_tbls_monitored_flags[i];
	}
	
	for ( i = 0; i < DIM_STD_PROC_USED; i++ ) {
		table_buffer[k++] = st_75_source.std_proc_monitored_flags[i];
	}
	
	for ( i = 0; i < DIM_MFG_PROC_USED; i++ ) {
		table_buffer[k++] = st_75_source.mfg_proc_monitored_flags[i];
	}
}

/**
 ******************************************************************************
 * \fn uint8_t EVNT_APP_ucGet_default_st_75_mfg_events(uint8_t set_nbr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t EVNT_APP_ucGet_default_st_75_std_events(uint8_t set_nbr) {
	
	uint8_t set_val;
	
	switch (set_nbr) {
		case 0:		set_val = ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_0;		break;
		case 1:		set_val = ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_1;		break;
		case 2:		set_val = ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_2;		break;
		case 3:		set_val = ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_3;		break;
		case 4:		set_val = ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_4;		break;
		case 5:		set_val = ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_5;		break;
		case 6:		set_val = ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_6;		break;
		case 7:		set_val = ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_7;		break;
		case 8:		set_val = ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_8;		break;
		case 9:		set_val = ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_9;		break;
		default:	set_val = 0;	break;
	}
	
	return set_val;
}

/**
 ******************************************************************************
 * \fn uint8_t EVNT_APP_ucGet_default_st_75_mfg_events(uint8_t set_nbr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t EVNT_APP_ucGet_default_st_75_mfg_events(uint8_t set_nbr) {
	
	uint8_t set_val;
	
	switch (set_nbr) {
		case 0:		set_val = ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_0;		break;
		case 1:		set_val = ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_1;		break;
		case 2:		set_val = ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_2;		break;
		case 3:		set_val = ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_3;		break;
		case 4:		set_val = ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_4;		break;
		case 5:		set_val = ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_5;		break;
		case 6:		set_val = ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_6;		break;
		case 7:		set_val = ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_7;		break;
		case 8:		set_val = ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_8;		break;
		case 9:		set_val = ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_9;		break;
		default:	set_val = 0;	break;
	}
	
	return set_val;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_75_validate_data(ST_73_TYPE *st_75_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_75_validate_data(ST_73_TYPE *st_75_structure) {
	
	bool is_ok = true;
	uint8_t i;
	uint8_t aux_set[MAX_NBR_OCTETS_SET];
	
	for ( i = 0; i < st_71.nbr_std_events; i++ ) {
		st_75_structure->std_events_monitored_flags[i] &= ST_72_STD_EVENTS_SUPPORTED(i);
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ ) {
		st_75_structure->mfg_events_monitored_flags[i] &= ST_72_MFG_EVENTS_SUPPORTED(i);
	}

	memset( &aux_set, 0, sizeof(aux_set) );
	GRAL_APP_vLoad_std_tbls_write(&aux_set[0]);
	for ( i = 0; i < DIM_STD_TBLS_USED; i++ ) {
		st_75_structure->std_tbls_monitored_flags[i] &= aux_set[i];
	}
	
	memset( &aux_set, 0, sizeof(aux_set) );
	GRAL_APP_vLoad_mfg_tbls_write(&aux_set[0]);
	for ( i = 0; i < DIM_MFG_TBLS_USED; i++ ) {
		st_75_structure->mfg_tbls_monitored_flags[i] &= aux_set[i];
	}
	
	memset( &aux_set, 0, sizeof(aux_set) );
	GRAL_APP_vLoad_std_proc_used(&aux_set[0]);
	for ( i = 0; i < DIM_STD_PROC_USED; i++ ) {
		st_75_structure->std_proc_monitored_flags[i] &= aux_set[i];
	}
	
	memset( &aux_set, 0, sizeof(aux_set) );
	GRAL_APP_vLoad_mfg_proc_used(&aux_set[0]);
	for ( i = 0; i < DIM_MFG_PROC_USED; i++ ) {
		st_75_structure->mfg_proc_monitored_flags[i] &= aux_set[i];
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_75_read_eeprom_values(ST_73_TYPE *st_75_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_75_read_eeprom_values(ST_73_TYPE *st_75_structure) { 	
	
	EEPROM_reader( ST_75_STD_EVENTS_MONITORED_FLAGS_ADDR, &st_75_structure->std_events_monitored_flags, st_71.nbr_std_events );
	EEPROM_reader( ST_75_MFG_EVENTS_MONITORED_FLAGS_ADDR, &st_75_structure->mfg_events_monitored_flags, st_71.nbr_mfg_events );
	EEPROM_reader( ST_75_STD_TBLS_MONITORED_FLAGS_ADDR, &st_75_structure->std_tbls_monitored_flags, DIM_STD_TBLS_USED );
	EEPROM_reader( ST_75_MFG_TBLS_MONITORED_FLAGS_ADDR, &st_75_structure->mfg_tbls_monitored_flags, DIM_MFG_TBLS_USED );
	EEPROM_reader( ST_75_STD_PROC_MONITORED_FLAGS_ADDR, &st_75_structure->std_proc_monitored_flags, DIM_STD_PROC_USED );
	EEPROM_reader( ST_75_MFG_PROC_MONITORED_FLAGS_ADDR, &st_75_structure->mfg_proc_monitored_flags, DIM_MFG_PROC_USED );
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_75_write_eeprom_values(ST_73_TYPE *st_75_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_75_write_eeprom_values(ST_73_TYPE *st_75_structure) {
	
	SM_APP_vChange_table_state(ST_75_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( ST_75_STD_EVENTS_MONITORED_FLAGS_ADDR, &st_75_structure->std_events_monitored_flags, st_71.nbr_std_events );
	EEPROM_writer( ST_75_MFG_EVENTS_MONITORED_FLAGS_ADDR, &st_75_structure->mfg_events_monitored_flags, st_71.nbr_mfg_events );
	EEPROM_writer( ST_75_STD_TBLS_MONITORED_FLAGS_ADDR, &st_75_structure->std_tbls_monitored_flags, DIM_STD_TBLS_USED );
	EEPROM_writer( ST_75_MFG_TBLS_MONITORED_FLAGS_ADDR, &st_75_structure->mfg_tbls_monitored_flags, DIM_MFG_TBLS_USED );
	EEPROM_writer( ST_75_STD_PROC_MONITORED_FLAGS_ADDR, &st_75_structure->std_proc_monitored_flags, DIM_STD_PROC_USED );
	EEPROM_writer( ST_75_MFG_PROC_MONITORED_FLAGS_ADDR, &st_75_structure->mfg_proc_monitored_flags, DIM_MFG_PROC_USED );
	
	SM_APP_vChange_table_state(ST_75_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_75_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_75_execute_table_reading(uint8_t *table_buffer) {
	
	EVNT_APP_vSt_75_to_array(st_75, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_75_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_75_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t i = 0;
	uint16_t k = 0;
	ST_73_TYPE st_75_temp;
	
	for ( i = 0; i < st_71.nbr_std_events; i++ ) {
		st_75_temp.std_events_monitored_flags[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < st_71.nbr_mfg_events; i++ ) {
		st_75_temp.mfg_events_monitored_flags[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < DIM_STD_TBLS_USED; i++ ) {
		st_75_temp.std_tbls_monitored_flags[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < DIM_MFG_TBLS_USED; i++ ) {
		st_75_temp.mfg_tbls_monitored_flags[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < DIM_STD_PROC_USED; i++ ) {
		st_75_temp.std_proc_monitored_flags[i] = table_buffer[k++];
	}
	
	for ( i = 0; i < DIM_MFG_PROC_USED; i++ ) {
		st_75_temp.mfg_proc_monitored_flags[i] = table_buffer[k++];
	}
	
	is_ok = EVNT_APP_bSt_75_validate_data(&st_75_temp);
	
	if ( is_ok ) {
		st_75 = st_75_temp;
		EVNT_APP_vSt_75_write_eeprom_values(&st_75);
		uint16_t crc = SM_APP_usSt_75_ram_calculate_crc();
		SM_APP_vSt_75_write_crc(crc);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t EVNT_APP_usSt_75_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t EVNT_APP_usSt_75_get_size(void) {

	uint16_t table_size;
	
	table_size = st_71.nbr_std_events + st_71.nbr_mfg_events + DIM_STD_TBLS_USED + DIM_MFG_TBLS_USED + DIM_STD_PROC_USED + DIM_MFG_PROC_USED;
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bLoad_st_76(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bLoad_st_76(void) {	
	
	bool is_ok;
	
	if ( SM_APP_ucGet_table_state(ST_76_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = EVNT_APP_bSt_76_restore_default_values();
	}
	else {
		is_ok = EVNT_APP_bSt_76_refresh_ram();
	}
	
	if ( (SM_APP_ucGet_table_state(ST_74_PROG_STATE_ADDR) == C1219_TABLE_FULL) && (SM_APP_ucGet_table_state(ST_76_PROG_STATE_ADDR) == C1219_TABLE_FULL) ) {
		EVNT_APP_vLoad_event_number();
	}
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_76_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_76_restore_default_values(void) {
	
	bool is_ok = false;
	static ST_74_TYPE st_76_temp;
	
	st_76_temp.event_flags = ST_76_DEFAULT_LIST_STATUS;
	st_76_temp.nbr_valid_entries = ST_76_DEFAULT_NBR_VALID_ENTRIES;
	st_76_temp.last_entry_element = ST_76_DEFAULT_LAST_ENTRY_ELEMENT;
	st_76_temp.last_entry_seq_nbr = ST_76_DEFAULT_LAST_ENTRY_SEQ_NBR;
	st_76_temp.nbr_unread_entries = ST_76_DEFAULT_NBR_UNREAD_ENTRIES;

	is_ok = EVNT_APP_bSt_76_validate_data(&st_76_temp);
	event_number = 0;
	
	st_76 = st_76_temp;
	EVNT_APP_vSt_76_write_eeprom_values(&st_76);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_76_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_76_refresh_ram(void) {
	
	static ST_74_TYPE st_76_temp;
	static bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		EVNT_APP_vSt_76_read_eeprom_values(&st_76_temp);
		is_ok = EVNT_APP_bSt_76_validate_data(&st_76_temp);
		
		if ( is_ok ) {
			st_76 = st_76_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_76 = st_76_temp;
		EVNT_APP_vSt_76_write_eeprom_values(&st_76);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_76_validate_data(ST_74_TYPE *st_76_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_76_validate_data(ST_74_TYPE *st_76_structure) {
	
	bool is_ok = true;
	
	st_76_structure->event_flags &= ~(uint8_t)(ORDER_FLAG_bm);
	st_76_structure->event_flags |= (uint8_t)(LIST_TYPE_FLAG_bm);
	st_76_structure->event_flags &= ~(uint8_t)(INHIBIT_OVERFLOW_FLAG_bm);
	
	if (st_76_structure->nbr_valid_entries >= st_71.nbr_event_entries) {
		
		st_76_structure->nbr_valid_entries = (st_71.nbr_event_entries - 1);
		is_ok = false;
		
		if (st_71.nbr_event_entries == 0) {
			st_76_structure->nbr_valid_entries = 0;
			is_ok = true;
		}
	}
	
	if (st_76_structure->last_entry_element >= st_76_structure->nbr_valid_entries) {
		
		st_76_structure->last_entry_element = (st_76_structure->nbr_valid_entries - 1);
		is_ok = false;
		
		if (st_76_structure->nbr_valid_entries == 0) {
			st_76_structure->last_entry_element = 0;
			is_ok = true;
		}
	}
	
	if (st_76_structure->nbr_unread_entries >= MAX_NBR_EVENT_ENTRIES) {
		st_76_structure->nbr_unread_entries = MAX_NBR_EVENT_ENTRIES;
		is_ok = false;
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_76_read_eeprom_values(ST_74_TYPE *st_76_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_76_read_eeprom_values(ST_74_TYPE *st_76_structure) { 	
	
	EEPROM_reader( ST_76_EVENT_FLAGS_ADDR, &st_76_structure->event_flags, 1 );
	EEPROM_reader( ST_76_NBR_VALID_ENTRIES_ADDR, &st_76_structure->nbr_valid_entries, 2 );
	EEPROM_reader( ST_76_LAST_ENTRY_ELEMENT_ADDR, &st_76_structure->last_entry_element, 2 );
	EEPROM_reader( ST_76_LAST_ENTRY_SEQ_NBR_ADDR, &st_76_structure->last_entry_seq_nbr, 4 );
	EEPROM_reader( ST_76_NBR_UNREAD_ENTRIES_ADDR, &st_76_structure->nbr_unread_entries, 2 );
}

/**
 ******************************************************************************
 * \fn void EVNT_APP_vSt_76_write_eeprom_values(ST_74_TYPE *st_76_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EVNT_APP_vSt_76_write_eeprom_values(ST_74_TYPE *st_76_structure) {
	
	SM_APP_vChange_table_state(ST_76_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( ST_76_EVENT_FLAGS_ADDR, &st_76_structure->event_flags, 1 );
	EEPROM_writer( ST_76_NBR_VALID_ENTRIES_ADDR, &st_76_structure->nbr_valid_entries, 2 );
	EEPROM_writer( ST_76_LAST_ENTRY_ELEMENT_ADDR, &st_76_structure->last_entry_element, 2 );
	EEPROM_writer( ST_76_LAST_ENTRY_SEQ_NBR_ADDR, &st_76_structure->last_entry_seq_nbr, 4 );
	EEPROM_writer( ST_76_NBR_UNREAD_ENTRIES_ADDR, &st_76_structure->nbr_unread_entries, 2 );
	
	SM_APP_vChange_table_state(ST_76_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
******************************************************************************
* \fn void EVNT_APP_vClear_st_74(uint16_t user_attempt)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void EVNT_APP_vClear_st_76(uint16_t user_attempt) {
	
	uint16_t i = 0;
	uint32_t clear_data = 0xFFFFFFFF;
	
	EVNT_APP_bSt_76_restore_default_values();
	
	for ( i = 0; i < st_71.nbr_event_entries; i++ ) {
		EEPROM_writer( (uint32_t)(ST_76_EVENT_TIME_ADDR + i*4), &clear_data, 4 );
		EEPROM_writer( (uint32_t)(ST_76_EVENT_NUMBER_ADDR + i*2), &clear_data, 2 );
		EEPROM_writer( (uint32_t)(ST_76_EVENT_SEQ_NBR_ADDR + i*2), &clear_data, 2 );
		EEPROM_writer( (uint32_t)(ST_76_USER_ID_ADDR + i*2), &clear_data, 2 );
		EEPROM_writer( (uint32_t)(ST_76_EVENT_CODE_ADDR + i*2), &clear_data, 2 );
		EEPROM_writer( (uint32_t)(ST_76_EVENT_ARGUMENT_ADDR + i*2), &clear_data, 2 );
	}
	
	EVNT_APP_vRegister_event(EVENT_LOG_CLEARED, 0, true, user_attempt, rtc.utc_timestamp);
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_76_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_76_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t i = 0;
	uint16_t k = 0;
	uint16_t eeprom_addr;
	
	delay_ms(10);
	
	EEPROM_reader( (uint32_t)(ST_76_EVENT_FLAGS_ADDR), &table_buffer[k], 1 );
	table_buffer[k] &= ~ORDER_FLAG_bm;
	table_buffer[k] |=	LIST_TYPE_FLAG_bm;
	table_buffer[k] &= ~INHIBIT_OVERFLOW_FLAG_bm;
	k += 1;
	
	EEPROM_reader( (uint32_t)(ST_76_NBR_VALID_ENTRIES_ADDR), &table_buffer[k], 2 );
	k += 2;
	
	EEPROM_reader( (uint32_t)(ST_76_LAST_ENTRY_ELEMENT_ADDR), &table_buffer[k], 2 );
	k += 2;
	
	EEPROM_reader( (uint32_t)(ST_76_LAST_ENTRY_SEQ_NBR_ADDR), &table_buffer[k], 4 );
	k += 4;
	
	EEPROM_reader( (uint32_t)(ST_76_NBR_UNREAD_ENTRIES_ADDR), &table_buffer[k], 2 );
	k += 2;
	
	for ( i = 0; i < st_71.nbr_event_entries; i++ )
	{
		EEPROM_reader( (uint32_t)(ST_76_EVENT_TIME_ADDR + i*4), &table_buffer[k], 4 );
		k += 4;
		
		if ( (st_71.log_flags & EVENT_NUMBER_FLAG_bm) == EVENT_NUMBER_FLAG_bm )
		{
			EEPROM_reader( (uint32_t)(ST_76_EVENT_NUMBER_ADDR + i*2), &table_buffer[k], 2 );
			k += 2;	
		}
		
		EEPROM_reader( (uint32_t)(ST_76_EVENT_SEQ_NBR_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
		
		EEPROM_reader( (uint32_t)(ST_76_USER_ID_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
		
		EEPROM_reader( (uint32_t)(ST_76_EVENT_CODE_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
		
		EEPROM_reader( (uint32_t)(ST_76_EVENT_ARGUMENT_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool EVNT_APP_bSt_76_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EVNT_APP_bSt_76_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t i = 0;
	uint16_t k = 0;
	
	EEPROM_writer( (uint32_t)(ST_76_EVENT_FLAGS_ADDR), &table_buffer[k], 1 );
	k += 1;
	
	EEPROM_writer( (uint32_t)(ST_76_NBR_VALID_ENTRIES_ADDR), &table_buffer[k], 2 );
	k += 2;
	
	EEPROM_writer( (uint32_t)(ST_76_LAST_ENTRY_ELEMENT_ADDR), &table_buffer[k], 2 );
	k += 2;
	
	EEPROM_writer( (uint32_t)(ST_76_LAST_ENTRY_SEQ_NBR_ADDR), &table_buffer[k], 4 );
	k += 4;
	
	EEPROM_writer( (uint32_t)(ST_76_NBR_UNREAD_ENTRIES_ADDR), &table_buffer[k], 2 );
	k += 2;
	
	for ( i = 0; i < st_71.nbr_event_entries; i++ )
	{
		EEPROM_writer( (uint32_t)(ST_76_EVENT_TIME_ADDR + i*4), &table_buffer[k], 4 );
		k += 4;
		
		if ( (st_71.log_flags & EVENT_NUMBER_FLAG_bm) == EVENT_NUMBER_FLAG_bm )
		{
			EEPROM_writer( (uint32_t)(ST_76_EVENT_NUMBER_ADDR + i*2), &table_buffer[k], 2 );
			k += 2;
		}
		
		EEPROM_writer( (uint32_t)(ST_76_EVENT_SEQ_NBR_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
		
		EEPROM_writer( (uint32_t)(ST_76_USER_ID_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
		
		EEPROM_writer( (uint32_t)(ST_76_EVENT_CODE_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
		
		EEPROM_writer( (uint32_t)(ST_76_EVENT_ARGUMENT_ADDR + i*2), &table_buffer[k], 2 );
		k += 2;
	}
	
	is_ok = true;
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t EVNT_APP_usSt_76_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t EVNT_APP_usSt_76_get_size(void) {

	uint16_t i;
	uint16_t table_size = 11;
	
	for ( i = 0; i < st_71.nbr_event_entries; i++ )
	{
		table_size += 12;
		
		if ( (st_71.log_flags & EVENT_NUMBER_FLAG_bm) == EVENT_NUMBER_FLAG_bm )
		{
			table_size += 2;
		}
	}
	
	return table_size;
}

/**
******************************************************************************
* \fn void EVNT_APP_vLoad_event_number(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void EVNT_APP_vLoad_event_number(void) {
	
	uint16_t last_entry_element;
	uint16_t event_nbr_st_74 = 0;
	uint16_t event_nbr_st_76 = 0;
	
	EEPROM_reader( ST_74_LAST_ENTRY_ELEMENT_ADDR, &last_entry_element, 2 );
	if ( (last_entry_element > 0) && (last_entry_element <= st_71.nbr_event_entries) ) {
		EEPROM_reader( ST_74_EVENT_NUMBER_ADDR + 2*(last_entry_element - 1), &event_nbr_st_74, 2 );
	}
	
	EEPROM_reader( ST_76_LAST_ENTRY_ELEMENT_ADDR, &last_entry_element, 2 );
	if ( (last_entry_element > 0) && (last_entry_element <= st_71.nbr_history_entries) ) {
		EEPROM_reader( ST_76_EVENT_NUMBER_ADDR + 2*(last_entry_element - 1), &event_nbr_st_76, 2 );
	}

	if (event_nbr_st_74 > event_nbr_st_76) {
		event_number = event_nbr_st_74;
	}
	else if (event_nbr_st_74 < event_nbr_st_76) {
		event_number = event_nbr_st_76;
	}
	else if (event_nbr_st_74 == event_nbr_st_76) {
		event_number = event_nbr_st_74;
	}
}

/**
******************************************************************************
* \fn void EVNT_APP_vRegister_event(uint8_t code, uint16_t argument, bool is_std, uint16_t user_id, uint32_t timestamp)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void EVNT_APP_vRegister_event(uint8_t code, uint16_t argument, bool is_std, uint16_t user_id, uint32_t timestamp) {
		
	bool registered = false;

	if ( !meter.system_state.changing_configuration ) {
	
		EVENT_ALARM_TYPE xEvent = {
			.code		= code,
			.argument	= argument,
			.is_std		= is_std,
			.user_id	= user_id,
			.timestamp	= timestamp
		};
	
		if ( is_std ) {
		
			if ( EVNT_APP_bIs_std_event_monitored_by_st_73(code) ) {	
				
				switch (code) {
				
					case TABLE_WRITTEN_TO:
						if ( EVNT_APP_bIs_table_monitored_by_st_73(argument) ) {
							event_number += registered ? 0 : 1;
							xEvent.event_number = event_number;
							EVNT_APP_vRegister_event_in_st_74(xEvent);
							registered = true;
						}
						break;
					
					case PROCEDURE_INVOKED:
						if ( EVNT_APP_bIs_procedure_monitored_by_st_73(argument) ) {
							event_number += registered ? 0 : 1;
							xEvent.event_number = event_number;
							EVNT_APP_vRegister_event_in_st_74(xEvent);
							registered = true;
						}
						break;
					
					default:
						event_number += registered ? 0 : 1;
						xEvent.event_number = event_number;
						EVNT_APP_vRegister_event_in_st_74(xEvent);
						registered = true;
						break;
				}
			}
		
			if ( EVNT_APP_bIs_std_event_monitored_by_st_75(code) ) {
			
				switch (code) {
				
					case TABLE_WRITTEN_TO:
						if ( EVNT_APP_bIs_table_monitored_by_st_75(argument) ) {
							event_number += registered ? 0 : 1;
							xEvent.event_number = event_number;
							EVNT_APP_vRegister_event_in_st_76(xEvent);
							registered = true;
						}
						break;
				
					case PROCEDURE_INVOKED:
						if ( EVNT_APP_bIs_procedure_monitored_by_st_75(argument) ) {
							event_number += registered ? 0 : 1;
							xEvent.event_number = event_number;
							EVNT_APP_vRegister_event_in_st_76(xEvent);
							registered = true;
						}
						break;
				
					default:
						event_number += registered ? 0 : 1;
						xEvent.event_number = event_number;
						EVNT_APP_vRegister_event_in_st_76(xEvent);
						registered = true;
						break;
				}
			}
		
			if ( EVNT_APP_bIs_std_alarm_monitored_by_mt_73(code) ) {
			
				EVNT_APP_vGenerate_std_alarm(code);
			}	
		}
		else {
		
			if ( EVNT_APP_bIs_mfg_event_monitored_by_st_73(code) ) {
				event_number += registered ? 0 : 1;
				xEvent.event_number = event_number;
				EVNT_APP_vRegister_event_in_st_74(xEvent);
				registered = true;
			}
							
			if ( EVNT_APP_bIs_mfg_event_monitored_by_st_75(code) ) {
				event_number += registered ? 0 : 1;
				xEvent.event_number = event_number;
				EVNT_APP_vRegister_event_in_st_76(xEvent);
				registered = true;
			}

			if ( EVNT_APP_bIs_mfg_alarm_monitored_by_mt_73(code) ) {
			
				EVNT_APP_vGenerate_mfg_alarm(code);
			}
		}
	}
}

/**
******************************************************************************
* \fn void EVNT_APP_bIs_std_event_monitored_by_st_73(uint8_t code)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
bool EVNT_APP_bIs_std_event_monitored_by_st_73(uint8_t code) {
	
	uint8_t octet_nbr;
	uint8_t elements_mask;
	bool is_event = false;
	
	octet_nbr = (code / 8);
	elements_mask = 1 << (code - 8*octet_nbr);
	if ( (st_73.std_events_monitored_flags[octet_nbr] & elements_mask) == elements_mask ) {
		is_event = true;
	}
	
	return is_event;
}

/**
******************************************************************************
* \fn void EVNT_APP_bIs_mfg_event_monitored_by_st_73(uint8_t code)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
bool EVNT_APP_bIs_mfg_event_monitored_by_st_73(uint8_t code) {
	
	uint8_t octet_nbr;
	uint8_t elements_mask;
	bool is_event = false;
	
	octet_nbr = (code / 8);
	elements_mask = 1 << (code - 8*octet_nbr);
	if ( (st_73.mfg_events_monitored_flags[octet_nbr] & elements_mask) == elements_mask ) {
		is_event = true;
	}
	
	return is_event;
}

/**
******************************************************************************
* \fn void EVNT_APP_bIs_table_monitored_by_st_73(uint16_t table_ida_bfld)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
bool EVNT_APP_bIs_table_monitored_by_st_73(uint16_t table_ida_bfld) {
	
	uint8_t octet_nbr;
	uint8_t elements_mask;
	bool is_event = false;
	
	if ( (table_ida_bfld & STD_VS_MFG_FLAG_bm) == 0 ) {
		octet_nbr = ( (table_ida_bfld & TBL_PROC_NBR_bm) / 8 );
		elements_mask = 1 << ( (table_ida_bfld & TBL_PROC_NBR_bm) - 8*octet_nbr);
		if ( (st_73.std_tbls_monitored_flags[octet_nbr] & elements_mask) == elements_mask ) {
			is_event = true;
		}
	}
	else if ( (table_ida_bfld & STD_VS_MFG_FLAG_bm) == STD_VS_MFG_FLAG_bm ) {
		octet_nbr = ( (table_ida_bfld & TBL_PROC_NBR_bm) / 8 );
		elements_mask = 1 << ( (table_ida_bfld & TBL_PROC_NBR_bm) - 8*octet_nbr);
		if ( (st_73.mfg_tbls_monitored_flags[octet_nbr] & elements_mask) == elements_mask ) {
			is_event = true;
		}
	}
	
	return is_event;
}

/**
******************************************************************************
* \fn void EVNT_APP_bIs_procedure_monitored_by_st_73(uint16_t table_idc_bfld)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
bool EVNT_APP_bIs_procedure_monitored_by_st_73(uint16_t table_idc_bfld) {
	
	uint8_t octet_nbr;
	uint8_t elements_mask;
	bool is_event = false;
	
	if ( (table_idc_bfld & STD_VS_MFG_FLAG_bm) == 0 ) {
		octet_nbr = ( (table_idc_bfld & TBL_PROC_NBR_bm) / 8 );
		elements_mask = 1 << ( (table_idc_bfld & TBL_PROC_NBR_bm) - 8*octet_nbr);
		if ( (st_73.std_proc_monitored_flags[octet_nbr] & elements_mask) == elements_mask ) {
			is_event = true;
		}
	}
	else if ( (table_idc_bfld & STD_VS_MFG_FLAG_bm) == STD_VS_MFG_FLAG_bm ) {
		octet_nbr = ( (table_idc_bfld & TBL_PROC_NBR_bm) / 8 );
		elements_mask = 1 << ( (table_idc_bfld & TBL_PROC_NBR_bm) - 8*octet_nbr);
		if ( (st_73.mfg_proc_monitored_flags[octet_nbr] & elements_mask) == elements_mask ) {
			is_event = true;
		}
	}
	
	return is_event;
}

/**
******************************************************************************
* \fn void EVNT_APP_bIs_std_event_monitored_by_st_75(uint8_t code)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
bool EVNT_APP_bIs_std_event_monitored_by_st_75(uint8_t code) {
	
	uint8_t octet_nbr;
	uint8_t elements_mask;
	bool is_event = false;

	octet_nbr = (code / 8);
	elements_mask = 1 << (code - 8*octet_nbr);
	if ( (st_75.std_events_monitored_flags[octet_nbr] & elements_mask ) == elements_mask ) {
		is_event = true;
	}
	
	return is_event;
}

/**
******************************************************************************
* \fn void EVNT_APP_bIs_mfg_event_monitored_by_st_75(uint8_t code)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
bool EVNT_APP_bIs_mfg_event_monitored_by_st_75(uint8_t code) {
	
	uint8_t octet_nbr;
	uint8_t elements_mask;
	bool is_event = false;

	octet_nbr = (code / 8);
	elements_mask = 1 << (code - 8*octet_nbr);
	if ( (st_75.mfg_events_monitored_flags[octet_nbr] & elements_mask ) == elements_mask ) {
		is_event = true;
	}
	
	return is_event;
}

/**
******************************************************************************
* \fn void EVNT_APP_bIs_table_monitored_by_st_75(uint16_t table_ida_bfld)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
bool EVNT_APP_bIs_table_monitored_by_st_75(uint16_t table_ida_bfld) {
	
	uint8_t octet_nbr;
	uint8_t elements_mask;
	bool is_event = false;
	
	if ( (table_ida_bfld & STD_VS_MFG_FLAG_bm) == 0 ) {
		octet_nbr = ( (table_ida_bfld & TBL_PROC_NBR_bm) / 8 );
		elements_mask = 1 << ( (table_ida_bfld & TBL_PROC_NBR_bm) - 8*octet_nbr);
		if ( (st_75.std_tbls_monitored_flags[octet_nbr] & elements_mask) == elements_mask ) {
			is_event = true;
		}
	}
	else if ( (table_ida_bfld & STD_VS_MFG_FLAG_bm) == STD_VS_MFG_FLAG_bm ) {
		octet_nbr = ( (table_ida_bfld & TBL_PROC_NBR_bm) / 8 );
		elements_mask = 1 << ( (table_ida_bfld & TBL_PROC_NBR_bm) - 8*octet_nbr);
		if ( (st_75.mfg_tbls_monitored_flags[octet_nbr] & elements_mask) == elements_mask ) {
			is_event = true;
		}
	}
		
	return is_event;
}

/**
******************************************************************************
* \fn void EVNT_APP_bIs_procedure_monitored_by_st_75(uint16_t table_idc_bfld)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
bool EVNT_APP_bIs_procedure_monitored_by_st_75(uint16_t table_idc_bfld) {
	
	uint8_t octet_nbr;
	uint8_t elements_mask;
	bool is_event = false;
	
	if ( (table_idc_bfld & STD_VS_MFG_FLAG_bm) == 0 ) {
		octet_nbr = ( (table_idc_bfld & TBL_PROC_NBR_bm) / 8 );
		elements_mask = 1 << ( (table_idc_bfld & TBL_PROC_NBR_bm) - 8*octet_nbr);
		if ( (st_75.std_proc_monitored_flags[octet_nbr] & elements_mask) == elements_mask ) {
			is_event = true;
		}
	}
	else if ( (table_idc_bfld & STD_VS_MFG_FLAG_bm) == STD_VS_MFG_FLAG_bm ) {
		octet_nbr = ( (table_idc_bfld & TBL_PROC_NBR_bm) / 8 );
		elements_mask = 1 << ( (table_idc_bfld & TBL_PROC_NBR_bm) - 8*octet_nbr);
		if ( (st_75.mfg_proc_monitored_flags[octet_nbr] & elements_mask) == elements_mask ) {
			is_event = true;
		}
	}
	
	return is_event;
}

/**
******************************************************************************
* \fn void EVNT_APP_bIs_std_alarm_monitored_by_mt_73(uint8_t code)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
bool EVNT_APP_bIs_std_alarm_monitored_by_mt_73(uint8_t code) {
	
	uint8_t octet_nbr;
	uint8_t elements_mask;
	bool is_event = false;
	
	octet_nbr = (code / 8);
	elements_mask = 1 << (code - 8*octet_nbr);
	if ( (mt_73.std_alarms_monitored_flags[octet_nbr] & elements_mask) == elements_mask ) {
		is_event = true;
	}
	
	return is_event;
}

/**
******************************************************************************
* \fn void EVNT_APP_bIs_mfg_alarm_monitored_by_mt_73(uint8_t code)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
bool EVNT_APP_bIs_mfg_alarm_monitored_by_mt_73(uint8_t code) {
	
	uint8_t octet_nbr;
	uint8_t elements_mask;
	bool is_event = false;
	
	octet_nbr = (code / 8);
	elements_mask = 1 << (code - 8*octet_nbr);
	if ( (mt_73.mfg_alarms_monitored_flags[octet_nbr] & elements_mask) == elements_mask ) {
		is_event = true;
	}
	
	return is_event;
}

/**
******************************************************************************
* \fn void EVNT_APP_vGenerate_std_alarm(uint8_t alarm_id)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void EVNT_APP_vGenerate_std_alarm(uint8_t alarm_id) {
	
	uint8_t octet_nbr;
	uint8_t elements_mask;
	
	octet_nbr = (alarm_id / 8);
	elements_mask = 1 << (alarm_id - 8*octet_nbr);

	st_3.ed_mfg_status[octet_nbr] |= elements_mask;

	NIC_APP_vAssert_attention_request();
}

/**
******************************************************************************
* \fn void EVNT_APP_vGenerate_mfg_alarm(uint8_t alarm_id)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void EVNT_APP_vGenerate_mfg_alarm(uint8_t alarm_id) {
	
	uint8_t octet_nbr;
	uint8_t elements_mask;
	
	octet_nbr = (alarm_id / 8);
	elements_mask = 1 << (alarm_id - 8*octet_nbr);

	st_3.ed_mfg_status[MAX_NBR_STD_EVENTS + octet_nbr] |= elements_mask;

	NIC_APP_vAssert_attention_request();
}

/**
******************************************************************************
* \fn void EVNT_APP_vRegister_event_in_st_74(EVENT_ALARM_TYPE xEvent)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void EVNT_APP_vRegister_event_in_st_74(EVENT_ALARM_TYPE xEvent) {

	/* Number of valid entries in the ST_74 array */
	if (st_74.nbr_valid_entries < st_71.nbr_history_entries) {
		st_74.nbr_valid_entries++;
	}
	else {
		st_74.nbr_valid_entries = st_71.nbr_history_entries;
	}
	
	/* Update last entry element and last entry sequence number in the ST_74 array */
	if (st_74.nbr_valid_entries <= 1) {
		st_74.last_entry_element = 0;
		st_74.last_entry_seq_nbr = 0;
		st_74.nbr_unread_entries = 0;
	}
	else {
		
		st_74.last_entry_element++;
		st_74.last_entry_seq_nbr++;
		st_74.nbr_unread_entries++;
		
		if (st_74.last_entry_element >= st_71.nbr_history_entries) {
			st_74.last_entry_element = 0;
			st_74.nbr_unread_entries = 0;
		}
	}
	
	uint16_t history_code = 0;
	history_code = (xEvent.code << TBL_PROC_NBR_bp) & TBL_PROC_NBR_bm;
	history_code |= xEvent.is_std ? (0 << STD_VS_MFG_FLAG_bp) : (1 << STD_VS_MFG_FLAG_bp);
	
	EEPROM_writer( ST_74_NBR_VALID_ENTRIES_ADDR, &st_74.nbr_valid_entries, 2 );
	EEPROM_writer( ST_74_LAST_ENTRY_ELEMENT_ADDR, &st_74.last_entry_element, 2 );
	EEPROM_writer( ST_74_LAST_ENTRY_SEQ_NBR_ADDR, &st_74.last_entry_seq_nbr, 4 );
	EEPROM_writer( ST_74_HISTORY_TIME_ADDR + 4*st_74.last_entry_element, &xEvent.timestamp, 4 );
	EEPROM_writer( ST_74_EVENT_NUMBER_ADDR + 2*st_74.last_entry_element, &xEvent.event_number, 2 );
	EEPROM_writer( ST_74_HISTORY_SEQ_NBR_ADDR + 2*st_74.last_entry_element, &st_74.last_entry_seq_nbr, 2 );
	EEPROM_writer( ST_74_USER_ID_ADDR + 2*st_74.last_entry_element, &xEvent.user_id, 2 );	
	EEPROM_writer( ST_74_HISTORY_CODE_ADDR + 2*st_74.last_entry_element, &history_code, 2 );
	EEPROM_writer( ST_74_HISTORY_ARGUMENT_ADDR + 2*st_74.last_entry_element, &xEvent.argument, 2 );		
}

/**
******************************************************************************
* \fn void EVNT_APP_vRegister_event_in_st_76(EVENT_ALARM_TYPE xEvent)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void EVNT_APP_vRegister_event_in_st_76(EVENT_ALARM_TYPE xEvent) {
		
	/* Number of valid entries in the ST_76 array */
	if (st_76.nbr_valid_entries < st_71.nbr_event_entries) {
		st_76.nbr_valid_entries++;
	}
	else {
		st_76.nbr_valid_entries = st_71.nbr_event_entries;
	}
	
	/* Update last entry element and last entry sequence number in the ST_76 array */
	if (st_76.nbr_valid_entries <= 1) {
		st_76.last_entry_element = 0;
		st_76.last_entry_seq_nbr = 0;
		st_76.nbr_unread_entries = 0;
	}
	else {
		
		st_76.last_entry_element++;
		st_76.last_entry_seq_nbr++;
		st_76.nbr_unread_entries++;
		
		if (st_76.last_entry_element >= st_71.nbr_event_entries) {
			st_76.last_entry_element = 0;
			st_76.nbr_unread_entries = 0;
		}
	}
	
	uint16_t event_code = 0;
	event_code = (xEvent.code << TBL_PROC_NBR_bp) & TBL_PROC_NBR_bm;
	event_code |= xEvent.is_std ? (0 << STD_VS_MFG_FLAG_bp) : (1 << STD_VS_MFG_FLAG_bp);
	
	EEPROM_writer( ST_76_NBR_VALID_ENTRIES_ADDR, &st_76.nbr_valid_entries, 2 );
	EEPROM_writer( ST_76_LAST_ENTRY_ELEMENT_ADDR, &st_76.last_entry_element, 2 );
	EEPROM_writer( ST_76_LAST_ENTRY_SEQ_NBR_ADDR, &st_76.last_entry_seq_nbr, 4 );
	EEPROM_writer( ST_76_EVENT_TIME_ADDR + 4*st_76.last_entry_element, &xEvent.timestamp, 4 );
	EEPROM_writer( ST_76_EVENT_NUMBER_ADDR + 2*st_76.last_entry_element, &xEvent.event_number, 2 );
	EEPROM_writer( ST_76_EVENT_SEQ_NBR_ADDR + 2*st_76.last_entry_element, &st_76.last_entry_seq_nbr, 2 );
	EEPROM_writer( ST_76_USER_ID_ADDR + 2*st_76.last_entry_element, &xEvent.user_id, 2 );
	EEPROM_writer( ST_76_EVENT_CODE_ADDR + 2*st_76.last_entry_element, &event_code, 2 );
	EEPROM_writer( ST_76_EVENT_ARGUMENT_ADDR + 2*st_76.last_entry_element, &xEvent.argument, 2 );	
}

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2016-03-07
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */