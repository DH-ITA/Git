/**
 ******************************************************************************
 * \file gral.c
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
#include "sm.h"
#include "c1219.h"
#include "timer.h"
#include "evnt.h"
#include "op.h"
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
static uint8_t set_buff[MAX_NBR_OCTETS_SET];

/*
 ******************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ******************************************************************************
 */
 ST_3_TYPE st_3;

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
static bool GRAL_APP_bSt_3_validate_data(ST_3_TYPE *st_3_structure);
static void GRAL_APP_vSt_3_read_eeprom_values(ST_3_TYPE *st_3_structure);
static void GRAL_APP_vSt_3_write_eeprom_values(ST_3_TYPE *st_3_structure);


/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bSt_0_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bSt_0_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	uint8_t set_buff[MAX_NBR_OCTETS_SET];
	uint8_t EFEF[DIM_STD_TBLS_USED];
	
	table_buffer[k++] = FORMAT_CONTROL_1;
	table_buffer[k++] = FORMAT_CONTROL_2;
	table_buffer[k++] = FORMAT_CONTROL_3;
	
	memcpy(&table_buffer[k], MANUFACTURER_GRAL, 4);
	k += 4;
	
	table_buffer[k++] = NAMEPLATE_TYPE;
	table_buffer[k++] = DEFAULT_SET_USED;
	table_buffer[k++] = MAX_PROC_PARM_LENGTH;
	table_buffer[k++] = MAX_RESP_DATA_LEN;
	table_buffer[k++] = STD_VERSION_NO;
	table_buffer[k++] = STD_REVISION_NO;
	table_buffer[k++] = DIM_STD_TBLS_USED;
	table_buffer[k++] = DIM_MFG_TBLS_USED;
	table_buffer[k++] = DIM_STD_PROC_USED;
	table_buffer[k++] = DIM_MFG_PROC_USED;
	table_buffer[k++] = DIM_MFG_STATUS_USED;
	table_buffer[k++] = NBR_PENDING;
	
	memset(&set_buff, 0, sizeof(set_buff));
	GRAL_APP_vLoad_std_tbls_used(&set_buff[0]);
	memcpy( &table_buffer[k], &set_buff[0], DIM_STD_TBLS_USED );
	k += DIM_STD_TBLS_USED;
	
	memset(&set_buff, 0, sizeof(set_buff));
	GRAL_APP_vLoad_mfg_tbls_used(&set_buff[0]);
	memcpy( &table_buffer[k], &set_buff[0], DIM_MFG_TBLS_USED );
	k += DIM_MFG_TBLS_USED;
	
	memset(&set_buff, 0, sizeof(set_buff));
	GRAL_APP_vLoad_std_proc_used(&set_buff[0]);
	memcpy( &table_buffer[k], &set_buff[0], DIM_STD_PROC_USED );
	k += DIM_STD_PROC_USED;
	
	memset(&set_buff, 0, sizeof(set_buff));
	GRAL_APP_vLoad_mfg_proc_used(&set_buff[0]);
	memcpy( &table_buffer[k], &set_buff[0], DIM_MFG_PROC_USED );
	k += DIM_MFG_PROC_USED;

	memset(&set_buff, 0, sizeof(set_buff));
	GRAL_APP_vLoad_std_tbls_write(&set_buff[0]);
	memcpy( &table_buffer[k], &set_buff[0], DIM_STD_TBLS_USED );
	k += DIM_STD_TBLS_USED;
	
	memset(&set_buff, 0, sizeof(set_buff));
	GRAL_APP_vLoad_mfg_tbls_write(&set_buff[0]);
	memcpy( &table_buffer[k], &set_buff[0], DIM_MFG_TBLS_USED );
	k += DIM_MFG_TBLS_USED;
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t GRAL_APP_usSt_0_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t GRAL_APP_usSt_0_get_size(void) {

	uint16_t table_size;
	
	table_size = 19;
	table_size += DIM_STD_TBLS_USED;
	table_size += DIM_MFG_TBLS_USED;
	table_size += DIM_STD_PROC_USED;
	table_size += DIM_MFG_PROC_USED;
	table_size += DIM_STD_TBLS_USED;
	table_size += DIM_MFG_TBLS_USED;
	
	return table_size;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bLoad_st_1(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bLoad_st_1(void) {
	
	if ( SM_APP_ucGet_table_state(ST_1_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		GRAL_APP_bSt_1_restore_default_values();
	}
	else {
		// There is no shadow ram for this table
	}
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bSt_1_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bSt_1_restore_default_values(void) {
	
	uint8_t data_1[32];
	
	SM_APP_vChange_table_state(ST_1_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	memset(&data_1, 0, 4);
	strcpy((char*)&data_1, MANUFACTURER_NAME);
	EEPROM_writer( ST_1_MANUFACTURER_ADDR, &data_1, 4);

	memset(data_1, 0, sizeof(data_1));
	strcpy((char*)&data_1, ED_MODEL);
	EEPROM_writer( ST_1_ED_MODEL_ADDR, &data_1, 8);
	
	memset(data_1, 0, sizeof(data_1));
	EEPROM_writer( ST_1_HW_VERSION_NUMBER_ADDR, &data_1, 1);
	EEPROM_writer( ST_1_HW_REVISION_NUMBER_ADDR, &data_1, 1);
	
	data_1[0] = FW_VERSION_NUMBER;
	EEPROM_writer( ST_1_FW_VERSION_NUMBER_ADDR, &data_1[0], 1);
	data_1[0] = FW_REVISION_NUMBER;
	EEPROM_writer( ST_1_FW_REVISION_NUMBER_ADDR, &data_1[0], 1);
	
	strcpy((char*)&data_1, "0000000000000000");
	EEPROM_writer( ST_1_SERIAL_NUMBER_ADDR, &data_1[0], 16);
	
	SM_APP_vChange_table_state(ST_1_PROG_STATE_ADDR, C1219_TABLE_FULL);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bSt_1_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bSt_1_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	
	delay_ms(10);
	EEPROM_reader( ST_1_START_ADDR, &table_buffer[0], ST_1_SIZE);
	memcpy((char*)&table_buffer[0], MANUFACTURER_NAME, 4);
	memcpy((char*)&table_buffer[4], ED_MODEL, 4);
	table_buffer[14] = (uint8_t)FW_VERSION_NUMBER;
	table_buffer[15] = (uint8_t)FW_REVISION_NUMBER;
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bSt_1_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bSt_1_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;

	EEPROM_writer( ST_1_START_ADDR, &table_buffer[0], ST_1_SIZE);
	
	is_ok = true;
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t GRAL_APP_usSt_1_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t GRAL_APP_usSt_1_get_size(void) {

	return (uint16_t)ST_1_SIZE;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bSt_2_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bSt_2_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	char E_KH[6];
	
	if ( PULSE_CONSTANT_PKT == 0.1 ) {

		memcpy(E_KH, "0.1   ", 6);
	}
	else if ( PULSE_CONSTANT_PKT == 0.2 ) {

		memcpy(E_KH, "0.2   ", 6);
	}
	else if ( PULSE_CONSTANT_PKT == 0.3 ) {

		memcpy(E_KH, "0.3   ", 6);
	}
	else if ( PULSE_CONSTANT_PKT == 0.4 ) {

		memcpy(E_KH, "0.4   ", 6);
	}
	else if ( PULSE_CONSTANT_PKT == 0.5 ) {

		memcpy(E_KH, "0.5   ", 6);
	}
	else {

		memcpy(E_KH, "      ", 6);
	}
	
	memcpy(&table_buffer[k], E_KH, 6);
	k += 6;
	
	memcpy(&table_buffer[k], E_KH, 6);
	k += 6;
	
	table_buffer[k++] = E_INPUT_SCALAR;
	memcpy(&table_buffer[k], E_ED_CONFIG, 5);
	k += 5;
	
	table_buffer[k++] = (uint8_t) (E_ELEMENTS);
	table_buffer[k++] = (uint8_t)(E_ELEMENTS  >> 8);
	table_buffer[k++] = E_VOLTS;
	memcpy(&table_buffer[k], E_CLASS_MAX_AMPS, 6);
	k += 6;
	
	memcpy(&table_buffer[k], E_TA, 6);
	k += 6;
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t GRAL_APP_usSt_2_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t GRAL_APP_usSt_2_get_size(void) {

	return (uint16_t)ST_2_SIZE;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bLoad_st_3(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bLoad_st_3(void) {
	
	bool is_ok = false;
	
	if ( SM_APP_ucGet_table_state(ST_3_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		
		is_ok = GRAL_APP_bSt_3_restore_default_values();
	}
	else {
		
		is_ok = GRAL_APP_bSt_3_refresh_ram();
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bSt_3_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bSt_3_restore_default_values(void) {
	
	bool is_ok = false;
	ST_3_TYPE st_3_temp;
	
	memset(&st_3_temp, 0, sizeof(st_3_temp));
	st_3_temp.ed_mode = METERING_FLAG_bm;
	
	is_ok = GRAL_APP_bSt_3_validate_data(&st_3_temp);
	
	st_3 = st_3_temp;
	GRAL_APP_vSt_3_write_eeprom_values(&st_3);
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bSt_3_refresh_ram(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bSt_3_refresh_ram(void) {
	
	ST_3_TYPE st_3_temp;
	bool is_ok = false;
	
	for (uint8_t i = 0; i < 3; i++) {
		
		GRAL_APP_vSt_3_read_eeprom_values(&st_3_temp);
		is_ok = GRAL_APP_bSt_3_validate_data(&st_3_temp);
		
		if ( is_ok ) {
			st_3 = st_3_temp;
			break;
		}
	}
	
	if ( !is_ok ) {
		st_3 = st_3_temp;
		GRAL_APP_vSt_3_write_eeprom_values(&st_3);
	}
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bSt_3_validate_data(ST_3_TYPE *st_3_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bSt_3_validate_data(ST_3_TYPE *st_3_structure) {
	
	return true;
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vSt_3_read_eeprom_values(ST_3_TYPE *st_3_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vSt_3_read_eeprom_values(ST_3_TYPE *st_3_structure) {	
	
	EEPROM_reader(ST_3_ED_MODE_ADDR, &st_3_structure->ed_mode, 1);
	EEPROM_reader(ST_3_ED_STD_STATUS1_ADDR, &st_3_structure->ed_std_status1, 2);
	EEPROM_reader(ST_3_ED_STD_STATUS2_ADDR, &st_3_structure->ed_std_status2, 1);
	EEPROM_reader(ST_3_ED_MFG_STATUS_ADDR, &st_3_structure->ed_mfg_status, DIM_MFG_STATUS_USED);
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vSt_3_write_eeprom_values(ST_3_TYPE *st_3_structure)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vSt_3_write_eeprom_values(ST_3_TYPE *st_3_structure)
{
	SM_APP_vChange_table_state(ST_3_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	EEPROM_writer( ST_3_ED_MODE_ADDR, &st_3_structure->ed_mode, 1 );
	EEPROM_writer( ST_3_ED_STD_STATUS1_ADDR, &st_3_structure->ed_std_status1, 2 );
	EEPROM_writer( ST_3_ED_STD_STATUS1_ADDR, &st_3_structure->ed_std_status2, 1 );
	EEPROM_writer( ST_3_ED_MFG_STATUS_ADDR, &st_3_structure->ed_mfg_status, sizeof(st_3_structure->ed_mfg_status) );
	
	SM_APP_vChange_table_state(ST_3_PROG_STATE_ADDR, C1219_TABLE_FULL);
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bSt_3_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bSt_3_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	uint8_t k = 0;
	
	table_buffer[k++] = st_3.ed_mode;
	st_3.ed_std_status1 &= ED_STD_STATUS1_SUPPORTED_FLAGS_bm;
	table_buffer[k++] = (uint8_t)(st_3.ed_std_status1 >> 0);
	table_buffer[k++] = (uint8_t)(st_3.ed_std_status1 >> 8);
	table_buffer[k++] = st_3.ed_std_status2;
	memcpy( &table_buffer[k], &st_3.ed_mfg_status, sizeof(st_3.ed_mfg_status) );
	k += sizeof(st_3.ed_mfg_status);

// 	GRAL_APP_vClear_st_3_ed_mfg_status();
// 	NIC_APP_vDeassert_attention_request();
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn uint16_t GRAL_APP_usSt_3_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t GRAL_APP_usSt_3_get_size(void) {

	return 	(uint16_t)ST_3_SIZE;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bLoad_st_6(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bLoad_st_6(void) {
	
 	if ( SM_APP_ucGet_table_state(ST_6_PROG_STATE_ADDR) != C1219_TABLE_FULL ) {
		GRAL_APP_bSt_6_restore_default_values();
	}
	else {	
		// There is no shadow ram for this table
	}
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bSt_6_restore_default_values(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bSt_6_restore_default_values(void) {
	
	uint8_t data[30];
	
	SM_APP_vChange_table_state(ST_6_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	
	memset(&data, 0, 4);
	
	strcpy((char*)&data, "                    ");
	EEPROM_writer( ST_6_OWNER_NAME_ADDR, &data, 20);
	EEPROM_writer( ST_6_UTILITY_DIV_ADDR, &data, 20);
	EEPROM_writer( ST_6_SERVICE_POINT_ID_ADDR, &data, 20);
	EEPROM_writer( ST_6_ELECT_ADDR_ADDR, &data, 20);
	EEPROM_writer( ST_6_DEVICE_ID_ADDR, &data, 20);
	EEPROM_writer( ST_6_UTIL_SER_NO_ADDR, &data, 20);
	EEPROM_writer( ST_6_CUSTOMER_ID_ADDR, &data, 20);
	
	memset(&data, 0, sizeof(data));
	EEPROM_writer( ST_6_COORDINATE_1_ADDR, &data, 10);
	EEPROM_writer( ST_6_COORDINATE_2_ADDR, &data, 10);
	EEPROM_writer( ST_6_COORDINATE_3_ADDR, &data, 10);
	
	strcpy((char*)&data, "        ");
	EEPROM_writer( ST_6_TARIFF_ID_ADDR, &data, 8);
	
	memset(&data, 0, sizeof(data));
	data[0] = 0x6D;
	data[1] = 0x6D;
	data[2] = 0x54;
	data[3] = 0x20;
	EEPROM_writer( ST_6_EX1_SW_VENDOR_ADDR, &data, 4);
	
	memset(&data, 0, sizeof(data));
	data[0] = 0x01;
	EEPROM_writer( ST_6_EX1_SW_VERSION_NUMBER_ADDR, &data, 1);
	
	memset(&data, 0, sizeof(data));
	data[0] = 0x00;
	EEPROM_writer( ST_6_EX1_SW_REVISION_NUMBER_ADDR, &data, 1);
	
	strcpy((char*)&data, "    ");
	EEPROM_writer( ST_6_EX2_SW_VENDOR_ADDR, &data, 4);
	
	memset(&data, 0, sizeof(data));
	EEPROM_writer( ST_6_EX2_SW_VERSION_NUMBER_ADDR, &data, 1);
	EEPROM_writer( ST_6_EX2_SW_REVISION_NUMBER_ADDR, &data, 1);
	
	strcpy((char*)&data, "                              ");
	EEPROM_writer( ST_6_PROGRAMMER_NAME_ADDR, &data, 10);
	EEPROM_writer( ST_6_MISC_ID_ADDR, &data, 30);
	
	SM_APP_vChange_table_state(ST_6_PROG_STATE_ADDR, C1219_TABLE_FULL);
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bSt_6_execute_table_reading(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bSt_6_execute_table_reading(uint8_t *table_buffer) {
	
	bool is_ok = false;
	
	EEPROM_reader( ST_6_START_ADDR, &table_buffer[0], ST_6_SIZE);
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bSt_6_execute_table_writing(uint8_t *table_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bSt_6_execute_table_writing(uint8_t *table_buffer) {
	
	bool is_ok = false;
	
	SM_APP_vChange_table_state(ST_6_PROG_STATE_ADDR, C1219_TABLE_EMPTY);
	EEPROM_writer(ST_6_START_ADDR, &table_buffer[0], ST_6_SIZE);
	SM_APP_vChange_table_state(ST_6_PROG_STATE_ADDR, C1219_TABLE_FULL);
	
	is_ok = true;
	
	return is_ok;	
}

/**
 ******************************************************************************
 * \fn uint16_t GRAL_APP_usSt_6_get_size(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t GRAL_APP_usSt_6_get_size(void) {

	return (uint16_t)ST_6_SIZE;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bEnable_metering_mode(uint16_t user_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bEnable_metering_mode(uint16_t user_id, Uart* uart) {
	
	if ( (st_3.ed_mode & METERING_FLAG_bm) == 0 ) {

		uint32_t timestamp = rtc.utc_timestamp;
		
		if ( (st_3.ed_mode & TEST_MODE_FLAG_bm) == TEST_MODE_FLAG_bm ) {
			
			/* Disable test mode */
			TIMER_reset_ms_timer(SM_TEST_MODE_TIMER);
			EVNT_APP_vRegister_event(TEST_MODE_STOPPED, 0, true, user_id, timestamp);
			
			if ( uart == OP_UART ) {
				EVNT_APP_vRegister_event(TEST_MODE_STOPPED_OP, 0, false, user_id, timestamp);
			}
			
			if ( uart == NIC_UART) {
				EVNT_APP_vRegister_event(TEST_MODE_STOPPED_NIC, 0, false, user_id, timestamp);
			}
			
			REGS_APP_vDisable_test_mode();
		}
		
		if ( (st_3.ed_mode & METER_SHOP_MODE_FLAG_bm) == METER_SHOP_MODE_FLAG_bm ) {
			
			/* Disable meter shop mode */
			EVNT_APP_vRegister_event(METER_SHOP_MODE_STOPPED, 0, true, user_id, timestamp);
		}
		
		/* Register metering mode is enabled */
		EVNT_APP_vRegister_event(METERING_MODE_STARTED, 0, true, user_id, timestamp);
		st_3.ed_mode = (uint8_t)(METERING_FLAG_bm);
	}
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bEnable_test_mode(uint16_t user_id, Uart* uart)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bEnable_test_mode(uint16_t user_id, Uart* uart) {
	
	if ( (st_3.ed_mode & TEST_MODE_FLAG_bm) == 0 ) {
		
		uint32_t timestamp = rtc.utc_timestamp;
		
		if ( (st_3.ed_mode & METERING_FLAG_bm) == METERING_FLAG_bm ) {
			
			/* Disable metering mode */
			EVNT_APP_vRegister_event(METERING_MODE_STOPPED, 0, true, user_id, timestamp);
			REGS_APP_vEnable_test_mode();
		}
		
		if ( (st_3.ed_mode & METER_SHOP_MODE_FLAG_bm) == METER_SHOP_MODE_FLAG_bm ) {
			
			/* Disable meter shop mode */
			EVNT_APP_vRegister_event(METER_SHOP_MODE_STOPPED, 0, true, user_id, timestamp);
		}
		
		/* Register test mode is enabled */
		EVNT_APP_vRegister_event(TEST_MODE_STARTED, 0, true, user_id, timestamp);
		
		if ( uart == OP_UART ) {
			EVNT_APP_vRegister_event(TEST_MODE_STARTED_OP, 0, false, user_id, rtc.utc_timestamp);
		}
		
		if ( uart == NIC_UART ) {
			EVNT_APP_vRegister_event(TEST_MODE_STARTED_NIC, 0, false, user_id, rtc.utc_timestamp);
		}
		
		st_3.ed_mode = (uint8_t)(TEST_MODE_FLAG_bm);
		TIMER_start_ms_timer(SM_TEST_MODE_TIMER, meter.ed_mode.test_mode.active_period, SM_APP_vSet_test_mode_expired_flag);
	}
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bEnable_meter_shop_mode(uint16_t user_id)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bEnable_meter_shop_mode(uint16_t user_id) {
	
	if ( (st_3.ed_mode & METER_SHOP_MODE_FLAG_bm) == 0 ) {
		
		uint32_t timestamp = rtc.utc_timestamp;
		
		if ( (st_3.ed_mode & METERING_FLAG_bm) == METERING_FLAG_bm ) {
			
			/* Disable metering mode */
			EVNT_APP_vRegister_event(METERING_MODE_STOPPED, 0, true, user_id, timestamp);
		}
		
		if ( (st_3.ed_mode & TEST_MODE_FLAG_bm) == TEST_MODE_FLAG_bm ) {
			
			/* Disable meter shop mode */
			EVNT_APP_vRegister_event(METER_SHOP_MODE_STOPPED, 0, true, user_id, timestamp);
		}
		
		/* Register test mode is enabled */
		EVNT_APP_vRegister_event(METER_SHOP_MODE_STARTED, 0, true, user_id, timestamp);
		st_3.ed_mode = (uint8_t)(METER_SHOP_MODE_FLAG_bm);
		
		TIMER_start_ms_timer(SM_TEST_MODE_TIMER, meter.ed_mode.test_mode.active_period, SM_APP_vSet_test_mode_expired_flag);
	}
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bEnable_factory_mode(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bEnable_factory_mode(void) {
	
	st_3.ed_mode |= FACTORY_FLAG_bm;
	
	return true;
}

/**
 ******************************************************************************
 * \fn bool GRAL_APP_bEnable_factory_mode(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool GRAL_APP_bDisable_factory_mode(void) {
	
	st_3.ed_mode &= ~FACTORY_FLAG_bm;
	REGS_APP_vClear_st_23();
	
	return true;
}

/**
 ******************************************************************************
 * \fn uint8_t GRAL_APP_ucGet_st_3_ed_mode(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t GRAL_APP_ucGet_st_3_ed_mode(void) {
	
	return st_3.ed_mode;
}

/**
 ******************************************************************************
 * \fn uint16_t GRAL_APP_usGet_st_3_ed_std_status1(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint16_t GRAL_APP_usGet_st_3_ed_std_status1(void) {
	
	return st_3.ed_std_status1;
}

/**
 ******************************************************************************
 * \fn uint8_t GRAL_APP_ucGet_st_3_ed_std_status2(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
uint8_t GRAL_APP_ucGet_st_3_ed_std_status2(void) {
	
	return st_3.ed_std_status2;
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vCopy_st_3_ed_mfg_status(uint8_t *buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vCopy_st_3_ed_mfg_status(uint8_t *buffer) {
	
	memcpy(buffer, &st_3.ed_mfg_status, (size_t)DIM_MFG_STATUS_USED);
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vClear_st_3_ed_std_status1(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vClear_st_3_ed_std_status1(void) {
	
	st_3.ed_std_status1 = 0;
	EEPROM_writer(ST_3_ED_STD_STATUS1_ADDR, &st_3.ed_std_status1, 2);
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vClear_st_3_ed_std_status2(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vClear_st_3_ed_std_status2(void) {
	
	st_3.ed_std_status2 = 0;
	EEPROM_writer(ST_3_ED_STD_STATUS2_ADDR, &st_3.ed_std_status2, 1);
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vClear_st_3_ed_mfg_status(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vClear_st_3_ed_mfg_status(void) {
	
	memset(&st_3.ed_mfg_status, 0 , sizeof(st_3.ed_mfg_status));
	EEPROM_writer(ST_3_ED_MFG_STATUS_ADDR, &st_3.ed_mfg_status, DIM_MFG_STATUS_USED);
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vStore_st_3_ed_std_status1(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vStore_st_3_ed_std_status1(void) {
	
	EEPROM_writer(ST_3_ED_STD_STATUS1_ADDR, &st_3.ed_std_status1, 2);
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vStore_st_3_ed_std_status2(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vStore_st_3_ed_std_status2(void) {
	
	EEPROM_writer(ST_3_ED_STD_STATUS2_ADDR, &st_3.ed_std_status2, 1);
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vStore_st_3_ed_mfg_status(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vStore_st_3_ed_mfg_status(void) {
	
	EEPROM_writer(ST_3_ED_MFG_STATUS_ADDR, &st_3.ed_mfg_status, DIM_MFG_STATUS_USED);
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vSet_flag_st_3_ed_std_status1(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vSet_flag_st_3_ed_std_status1(ED_STD_STATUS1_FLAGS_TYPE flag) {
	
	st_3.ed_std_status1 |= (uint16_t)(1 << flag);
	st_3.ed_std_status1 &= ED_STD_STATUS1_SUPPORTED_FLAGS_bm;
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vReset_flag_st_3_ed_std_status1(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vReset_flag_st_3_ed_std_status1(ED_STD_STATUS1_FLAGS_TYPE flag) {
	
	st_3.ed_std_status1 |= (uint16_t)(0 << flag);
	st_3.ed_std_status1 &= ED_STD_STATUS1_SUPPORTED_FLAGS_bm;
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vLoad_std_tbls_used(uint8_t* set_buff)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vLoad_std_tbls_used(uint8_t* set_buff) {
	
	*set_buff = 0;
	
	#ifdef ST_0_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_1_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_2_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_3_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_4_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_5_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_6_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_7_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_8_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_9_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_10_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_11_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_12_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_13_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_14_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_15_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_16_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_17_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_18_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_19_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_20_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_21_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_22_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_23_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_24_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_25_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_26_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_27_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_28_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_29_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_30_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_31_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_32_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_33_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_34_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_35_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_36_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_37_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_38_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_39_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_40_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_41_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_42_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_43_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_44_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_45_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_46_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_47_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_48_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_49_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_50_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_51_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_52_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_53_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_54_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_55_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_56_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_57_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_58_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_59_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_60_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_61_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_62_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_63_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_64_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_65_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_66_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_67_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_68_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_69_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_70_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_71_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_72_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_73_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_74_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_75_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_76_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_77_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_78_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_79_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_80_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_81_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_82_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_83_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_84_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_85_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_86_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_87_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_88_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_89_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_90_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_91_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_92_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_93_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_94_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_95_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_96_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_97_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_98_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_99_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_100_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_101_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_102_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_103_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_104_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_105_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_106_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_107_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_108_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_109_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_110_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_111_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_112_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef ST_113_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef ST_114_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef ST_115_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef ST_116_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef ST_117_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef ST_118_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef ST_119_ID
	*set_buff |= ( 1 << 7 );
	#endif
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vLoad_mfg_tbls_used(uint8_t* set_buff)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vLoad_mfg_tbls_used(uint8_t* set_buff) {
	
	*set_buff = 0;
	
	#ifdef MT_0_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_1_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_2_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_3_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_4_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_5_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_6_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_7_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_8_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_9_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_10_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_11_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_12_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_13_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_14_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_15_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_16_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_17_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_18_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_19_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_20_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_21_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_22_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_23_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_24_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_25_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_26_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_27_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_28_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_29_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_30_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_31_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_32_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_33_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_34_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_35_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_36_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_37_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_38_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_39_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_40_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_41_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_42_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_43_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_44_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_45_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_46_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_47_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_48_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_49_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_50_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_51_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_52_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_53_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_54_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_55_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_56_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_57_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_58_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_59_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_60_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_61_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_62_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_63_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_64_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_65_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_66_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_67_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_68_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_69_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_70_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_71_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_72_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_73_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_74_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_75_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_76_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_77_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_78_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_79_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_80_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_81_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_82_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_83_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_84_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_85_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_86_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_87_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_88_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_89_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_90_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_91_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_92_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_93_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_94_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_95_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_96_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_97_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_98_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_99_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_100_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_101_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_102_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_103_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_104_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_105_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_106_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_107_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_108_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_109_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_110_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_111_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_112_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_113_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_114_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_115_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_116_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_117_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_118_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_119_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_120_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_121_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_122_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_123_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_124_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_125_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_126_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_127_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_128_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_129_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_130_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_131_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_132_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_133_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_134_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_135_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_136_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_137_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_138_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_139_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_140_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_141_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_142_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_143_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_144_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_145_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_146_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_147_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_148_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_149_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_150_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_151_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_152_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_153_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_154_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_155_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_156_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_157_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_158_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_159_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_160_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_161_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_162_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_163_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_164_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_165_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_166_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_167_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_168_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_169_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_170_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_171_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_172_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_173_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_174_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_175_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_176_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_177_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_178_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_179_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_180_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_181_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_182_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_183_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_184_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_185_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_186_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_187_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_188_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_189_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_190_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_191_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_192_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_193_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_194_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_195_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_196_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_197_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_198_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_199_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_200_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_201_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_202_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_203_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_204_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_205_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_206_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_207_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_208_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_209_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_210_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_211_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_212_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_213_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_214_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_215_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_216_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_217_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_218_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_219_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_220_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_221_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_222_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_223_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_224_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_225_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_226_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_227_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_228_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_229_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_230_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_231_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_232_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_233_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_234_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_235_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_236_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_237_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_238_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_239_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_240_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_241_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_242_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_243_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_244_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_245_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_246_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_247_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_248_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_249_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_250_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_251_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_252_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_253_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_254_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_255_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_256_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_257_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_258_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_259_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_260_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_261_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_262_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_263_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_264_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef MT_265_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef MT_266_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef MT_267_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef MT_268_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef MT_269_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef MT_270_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef MT_271_ID
	*set_buff |= ( 1 << 7 );
	#endif
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vLoad_std_proc_used(uint8_t* set_buff)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vLoad_std_proc_used(uint8_t* set_buff) {
	
	*set_buff = 0;
	
	#ifdef SP_0_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef SP_1_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef SP_2_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef SP_3_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef SP_4_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef SP_5_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef SP_6_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef SP_7_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef SP_8_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef SP_9_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef SP_10_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef SP_11_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef SP_12_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef SP_13_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef SP_14_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef SP_15_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef SP_16_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef SP_17_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef SP_18_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef SP_19_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef SP_20_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef SP_21_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef SP_22_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef SP_23_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef SP_24_ID
	*set_buff |= ( 1 << 0 );
	#endif
	
	#ifdef SP_25_ID
	*set_buff |= ( 1 << 1 );
	#endif
	
	#ifdef SP_26_ID
	*set_buff |= ( 1 << 2 );
	#endif
	
	#ifdef SP_27_ID
	*set_buff |= ( 1 << 3 );
	#endif
	
	#ifdef SP_28_ID
	*set_buff |= ( 1 << 4 );
	#endif
	
	#ifdef SP_29_ID
	*set_buff |= ( 1 << 5 );
	#endif
	
	#ifdef SP_30_ID
	*set_buff |= ( 1 << 6 );
	#endif
	
	#ifdef SP_31_ID
	*set_buff |= ( 1 << 7 );
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef SP_32_ID
	*set_buff |= ( 1 << 0 );
	#endif
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vLoad_mfg_proc_used(uint8_t* set_buff)
 * \brief
 *
 * \param void
 * \retval void
- ******************************************************************************
 */
void GRAL_APP_vLoad_mfg_proc_used(uint8_t* set_buff) {
	
		*set_buff = 0;
		
		#ifdef MP_0_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_1_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_2_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_3_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_4_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_5_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_6_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_7_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_8_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_9_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_10_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_11_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_12_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_13_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_14_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_15_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_16_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_17_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_18_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_19_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_20_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_21_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_22_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_23_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_24_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_25_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_26_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_27_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_28_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_29_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_30_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_31_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_32_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_33_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_34_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_35_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_36_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_37_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_38_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_39_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_40_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_41_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_42_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_43_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_44_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_45_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_46_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_47_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_48_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_49_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_50_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_51_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_52_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_53_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_54_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_55_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_56_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_57_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_58_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_59_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_60_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_61_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_62_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_63_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_64_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_65_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_66_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_67_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_68_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_69_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_70_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_71_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_72_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_73_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_74_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_75_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_76_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_77_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_78_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_79_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_80_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_81_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_82_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_83_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_84_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_85_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_86_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_87_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_88_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_89_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_90_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_91_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_92_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_93_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_94_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_95_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_96_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_97_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_98_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_99_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_100_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_101_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_102_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_103_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_104_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_105_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_106_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_107_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_108_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_109_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_110_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_111_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_112_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_113_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_114_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_115_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_116_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_117_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_118_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_119_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_120_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_121_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_122_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_123_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_124_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_125_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_126_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_127_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_128_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_129_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_130_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_131_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_132_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_133_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_134_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_135_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_136_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_137_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_138_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_139_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_140_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_141_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_142_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_143_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_144_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_145_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_146_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_147_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_148_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_149_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_150_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_151_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_152_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_153_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_154_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_155_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_156_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_157_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_158_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_159_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_160_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_161_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_162_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_163_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_164_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_165_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_166_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_167_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_168_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_169_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_170_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_171_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_172_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_173_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_174_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_175_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_176_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_177_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_178_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_179_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_180_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_181_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_182_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_183_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_184_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_185_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_186_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_187_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_188_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_189_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_190_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_191_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_192_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_193_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_194_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_195_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_196_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_197_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_198_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_199_ID
		*set_buff |= ( 1 << 7 );
		#endif
		
		set_buff++;
		*set_buff = 0;
		
		#ifdef MP_200_ID
		*set_buff |= ( 1 << 0 );
		#endif
		
		#ifdef MP_201_ID
		*set_buff |= ( 1 << 1 );
		#endif
		
		#ifdef MP_202_ID
		*set_buff |= ( 1 << 2 );
		#endif
		
		#ifdef MP_203_ID
		*set_buff |= ( 1 << 3 );
		#endif
		
		#ifdef MP_204_ID
		*set_buff |= ( 1 << 4 );
		#endif
		
		#ifdef MP_205_ID
		*set_buff |= ( 1 << 5 );
		#endif
		
		#ifdef MP_206_ID
		*set_buff |= ( 1 << 6 );
		#endif
		
		#ifdef MP_207_ID
		*set_buff |= ( 1 << 7 );
		#endif
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vLoad_std_tbls_write(uint8_t* set_buff)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vLoad_std_tbls_write(uint8_t* set_buff) {
	
	*set_buff = 0;
	
	#ifdef ST_0_ID
	if ( ST_0_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_1_ID
	if ( ST_1_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_2_ID
	if ( ST_2_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_3_ID
	if ( ST_3_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_4_ID
	if ( ST_4_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_5_ID
	if ( ST_5_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_6_ID
	if ( ST_6_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_7_ID
	if ( ST_7_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_8_ID
	if ( ST_8_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_9_ID
	if ( ST_9_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_10_ID
	if ( ST_10_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_11_ID
	if ( ST_11_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_12_ID
	if ( ST_12_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_13_ID
	if ( ST_13_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_14_ID
	if ( ST_14_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_15_ID
	if ( ST_15_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_16_ID
	if ( ST_16_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_17_ID
	if ( ST_17_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_18_ID
	if ( ST_18_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_19_ID
	if ( ST_19_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_20_ID
	if ( ST_20_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_21_ID
	if ( ST_21_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_22_ID
	if ( ST_22_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_23_ID
	if ( ST_23_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_24_ID
	if ( ST_24_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_25_ID
	if ( ST_25_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_26_ID
	if ( ST_26_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_27_ID
	if ( ST_27_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_28_ID
	if ( ST_28_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_29_ID
	if ( ST_29_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_30_ID
	if ( ST_30_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_31_ID
	if ( ST_31_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_32_ID
	if ( ST_32_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_33_ID
	if ( ST_33_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_34_ID
	if ( ST_34_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_35_ID
	if ( ST_35_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_36_ID
	if ( ST_36_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_37_ID
	if ( ST_37_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_38_ID
	if ( ST_38_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_39_ID
	if ( ST_39_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_40_ID
	if ( ST_40_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_41_ID
	if ( ST_41_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_42_ID
	if ( ST_42_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_43_ID
	if ( ST_43_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_44_ID
	if ( ST_44_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_45_ID
	if ( ST_45_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_46_ID
	if ( ST_46_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_47_ID
	if ( ST_47_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_48_ID
	if ( ST_48_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_49_ID
	if ( ST_49_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_50_ID
	if ( ST_50_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_51_ID
	if ( ST_51_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_52_ID
	if ( ST_52_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_53_ID
	if ( ST_53_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_54_ID
	if ( ST_54_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_55_ID
	if ( ST_55_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_56_ID
	if ( ST_56_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_57_ID
	if ( ST_57_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_58_ID
	if ( ST_58_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_59_ID
	if ( ST_59_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_60_ID
	if ( ST_60_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_61_ID
	if ( ST_61_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_62_ID
	if ( ST_62_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_63_ID
	if ( ST_63_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_64_ID
	if ( ST_64_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_65_ID
	if ( ST_65_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_66_ID
	if ( ST_66_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_67_ID
	if ( ST_67_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_68_ID
	if ( ST_68_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_69_ID
	if ( ST_69_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_70_ID
	if ( ST_70_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_71_ID
	if ( ST_71_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_72_ID
	if ( ST_72_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_73_ID
	if ( ST_73_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_74_ID
	if ( ST_74_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_75_ID
	if ( ST_75_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_76_ID
	if ( ST_76_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_77_ID
	if ( ST_77_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_78_ID
	if ( ST_78_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_79_ID
	if ( ST_79_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_80_ID
	if ( ST_80_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_81_ID
	if ( ST_81_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_82_ID
	if ( ST_82_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_83_ID
	if ( ST_83_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_84_ID
	if ( ST_84_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_85_ID
	if ( ST_85_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_86_ID
	if ( ST_86_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_87_ID
	if ( ST_87_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_88_ID
	if ( ST_88_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_89_ID
	if ( ST_89_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_90_ID
	if ( ST_90_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_91_ID
	if ( ST_91_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_92_ID
	if ( ST_92_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_93_ID
	if ( ST_93_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_94_ID
	if ( ST_94_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_95_ID
	if ( ST_95_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_96_ID
	if ( ST_96_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_97_ID
	if ( ST_97_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_98_ID
	if ( ST_98_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_99_ID
	if ( ST_99_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_100_ID
	if ( ST_100_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_101_ID
	if ( ST_101_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_102_ID
	if ( ST_102_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_103_ID
	if ( ST_103_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_104_ID
	if ( ST_104_WRITE == TRUE )
	{
	*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_105_ID
	if ( ST_105_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_106_ID
	if ( ST_106_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_107_ID
	if ( ST_107_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_108_ID
	if ( ST_108_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_109_ID
	if ( ST_109_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_110_ID
	if ( ST_110_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_111_ID
	if ( ST_111_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef ST_112_ID
	if ( ST_112_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef ST_113_ID
	if ( ST_113_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef ST_114_ID
	if ( ST_114_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef ST_115_ID
	if ( ST_115_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef ST_116_ID
	if ( ST_116_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef ST_117_ID
	if ( ST_117_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef ST_118_ID
	if ( ST_118_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef ST_119_ID
	if ( ST_119_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
}

/**
 ******************************************************************************
 * \fn void GRAL_APP_vLoad_mfg_tbls_write(uint8_t* set_buff)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void GRAL_APP_vLoad_mfg_tbls_write(uint8_t* set_buff) {
	
	*set_buff = 0;
	
	#ifdef MT_0_ID
	if ( MT_0_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_1_ID
	if ( MT_1_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_2_ID
	if ( MT_2_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_3_ID
	if ( MT_3_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_4_ID
	if ( MT_4_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_5_ID
	if ( MT_5_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_3_ID
	if ( MT_3_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_7_ID
	if ( MT_7_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_8_ID
	if ( MT_8_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_9_ID
	if ( MT_9_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_23_ID
	if ( MT_23_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_11_ID
	if ( MT_11_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_12_ID
	if ( MT_12_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_13_ID
	if ( MT_13_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_14_ID
	if ( MT_14_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_4_ID
	if ( MT_4_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_5_ID
	if ( MT_5_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_17_ID
	if ( MT_17_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_18_ID
	if ( MT_18_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_19_ID
	if ( MT_19_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_22_ID
	if ( MT_22_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_21_ID
	if ( MT_21_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_22_ID
	if ( MT_22_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_23_ID
	if ( MT_23_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_24_ID
	if ( MT_24_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_25_ID
	if ( MT_25_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_26_ID
	if ( MT_26_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_27_ID
	if ( MT_27_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_28_ID
	if ( MT_28_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_29_ID
	if ( MT_29_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_30_ID
	if ( MT_30_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_31_ID
	if ( MT_31_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_32_ID
	if ( MT_32_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_33_ID
	if ( MT_33_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_34_ID
	if ( MT_34_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_35_ID
	if ( MT_35_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_36_ID
	if ( MT_36_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_37_ID
	if ( MT_37_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_38_ID
	if ( MT_38_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_39_ID
	if ( MT_39_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_40_ID
	if ( MT_40_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_41_ID
	if ( MT_41_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_42_ID
	if ( MT_42_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_43_ID
	if ( MT_43_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_44_ID
	if ( MT_44_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_45_ID
	if ( MT_45_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_46_ID
	if ( MT_46_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_47_ID
	if ( MT_47_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_48_ID
	if ( MT_48_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_49_ID
	if ( MT_49_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_50_ID
	if ( MT_50_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_51_ID
	if ( MT_51_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_52_ID
	if ( MT_52_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_53_ID
	if ( MT_53_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_54_ID
	if ( MT_54_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_55_ID
	if ( MT_55_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_56_ID
	if ( MT_56_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_57_ID
	if ( MT_57_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_58_ID
	if ( MT_58_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_59_ID
	if ( MT_59_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_30_ID
	if ( MT_30_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_31_ID
	if ( MT_31_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_32_ID
	if ( MT_32_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_33_ID
	if ( MT_33_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_34_ID
	if ( MT_34_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_35_ID
	if ( MT_35_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_36_ID
	if ( MT_36_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_37_ID
	if ( MT_37_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_38_ID
	if ( MT_38_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_39_ID
	if ( MT_39_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_70_ID
	if ( MT_70_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_71_ID
	if ( MT_71_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_72_ID
	if ( MT_72_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_73_ID
	if ( MT_73_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_74_ID
	if ( MT_74_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_75_ID
	if ( MT_75_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_76_ID
	if ( MT_76_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_77_ID
	if ( MT_77_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_78_ID
	if ( MT_78_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_79_ID
	if ( MT_79_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_80_ID
	if ( MT_80_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_81_ID
	if ( MT_81_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_82_ID
	if ( MT_82_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_83_ID
	if ( MT_83_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_84_ID
	if ( MT_84_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_85_ID
	if ( MT_85_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_86_ID
	if ( MT_86_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_87_ID
	if ( MT_87_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_88_ID
	if ( MT_88_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_89_ID
	if ( MT_89_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_90_ID
	if ( MT_90_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_91_ID
	if ( MT_91_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_92_ID
	if ( MT_92_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_93_ID
	if ( MT_93_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_94_ID
	if ( MT_94_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_95_ID
	if ( MT_95_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_96_ID
	if ( MT_96_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_97_ID
	if ( MT_97_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_98_ID
	if ( MT_98_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_99_ID
	if ( MT_99_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_230_ID
	if ( MT_230_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_24_ID
	if ( MT_24_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_25_ID
	if ( MT_25_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_233_ID
	if ( MT_233_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_234_ID
	if ( MT_234_WRITE == TRUE )
	{
	*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_235_ID
	if ( MT_235_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_236_ID
	if ( MT_236_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_237_ID
	if ( MT_237_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_238_ID
	if ( MT_238_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_239_ID
	if ( MT_239_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_110_ID
	if ( MT_110_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_111_ID
	if ( MT_111_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_112_ID
	if ( MT_112_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_113_ID
	if ( MT_113_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_114_ID
	if ( MT_114_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_115_ID
	if ( MT_115_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_116_ID
	if ( MT_116_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_117_ID
	if ( MT_117_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_118_ID
	if ( MT_118_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_119_ID
	if ( MT_119_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_120_ID
	if ( MT_120_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_121_ID
	if ( MT_121_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_122_ID
	if ( MT_122_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_123_ID
	if ( MT_123_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_124_ID
	if ( MT_124_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_125_ID
	if ( MT_125_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_126_ID
	if ( MT_126_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_127_ID
	if ( MT_127_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_128_ID
	if ( MT_128_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_129_ID
	if ( MT_129_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_130_ID
	if ( MT_130_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_131_ID
	if ( MT_131_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_132_ID
	if ( MT_132_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_133_ID
	if ( MT_133_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_134_ID
	if ( MT_134_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_135_ID
	if ( MT_135_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_136_ID
	if ( MT_136_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_137_ID
	if ( MT_137_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_138_ID
	if ( MT_138_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_139_ID
	if ( MT_139_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_140_ID
	if ( MT_140_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_141_ID
	if ( MT_141_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_142_ID
	if ( MT_142_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_143_ID
	if ( MT_143_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_202_ID
	if ( MT_202_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_145_ID
	if ( MT_145_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_146_ID
	if ( MT_146_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_147_ID
	if ( MT_147_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_148_ID
	if ( MT_148_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_149_ID
	if ( MT_149_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_40_ID
	if ( MT_40_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_41_ID
	if ( MT_41_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_42_ID
	if ( MT_42_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_43_ID
	if ( MT_43_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_44_ID
	if ( MT_44_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_45_ID
	if ( MT_45_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_46_ID
	if ( MT_46_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_47_ID
	if ( MT_47_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_48_ID
	if ( MT_48_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_49_ID
	if ( MT_49_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef Mt_50_ID
	if ( Mt_50_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef Mt_51_ID
	if ( Mt_51_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef Mt_52_ID
	if ( Mt_52_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef Mt_53_ID
	if ( Mt_53_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef Mt_54_ID
	if ( Mt_54_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef Mt_55_ID
	if ( Mt_55_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef Mt_56_ID
	if ( Mt_56_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef Mt_57_ID
	if ( Mt_57_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef Mt_58_ID
	if ( Mt_58_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef Mt_59_ID
	if ( Mt_59_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_170_ID
	if ( MT_170_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_171_ID
	if ( MT_171_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_172_ID
	if ( MT_172_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_173_ID
	if ( MT_173_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_174_ID
	if ( MT_174_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_175_ID
	if ( MT_175_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_176_ID
	if ( MT_176_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_177_ID
	if ( MT_177_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_178_ID
	if ( MT_178_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_179_ID
	if ( MT_179_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_180_ID
	if ( MT_180_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_181_ID
	if ( MT_181_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_182_ID
	if ( MT_182_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_183_ID
	if ( MT_183_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_184_ID
	if ( MT_184_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_185_ID
	if ( MT_185_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_186_ID
	if ( MT_186_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_187_ID
	if ( MT_187_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_188_ID
	if ( MT_188_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_189_ID
	if ( MT_189_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_190_ID
	if ( MT_190_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_191_ID
	if ( MT_191_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_192_ID
	if ( MT_192_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_193_ID
	if ( MT_193_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_194_ID
	if ( MT_194_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_195_ID
	if ( MT_195_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_196_ID
	if ( MT_196_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_197_ID
	if ( MT_197_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_198_ID
	if ( MT_198_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_199_ID
	if ( MT_199_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_220_ID
	if ( MT_220_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_221_ID
	if ( MT_221_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_222_ID
	if ( MT_222_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_223_ID
	if ( MT_223_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_224_ID
	if ( MT_224_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_225_ID
	if ( MT_225_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_226_ID
	if ( MT_226_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_227_ID
	if ( MT_227_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_228_ID
	if ( MT_228_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_229_ID
	if ( MT_229_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_210_ID
	if ( MT_210_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_211_ID
	if ( MT_211_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_212_ID
	if ( MT_212_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_213_ID
	if ( MT_213_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_214_ID
	if ( MT_214_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_215_ID
	if ( MT_215_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_216_ID
	if ( MT_216_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_217_ID
	if ( MT_217_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_218_ID
	if ( MT_218_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_219_ID
	if ( MT_219_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_220_ID
	if ( MT_220_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_221_ID
	if ( MT_221_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_222_ID
	if ( MT_222_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_223_ID
	if ( MT_223_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_224_ID
	if ( MT_224_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_225_ID
	if ( MT_225_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_226_ID
	if ( MT_226_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_227_ID
	if ( MT_227_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_228_ID
	if ( MT_228_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_229_ID
	if ( MT_229_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_230_ID
	if ( MT_230_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_24_ID
	if ( MT_24_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_25_ID
	if ( MT_25_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_233_ID
	if ( MT_233_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_234_ID
	if ( MT_234_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_235_ID
	if ( MT_235_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_236_ID
	if ( MT_236_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_237_ID
	if ( MT_237_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_238_ID
	if ( MT_238_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_239_ID
	if ( MT_239_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_240_ID
	if ( MT_240_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_241_ID
	if ( MT_241_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_242_ID
	if ( MT_242_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_243_ID
	if ( MT_243_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_244_ID
	if ( MT_244_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_245_ID
	if ( MT_245_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_246_ID
	if ( MT_246_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_247_ID
	if ( MT_247_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_248_ID
	if ( MT_248_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_249_ID
	if ( MT_249_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_250_ID
	if ( MT_250_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_251_ID
	if ( MT_251_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_252_ID
	if ( MT_252_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_253_ID
	if ( MT_253_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_254_ID
	if ( MT_254_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_255_ID
	if ( MT_255_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_256_ID
	if ( MT_256_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_257_ID
	if ( MT_257_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_258_ID
	if ( MT_258_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_259_ID
	if ( MT_259_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_260_ID
	if ( MT_260_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_261_ID
	if ( MT_261_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_262_ID
	if ( MT_262_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_263_ID
	if ( MT_263_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
	
	set_buff++;
	*set_buff = 0;
	
	#ifdef MT_264_ID
	if ( MT_264_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 0 );
	}
	#endif
	
	#ifdef MT_265_ID
	if ( MT_265_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 1 );
	}
	#endif
	
	#ifdef MT_266_ID
	if ( MT_266_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 2 );
	}
	#endif
	
	#ifdef MT_267_ID
	if ( MT_267_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 3 );
	}
	#endif
	
	#ifdef MT_268_ID
	if ( MT_268_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 4 );
	}
	#endif
	
	#ifdef MT_269_ID
	if ( MT_269_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 5 );
	}
	#endif
	
	#ifdef MT_270_ID
	if ( MT_270_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 6 );
	}
	#endif
	
	#ifdef MT_271_ID
	if ( MT_271_WRITE == TRUE )
	{
		*set_buff |= ( 1 << 7 );
	}
	#endif
}

/**
 ******************************************************************************
 * \fn uint8_t GRAL_APP_ucGet_dim_std_tbls_used(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
inline uint8_t GRAL_APP_ucGet_dim_std_tbls_used(void) {
	
	uint8_t nbr_octets, i;
	
	memset(&set_buff, 0, sizeof(set_buff));
	GRAL_APP_vLoad_std_tbls_used(&set_buff[0]);
	
	for (i = 0; i < sizeof(set_buff); i++) {
		if (set_buff[i] > 0) {
			nbr_octets = i + 1;
		}
	}
	
	return nbr_octets;
}

/**
 ******************************************************************************
 * \fn uint8_t GRAL_APP_ucGet_dim_mfg_tbls_used(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
inline uint8_t GRAL_APP_ucGet_dim_mfg_tbls_used(void)
{
	uint8_t nbr_octets, i;
	
	memset(&set_buff, 0, sizeof(set_buff));
	GRAL_APP_vLoad_mfg_tbls_used(&set_buff[0]);
	
	for (i = 0; i < sizeof(set_buff); i++) {
		if (set_buff[i] > 0) {
			nbr_octets = i + 1;
		}
	}
	
	return nbr_octets;
}

/**
 ******************************************************************************
 * \fn uint8_t GRAL_APP_ucGet_dim_std_proc_used(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
inline uint8_t GRAL_APP_ucGet_dim_std_proc_used(void)
{
	uint8_t nbr_octets, i;
	
	memset(&set_buff, 0, sizeof(set_buff));
	GRAL_APP_vLoad_std_proc_used(&set_buff[0]);
	
	for (i = 0; i < sizeof(set_buff); i++) {
		if (set_buff[i] > 0) {
			nbr_octets = i + 1;
		}
	}
	
	return nbr_octets;
}

/**
 ******************************************************************************
 * \fn uint8_t GRAL_APP_ucGet_dim_mfg_proc_used(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
inline uint8_t GRAL_APP_ucGet_dim_mfg_proc_used(void)
{
	uint8_t nbr_octets, i;
	
	memset(&set_buff, 0, sizeof(set_buff));
	GRAL_APP_vLoad_mfg_proc_used(&set_buff[0]);
	
	for (i = 0; i < sizeof(set_buff); i++) {
		if (set_buff[i] > 0) {
			nbr_octets = i + 1;
		}
	}
	
	return nbr_octets;
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