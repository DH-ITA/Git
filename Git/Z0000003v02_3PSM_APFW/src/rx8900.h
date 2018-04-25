/**
 ******************************************************************************
 * \file rx8900.h
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
 *   - Format: F_P06-DP_14
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
#ifndef  RX8900_H
#define  RX8900_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
 #include "compiler.h"
 
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/
/*===========================================================================*\
 * RX8900 SA/CE RTC Registers Addresses
\*===========================================================================*/

/* Address 00h to 0Fh: Basic time and calendar register */
#define RX8900_BR_SEC_ADDR							0x00
#define RX8900_BR_MIN_ADDR							0x01
#define RX8900_BR_HOUR_ADDR							0x02
#define RX8900_BR_WEEK_ADDR							0x03
#define RX8900_BR_DAY_ADDR							0x04
#define RX8900_BR_MONTH_ADDR						0x05
#define RX8900_BR_YEAR_ADDR							0x06
#define RX8900_BR_RAM_ADDR							0x07
#define RX8900_BR_MIN_ALARM_ADDR					0x08
#define RX8900_BR_HOUR_ALARM_ADDR					0x09
#define RX8900_BR_WEEK_DAY_ALARM_ADDR				0x0A
#define RX8900_BR_TIMER_COUNTER_0_ADDR				0x0B
#define RX8900_BR_TIMER_COUNTER_1_ADDR				0x0C
#define RX8900_BR_EXTENSION_REGISTER_ADDR			0x0D
#define RX8900_BR_FLAG_REGISTER_ADDR				0x0E
#define RX8900_BR_CONTROL_REGISTER_ADDR				0x0F

/* Address 10h to 1Fh: Extension register */
#define RX8900_ER_SEC_ADDR							0x10
#define RX8900_ER_MIN_ADDR							0x11
#define RX8900_ER_HOUR_ADDR							0x12
#define RX8900_ER_WEEK_ADDR							0x13
#define RX8900_ER_DAY_ADDR							0x14
#define RX8900_ER_MONTH_ADDR						0x15
#define RX8900_ER_YEAR_ADDR							0x16
#define RX8900_ER_TEMP_ADDR							0x17
#define RX8900_ER_BACKUP_FUNTION_ADDR				0x18
#define RX8900_ER_TIMER_COUNTER_0_ADDR				0x1B
#define RX8900_ER_TIMER_COUNTER_1_ADDR				0x1C
#define RX8900_ER_EXTENSION_REGISTER_ADDR			0x1D
#define RX8900_ER_FLAG_REGISTER_ADDR				0x1E
#define RX8900_ER_CONTROL_REGISTER_ADDR				0x1F

/*===========================================================================*\
 * Day of the Week counter Register Bit Mask
\*===========================================================================*/
#define WEEK_SUNDAY_bp								0
#define WEEK_SUNDAY_bm								0b00000001
#define WEEK_MONDAY_bp								1
#define WEEK_MONDAY_bm								0b00000010
#define WEEK_TUESDAY_bp								2
#define WEEK_TUESDAY_bm								0b00000100
#define WEEK_WEDNESDAY_bp							3
#define WEEK_WEDNESDAY_bm							0b00001000
#define WEEK_THURSDAY_bp							4
#define WEEK_THURSDAY_bm							0b00010000
#define WEEK_FRIDAY_bp								5
#define WEEK_FRIDAY_bm								0b00100000
#define WEEK_SATURDAY_bp							6
#define WEEK_SATURDAY_bm							0b01000000


/*===========================================================================*\
 * Backup Function Register Bit Mask
\*===========================================================================*/
#define BACKUP_REGISTER_BKSMP0_bp					0
#define BACKUP_REGISTER_BKSMP0_bm					0b00000001
#define BACKUP_REGISTER_BKSMP1_bp					1
#define BACKUP_REGISTER_BKSMP1_bm					0b00000010
#define BACKUP_REGISTER_SWOFF_bp					2
#define BACKUP_REGISTER_SWOFF_bm					0b00000100
#define BACKUP_REGISTER_VDETOFF_bp					3
#define BACKUP_REGISTER_VDETOFF_bm					0b00001000

/*===========================================================================*\
 * Flag Register Bit Mask
\*===========================================================================*/
#define FLAG_REGISTER_VDET_bp						0
#define FLAG_REGISTER_VDET_bm						0b00000001
#define FLAG_REGISTER_VLF_bp						1
#define FLAG_REGISTER_VLF_bm						0b00000010
#define FLAG_REGISTER_AF_bp							3
#define FLAG_REGISTER_AF_bm							0b00001000
#define FLAG_REGISTER_TF_bp							4
#define FLAG_REGISTER_TF_bm							0b00010000
#define FLAG_REGISTER_UF_bp							5
#define FLAG_REGISTER_UF_bm							0b00100000

/*===========================================================================*\
 * BCD definitions
\*===========================================================================*/
/* The BCD code shift value */
#define BCD_SHIFT									4

/* The BCD code mask value */
#define BCD_MASK									0xfu

/* The BCD mul/div factor value */
#define BCD_FACTOR									10


/*===========================================================================*\
 * Brief description for each block of constant definitions
\*===========================================================================*/

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each block of Macro definitions
\*===========================================================================*/

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each structure and typedefs
\*===========================================================================*/

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/
extern bool RX8900_APP_bRead_date_time(uint32_t *p_second, uint32_t *p_minute, uint32_t *p_hour, uint32_t *p_day_of_week, uint32_t *p_day, uint32_t *p_month, uint32_t *p_year);
extern bool RX8900_APP_bWrite_date_time(uint32_t second, uint32_t minute, uint32_t hour, uint32_t day_of_week, uint32_t day, uint32_t month, uint32_t year);
extern bool RX8900_APP_ucRead_backup_funtion_register(uint8_t* p_bkup);
extern bool RX8900_APP_ucEnable_vdd_monitor_2ms_per_sec(void);
extern bool RX8900_APP_ucEnable_vdd_monitor_16ms_per_sec(void);
extern bool RX8900_APP_ucEnable_vdd_monitor_128ms_per_sec(void);
extern bool RX8900_APP_ucEnable_vdd_monitor_256ms_per_sec(void);
extern bool RX8900_APP_ucDisable_vdd_monitor_pmos_sw_on(void);
extern bool RX8900_APP_ucDisable_vdd_monitor_pmos_sw_off(void);

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
 #endif