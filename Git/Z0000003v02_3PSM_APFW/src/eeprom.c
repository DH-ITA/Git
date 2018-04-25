/**
 ******************************************************************************
 * \file 	eeprom.c
 * \brief	EEPROM Port Library
 * \author 	Mariano Centeno Camarena
 * \version    1
 * \date      	2013-01-02
 *
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Softek Global eDesign S. de R.L. de C.V. All rights reserved.</b><br><br>
 * Softek Global eDesign Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup I2C Port Library
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all standard code file functions for I2C Eeprom Library using ATxmega core.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 *   - SIMCEGAB - Sistema de Medicion de Energia Electrica Centralizado en Gabinete.
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *     - CFE: Especificacion CFE G0100-05. Marzo 2010.
 *
 *   - Applicable Standards:
 *     - Elster: Communication Protocol for Option Boards. Rev B.
 *     - Elster: Third party communication board design specification. Rev D.
 *     - Elster: Communications Interface: Third-Party Accessible Tables and Functions. Rev A.
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - 2013-01-22 - Mariano Centeno Camarena.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "compiler.h"
#include "eeprom.h"
#include "i2c.h"
#include "op.h"
#include "nic.h"
#include "watchdog.h"

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

/*===========================================================================*\
 * Brief description for each Constant/Macro
\*===========================================================================*/

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
static twi_packet_t eeprom_twi_package;

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
 * \fn bool EEPROM_writer(uint32_t address, void* data_pointer, unsigned int data_length)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EEPROM_writer(uint32_t address, void* data_pointer, unsigned int data_length) {

	return I2C_HAL_bEeprom_writer(address, data_pointer, data_length);
}

/**
 ******************************************************************************
 * \fn bool EEPROM_reader(uint32_t address, void* data_pointer, unsigned int data_length)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EEPROM_reader(uint32_t address, void* data_pointer, unsigned int data_length) {

	return I2C_HAL_bEeprom_reader(address, data_pointer, data_length);
}

/**
 ******************************************************************************
 * \fn bool EEPROM_erase_all(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EEPROM_erase_all(void)
{
	uint8_t data[256];
	static uint32_t addr;
	
	memset(&data, 0xFF, (unsigned int)sizeof(data));

	for ( addr = EXT_EEPROM_DATA_START_ADDR; addr <= EXT_EEPROM_DATA_END_ADDR; addr += 256 )
	{
		EEPROM_writer( addr, data, (unsigned int)256 );
		WATCHDOG_APP_vRestart();
		
		NIC_task();
		WATCHDOG_APP_vRestart();
		
		OP_task();
		WATCHDOG_APP_vRestart();

		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();
	}
}

/**
 ******************************************************************************
 * \file eeprom.c
 *
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 *
 * - 2016-08-20		Jacob Anaya Candia
 *   - Created initial version for ATxmega core parts.
 ******************************************************************************
 */

