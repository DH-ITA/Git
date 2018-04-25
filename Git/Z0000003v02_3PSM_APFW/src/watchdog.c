/**
 ******************************************************************************
 * \file watchdog.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Eos Tech S. de A. de C.V. All rights reserved.</b><br><br>
 * Eos Tech Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup File content title
 * @section WATCH DOG MODULE:
 *
 * This file contains all standard code file functions for UART module.
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - Format: F_P06-DP_13
 *   - Revision: 01
 *   - Date: 1/OCT/2016
 *   - Author: Mariano Centeno Camarena.
 ******************************************************************************
 *
 * @defgroup File change log
 * @section FILE_CHANGE_LOG FILE CHANGE LOG:
 *
 * Top to bottom log: last revision to first revision.
 *
 ******************************************************************************
 * - Revision 01 / Jacob Anaya Candia / 2016-12-09
 *   - Change 1:  PCR#: Description: Initial emission
 *   - Change 2:  PCR#: Description.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "watchdog.h"
#include "rst.h"
#include "wdt.h"
#include "osc.h"
#include "sm.h"

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

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each block of Macros
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

/*
 ******************************************************************************
 * Static Function Prototypes for Private Functions with File Level Scope
 ******************************************************************************
*/
void WDT_Handler(void);
static void WATCHDOG_HAL_vRestart(void);

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
*/

/**
 ******************************************************************************
 * \fn void WATCHDOG_HAL_vInitialize(void)
 * \brief 
 *
 * \param 
 *	Pointer
 * \retval
 ******************************************************************************
 */
void WATCHDOG_HAL_vInitialize(void) {
	
	uint32_t wdt_mode, timeout_value;

	/* Get timeout value. */
	timeout_value = wdt_get_timeout_value(WATCHDOG_TIMEOUT_PERIOD, BOARD_FREQ_SLCK_XTAL);
	
	if (timeout_value == WDT_INVALID_ARGUMENT) {
		while (1) {
			/* Invalid timeout value, error. */
		}
	}
	
	/* Configure WDT to trigger an interrupt (or reset). */
	wdt_mode =	WDT_MR_WDFIEN		|  /* Enable WDT fault interrupt. */
				WDT_MR_WDRPROC		|  /* WDT fault resets processor only. */
				WDT_MR_WDDBGHLT		|  /* WDT stops in debug state. */
				WDT_MR_WDIDLEHLT	;  /* WDT stops in idle state. */
				
	/* Initialize WDT with the given parameters. */
	wdt_init(WDT, wdt_mode, timeout_value, timeout_value);

	/* Configure and enable WDT interrupt. */
	NVIC_DisableIRQ(WDT_IRQn);
	NVIC_ClearPendingIRQ(WDT_IRQn);
	NVIC_SetPriority(WDT_IRQn, 0);
	NVIC_EnableIRQ(WDT_IRQn);
}

/**
 ******************************************************************************
 * \fn void WATCHDOG_APP_vRestart(void)
 * \brief 
 *
 * \param 
 *	Pointer
 * \retval
 ******************************************************************************
 */
void WATCHDOG_APP_vRestart(void) {
	
	WATCHDOG_HAL_vRestart();
}

/**
 ******************************************************************************
 * \fn void WATCHDOG_HAL_vRestart(void)
 * \brief 
 *
 * \param 
 *	Pointer
 * \retval
 ******************************************************************************
 */
void WATCHDOG_HAL_vRestart(void) {
	
	wdt_restart(WDT);
}

/**
 ******************************************************************************
 * \fn uint32_t WATCHDOG_APP_ulGet_status(void)
 * \brief 
 *
 * \param 
 *	Pointer
 * \retval
 ******************************************************************************
 */
uint32_t WATCHDOG_HAL_ulGet_status(void) {
	
	/* Clear status bit to acknowledge interrupt by dummy read. */
	return wdt_get_status(WDT);
}

/**
 ******************************************************************************
 * \fn void WATCHDOG_APP_vHandler(void)
 * \brief 
 *
 * \param 
 *	Pointer
 * \retval
 ******************************************************************************
 */
void WATCHDOG_APP_vHandler(void) {
	
	static uint32_t wtd_status;

	/* Clear status bit to acknowledge interrupt by dummy read. */
	wtd_status = WATCHDOG_HAL_ulGet_status();
	
	/* Restart the WDT counter. */
	WATCHDOG_APP_vRestart();
	
	if ( (wtd_status & RSWDT_SR_WDUNF) == RSWDT_SR_WDUNF )
	{
		Disable_global_interrupt();
		SYSTEM_RESTART_TYPE mcu;
		mcu.restart_cause = WATCH_DOG;
		mcu.save_information = true;
		RST_APP_vExecute_mcu_reset(mcu);
	}
}

/**
 ******************************************************************************
 * \fn void WDT_Handler(void)
 * \brief 
 *
 * \param 
 *	Pointer
 * \retval
 ******************************************************************************
 */
void WDT_Handler(void) {
	
	WATCHDOG_APP_vHandler();
}

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */