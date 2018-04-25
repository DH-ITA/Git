/**
 ******************************************************************************
 * \file lpwr.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Eos Tech S. de A. de C.V. All rights reserved.</b><br><br>
 * Eos Tech Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup File content title
 * @section LOW POWER MODULE:
 *
 * \section Purpose
 *
 * This example shows all the different low power modes with several types
 * of wake-up sources. And the consumption of the core in different power
 * modes can be measured.
 *
 * \section Requirements
 *
 * This package can be used with SAM evaluation kits.
 *
 * \section Description
 *
 * The program will display a menu on console. It allows users to change the
 * configuration and enter into a different power mode, and then measure the
 * power consumption.
 *
 * For Eks, an amperemeter has to be plugged on the board instead of the
 * VDDx jumper.
 *
 * Note that for better consumption measurement:
 * - Run program out of flash without ICE connected.
 *
 * \section Usage
 *
 * -# Build the program and download it into the evaluation board.
 * -# On the computer, open and configure a terminal application
 *    (e.g., HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 *
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
 *   - Change 1:  PCR#: Description: Initial emission
 *   - Change 2:  PCR#: Description.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
 #include "lpwr.h"
 #include "asf.h"
 #include "stdio_serial.h"
 #include "conf_board.h"
 #include "conf_clock.h"
 #include "conf_uart_serial.h"
 #include "led.h"

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
 * Enumerations and Structures and Typedefs
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each structure and typedefs
\*===========================================================================*/

/*
 ******************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ******************************************************************************
 */
 /** Clock list from fast RC */
uint32_t g_fastrc_clock_list[][3] = {
	/* MCK,    FastRC,                  Prescaler */
	{125000,   CKGR_MOR_MOSCRCF_4_MHz,  PMC_MCKR_PRES_CLK_32},
	{250000,   CKGR_MOR_MOSCRCF_4_MHz,  PMC_MCKR_PRES_CLK_16},
	{500000,   CKGR_MOR_MOSCRCF_4_MHz,  PMC_MCKR_PRES_CLK_8},
	{1000000,  CKGR_MOR_MOSCRCF_4_MHz,  PMC_MCKR_PRES_CLK_4},
	{2000000,  CKGR_MOR_MOSCRCF_4_MHz,  PMC_MCKR_PRES_CLK_2},
	{4000000,  CKGR_MOR_MOSCRCF_4_MHz,  PMC_MCKR_PRES_CLK_1},
	{8000000,  CKGR_MOR_MOSCRCF_8_MHz,  PMC_MCKR_PRES_CLK_1},
	{12000000, CKGR_MOR_MOSCRCF_12_MHz, PMC_MCKR_PRES_CLK_1}
};

/** Clock list from PLL */
uint32_t g_pll_clock_list[][4] = {
	/* MCK, MUL, DIV, PRES */
	/* MCK = 8000000 * (11+1) / 1 / 4 = 24 MHz */
	{24000000, 11, 1, PMC_MCKR_PRES_CLK_4},
	/* MCK = 8000000 * (11+1) / 1 / 3 = 32 MHz */
	{32000000, 11, 1, PMC_MCKR_PRES_CLK_3},
	/* MCK = 8000000 * (11+1) / 1 / 2 = 48 MHz */
	{48000000, 11, 1, PMC_MCKR_PRES_CLK_2},
	/* MCK = 8000000 * (15+1) / 1 / 2 = 64 MHz */
	{64000000, 15, 1, PMC_MCKR_PRES_CLK_2},
	/* MCK = 8000000 * (20+1) / 2 / 1 = 84 MHz */
	{84000000, 20, 2, PMC_MCKR_PRES_CLK_1},
	/* MCK = 8000000 * (24+1) / 2 / 1 = 100 MHz */
	{100000000, 24, 2, PMC_MCKR_PRES_CLK_1},
	/* MCK = 8000000 * (29+1) / 2 / 1 = 120 MHz */
	{120000000, 29, 2, PMC_MCKR_PRES_CLK_1}
};

/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
 */
 /** Current MCK in Hz */
uint32_t g_ul_current_mck;

/** Button pressed flag */
volatile uint32_t g_ul_button_pressed = 0;

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

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
*/

#if !defined(PMC_PCK_PRES_CLK_1)
#define PMC_PCK_PRES_CLK_1   PMC_PCK_PRES(0)
#define PMC_PCK_PRES_CLK_2   PMC_PCK_PRES(1)
#define PMC_PCK_PRES_CLK_4   PMC_PCK_PRES(2)
#define PMC_PCK_PRES_CLK_8   PMC_PCK_PRES(3)
#define PMC_PCK_PRES_CLK_16  PMC_PCK_PRES(4)
#define PMC_PCK_PRES_CLK_32  PMC_PCK_PRES(5)
#define PMC_PCK_PRES_CLK_64  PMC_PCK_PRES(6)
#endif

#define STRING_EOL    "\r"
#define STRING_HEADER "-- Low Power Example --\r\n" \
	"-- "BOARD_NAME " --\r\n" \
	"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

#ifndef PLL_DEFAULT_MUL
#define PLL_DEFAULT_MUL  7
#endif

#ifndef PLL_DEFAULT_DIV
#define PLL_DEFAULT_DIV  1
#endif

#ifndef MCK_DEFAULT_DIV
#define MCK_DEFAULT_DIV							PMC_MCKR_PRES_CLK_4
#endif

#ifndef example_switch_clock
#define example_switch_clock(a, b, c, d) \
	do {                                 \
		pmc_enable_pllack(a, b, c);      \
		pmc_switch_mck_to_pllack(d);     \
	} while (0)
#endif

#ifndef example_disable_pll
#define example_disable_pll()					pmc_disable_pllack()
#endif

#ifndef example_set_wakeup_from_wait_mode
#define example_set_wakeup_from_wait_mode()		pmc_set_fast_startup_input(WAKEUP_WAIT_INPUT_ID)
#endif

#ifndef example_set_wakeup_from_backup_mode
#define example_set_wakeup_from_backup_mode()	supc_set_wakeup_mode(SUPC, SUPC_WUMR_RTCEN_ENABLE)
#endif

/**
******************************************************************************
* \fn void set_default_working_clock(void)
* \brief Set default clock (MCK = 24MHz).
*
* \param 
* \retval
******************************************************************************
*/
static void set_default_working_clock(void)
{
	/* Save current clock */
	g_ul_current_mck = g_pll_clock_list[6][0];

	/* Switch MCK to slow clock  */
	pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);

	/* Switch mainck to external xtal */
	pmc_switch_mainck_to_xtal(0, BOARD_OSC_STARTUP_US);

	/* Configure PLL and switch clock */
	example_switch_clock(g_pll_clock_list[6][1], PLL_COUNT, g_pll_clock_list[6][2], g_pll_clock_list[6][3]);

	/* Disable unused clock to save power */
	pmc_osc_disable_fastrc();
}

/**
******************************************************************************
* \fn void init_specific_board(void)
* \brief
*
* \param 
* \retval
******************************************************************************
*/
void init_specific_board(void)
{
	/* Configure all PIOs as inputs to save power */
	pio_set_input(PIOA, 0xFFFFFFFF, PIO_PULLUP);
	pio_set_input(PIOB, 0xFFFFFFFF, PIO_PULLUP);

	/* Enable the PMC clocks of push button for wakeup */
	pmc_enable_periph_clk(ID_PIOA);

	NVIC_SetPriority( PIOA_IRQn, 0 );
}

/**
******************************************************************************
* \fn void configure_console(void)
* \brief Configure UART console.
*
* \param
* \retval
******************************************************************************
*/
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONSOLE_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONSOLE_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	pio_configure_pin_group(CONSOLE_UART_PIO, CONSOLE_PINS_UART, CONSOLE_PINS_UART_FLAGS);
	stdio_serial_init(CONSOLE_UART, &uart_serial_options);
}

/**
******************************************************************************
* \fn void reconfigure_console(void)
* \brief Reconfigure UART console for changed MCK and baudrate.
*
* \param
* \retval
******************************************************************************
*/
static void reconfigure_console(void)
{
	ioport_set_pin_mode(PIO_PB4_IDX, IOPORT_MODE_MUX_A);
	ioport_disable_pin(PIO_PB4_IDX);
	
	ioport_set_pin_mode(PIO_PB5_IDX, IOPORT_MODE_MUX_A);
	ioport_disable_pin(PIO_PB5_IDX);
	
	Pdc *g_p_uart_pdc1;
	sam_uart_opt_t uart_console_settings;

	uart_console_settings.ul_mck = g_ul_current_mck;				//sysclk_get_cpu_hz();				// MCK for UART
	uart_console_settings.ul_baudrate = CONSOLE_UART_BAUDRATE;		// Expected baud rate.
	uart_console_settings.ul_chmode = UART_MR_CHMODE_NORMAL;		// Configure channel mode (Normal, Automatic, Local_loopback or Remote_loopback)
	uart_console_settings.ul_mode = UART_MR_PAR_NO;					// Initialize value for UART mode register
	
	/* Configure PMC. */
	pmc_enable_periph_clk( CONSOLE_UART_ID );
	
	/* Configure UART. */
	uart_init( CONSOLE_UART_ID, &uart_console_settings );
	
	/* Get board UART0 PDC base address and enable receiver and transmitter. */
	g_p_uart_pdc1 = uart_get_pdc_base( CONSOLE_UART_ID );
	pdc_enable_transfer( g_p_uart_pdc1, PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN );
	
	/* Enable the receiver and transmitter. */
	uart_enable(CONSOLE_UART_ID);
}

/**
******************************************************************************
* \fn void init_chip(void)
* \brief 
*
* \param
* \retval
******************************************************************************
*/
static void init_chip(void)
{
	/* Wait for the transmission done before changing clock */
	while (!uart_is_tx_empty(CONSOLE_UART)) {
	}

	/* Disable all the peripheral clocks */
	pmc_disable_all_periph_clk();

	/* Disable brownout detector */
	supc_disable_brownout_detector(SUPC);

	/* Initialize the specific board */
	init_specific_board();
}

/**
******************************************************************************
* \fn void LOW_POWER_APP_vEnable_fast_rc_clock(FAST_RC_CLOCK_TYPE fast_rc_clock)
* \brief Low power application entry point.
*
* \param
* \retval Unused (ANSI-C compatibility).
******************************************************************************
*/
void LOW_POWER_APP_vEnable_fast_rc_clock(FAST_RC_CLOCK_TYPE fast_rc_clock)
{
	/* Save current clock */
	g_ul_current_mck = g_fastrc_clock_list[(uint8_t)fast_rc_clock][0];

	/* Switch MCK to Slow clock  */
	pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);

	/* Switch mainck to fast RC */
	pmc_osc_enable_fastrc(CKGR_MOR_MOSCRCF_8_MHz);
	pmc_switch_mainck_to_fastrc(g_fastrc_clock_list[(uint8_t)fast_rc_clock][1]);

	/* Switch MCK to mainck */
	pmc_switch_mck_to_mainck(g_fastrc_clock_list[(uint8_t)fast_rc_clock][2]);

	/* Disable unused clock to save power */
	pmc_osc_disable_xtal(0);
	example_disable_pll();
}

/**
******************************************************************************
* \fn void LOW_POWER_APP_vEnable_pll_clock(PLL_CLOCK_TYPE pll_rc_clock)
* \brief Low power application entry point.
*
* \param
* \retval Unused (ANSI-C compatibility).
******************************************************************************
*/
void LOW_POWER_APP_vEnable_pll_clock(PLL_CLOCK_TYPE pll_rc_clock)
{
	/* Save current clock */
	g_ul_current_mck = g_pll_clock_list[(uint8_t)pll_rc_clock][0];

	/* Switch MCK to slow clock  */
	pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);

	/* Switch mainck to external xtal */
	pmc_switch_mainck_to_xtal(0, BOARD_OSC_STARTUP_US);

	/* Configure PLL and switch clock */
	example_switch_clock(g_pll_clock_list[(uint8_t)pll_rc_clock][1], PLL_COUNT, g_pll_clock_list[(uint8_t)pll_rc_clock][2], g_pll_clock_list[(uint8_t)pll_rc_clock][3]);

	/* Disable unused clock to save power */
	pmc_osc_disable_fastrc();
}

/**
******************************************************************************
* \fn void button_handler(void)
* \brief Handler for button interrupt.
*
* \param
* \retval
******************************************************************************
*/
void button_handler(void)
{
	g_ul_button_pressed = 1;
}

/**
******************************************************************************
* \fn void configure_button(void)
* \brief Configure the push button. Configure the PIO as inputs and generate
*        corresponding interrupt when pressed or released.
*
* \param
* \retval
******************************************************************************
*/
static void configure_button(void)
{
	/* Adjust PIO debounce filter parameters, using 10 Hz filter. */
	pio_set_debounce_filter(PIN_PUSHBUTTON_WAKEUP_PIO, PIN_PUSHBUTTON_WAKEUP_MASK, 10);

	/* Initialize PIO interrupt handlers, see PIO definition in board.h. */
	pio_configure_interrupt(PIN_PUSHBUTTON_WAKEUP_PIO, PIN_PUSHBUTTON_WAKEUP_ID, PIN_PUSHBUTTON_WAKEUP_MASK);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)PIN_PUSHBUTTON_WAKEUP_ID);

	/* Enable PIO line interrupts. */
	pio_enable_interrupt(PIN_PUSHBUTTON_WAKEUP_PIO, PIN_PUSHBUTTON_WAKEUP_MASK);
}

/**
******************************************************************************
* \fn uint32_t fib(uint32_t ul_n)
* \brief Fibonacci calculation.
*
* \param ul_n Number N.
* \retval
******************************************************************************
*/
static uint32_t fib(uint32_t ul_n)
{
	if (ul_n < 2) {
		return ul_n;
	} else {
		return fib(ul_n - 1) + fib(ul_n - 2);
	}
}

/**
******************************************************************************
* \fn void test_active_mode(void)
* \brief Test active mode.
*
* \param
* \retval
******************************************************************************
*/
static void test_active_mode(void)
{
	/* Configure button for exiting from active mode */
	configure_button();

	g_ul_button_pressed = 0;

	uint8_t uc_key;
	uint32_t ul_id;

	/* Print menu */
	puts(CLOCK_LIST_MENU);

	scanf("%c", (char *)&uc_key);
	printf("Select option is: %c\n\r\n\r", uc_key);

	if (STRING_ACTIVE) {
		puts((char const *)STRING_ACTIVE);
	}

	/* Wait for the transmission done before changing clock */
	while (!uart_is_tx_empty(CONSOLE_UART)) {
	}

	if ( (uc_key >= MIN_CLOCK_FAST_RC_ITEM) && (uc_key <= MAX_CLOCK_FAST_RC_ITEM) ) {

		ul_id = uc_key - MIN_CLOCK_FAST_RC_ITEM;
		LOW_POWER_APP_vEnable_fast_rc_clock( (FAST_RC_CLOCK_TYPE)ul_id );
	}
	else if ( (uc_key >= MIN_CLOCK_PLL_ITEM) && (uc_key <= MAX_CLOCK_PLL_ITEM) ) {

		ul_id = uc_key - MIN_CLOCK_PLL_ITEM;
		LOW_POWER_APP_vEnable_pll_clock( (PLL_CLOCK_TYPE)ul_id );
	}
	else {
		puts("Clock is not changed.\r");
	}


	/* Test active mode */
	do {
		/* Run Fibonacci calculation, n = 10 (may be changed) */
		fib(10);
	} while (!g_ul_button_pressed);

	/* Set default clock and re-configure UART */
	//set_default_working_clock();
	reconfigure_console();

	puts("Exit from active mode.\r");
}

/**
******************************************************************************
* \fn void test_sleep_mode(void)
* \brief Test sleep Mode.
*
* \param
* \retval
******************************************************************************
*/
static void test_sleep_mode(void)
{
	/* Configure button for waking up sleep mode */
	configure_button();
	
	uint8_t uc_key;
	uint32_t ul_id;

	/* Print menu */
	puts(CLOCK_LIST_MENU);

	scanf("%c", (char *)&uc_key);
	printf("Select option is: %c\n\r\n\r", uc_key);

	if (STRING_SLEEP) {
		puts((char const *)STRING_SLEEP);
	}

	/* Wait for the transmission done before changing clock */
	while (!uart_is_tx_empty(CONSOLE_UART)) {
	}

	/* Disable UART */
	pmc_disable_periph_clk(CONSOLE_UART_ID);

	if ( (uc_key >= MIN_CLOCK_FAST_RC_ITEM) && (uc_key <= MAX_CLOCK_FAST_RC_ITEM) ) {

		ul_id = uc_key - MIN_CLOCK_FAST_RC_ITEM;
		LOW_POWER_APP_vEnable_fast_rc_clock( (FAST_RC_CLOCK_TYPE)ul_id );
	}
	else if ( (uc_key >= MIN_CLOCK_PLL_ITEM) && (uc_key <= MAX_CLOCK_PLL_ITEM) ) {

		ul_id = uc_key - MIN_CLOCK_PLL_ITEM;
		LOW_POWER_APP_vEnable_pll_clock( (PLL_CLOCK_TYPE)ul_id );
	}
	else {
		puts("Clock is not changed.\r");
	}

	/* Enter into sleep Mode */
	pmc_enable_sleepmode(0);

	/* Set default clock and re-configure UART */
	set_default_working_clock();
	reconfigure_console();

	RTC->RTC_IER = RTC_IER_SECEN | RTC_IER_ALREN;

	puts("Exit from sleep Mode.\r");
}

/**
******************************************************************************
* \fn void LOW_POWER_APP_vEnable_sleep_mode(void)
* \brief Test sleep Mode.
*
* \param
* \retval
******************************************************************************
*/
void LOW_POWER_APP_vEnable_sleep_mode(void)
{
	uint32_t ul_id;
	uint8_t uc_key = 101;



	if ( (uc_key >= MIN_CLOCK_FAST_RC_ITEM) && (uc_key <= MAX_CLOCK_FAST_RC_ITEM) ) {

		ul_id = uc_key - MIN_CLOCK_FAST_RC_ITEM;
		LOW_POWER_APP_vEnable_fast_rc_clock( (FAST_RC_CLOCK_TYPE)ul_id );
	}
	else if ( (uc_key >= MIN_CLOCK_PLL_ITEM) && (uc_key <= MAX_CLOCK_PLL_ITEM) ) {

		ul_id = uc_key - MIN_CLOCK_PLL_ITEM;
		LOW_POWER_APP_vEnable_pll_clock( (PLL_CLOCK_TYPE)ul_id );
	}
	else {
		puts("Clock is not changed.\r");
	}

	/* Enter into sleep Mode */
	pmc_enable_sleepmode(1);
}

/**
******************************************************************************
* \fn void test_wait_mode(void)
* \brief Test wait mode.
*
* \param
* \retval
******************************************************************************
*/
static void test_wait_mode(void)
{
	puts(STRING_WAIT);

	/* Wait for the transmission done before changing clock */
	while (!uart_is_tx_empty(CONSOLE_UART)) {
	}

	/* Configure fast RC oscillator */
	pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);
	pmc_switch_mainck_to_fastrc(CKGR_MOR_MOSCRCF_4_MHz);
	pmc_switch_mck_to_mainck(PMC_PCK_PRES_CLK_1);

	g_ul_current_mck = 4000000; /* 4MHz */

	/* Disable unused clock to save power */
	pmc_osc_disable_xtal(0);
	example_disable_pll();

	/* Set wakeup input for fast startup */
	example_set_wakeup_from_wait_mode();

	/* Enter into wait Mode */
	pmc_enable_waitmode();

	/* Set default clock and re-configure UART */
	set_default_working_clock();
	reconfigure_console();

	puts("Exit from wait Mode.\r");
}

/**
******************************************************************************
* \fn void test_backup_mode(void)
* \brief Test backup mode. To test backup mode, the program must run out of flash.
*
* \param
* \retval
******************************************************************************
*/
void test_backup_mode(void)
{
	puts(STRING_BACKUP);

	/* Wait for the transmission done before changing clock */
 	while (!uart_is_tx_empty(CONSOLE_UART)) {
 	}

	/* GPBR0 is for recording times of entering into backup mode */
	//gpbr_write(GPBR0, gpbr_read(GPBR0) + 1);

	/* Enable the PIO for wake-up */
	/* Enable the PIO for wake-up */
	supc_set_wakeup_mode(SUPC, SUPC_WUMR_WKUPDBC_IMMEDIATE);
	supc_set_wakeup_inputs(SUPC, SUPC_WUIR_WKUPEN4_ENABLE, SUPC_WUIR_WKUPT4_LOW);

	/* Switch MCK to slow clock  */
	pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);

	/* Disable unused clock to save power */
	pmc_osc_disable_xtal(0);
	example_disable_pll();

	/* Enter into backup mode */
	pmc_enable_backupmode();

	/* Note: The core will reset when exiting from backup mode. */
}

/**
******************************************************************************
* \fn void display_menu_core(void)
* \brief Display test core menu.
*
* \param
* \retval
******************************************************************************
*/
static void display_menu_core(void)
{
	printf("\n\r");
	printf("===============================================\n\r");
	printf("Menu: press a key to continue.\n\r");
	printf("===============================================\n\r");
	printf("Configure:\n\r");
	printf("  F : 128-bit flash access\n\r");
	printf("  G : 64-bit flash access\n\r");
	printf("Mode:\n\r");
	printf("  A : Active Mode\n\r");
	printf("  S : Sleep Mode\n\r");
	printf("  W : Wait Mode\n\r");
#if (!(SAMG51 || SAMG53 || SAMG54))
	//printf("  B : Backup Mode(Entered %d times).\n\r", (int)gpbr_read(GPBR0));
#endif
	printf("Quit:\n\r");
	printf("  Q : Quit test.\n\r");

	printf("\n\r");
	printf("-----------------------------------------------\n\r");
	printf("Current configuration:\n\r");
	printf("  CPU Clock         : MCK=%d Hz\n\r", (int)g_ul_current_mck);
	if ((efc_get_flash_access_mode(EFC) & EEFC_FMR_FAM) == EEFC_FMR_FAM) {
		printf("  Flash access mode : 64-bit\n\r");
	} else {
		printf("  Flash access mode : 128-bit\n\r");
	}

	printf("-----------------------------------------------\n\r");
	printf("\n\r");
}

/**
******************************************************************************
* \fn void test_core(void)
* \brief Test Core consumption.
*
* \param
* \retval
******************************************************************************
*/
static void test_core(void)
{
	uint8_t uc_key = 0;

	while (1) {

		/* Display menu */
		display_menu_core();

		/* Read a key from console */
		scanf("%c", (char *)&uc_key);

		switch (uc_key) {
		/* Configuration */
		case 'f':
		case 'F':
			efc_set_flash_access_mode(EFC, 0); /* 128-bit */
			break;

		case 'g':
		case 'G':
			efc_set_flash_access_mode(EFC, EEFC_FMR_FAM); /* 64-bit */
			break;

		/* Mode test */
		case 'a':
		case 'A':
			test_active_mode();
			break;

		case 's':
		case 'S':
			test_sleep_mode();
			break;

		case 'w':
		case 'W':
			test_wait_mode();
			break;

		case 'b':
		case 'B':
			test_backup_mode();
			break;

		/* Quit test */
		case 'q':
		case 'Q':
			goto test_core_end;

		default:
			puts("This menu does not exist !\r");
			break;
		}       /* Switch */
	}

test_core_end:
	puts(" Exit from core consumption test mode.\r");
}

/**
******************************************************************************
* \fn void LOW_POWER_APP_vConsole_task(void)
* \brief Low power application entry point.
*
* \param
* \retval Unused (ANSI-C compatibility).
******************************************************************************
*/
void LOW_POWER_APP_vConsole_task(void)
{
 	/* Initialize the SAM system */
 	//sysclk_init();
 	g_ul_current_mck = sysclk_get_cpu_hz();
 	board_init();

 	/* Initialize the console uart */
 	configure_console();
 
 	/* Output example information */
 	puts(STRING_HEADER);
 	
 	/* Initialize the chip for the power consumption test */
 	init_chip();
 	
 	/* Set default clock and re-configure UART */
 	//set_default_working_clock();
 	reconfigure_console();
	ioport_init();

 	/* Test core consumption */
 	test_core();
 
 	while (1) {
		WATCHDOG_APP_vRestart();
 	}
}

/**
******************************************************************************
* \fn void test_backup_mode(void)
* \brief Test backup mode. To test backup mode, the program must run out of flash.
*
* \param
* \retval
******************************************************************************
*/
void LPWR_APP_vBackup_mode(void)
{
	
	// 1. Depending on the application, set the PIO lines in the correct mode and configuration (input pull-up or pulldown, output low or high levels).

	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 24), IOPORT_DIR_OUTPUT );
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 24), true );

	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 25), IOPORT_DIR_OUTPUT );
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 25), true );

	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOA, 26), IOPORT_DIR_OUTPUT );
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOA, 26), true );
	
	ioport_set_pin_dir( IOPORT_CREATE_PIN(PIOC, 1), IOPORT_DIR_OUTPUT );
	ioport_set_pin_level( IOPORT_CREATE_PIN(PIOC, 1), false );

	// 2. Disable the Main Crystal Oscillator (enabled by SAM-BA boot if the device is booting from ROM).

	/* Switch MCK to slow clock  */
	pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);

	/* Disable unused clock to save power */
	pmc_osc_disable_xtal(0);
	pmc_disable_all_pck();

	// 3. Configure PA30/PA31 (XIN/XOUT) into PIO mode depending on their use.

	// 4. Disable the JTAG lines using the SFR1 register in Matrix 0 (by default, internal pull-up or pull-down is disabled on JTAG lines).
	
	// 5. Enable the RTT in 1 Hz mode.
	// 6. Disable Normal mode of the RTT (RTT will run in 1 Hz mode).
	RTT->RTT_MR = 0;
	RTT->RTT_MR = RTT_MR_RTC1HZ;

	// 7. Disable the POR backup if not needed. This reduces power consumption.
	SUPC->SUPC_MR &= ~SUPC_MR_BUPPOREN;

	// 8. Disable the Core brownout detector.
	supc_disable_brownout_reset(SUPC);

	/* Enable Wake-up Input 4*/
	supc_set_wakeup_mode(SUPC, SUPC_WUMR_WKUPDBC_IMMEDIATE);
	supc_set_wakeup_inputs(SUPC, SUPC_WUIR_WKUPEN4_ENABLE, SUPC_WUIR_WKUPT4_HIGH);

	/* Enter into backup mode */
	pmc_enable_backupmode();

	/* Note: The core will reset when exiting from backup mode. */
}

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */