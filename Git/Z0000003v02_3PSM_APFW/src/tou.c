/**
 ******************************************************************************
 * \file tou.c
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
#include "tou.h"
#include "rtc.h"
#include "util.h"
#include "sm.h"
#include "rtc.h"
#include "regs.h"
#include "evnt.h"
#include "watchdog.h"
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
static TOU_TYPE tou;
static const uint8_t month_days[2][13] = { {0,31,28,31,30,31,30,31,31,30,31,30,31}, {0,31,29,31,30,31,30,31,31,30,31,30,31} };
static ST_50_TYPE st_51;

/*
 ******************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ******************************************************************************
 */
TOU_OS_TYPE tou_os;

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
static HUMAN_TIME_TYPE TOU_APP_xGet_postponed_date(uint8_t nbr_weekday_after, uint8_t weekday, HUMAN_TIME_TYPE date);

static void TOU_APP_vChange_rate(uint8_t tier, uint16_t user_attempt, RTC_TYPE *rtc);
static void TOU_APP_vChange_tier_switch(uint8_t tier_sum, uint8_t tier_dem, uint16_t user_attempt, RTC_TYPE *rtc);
static void TOU_APP_vActivate_special_sched(SPECIAL_SCHED_ID_TYPE sched_id, uint16_t user_attempt, RTC_TYPE *rtc);
static uint8_t TOU_APP_ucGet_special_sched_active(RTC_TYPE *rtc);
static uint8_t TOU_APP_ucGet_demand_and_self_read_flags(RTC_TYPE *rtc);

static void TOU_APP_vSearch_date_on_calendar(RTC_TYPE *rtc);
static void TOU_APP_vUpdate_current_tier(RTC_TYPE *rtc);
static void TOU_APP_vUpdate_current_season(RTC_TYPE *rtc);
static void TOU_APP_vLoad_day_tier_switchpoints(void);
static void TOU_APP_vLoad_year_season_dates(void);
static uint32_t TOU_APP_ulGet_last_tier_change(void);

static bool TOU_APP_bSt_51_validate_data(ST_50_TYPE *st_51_structure);
static void TOU_APP_vSt_51_read_eeprom_values(ST_50_TYPE *st_51_structure);
static void TOU_APP_vSt_51_write_eeprom_values(ST_50_TYPE *st_51_structure);
static void TOU_APP_vSt_51_to_array(ST_50_TYPE st_51_source, uint8_t *table_buffer);

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */

/** 
 ******************************************************************************
 * \fn void TOU_APP_vInitialize(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vInitialize(void) {
	
	memset(&tou, 0, sizeof(tou));
	TOU_APP_vRead_eeprom_current_season();
	tou.curr_special_sched_active = (uint8_t)NO_SPECIAL_SCHED_ACTIVE;
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_st_50_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_st_50_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	
	table_buffer[k++] = PERM_TIME_FUNC_FLAG1;
	table_buffer[k++] = PERM_TIME_FUNC_FLAG2;
	table_buffer[k++] = MAX_CALENDAR_FUNC_VALUES;
	table_buffer[k++] = MAX_NBR_NON_RECURR_DATES;
	table_buffer[k++] = MAX_NBR_RECURR_DATES;
	table_buffer[k++] = (uint8_t)((MAX_NBR_TIER_SWITCHES & 0x00FF) >> 0);
	table_buffer[k++] = (uint8_t)((MAX_NBR_TIER_SWITCHES & 0xFF00) >> 8);
	table_buffer[k++] = (uint8_t)((MAX_CALENDAR_TBL_SIZE & 0x00FF) >> 0);
	table_buffer[k++] = (uint8_t)((MAX_CALENDAR_TBL_SIZE & 0xFF00) >> 8);
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t TOU_APP_sSt_50_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t TOU_APP_sSt_50_get_size(void) {

	return (uint16_t) ST_50_SIZE;
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bLoad_st_51(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bLoad_st_51(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_51_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = TOU_APP_bSt_51_restore_default_values();
	}
	else {
		
		is_ok = TOU_APP_bSt_51_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bSt_51_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bSt_51_restore_default_values(void) {
	
	bool is_ok = false;
	ST_50_TYPE st_51_temp;
	
	st_51_temp.time_func_flag1 = DEFAULT_TIME_FUNC_FLAG1;
	st_51_temp.time_func_flag2 = DEFAULT_TIME_FUNC_FLAG2;
	st_51_temp.calendar_func = DEFAULT_CALENDAR_FUNC_VALUES;
	st_51_temp.nbr_non_recurr_dates = DEFAULT_NBR_NON_RECURR_DATES;
	st_51_temp.nbr_recurr_dates = DEFAULT_NBR_RECURR_DATES;
	st_51_temp.nbr_tier_switches = DEFAULT_NBR_TIER_SWITCHES;
	st_51_temp.calendar_tbl_size = TOU_APP_usSt_54_get_size();
	
	is_ok = TOU_APP_bSt_51_validate_data(&st_51_temp);
	
	st_51 = st_51_temp;
	TOU_APP_vSt_51_write_eeprom_values(&st_51);
	uint16_t crc = SM_APP_usSt_51_ram_calculate_crc();
	SM_APP_vSt_51_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bSt_51_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bSt_51_refresh_ram(void) {
	
	ST_50_TYPE st_51_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		TOU_APP_vSt_51_read_eeprom_values(&st_51_temp);
		is_ok = TOU_APP_bSt_51_validate_data(&st_51_temp);
		
		if ( is_ok == true ) {
			st_51 = st_51_temp;
			break;
		}
	}
	
	if ( is_ok == false ) {
		st_51 = st_51_temp;
		TOU_APP_vSt_51_write_eeprom_values(&st_51);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  TOU_APP_vCopy_st_51_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vCopy_st_51_ram(uint8_t* destination) {
	
	TOU_APP_vSt_51_to_array(st_51, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void TOU_APP_vCopy_st_51_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vCopy_st_51_nvm(uint8_t* destination) {
	
	ST_50_TYPE st_51_aux;
	
	TOU_APP_vSt_51_read_eeprom_values(&st_51_aux);
	TOU_APP_vSt_51_to_array(st_51_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void TOU_APP_vSt_51_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vSt_51_recover_nvm(void) {
	
	TOU_APP_vSt_51_write_eeprom_values(&st_51);
}

/**
 ******************************************************************************
 * \fn void TOU_APP_vSt_51_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vSt_51_recover_ram(void) {
	
	TOU_APP_vSt_51_read_eeprom_values(&st_51);
}

/**
 ******************************************************************************
 * \fn void TOU_APP_vSt_51_to_array(ST_50_TYPE st_51_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vSt_51_to_array(ST_50_TYPE st_51_source, uint8_t *table_buffer) {
	
	table_buffer[0] = st_51_source.time_func_flag1;
	table_buffer[1] = st_51_source.time_func_flag2;
	table_buffer[2] = st_51_source.calendar_func;
	table_buffer[3] = st_51_source.nbr_non_recurr_dates;
	table_buffer[4] = st_51_source.nbr_recurr_dates;
	table_buffer[5] = (uint8_t)((st_51_source.nbr_tier_switches & 0x00FF) >> 0);
	table_buffer[6] = (uint8_t)((st_51_source.nbr_tier_switches & 0xFF00) >> 8);
	
	st_51_source.calendar_tbl_size = TOU_APP_usSt_54_get_size();
	
	table_buffer[7] = (uint8_t)((st_51_source.calendar_tbl_size & 0x00FF) >> 0);
	table_buffer[8] = (uint8_t)((st_51_source.calendar_tbl_size & 0xFF00) >> 8);
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bSt_51_validate_data(ST_50_TYPE *st_51_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bSt_51_validate_data(ST_50_TYPE *st_51_structure) {
	
	bool is_ok = true;
	uint8_t i;
	
	for ( i = 0; i < 8; i++) {
		if ( ( ( (PERM_TIME_FUNC_FLAG1 >> i) & 0x01 ) == 0 ) && ( ( (st_51_structure->time_func_flag1 >> i) & 0x01 ) == 1 ) ) {
			is_ok = false;	
		}
	}
	st_51_structure->time_func_flag1 &= PERM_TIME_FUNC_FLAG1;
	
	for ( i = 0; i < 8; i++) {
		if ( ( ( (PERM_TIME_FUNC_FLAG2 >> i) & 0x01 ) == 0 ) && ( ( (st_51_structure->time_func_flag2 >> i) & 0x01 ) == 1 ) ) {
			is_ok = false;
		}
	}
	st_51_structure->time_func_flag2 &= PERM_TIME_FUNC_FLAG2;
	
	if ( ( (st_51_structure->calendar_func & 0x0F) >> 0 ) > MAX_NBR_SEASONS ) {
		st_51_structure->calendar_func |= (MAX_NBR_SEASONS << 0);
		is_ok = false;
	}
	
	if ( ( (st_51_structure->calendar_func & 0xF0) >> 4 ) > MAX_NBR_SPECIAL_SCHED ) {
		st_51_structure->calendar_func |= (MAX_NBR_SPECIAL_SCHED << 4);
		is_ok = false;
	}
	
	if ( st_51_structure->nbr_non_recurr_dates > MAX_NBR_NON_RECURR_DATES ) {
		st_51_structure->nbr_non_recurr_dates = MAX_NBR_NON_RECURR_DATES;
		is_ok = false;
	}
	
	if ( st_51_structure->nbr_recurr_dates > MAX_NBR_RECURR_DATES ) {
		st_51_structure->nbr_recurr_dates = MAX_NBR_RECURR_DATES;
		is_ok = false;
	}
	
	if ( st_51_structure->nbr_tier_switches > MAX_NBR_TIER_SWITCHES ) {
		st_51_structure->nbr_tier_switches = MAX_NBR_TIER_SWITCHES;
		is_ok = false;
	}
	
	if ( st_51_structure->calendar_tbl_size > MAX_CALENDAR_TBL_SIZE ) {
		st_51_structure->calendar_tbl_size = MAX_CALENDAR_TBL_SIZE;
		is_ok = false;
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void TOU_APP_vSt_51_read_eeprom_values(ST_50_TYPE *st_51_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vSt_51_read_eeprom_values(ST_50_TYPE *st_51_structure) {	
	
	EEPROM_reader( ST_51_TIME_FUNC_FLAG1_ADDR, &st_51_structure->time_func_flag1, 1 );
	EEPROM_reader( ST_51_TIME_FUNC_FLAG2_ADDR, &st_51_structure->time_func_flag2, 1 );
	EEPROM_reader( ST_51_CALENDAR_FUNC_ADDR, &st_51_structure->calendar_func, 1 );
	EEPROM_reader( ST_51_NBR_NON_RECURR_DATES_ADDR, &st_51_structure->nbr_non_recurr_dates, 1 );
	EEPROM_reader( ST_51_NBR_RECURR_DATES_ADDR, &st_51_structure->nbr_recurr_dates, 1 );
	EEPROM_reader( ST_51_NBR_TIER_SWITCHES_ADDR, &st_51_structure->nbr_tier_switches, 2 );
	EEPROM_reader( ST_51_CALENDAR_TBL_SIZE_ADDR, &st_51_structure->calendar_tbl_size, 2);
}

/**
 ******************************************************************************
 * \fn void TOU_APP_vSt_51_write_eeprom_values(ST_50_TYPE *st_51_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vSt_51_write_eeprom_values(ST_50_TYPE *st_51_structure) {
		
	SM_APP_vChange_table_state(ST_51_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
						
	EEPROM_writer( ST_51_TIME_FUNC_FLAG1_ADDR, &st_51_structure->time_func_flag1, 1 );
	EEPROM_writer( ST_51_TIME_FUNC_FLAG2_ADDR, &st_51_structure->time_func_flag2, 1 );
	EEPROM_writer( ST_51_CALENDAR_FUNC_ADDR, &st_51_structure->calendar_func, 1 );
	EEPROM_writer( ST_51_NBR_NON_RECURR_DATES_ADDR, &st_51_structure->nbr_non_recurr_dates, 1 );
	EEPROM_writer( ST_51_NBR_RECURR_DATES_ADDR, &st_51_structure->nbr_recurr_dates, 1 );
	EEPROM_writer( ST_51_NBR_TIER_SWITCHES_ADDR, &st_51_structure->nbr_tier_switches, 2 );
	EEPROM_writer( ST_51_CALENDAR_TBL_SIZE_ADDR, &st_51_structure->calendar_tbl_size, 2);
	
	SM_APP_vChange_table_state(ST_51_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bSt_51_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bSt_51_execute_table_reading(uint8_t *table_buffer) {
	
	TOU_APP_vSt_51_to_array(st_51, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bSt_51_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bSt_51_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	ST_50_TYPE st_51_temp;
	
	st_51_temp.time_func_flag1 = table_buffer[k++];
	st_51_temp.time_func_flag2 = table_buffer[k++];
	st_51_temp.calendar_func = table_buffer[k++];
	st_51_temp.nbr_non_recurr_dates = table_buffer[k++];
	st_51_temp.nbr_recurr_dates = table_buffer[k++];
	st_51_temp.nbr_tier_switches  = (uint16_t)(table_buffer[k++]);
	st_51_temp.nbr_tier_switches |= (uint16_t)(table_buffer[k++] << 8);
	st_51_temp.calendar_tbl_size  = (uint16_t)(table_buffer[k++]);
	st_51_temp.calendar_tbl_size |= (uint16_t)(table_buffer[k++] << 8);
	
	is_ok = TOU_APP_bSt_51_validate_data(&st_51_temp);
	
	if ( is_ok ) {
		
		st_51 = st_51_temp;
		TOU_APP_vSt_51_write_eeprom_values(&st_51);
		uint16_t crc = SM_APP_usSt_51_ram_calculate_crc();
		SM_APP_vSt_51_write_crc(crc);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t TOU_APP_usSt_51_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t TOU_APP_usSt_51_get_size(void) {

	return (uint16_t)ST_51_SIZE;
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bSt_52_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bSt_52_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	uint8_t time_date_qual = 0;
	RTC_TYPE rtc_temp;
	
	RTC_APP_vBreak_timestamp( &rtc_temp, rtc.utc_timestamp);
	time_date_qual  = rtc_temp.day_of_week << DAY_OF_WEEK_bp;
	time_date_qual |= rtc.is_dst << DST_FLAG_bp;
	time_date_qual |= 1 << GMT_FLAG_bp;
	time_date_qual |= 1 << TM_ZN_APPLIED_FLAG_bp;
	time_date_qual |= rtc.dst_applied_flag << DST_APPLIED_FLAG_bp;
	time_date_qual |= 1 << DST_SUPPORTED_FLAG_bp;
	
	table_buffer[k++] = (uint8_t)(rtc.utc_timestamp);
	table_buffer[k++] = (uint8_t)(rtc.utc_timestamp >> 8);
	table_buffer[k++] = (uint8_t)(rtc.utc_timestamp >> 16);
	table_buffer[k++] = (uint8_t)(rtc.utc_timestamp >> 24);
	table_buffer[k++] = time_date_qual;
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t TOU_APP_usSt_52_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t TOU_APP_usSt_52_get_size(void) {

	return (uint16_t) ST_52_SIZE;
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bLoad_st_53(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bLoad_st_53(void) {
	
	if ( SM_APP_ucGet_table_state(ST_53_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		TOU_APP_bSt_53_restore_default_values();
	}
	else {
		// There is no shadow ram for this table
	}
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bSt_53_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bSt_53_restore_default_values(void) {
	
	uint8_t i;
	uint8_t std_version_no = STD_VERSION_NO;
	uint8_t data[ST_53_MAX_SIZE];
	uint8_t null_data = 0;
	
	data[0] = (uint8_t)(DEFAULT_DST_TIME_EFF);
	data[1] = (uint8_t)(DEFAULT_DST_TIME_EFF >> 8);
	data[2] = (uint8_t)(DEFAULT_DST_TIME_EFF >> 16);
	data[3] = (uint8_t)(DEFAULT_DST_TIME_EFF >> 24);
	data[4] = DEFAULT_DST_TIME_AMT;
	data[5] = (uint8_t)(DEFAULT_TIME_ZONE_OFFSET);
	data[6] = (uint8_t)(DEFAULT_TIME_ZONE_OFFSET >> 8);
	data[7] = (uint8_t)(DEFAULT_STD_TIME_EFF);
	data[8] = (uint8_t)(DEFAULT_STD_TIME_EFF >> 8);
	data[9] = (uint8_t)(DEFAULT_STD_TIME_EFF >> 16);
	data[10] = (uint8_t)(DEFAULT_STD_TIME_EFF >> 24);
	
	SM_APP_vChange_table_state(ST_53_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	for ( i = 0; i < ST_53_MAX_SIZE; i++) {
		EEPROM_writer( ST_53_START_ADDR + i, &null_data, 1 );
	}
	
	EEPROM_writer( ST_53_DST_TIME_EFF_ADDR, &data[0], 4 );
		
	EEPROM_writer( ST_53_DST_TIME_AMT_ADDR, &data[4], 1 );
	
	if ( ((DEFAULT_TIME_FUNC_FLAG2 & CAP_TM_ZN_OFFSET_FLAG_bm) == CAP_TM_ZN_OFFSET_FLAG_bm) || ( std_version_no > 1 ) ) {
		EEPROM_writer( ST_53_TIME_ZONE_OFFSET_ADDR, &data[5], 2 );
	}
	
	if ( std_version_no > 1 ) {
		EEPROM_writer( ST_53_STD_TIME_EFF_ADDR, &data[7], 4 );
	}
	
	SM_APP_vChange_table_state(ST_53_PROG_STATE_ADDR, C1219_TABLE_FULL);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bSt_53_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bSt_53_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	uint8_t std_version_no = STD_VERSION_NO;

	delay_ms(10);

	EEPROM_reader( ST_53_DST_TIME_EFF_ADDR, &table_buffer[k], 4 );
	k += 4;
	
	EEPROM_reader( ST_53_DST_TIME_AMT_ADDR, &table_buffer[k++], 1 );
	
	if ( ((st_51.time_func_flag2 & CAP_TM_ZN_OFFSET_FLAG_bm) == CAP_TM_ZN_OFFSET_FLAG_bm) || ( std_version_no > 1 ) ) {
		EEPROM_reader( ST_53_TIME_ZONE_OFFSET_ADDR, &table_buffer[k], 2 );
		k += 2;
	}
	
	if ( std_version_no > 1 ) {
		EEPROM_reader( ST_53_STD_TIME_EFF_ADDR + 0, &table_buffer[k], 4 );
		k += 4;
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bSt_53_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bSt_53_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	uint8_t std_version_no = STD_VERSION_NO;
	
	EEPROM_writer( ST_53_DST_TIME_EFF_ADDR, &table_buffer[k], 4 );
	k += 4;
	
	EEPROM_writer( ST_53_DST_TIME_AMT_ADDR, &table_buffer[k++], 1 );
	
	if ( ((st_51.time_func_flag2 & CAP_TM_ZN_OFFSET_FLAG_bm) == CAP_TM_ZN_OFFSET_FLAG_bm) || ( std_version_no > 1 ) ) {
		EEPROM_writer( ST_53_TIME_ZONE_OFFSET_ADDR, &table_buffer[k], 2 );
		k += 2;
	}
	
	if ( std_version_no > 1 ) {
		EEPROM_writer( ST_53_STD_TIME_EFF_ADDR, &table_buffer[k], 4 );
		k += 4;
	}
	
	is_ok = true;
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t TOU_APP_usSt_53_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t TOU_APP_usSt_53_get_size(void) {

	uint16_t table_size = 5;
	uint8_t std_version_no = STD_VERSION_NO;

	if ( ((st_51.time_func_flag2 & CAP_TM_ZN_OFFSET_FLAG_bm) == CAP_TM_ZN_OFFSET_FLAG_bm) || ( std_version_no > 1 ) ) {
		table_size += 2;
	}
	
	if ( std_version_no > 1 ) {
		table_size += 4;
	}
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bSt_54_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bSt_54_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t i, j;
	uint16_t k = 0;
	
	delay_ms(10);
	
	if ( (st_51.time_func_flag1 & ANCHOR_DATE_FLAG_bm) == ANCHOR_DATE_FLAG_bm ) {
		EEPROM_reader( ST_54_ANCHOR_DATE_ADDR, &table_buffer[k], 2 );
		k += 2;
	}

	for ( i = 0; i < st_51.nbr_non_recurr_dates; i++ ) {
		EEPROM_reader( ST_54_NON_RECURR_DATES_ADDR + 3*i, &table_buffer[k], 3 );
		k += 3;
	}		

	for ( i = 0; i < st_51.nbr_recurr_dates; i++ ) {
		EEPROM_reader( ST_54_RECURR_DATES_ADDR + 3*i, &table_buffer[k], 3 );
		k += 3;
	}

	for ( i = 0; i < st_51.nbr_tier_switches; i++ ) {
		EEPROM_reader( ST_54_TIER_SWITCHES_ADDR + 3*i, &table_buffer[k], 3 );
		k += 3;
	}

	if ( (st_51.time_func_flag2 & SEPARATE_WEEKDAYS_FLAG_bm) == SEPARATE_WEEKDAYS_FLAG_bm ) {

	}
	else {
		for ( i = 0; i < ( (st_51.calendar_func & NBR_SEASONS_bm) >> NBR_SEASONS_bp ); i++ ) {
			EEPROM_reader( ST_54_DAILY_SCHEDULE_ID_MATRIX_ADDR + 5*i, &table_buffer[k], 3 );
			k += 3;
			
			for ( j = 0; j < ( (st_51.calendar_func & NBR_SPECIAL_SCHED_bm) >> NBR_SPECIAL_SCHED_bp ); j++ ) {
				EEPROM_reader( ST_54_DAILY_SCHEDULE_ID_MATRIX_ADDR + 5*i + 3 + j, &table_buffer[k], 1 );
				k += 1;
			}
		}
	}
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool TOU_APP_bSt_54_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bSt_54_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;

	uint16_t i, j;
	uint16_t k = 0;
	
	SM_APP_vChange_table_state(ST_54_PROG_STATE_ADDR, C1219_TABLE_EMPTY);

	if ( (st_51.time_func_flag1 & ANCHOR_DATE_FLAG_bm) == ANCHOR_DATE_FLAG_bm ) {
		EEPROM_writer( ST_54_ANCHOR_DATE_ADDR, &table_buffer[k], 2 );
		k += 2;
	}

	for ( i = 0; i < st_51.nbr_non_recurr_dates; i++ ) {
		EEPROM_writer( ST_54_NON_RECURR_DATES_ADDR + 3*i, &table_buffer[k], 3 );
		k += 3;
	}		
	
	for ( i = 0; i < st_51.nbr_recurr_dates; i++ ) {
		EEPROM_writer( ST_54_RECURR_DATES_ADDR + 3*i, &table_buffer[k], 3 );
		k += 3;
	}

	for ( i = 0; i < st_51.nbr_tier_switches; i++ ) {
		EEPROM_writer( ST_54_TIER_SWITCHES_ADDR + 3*i, &table_buffer[k], 3 );
		k += 3;
	}

	if ( (st_51.time_func_flag2 & SEPARATE_WEEKDAYS_FLAG_bm) == SEPARATE_WEEKDAYS_FLAG_bm ) {

	}
	else {
		for ( i = 0; i < ( (st_51.calendar_func & NBR_SEASONS_bm) >> NBR_SEASONS_bp ); i++ ) {
			EEPROM_writer( ST_54_DAILY_SCHEDULE_ID_MATRIX_ADDR + 5*i, &table_buffer[k], 3 );
			k += 3;
			
			for ( j = 0; j < ( (st_51.calendar_func & NBR_SPECIAL_SCHED_bm) >> NBR_SPECIAL_SCHED_bp ); j++ ) {
				EEPROM_writer( ST_54_DAILY_SCHEDULE_ID_MATRIX_ADDR + 5*i + 3 + j, &table_buffer[k], 1 );
				k += 1;
			}
		}
	}
	
	SM_APP_vChange_table_state(ST_54_PROG_STATE_ADDR, C1219_TABLE_FULL);
	
	is_ok = true;
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t TOU_APP_usSt_54_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t TOU_APP_usSt_54_get_size(void) {
	
	uint16_t table_size = 0;
	
	if ( (st_51.time_func_flag1 & ANCHOR_DATE_FLAG_bm) == ANCHOR_DATE_FLAG_bm ) {
		
		table_size += 2;
	}
	
	table_size += (3 * st_51.nbr_non_recurr_dates);
	table_size += (3 * st_51.nbr_recurr_dates);
	table_size += (3 * st_51.nbr_tier_switches);
	
	if ( (st_51.time_func_flag2 & SEPARATE_WEEKDAYS_FLAG_bm) == SEPARATE_WEEKDAYS_FLAG_bm ) {
		
		table_size += ( 7 * ( (st_51.calendar_func & NBR_SEASONS_bm) >> NBR_SEASONS_bp ) );
	}
	else {
		
		table_size += ( 
						( (st_51.calendar_func & NBR_SEASONS_bm) >> NBR_SEASONS_bp ) *						\
					    ( 3 + ( (st_51.calendar_func & NBR_SPECIAL_SCHED_bm) >> NBR_SPECIAL_SCHED_bp ) )
					  );
	}
		
	return table_size;
}


/**
 ******************************************************************************
 * \fn bool TOU_APP_bSt_55_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool TOU_APP_bSt_55_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	uint16_t status = 0;
	uint8_t tier_drive = 0;
	uint8_t time_date_qual = 0;
	RTC_TYPE rtc_temp;

	table_buffer[k++] = (uint8_t)(rtc.utc_timestamp);
	table_buffer[k++] = (uint8_t)(rtc.utc_timestamp >> 8);
	table_buffer[k++] = (uint8_t)(rtc.utc_timestamp >> 16);
	table_buffer[k++] = (uint8_t)(rtc.utc_timestamp >> 24);
	
	RTC_APP_vBreak_timestamp( &rtc_temp, rtc.utc_timestamp);
	
	time_date_qual  = rtc_temp.day_of_week << DAY_OF_WEEK_bp;
	time_date_qual |= rtc.is_dst << DST_FLAG_bp;
	time_date_qual |= 1 << GMT_FLAG_bp;
	time_date_qual |= 1 << TM_ZN_APPLIED_FLAG_bp;
	time_date_qual |= rtc.dst_applied_flag << DST_APPLIED_FLAG_bp;
	time_date_qual |= 1 << DST_SUPPORTED_FLAG_bp;
	
	table_buffer[k++] = time_date_qual;
	
	if ( (st_51.time_func_flag2 & SEPARATE_SUM_DEMANDS_FLAG_bm) == SEPARATE_SUM_DEMANDS_FLAG_bm ) {
		
		status =  tou.curr_tier_sum << CURR_SUMM_TIER_bp;
		status |= tou.curr_tier_dem << CURR_DEMAND_TIER_bp;
	}
	else {
		status = tou.curr_tier << CURR_TIER_bp;
	}
	
	status |= tier_drive << TIER_DRIVE_bp;
	status |= tou.curr_special_sched_active << SPECIAL_SCHD_ACTIVE_bp;
	status |= tou.curr_season << SEASON_bp;
	
	table_buffer[k++] = (uint8_t)(status);
	table_buffer[k++] = (uint8_t)(status >> 8);
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t TOU_APP_usSt_55_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t TOU_APP_usSt_55_get_size(void) {

	return (uint16_t)ST_55_SIZE;
}

/**
 ******************************************************************************
 * \fn void TOU_APP_vCreate_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vCreate_task(void) {
	
	tou_os.rtos_task_active = false;

	/* Create TOU refresh task */
	xTaskCreate( TOU_APP_vRtos_task, (const signed char * const ) "TOU_APP_vTask", TOU_STACK_SIZE, NULL, TOU_APP_vTask_PRIO, &tou_os.task_id );

	tou_os.rtos_task_active = true;
}

/**
 ******************************************************************************
 * \fn void TOU_APP_vRtos_task(void *pvParameters)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vRtos_task(void *pvParameters) {
	
	UNUSED( pvParameters );
		
	/* Time of Use module task Loop. */
	for ( ; ; ) {
		
		if ( tou_os.rtos_task_active ) {
			
			TOU_APP_vTask();
			WATCHDOG_APP_vRestart();
		}
		
		/* Suspend TOU Task */
		vTaskSuspend(tou_os.task_id);
	}
	
	/* Should never go there */
	vTaskDelete(NULL);
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vSuspend_rtos_task(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vSuspend_rtos_task(void) {

	tou_os.rtos_task_active = false;

	if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) {

		vTaskSuspend(tou_os.task_id);
	}
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vResume_rtos_task(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vResume_rtos_task(void) {

	tou_os.rtos_task_active = true;
}	

/** 
 ******************************************************************************
 * \fn void TOU_APP_vTask(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vTask(void) {
	
	if ( !meter.system_state.changing_configuration ) {

		if ( (rtc.is_time_relative == 0) && (TOU_APP_usSt_54_get_size() > 0) ) {
		
			RTC_TYPE tou_rtc = rtc;
		
			if (tou_os.register_dst_on) {
				tou_os.register_dst_on = false;
				EVNT_APP_vRegister_event(DAYLIGHT_SAVINGS_TIME_ON, 0, true, tou_os.user, tou_os.event_timestamp);
			}
		
			if (tou_os.register_dst_off) {
				tou_os.register_dst_off = false;
				EVNT_APP_vRegister_event(DAYLIGHT_SAVINGS_TIME_OFF, 0, true, tou_os.user, tou_os.event_timestamp);
			}
		
			if (tou_os.rtc_change_mask & YEAR_CHANGE_bm) {
			
				TOU_APP_vLoad_year_season_dates();		
			}
		
			if (tou_os.rtc_change_mask & DAY_CHANGE_bm) {
			
				TOU_APP_vUpdate_current_season(&tou_rtc);
			
				uint8_t calendar_flags = TOU_APP_ucGet_demand_and_self_read_flags(&tou_rtc);
			
				if ( calendar_flags & DEMAND_RESET_FLAG_bm ) {
					tou.pending_demand_reset = 1;
				}
			
				if ( calendar_flags & SELF_READ_FLAG_bm ) {
					tou.pending_self_read = 1;
				}
			
				uint8_t sched_id = TOU_APP_ucGet_special_sched_active(&tou_rtc);
			
				if (sched_id <= (uint8_t)NO_SPECIAL_SCHED_ACTIVE) {
				
					TOU_APP_vActivate_special_sched( (SPECIAL_SCHED_ID_TYPE)sched_id, END_DEVICE, &tou_rtc );
				}
			
				TOU_APP_vLoad_day_tier_switchpoints();
			}
		
			if (tou_os.rtc_change_mask & MINUTE_CHANGE_bm) {
			
				TOU_APP_vUpdate_current_tier(&tou_rtc);
			}
		
			if ( (tou.pending_season_change | tou.pending_demand_reset | tou.pending_self_read) == 1) {
			
				uint8_t season;
		
				REGS_APP_vCopy_st_23();
		
				if (tou.pending_season_change == 1) {
					EEPROM_reader(TOU_LAST_SEASON_ADDR, &season, 1);
					tou.pending_season_change = 0;
					TOU_APP_vSeason_change(END_DEVICE, season, tou_rtc.utc_timestamp);
				}
		
				if (tou.pending_demand_reset == 1) {
					tou.pending_demand_reset = 0;
					REGS_APP_vDemand_reset(END_DEVICE, tou.curr_season, tou_rtc.utc_timestamp);
				}
		
				if (tou.pending_self_read == 1) {
					tou.pending_self_read = 0;					
					REGS_APP_vSelf_read(END_DEVICE, tou.curr_season, tou_rtc.utc_timestamp);
				}
			}
		
			tou_os.rtc_change_mask = 0;
		}
	}
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vUpdate_current_tier(RTC_TYPE *rtc)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vUpdate_current_tier(RTC_TYPE *rtc) {
	
	uint8_t i;
	uint8_t tier_sum, tier_dem;
	uint8_t tier_temp;
	
	if (rtc->is_time_relative == 0) {
		
		tier_temp = 0;
		tier_sum = 0;
		tier_dem = 0;
		
		for ( i = 0; i < tou.nbr_day_tiers; i++ ) {
			
			if (rtc->timestamp >= tou.day_tier_switches[i].tier_switch) {
				tier_temp = tou.day_tier_switches[i].tier_id;
			}
			
			if ( (st_51.time_func_flag2 & SEPARATE_SUM_DEMANDS_FLAG_bm) == SEPARATE_SUM_DEMANDS_FLAG_bm ) {
				
				if ( tou.day_tier_switches[i].summ_switch_flag == 1 ) {
					
					if ( rtc->timestamp >= tou.day_tier_switches[i].tier_switch ) {
						
						tier_sum = tou.day_tier_switches[i].tier_id;
					}
				}
				
				if ( tou.day_tier_switches[i].demand_switch_flag == 1 ) {
					
					if ( rtc->timestamp >= tou.day_tier_switches[i].tier_switch ) {
						
						tier_dem = tou.day_tier_switches[i].tier_id;
					}
				}
			}
		}
		
		TOU_APP_vChange_rate(tier_temp, END_DEVICE, rtc);
		TOU_APP_vChange_tier_switch(tier_sum, tier_dem, END_DEVICE, rtc);
	}
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vChange_rate(uint8_t tier, uint16_t user_attempt, RTC_TYPE *rtc)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vChange_rate(uint8_t tier, uint16_t user_attempt, RTC_TYPE *rtc) {
	
	if ( (uint8_t)tier != tou.curr_tier ) {
		
		REGS_APP_vReset_present_demand();		
		tou.curr_tier = (uint8_t)tier;
	
		EVNT_APP_vRegister_event(RATE_CHANGE, (uint16_t)tier, true, user_attempt, rtc->utc_timestamp);
	}
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vChange_tier_switch(uint8_t tier_sum, uint8_t tier_dem, uint16_t user_attempt, RTC_TYPE *rtc)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vChange_tier_switch(uint8_t tier_sum, uint8_t tier_dem, uint16_t user_attempt, RTC_TYPE *rtc) {
	
	if ( (st_51.time_func_flag2 & SEPARATE_SUM_DEMANDS_FLAG_bm) == SEPARATE_SUM_DEMANDS_FLAG_bm ) {
		
		if ( ( (uint8_t)tier_sum != tou.curr_tier_sum ) || ( (uint8_t)tier_dem != tou.curr_tier_dem ) ) {
			
			if ((uint8_t)tier_sum != tou.curr_tier_dem) {
				REGS_APP_vReset_present_demand();
			}
			
			tou.curr_tier_sum = (uint8_t)tier_sum;
			tou.curr_tier_dem = (uint8_t)tier_dem;
	
			EVNT_APP_vRegister_event(TIER_SWITCH_CHANGE, (uint16_t)( (tou.curr_tier_sum << 0)|(tou.curr_tier_dem << 8) ), true, user_attempt, rtc->utc_timestamp);
		}
	}
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vLoad_day_tier_switchpoints(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vLoad_day_tier_switchpoints(void) {
	
	uint16_t i, j;
	uint8_t switch_hour, switch_min;
	uint32_t timestamp;
	uint32_t switchpoint_sec;
	RTC_TYPE rtc_temp;
	TIER_SWITCH_INFO_TYPE aux;

	if (rtc.is_time_relative == 0) {
		
		memset( &rtc_temp, 0, sizeof(rtc_temp) );
		RTC_APP_vBreak_timestamp(&rtc_temp, rtc.timestamp);		
		rtc_temp.htime.hour = 0;							
		rtc_temp.htime.minute = 0;							
		rtc_temp.htime.second = 0;							
		timestamp = RTC_APP_ulMake_timestamp( &rtc_temp.htime );	
	
		tou.nbr_day_tiers = 0;
		memset( tou.day_tier_switches, 0, sizeof(tou.day_tier_switches) );
	
		if ( tou.curr_special_sched_active == (uint8_t)(NO_SPECIAL_SCHED_ACTIVE) ) {
			
			switch ( rtc.day_of_week ) {
				
				case SUNDAY:
					//tou.curr_day_sched = st_54.daily_schedule_id_matrix[tou.curr_season].sunday_schedule;
					EEPROM_reader( ST_54_DAILY_SCHEDULE_ID_MATRIX_ADDR + 5*(tou.curr_season) + 1, &tou.curr_day_sched, 1 );
					break;
			
				case SATURDAY:
					//tou.curr_day_sched = st_54.daily_schedule_id_matrix[tou.curr_season].saturday_schedule;
					EEPROM_reader( ST_54_DAILY_SCHEDULE_ID_MATRIX_ADDR + 5*(tou.curr_season) + 0, &tou.curr_day_sched, 1 );
					break;
			
				case MONDAY:
				case TUESDAY:
				case WEDNESDAY:
				case THURSDAY:
				case FRIDAY:
					//tou.curr_day_sched = st_54.daily_schedule_id_matrix[tou.curr_season].weekday_schedule;
					EEPROM_reader( ST_54_DAILY_SCHEDULE_ID_MATRIX_ADDR + 5*(tou.curr_season) + 2, &tou.curr_day_sched, 1 );
					break;
				
				default:
					break;
			}
		}
		else {
			//tou.curr_day_sched = st_54.daily_schedule_id_matrix[tou.curr_season].special_schedule[tou.curr_special_sched_active];
			EEPROM_reader( ST_54_DAILY_SCHEDULE_ID_MATRIX_ADDR + 5*(tou.curr_season) + 3 + (tou.curr_special_sched_active), &tou.curr_day_sched, 1 );
		}
	
		for ( i = 0; i < st_51.nbr_tier_switches; i++ ) {
			
			uint8_t day_sch_num;
			EEPROM_reader( ST_54_TIER_SWITCHES_ADDR + 3*i + 2, &day_sch_num, 1 );
			
			if ( tou.curr_day_sched == day_sch_num ) {
				
				uint16_t tier_switch;
				EEPROM_reader( ST_54_TIER_SWITCHES_ADDR + 3*i + 0, &tier_switch, 2 );
				
				tou.day_tier_switches[tou.nbr_day_tiers].tier_id = (tier_switch & NEW_TIER_bm) >> NEW_TIER_bp;
			
				if ( (st_51.time_func_flag2 & SEPARATE_SUM_DEMANDS_FLAG_bm) == SEPARATE_SUM_DEMANDS_FLAG_bm ) {
					
					tou.day_tier_switches[tou.nbr_day_tiers].summ_switch_flag = (tier_switch & SUMMATION_SWITCH_FLAG_bm) >> SUMMATION_SWITCH_FLAG_bp;
					tou.day_tier_switches[tou.nbr_day_tiers].demand_switch_flag = (tier_switch & DEMANDS_SWITCH_FLAG_bm) >> DEMANDS_SWITCH_FLAG_bp;
				}
			
				switch_hour = (tier_switch & SWITCH_HOUR_bm) >> SWITCH_HOUR_bp;
				switch_min = (tier_switch & SWITCH_MIN_bm) >> SWITCH_MIN_bp;
			
				switchpoint_sec = switch_hour * SECONDS_PER_HOUR;
				switchpoint_sec += switch_min * SECONDS_PER_MINUTE;
			
				tou.day_tier_switches[tou.nbr_day_tiers].tier_switch = timestamp + switchpoint_sec;
			
				tou.nbr_day_tiers++;
			}
		}
	
		/* Organize tier switch points ascending */
		for (i = 0; i < tou.nbr_day_tiers; i++) {
			
			for (j = 0; j < tou.nbr_day_tiers; j++) {
				
				if (tou.day_tier_switches[i].tier_switch < tou.day_tier_switches[j].tier_switch) {
					
					aux = tou.day_tier_switches[i];
					tou.day_tier_switches[i] = tou.day_tier_switches[j];
					tou.day_tier_switches[j] = aux;
				}
			}
		}
	}
}

/** 
 ******************************************************************************
 * \fn uint32_t TOU_APP_ulGet_last_tier_change(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
uint32_t TOU_APP_ulGet_last_tier_change(void) {
	
	uint8_t i;
	uint32_t last_tier_change = 0;
	
	for ( i = 0; i < tou.nbr_day_tiers; i++ ) {
		
		if ( tou.day_tier_switches[i].tier_id == tou.curr_tier ) {
			
			last_tier_change = tou.day_tier_switches[i].tier_switch;
			break;
		}
	}
	
	return last_tier_change;
}

/** 
 ******************************************************************************
 * \fn uint8_t TOU_APP_ucGet_demand_and_self_read_flags(RTC_TYPE *rtc)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t TOU_APP_ucGet_demand_and_self_read_flags(RTC_TYPE *rtc) {
		
	uint8_t calendar_action = 0;
	
	if (rtc->is_time_relative == 0) {	
		
		if ( (rtc->htime.year >= 1990) && ( rtc->htime.year <= 2089) ) {
			static uint8_t i;
			static HUMAN_TIME_TYPE date_found;
			static uint16_t st_54_date;
			
			for ( i = 0; i < st_51.nbr_non_recurr_dates; i++) {
				
				EEPROM_reader( ST_54_NON_RECURR_DATES_ADDR + 3*i, &st_54_date, 2 );
				TOU_APP_vLoad_st_54_date_to_human_date(&date_found, rtc, st_54_date, false);
				
				if ( (date_found.year == rtc->htime.year) && (date_found.month == rtc->htime.month) && (date_found.day == rtc->htime.day) ) {
					
					EEPROM_reader( ST_54_NON_RECURR_DATES_ADDR + 3*i + 2, &calendar_action, 1 );
				}
			}
			
			for ( i = 0; i < st_51.nbr_recurr_dates; i++) {
				
				EEPROM_reader( ST_54_RECURR_DATES_ADDR + 3*i, &st_54_date, 2 );
				TOU_APP_vLoad_st_54_date_to_human_date(&date_found, rtc, st_54_date, true);
				
				if ( (date_found.year == rtc->htime.year) && (date_found.month == rtc->htime.month) && (date_found.day == rtc->htime.day) ) {
					
					EEPROM_reader( ST_54_RECURR_DATES_ADDR + 3*i + 2, &calendar_action, 1 );
				}
			}
			
			if ( (rtc->htime.hour != 0) || (rtc->htime.minute != 0) || (rtc->htime.second != 0) ) {
				
				calendar_action = ~(uint8_t)(DEMAND_RESET_FLAG_bm | SELF_READ_FLAG_bm);
			}
		}
	}
	
	return calendar_action;
}

/** 
 ******************************************************************************
 * \fn uint8_t TOU_APP_ucGet_special_sched_active(RTC_TYPE *rtc, TOU_TYPE *tou)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t TOU_APP_ucGet_special_sched_active(RTC_TYPE *rtc) {
		
	uint8_t special_sched_active = NO_SPECIAL_SCHED_ACTIVE;
	
	if (rtc->is_time_relative == 0) {
		
		if ( (rtc->htime.year >= 1990) && ( rtc->htime.year <= 2089) ) {
			
			static uint8_t i;
			static HUMAN_TIME_TYPE date_found;
			static uint8_t calendar_action;
			static uint8_t calendar_ctrl;
			static uint16_t st_54_date;
			
			for ( i = 0; i < st_51.nbr_non_recurr_dates; i++) {
				
				EEPROM_reader( ST_54_NON_RECURR_DATES_ADDR + 3*i, &st_54_date, 2 );
				TOU_APP_vLoad_st_54_date_to_human_date(&date_found, rtc, st_54_date, false);
				
				if ( (date_found.year == rtc->htime.year) && (date_found.month == rtc->htime.month) && (date_found.day == rtc->htime.day) ) {
					
					EEPROM_reader( ST_54_NON_RECURR_DATES_ADDR + 3*i + 2, &calendar_action, 1 );
					calendar_ctrl = (calendar_action & CALENDAR_CTRL_bm) >> CALENDAR_CTRL_bp;
					
					if ( (calendar_ctrl >= SPECIAL_SCHEDULE_0) && (calendar_ctrl <= SPECIAL_SCHEDULE_11) ) {
						
						special_sched_active = calendar_ctrl - SPECIAL_SCHEDULE_0;
					}
				}
			}
			
			for ( i = 0; i < st_51.nbr_recurr_dates; i++) {
				
				EEPROM_reader( ST_54_RECURR_DATES_ADDR + 3*i, &st_54_date, 2 );
				TOU_APP_vLoad_st_54_date_to_human_date(&date_found, rtc, st_54_date, true);
				
				if ( (date_found.year == rtc->htime.year) && (date_found.month == rtc->htime.month) && (date_found.day == rtc->htime.day) ) {
					
					EEPROM_reader( ST_54_RECURR_DATES_ADDR + 3*i + 2, &calendar_action, 1 );
					calendar_ctrl = (calendar_action & CALENDAR_CTRL_bm) >> CALENDAR_CTRL_bp;
					
					if ( (calendar_ctrl >= SPECIAL_SCHEDULE_0) && (calendar_ctrl <= SPECIAL_SCHEDULE_11) ) {
						
						special_sched_active = calendar_ctrl - SPECIAL_SCHEDULE_0;
					}
				}
			}
		}
	}
	
	return special_sched_active;
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vLoad_year_season_dates(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vLoad_year_season_dates(void) {
	
	uint8_t i, j;
	uint8_t calendar_ctrl, calendar_action;
	uint16_t season_date;
	HUMAN_TIME_TYPE date;
	SEASON_INFO_TYPE aux;
	
	tou.nbr_year_seasons = 0;
	
	if (rtc.is_time_relative == 0) {
		
		for ( i = 0; i < st_51.nbr_recurr_dates; i++) {
			
			EEPROM_reader( ST_54_RECURR_DATES_ADDR + 3*i + 2, &calendar_action, 1 );
			calendar_ctrl = (calendar_action & CALENDAR_CTRL_bm) >> CALENDAR_CTRL_bp;
		
			if ( (calendar_ctrl >= SELECT_SEASON_0) && (calendar_ctrl <= SELECT_SEASON_15) ) {
				
				EEPROM_reader( ST_54_RECURR_DATES_ADDR + 3*i, &season_date, 2 );
				TOU_APP_vLoad_st_54_date_to_human_date(&date, &rtc, season_date, true);
			
				tou.year_season_switches[tou.nbr_year_seasons].season_id = calendar_ctrl - 3; 
				tou.year_season_switches[tou.nbr_year_seasons].season_switch = RTC_APP_ulMake_timestamp(&date);
			
				tou.nbr_year_seasons++;
			}
		}
		
		for ( i = 0; i < st_51.nbr_non_recurr_dates; i++) {
			
			EEPROM_reader( ST_54_NON_RECURR_DATES_ADDR + 3*i + 2, &calendar_action, 1 );
			calendar_ctrl = (calendar_action & CALENDAR_CTRL_bm) >> CALENDAR_CTRL_bp;
		
			if ( (calendar_ctrl >= SELECT_SEASON_0) && (calendar_ctrl <= SELECT_SEASON_15) ) {
				
				EEPROM_reader( ST_54_NON_RECURR_DATES_ADDR + 3*i, &season_date, 2 );
				TOU_APP_vLoad_st_54_date_to_human_date(&date, &rtc, season_date, false);
			
				tou.year_season_switches[tou.nbr_year_seasons].season_id = calendar_ctrl - 3; 
				tou.year_season_switches[tou.nbr_year_seasons].season_switch = RTC_APP_ulMake_timestamp(&date);
			
				tou.nbr_year_seasons++;
			}
		}
	
		/* Organize season switch points ascending */
		for (i = 0; i < tou.nbr_year_seasons; i++) {
			
			for (j = 0; j < tou.nbr_year_seasons; j++) {
				
				if (tou.year_season_switches[i].season_switch < tou.year_season_switches[j].season_switch) {
					
					aux = tou.year_season_switches[i];
					tou.year_season_switches[i] = tou.year_season_switches[j];
					tou.year_season_switches[j] = aux;
				}
			}
		}
	}
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vUpdate_current_season(RTC_TYPE *rtc)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vUpdate_current_season(RTC_TYPE *rtc) {
	
	uint8_t i;
	SEASON_NBR_TYPE season;
	
	if (rtc->is_time_relative == 0) {
		
		if ( (rtc->timestamp < tou.year_season_switches[0].season_switch) || (rtc->timestamp >= tou.year_season_switches[tou.nbr_year_seasons - 1].season_switch) ) {
			
			season = tou.year_season_switches[tou.nbr_year_seasons - 1].season_id;
		}
		else {
			
			for ( i = 0; i < tou.nbr_year_seasons; i++ ) {
				
				if ( rtc->timestamp >= tou.year_season_switches[i].season_switch ) {
					season = tou.year_season_switches[i].season_id;
				}
			}
		}
		
		if ( (uint8_t)season != tou.curr_season ) {
			
			tou.pending_season_change = 1;
			TOU_APP_vWrite_eeprom_current_season();
			tou.curr_season = (uint8_t)season;
			
			EVNT_APP_vRegister_event(SEASON_CHANGES, season, true, END_DEVICE, rtc->utc_timestamp);
		}
	}
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vWrite_eeprom_current_season(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vWrite_eeprom_current_season(void) {
	
	EEPROM_writer(TOU_LAST_SEASON_ADDR, &tou.curr_season, 1);
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vRead_eeprom_current_season(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vRead_eeprom_current_season(void) {
	
	EEPROM_reader(TOU_LAST_SEASON_ADDR, &tou.curr_season, 1);
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vSeason_change(uint16_t user_attempt, uint8_t season, uint32_t timestamp)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vSeason_change(uint16_t user_attempt, uint8_t season, uint32_t timestamp) {
	
	REGS_APP_vTake_snapshot_st_23((uint8_t)SEASON_CHANGE, season, timestamp);
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vActivate_special_sched(SPECIAL_SCHED_ID_TYPE sched_id, uint16_t user, RTC_TYPE *rtc)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vActivate_special_sched(SPECIAL_SCHED_ID_TYPE sched_id, uint16_t user, RTC_TYPE *rtc) {
	
	if ( (uint8_t)sched_id != tou.curr_special_sched_active ) {
		
		tou.curr_special_sched_active = (uint8_t)sched_id;
		EVNT_APP_vRegister_event(SPECIAL_SCHEDULE_ACTIVATION, (uint16_t)sched_id, true, user, rtc->utc_timestamp);
	}
}

/** 
 ******************************************************************************
 * \fn void TOU_APP_vLoad_st_54_date_to_human_date(HUMAN_TIME_TYPE *pHuman_date, RTC_TYPE *rtc, uint16_t st_54_date, bool is_recurrent)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void TOU_APP_vLoad_st_54_date_to_human_date(HUMAN_TIME_TYPE *pHuman_date, RTC_TYPE *rtc, uint16_t st_54_date, bool is_recurrent) {
	
	HUMAN_TIME_TYPE human_date_temp;
	
	if (is_recurrent == true) {
		
		uint8_t month, offset, weekday, day_of_week;
		bool leap_year;
		
		month = (st_54_date & RDATE_MONTH_bm) >> RDATE_MONTH_bp;
		leap_year = RTC_APP_bIs_leap_year(rtc->htime.year);
		
		switch (month) {
			case 0:
			break;
			
			case JANUARY:
			case FEBRUARY:
			case MARCH:
			case APRIL:
			case MAY:
			case JUNE:
			case JULY:
			case AUGUST:
			case SEPTEMBER:
			case OCTOBER:
			case NOVEMBER:
			case DECEMBER:
			case 13:
			offset = (st_54_date & RDATE_OFFSET_bm) >> RDATE_OFFSET_bp;
			weekday = (st_54_date & RDATE_WEEKDAY_bm) >> RDATE_WEEKDAY_bp;
			
			human_date_temp.year = rtc->htime.year;
			human_date_temp.month = month;
			human_date_temp.day = (st_54_date & RDATE_DAY_bm) >> RDATE_DAY_bp;
			
			if (month == 13) {
				human_date_temp.month = rtc->htime.month;
			}
			
			switch (offset) {
				
				case 0:		// No offset
					break;
				
				case 1:		// Advance to WEEKDAY before date entered.
					break;
				
				case 2:		// Postpone to the first WEEKDAY on or after date entered.
					human_date_temp = TOU_APP_xGet_postponed_date( 1, weekday, human_date_temp );
					break;
				
				case 3:		// Postpone to the second WEEKDAY on or after date entered.
					human_date_temp = TOU_APP_xGet_postponed_date( 2, weekday, human_date_temp );
					break;
				
				case 4:		// Postpone to the third WEEKDAY on or after date entered.
					human_date_temp = TOU_APP_xGet_postponed_date( 3, weekday, human_date_temp );
					break;
				
				case 5:		// Postpone to the fourth WEEKDAY on or after date entered.
					human_date_temp = TOU_APP_xGet_postponed_date( 4, weekday, human_date_temp );
					break;
				
				case 6:		// Postpone to the last WEEKDAY of the MONTH on or after date entered.
					break;
				
				case 7:		// Observe on date entered as well as day following date entered.
					break;
				
				case 8:		// Postpone to Monday if Sunday.
					day_of_week = RTC_APP_ucDay_of_week(human_date_temp.year, human_date_temp.month, human_date_temp.day);
					
					if ( day_of_week == SUNDAY ) {
						human_date_temp = TOU_APP_xGet_postponed_date( 1, MONDAY, human_date_temp );
					}
					break;
				
				case 9:		// Advance to Friday if Sunday.
					break;
				
				case 10:	// Postpone to Monday if Saturday.
					break;
				
				case 11:	// Advance to Friday if Saturday.
					break;
				
				case 12:	// Postpone to Monday if Sunday or Saturday.
					break;
				
				case 13:	// Advance to Friday if Sunday or Saturday.
					break;
				
				case 14:	// Postpone to Monday if Sunday, advance to Friday if Saturday.
					break;
				
				case 15:	// Do not observe date entered. Observe on day following date entered.
					human_date_temp.day++;
					
					if (human_date_temp.day > month_days[leap_year][month]) {
						human_date_temp.day = 0;
						human_date_temp.month++;
					
						if ( human_date_temp.month > 12 ) {
							human_date_temp.month = 0;
							human_date_temp.year++;
						}
					}
					break;
			}
			break;
			
			case 14:
				break;
			
			case 15:
				break;
		}
		
		human_date_temp.hour	= 0;
		human_date_temp.minute = 0;
		human_date_temp.second = 0;
	}
	else {
		
		memset( &human_date_temp, 0, sizeof(human_date_temp) );
		human_date_temp.year = (st_54_date & DATE_YEAR_bm) >> DATE_YEAR_bp;
		
		if ( (human_date_temp.year >= 0) && (human_date_temp.year <= 99) ) {
			
			if ( (human_date_temp.year >= 0) && (human_date_temp.year <= 89) ) {
				human_date_temp.year += 2000;
			}
			else if ( (human_date_temp.year >= 90) && (human_date_temp.year <= 99) ) {
				human_date_temp.year += 1900;
			}
			
			human_date_temp.month = (st_54_date & DATE_MONTH_bm) >> DATE_MONTH_bp;
			human_date_temp.day = (st_54_date & DATE_DAY_bm) >> DATE_DAY_bp;
		}
	}
	
	pHuman_date->year = human_date_temp.year;
	pHuman_date->month = human_date_temp.month;
	pHuman_date->day = human_date_temp.day;
	pHuman_date->hour = human_date_temp.hour;
	pHuman_date->minute = human_date_temp.minute;
	pHuman_date->second = human_date_temp.second;
}

/** 
 ******************************************************************************
 * \fn HUMAN_TIME_TYPE TOU_APP_xGet_postponed_date(uint8_t nbr_weekday_after, uint8_t weekday, HUMAN_TIME_TYPE date)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
HUMAN_TIME_TYPE TOU_APP_xGet_postponed_date(uint8_t nbr_weekday_after, uint8_t weekday, HUMAN_TIME_TYPE date) {
	
	uint8_t i;
	uint8_t day_of_week;
	uint8_t weekday_count = 0;
	HUMAN_TIME_TYPE postponedDate;
	bool leap_year;
	uint8_t days_in_month;
	
	postponedDate = date;
	leap_year = RTC_APP_bIs_leap_year(postponedDate.year);
	
	for ( i = 0; i < 31; i++ ) {
		
		days_in_month = month_days[leap_year][postponedDate.month];
		day_of_week = RTC_APP_ucDay_of_week(postponedDate.year, postponedDate.month, postponedDate.day);
	
		if ( day_of_week == weekday ) {
			
			weekday_count++;
			
			if ( weekday_count == nbr_weekday_after ) {
				break;
			}
			else {
				postponedDate.day++;
			}
		}
		else {
			postponedDate.day++;
		}
		
		if (postponedDate.day > days_in_month) {
			postponedDate.day = 0;
			postponedDate.month++;
			
			if (postponedDate.month > 12){
				postponedDate.month = 1;
				postponedDate.year++;
			}
		}
	}
	
	return postponedDate;
}

/**
 ******************************************************************************
 * \fn uint8_t TOU_APP_ucLoad_dst_dates(RTC_TYPE* rtc)
 * \brief Load time zone amount and dst dates
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
uint8_t TOU_APP_ucLoad_dst_dates(RTC_TYPE* rtc) {
	
	uint8_t i;
	uint8_t dst_found;
	uint8_t calendar_action, calendar_ctrl;
	uint16_t st_54_dst;
	HUMAN_TIME_TYPE dst_date;
	uint8_t dst_ok;

	rtc->is_dst = 0;
	dst_found = 0;
	dst_ok = 0;
	
	EEPROM_reader( ST_53_DST_TIME_EFF_ADDR, &rtc->dst_to_effective, TM_FMAT_SIZE );
	rtc->dst_from_effective = rtc->dst_to_effective;
	//EEPROM_reader( ST_53_STD_TIME_EFF_ADDR, &rtc->dst_from_effective, TM_FMAT_SIZE );
	
	// Search dst dates in all recurrent dates 
	for ( i = 0; i < st_51.nbr_recurr_dates; i++) {
		
		EEPROM_reader( ST_54_RECURR_DATES_ADDR + 3*i + 2, &calendar_action, 1 );
		calendar_ctrl = (calendar_action & CALENDAR_CTRL_bm) >> CALENDAR_CTRL_bp;
		
		if ( (calendar_ctrl == DAYLIGHT_SAVING_TIME_ON) && ((dst_found & (uint8_t)0x01) == 0) ) {
			
			EEPROM_reader( ST_54_RECURR_DATES_ADDR + 3*i, &st_54_dst, 2 );
			
			TOU_APP_vLoad_st_54_date_to_human_date(&dst_date, rtc, st_54_dst, true);		
			rtc->dst_to_effective += RTC_APP_ulMake_timestamp(&dst_date);
			dst_found |= 0x01;
		}
		
		if ( (calendar_ctrl == DAYLIGHT_SAVING_TIME_OFF) && ((dst_found & (uint8_t)0x02) == 0) ) {
			
			EEPROM_reader( ST_54_RECURR_DATES_ADDR + 3*i, &st_54_dst, 2 );
			
			TOU_APP_vLoad_st_54_date_to_human_date(&dst_date, rtc, st_54_dst, true);	
			rtc->dst_from_effective += RTC_APP_ulMake_timestamp(&dst_date);
			dst_found |= 0x02;
		}
		
		if (dst_found == 0x03) {
			
			dst_ok = 1;
			break;
		}
	}
	
	// if dst dates were not found in "recurrent dates"
	if (dst_ok == 0) {
		
		dst_found = 0;
		
		for ( i = 0; i < st_51.nbr_non_recurr_dates; i++) {
			
			EEPROM_reader( ST_54_NON_RECURR_DATES_ADDR + 3*i + 2, &calendar_action, 1 );
			calendar_ctrl = (calendar_action & CALENDAR_CTRL_bm) >> CALENDAR_CTRL_bp;
			
			if ( (calendar_ctrl == DAYLIGHT_SAVING_TIME_ON) && ((dst_found & (uint8_t)0x01) == 0) ) {
				
				EEPROM_reader( ST_54_NON_RECURR_DATES_ADDR + 3*i, &st_54_dst, 2 );
				
				TOU_APP_vLoad_st_54_date_to_human_date(&dst_date, rtc, st_54_dst, false);
				
				if (dst_date.year == rtc->htime.year) {
					
					rtc->dst_to_effective += RTC_APP_ulMake_timestamp(&dst_date);
					dst_found |= 0x01;
				}
			}
			
			if ( (calendar_ctrl == DAYLIGHT_SAVING_TIME_OFF) && ((dst_found & (uint8_t)0x02) == 0) ) {
				EEPROM_reader( ST_54_NON_RECURR_DATES_ADDR + 3*i, &st_54_dst, 2 );
				
				TOU_APP_vLoad_st_54_date_to_human_date(&dst_date, rtc, st_54_dst, false);
				
				if (dst_date.year == rtc->htime.year) {
					
					rtc->dst_from_effective += RTC_APP_ulMake_timestamp(&dst_date);
					dst_found |= 0x02;
				}
			}
			
			if (dst_found == 0x03) {
				
				dst_ok = 1;
				break;
			}
		}
	}
	
	return dst_ok;
}

/**
 ******************************************************************************
 * \fn uint8_t TOU_APP_ucGet_current_season(void)
 * \brief 
 *
 * \param
 * \retval
 ******************************************************************************
 */
uint8_t TOU_APP_ucGet_current_season(void) {
	
	return tou.curr_season;
}

/**
 ******************************************************************************
 * \fn uint8_t TOU_APP_ucGet_current_tier(void)
 * \brief 
 *
 * \param
 * \retval
 ******************************************************************************
 */
uint8_t TOU_APP_ucGet_current_tier(void) {
	
	return tou.curr_tier;
}

/**
 ******************************************************************************
 * \fn uint8_t TOU_APP_ucGet_current_tier_dem(void)
 * \brief 
 *
 * \param
 * \retval
 ******************************************************************************
 */
uint8_t TOU_APP_ucGet_current_tier_dem(void) {
	
	return tou.curr_tier_dem;
}

/**
 ******************************************************************************
 * \fn uint8_t TOU_APP_ucGet_current_tier_sum(void)
 * \brief 
 *
 * \param
 * \retval
 ******************************************************************************
 */
uint8_t TOU_APP_ucGet_current_tier_sum(void) {
	
	return tou.curr_tier_sum;
}

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-08-13
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */