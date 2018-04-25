/**
 ******************************************************************************
 * \file regs.h
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

#ifndef REGS_H_
#define REGS_H_

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"
#include "compiler.h"
#include "config_meter.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */
#define MAX_REGISTER_VALUE							1000000000000		// 1000000000000 mWh, mVARh  -->  1000000 kWh, kVARh
#define METERING_STACK_SIZE							configMINIMAL_STACK_SIZE * 5
#define METERING_TASK_PRIO							tskIDLE_PRIORITY + 4

/*===========================================================================*\
 * ST_10. Data Source Dimension Limits Table (max values)
\*===========================================================================*/
/* SOURCE_FLAGS BIT MASK*/
#define PF_EXCLUDE_FLAG_bm							0b00000001
#define RESET_EXCLUDE_FLAG_bm						0b00000010
#define BLOCK_DEMAND_FLAG_bm						0b00000100
#define SLIDING_DEMAND_FLAG_bm						0b00001000
#define THERMAL_DEMAND_FLAG_bm						0b00010000
#define SET1_PRESENT_FLAG_bm						0b00100000
#define SET2_PRESENT_FLAG_bm						0b01000000

#define PERM_SOURCE_FLAGS							(uint8_t)( BLOCK_DEMAND_FLAG_bm | SLIDING_DEMAND_FLAG_bm | SET1_PRESENT_FLAG_bm )
#define MAX_NBR_UOM_ENTRIES							(uint8_t) 255
#define MAX_NBR_DEMAND_CTRL_ENTRIES					(uint8_t) 255
#define MAX_DATA_CTRL_LENGTH						(uint8_t)   0
#define MAX_NBR_DATA_CTRL_ENTRIES					(uint8_t)   0
#define MAX_NBR_CONSTANTS_ENTRIES					(uint8_t) 255
#define MAX_CONSTANTS_SELECTOR						(uint8_t)   2
#define MAX_NBR_SOURCES								(uint8_t) 255

/*===========================================================================*\
 * MM100 ST_11. Actual Data Sources Limiting Table (default values)
\*===========================================================================*/
#define DEFAULT_SOURCE_FLAGS						(uint8_t)( BLOCK_DEMAND_FLAG_bm | SET1_PRESENT_FLAG_bm )
#define DEFAULT_NBR_UOM_ENTRIES						(uint8_t) 145
#define DEFAULT_NBR_DEMAND_CTRL_ENTRIES				(uint8_t)  56
#define DEFAULT_DATA_CTRL_LENGTH					(uint8_t)   0
#define DEFAULT_NBR_DATA_CTRL_ENTRIES				(uint8_t)   0
#define DEFAULT_NBR_CONSTANTS_ENTRIES				(uint8_t) 145
#define DEFAULT_CONSTANTS_SELECTOR					(uint8_t)   2
#define DEFAULT_NBR_SOURCES							(uint8_t) 255

/*===========================================================================*\
 * ST_12. Units of Measure Entry Table (default values)
\*===========================================================================*/

/*---------------------------------------------------------------------------*\
 * Summations: to be used as source for ST_23 (Summations) and ST_64 (Load Profile)
\*---------------------------------------------------------------------------*/
#define UOM_SUM_Wh_Del								0x00025800		// Total Delivered Active Energy			
#define UOM_SUM_Wh_Rec								0x00019800		// Total Received Active Energy
#define UOM_SUM_Wh_Net								0x0007D800		// Total Net Active Energy
#define UOM_SUM_Wh_Sum								0x0003D800		// Total Sum Active Energy
#define UOM_SUM_Wh_Del_A							0x002A5800		// Phase A Delivered Active Energy
#define UOM_SUM_Wh_Del_B							0x00325800		// Phase B Delivered Active Energy
#define UOM_SUM_Wh_Del_C							0x003A5800		// Phase C Delivered Active Energy
#define UOM_SUM_Wh_Rec_A							0x00299800		// Phase A Received Active Energy
#define UOM_SUM_Wh_Rec_B							0x00319800		// Phase B Received Active Energy
#define UOM_SUM_Wh_Rec_C							0x00399800		// Phase C Received Active Energy
#define UOM_SUM_VARh_Q1								0x00005801		// Total Q1 Reactive Energy
#define UOM_SUM_VARh_Q2								0x00009801		// Total Q2 Reactive Energy
#define UOM_SUM_VARh_Q3								0x00011801		// Total Q3 Reactive Energy
#define UOM_SUM_VARh_Q4								0x00021801		// Total Q4 Reactive Energy
#define UOM_SUM_VARh_Del							0x0000D801		// Total Delivered Reactive Energy
#define UOM_SUM_VARh_Rec							0x00031801		// Total Received Reactive Energy
#define UOM_SUM_VARh_Net							0x0007D801		// Total Net Reactive Energy
#define UOM_SUM_VARh_Sum							0x0003D801		// Total Sum Reactive Energy
#define UOM_SUM_VARh_Q1_Q4							0x00025801		// Total (Q1+Q4) Reactive Energy
#define UOM_SUM_VARh_Q2_Q3							0x00019801		// Total (Q2+Q3) Reactive Energy
#define UOM_SUM_VAh_Del								0x00025802		// Total Delivered Apparent Energy
#define UOM_SUM_VAh_Rec								0x00019802		// Total Received Apparent Energy
#define UOM_SUM_VAh_Net								0x0007D802		// Total Net Apparent Energy
#define UOM_SUM_VAh_Sum								0x0003D802		// Total Sum Apparent Energy
#define UOM_SUM_VAh_Q1								0x00005802		// Total Q1 Apparent Energy
#define UOM_SUM_VAh_Q2								0x00009802		// Total Q2 Apparent Energy
#define UOM_SUM_VAh_Q3								0x00011802		// Total Q3 Apparent Energy
#define UOM_SUM_VAh_Q4								0x00021802		// Total Q4 Apparent Energy

/*---------------------------------------------------------------------------*\
 * Load Profile											
\*---------------------------------------------------------------------------*/
#define UOM_LP_Wh_Del								0x00024500		// Total Delivered Active Energy
#define	UOM_LP_Wh_Rec								0x00018500		// Total Received Active Energy
#define	UOM_LP_Wh_Net								0x0007C500		// Total Net Active Energy
#define	UOM_LP_Wh_Sum								0x0003C500		// Total Sum Active Energy
#define	UOM_LP_Wh_Del_A								0x002A4500		// Phase A Delivered Active Energy
#define	UOM_LP_Wh_Del_B								0x00324500		// Phase B Delivered Active Energy
#define	UOM_LP_Wh_Del_C								0x003A4500		// Phase C Delivered Active Energy
#define	UOM_LP_Wh_Rec_A								0x00298500		// Phase A Received Active Energy
#define	UOM_LP_Wh_Rec_B								0x00318500		// Phase B Received Active Energy
#define	UOM_LP_Wh_Rec_C								0x00398500		// Phase C Received Active Energy
#define	UOM_LP_VARh_Q1								0x00004501		// Total Q1 Reactive Energy
#define	UOM_LP_VARh_Q2								0x00008501		// Total Q2 Reactive Energy
#define	UOM_LP_VARh_Q3								0x00010501		// Total Q3 Reactive Energy
#define	UOM_LP_VARh_Q4								0x00020501		// Total Q4 Reactive Energy
#define	UOM_LP_VARh_Del								0x0000C501		// Total Delivered Reactive Energy
#define	UOM_LP_VARh_Rec								0x00030501		// Total Received Reactive Energy
#define	UOM_LP_VARh_Net								0x0007C501		// Total Net Reactive Energy
#define	UOM_LP_VARh_Sum								0x0003C501		// Total Sum Reactive Energy
#define	UOM_LP_VARh_Q1_Q4							0x00024501		// Total (Q1+Q4) Reactive Energy
#define	UOM_LP_VARh_Q2_Q3							0x00018501		// Total (Q2+Q3) Reactive Energy
#define	UOM_LP_VAh_Del								0x00024502		// Total Delivered Apparent Energy
#define	UOM_LP_VAh_Rec								0x00018502		// Total Received Apparent Energy
#define	UOM_LP_VAh_Net								0x0007C502		// Total Net Apparent Energy
#define	UOM_LP_VAh_Sum								0x0003C502		// Total Sum Apparent Energy
#define	UOM_LP_VAh_Q1								0x00004502		// Total Q1 Apparent Energy
#define	UOM_LP_VAh_Q2								0x00008502		// Total Q2 Apparent Energy
#define	UOM_LP_VAh_Q3								0x00010502		// Total Q3 Apparent Energy
#define	UOM_LP_VAh_Q4								0x00020502		// Total Q4 Apparent Energy

/*---------------------------------------------------------------------------*\
 * Average Demand: to be used as source for ST_23 (Max/Min and Cumulative)												
\*---------------------------------------------------------------------------*/
#define UOM_AD_W_Del								0x00025C00		// Total Delivered Active Power
#define UOM_AD_W_Rec								0x00019C00		// Total Received Active Power
#define UOM_AD_W_Net								0x0007DC00		// Total Net Active Power
#define UOM_AD_W_Sum								0x0003DC00		// Total Sum Active Power
#define UOM_AD_W_Del_A								0x002A5C00		// Phase A Delivered Active Power
#define UOM_AD_W_Del_B								0x00325C00		// Phase B Delivered Active Power
#define UOM_AD_W_Del_C								0x003A5C00		// Phase C Delivered Active Power
#define UOM_AD_W_Rec_A								0x00299C00		// Phase A Received Active Power
#define UOM_AD_W_Rec_B								0x00319C00		// Phase B Received Active Power
#define UOM_AD_W_Rec_C								0x00399C00		// Phase C Received Active Power
#define UOM_AD_VAR_Q1								0x00005C01		// Total Q1 Reactive Power
#define UOM_AD_VAR_Q2								0x00009C01		// Total Q2 Reactive Power
#define UOM_AD_VAR_Q3								0x00011C01		// Total Q3 Reactive Power
#define UOM_AD_VAR_Q4								0x00021C01		// Total Q4 Reactive Power
#define UOM_AD_VAR_Del								0x0000DC01		// Total Delivered Reactive Power
#define UOM_AD_VAR_Rec								0x00031C01		// Total Received Reactive Power
#define UOM_AD_VAR_Net								0x0007DC01		// Total Net Reactive Energy
#define UOM_AD_VAR_Sum								0x0003DC01		// Total Sum Reactive Energy
#define UOM_AD_VAR_Q1_Q4							0x00025C01		// Total (Q1+Q4) Reactive Energy
#define UOM_AD_VAR_Q2_Q3							0x00019C01		// Total (Q2+Q3) Reactive Energy
#define UOM_AD_VA_Del								0x00025C02		// Total Delivered Apparent Energy
#define UOM_AD_VA_Rec								0x00019C02		// Total Received Apparent Energy
#define UOM_AD_VA_Net								0x0007DC02		// Total Net Apparent Energy
#define UOM_AD_VA_Sum								0x0003DC02		// Total Sum Apparent Energy
#define UOM_AD_VA_Q1								0x00005C02		// Total Q1 Apparent Energy
#define UOM_AD_VA_Q2								0x00009C02		// Total Q2 Apparent Energy
#define UOM_AD_VA_Q3								0x00011C02		// Total Q3 Apparent Energy
#define UOM_AD_VA_Q4								0x00021C02		// Total Q4 Apparent Energy

/*---------------------------------------------------------------------------*\
 * Instantaneous Demand: to be used as source for ST_28 (Instantaneous value)																								
\*---------------------------------------------------------------------------*/
#define UOM_ID_W_Inst								0x0003D900		// Total Instantaneous Active Power
#define UOM_ID_W_Inst_A								0x002BD900		// Phase A Instantaneous Active Power
#define UOM_ID_W_Inst_B								0x0033D900		// Phase B Instantaneous Active Power
#define UOM_ID_W_Inst_C								0x003BD900		// Phase C Instantaneous Active Power
#define UOM_ID_VAR_inst								0x0003D901		// Total Instantaneous Reactive Power
#define UOM_ID_VAR_inst_A							0x002BD901		// Phase A Instantaneous Reactive Power
#define UOM_ID_VAR_inst_B							0x0033D901		// Phase B Instantaneous Reactive Power
#define UOM_ID_VAR_inst_C							0x003BD901		// Phase C Instantaneous Reactive Power
#define UOM_ID_VA_inst								0x0003D902		// Total Instantaneous Apparent Power
#define UOM_ID_VA_inst_A							0x002BD902		// Phase A Instantaneous Apparent Power
#define UOM_ID_VA_inst_B							0x0033D902		// Phase B Instantaneous Apparent Power
#define UOM_ID_VA_inst_C							0x003BD902		// Phase C Instantaneous Apparent Power

/*---------------------------------------------------------------------------*\
 * Instantaneous Values: to be used as source for ST_28																																				
\*---------------------------------------------------------------------------*/
#define UOM_IV_V_A									0x0028110B		// Phase A-N Voltage
#define UOM_IV_V_B									0x0030110B		// Phase B-N Voltage
#define UOM_IV_V_C									0x0038110B		// Phase C-N Voltage
#define UOM_IV_I_A									0x0028110F		// Phase A Current
#define UOM_IV_I_B									0x0030110F		// Phase B Current
#define UOM_IV_I_C									0x0038110F		// Phase C Current
#define UOM_IV_V_A_angle							0x00280114		// Phase A Voltage Phase Angle
#define UOM_IV_V_B_angle							0x00300114		// Phase B Voltage Phase Angle
#define UOM_IV_V_C_angle							0x00380114		// Phase C Voltage Phase Angle
#define UOM_IV_I_A_angle							0x00280116		// Phase A Current Phase Angle
#define UOM_IV_I_B_angle							0x00300116		// Phase B Current Phase Angle
#define UOM_IV_I_C_angle							0x00380116		// Phase C Current Phase Angle
#define UOM_IV_PF_angle								0x0000011B		// Total Power Factor Angle
#define UOM_IV_PF_A_angle							0x0028011B		// Phase A Power Factor Angle
#define UOM_IV_PF_B_angle							0x0030011B		// Phase B Power Factor Angle
#define UOM_IV_PF_C_angle							0x0038011B		// Phase C Power Factor Angle
#define UOM_IV_PF									0x00000118		// Total Power Factor
#define UOM_IV_PF_A									0x00280118		// Phase A Power Factor
#define UOM_IV_PF_B									0x00300118		// Phase B Power Factor
#define UOM_IV_PF_C									0x00380118		// Phase C Power Factor
#define UOM_IV_f									0x00000121		// Frequency

/*---------------------------------------------------------------------------*\
 * Present Demand: to be used as source for ST_28																																				
\*---------------------------------------------------------------------------*/
#define UOM_PD_W_Del								0x00025B00		// Total Delivered Active Present Power
#define UOM_PD_W_Rec								0x00019B00		// Total Received Active Present Power
#define UOM_PD_W_Net								0x0007DB00		// Total Net Active Present Power
#define UOM_PD_W_Sum								0x0003DB00		// Total Sum Active Present Power
#define UOM_PD_W_Del_A								0x002A5B00		// Phase A Delivered Active Present Power
#define UOM_PD_W_Del_B								0x00325B00		// Phase B Delivered Active Present Power
#define UOM_PD_W_Del_C								0x003A5B00		// Phase C Delivered Active Present Power
#define UOM_PD_W_Rec_A								0x00299B00		// Phase A Received Active Present Power
#define UOM_PD_W_Rec_B								0x00319B00		// Phase B Received Active Present Power
#define UOM_PD_W_Rec_C								0x00399B00		// Phase C Received Active Present Power
#define UOM_PD_VAR_Q1								0x00005B01		// Total Q1 Reactive Present Power
#define UOM_PD_VAR_Q2								0x00009B01		// Total Q2 Reactive Present Power
#define UOM_PD_VAR_Q3								0x00011B01		// Total Q3 Reactive Present Power
#define UOM_PD_VAR_Q4								0x00021B01		// Total Q4 Reactive Present Power
#define UOM_PD_VAR_Del								0x0000DB01		// Total Delivered Reactive Present Power
#define UOM_PD_VAR_Rec								0x00031B01		// Total Received Reactive Present Power
#define UOM_PD_VAR_Net								0x0007DB01		// Total Net Reactive Present Power
#define UOM_PD_VAR_Sum								0x0003DB01		// Total Sum Reactive Present Power
#define UOM_PD_VAR_Q1_Q4							0x00025B01		// Total (Q1+Q4) Reactive Present Power
#define UOM_PD_VAR_Q2_Q3							0x00019B01		// Total (Q2+Q3) Reactive Present Power
#define UOM_PD_VA_Del								0x00025B02		// Total Delivered Apparent Present Power
#define UOM_PD_VA_Rec								0x00019B02		// Total Received Apparent Present Power
#define UOM_PD_VA_Net								0x0007DB02		// Total Net Apparent Present Power
#define UOM_PD_VA_Sum								0x0003DB02		// Total Sum Apparent Present Power
#define UOM_PD_VA_Q1								0x00005B02		// Total Q1 Apparent Present Power
#define UOM_PD_VA_Q2								0x00009B02		// Total Q2 Apparent Present Power
#define UOM_PD_VA_Q3								0x00011B02		// Total Q3 Apparent Present Power
#define UOM_PD_VA_Q4								0x00021B02		// Total Q4 Apparent Present Power

/*===========================================================================*\
 * ST_13. Demand Control Table (default values)
\*===========================================================================*/
#define DEFAULT_RESET_EXCLUSION						(uint8_t)		60
#define DEFAULT_P_FAIL_RECOGNTN_TM					(uint8_t)		 3
#define DEFAULT_P_FAIL_EXCLUSION					(uint8_t)		15
#define DEFAULT_COLD_LOAD_PICKUP					(uint8_t)		 0
#define DEFAULT_SUB_INT								(uint8_t)		 5 // 5 minutes for subinterval length
#define DEFAULT_INT_MULTIPLIER						(uint8_t)		 3 // interval multiplier
#define DEFAULT_INT_LENGTH							(uint16_t)		15 // only used if BLOCK_DEMAND is activated

/*===========================================================================*\
 * ST_15. Constants Table (default values)
\*===========================================================================*/
#define DEFAULT_MULTIPLIER							(uint64_t)	1
#define DEFAULT_OFFSET								(uint64_t)	0
#define DEFAULT_SET_1_FLAGS							(uint8_t)	0
#define DEFAULT_SET_1_RATIO_F1						(uint64_t)	1
#define DEFAULT_SET_1_RATIO_P1						(uint64_t)	1
#define DEFAULT_SET_2_FLAGS							(uint8_t)	0
#define DEFAULT_SET_2_RATIO_F1						(uint64_t)	1
#define DEFAULT_SET_2_RATIO_P1						(uint64_t)	1

/*===========================================================================*\
 * ST_16. Source Definition Table (default values)
\*===========================================================================*/
#define DEFAULT_SOURCE_SUMMATIONS					(uint8_t)	0x39
#define DEFAULT_SOURCE_LOAD_PROFILE					(uint8_t)	0x39
#define DEFAULT_SOURCE_AVERAGE_DEMAND				(uint8_t)	0x3B
#define DEFAULT_SOURCE_INSTANTANEOUS_DEMAND			(uint8_t)	0x39
#define DEFAULT_SOURCE_INSTANTANEOUS_VALUE			(uint8_t)	0x39
#define DEFAULT_SOURCE_PRESENT_DEMAND				(uint8_t)	0x3B

/*===========================================================================*\
 * ST_20. Register Dimension Limits Table (max values)
\*===========================================================================*/
/* TYPE REG_FUNC1_BFLD = BIT FIELD OF UINT8 */
#define SEASON_INFO_FIELD_FLAG_bm					0b00000001
#define SEASON_INFO_FIELD_FLAG_bp					0
#define DATE_TIME_FIELD_FLAG_bm						0b00000010
#define DATE_TIME_FIELD_FLAG_bp						1
#define DEMAND_RESET_CTR_FLAG_bm					0b00000100
#define DEMAND_RESET_CTR_FLAG_bp					2
#define DEMAND_RESET_LOCK_FLAG_bm					0b00001000
#define DEMAND_RESET_LOCK_FLAG_bp					3
#define CUM_DEMAND_FLAG_bm							0b00010000
#define CUM_DEMAND_FLAG_bp							4
#define CONT_CUM_DEMAND_FLAG_bm						0b00100000
#define CONT_CUM_DEMAND_FLAG_bp						5
#define TIME_REMAINING_FLAG_bm						0b01000000
#define TIME_REMAINING_FLAG_bp						6

/* TYPE REG_FUNC2_BFLD = BIT FIELD OF UINT8 */
#define SELF_READ_INHIBIT_OVERFLOW_FLAG_bm			0b00000001
#define SELF_READ_INHIBIT_OVERFLOW_FLAG_bp			0
#define SELF_READ_SEQ_NBR_FLAG_bm					0b00000010
#define SELF_READ_SEQ_NBR_FLAG_bp					1
#define DAILY_SELF_READ_FLAG_bm						0b00000100
#define DAILY_SELF_READ_FLAG_bp						2
#define WEEKLY_SELF_READ_FLAG_bm					0b00001000
#define WEEKLY_SELF_READ_FLAG_bp					3
#define SELF_READ_DEMAND_RESET_bm					0b00110000
#define SELF_READ_DEMAND_RESET_bp					4

#define PERM_REG_FUNC1_FLAGS						(uint8_t) ( SEASON_INFO_FIELD_FLAG_bm | DATE_TIME_FIELD_FLAG_bm | DEMAND_RESET_CTR_FLAG_bm | DEMAND_RESET_LOCK_FLAG_bm | CUM_DEMAND_FLAG_bm | CONT_CUM_DEMAND_FLAG_bm | TIME_REMAINING_FLAG_bm )
#define PERM_REG_FUNC2_FLAGS						(uint8_t) ( SELF_READ_SEQ_NBR_FLAG_bm | DAILY_SELF_READ_FLAG_bm | (1 << SELF_READ_DEMAND_RESET_bp) )
#define MAX_NBR_SELF_READS							(uint8_t) 14 
#define MAX_NBR_SUMMATIONS							(uint8_t) 6
#define MAX_NBR_DEMANDS								(uint8_t) 2
#define MAX_NBR_COIN_VALUES							(uint8_t) 0
#define MAX_NBR_OCCUR								(uint8_t) 1
#define MAX_NBR_TIERS								(uint8_t) 4
#define MAX_NBR_PRESENT_DEMANDS						(uint8_t) MAX_NBR_DEMANDS
#define MAX_NBR_PRESENT_VALUES						(uint8_t) 40

/*===========================================================================*\
 * ST_21. Actual Register Limiting Table (default values)
\*===========================================================================*/
#define DEFAULT_REG_FUNC1_FLAGS						(uint8_t) ( SEASON_INFO_FIELD_FLAG_bm | DATE_TIME_FIELD_FLAG_bm | DEMAND_RESET_CTR_FLAG_bm | CUM_DEMAND_FLAG_bm | TIME_REMAINING_FLAG_bm )
#define DEFAULT_REG_FUNC2_FLAGS						(uint8_t) ( SELF_READ_SEQ_NBR_FLAG_bm )
#define DEFAULT_NBR_SELF_READS						(uint8_t) 14
#define DEFAULT_NBR_SUMMATIONS						(uint8_t) 6
#define DEFAULT_NBR_DEMANDS							(uint8_t) 2
#define DEFAULT_NBR_COIN_VALUES						(uint8_t) 0
#define DEFAULT_NBR_OCCUR							(uint8_t) 1
#define DEFAULT_NBR_TIERS							(uint8_t) 4
#define DEFAULT_NBR_PRESENT_DEMANDS					(uint8_t) DEFAULT_NBR_DEMANDS
#define DEFAULT_NBR_PRESENT_VALUES					(uint8_t) 6 + (NBR_PHASES * 9)

/*===========================================================================*\
 * ST_26. Self-read Data Table (default values)
\*===========================================================================*/
/* LIST_STATUS BIT MASK*/
#define ST_26_ORDER_FLAG_bm								0b00000001
#define ST_26_ORDER_FLAG_bp								0
#define ST_26_OVERFLOW_FLAG_bm							0b00000010
#define ST_26_OVERFLOW_FLAG_bp							1
#define ST_26_LIST_TYPE_FLAG_bm							0b00000100
#define ST_26_LIST_TYPE_FLAG_bp							2
#define ST_26_INHIBIT_OVERFLOW_FLAG_bm					0b00001000
#define ST_26_INHIBIT_OVERFLOW_FLAG_bp					3

#define ST_26_DEFAULT_LIST_STATUS					(uint8_t)   ( (0 << ST_26_ORDER_FLAG_bp) | (1 << ST_26_LIST_TYPE_FLAG_bm) )
#define ST_26_DEFAULT_NBR_VALID_ENTRIES				(uint16_t)  0
#define	ST_26_DEFAULT_LAST_ENTRY_ELEMENT			(uint16_t)  0
#define ST_26_DEFAULT_LAST_ENTRY_SEQ_NBR			(uint32_t)  0
#define ST_26_DEFAULT_NBR_UNREAD_ENTRIES			(uint16_t)  0

/*===========================================================================*\
 * C12.19 Decade 1 & 2 Tables Size
\*===========================================================================*/

/* ST_10 Elements size */
#define ST_10_SIZE										8

/* ST_11 Elements size */
#define ST_11_SOURCE_FLAGS_SIZE							1
#define ST_11_NBR_UOM_ENTRIES_SIZE						1
#define ST_11_NBR_DEMAND_CTRL_ENTRIES_SIZE				1
#define ST_11_DATA_CTRL_LENGTH_SIZE						1
#define ST_11_NBR_DATA_CTRL_ENTRIES_SIZE				1
#define ST_11_NBR_CONSTANTS_ENTRIES_SIZE				1
#define ST_11_CONSTANTS_SELECTOR_SIZE					1
#define ST_11_NBR_SOURCES_SIZE							1
#define ST_11_SIZE										8

/* ST_12 Elements size */														
#define ST_12_SIZE										8
														
/* ST_13 Elements size */
#define ST_13_RESET_EXCLUSION_SIZE						1
#define ST_13_P_FAIL_RECOGNTN_TM_SIZE					1
#define ST_13_P_FAIL_EXCLUSION_SIZE						1
#define ST_13_COLD_LOAD_PICKUP_SIZE						1
#define ST_13_SUB_INT_SIZE								2			//MAX_NBR_DEMAND_CTRL_ENTRIES
#define ST_13_INT_MULTIPLIER_SIZE						2			//MAX_NBR_DEMAND_CTRL_ENTRIES
#define ST_13_INT_LENGTH_SIZE							(2 * 2)		//MAX_NBR_DEMAND_CTRL_ENTRIES
#define ST_13_MAX_SIZE									(ST_13_RESET_EXCLUSION_SIZE		+ \
														ST_13_P_FAIL_RECOGNTN_TM_SIZE	+ \
														ST_13_P_FAIL_EXCLUSION_SIZE		+ \
														ST_13_COLD_LOAD_PICKUP_SIZE		+ \
														ST_13_INT_LENGTH_SIZE			)

/* ST_15 Elements size */
#define ST_15_SIZE										117

/* ST_16 Elements size */
#define ST_16_SIZE										256

/* ST_20 Elements size */
#define ST_20_SIZE										10

/* ST_21 Elements size */
#define ST_21_REG_FUNC1_BFLD_SIZE						1
#define ST_21_REG_FUNC2_BFLD_SIZE						1
#define ST_21_NBR_SELF_READS_SIZE						1
#define ST_21_NBR_SUMMATIONS_SIZE						1
#define ST_21_NBR_DEMANDS_SIZE							1
#define ST_21_NBR_COIN_VALUES_SIZE						1
#define ST_21_NBR_OCCUR_SIZE							1
#define ST_21_NBR_TIERS_SIZE							1
#define ST_21_NBR_PRESENT_DEMANDS_SIZE					1
#define ST_21_NBR_PRESENT_VALUES_SIZE					1
#define ST_21_SIZE										10
														
/* ST_22 Elements size */
#define ST_22_SUMMATION_SELECT_SIZE						MAX_NBR_SUMMATIONS
#define ST_22_DEMAND_SELECT_SIZE						MAX_NBR_DEMANDS
#define ST_22_MIN_OR_MAX_FLAGS_SIZE						((MAX_NBR_DEMANDS + 7)/8)
#define ST_22_COINCIDENT_SELECT_SIZE					MAX_NBR_COIN_VALUES
#define ST_22_COIN_DEMAND_ASSOC_SIZE					MAX_NBR_COIN_VALUES
#define ST_22_MAX_SIZE								    (ST_22_SUMMATION_SELECT_SIZE	+ \
														ST_22_DEMAND_SELECT_SIZE		+ \
														ST_22_MIN_OR_MAX_FLAGS_SIZE		+ \
														ST_22_COINCIDENT_SELECT_SIZE	+ \
														ST_22_COIN_DEMAND_ASSOC_SIZE	)

/* ST_23 Elements size */
#define ST_23_NBR_DEMAND_RESETS_SIZE					1
#define ST_23_TOT_SUMMATIONS_SIZE						(NI_FMAT1_SIZE * MAX_NBR_SUMMATIONS)
#define ST_23_TOT_DEMANDS_EVENT_TIME_SIZE				(TM_FMAT_SIZE  * MAX_NBR_DEMANDS)
#define ST_23_TOT_DEMANDS_CUM_DEMAND_SIZE				(NI_FMAT1_SIZE * MAX_NBR_DEMANDS)
#define ST_23_TOT_DEMANDS_CONT_CUM_DEMAND_SIZE			(NI_FMAT1_SIZE * MAX_NBR_DEMANDS)
#define ST_23_TOT_DEMANDS_DEMAND_SIZE					(NI_FMAT2_SIZE * MAX_NBR_DEMANDS)
#define ST_23_TOT_COINCIDENTS_SIZE						(NI_FMAT2_SIZE * MAX_NBR_COIN_VALUES)
#define ST_23_TIER_SUMMATIONS_SIZE						(NI_FMAT1_SIZE * MAX_NBR_TIERS * MAX_NBR_SUMMATIONS)
#define ST_23_TIER_DEMANDS_EVENT_TIME_SIZE				(TM_FMAT_SIZE  * MAX_NBR_TIERS * MAX_NBR_DEMANDS)
#define ST_23_TIER_DEMANDS_CUM_DEMAND_SIZE				(NI_FMAT1_SIZE * MAX_NBR_TIERS * MAX_NBR_DEMANDS)
#define ST_23_TIER_DEMANDS_CONT_CUM_DEMAND_SIZE			(NI_FMAT1_SIZE * MAX_NBR_TIERS * MAX_NBR_DEMANDS)
#define ST_23_TIER_DEMANDS_DEMAND_SIZE					(NI_FMAT2_SIZE * MAX_NBR_TIERS * MAX_NBR_DEMANDS)
#define ST_23_TIER_COINCIDENTS_SIZE						(NI_FMAT2_SIZE * MAX_NBR_TIERS * MAX_NBR_COIN_VALUES)
#define ST_23_MAX_SIZE								   (ST_23_NBR_DEMAND_RESETS_SIZE				+ \
														ST_23_TOT_SUMMATIONS_SIZE					+ \
														ST_23_TOT_DEMANDS_EVENT_TIME_SIZE			+ \
														ST_23_TOT_DEMANDS_CUM_DEMAND_SIZE			+ \
														ST_23_TOT_DEMANDS_CONT_CUM_DEMAND_SIZE		+ \
														ST_23_TOT_DEMANDS_DEMAND_SIZE				+ \
														ST_23_TOT_COINCIDENTS_SIZE					+ \
														ST_23_TIER_SUMMATIONS_SIZE					+ \
														ST_23_TIER_DEMANDS_EVENT_TIME_SIZE			+ \
														ST_23_TIER_DEMANDS_CUM_DEMAND_SIZE			+ \
														ST_23_TIER_DEMANDS_CONT_CUM_DEMAND_SIZE		+ \
														ST_23_TIER_DEMANDS_DEMAND_SIZE				+ \
														ST_23_TIER_COINCIDENTS_SIZE					)

/* ST_24 Elements size */
#define ST_24_END_DATE_TIME_SIZE						TM_FMAT_SIZE
#define ST_24_SEASON_SIZE								1
#define ST_24_PREV_SEASON_REG_DATA_SIZE					ST_23_MAX_SIZE
#define ST_24_MAX_SIZE									(ST_24_END_DATE_TIME_SIZE + ST_24_SEASON_SIZE + ST_24_PREV_SEASON_REG_DATA_SIZE)

/* ST_25 Elements size */
#define ST_25_END_DATE_TIME_SIZE						TM_FMAT_SIZE
#define ST_25_SEASON_SIZE								1
#define ST_25_PREV_DEMAND_RESET_DATA_SIZE				ST_23_MAX_SIZE
#define ST_25_MAX_SIZE									(ST_25_END_DATE_TIME_SIZE + ST_25_SEASON_SIZE + ST_25_PREV_DEMAND_RESET_DATA_SIZE)

/* ST_26 Elements size */
#define ST_26_LIST_STATUS_SIZE							1
#define ST_26_NBR_VALID_ENTRIES_SIZE					1
#define ST_26_LAST_ENTRY_ELEMENT_SIZE					1
#define ST_26_LAST_ENTRY_SEQ_NBR_SIZE					2
#define ST_26_NBR_UNREAD_ENTRIES_SIZE					1
#define ST_26_SELF_READ_SEQ_NBR_SIZE					(2 * MAX_NBR_SELF_READS)
#define ST_26_END_DATE_TIME_SIZE						(TM_FMAT_SIZE * MAX_NBR_SELF_READS)
#define ST_26_SEASON_SIZE								MAX_NBR_SELF_READS
#define ST_26_SELF_READ_REGISTER_DATA_SIZE				(ST_23_MAX_SIZE * MAX_NBR_SELF_READS)
#define ST_26_MAX_SIZE								   (ST_26_LIST_STATUS_SIZE				+ \	
														ST_26_NBR_VALID_ENTRIES_SIZE		+ \
														ST_26_LAST_ENTRY_ELEMENT_SIZE		+ \
														ST_26_LAST_ENTRY_SEQ_NBR_SIZE		+ \
														ST_26_NBR_UNREAD_ENTRIES_SIZE		+ \
														ST_26_SELF_READ_SEQ_NBR_SIZE		+ \
														ST_26_END_DATE_TIME_SIZE			+ \
														ST_26_SEASON_SIZE					+ \
														ST_26_SELF_READ_REGISTER_DATA_SIZE	)

/* ST_27 Elements size */
#define ST_27_PRESENT_DEMAND_SELECT_SIZE				MAX_NBR_PRESENT_DEMANDS
#define ST_27_PRESENT_VALUE_SELECT_SIZE					MAX_NBR_PRESENT_VALUES
#define ST_27_MAX_SIZE									(ST_27_PRESENT_DEMAND_SELECT_SIZE + ST_27_PRESENT_VALUE_SELECT_SIZE)

/* ST_28 Elements size */
#define ST_28_MAX_SIZE									( (MAX_NBR_PRESENT_DEMANDS * (TM_FMAT_SIZE + NI_FMAT2_SIZE) + (MAX_NBR_PRESENT_VALUES * NI_FMAT1_SIZE) )

/*===========================================================================*\
 * Selectable ID´s starts for ST_22, ST_27 and ST_62
\*===========================================================================*/
#define SUMMATIONS_INITIAL_ID							0
#define LOAD_PROFILE_INITIAL_ID							0
#define AVERAGE_DEMAND_INITIAL_ID						0
#define PRESENT_DEMAND_INITIAL_ID						0
#define INSTANTANEOUS_INITIAL_ID						0

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
 * Interval IDs for sliding demand
\*===========================================================================*/
typedef enum {
	
	INTERVAL_0 = 0,
	INTERVAL_1,
	INTERVAL_2,
	INTERVAL_3,
	INTERVAL_4,
	INTERVAL_5,
	INTERVAL_6,
	INTERVAL_7,
	INTERVAL_8,
	INTERVAL_9,
	INTERVAL_10,
	INTERVAL_11,
	INTERVAL_12,
	INTERVAL_13,
	INTERVAL_14,
	INTERVAL_15,
	MAX_NBR_DEMAND_INTERVALS
		
} DEMAND_INTERVAL_ID_TYPE;

#if ( NBR_PHASES == 1 )

/*===========================================================================*\
 * Summations Sources ID
\*===========================================================================*/
typedef enum {
	
	SM_WH_DEL_ID = SUMMATIONS_INITIAL_ID,	// Delivered Active Energy
	SM_WH_REC_ID,		// Received Active Energy
	SM_WH_NET_ID,		// Net Active Energy
	SM_WH_SUM_ID,		// Sum Active Energy
	SM_VARH_Q1_ID,		// Q1 Reactive Energy
	SM_VARH_Q2_ID,		// Q2 Reactive Energy
	SM_VARH_Q3_ID,		// Q3 Reactive Energy
	SM_VARH_Q4_ID,		// Q4 Reactive Energy
	SM_VARH_DEL_ID,		// Delivered Reactive Energy
	SM_VARH_REC_ID,		// Received Reactive Energy
	SM_VARH_NET_ID,		// Net Reactive Energy
	SM_VARH_SUM_ID,		// Sum Reactive Energy
	SM_VARH_Q1_Q4_ID,	// (Q1+Q4) Reactive Energy
	SM_VARH_Q2_Q3_ID,	// (Q2+Q3) Reactive Energy
	SM_VAH_DEL_ID,		// Delivered Apparent Energy
	SM_VAH_REC_ID,		// Received Apparent Energy
	SM_VAH_NET_ID,		// Net Apparent Energy
	SM_VAH_SUM_ID,		// Sum Apparent Energy
	SM_VAH_Q1_ID,		// Q1 Apparent Energy
	SM_VAH_Q2_ID,		// Q2 Apparent Energy
	SM_VAH_Q3_ID,		// Q3 Apparent Energy
	SM_VAH_Q4_ID,		// Q4 Apparent Energy
	SM_NUMBER_OF_IDS

} SUMMATION_ID_TYPE;

/*===========================================================================*\
 * Load Profile Sources ID
\*===========================================================================*/
typedef enum {

	LP_WH_DEL_ID = LOAD_PROFILE_INITIAL_ID,	// Delivered Active Energy
	LP_WH_REC_ID,		// Received Active Energy
	LP_WH_NET_ID,		// Net Active Energy
	LP_WH_SUM_ID,		// Sum Active Energy
	LP_VARH_Q1_ID,		// Q1 Reactive Energy
	LP_VARH_Q2_ID,		// Q2 Reactive Energy
	LP_VARH_Q3_ID,		// Q3 Reactive Energy
	LP_VARH_Q4_ID,		// Q4 Reactive Energy
	LP_VARH_DEL_ID,		// Delivered Reactive Energy
	LP_VARH_REC_ID,		// Received Reactive Energy
	LP_VARH_NET_ID,		// Net Reactive Energy
	LP_VARH_SUM_ID,		// Sum Reactive Energy
	LP_VARH_Q1_Q4_ID,	// (Q1+Q4) Reactive Energy
	LP_VARH_Q2_Q3_ID,	// (Q2+Q3) Reactive Energy
	LP_VAH_DEL_ID,		// Delivered Apparent Energy
	LP_VAH_REC_ID,		// Received Apparent Energy
	LP_VAH_NET_ID,		// Net Apparent Energy
	LP_VAH_SUM_ID,		// Sum Apparent Energy
	LP_VAH_Q1_ID,		// Q1 Apparent Energy
	LP_VAH_Q2_ID,		// Q2 Apparent Energy
	LP_VAH_Q3_ID,		// Q3 Apparent Energy
	LP_VAH_Q4_ID,		// Q4 Apparent Energy
	LP_NUMBER_OF_IDS

} LP_SOURCE_ID_TYPE;

/*===========================================================================*\
 * Average Demand Sources ID
\*===========================================================================*/
typedef enum {

	AD_W_DEL_ID = AVERAGE_DEMAND_INITIAL_ID,	// Delivered Active Energy
	AD_W_REC_ID,		// Received Active Energy
	AD_W_NET_ID,		// Net Active Energy
	AD_W_SUM_ID,		// Sum Active Energy
	AD_VAR_Q1_ID,		// Q1 Reactive Energy
	AD_VAR_Q2_ID,		// Q2 Reactive Energy
	AD_VAR_Q3_ID,		// Q3 Reactive Energy
	AD_VAR_Q4_ID,		// Q4 Reactive Energy
	AD_VAR_DEL_ID,		// Delivered Reactive Energy
	AD_VAR_REC_ID,		// Received Reactive Energy
	AD_VAR_NET_ID,		// Net Reactive Energy
	AD_VAR_SUM_ID,		// Sum Reactive Energy
	AD_VAR_Q1_Q4_ID,	// (Q1+Q4) Reactive Energy
	AD_VAR_Q2_Q3_ID,	// (Q2+Q3) Reactive Energy
	AD_VA_DEL_ID,		// Delivered Apparent Energy
	AD_VA_REC_ID,		// Received Apparent Energy
	AD_VA_NET_ID,		// Net Apparent Energy
	AD_VA_SUM_ID,		// Sum Apparent Energy
	AD_VA_Q1_ID,		// Q1 Apparent Energy
	AD_VA_Q2_ID,		// Q2 Apparent Energy
	AD_VA_Q3_ID,		// Q3 Apparent Energy
	AD_VA_Q4_ID,		// Q4 Apparent Energy
	AD_NUMBER_OF_IDS

} AVERAGE_DEMAND_ID_TYPE;

/*===========================================================================*\
 * Present Demand Sources ID
\*===========================================================================*/
typedef enum {
	
	PD_W_DEL_ID = PRESENT_DEMAND_INITIAL_ID,	// Delivered Active Energy
	PD_W_REC_ID,		// Received Active Energy
	PD_W_NET_ID,		// Net Active Energy
	PD_W_SUM_ID,		// Sum Active Energy
	PD_VAR_Q1_ID,		// Q1 Reactive Energy
	PD_VAR_Q2_ID,		// Q2 Reactive Energy
	PD_VAR_Q3_ID,		// Q3 Reactive Energy
	PD_VAR_Q4_ID,		// Q4 Reactive Energy
	PD_VAR_DEL_ID,		// Delivered Reactive Energy
	PD_VAR_REC_ID,		// Received Reactive Energy
	PD_VAR_NET_ID,		// Net Reactive Energy
	PD_VAR_SUM_ID,		// Sum Reactive Energy
	PD_VAR_Q1_Q4_ID,	// (Q1+Q4) Reactive Energy
	PD_VAR_Q2_Q3_ID,	// (Q2+Q3) Reactive Energy
	PD_VA_DEL_ID,		// Delivered Apparent Energy
	PD_VA_REC_ID,		// Received Apparent Energy
	PD_VA_NET_ID,		// Net Apparent Energy
	PD_VA_SUM_ID,		// Sum Apparent Energy
	PD_VA_Q1_ID,		// Q1 Apparent Energy
	PD_VA_Q2_ID,		// Q2 Apparent Energy
	PD_VA_Q3_ID,		// Q3 Apparent Energy
	PD_VA_Q4_ID,		// Q4 Apparent Energy
	PD_NUMBER_OF_IDS

} PRESENT_DEMAND_ID_TYPE;

/*===========================================================================*\
 * Instantaneous Value Sources ID
\*===========================================================================*/
typedef enum {
	
	IV_W_TOTAL_ID = INSTANTANEOUS_INITIAL_ID,		// Total Instantaneous Active Power
	IV_VAR_TOTAL_ID,		// Total Instantaneous Reactive Power
	IV_VA_TOTAL_ID,			// Total Instantaneous Apparent Power
	IV_V_A_ID,				// Phase A-N Voltage
	IV_I_A_ID,				// Phase A Current
	IV_PF_ANGLE_ID,			// Total Power Factor Angle
	IV_PF_ID,				// Total Power Factor
	IV_F_ID,				// Frequency
	IV_NUMBER_OF_IDS

} INSTANTANEOUS_VALUE_ID_TYPE;

#elif ( NBR_PHASES == 2 )

/*===========================================================================*\
 * Summations Sources ID
\*===========================================================================*/
typedef enum {
	
	SM_WH_DEL_ID = SUMMATIONS_INITIAL_ID,	// Total Delivered Active Energy
	SM_WH_REC_ID,		// Total Received Active Energy
	SM_WH_NET_ID,		// Total Net Active Energy
	SM_WH_SUM_ID,		// Total Sum Active Energy
	SM_VARH_Q1_ID,		// Total Q1 Reactive Energy
	SM_VARH_Q2_ID,		// Total Q2 Reactive Energy
	SM_VARH_Q3_ID,		// Total Q3 Reactive Energy
	SM_VARH_Q4_ID,		// Total Q4 Reactive Energy
	SM_VARH_DEL_ID,		// Total Delivered Reactive Energy
	SM_VARH_REC_ID,		// Total Received Reactive Energy
	SM_VARH_NET_ID,		// Total Net Reactive Energy
	SM_VARH_SUM_ID,		// Total Sum Reactive Energy
	SM_VARH_Q1_Q4_ID,	// Total (Q1+Q4) Reactive Energy
	SM_VARH_Q2_Q3_ID,	// Total (Q2+Q3) Reactive Energy
	SM_VAH_DEL_ID,		// Total Delivered Apparent Energy
	SM_VAH_REC_ID,		// Total Received Apparent Energy
	SM_VAH_NET_ID,		// Total Net Apparent Energy
	SM_VAH_SUM_ID,		// Total Sum Apparent Energy
	SM_VAH_Q1_ID,		// Total Q1 Apparent Energy
	SM_VAH_Q2_ID,		// Total Q2 Apparent Energy
	SM_VAH_Q3_ID,		// Total Q3 Apparent Energy
	SM_VAH_Q4_ID,		// Total Q4 Apparent Energy

	SM_WH_DEL_A_ID,		// Phase A Delivered Active Energy
	SM_WH_REC_A_ID,		// Phase A Received Active Energy
	SM_WH_NET_A_ID,		// Phase A Net Active Energy
	SM_WH_SUM_A_ID,		// Phase A Sum Active Energy
	SM_VARH_Q1_A_ID,	// Phase A Q1 Reactive Energy
	SM_VARH_Q2_A_ID,	// Phase A Q2 Reactive Energy
	SM_VARH_Q3_A_ID,	// Phase A Q3 Reactive Energy
	SM_VARH_Q4_A_ID,	// Phase A Q4 Reactive Energy
	SM_VARH_DEL_A_ID,	// Phase A Delivered Reactive Energy
	SM_VARH_REC_A_ID,	// Phase A Received Reactive Energy
	SM_VARH_NET_A_ID,	// Phase A Net Reactive Energy
	SM_VARH_SUM_A_ID,	// Phase A Sum Reactive Energy
	SM_VARH_Q1_Q4_A_ID,	// Phase A (Q1+Q4) Reactive Energy
	SM_VARH_Q2_Q3_A_ID,	// Phase A (Q2+Q3) Reactive Energy
	SM_VAH_DEL_A_ID,	// Phase A Delivered Apparent Energy
	SM_VAH_REC_A_ID,	// Phase A Received Apparent Energy
	SM_VAH_NET_A_ID,	// Phase A Net Apparent Energy
	SM_VAH_SUM_A_ID,	// Phase A Sum Apparent Energy
	SM_VAH_Q1_A_ID,		// Phase A Q1 Apparent Energy
	SM_VAH_Q2_A_ID,		// Phase A Q2 Apparent Energy
	SM_VAH_Q3_A_ID,		// Phase A Q3 Apparent Energy
	SM_VAH_Q4_A_ID,		// Phase A Q4 Apparent Energy

	SM_WH_DEL_B_ID,		// Phase B Delivered Active Energy
	SM_WH_REC_B_ID,		// Phase B Received Active Energy
	SM_WH_NET_B_ID,		// Phase B Net Active Energy
	SM_WH_SUM_B_ID,		// Phase B Sum Active Energy
	SM_VARH_Q1_B_ID,	// Phase B Q1 Reactive Energy
	SM_VARH_Q2_B_ID,	// Phase B Q2 Reactive Energy
	SM_VARH_Q3_B_ID,	// Phase B Q3 Reactive Energy
	SM_VARH_Q4_B_ID,	// Phase B Q4 Reactive Energy
	SM_VARH_DEL_B_ID,	// Phase B Delivered Reactive Energy
	SM_VARH_REC_B_ID,	// Phase B Received Reactive Energy
	SM_VARH_NET_B_ID,	// Phase B Net Reactive Energy
	SM_VARH_SUM_B_ID,	// Phase B Sum Reactive Energy
	SM_VARH_Q1_Q4_B_ID,	// Phase B (Q1+Q4) Reactive Energy
	SM_VARH_Q2_Q3_B_ID,	// Phase B (Q2+Q3) Reactive Energy
	SM_VAH_DEL_B_ID,	// Phase B Delivered Apparent Energy
	SM_VAH_REC_B_ID,	// Phase B Received Apparent Energy
	SM_VAH_NET_B_ID,	// Phase B Net Apparent Energy
	SM_VAH_SUM_B_ID,	// Phase B Sum Apparent Energy
	SM_VAH_Q1_B_ID,		// Phase B Q1 Apparent Energy
	SM_VAH_Q2_B_ID,		// Phase B Q2 Apparent Energy
	SM_VAH_Q3_B_ID,		// Phase B Q3 Apparent Energy
	SM_VAH_Q4_B_ID,		// Phase B Q4 Apparent Energy
	SM_NUMBER_OF_IDS

} SUMMATION_ID_TYPE;

/*===========================================================================*\
 * Load Profile Sources ID
\*===========================================================================*/
typedef enum {

	LP_WH_DEL_ID = LOAD_PROFILE_INITIAL_ID,	// Total Delivered Active Energy
	LP_WH_REC_ID,		// Total Received Active Energy
	LP_WH_NET_ID,		// Total Net Active Energy
	LP_WH_SUM_ID,		// Total Sum Active Energy
	LP_VARH_Q1_ID,		// Total Q1 Reactive Energy
	LP_VARH_Q2_ID,		// Total Q2 Reactive Energy
	LP_VARH_Q3_ID,		// Total Q3 Reactive Energy
	LP_VARH_Q4_ID,		// Total Q4 Reactive Energy
	LP_VARH_DEL_ID,		// Total Delivered Reactive Energy
	LP_VARH_REC_ID,		// Total Received Reactive Energy
	LP_VARH_NET_ID,		// Total Net Reactive Energy
	LP_VARH_SUM_ID,		// Total Sum Reactive Energy
	LP_VARH_Q1_Q4_ID,	// Total (Q1+Q4) Reactive Energy
	LP_VARH_Q2_Q3_ID,	// Total (Q2+Q3) Reactive Energy
	LP_VAH_DEL_ID,		// Total Delivered Apparent Energy
	LP_VAH_REC_ID,		// Total Received Apparent Energy
	LP_VAH_NET_ID,		// Total Net Apparent Energy
	LP_VAH_SUM_ID,		// Total Sum Apparent Energy
	LP_VAH_Q1_ID,		// Total Q1 Apparent Energy
	LP_VAH_Q2_ID,		// Total Q2 Apparent Energy
	LP_VAH_Q3_ID,		// Total Q3 Apparent Energy
	LP_VAH_Q4_ID,		// Total Q4 Apparent Energy

	LP_WH_DEL_A_ID,		// Phase A Delivered Active Energy
	LP_WH_REC_A_ID,		// Phase A Received Active Energy
	LP_WH_NET_A_ID,		// Phase A Net Active Energy
	LP_WH_SUM_A_ID,		// Phase A Sum Active Energy
	LP_VARH_Q1_A_ID,	// Phase A Q1 Reactive Energy
	LP_VARH_Q2_A_ID,	// Phase A Q2 Reactive Energy
	LP_VARH_Q3_A_ID,	// Phase A Q3 Reactive Energy
	LP_VARH_Q4_A_ID,	// Phase A Q4 Reactive Energy
	LP_VARH_DEL_A_ID,	// Phase A Delivered Reactive Energy
	LP_VARH_REC_A_ID,	// Phase A Received Reactive Energy
	LP_VARH_NET_A_ID,	// Phase A Net Reactive Energy
	LP_VARH_SUM_A_ID,	// Phase A Sum Reactive Energy
	LP_VARH_Q1_Q4_A_ID,	// Phase A (Q1+Q4) Reactive Energy
	LP_VARH_Q2_Q3_A_ID,	// Phase A (Q2+Q3) Reactive Energy
	LP_VAH_DEL_A_ID,	// Phase A Delivered Apparent Energy
	LP_VAH_REC_A_ID,	// Phase A Received Apparent Energy
	LP_VAH_NET_A_ID,	// Phase A Net Apparent Energy
	LP_VAH_SUM_A_ID,	// Phase A Sum Apparent Energy
	LP_VAH_Q1_A_ID,		// Phase A Q1 Apparent Energy
	LP_VAH_Q2_A_ID,		// Phase A Q2 Apparent Energy
	LP_VAH_Q3_A_ID,		// Phase A Q3 Apparent Energy
	LP_VAH_Q4_A_ID,		// Phase A Q4 Apparent Energy

	LP_WH_DEL_B_ID,		// Phase B Delivered Active Energy
	LP_WH_REC_B_ID,		// Phase B Received Active Energy
	LP_WH_NET_B_ID,		// Phase B Net Active Energy
	LP_WH_SUM_B_ID,		// Phase B Sum Active Energy
	LP_VARH_Q1_B_ID,	// Phase B Q1 Reactive Energy
	LP_VARH_Q2_B_ID,	// Phase B Q2 Reactive Energy
	LP_VARH_Q3_B_ID,	// Phase B Q3 Reactive Energy
	LP_VARH_Q4_B_ID,	// Phase B Q4 Reactive Energy
	LP_VARH_DEL_B_ID,	// Phase B Delivered Reactive Energy
	LP_VARH_REC_B_ID,	// Phase B Received Reactive Energy
	LP_VARH_NET_B_ID,	// Phase B Net Reactive Energy
	LP_VARH_SUM_B_ID,	// Phase B Sum Reactive Energy
	LP_VARH_Q1_Q4_B_ID,	// Phase B (Q1+Q4) Reactive Energy
	LP_VARH_Q2_Q3_B_ID,	// Phase B (Q2+Q3) Reactive Energy
	LP_VAH_DEL_B_ID,	// Phase B Delivered Apparent Energy
	LP_VAH_REC_B_ID,	// Phase B Received Apparent Energy
	LP_VAH_NET_B_ID,	// Phase B Net Apparent Energy
	LP_VAH_SUM_B_ID,	// Phase B Sum Apparent Energy
	LP_VAH_Q1_B_ID,		// Phase B Q1 Apparent Energy
	LP_VAH_Q2_B_ID,		// Phase B Q2 Apparent Energy
	LP_VAH_Q3_B_ID,		// Phase B Q3 Apparent Energy
	LP_VAH_Q4_B_ID,		// Phase B Q4 Apparent Energy
	LP_NUMBER_OF_IDS

} LP_SOURCE_ID_TYPE;

/*===========================================================================*\
 * Average Demand Sources ID
\*===========================================================================*/
typedef enum {

	AD_W_DEL_ID = AVERAGE_DEMAND_INITIAL_ID,	// Total Delivered Active Energy
	AD_W_REC_ID,		// Total Received Active Energy
	AD_W_NET_ID,		// Total Net Active Energy
	AD_W_SUM_ID,		// Total Sum Active Energy
	AD_VAR_Q1_ID,		// Total Q1 Reactive Energy
	AD_VAR_Q2_ID,		// Total Q2 Reactive Energy
	AD_VAR_Q3_ID,		// Total Q3 Reactive Energy
	AD_VAR_Q4_ID,		// Total Q4 Reactive Energy
	AD_VAR_DEL_ID,		// Total Delivered Reactive Energy
	AD_VAR_REC_ID,		// Total Received Reactive Energy
	AD_VAR_NET_ID,		// Total Net Reactive Energy
	AD_VAR_SUM_ID,		// Total Sum Reactive Energy
	AD_VAR_Q1_Q4_ID,	// Total (Q1+Q4) Reactive Energy
	AD_VAR_Q2_Q3_ID,	// Total (Q2+Q3) Reactive Energy
	AD_VA_DEL_ID,		// Total Delivered Apparent Energy
	AD_VA_REC_ID,		// Total Received Apparent Energy
	AD_VA_NET_ID,		// Total Net Apparent Energy
	AD_VA_SUM_ID,		// Total Sum Apparent Energy
	AD_VA_Q1_ID,		// Total Q1 Apparent Energy
	AD_VA_Q2_ID,		// Total Q2 Apparent Energy
	AD_VA_Q3_ID,		// Total Q3 Apparent Energy
	AD_VA_Q4_ID,		// Total Q4 Apparent Energy

	AD_W_DEL_A_ID,		// Phase A Delivered Active Energy
	AD_W_REC_A_ID,		// Phase A Received Active Energy
	AD_W_NET_A_ID,		// Phase A Net Active Energy
	AD_W_SUM_A_ID,		// Phase A Sum Active Energy
	AD_VAR_Q1_A_ID,		// Phase A Q1 Reactive Energy
	AD_VAR_Q2_A_ID,		// Phase A Q2 Reactive Energy
	AD_VAR_Q3_A_ID,		// Phase A Q3 Reactive Energy
	AD_VAR_Q4_A_ID,		// Phase A Q4 Reactive Energy
	AD_VAR_DEL_A_ID,	// Phase A Delivered Reactive Energy
	AD_VAR_REC_A_ID,	// Phase A Received Reactive Energy
	AD_VAR_NET_A_ID,	// Phase A Net Reactive Energy
	AD_VAR_SUM_A_ID,	// Phase A Sum Reactive Energy
	AD_VAR_Q1_Q4_A_ID,	// Phase A (Q1+Q4) Reactive Energy
	AD_VAR_Q2_Q3_A_ID,	// Phase A (Q2+Q3) Reactive Energy
	AD_VA_DEL_A_ID,		// Phase A Delivered Apparent Energy
	AD_VA_REC_A_ID,		// Phase A Received Apparent Energy
	AD_VA_NET_A_ID,		// Phase A Net Apparent Energy
	AD_VA_SUM_A_ID,		// Phase A Sum Apparent Energy
	AD_VA_Q1_A_ID,		// Phase A Q1 Apparent Energy
	AD_VA_Q2_A_ID,		// Phase A Q2 Apparent Energy
	AD_VA_Q3_A_ID,		// Phase A Q3 Apparent Energy
	AD_VA_Q4_A_ID,		// Phase A Q4 Apparent Energy

	AD_W_DEL_B_ID,		// Phase B Delivered Active Energy
	AD_W_REC_B_ID,		// Phase B Received Active Energy
	AD_W_NET_B_ID,		// Phase B Net Active Energy
	AD_W_SUM_B_ID,		// Phase B Sum Active Energy
	AD_VAR_Q1_B_ID,		// Phase B Q1 Reactive Energy
	AD_VAR_Q2_B_ID,		// Phase B Q2 Reactive Energy
	AD_VAR_Q3_B_ID,		// Phase B Q3 Reactive Energy
	AD_VAR_Q4_B_ID,		// Phase B Q4 Reactive Energy
	AD_VAR_DEL_B_ID,	// Phase B Delivered Reactive Energy
	AD_VAR_REC_B_ID,	// Phase B Received Reactive Energy
	AD_VAR_NET_B_ID,	// Phase B Net Reactive Energy
	AD_VAR_SUM_B_ID,	// Phase B Sum Reactive Energy
	AD_VAR_Q1_Q4_B_ID,	// Phase B (Q1+Q4) Reactive Energy
	AD_VAR_Q2_Q3_B_ID,	// Phase B (Q2+Q3) Reactive Energy
	AD_VA_DEL_B_ID,		// Phase B Delivered Apparent Energy
	AD_VA_REC_B_ID,		// Phase B Received Apparent Energy
	AD_VA_NET_B_ID,		// Phase B Net Apparent Energy
	AD_VA_SUM_B_ID,		// Phase B Sum Apparent Energy
	AD_VA_Q1_B_ID,		// Phase B Q1 Apparent Energy
	AD_VA_Q2_B_ID,		// Phase B Q2 Apparent Energy
	AD_VA_Q3_B_ID,		// Phase B Q3 Apparent Energy
	AD_VA_Q4_B_ID,		// Phase B Q4 Apparent Energy
	AD_NUMBER_OF_IDS

} AVERAGE_DEMAND_ID_TYPE;

/*===========================================================================*\
 * Present Demand Sources ID
\*===========================================================================*/
typedef enum {
	
	PD_W_DEL_ID = PRESENT_DEMAND_INITIAL_ID,	// Total Delivered Active Energy
	PD_W_REC_ID,		// Total Received Active Energy
	PD_W_NET_ID,		// Total Net Active Energy
	PD_W_SUM_ID,		// Total Sum Active Energy
	PD_VAR_Q1_ID,		// Total Q1 Reactive Energy
	PD_VAR_Q2_ID,		// Total Q2 Reactive Energy
	PD_VAR_Q3_ID,		// Total Q3 Reactive Energy
	PD_VAR_Q4_ID,		// Total Q4 Reactive Energy
	PD_VAR_DEL_ID,		// Total Delivered Reactive Energy
	PD_VAR_REC_ID,		// Total Received Reactive Energy
	PD_VAR_NET_ID,		// Total Net Reactive Energy
	PD_VAR_SUM_ID,		// Total Sum Reactive Energy
	PD_VAR_Q1_Q4_ID,	// Total (Q1+Q4) Reactive Energy
	PD_VAR_Q2_Q3_ID,	// Total (Q2+Q3) Reactive Energy
	PD_VA_DEL_ID,		// Total Delivered Apparent Energy
	PD_VA_REC_ID,		// Total Received Apparent Energy
	PD_VA_NET_ID,		// Total Net Apparent Energy
	PD_VA_SUM_ID,		// Total Sum Apparent Energy
	PD_VA_Q1_ID,		// Total Q1 Apparent Energy
	PD_VA_Q2_ID,		// Total Q2 Apparent Energy
	PD_VA_Q3_ID,		// Total Q3 Apparent Energy
	PD_VA_Q4_ID,		// Total Q4 Apparent Energy

	PD_W_DEL_A_ID,		// Phase A Delivered Active Energy
	PD_W_REC_A_ID,		// Phase A Received Active Energy
	PD_W_NET_A_ID,		// Phase A Net Active Energy
	PD_W_SUM_A_ID,		// Phase A Sum Active Energy
	PD_VAR_Q1_A_ID,		// Phase A Q1 Reactive Energy
	PD_VAR_Q2_A_ID,		// Phase A Q2 Reactive Energy
	PD_VAR_Q3_A_ID,		// Phase A Q3 Reactive Energy
	PD_VAR_Q4_A_ID,		// Phase A Q4 Reactive Energy
	PD_VAR_DEL_A_ID,	// Phase A Delivered Reactive Energy
	PD_VAR_REC_A_ID,	// Phase A Received Reactive Energy
	PD_VAR_NET_A_ID,	// Phase A Net Reactive Energy
	PD_VAR_SUM_A_ID,	// Phase A Sum Reactive Energy
	PD_VAR_Q1_Q4_A_ID,	// Phase A (Q1+Q4) Reactive Energy
	PD_VAR_Q2_Q3_A_ID,	// Phase A (Q2+Q3) Reactive Energy
	PD_VA_DEL_A_ID,		// Phase A Delivered Apparent Energy
	PD_VA_REC_A_ID,		// Phase A Received Apparent Energy
	PD_VA_NET_A_ID,		// Phase A Net Apparent Energy
	PD_VA_SUM_A_ID,		// Phase A Sum Apparent Energy
	PD_VA_Q1_A_ID,		// Phase A Q1 Apparent Energy
	PD_VA_Q2_A_ID,		// Phase A Q2 Apparent Energy
	PD_VA_Q3_A_ID,		// Phase A Q3 Apparent Energy
	PD_VA_Q4_A_ID,		// Phase A Q4 Apparent Energy

	PD_W_DEL_B_ID,		// Phase B Delivered Active Energy
	PD_W_REC_B_ID,		// Phase B Received Active Energy
	PD_W_NET_B_ID,		// Phase B Net Active Energy
	PD_W_SUM_B_ID,		// Phase B Sum Active Energy
	PD_VAR_Q1_B_ID,		// Phase B Q1 Reactive Energy
	PD_VAR_Q2_B_ID,		// Phase B Q2 Reactive Energy
	PD_VAR_Q3_B_ID,		// Phase B Q3 Reactive Energy
	PD_VAR_Q4_B_ID,		// Phase B Q4 Reactive Energy
	PD_VAR_DEL_B_ID,	// Phase B Delivered Reactive Energy
	PD_VAR_REC_B_ID,	// Phase B Received Reactive Energy
	PD_VAR_NET_B_ID,	// Phase B Net Reactive Energy
	PD_VAR_SUM_B_ID,	// Phase B Sum Reactive Energy
	PD_VAR_Q1_Q4_B_ID,	// Phase B (Q1+Q4) Reactive Energy
	PD_VAR_Q2_Q3_B_ID,	// Phase B (Q2+Q3) Reactive Energy
	PD_VA_DEL_B_ID,		// Phase B Delivered Apparent Energy
	PD_VA_REC_B_ID,		// Phase B Received Apparent Energy
	PD_VA_NET_B_ID,		// Phase B Net Apparent Energy
	PD_VA_SUM_B_ID,		// Phase B Sum Apparent Energy
	PD_VA_Q1_B_ID,		// Phase B Q1 Apparent Energy
	PD_VA_Q2_B_ID,		// Phase B Q2 Apparent Energy
	PD_VA_Q3_B_ID,		// Phase B Q3 Apparent Energy
	PD_VA_Q4_B_ID,		// Phase B Q4 Apparent Energy
	PD_NUMBER_OF_IDS

} PRESENT_DEMAND_ID_TYPE;

/*===========================================================================*\
 * Instantaneous Value Sources ID
\*===========================================================================*/
typedef enum {
	
	IV_W_TOTAL_ID = INSTANTANEOUS_INITIAL_ID,		// Total Instantaneous Active Power
	IV_W_A_ID,				// Phase A Instantaneous Active Power
	IV_W_B_ID,				// Phase B Instantaneous Active Power
	IV_VAR_TOTAL_ID,		// Total Instantaneous Reactive Power
	IV_VAR_A_ID,			// Phase A Instantaneous Reactive Power
	IV_VAR_B_ID,			// Phase B Instantaneous Reactive Power
	IV_VA_TOTAL_ID,			// Total Instantaneous Apparent Power
	IV_VA_A_ID,				// Phase A Instantaneous Apparent Power
	IV_VA_B_ID,				// Phase B Instantaneous Apparent Power
	IV_V_A_ID,				// Phase A-N Voltage
	IV_V_B_ID,				// Phase B-N Voltage
	IV_I_A_ID,				// Phase A Current
	IV_I_B_ID,				// Phase B Current
	IV_V_A_ANGLE_ID,		// Phase A Voltage Phase Angle
	IV_V_B_ANGLE_ID,		// Phase B Voltage Phase Angle
	IV_I_A_ANGLE_ID,		// Phase A Current Phase Angle
	IV_I_B_ANGLE_ID,		// Phase B Current Phase Angle
	IV_PF_ANGLE_ID,			// Total Power Factor Angle
	IV_PF_A_ANGLE_ID,		// Phase A Power Factor Angle
	IV_PF_B_ANGLE_ID,		// Phase B Power Factor Angle
	IV_PF_ID,				// Total Power Factor
	IV_PF_A_ID,				// Phase A Power Factor
	IV_PF_B_ID,				// Phase B Power Factor
	IV_F_ID,				// Frequency
	IV_NUMBER_OF_IDS

} INSTANTANEOUS_VALUE_ID_TYPE;

#elif ( NBR_PHASES == 3 )

/*===========================================================================*\
 * Summations Sources ID
\*===========================================================================*/
typedef enum {
	
	SM_WH_DEL_ID = SUMMATIONS_INITIAL_ID,	// Total Delivered Active Energy
	SM_WH_REC_ID,		// Total Received Active Energy
	SM_WH_NET_ID,		// Total Net Active Energy
	SM_WH_SUM_ID,		// Total Sum Active Energy
	SM_VARH_Q1_ID,		// Total Q1 Reactive Energy
	SM_VARH_Q2_ID,		// Total Q2 Reactive Energy
	SM_VARH_Q3_ID,		// Total Q3 Reactive Energy
	SM_VARH_Q4_ID,		// Total Q4 Reactive Energy
	SM_VARH_DEL_ID,		// Total Delivered Reactive Energy
	SM_VARH_REC_ID,		// Total Received Reactive Energy
	SM_VARH_NET_ID,		// Total Net Reactive Energy
	SM_VARH_SUM_ID,		// Total Sum Reactive Energy
	SM_VARH_Q1_Q4_ID,	// Total (Q1+Q4) Reactive Energy
	SM_VARH_Q2_Q3_ID,	// Total (Q2+Q3) Reactive Energy
	SM_VAH_DEL_ID,		// Total Delivered Apparent Energy
	SM_VAH_REC_ID,		// Total Received Apparent Energy
	SM_VAH_NET_ID,		// Total Net Apparent Energy
	SM_VAH_SUM_ID,		// Total Sum Apparent Energy
	SM_VAH_Q1_ID,		// Total Q1 Apparent Energy
	SM_VAH_Q2_ID,		// Total Q2 Apparent Energy
	SM_VAH_Q3_ID,		// Total Q3 Apparent Energy
	SM_VAH_Q4_ID,		// Total Q4 Apparent Energy

	SM_WH_DEL_A_ID,		// Phase A Delivered Active Energy
	SM_WH_REC_A_ID,		// Phase A Received Active Energy
	SM_WH_NET_A_ID,		// Phase A Net Active Energy
	SM_WH_SUM_A_ID,		// Phase A Sum Active Energy
	SM_VARH_Q1_A_ID,	// Phase A Q1 Reactive Energy
	SM_VARH_Q2_A_ID,	// Phase A Q2 Reactive Energy
	SM_VARH_Q3_A_ID,	// Phase A Q3 Reactive Energy
	SM_VARH_Q4_A_ID,	// Phase A Q4 Reactive Energy
	SM_VARH_DEL_A_ID,	// Phase A Delivered Reactive Energy
	SM_VARH_REC_A_ID,	// Phase A Received Reactive Energy
	SM_VARH_NET_A_ID,	// Phase A Net Reactive Energy
	SM_VARH_SUM_A_ID,	// Phase A Sum Reactive Energy
	SM_VARH_Q1_Q4_A_ID,	// Phase A (Q1+Q4) Reactive Energy
	SM_VARH_Q2_Q3_A_ID,	// Phase A (Q2+Q3) Reactive Energy
	SM_VAH_DEL_A_ID,	// Phase A Delivered Apparent Energy
	SM_VAH_REC_A_ID,	// Phase A Received Apparent Energy
	SM_VAH_NET_A_ID,	// Phase A Net Apparent Energy
	SM_VAH_SUM_A_ID,	// Phase A Sum Apparent Energy
	SM_VAH_Q1_A_ID,		// Phase A Q1 Apparent Energy
	SM_VAH_Q2_A_ID,		// Phase A Q2 Apparent Energy
	SM_VAH_Q3_A_ID,		// Phase A Q3 Apparent Energy
	SM_VAH_Q4_A_ID,		// Phase A Q4 Apparent Energy

	SM_WH_DEL_B_ID,		// Phase B Delivered Active Energy
	SM_WH_REC_B_ID,		// Phase B Received Active Energy
	SM_WH_NET_B_ID,		// Phase B Net Active Energy
	SM_WH_SUM_B_ID,		// Phase B Sum Active Energy
	SM_VARH_Q1_B_ID,	// Phase B Q1 Reactive Energy
	SM_VARH_Q2_B_ID,	// Phase B Q2 Reactive Energy
	SM_VARH_Q3_B_ID,	// Phase B Q3 Reactive Energy
	SM_VARH_Q4_B_ID,	// Phase B Q4 Reactive Energy
	SM_VARH_DEL_B_ID,	// Phase B Delivered Reactive Energy
	SM_VARH_REC_B_ID,	// Phase B Received Reactive Energy
	SM_VARH_NET_B_ID,	// Phase B Net Reactive Energy
	SM_VARH_SUM_B_ID,	// Phase B Sum Reactive Energy
	SM_VARH_Q1_Q4_B_ID,	// Phase B (Q1+Q4) Reactive Energy
	SM_VARH_Q2_Q3_B_ID,	// Phase B (Q2+Q3) Reactive Energy
	SM_VAH_DEL_B_ID,	// Phase B Delivered Apparent Energy
	SM_VAH_REC_B_ID,	// Phase B Received Apparent Energy
	SM_VAH_NET_B_ID,	// Phase B Net Apparent Energy
	SM_VAH_SUM_B_ID,	// Phase B Sum Apparent Energy
	SM_VAH_Q1_B_ID,		// Phase B Q1 Apparent Energy
	SM_VAH_Q2_B_ID,		// Phase B Q2 Apparent Energy
	SM_VAH_Q3_B_ID,		// Phase B Q3 Apparent Energy
	SM_VAH_Q4_B_ID,		// Phase B Q4 Apparent Energy

	SM_WH_DEL_C_ID,		// Phase C Delivered Active Energy
	SM_WH_REC_C_ID,		// Phase C Received Active Energy
	SM_WH_NET_C_ID,		// Phase C Net Active Energy
	SM_WH_SUM_C_ID,		// Phase C Sum Active Energy
	SM_VARH_Q1_C_ID,	// Phase C Q1 Reactive Energy
	SM_VARH_Q2_C_ID,	// Phase C Q2 Reactive Energy
	SM_VARH_Q3_C_ID,	// Phase C Q3 Reactive Energy
	SM_VARH_Q4_C_ID,	// Phase C Q4 Reactive Energy
	SM_VARH_DEL_C_ID,	// Phase C Delivered Reactive Energy
	SM_VARH_REC_C_ID,	// Phase C Received Reactive Energy
	SM_VARH_NET_C_ID,	// Phase C Net Reactive Energy
	SM_VARH_SUM_C_ID,	// Phase C Sum Reactive Energy
	SM_VARH_Q1_Q4_C_ID,	// Phase C (Q1+Q4) Reactive Energy
	SM_VARH_Q2_Q3_C_ID,	// Phase C (Q2+Q3) Reactive Energy
	SM_VAH_DEL_C_ID,	// Phase C Delivered Apparent Energy
	SM_VAH_REC_C_ID,	// Phase C Received Apparent Energy
	SM_VAH_NET_C_ID,	// Phase C Net Apparent Energy
	SM_VAH_SUM_C_ID,	// Phase C Sum Apparent Energy
	SM_VAH_Q1_C_ID,		// Phase C Q1 Apparent Energy
	SM_VAH_Q2_C_ID,		// Phase C Q2 Apparent Energy
	SM_VAH_Q3_C_ID,		// Phase C Q3 Apparent Energy
	SM_VAH_Q4_C_ID,		// Phase C Q4 Apparent Energy
	SM_NUMBER_OF_IDS

} SUMMATION_ID_TYPE;

/*===========================================================================*\
 * Load Profile Sources ID
\*===========================================================================*/
typedef enum {

	LP_WH_DEL_ID = LOAD_PROFILE_INITIAL_ID,	// Total Delivered Active Energy
	LP_WH_REC_ID,		// Total Received Active Energy
	LP_WH_NET_ID,		// Total Net Active Energy
	LP_WH_SUM_ID,		// Total Sum Active Energy
	LP_VARH_Q1_ID,		// Total Q1 Reactive Energy
	LP_VARH_Q2_ID,		// Total Q2 Reactive Energy
	LP_VARH_Q3_ID,		// Total Q3 Reactive Energy
	LP_VARH_Q4_ID,		// Total Q4 Reactive Energy
	LP_VARH_DEL_ID,		// Total Delivered Reactive Energy
	LP_VARH_REC_ID,		// Total Received Reactive Energy
	LP_VARH_NET_ID,		// Total Net Reactive Energy
	LP_VARH_SUM_ID,		// Total Sum Reactive Energy
	LP_VARH_Q1_Q4_ID,	// Total (Q1+Q4) Reactive Energy
	LP_VARH_Q2_Q3_ID,	// Total (Q2+Q3) Reactive Energy
	LP_VAH_DEL_ID,		// Total Delivered Apparent Energy
	LP_VAH_REC_ID,		// Total Received Apparent Energy
	LP_VAH_NET_ID,		// Total Net Apparent Energy
	LP_VAH_SUM_ID,		// Total Sum Apparent Energy
	LP_VAH_Q1_ID,		// Total Q1 Apparent Energy
	LP_VAH_Q2_ID,		// Total Q2 Apparent Energy
	LP_VAH_Q3_ID,		// Total Q3 Apparent Energy
	LP_VAH_Q4_ID,		// Total Q4 Apparent Energy

	LP_WH_DEL_A_ID,		// Phase A Delivered Active Energy
	LP_WH_REC_A_ID,		// Phase A Received Active Energy
	LP_WH_NET_A_ID,		// Phase A Net Active Energy
	LP_WH_SUM_A_ID,		// Phase A Sum Active Energy
	LP_VARH_Q1_A_ID,	// Phase A Q1 Reactive Energy
	LP_VARH_Q2_A_ID,	// Phase A Q2 Reactive Energy
	LP_VARH_Q3_A_ID,	// Phase A Q3 Reactive Energy
	LP_VARH_Q4_A_ID,	// Phase A Q4 Reactive Energy
	LP_VARH_DEL_A_ID,	// Phase A Delivered Reactive Energy
	LP_VARH_REC_A_ID,	// Phase A Received Reactive Energy
	LP_VARH_NET_A_ID,	// Phase A Net Reactive Energy
	LP_VARH_SUM_A_ID,	// Phase A Sum Reactive Energy
	LP_VARH_Q1_Q4_A_ID,	// Phase A (Q1+Q4) Reactive Energy
	LP_VARH_Q2_Q3_A_ID,	// Phase A (Q2+Q3) Reactive Energy
	LP_VAH_DEL_A_ID,	// Phase A Delivered Apparent Energy
	LP_VAH_REC_A_ID,	// Phase A Received Apparent Energy
	LP_VAH_NET_A_ID,	// Phase A Net Apparent Energy
	LP_VAH_SUM_A_ID,	// Phase A Sum Apparent Energy
	LP_VAH_Q1_A_ID,		// Phase A Q1 Apparent Energy
	LP_VAH_Q2_A_ID,		// Phase A Q2 Apparent Energy
	LP_VAH_Q3_A_ID,		// Phase A Q3 Apparent Energy
	LP_VAH_Q4_A_ID,		// Phase A Q4 Apparent Energy

	LP_WH_DEL_B_ID,		// Phase B Delivered Active Energy
	LP_WH_REC_B_ID,		// Phase B Received Active Energy
	LP_WH_NET_B_ID,		// Phase B Net Active Energy
	LP_WH_SUM_B_ID,		// Phase B Sum Active Energy
	LP_VARH_Q1_B_ID,	// Phase B Q1 Reactive Energy
	LP_VARH_Q2_B_ID,	// Phase B Q2 Reactive Energy
	LP_VARH_Q3_B_ID,	// Phase B Q3 Reactive Energy
	LP_VARH_Q4_B_ID,	// Phase B Q4 Reactive Energy
	LP_VARH_DEL_B_ID,	// Phase B Delivered Reactive Energy
	LP_VARH_REC_B_ID,	// Phase B Received Reactive Energy
	LP_VARH_NET_B_ID,	// Phase B Net Reactive Energy
	LP_VARH_SUM_B_ID,	// Phase B Sum Reactive Energy
	LP_VARH_Q1_Q4_B_ID,	// Phase B (Q1+Q4) Reactive Energy
	LP_VARH_Q2_Q3_B_ID,	// Phase B (Q2+Q3) Reactive Energy
	LP_VAH_DEL_B_ID,	// Phase B Delivered Apparent Energy
	LP_VAH_REC_B_ID,	// Phase B Received Apparent Energy
	LP_VAH_NET_B_ID,	// Phase B Net Apparent Energy
	LP_VAH_SUM_B_ID,	// Phase B Sum Apparent Energy
	LP_VAH_Q1_B_ID,		// Phase B Q1 Apparent Energy
	LP_VAH_Q2_B_ID,		// Phase B Q2 Apparent Energy
	LP_VAH_Q3_B_ID,		// Phase B Q3 Apparent Energy
	LP_VAH_Q4_B_ID,		// Phase B Q4 Apparent Energy

	LP_WH_DEL_C_ID,		// Phase C Delivered Active Energy
	LP_WH_REC_C_ID,		// Phase C Received Active Energy
	LP_WH_NET_C_ID,		// Phase C Net Active Energy
	LP_WH_SUM_C_ID,		// Phase C Sum Active Energy
	LP_VARH_Q1_C_ID,	// Phase C Q1 Reactive Energy
	LP_VARH_Q2_C_ID,	// Phase C Q2 Reactive Energy
	LP_VARH_Q3_C_ID,	// Phase C Q3 Reactive Energy
	LP_VARH_Q4_C_ID,	// Phase C Q4 Reactive Energy
	LP_VARH_DEL_C_ID,	// Phase C Delivered Reactive Energy
	LP_VARH_REC_C_ID,	// Phase C Received Reactive Energy
	LP_VARH_NET_C_ID,	// Phase C Net Reactive Energy
	LP_VARH_SUM_C_ID,	// Phase C Sum Reactive Energy
	LP_VARH_Q1_Q4_C_ID,	// Phase C (Q1+Q4) Reactive Energy
	LP_VARH_Q2_Q3_C_ID,	// Phase C (Q2+Q3) Reactive Energy
	LP_VAH_DEL_C_ID,	// Phase C Delivered Apparent Energy
	LP_VAH_REC_C_ID,	// Phase C Received Apparent Energy
	LP_VAH_NET_C_ID,	// Phase C Net Apparent Energy
	LP_VAH_SUM_C_ID,	// Phase C Sum Apparent Energy
	LP_VAH_Q1_C_ID,		// Phase C Q1 Apparent Energy
	LP_VAH_Q2_C_ID,		// Phase C Q2 Apparent Energy
	LP_VAH_Q3_C_ID,		// Phase C Q3 Apparent Energy
	LP_VAH_Q4_C_ID,		// Phase C Q4 Apparent Energy
	LP_NUMBER_OF_IDS

} LP_SOURCE_ID_TYPE;

/*===========================================================================*\
 * Average Demand Sources ID
\*===========================================================================*/
typedef enum {

	AD_W_DEL_ID = AVERAGE_DEMAND_INITIAL_ID,	// Total Delivered Active Energy
	AD_W_REC_ID,		// Total Received Active Energy
	AD_W_NET_ID,		// Total Net Active Energy
	AD_W_SUM_ID,		// Total Sum Active Energy
	AD_VAR_Q1_ID,		// Total Q1 Reactive Energy
	AD_VAR_Q2_ID,		// Total Q2 Reactive Energy
	AD_VAR_Q3_ID,		// Total Q3 Reactive Energy
	AD_VAR_Q4_ID,		// Total Q4 Reactive Energy
	AD_VAR_DEL_ID,		// Total Delivered Reactive Energy
	AD_VAR_REC_ID,		// Total Received Reactive Energy
	AD_VAR_NET_ID,		// Total Net Reactive Energy
	AD_VAR_SUM_ID,		// Total Sum Reactive Energy
	AD_VAR_Q1_Q4_ID,	// Total (Q1+Q4) Reactive Energy
	AD_VAR_Q2_Q3_ID,	// Total (Q2+Q3) Reactive Energy
	AD_VA_DEL_ID,		// Total Delivered Apparent Energy
	AD_VA_REC_ID,		// Total Received Apparent Energy
	AD_VA_NET_ID,		// Total Net Apparent Energy
	AD_VA_SUM_ID,		// Total Sum Apparent Energy
	AD_VA_Q1_ID,		// Total Q1 Apparent Energy
	AD_VA_Q2_ID,		// Total Q2 Apparent Energy
	AD_VA_Q3_ID,		// Total Q3 Apparent Energy
	AD_VA_Q4_ID,		// Total Q4 Apparent Energy

	AD_W_DEL_A_ID,		// Phase A Delivered Active Energy
	AD_W_REC_A_ID,		// Phase A Received Active Energy
	AD_W_NET_A_ID,		// Phase A Net Active Energy
	AD_W_SUM_A_ID,		// Phase A Sum Active Energy
	AD_VAR_Q1_A_ID,		// Phase A Q1 Reactive Energy
	AD_VAR_Q2_A_ID,		// Phase A Q2 Reactive Energy
	AD_VAR_Q3_A_ID,		// Phase A Q3 Reactive Energy
	AD_VAR_Q4_A_ID,		// Phase A Q4 Reactive Energy
	AD_VAR_DEL_A_ID,	// Phase A Delivered Reactive Energy
	AD_VAR_REC_A_ID,	// Phase A Received Reactive Energy
	AD_VAR_NET_A_ID,	// Phase A Net Reactive Energy
	AD_VAR_SUM_A_ID,	// Phase A Sum Reactive Energy
	AD_VAR_Q1_Q4_A_ID,	// Phase A (Q1+Q4) Reactive Energy
	AD_VAR_Q2_Q3_A_ID,	// Phase A (Q2+Q3) Reactive Energy
	AD_VA_DEL_A_ID,		// Phase A Delivered Apparent Energy
	AD_VA_REC_A_ID,		// Phase A Received Apparent Energy
	AD_VA_NET_A_ID,		// Phase A Net Apparent Energy
	AD_VA_SUM_A_ID,		// Phase A Sum Apparent Energy
	AD_VA_Q1_A_ID,		// Phase A Q1 Apparent Energy
	AD_VA_Q2_A_ID,		// Phase A Q2 Apparent Energy
	AD_VA_Q3_A_ID,		// Phase A Q3 Apparent Energy
	AD_VA_Q4_A_ID,		// Phase A Q4 Apparent Energy

	AD_W_DEL_B_ID,		// Phase B Delivered Active Energy
	AD_W_REC_B_ID,		// Phase B Received Active Energy
	AD_W_NET_B_ID,		// Phase B Net Active Energy
	AD_W_SUM_B_ID,		// Phase B Sum Active Energy
	AD_VAR_Q1_B_ID,		// Phase B Q1 Reactive Energy
	AD_VAR_Q2_B_ID,		// Phase B Q2 Reactive Energy
	AD_VAR_Q3_B_ID,		// Phase B Q3 Reactive Energy
	AD_VAR_Q4_B_ID,		// Phase B Q4 Reactive Energy
	AD_VAR_DEL_B_ID,	// Phase B Delivered Reactive Energy
	AD_VAR_REC_B_ID,	// Phase B Received Reactive Energy
	AD_VAR_NET_B_ID,	// Phase B Net Reactive Energy
	AD_VAR_SUM_B_ID,	// Phase B Sum Reactive Energy
	AD_VAR_Q1_Q4_B_ID,	// Phase B (Q1+Q4) Reactive Energy
	AD_VAR_Q2_Q3_B_ID,	// Phase B (Q2+Q3) Reactive Energy
	AD_VA_DEL_B_ID,		// Phase B Delivered Apparent Energy
	AD_VA_REC_B_ID,		// Phase B Received Apparent Energy
	AD_VA_NET_B_ID,		// Phase B Net Apparent Energy
	AD_VA_SUM_B_ID,		// Phase B Sum Apparent Energy
	AD_VA_Q1_B_ID,		// Phase B Q1 Apparent Energy
	AD_VA_Q2_B_ID,		// Phase B Q2 Apparent Energy
	AD_VA_Q3_B_ID,		// Phase B Q3 Apparent Energy
	AD_VA_Q4_B_ID,		// Phase B Q4 Apparent Energy

	AD_W_DEL_C_ID,		// Phase C Delivered Active Energy
	AD_W_REC_C_ID,		// Phase C Received Active Energy
	AD_W_NET_C_ID,		// Phase C Net Active Energy
	AD_W_SUM_C_ID,		// Phase C Sum Active Energy
	AD_VAR_Q1_C_ID,		// Phase C Q1 Reactive Energy
	AD_VAR_Q2_C_ID,		// Phase C Q2 Reactive Energy
	AD_VAR_Q3_C_ID,		// Phase C Q3 Reactive Energy
	AD_VAR_Q4_C_ID,		// Phase C Q4 Reactive Energy
	AD_VAR_DEL_C_ID,	// Phase C Delivered Reactive Energy
	AD_VAR_REC_C_ID,	// Phase C Received Reactive Energy
	AD_VAR_NET_C_ID,	// Phase C Net Reactive Energy
	AD_VAR_SUM_C_ID,	// Phase C Sum Reactive Energy
	AD_VAR_Q1_Q4_C_ID,	// Phase C (Q1+Q4) Reactive Energy
	AD_VAR_Q2_Q3_C_ID,	// Phase C (Q2+Q3) Reactive Energy
	AD_VA_DEL_C_ID,		// Phase C Delivered Apparent Energy
	AD_VA_REC_C_ID,		// Phase C Received Apparent Energy
	AD_VA_NET_C_ID,		// Phase C Net Apparent Energy
	AD_VA_SUM_C_ID,		// Phase C Sum Apparent Energy
	AD_VA_Q1_C_ID,		// Phase C Q1 Apparent Energy
	AD_VA_Q2_C_ID,		// Phase C Q2 Apparent Energy
	AD_VA_Q3_C_ID,		// Phase C Q3 Apparent Energy
	AD_VA_Q4_C_ID,		// Phase C Q4 Apparent Energy
	AD_NUMBER_OF_IDS

} AVERAGE_DEMAND_ID_TYPE;

/*===========================================================================*\
 * Present Demand Sources ID
\*===========================================================================*/
typedef enum {
	
	PD_W_DEL_ID = PRESENT_DEMAND_INITIAL_ID,	// Total Delivered Active Energy
	PD_W_REC_ID,		// Total Received Active Energy
	PD_W_NET_ID,		// Total Net Active Energy
	PD_W_SUM_ID,		// Total Sum Active Energy
	PD_VAR_Q1_ID,		// Total Q1 Reactive Energy
	PD_VAR_Q2_ID,		// Total Q2 Reactive Energy
	PD_VAR_Q3_ID,		// Total Q3 Reactive Energy
	PD_VAR_Q4_ID,		// Total Q4 Reactive Energy
	PD_VAR_DEL_ID,		// Total Delivered Reactive Energy
	PD_VAR_REC_ID,		// Total Received Reactive Energy
	PD_VAR_NET_ID,		// Total Net Reactive Energy
	PD_VAR_SUM_ID,		// Total Sum Reactive Energy
	PD_VAR_Q1_Q4_ID,	// Total (Q1+Q4) Reactive Energy
	PD_VAR_Q2_Q3_ID,	// Total (Q2+Q3) Reactive Energy
	PD_VA_DEL_ID,		// Total Delivered Apparent Energy
	PD_VA_REC_ID,		// Total Received Apparent Energy
	PD_VA_NET_ID,		// Total Net Apparent Energy
	PD_VA_SUM_ID,		// Total Sum Apparent Energy
	PD_VA_Q1_ID,		// Total Q1 Apparent Energy
	PD_VA_Q2_ID,		// Total Q2 Apparent Energy
	PD_VA_Q3_ID,		// Total Q3 Apparent Energy
	PD_VA_Q4_ID,		// Total Q4 Apparent Energy

	PD_W_DEL_A_ID,		// Phase A Delivered Active Energy
	PD_W_REC_A_ID,		// Phase A Received Active Energy
	PD_W_NET_A_ID,		// Phase A Net Active Energy
	PD_W_SUM_A_ID,		// Phase A Sum Active Energy
	PD_VAR_Q1_A_ID,		// Phase A Q1 Reactive Energy
	PD_VAR_Q2_A_ID,		// Phase A Q2 Reactive Energy
	PD_VAR_Q3_A_ID,		// Phase A Q3 Reactive Energy
	PD_VAR_Q4_A_ID,		// Phase A Q4 Reactive Energy
	PD_VAR_DEL_A_ID,	// Phase A Delivered Reactive Energy
	PD_VAR_REC_A_ID,	// Phase A Received Reactive Energy
	PD_VAR_NET_A_ID,	// Phase A Net Reactive Energy
	PD_VAR_SUM_A_ID,	// Phase A Sum Reactive Energy
	PD_VAR_Q1_Q4_A_ID,	// Phase A (Q1+Q4) Reactive Energy
	PD_VAR_Q2_Q3_A_ID,	// Phase A (Q2+Q3) Reactive Energy
	PD_VA_DEL_A_ID,		// Phase A Delivered Apparent Energy
	PD_VA_REC_A_ID,		// Phase A Received Apparent Energy
	PD_VA_NET_A_ID,		// Phase A Net Apparent Energy
	PD_VA_SUM_A_ID,		// Phase A Sum Apparent Energy
	PD_VA_Q1_A_ID,		// Phase A Q1 Apparent Energy
	PD_VA_Q2_A_ID,		// Phase A Q2 Apparent Energy
	PD_VA_Q3_A_ID,		// Phase A Q3 Apparent Energy
	PD_VA_Q4_A_ID,		// Phase A Q4 Apparent Energy

	PD_W_DEL_B_ID,		// Phase B Delivered Active Energy
	PD_W_REC_B_ID,		// Phase B Received Active Energy
	PD_W_NET_B_ID,		// Phase B Net Active Energy
	PD_W_SUM_B_ID,		// Phase B Sum Active Energy
	PD_VAR_Q1_B_ID,		// Phase B Q1 Reactive Energy
	PD_VAR_Q2_B_ID,		// Phase B Q2 Reactive Energy
	PD_VAR_Q3_B_ID,		// Phase B Q3 Reactive Energy
	PD_VAR_Q4_B_ID,		// Phase B Q4 Reactive Energy
	PD_VAR_DEL_B_ID,	// Phase B Delivered Reactive Energy
	PD_VAR_REC_B_ID,	// Phase B Received Reactive Energy
	PD_VAR_NET_B_ID,	// Phase B Net Reactive Energy
	PD_VAR_SUM_B_ID,	// Phase B Sum Reactive Energy
	PD_VAR_Q1_Q4_B_ID,	// Phase B (Q1+Q4) Reactive Energy
	PD_VAR_Q2_Q3_B_ID,	// Phase B (Q2+Q3) Reactive Energy
	PD_VA_DEL_B_ID,		// Phase B Delivered Apparent Energy
	PD_VA_REC_B_ID,		// Phase B Received Apparent Energy
	PD_VA_NET_B_ID,		// Phase B Net Apparent Energy
	PD_VA_SUM_B_ID,		// Phase B Sum Apparent Energy
	PD_VA_Q1_B_ID,		// Phase B Q1 Apparent Energy
	PD_VA_Q2_B_ID,		// Phase B Q2 Apparent Energy
	PD_VA_Q3_B_ID,		// Phase B Q3 Apparent Energy
	PD_VA_Q4_B_ID,		// Phase B Q4 Apparent Energy

	PD_W_DEL_C_ID,		// Phase C Delivered Active Energy
	PD_W_REC_C_ID,		// Phase C Received Active Energy
	PD_W_NET_C_ID,		// Phase C Net Active Energy
	PD_W_SUM_C_ID,		// Phase C Sum Active Energy
	PD_VAR_Q1_C_ID,		// Phase C Q1 Reactive Energy
	PD_VAR_Q2_C_ID,		// Phase C Q2 Reactive Energy
	PD_VAR_Q3_C_ID,		// Phase C Q3 Reactive Energy
	PD_VAR_Q4_C_ID,		// Phase C Q4 Reactive Energy
	PD_VAR_DEL_C_ID,	// Phase C Delivered Reactive Energy
	PD_VAR_REC_C_ID,	// Phase C Received Reactive Energy
	PD_VAR_NET_C_ID,	// Phase C Net Reactive Energy
	PD_VAR_SUM_C_ID,	// Phase C Sum Reactive Energy
	PD_VAR_Q1_Q4_C_ID,	// Phase C (Q1+Q4) Reactive Energy
	PD_VAR_Q2_Q3_C_ID,	// Phase C (Q2+Q3) Reactive Energy
	PD_VA_DEL_C_ID,		// Phase C Delivered Apparent Energy
	PD_VA_REC_C_ID,		// Phase C Received Apparent Energy
	PD_VA_NET_C_ID,		// Phase C Net Apparent Energy
	PD_VA_SUM_C_ID,		// Phase C Sum Apparent Energy
	PD_VA_Q1_C_ID,		// Phase C Q1 Apparent Energy
	PD_VA_Q2_C_ID,		// Phase C Q2 Apparent Energy
	PD_VA_Q3_C_ID,		// Phase C Q3 Apparent Energy
	PD_VA_Q4_C_ID,		// Phase C Q4 Apparent Energy
	PD_NUMBER_OF_IDS

} PRESENT_DEMAND_ID_TYPE;

/*===========================================================================*\
 * Instantaneous Value Sources ID
\*===========================================================================*/
typedef enum {
	
	IV_W_TOTAL_ID = INSTANTANEOUS_INITIAL_ID,		// Total Instantaneous Active Power
	IV_W_A_ID,				// Phase A Instantaneous Active Power
	IV_W_B_ID,				// Phase B Instantaneous Active Power
	IV_W_C_ID,				// Phase C Instantaneous Active Power
	IV_VAR_TOTAL_ID,		// Total Instantaneous Reactive Power
	IV_VAR_A_ID,			// Phase A Instantaneous Reactive Power
	IV_VAR_B_ID,			// Phase B Instantaneous Reactive Power
	IV_VAR_C_ID,			// Phase C Instantaneous Reactive Power
	IV_VA_TOTAL_ID,			// Total Instantaneous Apparent Power
	IV_VA_A_ID,				// Phase A Instantaneous Apparent Power
	IV_VA_B_ID,				// Phase B Instantaneous Apparent Power
	IV_VA_C_ID,				// Phase C Instantaneous Apparent Power	
	IV_V_A_ID,				// Phase A-N Voltage
	IV_V_B_ID,				// Phase B-N Voltage
	IV_V_C_ID,				// Phase C-N Voltage
	IV_I_A_ID,				// Phase A Current
	IV_I_B_ID,				// Phase B Current
	IV_I_C_ID,				// Phase C Current
	IV_V_A_ANGLE_ID,		// Phase A Voltage Phase Angle
	IV_V_B_ANGLE_ID,		// Phase B Voltage Phase Angle
	IV_V_C_ANGLE_ID,		// Phase C Voltage Phase Angle
	IV_I_A_ANGLE_ID,		// Phase A Current Phase Angle
	IV_I_B_ANGLE_ID,		// Phase B Current Phase Angle
	IV_I_C_ANGLE_ID,		// Phase C Current Phase Angle
	IV_PF_ANGLE_ID,			// Total Power Factor Angle
	IV_PF_A_ANGLE_ID,		// Phase A Power Factor Angle
	IV_PF_B_ANGLE_ID,		// Phase B Power Factor Angle
	IV_PF_C_ANGLE_ID,		// Phase C Power Factor Angle
	IV_PF_ID,				// Total Power Factor
	IV_PF_A_ID,				// Phase A Power Factor
	IV_PF_B_ID,				// Phase B Power Factor
	IV_PF_C_ID,				// Phase C Power Factor
	IV_F_ID,					// Frequency
	IV_NUMBER_OF_IDS

} INSTANTANEOUS_VALUE_ID_TYPE;

#endif


/*===========================================================================*\
 * Data Blocks Types
\*===========================================================================*/
typedef enum {
		
	TOTAL_DATA = 0,		// Total Data Block
	TIER_A_DATA,		// Tier A Data Block
	TIER_B_DATA,		// Tier B Data Block
	TIER_C_DATA,		// Tier C Data Block
	TIER_D_DATA,		// Tier D Data Block
			
} DATA_BLOCK_TYPE;

/*===========================================================================*\
 * ST-23 Snapshot Causes 
\*===========================================================================*/
typedef enum {
	
	SEASON_CHANGE = 0,
	DEMAND_RESET,
	SELF_READ

} SNAPSHOT_CAUSE_TYPE;

/*===========================================================================*\
 * Present Demand Interval
\*===========================================================================*/
typedef struct {
	
	uint16_t current_seconds;
	bool run_next_interval;
	bool is_int_expiried;
	double present_demand[MAX_NBR_DEMANDS];

} PRESENT_DEMAND_INTERVAL_TYPE;

/*===========================================================================*\
 * Demand Settings
\*===========================================================================*/
typedef struct {
	
	bool is_sliding_demand;
	uint16_t interval_length;
	uint16_t subinterval_length;
	uint8_t nbr_intervals;

} DEMAND_CONFIGURATIONS_TYPE;	

/*===========================================================================*\
 * Demand
\*===========================================================================*/
typedef struct {
	
	DEMAND_CONFIGURATIONS_TYPE config;
	PRESENT_DEMAND_INTERVAL_TYPE interval[MAX_NBR_DEMAND_INTERVALS];

} DEMAND_TYPE;

/*===========================================================================*\
 * ST_10 & ST_11 structure												       
\*===========================================================================*/
typedef struct {
	
	uint8_t source_flags;
	uint8_t nbr_oum_entries;
	uint8_t nbr_demand_ctrl_entries;
	uint8_t data_ctrl_length;
	uint8_t nbr_data_ctrl_entries;
	uint8_t nbr_constants_entries;
	uint8_t constants_selector;
	uint8_t nbr_sources;

} ST_10_TYPE;

/*===========================================================================*\
 * ST_13 structure												       
\*===========================================================================*/
typedef struct {
	
	uint8_t reset_exclusion;
	uint8_t p_fail_recogntn_tm;
	uint8_t p_fail_exclusion;
	uint8_t cold_load_pickup;
	uint8_t sub_int;
	uint8_t int_multiplier;
	uint16_t int_length;

} ST_13_TYPE;

/*===========================================================================*\
 * ST_20 & ST_21 structure
\*===========================================================================*/
typedef struct {
	
	uint8_t reg_func1_flags;
	uint8_t reg_func2_flags;
	uint8_t nbr_self_reads;
	uint8_t nbr_summations;
	uint8_t nbr_demands;
	uint8_t nbr_coin_values;
	uint8_t nbr_occur;
	uint8_t nbr_tiers;
	uint8_t nbr_present_demands;
	uint8_t nbr_present_values;
	
} ST_20_TYPE;

/*===========================================================================*\
 * ST_22 structure
\*===========================================================================*/
typedef struct {
	
	SUMMATION_ID_TYPE summation_select[MAX_NBR_SUMMATIONS];
	AVERAGE_DEMAND_ID_TYPE demand_select[MAX_NBR_DEMANDS];
	uint8_t min_or_max_flags;
	uint8_t coincident_select[MAX_NBR_COIN_VALUES];
	uint8_t coin_demand_assoc[MAX_NBR_COIN_VALUES];	

} ST_22_TYPE;

/*===========================================================================*\
 * ST_23 structures
\*===========================================================================*/
typedef struct {
	
	uint64_t coincident_values[MAX_NBR_OCCUR];
	
} COINCIDENTS_RCD_TYPE;

typedef struct {
	
	uint32_t event_time[MAX_NBR_OCCUR];
	uint64_t cum_demand;
	uint64_t cont_cum_demand;
	uint64_t demand[MAX_NBR_OCCUR];
	
} DEMANDS_RCD_TYPE;

typedef struct {
	
	double summations[MAX_NBR_SUMMATIONS];
	DEMANDS_RCD_TYPE demands[MAX_NBR_DEMANDS];
	COINCIDENTS_RCD_TYPE coincidents[MAX_NBR_COIN_VALUES];
	
} DATA_BLK_RCD_TYPE;

typedef struct {
	
	uint8_t nbr_demand_resets;
	DATA_BLK_RCD_TYPE tot_data_block;
	DATA_BLK_RCD_TYPE tier_data_block[MAX_NBR_TIERS];
	
} ST_23_TYPE;

/*===========================================================================*\
 * ST_27 structure
\*===========================================================================*/
typedef struct {
	
	PRESENT_DEMAND_ID_TYPE present_demand_select[MAX_NBR_PRESENT_DEMANDS];
	INSTANTANEOUS_VALUE_ID_TYPE present_value_select[MAX_NBR_PRESENT_VALUES];
	
} ST_27_TYPE;

/*===========================================================================*\
 * Registers Type
\*===========================================================================*/
typedef struct {
	
	xTaskHandle storage_task_id;
	bool rtos_task_active;

	bool task_execute;						// Should be TRUE each second
	uint32_t time_after_last_backup;		// seconds
	bool max_demand;
	
} REGISTERS_OS_TYPE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
extern REGISTERS_OS_TYPE registers_os;

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
 */
extern void REGS_APP_vInitialize(void);
extern void REGS_APP_vCreate_task(void);
extern void REGS_APP_v1sec_handler(void);
extern void REGS_APP_vSummation_task(void);
extern void REGS_APP_vDemand_task(void);
extern void REGS_APP_vStore_summations(void);
extern void REGS_APP_vStore_demands(void);

extern void REGS_APP_vSuspend_storage_rtos_task(void);
extern void REGS_APP_vResume_storage_rtos_task(void);

extern bool REGS_APP_bSt_10_execute_table_reading(uint8_t *table_buffer);
extern uint16_t REGS_APP_usSt_10_get_size(void);

extern bool REGS_APP_bLoad_st_11(void);
extern bool REGS_APP_bSt_11_restore_default_values(void);
extern bool REGS_APP_bSt_11_refresh_ram(void);
extern bool REGS_APP_bSt_11_execute_table_reading(uint8_t *table_buffer);
extern bool REGS_APP_bSt_11_execute_table_writing(uint8_t *table_buffer);
extern void REGS_APP_vCopy_st_11_nvm(uint8_t* destination);
extern void REGS_APP_vCopy_st_11_ram(uint8_t* destination);
extern void REGS_APP_vSt_11_recover_nvm(void);
extern void REGS_APP_vSt_11_recover_ram(void);
extern uint16_t REGS_APP_usSt_11_get_size(void);

extern bool REGS_APP_bSt_12_execute_table_reading(uint8_t *table_buffer);
extern uint16_t REGS_APP_usSt_12_get_size(void);

extern bool REGS_APP_bLoad_st_13(void);
extern void REGS_APP_bSt_13_restore_default_values(void);
extern bool REGS_APP_bSt_13_execute_table_reading(uint8_t *table_buffer);
extern bool REGS_APP_bSt_13_execute_table_writing(uint8_t *table_buffer);
extern uint16_t REGS_APP_usSt_13_get_size(void);

extern bool REGS_APP_bSt_15_execute_table_reading(uint8_t *table_buffer);
extern uint16_t REGS_APP_usSt_15_get_size(void);

extern bool REGS_APP_bSt_16_execute_table_reading(uint8_t *table_buffer);
extern uint16_t REGS_APP_usSt_16_get_size(void);

extern bool REGS_APP_bSt_20_execute_table_reading(uint8_t *table_buffer);
extern uint16_t REGS_APP_usSt_20_get_size(void);

extern bool REGS_APP_bLoad_st_21(void);
extern bool REGS_APP_bSt_21_restore_default_values(void);
extern bool REGS_APP_bSt_21_refresh_ram(void);
extern bool REGS_APP_bSt_21_execute_table_reading(uint8_t *table_buffer);
extern bool REGS_APP_bSt_21_execute_table_writing(uint8_t *table_buffer);
extern void REGS_APP_vCopy_st_21_nvm(uint8_t* destination);
extern void REGS_APP_vCopy_st_21_ram(uint8_t* destination);
extern void REGS_APP_vSt_21_recover_nvm(void);
extern void REGS_APP_vSt_21_recover_ram(void);
extern uint16_t REGS_APP_usSt_21_get_size(void);

extern bool REGS_APP_bLoad_st_22(void);
extern bool REGS_APP_bSt_22_restore_default_values(void);
extern bool REGS_APP_bSt_22_refresh_ram(void);
extern bool REGS_APP_bSt_22_execute_table_reading(uint8_t *table_buffer);
extern bool REGS_APP_bSt_22_execute_table_writing(uint8_t *table_buffer);
extern void REGS_APP_vCopy_st_22_nvm(uint8_t* destination);
extern void REGS_APP_vCopy_st_22_ram(uint8_t* destination);
extern void REGS_APP_vSt_22_recover_nvm(void);
extern void REGS_APP_vSt_22_recover_ram(void);
extern uint16_t REGS_APP_usSt_22_get_size(void);

extern bool REGS_APP_bLoad_st_23(void);
extern void REGS_APP_vClear_st_23(void);
extern bool REGS_APP_bSt_23_execute_table_reading(uint8_t *table_buffer);
extern bool REGS_APP_bSt_23_execute_table_writing(uint8_t *table_buffer);
extern uint16_t REGS_APP_usSt_23_get_size(void);

extern void REGS_APP_vClear_st_24(void);
extern bool REGS_APP_bSt_24_execute_table_reading(uint8_t *table_buffer);
extern bool REGS_APP_bSt_24_execute_table_writing(uint8_t *table_buffer);
extern uint16_t REGS_APP_usSt_24_get_size(void);

extern void REGS_APP_vClear_st_25(void);
extern bool REGS_APP_bSt_25_execute_table_reading(uint8_t *table_buffer);
extern bool REGS_APP_bSt_25_execute_table_writing(uint8_t *table_buffer);
extern uint16_t REGS_APP_usSt_25_get_size(void);

extern bool REGS_APP_bLoad_st_26(void);
extern bool REGS_APP_bSt_26_restore_default_values(void);
extern void REGS_APP_vClear_st_26(void);
extern bool REGS_APP_bSt_26_execute_table_reading(uint8_t *table_buffer);
extern bool REGS_APP_bSt_26_execute_table_writing(uint8_t *table_buffer);
extern uint16_t REGS_APP_usSt_26_get_size(void);

extern bool REGS_APP_bLoad_st_27(void);
extern bool REGS_APP_bSt_27_restore_default_values(void);
extern bool REGS_APP_bSt_27_refresh_ram(void);
extern bool REGS_APP_bSt_27_execute_table_reading(uint8_t *table_buffer);
extern bool REGS_APP_bSt_27_execute_table_writing(uint8_t *table_buffer);
extern void REGS_APP_vCopy_st_27_nvm(uint8_t* destination);
extern void REGS_APP_vCopy_st_27_ram(uint8_t* destination);
extern void REGS_APP_vSt_27_recover_nvm(void);
extern void REGS_APP_vSt_27_recover_ram(void);
extern uint16_t REGS_APP_usSt_27_get_size(void);

extern bool REGS_APP_bSt_28_execute_table_reading(uint8_t *table_buffer);
extern uint16_t REGS_APP_usSt_28_get_size(void);

extern void REGS_APP_vReset_summations(void);

extern void REGS_APP_vDemand_reset(uint16_t user, uint8_t season, uint32_t timestamp);
extern void REGS_APP_vSelf_read(uint16_t user, uint8_t season, uint32_t timestamp);

extern void REGS_APP_vReset_present_demand(void);
extern void REGS_APP_vGet_interval_time_remining(DEMAND_INTERVAL_ID_TYPE int_nbr, uint8_t* minute, uint8_t* second);

extern double REGS_APP_dDetermine_instantaneous_power(uint8_t source);

extern void REGS_APP_vCopy_st_23(void);
extern void REGS_APP_vTake_snapshot_st_23(uint8_t cause, uint8_t season, uint32_t timestamp);

extern bool REGS_APP_bCopy_summation(DATA_BLOCK_TYPE data_block, SUMMATION_ID_TYPE summation_id, uint64_t *value_ptr);
extern bool REGS_APP_bCopy_average_demand(DATA_BLOCK_TYPE data_block, AVERAGE_DEMAND_ID_TYPE demand_id, uint64_t *value_ptr);
extern bool REGS_APP_bCopy_present_demand(DEMAND_INTERVAL_ID_TYPE int_nbr, PRESENT_DEMAND_ID_TYPE demand_id, uint64_t *value_ptr);

extern void REGS_APP_vEnable_test_mode(void);
extern void REGS_APP_vDisable_test_mode(void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-06-22
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
#endif /* REGS_H_ */