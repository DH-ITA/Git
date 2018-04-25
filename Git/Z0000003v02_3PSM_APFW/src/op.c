/**
 ******************************************************************************
 * \file op.c
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
#include "target.h"
#include "c1218.h"
#include "op.h"
#include "secr.h"
#include "uart.h"
#include "crc.h"
#include "timer.h"
#include "util.h"
#include "sm.h"
#include "evnt.h"
#include "secr.h"
#include "watchdog.h"
#include "rtc_driver.h"
#include "c1219.h"

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
static uint8_t op_rx_data[C1218_APL_SIZE];
static uint8_t op_tx_data[C1218_APL_SIZE];
static uint32_t op_logon_timeout;

/*
 ************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ************************************************************************
 */
C1218_DLL_TYPE op_c1218_dll;
C1218_APL_TYPE op_c1218_apl;
SECURITY_ENTRY_RCD_TYPE op_security;

/**
 ************************************************************************
 * Function Prototypes for Private Functions with File Level Scope
 ************************************************************************
 */
static void OP_identification_service(void);
static void OP_negotiate_service(void);
static void OP_logon_service(void);
static void OP_security_service(void);
static void OP_logoff_service(void);
static void OP_terminate_service(void);

static void OP_rx_abort(void);
static void OP_tx_abort(void);
static void OP_tx_retry(void);

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
 * \fn void OP_initialize(void)
 * \brief OP Library Initialization
 *
 * I/O port, Usart and variables initialization
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_initialize(void)
{ 
	OP_io_initialize();
	OP_io_configure();
	
	op_c1218_dll.io_configure = (void_fptr)OP_io_configure;
	
	TIMER_reset_ms_timer(OP_LOGON_TIMER);
	op_logon_timeout = C1218_LOGON_TIMEOUT;
	memset(&op_c1218_apl, 0, sizeof(C1218_APL_TYPE));
	op_c1218_apl.communication_state = BASE_STATE;
	op_c1218_apl.logon_session_expired = false;
	
	op_c1218_dll.interrupt_context = true;
	
	op_c1218_dll.rx_state = C1218_RX_IDLE;
	op_c1218_dll.rx_timer = OP_RX_TIMER;
	op_c1218_dll.rx_abort = (void_fptr)OP_rx_abort;
	op_c1218_dll.rx_error = 0;
	op_c1218_dll.rx_eop = false;

	op_c1218_dll.rx_data = (uint8_t*)op_rx_data;
	op_c1218_dll.rx_data_length = 0;
	op_c1218_dll.rx_data_bytes = 0;
	op_c1218_dll.rx_data_bytes_remaining = 0;
	memset(op_c1218_dll.rx_buffer, 0, (unsigned int)sizeof(op_c1218_dll.rx_buffer));
	op_c1218_dll.rx_buffer_bytes = 0;
	memset(op_rx_data, 0, (unsigned int)sizeof(op_rx_data));
	
	op_c1218_dll.tx_state = C1218_TX_IDLE;
	op_c1218_dll.tx_timer = OP_TX_TIMER;
	op_c1218_dll.tx_abort = (void_fptr)OP_tx_abort;
	op_c1218_dll.tx_retry = (void_fptr)OP_tx_retry;
	op_c1218_dll.tx_nbr_retries = 0;
	op_c1218_dll.response_timeout_timer = OP_RESPONSE_TIMEOUT_TIMER;
	op_c1218_dll.tx_error = 0;
	op_c1218_dll.tx_eop = false;

	op_c1218_dll.tx_data = (uint8_t*)op_tx_data;
	op_c1218_dll.tx_data_length = 0;
	op_c1218_dll.tx_data_bytes = 0;
	op_c1218_dll.tx_data_bytes_remaining = 0;
	memset(op_c1218_dll.tx_buffer, 0, (unsigned int)sizeof(op_c1218_dll.tx_buffer));
	op_c1218_dll.tx_buffer_bytes = 0;
	memset(op_tx_data, 0, (unsigned int)sizeof(op_tx_data));

	op_c1218_dll.uart = OP_UART;
	op_c1218_dll.ongoing_transmission = false;
	op_c1218_dll.interrupt_context = false;
	
	op_security.incorrect_password = 0;
}

/**
 ******************************************************************************
 * \fn  void OP_io_initialize(void)
 * \brief OP I/O Port Registers Initialization
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_io_initialize(void)
{
	
}

/**
 ******************************************************************************
 * \fn void OP_io_configure(void)
 * \brief OP I/O Port Registers Configuration
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_io_configure(void)
{
	ioport_set_pin_mode(PIO_PC0_IDX, IOPORT_MODE_MUX_A);
	ioport_disable_pin(PIO_PC0_IDX);
	
	ioport_set_pin_mode(PIO_PC1_IDX, IOPORT_MODE_MUX_A);
	ioport_disable_pin(PIO_PC1_IDX);
	
	Pdc *g_p_uart_pdc1;
	sam_uart_opt_t uart_console_settings;

	uart_console_settings.ul_mck = sysclk_get_cpu_hz();				// MCK for UART
	uart_console_settings.ul_baudrate = OP_BAUDRATE;				// Expected baud rate.
	uart_console_settings.ul_chmode = OP_MR_CHMODE_NORMAL;			// Configure channel mode (Normal, Automatic, Local_loopback or Remote_loopback)
	uart_console_settings.ul_mode = OP_MR_PAR_NO;					// Initialize value for UART mode register
	
	/* Configure PMC. */
	pmc_enable_periph_clk( OP_ID );
	
	/* Configure UART. */
	uart_init( OP_UART, &uart_console_settings );
	
	/* Get board UART1 PDC base address and enable receiver and transmitter. */
	g_p_uart_pdc1 = uart_get_pdc_base( OP_UART );
	pdc_enable_transfer( g_p_uart_pdc1, PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN );
	
	/*Enable RXRDY interrupt*/
	uart_enable_interrupt( OP_UART, UART_IER_RXRDY );	
	
	/* Enable the receiver and transmitter. */
	uart_enable(OP_UART);
	
	/* Configure and enable interrupt of USART. */
	NVIC_SetPriority( ( IRQn_Type ) OP_IRQn, OP_PRIO );
	NVIC_EnableIRQ( OP_IRQn );
}

/**
 ******************************************************************************
 * \fn void OP_io_reset(void)
 * \brief OP I/O Registers Reset
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_io_reset(void)
 {
	 if (op_c1218_dll.ongoing_transmission == false)
	 {
		 OP_io_configure();
	 }
 }
 
 /**
 ******************************************************************************
 * \fn void OP_create_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_create_task(void)
{
	/* Create OP task */
	xTaskCreate( OP_rtos_task, (const signed char * const ) "OP_Task", OP_STACK_SIZE, NULL, OP_TASK_PRIO, &op_c1218_dll.os_task_id );
}

/**
 ******************************************************************************
 * \fn void OP_rtos_task(void *pvParameters)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_rtos_task(void *pvParameters)
{
	UNUSED( pvParameters );
	
	/* NIC module task Loop. */
	for ( ; ; )
	{
		OP_task();
		
		/* Suspend OP Task */
		vTaskSuspend(op_c1218_dll.os_task_id);
	}
	
	/* Should never go there */
	vTaskDelete(NULL);
}

/**
 ******************************************************************************
 * \fn void OP_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_task(void)
{
	if ( op_c1218_dll.rx_eop == true )
	{
		op_c1218_dll.rx_eop = false;
	
		if ( (op_c1218_dll.rx_buffer[C1218_IDENTITY_INDEX] == 0) && (op_c1218_dll.rx_error == 0) )
		{
			switch ( op_c1218_apl.communication_state )
			{
				case BASE_STATE:
// 					switch ( op_c1218_dll.rx_data[SERVICE_INDEX] )
// 					{
// 						case IDENTIFICATION:
// 							OP_identification_service();
// 							WATCHDOG_APP_vRestart();
// 							break;
// 							
// 						default:
// 							C1218_send_isss(&op_c1218_dll);
// 							break;
// 					}
// 					break;
				
				case ID_STATE:
					switch ( op_c1218_dll.rx_data[SERVICE_INDEX] )
					{
						case IDENTIFICATION:
							OP_identification_service();
							WATCHDOG_APP_vRestart();
							break;
			
						case NEGOTIATE:
							OP_negotiate_service();
							WATCHDOG_APP_vRestart();
							break;
							
						case TERMINATE:
							OP_terminate_service();
							WATCHDOG_APP_vRestart();
							break;
			
						case LOGON:
							OP_logon_service();
							WATCHDOG_APP_vRestart();
							break;
			
						default:
							C1218_send_isss(&op_c1218_dll);
							break;
					}
					break;
		
				case SESSION_STATE:
					TIMER_start_ms_timer(OP_LOGON_TIMER, op_logon_timeout, OP_reset_logon_session);
					switch ( op_c1218_dll.rx_data[SERVICE_INDEX] )
					{
						case FULL_READ:
							C1219_full_read_service(&op_c1218_dll, op_c1218_apl);
							WATCHDOG_APP_vRestart();
							break;
						
						case PREAD_OFFSET:
							C1219_pread_offset_service(&op_c1218_dll, op_c1218_apl);
							WATCHDOG_APP_vRestart();
							break;
						
						case FULL_WRITE:
							if ( !meter.system_state.busy ) {
								
								C1219_full_write_service(&op_c1218_dll, op_c1218_apl);
								WATCHDOG_APP_vRestart();
							}
							else {
								C1218_send_bsy(&op_c1218_dll);
							}
							break;
						
						case PWRITE_OFFSET:
							if ( !meter.system_state.busy ) {
								
								C1219_pwrite_offset_service(&op_c1218_dll, op_c1218_apl);
								WATCHDOG_APP_vRestart();
							}
							else {
								C1218_send_bsy(&op_c1218_dll);
							}
							break;
			
						case SECURITY:
							OP_security_service();
							WATCHDOG_APP_vRestart();
							break;
							
						case TERMINATE:
							OP_terminate_service();
							WATCHDOG_APP_vRestart();
							break;
							
						case LOGOFF:
							OP_logoff_service();
							WATCHDOG_APP_vRestart();
							break;

						default:
							C1218_send_isss(&op_c1218_dll);
							break;
					}
					break;
		
				default:
					C1218_send_isss(&op_c1218_dll);
					op_c1218_dll.rx_eop = false;
					break;
			}
		} 
	}
}

/**
 ******************************************************************************
 * \fn void OP_identification_service(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_identification_service(void)
{
	op_c1218_apl.communication_state = ID_STATE;
	op_c1218_dll.tx_data_length = 0;
	op_tx_data[op_c1218_dll.tx_data_length++] = SERVICE_R_OK;
	op_tx_data[op_c1218_dll.tx_data_length++] = STD;
	op_tx_data[op_c1218_dll.tx_data_length++] = VER;
	op_tx_data[op_c1218_dll.tx_data_length++] = REV;
	op_tx_data[op_c1218_dll.tx_data_length++] = END_OF_LIST;
	
	C1218_send_response(&op_c1218_dll);
}

/**
 ******************************************************************************
 * \fn void OP_negotiate_service(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_negotiate_service(void)
{
	op_c1218_apl.communication_state = ID_STATE;
	op_c1218_dll.tx_data_length = 0;
	op_tx_data[op_c1218_dll.tx_data_length++] = SERVICE_R_OK;
	op_tx_data[op_c1218_dll.tx_data_length++] = 0x00;
	op_tx_data[op_c1218_dll.tx_data_length++] = C1218_PACKET_SIZE;
	op_tx_data[op_c1218_dll.tx_data_length++] = OP_APL_NUM_PACKETS;
	op_tx_data[op_c1218_dll.tx_data_length++] = 0x0B;
	
	C1218_send_response(&op_c1218_dll);
}

/**
 ******************************************************************************
 * \fn void OP_logon_service(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_logon_service(void)
{
	op_c1218_apl.communication_state = SESSION_STATE;
	op_c1218_apl.user_id = (uint16_t)(op_rx_data[1] >> 8);
	op_c1218_apl.user_id |= (uint16_t)op_rx_data[2];
	memcpy(op_c1218_apl.user, &op_rx_data[3], C1218_USER_LENGTH);
	op_security.access_permissions = DEFAULT_LOG_ON_ACCESS_PERM;

	C1218_send_ok(&op_c1218_dll);
	
	TIMER_start_ms_timer(OP_LOGON_TIMER, op_logon_timeout, OP_reset_logon_session);
}

/**
 ******************************************************************************
 * \fn void OP_security_service(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_security_service(void)
{
	uint8_t i;
	char password_rx[MAX_PASSWORD_LEN];
	bool pass_ok = false;

	for ( i = 0; i < (op_c1218_dll.rx_data_length - 1); i++ )
	{
		password_rx[i] = op_rx_data[i + 1];
	}
	
	for ( i = 0; i < st_41.nbr_passwords; i++ )
	{
		if ( memcmp( (const char*)password_rx, (const char*)st_42.security_entries[i].password, st_41.password_len) == 0 )
		{
			memcpy(&op_c1218_apl.password, &st_42.security_entries[i].password, st_41.password_len);
			op_c1218_apl.access_permissions = st_42.security_entries[i].access_permissions;
			pass_ok = true;
			break;
		}
	}
	
	if ( pass_ok == false )
	{
		if ( memcmp( (const char*)password_rx, (const char*)MASTER_PASSWORD, st_41.password_len) == 0 )
		{
			strcpy((char*)op_c1218_apl.password, MASTER_PASSWORD);
			op_c1218_apl.access_permissions = MASTER_PASS_ACCESS_PERM;
			pass_ok = true;
		}
	}		
 	
 	if ( pass_ok == true )
 	{
 		op_c1218_apl.communication_state = SESSION_STATE;
		op_security.incorrect_password = 0;
 		C1218_send_ok(&op_c1218_dll);
 	}
 	else
 	{
 		C1218_send_err(&op_c1218_dll);
		op_security.incorrect_password++;
 	}
}

/**
 ******************************************************************************
 * \fn void OP_logoff_service(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_logoff_service(void)
{
	op_c1218_apl.communication_state = ID_STATE;
	C1218_send_ok(&op_c1218_dll);
	
	if ( op_security.incorrect_password > 0 ) {
		
		EVNT_APP_vRegister_event(INCORRECT_PASSWORD_OP, 0, false, op_c1218_apl.user_id, rtc.utc_timestamp);
		op_security.incorrect_password = 0;
	}
}

/**
 ******************************************************************************
 * \fn void OP_terminate_service(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_terminate_service(void)
{
	C1218_send_ok(&op_c1218_dll);
	
	if ( op_c1218_apl.communication_state != BASE_STATE ) {
		
		EVNT_APP_vRegister_event(COMMUNICATION_TERMINATED_NORMALLY, 0, true, op_c1218_apl.user_id, rtc.utc_timestamp);
		EVNT_APP_vRegister_event(COMMUNICATION_TERMINATED_NORMALLY_OP, 0, false, op_c1218_apl.user_id, rtc.utc_timestamp);					
	}
	
	if ( op_security.incorrect_password > 0 ) {
		
		EVNT_APP_vRegister_event(INCORRECT_PASSWORD_OP, 0, false, op_c1218_apl.user_id, rtc.utc_timestamp);
		op_security.incorrect_password = 0;
	}
	
	op_c1218_apl.communication_state = BASE_STATE;
}

/**
 ******************************************************************************
 * \fn void OP_reset_logon_session(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_reset_logon_session(void)
{
	op_c1218_apl.communication_state = BASE_STATE;
	
	OP_rx_abort();
	OP_tx_abort();
	
	op_c1218_dll.tx_nbr_retries = 0;
	op_c1218_dll.wait_for_ack = false;
	
	TIMER_reset_ms_timer(OP_LOGON_TIMER);
	memset(&op_c1218_apl, 0, sizeof(C1218_APL_TYPE));
	
	op_c1218_apl.logon_session_expired = true;
}

/**
 ******************************************************************************
 * \fn void OP_rx_abort(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_rx_abort(void)
{
	op_c1218_dll.interrupt_context = true;
	
	op_c1218_dll.ongoing_transmission = false;
	op_c1218_dll.uart = OP_UART;
	op_c1218_dll.rx_state = C1218_RX_IDLE;
	op_c1218_dll.rx_timer = OP_RX_TIMER;
	op_c1218_dll.rx_abort = OP_rx_abort;
	op_c1218_dll.rx_data = (uint8_t*)op_rx_data;
	TIMER_reset_ms_timer(op_c1218_dll.rx_timer);

	op_c1218_dll.interrupt_context = false;
}

/**
 ******************************************************************************
 * \fn void OP_tx_abort(void)
 * \brief NIC I/O Port Registers Configuration
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_tx_abort(void)
{
	op_c1218_dll.interrupt_context = true;
	
	op_c1218_dll.ongoing_transmission = false;
	op_c1218_dll.uart->UART_IDR = UART_IDR_TXRDY;
	op_c1218_dll.uart->UART_IDR = UART_IDR_TXEMPTY;
	op_c1218_dll.uart = OP_UART;
	op_c1218_dll.tx_state = C1218_TX_IDLE;
	op_c1218_dll.tx_timer = OP_TX_TIMER;
	op_c1218_dll.tx_abort = OP_tx_abort;
	op_c1218_dll.tx_data = (uint8_t*)op_tx_data;
	TIMER_reset_ms_timer(op_c1218_dll.tx_timer);
	
	op_c1218_dll.interrupt_context = false;
}

/**
 ******************************************************************************
 * \fn void OP_tx_retry(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void OP_tx_retry(void)
{
	if ( op_c1218_dll.tx_nbr_retries < 3)
	{
		C1218_resend_packet(&op_c1218_dll);
		op_c1218_dll.tx_nbr_retries++;
	}
	else
	{
		OP_reset_logon_session();
	}
}

/**
 ******************************************************************************
 * \fn void UART1_Handler( void )
 * \brief
 *
 * \param	None
 * \retval	None
 ******************************************************************************
 */
void UART1_Handler( void )
{
	uint32_t ul_status;
	
	/* Read UART Status. */
	ul_status = uart_get_status( UART1 );
	
	if ( ( (ul_status & UART_SR_RXRDY) == UART_SR_RXRDY ) && (op_c1218_dll.ongoing_transmission == false) )
	{
		op_c1218_dll.interrupt_context = true;
		op_c1218_dll.uart = OP_UART;
		op_c1218_dll.rx_timer = OP_RX_TIMER;
		op_c1218_dll.rx_abort = OP_rx_abort;
		op_c1218_dll.rx_data = (uint8_t*)op_rx_data;
	
		C1218_rx_int_handler(&op_c1218_dll);
	}
	else if ( (ul_status & UART_SR_TXEMPTY) == UART_SR_TXEMPTY )
	{
		op_c1218_dll.interrupt_context = true;
		op_c1218_dll.uart = OP_UART;
		op_c1218_dll.tx_timer = OP_TX_TIMER;
		op_c1218_dll.tx_abort = OP_tx_abort;
		op_c1218_dll.tx_retry = OP_tx_retry;
		op_c1218_dll.io_configure = OP_io_configure;
		op_c1218_dll.tx_data = (uint8_t*)op_tx_data;
	
		C1218_tx_int_handler(&op_c1218_dll);
	}
}
			
/**
 ******************************************************************************
 * \fn OP_logon_mfg_timeout_sesion( void )
 * \brief
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void OP_logon_mfg_timeout_sesion (void)
{
	op_logon_timeout = C1218_LOGON_MFG_TIMEOUT;
	TIMER_start_ms_timer(OP_LOGON_TIMER, op_logon_timeout, OP_reset_logon_session);
}

/**
 ******************************************************************************
 * \fn OP_logon_timeout_sesion( void )
 * \brief
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void OP_logon_timeout_sesion (void)
{
	op_logon_timeout = C1218_LOGON_TIMEOUT;
	TIMER_start_ms_timer(OP_LOGON_TIMER, op_logon_timeout, OP_reset_logon_session);
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
