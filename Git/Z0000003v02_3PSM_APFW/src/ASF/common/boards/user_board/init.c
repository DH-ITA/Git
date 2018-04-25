/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

#define ioport_set_pin_peripheral_mode(pin, mode) \
do {\
	ioport_set_pin_mode(pin, mode);\
	ioport_disable_pin(pin);\
} while (0)

void board_init(void)
{
	wdt_disable(WDT);
	
	ioport_init();
	
#ifdef CONF_BOARD_UART_SOCKET
	ioport_set_pin_peripheral_mode( PIO_PB4_IDX, IOPORT_MODE_MUX_A );
	ioport_set_pin_peripheral_mode( PIO_PB5_IDX, IOPORT_MODE_MUX_A );
#endif
/* Configure UART1 pins */
#ifdef CONF_BOARD_UART_USER
	ioport_set_pin_peripheral_mode( PIO_PC0_IDX, IOPORT_MODE_MUX_A);
	ioport_set_pin_peripheral_mode( PIO_PC1_IDX, IOPORT_MODE_MUX_A);
#endif

	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
}
