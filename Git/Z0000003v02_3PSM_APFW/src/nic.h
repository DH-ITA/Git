/**
 ******************************************************************************
 * \file nic.h
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
#ifndef NIC_H
#define NIC_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "c1218.h"
#include "secr.h"
#include "pfail.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */

#define NIC_STACK_SIZE		configMINIMAL_STACK_SIZE * 10
#define NIC_TASK_PRIO		tskIDLE_PRIORITY + 2

/*===========================================================================*\
 * Optical Port TX/RX Data buffer length
\*===========================================================================*/
#define NIC_APL_NUM_PACKETS					C1218_NUM_PACKETS
#define NIC_APL_PACKET_SIZE					C1218_PACKET_SIZE
#define NIC_APL_SIZE						C1218_NUM_PACKETS*C1218_PACKET_SIZE + C1218_PACKET_OVERHEAD

/*===========================================================================*\
 *  NIC i/o Port definition
\*===========================================================================*/
#define NIC_UART 							UART0
#define NIC_ID								ID_UART0
#define NIC_IRQn							UART0_IRQn
#define NIC_PRIO							configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY		// note In case of use of FreeRTOS, GROUP_PRIO is same value than configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY

#define NIC_BAUDRATE						115200
#define NIC_MR_CHMODE_NORMAL				UART_MR_CHMODE_NORMAL
#define NIC_MR_PAR_NO						UART_MR_PAR_NO

#define NIC_SM_POWER_FAIL_PIN				IOPORT_CREATE_PIN(PIOA, 0)
#define NIC_SM_ATTENTION_PIN				IOPORT_CREATE_PIN(PIOA, 14)
#define NIC_SM_BUSY_PIN						IOPORT_CREATE_PIN(PIOA, 15)

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs                          
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
extern C1218_DLL_TYPE nic_c1218_dll;
extern C1218_APL_TYPE nic_c1218_apl;
extern SECURITY_ENTRY_RCD_TYPE nic_security;

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
 */
extern void NIC_initialize(void);
extern void NIC_io_initialize(void);
extern void NIC_io_configure(void);
extern void NIC_APP_vAssert_attention_request(void);
extern void NIC_APP_vDeassert_attention_request(void);
extern void NIC_APP_vAssert_busy(void);
extern void NIC_APP_vDeassert_busy(void);
extern void NIC_APP_vAssert_power_fail(void);
extern void NIC_APP_vDeassert_power_fail(void);
extern void NIC_io_reset(void);
extern void NIC_rtos_task(void *pvParameters);
extern void NIC_task(void);
extern void NIC_create_task(void);
extern void NIC_logon_mfg_timeout_sesion (void);
extern void NIC_logon_timeout_sesion (void);
extern void NIC_reset_logon_session(void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
#endif
