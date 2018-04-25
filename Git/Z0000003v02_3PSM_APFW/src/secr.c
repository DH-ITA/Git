/**
 ******************************************************************************
 * \file secr.c
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
#include "target.h"
#include "secr.h"
#include "sm.h"
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
static ACCESS_CONTROL_ENTRY_RCD_TYPE access_control;
static ST_43_TYPE st_43;
static ST_44_TYPE st_44;

/*
 ************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ************************************************************************
 */
 ST_40_TYPE st_41;
 ST_42_TYPE st_42;

/**
 ************************************************************************
 * Function Prototypes for Private Functions with File Level Scope
 ************************************************************************
 */
static bool SECR_APP_bSt_41_validate_data(ST_40_TYPE *st_41_structure);
static void SECR_APP_vSt_41_read_eeprom_values(ST_40_TYPE *st_41_structure);
static void SECR_APP_vSt_41_write_eeprom_values(ST_40_TYPE *st_41_structure);
static void SECR_APP_vSt_41_to_array(ST_40_TYPE st_41_source, uint8_t *table_buffer);

static bool SECR_APP_bSt_42_validate_data(ST_42_TYPE *st_42_structure);
static void SECR_APP_vSt_42_read_eeprom_values(ST_42_TYPE *st_42_structure);
static void SECR_APP_vSt_42_write_eeprom_values(ST_42_TYPE *st_42_structure);
static void SECR_APP_vSt_42_to_array(ST_42_TYPE st_42_source, uint8_t *table_buffer);

static bool SECR_APP_bSt_43_validate_data(ST_43_TYPE *st_43_structure);
static void SECR_APP_vSt_43_read_eeprom_values(ST_43_TYPE *st_43_structure);
static void SECR_APP_vSt_43_write_eeprom_values(ST_43_TYPE *st_43_structure);
static void SECR_APP_vSt_43_to_array(ST_43_TYPE st_43_source, uint8_t *table_buffer);

static bool SECR_APP_bSt_44_validate_data(ST_44_TYPE *st_44_structure);
static void SECR_APP_vSt_44_read_eeprom_values(ST_44_TYPE *st_44_structure);
static void SECR_APP_vSt_44_write_eeprom_values(ST_44_TYPE *st_44_structure);
static void SECR_APP_vSt_44_to_array(ST_44_TYPE st_44_source, uint8_t *table_buffer);

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
 * \fn void SECR_APP_vInitialize(void)
 * \brief SECR Library Initialization
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vInitialize(void) {

}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_40_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_40_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	
	table_buffer[0] = MAX_NBR_PASSWORDS;
	table_buffer[1] = MAX_PASSWORD_LEN;
	table_buffer[2] = MAX_NBR_KEYS;
	table_buffer[3] = MAX_KEY_LEN;
	table_buffer[4] = (uint8_t)((MAX_NBR_PERM_USED & 0x00FF) >> 0);
	table_buffer[5] = (uint8_t)((MAX_NBR_PERM_USED & 0xFF00) >> 8);
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t SECR_APP_usSt_40_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SECR_APP_usSt_40_get_size(void) {

	return (uint16_t) ST_40_MAX_SIZE;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bLoad_st_41(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bLoad_st_41(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_41_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = SECR_APP_bSt_41_restore_default_values();
	}
	else {
		
		is_ok = SECR_APP_bSt_41_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_41_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_41_restore_default_values(void) {
	
	bool is_ok = false;
	ST_40_TYPE st_41_temp;
	
	st_41_temp.nbr_passwords = DEFAULT_NBR_PASSWORDS;
	st_41_temp.password_len = DEFAULT_PASSWORD_LEN;
	st_41_temp.nbr_keys = DEFAULT_NBR_KEYS;
	st_41_temp.key_len = DEFAULT_KEY_LEN;
	st_41_temp.nbr_perm_used = DEFAULT_NBR_PERM_USED;
	
	is_ok = SECR_APP_bSt_41_validate_data(&st_41_temp);
	
	st_41 = st_41_temp;
	SECR_APP_vSt_41_write_eeprom_values(&st_41);
	uint16_t crc = SM_APP_usSt_41_ram_calculate_crc();
	SM_APP_vSt_41_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_41_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_41_refresh_ram(void) {
	
	ST_40_TYPE st_41_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		SECR_APP_vSt_41_read_eeprom_values(&st_41_temp);
		is_ok = SECR_APP_bSt_41_validate_data(&st_41_temp);
		
		if ( is_ok ) {
			st_41 = st_41_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_41 = st_41_temp;
		SECR_APP_vSt_41_write_eeprom_values(&st_41);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vCopy_st_41_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vCopy_st_41_ram(uint8_t* destination) {
	
	SECR_APP_vSt_41_to_array(st_41, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vCopy_st_41_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vCopy_st_41_nvm(uint8_t* destination) {
	
	ST_40_TYPE st_41_aux;
	
	SECR_APP_vSt_41_read_eeprom_values(&st_41_aux);
	SECR_APP_vSt_41_to_array(st_41_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_41_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_41_recover_nvm(void) {
	
	SECR_APP_vSt_41_write_eeprom_values(&st_41);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_41_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_41_recover_ram(void) {
	
	SECR_APP_vSt_41_read_eeprom_values(&st_41);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_41_to_array(ST_40_TYPE st_41_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_41_to_array(ST_40_TYPE st_41_source, uint8_t *table_buffer) {
		
	table_buffer[0] = st_41_source.nbr_passwords;
	table_buffer[1] = st_41_source.password_len;
	table_buffer[2] = st_41_source.nbr_keys;
	table_buffer[3] = st_41_source.key_len;
	table_buffer[4] = (uint8_t)((st_41_source.nbr_perm_used & 0x00FF) >> 0);
	table_buffer[5] = (uint8_t)((st_41_source.nbr_perm_used & 0xFF00) >> 8);
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_41_validate_data(ST_40_TYPE *st_41_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_41_validate_data(ST_40_TYPE *st_41_structure) {
	
	bool is_ok = true;
	uint8_t i;
	
	if (st_41_structure->nbr_passwords > MAX_NBR_PASSWORDS) {
		st_41_structure->nbr_passwords = MAX_NBR_PASSWORDS;
		is_ok = false;
	}
	
	if (st_41_structure->password_len > MAX_PASSWORD_LEN) {
		st_41_structure->password_len = MAX_PASSWORD_LEN;
		is_ok = false;
	}
	
	if (st_41_structure->nbr_keys > MAX_NBR_KEYS) {
		st_41_structure->nbr_keys = MAX_NBR_KEYS;
		is_ok = false;
	}
	
	if (st_41_structure->key_len > MAX_KEY_LEN) {
		st_41_structure->key_len = MAX_KEY_LEN;
		is_ok = false;
	}
	
	if (st_41_structure->nbr_perm_used > MAX_NBR_PERM_USED) {
		st_41_structure->nbr_perm_used = MAX_NBR_PERM_USED;
		is_ok = false;
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_41_read_eeprom_values(ST_40_TYPE *st_41_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_41_read_eeprom_values(ST_40_TYPE *st_41_structure) {	
	
	EEPROM_reader(ST_41_NBR_PASSWORDS_ADDR, &st_41_structure->nbr_passwords, 1);
	EEPROM_reader(ST_41_PASSWORD_LEN_ADDR, &st_41_structure->password_len, 1);
	EEPROM_reader(ST_41_NBR_KEYS_ADDR, &st_41_structure->nbr_keys, 1);
	EEPROM_reader(ST_41_KEY_LEN_ADDR, &st_41_structure->key_len, 1);
	EEPROM_reader(ST_41_NBR_PERM_USED_ADDR, &st_41_structure->nbr_perm_used, 2);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_41_write_eeprom_values(ST_40_TYPE *st_41_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_41_write_eeprom_values(ST_40_TYPE *st_41_structure) {
	
	SM_APP_vChange_table_state(ST_41_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer(ST_41_NBR_PASSWORDS_ADDR, &st_41_structure->nbr_passwords, 1);
	EEPROM_writer(ST_41_PASSWORD_LEN_ADDR, &st_41_structure->password_len, 1);
	EEPROM_writer(ST_41_NBR_KEYS_ADDR, &st_41_structure->nbr_keys, 1);
	EEPROM_writer(ST_41_KEY_LEN_ADDR, &st_41_structure->key_len, 1);
	EEPROM_writer(ST_41_NBR_PERM_USED_ADDR, &st_41_structure->nbr_perm_used, 2);
	
	SM_APP_vChange_table_state(ST_41_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_41_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_41_execute_table_reading(uint8_t *table_buffer) {
	
	SECR_APP_vSt_41_to_array(st_41, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_41_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_41_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	ST_40_TYPE st_41_temp;
	
	st_41_temp.nbr_passwords = table_buffer[0];
	st_41_temp.password_len = table_buffer[1];
	st_41_temp.nbr_keys = table_buffer[2];
	st_41_temp.key_len = table_buffer[3];
	st_41_temp.nbr_perm_used  = (uint16_t)(table_buffer[4] << 0);
	st_41_temp.nbr_perm_used |= (uint16_t)(table_buffer[5] << 8);
	
	is_ok = SECR_APP_bSt_41_validate_data(&st_41_temp);
	
	if ( is_ok ) {
		st_41 = st_41_temp;
		SECR_APP_vSt_41_write_eeprom_values(&st_41);
		uint16_t crc = SM_APP_usSt_41_ram_calculate_crc();
		SM_APP_vSt_41_write_crc(crc);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t SECR_APP_usSt_41_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SECR_APP_usSt_41_get_size(void) {

	return 	(uint16_t)ST_41_MAX_SIZE;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bLoad_st_42(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bLoad_st_42(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_42_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = SECR_APP_bSt_42_restore_default_values();
	}
	else {
		is_ok = SECR_APP_bSt_42_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_42_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_42_restore_default_values(void) {
	
	bool is_ok = false;
	ST_42_TYPE st_42_temp;
	
	memcpy(st_42_temp.security_entries[0].password, DEFAULT_PASSWORD_0, MAX_PASSWORD_LEN);
	st_42_temp.security_entries[0].access_permissions = DEFAULT_PASS_0_ACCESS_PERM;
	
	memcpy(st_42_temp.security_entries[1].password, DEFAULT_PASSWORD_1, MAX_PASSWORD_LEN);
	st_42_temp.security_entries[1].access_permissions = DEFAULT_PASS_1_ACCESS_PERM;
	
	memcpy(st_42_temp.security_entries[2].password, DEFAULT_PASSWORD_2, MAX_PASSWORD_LEN);
	st_42_temp.security_entries[2].access_permissions = DEFAULT_PASS_2_ACCESS_PERM;
	
	memcpy(st_42_temp.security_entries[3].password, DEFAULT_PASSWORD_3, MAX_PASSWORD_LEN);
	st_42_temp.security_entries[3].access_permissions = DEFAULT_PASS_3_ACCESS_PERM;
	
	memcpy(st_42_temp.security_entries[4].password, DEFAULT_PASSWORD_4, MAX_PASSWORD_LEN);
	st_42_temp.security_entries[4].access_permissions = DEFAULT_PASS_4_ACCESS_PERM;
	
	is_ok = SECR_APP_bSt_42_validate_data(&st_42_temp);
	
	st_42 = st_42_temp;
	SECR_APP_vSt_42_write_eeprom_values(&st_42);
	uint16_t crc = SM_APP_usSt_42_ram_calculate_crc();
	SM_APP_vSt_42_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_42_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_42_refresh_ram(void) {
	
	ST_42_TYPE st_42_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		SECR_APP_vSt_42_read_eeprom_values(&st_42_temp);
		is_ok = SECR_APP_bSt_42_validate_data(&st_42_temp);
		
		if ( is_ok ) {
			st_42 = st_42_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_42 = st_42_temp;
		SECR_APP_vSt_42_write_eeprom_values(&st_42);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  SECR_APP_vCopy_st_42_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vCopy_st_42_ram(uint8_t* destination) {
	
	SECR_APP_vSt_42_to_array(st_42, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vCopy_st_42_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vCopy_st_42_nvm(uint8_t* destination) {
	
	ST_42_TYPE st_42_aux;
	
	SECR_APP_vSt_42_read_eeprom_values(&st_42_aux);
	SECR_APP_vSt_42_to_array(st_42_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_42_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_42_recover_nvm(void) {
	
	SECR_APP_vSt_42_write_eeprom_values(&st_42);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_42_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_42_recover_ram(void) {
	
	SECR_APP_vSt_42_read_eeprom_values(&st_42);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_42_to_array(ST_42_TYPE st_42_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_42_to_array(ST_42_TYPE st_42_source, uint8_t *table_buffer) {
	
	uint16_t k = 0;
	
	for (uint8_t i = 0; i < st_41.nbr_passwords; i++) {
		
		memcpy(&table_buffer[k], &st_42_source.security_entries[i].password, st_41.password_len);
		k += st_41.password_len;
		 
		table_buffer[k] = st_42_source.security_entries[i].access_permissions;
		k++;
	}
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_42_validate_data(ST_42_TYPE *st_42_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_42_validate_data(ST_42_TYPE *st_42_structure) {
		
	return true;
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_42_read_eeprom_values(ST_42_TYPE *st_42_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_42_read_eeprom_values(ST_42_TYPE *st_42_structure) {	
	
	for (uint8_t i = 0; i < st_41.nbr_passwords; i++) {
		
		EEPROM_reader(ST_42_PASSWORD_ADDR + (MAX_PASSWORD_LEN * i), &st_42_structure->security_entries[i].password, st_41.password_len);
		EEPROM_reader(ST_42_ACCESS_PERMISSION_BFLD_ADDR + i, &st_42_structure->security_entries[i].access_permissions, 1);
	}
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_42_write_eeprom_values(ST_42_TYPE *st_42_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_42_write_eeprom_values(ST_42_TYPE *st_42_structure) {
	
	SM_APP_vChange_table_state(ST_42_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	for (uint8_t i = 0; i < st_41.nbr_passwords; i++) {
		
		EEPROM_writer(ST_42_PASSWORD_ADDR + (MAX_PASSWORD_LEN * i), &st_42_structure->security_entries[i].password, st_41.password_len);
		EEPROM_writer(ST_42_ACCESS_PERMISSION_BFLD_ADDR + i, &st_42_structure->security_entries[i].access_permissions, 1);
	}
	
	SM_APP_vChange_table_state(ST_42_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_42_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_42_execute_table_reading(uint8_t *table_buffer) {
	
	SECR_APP_vSt_42_to_array(st_42, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_42_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_42_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	ST_42_TYPE st_42_temp;
	
	for (uint8_t i = 0; i < st_41.nbr_passwords; i++) {
		
		memcpy(&st_42_temp.security_entries[i].password, &table_buffer[k], st_41.password_len);
		k += st_41.password_len;
		st_42_temp.security_entries[i].access_permissions = table_buffer[k++];
	}
	
	is_ok = SECR_APP_bSt_42_validate_data(&st_42_temp);
	
	if ( is_ok ) {
		st_42 = st_42_temp;
		SECR_APP_vSt_42_write_eeprom_values(&st_42);
		uint16_t crc = SM_APP_usSt_42_ram_calculate_crc();
		SM_APP_vSt_42_write_crc(crc);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t SECR_APP_usSt_42_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SECR_APP_usSt_42_get_size(void) {

	return (uint16_t)( (st_41.nbr_passwords) * (st_41.password_len + 1) );
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bLoad_st_43(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bLoad_st_43(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_43_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = SECR_APP_bSt_43_restore_default_values();
	}
	else {
		is_ok = SECR_APP_bSt_43_refresh_ram();
	}
	
	return is_ok;
}

/** 
 ******************************************************************************
 * \fn bool SECR_APP_bSt_43_restore_default_values(void)
 * \brief 
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_43_restore_default_values(void) {
	
	bool is_ok = false;
	ST_43_TYPE st_43_temp;
	
	st_43_temp.table_default.access_table_default = DEFAULT_TABLE_INFO;
	st_43_temp.table_default.read = (uint8_t)DEFAULT_TABLE_ACCESS_CTRL_READ;
	st_43_temp.table_default.write = (uint8_t)DEFAULT_TABLE_ACCESS_CTRL_WRITE;
	st_43_temp.procedure_default.access_table_default = DEFAULT_PROCEDURE_INFO;
	st_43_temp.procedure_default.read = (uint8_t)DEFAULT_TABLE_ACCESS_CTRL_READ;
	st_43_temp.procedure_default.write = (uint8_t)DEFAULT_TABLE_ACCESS_CTRL_WRITE;
	
	is_ok = SECR_APP_bSt_43_validate_data(&st_43_temp);
	
	st_43 = st_43_temp;
	SECR_APP_vSt_43_write_eeprom_values(&st_43);
	uint16_t crc = SM_APP_usSt_43_ram_calculate_crc();
	SM_APP_vSt_43_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_43_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_43_refresh_ram(void) {
	
	ST_43_TYPE st_43_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		SECR_APP_vSt_43_read_eeprom_values(&st_43_temp);
		is_ok = SECR_APP_bSt_43_validate_data(&st_43_temp);
		
		if ( is_ok ) {
			st_43 = st_43_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_43 = st_43_temp;
		SECR_APP_vSt_43_write_eeprom_values(&st_43);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  SECR_APP_vCopy_st_43_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vCopy_st_43_ram(uint8_t* destination) {
	
	SECR_APP_vSt_43_to_array(st_43, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vCopy_st_43_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vCopy_st_43_nvm(uint8_t* destination) {
	
	ST_43_TYPE st_43_aux;
	
	SECR_APP_vSt_43_read_eeprom_values(&st_43_aux);
	SECR_APP_vSt_43_to_array(st_43_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_43_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_43_recover_nvm(void) {
	
	SECR_APP_vSt_43_write_eeprom_values(&st_43);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_43_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_43_recover_ram(void) {
	
	SECR_APP_vSt_43_read_eeprom_values(&st_43);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_43_to_array(ST_43_TYPE st_43_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_43_to_array(ST_43_TYPE st_43_source, uint8_t *table_buffer) {
	
	table_buffer[0] = (uint8_t)(st_43_source.table_default.access_table_default >> 0);
	table_buffer[1] = (uint8_t)(st_43_source.table_default.access_table_default >> 8);
	table_buffer[2] = st_43_source.table_default.read;
	table_buffer[3] = st_43_source.table_default.write;
	table_buffer[4] = (uint8_t)(st_43_source.procedure_default.access_table_default >> 0);
	table_buffer[5] = (uint8_t)(st_43_source.procedure_default.access_table_default >> 8);
	table_buffer[6] = st_43_source.procedure_default.read;
	table_buffer[7] = st_43_source.procedure_default.write;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_43_validate_data(ST_42_TYPE *st_42_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_43_validate_data(ST_43_TYPE *st_43_structure) {
		
	return true;
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_43_read_eeprom_values(ST_43_TYPE *st_43_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_43_read_eeprom_values(ST_43_TYPE *st_43_structure) {	
	
	EEPROM_reader(ST_43_ACCESS_TABLE_DEFAULT_ADDR, &st_43_structure->table_default.access_table_default, 2);
	EEPROM_reader(ST_43_TABLE_ACCESS_PERM_BFLD_READ_ADDR, &st_43_structure->table_default.read, 1);
	EEPROM_reader(ST_43_TABLE_ACCESS_PERM_BFLD_WRITE_ADDR, &st_43_structure->table_default.write, 1);
	EEPROM_reader(ST_43_ACCESS_PROCEDURE_DEFAULT_ADDR, &st_43_structure->procedure_default.access_table_default, 2);
	EEPROM_reader(ST_43_PROCEDURE_ACCESS_PERM_BFLD_READ_ADDR, &st_43_structure->procedure_default.read, 1);
	EEPROM_reader(ST_43_PROCEDURE_ACCESS_PERM_BFLD_WRITE_ADDR, &st_43_structure->procedure_default.write, 1);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_43_write_eeprom_values(ST_43_TYPE *st_43_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_43_write_eeprom_values(ST_43_TYPE *st_43_structure) {
	
	SM_APP_vChange_table_state(ST_43_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer(ST_43_ACCESS_TABLE_DEFAULT_ADDR, &st_43_structure->table_default.access_table_default, 2);
	EEPROM_writer(ST_43_TABLE_ACCESS_PERM_BFLD_READ_ADDR, &st_43_structure->table_default.read, 1);
	EEPROM_writer(ST_43_TABLE_ACCESS_PERM_BFLD_WRITE_ADDR, &st_43_structure->table_default.write, 1);
	EEPROM_writer(ST_43_ACCESS_PROCEDURE_DEFAULT_ADDR, &st_43_structure->procedure_default.access_table_default, 2);
	EEPROM_writer(ST_43_PROCEDURE_ACCESS_PERM_BFLD_READ_ADDR, &st_43_structure->procedure_default.read, 1);
	EEPROM_writer(ST_43_PROCEDURE_ACCESS_PERM_BFLD_WRITE_ADDR, &st_43_structure->procedure_default.write, 1);
	
	SM_APP_vChange_table_state(ST_43_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_43_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_43_execute_table_reading(uint8_t *table_buffer) {
	
	SECR_APP_vSt_43_to_array(st_43, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_43_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_43_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	ST_43_TYPE st_43_temp;
	
	st_43_temp.table_default.access_table_default  = (uint16_t)(table_buffer[0] << 0);
	st_43_temp.table_default.access_table_default |= (uint16_t)(table_buffer[1] << 8);
	st_43_temp.table_default.read = table_buffer[2];
	st_43_temp.table_default.write = table_buffer[3];
	st_43_temp.procedure_default.access_table_default  = (uint16_t)(table_buffer[4] << 0);
	st_43_temp.procedure_default.access_table_default |= (uint16_t)(table_buffer[5] << 8);
	st_43_temp.procedure_default.read = table_buffer[6];
	st_43_temp.procedure_default.write = table_buffer[7];
	
	is_ok = SECR_APP_bSt_43_validate_data(&st_43_temp);
	
	if ( is_ok ) {
		st_43 = st_43_temp;
		SECR_APP_vSt_43_write_eeprom_values(&st_43);
		uint16_t crc = SM_APP_usSt_43_ram_calculate_crc();
		SM_APP_vSt_43_write_crc(crc);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t SECR_APP_usSt_43_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SECR_APP_usSt_43_get_size(void) {
	
	return (uint16_t)ST_43_MAX_SIZE;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bLoad_st_44(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bLoad_st_44(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_44_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = SECR_APP_bSt_44_restore_default_values();
	}
	else {
		
		is_ok = SECR_APP_bSt_44_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_44_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_44_restore_default_values(void) {
	
	bool is_ok = false;
	ST_44_TYPE st_44_temp;
	uint16_t i;
	
	for ( i = 0; i < st_41.nbr_perm_used; i++ ) {
		
		st_44_temp.access_control[i].access_table_def = DEFAULT_ACCESS_TABLE_DEF;
		st_44_temp.access_control[i].read = DEFAULT_DEF_CTRL_ACCESS_READ;
		st_44_temp.access_control[i].write = DEFAULT_DEF_CTRL_ACCESS_WRITE;
	}
	
	is_ok = SECR_APP_bSt_44_validate_data(&st_44_temp);
	
	st_44 = st_44_temp;
	SECR_APP_vSt_44_write_eeprom_values(&st_44);
	uint16_t crc = SM_APP_usSt_44_ram_calculate_crc();
	SM_APP_vSt_44_write_crc(crc);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_44_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_44_refresh_ram(void) {
	
	ST_44_TYPE st_44_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		SECR_APP_vSt_44_read_eeprom_values(&st_44_temp);
		is_ok = SECR_APP_bSt_44_validate_data(&st_44_temp);
		
		if ( is_ok ) {
			st_44 = st_44_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_44 = st_44_temp;
		SECR_APP_vSt_44_write_eeprom_values(&st_44);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn  SECR_APP_vCopy_st_44_ram(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vCopy_st_44_ram(uint8_t* destination) {
	
	SECR_APP_vSt_44_to_array(st_44, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vCopy_st_44_nvm(uint8_t* destination)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vCopy_st_44_nvm(uint8_t* destination) {
	
	ST_44_TYPE st_44_aux;
	
	SECR_APP_vSt_44_read_eeprom_values(&st_44_aux);
	SECR_APP_vSt_44_to_array(st_44_aux, &destination[0]);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_44_recover_nvm(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_44_recover_nvm(void) {
	
	SECR_APP_vSt_44_write_eeprom_values(&st_44);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_44_recover_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_44_recover_ram(void) {
	
	SECR_APP_vSt_44_read_eeprom_values(&st_44);
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_44_to_array(ST_44_TYPE st_44_source, uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_44_to_array(ST_44_TYPE st_44_source, uint8_t *table_buffer) {
	
	uint16_t k = 0;
	
	for ( uint16_t i = 0; i < st_41.nbr_perm_used; i++) {
		
		table_buffer[k++] = (uint8_t)((st_44_source.access_control[i].access_table_def & 0x00FF) >> 0);
		table_buffer[k++] = (uint8_t)((st_44_source.access_control[i].access_table_def & 0xFF00) >> 8);
		table_buffer[k++] = st_44_source.access_control[i].read;
		table_buffer[k++] = st_44_source.access_control[i].write;
	}
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_44_validate_data(ST_44_TYPE *st_44_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_44_validate_data(ST_44_TYPE *st_44_structure) {
	
	return true;
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_44_read_eeprom_values(ST_44_TYPE *st_44_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_44_read_eeprom_values(ST_44_TYPE *st_44_structure) {	
	
	for ( uint16_t i = 0; i < st_41.nbr_perm_used; i++ ) {
		
		EEPROM_reader(ST_44_ACCESS_TABLE_DEFAULT_ADDR + 2*i, &st_44_structure->access_control[i].access_table_def, 2);
		EEPROM_reader(ST_44_TABLE_ACCESS_PERMISSION_BFLD_READ_ADDR + i, &st_44_structure->access_control[i].read, 1);
		EEPROM_reader(ST_44_TABLE_ACCESS_PERMISSION_BFLD_WRITE_ADDR + i, &st_44_structure->access_control[i].write, 1);
		WATCHDOG_APP_vRestart();
	}
}

/**
 ******************************************************************************
 * \fn void SECR_APP_vSt_44_write_eeprom_values(ST_44_TYPE *st_44_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SECR_APP_vSt_44_write_eeprom_values(ST_44_TYPE *st_44_structure) {
	
	SM_APP_vChange_table_state(ST_44_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	for ( uint16_t i = 0; i < st_41.nbr_perm_used; i++ ) {
		
		EEPROM_writer(ST_44_ACCESS_TABLE_DEFAULT_ADDR + 2*i, &st_44_structure->access_control[i].access_table_def, 2);
		EEPROM_writer(ST_44_TABLE_ACCESS_PERMISSION_BFLD_READ_ADDR + i, &st_44_structure->access_control[i].read, 1);
		EEPROM_writer(ST_44_TABLE_ACCESS_PERMISSION_BFLD_WRITE_ADDR + i, &st_44_structure->access_control[i].write, 1);
		WATCHDOG_APP_vRestart();
	}
	
	SM_APP_vChange_table_state(ST_44_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_44_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_44_execute_table_reading(uint8_t *table_buffer) {
	
	SECR_APP_vSt_44_to_array(st_44, &table_buffer[0]);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool SECR_APP_bSt_44_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bSt_44_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint16_t k = 0;
	ST_44_TYPE st_44_temp;
	
	for ( uint16_t i = 0; i < st_41.nbr_perm_used; i++) {
		
		st_44_temp.access_control[i].access_table_def = table_buffer[k++];
		st_44_temp.access_control[i].access_table_def |= (table_buffer[k++] << 8);
		st_44_temp.access_control[i].read = table_buffer[k++];
		st_44_temp.access_control[i].write = table_buffer[k++];
	}
	
	is_ok = SECR_APP_bSt_44_validate_data(&st_44_temp);
	
	if ( is_ok ) {
		
		st_44 = st_44_temp;
		SECR_APP_vSt_44_write_eeprom_values(&st_44);
		uint16_t crc = SM_APP_usSt_44_ram_calculate_crc();
		SM_APP_vSt_44_write_crc(crc);
	}
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t SECR_APP_usSt_44_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t SECR_APP_usSt_44_get_size(void) {

	return (uint16_t)(4 * st_41.nbr_perm_used);
}

/** 
 ******************************************************************************
 * \fn bool SECR_APP_bCheck_access_to_read_table(SECURITY_USER_INFO_TYPE user_info)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bCheck_access_to_read_table(SECURITY_USER_INFO_TYPE user_info) {
	
	bool permissions_ok = false;
	uint8_t nbr_retries = 0;
	
	while( (nbr_retries < 3) && (permissions_ok == false) ) {
		
		char master_password[MAX_PASSWORD_LEN];
		strcpy((char*)master_password, MASTER_PASSWORD);
		
		if ( memcmp( (const char*)master_password, (const char*)user_info.password, MAX_PASSWORD_LEN) == 0 ) {
			permissions_ok = true;
		}
		else {
			
			/* Read default access permissions for all tables */
			EEPROM_reader(ST_43_ACCESS_TABLE_DEFAULT_ADDR, &access_control.access_table_def, 2);								
			EEPROM_reader(ST_43_TABLE_ACCESS_PERM_BFLD_READ_ADDR, &access_control.read, 1);
			access_control.read |= DEFAULT_TABLE_ACCESS_CTRL_READ;
			
			/* Search if requested table is defined on ST-44 */
			for ( uint16_t i = 0; i < st_41.nbr_perm_used; i++) {																
			
				if ( ( (st_44.access_control[i].access_table_def & PROC_FLAG_bm) == 0 )														&&
					 ( (st_44.access_control[i].access_table_def & TBL_PROC_NBR_bm) == (user_info.tbl_proc_id & TBL_PROC_NBR_bm) )			&&
					 ( (bool)(st_44.access_control[i].access_table_def & STD_VS_MFG_FLAG_bm) >> STD_VS_MFG_FLAG_bp) == user_info.mfg_flag   ){
					
					access_control = st_44.access_control[i];
					break;
				}
			}
			
			/* Read the UNRESTRICTED_READ bit from the ACCESS_TABLE_DEF bitfield */
			if ( (access_control.access_table_def & UNRESTRICTED_READ_bm) == UNRESTRICTED_READ_bm ) {					
				
				permissions_ok = true;
			}
			else {
				
				/* Check if password has the permissions to access this table. This compare the password groups and the read table associated groups */
				if ( (user_info.access_permissions & access_control.read) > 0 ) {												
				
					permissions_ok = true;
				}
			}
		}
		
		nbr_retries++;
	}
	
	return permissions_ok;
}

/** 
 ******************************************************************************
 * \fn bool SECR_APP_bCheck_access_to_write_table(SECURITY_USER_INFO_TYPE user_info)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bCheck_access_to_write_table(SECURITY_USER_INFO_TYPE user_info) {
	
	bool permissions_ok = false;
	uint8_t nbr_retries = 0;
	
	while( (nbr_retries < 3) && (permissions_ok == false) ) {
		
		char master_password[MAX_PASSWORD_LEN];
		strcpy((char*)master_password, MASTER_PASSWORD);
		
		if ( memcmp( (const char*)master_password, (const char*)user_info.password, MAX_PASSWORD_LEN) == 0 ) {
			permissions_ok = true;
		}
		else {
			
			/* Read default access permissions for all tables */
			EEPROM_reader(ST_43_ACCESS_TABLE_DEFAULT_ADDR, &access_control.access_table_def, 2);
			EEPROM_reader(ST_43_TABLE_ACCESS_PERM_BFLD_WRITE_ADDR, &access_control.write, 1);
			access_control.write |= DEFAULT_PROCEDURE_ACCESS_CTRL_WRITE;
			
			/* Search if requested table is defined on ST-44 */
			for ( uint16_t i = 0; i < st_41.nbr_perm_used; i++) {
				
				if ( ( (st_44.access_control[i].access_table_def & PROC_FLAG_bm) == 0 )															&&
					 ( (st_44.access_control[i].access_table_def & TBL_PROC_NBR_bm) == (user_info.tbl_proc_id & TBL_PROC_NBR_bm) )				&&
					 ( (bool)(st_44.access_control[i].access_table_def & STD_VS_MFG_FLAG_bm) >> STD_VS_MFG_FLAG_bp) == user_info.mfg_flag		){
					
					access_control = st_44.access_control[i];
					break;
				}
			}
			
			/* Read the UNRESTRICTED_WRITE bit from the ACCESS_TABLE_DEF bitfield */
			if ( (access_control.access_table_def & UNRESTRICTED_WRITE_bm) == UNRESTRICTED_WRITE_bm ) {
				
				permissions_ok = true;
			}
			else {
				
				/* Check if password has the permissions to access this table. This compare the password groups and the write table associated groups */
				if ( (user_info.access_permissions & access_control.write) > 0 ) {
					
					permissions_ok = true;
				}
			}
		}
		
		nbr_retries++;
	}
	
	return permissions_ok; 
}

/** 
 ******************************************************************************
 * \fn bool SECR_APP_bCheck_access_to_execute_procedure(SECURITY_USER_INFO_TYPE user_info)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
bool SECR_APP_bCheck_access_to_execute_procedure(SECURITY_USER_INFO_TYPE user_info) {
	
	 bool permissions_ok = false;
	 uint8_t nbr_retries = 0;
	 char master_password[MAX_PASSWORD_LEN];
	 uint16_t i;
	 
	 while( (nbr_retries < 3) && (permissions_ok == false) ) {
		 
		 strcpy((char*)master_password, MASTER_PASSWORD);
		 
		 if ( memcmp( (const char*)master_password, (const char*)user_info.password, MAX_PASSWORD_LEN) == 0 ) {
			 permissions_ok = true;
		 }
		 else {
			 
			 /* Read default access permissions for all tables */
			 EEPROM_reader(ST_43_ACCESS_PROCEDURE_DEFAULT_ADDR, &access_control.access_table_def, 2);
			 EEPROM_reader(ST_43_PROCEDURE_ACCESS_PERM_BFLD_WRITE_ADDR, &access_control.write, 1);
			 
			 /* Search if requested table is defined on ST-44 */
			 for ( i = 0; i < st_41.nbr_perm_used; i++ ) {
				 
				 if ( ( (st_44.access_control[i].access_table_def & PROC_FLAG_bm) == PROC_FLAG_bm )												&&
					  ( (st_44.access_control[i].access_table_def & TBL_PROC_NBR_bm) == (user_info.tbl_proc_id & TBL_PROC_NBR_bm) )				&&
					  ( (bool)(st_44.access_control[i].access_table_def & STD_VS_MFG_FLAG_bm) >> STD_VS_MFG_FLAG_bp) == user_info.mfg_flag		){
					 
					 access_control = st_44.access_control[i];
					 break;
				 }
			 }
			 
			 /* Read the UNRESTRICTED_WRITE bit from the ACCESS_TABLE_DEF bitfield */
			 if ( (access_control.access_table_def & UNRESTRICTED_WRITE_bm) == UNRESTRICTED_WRITE_bm ) {
				 
				 permissions_ok = true;
			 }
			 else {
				 
				 /* Check if password has the permissions to access this table. This compare the password groups and the write table associated groups */
				 if ( (user_info.access_permissions & access_control.write) > 0 ) {
					 
					 permissions_ok = true;
				 }
			 }
		 }
		 
		 nbr_retries++;
	 }
	 
	 return permissions_ok;
}

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-06-09
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */