/**
 ******************************************************************************
 * \file rst.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Eos Tech S. de A. de C.V. All rights reserved.</b><br><br>
 * Eos Tech Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup File content title
 * @section RESET MODULE:
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
#include "rst.h"
#include "rstc.h"
#include "sm.h"
#include "regs.h"
#include "tou.h"
#include "led.h"
#include "timer.h"

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
static uint8_t nbr_reset_attemps;

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
 * \fn void RST_APP_vExecute_mcu_reset(SYSTEM_RESTART_TYPE mcu)
 * \brief 
 *
 * \param 
 *	Pointer
 * \retval
 ******************************************************************************
 */
void RST_APP_vExecute_mcu_reset(SYSTEM_RESTART_TYPE mcu) {
	
	uint16_t aux;
	
	if ( meter.power_failure == false )
	{

		TIMER_start_ms_timer(SAVE_INFORMATION_TIMER, TIMER_ms_to_tic(5000), RST_HAL_vExecute_software_reset);
		
		if (mcu.save_information == true) {
			RST_APP_vSave_information();
		}
			
		switch ((uint16_t)mcu.restart_cause) {
			
			case (uint16_t)POWER_FAILURE:
				aux = 0;
				EEPROM_reader( MT_4_MCU_RESET_POWER_FAILURE_ADDR, (uint8_t*) &aux, 2 );
				aux++;
				EEPROM_writer( MT_4_MCU_RESET_POWER_FAILURE_ADDR, (uint8_t*) &aux, 2 );
				break;
				
			case (uint16_t)FIRMWARE_UPDATE:
				aux = 0;
				EEPROM_reader( MT_4_MCU_RESET_FIRMWARE_UPDATE_ADDR, (uint8_t*) &aux, 2 );
				aux++;
				EEPROM_writer( MT_4_MCU_RESET_FIRMWARE_UPDATE_ADDR, (uint8_t*) &aux, 2 );
				break;
				
			case (uint16_t)WATCH_DOG:
				aux = 0;
				EEPROM_reader( MT_4_MCU_RESET_WATCH_DOG_ADDR, (uint8_t*) &aux, 2 );
				aux++;
				EEPROM_writer( MT_4_MCU_RESET_WATCH_DOG_ADDR, (uint8_t*) &aux, 2 );
				break;
				
			case (uint16_t)COLD_START:
				aux = 0;
				EEPROM_reader( MT_4_MCU_RESET_COLD_START_ADDR, (uint8_t*) &aux, 2 );
				aux++;
				EEPROM_writer( MT_4_MCU_RESET_COLD_START_ADDR, (uint8_t*) &aux, 2 );
				break;
				
			case (uint16_t)WARM_START:
				aux = 0;
				EEPROM_reader( MT_4_MCU_RESET_WARM_START_ADDR, (uint8_t*) &aux, 2 );
				aux++;
				EEPROM_writer( MT_4_MCU_RESET_WARM_START_ADDR, (uint8_t*) &aux, 2 );
				break;
				
			case (uint16_t)KT_CONSTANT_CHANGE:
				aux = 0;
				EEPROM_reader( MT_4_MCU_RESET_KT_CONSTANT_CHANGE_ADDR, (uint8_t*) &aux, 2 );
				aux++;
				EEPROM_writer( MT_4_MCU_RESET_KT_CONSTANT_CHANGE_ADDR, (uint8_t*) &aux, 2 );
				break;
				
			case (uint16_t)SYSTEM_FAULT:
			
				switch ((uint16_t)mcu.fault) {
				
					case (uint16_t)VLISTINSERT:
						aux = 0;
						EEPROM_reader( MT_4_MCU_RESET_VLISTINSERT_COUNTER_ADDR, (uint8_t*) &aux, 2 );
						aux++;
						EEPROM_writer( MT_4_MCU_RESET_VLISTINSERT_COUNTER_ADDR, (uint8_t*) &aux, 2 );
						break;
						
					case (uint16_t)DUMMY_HANDLER:
						aux = 0;
						EEPROM_reader( MT_4_MCU_RESET_DUMMY_HANDLER_COUNTER_ADDR, (uint8_t*) &aux, 2 );
						aux++;
						EEPROM_writer( MT_4_MCU_RESET_DUMMY_HANDLER_COUNTER_ADDR, (uint8_t*) &aux, 2 );
						break;
						
					case (uint16_t)ASSERT_TRIGGERED:
						aux = 0;
						EEPROM_reader( MT_4_MCU_RESET_ASSERT_TRIGGERED_COUNTER_ADDR, (uint8_t*) &aux, 2 );
						aux++;
						EEPROM_writer( MT_4_MCU_RESET_ASSERT_TRIGGERED_COUNTER_ADDR, (uint8_t*) &aux, 2 );
						break;
						
					case (uint16_t)STACK_OVERFLOW:
						aux = 0;
						EEPROM_reader( MT_4_MCU_RESET_STACK_OVERFLOW_COUNTER_ADDR, (uint8_t*) &aux, 2 );
						aux++;
						EEPROM_writer( MT_4_MCU_RESET_STACK_OVERFLOW_COUNTER_ADDR, (uint8_t*) &aux, 2 );
						break;
						
					default:
						break;
				}
				break;
				
			default:
				break;
		}
	}

	RST_HAL_vExecute_software_reset();
}

/**
 ******************************************************************************
 * \fn void RST_APP_vSave_information(void)
 * \brief 
 *
 * \param 
 *	Pointer
 * \retval
 ******************************************************************************
 */
void RST_APP_vSave_information(void) {
	
	/* Put below the information that should always be saved */
	
	if ( (!meter.system_state.changing_configuration) && (!meter.system_state.initialiting) ) {
		
		REGS_APP_vStore_summations();
		REGS_APP_vStore_demands();
		LP_APP_vPower_fail_detection();
		TOU_APP_vWrite_eeprom_current_season();
		GRAL_APP_vStore_st_3_ed_std_status1();
		GRAL_APP_vStore_st_3_ed_mfg_status();
	}
}

/**
 ******************************************************************************
 * \fn void RST_HAL_vExecute_mcu_reset(void)
 * \brief 
 *
 * \param 
 *	Pointer
 * \retval
 ******************************************************************************
 */
void RST_HAL_vExecute_software_reset(void) {

	/* Write key password */
	RSTC->RSTC_CPMR = 0xA5000000;
	
	/* Start reset */
	RSTC->RSTC_CR = 0xA500000D;
}

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */