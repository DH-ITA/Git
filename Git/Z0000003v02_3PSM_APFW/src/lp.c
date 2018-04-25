/**
 ******************************************************************************
 * \file lp.c
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
#include "lp.h"
#include "timer.h"
#include "regs.h"
#include "rtc_driver.h"
#include "eeprom.h"
#include "watchdog.h"

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
static uint64_t summ_integer[MAX_NBR_CHNS_SET1];
static uint64_t summ_remaind[MAX_NBR_CHNS_SET1];

/*
 ******************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ******************************************************************************
 */
ST_60_TYPE st_61;
ST_62_TYPE st_62;
ST_63_TYPE st_63;
ST_64_ADDR_TYPE st_64_addr;
LP_STATUS lp;

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
static bool LP_APP_bSt_61_validate_data(ST_60_TYPE *st_61_structure);
static void LP_APP_vSt_61_read_eeprom_values(ST_60_TYPE *st_61_structure);
static void LP_APP_vSt_61_write_eeprom_values(ST_60_TYPE *st_61_structure);
static void LP_APP_vSt_61_to_array(ST_60_TYPE st_61_source, uint8_t *table_buffer);

static bool LP_APP_bSt_62_validate_data(ST_62_TYPE *st_62_structure);
static void LP_APP_vSt_62_read_eeprom_values(ST_62_TYPE *st_62_structure);
static void LP_APP_vSt_62_write_eeprom_values(ST_62_TYPE *st_62_structure);
static void LP_APP_vSt_62_to_array(ST_62_TYPE st_62_source, uint8_t *table_buffer);

static bool LP_APP_bSt_63_validate_data(ST_63_TYPE *st_63_structure);
static void LP_APP_vSt_63_read_eeprom_values(ST_63_TYPE *st_63_structure);
static void LP_APP_vSt_63_write_eeprom_values(ST_63_TYPE *st_63_structure);

static void LP_APP_vWrite_eeprom_pulse_recorder(void);
static void LP_APP_vRead_eeprom_pulse_recorder(void);
static uint16_t LP_APP_usGet_block_size(void);

static void LP_APP_vChange_extended_int_channel_status(uint8_t* extended_int_status_array, uint8_t channel_number,  uint8_t channel_status);

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */
/**
 ******************************************************************************
 * \fn void LP_APP_vInitialize(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vInitialize(void) {

	lp.is_active = false;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_60_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_60_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	
	table_buffer[k++] = (uint8_t)(MAX_LP_MEMORY_LEN);
	table_buffer[k++] = (uint8_t)(MAX_LP_MEMORY_LEN >> 8);
	table_buffer[k++] = (uint8_t)(MAX_LP_MEMORY_LEN >> 16);
	table_buffer[k++] = (uint8_t)(MAX_LP_MEMORY_LEN >> 24);
	table_buffer[k++] = (uint8_t)(PERM_LP_FLAGS);
	table_buffer[k++] = (uint8_t)(PERM_LP_FLAGS >> 8);
	table_buffer[k++] = (uint8_t)(PERM_LP_FMATS);
	table_buffer[k++] = (uint8_t)(st_61.nbr_blks_set1);
	table_buffer[k++] = (uint8_t)(st_61.nbr_blks_set1 >> 8);
	table_buffer[k++] = (uint8_t)(MAX_NBR_BLK_INTS_SET1);
	table_buffer[k++] = (uint8_t)(MAX_NBR_BLK_INTS_SET1 >> 8);
	table_buffer[k++] = (uint8_t)(MAX_NBR_CHNS_SET1);
	table_buffer[k++] = (uint8_t)(MAX_MAX_INT_TIME_SET1);
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t LP_APP_usSt_60_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t LP_APP_usSt_60_get_size(void) {

	return (uint16_t) ST_60_SIZE;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bLoad_st_61(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bLoad_st_61(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_61_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = LP_APP_bSt_61_restore_default_values();
	}
	else {
		
		is_ok = LP_APP_bSt_61_refresh_ram();
	}
	
	st_61.nbr_blk_ints_set1 = DEFAULT_NBR_BLK_INTS_SET1;
	LP_APP_ulLoad_nbr_blocks();
	st_61.lp_memory_len = st_61.nbr_blks_set1 * LP_APP_usGet_block_size();
	LP_APP_vSt_61_write_eeprom_values(&st_61);

	uint16_t crc = SM_APP_usSt_61_ram_calculate_crc();
	SM_APP_vSt_61_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_61_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_61_restore_default_values(void) {
	
	bool is_ok = false;
	ST_60_TYPE st_61_temp;
	
	st_61_temp.lp_flags = DEFAULT_LP_FLAGS;
	st_61_temp.lp_fmats = DEFAULT_LP_FMATS;
	st_61_temp.nbr_blk_ints_set1 = DEFAULT_NBR_BLK_INTS_SET1;
	st_61_temp.nbr_chns_set1 =  DEFAULT_NBR_CHNS_SET1;
	st_61_temp.max_int_time_set1 = DEFAULT_MAX_INT_TIME_SET1;
	
	is_ok = LP_APP_bSt_61_validate_data(&st_61_temp);
	
	st_61 = st_61_temp;
	st_61.nbr_blk_ints_set1 = DEFAULT_NBR_BLK_INTS_SET1;
	LP_APP_ulLoad_nbr_blocks();
	st_61.lp_memory_len = st_61.nbr_blks_set1 * LP_APP_usGet_block_size();
	LP_APP_vSt_61_write_eeprom_values(&st_61);
	uint16_t crc = SM_APP_usSt_61_ram_calculate_crc();
	SM_APP_vSt_61_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_61_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_61_refresh_ram(void) {
	
	ST_60_TYPE st_61_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		LP_APP_vSt_61_read_eeprom_values(&st_61_temp);
		is_ok = LP_APP_bSt_61_validate_data(&st_61_temp);
		
		if ( is_ok ) {
			st_61 = st_61_temp;
			st_61.nbr_blk_ints_set1 = DEFAULT_NBR_BLK_INTS_SET1;
			LP_APP_ulLoad_nbr_blocks();
			st_61.lp_memory_len = st_61.nbr_blks_set1 * LP_APP_usGet_block_size();
			break;
		}
	}
	
	if ( !is_ok ) {
		st_61 = st_61_temp;
		st_61.nbr_blk_ints_set1 = DEFAULT_NBR_BLK_INTS_SET1;
		LP_APP_ulLoad_nbr_blocks();
		st_61.lp_memory_len = st_61.nbr_blks_set1 * LP_APP_usGet_block_size();
		LP_APP_vSt_61_write_eeprom_values(&st_61);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  LP_APP_vCopy_st_61_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vCopy_st_61_ram(uint8_t* destination) {
	
	LP_APP_vSt_61_to_array(st_61, &destination[0]);
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
void LP_APP_vCopy_st_61_nvm(uint8_t* destination) {
	
	ST_60_TYPE st_61_aux;
	
	LP_APP_vSt_61_read_eeprom_values(&st_61_aux);
	LP_APP_vSt_61_to_array(st_61_aux, &destination[0]);
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
void LP_APP_vSt_61_recover_nvm(void) {
	
	LP_APP_vSt_61_write_eeprom_values(&st_61);
}

/**
 ******************************************************************************
 * \fn void LP_APP_vSt_61_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vSt_61_recover_ram(void) {
	
	LP_APP_vSt_61_read_eeprom_values(&st_61);
}

/**
 ******************************************************************************
 * \fn void LP_APP_vSt_61_to_array(ST_60_TYPE st_61_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vSt_61_to_array(ST_60_TYPE st_61_source, uint8_t *table_buffer) {
	
	table_buffer[0] = (uint8_t)(st_61_source.lp_memory_len);
	table_buffer[1] = (uint8_t)(st_61_source.lp_memory_len >> 8);
	table_buffer[2] = (uint8_t)(st_61_source.lp_memory_len >> 16);
	table_buffer[3] = (uint8_t)(st_61_source.lp_memory_len >> 24);
	table_buffer[4] = (uint8_t)(st_61_source.lp_flags);
	table_buffer[5] = (uint8_t)(st_61_source.lp_flags >> 8);
	table_buffer[6] = st_61_source.lp_fmats;
	table_buffer[7] = (uint8_t)(st_61_source.nbr_blks_set1);
	table_buffer[8] = (uint8_t)(st_61_source.nbr_blks_set1 >> 8);
	table_buffer[9] = (uint8_t)(st_61_source.nbr_blk_ints_set1);
	table_buffer[10] = (uint8_t)(st_61_source.nbr_blk_ints_set1 >> 8);
	table_buffer[11] = st_61_source.nbr_chns_set1;
	table_buffer[12] = st_61_source.max_int_time_set1;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_61_validate_data(ST_60_TYPE *st_61_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_61_validate_data(ST_60_TYPE *st_61_structure) {
	
	bool is_ok = true;
	uint8_t i;
	
	LP_APP_ulLoad_nbr_blocks();
	if ( st_61_structure->lp_memory_len > MAX_LP_MEMORY_LEN )
	{
		st_61_structure->lp_memory_len = st_61_structure->nbr_blks_set1 * LP_APP_usGet_block_size();
		is_ok = false;
	}
	
	for ( i = 0; i < 16; i++)
	{
		if ( ( ( (PERM_LP_FLAGS >> i) & 0x01 ) == 0 ) && ( ( (st_61_structure->lp_flags >> i) & 0x01 ) == 1 ) )
		{
			is_ok = false;	
		}
	}
	st_61_structure->lp_flags &= PERM_LP_FLAGS;
	
	for ( i = 0; i < 8; i++)
	{
		if ( ( ( (PERM_LP_FMATS >> i) & 0x01 ) == 0 ) && ( ( (st_61_structure->lp_fmats >> i) & 0x01 ) == 1 ) )
		{
			is_ok = false;
		}
	}
	st_61_structure->lp_fmats &= PERM_LP_FMATS;
	
	if ( st_61_structure->nbr_blk_ints_set1 > MAX_NBR_BLK_INTS_SET1 )
	{
		st_61_structure->nbr_blk_ints_set1 = MAX_NBR_BLK_INTS_SET1;
		is_ok = false;
	}
	
	if ( st_61_structure->nbr_chns_set1 > MAX_NBR_CHNS_SET1 )
	{
		st_61_structure->nbr_chns_set1 = MAX_NBR_CHNS_SET1;
		is_ok = false;
	}
	
	if ( (60 % st_61_structure->max_int_time_set1) == 0 )
	{
		if (st_61_structure->max_int_time_set1 < MIN_INT_TIME_SET1)
		{
			st_61_structure->max_int_time_set1 = MIN_INT_TIME_SET1;
			is_ok = false;
		}
		else if (st_61_structure->max_int_time_set1 > MAX_INT_TIME_SET1)
		{
			st_61_structure->max_int_time_set1 = MAX_INT_TIME_SET1;
			is_ok = false;
		}
	}
	else
	{
		st_61_structure->max_int_time_set1 = DEFAULT_MAX_INT_TIME_SET1;
		is_ok = false;
	}
		
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void LP_APP_vSt_61_read_eeprom_values(ST_60_TYPE *st_61_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vSt_61_read_eeprom_values(ST_60_TYPE *st_61_structure) {
		
	EEPROM_reader( ST_61_LP_MEMORY_LEN_ADDR, &st_61_structure->lp_memory_len, 4 );
	EEPROM_reader( ST_61_LP_FLAGS_ADDR, &st_61_structure->lp_flags, 2 );
	EEPROM_reader( ST_61_LP_FMATS_ADDR, &st_61_structure->lp_fmats, 1 );
	EEPROM_reader( ST_61_NBR_BLKS_SET1_ADDR, &st_61_structure->nbr_blks_set1, 2 );
	EEPROM_reader( ST_61_NBR_BLK_INTS_SET1_ADDR, &st_61_structure->nbr_blk_ints_set1, 2 );
	EEPROM_reader( ST_61_NBR_CHNS_SET1_ADDR, &st_61_structure->nbr_chns_set1, 1 );
	EEPROM_reader( ST_61_MAX_INT_TIME_SET1_ADDR, &st_61_structure->max_int_time_set1, 1 );
}

/**
 ******************************************************************************
 * \fn void LP_APP_vSt_61_write_eeprom_values(ST_60_TYPE *st_61_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vSt_61_write_eeprom_values(ST_60_TYPE *st_61_structure) {
	
	SM_APP_vChange_table_state(ST_61_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( ST_61_LP_MEMORY_LEN_ADDR, &st_61_structure->lp_memory_len, 4 );
	EEPROM_writer( ST_61_LP_FLAGS_ADDR, &st_61_structure->lp_flags, 2 );
	EEPROM_writer( ST_61_LP_FMATS_ADDR, &st_61_structure->lp_fmats, 1 );
	EEPROM_writer( ST_61_NBR_BLKS_SET1_ADDR, &st_61_structure->nbr_blks_set1, 2 );
	EEPROM_writer( ST_61_NBR_BLK_INTS_SET1_ADDR, &st_61_structure->nbr_blk_ints_set1, 2 );
	EEPROM_writer( ST_61_NBR_CHNS_SET1_ADDR, &st_61_structure->nbr_chns_set1, 1 );
	EEPROM_writer( ST_61_MAX_INT_TIME_SET1_ADDR, &st_61_structure->max_int_time_set1, 1 );
	
	SM_APP_vChange_table_state(ST_61_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_61_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_61_execute_table_reading(uint8_t *table_buffer) {
	
	LP_APP_vSt_61_to_array(st_61, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_61_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_61_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	ST_60_TYPE st_61_temp;
	
	st_61_temp = st_61;
	
	//  st_61_temp.lp_memory_len  = (uint32_t)(table_buffer[k++]);
	// 	st_61_temp.lp_memory_len	|= (uint32_t)(table_buffer[k++] << 8);
	// 	st_61_temp.lp_memory_len |= (uint32_t)(table_buffer[k++] << 16);
	// 	st_61_temp.lp_memory_len |= (uint32_t)(table_buffer[k++] << 24);
	k += 4;
	
	st_61_temp.lp_flags  = (uint16_t)(table_buffer[k++]);
	st_61_temp.lp_flags |= (uint16_t)(table_buffer[k++] << 8);
	
	//	st_61_temp.lp_fmats = table_buffer[k++];
	k += 1;
	
	// 	st_61_temp.nbr_blks_set1  = (uint16_t)(table_buffer[k++]);
	// 	st_61_temp.nbr_blks_set1 |= (uint16_t)(table_buffer[k++] << 8);
	k += 2;
	
	// 	st_61_temp.nbr_blk_ints_set1  = (uint16_t)(table_buffer[k++]);
	// 	st_61_temp.nbr_blk_ints_set1	|= (uint16_t)(table_buffer[k++] << 8);
	k += 2;

	st_61_temp.nbr_chns_set1 = table_buffer[k++];
	st_61_temp.max_int_time_set1 = table_buffer[k++];
	
	is_ok = LP_APP_bSt_61_validate_data(&st_61_temp);
	
	if ( is_ok )
	{
		st_61 = st_61_temp;
		st_61.nbr_blk_ints_set1 = DEFAULT_NBR_BLK_INTS_SET1;
		LP_APP_ulLoad_nbr_blocks();
		st_61.lp_memory_len = st_61.nbr_blks_set1 * LP_APP_usGet_block_size();
		LP_APP_vSt_61_write_eeprom_values(&st_61);
		uint16_t crc = SM_APP_usSt_61_ram_calculate_crc();
		SM_APP_vSt_61_write_crc(crc);
		
		LP_APP_bSt_63_restore_default_values();
		LP_APP_ulLoad_nbr_blocks();
		LP_APP_vLoad_st_64_eeprom_addr();
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t LP_APP_usSt_61_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t LP_APP_usSt_61_get_size(void) {

	return (uint16_t)ST_61_SIZE;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bLoad_st_62(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bLoad_st_62(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_62_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = LP_APP_bSt_62_restore_default_values();
	}
	else {
		
		is_ok = LP_APP_bSt_62_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_62_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_62_restore_default_values(void) {
	
	bool is_ok = false;
	ST_62_TYPE st_62_temp;
	
	memset( &st_62_temp, 0, sizeof(st_62_temp) );
	
	/* Set all selected channels with the same configuration */
	for ( uint8_t i = 0; i < st_61.nbr_chns_set1; i++ )
	{
		st_62_temp.lp_sel_set1[i].chnl_flag = (uint8_t)(END_RDG_FLAG_bm);
		st_62_temp.lp_sel_set1[i].lp_source_select = LP_WH_DEL_ID;
		st_62_temp.lp_sel_set1[i].end_blk_rdg_source_select = SM_WH_DEL_ID;
		st_62_temp.divisor_set1[i] = DEFAULT_DIVISOR_SET1;
		st_62_temp.scalars_set1[i] = DEFAULT_SCALARS_SET1;
	}
	st_62_temp.int_fmt_cde1 = INT16;
	
	is_ok = LP_APP_bSt_62_validate_data(&st_62_temp);
	
	st_62 = st_62_temp;
	LP_APP_vSt_62_write_eeprom_values(&st_62);
	uint16_t crc = SM_APP_usSt_62_ram_calculate_crc();
	SM_APP_vSt_62_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_62_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_62_refresh_ram(void) {
	
	ST_62_TYPE st_62_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		LP_APP_vSt_62_read_eeprom_values(&st_62_temp);
		is_ok = LP_APP_bSt_62_validate_data(&st_62_temp);
		
		if ( is_ok ) {
			st_62 = st_62_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_62 = st_62_temp;
		LP_APP_vSt_62_write_eeprom_values(&st_62);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  LP_APP_vCopy_st_62_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vCopy_st_62_ram(uint8_t* destination) {
	
	LP_APP_vSt_62_to_array(st_62, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void LP_APP_vCopy_st_62_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vCopy_st_62_nvm(uint8_t* destination) {
	
	ST_62_TYPE st_62_aux;
	
	LP_APP_vSt_62_read_eeprom_values(&st_62_aux);
	LP_APP_vSt_62_to_array(st_62_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void LP_APP_vSt_62_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vSt_62_recover_nvm(void) {
	
	LP_APP_vSt_62_write_eeprom_values(&st_62);
}

/**
 ******************************************************************************
 * \fn void LP_APP_vSt_62_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vSt_62_recover_ram(void) {
	
	LP_APP_vSt_62_read_eeprom_values(&st_62);
}

/**
 ******************************************************************************
 * \fn void LP_APP_vSt_62_to_array(ST_62_TYPE st_62_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vSt_62_to_array(ST_62_TYPE st_62_source, uint8_t *table_buffer) {
	
	uint8_t i, k = 0;
	
	for ( i = 0; i < st_61.nbr_chns_set1; i++ ) {
		table_buffer[k++] = st_62_source.lp_sel_set1[i].chnl_flag;
		table_buffer[k++] = st_62_source.lp_sel_set1[i].lp_source_select;
		table_buffer[k++] = st_62_source.lp_sel_set1[i].end_blk_rdg_source_select;
	}
	
	table_buffer[k++] = (uint8_t)16;
	
	for ( i = 0; i < st_61.nbr_chns_set1; i++ ) {
		convert_16_bit_to_byte_array(st_62_source.scalars_set1[i], &table_buffer[k]);
		k += 2;
	}
	
	for ( i = 0; i < st_61.nbr_chns_set1; i++ ) {
		convert_16_bit_to_byte_array(st_62_source.divisor_set1[i], &table_buffer[k]);
		k += 2;
	}
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_62_validate_data(ST_62_TYPE *st_62_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_62_validate_data(ST_62_TYPE *st_62_structure) {
	
	bool is_ok = true;
	uint8_t i;
	
	for ( uint8_t i = 0; i < st_61.nbr_chns_set1; i++ ) {
		
		st_62_structure->lp_sel_set1[i].chnl_flag = 1;
		
		if ( (st_62_structure->lp_sel_set1[i].lp_source_select < LOAD_PROFILE_INITIAL_ID) || (st_62_structure->lp_sel_set1[i].lp_source_select >= LP_NUMBER_OF_IDS) ) {
			is_ok = false;
			st_62_structure->lp_sel_set1[i].lp_source_select = LOAD_PROFILE_INITIAL_ID;			
		}
		
		if ( (st_62_structure->lp_sel_set1[i].end_blk_rdg_source_select < SUMMATIONS_INITIAL_ID) || (st_62_structure->lp_sel_set1[i].end_blk_rdg_source_select >= SM_NUMBER_OF_IDS) ) {
			is_ok = false;
			st_62_structure->lp_sel_set1[i].end_blk_rdg_source_select = SUMMATIONS_INITIAL_ID;
		}
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void LP_APP_vSt_62_read_eeprom_values(ST_62_TYPE *st_62_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vSt_62_read_eeprom_values(ST_62_TYPE *st_62_structure) {	
	
	for ( uint8_t i = 0; i < st_61.nbr_chns_set1; i++ ) {

		EEPROM_reader( ST_62_CHNL_FLAG_SET1_ADDR + i, &st_62_structure->lp_sel_set1[i].chnl_flag, 1 );
		EEPROM_reader( ST_62_LP_SOURCE_SELECT_SET1_ADDR + i, &st_62_structure->lp_sel_set1[i].lp_source_select, 1 );
		EEPROM_reader( ST_62_END_BLK_RDG_SOURCE_SELECT_SET1_ADDR + i, &st_62_structure->lp_sel_set1[i].end_blk_rdg_source_select, 1 );
		EEPROM_reader( ST_62_SCALARS_SET1_ADDR + 2*i, &st_62_structure->scalars_set1[i], 2 );
		EEPROM_reader( ST_62_DIVISOR_SET1_ADDR + 2*i, &st_62_structure->divisor_set1[i], 2 );
	}
	
	EEPROM_reader( ST_62_INT_FMT_CDE1_ADDR, &st_62_structure->int_fmt_cde1, 1 );
}

/**
 ******************************************************************************
 * \fn void LP_APP_vSt_62_write_eeprom_values(ST_62_TYPE *st_62_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vSt_62_write_eeprom_values(ST_62_TYPE *st_62_structure) {

	uint8_t i;
	
	SM_APP_vChange_table_state(ST_62_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	for ( i = 0; i < st_61.nbr_chns_set1; i++ ) {

		EEPROM_writer( ST_62_CHNL_FLAG_SET1_ADDR + i, &st_62_structure->lp_sel_set1[i].chnl_flag, 1 );
		EEPROM_writer( ST_62_LP_SOURCE_SELECT_SET1_ADDR + i, &st_62_structure->lp_sel_set1[i].lp_source_select, 1 );
		EEPROM_writer( ST_62_END_BLK_RDG_SOURCE_SELECT_SET1_ADDR + i, &st_62_structure->lp_sel_set1[i].end_blk_rdg_source_select, 1 );
		EEPROM_writer( ST_62_SCALARS_SET1_ADDR + 2*i, &st_62_structure->scalars_set1[i], 2 );
		EEPROM_writer( ST_62_DIVISOR_SET1_ADDR + 2*i, &st_62_structure->divisor_set1[i], 2 );
	}
	
	EEPROM_writer( ST_62_INT_FMT_CDE1_ADDR, &st_62_structure->int_fmt_cde1, 1 );
	
	SM_APP_vChange_table_state(ST_62_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_62_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_62_execute_table_reading(uint8_t *table_buffer) {
	
	LP_APP_vSt_62_to_array(st_62, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_62_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_62_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t i, k = 0;
	ST_62_TYPE st_62_temp;
	
	for ( i = 0; i < st_61.nbr_chns_set1; i++ ) {
		st_62_temp.lp_sel_set1[i].chnl_flag = table_buffer[k++];
		st_62_temp.lp_sel_set1[i].lp_source_select = table_buffer[k++];
		st_62_temp.lp_sel_set1[i].end_blk_rdg_source_select = table_buffer[k++];
	}
	
	// st_62_temp.int_fmt_cde1 = table_buffer;
	k++;
	
	for ( i = 0; i < st_61.nbr_chns_set1; i++ ) {
		st_62_temp.scalars_set1[i] = (uint16_t)(table_buffer[k++]);
		st_62_temp.scalars_set1[i] |= (uint16_t)(table_buffer[k++] << 8);
	}
	
	for ( i = 0; i < st_61.nbr_chns_set1; i++ ) {
		st_62_temp.divisor_set1[i] = (uint16_t)(table_buffer[k++]);
		st_62_temp.divisor_set1[i] |= (uint16_t)(table_buffer[k++] << 8);
	}	
	
	is_ok = LP_APP_bSt_62_validate_data(&st_62_temp);
	
	if ( is_ok == true ) {
		st_62 = st_62_temp;
		LP_APP_vSt_62_write_eeprom_values(&st_62);
		uint16_t crc = SM_APP_usSt_62_ram_calculate_crc();
		SM_APP_vSt_62_write_crc(crc);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t LP_APP_usSt_62_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t LP_APP_usSt_62_get_size(void) {

	uint16_t table_size;
	
	table_size = st_61.nbr_chns_set1 * 7 + 1;
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bLoad_st_63(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bLoad_st_63(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_63_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = LP_APP_bSt_63_restore_default_values();
	}
	else {
		
		is_ok = LP_APP_bSt_63_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_63_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_63_restore_default_values(void) {
	
	bool is_ok = false;
	ST_63_TYPE st_63_temp;
	
	st_63_temp.lp_set_status_flags = DEFAULT_LP_SET_STATUS_FLAGS;
	st_63_temp.nbr_valid_blocks = DEFAULT_NBR_VALID_BLOCKS;
	st_63_temp.last_block_element = DEFAULT_LAST_BLOCK_ELEMENT;
	st_63_temp.last_block_seq_nbr = DEFAULT_LAST_BLOCK_SEQ_NBR;
	st_63_temp.nbr_unread_blocks = DEFAULT_NBR_UNREAD_BLOCKS;
	st_63_temp.nbr_valid_int = DEFAULT_NBR_VALID_INT;
	
	is_ok = LP_APP_bSt_63_validate_data(&st_63_temp);
	
	st_63 = st_63_temp;
	LP_APP_vSt_63_write_eeprom_values(&st_63);
	
	lp.curr_data.int_elapsed_time = 0;
	EEPROM_writer( LP_INTERVAL_TIME_ADDR, &lp.curr_data.int_elapsed_time, TM_FMAT_SIZE );

	lp.is_active = false;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_63_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_63_refresh_ram(void) {
	
	ST_63_TYPE st_63_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		LP_APP_vSt_63_read_eeprom_values(&st_63_temp);
		is_ok = LP_APP_bSt_63_validate_data(&st_63_temp);
		
		if ( is_ok == true ) {
			st_63 = st_63_temp;
			break;
		}
	}
	
	if ( is_ok == false ) {
		st_63 = st_63_temp;
		LP_APP_vSt_63_write_eeprom_values(&st_63);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_63_validate_data(ST_63_TYPE *st_63_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_63_validate_data(ST_63_TYPE *st_63_structure) {
	
	bool is_ok = true;
			
	return is_ok;
}


/**
 ******************************************************************************
 * \fn void LP_APP_vSt_63_read_eeprom_values(ST_63_TYPE *st_63_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vSt_63_read_eeprom_values(ST_63_TYPE *st_63_structure) {
		
	EEPROM_reader( ST_63_SET_STATUS_FLAGS_ADDR, &st_63_structure->lp_set_status_flags, 1 ); 
	EEPROM_reader( ST_63_NBR_VALID_BLOCKS_ADDR, &st_63_structure->nbr_valid_blocks, 2 );
	EEPROM_reader( ST_63_LAST_BLOCK_ELEMENT_ADDR, &st_63_structure->last_block_element, 2 );
	EEPROM_reader( ST_63_LAST_BLOCK_SEQ_NBR_ADDR, &st_63_structure->last_block_seq_nbr, 4 ); 
	EEPROM_reader( ST_63_NBR_UNREAD_BLOCKS_ADDR, &st_63_structure->nbr_unread_blocks, 2 );
	EEPROM_reader( ST_63_NBR_VALID_INT_ADDR, &st_63_structure->nbr_valid_int, 2 );
}

/**
 ******************************************************************************
 * \fn void LP_APP_vSt_63_write_eeprom_values(ST_63_TYPE *st_63_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vSt_63_write_eeprom_values(ST_63_TYPE *st_63_structure) {
		
	SM_APP_vChange_table_state(ST_63_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( ST_63_SET_STATUS_FLAGS_ADDR, &st_63_structure->lp_set_status_flags, 1 );
	EEPROM_writer( ST_63_NBR_VALID_BLOCKS_ADDR, &st_63_structure->nbr_valid_blocks, 2 );
	EEPROM_writer( ST_63_LAST_BLOCK_ELEMENT_ADDR, &st_63_structure->last_block_element, 2 );
	EEPROM_writer( ST_63_LAST_BLOCK_SEQ_NBR_ADDR, &st_63_structure->last_block_seq_nbr, 4 );
	EEPROM_writer( ST_63_NBR_UNREAD_BLOCKS_ADDR, &st_63_structure->nbr_unread_blocks, 2 );
	EEPROM_writer( ST_63_NBR_VALID_INT_ADDR, &st_63_structure->nbr_valid_int, 2 );
	
	SM_APP_vChange_table_state(ST_63_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool LP_APP_bSt_63_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bSt_63_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	
	table_buffer[k++] = st_63.lp_set_status_flags;
	table_buffer[k++] = (uint8_t)(st_63.nbr_valid_blocks);
	table_buffer[k++] = (uint8_t)(st_63.nbr_valid_blocks >> 8);
	
	if ( (st_63.lp_set_status_flags & LIST_TYPE_bm) == LIST_TYPE_bm )
	{
		table_buffer[k++] = (uint8_t)(st_63.last_block_element);
		table_buffer[k++] = (uint8_t)(st_63.last_block_element >> 8);
	}
	else
	{
		table_buffer[k++] = 0;
		table_buffer[k++] = 0;
	}
	
	table_buffer[k++] = (uint8_t)(st_63.last_block_seq_nbr);
	table_buffer[k++] = (uint8_t)(st_63.last_block_seq_nbr >> 8);
	table_buffer[k++] = (uint8_t)(st_63.last_block_seq_nbr >> 16);
	table_buffer[k++] = (uint8_t)(st_63.last_block_seq_nbr >> 24);
	table_buffer[k++] = (uint8_t)(st_63.nbr_unread_blocks);
	table_buffer[k++] = (uint8_t)(st_63.nbr_unread_blocks >> 8);
	table_buffer[k++] = (uint8_t)(st_63.nbr_valid_int);
	table_buffer[k++] = (uint8_t)(st_63.nbr_valid_int >> 8);
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t LP_APP_usSt_63_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t LP_APP_usSt_63_get_size(void) {

	return (uint16_t)ST_63_SIZE;
}

/**
 ******************************************************************************
 * \fn uint32_t LP_APP_ulSt_64_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint32_t LP_APP_ulSt_64_get_size(void) {

	uint16_t i, j, k, l = 0;
	uint32_t table_size = 0;
	
	for ( i = 0; i < st_61.nbr_blks_set1; i++ ) {

		table_size += 4;
		
		for ( j = 0; j < st_61.nbr_chns_set1; j++ ) {

			if ( (st_61.lp_flags & BLK_END_READ_FLAG_bm) == BLK_END_READ_FLAG_bm ) {

				table_size += NI_FMAT1_SIZE;
			}
			
			if ( (st_61.lp_flags & BLK_END_PULSE_FLAG_bm) == BLK_END_PULSE_FLAG_bm ) {

				table_size += 4;
			}
		}
		
		if ( (st_61.lp_flags & SIMPLE_INT_STATUS_FLAG_bm) == SIMPLE_INT_STATUS_FLAG_bm ) {

			static uint8_t nbr_set_bytes;
			nbr_set_bytes = ( (st_61.nbr_blk_ints_set1 + 7) / 8 );
			
			table_size += nbr_set_bytes;
		}
		
		for ( j = 0; j < st_61.nbr_blk_ints_set1; j++ ) {

			if ( (st_61.lp_flags & EXTENDED_INT_STATUS_FLAG_bm) == EXTENDED_INT_STATUS_FLAG_bm ) {

				static uint8_t nbr_set_bytes;
				nbr_set_bytes = ( (st_61.nbr_chns_set1 / 2) + 1 );
				
				table_size += nbr_set_bytes;
			}
			
			for ( l = 0; l < st_61.nbr_chns_set1; l++ ) {

				table_size += 2;
			}
		}
	}
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn void LP_APP_vLoad_st_64_eeprom_addr(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vLoad_st_64_eeprom_addr(void) {
	
	st_64_addr.blk_end_time = ST_64_START_ADDR;
	st_64_addr.block_end_read = st_64_addr.blk_end_time + (TM_FMAT_SIZE * st_61.nbr_blks_set1);
	st_64_addr.simple_int_status = st_64_addr.block_end_read + (NI_FMAT1_SIZE * st_61.nbr_chns_set1 * st_61.nbr_blks_set1);
	st_64_addr.extended_int_status = st_64_addr.simple_int_status + ( ( (st_61.nbr_blk_ints_set1 + 7) / 8) * st_61.nbr_blks_set1 );
	st_64_addr.int_data = st_64_addr.extended_int_status + ( ( (st_61.nbr_chns_set1 / 2) + 1) * st_61.nbr_blk_ints_set1 * st_61.nbr_blks_set1 );
	
	st_64_addr.end_addr = st_64_addr.int_data + ( 2 * st_61.nbr_chns_set1 * st_61.nbr_blk_ints_set1 * st_61.nbr_blks_set1 );
}

/**
 ******************************************************************************
 * \fn uint16_t LP_APP_usGet_block_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t LP_APP_usGet_block_size(void) {

	uint8_t i, j, l;
	uint16_t block_size;
	static uint8_t nbr_set_bytes;
	
	block_size += TM_FMAT_SIZE;
		
	for ( j = 0; j < st_61.nbr_chns_set1; j++ ) {

		block_size += NI_FMAT1_SIZE;
	}

	nbr_set_bytes = ( (st_61.nbr_blk_ints_set1 + 7) / 8 );
	block_size += nbr_set_bytes;
	
	for ( j = 0; j < st_61.nbr_blk_ints_set1; j++ ) {

		nbr_set_bytes = ( (st_61.nbr_chns_set1 / 2) + 1 );
		block_size += nbr_set_bytes;
		
		for ( l = 0; l < st_61.nbr_chns_set1; l++ ) {

			block_size += 2;
		}
	}
	
	return block_size;
}

/**
 ******************************************************************************
 * \fn uint32_t LP_APP_ulLoad_nbr_blocks(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint32_t LP_APP_ulLoad_nbr_blocks(void) {

	uint32_t size_available;
	uint32_t nbr_blocks;
	uint16_t block_size;
	
	size_available = ST_64_END_ADDR - ST_64_START_ADDR;
	block_size = LP_APP_usGet_block_size();
	
	nbr_blocks = size_available/block_size;
	
	st_61.nbr_blks_set1 = nbr_blocks;
	
	return nbr_blocks;
}

/** 
 ******************************************************************************
 * \fn void LP_APP_vRecorder_task(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vRecorder_task(void) {

	volatile uint8_t i;
	volatile double inst_value;

	if ( lp.is_active ) {
			
		lp.curr_data.int_elapsed_time++;
		
		st_63.lp_set_status_flags |= ACTIVE_MODE_FLAG_bm;

		/*===========================================================================*\
		 * Pulse Energy Accumulator
		\*===========================================================================*/
		for ( i = 0; i < st_61.nbr_chns_set1; i++) {

			/* Get instantaneous value for the source selected */
			inst_value = REGS_APP_dDetermine_instantaneous_power( (uint8_t)st_62.lp_sel_set1[i].lp_source_select );
			
			/* Accumulate instantaneous values */
			summ_integer[i] += (uint64_t)( (uint64_t)inst_value / LP_SAMPLES_PER_HOUR );
			summ_remaind[i] += (uint64_t)( (uint64_t)inst_value % LP_SAMPLES_PER_HOUR );
			
			/* Add all remainders */
			if ( (summ_remaind[i] / LP_SAMPLES_PER_HOUR) > 0 ) {
				summ_integer[i] += summ_remaind[i] / LP_SAMPLES_PER_HOUR;
				summ_remaind[i]  = summ_remaind[i] % LP_SAMPLES_PER_HOUR;
			}
			
			/* Convert accumulate values in pulse registers */
			if ( ( (summ_integer[i] * (uint64_t)st_62.divisor_set1[i]) / (uint64_t)st_62.scalars_set1[i] ) > 0 ) {

				lp.pulse_recorder[i] += (summ_integer[i] * (uint64_t)st_62.divisor_set1[i]) / (uint64_t)st_62.scalars_set1[i];
				summ_integer[i]	      = (summ_integer[i] * (uint64_t)st_62.divisor_set1[i]) % (uint64_t)st_62.scalars_set1[i];
			}

			lp.curr_data.pulse_register[i] = lp.pulse_recorder[i] / 1000;
		}
		
		/*===========================================================================*\
		 * Interval period expired 
		\*===========================================================================*/
		if ( rtc.utc_timestamp >= lp.curr_data.int_end_time ) {
			
			for ( i = 0; i < st_61.nbr_chns_set1; i++) {

				/* Detect numeric overflow */
				if ( (lp.pulse_recorder[i] / 1000) > LP_MAX_PULSE_REGISTER_VALUE ) {
					
					LP_APP_vChange_extended_int_channel_status(&lp.curr_data.extended_int_status[0], (uint8_t)i, (uint8_t)CHANNEL_STATUS_1);
					lp.curr_data.pulse_register[i] = LP_MAX_PULSE_REGISTER_VALUE;
					lp.pulse_recorder[i] -= LP_MAX_PULSE_REGISTER_VALUE;
				}
				else {
					lp.pulse_recorder[i] %= 1000;
				}
			}

			lp.last_data = lp.curr_data;
			
			lp.curr_data.int_elapsed_time = 0;
			memset( &lp.curr_data.extended_int_status, 0, sizeof(lp.curr_data.extended_int_status) );
			lp.curr_data.int_number++;
			lp.curr_data.int_number = ( lp.curr_data.int_number >= st_61.nbr_blk_ints_set1 ) ? 0 : lp.curr_data.int_number;
			lp.curr_data.int_end_time += st_61.max_int_time_set1 * SECONDS_PER_MINUTE;
			lp.eoi = true;

			/*===========================================================================*\
			 * Block period expired 
			\*===========================================================================*/
			if ( rtc.utc_timestamp >= lp.curr_data.blk_end_time ) {
				
				lp.curr_data.int_number = 0;
				memset( &lp.last_data.block_simple_flags, 0, sizeof(lp.curr_data.block_simple_flags) );
				lp.curr_data.blk_number++;
				lp.curr_data.blk_number = ( lp.curr_data.blk_number >= st_61.nbr_blks_set1 ) ? 0 : lp.curr_data.blk_number;
				lp.curr_data.blk_end_time += st_61.nbr_blk_ints_set1 * st_61.max_int_time_set1 * SECONDS_PER_MINUTE;
				lp.eob = true;
			}
		}
	}
	else {

		/*===========================================================================*\
		 * Reset lp_set_status_flags if Load Profile is not running
		\*===========================================================================*/
		st_63.lp_set_status_flags &= ~ACTIVE_MODE_FLAG_bm;
	}

	/*===========================================================================*\
	 * Verify if Load Profile is running when Test Mode is enabled
	\*===========================================================================*/
	if ( (GRAL_APP_ucGet_st_3_ed_mode() & TEST_MODE_FLAG_bm) == TEST_MODE_FLAG_bm ) {
		
		st_63.lp_set_status_flags |= TEST_MODE_bm;

		for ( uint16_t i = 0; i < st_61.nbr_chns_set1; i++ ) {
			
			LP_APP_vChange_extended_int_channel_status(&lp.curr_data.extended_int_status[0], (uint8_t)i, (uint8_t)CHANNEL_STATUS_5);
		}
	}
	else {
		st_63.lp_set_status_flags &= ~TEST_MODE_bm;
	}

	/*===========================================================================*\
	 * Verify Overflow Memory assigned to Load Profile
	\*===========================================================================*/
	st_63.lp_set_status_flags = (st_63.nbr_unread_blocks >= st_61.nbr_blks_set1) ? (st_63.lp_set_status_flags |  OVERFLOW_FLAG) 
																				 : (st_63.lp_set_status_flags & ~OVERFLOW_FLAG);

	/*===========================================================================*\
	 * Check if Load Profile is running when DST is on. 
	\*===========================================================================*/
	lp.curr_data.extended_int_status[0] = (rtc.is_dst == 1) ? (lp.curr_data.extended_int_status[0] |  COMMON_DST_FLAG_bm)
															: (lp.curr_data.extended_int_status[0] & ~COMMON_DST_FLAG_bm);
}

/** 
 ******************************************************************************
 * \fn void LP_APP_vChange_extended_int_channel_status(uint8_t* extended_int_status_array, uint8_t channel_number, uint8_t channel_status)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vChange_extended_int_channel_status(uint8_t* extended_int_status_array, uint8_t channel_number,  uint8_t channel_status) {
	
	extended_int_status_array[(channel_number + 1)/2] = ( extended_int_status_array[(channel_number + 1)/2] & ( 0xF << (4 * ( (channel_number + 1) % 2) ) ) ) 
													  | ( channel_status << (4 * (channel_number % 2) ) );
}

/** 
 ******************************************************************************
 * \fn void LP_APP_vStore_data_task(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vStore_data_task(void) {

	if ( (!meter.system_state.changing_configuration) && lp.eoi ) {
		
		uint32_t eeprom_addr;

		/* Save Interval Pulse Register */
		for ( uint8_t i = 0; i < st_61.nbr_chns_set1; i++ ) {
			
			if ( (lp.last_data.int_elapsed_time <= (st_61.max_int_time_set1 * SECONDS_PER_MINUTE) - 1 ) || (lp.last_data.int_elapsed_time >= (st_61.max_int_time_set1 * SECONDS_PER_MINUTE) + 1 ) ) {
					
					if ( lp.last_data.int_elapsed_time < (st_61.max_int_time_set1 * SECONDS_PER_MINUTE) ) {
						
						LP_APP_vChange_extended_int_channel_status(&lp.last_data.extended_int_status[0], (uint8_t)i, (uint8_t)CHANNEL_STATUS_2);		// Partial interval
					}
					else if ( lp.last_data.int_elapsed_time > (st_61.max_int_time_set1 * SECONDS_PER_MINUTE) ) {
						
						LP_APP_vChange_extended_int_channel_status(&lp.last_data.extended_int_status[0], (uint8_t)i, (uint8_t)CHANNEL_STATUS_3);		// Long Interval
					}
				}

			eeprom_addr = st_64_addr.int_data + (lp.last_data.blk_number * MAX_NBR_BLK_INTS_SET1 * st_61.nbr_chns_set1 * 2) + (lp.last_data.int_number * st_61.nbr_chns_set1 * 2) + (i * 2);
			EEPROM_writer( eeprom_addr, &lp.last_data.pulse_register[i], 2 );
		}

		/* Save Extended Interval Status */
		uint8_t nbr_set_bytes1 = ( (st_61.nbr_chns_set1 / 2) + 1 );
		eeprom_addr = st_64_addr.extended_int_status + (lp.last_data.blk_number * nbr_set_bytes1 * MAX_NBR_BLK_INTS_SET1) + (lp.last_data.int_number * nbr_set_bytes1);
		EEPROM_writer( eeprom_addr, &lp.last_data.extended_int_status, nbr_set_bytes1 );

		/* Make this interval valid */
		uint8_t octet_nbr = lp.last_data.int_number / 8;
		lp.curr_data.block_simple_flags[octet_nbr] |= 1 << ( lp.last_data.int_number - 8*octet_nbr );
		memcpy( &lp.last_data.block_simple_flags, &lp.curr_data.block_simple_flags, sizeof(lp.curr_data.block_simple_flags) );
		
		/* Save Block Simple Flags */
		uint8_t nbr_set_bytes2 = ( (st_61.nbr_blk_ints_set1 + 7) / 8 );
		EEPROM_writer( st_64_addr.simple_int_status + (lp.last_data.blk_number * nbr_set_bytes2), &lp.last_data.block_simple_flags, nbr_set_bytes2 );

		/* The block is considered valid when at least one interval is written */
		uint8_t nbr_real_valid_int = 0;
		
		for ( uint16_t i = 0; i < st_61.nbr_blk_ints_set1; i++ ) {

			octet_nbr = i / 8;
			nbr_real_valid_int += ( ( lp.last_data.block_simple_flags[octet_nbr] >> (i - 8*octet_nbr) ) & 0x01 );
		}
		
		if ( nbr_real_valid_int == 1 ) {

			if (st_63.nbr_valid_blocks > 0) {
				st_63.last_block_element++;
				st_63.last_block_seq_nbr++;
				
				if ( st_63.last_block_element >= st_61.nbr_blks_set1 ) {
					st_63.last_block_element = 0;
				}
			}
			
			if ( st_63.nbr_valid_blocks < st_61.nbr_blks_set1 ) {
				st_63.nbr_valid_blocks++;
			}

			st_63.nbr_unread_blocks++;
		}
		
		/* Write Block End Time and Block End Read */
		EEPROM_writer( st_64_addr.blk_end_time + (lp.last_data.blk_number * TM_FMAT_SIZE), &lp.last_data.int_end_time, TM_FMAT_SIZE );
		
		uint64_t summation;
		for ( uint8_t i = 0; i < st_61.nbr_chns_set1; i++ ) {
			
			if ( !REGS_APP_bCopy_summation(TOTAL_DATA, (SUMMATION_ID_TYPE)(st_62.lp_sel_set1[i].lp_source_select - LP_WH_DEL_ID), &summation ) ) { summation = 0; }
			EEPROM_writer( st_64_addr.block_end_read + (lp.last_data.blk_number * NI_FMAT1_SIZE * st_61.nbr_chns_set1) + (i * NI_FMAT1_SIZE), &summation, NI_FMAT1_SIZE );
		}
		
		st_63.nbr_valid_int++;
		LP_APP_vSt_63_write_eeprom_values(&st_63);

		lp.eoi = false;
	}

	if ( lp.eob ) {
		
		EEPROM_writer( LP_EXPECTED_BLOCK_END_TIME_ADDR, &lp.curr_data.blk_end_time, TM_FMAT_SIZE );

		memset( &lp.curr_data.block_simple_flags, 0, sizeof(lp.curr_data.block_simple_flags) );
		EEPROM_writer( st_64_addr.simple_int_status + (lp.curr_data.blk_number * ( (st_61.nbr_blk_ints_set1 + 7) / 8 )), lp.curr_data.block_simple_flags, ( (st_61.nbr_blk_ints_set1 + 7) / 8 ) );
		lp.eob = false;
	}
}

/** 
 ******************************************************************************
 * \fn void LP_restart_pulse_recorder(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_restart_pulse_recorder(void) {

	memset( &summ_integer, 0, sizeof(summ_integer) );
	memset( &summ_remaind, 0, sizeof(summ_remaind) );
	memset( &lp.pulse_recorder, 0, sizeof(lp.pulse_recorder) );	
}

/** 
 ******************************************************************************
 * \fn bool LP_APP_bStart_load_profile(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
bool LP_APP_bStart_load_profile(void) {

	if ( (lp.is_active == false) && (rtc.is_time_relative == 0) ) {

		LP_restart_pulse_recorder();

		if ( st_63.nbr_valid_blocks == 0 ) {
			
			/*===========================================================================*\
			 * CASE 1: Load Profile starts for the first time.
			\*===========================================================================*/

			lp.curr_data.blk_number = 0;

			/* Calculate the block start/end time using the current time, the block start time should be a closed hour */
			uint32_t blk_start_time = rtc.utc_timestamp - (rtc.utc_timestamp % 60) - ( ( (rtc.utc_timestamp/60) % 60 ) * 60);
			lp.curr_data.blk_end_time = blk_start_time + st_61.max_int_time_set1 * st_61.nbr_blk_ints_set1 * SECONDS_PER_MINUTE;

			/* Store the expected block end time */
			EEPROM_writer( LP_EXPECTED_BLOCK_END_TIME_ADDR, &lp.curr_data.blk_end_time, TM_FMAT_SIZE );
			
			/* Determine the current interval and its end time */
			for ( uint16_t i = 0; i < st_61.nbr_blk_ints_set1; i++ ) {

				if ( rtc.utc_timestamp < (blk_start_time + st_61.max_int_time_set1 * 60 * (i + 1) ) ) {

					lp.curr_data.int_number = i;
					break;
				}
			}
			
			st_63.nbr_valid_int = lp.curr_data.int_number;
			lp.curr_data.int_end_time = lp.curr_data.blk_end_time - (st_61.nbr_blk_ints_set1 - (lp.curr_data.int_number + 1 ) ) * st_61.max_int_time_set1 * SECONDS_PER_MINUTE;

			/* Make all intervals as invalids and store */
			memset( &lp.curr_data.block_simple_flags, 0, sizeof(lp.curr_data.block_simple_flags) );
			EEPROM_writer( st_64_addr.simple_int_status + (lp.curr_data.blk_number * ( (st_61.nbr_blk_ints_set1 + 7) / 8 )), lp.curr_data.block_simple_flags, ( (st_61.nbr_blk_ints_set1 + 7) / 8 ) );
			lp.curr_data.int_elapsed_time = 0;

			memset( &lp.curr_data.extended_int_status, 0, sizeof(lp.curr_data.extended_int_status) );
		}
		else {
			
			/*===========================================================================*\
			 * CASE 2: Load profile has been started before.
			\*===========================================================================*/
	
			uint32_t blk_start_time;
			
			/* Determine last block stored and then read the last expected block end time */
			EEPROM_reader( LP_LAST_INTERVAL_USED_ADDR, &lp.curr_data.int_number, 2 );
			EEPROM_reader( LP_LAST_BLOCK_USED_ADDR, &lp.curr_data.blk_number, 2 );
			EEPROM_reader( LP_EXPECTED_BLOCK_END_TIME_ADDR, &lp.curr_data.blk_end_time, TM_FMAT_SIZE );
			lp.curr_data.int_end_time = lp.curr_data.blk_end_time - (st_61.nbr_blk_ints_set1 - (lp.curr_data.int_number + 1 ) ) * st_61.max_int_time_set1 * SECONDS_PER_MINUTE;
			
			lp.last_data.int_number = lp.curr_data.int_number;
			lp.last_data.blk_number = lp.last_data.blk_number;

			/* Read extended status of last interval stored */
			uint8_t set_1_extended_status;
			EEPROM_reader( st_64_addr.extended_int_status + (lp.curr_data.blk_number * ( (st_61.nbr_chns_set1 / 2) + 1 ) * MAX_NBR_BLK_INTS_SET1) + (lp.last_data.int_number * ( (st_61.nbr_chns_set1 / 2) + 1 ) ), &set_1_extended_status, 1 );
			lp.curr_data.extended_int_status[0] = set_1_extended_status & (COMMON_POWER_FAIL_FLAG_bm | COMMON_CLOCK_SET_FWD_FLAG_bm | COMMON_CLOCK_SET_BKWD_FLAG_bm);
			lp.last_data.extended_int_status[0] = lp.curr_data.extended_int_status[0];

			if ( ( rtc.utc_timestamp <= (lp.curr_data.int_end_time - st_61.max_int_time_set1 * SECONDS_PER_MINUTE) ) || ( rtc.utc_timestamp >  lp.curr_data.blk_end_time ) ) {

				/*===========================================================================*\
				 * CASE 2.1: The clock was synchronized to a lesser time at the beginning of 
				 *           the last recorded interval or the new time is greater than the 
				 *           expected final time of the last registered block. A new block 
				 *  		 must be created if any of these conditions are true.
				\*===========================================================================*/

				lp.curr_data.blk_number++;
				lp.curr_data.blk_number = ( lp.curr_data.blk_number >= st_61.nbr_blks_set1 ) ? 0 : lp.curr_data.blk_number;

				/* Calculate the block start/end time using the current time, the block start time should be a closed hour */
				blk_start_time = rtc.utc_timestamp - (rtc.utc_timestamp % 60) - ( ( (rtc.utc_timestamp/60) % 60 ) * 60);
				lp.curr_data.blk_end_time = blk_start_time + st_61.max_int_time_set1 * st_61.nbr_blk_ints_set1 * SECONDS_PER_MINUTE;

				/* Store the expected block end time */
				EEPROM_writer( LP_EXPECTED_BLOCK_END_TIME_ADDR, &lp.curr_data.blk_end_time, TM_FMAT_SIZE );
				
				/* Determine the current interval and its end time */
				for ( uint16_t i = 0; i < st_61.nbr_blk_ints_set1; i++ ) {

					if ( rtc.utc_timestamp < (blk_start_time + st_61.max_int_time_set1 * 60 * (i + 1) ) ) {

						lp.curr_data.int_number = i;
						break;
					}
				}
				
				st_63.nbr_valid_int = lp.curr_data.int_number;
				lp.curr_data.int_end_time = lp.curr_data.blk_end_time - (st_61.nbr_blk_ints_set1 - (lp.curr_data.int_number + 1 ) ) * st_61.max_int_time_set1 * SECONDS_PER_MINUTE;

				/* Make all intervals as invalids and store */
				memset( &lp.curr_data.block_simple_flags, 0, sizeof(lp.curr_data.block_simple_flags) );
				EEPROM_writer( st_64_addr.simple_int_status + (lp.curr_data.blk_number * ( (st_61.nbr_blk_ints_set1 + 7) / 8 )), lp.curr_data.block_simple_flags, ( (st_61.nbr_blk_ints_set1 + 7) / 8 ) );
				lp.curr_data.int_elapsed_time = 0;

			}
			else {

				/*===========================================================================*\
				 * CASE 2.2: The clock was synchronized to a greater time at the beginning of 
				 *           the last recorded interval and the new time is lesser than the 
				 *           expected final time of the last registered block. No new block 
				 *           must be created and possibly the last interval is reused.
				\*===========================================================================*/
				
				/* Calculate the block start time */
				blk_start_time = lp.curr_data.blk_end_time - (60 * st_61.max_int_time_set1 * st_61.nbr_blk_ints_set1);

				/* Determine the current interval and its end time */
				for ( lp.curr_data.int_number = 0; lp.curr_data.int_number < st_61.nbr_blk_ints_set1; lp.curr_data.int_number++ ) {

					if ( rtc.utc_timestamp < (blk_start_time + st_61.max_int_time_set1 * 60 * (lp.curr_data.int_number + 1) ) ) {
						break;
					}
				}
				
				st_63.nbr_valid_int = lp.curr_data.int_number;
				lp.curr_data.int_end_time = lp.curr_data.blk_end_time - (st_61.nbr_blk_ints_set1 - (lp.curr_data.int_number + 1 ) ) * st_61.max_int_time_set1 * SECONDS_PER_MINUTE;

				lp.curr_data.int_elapsed_time = 0;

				/* */
				if ( lp.curr_data.int_number == lp.last_data.int_number ) {
					
					EEPROM_reader( LP_INTERVAL_TIME_ADDR, &lp.curr_data.int_elapsed_time, TM_FMAT_SIZE);
					LP_APP_vRead_eeprom_pulse_recorder();
				}

				/* Read simple status flags */
				uint8_t nbr_set_bytes = ( (st_61.nbr_blk_ints_set1 + 7) / 8 );	
				EEPROM_reader( st_64_addr.simple_int_status + (lp.curr_data.blk_number * nbr_set_bytes), &lp.curr_data.block_simple_flags, nbr_set_bytes );
			}
		}

		LP_APP_vSt_63_write_eeprom_values(&st_63);
		lp.is_active = true;
	}
	
	return lp.is_active;
}

/** 
 ******************************************************************************
 * \fn void LP_APP_vStop_load_profile(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vStop_load_profile(void) {

	if (lp.is_active == true) {

		lp.is_active = false;
		lp.last_data = lp.curr_data;
		lp.eoi = true;
		lp.eob = false;

		LP_APP_vStore_data_task();
		LP_APP_vWrite_eeprom_pulse_recorder();
		
		EEPROM_writer( LP_INTERVAL_TIME_ADDR, &lp.curr_data.int_elapsed_time, 4);
		EEPROM_writer( LP_LAST_INTERVAL_USED_ADDR, &lp.curr_data.int_number, 2);
		EEPROM_writer( LP_LAST_BLOCK_USED_ADDR, &lp.curr_data.blk_number, 2);
	}
}

/** 
 ******************************************************************************
 * \fn void LP_APP_vSynchronize_clock(uint32_t old_time, uint32_t new_time)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vSynchronize_clock(uint32_t old_time, uint32_t new_time) {
	
	if ( lp.is_active == true ) {
		
		if (old_time != new_time) {

			lp.curr_data.extended_int_status[0] |= (new_time > old_time) ? COMMON_CLOCK_SET_FWD_FLAG_bm : COMMON_CLOCK_SET_BKWD_FLAG_bm;

			LP_APP_vStop_load_profile();
			LP_APP_bStart_load_profile();

			lp.curr_data.extended_int_status[0] |= (new_time > old_time) ? COMMON_CLOCK_SET_FWD_FLAG_bm : COMMON_CLOCK_SET_BKWD_FLAG_bm;
		}
	}
	else {
		
		LP_APP_bStart_load_profile();
	}
}

/** 
 ******************************************************************************
 * \fn void LP_APP_vWrite_eeprom_pulse_recorder(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vWrite_eeprom_pulse_recorder(void) {

	for ( uint8_t i = 0; i < st_61.nbr_chns_set1; i++) {
		EEPROM_writer( LP_BACKUP_PULSE_RECORDER_ADDR + i*4, &lp.pulse_recorder[i], 4);
	}
}

/** 
 ******************************************************************************
 * \fn void LP_APP_vRead_eeprom_pulse_recorder(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vRead_eeprom_pulse_recorder(void) {

	for ( uint8_t i = 0; i < st_61.nbr_chns_set1; i++) {
		EEPROM_reader( LP_BACKUP_PULSE_RECORDER_ADDR + i*4, &lp.pulse_recorder[i], 4);
	}
}

/** 
 ******************************************************************************
 * \fn void LP_APP_vPower_fail_detection(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vPower_fail_detection(void) {

	if ( lp.is_active ) {
		
		lp.curr_data.extended_int_status[0] |= COMMON_POWER_FAIL_FLAG_bm;
		LP_APP_vStop_load_profile();
	}
}

/** 
 ******************************************************************************
 * \fn void LP_APP_vResume(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vResume(void) {

	lp.is_active = true;
}

/** 
 ******************************************************************************
 * \fn void LP_APP_vGet_data_block_circ_table(uint16_t block_offset, uint16_t block_count, uint8_t* data_buffer)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LP_APP_vGet_data_block_circ_table(uint16_t block_offset, uint16_t block_count, uint8_t* data_buffer) {

	uint16_t i, j, k, l;
	static uint32_t eeprom_addr;
	uint8_t nbr_set_bytes;
	
	k = 0;
	
	for ( i = block_offset; i < (block_offset + block_count); i++ ) {

		eeprom_addr = st_64_addr.blk_end_time + (i * TM_FMAT_SIZE);
		EEPROM_reader( eeprom_addr, &data_buffer[k], TM_FMAT_SIZE);
		k += TM_FMAT_SIZE;
		
		for ( j = 0; j < st_61.nbr_chns_set1; j++ ) {

			if ( (st_61.lp_flags & BLK_END_READ_FLAG_bm) == BLK_END_READ_FLAG_bm ) {

				eeprom_addr = st_64_addr.block_end_read + (i * NI_FMAT1_SIZE * st_61.nbr_chns_set1) + (j * NI_FMAT1_SIZE);
				EEPROM_reader( eeprom_addr, &data_buffer[k], NI_FMAT1_SIZE);
				k += NI_FMAT1_SIZE;
			}
		}
		
		if ( (st_61.lp_flags & SIMPLE_INT_STATUS_FLAG_bm) == SIMPLE_INT_STATUS_FLAG_bm ) {

			nbr_set_bytes = ( (st_61.nbr_blk_ints_set1 + 7) / 8 );
			eeprom_addr = st_64_addr.simple_int_status + (i * nbr_set_bytes);
			EEPROM_reader( eeprom_addr, &data_buffer[k], nbr_set_bytes);
			k += nbr_set_bytes;
		}
		
		for ( j = 0; j < st_61.nbr_blk_ints_set1; j++ ) {

			if ( (st_61.lp_flags & EXTENDED_INT_STATUS_FLAG_bm) == EXTENDED_INT_STATUS_FLAG_bm ) {

				nbr_set_bytes = ( (st_61.nbr_chns_set1 / 2) + 1 );
				eeprom_addr = st_64_addr.extended_int_status + (i * nbr_set_bytes * MAX_NBR_BLK_INTS_SET1) + (j * nbr_set_bytes);
				EEPROM_reader( eeprom_addr, &data_buffer[k], nbr_set_bytes);
				k += nbr_set_bytes;
			}
			
			for ( l = 0; l < st_61.nbr_chns_set1; l++ ) {

				eeprom_addr = st_64_addr.int_data + (i * MAX_NBR_BLK_INTS_SET1 * st_61.nbr_chns_set1 * 2) + (j * st_61.nbr_chns_set1 * 2) + (l * 2);
				EEPROM_reader( eeprom_addr, &data_buffer[k], 2);
				k += 2;
			}
		}
		
		WATCHDOG_APP_vRestart();
	}
}

/** 
 ******************************************************************************
 * \fn void LP_APP_vGet_data_block_fifo_table(uint16_t block_offset, uint16_t block_count, uint8_t* data_buffer)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
extern void LP_APP_vGet_data_block_fifo_table(uint16_t block_offset, uint16_t block_count, uint8_t* data_buffer) {

	uint16_t j, k, l, m;
	int16_t i; 
	static uint32_t eeprom_addr;
	uint8_t nbr_set_bytes;
	
	k = 0;
	
	for ( m = block_offset; m < (block_offset + block_count); m++ ) {

		i = st_63.last_block_element - m;
		if ( i < 0 ) { i += st_61.nbr_blks_set1; }
		
		eeprom_addr = st_64_addr.blk_end_time + (i * TM_FMAT_SIZE);
		EEPROM_reader( eeprom_addr, &data_buffer[k], TM_FMAT_SIZE);
		k += TM_FMAT_SIZE;
		
		for ( j = 0; j < st_61.nbr_chns_set1; j++ ) {

			if ( (st_61.lp_flags & BLK_END_READ_FLAG_bm) == BLK_END_READ_FLAG_bm ) {

				eeprom_addr = st_64_addr.block_end_read + (i * NI_FMAT1_SIZE * st_61.nbr_chns_set1) + (j * NI_FMAT1_SIZE);
				EEPROM_reader( eeprom_addr, &data_buffer[k], NI_FMAT1_SIZE);
				k += NI_FMAT1_SIZE;
			}
		}
		
		if ( (st_61.lp_flags & SIMPLE_INT_STATUS_FLAG_bm) == SIMPLE_INT_STATUS_FLAG_bm ) {

			nbr_set_bytes = ( (st_61.nbr_blk_ints_set1 + 7) / 8 );
			eeprom_addr = st_64_addr.simple_int_status + (i * nbr_set_bytes);
			EEPROM_reader( eeprom_addr, &data_buffer[k], nbr_set_bytes);
			k += nbr_set_bytes;
		}
		
		for ( j = 0; j < st_61.nbr_blk_ints_set1; j++ ) {

			if ( (st_61.lp_flags & EXTENDED_INT_STATUS_FLAG_bm) == EXTENDED_INT_STATUS_FLAG_bm ) {

				nbr_set_bytes = ( (st_61.nbr_chns_set1 / 2) + 1 );
				eeprom_addr = st_64_addr.extended_int_status + (i * nbr_set_bytes * MAX_NBR_BLK_INTS_SET1) + (j * nbr_set_bytes);
				EEPROM_reader( eeprom_addr, &data_buffer[k], nbr_set_bytes);
				k += nbr_set_bytes;
			}
			
			for ( l = 0; l < st_61.nbr_chns_set1; l++ ) {

				eeprom_addr = st_64_addr.int_data + (i * MAX_NBR_BLK_INTS_SET1 * st_61.nbr_chns_set1 * 2) + (j * st_61.nbr_chns_set1 * 2) + (l * 2);
				EEPROM_reader( eeprom_addr, &data_buffer[k], 2);
				k += 2;
			}
		}
		
		WATCHDOG_APP_vRestart();
	}
}

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-12-29
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */