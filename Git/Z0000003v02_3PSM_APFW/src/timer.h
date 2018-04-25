/**
 ******************************************************************************
 * \file timer.h
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2015 by Softek Global eDesign S. de R.L. de C.V. All rights reserved.</b><br><br>
 * Softek Global eDesign Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup system_init system_init Library
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all exportable functions for system_init Library.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 *   - SIMCEGAB - Sistema de Medicion de Energia Electrica Centralizado en Gabinete.
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *     - CFE: Specification CFE G0100. April 2015.
 *
 *   - Applicable Standards:
 *     - C1218 : Communication Protocol for MM100 Boards
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - Format: F-P06-DS-03
 *   - Revision: 01
 *   - Date: 2015-10-02
 *   - Author: Mariano Centeno Camarena.
 ******************************************************************************
 */

#ifndef TIMER_H_
#define TIMER_H_

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
#define TIMER_ms_tic						0.001 // Tick period in seconds.
#define TIMER_ms_to_tic( ms_time )			( ms_time / (TIMER_ms_tic*1000) )
#define TIMER_sec_to_ms_tic( sec_time )		( sec_time * 100UL )

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
 * Miliseconds timer structure
\*===========================================================================*/
typedef struct ms_timer_type
{
	bool running;
	bool expired;
	volatile uint32_t timer;
	void_fptr fptr;
}MS_TIMER_TYPE;

/*===========================================================================*\
 * Seconds timer structure
\*===========================================================================*/
typedef struct min_timer_type
{
	bool running;
	bool expired;
	uint16_t timer;
	void_fptr fptr;
}MIN_TIMER_TYPE;

typedef enum ms_timer_id_type
{
	NIC_RX_TIMER,
	NIC_TX_TIMER,
	NIC_LOGON_TIMER,
	NIC_RESPONSE_TIMEOUT_TIMER,
	
	OP_RX_TIMER,
	OP_TX_TIMER,
	OP_LOGON_TIMER,
	OP_RESPONSE_TIMEOUT_TIMER,
	
	LED_BLINKING_TIMER,
	LP_INTERVAL_TIMER,
	
	METROLOGY_CHECK_TIMER,
	IO_MCU_CONFIG_TIMER,
	DIAGNOSTICS_EXECUTION_TIMER,
	CONFIGURATION_ERROR_TIMER,	
	SM_FWU_TIMER,
	SM_START_RESET_TIMER,
	
	SM_PHA_VOLTAGE_MONITOR_TIMER,
	SM_PHB_VOLTAGE_MONITOR_TIMER,
	SM_PHC_VOLTAGE_MONITOR_TIMER,
	
	SM_TEST_MODE_TIMER,
	
	MCU_FREEZIG_TIMER,
	SAVE_INFORMATION_TIMER,
	
	NUMBER_OF_MS_TIMERS
}MS_TIMER_ID_TYPE;

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
extern void TIMER_initialize(void);
extern void TIMER_io_initialize(void);
extern void TIMER_io_configure(void);

extern void TIMER_ms_tick_handler(void);

extern void TIMER_start_ms_timer (MS_TIMER_ID_TYPE timer_id, uint32_t period, void_fptr function_ptr);
extern bool TIMER_is_ms_timer_running(MS_TIMER_ID_TYPE timer_id);
extern bool TIMER_is_ms_timer_expired(MS_TIMER_ID_TYPE timer_id);
extern void TIMER_reset_ms_timer(MS_TIMER_ID_TYPE timer_id);
extern uint16_t TIMER_get_pending_ms_time(MS_TIMER_ID_TYPE timer_id);
extern void_fptr TIMER_get_timer_fptr(uint8_t timer_id);
extern uint16_t TIMER_get_tic_ms_time(void);
extern void null_action (void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */

#endif /* TIMER_H_ */