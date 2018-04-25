/**
 ******************************************************************************
 * \file rx8900.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Eos Tech S. de A. de C.V. All rights reserved.</b><br><br>
 * Eos Tech Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup File content title
 * @section DESCRIPTION DESCRIPTION:
 *
 * File content description.
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - Format: F_P06-DP_13
 *   - Revision: 02
 *   - Date: 16/MAY/2017
 *   - Author: Ernesto Cocoletzi.
 *   - Aprove: Mariano Centeno Camarena.
 ******************************************************************************
 *
 * @defgroup File change log
 * @section FILE_CHANGE_LOG FILE CHANGE LOG:
 *
 ******************************************************************************
 *   - Revision 01 / 2017-11-22
 *   - PCR#00000001/Jacob Anaya Candia: Initial issue.
 *   - PCR#00000002/Author: Description.
 ******************************************************************************
 *   - Revision 02 / YYYY-MM-DD
 *   - PCR#00000003/Author: Description.
 *   - PCR#00000004/Author: Description.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "rx8900.h"
#include "compiler.h"
#include "i2c.h"
#include <string.h>

/*
 ******************************************************************************
 * File level pragmas                                                
 ******************************************************************************
*/ 

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each block of constant definitions
\*===========================================================================*/

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each block of Macros
\*===========================================================================*/

/*
 ******************************************************************************
 * Enumerations, Structures and Typedefs
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each enumeration, structure and typedef
\*===========================================================================*/

/*
 ******************************************************************************
 * Global and Const Variable Defining Definitions
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
 */

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

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
*/

/**
 ******************************************************************************
 * \fn bool RX8900_APP_bRead_date_time(uint32_t *p_second, uint32_t *p_minute, uint32_t *p_hour, uint32_t *p_day_of_week, uint32_t *p_day, uint32_t *p_month, uint32_t *p_year)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool RX8900_APP_bRead_date_time(uint32_t *p_second, uint32_t *p_minute, uint32_t *p_hour, uint32_t *p_day_of_week, uint32_t *p_day, uint32_t *p_month, uint32_t *p_year) {
	
	static bool rx8900_read = false;

	uint8_t flag_reg = 0;

	rx8900_read = I2C_HAL_bRtc_reader(RX8900_BR_FLAG_REGISTER_ADDR, &flag_reg, 1);  

	if ( rx8900_read ) {

		if ( !(flag_reg & FLAG_REGISTER_VLF_bm) ) {
			
			uint8_t date_time[8];
			memset(&date_time, 0, sizeof(date_time));
			rx8900_read = I2C_HAL_bRtc_reader(RX8900_BR_SEC_ADDR, &date_time[0], 8);

			if ( rx8900_read  ) {

				/* Second */
				if (p_second) {
					*p_second = (date_time[0] >> BCD_SHIFT) * BCD_FACTOR + (date_time[0] & BCD_MASK);
				}

				/* Minute */
				if (p_minute) {
					*p_minute = (date_time[1] >> BCD_SHIFT) * BCD_FACTOR + (date_time[1] & BCD_MASK);
				}
				
				/* Hour */
				if (p_hour) {
					*p_hour = (date_time[2] >> BCD_SHIFT) * BCD_FACTOR + (date_time[2] & BCD_MASK);
				}

				/* Retrieve week */
				if (p_day_of_week) {
					*p_day_of_week = date_time[3];

					switch ( date_time[3] ) {

						case WEEK_SUNDAY_bm:
							*p_day_of_week = 0;
							break;

						case WEEK_MONDAY_bm:
							*p_day_of_week = 1;
							break;

						case WEEK_TUESDAY_bm:
							*p_day_of_week = 2;
							break;

						case WEEK_WEDNESDAY_bm:
							*p_day_of_week = 3;
							break;

						case WEEK_THURSDAY_bm:
							*p_day_of_week = 4;
							break;

						case WEEK_FRIDAY_bm:
							*p_day_of_week = 5;
							break;

						case WEEK_SATURDAY_bm:
							*p_day_of_week = 6;
							break;
					}
				}

				/* Retrieve day */
				if (p_day) {
					*p_day = (date_time[4] >> BCD_SHIFT) * BCD_FACTOR + (date_time[4] & BCD_MASK);
				}

				/* Retrieve month */
				if (p_month) {
					*p_month = (date_time[5] >> BCD_SHIFT) * BCD_FACTOR + (date_time[5] & BCD_MASK);
				}

				/* Retrieve year */
				if (p_year) {
					*p_year = (date_time[6] >> BCD_SHIFT) * BCD_FACTOR + (date_time[6] & BCD_MASK) + 2000;
				}
			}
		}
		else {
			
			rx8900_read = false;
			*p_second = 0;
			*p_minute = 0;
			*p_hour = 0;
			*p_day_of_week = 0;
			*p_day = 1;
			*p_month = 1;
			*p_year = 2000;
		}
	}

	return rx8900_read;
}

/**
 ******************************************************************************
 * \fn bool RX8900_APP_bWrite_date_time(uint32_t second, uint32_t minute, uint32_t hour, uint32_t day_of_week, uint32_t day, uint32_t month, uint32_t year)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool RX8900_APP_bWrite_date_time(uint32_t second, uint32_t minute, uint32_t hour, uint32_t day_of_week, uint32_t day, uint32_t month, uint32_t year) {
	
	static bool rx8900_write = false;

	uint8_t flag_reg = 0;
	uint8_t time_mask_ok = 0;
	uint8_t date_time[8];
	memset(&date_time, 0, sizeof(date_time));
	
	/* Second */
	if ( second < 60 ) {
		date_time[0] = ( (second / BCD_FACTOR) << 4 ) | ( (second % BCD_FACTOR) << 0 );
		time_mask_ok |= 0x01;
	}
	
	/* Minute */
	if ( minute < 60 ) {
		date_time[1] = ( (minute / BCD_FACTOR) << 4 ) | ( (minute % BCD_FACTOR) << 0 );
		time_mask_ok |= 0x02;
	}

	/* Hour */
	if ( hour < 24 ) {
		date_time[2] = ( (hour / BCD_FACTOR) << 4 ) | ( (hour % BCD_FACTOR) << 0 );
		time_mask_ok |= 0x04;
	}
	
	/* Day of Week */
	if ( day_of_week < 7 ) {
		date_time[3] = 1 << day_of_week;
		time_mask_ok |= 0x08;
	}

	/* Day */
	switch ( month ) {
		
		case 1:
		case 3: 
		case 5: 
		case 7: 
		case 8: 
		case 10: 
		case 12:
			if ( day <= 31 ) {
				date_time[4] = ( (day / BCD_FACTOR) << 4 ) | ( (day % BCD_FACTOR) << 0 );
				time_mask_ok |= 0x10;
			}
			break;

		case 4:
		case 6: 
		case 9:
		case 11:
			if ( day <= 30 ) {
				date_time[4] = ( (day / BCD_FACTOR) << 4 ) | ( (day % BCD_FACTOR) << 0 );
				time_mask_ok |= 0x10;
			}
			break;

		case 2:
			
			if ( (year > 2000) && (year < 2099) ) {
				year -= 2000;

				if ( (year % 4) == 0 ) {
					
					if ( day <= 29 ) {
						date_time[4] = ( (day / BCD_FACTOR) << 4 ) | ( (day % BCD_FACTOR) << 0 );
						time_mask_ok |= 0x10;
					}
				}
				else {

					if ( day <= 28 ) {
						date_time[4] = ( (day / BCD_FACTOR) << 4 ) | ( (day % BCD_FACTOR) << 0 );
						time_mask_ok |= 0x10;
					}
				}
			}
			
			break;

		default:
			break;
	}
	
	/* Month */
	if ( (month > 0) && (month < 13) ) {
		date_time[5] = ( (month / BCD_FACTOR) << 4 ) | ( (month % BCD_FACTOR) << 0 );
		time_mask_ok |= 0x20;
	}
	
	/* Year */
	if ( (year >= 2000) && (year <= 2099) ) {
		year -= 2000;
		date_time[6] = ( (year / BCD_FACTOR) << 4 ) | ( (year % BCD_FACTOR) << 0 );
		time_mask_ok |= 0x40;
	}
	
	if ( time_mask_ok == 0x7F ) {

		rx8900_write = I2C_HAL_bRtc_writer(RX8900_BR_SEC_ADDR, &date_time[0], 7);

		if ( rx8900_write ) {
			
			uint8_t time_comp[8];
			I2C_HAL_bRtc_reader(RX8900_BR_SEC_ADDR, &date_time[0], 7);

			if ( memcmp(&date_time[0], &date_time[0], 7) == 0 ) {
				
				rx8900_write = false;

				uint8_t flag_reg = 0xFF;
				I2C_HAL_bRtc_reader(RX8900_BR_FLAG_REGISTER_ADDR, &flag_reg, 1);
				flag_reg &= ~FLAG_REGISTER_VLF_bm;
				I2C_HAL_bRtc_writer(RX8900_BR_FLAG_REGISTER_ADDR, &flag_reg, 1);
				flag_reg = 0xFF;
				I2C_HAL_bRtc_reader(RX8900_BR_FLAG_REGISTER_ADDR, &flag_reg, 1);

				if ( !(flag_reg & FLAG_REGISTER_VLF_bm) ) {
					
					rx8900_write = true;
				}
			}
			else {
				rx8900_write = false;
			}
		}
	}

	return rx8900_write;
}

/**
 ******************************************************************************
 * \fn void void RX8900_APP_ucGet_backup_funtion_register(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool RX8900_APP_ucRead_backup_funtion_register(uint8_t* p_bkup) {
	
	return I2C_HAL_bRtc_reader(RX8900_ER_BACKUP_FUNTION_ADDR, p_bkup, 1);
}

/**
 ******************************************************************************
 * \fn bool RX8900_APP_ucEnable_vdd_monitor_2ms_per_sec(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool RX8900_APP_ucEnable_vdd_monitor_2ms_per_sec(void) {
	
	uint8_t bkup_reg = 0;

	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_BKSMP0_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_BKSMP1_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_SWOFF_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_VDETOFF_bp);

	return I2C_HAL_bRtc_writer(RX8900_ER_BACKUP_FUNTION_ADDR, &bkup_reg, 1);
}

/**
 ******************************************************************************
 * \fn bool RX8900_APP_ucEnable_vdd_monitor_16ms_per_sec(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool RX8900_APP_ucEnable_vdd_monitor_16ms_per_sec(void) {
	
	uint8_t bkup_reg = 0;

	bkup_reg |= (uint8_t)(1 << BACKUP_REGISTER_BKSMP0_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_BKSMP1_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_SWOFF_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_VDETOFF_bp);

	return I2C_HAL_bRtc_writer(RX8900_ER_BACKUP_FUNTION_ADDR, &bkup_reg, 1);
}

/**
 ******************************************************************************
 * \fn bool RX8900_APP_ucEnable_vdd_monitor_128ms_per_sec(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool RX8900_APP_ucEnable_vdd_monitor_128ms_per_sec(void) {
	
	uint8_t bkup_reg = 0;

	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_BKSMP0_bp);
	bkup_reg |= (uint8_t)(1 << BACKUP_REGISTER_BKSMP1_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_SWOFF_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_VDETOFF_bp);

	return I2C_HAL_bRtc_writer(RX8900_ER_BACKUP_FUNTION_ADDR, &bkup_reg, 1);
}

/**
 ******************************************************************************
 * \fn bool RX8900_APP_ucEnable_vdd_monitor_256ms_per_sec(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool RX8900_APP_ucEnable_vdd_monitor_256ms_per_sec(void) {
	
	uint8_t bkup_reg = 0;

	bkup_reg |= (uint8_t)(1 << BACKUP_REGISTER_BKSMP0_bp);
	bkup_reg |= (uint8_t)(1 << BACKUP_REGISTER_BKSMP1_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_SWOFF_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_VDETOFF_bp);

	return I2C_HAL_bRtc_writer(RX8900_ER_BACKUP_FUNTION_ADDR, &bkup_reg, 1);
}

/**
 ******************************************************************************
 * \fn bool RX8900_APP_ucDisable_vdd_monitor_pmos_sw_on(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool RX8900_APP_ucDisable_vdd_monitor_pmos_sw_on(void) {
	
	uint8_t bkup_reg = 0;

	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_BKSMP0_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_BKSMP1_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_SWOFF_bp);
	bkup_reg |= (uint8_t)(1 << BACKUP_REGISTER_VDETOFF_bp);

	return I2C_HAL_bRtc_writer(RX8900_ER_BACKUP_FUNTION_ADDR, &bkup_reg, 1);
}

/**
 ******************************************************************************
 * \fn bool RX8900_APP_ucDisable_vdd_monitor_pmos_sw_off(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool RX8900_APP_ucDisable_vdd_monitor_pmos_sw_off(void) {
	
	uint8_t bkup_reg = 0;

	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_BKSMP0_bp);
	bkup_reg |= (uint8_t)(0 << BACKUP_REGISTER_BKSMP1_bp);
	bkup_reg |= (uint8_t)(1 << BACKUP_REGISTER_SWOFF_bp);
	bkup_reg |= (uint8_t)(1 << BACKUP_REGISTER_VDETOFF_bp);

	return I2C_HAL_bRtc_writer(RX8900_ER_BACKUP_FUNTION_ADDR, &bkup_reg, 1);
}

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */