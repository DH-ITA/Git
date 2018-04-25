/**
 ******************************************************************************
 * \file i2c.c
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

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "twi.h"
#include "i2c.h"
#include "ioport.h"
#include "delay.h"

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
 * Brief description for each block of constant definitions
\*===========================================================================*/

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
 * Enumerations, Structures and Typedefs
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each enumeration, structure and typedef
\*===========================================================================*/

/*
 ******************************************************************************
 * Global and Const Variable Defining Definitions
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
 */
 static twi_packet_t i2c_twi_package;
 static bool i2c_busy;
 static uint16_t i2c_over_attemps;

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
static void I2C_HAL_vIo_initialize(void);
static void I2C_HAL_vIo_configure(void);
static void I2C_HAL_vRecovery(void);
static void I2C_HAL_vTwi_enable_peripheral_mode(void);
static void I2C_HAL_vTwi_enable_pio_mode(void);

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
*/
/** 
 ******************************************************************************
 * \fn void I2C_HAL_vInitialize(void)
 * \brief I2C Initialization
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void I2C_HAL_vInitialize(void) {
  
	I2C_HAL_vIo_initialize();
	I2C_HAL_vIo_configure();

	i2c_over_attemps = 0;
}

/**
 ******************************************************************************
 * \fn void I2C_HAL_vIo_initialize(void)
 * \brief I2C I/O Port Registers Initialization
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void I2C_HAL_vIo_initialize(void) {

	ioport_set_pin_level(I2C_WC_PIN, true);
}

/**
 ******************************************************************************
 * \fn void I2C_HAL_vIo_configure(void)
 * \brief I2C I/O Port Registers Configuration
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void I2C_HAL_vIo_configure(void) {	

	twi_options_t opt;
		
	I2C_HAL_vRecovery();
	
	// Set WC_PIN as output
	ioport_set_pin_dir(I2C_WC_PIN, IOPORT_DIR_OUTPUT);
	
	//disable PIO control of pins to enable peripheral control
	I2C_HAL_vTwi_enable_peripheral_mode();
	
	/* Configure PMC. */
	pmc_enable_periph_clk( ID_TWI0 );
	PDC_TWI0->PERIPH_PTCR = PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN;
	
	/* Initialize TWI master. */
	opt.master_clk = I2C_TWI_MASTER_CLK;
	opt.speed = I2C_TWI_FAST_MODE_SPEED;
	opt.smbus = 0;
	twi_master_init(I2C_TWI, &opt);
}

/**
 ******************************************************************************
 * \fn void I2C_HAL_vRecovery(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void I2C_HAL_vRecovery(void) {

	I2C_HAL_vTwi_enable_pio_mode();
	
	ioport_set_pin_dir(I2C_SCL_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(I2C_SDA_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(I2C_WC_PIN, IOPORT_DIR_OUTPUT);
	
	ioport_set_pin_level(I2C_SDA_PIN, true);
	ioport_set_pin_dir(I2C_SDA_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(I2C_SCL_PIN, IOPORT_DIR_OUTPUT);
	
	ioport_set_pin_level(I2C_SDA_PIN, true);
	ioport_set_pin_level(I2C_SCL_PIN, true);
	delay_us(10);
	
	ioport_set_pin_level(I2C_SDA_PIN, false);
	delay_us(10);

	ioport_set_pin_level(I2C_SCL_PIN, false);
	delay_us(10);
	
	__NOP();
	__NOP();
	
	ioport_set_pin_dir(I2C_SDA_PIN, IOPORT_DIR_INPUT);
	
	while(ioport_get_pin_level(I2C_SDA_PIN) == false) {

		ioport_toggle_pin_level(I2C_SCL_PIN);
		delay_us(10);
	}
	
	ioport_set_pin_dir(I2C_SDA_PIN, IOPORT_DIR_INPUT);
}

/**
 ******************************************************************************
 * \fn void I2C_HAL_vTwi_enable_peripheral_mode(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void I2C_HAL_vTwi_enable_peripheral_mode(void) {

	REG_PIOA_PDR |= PIO_PDR_P24;
	REG_PIOA_PDR |= PIO_PDR_P25;
	
	REG_PIOA_PER &= ~PIO_PER_P24;
	REG_PIOA_PER &= ~PIO_PER_P25;
}

/**
 ******************************************************************************
 * \fn void I2C_APP_vTwi_enable_pio_mode(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void I2C_HAL_vTwi_enable_pio_mode(void) {

	REG_PIOA_PDR &= ~PIO_PDR_P24;
	REG_PIOA_PDR &= ~PIO_PDR_P25;
	
	REG_PIOA_PER |= PIO_PER_P24;
	REG_PIOA_PER |= PIO_PER_P25;
}

/**
 ******************************************************************************
 * \fn bool I2C_HAL_bEeprom_writer(uint32_t address, void* data_pointer, unsigned int data_length)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool I2C_HAL_bEeprom_writer(uint32_t address, void* data_pointer, unsigned int data_length) {

	bool write_ok = false;

	if ( !i2c_busy ) {
		
		i2c_busy = true;
		
		volatile uint8_t tries;
		volatile unsigned int i;
		volatile uint8_t* data = data_pointer;
		volatile uint8_t temp_data[256];
		volatile unsigned int wr_bytes;
	
		/* --------------------------------------------------------------------- */
		/* Determine size of first block write                                   */
		/* --------------------------------------------------------------------- */
		wr_bytes = 256 - (address & 0x000000FF);			/* get maximum size for first write */
	
		if ( data_length < wr_bytes )                       /* sync to page boudary */
		{
			wr_bytes = data_length;                         /* yes, then just write the request amount */
		}
	
		/* --------------------------------------------------------------------- */
		/* Write blocks */
		/* --------------------------------------------------------------------- */
		do {
			
			write_ok = true;
		
			for ( tries = 0; tries < 5; tries++ ) {

 				// Enable WR pin
 	 			ioport_set_pin_level(I2C_WC_PIN, false);
 	 			ioport_set_pin_dir(I2C_WC_PIN, IOPORT_DIR_OUTPUT);
						
				// Configure the TWI data
				i2c_twi_package.chip = I2C_EEPROM_CHIP_ADDR(address);
				i2c_twi_package.addr[0] = (uint8_t)(address >> 8);
				i2c_twi_package.addr[1] = (uint8_t)(address);
				i2c_twi_package.addr_length = 2;
				i2c_twi_package.length = wr_bytes; // Data size
				i2c_twi_package.buffer = (void*)data_pointer;
			
				if ( twi_master_write(TWI0, &i2c_twi_package) != TWI_SUCCESS ) {
					I2C_HAL_vIo_configure();
				}
				delay_ms(10);
		
				// Disable WR pin
	 			ioport_set_pin_level(I2C_WC_PIN, true);
	 			ioport_set_pin_dir(I2C_WC_PIN, IOPORT_DIR_OUTPUT);
	 				
				// Configure the TWI data
				i2c_twi_package.chip = I2C_EEPROM_CHIP_ADDR(address);
				i2c_twi_package.addr[0] = (uint8_t)(address >> 8);
				i2c_twi_package.addr[1] = (uint8_t)(address);
				i2c_twi_package.addr_length = 2;
				i2c_twi_package.length = wr_bytes; // Data size
				i2c_twi_package.buffer = (void*) &temp_data;
	
				if ( twi_master_read(TWI0, &i2c_twi_package) != TWI_SUCCESS ) {
					I2C_HAL_vIo_configure();
				}
	
				for ( i = 0; i < wr_bytes; i++) {

					if ( data[i] != temp_data[i] ) {

						write_ok = false;
						break;
					}
				}

				if ( write_ok == true ) {
					break;
				}
			}
			
			address  += wr_bytes;                                       /* bump the pointers by amount written */
			data_pointer  += wr_bytes;
			data = data_pointer;
			data_length -= wr_bytes;                                    /* number of bytes left */
		
			if ( 256 > data_length ) {

				wr_bytes = data_length;
			} 
			else {
				wr_bytes = 256;
			}
		}
		while ( (data_length > 0) && (write_ok == true) );

		i2c_busy = false;
	}
	else {
		i2c_over_attemps++;
	}
	
	return write_ok;
}

/**
 ******************************************************************************
 * \fn bool I2C_HAL_bEeprom_reader(uint32_t address, void* data_pointer, unsigned int data_length)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool I2C_HAL_bEeprom_reader(uint32_t address, void* data_pointer, unsigned int data_length) {

	bool read_ok = false;

	if ( !i2c_busy ) {
		
		i2c_busy = true;

		uint8_t nbr_retries = 0;
	
		while ( (nbr_retries < 20) && (read_ok == false) ) {

			// Configure the TWI data
			i2c_twi_package.chip = I2C_EEPROM_CHIP_ADDR(address);
			i2c_twi_package.addr[0] = (uint8_t)(address >> 8);
			i2c_twi_package.addr[1] = (uint8_t)(address);
			i2c_twi_package.addr_length = 2;
			i2c_twi_package.length = data_length; // Data size
			i2c_twi_package.buffer = (void*) data_pointer;
		
			if ( twi_master_read(TWI0, &i2c_twi_package) == TWI_SUCCESS ) {

				read_ok = true;
			}
			else {

				read_ok = false;
				I2C_HAL_vIo_configure();
				delay_ms(10);
			}
		
			nbr_retries++;
		}
	
		if ( read_ok == false ) {
			I2C_HAL_vIo_configure();
		}

		i2c_busy = false;
	}
	else {
		i2c_over_attemps++;
	}
	
	return read_ok;
}

/**
 ******************************************************************************
 * \fn bool I2C_HAL_bRtc_writer(uint32_t address, void* data_pointer, unsigned int data_length)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool I2C_HAL_bRtc_writer(uint32_t address, void* data_pointer, unsigned int data_length) {

	bool write_ok;

	if ( !i2c_busy ) {
		
		i2c_busy = true;

		volatile uint8_t tries;
		volatile unsigned int i;
		volatile uint8_t* data = data_pointer;
		volatile uint8_t temp_data[256];
		volatile unsigned int wr_bytes;
	
		/* --------------------------------------------------------------------- */
		/* Determine size of first block write                                   */
		/* --------------------------------------------------------------------- */
		wr_bytes = 256 - (address & 0x000000FF);			/* get maximum size for first write */
	
		if ( data_length < wr_bytes ) {                      /* sync to page boudary */
		
			wr_bytes = data_length;                         /* yes, then just write the request amount */
		}
	
		/* --------------------------------------------------------------------- */
		/* Write blocks */
		/* --------------------------------------------------------------------- */
		do {
		
			write_ok = true;
		
			for ( tries = 0; tries < 5; tries++ ) {
			
				// Configure the TWI data
				i2c_twi_package.chip = 0x32;
				i2c_twi_package.addr[0] = (uint8_t)(address);
				i2c_twi_package.addr_length = 1;
				i2c_twi_package.length = wr_bytes; // Data size
				i2c_twi_package.buffer = (void*)data_pointer;
			
				if ( twi_master_write(TWI0, &i2c_twi_package) != TWI_SUCCESS ) {
					I2C_HAL_vIo_configure();
				}
				delay_ms(10);
			
				// Configure the TWI data
				i2c_twi_package.chip = 0x32;
				i2c_twi_package.addr[0] = (uint8_t)(address);
				i2c_twi_package.addr_length = 1;
				i2c_twi_package.length = wr_bytes; // Data size
				i2c_twi_package.buffer = (void*) &temp_data;
			
				if ( twi_master_read(TWI0, &i2c_twi_package) != TWI_SUCCESS ) {
					I2C_HAL_vIo_configure();
				}
			
				for ( i = 0; i < wr_bytes; i++) {

					if ( data[i] != temp_data[i] ) {

						write_ok = false;
						break;
					}
				}

				if ( write_ok == true ) {
					break;
				}
			}
		
			address  += wr_bytes;                                       /* bump the pointers by amount written */
			data_pointer  += wr_bytes;
			data = data_pointer;
			data_length -= wr_bytes;                                    /* number of bytes left */
		
			if ( 256 > data_length ) {

				wr_bytes = data_length;
			}
			else {
				wr_bytes = 256;
			}
		}
		while ( (data_length > 0) && (write_ok == true) );

		i2c_busy = false;
	}
	else {
		i2c_over_attemps++;
	}
	
	return write_ok;
}

/**
 ******************************************************************************
 * \fn bool I2C_HAL_bRtc_reader(uint32_t address, void* data_pointer, unsigned int data_length)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool I2C_HAL_bRtc_reader(uint32_t address, void* data_pointer, unsigned int data_length) {

	bool read_ok = false;

	if ( !i2c_busy ) {
		
		i2c_busy = true;

		uint8_t nbr_retries = 0;
	
		while ( (nbr_retries < 20) && (read_ok == false) ) {

			// Configure the TWI data
			i2c_twi_package.chip = 0x32;
			i2c_twi_package.addr[0] = (uint8_t)(address);
			i2c_twi_package.addr_length = 1;
			i2c_twi_package.length = data_length; // Data size
			i2c_twi_package.buffer = (void*) data_pointer;
		
			if ( twi_master_read(TWI0, &i2c_twi_package) == TWI_SUCCESS ) {

				read_ok = true;
			}
			else {

				read_ok = false;
				I2C_HAL_vIo_configure();
				delay_ms(10);
			}
		
			nbr_retries++;
		}
	
		if ( read_ok == false ) {
			I2C_HAL_vIo_configure();
		}

		i2c_busy = false;
	}
	else {
		i2c_over_attemps++;
	}
	
	return read_ok;
}

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */