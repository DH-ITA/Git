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

#ifndef CRC_H
#define CRC_H

/*
 ******************************************************************************
 * Include Header Files                                                     
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

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
 */
extern uint8_t CRC_crc4(uint8_t *pData, uint8_t numBytes);
extern uint8_t CRC_crc8(const uint8_t *ptr_data, uint16_t num_bytes);
extern uint16_t CRC_crc12(const uint8_t *ptr_data, uint16_t length);
extern uint16_t CRC_crc16(const uint8_t *pData, uint16_t numBytes);
extern uint8_t CRC_2s_complement(const uint8_t *pData, uint16_t num_bytes);
extern uint32_t CRC_image_crc16_calc(void);
extern uint32_t CRC_app_crc16_calc(void);
extern uint16_t CRC_crc16_c59x(const uint8_t *pData, uint16_t num_bytes);

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
