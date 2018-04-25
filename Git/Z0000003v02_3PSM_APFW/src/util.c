/**
 ******************************************************************************
 * \file util.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Softek Global eDesign S. de R.L. de C.V. All rights reserved.</b><br><br>
 * Softek Global eDesign Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup UTILITY Utility Library
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all standard code functions for Utility Library using ATxmega core.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 *   - SIMCEGAB - Sistema de Medicion de Energia Electrica Centralizado en Gabinete.
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *     - CFE: Especificacion CFE G0100-05. Marzo 2010.
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
 *   - Format: F-P06-DS-02
 *   - Revision: 01
 *   - Date: 2013-06-22
 *   - Author: Mariano Centeno Camarena.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "target.h"
#include "util.h"

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
/*===========================================================================*\
 * Brief description for each Constant/Macro
\*===========================================================================*/

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
 * Enumerations and Structures and Typedefs
 ******************************************************************************
*/

/*
************************************************************************
* Global and Const Variable Defining Definitions / Initializations
************************************************************************
*/

/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
*/

/*
 ******************************************************************************
 * Static Function Prototypes for Private Functions with File Level Scope
 ******************************************************************************
*/

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
 * \fn	uint8_t UTIL_limit_value (uint8_t value, uint8_t minv, uint8_t maxv)
 * \brief 	Forces value to be within min and max inclusive 
 *
 * \param 	value, min, and max
 * \retval 	Limit checked value
 ******************************************************************************
 */
uint8_t UTIL_limit_value (uint8_t value, uint8_t minv, uint8_t maxv)
{
   if (value < minv)
   {
      value = minv;
   }
   else if (value > maxv)
   {
      value = maxv;
   }
   return(value);
}

/** 
 ******************************************************************************
 * \fn	uint8_t UTIL_wrap_value(uint8_t value, uint8_t minv, uint8_t maxv)
 * \brief 	Wraps a value if out of limits (i.e > max goes to min)
 *
 * \param 	value, min, and max
 * \retval 	Wrapped value
 ******************************************************************************
 */
uint8_t UTIL_wrap_value(uint8_t value, uint8_t minv, uint8_t maxv)
{
   if (minv > value)
   {
      value = maxv;
   }
   else if (maxv < value)
   {
      value = minv;
   }
   return(value);
}


/** 
 ******************************************************************************
 * \fn	uint8_t int Scale_8(uint8_t input, uint8_t old_min, uint8_t old_max, uint8_t new_min, uint8_t new_max)
 * \brief 	Scales a value from one range to another.
 *
 * \param 	input
 *			value to be scaled
 * \param   old_min
 *			minimum value of old range
 * \param   old_max
 *			maximum value of old range
 * \param   new_min
 *			minimum value of new scaled range
 * \param   new_max
 *			maximum value of new scaled range
 * \retval 	Scaled value
 ******************************************************************************
 */
uint8_t UTIL_scale_value_8(uint8_t input, uint8_t old_min, uint8_t old_max, uint8_t new_min, uint8_t new_max)
{
   int old_range = old_max - old_min;

   return(((((input - old_min) * (new_max - new_min)) + (old_range/2)) / old_range) + new_min);
}

/** 
 ******************************************************************************
 * \fn	uint8_t int Scale_16(uint8_t input, uint8_t old_min, uint8_t old_max, uint8_t new_min, uint8_t new_max)
 * \brief 	Scales a value from one range to another.
 *
 * \param 	input
 *			value to be scaled
 * \param   old_min
 *			minimum value of old range
 * \param   old_max
 *			maximum value of old range
 * \param   new_min
 *			minimum value of new scaled range
 * \param   new_max
 *			maximum value of new scaled range
 * \retval 	Scaled value
 ******************************************************************************
 */
uint16_t UTIL_scale_value_16(uint16_t input, uint16_t old_min, uint16_t old_max, uint16_t new_min, uint16_t new_max)
{
   uint32_t old_range = (uint32_t)(old_max - old_min);
   uint32_t new_range = (uint32_t)(new_max - new_min);
   volatile uint32_t result;
   
   result = (((((input - old_min) * (new_range)) + (old_range/2)) / old_range) + new_min);
   
   return (uint16_t)result;
}

/** 
 ******************************************************************************
 * \fn	uint8_t UTIL_bcd_to_hex (uint8_t bcd_value)
 * \brief 	This routine converts a received value in BCD and converts it to a Hexidecimal value.
 *
 * \param 	BCD_Value
 *			BCD value to be convereed to hex
 * \retval 	Equivalent value in hexidecimal format
 ******************************************************************************
 */
uint8_t UTIL_bcd_to_hex(uint8_t bcd_value)
{
   return((uint8_t) (((bcd_value / 16) * 10) + (bcd_value % 16)));
}

/** 
 ******************************************************************************
 * \fn	bool UTIL_hex_to_bcd(uint64_t hex_value, uint8_t *bcd_outputBuffer)
 * \brief 	
 *
 * \param 	
 * \retval 	
 ******************************************************************************
 */
bool UTIL_hex_to_bcd(uint64_t hex_value, uint8_t *bcd_outputBuffer)
{	
	uint8_t i;
	uint8_t bcd_value[8];
	
	memset( &bcd_value, 0, sizeof(bcd_value) );
	
	while( hex_value > (uint64_t)(99999999999999) )
	{
		bcd_value[0] += 1;
		hex_value -= 100000000000000;
		if ( (bcd_value[0] & 0xF) > 9 )
		{
			bcd_value[0] += 6;
		}
	}

	while( hex_value > (uint64_t)(999999999999) )
	{
		bcd_value[1] += 1;
		hex_value -= 1000000000000;
		if ( (bcd_value[1] & 0xF) > 9 )
		{
			bcd_value[1] += 6;
		}
	}

	while( hex_value > (uint64_t)(9999999999) )
	{
		bcd_value[2] += 1;
		hex_value -= 10000000000;
		if ( (bcd_value[2] & 0xF) > 9 )
		{
			bcd_value[2] += 6;
		}
	}

	while( hex_value > (uint64_t)(99999999) )
	{
		bcd_value[3] += 1;
		hex_value -= 100000000;
		if ( (bcd_value[3] & 0xF) > 9 )
		{
			bcd_value[3] += 6;
		}
	}

	while( hex_value > (uint64_t)(999999) )
	{
		bcd_value[4] += 1;
		hex_value -= 1000000;
		if ( (bcd_value[4] & 0xF) > 9 )
		{
			bcd_value[4] += 6;
		}
	}

	while (hex_value > 9999)
	{
		bcd_value[5] += 1;
		hex_value -= 10000;
		if ( (bcd_value[5] & 0xF) > 9 )
		{
			bcd_value[5] += 6;
		}
	}

	while (hex_value > 99)
	{
		bcd_value[6] += 1;
		hex_value -= 100;
		if ( (bcd_value[6] & 0xF) > 9 )
		{
			bcd_value[6] += 6;
		}
	}

	while (hex_value > 9)
	{
		bcd_value[7] += 0x10;
		hex_value -= 10;
	}
	
	bcd_value[7] += hex_value;
	
	for ( i = 0; i < 8; i++)
	{
		*bcd_outputBuffer++ = bcd_value[i];
	}
}

/** 
 ******************************************************************************
 * \fn	unsigned char UTIL_nibble_to_ascii(uint8_t nibble)
 * \brief 	Convert an binary nibble to the corresponding hex-ASCII-character
 *
 * \param 	nibble
 *			binary nibble to convert: 0x00.. 0x0F. Must NOT be > 0x0F
 * \retval 	Conversion result: '0'..'F' or '\n' conversion error
 ******************************************************************************
 */
unsigned char UTIL_nibble_to_ascii(uint8_t nibble)
{
	nibble = nibble & 0x0F;
	
	if (nibble < 0x0A)
	{
		return ( nibble + 0x30 );
	}
	else
	{
		return ( nibble + 0x37 );
	}
}

/** 
 ******************************************************************************
 * \fn	uint8_t UTIL_ascii_to_nibble(unsigned char ascii_value)
 * \brief Converts an hex value presented as ASCII-characters to an binary nibble
 *
 * \param	ascii_value
 *			'0' to '9' and 'A' to 'F' and 'a' to 'f'
 * \retval 	0x00..0x0F converted nibble or 0xFF conversion error
 ******************************************************************************
 */
uint8_t UTIL_ascii_to_nibble(unsigned char ascii_value)
{
	if ( ascii_value >= '0' && ascii_value <= '9' )
	{
		return ascii_value - 0x30;
	}	
	else if ( ascii_value >= 'A' && ascii_value <='F' )
	{
		return ascii_value - 0x37;
	}	
	else if (ascii_value >= 'a' && ascii_value<='f')
	{
		return ascii_value - 0x57;
	}	
	return 0xff;
}

/** 
 ******************************************************************************
 * \fn void UTIL_array_bcd_to_ascii(uint8_t *bcd_pointer, uint8_t *ascii_pointer, uint8_t length)
 * \brief 
 *
 * \param	
 *			
 * \retval 	
 ******************************************************************************
 */
void UTIL_array_bcd_to_ascii(uint8_t *bcd_pointer, uint8_t *ascii_pointer, uint8_t length)
{
	do 
	{ 
		*ascii_pointer = *bcd_pointer & 0xF0;
		*ascii_pointer >> 4;
		*ascii_pointer += 30;
		
		ascii_pointer++;
		*ascii_pointer = *bcd_pointer & 0x0F;
		*ascii_pointer += 30;
		
		ascii_pointer++;
		bcd_pointer++;
	}
	while (--length);
}

/** 
 ******************************************************************************
 * \fn bool UTIL_array_compare(uint8_t *array_1_ptr, uint8_t *array_2_ptr, uint8_t array_length)
 * \brief 
 *
 * \param	
 *			
 * \retval 	
 ******************************************************************************
 */
bool UTIL_compare_array(uint8_t *array_1_ptr, uint8_t *array_2_ptr, uint8_t array_length)
{
	bool compare_ok = false;
	
	for (uint8_t i = 0; i < array_length; i++)
	{
		if ( *array_1_ptr == *array_2_ptr)
		{
			array_1_ptr++;
			array_2_ptr++;
			if ( i == (array_length - 1) )
			{
				compare_ok = true;
			}
		}
		else
		{
			i = array_length;
		}
	}
	
	return compare_ok;
}


/** 
 ******************************************************************************
 * \fn uint8_t UTIL_matrix_to_slot(uint8_t column, uint8_t row)
 * \brief 
 *
 * \param	
 *			
 * \retval 	
 ******************************************************************************
 */
uint8_t UTIL_matrix_to_slot(uint8_t column, uint8_t row)
{
	uint8_t slot = 0xFF;
	
	if ( (row > '0') && (row < '5') )
	{
		slot = row;
		slot &= 0x0F;
	
		switch ( column )
		{
			case 'a':
			case 'A':
				slot--;
				break;
	
			case 'b':
			case 'B':
				slot = slot + 4;
				slot--;
				break;
		
			case 'c':
			case 'C':
				slot = slot + 8;
				slot--;
				break;
			
			default:
				slot = 0xFF;
				break;
		}
	}
	
	return slot;
}

/**
 ******************************************************************************
 * \fn uint8_t UTIL_auth_code(const uint8_t *ptr_data, uint16_t num_bytes)
 * \brief Computes the 8-bit authentication code of the specified data.
 *
 * \param ptr_data Pointer to data
 * \param num_bytes The length of the data
 * \retval auth_code The CRC of the data
 ******************************************************************************
 */
uint8_t UTIL_auth_code(const uint8_t *ptr_data, uint16_t num_bytes)
{  
	static volatile uint8_t auth_code; 
	static volatile uint8_t i, data_sum, data_xor;
	
    data_sum = (uint8_t)num_bytes;
	data_xor = (uint8_t)num_bytes;
    
	for ( i = 0; i < num_bytes; i++ )
	{
		data_sum += ptr_data[i];
		data_xor ^= ptr_data[i];
	}
	
	auth_code = (uint8_t)(1 - (data_sum + data_xor));
	return auth_code;
}
/** 
 ******************************************************************************
 * \fn void UTIL_little_to_big_endian (uint8_t *to, uint8_t *from, size_t num_bytes)
 * \brief Converts a half_word or word from Little Endian to Big Endian
 *
 * \param *from
 *		  pointer to half_word or word to be converted
 * \param *to
 *        pointer to destination of the conversion
 * \para num_bytes
 *        # of bytes to convert
 *			
 * \retval 	
 ******************************************************************************
 */
void UTIL_little_to_big_endian (uint8_t *to, uint8_t *from, size_t num_bytes)
{
   char *temp_to = (char *)to;
   const char *temp_from = ((const char *)from) + num_bytes;

   for ( ; 0 != num_bytes ; num_bytes--)
   {
      *(temp_to++) = *(--temp_from);
   }
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

