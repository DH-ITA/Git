/**
 ******************************************************************************
 * \file tou.h
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

#ifndef TOU_H_
#define TOU_H_

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "util.h"
#include "rtc_driver.h"
   
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/
#define TOU_STACK_SIZE						configMINIMAL_STACK_SIZE * 2
#define TOU_APP_vTask_PRIO					tskIDLE_PRIORITY + 3

/*===========================================================================*\
 * BIT MASK FOR USE IN ST_50 & ST_51 
\*===========================================================================*/


/*===========================================================================*\
 * BIT MASK FOR USE IN ST_52
\*===========================================================================*/
/* TIME_DATE_QUAL BIT MASK*/
#define DAY_OF_WEEK_bm					0b00000111
#define DAY_OF_WEEK_bp					0
#define DST_FLAG_bm						0b00001000
#define DST_FLAG_bp						3
#define GMT_FLAG_bm						0b00010000
#define GMT_FLAG_bp						4	
#define TM_ZN_APPLIED_FLAG_bm			0b00100000
#define TM_ZN_APPLIED_FLAG_bp			5
#define DST_APPLIED_FLAG_bm				0b01000000
#define DST_APPLIED_FLAG_bp				6
#define DST_SUPPORTED_FLAG_bm			0b10000000
#define DST_SUPPORTED_FLAG_bp			7

/*===========================================================================*\
 * BIT MASK FOR USE IN ST_54
\*===========================================================================*/
/* CALENDAR_ACTION_BFLD BIT MASK*/
#define CALENDAR_CTRL_bm				0b00011111
#define CALENDAR_CTRL_bp				0
#define DEMAND_RESET_FLAG_bm			0b00100000
#define DEMAND_RESET_FLAG_bp			5
#define SELF_READ_FLAG_bm				0b01000000
#define SELF_READ_FLAG_bp				6

/* TIER_SWITCH_BFLD BIT MASK*/
#define NEW_TIER_bm					0b0000000000000111
#define NEW_TIER_bp					0
#define SUMMATION_SWITCH_FLAG_bm	0b0000000000001000
#define SUMMATION_SWITCH_FLAG_bp	3
#define DEMANDS_SWITCH_FLAG_bm		0b0000000000010000
#define DEMANDS_SWITCH_FLAG_bp		4
#define SWITCH_MIN_bm				0b0000011111100000
#define SWITCH_MIN_bp				5
#define SWITCH_HOUR_bm				0b1111100000000000
#define SWITCH_HOUR_bp				11

/* DATE_BFLD BIT MASK*/
#define DATE_YEAR_bm				0b0000000001111111
#define DATE_YEAR_bp				0
#define DATE_MONTH_bm				0b0000011110000000
#define DATE_MONTH_bp				7
#define DATE_DAY_bm					0b1111100000000000
#define DATE_DAY_bp					11

/* RDATE_BFLD BIT MASK*/
#define RDATE_MONTH_bm				0b0000000000001111
#define RDATE_MONTH_bp				0
#define RDATE_PERIOD_IN_MINUTES_bm	0b1111111111110000
#define RDATE_PERIOD_IN_MINUTES_bp	4
#define RDATE_OFFSET_bm				0b0000000011110000
#define RDATE_OFFSET_bp				4
#define RDATE_WEEKDAY_bm			0b0000011100000000
#define RDATE_WEEKDAY_bp			8
#define RDATE_DAY_bm				0b1111100000000000
#define RDATE_DAY_bp				11
#define RDATE_PERIOD_IN_DAYS_bm		0b0000001111110000
#define RDATE_PERIOD_IN_DAYS_bp		4
#define RDATE_DELTA_bm				0b1111110000000000
#define RDATE_DALTA_bp				10

/*===========================================================================*\
 * BIT MASK FOR USE IN ST_55
\*===========================================================================*/
/* STATUS_BFLD BIT MASK*/
#define CURR_SUMM_TIER_bm			0b0000000000000111			
#define CURR_SUMM_TIER_bp			0
#define CURR_DEMAND_TIER_bm			0b0000000000111000
#define CURR_DEMAND_TIER_bp			3
#define CURR_TIER_bm				0b0000000000000111
#define CURR_TIER_bp				0
#define TIER_DRIVE_bm				0b0000000011000000
#define TIER_DRIVE_bp				6
#define SPECIAL_SCHD_ACTIVE_bm		0b0000111100000000
#define SPECIAL_SCHD_ACTIVE_bp		8
#define SEASON_bm					0b1111000000000000
#define SEASON_bp					12

#define JANUARY						(uint8_t) 1
#define	FEBRUARY					(uint8_t) 2
#define	MARCH						(uint8_t) 3
#define	APRIL						(uint8_t) 4
#define MAY							(uint8_t) 5
#define	JUNE						(uint8_t) 6
#define	JULY						(uint8_t) 7
#define	AUGUST						(uint8_t) 8
#define	SEPTEMBER					(uint8_t) 9
#define OCTOBER						(uint8_t) 10
#define NOVEMBER					(uint8_t) 11
#define DECEMBER					(uint8_t) 12

#define SUNDAY						(uint8_t) 0
#define MONDAY						(uint8_t) 1
#define TUESDAY						(uint8_t) 2
#define WEDNESDAY					(uint8_t) 3
#define THURSDAY					(uint8_t) 4
#define FRIDAY						(uint8_t) 5
#define SATURDAY					(uint8_t) 6

#define MAX_NBR_DAY_TIERS			 7
#define MAX_NBR_YEAR_SEASONS		15
#define MAX_ACTIONS_PER_DAY			 5

#define SECOND_CHANGE_bm			(uint8_t)0x01
#define MINUTE_CHANGE_bm			(uint8_t)0x02
#define HOUR_CHANGE_bm				(uint8_t)0x04
#define DAY_CHANGE_bm				(uint8_t)0x08
#define MONTH_CHANGE_bm				(uint8_t)0x10
#define YEAR_CHANGE_bm				(uint8_t)0x20

/*===========================================================================*\
 * MM100 ST_50. Dimension Time and TOU Table (max values)
\*===========================================================================*/

/* TYPE TIME_FUNC_FLAG1 = BIT FIELD OF UINT8 */
#define TOU_SELF_READ_FLAG_bm			0b00000001
#define TOU_SELF_READ_FLAG_bp			0
#define SEASON_SELF_READ_FLAG_bm		0b00000010
#define SEASON_SELF_READ_FLAG_bp		1
#define SEASON_DEMAND_RESET_FLAG_bm		0b00000100
#define SEASON_DEMAND_RESET_FLAG_bp		2
#define SEASON_CHNG_ARMED_FLAG_bm		0b00001000
#define SEASON_CHNG_ARMED_FLAG_bp		3
#define SORT_DATES_FLAG_bm				0b00010000
#define SORT_DATES_FLAG_bp				4
#define ANCHOR_DATE_FLAG_bm				0b00100000
#define ANCHOR_DATE_FLAG_bp				5

/* TYPE TIME_FUNC_FLAG2 = BIT FIELD OF UINT8 */
#define CAP_DST_AUTO_FLAG_bm			0b00000001
#define CAP_DST_AUTO_FLAG_bp			0
#define SEPARATE_WEEKDAYS_FLAG_bm		0b00000010
#define SEPARATE_WEEKDAYS_FLAG_bp		1
#define SEPARATE_SUM_DEMANDS_FLAG_bm	0b00000100
#define SEPARATE_SUM_DEMANDS_FLAG_bp	2
#define SORT_TIER_SWITCHES_FLAG_bm		0b00001000
#define SORT_TIER_SWITCHES_FLAG_bp		3
#define CAP_TM_ZN_OFFSET_FLAG_bm		0b00010000
#define CAP_TM_ZN_OFFSET_FLAG_bp		4

/* TYPE CALENDAR_BFLD = BIT FIELD OF UINT8 */
#define NBR_SEASONS_bm					0b00001111
#define NBR_SEASONS_bp					0
#define NBR_SPECIAL_SCHED_bm			0b11110000
#define NBR_SPECIAL_SCHED_bp			4

/* MAX_CALENDAR_FUNC_VALUES */
#define MAX_NBR_SEASONS					12
#define MAX_NBR_SPECIAL_SCHED			2

#define PERM_TIME_FUNC_FLAG1			(uint8_t) ( TOU_SELF_READ_FLAG_bm | SORT_DATES_FLAG_bm | ANCHOR_DATE_FLAG_bm )
#define PERM_TIME_FUNC_FLAG2			(uint8_t) ( SEPARATE_WEEKDAYS_FLAG_bm | SORT_TIER_SWITCHES_FLAG_bm | CAP_TM_ZN_OFFSET_FLAG_bm )
#define MAX_CALENDAR_FUNC_VALUES		(uint8_t) ( (MAX_NBR_SPECIAL_SCHED << NBR_SPECIAL_SCHED_bp) | (MAX_NBR_SEASONS << NBR_SEASONS_bp) )
#define MAX_NBR_NON_RECURR_DATES		(uint8_t)  50
#define MAX_NBR_RECURR_DATES			(uint8_t)  40
#define MAX_NBR_TIER_SWITCHES			(uint16_t) 80
#define MAX_CALENDAR_TBL_SIZE			(uint16_t) 620

/*===========================================================================*\
 * MM100 ST_51. Actual Time and TOU Table (default values)
\*===========================================================================*/
/* DEFAULT_CALENDAR_FUNC_VALUES */
#define DEFAULT_NBR_SEASONS				0
#define DEFAULT_NBR_SPECIAL_SCHED		0
									  
#define DEFAULT_TIME_FUNC_FLAG1			(uint8_t) ( TOU_SELF_READ_FLAG_bm | SORT_DATES_FLAG_bm )
#define DEFAULT_TIME_FUNC_FLAG2			(uint8_t) ( SORT_TIER_SWITCHES_FLAG_bm | CAP_TM_ZN_OFFSET_FLAG_bm )
#define DEFAULT_CALENDAR_FUNC_VALUES	(uint8_t) ( (DEFAULT_NBR_SPECIAL_SCHED << NBR_SPECIAL_SCHED_bp) | (DEFAULT_NBR_SEASONS << NBR_SEASONS_bp) )
#define DEFAULT_NBR_NON_RECURR_DATES	(uint8_t)	0
#define DEFAULT_NBR_RECURR_DATES		(uint8_t)	0
#define DEFAULT_NBR_TIER_SWITCHES		(uint16_t)	0
#define DEFAULT_CALENDAR_TBL_SIZE		(uint16_t)	207

/*===========================================================================*\
 * MM100 ST_53. Time Offset Table (default values)
\*===========================================================================*/
#define DEFAULT_DST_TIME_EFF		(uint32_t) 7200			// 2AM - 7200sec
#define DEFAULT_DST_TIME_AMT		(uint8_t)    60			// 60mins	
#define DEFAULT_TIME_ZONE_OFFSET	(int16_t)  -360			// UTC-06:00	
#define DEFAULT_STD_TIME_EFF		(uint32_t) 7200			// 2AM - 7200sec

/*===========================================================================*\
 * C12.19 Decade 5 Tables Size
\*===========================================================================*/

/* ST_50 Elements size */
#define ST_50_SIZE										9

/* ST_51 Elements size */
#define ST_51_SIZE										9

#define ST_51_TIME_FUNC_FLAG1_SIZE						1
#define ST_51_TIME_FUNC_FLAG2_SIZE						1
#define ST_51_CALENDAR_FUNC_SIZE						1
#define ST_51_NBR_NON_RECURR_DATES_SIZE					1
#define ST_51_NBR_RECURR_DATES_SIZE						1
#define ST_51_NBR_TIER_SWITCHES_SIZE					2
#define ST_51_CALENDAR_TBL_SIZE_SIZE					2
#define ST_51_MAX_SIZE								   (ST_51_TIME_FUNC_FLAG1_SIZE			+ \	
														ST_51_TIME_FUNC_FLAG2_SIZE			+ \
														ST_51_CALENDAR_FUNC_SIZE			+ \
														ST_51_NBR_NON_RECURR_DATES_SIZE		+ \
														ST_51_NBR_RECURR_DATES_SIZE			+ \
														ST_51_NBR_TIER_SWITCHES_SIZE		+ \
														ST_51_CALENDAR_TBL_SIZE_SIZE		)
														
/* ST_52 Elements size */
#define ST_52_SIZE										5

/* ST_53 Elements size */
#define ST_53_DST_TIME_EFF_SIZE							4
#define ST_53_DST_TIME_AMT_SIZE							1
#define ST_53_TIME_ZONE_OFFSET_SIZE						2
#define ST_53_STD_TIME_EFF_SIZE							4
#define ST_53_MAX_SIZE								   (ST_53_DST_TIME_EFF_SIZE			+ \
														ST_53_DST_TIME_AMT_SIZE			+ \
														ST_53_TIME_ZONE_OFFSET_SIZE		+ \
														ST_53_STD_TIME_EFF_SIZE			)

/* ST_54 Elements size */
#define ST_54_ANCHOR_DATE_SIZE							2
#define ST_54_NON_RECURR_DATES_SIZE						(3 * MAX_NBR_NON_RECURR_DATES)
#define ST_54_RECURR_DATES_SIZE							(3 * MAX_NBR_RECURR_DATES)
#define ST_54_TIER_SWITCHES_SIZE						(3 * MAX_NBR_TIER_SWITCHES)
#define ST_54_DAILY_SCHEDULE_ID_MATRIX_SIZE				((3 + 1*MAX_NBR_SPECIAL_SCHED) * MAX_NBR_SEASONS)
#define ST_54_MAX_SIZE								   (ST_54_ANCHOR_DATE_SIZE				+ \
														ST_54_NON_RECURR_DATES_SIZE			+ \
														ST_54_RECURR_DATES_SIZE				+ \
														ST_54_TIER_SWITCHES_SIZE			+ \
														ST_54_DAILY_SCHEDULE_ID_MATRIX_SIZE	)

/* ST_55 Elements size */
#define ST_55_SIZE										7

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
 * CALENDAR_CTRL ID
\*===========================================================================*/
typedef enum {
	
	NO_ACTION = 0,
	DAYLIGHT_SAVING_TIME_ON,
	DAYLIGHT_SAVING_TIME_OFF,
	SELECT_SEASON_0,
	SELECT_SEASON_1,
	SELECT_SEASON_2,
	SELECT_SEASON_3,
	SELECT_SEASON_4,
	SELECT_SEASON_5,
	SELECT_SEASON_6,
	SELECT_SEASON_7,
	SELECT_SEASON_8,
	SELECT_SEASON_9,
	SELECT_SEASON_10,
	SELECT_SEASON_11,
	SELECT_SEASON_12,
	SELECT_SEASON_13,
	SELECT_SEASON_14,
	SELECT_SEASON_15,
	SPECIAL_SCHEDULE_0,
	SPECIAL_SCHEDULE_1,
	SPECIAL_SCHEDULE_2,
	SPECIAL_SCHEDULE_3,
	SPECIAL_SCHEDULE_4,
	SPECIAL_SCHEDULE_5,
	SPECIAL_SCHEDULE_6,
	SPECIAL_SCHEDULE_7,
	SPECIAL_SCHEDULE_8,
	SPECIAL_SCHEDULE_9,
	SPECIAL_SCHEDULE_10,
	SPECIAL_SCHEDULE_11
		
} CALENDAR_CTRL_ID_TYPE;

/*===========================================================================*\
 * SEASON ID
\*===========================================================================*/
typedef enum {
	
	SEASON_0 = 0,
	SEASON_1,
	SEASON_2,
	SEASON_3,
	SEASON_4,
	SEASON_5,
	SEASON_6,
	SEASON_7,
	SEASON_8,
	SEASON_9,
	SEASON_10,
	SEASON_11,
	SEASON_12,
	SEASON_13,
	SEASON_14,
	SEASON_15
	
} SEASON_NBR_TYPE;

/*===========================================================================*\
 * SPECIAL SCHED ID
\*===========================================================================*/
typedef enum {
	
	SPECIAL_SCHED_0 = 0,
	SPECIAL_SCHED_1,
	SPECIAL_SCHED_2,
	SPECIAL_SCHED_3,
	SPECIAL_SCHED_4,
	SPECIAL_SCHED_5,
	SPECIAL_SCHED_6,
	SPECIAL_SCHED_7,
	SPECIAL_SCHED_8,
	SPECIAL_SCHED_9,
	SPECIAL_SCHED_10,
	SPECIAL_SCHED_11,
	RESERVED_SCHED_0,
	RESERVED_SCHED_1,
	RESERVED_SCHED_2,
	NO_SPECIAL_SCHED_ACTIVE
	
} SPECIAL_SCHED_ID_TYPE;

/*===========================================================================*\
 * ST_50 & ST_51 structure												       
\*===========================================================================*/
typedef struct {
	
	uint8_t time_func_flag1;
	uint8_t time_func_flag2;
	uint8_t calendar_func;
	uint8_t nbr_non_recurr_dates;
	uint8_t nbr_recurr_dates;
	uint16_t nbr_tier_switches;
	uint16_t calendar_tbl_size;
	
} ST_50_TYPE;

/*===========================================================================*\
 * ST_54 structures
\*===========================================================================*/
typedef struct {
	
	uint16_t recurr_date;
	uint8_t calendar_action;
	
} RECURR_DATE_RCD_TYPE;

typedef struct {
	
	uint16_t non_recurr_date;
	uint8_t calendar_action;
	
} NON_RECURR_DATE_RCD_TYPE;

typedef struct {
	
	uint16_t tier_switch;
	uint8_t day_sch_num;
	
} TIER_SWITCH_RCD_TYPE;

typedef struct {
	
	uint8_t sunday_schedule;
	uint8_t monday_schedule;
	uint8_t tuesday_schedule;
	uint8_t wednesday_schedule;
	uint8_t thursday_schedule;
	uint8_t friday_schedule;
	uint8_t saturday_schedule;
	uint8_t special_schedule[MAX_NBR_SPECIAL_SCHED];
	
} SCHEDULE_RCD_TYPE;

typedef struct {
	
	uint8_t saturday_schedule;
	uint8_t sunday_schedule;
	uint8_t weekday_schedule;
	uint8_t special_schedule[MAX_NBR_SPECIAL_SCHED];
	
} WEEKDAYS_SCHEDULE_RCD_TYPE;

typedef struct {
	
	uint16_t anchor_date;
	uint16_t non_recurr_dates[MAX_NBR_NON_RECURR_DATES];
	uint16_t recurr_dates[MAX_NBR_RECURR_DATES];
	TIER_SWITCH_RCD_TYPE tier_switches[MAX_NBR_TIER_SWITCHES];
	WEEKDAYS_SCHEDULE_RCD_TYPE daily_schedule_id_matrix[MAX_NBR_SEASONS];
	
} CALENDAR_RCD_TYPE;

/*===========================================================================*\
 * TOU structures
\*===========================================================================*/
typedef struct {
	
	uint8_t tier_id;
	uint8_t summ_switch_flag;
	uint8_t demand_switch_flag;
	uint32_t tier_switch;
	
} TIER_SWITCH_INFO_TYPE;

typedef struct {
	
	uint8_t season_id;
	uint32_t season_switch;
	
} SEASON_INFO_TYPE;

typedef struct {
	
	uint8_t nbr_day_tiers;
	TIER_SWITCH_INFO_TYPE day_tier_switches[MAX_NBR_DAY_TIERS];
	
	uint8_t nbr_year_seasons;
	SEASON_INFO_TYPE year_season_switches[MAX_NBR_YEAR_SEASONS];
	
	uint8_t curr_season;
	uint8_t curr_day_sched;
	uint8_t curr_tier;
	uint8_t curr_tier_sum;
	uint8_t curr_tier_dem;
	uint8_t curr_special_sched_active;
	
	uint8_t pending_season_change;
	uint8_t pending_demand_reset;
	uint8_t pending_self_read;
	
} TOU_TYPE;

typedef struct {
	
	xTaskHandle task_id;
	bool rtos_task_active;

	uint8_t rtc_change_mask;
	
	bool register_dst_on;
	bool register_dst_off;
	uint32_t event_timestamp;
	uint8_t user;
	
} TOU_OS_TYPE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
extern TOU_OS_TYPE tou_os;

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/
extern void TOU_APP_vInitialize(void);
extern void TOU_APP_vCreate_task(void);
extern void TOU_APP_vRtos_task(void *pvParameters);
extern void TOU_APP_vTask(void);

extern void TOU_APP_vSuspend_rtos_task(void);
extern void TOU_APP_vResume_rtos_task(void);

extern void TOU_APP_vSeason_change(uint16_t user, uint8_t season, uint32_t timestamp);

extern void TOU_APP_vRead_eeprom_current_season(void);
extern void TOU_APP_vWrite_eeprom_current_season(void);

extern void TOU_APP_vLoad_st_54_date_to_human_date(HUMAN_TIME_TYPE *pHuman_date, RTC_TYPE *rtc, uint16_t st_54_date, bool is_recurrent);

extern uint8_t TOU_APP_ucLoad_dst_dates(RTC_TYPE* rtc);

extern uint8_t TOU_APP_ucGet_current_season(void);
extern uint8_t TOU_APP_ucGet_current_tier(void);
extern uint8_t TOU_APP_ucGet_current_tier_dem(void);
extern uint8_t TOU_APP_ucGet_current_tier_sum(void);

extern bool TOU_APP_st_50_execute_table_reading(uint8_t *table_buffer);
extern uint16_t TOU_APP_sSt_50_get_size(void);

extern bool TOU_APP_bLoad_st_51(void);
extern bool TOU_APP_bSt_51_restore_default_values(void);
extern bool TOU_APP_bSt_51_refresh_ram(void);
extern bool TOU_APP_bSt_51_execute_table_reading(uint8_t *table_buffer);
extern bool TOU_APP_bSt_51_execute_table_writing(uint8_t *table_buffer);
extern void TOU_APP_vCopy_st_51_nvm(uint8_t* destination);
extern void TOU_APP_vCopy_st_51_ram(uint8_t* destination);
extern void TOU_APP_vSt_51_recover_nvm(void);
extern void TOU_APP_vSt_51_recover_ram(void);
extern uint16_t TOU_APP_usSt_51_get_size(void);

extern bool TOU_APP_bSt_52_execute_table_reading(uint8_t *table_buffer);
extern uint16_t TOU_APP_usSt_52_get_size(void);

extern bool TOU_APP_bLoad_st_53(void);
extern bool TOU_APP_bSt_53_restore_default_values(void);
extern bool TOU_APP_bSt_53_execute_table_writing(uint8_t *table_buffer);
extern bool TOU_APP_bSt_53_execute_table_reading(uint8_t *table_buffer);
extern uint16_t TOU_APP_usSt_53_get_size(void);

extern bool TOU_APP_bSt_54_execute_table_writing(uint8_t *table_buffer);
extern bool TOU_APP_bSt_54_execute_table_reading(uint8_t *table_buffer);
extern uint16_t TOU_APP_usSt_54_get_size(void);

extern bool TOU_APP_bSt_55_execute_table_reading(uint8_t *table_buffer);
extern uint16_t TOU_APP_usSt_55_get_size(void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
 #endif /* TOU_H_ */
