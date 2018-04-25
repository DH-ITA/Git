/**
 ******************************************************************************
 * \file led.c
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
#include <asf.h>
#include "target.h"
#include "led.h"
#include "timer.h"
#include "sm.h"
#include "gral.h"
#include "watchdog.h"

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

/*
 ******************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ******************************************************************************
 */
LED_TYPE led;

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

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */

/** 
 ******************************************************************************
 * \fn void LED_io_initialize(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_io_initialize(void)
{
	ioport_set_pin_level(LED_R_IO, true);
	ioport_set_pin_level(LED_G_IO, true);
	ioport_set_pin_level(LED_B_IO, true);
}

/** 
 ******************************************************************************
 * \fn void LED_io_configure(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_io_configure(void)
{
	/* RGB LED i/o configuration. */
	ioport_set_pin_dir(LED_R_IO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED_G_IO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED_B_IO, IOPORT_DIR_OUTPUT);
}

/** 
 ******************************************************************************
 * \fn void LED_initialize(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_initialize(void)
{
	LED_io_initialize();
	LED_io_configure();
}

/**
 ******************************************************************************
 * \fn void LED_turn_on(uint8_t led_color)
 * \brief
 *
 * \param uint8_t led_color
 * \retval
 ******************************************************************************
 */
void LED_turn_on(uint8_t led_color)
{
	LED_io_configure();
	LED_turn_off(LED_ALL);
	
	switch (led_color)
	{
		case LED_RED:
			ioport_set_pin_level(LED_R_IO, false);
			break;
		
		case LED_GREEN:
			ioport_set_pin_level(LED_G_IO, false);
			break;
			
		case LED_BLUE:
			ioport_set_pin_level(LED_B_IO, false);
			break;
			
		case LED_YELLOW:
			ioport_set_pin_level(LED_R_IO, false);
			ioport_set_pin_level(LED_G_IO, false);
			break;
		
		case LED_CYAN:
			ioport_set_pin_level(LED_B_IO, false);
			ioport_set_pin_level(LED_G_IO, false);
			break;
			
		case LED_MAGENTA:
			ioport_set_pin_level(LED_B_IO, false);
			ioport_set_pin_level(LED_R_IO, false);
			break;
		
		case LED_WHITE:
			ioport_set_pin_level(LED_R_IO, false);
			ioport_set_pin_level(LED_G_IO, false);
			ioport_set_pin_level(LED_B_IO, false);
			break;
			
		case LED_ALL:
			ioport_set_pin_level(LED_R_IO, false);
			ioport_set_pin_level(LED_G_IO, false);
			ioport_set_pin_level(LED_B_IO, false);
			break;
		
		default:
			break;
	}
}

/**
 ******************************************************************************
 * \fn void LED_turn_off(uint8_t led_color)
 * \brief
 *
 * \param uint8_t led_color
 * \retval void
 ******************************************************************************
 */
void LED_turn_off(uint8_t led_color)
{
	LED_io_configure();
	
	switch (led_color)
	{
		case LED_RED:
			ioport_set_pin_level(LED_R_IO, true);
			break;
		
		case LED_GREEN:
			ioport_set_pin_level(LED_G_IO, true);
			break;
		
		case LED_BLUE:
			ioport_set_pin_level(LED_B_IO, true);
			break;
		
		case LED_YELLOW:
			ioport_set_pin_level(LED_R_IO, true);
			ioport_set_pin_level(LED_G_IO, true);
			break;
		
		case LED_CYAN:
			ioport_set_pin_level(LED_B_IO, true);
			ioport_set_pin_level(LED_G_IO, true);
			break;
		
		case LED_MAGENTA:
			ioport_set_pin_level(LED_B_IO, true);
			ioport_set_pin_level(LED_R_IO, true);
			break;
		
		case LED_WHITE:
			ioport_set_pin_level(LED_R_IO, true);
			ioport_set_pin_level(LED_G_IO, true);
			ioport_set_pin_level(LED_B_IO, true);
			break;
		
		case LED_ALL:
			ioport_set_pin_level(LED_R_IO, true);
			ioport_set_pin_level(LED_G_IO, true);
			ioport_set_pin_level(LED_B_IO, true);
			break;
		
		default:
			break;
	}
}

/**
 ******************************************************************************
 * \fn void LED_test_rgb(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_test_rgb(void)
{	
	LED_turn_on(LED_RED);
	delay_ms(500);	
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();

	LED_turn_on(LED_GREEN);
	delay_ms(500);
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	LED_turn_on(LED_BLUE);
	delay_ms(500);
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	LED_turn_off(LED_ALL);
	delay_ms(500);		
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
}

/**
 ******************************************************************************
 * \fn void LED_diagnosis_led_task(void)
 * \brief LED Diagnosis Task
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_diagnosis_led_task(void)
{	
	if ( meter.system_state.busy ) {
		LED_turn_on(LED_MAGENTA);
		TIMER_start_ms_timer(LED_BLINKING_TIMER, LED_ON_PERIOD, LED_turn_off_all_delay_diagnosis_task);
	}
	else {
		uint8_t ed_mode = GRAL_APP_ucGet_st_3_ed_mode();
		
		if ( (ed_mode & FACTORY_FLAG_bm) == 0 ) {
			
			uint16_t ed_std_status1 = GRAL_APP_usGet_st_3_ed_std_status1();
	
			//if ( (ed_std_status1 & LED_CHECK_ST_3_ERROR_FLAGS) == 0 ) {
				LED_turn_on(LED_MAGENTA);
				TIMER_start_ms_timer(LED_BLINKING_TIMER, LED_ON_PERIOD, LED_turn_off_all_delay_diagnosis_task);
			//}
			//else {
			//	LED_turn_on(LED_BLUE);
			//	TIMER_start_ms_timer(LED_BLINKING_TIMER, LED_ON_PERIOD_FAIL, LED_diagnosis_led_task);
			//}
		}
		else {
			//LED_turn_on(LED_WHITE);
			//TIMER_start_ms_timer(LED_BLINKING_TIMER, LED_ON_PERIOD, LED_turn_off_all_delay_diagnosis_task);
		}
	}
}

/**
 ******************************************************************************
 * \fn void LED_turn_off_all_delay_diagnosis_task(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_turn_off_all_delay_diagnosis_task(void)
{
	LED_turn_off(LED_ALL);
	TIMER_start_ms_timer(LED_BLINKING_TIMER, LED_OFF_PERIOD, LED_diagnosis_led_task);
}

/**
 ******************************************************************************
 * \fn void LED_turn_off_all_delay_diagnosis_task(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_stop_diagnosis_task(void)
{
	TIMER_reset_ms_timer(LED_BLINKING_TIMER);
}

/*
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-08-13
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */