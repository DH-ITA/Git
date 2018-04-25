/**
 ******************************************************************************
 * \file nic.c
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
#include "nic.h"
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
static uint8_t nic_rx_data[C1218_APL_SIZE];
static uint8_t nic_tx_data[C1218_APL_SIZE];
static uint32_t nic_logon_timeout;

/*
 ************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ************************************************************************
 */
C1218_DLL_TYPE nic_c1218_dll;
C1218_APL_TYPE nic_c1218_apl;
SECURITY_ENTRY_RCD_TYPE nic_security;

/**
 ************************************************************************
 * Function Prototypes for Private Functions with File Level Scope
 ************************************************************************
 */
static void NIC_identification_service(void);
static void NIC_negotiate_service(void);
static void NIC_logon_service(void);
static void NIC_security_service(void);
static void NIC_logoff_service(void);
static void NIC_terminate_service(void);
static void NIC_rx_abort(void);
static void NIC_tx_abort(void);
static void NIC_tx_retry(void);

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
 * \fn void NIC_initialize(void)
 * \brief NIC Library Initialization
 *
 * I/O port, Usart and variables initialization
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_initialize(void)
{ 	
	NIC_io_initialize();
	NIC_io_configure();
	
	nic_c1218_dll.io_configure = (void_fptr)NIC_io_configure;
	
	TIMER_reset_ms_timer(NIC_LOGON_TIMER);
	nic_logon_timeout = C1218_LOGON_TIMEOUT;
	memset(&nic_c1218_apl, 0, sizeof(C1218_APL_TYPE));
	nic_c1218_apl.communication_state = BASE_STATE;
	nic_c1218_apl.logon_session_expired = false;
	
	nic_c1218_dll.interrupt_context = true;
	
	nic_c1218_dll.rx_state = C1218_RX_IDLE;
	nic_c1218_dll.rx_timer = NIC_RX_TIMER;
	nic_c1218_dll.rx_abort = (void_fptr)NIC_rx_abort;
	nic_c1218_dll.rx_error = 0;
	nic_c1218_dll.rx_eop = false;

	nic_c1218_dll.rx_data = (uint8_t*)nic_rx_data;
	nic_c1218_dll.rx_data_length = 0;
	nic_c1218_dll.rx_data_bytes = 0;
	nic_c1218_dll.rx_data_bytes_remaining = 0;
	memset(nic_c1218_dll.rx_buffer, 0, (unsigned int)sizeof(nic_c1218_dll.rx_buffer));
	nic_c1218_dll.rx_buffer_bytes = 0;
	memset(nic_rx_data, 0, (unsigned int)sizeof(nic_rx_data));
	
	nic_c1218_dll.tx_state = C1218_TX_IDLE;
	nic_c1218_dll.tx_timer = NIC_TX_TIMER;
	nic_c1218_dll.tx_abort = (void_fptr)NIC_tx_abort;
	nic_c1218_dll.tx_retry = (void_fptr)NIC_tx_retry;
	nic_c1218_dll.tx_nbr_retries = 0;
	nic_c1218_dll.response_timeout_timer = NIC_RESPONSE_TIMEOUT_TIMER;
	nic_c1218_dll.tx_error = 0;
	nic_c1218_dll.tx_eop = false;

	nic_c1218_dll.tx_data = (uint8_t*)nic_tx_data;
	nic_c1218_dll.tx_data_length = 0;
	nic_c1218_dll.tx_data_bytes = 0;
	nic_c1218_dll.tx_data_bytes_remaining = 0;
	memset(nic_c1218_dll.tx_buffer, 0, (unsigned int)sizeof(nic_c1218_dll.tx_buffer));
	nic_c1218_dll.tx_buffer_bytes = 0;
	memset(nic_tx_data, 0, (unsigned int)sizeof(nic_tx_data));

	nic_c1218_dll.uart = NIC_UART;
	nic_c1218_dll.ongoing_transmission = false;
	nic_c1218_dll.interrupt_context = false;
	
	nic_security.incorrect_password = 0;
}

/**
 ******************************************************************************
 * \fn  void NIC_io_initialize(void)
 * \brief NIC I/O Port Registers Initialization
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_io_initialize(void)
{
	ioport_set_pin_level(NIC_SM_ATTENTION_PIN, false);
	ioport_set_pin_level(NIC_SM_BUSY_PIN, false);
}

/**
 ******************************************************************************
 * \fn void NIC_io_configure(void)
 * \brief NIC I/O Port Registers Configuration
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_io_configure(void)
{	
	ioport_set_pin_dir(NIC_SM_ATTENTION_PIN, IOPORT_DIR_OUTPUT);	
	ioport_set_pin_dir(NIC_SM_BUSY_PIN, IOPORT_DIR_OUTPUT);	
	
	ioport_set_pin_mode(PIO_PB4_IDX, IOPORT_MODE_MUX_A);
	ioport_disable_pin(PIO_PB4_IDX);
	
	ioport_set_pin_mode(PIO_PB5_IDX, IOPORT_MODE_MUX_A);
	ioport_disable_pin(PIO_PB5_IDX);
	
	Pdc *g_p_uart_pdc1;
	sam_uart_opt_t uart_console_settings;

	uart_console_settings.ul_mck = sysclk_get_cpu_hz();				// MCK for UART
	uart_console_settings.ul_baudrate = NIC_BAUDRATE;				// Expected baud rate.
	uart_console_settings.ul_chmode = NIC_MR_CHMODE_NORMAL;			// Configure channel mode (Normal, Automatic, Local_loopback or Remote_loopback)
	uart_console_settings.ul_mode = NIC_MR_PAR_NO;					// Initialize value for UART mode register
	
	/* Configure PMC. */
	pmc_enable_periph_clk( NIC_ID );
	
	/* Configure UART. */
	uart_init( NIC_UART, &uart_console_settings );
	
	/* Get board UART0 PDC base address and enable receiver and transmitter. */
	g_p_uart_pdc1 = uart_get_pdc_base( NIC_UART );
	pdc_enable_transfer( g_p_uart_pdc1, PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN );
	
	/*Enable RXRDY interrupt*/
	uart_enable_interrupt( NIC_UART, UART_IER_RXRDY );	
	
	/* Enable the receiver and transmitter. */
	uart_enable(NIC_UART);
	
	/* Configure and enable interrupt of USART. */
	NVIC_SetPriority( ( IRQn_Type ) NIC_IRQn, NIC_PRIO );
	NVIC_EnableIRQ( NIC_IRQn );
}

/**
******************************************************************************
* \fn void NIC_APP_vAssert_attention_request(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void NIC_APP_vAssert_attention_request(void) {
	
	ioport_set_pin_level(NIC_SM_ATTENTION_PIN, true);
}

/**
******************************************************************************
* \fn void NIC_APP_vDeassert_attention_request(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void NIC_APP_vDeassert_attention_request(void) {
	
	ioport_set_pin_level(NIC_SM_ATTENTION_PIN, false);
}

/**
******************************************************************************
* \fn void NIC_APP_vAssert_busy(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void NIC_APP_vAssert_busy(void) {
	
	ioport_set_pin_level(NIC_SM_BUSY_PIN, true);
}

/**
******************************************************************************
* \fn void NIC_APP_vDeassert_busy(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void NIC_APP_vDeassert_busy(void) {
	
	ioport_set_pin_level(NIC_SM_BUSY_PIN, false);
}

/**
******************************************************************************
* \fn void NIC_APP_vAssert_power_fail(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void NIC_APP_vAssert_power_fail(void) {
	
	ioport_set_pin_dir(NIC_SM_POWER_FAIL_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(NIC_SM_POWER_FAIL_PIN, false);
}

/**
******************************************************************************
* \fn void NIC_APP_vDeassert_power_fail(void)
* \brief
*
* \param void
* \retval void
******************************************************************************
*/
void NIC_APP_vDeassert_power_fail(void) {
	
	ioport_set_pin_dir(NIC_SM_POWER_FAIL_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(NIC_SM_POWER_FAIL_PIN, true);
}

/**
 ******************************************************************************
 * \fn void NIC_io_reset(void)
 * \brief NIC I/O Registers Reset
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_io_reset(void)
 {
	 if (nic_c1218_dll.ongoing_transmission == false)
	 {
		 NIC_io_configure();
	 }
 }

/**
 ******************************************************************************
 * \fn void NIC_create_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_create_task(void)
{
	/* Create NIC task */
	xTaskCreate( NIC_rtos_task, (const signed char * const ) "NIC_Task", NIC_STACK_SIZE, NULL, NIC_TASK_PRIO, &nic_c1218_dll.os_task_id );
}

/**
 ******************************************************************************
 * \fn void NIC_rtos_task(void *pvParameters)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_rtos_task(void *pvParameters)
{	
	UNUSED( pvParameters );
	
	/* NIC module task Loop. */
	for ( ; ; )
	{
		NIC_task();
		
		/* Suspend OP Task */
		vTaskSuspend(nic_c1218_dll.os_task_id);
	}
	
	/* Should never go there */
	vTaskDelete(NULL);
}

/**
 ******************************************************************************
 * \fn void NIC_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_task(void)
{	
	if ( nic_c1218_dll.rx_eop == true )
	{
		uint8_t access_permissions;
		
		nic_c1218_dll.rx_eop = false;
				
		if ( (nic_c1218_dll.rx_buffer[C1218_IDENTITY_INDEX] == 0) && (nic_c1218_dll.rx_error == 0) )
		{
			switch ( nic_c1218_apl.communication_state )
			{
				case BASE_STATE:
//  					switch ( nic_c1218_dll.rx_data[SERVICE_INDEX] )
//  					{
// 	 					case IDENTIFICATION:
// 	 					NIC_identification_service();
// 	 					WATCHDOG_APP_vRestart();
// 	 					break;
// 	 					
// 	 					default:
// 	 					C1218_send_isss(&nic_c1218_dll);
// 	 					break;
//  					}
//  					break;
				
				case ID_STATE:
					switch ( nic_c1218_dll.rx_data[SERVICE_INDEX] )
					{
						case IDENTIFICATION:
							NIC_identification_service();
							WATCHDOG_APP_vRestart();
							break;
			
						case NEGOTIATE:
							NIC_negotiate_service();
							WATCHDOG_APP_vRestart();
							break;
							
						case TERMINATE:
							NIC_terminate_service();
							WATCHDOG_APP_vRestart();
							break;
			
						case LOGON:
							NIC_logon_service();
							WATCHDOG_APP_vRestart();
							break;
							
						case FULL_READ:
							if ( !meter.system_state.busy ) {

								access_permissions = nic_c1218_apl.access_permissions;
								nic_c1218_apl.access_permissions = 0xFF;
								C1219_full_read_service(&nic_c1218_dll, nic_c1218_apl);
								nic_c1218_apl.access_permissions = access_permissions;
								WATCHDOG_APP_vRestart();
							}
							else {
								C1218_send_bsy(&nic_c1218_dll);
							}
							break;
						
						case PREAD_OFFSET:
							if (!meter.system_state.busy ) {

								access_permissions = nic_c1218_apl.access_permissions;
								nic_c1218_apl.access_permissions = 0xFF;
								C1219_pread_offset_service(&nic_c1218_dll, nic_c1218_apl);
								nic_c1218_apl.access_permissions = access_permissions;
								WATCHDOG_APP_vRestart();
							}
							else {
								C1218_send_bsy(&nic_c1218_dll);
							}
							break;

						case PWRITE_OFFSET:
							if ( !meter.system_state.busy ) {
								
								if ( (nic_c1218_dll.rx_data[SERVICE_INDEX + 1] == 0x08) && (nic_c1218_dll.rx_data[SERVICE_INDEX + 2] == 144) ) {
									
									access_permissions = nic_c1218_apl.access_permissions;
									nic_c1218_apl.access_permissions = 0xFF;
									C1219_pwrite_offset_service(&nic_c1218_dll, nic_c1218_apl);
									nic_c1218_apl.access_permissions = access_permissions;
									WATCHDOG_APP_vRestart();
								}
								else {
									C1218_send_isss(&nic_c1218_dll);
								}
							}
							else {
								C1218_send_bsy(&nic_c1218_dll);
							}
							break;
			
						default:
							C1218_send_isss(&nic_c1218_dll);
							break;
					}
					break;
		
				case SESSION_STATE:
					TIMER_start_ms_timer(NIC_LOGON_TIMER, nic_logon_timeout, NIC_reset_logon_session);
					switch ( nic_c1218_dll.rx_data[SERVICE_INDEX] )
					{	
						case FULL_READ:
							if ( !meter.system_state.busy ) {

								access_permissions = nic_c1218_apl.access_permissions;
								nic_c1218_apl.access_permissions = 0xFF;
								C1219_full_read_service(&nic_c1218_dll, nic_c1218_apl);
								nic_c1218_apl.access_permissions = access_permissions;
								WATCHDOG_APP_vRestart();
							}
							else {
								C1218_send_bsy(&nic_c1218_dll);
							}
							break;
						
						case PREAD_OFFSET:
							if ( !meter.system_state.busy ) {

								access_permissions = nic_c1218_apl.access_permissions;
								nic_c1218_apl.access_permissions = 0xFF;
								C1219_pread_offset_service(&nic_c1218_dll, nic_c1218_apl);
								nic_c1218_apl.access_permissions = access_permissions;
								WATCHDOG_APP_vRestart();
							}
							else {
								C1218_send_bsy(&nic_c1218_dll);
							}
							break;
											
						case FULL_WRITE:
							if ( !meter.system_state.busy ) {

								C1219_full_write_service(&nic_c1218_dll, nic_c1218_apl);
								WATCHDOG_APP_vRestart();
							}
							else {
								C1218_send_bsy(&nic_c1218_dll);
							}
							break;
						
						case PWRITE_OFFSET:
							if ( !meter.system_state.busy ) {
								
								if ( (nic_c1218_dll.rx_data[SERVICE_INDEX + 1] == 0x08) && (nic_c1218_dll.rx_data[SERVICE_INDEX + 2] == 144) ) {
									
									access_permissions = nic_c1218_apl.access_permissions;
									nic_c1218_apl.access_permissions = 0xFF;
									C1219_pwrite_offset_service(&nic_c1218_dll, nic_c1218_apl);
									nic_c1218_apl.access_permissions = access_permissions;
									WATCHDOG_APP_vRestart();
								}
								else {
									C1219_pwrite_offset_service(&nic_c1218_dll, nic_c1218_apl);
									WATCHDOG_APP_vRestart();
								}
							}
							else {
								C1218_send_bsy(&nic_c1218_dll);
							}
							break;
			
						case SECURITY:
							NIC_security_service();
							WATCHDOG_APP_vRestart();
							break;
							
						case TERMINATE:
							NIC_terminate_service();
							WATCHDOG_APP_vRestart();
							break;
							
						case LOGOFF:
							NIC_logoff_service();
							WATCHDOG_APP_vRestart();
							break;

						default:
							C1218_send_isss(&nic_c1218_dll);
							break;
					}
					break;
		
				default:
					C1218_send_isss(&nic_c1218_dll);
					nic_c1218_dll.rx_eop = false;
					break;
			}					
		} 
	}
}

/**
 ******************************************************************************
 * \fn void NIC_identification_service(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_identification_service(void)
{
	nic_c1218_apl.communication_state = ID_STATE;
	nic_c1218_dll.tx_data_length = 0;
	nic_tx_data[nic_c1218_dll.tx_data_length++] = SERVICE_R_OK;
	nic_tx_data[nic_c1218_dll.tx_data_length++] = STD;
	nic_tx_data[nic_c1218_dll.tx_data_length++] = VER;
	nic_tx_data[nic_c1218_dll.tx_data_length++] = REV;
	nic_tx_data[nic_c1218_dll.tx_data_length++] = END_OF_LIST;
	
	C1218_send_response(&nic_c1218_dll);
}

/**
 ******************************************************************************
 * \fn void NIC_negotiate_service(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_negotiate_service(void)
{
	nic_c1218_apl.communication_state = ID_STATE;
	nic_c1218_dll.tx_data_length = 0;
	nic_tx_data[nic_c1218_dll.tx_data_length++] = SERVICE_R_OK;
	nic_tx_data[nic_c1218_dll.tx_data_length++] = 0x00;
	nic_tx_data[nic_c1218_dll.tx_data_length++] = C1218_PACKET_SIZE;
	nic_tx_data[nic_c1218_dll.tx_data_length++] = NIC_APL_NUM_PACKETS;
	nic_tx_data[nic_c1218_dll.tx_data_length++] = 0x0B;
	
	C1218_send_response(&nic_c1218_dll);
}

/**
 ******************************************************************************
 * \fn void NIC_logon_service(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_logon_service(void)
{
	nic_c1218_apl.communication_state = SESSION_STATE;
	nic_c1218_apl.user_id = (uint16_t)(nic_rx_data[1] >> 8);
	nic_c1218_apl.user_id |= (uint16_t)nic_rx_data[2];
	memcpy(nic_c1218_apl.user, &nic_rx_data[3], C1218_USER_LENGTH);
	nic_c1218_apl.access_permissions = DEFAULT_LOG_ON_ACCESS_PERM;
	
	C1218_send_ok(&nic_c1218_dll);
	
	TIMER_start_ms_timer(NIC_LOGON_TIMER, nic_logon_timeout, NIC_reset_logon_session);
}

/**
 ******************************************************************************
 * \fn void NIC_security_service(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_security_service(void)
{
	uint8_t i;
	char password_rx[MAX_PASSWORD_LEN];
	bool pass_ok = false;

	for ( i = 0; i < (nic_c1218_dll.rx_data_length - 1); i++ )
	{
		password_rx[i] = nic_rx_data[i + 1];
	}
	
	for ( i = 0; i < st_41.nbr_passwords; i++ )
	{
		if ( memcmp( (const char*)password_rx, (const char*)st_42.security_entries[i].password, st_41.password_len) == 0 )
		{
			memcpy(&nic_c1218_apl.password, &st_42.security_entries[i].password, st_41.password_len);
			nic_c1218_apl.access_permissions = st_42.security_entries[i].access_permissions;
			pass_ok = true;
			break;
		}
	}
	
	if ( pass_ok == false )
	{
		if ( memcmp( (const char*)password_rx, (const char*)MASTER_PASSWORD, st_41.password_len) == 0 )
		{
			strcpy((char*)nic_c1218_apl.password, MASTER_PASSWORD);
			nic_c1218_apl.access_permissions = MASTER_PASS_ACCESS_PERM;
			pass_ok = true;
		}
	}
	
	if ( pass_ok == true )
	{
		nic_c1218_apl.communication_state = SESSION_STATE;
		nic_security.incorrect_password = 0;
		C1218_send_ok(&nic_c1218_dll);
	}
	else
	{
		C1218_send_err(&nic_c1218_dll);
		nic_security.incorrect_password++;
	}
}

/**
 ******************************************************************************
 * \fn void NIC_logoff_service(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_logoff_service(void)
{
	nic_c1218_apl.communication_state = ID_STATE;
	C1218_send_ok(&nic_c1218_dll);
	
	if( nic_security.incorrect_password > 0 ) {
		
		EVNT_APP_vRegister_event(INCORRECT_PASSWORD_NIC, 0, false, nic_c1218_apl.user_id, rtc.utc_timestamp);
		nic_security.incorrect_password = 0;
	}
}

/**
 ******************************************************************************
 * \fn void NIC_terminate_service(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_terminate_service(void)
{
	C1218_send_ok(&nic_c1218_dll);
		
	if ( nic_c1218_apl.communication_state != BASE_STATE ) {
		
		EVNT_APP_vRegister_event(COMMUNICATION_TERMINATED_NORMALLY, 0, true, nic_c1218_apl.user_id, rtc.utc_timestamp);
		EVNT_APP_vRegister_event(COMMUNICATION_TERMINATED_NORMALLY_NIC, 0, false, nic_c1218_apl.user_id, rtc.utc_timestamp);
	}
	
	if ( nic_security.incorrect_password > 0 ) {
	
		EVNT_APP_vRegister_event(INCORRECT_PASSWORD_NIC, 0, false, nic_c1218_apl.user_id, rtc.utc_timestamp);
		nic_security.incorrect_password = 0;
	}
	
	nic_c1218_apl.communication_state = BASE_STATE;
}

/**
 ******************************************************************************
 * \fn void NIC_reset_logon_session(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_reset_logon_session(void)
{
	nic_c1218_apl.communication_state = BASE_STATE;
	
	NIC_rx_abort();
	NIC_tx_abort();
	
	nic_c1218_dll.tx_nbr_retries = 0;
	nic_c1218_dll.wait_for_ack = false;
	
	TIMER_reset_ms_timer(NIC_LOGON_TIMER);
	memset(&nic_c1218_apl, 0, sizeof(C1218_APL_TYPE));
	
	nic_c1218_apl.logon_session_expired = true;
}

/**
 ******************************************************************************
 * \fn void NIC_rx_abort(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_rx_abort(void)
{
	nic_c1218_dll.interrupt_context = true;
	
	nic_c1218_dll.ongoing_transmission = false;
	nic_c1218_dll.uart = NIC_UART;
	nic_c1218_dll.rx_state = C1218_RX_IDLE;
	nic_c1218_dll.rx_timer = NIC_RX_TIMER;
	nic_c1218_dll.rx_abort = NIC_rx_abort;
	nic_c1218_dll.rx_data = (uint8_t*)nic_rx_data;
	TIMER_reset_ms_timer(nic_c1218_dll.rx_timer);

	nic_c1218_dll.interrupt_context = false;
}

/**
 ******************************************************************************
 * \fn void NIC_tx_abort(void)
 * \brief NIC I/O Port Registers Configuration
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_tx_abort(void)
{
	nic_c1218_dll.interrupt_context = true;
	
	nic_c1218_dll.ongoing_transmission = false;
	nic_c1218_dll.uart->UART_IDR = UART_IDR_TXRDY;
	nic_c1218_dll.uart->UART_IDR = UART_IDR_TXEMPTY;
	nic_c1218_dll.uart = NIC_UART;
	nic_c1218_dll.tx_state = C1218_TX_IDLE;
	nic_c1218_dll.tx_timer = NIC_TX_TIMER;
	nic_c1218_dll.tx_abort = NIC_tx_abort;
	nic_c1218_dll.tx_data = (uint8_t*)nic_tx_data;
	TIMER_reset_ms_timer(nic_c1218_dll.tx_timer);
	
	nic_c1218_dll.interrupt_context = false;
}

/**
 ******************************************************************************
 * \fn void NIC_tx_retry(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_tx_retry(void)
{
	if ( nic_c1218_dll.tx_nbr_retries < 0)
	{
		C1218_resend_packet(&nic_c1218_dll);
		nic_c1218_dll.tx_nbr_retries++;
	}
	else
	{
		//NIC_reset_logon_session();
	}
}

/**
 ******************************************************************************
 * \fn void UART0_Handler( void )
 * \brief
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */

void UART0_Handler( void )
{
	uint32_t ul_status;
	
	/* Read UART Status. */
	ul_status = uart_get_status( UART0);
		
	if ( ( (ul_status & UART_SR_RXRDY) == UART_SR_RXRDY ) && (nic_c1218_dll.ongoing_transmission == false) )
	{
		nic_c1218_dll.interrupt_context = true;
		nic_c1218_dll.uart = NIC_UART;
		nic_c1218_dll.rx_timer = NIC_RX_TIMER;
		nic_c1218_dll.rx_abort = NIC_rx_abort;
		nic_c1218_dll.rx_data = (uint8_t*)nic_rx_data;
	
		C1218_rx_int_handler(&nic_c1218_dll);
	}
	else if ( (ul_status & UART_SR_TXEMPTY) == UART_SR_TXEMPTY )
	{
		nic_c1218_dll.interrupt_context = true;
		nic_c1218_dll.uart = NIC_UART;
		nic_c1218_dll.tx_timer = NIC_TX_TIMER;
		nic_c1218_dll.tx_abort = NIC_tx_abort;
		nic_c1218_dll.tx_retry = NIC_tx_retry;
		nic_c1218_dll.io_configure = NIC_io_configure;
		nic_c1218_dll.tx_data = (uint8_t*)nic_tx_data;
	
		C1218_tx_int_handler(&nic_c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn NIC_logon_mfg_timeout_sesion( void )
 * \brief
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void NIC_logon_mfg_timeout_sesion (void)
{
	nic_logon_timeout = C1218_LOGON_MFG_TIMEOUT;
	TIMER_start_ms_timer(NIC_LOGON_TIMER, nic_logon_timeout, NIC_reset_logon_session);
}

/**
 ******************************************************************************
 * \fn NIC_logon_mfg_timeout_sesion( void )
 * \brief
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void NIC_logon_timeout_sesion (void)
{
	nic_logon_timeout = C1218_LOGON_TIMEOUT;
	TIMER_start_ms_timer(NIC_LOGON_TIMER, nic_logon_timeout, NIC_reset_logon_session);
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
