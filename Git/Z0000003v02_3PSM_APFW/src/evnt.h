/**
 ******************************************************************************
 * \file hist_evnt.h
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

#ifndef HIST_EVNT_H_
#define HIST_EVNT_H_

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "stdint.h"
#include "compiler.h"
   
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/

/* TABLE_IDB_BFLD BIT MASK */
#define	TBL_PROC_NBR_bm										0b0000011111111111
#define	TBL_PROC_NBR_bp										0
#define STD_VS_MFG_FLAG_bm									0b0000100000000000
#define STD_VS_MFG_FLAG_bp									11
#define SELECTOR_bm											0b1111000000000000
#define SELECTOR_bp											12

/* LOG_FLAGS_BFLD BIT MASK */
#define	EVENT_NUMBER_FLAG_bm								0b00000001
#define HIST_DATE_TIME_FLAG_bm								0b00000010
#define HIST_SEQ_NBR_FLAG_bm								0b00000100
#define HIST_INHIBIT_OVF_FLAG_bm							0b00001000
#define EVENT_INHIBIT_OVF_FLAG_bm							0b00010000

/* EXT_LOG_FLAGS_BFLD BIT MASK */
#define	METROLOGICAL_SIG_FLAG_bm							0b00000001
#define PROGRAM_SIG_FLAG_bm									0b00000010
#define ALTERNATE_SIG_FLAG_bm								0b00000100
#define SECURED_REGISTERS_FLAG_bm							0b00001000

/* DISABLE STD/MFG_MONITORED_FLAGS */
#define RELAY_OPEN_CLOSE_MONITORED_FLAG						0x06
#define LOW_VOLTAGE_MONITORED_FLAG							0x80
#define NORMAL_HIGH_VOLTAGE_MONITORED_FLAG					0x03

/*===========================================================================*\
 * ST_70. Log Dimension Limits Table (max values)
\*===========================================================================*/
#define MAX_LOG_FLAGS										(uint8_t)  ( EVENT_NUMBER_FLAG_bm | HIST_DATE_TIME_FLAG_bm | HIST_SEQ_NBR_FLAG_bm | HIST_INHIBIT_OVF_FLAG_bm | EVENT_INHIBIT_OVF_FLAG_bm )
#define MAX_NBR_STD_EVENTS									(uint8_t)  10
#define MAX_NBR_MFG_EVENTS									(uint8_t)  5
#define MAX_HIST_DATA_LENGTH								(uint8_t)  2
#define MAX_EVENT_DATA_LENGTH								(uint8_t)  2
#define MAX_NBR_HISTORY_ENTRIES								(uint16_t) 255
#define MAX_NBR_EVENT_ENTRIES								(uint16_t) 255
#define MAX_EXT_LOG_FLAGS									(uint8_t)  0	// not used for this standard 
#define MAX_NBR_PROGRAM_TABLES								(uint16_t) 0	// not used for this standard

/*===========================================================================*\
 * ST_71. Actual Log Limiting Table (default values)
\*===========================================================================*/
#define DEFAULT_LOG_FLAGS									(uint8_t)  ( EVENT_NUMBER_FLAG_bm | HIST_DATE_TIME_FLAG_bm | HIST_SEQ_NBR_FLAG_bm | HIST_INHIBIT_OVF_FLAG_bm | EVENT_INHIBIT_OVF_FLAG_bm )
#define DEFAULT_NBR_STD_EVENTS								(uint8_t)  10
#define DEFAULT_NBR_MFG_EVENTS								(uint8_t)  5
#define DEFAULT_HIST_DATA_LENGTH							(uint8_t)  2
#define DEFAULT_EVENT_DATA_LENGTH							(uint8_t)  2
#define DEFAULT_NBR_HISTORY_ENTRIES							(uint16_t) 50
#define DEFAULT_NBR_EVENT_ENTRIES							(uint16_t) 50
#define DEFAULT_EXT_LOG_FLAGS								(uint8_t)  0	// not used for this standard 
#define DEFAULT_NBR_PROGRAM_TABLES							(uint16_t) 0	// not used for this standard

/*===========================================================================*\
 * ST_72. Events Identification Table (default values)
\*===========================================================================*/
#define ST_72_STD_EVENTS_SUPPORTED_SET_0					(uint8_t) 0x9E
#define ST_72_STD_EVENTS_SUPPORTED_SET_1					(uint8_t) 0x37
#define ST_72_STD_EVENTS_SUPPORTED_SET_2					(uint8_t) 0xF5
#define ST_72_STD_EVENTS_SUPPORTED_SET_3					(uint8_t) 0xCF
#define ST_72_STD_EVENTS_SUPPORTED_SET_4					(uint8_t) 0x0F
#define ST_72_STD_EVENTS_SUPPORTED_SET_5					(uint8_t) 0x4D
#define ST_72_STD_EVENTS_SUPPORTED_SET_6					(uint8_t) 0x01
#define ST_72_STD_EVENTS_SUPPORTED_SET_7					(uint8_t) 0x00
#define ST_72_STD_EVENTS_SUPPORTED_SET_8					(uint8_t) 0x00
#define ST_72_STD_EVENTS_SUPPORTED_SET_9					(uint8_t) 0x00
#define ST_72_STD_EVENTS_SUPPORTED( set )					EVNT_APP_ucGet_st_72_std_events( set )

#define ST_72_MFG_EVENTS_SUPPORTED_SET_0					(uint8_t) 0xFE
#define ST_72_MFG_EVENTS_SUPPORTED_SET_1					(uint8_t) 0xFF
#define ST_72_MFG_EVENTS_SUPPORTED_SET_2					(uint8_t) 0xFF
#define ST_72_MFG_EVENTS_SUPPORTED_SET_3					(uint8_t) 0xFF
#define ST_72_MFG_EVENTS_SUPPORTED_SET_4					(uint8_t) 0xC3
#define ST_72_MFG_EVENTS_SUPPORTED_SET_5					(uint8_t) 0x00
#define ST_72_MFG_EVENTS_SUPPORTED_SET_6					(uint8_t) 0x00
#define ST_72_MFG_EVENTS_SUPPORTED_SET_7					(uint8_t) 0x00
#define ST_72_MFG_EVENTS_SUPPORTED_SET_8					(uint8_t) 0x00
#define ST_72_MFG_EVENTS_SUPPORTED_SET_9					(uint8_t) 0x00
#define ST_72_MFG_EVENTS_SUPPORTED( set )					EVNT_APP_ucGet_st_72_mfg_events( set )

/*===========================================================================*\
 * ST_73. History Log Control Table (default values)
\*===========================================================================*/
#define ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_0			(uint8_t) 0x00
#define ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_1			(uint8_t) 0x00
#define ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_2			(uint8_t) 0x00
#define ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_3			(uint8_t) 0x00
#define ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_4			(uint8_t) 0x00
#define ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_5			(uint8_t) 0x00
#define ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_6			(uint8_t) 0x00
#define ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_7			(uint8_t) 0x00
#define ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_8			(uint8_t) 0x00
#define ST_73_DEFAULT_STD_EVENTS_MONITORED_SET_9			(uint8_t) 0x00
#define ST_73_DEFAULT_STD_EVENTS_MONITORED( set )			EVNT_APP_ucGet_default_st_73_std_events( set )

#define ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_0			(uint8_t) 0x00
#define ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_1			(uint8_t) 0x00
#define ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_2			(uint8_t) 0x00
#define ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_3			(uint8_t) 0x00
#define ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_4			(uint8_t) 0x00
#define ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_5			(uint8_t) 0x00
#define ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_6			(uint8_t) 0x00
#define ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_7			(uint8_t) 0x00
#define ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_8			(uint8_t) 0x00
#define ST_73_DEFAULT_MFG_EVENTS_MONITORED_SET_9			(uint8_t) 0x00
#define ST_73_DEFAULT_MFG_EVENTS_MONITORED( set )			EVNT_APP_ucGet_default_st_73_mfg_events( set )

/*===========================================================================*\
 * ST_75. Event Log Control Table (default values)
\*===========================================================================*/
#define ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_0			(uint8_t) 0x00
#define ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_1			(uint8_t) 0x00
#define ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_2			(uint8_t) 0x00
#define ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_3			(uint8_t) 0x00
#define ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_4			(uint8_t) 0x00
#define ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_5			(uint8_t) 0x00
#define ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_6			(uint8_t) 0x00
#define ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_7			(uint8_t) 0x00
#define ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_8			(uint8_t) 0x00
#define ST_75_DEFAULT_STD_EVENTS_MONITORED_SET_9			(uint8_t) 0x00
#define ST_75_DEFAULT_STD_EVENTS_MONITORED( set )			EVNT_APP_ucGet_default_st_75_std_events( set )

#define ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_0			(uint8_t) 0x00
#define ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_1			(uint8_t) 0x00
#define ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_2			(uint8_t) 0x00
#define ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_3			(uint8_t) 0x00
#define ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_4			(uint8_t) 0x00
#define ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_5			(uint8_t) 0x00
#define ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_6			(uint8_t) 0x00
#define ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_7			(uint8_t) 0x00
#define ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_8			(uint8_t) 0x00
#define ST_75_DEFAULT_MFG_EVENTS_MONITORED_SET_9			(uint8_t) 0x00
#define ST_75_DEFAULT_MFG_EVENTS_MONITORED( set )			EVNT_APP_ucGet_default_st_75_mfg_events( set )

/*===========================================================================*\
 * ST_74 & ST-76. History Log Data Table & Event Log Data Table (default values)
\*===========================================================================*/
#define ORDER_FLAG_bm										0b00000001
#define ORDER_FLAG_bp										0
#define OVERFLOW_FLAG_bm									0b00000010
#define OVERFLOW_FLAG_bp									1
#define LIST_TYPE_FLAG_bm									0b00000100
#define LIST_TYPE_FLAG_bp									2
#define INHIBIT_OVERFLOW_FLAG_bm							0b00001000
#define INHIBIT_OVERFLOW_FLAG_bp							3

#define ST_74_DEFAULT_LIST_STATUS							(uint8_t)   ( (0 << ORDER_FLAG_bp) | (0 << OVERFLOW_FLAG_bp) | (1 << LIST_TYPE_FLAG_bp) | (0 << INHIBIT_OVERFLOW_FLAG_bp) )
#define ST_74_DEFAULT_NBR_VALID_ENTRIES						(uint16_t)  0
#define	ST_74_DEFAULT_LAST_ENTRY_ELEMENT					(uint16_t)  0
#define ST_74_DEFAULT_LAST_ENTRY_SEQ_NBR					(uint32_t)  0
#define ST_74_DEFAULT_NBR_UNREAD_ENTRIES					(uint16_t)  0

#define ST_76_DEFAULT_LIST_STATUS							(uint8_t)   ( (0 << ORDER_FLAG_bp) | (0 << OVERFLOW_FLAG_bp) | (1 << LIST_TYPE_FLAG_bp) | (0 << INHIBIT_OVERFLOW_FLAG_bp) )
#define ST_76_DEFAULT_NBR_VALID_ENTRIES						(uint16_t)  0
#define	ST_76_DEFAULT_LAST_ENTRY_ELEMENT					(uint16_t)  0
#define ST_76_DEFAULT_LAST_ENTRY_SEQ_NBR					(uint32_t)  0
#define ST_76_DEFAULT_NBR_UNREAD_ENTRIES					(uint16_t)  0

/*===========================================================================*\
 * MT_72. Alarms Identification Table (default values)
\*===========================================================================*/
/* Alarms related with standard events */
#define MT_72_STD_ALARMS_SUPPORTED_SET_0					(uint8_t) 0x9E	// 0-7
#define MT_72_STD_ALARMS_SUPPORTED_SET_1					(uint8_t) 0x37	// 8-15
#define MT_72_STD_ALARMS_SUPPORTED_SET_2					(uint8_t) 0xFF	// 16-23
#define MT_72_STD_ALARMS_SUPPORTED_SET_3					(uint8_t) 0xCF	// 24-31
#define MT_72_STD_ALARMS_SUPPORTED_SET_4					(uint8_t) 0x63	// 32-39
#define MT_72_STD_ALARMS_SUPPORTED_SET_5					(uint8_t) 0x5F	// 40-47
#define MT_72_STD_ALARMS_SUPPORTED_SET_6					(uint8_t) 0x01	// 48-55
#define MT_72_STD_ALARMS_SUPPORTED_SET_7					(uint8_t) 0x00	// 56-63
#define MT_72_STD_ALARMS_SUPPORTED_SET_8					(uint8_t) 0x00	// 64-71
#define MT_72_STD_ALARMS_SUPPORTED_SET_9					(uint8_t) 0x00	// 72-79
#define MT_72_STD_ALARMS_SUPPORTED( set )					EVNT_APP_ucGet_mt_72_std_alarms( set )

/* Alarms related with manufacturer events */
#define MT_72_MFG_ALARMS_SUPPORTED_SET_0					(uint8_t) 0xFE
#define MT_72_MFG_ALARMS_SUPPORTED_SET_1					(uint8_t) 0xFF
#define MT_72_MFG_ALARMS_SUPPORTED_SET_2					(uint8_t) 0xFF
#define MT_72_MFG_ALARMS_SUPPORTED_SET_3					(uint8_t) 0xFF
#define MT_72_MFG_ALARMS_SUPPORTED_SET_4					(uint8_t) 0xC3
#define MT_72_MFG_ALARMS_SUPPORTED_SET_5					(uint8_t) 0x00
#define MT_72_MFG_ALARMS_SUPPORTED_SET_6					(uint8_t) 0x00
#define MT_72_MFG_ALARMS_SUPPORTED_SET_7					(uint8_t) 0x00
#define MT_72_MFG_ALARMS_SUPPORTED_SET_8					(uint8_t) 0x00
#define MT_72_MFG_ALARMS_SUPPORTED_SET_9					(uint8_t) 0x00
#define MT_72_MFG_ALARMS_SUPPORTED( set )					EVNT_APP_ucGet_mt_72_mfg_alarms( set )

/*===========================================================================*\
 * MT_73. Alarms Control Table (default values)
\*===========================================================================*/
/* Alarms related with standard events */
#define MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_0			(uint8_t) 0x00
#define MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_1			(uint8_t) 0x00
#define MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_2			(uint8_t) 0x00
#define MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_3			(uint8_t) 0x00
#define MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_4			(uint8_t) 0x00
#define MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_5			(uint8_t) 0x00
#define MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_6			(uint8_t) 0x00
#define MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_7			(uint8_t) 0x00
#define MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_8			(uint8_t) 0x00
#define MT_73_DEFAULT_STD_ALARMS_MONITORED_SET_9			(uint8_t) 0x00
#define MT_73_DEFAULT_STD_ALARMS_MONITORED( set )			EVNT_APP_ucGet_default_mt_73_std_alarms( set )

/* Alarms related with manufacturer events */
#define MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_0			(uint8_t) 0x00
#define MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_1			(uint8_t) 0x00
#define MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_2			(uint8_t) 0x00
#define MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_3			(uint8_t) 0x00
#define MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_4			(uint8_t) 0x00
#define MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_5			(uint8_t) 0x00
#define MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_6			(uint8_t) 0x00
#define MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_7			(uint8_t) 0x00
#define MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_8			(uint8_t) 0x00
#define MT_73_DEFAULT_MFG_ALARMS_MONITORED_SET_9			(uint8_t) 0x00
#define MT_73_DEFAULT_MFG_ALARMS_MONITORED( set )			EVNT_APP_ucGet_default_mt_73_mfg_alarms( set )

/*===========================================================================*\
 * C12.19 Decade 7 Tables Size
\*===========================================================================*/

/* ST_70 Elements size */
#define ST_70_SIZE										9

/* ST_71 Elements size */
#define ST_71_SIZE										9

#define ST_71_LOG_FLAGS_SIZE							1
#define ST_71_NBR_STD_EVENTS_SIZE						1
#define ST_71_NBR_MFG_EVENTS_SIZE						1
#define ST_71_HIST_DATA_LENGTH_SIZE						1
#define ST_71_EVENT_DATA_LENGTH_SIZE					1
#define ST_71_NBR_HISTORY_ENTRIES_SIZE					2
#define	ST_71_NBR_EVENT_ENTRIES_SIZE					2
#define	ST_71_MAX_SIZE								   (ST_71_LOG_FLAGS_SIZE			+ \
														ST_71_NBR_STD_EVENTS_SIZE		+ \
														ST_71_NBR_MFG_EVENTS_SIZE		+ \
														ST_71_HIST_DATA_LENGTH_SIZE		+ \
														ST_71_EVENT_DATA_LENGTH_SIZE	+ \
														ST_71_NBR_HISTORY_ENTRIES_SIZE	+ \
														ST_71_NBR_EVENT_ENTRIES_SIZE	)

/* ST_72 Elements size */
#define ST_72_STD_EVENTS_SUPPORTED_SIZE					MAX_NBR_STD_EVENTS
#define ST_72_MFG_EVENTS_SUPPORTED_SIZE					MAX_NBR_MFG_EVENTS
#define ST_72_MAX_SIZE									(ST_72_STD_EVENTS_SUPPORTED_SIZE + ST_72_MFG_EVENTS_SUPPORTED_SIZE)

/* ST_73 Elements size */
#define ST_73_STD_EVENTS_MONITORED_FLAGS_SIZE			MAX_NBR_STD_EVENTS
#define ST_73_MFG_EVENTS_MONITORED_FLAGS_SIZE			MAX_NBR_MFG_EVENTS
#define ST_73_STD_TBLS_MONITORED_FLAGS_SIZE				DIM_STD_TBLS_USED
#define ST_73_MFG_TBLS_MONITORED_FLAGS_SIZE				DIM_MFG_TBLS_USED
#define ST_73_STD_PROC_MONITORED_FLAGS_SIZE				DIM_STD_PROC_USED
#define ST_73_MFG_PROC_MONITORED_FLAGS_SIZE				DIM_MFG_PROC_USED
#define ST_73_MAX_SIZE								   (ST_73_STD_EVENTS_MONITORED_FLAGS_SIZE	+ \
														ST_73_MFG_EVENTS_MONITORED_FLAGS_SIZE	+ \
														ST_73_STD_TBLS_MONITORED_FLAGS_SIZE		+ \
														ST_73_MFG_TBLS_MONITORED_FLAGS_SIZE		+ \
														ST_73_STD_PROC_MONITORED_FLAGS_SIZE		+ \
														ST_73_MFG_PROC_MONITORED_FLAGS_SIZE		)

/* ST_74 Elements size */
#define ST_74_HIST_FLAGS_SIZE							 1
#define ST_74_NBR_VALID_ENTRIES_SIZE					 2
#define ST_74_LAST_ENTRY_ELEMENT_SIZE					 2
#define ST_74_LAST_ENTRY_SEQ_NBR_SIZE					 4
#define ST_74_NBR_UNREAD_ENTRIES_SIZE					 2
#define ST_74_HISTORY_TIME_SIZE							 (4 * MAX_NBR_HISTORY_ENTRIES)
#define ST_74_EVENT_NUMBER_SIZE							 (2 * MAX_NBR_HISTORY_ENTRIES)
#define ST_74_HISTORY_SEQ_NBR_SIZE						 (2 * MAX_NBR_HISTORY_ENTRIES)
#define ST_74_USER_ID_SIZE								 (2 * MAX_NBR_HISTORY_ENTRIES)
#define ST_74_HISTORY_CODE_SIZE							 (2 * MAX_NBR_HISTORY_ENTRIES)
#define ST_74_HISTORY_ARGUMENT_SIZE						 (2 * MAX_NBR_HISTORY_ENTRIES)
#define ST_74_MAX_SIZE									(ST_74_HIST_FLAGS_SIZE			+ \
														 ST_74_NBR_VALID_ENTRIES_SIZE	+ \
														 ST_74_LAST_ENTRY_ELEMENT_SIZE	+ \
														 ST_74_LAST_ENTRY_SEQ_NBR_SIZE	+ \
														 ST_74_NBR_UNREAD_ENTRIES_SIZE	+ \
														 ST_74_HISTORY_TIME_SIZE		+ \	
														 ST_74_EVENT_NUMBER_SIZE		+ \	
														 ST_74_HISTORY_SEQ_NBR_SIZE		+ \
														 ST_74_USER_ID_SIZE				+ \
														 ST_74_HISTORY_CODE_SIZE		+ \	
														 ST_74_HISTORY_ARGUMENT_SIZE	)
														 	
/* ST_75 Elements size */
#define ST_75_STD_EVENTS_MONITORED_FLAGS_SIZE			MAX_NBR_STD_EVENTS
#define ST_75_MFG_EVENTS_MONITORED_FLAGS_SIZE			MAX_NBR_MFG_EVENTS
#define ST_75_STD_TBLS_MONITORED_FLAGS_SIZE				DIM_STD_TBLS_USED
#define ST_75_MFG_TBLS_MONITORED_FLAGS_SIZE				DIM_MFG_TBLS_USED
#define ST_75_STD_PROC_MONITORED_FLAGS_SIZE				DIM_STD_PROC_USED
#define ST_75_MFG_PROC_MONITORED_FLAGS_SIZE				DIM_MFG_PROC_USED
#define ST_75_MAX_SIZE								   (ST_75_STD_EVENTS_MONITORED_FLAGS_SIZE	+ \
														ST_75_MFG_EVENTS_MONITORED_FLAGS_SIZE	+ \
														ST_75_STD_TBLS_MONITORED_FLAGS_SIZE		+ \
														ST_75_MFG_TBLS_MONITORED_FLAGS_SIZE		+ \
														ST_75_STD_PROC_MONITORED_FLAGS_SIZE		+ \
														ST_75_MFG_PROC_MONITORED_FLAGS_SIZE		)

/* ST_76 Elements size */
#define ST_76_EVENT_FLAGS_SIZE							 1
#define ST_76_NBR_VALID_ENTRIES_SIZE					 2
#define ST_76_LAST_ENTRY_ELEMENT_SIZE					 2
#define ST_76_LAST_ENTRY_SEQ_NBR_SIZE					 4
#define ST_76_NBR_UNREAD_ENTRIES_SIZE					 2
#define ST_76_EVENT_TIME_SIZE							 (4 * MAX_NBR_EVENT_ENTRIES)
#define ST_76_EVENT_NUMBER_SIZE							 (2 * MAX_NBR_EVENT_ENTRIES)
#define ST_76_EVENT_SEQ_NBR_SIZE						 (2 * MAX_NBR_EVENT_ENTRIES)
#define ST_76_USER_ID_SIZE								 (2 * MAX_NBR_EVENT_ENTRIES)
#define ST_76_EVENT_CODE_SIZE							 (2 * MAX_NBR_EVENT_ENTRIES)
#define ST_76_EVENT_ARGUMENT_SIZE						 (2 * MAX_NBR_EVENT_ENTRIES)
#define ST_76_MAX_SIZE								   (ST_76_EVENT_FLAGS_SIZE			+ \
														ST_76_NBR_VALID_ENTRIES_SIZE	+ \
														ST_76_LAST_ENTRY_ELEMENT_SIZE	+ \
														ST_76_LAST_ENTRY_SEQ_NBR_SIZE	+ \
														ST_76_NBR_UNREAD_ENTRIES_SIZE	+ \
														ST_76_EVENT_TIME_SIZE			+ \
														ST_76_EVENT_NUMBER_SIZE			+ \
														ST_76_EVENT_SEQ_NBR_SIZE		+ \
														ST_76_USER_ID_SIZE				+ \
														ST_76_EVENT_CODE_SIZE			+ \
														ST_76_EVENT_ARGUMENT_SIZE		)

/* MT_72 Elements size */
#define MT_72_MAX_SIZE									(ST_72_STD_EVENTS_SUPPORTED_SIZE + ST_72_MFG_EVENTS_SUPPORTED_SIZE)

/* MT_73 Elements size */
#define MT_73_STD_ALARMS_MONITORED_FLAGS_SIZE			MAX_NBR_STD_EVENTS
#define MT_73_MFG_ALARMS_MONITORED_FLAGS_SIZE			MAX_NBR_MFG_EVENTS
#define MT_73_STD_TBLS_MONITORED_FLAGS_SIZE				DIM_STD_TBLS_USED
#define MT_73_MFG_TBLS_MONITORED_FLAGS_SIZE				DIM_MFG_TBLS_USED
#define MT_73_STD_PROC_MONITORED_FLAGS_SIZE				DIM_STD_PROC_USED
#define MT_73_MFG_PROC_MONITORED_FLAGS_SIZE				DIM_MFG_PROC_USED
#define MT_73_MAX_SIZE								   (MT_73_STD_ALARMS_MONITORED_FLAGS_SIZE	+ \
														MT_73_MFG_ALARMS_MONITORED_FLAGS_SIZE	+ \
														MT_73_STD_TBLS_MONITORED_FLAGS_SIZE		+ \
														MT_73_MFG_TBLS_MONITORED_FLAGS_SIZE		+ \
														MT_73_STD_PROC_MONITORED_FLAGS_SIZE		+ \
														MT_73_MFG_PROC_MONITORED_FLAGS_SIZE		)

/*===========================================================================*\
 * USER_ID. User ID associated with Event Log and History Log (ST_74 & ST_76)
\*===========================================================================*/
#define END_DEVICE		(uint16_t) 0x0000
#define MANUALLY		(uint16_t) 0x0001

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
 * Standard Event Codes
\*===========================================================================*/
typedef enum {
	
	NO_EVENT						= 0,
	PRIMARY_POWER_DOWN,
	PRIMARY_POWER_UP,
	TIME_CHANGED_A,
	TIME_CHANGED_B,
	TIME_CHANGED_C,
	TIME_CHANGED_D,
	END_DEVICE_ACCESSED_FOR_READ,
	END_DEVICE_ACCESSED_FOR_WRITE,
	PROCEDURE_INVOKED,
	TABLE_WRITTEN_TO,
	END_DEVIDE_PROGRAMMED,
	COMMUNICATION_TERMINATED_NORMALLY,
	COMMUNICATION_TERMINATED_ABNORMALLY,
	RESET_LIST_POINTERS,
	UPDATE_LIST_POINTERS,
	HISTORY_LOG_CLEARED,
	HISTORY_LOG_POINTERS_UPDATED,
	EVENT_LOG_CLEARED,
	EVENT_LOG_POINTERS_UPDATED,
	DEMAND_RESET_OCCURRED,
	SELF_READ_OCCURRED,
	DAYLIGHT_SAVINGS_TIME_ON,
	DAYLIGHT_SAVINGS_TIME_OFF,
	SEASON_CHANGES,
	RATE_CHANGE,
	SPECIAL_SCHEDULE_ACTIVATION,
	TIER_SWITCH_CHANGE,
	PENDING_TABLE_ACTIVATION,
	PENDING_TABLE_CLEAR,
	METERING_MODE_STARTED,
	METERING_MODE_STOPPED,
	TEST_MODE_STARTED,
	TEST_MODE_STOPPED,
	METER_SHOP_MODE_STARTED,
	METER_SHOP_MODE_STOPPED,
	END_DEVICE_REPROGRAMMED,
	CONFIGURATION_ERROR_DETECTED,
	SELF_CHECK_ERROR_DETECTED,
	RAM_FAILURE_DETECTED,
	ROM_FAILURE_DETECTED,
	NONVOLATILE_MEMORY_FAILURE_DETECTED,
	CLOCK_ERROR_DETECTED,
	MEASUREMENT_ERROR_DETECTED,
	LOW_BATTERY_DETECTED,
	LOW_LOSS_POTENTIAL_DETECTED,
	DEMAND_OVERLOAD_DETECTED,
	TAMPER_ATTEMP_DETECTED,
	REVERSE_ROTATION_DETECTED
	
} STD_EVNT_CODES_TYPE;

/*===========================================================================*\
 * Manufacturer Event Codes
\*===========================================================================*/
typedef enum {
	
	RESERVED_0			= 0,
	PHASE_A_FAILURE,
	PHASE_A_RESTORATION,
	PHASE_B_FAILURE,
	PHASE_B_RESTORATION,
	PHASE_C_FAILURE,
	PHASE_C_RESTORATION,
	PHASE_A_LOW_VOLTAGE,
	PHASE_A_NORMAL_VOLTAGE,
	PHASE_A_HIGH_VOLTAGE,
	PHASE_B_LOW_VOLTAGE,
	PHASE_B_NORMAL_VOLTAGE,
	PHASE_B_HIGH_VOLTAGE,
	PHASE_C_LOW_VOLTAGE,
	PHASE_C_NORMAL_VOLTAGE,
	PHASE_C_HIGH_VOLTAGE,
	TIME_CHANGED_A_OP,
	TIME_CHANGED_B_OP,
	TIME_CHANGED_A_NIC,
	TIME_CHANGED_B_NIC,
	END_DEVICE_ACCESSED_FOR_READ_OP,
	END_DEVICE_ACCESSED_FOR_READ_NIC,
	END_DEVICE_ACCESSED_FOR_WRITE_OP,
	END_DEVICE_ACCESSED_FOR_WRITE_NIC,
	COMMUNICATION_TERMINATED_NORMALLY_OP,
	COMMUNICATION_TERMINATED_NORMALLY_NIC,
	COMMUNICATION_TERMINATED_ABNORMALLY_OP,
	COMMUNICATION_TERMINATED_ABNORMALLY_NIC,
	DEMAND_RESET_OCCURRED_OP,
	DEMAND_RESET_OCCURRED_NIC,
	TEST_MODE_STARTED_OP,
	TEST_MODE_STARTED_NIC,
	TEST_MODE_STOPPED_OP,
	TEST_MODE_STOPPED_NIC,
	RELAY_CLOSED_OP,
	RELAY_CLOSED_NIC,
	RELAY_OPENED_OP,
	RELAY_OPENED_NIC,
	INCORRECT_PASSWORD_OP,
	INCORRECT_PASSWORD_NIC
} MFG_EVNT_CODES_TYPE;

/*===========================================================================*\
 * ST_70 & ST_71 structure												       
\*===========================================================================*/
typedef struct {
	
	uint8_t log_flags;
	uint8_t nbr_std_events;
	uint8_t nbr_mfg_events;
	uint8_t hist_data_length;
	uint8_t event_data_length;
	uint16_t nbr_history_entries;
	uint16_t nbr_event_entries;
	uint8_t ext_log_flags;
	uint16_t nbr_program_tables;
	
} ST_70_TYPE;

/*===========================================================================*\
 * ST_73 & ST_75 structure												       
\*===========================================================================*/
typedef struct {
	
	uint8_t std_events_monitored_flags[MAX_NBR_STD_EVENTS];
	uint8_t mfg_events_monitored_flags[MAX_NBR_MFG_EVENTS];
	uint8_t std_tbls_monitored_flags[40];
	uint8_t mfg_tbls_monitored_flags[40];
	uint8_t std_proc_monitored_flags[40];
	uint8_t mfg_proc_monitored_flags[40];
	
} ST_73_TYPE;

/*===========================================================================*\
 * MT_73 structure												       
\*===========================================================================*/
typedef struct {
	
	uint8_t std_alarms_monitored_flags[MAX_NBR_STD_EVENTS];
	uint8_t mfg_alarms_monitored_flags[MAX_NBR_MFG_EVENTS];
	
} MT_73_TYPE;

/*===========================================================================*\
 * ST_74 & ST_76 Structure												       
\*===========================================================================*/
typedef struct {
	
	uint8_t event_flags;
	uint16_t nbr_valid_entries;
	uint16_t last_entry_element;
	uint32_t last_entry_seq_nbr;
	uint16_t nbr_unread_entries;
	
} ST_74_TYPE;

/*===========================================================================*\
 * Event/Alarm Structure												       
\*===========================================================================*/
typedef struct {
	
	uint8_t code;
	uint16_t argument;
	bool is_std;
	uint16_t user_id;
	uint32_t timestamp;
	uint16_t event_number;
	
} EVENT_ALARM_TYPE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
extern ST_70_TYPE st_71;
extern ST_73_TYPE st_73;
extern ST_73_TYPE st_75;
extern MT_73_TYPE mt_73;

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/
extern void EVNT_APP_vRegister_event(uint8_t code, uint16_t argument, bool is_std, uint16_t user_id, uint32_t timestamp);
extern void EVNT_APP_vGenerate_std_alarm(uint8_t alarm_id);
extern void EVNT_APP_vGenerate_mfg_alarm(uint8_t alarm_id);

extern bool EVNT_APP_ubSt_70_execute_table_reading(uint8_t *table_buffer);
extern uint16_t EVNT_APP_usSt_70_get_size(void);

extern bool EVNT_APP_bLoad_st_71(void);
extern bool EVNT_APP_bSt_71_restore_default_values(void);
extern bool EVNT_APP_bSt_71_refresh_ram(void);
extern bool EVNT_APP_bSt_71_execute_table_reading(uint8_t *table_buffer);
extern bool EVNT_APP_bSt_71_execute_table_writing(uint8_t *table_buffer);
extern void EVNT_APP_vCopy_st_71_nvm(uint8_t* destination);
extern void EVNT_APP_vCopy_st_71_ram(uint8_t* destination);
extern void EVNT_APP_vSt_71_recover_nvm(void);
extern void EVNT_APP_vSt_71_recover_ram(void);
extern uint16_t EVNT_APP_usSt_71_get_size(void);

extern bool EVNT_APP_bSt_72_execute_table_reading(uint8_t *table_buffer);
extern uint16_t EVNT_APP_usSt_72_get_size(void);
extern uint8_t EVNT_APP_ucGet_st_72_std_events(uint8_t set_nbr);
extern uint8_t EVNT_APP_ucGet_st_72_mfg_events(uint8_t set_nbr);

extern bool EVNT_APP_bMt_72_execute_table_reading(uint8_t *table_buffer);
extern uint16_t EVNT_APP_usMt_72_get_size(void);
extern uint8_t EVNT_APP_ucGet_mt_72_std_alarms(uint8_t set_nbr);
extern uint8_t EVNT_APP_ucGet_mt_72_mfg_alarms(uint8_t set_nbr);

extern bool EVNT_APP_bLoad_st_73(void);
extern bool EVNT_APP_bSt_73_restore_default_values(void);
extern bool EVNT_APP_bSt_73_refresh_ram(void);
extern bool EVNT_APP_bSt_73_execute_table_reading(uint8_t *table_buffer);
extern bool EVNT_APP_bSt_73_execute_table_writing(uint8_t *table_buffer);
extern void EVNT_APP_vCopy_st_73_nvm(uint8_t* destination);
extern void EVNT_APP_vCopy_st_73_ram(uint8_t* destination);
extern void EVNT_APP_vSt_73_recover_nvm(void);
extern void EVNT_APP_vSt_73_recover_ram(void);
extern uint16_t EVNT_APP_usSt_73_get_size(void);
extern uint8_t EVNT_APP_ucGet_default_st_73_std_events(uint8_t set_nbr);
extern uint8_t EVNT_APP_ucGet_default_st_73_mfg_events(uint8_t set_nbr);

extern bool EVNT_APP_bLoad_mt_73(void);
extern bool EVNT_APP_bMt_73_restore_default_values(void);
extern bool EVNT_APP_bMt_73_refresh_ram(void);
extern bool EVNT_APP_bMt_73_execute_table_reading(uint8_t *table_buffer);
extern bool EVNT_APP_bMt_73_execute_table_writing(uint8_t *table_buffer);
extern void EVNT_APP_vCopy_mt_73_nvm(uint8_t* destination);
extern void EVNT_APP_vCopy_mt_73_ram(uint8_t* destination);
extern void EVNT_APP_vMt_73_recover_nvm(void);
extern void EVNT_APP_vMt_73_recover_ram(void);
extern uint16_t EVNT_APP_usMt_73_get_size(void);
extern uint8_t EVNT_APP_ucGet_default_mt_73_std_alarms(uint8_t set_nbr);
extern uint8_t EVNT_APP_ucGet_default_mt_73_mfg_alarms(uint8_t set_nbr);

extern bool EVNT_APP_bIs_std_alarm_monitored_by_mt_73(uint8_t code);
extern bool EVNT_APP_bIs_mfg_alarm_monitored_by_mt_73(uint8_t code);

extern bool EVNT_APP_bLoad_st_74(void);
extern bool EVNT_APP_bSt_74_restore_default_values(void);
extern bool EVNT_APP_bSt_74_refresh_ram(void);
extern void EVNT_APP_vClear_st_74(uint16_t user_attempt);
extern bool EVNT_APP_bSt_74_execute_table_reading(uint8_t *table_buffer);
extern bool EVNT_APP_bSt_74_execute_table_writing(uint8_t *table_buffer);
extern uint16_t EVNT_APP_usSt_74_get_size(void);

extern bool EVNT_APP_bLoad_st_75(void);
extern bool EVNT_APP_bSt_75_restore_default_values(void);
extern bool EVNT_APP_bSt_75_refresh_ram(void);
extern bool EVNT_APP_bSt_75_execute_table_reading(uint8_t *table_buffer);
extern bool EVNT_APP_bSt_75_execute_table_writing(uint8_t *table_buffer);
extern void EVNT_APP_vCopy_st_75_nvm(uint8_t* destination);
extern void EVNT_APP_vCopy_st_75_ram(uint8_t* destination);
extern void EVNT_APP_vSt_75_recover_nvm(void);
extern void EVNT_APP_vSt_75_recover_ram(void);
extern uint16_t EVNT_APP_usSt_75_get_size(void);
extern uint8_t EVNT_APP_ucGet_default_st_75_std_events(uint8_t set_nbr);
extern uint8_t EVNT_APP_ucGet_default_st_75_mfg_events(uint8_t set_nbr);

extern bool EVNT_APP_bLoad_st_76(void);
extern bool EVNT_APP_bSt_76_restore_default_values(void);
extern bool EVNT_APP_bSt_76_refresh_ram(void);
extern void EVNT_APP_vClear_st_76(uint16_t user_attempt);
extern bool EVNT_APP_bSt_76_execute_table_reading(uint8_t *table_buffer);
extern bool EVNT_APP_bSt_76_execute_table_writing(uint8_t *table_buffer);
extern uint16_t EVNT_APP_usSt_76_get_size(void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-08-13
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
#endif /* HIST_EVNT_H_ */
