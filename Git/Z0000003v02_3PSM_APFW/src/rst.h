/**
 ******************************************************************************
 * \file rst.h
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
#ifndef  RST_H
#define  RST_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "compiler.h"
 
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/

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
 * System Faults enumeration
\*===========================================================================*/
typedef enum system_fault_type
{
	VLISTINSERT				= 0,
	DUMMY_HANDLER,	
	ASSERT_TRIGGERED,
	STACK_OVERFLOW,
	NUMBER_OF_SYSTEM_FAULTS	
} SYSTEM_FAULT_TYPE;

/*===========================================================================*\
 * System Restarts Causes enumeration
\*===========================================================================*/
typedef enum system_restarts_causes_type
{
	POWER_FAILURE				= 0,
	FIRMWARE_UPDATE,
	WATCH_DOG,
	COLD_START,
	WARM_START,
	KT_CONSTANT_CHANGE,
	SYSTEM_FAULT,
	NUMBER_OF_SYSTEM_RESTART_CAUSES	
} SYSTEM_RESTARTS_CAUSES_TYPE;

/*===========================================================================*\
 * System Restarts Causes enumeration
\*===========================================================================*/
typedef struct system_restart_type
{
	bool save_information;
	SYSTEM_RESTARTS_CAUSES_TYPE restart_cause;
	SYSTEM_FAULT_TYPE fault;
} SYSTEM_RESTART_TYPE;

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
extern void RST_APP_vSave_information(void);
extern void RST_APP_vExecute_mcu_reset(SYSTEM_RESTART_TYPE system);
extern void RST_HAL_vExecute_software_reset(void);

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
 #endif