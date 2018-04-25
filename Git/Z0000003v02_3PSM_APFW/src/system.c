/**
 ******************************************************************************
 * \file system.c
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
#include "flash_efc.h"
#include "flash.h"
#include "target.h"
#include "nic.h"
#include "op.h"
#include "secr.h"
#include "pfail.h"
#include "regs.h"
#include "tou.h"
#include "lp.h"
#include "evnt.h"
#include "sm.h"
#include "rtc.h"
#include "timer.h"
#include "led.h"
#include "system.h"
#include "metrology.h"
#include "eeprom.h"
#include "i2c.h"
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
static void SYSTEM_set_PCK_output(void);
static void SYSTEM_configure_unused_io(void);
static void SYSTEM_initialize_unused_io(void);

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */

/**
 ******************************************************************************
 * \fn void SYSTEM_initialize(void)
 * \brief Initialize all system configurations
 *
 * \param void
 * \retval 
 ******************************************************************************
 */
void SYSTEM_initialize(void) {

	SM_APP_vInitialize_system_state();
		
	/* Disable global interrupts */
	Disable_global_interrupt();
	
	/* Initialize Watch Dog */
	WATCHDOG_HAL_vInitialize();	

	/* Enable global interrupts */
	Enable_global_interrupt();

	/* Initialize Power Fail */
	PFAIL_APP_vInitialize();
	WATCHDOG_APP_vRestart();
	
    MATRIX0->MATRIX_SCFG[2] = MATRIX_SCFG_DEFMSTR_TYPE( 2 ) | MATRIX_SCFG_FIXED_DEFMSTR( 6 );
    CMCC0->CMCC_CTRL = CMCC_CTRL_CEN;
	
	/* System Clock Initialization */
 	//sysclk_init();
	//WATCHDOG_APP_vRestart();

	/* Set EMAFE clk to 4.096Mhz */
	SYSTEM_set_PCK_output();
	WATCHDOG_APP_vRestart();
	
	/* io_port initialize */
	ioport_init();
	WATCHDOG_APP_vRestart();
	
	/* Core 1 Initialization */
	Init_Core1();
	WATCHDOG_APP_vRestart();
	
 	//EEPROM_erase_all();
	
	/* Init Flash */
	FLASH_initialize();
	WATCHDOG_APP_vRestart();

	/* I2C initialize */
	I2C_HAL_vInitialize();
	WATCHDOG_APP_vRestart();

	/* Load mm100 C1219 Tables */
	SM_APP_vLoad_c1219_tables();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	/* Initialize meter modules */
	SM_APP_vInitialize();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	/* Initialize Registers module */
	REGS_APP_vInitialize();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	/* Initialize Real Time Clock */
	RTC_APP_vInitialize();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();

	/* Load Time of Use */
	TOU_APP_vInitialize();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();

	/* Initialize tick clock and Timers */
	TIMER_initialize();
	WATCHDOG_APP_vRestart();

	/* Initialize RGB LED */
	LED_initialize();
	LED_test_rgb();
	LED_diagnosis_led_task();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
		
	/* Initialize Front Optical Port */
	OP_initialize();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	/* Initialize Rear Optical Port */
	NIC_initialize();
	WATCHDOG_APP_vRestart();
	PFAIL_APP_vTask();
	
	PFAIL_APP_vRegister_power_up();
	WATCHDOG_APP_vRestart(); 
	PFAIL_APP_vTask();
	
	SM_APP_vTrigger_timers();
	WATCHDOG_APP_vRestart(); 
	PFAIL_APP_vTask();
}

/**
 ******************************************************************************
 * \fn void SYSTEM_set_PCK_output(void)
 * \brief Initialize all the system configurations
 *
 * \param void
 * \retval 
 ******************************************************************************
 */
void SYSTEM_set_PCK_output(void)
{
	pmc_enable_periph_clk(ID_PIOA);
	pio_set_peripheral(PIOA, PIO_PERIPH_A, (uint32_t)(PIO_PA29A_PCK1));
	pmc_switch_pck_to_pllack(1, PMC_PCK_PRES_CLK_2);
	pmc_enable_pck(PMC_PCK_1);
}

/**
 ******************************************************************************
 * \fn void SYSTEM_unused_io_configure(void)
 * \brief 
 *
 * \param void
 * \retval 
 ******************************************************************************
 */
void SYSTEM_configure_unused_io(void)
{
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOB, 24), IOPORT_DIR_OUTPUT );		// NO CONNECTED
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOB, 25), IOPORT_DIR_OUTPUT );		// NO CONNECTED
	
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOB, 14), IOPORT_DIR_OUTPUT );		// OB_GPIO1
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOB, 7), IOPORT_DIR_OUTPUT );		// OB_GPIO2
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 11), IOPORT_DIR_OUTPUT );		// OB_GPIO3
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 10), IOPORT_DIR_OUTPUT );		// OB_RX
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 9), IOPORT_DIR_OUTPUT );		// OB_TX

	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 3), IOPORT_DIR_OUTPUT );		// SPI0_WP
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 6), IOPORT_DIR_OUTPUT );		// SPI0_MISO
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 7), IOPORT_DIR_OUTPUT );		// SPI0_MOSI
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 8), IOPORT_DIR_OUTPUT );		// SPI0_SPCK
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 13), IOPORT_DIR_OUTPUT );		// SPI0_RESET
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 21), IOPORT_DIR_OUTPUT );		// SPI0_NPCS1
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 22), IOPORT_DIR_OUTPUT );		// SPI0_NPCS2
	
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOB, 15), IOPORT_DIR_INPUT );		// BUTTON_01
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 18), IOPORT_DIR_INPUT );		// BUTTON_02
	
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 27), IOPORT_DIR_OUTPUT );		// RELAY_1
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 28), IOPORT_DIR_OUTPUT );		// RELAY_1
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 17), IOPORT_DIR_OUTPUT );		// RELAY_2
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 23), IOPORT_DIR_OUTPUT );		// RELAY_2
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 30), IOPORT_DIR_OUTPUT );		// RELAY_3
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 31), IOPORT_DIR_OUTPUT );		// RELAY_3
	
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOB, 12), IOPORT_DIR_INPUT );		// REED_SWITCH
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 20), IOPORT_DIR_INPUT );		// ACCEL_INT1
																				
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOB, 6), IOPORT_DIR_OUTPUT );		// LCD_GPIO1
}

/**
 ******************************************************************************
 * \fn void SYSTEM_unused_io_configure(void)
 * \brief 
 *
 * \param void
 * \retval 
 ******************************************************************************
 */
void SYSTEM_initialize_unused_io(void)
{
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOB, 24), false );		// NO CONNECTED
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOB, 25), false );		// NO CONNECTED
	
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOB, 14), false );		// OB_GPIO1
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOB, 7), false );		// OB_GPIO2
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 11), false );		// OB_GPIO3
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 10), false );		// OB_RX
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 9), false );		// OB_RX

	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 3), false );		// SPI0_WP
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 6), false );		// SPI0_MISO
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 7), false );		// SPI0_MOSI
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 8), false );		// SPI0_SPCK
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 13), false );		// SPI0_RESET
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 21), false );		// SPI0_NPCS1
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 22), false );		// SPI0_NPCS2
	
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOB, 15), false );		// BUTTON_01
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 18), false );		// BUTTON_02
	
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 27), false );		// RELAY_1
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 28), false );		// RELAY_1
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 17), false );		// RELAY_2
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 23), false );		// RELAY_2
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 30), false );		// RELAY_3
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 31), false );		// RELAY_3
	
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOB, 12), false );		// REED_SWITCH
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 20), false );		// ACCEL_INT1
	
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOB, 6), false );		// LCD_GPIO1
}

/**
 ******************************************************************************
 * \fn void IO_unused_initialize(void)
 * \brief
 *
 * \param
 * \retval
 ******************************************************************************
 */
void SYSTEM_initialize_io(void)
{
	SYSTEM_initialize_unused_io();
	SYSTEM_configure_unused_io();
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