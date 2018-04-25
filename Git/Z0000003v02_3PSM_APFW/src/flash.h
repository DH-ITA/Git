

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
 * @defgroup CRC CRC Library
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all exportable functions for CRC Library.
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

#ifndef FLASH_H_
#define FLASH_H_
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
#define TEST_PAGE_ADDRESS (IFLASH_CNC_ADDR + IFLASH_SIZE - IFLASH_PAGE_SIZE * 4)

#define	BOOTLOADER_APP_START_ADDR            ((uint32_t)0x01000000)
//#define MAIN_APP_START_ADDR                  ((uint32_t)0x01010000)
#define MAIN_APP_START_ADDR                  ((uint32_t)0x01010000)
#define MAIN_APP_END_ADDR                    ((uint32_t)0x0103FFFF)
#define IMAGE_APP_START_ADDR                 ((uint32_t)0x01050000)
// #define IMAGE_APP_START_ADDR                 ((uint32_t)0x01000000)
#define IMAGE_APP_END_ADDR                   ((uint32_t)0x0107FFFF)
#define BOOT_ORIGIN_ADDR                     ((uint32_t)0x01008000)

/*SAM4CMP FLASH PAGES DIRECTORY*/
#define FLASH_PAGE_1_START_ADDR               ((uint32_t)0x01000000)
#define FLASH_PAGE_1_END_ADDR                 ((uint32_t)0x010001FF)
#define FLASH_PAGE_100_START_ADDR             ((uint32_t)0x0100C800)
#define FLASH_PAGE_100_END_ADDR               ((uint32_t)0x0100C9FF)
#define FLASH_PAGE_200_START_ADDR             ((uint32_t)0x01019000)
#define FLASH_PAGE_200_END_ADDR               ((uint32_t)0x010191FF)
#define FLASH_PAGE_500_START_ADDR             ((uint32_t)0x0103E800)
#define FLASH_PAGE_500_END_ADDR               ((uint32_t)0x0103E9FF)
#define FLASH_PAGE_1000_START_ADDR            ((uint32_t)0x0107D000)
#define FLASH_PAGE_1000_END_ADDR              ((uint32_t)0x0107D1FF)
#define FLASH_PAGE_1024_START_ADDR            ((uint32_t)0x01080000)
#define FLASH_PAGE_2048_START_ADDR            ((uint32_t)0x010FE000)
#define FLASH_PAGE_2048_END_ADDR              ((uint32_t)0x010FFFFF)

#define FWU_TIMER								2000                  


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

extern void FLASH_initialize(void);
extern void FLASH_read(uint32_t flash_addr,uint8_t *ptr_buffer, uint8_t read_size);
extern void FLASH_boot_origin(uint8_t origin);
extern void FLASH_fwu_reset(void);
extern void FLASH_erase_main_app_sector(void);
extern void FLASH_erase_image_sector(void);
extern void FLASH_fwu_boot_mode(void);

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
