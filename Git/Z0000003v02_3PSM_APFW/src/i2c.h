/**
 ******************************************************************************
 * \file i2c.h
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
 *   - Format: F_P06-DP_14
 *   - Revision: 02
 *   - Date: 16/MAY/2017
 *   - Author: Ernesto Cocoletzi.
 *   - Aprove: Mariano Centeno Camarena.
 ******************************************************************************
 *
 * @defgroup File change log
 * @section FILE_CHANGE_LOG FILE CHANGE LOG:
 *
 ******************************************************************************
 *   - Revision 01 / 2017-11-22
 *   - PCR#00000001/Jacob Anaya Candia: Initial issue.
 *   - PCR#00000002/Author: Description.
 ******************************************************************************
 *   - Revision 02 / YYYY-MM-DD
 *   - PCR#00000003/Author: Description.
 *   - PCR#00000004/Author: Description.
 ******************************************************************************
 */
#ifndef  I2C_H
#define  I2C_H

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
#define	I2C_SDA_PIN										IOPORT_CREATE_PIN(PIOA, 24)
#define	I2C_SCL_PIN										IOPORT_CREATE_PIN(PIOA, 25)

/*===========================================================================*\
 * TWI Port Devices Identification
\*===========================================================================*/
#define I2C_EEPROM_DEVICE_IDENTIFIER					0x0A
#define I2C_EEPROM_CHIP_ADDR(internal_addr)				( (I2C_EEPROM_DEVICE_IDENTIFIER << 3) | ( ((internal_addr >> 16) & 0x07) << 0) )

#define I2C_RTC_DEVICE_IDENTIFIER						0x06
#define I2C_RTC_CHIP_ADDR(internal_addr)				( (I2C_RTC_DEVICE_IDENTIFIER << 3) | ( ((internal_addr >> 16) & 0x07) << 0) )

/*===========================================================================*\
 * TWI Port configuration
\*===========================================================================*/
#define I2C_WC_PIN										IOPORT_CREATE_PIN(PIOA, 26)
#define I2C_TWI											TWI0
#define I2C_TWI_MASTER_CLK								sysclk_get_cpu_hz()
#define I2C_TWI_STANDARD_MODE_SPEED						200000
#define I2C_TWI_FAST_MODE_SPEED							400000

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
extern void I2C_HAL_vInitialize(void);
extern bool I2C_HAL_bEeprom_writer(uint32_t address, void* data_pointer, unsigned int data_length);
extern bool I2C_HAL_bEeprom_reader(uint32_t address, void* data_pointer, unsigned int data_length);
extern bool I2C_HAL_bRtc_writer(uint32_t address, void* data_pointer, unsigned int data_length);
extern bool I2C_HAL_bRtc_reader(uint32_t address, void* data_pointer, unsigned int data_length);

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
 #endif