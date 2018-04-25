/**
 ******************************************************************************
 * \file rtc.c
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
#include "rtc_driver.h"
#include "tou.h"
#include "rtc.h"
#include "sm.h"
#include "regs.h"
#include "evnt.h"
#include "led.h"
#include "i2c.h"
#include "rx8900.h"

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
#define RTC_TICKS_PER_SEC	(uint16_t)(1)//configTICK_RATE_HZ)

/*===========================================================================*\
 * Brief description for each constant definition
\*===========================================================================*/

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each Macro
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
 * Global and Const Variable Defining Definitions / Initializations
 ******************************************************************************
 */
RTC_TYPE rtc;

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
static const uint8_t month_days[2][13] = { {0,31,28,31,30,31,30,31,31,30,31,30,31}, {0,31,29,31,30,31,30,31,31,30,31,30,31} };

/*
 ******************************************************************************
 * Static Function Prototypes for Private Functions with File Level Scope
 ******************************************************************************
 */
void RTC_Handler(void);

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */

/**
 ******************************************************************************
 * \fn void RTC_APP_vInitialize(void)
 * \brief RTC initialize
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void RTC_APP_vInitialize(void) {

	RTC_TYPE rtc_temp;

	RTC_APP_vIo_initialize();
	RTC_APP_vIo_configure();
	
	rtc_temp.timestamp = 0;
	RTC_APP_vLoad_tm_zn_dst(&rtc_temp);
	rtc_temp.utc_timestamp = 0;
	RTC_APP_vBreak_timestamp(&rtc_temp, rtc_temp.timestamp);
	rtc_temp.is_dst = 0;
	rtc_temp.dst_step = 0;
	rtc_temp.is_time_relative = 1;
	rtc_temp.dst_applied_flag = 0;
	
	rtc = rtc_temp;
	rtc.tick_count = 0;

	/************************************************************************/
	/* End Device is using an independent RTC                               */
	/************************************************************************/
	if ( RTC_TYPE_USED != NO_RTC ) {
		RTC_APP_vRecover_rtc();
	}
}

/**
 ******************************************************************************
 * \fn void RTC_APP_vIo_initialize(void)
 * \brief RTC i/o initialize
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void RTC_APP_vIo_initialize(void) {

	nop();
}

/**
 ******************************************************************************
 * \fn void RTC_APP_vIo_configure(void)
 * \brief RTC i/o configure
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void RTC_APP_vIo_configure(void) {

	/* Default RTC configuration, 24-hour mode */
	rtc_set_hour_mode(RTC, 0);

	/* Configure RTC interrupts */
	NVIC_DisableIRQ(RTC_IRQn);
	NVIC_ClearPendingIRQ(RTC_IRQn);
	NVIC_SetPriority(RTC_IRQn, 0);
	NVIC_EnableIRQ(RTC_IRQn);
	RTC->RTC_IER = RTC_IER_SECEN;
	
	/*===========================================================================*\
	 * EPSON RTC Model: RX8900
	\*===========================================================================*/
	if ( RTC_TYPE_USED == RTC_RX8900 ) {
		
		RX8900_APP_ucDisable_vdd_monitor_pmos_sw_off();
	}
}

/**
 ******************************************************************************
 * \fn void RTC_APP_vRecover_rtc(void)
 * \brief Recover the user time/date 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void RTC_APP_vRecover_rtc(void) {
	
	bool clock_ok = false;
	RTC_TYPE rtc_temp, rtc_backup;
	uint32_t year = 2000, month = 2, day = 1, day_of_week = 0, hour = 0, minute = 0, second = 0;

	memset( &rtc_temp, 0, sizeof(rtc_temp) );

	/*===========================================================================*\
	 * Get time/date from:
	\*===========================================================================*/
	switch ( RTC_TYPE_USED ) {

		case RTC_RX8900:
			clock_ok = RX8900_APP_bRead_date_time(&second, &minute, &hour, &day_of_week, &day, &month, &year);		
			break;

		case RTC_SAM4CM:
			rtc_get_time(RTC, &hour, &minute, &second);
			rtc_get_date(RTC, &year, &month, &day, &day_of_week);
			clock_ok = true;
			break;

		default:
			clock_ok = false;
			break;
	}

	rtc_temp.htime.year = year;
	rtc_temp.htime.month = month;
	rtc_temp.htime.day = day;
	rtc_temp.day_of_week = day_of_week;
	rtc_temp.htime.hour = hour;
	rtc_temp.htime.minute = minute;
	rtc_temp.htime.second = second;
	rtc_temp.utc_timestamp = RTC_APP_ulMake_timestamp(&rtc_temp.htime);
	

	/*===========================================================================*\
	 * Calculate the date/time for the time zone, determine DST and starts 
	 * the user RTC.
	\*===========================================================================*/
	if ( meter.system_state.initialiting || ( (!meter.system_state.initialiting) && clock_ok ) ) {
	
		RTC_APP_vLoad_tm_zn_dst(&rtc_temp);
		rtc_temp.timestamp = rtc_temp.utc_timestamp + rtc_temp.time_zone_offset;
		RTC_APP_vBreak_timestamp(&rtc_temp, rtc_temp.timestamp);
		rtc_temp.dst_applied_flag = TOU_APP_ucLoad_dst_dates(&rtc_temp);
		rtc_temp.is_dst = 0;
		rtc_temp.dst_step = 0;
		
		if ( (rtc_temp.timestamp >= rtc_temp.dst_to_effective) && (rtc_temp.timestamp < rtc_temp.dst_from_effective) ) {
			
			rtc_temp.is_dst = 1;
			rtc_temp.dst_step = 1;
			rtc_temp.timestamp += rtc_temp.dst_time_amount;
			RTC_APP_vBreak_timestamp(&rtc_temp, rtc_temp.timestamp);
		}
		
		if ( rtc_temp.timestamp >= rtc_temp.dst_from_effective ) {
			rtc_temp.dst_step = 2;
		}
		
		rtc_temp.is_time_relative = 0;
		
		Disable_global_interrupt();
		rtc_backup = rtc;
		rtc = rtc_temp;
		rtc.tick_count = 0;
		Enable_global_interrupt();

		if ( (rtc_backup.is_dst == 0) && (rtc_backup.is_dst != rtc_temp.is_dst) ) {
			
			tou_os.register_dst_on = true;
			tou_os.user = END_DEVICE;
			tou_os.event_timestamp = rtc.utc_timestamp;
		}
		else if ( (rtc_backup.is_dst == 1) && (rtc_backup.is_dst != rtc_temp.is_dst) ) {
			
			tou_os.register_dst_off = true;
			tou_os.user = END_DEVICE;
			tou_os.event_timestamp = rtc.utc_timestamp;
		}
	}
	
	/*===========================================================================*\
	 * A failure occurs while reading the external RTC
	\*===========================================================================*/
	if ( !clock_ok ) {

		/* Register clock error detected*/
		EVNT_APP_vRegister_event(CLOCK_ERROR_DETECTED, 0, true, END_DEVICE, rtc.utc_timestamp);
		RTC_APP_vBreak_timestamp(&rtc_temp, rtc.utc_timestamp);

		/* Synchronize External RTC using User RTC */
		switch ( RTC_TYPE_USED ) {

			case RTC_RX8900:
				RX8900_APP_bWrite_date_time(rtc_temp.htime.second, rtc_temp.htime.minute, rtc_temp.htime.hour, 
											rtc_temp.day_of_week, rtc_temp.htime.day, rtc_temp.htime.month, rtc_temp.htime.year);
				break;

			case RTC_SAM4CM:
				rtc_set_time(RTC, rtc_temp.htime.hour, rtc_temp.htime.minute, rtc_temp.htime.second);
				rtc_set_date(RTC, rtc_temp.htime.year, rtc_temp.htime.month, rtc_temp.htime.day, rtc_temp.day_of_week);
				break;

			default:
				break;
		}
	}
	
	/* Initialize Time of Use*/
	tou_os.rtc_change_mask = (SECOND_CHANGE_bm | MINUTE_CHANGE_bm | HOUR_CHANGE_bm | DAY_CHANGE_bm | MONTH_CHANGE_bm | YEAR_CHANGE_bm);
	TOU_APP_vTask();
	
	/* Start Load Profile */
	LP_APP_vSynchronize_clock(rtc_backup.utc_timestamp, rtc.utc_timestamp);
}

/**
 ******************************************************************************
 * \fn bool RTC_APP_bIs_leap_year(uint16_t yr)
 * \brief Determines if a given year is a leap year
 *
 * \param Year (e.g., 2004)
 * \retval true if year is a leap year
 ******************************************************************************
 */
bool RTC_APP_bIs_leap_year(uint16_t yr)
{
	return (((yr % 400) == 0) || (((yr % 4) == 0) && ((yr % 100) != 0)));
}

/**
 ******************************************************************************
 * \fn uint8_t RTC_APP_ucDay_of_week(uint16_t year, uint8_t month, uint8_t day)
 * \brief Calculate day of week in proleptic Gregorian calendar. Sunday == 0.
 *
 * \param year (e.g., 2004)
 * \param month
 * \param day
 * \retval weekday
 ******************************************************************************
 */
uint8_t RTC_APP_ucDay_of_week(uint16_t year, uint8_t month, uint8_t day)
{
	int adjustment, mm, yy, day_of_week;
	
	adjustment = (14 - month) / 12;
	mm = month + 12 * adjustment - 2;
	yy = year - adjustment;
	day_of_week = (day + (13 * mm - 1) / 5 + yy + yy / 4 - yy / 100 + yy / 400) % 7;
	
	return (uint8_t)day_of_week;
}

/**
 ******************************************************************************
 * \fn void RTC_APP_v1sec_handler(void)
 * \brief RTC i/o initialize
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void RTC_APP_v1sec_handler(void) {
	
	uint8_t leap_year;
	
	rtc.tick_count++;

	if ( rtc.tick_count >= RTC_TICKS_PER_SEC) {
		
		rtc.tick_count = 0;
		rtc.utc_timestamp++;
		rtc.timestamp++;
		rtc.htime.second++;
		meter.time_running++;
		tou_os.rtc_change_mask |= SECOND_CHANGE_bm;

		if ( rtc.htime.second >= 60 ) {
			
			rtc.htime.second = 0;
			rtc.htime.minute++;
			tou_os.rtc_change_mask |= MINUTE_CHANGE_bm;

			if ( rtc.htime.minute >= 60 ) {
				
				rtc.htime.minute = 0;
				rtc.htime.hour++;
				tou_os.rtc_change_mask |= HOUR_CHANGE_bm;
				
				if ( rtc.htime.hour >= 24 ) {
					
					rtc.htime.hour = 0;
					
					rtc.day_of_week++;
					if ( rtc.day_of_week > 6 ) {
						
						rtc.day_of_week = 0;
					}
					
					rtc.htime.day++;
					rtc.day_of_year++;
					tou_os.rtc_change_mask |= DAY_CHANGE_bm;
										
					leap_year = RTC_APP_bIs_leap_year(rtc.htime.year);
					
					if ( rtc.htime.day > month_days[leap_year][rtc.htime.month] ) {
						
						rtc.htime.day = 1;
						rtc.htime.month++;
						tou_os.rtc_change_mask |= MONTH_CHANGE_bm;
						
						if ( rtc.htime.month > 12 ) {
							
							rtc.day_of_year = 1;
							rtc.htime.month = 1;
							rtc.htime.year++;
							tou_os.rtc_change_mask |= YEAR_CHANGE_bm;
							
							rtc.is_dst = 0;
							rtc.dst_step = 0;
						}
					}
				}
			}
		}
		
		if (rtc.dst_applied_flag == 1) {
			
			if ( (rtc.timestamp >= rtc.dst_to_effective) && (rtc.timestamp < rtc.dst_from_effective) && (rtc.dst_step < 2) ) {
				
				if ( rtc.is_dst == 0 ) {
					
					tou_os.register_dst_on = true;
					tou_os.user = END_DEVICE;
					tou_os.event_timestamp = rtc.utc_timestamp;
				}
				
				rtc.is_dst = 1;
			}
			else {
				
				if ( rtc.is_dst == 1 ) {
					
					tou_os.register_dst_off = true;
					tou_os.user = END_DEVICE;
					tou_os.event_timestamp = rtc.utc_timestamp;
				}
				
				rtc.is_dst = 0;
			}
			
			if ( (rtc.is_dst == 1) && (rtc.dst_step == 0) ) {
				
				rtc.timestamp += rtc.dst_time_amount;
				RTC_APP_vBreak_timestamp(&rtc, rtc.timestamp);
				rtc.dst_step = 1;
			}
			else if ( (rtc.is_dst == 0) && (rtc.dst_step == 1) ) {
				
				rtc.timestamp -= rtc.dst_time_amount;
				RTC_APP_vBreak_timestamp(&rtc, rtc.timestamp);
				rtc.dst_step = 2;
			}
		}
	}
	
	if ( !meter.system_state.initialiting ) {
		
		if ( tou_os.rtc_change_mask != 0 ) {
			
			if ( tou_os.rtos_task_active ) {

				portBASE_TYPE xYieldRequired;
				xYieldRequired = xTaskResumeFromISR(tou_os.task_id);
				
				if ( xYieldRequired == pdTRUE ){
					taskYIELD();
				}
			}
		}
	}		
}

/**
 ******************************************************************************
 * \fn void RTC_APP_vBreak_timestamp(RTC_TYPE* rtc, uint32_t timestamp)
 * \brief
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
void RTC_APP_vBreak_timestamp(RTC_TYPE* rtc, uint32_t timestamp)
{
	uint16_t year;
	uint8_t month;
	uint8_t month_length;
	uint32_t time;
	unsigned long days;
	uint8_t month_days[12]={31,28,31,30,31,30,31,31,30,31,30,31};

	time = timestamp;
	rtc->htime.second = time % 60;
	time /= 60;									// time = minutes
	rtc->htime.minute = time % 60;
	time /= 60;									// time = hours
	rtc->htime.hour = time % 24;
	time /= 24;									// time = days
	rtc->day_of_week = ((time + 4) % 7);		// Sunday = day 0
	
	year = 1970;
	days = 0;

	while((unsigned)(days += (RTC_APP_bIs_leap_year(year) ? 366 : 365)) <= time)
	{
		year++;
	}
	rtc->htime.year = year;
	
	days -= RTC_APP_bIs_leap_year(year) ? 366 : 365;
	time  -= days;								// time = days in this year, starting at 0
	rtc->day_of_year = time + 1;
	
	days = 0;
	month = 0;
	month_length = 0;
	
	for ( month = 0; month < 12; month++)
	{
		if ( month == 1 )
		{
			// Feb
			if ( RTC_APP_bIs_leap_year(year) )
			{
				month_length = 29;
			}
			else
			{
				month_length = 28;
			}
		}
		else
		{
			month_length = month_days[month];
		}
		
		if ( time >= month_length )
		{
			time -= month_length;
		}
		else
		{
			break;
		}
	}
	
	rtc->htime.month = month + 1;  // Jan = month 1
	rtc->htime.day = time + 1;     // Day of month
}

/**
 ******************************************************************************
 * \fn uint32_t RTC_APP_ulMake_timestamp(HUMAN_TIME_TYPE *time)
 * \brief
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
uint32_t RTC_APP_ulMake_timestamp(HUMAN_TIME_TYPE *time)
{	
	int i;
	uint32_t seconds;
	uint8_t monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

	// Seconds from 1970 until 1 jan 00:00:00
	seconds = (time->year - 1970)*(SECONDS_PER_DAY * 365);
	
	for ( i = 1970; i < time->year; i++ )
	{
		if ( RTC_APP_bIs_leap_year(i) )
		{
			seconds +=  SECONDS_PER_DAY;   // Add additional days for leap years
		}
	}
	
	// Add days for this year, months start from 1
	for ( i = 1; i < time->month; i++)
	{
		if ( (i == 2) && RTC_APP_bIs_leap_year(time->year) )
		{
			seconds += SECONDS_PER_DAY * 29;
		}
		else
		{
			seconds += SECONDS_PER_DAY * monthDays[i-1];
		}
	}
	
	seconds += (time->day - 1) * SECONDS_PER_DAY;
	seconds += time->hour * SECONDS_PER_HOUR;
	seconds += time->minute * SECONDS_PER_MINUTE;
	seconds += time->second;
	
	return seconds;
}

/**
 ******************************************************************************
 * \fn void RTC_load_tm_zn(RTC_TYPE* rtc)
 * \brief Load time zone amount and dst dates
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
void RTC_APP_vLoad_tm_zn_dst(RTC_TYPE* rtc)
{
	uint8_t dst_amount;
	int16_t tm_zn_offset;
		
	EEPROM_reader( ST_53_DST_TIME_AMT_ADDR, &dst_amount, sizeof(dst_amount));
	EEPROM_reader( ST_53_TIME_ZONE_OFFSET_ADDR, &tm_zn_offset, sizeof(tm_zn_offset));
	
	rtc->dst_time_amount = ((uint32_t)dst_amount)*SECONDS_PER_MINUTE;
	rtc->time_zone_offset = (uint32_t)tm_zn_offset;
	if ( tm_zn_offset < 0 )
	{
		rtc->time_zone_offset |= 0xFFFF0000;
	} 
	rtc->time_zone_offset *= SECONDS_PER_MINUTE;
}

/**
 ******************************************************************************
 * \fn void RTC_Handler(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void RTC_Handler(void) {
	
	uint32_t ul_status = rtc_get_status(RTC);

	/* Second increment interrupt */
	if ( (ul_status & RTC_SR_SEC) == RTC_SR_SEC ) {
		
		/* Disable RTC interrupt */
		rtc_disable_interrupt(RTC, RTC_IDR_SECDIS);

		if ( !meter.system_state.initialiting ) {
			
			/* Update RTC */
			RTC_APP_v1sec_handler();

			/* Refresh Instrumentation Values */
			METROLOGY_APP_vRefresh_inst_values();

			/* Accumulate energy each second */
			REGS_APP_vSummation_task();

			/* Accumulate demand each second */
			REGS_APP_vDemand_task();

			/* Load profile recorder */
			LP_APP_vRecorder_task();

			/* Resume tasks to store data */
			REGS_APP_v1sec_handler();
		}

		rtc_clear_status(RTC, RTC_SCCR_SECCLR);
		rtc_enable_interrupt(RTC, RTC_IER_SECEN);
	} 

	if ( (ul_status & RTC_SR_ALARM_ALARMEVENT) == RTC_SR_ALARM_ALARMEVENT ) {
		
		rtc_clear_status(RTC, RTC_SCCR_ALRCLR);
		RTC->RTC_IER = RTC_IER_ALREN;
	}
}

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
