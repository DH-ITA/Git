/**
 ******************************************************************************
 * \file crc.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2015 by Softek Global eDesign S. de R.L. de C.V. All rights reserved.</b><br><br>
 * Softek Global eDesign Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup timer system_init Library
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all standard code file functions for timer Module using ATSAM4CMP core.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 *   - SIMCEGAB - Sistema de Medicion de Energia Electrica Centralizado en Gabinete.
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *     - CFE: Especificacion CFE G0100. Abril 2015.
 *
 *   - Applicable Standards:
 *     - EOS: Communication Protocol for SMCG.
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - Format: F-P06-DS-02
 *   - Revision: 01
 *   - Date: 2015-10-02
 *   - Author: Mariano Centeno Camarena.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include <asf.h>
#include "timer.h"
#include "util.h"

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

/*
 ******************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
 */
static uint8_t ms_timer_flag;
static uint32_t TIMER_tick_ms_timer;

static MS_TIMER_TYPE TIMER_ms_timers[NUMBER_OF_MS_TIMERS];

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
 * \fn void TIMER_initialize(void) ISR( TCC0_OVF_vect )
 * \brief
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void TIMER_initialize(void)
{
	TIMER_io_initialize();
	TIMER_io_configure();
	
	ms_timer_flag = 0;
	TIMER_tick_ms_timer = 0;
	
	for (uint8_t i=0; i < NUMBER_OF_MS_TIMERS; i++)
	{
		TIMER_ms_timers[i].running = false;
		TIMER_ms_timers[i].expired = false;
		TIMER_ms_timers[i].timer = 0;
		TIMER_ms_timers[i].fptr = null_action;
	}
} 

/**
 ******************************************************************************
 * \fn void TIMER_io_initialiaze(void)
 * \brief
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void TIMER_io_initialize(void)
{
	
}

/**
 ******************************************************************************
 * \fn void TIMER_io_configure(void)
 * \brief
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void TIMER_io_configure(void)
{
	/* Setup SysTick Timer for 10 msec interrupts */
	//SysTick_Config(sysclk_get_cpu_hz() / 100);
}

/**
 ******************************************************************************
 * \fn void TIMER_start_ms_timer (MS_TIMER_ID_TYPE timer_id, uint16_t period, void_fptr function_ptr)
 * \brief 
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
void TIMER_start_ms_timer (MS_TIMER_ID_TYPE timer_id, uint32_t period, void_fptr function_ptr)
{
	if (NUMBER_OF_MS_TIMERS > timer_id)
	{
		Disable_global_interrupt();
		
		TIMER_ms_timers[timer_id].running = true;
		TIMER_ms_timers[timer_id].expired = false;
		TIMER_ms_timers[timer_id].timer = TIMER_tick_ms_timer + period;
		TIMER_ms_timers[timer_id].fptr = function_ptr;
		
		if ( period == 0x0000 )
		{
			TIMER_ms_timers[timer_id].running = false;
			TIMER_ms_timers[timer_id].expired = true;
			(*TIMER_ms_timers[timer_id].fptr)();		//execute the desired call back function
		}
		
		Enable_global_interrupt();
	}
}

/**
 ******************************************************************************
 * \fn bool TIMER_is_ms_timer_running(MS_TIMER_ID_TYPE timer_id)
 * \brief 
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
bool TIMER_is_ms_timer_running(MS_TIMER_ID_TYPE timer_id)
{
	return TIMER_ms_timers[timer_id].running;
}

/**
 ******************************************************************************
 * \fn bool TIMER_is_ms_timer_expired(MS_TIMER_ID_TYPE timer_id)
 * \brief 
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */ 
bool TIMER_is_ms_timer_expired(MS_TIMER_ID_TYPE timer_id)
{
	return TIMER_ms_timers[timer_id].expired;
}

/**
 ******************************************************************************
 * \fn void TIMER_reset_ms_timer(MS_TIMER_ID_TYPE timer_id)
 * \brief 
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
void TIMER_reset_ms_timer(MS_TIMER_ID_TYPE timer_id)
{
	Disable_global_interrupt();
	TIMER_ms_timers[timer_id].running = false;
	TIMER_ms_timers[timer_id].expired = false;
	TIMER_ms_timers[timer_id].timer = 0;
	TIMER_ms_timers[timer_id].fptr = null_action;
	Enable_global_interrupt();
	
}

/**
 ******************************************************************************
 * \fn uint16_t TIMER_get_pending_ms_time(MS_TIMER_ID_TYPE timer_id)
 * \brief returns the pending time of a timer
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
uint16_t TIMER_get_pending_ms_time(MS_TIMER_ID_TYPE timer_id)
{
  uint16_t pending_time = 0;

  if (NUMBER_OF_MS_TIMERS > timer_id)
  {
      if ( TIMER_tick_ms_timer < TIMER_ms_timers[timer_id].timer)
      {
          pending_time = TIMER_ms_timers[timer_id].timer - TIMER_tick_ms_timer;
      }
  }
  return(pending_time);
}

/**
 ******************************************************************************
 * \fn void_fptr TIMER_get_timer_fptr(uint8_t timer_id)
 * \brief
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void_fptr TIMER_get_timer_fptr(uint8_t timer_id)
{
	void_fptr fptr = NULL;
	
	if (NUMBER_OF_MS_TIMERS > timer_id)
	{
		fptr = TIMER_ms_timers[timer_id].fptr;
	}
	return fptr;
}

/**
 ******************************************************************************
 * \fn uint16_t TIMER_get_tic_ms_time(void)
 * \brief Returns the current tick time
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
uint16_t TIMER_get_tic_ms_time(void)
{
	return TIMER_tick_ms_timer;
}

/**
 ******************************************************************************
 * \fn void null_action (void)
 * \brief Null action routine
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void null_action (void)
{}
	
/**
 ******************************************************************************
 * \fn void TIMER_ms_tick_handler(void)
 * \brief Tic timer interrupt service routine.
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void TIMER_ms_tick_handler(void)
{
	uint8_t i;
	
	TIMER_tick_ms_timer++;
	
	for (i = 0; i < NUMBER_OF_MS_TIMERS; i++)
	{
		if ( TIMER_ms_timers[i].running == true )
		{
			if ( TIMER_tick_ms_timer == TIMER_ms_timers[i].timer )
			{
				TIMER_ms_timers[i].running = false;
				TIMER_ms_timers[i].expired = true;
				(*TIMER_ms_timers[i].fptr)();		//execute the desired call back function
			}	
		}
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