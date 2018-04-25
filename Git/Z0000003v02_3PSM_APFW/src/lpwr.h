/**
 ******************************************************************************
 * \file lpwr.h
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
 *   - Revision: 01
 *   - Date: 1/OCT/2016
 *   - Author: Mariano Centeno Camarena.
 ******************************************************************************
 *
 * @defgroup File change log
 * @section FILE_CHANGE_LOG FILE CHANGE LOG:
 *
 * Top to bottom log: last revision to first revision.
 *
 ******************************************************************************
 * - Revision 01 / Jacob Anaya Candia / 2017-10-11
 *   - Change 1:  PCR#: Description.
 *   - Change 2:  PCR#: Description.
 ******************************************************************************
 */
#ifndef LPWR_H_
#define LPWR_H_

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "asf.h"
#include "stdio_serial.h"
 
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/

/**
 * \name UART0 pis (UTXD0 and URXD0) definitions
 * @{
 */
#define PINS_UART0					(PIO_PB4A_URXD0 | PIO_PB5A_UTXD0)
#define PINS_UART0_FLAGS			(IOPORT_MODE_MUX_A)

#define PINS_UART0_PORT				IOPORT_PIOB
#define PINS_UART0_MASK				(PIO_PB4A_URXD0 | PIO_PB5A_UTXD0)
#define PINS_UART0_PIO				PIOB
#define PINS_UART0_ID				ID_PIOB
#define PINS_UART0_TYPE				PIO_PERIPH_A
#define PINS_UART0_ATTR				PIO_DEFAULT

#define CONSOLE_UART_ID				ID_UART0
#define CONSOLE_UART				UART0								// UART Interface
#define CONSOLE_UART_BAUDRATE		115200UL							// Baud rate setting
#define CONSOLE_UART_PARITY			UART_MR_PAR_NO						// Parity setting
#define CONSOLE_UART_PIO			PINS_UART0_PIO						// Configure UART pins PIO
#define CONSOLE_PINS_UART			(PIO_PB4A_URXD0 | PIO_PB5A_UTXD0)	// Configure UART pins
#define CONSOLE_PINS_UART_FLAGS		PIO_PERIPH_A						// Configure UART pins flags





#define BOARD_NAME "SAM4C-EK"

/**
 * Push button definitions for sleep mode and active mode
 * @{
 */
#define PIN_PUSHBUTTON_WAKEUP_PIO    PIOA
#define PIN_PUSHBUTTON_WAKEUP_MASK   PIO_PA18
#define PIN_PUSHBUTTON_WAKEUP_ID     ID_PIOA
#define PIN_PUSHBUTTON_WAKEUP_ATTR   (PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE)
/** @} */

/** Wakeup pin for wait mode: FWUP pin */
#define WAKEUP_WAIT_INPUT_ID    (1u << 4)

/** Hint message for active mode */
#define STRING_ACTIVE \
		"Enter into active mode.\n\r" \
		"- Press SCROLL_UP button to exit.\n\r"

/** Hint message for sleep mode */
#define STRING_SLEEP \
		"Enter into sleep mode.\n\r" \
		"- Press SCROLL_UP button to wake up.\n\r"

/** Hint message for wait mode */
#define STRING_WAIT \
		"Enter into wait mode.\n\r" \
		"- Switch to 4MHz Fast RC oscillator, PLL stopped.\n\r" \
		"- Press SCROLL_UP button to wake up.\n\r"

/** Hint message for backup mode */
#define STRING_BACKUP \
		"Enter into backup mode.\n\r" \
		"- Press FWUP button to wake up.\n\r"

#define CLOCK_LIST_MENU \
		"\n\rSelect one of the following clock configurations:\n\r" \
		"  1: 125KHz from Fast RC\n\r"                     \
		"  2: 250KHz from Fast RC\n\r"                     \
		"  3: 500KHz from Fast RC\n\r"                     \
		"  4: 1MHz from Fast RC\n\r"                       \
		"  5: 2MHz from Fast RC\n\r"                       \
		"  6: 4MHz from Fast RC\n\r"                       \
		"  7: 8MHz from Fast RC\n\r"                       \
		"  8: 12MHz from Fast RC\n\r"                      \
		"  a: 24MHz from PLL clock\n\r"                    \
		"  b: 32MHz from PLL clock\n\r"                    \
		"  c: 48MHz from PLL clock\n\r"                    \
		"  d: 64MHz from PLL clock\n\r"                    \
		"  e: 84MHz from PLL clock\n\r"                    \
		"  f: 100MHz from PLL clock\n\r"                   \
		"  g: 120MHz from PLL clock\n\r"                   \

#define MIN_CLOCK_FAST_RC_ITEM '1'
#define MAX_CLOCK_FAST_RC_ITEM '8'
#define MIN_CLOCK_PLL_ITEM     'a'
#define MAX_CLOCK_PLL_ITEM     'g'

#define PLL_DEFAULT_MUL  23
#define PLL_DEFAULT_DIV  2
#define MCK_DEFAULT_DIV  PMC_MCKR_PRES_CLK_4

#define example_switch_clock(a, b, c, d) \
	do {                                 \
		pmc_enable_pllbck(a, b, c);      \
		pmc_switch_mck_to_pllbck(d);     \
	} while (0)

#define example_set_wakeup_from_backup_mode()			supc_set_wakeup_mode(SUPC, SUPC_WUMR_RTCEN_ENABLE)

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
 * Fast RC Clock Types
\*===========================================================================*/
typedef enum {
	
	FAST_RC_CLOCK_125KHz = 0,
	FAST_RC_CLOCK_250KHz,
	FAST_RC_CLOCK_500KHz,
	FAST_RC_CLOCK_1MHz,
	FAST_RC_CLOCK_2MHz,
	FAST_RC_CLOCK_4MHz,
	FAST_RC_CLOCK_8MHz,
	FAST_RC_CLOCK_12MHz

} FAST_RC_CLOCK_TYPE;

/*===========================================================================*\
 * PLL Clock Types
\*===========================================================================*/
typedef enum {

	PLL_CLOCK_24MHz = 0,
	PLL_CLOCK_32MHz,
	PLL_CLOCK_48MHz,
	PLL_CLOCK_64MHz,
	PLL_CLOCK_84MHz,
	PLL_CLOCK_100MHz,
	PLL_CLOCK_120MHz

} PLL_CLOCK_TYPE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
 extern uint32_t g_fastrc_clock_list[][3];
 extern uint32_t g_pll_clock_list[][4];

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/
extern void LOW_POWER_APP_vConsole_task(void);
extern void LOW_POWER_APP_vEnable_sleep_mode(void);
extern void test_backup_mode(void);
extern void LPWR_APP_vBackup_mode(void);

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
 #endif /* LPWR_H_ */