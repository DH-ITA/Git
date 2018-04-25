/**
 ******************************************************************************
 * \file watchdog.h
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Eos Tech S. de A. de C.V. All rights reserved.</b><br><br>
 * Eos Tech Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup File content title
 * @section DESCRIPTION DESCRIPTION:
 *
 * File content description.
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
 *   - Change 1:  PCR#: Description.
 *   - Change 2:  PCR#: Description.
 ******************************************************************************
 */
#ifndef  WATCHDOG_H
#define  WATCHDOG_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
 
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/
#define WATCHDOG_TIMEOUT_PERIOD		(uint32_t) (5 * 1000000)		// the desired period should be calculated in microseconds

/*===========================================================================*\
 * Brief description for each block of constant definitions
\*===========================================================================*/

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each block of Macro definitions
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
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/
extern void WATCHDOG_HAL_vInitialize(void);
extern void WATCHDOG_APP_vRestart(void);

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
 #endif