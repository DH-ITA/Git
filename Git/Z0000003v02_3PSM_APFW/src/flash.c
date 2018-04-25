/**
 ******************************************************************************
 * \file flash.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2016 by Softek Global eDesign S. de R.L. de C.V. All rights reserved.</b><br><br>
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
#include "stdint.h"
#include "target.h"
#include <asf.h>
#include "flash.h"
#include "led.h"
#include "ASF/common/services/delay/delay.h"
#include "sm.h"
#include "c1218.h"
#include "c1219.h"
#include "nic.h"
#include "op.h"
#include "timer.h"
#include "pfail.h"
#include "rst.h"


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
 #define TEST_FLASH_ADDR             ((uint32_t)0x010FF800)
/*===========================================================================*\
 * Brief description for each constant definition
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
static uint8_t flash_read_buffer[256];
static uint8_t ul_page_buffer[256];
static uint32_t  *pul_test_page;// = (uint32_t *) ul_test_page_addr;
static uint32_t ul_test_page_addr = BOOT_ORIGIN_ADDR;
uint32_t ul_idx;
uint8_t uc_key,i;

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
 * \fn void FLASH_initialize(void)
 * \brief Init the flash 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void FLASH_initialize(void)
{      
	pul_test_page = (uint32_t *) ul_test_page_addr;  
    
    /* Initialize flash: 6 wait states for flash writing. */
    flash_init(FLASH_ACCESS_MODE_128, 6);
}
/**
 ******************************************************************************
 * \fn void FLASH_boot_origin(uint8_t origin)
 * \brief Init the flash 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void FLASH_boot_origin(uint8_t origin)
{ 
	flash_lock( (uint32_t *)0x01000000,  (uint32_t *)0x01100000, 0, 0); 
	
		//GET the origin of the RESET	
	pul_test_page  = (uint32_t *) BOOT_ORIGIN_ADDR;
	     
	//pul_test_page = (uint32_t *) ul_test_page_addr;   
    
    flash_unlock(ul_test_page_addr, ul_test_page_addr + IFLASH_PAGE_SIZE - 1, 0, 0);  
    																				  
   flash_erase_page(ul_test_page_addr,1);									  
    																					
    for (ul_idx = 0; ul_idx < 4; ul_idx++)											  
    {																					
	    ul_page_buffer[ul_idx] = origin;												
    }																					
    																					
    flash_write(ul_test_page_addr, ul_page_buffer, 4, 0);							  
    																					
    flash_lock(ul_test_page_addr, ul_test_page_addr + IFLASH_PAGE_SIZE - 1, 0, 0); 	  
}

/**
 ******************************************************************************
 * \fn void FLASH_read(uint32_t flash_addr,uint8_t *ptr_buffer, uint8_t read_size)
 * \brief Read among of values from the flash address and also save into a buffer  
 *
 * \param flash_addr The start address of the flash to read 
 * \param prt_buffer The buffer where the data will be saved
 * \param read_size  The size of data to read the maximum number of data is 256
 * \retval void
 ******************************************************************************
 */
void FLASH_read(uint32_t flash_addr,uint8_t * ptr_buffer, uint8_t read_size)
{ 
	static uint32_t flash_value;
	i=0;
	
	for (ul_idx = 0; ul_idx <= read_size; )
	{
		
		flash_value = (uint32_t)pul_test_page[ul_idx];
			   	
		if (pul_test_page[ul_idx] == flash_value)
		{
			flash_read_buffer[i++] = (uint32_t)flash_value;
			flash_read_buffer[i++] = (uint32_t)flash_value>>8;
			flash_read_buffer[i++] = (uint32_t)flash_value>>16;
			flash_read_buffer[i++] = (uint32_t)flash_value>>24;
			
// 			ioport_set_pin_level(RED, false);
// 			delay_ms(25);
// 			ioport_set_pin_level(RED, true);
// 			delay_ms(25);
			ul_idx = ul_idx + 4;
		}
	} 
}
/**
 ******************************************************************************
 * \fn void FLASH_fwu_task_reset( void )
 * \brief reset interrupt process
 *
 * \param void
 * \retval void
 *
 ******************************************************************************
 */
void FLASH_fwu_reset(void)
{	
	static uint32_t *ptrboot_origin;
	static uint32_t boot_origin;
	
	ptrboot_origin = (uint32_t *) BOOT_ORIGIN_ADDR;
	boot_origin = (uint32_t)ptrboot_origin[0];

	//GET the origin of the RESET
	if(boot_origin == REFLASH_MODE)
	{
		if ( (op_c1218_apl.communication_state == BASE_STATE) && (nic_c1218_apl.communication_state == BASE_STATE) )
		{
			SYSTEM_RESTART_TYPE mcu;
			mcu.restart_cause = FIRMWARE_UPDATE;
			mcu.save_information = true;
			RST_APP_vExecute_mcu_reset(mcu);
		}
		
		TIMER_start_ms_timer(SM_FWU_TIMER, FWU_TIMER, FLASH_fwu_reset);
	}
}

/**
 ******************************************************************************
 * \fn void FLASH_erase_main_app_sector(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void FLASH_erase_main_app_sector(void) {
	
  static uint32_t image_start_addr = MAIN_APP_START_ADDR;
  
  flash_unlock(image_start_addr, image_start_addr + (TOTAL_IMAGE_SECTOR_SIZE) - 1, 0, 0);  
  flash_erase_sector(image_start_addr );
  flash_erase_sector(image_start_addr + (0x10000));
  flash_erase_sector(image_start_addr + (0x20000));
  
  flash_lock(image_start_addr, image_start_addr + TOTAL_IMAGE_SECTOR_SIZE - 1, 0, 0);   
}

/**
 ******************************************************************************
 * \fn void FLASH_erase_image_sector(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void FLASH_erase_image_sector(void) {
	
  static uint32_t image_start_addr = IMAGE_APP_START_ADDR;
  
  flash_unlock(image_start_addr, image_start_addr + (TOTAL_IMAGE_SECTOR_SIZE) - 1, 0, 0);  
  flash_erase_sector(image_start_addr );
  flash_erase_sector(image_start_addr + (0x10000));
  flash_erase_sector(image_start_addr + (0x20000));
  
  flash_lock(image_start_addr, image_start_addr + TOTAL_IMAGE_SECTOR_SIZE - 1, 0, 0);   
}


/**
 ******************************************************************************
 * \fn void FLASH_fwu_boot_mode(void)
 * \brief reset interrupt process
 *
 * \param void
 * \retval void
 *
 ******************************************************************************
 */
void FLASH_fwu_boot_mode(void)
{
	FLASH_boot_origin(0xAA);
	SM_APP_vFwup_mcu_restart();
}		
/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Ulises Contreras Lean / 2016-01-19
 * - Revision 02 / José Luis Bonilla Hernandez / 2016-06-14
 *   - Change 1:  Function to reset meter on a Firmware upgrade when metrology is running
 *   - Change 2:  Funtion to change the meter to BOOT MODE
 ******************************************************************************
 */
