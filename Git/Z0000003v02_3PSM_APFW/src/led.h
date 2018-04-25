/**
 ******************************************************************************
 * \file led.h
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
#ifndef LED_H_
#define LED_H_

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "target.h"
   
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/
#define LED_ON_PERIOD_FAIL				TIMER_ms_to_tic(1000)
#define LED_ON_PERIOD					TIMER_ms_to_tic(50)
#define LED_OFF_PERIOD					TIMER_ms_to_tic(900)

#define LED_CHECK_ST_3_ERROR_FLAGS		( DEMAND_OVERLOAD_FLAG_bm | REVERSE_ROTATION_FLAG_bm )

/*===========================================================================*\
 *  LED i/o Port definition
\*===========================================================================*/
#define LED_R_IO					IOPORT_CREATE_PIN(PIOA, 2)
#define LED_G_IO 					IOPORT_CREATE_PIN(PIOA, 1)
#define LED_B_IO					IOPORT_CREATE_PIN(PIOB, 8) 

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs
 ******************************************************************************
*/
/*
 *-----------------------------------------------------------------------------
 * LED color enumeration
 *-----------------------------------------------------------------------------
 */
typedef enum led_colors
{
	LED_RED = 0,
	LED_GREEN,
	LED_BLUE,
	LED_YELLOW,
	LED_CYAN,
	LED_MAGENTA,
	LED_WHITE,
	LED_ALL
} LED_COLORS;

typedef struct led_type
{
	uint8_t led_state;
} LED_TYPE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
extern LED_TYPE led;

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/
extern void LED_io_initialize(void);
extern void LED_io_configure(void);
extern void LED_initialize(void);
extern void LED_turn_on(uint8_t led_color);
extern void LED_turn_off(uint8_t led_color);
extern void LED_start_diagnosis_timer(void);
extern void LED_test_rgb(void);
extern void LED_diagnosis_led_task(void);
extern void LED_stop_diagnosis_task(void);
extern void LED_turn_off_all_delay_diagnosis_task(void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
 #endif /* LED_H_ */
