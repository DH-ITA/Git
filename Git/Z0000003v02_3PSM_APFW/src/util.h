/**
 ******************************************************************************
 * \file 	util.h
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Softek Global eDesign S. de R.L. de C.V. All rights reserved.</b><br><br>
 * Softek Global eDesign Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup UTILITY Utility Function Library
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all standard code functions exportables for Utility Module using ATxmega core.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 *   - SIMCEGAB - Sistema de Medicion de Energia Electrica Centralizado en Gabinete.
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *     - CFE: Specification CFE G0100-05. Marzo 2010.
 *
 *   - Applicable Standards:
 *     - Elster: Communication Protocol for Option Boards. Rev B.
 *     - Elster: Third party communication board design specification. Rev D.
 *     - Elster: Communications Interface: Third-Party Accessible Tables and Functions. Rev A.
 *     - Elster: MF-262 Table. Rev 03 [2012-10-01].
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - Format: F-P06-DS-03
 *   - Revision: 01
 *   - Date: 2013-06-22
 *   - Author: Mariano Centeno Camarena.
 ******************************************************************************
 */
#ifndef  UTIL_H
#define  UTIL_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "target.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */

/*===========================================================================*\
 * Brief description for each Define
\*===========================================================================*/
#define BIT0_P			0
#define BIT1_P			1
#define BIT2_P			2
#define BIT3_P			3
#define BIT4_P			4
#define BIT5_P			5
#define BIT6_P			6
#define BIT7_P			7
#define BIT8_P			8
#define BIT9_P			9
#define BIT10_P			10
#define BIT11_P			11
#define BIT12_P			12
#define BIT13_P			13
#define BIT14_P			14
#define BIT15_P			15

#define BIT0_M			0x0001
#define BIT1_M			0x0002
#define BIT2_M			0x0004
#define BIT3_M			0x0008
#define BIT4_M			0x0010
#define BIT5_M			0x0020
#define BIT6_M			0x0040
#define BIT7_M			0x0080
#define BIT8_M			0x0100
#define BIT9_M			0x0200
#define BIT10_M			0x0400
#define BIT11_M			0x0800
#define BIT12_M			0x1000
#define BIT13_M			0x2000
#define BIT14_M			0x4000
#define BIT15_M			0x8000

#define SHIFT1			1
#define SHIFT2			2
#define SHIFT3			3
#define SHIFT4			4
#define SHIFT5			5
#define SHIFT6			6
#define SHIFT7			7
#define SHIFT8			8

#define HIGH_NIBBLE		0xF0
#define LOW_NIBBLE		0x0F

#define  FALSE			0
#define  TRUE           (!false)

/*===========================================================================*\
 * Exact Width integer types
\*===========================================================================*/
#define UINT8_T_MIN		(0)
#define UINT8_T_MAX		(255)
#define UINT16_T_MIN	(0)
#define UINT16_T_MAX	(65535)
#define UINT32_T_MIN	(0)
#define UINT32_T_MAX	(4294967295U)

#define INT8_T_MIN		(-128)
#define INT8_T_MAX		(127)
#define INT16_T_MIN		(-32768)
#define INT16_T_MAX		(32767)
#define INT32_T_MIN		(-2147483648)
#define INT32_T_MAX		(2147483647)

#define	TYPE_UINT16		0
#define	TYPE_UINT32		1
#define	TYPE_UINT64		2

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
 */

/*===========================================================================*\
 * Brief description for each Macro
\*===========================================================================*/

/*
 ******************************************************************************
 * Global Enumerations, Structures and Typedefs                          
 ******************************************************************************
 */
typedef void (*void_fptr) (void);               // void void Function pointer typedef

typedef struct uint8_t_to_bits                           /* generic type to access uint8_t in BIT size */
{
	uint8_t Bit0 : 1;
	uint8_t Bit1 : 1;
	uint8_t Bit2 : 1;
	uint8_t Bit3 : 1;
	uint8_t Bit4 : 1;
	uint8_t Bit5 : 1;
	uint8_t Bit6 : 1;
	uint8_t Bit7 : 1;
}UINT8_T_TO_BITS;

typedef struct uint16_t_to_bits                           /* generic type to access uint16_t in BIT size */
{
	uint8_t Bit0 : 1;
	uint8_t Bit1 : 1;
	uint8_t Bit2 : 1;
	uint8_t Bit3 : 1;
	uint8_t Bit4 : 1;
	uint8_t Bit5 : 1;
	uint8_t Bit6 : 1;
	uint8_t Bit7 : 1;
	uint8_t Bit8 : 1;
	uint8_t Bit9 : 1;
	uint8_t Bit10 : 1;
	uint8_t Bit11 : 1;
	uint8_t Bit12 : 1;
	uint8_t Bit13 : 1;
	uint8_t Bit14 : 1;
	uint8_t Bit15 : 1;
}UINT16_T_TO_BITS;

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
extern uint8_t UTIL_limit_value (uint8_t value, uint8_t minv, uint8_t maxv);
extern uint8_t UTIL_wrap_value(uint8_t value, uint8_t minv, uint8_t maxv);
extern uint8_t UTIL_scale_value_8(uint8_t input, uint8_t old_min, uint8_t old_max, uint8_t new_min, uint8_t new_max);
extern uint16_t UTIL_scale_value_16(uint16_t input, uint16_t old_min, uint16_t old_max, uint16_t new_min, uint16_t new_max);
extern uint8_t UTIL_bcd_to_hex (uint8_t bcd_value);
extern bool UTIL_hex_to_bcd(uint64_t hex_value, uint8_t *bcd_outputBuffer);
extern unsigned char UTIL_nibble_to_ascii(uint8_t nibble);
extern uint8_t UTIL_ascii_to_nibble(unsigned char ascii_value);
extern void UTIL_array_bcd_to_ascii(uint8_t *bcd_pointer, uint8_t *ascii_pointer, uint8_t length);
extern bool UTIL_compare_array(uint8_t *array_1_ptr, uint8_t *array_2_ptr, uint8_t array_length);
extern uint8_t UTIL_matrix_to_slot(uint8_t column, uint8_t row);
extern void UTIL_little_to_big_endian (uint8_t *to, uint8_t *from, size_t num_bytes);
extern uint8_t UTIL_auth_code(const uint8_t *ptr_data, uint16_t num_bytes);

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
