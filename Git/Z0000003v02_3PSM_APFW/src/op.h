/**
 ******************************************************************************
 * \file op.h
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
#ifndef OP_H
#define OP_H

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

#define OP_STACK_SIZE		configMINIMAL_STACK_SIZE * 10
#define OP_TASK_PRIO		tskIDLE_PRIORITY + 2

 /*===========================================================================*\
 *  Optical Port TX/RX Data buffer length
\*===========================================================================*/
#define OP_APL_NUM_PACKETS					C1218_NUM_PACKETS
#define OP_APL_PACKET_SIZE					C1218_PACKET_SIZE
#define OP_APL_SIZE							C1218_NUM_PACKETS*C1218_PACKET_SIZE + C1218_PACKET_OVERHEAD

/*===========================================================================*\
 *  OP i/o Port definition
\*===========================================================================*/
#define OP_UART 							UART1
#define OP_ID								ID_UART1
#define OP_IRQn								UART1_IRQn
#define OP_PRIO								configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY		// note In case of use of FreeRTOS, GROUP_PRIO is same value than configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY

#define OP_BAUDRATE							9600
#define OP_MR_CHMODE_NORMAL					UART_MR_CHMODE_NORMAL
#define OP_MR_PAR_NO						UART_MR_PAR_NO

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
extern C1218_DLL_TYPE op_c1218_dll;
extern C1218_APL_TYPE op_c1218_apl;
extern SECURITY_ENTRY_RCD_TYPE op_security;

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
 */
extern void OP_initialize(void);
extern void OP_io_initialize(void);
extern void OP_io_configure(void);
extern void OP_io_reset(void);
extern void OP_rtos_task(void *pvParameters);
extern void OP_task(void);
extern void OP_create_task(void);
extern void OP_logon_mfg_timeout_sesion(void);
extern void OP_logon_timeout_sesion (void);
extern void OP_reset_logon_session(void);
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
