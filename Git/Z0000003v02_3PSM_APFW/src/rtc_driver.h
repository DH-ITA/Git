/**
 ******************************************************************************
 * \file rtc.h
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

#ifndef RTC_TEMP_H
#define RTC_TEMP_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "util.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */

/*===========================================================================*\
 * RTC Type Used
\*===========================================================================*/
#define RTC_TYPE_USED					RTC_RX8900					

/*===========================================================================*\
 * RTC conversion factors
\*===========================================================================*/
#define SECONDS_PER_MINUTE				60
#define SECONDS_PER_HOUR				3600
#define SECONDS_PER_DAY					86400
#define MILISECONDS_PER_SECOND			1000

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
 */
		
/*				 
 ******************************************************************************
 * Enumerations and Structures and Typedefs                          
 ******************************************************************************
 */


/*===========================================================================*\
 * External/Internal Real Time Clocks Type Enumeration
\*===========================================================================*/
typedef enum {

	NO_RTC = 0,
	RTC_RX8900,
	RTC_SAM4CM,
} RTC_ENUM_TYPE;

/*===========================================================================*\
 * Human Time Type Format Structure
\*===========================================================================*/
typedef struct {

	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;

} HUMAN_TIME_TYPE;

/*===========================================================================*\
 * Real Time Structure
\*===========================================================================*/
typedef struct {

	int16_t tick_count;
	uint8_t is_time_relative;
	
	uint32_t utc_timestamp;
	
	uint32_t timestamp;
	HUMAN_TIME_TYPE htime;
	
	uint8_t dst_applied_flag;
	uint8_t is_dst;
	uint8_t dst_step;

	uint8_t day_of_week;
	uint8_t day_of_year;
	
	uint32_t dst_to_effective;
	uint32_t dst_from_effective;
	uint32_t dst_time_amount;
	int32_t time_zone_offset;
} RTC_TYPE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
extern RTC_TYPE rtc;

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
 */
extern void RTC_APP_vInitialize(void);
extern void RTC_APP_vIo_initialize(void);
extern void RTC_APP_vIo_configure(void);
extern bool RTC_APP_bIs_leap_year(uint16_t yr);
extern uint8_t RTC_APP_ucDay_of_week(uint16_t year, uint8_t month, uint8_t day);
extern void RTC_APP_v1sec_handler(void);
extern void RTC_APP_vBreak_timestamp(RTC_TYPE* rtc, uint32_t timestamp);
extern uint32_t RTC_APP_ulMake_timestamp(HUMAN_TIME_TYPE *time);
extern void RTC_APP_vLoad_tm_zn_dst(RTC_TYPE* rtc);
extern void RTC_APP_vRecover_rtc(void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
#endif