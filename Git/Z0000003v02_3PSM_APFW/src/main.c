/**
 ******************************************************************************
 * \file main.c
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
#include "system.h"
#include "target.h"
#include "nic.h"
#include "op.h"
#include "secr.h"
#include "pfail.h"
#include "regs.h"
#include "tou.h"
#include "lp.h"
#include "sm.h"
#include "rtc.h"
#include "timer.h"
#include "led.h"
#include "crc.h"
#include "rst.h"
#include "watchdog.h"

/*
 ******************************************************************************
 * File level pragmas                                                
 ******************************************************************************
 */

/* Application Section CRC */
#define APP_CRC			0xBDF2

/* Place first "  ENDAPP" and then "CRC big endian" */
__attribute__ ((section(".end_of_app"))) const uint8_t end_of_app[11] = 
{
	(const uint8_t)0x80,						// " "
	(const uint8_t)0x20, 						// " "
	(const uint8_t)0x45, 						// "E"
	(const uint8_t)0x90, 						// "N"
	(const uint8_t)0x90, 						// "D"
	(const uint8_t)0x80, 						// "A"
	(const uint8_t)0x80, 						// "P"
	(const uint8_t)0x80, 						// "P"
	(const uint8_t)0x00,						//
	(const uint8_t)( (APP_CRC & 0x00FF) >> 0),	// CRC Low
	(const uint8_t)( (APP_CRC & 0xFF00) >> 8)	// CRC High
};

// Old Code
///* Place first CRC big endian*/
//__attribute__ ((section(".crc_slot"))) const uint16_t app_crc = 0x9262;
//
///* Place second end of application mark */
//__attribute__ ((section(".end_of_app"))) const char end_of_app[8] = " ENDAPP";

/*
 ******************************************************************************
 * Constant and Macro Definitions using #define
 ******************************************************************************
 */

/**
************************************************************************
* Enumerations and Structures and Typedefs
************************************************************************
*/

/*
 ******************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ******************************************************************************
 */

/**
************************************************************************
* Static Variables and Const Variables With File Level Scope
************************************************************************
*/

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
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName );
void assert_triggered( const char * file, uint32_t line );
void vApplicationMallocFailedHook( void );
static void restart_mode( void );

/**
 ******************************************************************************
 * \fn int main(void)
 * \brief main function
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
int main (void)
{	
	/* System initialize */
	SYSTEM_initialize();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	/* Metrology data refresh task */
	METROLOGY_APP_vInitialize();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	/* OPF task */
	OP_create_task();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	/* OPR task */
	NIC_create_task();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	/* MM task */
	SM_APP_vCreate_task();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	/* Time of Use task */
	TOU_APP_vCreate_task();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	/* Metering task */
	REGS_APP_vCreate_task();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	/* Power Fail task */
	PFAIL_APP_vCreate_task();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	meter.system_state.initialiting = false;
	
    /* Start the RTOS scheduler */
    vTaskStartScheduler();
	
    /* If all is well, the scheduler will now be running, and the following line
    will never be reached.  If the following line does execute, then there was
    insufficient FreeRTOS heap memory available for the idle and/or timer tasks
    to be created.  See the memory management section on the FreeRTOS web site
    for more details. */
    for ( ;; )
    {
    }
}

/**
 ******************************************************************************
 * \fn void vApplicationStackOverflowHook( xTaskHandle *pxTask,	signed char *pcTaskName )
 * \brief Called if stack overflow during execution
 *
 * \param pxTask: Task Function
 * \param pcTaskName: Task Name
 * \retval void
 ******************************************************************************
 */
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
    UNUSED( pxTask );
    UNUSED( pcTaskName );

    /* If the parameters have been corrupted then inspect pxCurrentTCB to
     * identify which task has overflowed its stack.
     */
    for ( ;; )
    {
		LED_turn_on(LED_CYAN);
		RST_HAL_vExecute_software_reset();

		//SYSTEM_RESTART_TYPE mcu;
		//mcu.restart_cause = SYSTEM_FAULT;
		//mcu.fault = STACK_OVERFLOW;
		//mcu.save_information = true;
		//RST_APP_vExecute_mcu_reset(mcu);
    }
}

/**
 ******************************************************************************
 * \fn void assert_triggered( const char * file, uint32_t line )
 * \brief Called if assert triggered during execution
 *
 * \param *file: 
 * \param line:
 * \retval void
 ******************************************************************************
*/
void assert_triggered( const char * file, uint32_t line )
{
    UNUSED( file );
    UNUSED( line );

    for ( ;; )
    {
		LED_turn_on(LED_CYAN);
		RST_HAL_vExecute_software_reset();

		//SYSTEM_RESTART_TYPE mcu;
		//mcu.restart_cause = SYSTEM_FAULT;
		//mcu.fault = ASSERT_TRIGGERED;
		//mcu.save_information = true;
		//RST_APP_vExecute_mcu_reset(mcu);
    }
}

/**
 ******************************************************************************
 * \fn void vApplicationMallocFailedHook( void )
 * \brief Malloc fail call function
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    for ( ;; )
    {
        while ( 1 );
    }
}

/**
 ******************************************************************************
 * \fn void restart_mode( void )
 * \brief Initiate reinitialization process
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
static void restart_mode( void )
{
    /* Start software reset */
    rstc_start_software_reset( RSTC );
}

/**
 ******************************************************************************
 * \fn void RSTC_Handler( void )
 * \brief reset interrupt process
 *
 * \param void
 * \retval void
 *
 ******************************************************************************
 */
void RSTC_Handler( void )
{
	if ( RSTC_WATCHDOG_RESET <= rstc_get_reset_cause( RSTC ) )
	{
		restart_mode();
	}
}

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-08-13
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */