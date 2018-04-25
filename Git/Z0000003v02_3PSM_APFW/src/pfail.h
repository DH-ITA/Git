/**
 ******************************************************************************
 * \file pfail.h
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
#ifndef PFAIL_H_
#define PFAIL_H_

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include <stdint.h>
#include "compiler.h"
#include "target.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */

/*===========================================================================*\
 * Minimum time to consider a phase failure
\*===========================================================================*/
#define PFAIL_MIN_POWER_FAILURE_PERIOD			3000

/*===========================================================================*\
 * Minimum time to consider a phase restoration
\*===========================================================================*/
#define PFAIL_MIN_RESTORATION_PERIOD			3000

/*===========================================================================*\
 * Power Failure Pin Definitions (Analog circuit)
\*===========================================================================*/
#define PFAIL_POWER_FAIL_PIN					PIO_PA19_IDX
#define PFAIL_POWER_FAIL_PIO					PIOA
#define PFAIL_POWER_FAIL_PIO_ID					ID_PIOA
#define PFAIL_POWER_FAIL_PIN_BIT_MASK			PIO_PA19
#define PFAIL_POWER_FAIL_IRQn					PIOA_IRQn

/*===========================================================================*\
 * 3 Phase Zero Crossing Pin Definitions
\*===========================================================================*/
#define PFAIL_ZC_PIN							PIO_PA16_IDX
#define PFAIL_ZC_PIO							PIOA
#define PFAIL_ZC_PIO_ID							ID_PIOA
#define PFAIL_ZC_PIN_BIT_MASK					PIO_PA16
#define PFAIL_ZC_IRQn							PIOA_IRQn

/*===========================================================================*\
 * Phase A Zero Crossing Pin Definitions
\*===========================================================================*/
#define PFAIL_ZC_PHA_PIN						PIO_PB11_IDX
#define PFAIL_ZC_PHA_PIO						PIOB
#define PFAIL_ZC_PHA_PIO_ID						ID_PIOB
#define PFAIL_ZC_PHA_PIN_BIT_MASK				PIO_PB11
#define PFAIL_ZC_PHA_IRQn						PIOB_IRQn

/*===========================================================================*\
 * Phase B Zero Crossing Pin Definitions
\*===========================================================================*/
#define PFAIL_ZC_PHB_PIN						PIO_PB10_IDX
#define PFAIL_ZC_PHB_PIO						PIOB
#define PFAIL_ZC_PHB_PIO_ID						ID_PIOB
#define PFAIL_ZC_PHB_PIN_BIT_MASK				PIO_PB10
#define PFAIL_ZC_PHB_IRQn						PIOB_IRQn

/*===========================================================================*\
 * Phase C Zero Crossing Pin Definitions
\*===========================================================================*/
#define PFAIL_ZC_PHC_PIN						PIO_PB9_IDX
#define PFAIL_ZC_PHC_PIO						PIOB
#define PFAIL_ZC_PHC_PIO_ID						ID_PIOB
#define PFAIL_ZC_PHC_PIN_BIT_MASK				PIO_PB9
#define PFAIL_ZC_PHC_IRQn						PIOB_IRQn

/*===========================================================================*\
 * Phase A, B & C Zero Crossing Pin Definitions
\*===========================================================================*/
#define PFAIL_ZC_PHA_PHB_PHC_PIO				PIOB
#define PFAIL_ZC_PHA_PHB_PHC_PIO_ID				ID_PIOB
#define PFAIL_ZC_PHA_PHB_PHC_PIN_BIT_MASK		(PFAIL_ZC_PHA_PIN_BIT_MASK | PFAIL_ZC_PHB_PIN_BIT_MASK | PFAIL_ZC_PHC_PIN_BIT_MASK)
#define PFAIL_ZC_PHA_PHB_PHC_IRQn				PIOB_IRQn

/*===========================================================================*\
 * Brownout Voltage Levels Options
\*===========================================================================*/
#define PFAIL_TH_RESET_1_9V						0
#define PFAIL_TH_RESET_2_0V						1
#define PFAIL_TH_RESET_2_1V						2
#define PFAIL_TH_RESET_2_2V						3
#define PFAIL_TH_RESET_2_3V						4
#define PFAIL_TH_RESET_2_4V						5
#define PFAIL_TH_RESET_2_5V						6
#define PFAIL_TH_RESET_2_6V						7
#define PFAIL_TH_RESET_2_7V						8
#define PFAIL_TH_RESET_2_8V						9
#define PFAIL_TH_RESET_2_9V						10
#define PFAIL_TH_RESET_3_0V						11
#define PFAIL_TH_RESET_3_1V						12
#define PFAIL_TH_RESET_3_2V						13
#define PFAIL_TH_RESET_3_3V						14
#define PFAIL_TH_RESET_3_4V						15

/*===========================================================================*\
 * Power Failure task config
\*===========================================================================*/
#define PFAIL_STACK_SIZE						configMINIMAL_STACK_SIZE * 2
#define PFAIL_APP_vTask_PRIO					tskIDLE_PRIORITY + 4

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
typedef enum pf_ms_timer_id_type {

	PFAIL_POWER_BACK_TIMER = 0,
	PFAIL_PHA_FAILURE_TIMER,
	PFAIL_PHB_FAILURE_TIMER,
	PFAIL_PHC_FAILURE_TIMER,
	PFAIL_PHA_RESTORATION_TIMER,
	PFAIL_PHB_RESTORATION_TIMER,
	PFAIL_PHC_RESTORATION_TIMER,
	PFAIL_ZC_TIMER,
	PFAIL_PHA_ZC_TIMER,
	PFAIL_PHB_ZC_TIMER,
	PFAIL_PHC_ZC_TIMER,
	NUMBER_OF_PF_MS_TIMERS

} PF_MS_TIMER_ID_TYPE;

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
extern void PFAIL_APP_vTask(void);
extern void PFAIL_APP_vInitialize(void);
extern void PFAIL_APP_vCreate_task(void);
extern void PFAIL_APP_v3ph_failure_detection(void);
extern void PFAIL_APP_vRegister_power_up(void);
extern void PFAIL_APP_vRegister_power_down(void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-08-13
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
#endif /* PFAIL_H_ */