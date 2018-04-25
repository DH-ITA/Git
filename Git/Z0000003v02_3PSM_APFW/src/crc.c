/**
 ******************************************************************************
 * \file crc.h
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
#include "flash.h"

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
 * Brief description for each constant definition
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

/*===========================================================================*\
 * Brief description for each structure and typedefs
\*===========================================================================*/

/*
 ******************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
 */

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

/**
 ******************************************************************************
 * \fn uint8_t CRC_crc8(const uint8_t *ptr_data, uint16_t num_bytes)
 * \brief Computes the 8-bit two complement CRC of the specified data.
 *
 * \param pData Pointer to data
 * \param num_bytes The length of the data
 * \retval crc The CRC of the data
 ******************************************************************************
 */
uint8_t CRC_crc8(const uint8_t *ptr_data, uint16_t num_bytes)
{  
	static volatile uint16_t crc; 
	static volatile uint8_t i;
	
    crc = 0;    // Reset crc
    
	for ( i = 0; i < num_bytes; i++ )
	{
		crc += ptr_data[i];
	}
	crc = (~crc) + 1;
	
	return crc;
}

/**
 ******************************************************************************
 * \fn uint8_t CRC_crc4(uint8_t *pData, uint8_t numBytes)
 * \brief Computes the 4-bit CRC of the specified data.
 *
 * \param pData Pointer to data
 * \param num_bytes The length of the data
 * \retval crc4 The CRC of the data
 ******************************************************************************
 */
uint8_t CRC_crc4(uint8_t *pData, uint8_t num_bytes)
{
	volatile uint8_t crc4;
	volatile uint8_t old_crc4 = 0;
	volatile uint8_t data;
	
	for (uint8_t i = 1; i <= num_bytes; i++)
	{
		crc4 = 0;
		data = *pData++;
		crc4 = crc4 | (((data>>3)&0x01)^(data&0x01)^(old_crc4 & 0x01)^((old_crc4>>3)&0x01));
		crc4 = crc4 | ((((data>>3)&0x01)^((data>>1)&0x01)^(data & 0x01)^(old_crc4 & 0x01)^((old_crc4>>1)&0x01)^((old_crc4>>3)&0x01))<<1);
		crc4 = crc4 | ((((data>>2)&0x01)^((data>>1)&0x01)^((old_crc4>>1)&0x01)^((old_crc4>>2)&0x01))<<2);
		crc4 = crc4 | ((((data>>3)&0x01)^((data>>2)&0x01)^((old_crc4>>2)&0x01)^((old_crc4>>3)&0x01))<<3);
	
		old_crc4 = crc4;
	}
	return crc4;
}

/**
 ******************************************************************************
 * \fn uint16_t CRC_crc12(const uint8_t *pData, uint16_t num_bytes)
 * \brief Computes the 12-bit CRC of the specified data. 
 *                                                   11   12   3    2
 *        This is the CCITT CRC 16 polynomial X  + X  + X  + X  + X + 1. 
 *        This works out to be 0x80F, but the way the algorithm works 
 *        lets us use 0xF01 (the reverse of the bit pattern).  The high 
 *        bit is always assumed to be set, thus we only use 12 bits to 
 *        represent the 13 bit value. 
 * \param pData Pointer to data
 * \param num_bytes The length of the data
 * \retval crc12 The CRC of the data
 ******************************************************************************
 */
uint16_t CRC_crc12(const uint8_t *ptr_data, uint16_t num_bytes)
{ 
	volatile uint16_t crc; 
	volatile uint8_t i, byte, temp;
  
	crc = 0x0fff; 
	do 
	{ 
		for (i = 0, byte = *ptr_data++; i < 8; i++, byte >>= 1) 
		{ 
			temp = (uint8_t) ((uint8_t) crc & (uint8_t) 0x01); 
			crc >>= 1; 
			if (temp ^ (byte & 0x01)) 
			crc ^= 0x0f01; 
		} 
	} 
	while (--num_bytes);
  
	return crc;
}

/**
 ******************************************************************************
 * \fn uint16_t CRC_crc16(const uint8_t *pData, uint16_t num_bytes)
 * \brief Computes the 16-bit CRC of the specified data.
 *
 *                                              16   12   5
 *        This is the CCITT CRC 16 polynomial X  + X  + X  + 1.
 *        This works out to be 0x1021, but the way the algorithm works
 *        lets us use 0x8408 (the reverse of the bit pattern).  The high
 *        bit is always assumed to be set, thus we only use 16 bits to
 *        represent the 17 bit value.
 * \param pData Pointer to data
 * \param num_bytes The length of the data
 * \retval crc12 The CRC of the data
 ******************************************************************************
 */
uint16_t CRC_crc16(const uint8_t *pData, uint16_t num_bytes)
{  
  static volatile uint16_t crc; 
  static volatile uint8_t i, byte, temp;
	
    crc = 0xffff;    // Reset crc
    do 
    { 
        for (i = 0, byte = *pData++; i < 8; i++, byte >>= 1) 
        { 
            temp = (uint8_t) ((uint8_t) crc & (uint8_t) 0x01); 
            crc >>= 1; 
            if (temp ^ (byte & 0x01)) 
            crc ^= 0x8408; 
        }
    } 
    while (--num_bytes); 
  
  crc = ~((crc << 8) | (crc >> 8));
  return crc;
}

/**
 ******************************************************************************
 * \fn uint8_t CRC_2s_complement(const uint8_t *pData, uint16_t num_bytes)
 * \brief Computes the 2's complement CRC of the specified data.
 *
 * \param pData Pointer to data
 * \param num_bytes The length of the data
 * \retval crc The CRC of the data
 ******************************************************************************
 */
uint8_t CRC_2s_complement(const uint8_t *pData, uint16_t num_bytes)
{  
	static volatile uint8_t crc; 
	static volatile uint16_t i;
	
    crc = 0;    // Reset crc

    for (i = 0; i < num_bytes; i++) 
    { 
        crc += *pData++;
    }

	crc = 0xFF - crc + 1;
	return crc;
}
/**
 ******************************************************************************
 * \fn uint32_t CRC_image_crc16_calc(void)
 * \brief Calculate de CRC of the main application , and compare to the CRC that the app has at the end of the code, 
 * /in case the app crc do not coincide with the calculated one the app will not run 
 * \param pData Pointer to data
 * \param num_bytes The length of the data
 * \retval crc The CRC of the data
 ******************************************************************************
 */
extern uint32_t CRC_image_crc16_calc(void)
{
	static volatile uint16_t crc;
	static volatile uint8_t i, byte, temp;
	static bool do_crc_task;
	static volatile uint32_t crc16_flash_calc;
	static volatile uint8_t end_app[10] = {0,0,0,0,0,0,0,0,0,0};
    static volatile uint8_t crc_buffer[8] = {0,0,0,0,0,0,0,0};
    static uint8_t var_1; 
    static bool last_turn = false;        
    static volatile uint32_t data_flash;
    static uint8_t flash_buffer[4];
    static volatile uint8_t  j;
    static uint32_t app_address;
    static uint32_t data_count;
	static uint32_t flash_null_values_counter= 0x00000000;

    //***verify update integrity        
    data_count = 0x6000; /*24 pages 1kbyte*/
    app_address = IMAGE_APP_START_ADDR;
    data_flash  = (*(__IO uint32_t *)app_address);   
	
	do_crc_task = true;
    last_turn = false;
	crc = 0xffff;
	
	do
	{		
		 data_flash  = (*(__IO uint32_t *)app_address); 
		 flash_buffer[3]= (uint8_t)(data_flash >> 24);
		 flash_buffer[2]= (uint8_t)(data_flash >> 16);
		 flash_buffer[1]= (uint8_t)(data_flash >> 8);       
		 flash_buffer[0]= (uint8_t)(data_flash); 
		
		  if(last_turn == false)
		  {
		 
		     for(j=0;j<4;j++)
		     {
		           
		          for (i = 0, byte = flash_buffer[j] ; i < 8; i++, byte >>= 1) 
		          {                  
		             temp = (uint8_t) ((uint8_t) crc & (uint8_t) 0x01); 
		             crc >>= 1; 
		             if (temp ^ (byte & 0x01)) 
		             crc ^= 0x8408;               
		         //IWDG_refresh();                
		          }                
		          end_app[0] = end_app[1];
				  end_app[1] = end_app[2];
				  end_app[2] = end_app[3];
				  end_app[3] = end_app[4];
				  end_app[4] = end_app[5];
		          end_app[5] = end_app[6];
		          end_app[6] = end_app[7];
		          end_app[7] = flash_buffer[j];
		          
		          if ((end_app[0] == ' ') &(end_app[1] == ' ') & (end_app[2] == 'E') && (end_app[3] == 'N') && (end_app[4] == 'D') && (end_app[5] == 'A') && (end_app[6] == 'P') && (end_app[7] == 'P'))
		          {
			          crc = ~((crc << 8) | (crc >> 8));
			          crc16_flash_calc = (uint32_t)(((uint32_t)crc)<<16);
			          last_turn = true;
			          var_1 = j;
			          j=4;
		          }
		     }
		   }
		  
		   else
		   {
		      end_app[0] = end_app[4];
			  end_app[1] = end_app[5];
			  end_app[2] = end_app[6];
			  end_app[3] = end_app[7];
			  end_app[4] = flash_buffer[0];
		      end_app[5] = flash_buffer[1];
		      end_app[6] = flash_buffer[2];
		      end_app[7] = flash_buffer[3];
		      do_crc_task = false;              
		      
		      crc16_flash_calc = crc16_flash_calc | (uint32_t)((uint32_t) end_app[var_1+3]<<8);
		      crc16_flash_calc = crc16_flash_calc | (uint32_t) end_app[var_1+2];
		   }
		          
		app_address = app_address + 4;
		data_count = data_count - 4;
		
		if (data_flash == 0xFFFFFFFF)
		{
		  flash_null_values_counter++;
		}  
		else
		{
		  flash_null_values_counter = 0;
		}
		
		if (flash_null_values_counter >= 0x80)
		{
			 crc = ~((crc << 8) | (crc >> 8));
		     crc16_flash_calc = (uint32_t)(((uint32_t)crc)<<16);
			 crc16_flash_calc = crc16_flash_calc | 0x00;
		     crc16_flash_calc = crc16_flash_calc | 0x00;
			 return crc16_flash_calc;
		}
	}
	while ( (do_crc_task == true) && (app_address < IMAGE_APP_END_ADDR));
	return crc16_flash_calc;	
}

/**
 ******************************************************************************
 * \fn uint32_t CRC_app_crc16_calc(void)
 * \brief Calculate de CRC of the main application , and compare to the CRC that the app has at the end of the code, 
 * /in case the app crc do not coincide with the calculated one the app will not run 
 * \param pData Pointer to data
 * \param num_bytes The length of the data
 * \retval crc The CRC of the data
 ******************************************************************************
 */
extern uint32_t CRC_app_crc16_calc(void)
{
	static volatile uint16_t crc;
	static volatile uint8_t i, byte, temp;
	static bool do_crc_task;
	static volatile uint32_t crc16_flash_calc;
	static volatile uint8_t end_app[10] = {0,0,0,0,0,0,0,0,0,0};
    static volatile uint8_t crc_buffer[8] = {0,0,0,0,0,0,0,0};
    static uint8_t var_1; 
    static bool last_turn = false;        
    static uint32_t data_flash;
    static uint8_t flash_buffer[4];
    static volatile uint8_t  j;
    static uint32_t app_address;
    static uint32_t data_count;
	static uint32_t flash_null_values_counter= 0x00000000;

    //***verify update integrity        
    data_count = 0x6000; /*24 pages 1kbyte*/
    app_address = MAIN_APP_START_ADDR;
    data_flash  = (*(__IO uint32_t *)app_address);   
	
	do_crc_task = true;
    last_turn = false;
	crc = 0xffff;
	
	do
	{		
		 data_flash  = (*(__IO uint32_t *)app_address); 
		 flash_buffer[3]= (uint8_t)(data_flash >> 24);
		 flash_buffer[2]= (uint8_t)(data_flash >> 16);
		 flash_buffer[1]= (uint8_t)(data_flash >> 8);       
		 flash_buffer[0]= (uint8_t)(data_flash); 
		
		  if(last_turn == false)
		  {
		 
		     for(j=0;j<4;j++)
		     {
		           
		          for (i = 0, byte = flash_buffer[j] ; i < 8; i++, byte >>= 1) 
		          {                  
		             temp = (uint8_t) ((uint8_t) crc & (uint8_t) 0x01); 
		             crc >>= 1; 
		             if (temp ^ (byte & 0x01)) 
		             crc ^= 0x8408;               
		            //IWDG_refresh();                
		          }                
		          end_app[0] = end_app[1];
				  end_app[1] = end_app[2];
				  end_app[2] = end_app[3];
				  end_app[3] = end_app[4];
				  end_app[4] = end_app[5];
		          end_app[5] = end_app[6];
		          end_app[6] = end_app[7];
		          end_app[7] = flash_buffer[j];
		          
		         if ((end_app[0] == ' ') &(end_app[1] == ' ') & (end_app[2] == 'E') && (end_app[3] == 'N') && (end_app[4] == 'D') && (end_app[5] == 'A') && (end_app[6] == 'P') && (end_app[7] == 'P'))
		          {
		            crc = ~((crc << 8) | (crc >> 8));
		            crc16_flash_calc = (uint32_t)(((uint32_t)crc)<<16);
		            last_turn = true;
		            var_1 = j;                     
		            j=4;
		          }
		     }
		   }
		  
		   else
		   {
		      end_app[0] = end_app[4];
			  end_app[1] = end_app[5];
			  end_app[2] = end_app[6];
			  end_app[3] = end_app[7];
			  end_app[4] = flash_buffer[0];
		      end_app[5] = flash_buffer[1];
		      end_app[6] = flash_buffer[2];
		      end_app[7] = flash_buffer[3];
		      do_crc_task = false;              
		      
		      crc16_flash_calc = crc16_flash_calc | (uint32_t)((uint32_t) end_app[var_1+3]<<8);
		      crc16_flash_calc = crc16_flash_calc | (uint32_t) end_app[var_1+2];
		   }
		          
		app_address = app_address + 4;
		data_count = data_count - 4;
		
		if (data_flash == 0xFFFFFFFF)
		{
		  flash_null_values_counter++;
		}  
		else
		{
		  flash_null_values_counter = 0;
		}
		
		if (flash_null_values_counter >= 0x80)
		{
			 crc = ~((crc << 8) | (crc >> 8));
		     crc16_flash_calc = (uint32_t)(((uint32_t)crc)<<16);
			 crc16_flash_calc = crc16_flash_calc | 0x00;
		     crc16_flash_calc = crc16_flash_calc | 0x00;
			 return crc16_flash_calc;
		}
	}
	while ( (do_crc_task == true) && (app_address < MAIN_APP_END_ADDR));
	return crc16_flash_calc;	
}

/**
 ******************************************************************************
 * \fn uint16_t CRC_crc16(const uint8_t *pData, uint16_t num_bytes)
 * \brief Computes the 16-bit CRC of the specified data.
 *
 *                                              16   12   5
 *        This is the CCITT CRC 16 polynomial X  + X  + X  + 1.
 *        This works out to be 0x1021, but the way the algorithm works
 *        lets us use 0x8408 (the reverse of the bit pattern).  The high
 *        bit is always assumed to be set, thus we only use 16 bits to
 *        represent the 17 bit value.
 * \param pData Pointer to data
 * \param num_bytes The length of the data
 * \retval crc12 The CRC of the data
 ******************************************************************************
 */
uint16_t CRC_crc16_c59x(const uint8_t *pData, uint16_t num_bytes)
{  
  static volatile uint16_t k, crc; 
	
    crc = 0xffff;    // Reset crc
	
	for ( k = 0; k < num_bytes; k++ )
	{
		crc = (uint8_t) (crc >> 8) | (crc << 8);
		crc ^= pData[k];
		crc ^= (uint8_t) (crc & 0xFF) >> 4;
		crc ^= (crc << 8) << 4;
		crc ^= ((crc & 0xff) << 4) << 1;
	}
	
	return crc;
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
