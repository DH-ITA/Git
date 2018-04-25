/**
 ******************************************************************************
 * \file c1218.c
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
#include "c1218.h"
#include "crc.h"
#include "timer.h"
#include "sm.h"

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
 ************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ************************************************************************
 */

/**
 ************************************************************************
 * Function Prototypes for Private Functions with File Level Scope
 ************************************************************************
 */

/*
 ******************************************************************************
 * ROM Const Variables With File Level Scope
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */

/** 
 ******************************************************************************
 * \fn C1218_dll_reset(C1218_DLL_TYPE *c1218)
 * \brief C1218 Library Initialization
 *
 * I/O port, Usart and variables initialization
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1218_dll_reset(C1218_DLL_TYPE *c1218)
{
	c1218->interrupt_context = true;
	
	c1218->rx_state = C1218_RX_IDLE;
	TIMER_reset_ms_timer(c1218->rx_timer);
	c1218->rx_error = 0;
	c1218->rx_eop = false;

	c1218->rx_data_length = 0;
	c1218->rx_data_bytes_remaining = 0;
	memset(&c1218->rx_buffer, 0, (unsigned int)sizeof(c1218->rx_buffer));
	c1218->rx_buffer_bytes = 0;
	
	c1218->tx_state = C1218_TX_IDLE;
	TIMER_reset_ms_timer(c1218->tx_timer);
	c1218->tx_error = 0;
	c1218->tx_eop = false;
	
	c1218->tx_data_length = 0;
	c1218->tx_data_bytes_remaining = 0;
	memset(&c1218->tx_buffer, 0, (unsigned int)sizeof(c1218->tx_buffer));
	c1218->tx_buffer_bytes = 0;
	
	c1218->ongoing_transmission = false;
	c1218->interrupt_context = false;
}

/**
 ******************************************************************************
 * \fn void C1218_send_response(C1218_DLL_TYPE *c1218)
 * \brief Put the first byte of the packet to the usart.
 *
 * Once the first byte is placed on the usart tx register, it will trigger
 * the usart tx register empty interrupt, and the rest of the bytes of the frame
 * are transmitted in this interrupt service routine.
 *
 * \param *c1218
 *	Pointer to ram memory location where packet is stored.
 * \retval void
 ******************************************************************************
 */
void C1218_send_response(C1218_DLL_TYPE *c1218)
{
	uint16_t crc_calc;
	uint32_t timeout = 15000;
	
	if( c1218->tx_state == C1218_TX_IDLE )
	{	
		c1218->interrupt_context = true;
		c1218->tx_error = 0;
		c1218->tx_eop = false;
		c1218->tx_buffer_bytes = 0;
		
		c1218->tx_buffer[C1218_STP_INDEX] = C1218_STP;
		c1218->tx_buffer[C1218_IDENTITY_INDEX] = c1218->rx_buffer[C1218_IDENTITY_INDEX];
		c1218->tx_buffer[C1218_CTRL_INDEX] = (c1218->tx_buffer[C1218_CTRL_INDEX] & 0b00100000) ^ 0b00100000;
		c1218->tx_buffer[C1218_SEQ_NBR_INDEX] = 0;
		c1218->tx_data_bytes_remaining = c1218->tx_data_length;
		c1218->tx_data_offset = 0;

		if ( c1218->tx_data_length > (C1218_DLL_SIZE) )
		{
			c1218->tx_data_bytes_remaining = C1218_DLL_SIZE;
			
			// Set first packet
			c1218->tx_buffer[C1218_CTRL_INDEX] |= 0b11000000;
			
			if ( (c1218->tx_data_length - c1218->tx_data_bytes_remaining) % C1218_DLL_SIZE == 0)
			{
				// Perfect divisor
				c1218->tx_buffer[C1218_SEQ_NBR_INDEX] = (c1218->tx_data_length - c1218->tx_data_bytes_remaining) / C1218_DLL_SIZE;
			}
			else
			{
				// The number doesn't divide perfectly by divisor
				c1218->tx_buffer[C1218_SEQ_NBR_INDEX] = 1 + (c1218->tx_data_length - c1218->tx_data_bytes_remaining) / C1218_DLL_SIZE;
			}
		}
		
		c1218->tx_buffer[C1218_LENGTH_H_INDEX] = (uint8_t)(c1218->tx_data_bytes_remaining >> 8);
		c1218->tx_buffer[C1218_LENGTH_L_INDEX] = (uint8_t)(c1218->tx_data_bytes_remaining & 0x00FF);
			
		memcpy(&c1218->tx_buffer[C1218_DATA_INDEX], (const void*)&c1218->tx_data[0], (unsigned int)c1218->tx_data_bytes_remaining);
		crc_calc = CRC_crc16((uint8_t*)c1218->tx_buffer, c1218->tx_data_bytes_remaining + 6);
		
		c1218->tx_buffer[c1218->tx_data_bytes_remaining + 6] = (uint8_t)(crc_calc >> 8);
		c1218->tx_buffer[c1218->tx_data_bytes_remaining + 7] = (uint8_t)(crc_calc & 0x00FF);
	
		/* Start sending first data byte.
			* Wait until it is possible to put data into UART_THR register.
			* NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */ 
		while( (c1218->uart->UART_SR & UART_SR_TXRDY) == 0 ) {
			if (!timeout--) {
				c1218->io_configure();
				break;
			}
		}
			
		c1218->ongoing_transmission = true;
		c1218->uart->UART_THR = c1218->tx_buffer[c1218->tx_buffer_bytes];
		
		c1218->tx_buffer_bytes++;
		
		c1218->uart->UART_IER = UART_IER_TXRDY; // uart enable TXRDY interrupt
		
		c1218->tx_state = C1218_TX_STP;
		
		TIMER_start_ms_timer(c1218->tx_timer, C1218_INTER_CHARACTER_TIMEOUT, null_action);
		c1218->interrupt_context = false;
	}
}

/**
 ******************************************************************************
 * \fn void C1218_send_next_packet(C1218_DLL_TYPE *c1218)
 * \brief Put the first byte of the packet to the usart.
 *
 * Once the first byte is placed on the usart tx register, it will trigger
 * the usart tx register empty interrupt, and the rest of the bytes of the frame
 * are transmitted in this interrupt service routine.
 *
 * \param *c1218
 *	Pointer to ram memory location where packet is stored.
 * \retval void
 ******************************************************************************
 */
void C1218_send_next_packet(C1218_DLL_TYPE *c1218)
{
	uint16_t crc_calc;
	uint32_t timeout = 15000;
	
	if( (c1218->tx_state == C1218_TX_IDLE) && (c1218->tx_buffer[C1218_SEQ_NBR_INDEX] > 0) )
	{	
		c1218->interrupt_context = true;
		c1218->tx_error = 0;
		c1218->tx_eop = false;
		c1218->tx_buffer_bytes = 0;
		
		c1218->tx_data_offset += C1218_DLL_SIZE;
		c1218->tx_buffer[C1218_STP_INDEX] = C1218_STP;
		c1218->tx_buffer[C1218_IDENTITY_INDEX] = c1218->rx_buffer[C1218_IDENTITY_INDEX];
		c1218->tx_buffer[C1218_CTRL_INDEX] = (c1218->tx_buffer[C1218_CTRL_INDEX] & 0b00100000) ^ 0b00100000;
		c1218->tx_buffer[C1218_CTRL_INDEX] |= 0b10000000;
		c1218->tx_buffer[C1218_SEQ_NBR_INDEX]--;
		
		if ( c1218->tx_buffer[C1218_SEQ_NBR_INDEX] > 0 )
		{
			c1218->tx_data_bytes_remaining = C1218_DLL_SIZE;
		}
		else
		{
			c1218->tx_data_bytes_remaining = c1218->tx_data_length - c1218->tx_data_offset;
		}
		
		c1218->tx_buffer[C1218_LENGTH_H_INDEX] = (uint8_t)(c1218->tx_data_bytes_remaining >> 8);
		c1218->tx_buffer[C1218_LENGTH_L_INDEX] = (uint8_t)(c1218->tx_data_bytes_remaining & 0x00FF);
			
		memcpy(&c1218->tx_buffer[C1218_DATA_INDEX], &c1218->tx_data[c1218->tx_data_offset], (unsigned int)c1218->tx_data_bytes_remaining);
		crc_calc = CRC_crc16((uint8_t*)c1218->tx_buffer, c1218->tx_data_bytes_remaining + 6);
		
		c1218->tx_buffer[c1218->tx_data_bytes_remaining + 6] = (uint8_t)(crc_calc >> 8);
		c1218->tx_buffer[c1218->tx_data_bytes_remaining + 7] = (uint8_t)(crc_calc & 0x00FF);
		
			/* Start sending first data byte.
			* Wait until it is possible to put data into UART_THR register.
			* NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */
		while( (c1218->uart->UART_SR & UART_SR_TXRDY) == 0 ) {
			if (!timeout--) {
				c1218->io_configure();
				break;
			}
		}
		
		c1218->ongoing_transmission = true;
		c1218->uart->UART_THR = c1218->tx_buffer[c1218->tx_buffer_bytes];
		
		c1218->tx_buffer_bytes++;
		
		c1218->uart->UART_IER = UART_IER_TXRDY; // uart enable TXRDY interrupt
		
		c1218->tx_state = C1218_TX_STP;
		
		TIMER_start_ms_timer(c1218->tx_timer, C1218_INTER_CHARACTER_TIMEOUT, null_action);
		c1218->interrupt_context = false;
	}
}

/**
 ******************************************************************************
 * \fn void C1218_resend_packet(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_resend_packet(C1218_DLL_TYPE *c1218)
{
	uint32_t timeout = 15000;
	
	if( c1218->tx_state == C1218_TX_IDLE )
	{	
		c1218->interrupt_context = true;
		c1218->tx_error = 0;
		c1218->tx_eop = false;	
		c1218->tx_buffer_bytes = 0;
		
		c1218->tx_data_bytes_remaining  = (uint16_t)(c1218->tx_buffer[C1218_LENGTH_H_INDEX] << 8); 
		c1218->tx_data_bytes_remaining |= (uint16_t)(c1218->tx_buffer[C1218_LENGTH_L_INDEX] << 0);
		
		uint16_t crc_calc = CRC_crc16((uint8_t*)c1218->tx_buffer, c1218->tx_data_bytes_remaining + 6);
		
		c1218->tx_buffer[c1218->tx_data_bytes_remaining + 6] = (uint8_t)(crc_calc >> 8);
		c1218->tx_buffer[c1218->tx_data_bytes_remaining + 7] = (uint8_t)(crc_calc & 0x00FF);
	
		/* Start sending first data byte.
			* Wait until it is possible to put data into UART_THR register.
			* NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */ 
		while( (c1218->uart->UART_SR & UART_SR_TXRDY) == 0 ) {
			if (!timeout--) {
				c1218->io_configure();
				break;
			}
		}
			
		c1218->ongoing_transmission = true;
		c1218->uart->UART_THR = c1218->tx_buffer[c1218->tx_buffer_bytes];
		
		c1218->tx_buffer_bytes++;
		
		c1218->uart->UART_IER = UART_IER_TXRDY; // uart enable TXRDY interrupt
		
		c1218->tx_state = C1218_TX_STP;
		
		TIMER_start_ms_timer(c1218->tx_timer, C1218_INTER_CHARACTER_TIMEOUT, null_action);
		c1218->interrupt_context = false;
	}
}

/**
 ******************************************************************************
 * \fn void C1218_send_aok(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_aok(C1218_DLL_TYPE *c1218)
{
	uint32_t timeout = 15000;
	
	/* Wait until it is possible to put data into UART_THR register.
	* NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */ 
	while( (c1218->uart->UART_SR & UART_SR_TXRDY) == 0 ) {
		if (!timeout--) {
			c1218->io_configure();
			break;
		}
	}
	
	c1218->uart->UART_THR = C1218_ACK;
}

/**
 ******************************************************************************
 * \fn void C1218_send_nok(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_nok(C1218_DLL_TYPE *c1218)
{
	uint32_t timeout = 15000;
	
	/* Wait until it is possible to put data into UART_THR register.
	* NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */ 
	while( (c1218->uart->UART_SR & UART_SR_TXRDY) == 0 ) {
		if (!timeout--) {
			c1218->io_configure();
			break;
		}
	}
	
	c1218->uart->UART_THR = C1218_NOK;
}

/**
 ******************************************************************************
 * \fn void C1218_send_ok(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_ok(C1218_DLL_TYPE *c1218)
{
	c1218->tx_data_length = 1;
	c1218->tx_data[0] = C1218_OK;
	C1218_send_response(c1218);
}

/**
 ******************************************************************************
 * \fn void C1218_send_err(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_err(C1218_DLL_TYPE *c1218)
{
	c1218->tx_data_length = 1;
	c1218->tx_data[0] = C1218_ERR;
	C1218_send_response(c1218);
}

/**
 ******************************************************************************
 * \fn void C1218_send_sns(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_sns(C1218_DLL_TYPE *c1218)
{
	c1218->tx_data_length = 1;
	c1218->tx_data[0] = C1218_SNS;
	C1218_send_response(c1218);
}

/**
 ******************************************************************************
 * \fn void C1218_send_isc(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_isc(C1218_DLL_TYPE *c1218)
{
	c1218->tx_data_length = 1;
	c1218->tx_data[0] = C1218_ISC;
	C1218_send_response(c1218);
}

/**
 ******************************************************************************
 * \fn void C1218_send_onp(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_onp(C1218_DLL_TYPE *c1218)
{
	c1218->tx_data_length = 1;
	c1218->tx_data[0] = C1218_ONP;
	C1218_send_response(c1218);
}

/**
 ******************************************************************************
 * \fn void C1218_send_iar(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_iar(C1218_DLL_TYPE *c1218)
{
	c1218->tx_data_length = 1;
	c1218->tx_data[0] = C1218_IAR;
	C1218_send_response(c1218);
}
	
/**
 ******************************************************************************
 * \fn void C1218_send_bsy(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_bsy(C1218_DLL_TYPE *c1218)
{
	c1218->tx_data_length = 1;
	c1218->tx_data[0] = C1218_BSY;
	C1218_send_response(c1218);
}

/**
 ******************************************************************************
 * \fn void C1218_send_dnr(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_dnr(C1218_DLL_TYPE *c1218)
{
	c1218->tx_data_length = 1;
	c1218->tx_data[0] = C1218_DNR;
	C1218_send_response(c1218);
}

/**
 ******************************************************************************
 * \fn void C1218_send_dlk(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_dlk(C1218_DLL_TYPE *c1218)
{
	c1218->tx_data_length = 1;
	c1218->tx_data[0] = C1218_DLK;
	C1218_send_response(c1218);
}

/**
 ******************************************************************************
 * \fn void C1218_send_isc(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_rno(C1218_DLL_TYPE *c1218)
{
	c1218->tx_data_length = 1;
	c1218->tx_data[0] = C1218_RNO;
	C1218_send_response(c1218);
}

/**
 ******************************************************************************
 * \fn void C1218_send_isss(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_send_isss(C1218_DLL_TYPE *c1218)
{
	c1218->tx_data_length = 1;
	c1218->tx_data[0] = C1218_ISSS;
	C1218_send_response(c1218);
}

/**
 ******************************************************************************
 * \fn void C1218_rx_abort(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_rx_abort(C1218_DLL_TYPE *c1218)
{
	c1218->interrupt_context = true;

	c1218->rx_state = C1218_RX_IDLE;
	TIMER_reset_ms_timer(c1218->rx_timer);
	c1218->ongoing_transmission = false;
	
	c1218->interrupt_context = false;
}

/**
 ******************************************************************************
 * \fn void C1218_tx_abort(C1218_DLL_TYPE *c1218)
 * \brief
 *
 * \param 
 * \retval
 ******************************************************************************
 */
void C1218_tx_abort(C1218_DLL_TYPE *c1218)
{
	c1218->interrupt_context = true;

	c1218->tx_state = C1218_TX_IDLE;
	TIMER_reset_ms_timer(c1218->tx_timer);
	c1218->ongoing_transmission = false;
	
	c1218->interrupt_context = false;
	c1218->uart->UART_IDR = UART_IDR_TXRDY; // uart disable TXRDY interrupt
}

/**
 ******************************************************************************
 * \fn void C1218_tx_int_handler(C1218_DLL_TYPE *c1218)
 * \brief Transmit byte complete interrupt service routine handler.
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void C1218_tx_int_handler(C1218_DLL_TYPE *c1218)
{
    c1218->interrupt_context = true;
	c1218->ongoing_transmission = true;
	
	TIMER_reset_ms_timer(c1218->tx_timer);
	
	switch( c1218->tx_state )
	{
		case C1218_TX_STP:
			if ( (c1218->tx_buffer[C1218_STP_INDEX] == C1218_STP) && (meter.power_failure == false) )
			{
				/* Wait until it is possible to put data into UART_THR register.
				* NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */
				while( (c1218->uart->UART_SR & UART_SR_TXRDY) == 0 ) {}
				c1218->uart->UART_THR = c1218->tx_buffer[c1218->tx_buffer_bytes];
				c1218->tx_buffer_bytes++;
				c1218->tx_state = C1218_TX_IDENTITY;
			}
			else
			{
				c1218->tx_abort();
			}
			break;
			
		case C1218_TX_IDENTITY:
			/* Wait until it is possible to put data into UART_THR register.
			 * NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */ 
			while( (c1218->uart->UART_SR & UART_SR_TXRDY) == 0 ) {}
			c1218->uart->UART_THR = c1218->tx_buffer[c1218->tx_buffer_bytes];
			c1218->tx_buffer_bytes++;
			c1218->tx_state = C1218_TX_CTRL;
			break;
		
		case C1218_TX_CTRL:
			/* Wait until it is possible to put data into UART_THR register.
			 * NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */ 
			while( (c1218->uart->UART_SR & UART_SR_TXRDY) == 0 ) {}
			c1218->uart->UART_THR = c1218->tx_buffer[c1218->tx_buffer_bytes];
			c1218->tx_buffer_bytes++;
			c1218->tx_state = C1218_TX_SEQ_NBR;
			break;
			
		case C1218_TX_SEQ_NBR:
			/* Wait until it is possible to put data into UART_THR register.
			 * NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */ 
			while( (c1218->uart->UART_SR & UART_SR_TXRDY) == 0 ) {}
			c1218->uart->UART_THR = c1218->tx_buffer[c1218->tx_buffer_bytes];
			c1218->tx_buffer_bytes++;
			c1218->tx_state = C1218_TX_LENGTH_H;
			break;
		
		case C1218_TX_LENGTH_H:
			/* Wait until it is possible to put data into UART_THR register.
			 * NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */ 
			while( (c1218->uart->UART_SR & UART_SR_TXRDY) == 0 ) {}
			c1218->uart->UART_THR = c1218->tx_buffer[c1218->tx_buffer_bytes];
			c1218->tx_buffer_bytes++;
			c1218->tx_state = C1218_TX_LENGTH_L;
			break;
			
		case C1218_TX_LENGTH_L:
			if ( c1218->tx_data_bytes_remaining > 0 )
			{		
				/* Wait until it is possible to put data into UART_THR register.
			 	 * NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */
				while( (c1218->uart->UART_SR & UART_SR_TXRDY) == 0 ) {}
				c1218->uart->UART_THR = c1218->tx_buffer[c1218->tx_buffer_bytes];
				c1218->tx_buffer_bytes++;
				c1218->tx_data_bytes_remaining--;
				c1218->tx_state = C1218_TX_LENGTH_L;
				
				if ( c1218->tx_data_bytes_remaining == 0 )
				{
					/* Transmit complete, set tx idle state for new transmissions */
      				c1218->tx_state = C1218_TX_DATA;
				}
			}
			break;

		case C1218_TX_DATA:
			/* Wait until it is possible to put data into UART_THR register.
			 * NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */ 
			while( (c1218->uart->UART_SR & UART_SR_TXRDY) == 0 ) {}
			c1218->uart->UART_THR = c1218->tx_buffer[c1218->tx_buffer_bytes];
			c1218->tx_buffer_bytes++;
			c1218->tx_state = C1218_TX_CRC_H;
			break;
		
		case C1218_TX_CRC_H:
			/* Wait until it is possible to put data into UART_THR register.
			 * NOTE: If UART_THR Register never becomes empty this will be a DEADLOCK. */ 
			while( (c1218->uart->UART_SR & UART_SR_TXEMPTY) == 0 ) {}
			c1218->uart->UART_THR = c1218->tx_buffer[c1218->tx_buffer_bytes];
			c1218->tx_buffer_bytes++;
			if ( c1218->tx_buffer[C1218_SEQ_NBR_INDEX] == 0 )
			{
				c1218->tx_eop = true;
			}
			c1218->ongoing_transmission = false;
			c1218->tx_state = C1218_TX_IDLE;
			c1218->uart->UART_IDR = UART_IDR_TXRDY; // uart disable TXRDY interrupt
			c1218->uart->UART_IDR = UART_IDR_TXEMPTY;
			c1218->wait_for_ack = true;
			TIMER_start_ms_timer(c1218->response_timeout_timer, C1218_RESPONSE_TIMEOUT, c1218->tx_retry);
			break;

		default:
			c1218->ongoing_transmission = false;
			c1218->tx_state = C1218_TX_IDLE;
			c1218->uart->UART_IDR = UART_IDR_TXRDY; // uart disable TXRDY interrupt
			c1218->uart->UART_IDR = UART_IDR_TXEMPTY;
			break;
	}

	if ( c1218->ongoing_transmission == true )
	{
		TIMER_start_ms_timer(c1218->tx_timer, C1218_INTER_CHARACTER_TIMEOUT,  c1218->tx_abort);
	}
	
	c1218->interrupt_context = false;
}

/**
 ******************************************************************************
 * \fn void C1218_rx_int_handler(C1218_DLL_TYPE *c1218)
 * \brief Transmit byte complete interrupt service routine handler.
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void C1218_rx_int_handler(C1218_DLL_TYPE *c1218)
{
	volatile uint8_t data;
	volatile uint16_t crc_rx;
	volatile uint16_t crc_calc;
	
    c1218->interrupt_context = true;
	c1218->ongoing_transmission = true;
	
	TIMER_reset_ms_timer(c1218->rx_timer);
    data = c1218->uart->UART_RHR;

    switch( c1218->rx_state )
    {
		case C1218_RX_IDLE:
			if ( (data == C1218_STP) /*&& (c1218->wait_for_ack == false)*/ && (meter.power_failure == false))
			{
				c1218->rx_buffer_bytes = 0;
				c1218->rx_buffer[c1218->rx_buffer_bytes] = data;
				c1218->rx_buffer_bytes++;
				
				c1218->rx_data_length = 0;
				c1218->rx_data_bytes_remaining = 0;
				c1218->rx_error = 0;
				c1218->rx_state = C1218_RX_STP;
			}
			else if ( (data == C1218_ACK) && (c1218->wait_for_ack == true) )
			{
				c1218->wait_for_ack = false;
				c1218->tx_nbr_retries = 0;
				c1218->ongoing_transmission = false;
				TIMER_reset_ms_timer(c1218->response_timeout_timer);
				
				if ( c1218->tx_buffer[C1218_SEQ_NBR_INDEX] > 0 )
				{
					C1218_send_next_packet(c1218);
				}
			}
			else if ( (data == C1218_NOK) && (c1218->wait_for_ack == true) )
			{
				TIMER_reset_ms_timer(c1218->response_timeout_timer);
				c1218->tx_retry();
				c1218->tx_nbr_retries = 0;
			}
			else
			{
				c1218->rx_state = C1218_RX_IDLE;
			}
			break;
		
        case C1218_RX_STP:
			c1218->rx_buffer[c1218->rx_buffer_bytes] = data;
			c1218->rx_buffer_bytes++;
			c1218->rx_state = C1218_RX_IDENTITY;
			break;
			
		case C1218_RX_IDENTITY:
			c1218->rx_buffer[c1218->rx_buffer_bytes] = data;
			c1218->rx_buffer_bytes++;
			
			if ( (data == 0) || (data == 0x20) || ((data & 0b11000000) == 0b11000000) )
			{
				c1218->rx_data_offset = 0;
			}
			c1218->rx_state = C1218_RX_CTRL;
			break;
		
		case C1218_RX_CTRL:
			c1218->rx_buffer[c1218->rx_buffer_bytes] = data;
			c1218->rx_buffer_bytes++;
			c1218->rx_state = C1218_RX_SEQ_NBR;
			break;
		
		case C1218_RX_SEQ_NBR:
			c1218->rx_buffer[c1218->rx_buffer_bytes] = data;
			c1218->rx_buffer_bytes++;
			c1218->rx_state = C1218_RX_LENGTH_H;
			break;
		
		case C1218_RX_LENGTH_H:
		if (c1218->rx_buffer_bytes < C1218_PACKET_SIZE + C1218_PACKET_OVERHEAD)
		{
			c1218->rx_buffer[c1218->rx_buffer_bytes] = data;
		}
			c1218->rx_buffer_bytes++;
			c1218->rx_data_bytes = 0;
			c1218->rx_data_bytes_remaining = (uint16_t)(c1218->rx_buffer[c1218->rx_buffer_bytes - 2] << 8);
			c1218->rx_data_bytes_remaining |= (uint16_t)(c1218->rx_buffer[c1218->rx_buffer_bytes - 1]);
			c1218->rx_data_length = c1218->rx_data_bytes_remaining;
			c1218->rx_state = C1218_RX_LENGTH_L;
			break;
			
		case C1218_RX_LENGTH_L:	
	        if (c1218->rx_data_bytes_remaining > 0)
			{
				if (c1218->rx_buffer_bytes < C1218_PACKET_SIZE + C1218_PACKET_OVERHEAD)
				{
					c1218->rx_buffer[c1218->rx_buffer_bytes] = data;
				}
			    c1218->rx_buffer_bytes++;
				c1218->rx_data_bytes++;
	            c1218->rx_data_bytes_remaining--;
				c1218->rx_state = C1218_RX_LENGTH_L;

				if (c1218->rx_data_bytes_remaining == 0)
				{
				    c1218->rx_state = C1218_RX_DATA;
                }
            }
	        break;
			
		case C1218_RX_DATA:
			if (c1218->rx_buffer_bytes < (C1218_PACKET_SIZE + C1218_PACKET_OVERHEAD))
			{
				c1218->rx_buffer[c1218->rx_buffer_bytes] = data;
			}
			c1218->rx_buffer_bytes++;
			c1218->rx_state = C1218_RX_CRC_H;
			break;
		
		case C1218_RX_CRC_H:
			if (c1218->rx_buffer_bytes < C1218_PACKET_SIZE + C1218_PACKET_OVERHEAD)
			{
				c1218->rx_buffer[c1218->rx_buffer_bytes] = data;
			}
			c1218->rx_buffer_bytes++;
			
			crc_rx = (uint16_t)(c1218->rx_buffer[c1218->rx_buffer_bytes - 2] << 8);
			crc_rx = crc_rx | (uint16_t)(c1218->rx_buffer[c1218->rx_buffer_bytes - 1]);

			crc_calc = CRC_crc16((uint8_t*)c1218->rx_buffer, c1218->rx_buffer_bytes - 2);
			
			if (crc_rx == crc_calc)
			{
				if ( (c1218->rx_buffer[C1218_CTRL_INDEX] & 0b10000000) == 0b10000000 )
				{
					if ( (c1218->rx_buffer[C1218_CTRL_INDEX] & 0b01000000) == 0b01000000 )
					{
						c1218->rx_buffer_previous_identity = c1218->rx_buffer[C1218_IDENTITY_INDEX];
						c1218->rx_buffer_previous_toggle_bit = c1218->rx_buffer[C1218_CTRL_INDEX] & 0b00100000;
						c1218->rx_buffer_previous_crc = crc_rx;
						
						if ( (c1218->rx_data_offset + c1218->rx_data_bytes) < (C1218_NUM_PACKETS*C1218_PACKET_SIZE) )
						{
							memcpy(&c1218->rx_data[c1218->rx_data_offset], &c1218->rx_buffer[C1218_DATA_INDEX], c1218->rx_data_bytes);
							c1218->rx_data_offset += c1218->rx_data_bytes;
							c1218->rx_error = 0;
						}
						else
						{
							c1218->rx_data_offset += c1218->rx_data_bytes;	
							c1218->rx_error = 1;							
						}
					}
					else
					{
						if ( (c1218->rx_buffer_previous_identity == c1218->rx_buffer[C1218_IDENTITY_INDEX]) &&
						     (c1218->rx_buffer_previous_toggle_bit == (c1218->rx_buffer[C1218_CTRL_INDEX] & 0b00100000)) &&
							 (c1218->rx_buffer_previous_crc == crc_rx) )
						{
							nop();
						}
						else
						{
							if ( (c1218->rx_data_offset + c1218->rx_data_bytes) < (C1218_NUM_PACKETS*C1218_PACKET_SIZE) )//fix_added
							{
								memcpy(&c1218->rx_data[c1218->rx_data_offset], &c1218->rx_buffer[C1218_DATA_INDEX], c1218->rx_data_bytes);
								c1218->rx_data_offset += c1218->rx_data_bytes;
								c1218->rx_error = 0;
								c1218->rx_buffer_previous_identity = c1218->rx_buffer[C1218_IDENTITY_INDEX];
								c1218->rx_buffer_previous_toggle_bit = c1218->rx_buffer[C1218_CTRL_INDEX] & 0b00100000;
								c1218->rx_buffer_previous_crc = crc_rx;
							}
							else
							{
								c1218->rx_data_offset += c1218->rx_data_bytes;
								c1218->rx_error = 1;
								c1218->rx_buffer_previous_identity = c1218->rx_buffer[C1218_IDENTITY_INDEX];//fix_added
								c1218->rx_buffer_previous_toggle_bit = c1218->rx_buffer[C1218_CTRL_INDEX] & 0b00100000;//fix_added
								c1218->rx_buffer_previous_crc = crc_rx;
							}
						}
					}
					if(c1218->rx_error == 0)
					{
						C1218_send_aok(c1218);
					}
					else
					{
						C1218_send_nok(c1218);
					}
				}
				else
				{
					if ( (c1218->rx_data_offset + c1218->rx_data_bytes) < (C1218_NUM_PACKETS*C1218_PACKET_SIZE) )
					{
						memcpy(&c1218->rx_data[c1218->rx_data_offset], &c1218->rx_buffer[C1218_DATA_INDEX], c1218->rx_data_bytes);
						c1218->rx_data_offset += c1218->rx_data_bytes;
						c1218->rx_error = 0;
						C1218_send_aok(c1218);
					}
					else
					{
						c1218->rx_data_offset += c1218->rx_data_bytes;
						c1218->rx_error = 1;
						C1218_send_nok(c1218);
					}
				}					
			}
			else
			{
				c1218->rx_error = 1;
				C1218_send_nok(c1218);
			}
			
			if ( c1218->rx_buffer[C1218_SEQ_NBR_INDEX] == 0 )
			{
				c1218->rx_eop = true;
				SM_APP_vExecute_mfg_service(c1218);
								
				portBASE_TYPE xYieldRequired;
				xYieldRequired = xTaskResumeFromISR(c1218->os_task_id);
				
				if ( xYieldRequired == pdTRUE )
				{
					portYIELD();
				}
			}
			
			c1218->ongoing_transmission = false;
			c1218->rx_state = C1218_RX_IDLE;
			
			break;

        default:
			c1218->rx_state = C1218_RX_IDLE;
			c1218->ongoing_transmission = false;
		    break;
    }
	
	if ( c1218->ongoing_transmission == true )
	{
		TIMER_start_ms_timer(c1218->rx_timer, C1218_INTER_CHARACTER_TIMEOUT, c1218->rx_abort);
	}
	
    c1218->interrupt_context = false;
}
			
/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
