/**
 ******************************************************************************
 * \file pfail.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Softek Global eDesign S. de R.L. de C.V. All rights reserved.</b><br><br>
 * Softek Global eDesign Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup Main Module Control library
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all the power fail functions for mm100 modules using ATSAM4CMP16C.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 *   - mm - Medidor Modular.
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *	 -
 *
 *   - Requirements Document(s):
 *     - CFE: Especificacion CFE G0100-05. Marzo 2010.
 *
 *   - Applicable Standards:
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - Format: F-P06-DS-03
 *   - Revision: 01
 *   - Date: 2013-06-22
 *   - Author: Mariano Centeno Camarena.
 ******************************************************************************
 */

 /**
 ************************************************************************
 * Include files
 ************************************************************************
 */
#include "pfail.h"
#include "regs.h"
#include "evnt.h"
#include "sm.h"
#include "led.h"
#include "system.h"
#include "op.h"
#include "nic.h"
#include "tou.h"
#include "timer.h"
#include "lp.h"
#include "rst.h"
#include "watchdog.h"

/**
************************************************************************
*  File level pragmas
************************************************************************
*/

/**
************************************************************************
* Constant and Macro Definitions using #define
************************************************************************
*/

/**
************************************************************************
* Enumerations and Structures and Typedefs
************************************************************************
*/

/**
************************************************************************
* Global and Const Variable Defining Definitions / Initializations
************************************************************************
*/

/**
************************************************************************
* Static Variables and Const Variables With File Level Scope
************************************************************************
*/
xTaskHandle pfail_os_task_id;
static bool executing_pfail_task;
static volatile uint32_t tick_ms_timer;
static MS_TIMER_TYPE ms_timers[NUMBER_OF_PF_MS_TIMERS];

/**
************************************************************************
* ROM Const Variables With File Level Scope
************************************************************************
*/

/**
************************************************************************
* Function Prototypes for Private Functions with File Level Scope
************************************************************************
*/
static void PFAIL_APP_vRtos_task(void *pvParameters);

static void PFAIL_APP_vStart_ms_timer(PF_MS_TIMER_ID_TYPE timer_id, uint32_t period, void_fptr function_ptr);
static bool PFAIL_APP_bIs_ms_timer_running(PF_MS_TIMER_ID_TYPE timer_id);
static bool PFAIL_APP_bIs_ms_timer_expired(PF_MS_TIMER_ID_TYPE timer_id);
static void PFAIL_APP_vReset_ms_timer(PF_MS_TIMER_ID_TYPE timer_id);
static uint16_t PFAIL_APP_vGet_pending_ms_time(PF_MS_TIMER_ID_TYPE timer_id);
static void_fptr PFAIL_APP_vGet_timer_fptr(uint8_t timer_id);
static uint16_t PFAIL_APP_ucGet_tic_ms_time(void);
static void PFAIL_APP_vNull_action(void);

static void PFAIL_APP_vPower_failure_handler(void);
static void PFAIL_APP_vPhase_A_failure_handler(void);
static void PFAIL_APP_vPhase_B_failure_handler(void);
static void PFAIL_APP_vPhase_C_failure_handler(void);

static void PFAIL_APP_vReset_zc_total_detector(void);
static void PFAIL_APP_vReset_zc_pha_detector(void);
static void PFAIL_APP_vReset_zc_phb_detector(void);
static void PFAIL_APP_vReset_zc_phc_detector(void);

/**
************************************************************************
* Function Definitions
************************************************************************
*/

/**
 ******************************************************************************
 * \fn void PFAIL_APP_vInitialize(void)
 * \brief Initialize power fail module
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void PFAIL_APP_vInitialize(void) {
	
	executing_pfail_task = false;
	meter.power_failure = true;
	meter.phase_a.delayed_failure = true;
	meter.phase_a.failure = true;
	meter.phase_b.delayed_failure = true;
	meter.phase_b.failure = true;
	meter.phase_c.delayed_failure = true;
	meter.phase_c.failure = true;

	supc_set_monitor_sampling_period(SUPC, SUPC_SMMR_SMSMPL_CSM);
	supc_set_monitor_threshold( SUPC, SUPC_SMMR_SMTH(PFAIL_TH_RESET_2_2V) );
	supc_enable_monitor_reset(SUPC);	
	supc_enable_brownout_reset(SUPC);

	/* Enable RTT interrupt */
	uint32_t ul_previous_time;
	RTT->RTT_MR = 34;
	ul_previous_time = rtt_read_timer_value(RTT);
	while (ul_previous_time == rtt_read_timer_value(RTT));

	NVIC_DisableIRQ(RTT_IRQn);
	NVIC_ClearPendingIRQ(RTT_IRQn);
	NVIC_SetPriority(RTT_IRQn, 0);
	NVIC_EnableIRQ(RTT_IRQn);
	rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN | RTT_MR_ALMIEN);

	/* Configure ZC interrupt */
	pio_configure_pin(PFAIL_ZC_PIN, PIO_INPUT | PIO_DEBOUNCE | PIO_IT_FALL_EDGE);
	pio_configure_interrupt(PFAIL_ZC_PIO, PFAIL_ZC_PIN_BIT_MASK, PIO_IT_FALL_EDGE);
	pio_enable_interrupt( PFAIL_ZC_PIO, PFAIL_ZC_PIN_BIT_MASK );

	NVIC_DisableIRQ( PFAIL_ZC_IRQn );
	NVIC_ClearPendingIRQ( PFAIL_ZC_IRQn );
	NVIC_SetPriority( PFAIL_ZC_IRQn, 0 );
	NVIC_EnableIRQ( PFAIL_ZC_IRQn );
	
	/* Configure ZC1, ZC2 and ZC3 interrupt */
	pio_configure_pin(PFAIL_ZC_PHA_PIN, PIO_INPUT | PIO_DEBOUNCE | PIO_IT_FALL_EDGE);
	pio_configure_pin(PFAIL_ZC_PHB_PIN, PIO_INPUT | PIO_DEBOUNCE | PIO_IT_FALL_EDGE);
	pio_configure_pin(PFAIL_ZC_PHC_PIN, PIO_INPUT | PIO_DEBOUNCE | PIO_IT_FALL_EDGE);
	pio_configure_interrupt(PFAIL_ZC_PHA_PHB_PHC_PIO, PFAIL_ZC_PHA_PHB_PHC_PIN_BIT_MASK, PIO_IT_FALL_EDGE);
	pio_enable_interrupt( PFAIL_ZC_PHA_PHB_PHC_PIO, PFAIL_ZC_PHA_PHB_PHC_PIN_BIT_MASK );

	NVIC_DisableIRQ( PFAIL_ZC_PHA_PHB_PHC_IRQn );
	NVIC_ClearPendingIRQ( PFAIL_ZC_PHA_PHB_PHC_IRQn );
	NVIC_SetPriority( PFAIL_ZC_PHA_PHB_PHC_IRQn, 0 );
	NVIC_EnableIRQ( PFAIL_ZC_PHA_PHB_PHC_IRQn );

	/* Check for a real power up */
	PFAIL_APP_vStart_ms_timer(PFAIL_POWER_BACK_TIMER, TIMER_ms_to_tic(1000), null_action);

	while( ( PFAIL_APP_bIs_ms_timer_expired(PFAIL_POWER_BACK_TIMER) == false ) && (meter.power_failure == true) ) {
		//WATCHDOG_APP_vRestart();
	}

	meter.phase_a.delayed_failure = meter.phase_a.failure;
	meter.phase_b.delayed_failure = meter.phase_b.failure;
	meter.phase_c.delayed_failure = meter.phase_c.failure;
}

/**
 ******************************************************************************
 * \fn void PFAIL_APP_vCreate_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void PFAIL_APP_vCreate_task(void) {
	
	/* Create Power Fail Task */
	xTaskCreate( PFAIL_APP_vRtos_task, (const signed char * const ) "PFAIL_APP_vTask", PFAIL_STACK_SIZE, NULL, PFAIL_APP_vTask_PRIO, &pfail_os_task_id );
}

/**
 ******************************************************************************
 * \fn void PFAIL_APP_vRtos_task(void *pvParameters)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void PFAIL_APP_vRtos_task(void *pvParameters) {

	UNUSED( pvParameters );
	
	/* Power Failure Module task Loop. */
	for ( ; ; ) {

		if (!executing_pfail_task) {

			PFAIL_APP_vTask();
		}
		
		/* Suspend OP Task */
		vTaskSuspend(pfail_os_task_id);
	}
	
	/* Should never go there */
	vTaskDelete(NULL);
}

/**
 ******************************************************************************
 * \fn void PFAIL_APP_vTask(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void PFAIL_APP_vTask(void) {
	
	if (!executing_pfail_task) {

		executing_pfail_task = true;

		if ( meter.power_failure ) {

			if ( (xTaskGetSchedulerState() != taskSCHEDULER_RUNNING) || (meter.system_state.initialiting) ) {
				
				RST_HAL_vExecute_software_reset();
			}
			else {
				
				REGS_APP_vSuspend_storage_rtos_task();
				TOU_APP_vSuspend_rtos_task();

				/* Wait for 3 seconds to see if power comes back */
				while ( (PFAIL_APP_bIs_ms_timer_expired(PFAIL_POWER_BACK_TIMER) == FALSE) && (meter.power_failure == true) ) {
					
					WATCHDOG_APP_vRestart();
				}
				
				/* Check for a real power back */
				if ( meter.power_failure == false ) {
					
					LED_diagnosis_led_task();
					REGS_APP_vResume_storage_rtos_task();
					TOU_APP_vResume_rtos_task();

					Enable_global_interrupt();
				}
				else {
					
					Disable_global_interrupt();
					
					PFAIL_APP_vRegister_power_down();
					PFAIL_APP_v3ph_failure_detection();
					RST_APP_vSave_information();

					while ( meter.power_failure );

					RST_HAL_vExecute_software_reset();
				}
			}
		}
		executing_pfail_task = false;
	}
}

/**
 ******************************************************************************
 * \fn void PFAIL_APP_vStart_ms_timer(PF_MS_TIMER_ID_TYPE timer_id, uint16_t period, void_fptr function_ptr)
 * \brief 
 *
 * \param
 * \retval
 ******************************************************************************
 */
void PFAIL_APP_vStart_ms_timer(PF_MS_TIMER_ID_TYPE timer_id, uint32_t period, void_fptr function_ptr)
{
	if (NUMBER_OF_PF_MS_TIMERS > timer_id)
	{
		Disable_global_interrupt();
		
		ms_timers[timer_id].running = true;
		ms_timers[timer_id].expired = false;
		ms_timers[timer_id].timer = tick_ms_timer + period;
		ms_timers[timer_id].fptr = function_ptr;
		
		if ( period == 0x0000 )
		{
			ms_timers[timer_id].running = false;
			ms_timers[timer_id].expired = true;
			(*ms_timers[timer_id].fptr)();		//execute the desired call back function
		}
		
		Enable_global_interrupt();
	}
}

/**
 ******************************************************************************
 * \fn bool PFAIL_APP_bIs_ms_timer_running(PF_MS_TIMER_ID_TYPE timer_id)
 * \brief 
 *
 * \param
 * \retval
 ******************************************************************************
 */
bool PFAIL_APP_bIs_ms_timer_running(PF_MS_TIMER_ID_TYPE timer_id)
{
	return ms_timers[timer_id].running;
}

/**
 ******************************************************************************
 * \fn bool PFAIL_APP_bIs_ms_timer_expired(PF_MS_TIMER_ID_TYPE timer_id)
 * \brief 
 *
 * \param
 * \retval
 ******************************************************************************
 */ 
bool PFAIL_APP_bIs_ms_timer_expired(PF_MS_TIMER_ID_TYPE timer_id)
{
	return ms_timers[timer_id].expired;
}

/**
 ******************************************************************************
 * \fn void PFAIL_APP_vReset_ms_timer(PF_MS_TIMER_ID_TYPE timer_id)
 * \brief 
 *
 * \param
 * \retval
 ******************************************************************************
 */
void PFAIL_APP_vReset_ms_timer(PF_MS_TIMER_ID_TYPE timer_id)
{
	Disable_global_interrupt();
	ms_timers[timer_id].running = false;
	ms_timers[timer_id].expired = false;
	ms_timers[timer_id].timer = 0;
	ms_timers[timer_id].fptr = PFAIL_APP_vNull_action;
	Enable_global_interrupt();
}

/**
 ******************************************************************************
 * \fn uint16_t PFAIL_APP_vGet_pending_ms_time(PF_MS_TIMER_ID_TYPE timer_id)
 * \brief returns the pending time of a timer
 *
 * \param
 * \retval
 ******************************************************************************
 */
uint16_t PFAIL_APP_vGet_pending_ms_time(PF_MS_TIMER_ID_TYPE timer_id)
{
  uint16_t pending_time = 0;

  if (NUMBER_OF_PF_MS_TIMERS > timer_id)
  {
      if ( tick_ms_timer < ms_timers[timer_id].timer)
      {
          pending_time = ms_timers[timer_id].timer - tick_ms_timer;
      }
  }
  return (pending_time);
}

/**
 ******************************************************************************
 * \fn void_fptr PFAIL_APP_vGet_timer_fptr(uint8_t timer_id)
 * \brief
 *
 * \param	None
 * \retval	None
 ******************************************************************************
 */
void_fptr PFAIL_APP_vGet_timer_fptr(uint8_t timer_id)
{
	void_fptr fptr = NULL;
	
	if (NUMBER_OF_PF_MS_TIMERS > timer_id)
	{
		fptr = ms_timers[timer_id].fptr;
	}
	return fptr;
}

/**
 ******************************************************************************
 * \fn uint16_t PFAIL_APP_ucGet_tic_ms_time(void)
 * \brief Returns the current tick time
 *
 * \param
 * \retval
 ******************************************************************************
 */
uint16_t PFAIL_APP_ucGet_tic_ms_time(void)
{
	return tick_ms_timer;
}

/**
 ******************************************************************************
 * \fn void PFAIL_APP_vNull_action (void)
 * \brief Null action routine
 *
 * \param	None
 * \retval	None
 ******************************************************************************
 */
void PFAIL_APP_vNull_action(void) {

}

/**
 ******************************************************************************
 * \fn void PFAIL_APP_vPower_failure_handler(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void PFAIL_APP_vPower_failure_handler(void) {

	meter.power_failure = true;
	
	if ( (meter.phase_a.failure == true) && (meter.phase_b.failure == true) && (meter.phase_c.failure == true) ) {
	
		NIC_APP_vAssert_power_fail();

		if ( executing_pfail_task == false ) {
			
			PFAIL_APP_vStart_ms_timer(PFAIL_POWER_BACK_TIMER, TIMER_ms_to_tic(PFAIL_MIN_POWER_FAILURE_PERIOD), null_action);
		
			TIMER_reset_ms_timer(LED_BLINKING_TIMER);
			LED_turn_off(LED_ALL);
			
			portBASE_TYPE xYieldRequired;
			xYieldRequired = xTaskResumeFromISR(pfail_os_task_id);
			
			if ( xYieldRequired == pdTRUE ) {
				portYIELD();
			}
		}
	}
}

/**
 ******************************************************************************
 * \fn void PFAIL_APP_vPhase_A_failure_handler(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void PFAIL_APP_vPhase_A_failure_handler(void) {

	meter.phase_a.failure = true;

	PFAIL_APP_vReset_ms_timer(PFAIL_PHA_RESTORATION_TIMER);

	if ( !PFAIL_APP_bIs_ms_timer_running(PFAIL_PHA_FAILURE_TIMER) ) {
		PFAIL_APP_vStart_ms_timer(PFAIL_PHA_FAILURE_TIMER, TIMER_ms_to_tic(PFAIL_MIN_POWER_FAILURE_PERIOD), null_action);
	}

	if ( (meter.phase_b.failure == true) && (meter.phase_c.failure == true) ) {
		
		meter.power_failure = true;
		NIC_APP_vAssert_power_fail();

		if ( executing_pfail_task == false ) {
			
			PFAIL_APP_vStart_ms_timer(PFAIL_POWER_BACK_TIMER, TIMER_ms_to_tic(PFAIL_MIN_POWER_FAILURE_PERIOD), null_action);
			
			TIMER_reset_ms_timer(LED_BLINKING_TIMER);
			LED_turn_off(LED_ALL);
			
			portBASE_TYPE xYieldRequired;
			xYieldRequired = xTaskResumeFromISR(pfail_os_task_id);
			
			if ( xYieldRequired == pdTRUE ) {
				portYIELD();
			}
		}
	}
}

/**
 ******************************************************************************
 * \fn void PFAIL_APP_vPhase_B_failure_handler(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void PFAIL_APP_vPhase_B_failure_handler(void) {
	
	meter.phase_b.failure = true;

	PFAIL_APP_vReset_ms_timer(PFAIL_PHB_RESTORATION_TIMER);

	if ( !PFAIL_APP_bIs_ms_timer_running(PFAIL_PHB_FAILURE_TIMER) ) {
		PFAIL_APP_vStart_ms_timer(PFAIL_PHB_FAILURE_TIMER, TIMER_ms_to_tic(PFAIL_MIN_POWER_FAILURE_PERIOD), null_action);
	}

	if ( (meter.phase_a.failure == true) && (meter.phase_c.failure == true) ) {

		meter.power_failure = true;
		NIC_APP_vAssert_power_fail();
		
		if ( executing_pfail_task == false ) {
			
			PFAIL_APP_vStart_ms_timer(PFAIL_POWER_BACK_TIMER, TIMER_ms_to_tic(PFAIL_MIN_POWER_FAILURE_PERIOD), null_action);
			
			TIMER_reset_ms_timer(LED_BLINKING_TIMER);
			LED_turn_off(LED_ALL);
			
			portBASE_TYPE xYieldRequired;
			xYieldRequired = xTaskResumeFromISR(pfail_os_task_id);
			
			if ( xYieldRequired == pdTRUE ) {
				portYIELD();
			}
		}
	}
}

/**
 ******************************************************************************
 * \fn void PFAIL_APP_vPhase_C_failure_handler(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void PFAIL_APP_vPhase_C_failure_handler(void) {

	meter.phase_c.failure = true;

	PFAIL_APP_vReset_ms_timer(PFAIL_PHC_RESTORATION_TIMER);

	if ( !PFAIL_APP_bIs_ms_timer_running(PFAIL_PHC_FAILURE_TIMER) ) {
		PFAIL_APP_vStart_ms_timer(PFAIL_PHC_FAILURE_TIMER, TIMER_ms_to_tic(PFAIL_MIN_POWER_FAILURE_PERIOD), null_action);
	}

	if ( (meter.phase_a.failure == true) && (meter.phase_b.failure == true) ) {

		meter.power_failure = true;
		NIC_APP_vAssert_power_fail();
		
		if ( executing_pfail_task == false ) {
			
			PFAIL_APP_vStart_ms_timer(PFAIL_POWER_BACK_TIMER, TIMER_ms_to_tic(PFAIL_MIN_POWER_FAILURE_PERIOD), null_action);
			
			TIMER_reset_ms_timer(LED_BLINKING_TIMER);
			LED_turn_off(LED_ALL);
			
			portBASE_TYPE xYieldRequired;
			xYieldRequired = xTaskResumeFromISR(pfail_os_task_id);
			
			if ( xYieldRequired == pdTRUE ) {
				portYIELD();
			}
		}
	}
}

/**
******************************************************************************
* \fn void PFAIL_APP_vPower_up(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void PFAIL_APP_vRegister_power_up(void) {
	
	EVNT_APP_vRegister_event((uint8_t)PRIMARY_POWER_UP, 0, true, END_DEVICE, rtc.utc_timestamp);

	if ( !meter.phase_a.failure ) {
		EVNT_APP_vRegister_event((uint8_t)PHASE_A_RESTORATION, 0, false, END_DEVICE, rtc.utc_timestamp);
	}

	if ( !meter.phase_b.failure ) {
		EVNT_APP_vRegister_event((uint8_t)PHASE_B_RESTORATION, 0, false, END_DEVICE, rtc.utc_timestamp);
	}

	if ( !meter.phase_c.failure ) {
		EVNT_APP_vRegister_event((uint8_t)PHASE_C_RESTORATION, 0, false, END_DEVICE, rtc.utc_timestamp);
	}
}

/**
******************************************************************************
* \fn void PFAIL_APP_vRegister_power_down(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void PFAIL_APP_vRegister_power_down(void) {
	
	uint32_t nbr_power_outages = 0;
	
	EEPROM_reader(MT_3_NBR_POWER_OUTAGES_ADDR, &nbr_power_outages, 4);
	
	if (nbr_power_outages < 0xFFFFFFFF) {
		nbr_power_outages++;
	}
	else {
		nbr_power_outages = 1;
	}
	
	EEPROM_writer( MT_3_NBR_POWER_OUTAGES_ADDR, &nbr_power_outages, 4 );
	EEPROM_writer( MT_4_MCU_RESET_POWER_FAILURE_ADDR, &nbr_power_outages, 2 );
	
	EVNT_APP_vRegister_event(PRIMARY_POWER_DOWN, 0, true, END_DEVICE, rtc.utc_timestamp);
	GRAL_APP_vSet_flag_st_3_ed_std_status1(POWER_FAILURE_FLAG);
}

/**
******************************************************************************
* \fn void PFAIL_APP_vReset_zc_total_detector(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void PFAIL_APP_vReset_zc_total_detector(void) {
	
	PFAIL_APP_vReset_ms_timer( PFAIL_ZC_TIMER );
	meter.power_failure = false;
	NIC_APP_vDeassert_power_fail();
	PFAIL_APP_vStart_ms_timer( PFAIL_ZC_TIMER, (uint32_t)TIMER_ms_to_tic(100), PFAIL_APP_vPower_failure_handler );
}

/**
******************************************************************************
* \fn void PFAIL_APP_vReset_zc_pha_detector(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void PFAIL_APP_vReset_zc_pha_detector(void) {
	
	PFAIL_APP_vReset_ms_timer(PFAIL_PHA_ZC_TIMER);
	PFAIL_APP_vReset_ms_timer(PFAIL_PHA_FAILURE_TIMER);

	if ( meter.phase_a.delayed_failure && (!meter.system_state.initialiting) ) {

		if ( !PFAIL_APP_bIs_ms_timer_running(PFAIL_PHA_RESTORATION_TIMER) ) {
			PFAIL_APP_vStart_ms_timer(PFAIL_PHA_RESTORATION_TIMER, TIMER_ms_to_tic(PFAIL_MIN_RESTORATION_PERIOD), null_action);
		}
	}

	meter.power_failure = false;
	meter.phase_a.failure = false;
	NIC_APP_vDeassert_power_fail();

	PFAIL_APP_vStart_ms_timer( PFAIL_PHA_ZC_TIMER, (uint32_t)TIMER_ms_to_tic(100), PFAIL_APP_vPhase_A_failure_handler );
}


/**
******************************************************************************
* \fn void PFAIL_APP_vReset_zc_phb_detector(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void PFAIL_APP_vReset_zc_phb_detector(void) {
	
	PFAIL_APP_vReset_ms_timer(PFAIL_PHB_ZC_TIMER);
	PFAIL_APP_vReset_ms_timer(PFAIL_PHB_FAILURE_TIMER);

	if ( meter.phase_b.delayed_failure && (!meter.system_state.initialiting) ) {

		if ( !PFAIL_APP_bIs_ms_timer_running(PFAIL_PHB_RESTORATION_TIMER) ) {
			PFAIL_APP_vStart_ms_timer(PFAIL_PHB_RESTORATION_TIMER, TIMER_ms_to_tic(PFAIL_MIN_RESTORATION_PERIOD), null_action);
		}
	}

	meter.power_failure = false;
	meter.phase_b.failure = false;
	NIC_APP_vDeassert_power_fail();

	PFAIL_APP_vStart_ms_timer( PFAIL_PHB_ZC_TIMER, (uint32_t)TIMER_ms_to_tic(100), PFAIL_APP_vPhase_B_failure_handler );
}

/**
******************************************************************************
* \fn void PFAIL_APP_vReset_zc_phc_detector(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void PFAIL_APP_vReset_zc_phc_detector(void) {

	PFAIL_APP_vReset_ms_timer(PFAIL_PHC_ZC_TIMER);
	PFAIL_APP_vReset_ms_timer(PFAIL_PHC_FAILURE_TIMER);
	
	if ( meter.phase_c.delayed_failure && (!meter.system_state.initialiting) ) {

		if ( !PFAIL_APP_bIs_ms_timer_running(PFAIL_PHC_RESTORATION_TIMER) ) {
			PFAIL_APP_vStart_ms_timer(PFAIL_PHC_RESTORATION_TIMER, TIMER_ms_to_tic(PFAIL_MIN_RESTORATION_PERIOD), null_action);
		}
	}

	meter.power_failure = false;
	meter.phase_c.failure = false;
	NIC_APP_vDeassert_power_fail();

	PFAIL_APP_vStart_ms_timer( PFAIL_PHC_ZC_TIMER, (uint32_t)TIMER_ms_to_tic(100), PFAIL_APP_vPhase_C_failure_handler );
}

/**
 ******************************************************************************
 * \fn void PIOA_Handler(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void PIOA_Handler(void) {

	uint32_t zc_status;

	zc_status = pio_get_interrupt_status(PFAIL_ZC_PIO);
	
	if (zc_status & PFAIL_ZC_PIN_BIT_MASK) {
	
		PFAIL_APP_vReset_zc_total_detector();
	}
}

/**
 ******************************************************************************
 * \fn void PIOB_Handler(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void PIOB_Handler(void) {

	volatile uint32_t zc_abc_status = pio_get_interrupt_status(PFAIL_ZC_PHA_PHB_PHC_PIO);

	if (zc_abc_status & PFAIL_ZC_PHA_PIN_BIT_MASK) {

		PFAIL_APP_vReset_zc_pha_detector();
	}

	if (zc_abc_status & PFAIL_ZC_PHB_PIN_BIT_MASK) {

		PFAIL_APP_vReset_zc_phb_detector();
	}

	if (zc_abc_status & PFAIL_ZC_PHC_PIN_BIT_MASK) {

		PFAIL_APP_vReset_zc_phc_detector();
	}
}

/**
 ******************************************************************************
 * \fn void PFAIL_APP_v3ph_failure_detection(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void PFAIL_APP_v3ph_failure_detection(void) {

	// The time necessary to consider Phase A power failure has elapsed
	if ( PFAIL_APP_bIs_ms_timer_expired(PFAIL_PHA_FAILURE_TIMER) ) {

		meter.phase_a.delayed_failure = true;
		EVNT_APP_vRegister_event((uint8_t)PHASE_A_FAILURE, 0, false, END_DEVICE, rtc.utc_timestamp);
		PFAIL_APP_vReset_ms_timer(PFAIL_PHA_FAILURE_TIMER);
	}

	// The time necessary to consider Phase B power failure has elapsed
	if ( PFAIL_APP_bIs_ms_timer_expired(PFAIL_PHB_FAILURE_TIMER) ) {

		meter.phase_b.delayed_failure = true;
		EVNT_APP_vRegister_event((uint8_t)PHASE_B_FAILURE, 0, false, END_DEVICE, rtc.utc_timestamp);
		PFAIL_APP_vReset_ms_timer(PFAIL_PHB_FAILURE_TIMER);
	}

	// The time necessary to consider Phase C power failure has elapsed
	if ( PFAIL_APP_bIs_ms_timer_expired(PFAIL_PHC_FAILURE_TIMER) ) {

		meter.phase_c.delayed_failure = true;
		EVNT_APP_vRegister_event((uint8_t)PHASE_C_FAILURE, 0, false, END_DEVICE, rtc.utc_timestamp);
		PFAIL_APP_vReset_ms_timer(PFAIL_PHC_FAILURE_TIMER);
	}

	// The time necessary to consider Phase A restoration has elapsed
	if ( PFAIL_APP_bIs_ms_timer_expired(PFAIL_PHA_RESTORATION_TIMER) ) {

		meter.phase_a.delayed_failure = false;
		EVNT_APP_vRegister_event((uint8_t)PHASE_A_RESTORATION, 0, false, END_DEVICE, rtc.utc_timestamp);
		PFAIL_APP_vReset_ms_timer(PFAIL_PHA_RESTORATION_TIMER);
	}

	// The time necessary to consider Phase B restoration has elapsed
	if ( PFAIL_APP_bIs_ms_timer_expired(PFAIL_PHB_RESTORATION_TIMER) ) {

		meter.phase_b.delayed_failure = false;
		EVNT_APP_vRegister_event((uint8_t)PHASE_B_RESTORATION, 0, false, END_DEVICE, rtc.utc_timestamp);
		PFAIL_APP_vReset_ms_timer(PFAIL_PHB_RESTORATION_TIMER);
	}

	// The time necessary to consider Phase C restoration has elapsed
	if ( PFAIL_APP_bIs_ms_timer_expired(PFAIL_PHC_RESTORATION_TIMER) ) {

		meter.phase_c.delayed_failure = false;
		EVNT_APP_vRegister_event((uint8_t)PHASE_C_RESTORATION, 0, false, END_DEVICE, rtc.utc_timestamp);
		PFAIL_APP_vReset_ms_timer(PFAIL_PHC_RESTORATION_TIMER);
	}
}

/**
 ******************************************************************************
 * \fn void RTT_Handler(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void RTT_Handler(void)
{
	uint32_t ul_status;

	/* Get RTT status */
	ul_status = rtt_get_status(RTT);

	/* Real Time Timer Tick Increment */
	if ((ul_status & RTT_SR_RTTINC) == RTT_SR_RTTINC) {
		
		uint8_t i;
		
		tick_ms_timer++;
		
		for (i = 0; i < NUMBER_OF_PF_MS_TIMERS; i++)
		{
			if ( ms_timers[i].running == true )
			{
				if ( tick_ms_timer == ms_timers[i].timer )
				{
					ms_timers[i].running = false;
					ms_timers[i].expired = true;
					(*ms_timers[i].fptr)();		//execute the desired call back function
				}
			}
		}
	}

	/* Alarm */
	if ((ul_status & RTT_SR_ALMS) == RTT_SR_ALMS) {
		
	}
}

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY
 * ----------------------------------------------------------------------------
 * - Revision 04 / Jacob Anaya Candia / 2017-10-13
 *   - Change 3:  Description.
 *   - Change 2:  Description.
 *   - Change 1:  Description.
  ******************************************************************************
  */