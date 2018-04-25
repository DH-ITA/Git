/**
 * \file
 *
 * \brief Board configuration.
 *
 * Copyright (c) 2012-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

/** UART0 pins (UTXD0 and URXD0) definitions, PB4,5. */
#define PINS_UART0			(PIO_PB4A_URXD0 | PIO_PB5A_UTXD0)
#define PINS_UART0_FLAGS	(PIO_PERIPH_A | PIO_DEFAULT)

#define PINS_UART0_MASK 	PIO_PB4A_URXD0|PIO_PB5A_UTXD0
#define PINS_UART0_PIO 		PIOB
#define PINS_UART0_ID 		ID_PIOB
#define PINS_UART0_TYPE 	PIO_PERIPH_A
#define PINS_UART0_ATTR 	PIO_DEFAULT

#define SOCKET_UART		UART0
#define SOCKET_UART_ID		ID_UART0

//--- UART1 pins (UTXD1 and URXD1) definitions, PC1,0. ---//
#define PINS_UART1        	(PIO_PC1A_URXD1 | PIO_PC0A_UTXD1)
#define PINS_UART1_FLAGS    (PIO_PERIPH_C | PIO_DEFAULT)

#define PINS_UART1_MASK 	PIO_PC1A_URXD1 | PIO_PC0A_UTXD1
#define PINS_UART1_PIO 		PIOC
#define PINS_UART1_ID 		ID_PIOC
#define PINS_UART1_TYPE 	PIO_PERIPH_C
#define PINS_UART1_ATTR 	PIO_DEFAULT

#define	USER_UART				UART1
#define	USER_UART_ID			ID_UART1

//RGB LED pin
#define GPIO_GREEN_PIN		(PIO_PA9_IDX)
#define GPIO_RGB_FLAGS    (PIO_OUTPUT_1 | 0 | PIO_DEBOUNCE | 0)

#define LED_GREEN_MASK		 PIO_PA9
#define RGB_PIO			 PIOA
#define RGB_ID			 ID_PIOA
#define RGB_TYPE			 PIO_OUTPUT_1
#define RGB_ATTR		 0 | PIO_DEBOUNCE | 0

#define GPIO_BLUE_PIN		(PIO_PA10_IDX)
#define LED_BLUE_MASK		 PIO_PA10

#define GPIO_RED_PIN		(PIO_PA11_IDX)
#define LED_RED_MASK		 PIO_PA11

/* The timer period. */
#define mainSOFTWARE_TIMER_RATE                 (500 / portTICK_RATE_MS)
#define one_minute_time							(60000 / portTICK_RATE_MS)

/* A block time of 0 ticks simply means "don't block". */
#define mainDONT_BLOCK                          (0)

/* Stack definition */
#define TASK_METROLOGY_REFRESH_STACK		(configMINIMAL_STACK_SIZE * 1)
/* Task priority definition */
#define TASK_METROLOGY_REFRESH_PRIO			(tskIDLE_PRIORITY + 1)

/** Enable Com Port. */
#define CONF_BOARD_UART_SOCKET
#define CONF_BOARD_UART_USER

/** Watchdog period 3000ms */
#define WDT_PERIOD					3000
/** Watchdog restart 2000ms */
#define WDT_RESTART_PERIOD			2000

#define CONF_BOARD_ENABLE_COPROCESSOR

//! [tc_define_peripheral]
/* Use TC Peripheral 0. */
#define TC             TC0
#define TC_PERIPHERAL  0
//! [tc_define_peripheral]

//! [tc_define_ch1]
/* Configure TC0 channel 1 as waveform output. */
#define TC_CHANNEL_WAVEFORM 1
#define ID_TC_WAVEFORM      ID_TC1
#define PIN_TC_WAVEFORM     PIN_TC0_TIOA1
#define PIN_TC_WAVEFORM_MUX PIN_TC0_TIOA1_MUX
//! [tc_define_ch1]

//! [tc_define_ch2]
/* Configure TC0 channel 2 as capture input. */
#define TC_CHANNEL_CAPTURE 2
#define ID_TC_CAPTURE ID_TC2
#define PIN_TC_CAPTURE PIN_TC0_TIOA2
#define PIN_TC_CAPTURE_MUX PIN_TC0_TIOA2_MUX
//! [tc_define_ch2]

//! [tc_define_irq_handler]
/* Use TC2_Handler for TC capture interrupt. */
#define TC_Handler  TC2_Handler
#define TC_IRQn     TC2_IRQn
//! [tc_define_irq_handler]

/*oscilator settings*/
#define BOARD_FREQ_SLCK_XTAL        (32768U)
#define BOARD_FREQ_SLCK_BYPASS      (32768U)
#define BOARD_FREQ_MAINCK_XTAL      (8192000U)
#define BOARD_FREQ_MAINCK_BYPASS    (8192000U)

/** Master clock frequency */
#define BOARD_MCK                   CHIP_FREQ_CPU_MAX
/** board main clock xtal statup time */
#define BOARD_OSC_STARTUP_US        15625U

#endif /* CONF_BOARD_H_INCLUDED */
