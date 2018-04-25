/**
 ******************************************************************************
 * \file eeprom.h
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
#ifndef EEPROM_H_
#define EEPROM_H_

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

/*===========================================================================*\
 * External 512KB EEPROM
\*===========================================================================*/
#define EXT_EEPROM_START_ADDRESS			0x00000
#define EXT_EEPROM_END_ADDRESS				0x7FFFF
#define EXT_EEPROM_DATA_START_ADDR			0x00400
#define EXT_EEPROM_DATA_END_ADDR			0x7FFFF

#define EXT_EEPROM_1_START_ADDRESS			0x00000
#define EXT_EEPROM_1_END_ADDRESS			0x3FFFF
#define EXT_EEPROM_2_START_ADDRESS			0x40000
#define EXT_EEPROM_2_END_ADDRESS			0x7FFFF

#define LAST_EEPROM_1_ADDR					0x3FFFF		
#define LAST_EEPROM_2_ADDR					0x7FFFF		  

/*========================================================================================================*\
									CONFIGURATION/CONTROL TABLES
 *--------------------------------------------------------------|---------------|-------------------------*
 * 		NAME						ADDRESS						| RESERVED SIZE | OCCUPED SIZE			   
\*==============================================================|===============|=========================*/	

// Metrology DSP Control
#define DSP_CTRL_START_ADDR			0x00300		//00300-003FF	| 256			| 256	

// Configuration Program			
#define CP_ADDR						0x00600		//00600-01987	| 5000			|										
#define ST_1_PROG_STATE_ADDR		0x00600		//00600-00600	| 1				| 1
#define ST_1_NVM_CRC_ADDR			0x00601		//00601-00602	| 2				| 2
#define ST_1_START_ADDR				0x00603		//00603-00632	| 48			| ST_1_SIZE			= 32
#define ST_6_PROG_STATE_ADDR		0x00633		//00633-00633	| 1				| 1
#define ST_6_NVM_CRC_ADDR			0x00634		//00634-00635	| 2				| 2
#define ST_6_START_ADDR				0x00636		//00636-00794	| 351			| ST_6_MAX_SIZE		= 234
#define ST_11_PROG_STATE_ADDR		0x00795		//00795-00795	| 1				| 1
#define ST_11_NVM_CRC_ADDR			0x00796		//00796-00797	| 2				| 2
#define ST_11_START_ADDR			0x00798		//00798-007A3	| 12			| ST_11_MAX_SIZE	= 8
#define ST_13_PROG_STATE_ADDR		0x007A4		//007A4-007A4	| 1				| 1
#define ST_13_NVM_CRC_ADDR			0x007A5		//007A5-007A6	| 2				| 2
#define ST_13_START_ADDR			0x007A7		//007A7-007B2	| 12			| ST_13_MAX_SIZE	= 8
#define ST_21_PROG_STATE_ADDR		0x007B3		//007B3-007B3	| 1				| 1
#define ST_21_NVM_CRC_ADDR			0x007B4		//007B4-007B5	| 2				| 2
#define ST_21_START_ADDR			0x007B6		//007B6-007C4	| 15			| ST_21_MAX_SIZE	= 10
#define ST_22_PROG_STATE_ADDR		0x007C5		//007C5-007C5	| 1				| 1
#define ST_22_NVM_CRC_ADDR			0x007C6		//007C6-007C7	| 2				| 2
#define ST_22_START_ADDR			0x007C8		//007C8-007D4	| 13			| ST_22_MAX_SIZE	= 9
#define ST_27_PROG_STATE_ADDR		0x007D5		//007D5-007D5	| 1				| 1
#define ST_27_NVM_CRC_ADDR			0x007D6		//007D6-007D7	| 2				| 2
#define ST_27_START_ADDR			0x007D8		//007D8-0080A	| 51			| ST_27_MAX_SIZE	= 40
#define ST_41_PROG_STATE_ADDR		0x0080B		//0080B-0080B	| 1				| 1
#define ST_41_NVM_CRC_ADDR			0x0080C		//0080C-0080D	| 2				| 2
#define ST_41_START_ADDR			0x0080E		//0080E-00816	| 9				| ST_41_MAX_SIZE	= 6
#define ST_42_PROG_STATE_ADDR		0x00817		//00817-00817	| 1				| 1
#define ST_42_NVM_CRC_ADDR			0x00818		//00818-00819	| 2				| 2
#define ST_42_START_ADDR			0x0081A		//0081A-008D6	| 189			| ST_42_MAX_SIZE	= 126
#define ST_43_PROG_STATE_ADDR		0x008D7		//008D7-008D7	| 1				| 1
#define ST_43_NVM_CRC_ADDR			0x008D8		//008D8-008D9	| 2				| 2
#define ST_43_START_ADDR			0x008DA		//008DA-008E5	| 12			| ST_43_MAX_SIZE	= 8
#define ST_44_PROG_STATE_ADDR		0x008E6		//008E6-008E6	| 1				| 1
#define ST_44_NVM_CRC_ADDR			0x008E7		//008E7-008E8	| 2				| 2
#define ST_44_START_ADDR			0x008E9		//008E9-00A68	| 384			| ST_44_MAX_SIZE	= 256
#define ST_51_PROG_STATE_ADDR		0x00A69		//00A69-00A69	| 1				| 1
#define ST_51_NVM_CRC_ADDR			0x00A6A		//00A6A-00A6B	| 2				| 2
#define ST_51_START_ADDR			0x00A6C		//00A6C-00A78	| 13			| ST_51_MAX_SIZE	= 9
#define ST_53_PROG_STATE_ADDR		0x00A79		//00A79-00A79	| 1				| 1
#define ST_53_NVM_CRC_ADDR			0x00A7A		//00A7A-00A7B	| 2				| 2
#define ST_53_START_ADDR			0x00A7C		//00A7C-00A8B	| 16			| ST_53_MAX_SIZE	= 11
#define ST_54_PROG_STATE_ADDR		0x00A8C		//00A8C-00A8C	| 1				| 1
#define ST_54_NVM_CRC_ADDR			0x00A8D		//00A8D-00A8E	| 2				| 2
#define ST_54_START_ADDR			0x00A8F		//00A8F-00DD9	| 843			| ST_54_MAX_SIZE	= 572
#define ST_61_PROG_STATE_ADDR		0x00DDA		//00DDA-00DDA	| 1				| 1
#define ST_61_NVM_CRC_ADDR			0x00DDB		//00DDB-00DDC	| 2				| 2
#define ST_61_START_ADDR			0x00DDD		//00DDD-00DEF	| 19			| ST_61_MAX_SIZE	= 13
#define ST_62_PROG_STATE_ADDR		0x00DF0		//00DF0-00DF0	| 1				| 1
#define ST_62_NVM_CRC_ADDR			0x00DF1		//00DF1-00DF2	| 2				| 2
#define ST_62_START_ADDR			0x00DF3		//00DF3-00E1D	| 43			| ST_62_MAX_SIZE	= 29
#define ST_71_PROG_STATE_ADDR		0x00E1E		//00E1E-00E1E	| 1				| 1
#define ST_71_NVM_CRC_ADDR			0x00E1F		//00E1F-00E20	| 2				| 2
#define ST_71_START_ADDR			0x00E21		//00E21-00E2D	| 13			| ST_71_MAX_SIZE	= 9
#define ST_73_PROG_STATE_ADDR		0x00E2E		//00E2E-00E2E	| 1				| 1
#define ST_73_NVM_CRC_ADDR			0x00E2F		//00E2F-00E30	| 2				| 2
#define ST_73_START_ADDR			0x00E31		//00E31-00EB1	| 129 			| ST_73_MAX_SIZE	= 86
#define ST_75_PROG_STATE_ADDR		0x00EB2		//00EB2-00EB2	| 1				| 1
#define ST_75_NVM_CRC_ADDR			0x00EB3		//00EB3-00EB4	| 2				| 2
#define ST_75_START_ADDR			0x00EB5		//00EB5-00F35	| 129 			| ST_75_MAX_SIZE	= 86
#define MT_1_PROG_STATE_ADDR		0x00F36		//00F36-00F36	| 1				| 1
#define MT_1_NVM_CRC_ADDR			0x00F37		//00F37-00F38	| 2				| 2
#define MT_1_START_ADDR				0x00F39		//00F39-00F81	| 73			| MT_1_MAX_SIZE		= 49
#define MT_3_PROG_STATE_ADDR		0x00F82		//00F82-00F82	| 1				| 1
#define MT_3_NVM_CRC_ADDR			0x00F83		//00F83-00F84	| 2				| 2
#define MT_3_START_ADDR				0x00F85		//00F85-01065	| 225			| MT_3_MAX_SIZE		= 150
#define MT_23_PROG_STATE_ADDR		0x01066		//01066-01066	| 1				| 1
#define MT_23_NVM_CRC_ADDR			0x01067		//01067-01068	| 2				| 2
#define MT_23_START_ADDR			0x01069		//01069-01074	| 12			| MT_23_MAX_SIZE	= 8
#define MT_53_PROG_STATE_ADDR		0x01075		//01075-01075	| 1				| 1
#define MT_53_NVM_CRC_ADDR			0x01076		//01076-01077	| 2				| 2
#define MT_53_START_ADDR			0x01078		//01078-010E3	| 108			| MT_53_MAX_SIZE	= 72
#define MT_73_PROG_STATE_ADDR		0x010E4		//010E4-010E4	| 1				| 1
#define MT_73_NVM_CRC_ADDR			0x010E5		//010E5-010E6	| 2				| 2
#define MT_73_START_ADDR			0x010E7		//010E7-01167	| 129			| MT_73_MAX_SIZE	= 86

// Manufacturing Configuration Program Backup
#define MCP_ADDR					0x01988		//01988-02D0F	| 5000			|	
#define MCP_ST_1_PROG_STATE_ADDR	0x01988		//01988-01988	| 1				| 1
#define MCP_ST_1_NVM_CRC_ADDR		0x01989		//01989-0198A	| 2				| 2
#define MCP_ST_1_START_ADDR			0x0198B		//0198B-019BA	| 48			| ST_1_SIZE			= 32
#define MCP_ST_6_PROG_STATE_ADDR	0x019BB		//019BB-019BB	| 1				| 1
#define MCP_ST_6_NVM_CRC_ADDR		0x019BC		//019BC-019BD	| 2				| 2
#define MCP_ST_6_START_ADDR			0x019BE		//019BE-01B1C	| 351			| ST_6_MAX_SIZE		= 234
#define MCP_ST_11_PROG_STATE_ADDR	0x01B1D		//01B1D-01B1D	| 1				| 1
#define MCP_ST_11_NVM_CRC_ADDR		0x01B1E		//01B1E-01B1F	| 2				| 2
#define MCP_ST_11_START_ADDR		0x01B20		//01B20-01B2B	| 12			| ST_11_MAX_SIZE	= 8
#define MCP_ST_13_PROG_STATE_ADDR	0x01B2C		//01B2C-01B2C	| 1				| 1
#define MCP_ST_13_NVM_CRC_ADDR		0x01B2D		//01B2D-01B2E	| 2				| 2
#define MCP_ST_13_START_ADDR		0x01B2F		//01B2F-01B3A	| 12			| ST_13_MAX_SIZE	= 8
#define MCP_ST_21_PROG_STATE_ADDR	0x01B3B		//01B3B-01B3B	| 1				| 1
#define MCP_ST_21_NVM_CRC_ADDR		0x01B3C		//01B3C-01B3D	| 2				| 2
#define MCP_ST_21_START_ADDR		0x01B3E		//01B3E-01B4C	| 15			| ST_21_MAX_SIZE	= 10
#define MCP_ST_22_PROG_STATE_ADDR	0x01B4D		//01B4D-01B4D	| 1				| 1
#define MCP_ST_22_NVM_CRC_ADDR		0x01B4E		//01B4E-01B4F	| 2				| 2
#define MCP_ST_22_START_ADDR		0x01B50		//01B50-01B5C	| 13			| ST_22_MAX_SIZE	= 9
#define MCP_ST_27_PROG_STATE_ADDR	0x01B5D		//01B5D-01B5D	| 1				| 1
#define MCP_ST_27_NVM_CRC_ADDR		0x01B5E		//01B5E-01B5F	| 2				| 2
#define MCP_ST_27_START_ADDR		0x01B60		//01B60-01B92	| 51			| ST_27_MAX_SIZE	= 40
#define MCP_ST_41_PROG_STATE_ADDR	0x01B93		//01B93-01B93	| 1				| 1
#define MCP_ST_41_NVM_CRC_ADDR		0x01B94		//01B94-01B95	| 2				| 2
#define MCP_ST_41_START_ADDR		0x01B96		//01B96-01B9E	| 9				| ST_41_MAX_SIZE	= 6
#define MCP_ST_42_PROG_STATE_ADDR	0x01B9F		//01B9F-01B9F	| 1				| 1
#define MCP_ST_42_NVM_CRC_ADDR		0x01BA0		//01BA0-01BA1	| 2				| 2
#define MCP_ST_42_START_ADDR		0x01BA2		//01BA2-01C5E	| 189			| ST_42_MAX_SIZE	= 126
#define MCP_ST_43_PROG_STATE_ADDR	0x01C5F		//01C5F-01C5F	| 1				| 1
#define MCP_ST_43_NVM_CRC_ADDR		0x01C60		//01C60-01C61	| 2				| 2
#define MCP_ST_43_START_ADDR		0x01C62		//01C62-01C6D	| 12			| ST_43_MAX_SIZE	= 8
#define MCP_ST_44_PROG_STATE_ADDR	0x01C6E		//01C6E-01C6E	| 1				| 1
#define MCP_ST_44_NVM_CRC_ADDR		0x01C6F		//01C6F-01C70	| 2				| 2
#define MCP_ST_44_START_ADDR		0x01C71		//01C71-01DF0	| 384			| ST_44_MAX_SIZE	= 256
#define MCP_ST_51_PROG_STATE_ADDR	0x01DF1		//01DF1-01DF1	| 1				| 1
#define MCP_ST_51_NVM_CRC_ADDR		0x01DF2		//01DF2-01DF3	| 2				| 2
#define MCP_ST_51_START_ADDR		0x01DF4		//01DF4-01E00	| 13			| ST_51_MAX_SIZE	= 9
#define MCP_ST_53_PROG_STATE_ADDR	0x01E01		//01E01-01E01	| 1				| 1
#define MCP_ST_53_NVM_CRC_ADDR		0x01E02		//01E02-01E03	| 2				| 2
#define MCP_ST_53_START_ADDR		0x01E04		//01E04-01E13	| 16			| ST_53_MAX_SIZE	= 11
#define MCP_ST_54_PROG_STATE_ADDR	0x01E14		//01E14-01E14	| 1				| 1
#define MCP_ST_54_NVM_CRC_ADDR		0x01E15		//01E15-01E16	| 2				| 2
#define MCP_ST_54_START_ADDR		0x01E17		//01E17-02161	| 843			| ST_54_MAX_SIZE	= 572
#define MCP_ST_61_PROG_STATE_ADDR	0x02162		//02162-02162	| 1				| 1
#define MCP_ST_61_NVM_CRC_ADDR		0x02163		//02163-02164	| 2				| 2
#define MCP_ST_61_START_ADDR		0x02165		//02165-02177	| 19			| ST_61_MAX_SIZE	= 13
#define MCP_ST_62_PROG_STATE_ADDR	0x02178		//02178-02178	| 1				| 1
#define MCP_ST_62_NVM_CRC_ADDR		0x02179		//02179-0217A	| 2				| 2
#define MCP_ST_62_START_ADDR		0x0217B		//0217B-021A5	| 43			| ST_62_MAX_SIZE	= 29
#define MCP_ST_71_PROG_STATE_ADDR	0x021A6		//021A6-021A6	| 1				| 1
#define MCP_ST_71_NVM_CRC_ADDR		0x021A7		//021A7-021A8	| 2				| 2
#define MCP_ST_71_START_ADDR		0x021A9		//021A9-021B5	| 13			| ST_71_MAX_SIZE	= 9
#define MCP_ST_73_PROG_STATE_ADDR	0x021B6		//021B6-021B6	| 1				| 1
#define MCP_ST_73_NVM_CRC_ADDR		0x021B7		//021B7-021B8	| 2				| 2
#define MCP_ST_73_START_ADDR		0x021B9		//021B9-02239	| 129			| ST_73_MAX_SIZE	= 86
#define MCP_ST_75_PROG_STATE_ADDR	0x0223A		//0223A-0223A	| 1				| 1
#define MCP_ST_75_NVM_CRC_ADDR		0x0223B		//0223B-0223C	| 2				| 2
#define MCP_ST_75_START_ADDR		0x0223D		//0223D-022BD	| 129			| ST_75_MAX_SIZE	= 86
#define MCP_MT_1_PROG_STATE_ADDR	0x022BE		//022BE-022BE	| 1				| 1
#define MCP_MT_1_NVM_CRC_ADDR		0x022BF		//022BF-022C0	| 2				| 2
#define MCP_MT_1_START_ADDR			0x022C1		//022C1-02309	| 73			| MT_1_MAX_SIZE		= 49
#define MCP_MT_3_PROG_STATE_ADDR	0x0230A		//0230A-0230A	| 1				| 1
#define MCP_MT_3_NVM_CRC_ADDR		0x0230B		//0230B-0230C	| 2				| 2
#define MCP_MT_3_START_ADDR			0x0230D		//0230D-023ED	| 225			| MT_3_MAX_SIZE		= 150
#define MCP_MT_23_PROG_STATE_ADDR	0x023EE		//023EE-023EE	| 1				| 1
#define MCP_MT_23_NVM_CRC_ADDR		0x023EF		//023EF-023F0	| 2				| 2
#define MCP_MT_23_START_ADDR		0x023F1		//023F1-023FC	| 12			| MT_23_MAX_SIZE	= 8
#define MCP_MT_53_PROG_STATE_ADDR	0x023FD		//023FD-023FD	| 1				| 1
#define MCP_MT_53_NVM_CRC_ADDR		0x023FE		//023FE-023FF	| 2				| 2
#define MCP_MT_53_START_ADDR		0x02400		//02400-0246B	| 108			| MT_53_MAX_SIZE	= 72
#define MCP_MT_73_PROG_STATE_ADDR	0x0246C		//0246C-0246C	| 1				| 1
#define MCP_MT_73_NVM_CRC_ADDR		0x0246D		//0246D-0246E	| 2				| 2
#define MCP_MT_73_START_ADDR		0x0246F		//0246F-024EF	| 129			| MT_73_MAX_SIZE	= 86

// Customer Configuration Program Backup
#define CCP_ADDR					0x02D10		//02D10-04097	| 5000			|
#define CCP_ST_1_PROG_STATE_ADDR	0x02D10		//02D10-02D10	| 1				| 1
#define CCP_ST_1_NVM_CRC_ADDR		0x02D11		//02D11-02D12	| 2				| 2
#define CCP_ST_1_START_ADDR			0x02D13		//02D13-02D42	| 48			| ST_1_SIZE			= 32
#define CCP_ST_6_PROG_STATE_ADDR	0x02D43		//02D43-02D43	| 1				| 1
#define CCP_ST_6_NVM_CRC_ADDR		0x02D44		//02D44-02D45	| 2				| 2
#define CCP_ST_6_START_ADDR			0x02D46		//02D46-02EA4	| 351			| ST_6_MAX_SIZE		= 234
#define CCP_ST_11_PROG_STATE_ADDR	0x02EA5		//02EA5-02EA5	| 1				| 1
#define CCP_ST_11_NVM_CRC_ADDR		0x02EA6		//02EA6-02EA7	| 2				| 2
#define CCP_ST_11_START_ADDR		0x02EA8		//02EA8-02EB3	| 12			| ST_11_MAX_SIZE	= 8
#define CCP_ST_13_PROG_STATE_ADDR	0x02EB4		//02EB4-02EB4	| 1				| 1
#define CCP_ST_13_NVM_CRC_ADDR		0x02EB5		//02EB5-02EB6	| 2				| 2
#define CCP_ST_13_START_ADDR		0x02EB7		//02EB7-02EC2	| 12			| ST_13_MAX_SIZE	= 8
#define CCP_ST_21_PROG_STATE_ADDR	0x02EC3		//02EC3-02EC3	| 1				| 1
#define CCP_ST_21_NVM_CRC_ADDR		0x02EC4		//02EC4-02EC5	| 2				| 2
#define CCP_ST_21_START_ADDR		0x02EC6		//02EC6-02ED4	| 15			| ST_21_MAX_SIZE	= 10
#define CCP_ST_22_PROG_STATE_ADDR	0x02ED5		//02ED5-02ED5	| 1				| 1
#define CCP_ST_22_NVM_CRC_ADDR		0x02ED6		//02ED6-02ED7	| 2				| 2
#define CCP_ST_22_START_ADDR		0x02ED8		//02ED8-02EE4	| 13			| ST_22_MAX_SIZE	= 9
#define CCP_ST_27_PROG_STATE_ADDR	0x02EE5		//02EE5-02EE5	| 1				| 1
#define CCP_ST_27_NVM_CRC_ADDR		0x02EE6		//02EE6-02EE7	| 2				| 2
#define CCP_ST_27_START_ADDR		0x02EE8		//02EE8-02F1A	| 51			| ST_27_MAX_SIZE	= 40
#define CCP_ST_41_PROG_STATE_ADDR	0x02F1B		//02F1B-02F1B	| 1				| 1
#define CCP_ST_41_NVM_CRC_ADDR		0x02F1C		//02F1C-02F1D	| 2				| 2
#define CCP_ST_41_START_ADDR		0x02F1E		//02F1E-02F26	| 9				| ST_41_MAX_SIZE	= 6
#define CCP_ST_42_PROG_STATE_ADDR	0x02F27		//02F27-02F27	| 1				| 1
#define CCP_ST_42_NVM_CRC_ADDR		0x02F28		//02F28-02F29	| 2				| 2
#define CCP_ST_42_START_ADDR		0x02F2A		//02F2A-02FE6	| 189			| ST_42_MAX_SIZE	= 126
#define CCP_ST_43_PROG_STATE_ADDR	0x02FE7		//02FE7-02FE7	| 1				| 1
#define CCP_ST_43_NVM_CRC_ADDR		0x02FE8		//02FE8-02FE9	| 2				| 2
#define CCP_ST_43_START_ADDR		0x02FEA		//02FEA-02FF5	| 12			| ST_43_MAX_SIZE	= 8
#define CCP_ST_44_PROG_STATE_ADDR	0x02FF6		//02FF6-02FF6	| 1				| 1
#define CCP_ST_44_NVM_CRC_ADDR		0x02FF7		//02FF7-02FF8	| 2				| 2
#define CCP_ST_44_START_ADDR		0x02FF9		//02FF9-03178	| 384			| ST_44_MAX_SIZE	= 256
#define CCP_ST_51_PROG_STATE_ADDR	0x03179		//03179-03179	| 1				| 1
#define CCP_ST_51_NVM_CRC_ADDR		0x0317A		//0317A-0317B	| 2				| 2
#define CCP_ST_51_START_ADDR		0x0317C		//0317C-03188	| 13			| ST_51_MAX_SIZE	= 9
#define CCP_ST_53_PROG_STATE_ADDR	0x03189		//03189-03189	| 1				| 1
#define CCP_ST_53_NVM_CRC_ADDR		0x0318A		//0318A-0318B	| 2				| 2
#define CCP_ST_53_START_ADDR		0x0318C		//0318C-0319B	| 16			| ST_53_MAX_SIZE	= 11
#define CCP_ST_54_PROG_STATE_ADDR	0x0319C		//0319C-0319C	| 1				| 1
#define CCP_ST_54_NVM_CRC_ADDR		0x0319D		//0319D-0319E	| 2				| 2
#define CCP_ST_54_START_ADDR		0x0319F		//0319F-034E9	| 843			| ST_54_MAX_SIZE	= 572
#define CCP_ST_61_PROG_STATE_ADDR	0x034EA		//034EA-034EA	| 1				| 1
#define CCP_ST_61_NVM_CRC_ADDR		0x034EB		//034EB-034EC	| 2				| 2
#define CCP_ST_61_START_ADDR		0x034ED		//034ED-034FF	| 19			| ST_61_MAX_SIZE	= 13
#define CCP_ST_62_PROG_STATE_ADDR	0x03500		//03500-03500	| 1				| 1
#define CCP_ST_62_NVM_CRC_ADDR		0x03501		//03501-03502	| 2				| 2
#define CCP_ST_62_START_ADDR		0x03503		//03503-0352D	| 43			| ST_62_MAX_SIZE	= 29
#define CCP_ST_71_PROG_STATE_ADDR	0x0352E		//0352E-0352E	| 1				| 1
#define CCP_ST_71_NVM_CRC_ADDR		0x0352F		//0352F-03530	| 2				| 2
#define CCP_ST_71_START_ADDR		0x03531		//03531-0353D	| 13			| ST_71_MAX_SIZE	= 9
#define CCP_ST_73_PROG_STATE_ADDR	0x0353E		//0353E-0353E	| 1				| 1
#define CCP_ST_73_NVM_CRC_ADDR		0x0353F		//0353F-03540	| 2				| 2
#define CCP_ST_73_START_ADDR		0x03541		//03541-035C1	| 129			| ST_73_MAX_SIZE	= 86
#define CCP_ST_75_PROG_STATE_ADDR	0x035C2		//035C2-035C2	| 1				| 1
#define CCP_ST_75_NVM_CRC_ADDR		0x035C3		//035C3-035C4	| 2				| 2
#define CCP_ST_75_START_ADDR		0x035C5		//035C5-03645	| 129			| ST_75_MAX_SIZE	= 86
#define CCP_MT_1_PROG_STATE_ADDR	0x03646		//03646-03646	| 1				| 1
#define CCP_MT_1_NVM_CRC_ADDR		0x03647		//03647-03648	| 2				| 2
#define CCP_MT_1_START_ADDR			0x03649		//03649-03691	| 73			| MT_1_MAX_SIZE		= 49
#define CCP_MT_3_PROG_STATE_ADDR	0x03692		//03692-03692	| 1				| 1
#define CCP_MT_3_NVM_CRC_ADDR		0x03693		//03693-03694	| 2				| 2
#define CCP_MT_3_START_ADDR			0x03695		//03695-03775	| 225			| MT_3_MAX_SIZE		= 150
#define CCP_MT_23_PROG_STATE_ADDR	0x03776		//03776-03776	| 1				| 1
#define CCP_MT_23_NVM_CRC_ADDR		0x03777		//03777-03778	| 2				| 2
#define CCP_MT_23_START_ADDR		0x03779		//03779-03784	| 12			| MT_23_MAX_SIZE	= 8
#define CCP_MT_53_PROG_STATE_ADDR	0x03785		//03785-03785	| 1				| 1
#define CCP_MT_53_NVM_CRC_ADDR		0x03786		//03786-03787	| 2				| 2
#define CCP_MT_53_START_ADDR		0x03788		//03788-037F3	| 108			| MT_53_MAX_SIZE	= 72
#define CCP_MT_73_PROG_STATE_ADDR	0x037F4		//037F4-037F4	| 1				| 1
#define CCP_MT_73_NVM_CRC_ADDR		0x037F5		//037F5-037F6	| 2				| 2
#define CCP_MT_73_START_ADDR		0x037F7		//037F7-03877	| 129			| MT_73_MAX_SIZE	= 86

/*========================================================================================================*\
												STATUS TABLES
 *--------------------------------------------------------------|---------------|-------------------------*
 * 		NAME						ADDRESS						| ASSIGNED SIZE | TABLE SIZE			   
\*==============================================================|===============|=========================*/
#define ST_3_PROG_STATE_ADDR		0x04098		//04098-04098	| 1				| 1
#define ST_3_START_ADDR				0x04099		//04099-04198	| 256			| ST_3_SIZE			 = 24
#define ST_23_PROG_STATE_ADDR		0x04199		//04199-04199	| 1				| 1
#define ST_23_START_ADDR			0x0419A		//0419A-043E3	| 586			| ST_23_MAX_SIZE	 = 391
#define ST_24_PROG_STATE_ADDR		0x043E4		//043E4-043E4	| 1				| 1
#define ST_24_START_ADDR			0x043E5		//043E5-04636	| 594			| ST_24_MAX_SIZE	 = 396
#define ST_25_PROG_STATE_ADDR		0x04637		//04637-04637	| 1				| 1
#define ST_25_START_ADDR			0x04638		//04638-04889	| 594			| ST_25_MAX_SIZE	 = 396
#define ST_26_PROG_STATE_ADDR		0x0488A		//0488A-0488A	| 1				| 1
#define ST_26_START_ADDR			0x0488B		//0488B-06939	| 8367			| ST_26_MAX_SIZE	 = 5578
#define ST_63_PROG_STATE_ADDR		0x0693A		//0693A-0693A	| 1				| 1
#define ST_63_START_ADDR			0x0693B		//0693B-06976	| 60			| ST_63_MAX_SIZE	 = 13
#define ST_74_PROG_STATE_ADDR		0x06977		//06977-06977	| 1				| 1
#define ST_74_START_ADDR			0x06978		//06978-07DC7	| 5200			| ST_74_MAX_SIZE	 = 3581
#define ST_76_PROG_STATE_ADDR		0x07DC8		//07DC8-07DC8	| 1				| 1
#define ST_76_START_ADDR			0x07DC9		//07DC9-09218	| 5200			| ST_76_MAX_SIZE	 = 3581
#define MT_4_PROG_STATE_ADDR		0x09219		//09219-09219	| 1				| 1
#define MT_4_START_ADDR				0x0921A		//0921A-09255	| 60			| MT_4_MAX_SIZE	 = 40
#define Mt_5_PROG_STATE_ADDR		0x09256		//09256-09256	| 1				| 1
#define MT_5_START_ADDR				0x09257		//09257-092BA	| 100			| MT_5_SIZE		 = 16
#define TOU_BACKUP_START_ADDR		0x092BB		//092BB-092C9	| 15			| 10
#define LP_BACKUP_START_ADDR		0x092CA		//092CA-092F3	| 42			| 28
												//				|				|
#define ST_64_START_ADDR			0x40000		//40000-7FFE2   | 262145		| ST_64_MAX_SIZE  = 262145

/*===========================================================================*\
 * DSP CONTROL
\*===========================================================================*/
#define DSP_CTRL_STATE_CTRL_ADDR						DSP_CTRL_START_ADDR					
#define DSP_CTRL_FEATURE_CTRL0_ADDR						DSP_CTRL_STATE_CTRL_ADDR			+ 4
#define DSP_CTRL_FEATURE_CTRL1_ADDR						DSP_CTRL_FEATURE_CTRL0_ADDR			+ 4
#define DSP_CTRL_METER_TYPE_ADDR						DSP_CTRL_FEATURE_CTRL1_ADDR			+ 4
#define DSP_CTRL_M_ADDR									DSP_CTRL_METER_TYPE_ADDR			+ 4
#define DSP_CTRL_N_MAX_ADDR								DSP_CTRL_M_ADDR						+ 4
#define DSP_CTRL_PULSE0_CTRL_ADDR						DSP_CTRL_N_MAX_ADDR					+ 4
#define DSP_CTRL_PULSE1_CTRL_ADDR						DSP_CTRL_PULSE0_CTRL_ADDR			+ 4
#define DSP_CTRL_PULSE2_CTRL_ADDR						DSP_CTRL_PULSE1_CTRL_ADDR			+ 4
#define DSP_CTRL_P_K_T_ADDR								DSP_CTRL_PULSE2_CTRL_ADDR			+ 4
#define DSP_CTRL_Q_K_T_ADDR								DSP_CTRL_P_K_T_ADDR					+ 4
#define DSP_CTRL_I_K_T_ADDR								DSP_CTRL_Q_K_T_ADDR					+ 4
#define DSP_CTRL_CREEP_THR_P_ADDR						DSP_CTRL_I_K_T_ADDR					+ 4
#define DSP_CTRL_CREEP_THR_Q_ADDR						DSP_CTRL_CREEP_THR_P_ADDR			+ 4
#define DSP_CTRL_CREEP_THR_I_ADDR						DSP_CTRL_CREEP_THR_Q_ADDR			+ 4
#define DSP_CTRL_POWER_OFFSET_CTRL_ADDR					DSP_CTRL_CREEP_THR_I_ADDR			+ 4
#define DSP_CTRL_POWER_OFFSET_P_ADDR					DSP_CTRL_POWER_OFFSET_CTRL_ADDR		+ 4
#define DSP_CTRL_POWER_OFFSET_Q_ADDR					DSP_CTRL_POWER_OFFSET_P_ADDR		+ 4
#define DSP_CTRL_SWELL_THR_VA_ADDR						DSP_CTRL_POWER_OFFSET_Q_ADDR		+ 4
#define DSP_CTRL_SWELL_THR_VB_ADDR						DSP_CTRL_SWELL_THR_VA_ADDR			+ 4
#define DSP_CTRL_SWELL_THR_VC_ADDR						DSP_CTRL_SWELL_THR_VB_ADDR			+ 4
#define DSP_CTRL_SAG_THR_VA_ADDR						DSP_CTRL_SWELL_THR_VC_ADDR			+ 4
#define DSP_CTRL_SAG_THR_VB_ADDR						DSP_CTRL_SAG_THR_VA_ADDR			+ 4
#define DSP_CTRL_SAG_THR_VC_ADDR						DSP_CTRL_SAG_THR_VB_ADDR			+ 4
#define DSP_CTRL_K_IA_ADDR								DSP_CTRL_SAG_THR_VC_ADDR			+ 4
#define DSP_CTRL_K_VA_ADDR								DSP_CTRL_K_IA_ADDR					+ 4
#define DSP_CTRL_K_IB_ADDR								DSP_CTRL_K_VA_ADDR					+ 4
#define DSP_CTRL_K_VB_ADDR								DSP_CTRL_K_IB_ADDR					+ 4
#define DSP_CTRL_K_IC_ADDR								DSP_CTRL_K_VB_ADDR					+ 4
#define DSP_CTRL_K_VC_ADDR								DSP_CTRL_K_IC_ADDR					+ 4
#define DSP_CTRL_RESERVED1_ADDR							DSP_CTRL_K_VC_ADDR					+ 4
#define DSP_CTRL_CAL_M_IA_ADDR							DSP_CTRL_RESERVED1_ADDR				+ 4
#define DSP_CTRL_CAL_M_VA_ADDR							DSP_CTRL_CAL_M_IA_ADDR				+ 4
#define DSP_CTRL_CAL_M_IB_ADDR							DSP_CTRL_CAL_M_VA_ADDR				+ 4
#define DSP_CTRL_CAL_M_VB_ADDR							DSP_CTRL_CAL_M_IB_ADDR				+ 4
#define DSP_CTRL_CAL_M_IC_ADDR							DSP_CTRL_CAL_M_VB_ADDR				+ 4
#define DSP_CTRL_CAL_M_VC_ADDR							DSP_CTRL_CAL_M_IC_ADDR				+ 4
#define DSP_CTRL_RESERVED2_ADDR							DSP_CTRL_CAL_M_VC_ADDR				+ 4
#define DSP_CTRL_CAL_PH_IA_ADDR							DSP_CTRL_RESERVED2_ADDR				+ 4
#define DSP_CTRL_CAL_PH_VA_ADDR							DSP_CTRL_CAL_PH_IA_ADDR				+ 4
#define DSP_CTRL_CAL_PH_IB_ADDR							DSP_CTRL_CAL_PH_VA_ADDR				+ 4
#define DSP_CTRL_CAL_PH_VB_ADDR							DSP_CTRL_CAL_PH_IB_ADDR				+ 4
#define DSP_CTRL_CAL_PH_IC_ADDR							DSP_CTRL_CAL_PH_VB_ADDR				+ 4
#define DSP_CTRL_CAL_PH_VC_ADDR							DSP_CTRL_CAL_PH_IC_ADDR				+ 4
#define DSP_CTRL_RESERVED3_ADDR							DSP_CTRL_CAL_PH_VC_ADDR				+ 4
#define DSP_CTRL_RESERVED4_ADDR							DSP_CTRL_RESERVED3_ADDR				+ 4
#define DSP_CTRL_RESERVED5_ADDR							DSP_CTRL_RESERVED4_ADDR				+ 4
#define DSP_CTRL_RESERVED6_ADDR							DSP_CTRL_RESERVED5_ADDR				+ 4
#define DSP_CTRL_RESERVED7_ADDR							DSP_CTRL_RESERVED6_ADDR				+ 4
#define DSP_CTRL_RESERVED8_ADDR							DSP_CTRL_RESERVED7_ADDR				+ 4
#define DSP_CTRL_RESERVED9_ADDR							DSP_CTRL_RESERVED8_ADDR				+ 4
#define DSP_CTRL_ATSENSE_CTRL_20_23_ADDR				DSP_CTRL_RESERVED9_ADDR				+ 4
#define DSP_CTRL_ATSENSE_CTRL_24_27_ADDR				DSP_CTRL_ATSENSE_CTRL_20_23_ADDR	+ 4
#define DSP_CTRL_ATSENSE_CTRL_28_2B_ADDR				DSP_CTRL_ATSENSE_CTRL_24_27_ADDR	+ 4
#define DSP_CTRL_RESERVED10_ADDR						DSP_CTRL_ATSENSE_CTRL_28_2B_ADDR	+ 4																		
#define DSP_CTRL_CAL_M_IA_COPY_ADDR						DSP_CTRL_RESERVED10_ADDR			+ 4
#define DSP_CTRL_CAL_M_VA_COPY_ADDR						DSP_CTRL_CAL_M_IA_COPY_ADDR			+ 4
#define DSP_CTRL_CAL_PH_IA_COPY_ADDR					DSP_CTRL_CAL_M_VA_COPY_ADDR			+ 4
#define DSP_CTRL_CAL_M_IB_COPY_ADDR						DSP_CTRL_CAL_PH_IA_COPY_ADDR		+ 4
#define DSP_CTRL_CAL_M_VB_COPY_ADDR						DSP_CTRL_CAL_M_IB_COPY_ADDR			+ 4
#define DSP_CTRL_CAL_PH_IB_COPY_ADDR					DSP_CTRL_CAL_M_VB_COPY_ADDR			+ 4
#define DSP_CTRL_CAL_M_IC_COPY_ADDR						DSP_CTRL_CAL_PH_IB_COPY_ADDR		+ 4
#define DSP_CTRL_CAL_M_VC_COPY_ADDR						DSP_CTRL_CAL_M_IC_COPY_ADDR			+ 4
#define DSP_CTRL_CAL_PH_IC_COPY_ADDR					DSP_CTRL_CAL_M_VC_COPY_ADDR			+ 4

/*===========================================================================*\
 * ST_1: General Manufacturer Identification Table
\*===========================================================================*/
#define ST_1_MANUFACTURER_ADDR							ST_1_START_ADDR
#define ST_1_ED_MODEL_ADDR								ST_1_MANUFACTURER_ADDR						+ ST_1_MANUFACTURER_SIZE		
#define ST_1_HW_VERSION_NUMBER_ADDR						ST_1_ED_MODEL_ADDR							+ ST_1_ED_MODEL_SIZE			
#define ST_1_HW_REVISION_NUMBER_ADDR					ST_1_HW_VERSION_NUMBER_ADDR					+ ST_1_HW_VERSION_NUMBER_SIZE	
#define ST_1_FW_VERSION_NUMBER_ADDR						ST_1_HW_REVISION_NUMBER_ADDR				+ ST_1_HW_REVISION_NUMBER_SIZE
#define ST_1_FW_REVISION_NUMBER_ADDR					ST_1_FW_VERSION_NUMBER_ADDR					+ ST_1_FW_VERSION_NUMBER_SIZE	
#define ST_1_SERIAL_NUMBER_ADDR							ST_1_FW_REVISION_NUMBER_ADDR				+ ST_1_FW_REVISION_NUMBER_SIZE

/*===========================================================================*\
 * ST_3: End Device Mode Status Table
\*===========================================================================*/
#define ST_3_ED_MODE_ADDR								ST_3_START_ADDR
#define ST_3_ED_STD_STATUS1_ADDR						ST_3_ED_MODE_ADDR							+ ST_3_ED_MODE_SIZE		
#define ST_3_ED_STD_STATUS2_ADDR						ST_3_ED_STD_STATUS1_ADDR					+ ST_3_ED_STD_STATUS1_SIZE
#define ST_3_ED_MFG_STATUS_ADDR							ST_3_ED_STD_STATUS2_ADDR					+ ST_3_ED_STD_STATUS2_SIZE

/*===========================================================================*\
 * ST_6: General Configuration Table
\*===========================================================================*/
#define ST_6_OWNER_NAME_ADDR							ST_6_START_ADDR								
#define ST_6_UTILITY_DIV_ADDR							ST_6_OWNER_NAME_ADDR						+ ST_6_OWNER_NAME_SIZE			
#define ST_6_SERVICE_POINT_ID_ADDR						ST_6_UTILITY_DIV_ADDR						+ ST_6_UTILITY_DIV_SIZE			
#define ST_6_ELECT_ADDR_ADDR							ST_6_SERVICE_POINT_ID_ADDR					+ ST_6_SERVICE_POINT_ID_SIZE		
#define ST_6_DEVICE_ID_ADDR								ST_6_ELECT_ADDR_ADDR						+ ST_6_ELECT_ADDR_SIZE			
#define ST_6_UTIL_SER_NO_ADDR							ST_6_DEVICE_ID_ADDR							+ ST_6_DEVICE_ID_SIZE				
#define ST_6_CUSTOMER_ID_ADDR							ST_6_UTIL_SER_NO_ADDR						+ ST_6_UTIL_SER_NO_SIZE			
#define ST_6_COORDINATE_1_ADDR							ST_6_CUSTOMER_ID_ADDR						+ ST_6_CUSTOMER_ID_SIZE			
#define ST_6_COORDINATE_2_ADDR							ST_6_COORDINATE_1_ADDR						+ ST_6_COORDINATE_1_SIZE			
#define ST_6_COORDINATE_3_ADDR							ST_6_COORDINATE_2_ADDR						+ ST_6_COORDINATE_2_SIZE			
#define ST_6_TARIFF_ID_ADDR								ST_6_COORDINATE_3_ADDR						+ ST_6_COORDINATE_3_SIZE			
#define ST_6_EX1_SW_VENDOR_ADDR							ST_6_TARIFF_ID_ADDR							+ ST_6_TARIFF_ID_SIZE				
#define ST_6_EX1_SW_VERSION_NUMBER_ADDR					ST_6_EX1_SW_VENDOR_ADDR						+ ST_6_EX1_SW_VENDOR_SIZE			
#define ST_6_EX1_SW_REVISION_NUMBER_ADDR				ST_6_EX1_SW_VERSION_NUMBER_ADDR				+ ST_6_EX1_SW_VERSION_NUMBER_SIZE	
#define ST_6_EX2_SW_VENDOR_ADDR							ST_6_EX1_SW_REVISION_NUMBER_ADDR			+ ST_6_EX1_SW_REVISION_NUMBER_SIZE
#define ST_6_EX2_SW_VERSION_NUMBER_ADDR					ST_6_EX2_SW_VENDOR_ADDR						+ ST_6_EX2_SW_VENDOR_SIZE			
#define ST_6_EX2_SW_REVISION_NUMBER_ADDR				ST_6_EX2_SW_VERSION_NUMBER_ADDR				+ ST_6_EX2_SW_VERSION_NUMBER_SIZE	
#define ST_6_PROGRAMMER_NAME_ADDR						ST_6_EX2_SW_REVISION_NUMBER_ADDR			+ ST_6_EX2_SW_REVISION_NUMBER_SIZE
#define ST_6_MISC_ID_ADDR								ST_6_PROGRAMMER_NAME_ADDR					+ ST_6_PROGRAMMER_NAME_SIZE	
#define ST_6_FW_LOAD_DATE_ADDR							ST_6_MISC_ID_ADDR							+ ST_6_MISC_ID_SIZE	

/*===========================================================================*\
 * ST_11: Actual Data Sources Limiting Table
\*===========================================================================*/
#define ST_11_SOURCE_FLAGS_ADDR							ST_11_START_ADDR
#define ST_11_NBR_UOM_ENTRIES_ADDR						ST_11_SOURCE_FLAGS_ADDR						+ ST_11_SOURCE_FLAGS_SIZE				
#define ST_11_NBR_DEMAND_CTRL_ENTRIES_ADDR				ST_11_NBR_UOM_ENTRIES_ADDR					+ ST_11_NBR_UOM_ENTRIES_SIZE			
#define ST_11_DATA_CTRL_LENGTH_ADDR						ST_11_NBR_DEMAND_CTRL_ENTRIES_ADDR			+ ST_11_NBR_DEMAND_CTRL_ENTRIES_SIZE	
#define ST_11_NBR_DATA_CTRL_ENTRIES_ADDR				ST_11_DATA_CTRL_LENGTH_ADDR					+ ST_11_DATA_CTRL_LENGTH_SIZE			
#define ST_11_NBR_CONSTANTS_ENTRIES_ADDR				ST_11_NBR_DATA_CTRL_ENTRIES_ADDR			+ ST_11_NBR_DATA_CTRL_ENTRIES_SIZE	
#define ST_11_CONSTANTS_SELECTOR_ADDR					ST_11_NBR_CONSTANTS_ENTRIES_ADDR			+ ST_11_NBR_CONSTANTS_ENTRIES_SIZE	
#define ST_11_NBR_SOURCES_ADDR							ST_11_CONSTANTS_SELECTOR_ADDR				+ ST_11_CONSTANTS_SELECTOR_SIZE		

/*===========================================================================*\
 * ST_13: Demand Control Table
\*===========================================================================*/
#define ST_13_RESET_EXCLUSION_ADDR						ST_13_START_ADDR
#define ST_13_P_FAIL_RECOGNTN_TM_ADDR					ST_13_RESET_EXCLUSION_ADDR					+ ST_13_RESET_EXCLUSION_SIZE		
#define ST_13_P_FAIL_EXCLUSION_ADDR						ST_13_P_FAIL_RECOGNTN_TM_ADDR				+ ST_13_P_FAIL_RECOGNTN_TM_SIZE	
#define ST_13_COLD_LOAD_PICKUP_ADDR						ST_13_P_FAIL_EXCLUSION_ADDR					+ ST_13_P_FAIL_EXCLUSION_SIZE		
#define ST_13_SUB_INT_ADDR								ST_13_COLD_LOAD_PICKUP_ADDR					+ ST_13_COLD_LOAD_PICKUP_SIZE		
#define ST_13_INT_MULTIPLIER_ADDR						ST_13_SUB_INT_ADDR							+ ST_13_SUB_INT_SIZE				
#define ST_13_INT_LENGTH_ADDR							ST_13_COLD_LOAD_PICKUP_ADDR					+ ST_13_COLD_LOAD_PICKUP_SIZE

/*===========================================================================*\
 * ST_21: Actual Register Limiting Table
\*===========================================================================*/
#define ST_21_REG_FUNC1_BFLD_ADDR						ST_21_START_ADDR
#define ST_21_REG_FUNC2_BFLD_ADDR						ST_21_REG_FUNC1_BFLD_ADDR					+ ST_21_REG_FUNC1_BFLD_SIZE		
#define ST_21_NBR_SELF_READS_ADDR						ST_21_REG_FUNC2_BFLD_ADDR					+ ST_21_REG_FUNC2_BFLD_SIZE		
#define ST_21_NBR_SUMMATIONS_ADDR						ST_21_NBR_SELF_READS_ADDR					+ ST_21_NBR_SELF_READS_SIZE		
#define ST_21_NBR_DEMANDS_ADDR							ST_21_NBR_SUMMATIONS_ADDR					+ ST_21_NBR_SUMMATIONS_SIZE		
#define ST_21_NBR_COIN_VALUES_ADDR						ST_21_NBR_DEMANDS_ADDR						+ ST_21_NBR_DEMANDS_SIZE			
#define ST_21_NBR_OCCUR_ADDR							ST_21_NBR_COIN_VALUES_ADDR					+ ST_21_NBR_COIN_VALUES_SIZE		
#define ST_21_NBR_TIERS_ADDR							ST_21_NBR_OCCUR_ADDR						+ ST_21_NBR_OCCUR_SIZE			
#define ST_21_NBR_PRESENT_DEMANDS_ADDR					ST_21_NBR_TIERS_ADDR						+ ST_21_NBR_TIERS_SIZE			
#define ST_21_NBR_PRESENT_VALUES_ADDR					ST_21_NBR_PRESENT_DEMANDS_ADDR				+ ST_21_NBR_PRESENT_DEMANDS_SIZE	

/*===========================================================================*\
 * ST_22: Data Selection Table
\*===========================================================================*/
#define ST_22_SUMMATION_SELECT_ADDR						ST_22_START_ADDR
#define ST_22_DEMAND_SELECT_ADDR						ST_22_SUMMATION_SELECT_ADDR					+ ST_22_SUMMATION_SELECT_SIZE	
#define ST_22_MIN_OR_MAX_FLAGS_ADDR						ST_22_DEMAND_SELECT_ADDR					+ ST_22_DEMAND_SELECT_SIZE	
#define ST_22_COINCIDENT_SELECT_ADDR					ST_22_MIN_OR_MAX_FLAGS_ADDR					+ ST_22_MIN_OR_MAX_FLAGS_SIZE	
#define ST_22_COIN_DEMAND_ASSOC_ADDR					ST_22_COINCIDENT_SELECT_ADDR				+ ST_22_COINCIDENT_SELECT_SIZE
														
/*===========================================================================*\
 * ST_23: Current Register Data Table
\*===========================================================================*/
#define ST_23_NBR_DEMAND_RESETS_ADDR					ST_23_START_ADDR		
#define ST_23_TOT_SUMMATIONS_ADDR						ST_23_NBR_DEMAND_RESETS_ADDR				+ ST_23_NBR_DEMAND_RESETS_SIZE
#define ST_23_TOT_DEMANDS_EVENT_TIME_ADDR				ST_23_TOT_SUMMATIONS_ADDR					+ ST_23_TOT_SUMMATIONS_SIZE
#define ST_23_TOT_DEMANDS_CUM_DEMAND_ADDR				ST_23_TOT_DEMANDS_EVENT_TIME_ADDR			+ ST_23_TOT_DEMANDS_EVENT_TIME_SIZE
#define ST_23_TOT_DEMANDS_CONT_CUM_DEMAND_ADDR			ST_23_TOT_DEMANDS_CUM_DEMAND_ADDR			+ ST_23_TOT_DEMANDS_CUM_DEMAND_SIZE
#define ST_23_TOT_DEMANDS_DEMAND_ADDR					ST_23_TOT_DEMANDS_CONT_CUM_DEMAND_ADDR		+ ST_23_TOT_DEMANDS_CONT_CUM_DEMAND_SIZE
#define ST_23_TOT_COINCIDENTS_ADDR						ST_23_TOT_DEMANDS_DEMAND_ADDR				+ ST_23_TOT_DEMANDS_DEMAND_SIZE											
#define ST_23_TIER_SUMMATIONS_ADDR						ST_23_TOT_COINCIDENTS_ADDR					+ ST_23_TOT_COINCIDENTS_SIZE
#define ST_23_TIER_DEMANDS_EVENT_TIME_ADDR				ST_23_TIER_SUMMATIONS_ADDR					+ ST_23_TIER_SUMMATIONS_SIZE
#define ST_23_TIER_DEMANDS_CUM_DEMAND_ADDR				ST_23_TIER_DEMANDS_EVENT_TIME_ADDR			+ ST_23_TIER_DEMANDS_EVENT_TIME_SIZE
#define ST_23_TIER_DEMANDS_CONT_CUM_DEMAND_ADDR			ST_23_TIER_DEMANDS_CUM_DEMAND_ADDR			+ ST_23_TIER_DEMANDS_CUM_DEMAND_SIZE
#define ST_23_TIER_DEMANDS_DEMAND_ADDR					ST_23_TIER_DEMANDS_CONT_CUM_DEMAND_ADDR		+ ST_23_TIER_DEMANDS_CONT_CUM_DEMAND_SIZE
#define ST_23_TIER_COINCIDENTS_ADDR						ST_23_TIER_DEMANDS_DEMAND_ADDR				+ ST_23_TIER_DEMANDS_DEMAND_SIZE

/*===========================================================================*\
 * ST_24: Previous Season Data Table
\*===========================================================================*/
#define ST_24_END_DATE_TIME_ADDR						ST_24_START_ADDR
#define ST_24_SEASON_ADDR								ST_24_END_DATE_TIME_ADDR					+ ST_24_END_DATE_TIME_SIZE
#define ST_24_PREV_SEASON_REG_DATA_ADDR					ST_24_SEASON_ADDR							+ ST_24_SEASON_SIZE

/*===========================================================================*\
 * ST_25: Previous Demand Reset Data Table
\*===========================================================================*/
#define ST_25_END_DATE_TIME_ADDR						ST_25_START_ADDR
#define ST_25_SEASON_ADDR								ST_25_END_DATE_TIME_ADDR					+ ST_25_END_DATE_TIME_SIZE
#define ST_25_PREV_DEMAND_RESET_DATA_ADDR				ST_25_SEASON_ADDR							+ ST_25_SEASON_SIZE

/*===========================================================================*\
 * ST_26: Self-read Data Table
\*===========================================================================*/
#define ST_26_LIST_STATUS_ADDR							ST_26_START_ADDR
#define ST_26_NBR_VALID_ENTRIES_ADDR					ST_26_LIST_STATUS_ADDR						+ ST_26_LIST_STATUS_SIZE
#define ST_26_LAST_ENTRY_ELEMENT_ADDR					ST_26_NBR_VALID_ENTRIES_ADDR				+ ST_26_NBR_VALID_ENTRIES_SIZE
#define ST_26_LAST_ENTRY_SEQ_NBR_ADDR					ST_26_LAST_ENTRY_ELEMENT_ADDR				+ ST_26_LAST_ENTRY_ELEMENT_SIZE
#define ST_26_NBR_UNREAD_ENTRIES_ADDR					ST_26_LAST_ENTRY_SEQ_NBR_ADDR				+ ST_26_LAST_ENTRY_SEQ_NBR_SIZE
#define ST_26_SELF_READ_SEQ_NBR_ADDR					ST_26_NBR_UNREAD_ENTRIES_ADDR				+ ST_26_NBR_UNREAD_ENTRIES_SIZE
#define ST_26_END_DATE_TIME_ADDR						ST_26_SELF_READ_SEQ_NBR_ADDR				+ ST_26_SELF_READ_SEQ_NBR_SIZE
#define ST_26_SEASON_ADDR								ST_26_END_DATE_TIME_ADDR					+ ST_26_END_DATE_TIME_SIZE
#define ST_26_SELF_READ_REGISTER_DATA_ADDR				ST_26_SEASON_ADDR							+ ST_26_SEASON_SIZE
														
/*===========================================================================*\
 * ST_27: Present Register Selection Table
\*===========================================================================*/
#define ST_27_PRESENT_DEMAND_SELECT_ADDR				ST_27_START_ADDR
#define ST_27_PRESENT_VALUE_SELECT_ADDR					ST_27_PRESENT_DEMAND_SELECT_ADDR			+ ST_27_PRESENT_DEMAND_SELECT_SIZE

/*===========================================================================*\
 * ST_41: Actual Security Limiting Table
\*===========================================================================*/
#define ST_41_NBR_PASSWORDS_ADDR						ST_41_START_ADDR
#define ST_41_PASSWORD_LEN_ADDR							ST_41_NBR_PASSWORDS_ADDR					+ ST_41_NBR_PASSWORDS_SIZE
#define ST_41_NBR_KEYS_ADDR								ST_41_PASSWORD_LEN_ADDR						+ ST_41_PASSWORD_LEN_SIZE	
#define ST_41_KEY_LEN_ADDR								ST_41_NBR_KEYS_ADDR							+ ST_41_NBR_KEYS_SIZE		
#define ST_41_NBR_PERM_USED_ADDR						ST_41_KEY_LEN_ADDR							+ ST_41_KEY_LEN_SIZE		
																												
/*===========================================================================*\
 * ST_42: Security Table
\*===========================================================================*/
#define ST_42_PASSWORD_ADDR								ST_42_START_ADDR
#define ST_42_ACCESS_PERMISSION_BFLD_ADDR				ST_42_PASSWORD_ADDR							+ ST_42_PASSWORD_SIZE				

/*===========================================================================*\
 * ST_43: Default Access Control Table
\*===========================================================================*/
#define ST_43_ACCESS_TABLE_DEFAULT_ADDR					ST_43_START_ADDR
#define ST_43_TABLE_ACCESS_PERM_BFLD_READ_ADDR			ST_43_ACCESS_TABLE_DEFAULT_ADDR				+ ST_43_ACCESS_TABLE_DEFAULT_SIZE				
#define ST_43_TABLE_ACCESS_PERM_BFLD_WRITE_ADDR			ST_43_TABLE_ACCESS_PERM_BFLD_READ_ADDR		+ ST_43_TABLE_ACCESS_PERM_BFLD_READ_SIZE		
#define ST_43_ACCESS_PROCEDURE_DEFAULT_ADDR				ST_43_TABLE_ACCESS_PERM_BFLD_WRITE_ADDR		+ ST_43_TABLE_ACCESS_PERM_BFLD_WRITE_SIZE		
#define ST_43_PROCEDURE_ACCESS_PERM_BFLD_READ_ADDR		ST_43_ACCESS_PROCEDURE_DEFAULT_ADDR			+ ST_43_ACCESS_PROCEDURE_DEFAULT_SIZE			
#define ST_43_PROCEDURE_ACCESS_PERM_BFLD_WRITE_ADDR		ST_43_PROCEDURE_ACCESS_PERM_BFLD_READ_ADDR	+ ST_43_PROCEDURE_ACCESS_PERM_BFLD_READ_SIZE	

/*===========================================================================*\
 * ST_44: Access Control Table
\*===========================================================================*/
#define ST_44_ACCESS_TABLE_DEFAULT_ADDR					ST_44_START_ADDR
#define ST_44_TABLE_ACCESS_PERMISSION_BFLD_READ_ADDR	ST_44_ACCESS_TABLE_DEFAULT_ADDR				  + ST_44_ACCESS_TABLE_DEFAULT_SIZE
#define ST_44_TABLE_ACCESS_PERMISSION_BFLD_WRITE_ADDR	ST_44_TABLE_ACCESS_PERMISSION_BFLD_READ_ADDR  + ST_44_TABLE_ACCESS_PERMISSION_BFLD_READ_SIZE

/*===========================================================================*\
 * ST_51: Time Offset Table
\*===========================================================================*/
#define ST_51_TIME_FUNC_FLAG1_ADDR						ST_51_START_ADDR							
#define ST_51_TIME_FUNC_FLAG2_ADDR						ST_51_TIME_FUNC_FLAG1_ADDR					+ ST_51_TIME_FUNC_FLAG1_SIZE		
#define ST_51_CALENDAR_FUNC_ADDR						ST_51_TIME_FUNC_FLAG2_ADDR					+ ST_51_TIME_FUNC_FLAG2_SIZE		
#define ST_51_NBR_NON_RECURR_DATES_ADDR					ST_51_CALENDAR_FUNC_ADDR					+ ST_51_CALENDAR_FUNC_SIZE		
#define ST_51_NBR_RECURR_DATES_ADDR						ST_51_NBR_NON_RECURR_DATES_ADDR				+ ST_51_NBR_NON_RECURR_DATES_SIZE
#define ST_51_NBR_TIER_SWITCHES_ADDR					ST_51_NBR_RECURR_DATES_ADDR					+ ST_51_NBR_RECURR_DATES_SIZE		
#define ST_51_CALENDAR_TBL_SIZE_ADDR					ST_51_NBR_TIER_SWITCHES_ADDR				+ ST_51_NBR_TIER_SWITCHES_SIZE	

/*===========================================================================*\
 * ST_53: Time Offset Table
\*===========================================================================*/						
#define ST_53_DST_TIME_EFF_ADDR							ST_53_START_ADDR							
#define ST_53_DST_TIME_AMT_ADDR							ST_53_DST_TIME_EFF_ADDR						+ ST_53_DST_TIME_EFF_SIZE		
#define ST_53_TIME_ZONE_OFFSET_ADDR						ST_53_DST_TIME_AMT_ADDR						+ ST_53_DST_TIME_AMT_SIZE		
#define ST_53_STD_TIME_EFF_ADDR							ST_53_TIME_ZONE_OFFSET_ADDR					+ ST_53_TIME_ZONE_OFFSET_SIZE	

/*===========================================================================*\
 * ST_54: Calendar Table
\*===========================================================================*/
#define ST_54_ANCHOR_DATE_ADDR							ST_54_START_ADDR
#define ST_54_NON_RECURR_DATES_ADDR						ST_54_ANCHOR_DATE_ADDR						+ ST_54_ANCHOR_DATE_SIZE				
#define ST_54_RECURR_DATES_ADDR							ST_54_NON_RECURR_DATES_ADDR					+ ST_54_NON_RECURR_DATES_SIZE			
#define ST_54_TIER_SWITCHES_ADDR						ST_54_RECURR_DATES_ADDR						+ ST_54_RECURR_DATES_SIZE				
#define ST_54_DAILY_SCHEDULE_ID_MATRIX_ADDR				ST_54_TIER_SWITCHES_ADDR					+ ST_54_TIER_SWITCHES_SIZE

/*===========================================================================*\
 * TOU reserved addresses
\*===========================================================================*/
#define TOU_LAST_SEASON_ADDR							TOU_BACKUP_START_ADDR						// 15 BYTES RESERVED

/*===========================================================================*\
 * ST_61: Actual Load Profile Limiting Table
\*===========================================================================*/
#define ST_61_LP_MEMORY_LEN_ADDR						ST_61_START_ADDR							
#define ST_61_LP_FLAGS_ADDR								ST_61_LP_MEMORY_LEN_ADDR					+ ST_61_LP_MEMORY_LEN_SIZE	
#define ST_61_LP_FMATS_ADDR								ST_61_LP_FLAGS_ADDR							+ ST_61_LP_FLAGS_SIZE			
#define ST_61_NBR_BLKS_SET1_ADDR						ST_61_LP_FMATS_ADDR							+ ST_61_LP_FMATS_SIZE			
#define ST_61_NBR_BLK_INTS_SET1_ADDR					ST_61_NBR_BLKS_SET1_ADDR					+ ST_61_NBR_BLKS_SET1_SIZE	
#define ST_61_NBR_CHNS_SET1_ADDR						ST_61_NBR_BLK_INTS_SET1_ADDR				+ ST_61_NBR_BLK_INTS_SET1_SIZE
#define ST_61_MAX_INT_TIME_SET1_ADDR					ST_61_NBR_CHNS_SET1_ADDR					+ ST_61_NBR_CHNS_SET1_SIZE	

/*===========================================================================*\
 * ST_62: Load Profile Control Table
\*===========================================================================*/
#define ST_62_CHNL_FLAG_SET1_ADDR						ST_62_START_ADDR
#define ST_62_LP_SOURCE_SELECT_SET1_ADDR				ST_62_CHNL_FLAG_SET1_ADDR					+ ST_62_CHNL_FLAG_SET1_SIZE					
#define ST_62_END_BLK_RDG_SOURCE_SELECT_SET1_ADDR		ST_62_LP_SOURCE_SELECT_SET1_ADDR			+ ST_62_LP_SOURCE_SELECT_SET1_SIZE			
#define ST_62_INT_FMT_CDE1_ADDR							ST_62_END_BLK_RDG_SOURCE_SELECT_SET1_ADDR	+ ST_62_END_BLK_RDG_SOURCE_SELECT_SET1_SIZE	
#define ST_62_SCALARS_SET1_ADDR							ST_62_INT_FMT_CDE1_ADDR						+ ST_62_INT_FMT_CDE1_SIZE						
#define ST_62_DIVISOR_SET1_ADDR							ST_62_SCALARS_SET1_ADDR						+ ST_62_SCALARS_SET1_SIZE						

/*===========================================================================*\
 * ST_63: Load Profile Control Table
\*===========================================================================*/
#define ST_63_SET_STATUS_FLAGS_ADDR						ST_63_START_ADDR
#define ST_63_NBR_VALID_BLOCKS_ADDR						ST_63_SET_STATUS_FLAGS_ADDR					+ ST_63_SET_STATUS_FLAGS_SIZE		
#define ST_63_LAST_BLOCK_ELEMENT_ADDR					ST_63_NBR_VALID_BLOCKS_ADDR					+ ST_63_NBR_VALID_BLOCKS_SIZE		
#define ST_63_LAST_BLOCK_SEQ_NBR_ADDR					ST_63_LAST_BLOCK_ELEMENT_ADDR				+ ST_63_LAST_BLOCK_ELEMENT_SIZE	
#define ST_63_NBR_UNREAD_BLOCKS_ADDR					ST_63_LAST_BLOCK_SEQ_NBR_ADDR				+ ST_63_LAST_BLOCK_SEQ_NBR_SIZE	
#define ST_63_NBR_VALID_INT_ADDR						ST_63_NBR_UNREAD_BLOCKS_ADDR				+ ST_63_NBR_UNREAD_BLOCKS_SIZE	

/*===========================================================================*\
 * ST_71: Actual Log Limiting Table
\*===========================================================================*/
#define ST_71_LOG_FLAGS_ADDR							ST_71_START_ADDR							 
#define ST_71_NBR_STD_EVENTS_ADDR						ST_71_LOG_FLAGS_ADDR						+ ST_71_LOG_FLAGS_SIZE
#define ST_71_NBR_MFG_EVENTS_ADDR						ST_71_NBR_STD_EVENTS_ADDR					+ ST_71_NBR_STD_EVENTS_SIZE
#define ST_71_HIST_DATA_LENGTH_ADDR						ST_71_NBR_MFG_EVENTS_ADDR					+ ST_71_NBR_MFG_EVENTS_SIZE
#define ST_71_EVENT_DATA_LENGTH_ADDR					ST_71_HIST_DATA_LENGTH_ADDR					+ ST_71_HIST_DATA_LENGTH_SIZE
#define ST_71_NBR_HISTORY_ENTRIES_ADDR					ST_71_EVENT_DATA_LENGTH_ADDR				+ ST_71_EVENT_DATA_LENGTH_SIZE
#define	ST_71_NBR_EVENT_ENTRIES_ADDR					ST_71_NBR_HISTORY_ENTRIES_ADDR				+ ST_71_NBR_HISTORY_ENTRIES_SIZE

/*===========================================================================*\
 * ST_73: History Log Control Table
\*===========================================================================*/
#define ST_73_STD_EVENTS_MONITORED_FLAGS_ADDR			ST_73_START_ADDR						
#define ST_73_MFG_EVENTS_MONITORED_FLAGS_ADDR			ST_73_STD_EVENTS_MONITORED_FLAGS_ADDR		+ ST_73_STD_EVENTS_MONITORED_FLAGS_SIZE	
#define ST_73_STD_TBLS_MONITORED_FLAGS_ADDR				ST_73_MFG_EVENTS_MONITORED_FLAGS_ADDR		+ ST_73_MFG_EVENTS_MONITORED_FLAGS_SIZE	
#define ST_73_MFG_TBLS_MONITORED_FLAGS_ADDR				ST_73_STD_TBLS_MONITORED_FLAGS_ADDR			+ ST_73_STD_TBLS_MONITORED_FLAGS_SIZE		
#define ST_73_STD_PROC_MONITORED_FLAGS_ADDR				ST_73_MFG_TBLS_MONITORED_FLAGS_ADDR			+ ST_73_MFG_TBLS_MONITORED_FLAGS_SIZE		
#define ST_73_MFG_PROC_MONITORED_FLAGS_ADDR				ST_73_STD_PROC_MONITORED_FLAGS_ADDR			+ ST_73_STD_PROC_MONITORED_FLAGS_SIZE		

/*===========================================================================*\
 * ST_74: History Log Data Table
\*===========================================================================*/
#define ST_74_HIST_FLAGS_ADDR							ST_74_START_ADDR							
#define ST_74_NBR_VALID_ENTRIES_ADDR					ST_74_HIST_FLAGS_ADDR						+ ST_74_HIST_FLAGS_SIZE			
#define ST_74_LAST_ENTRY_ELEMENT_ADDR					ST_74_NBR_VALID_ENTRIES_ADDR				+ ST_74_NBR_VALID_ENTRIES_SIZE	
#define ST_74_LAST_ENTRY_SEQ_NBR_ADDR					ST_74_LAST_ENTRY_ELEMENT_ADDR				+ ST_74_LAST_ENTRY_ELEMENT_SIZE	
#define ST_74_NBR_UNREAD_ENTRIES_ADDR					ST_74_LAST_ENTRY_SEQ_NBR_ADDR				+ ST_74_LAST_ENTRY_SEQ_NBR_SIZE	
#define ST_74_HISTORY_TIME_ADDR							ST_74_NBR_UNREAD_ENTRIES_ADDR				+ ST_74_NBR_UNREAD_ENTRIES_SIZE	
#define ST_74_EVENT_NUMBER_ADDR							ST_74_HISTORY_TIME_ADDR						+ ST_74_HISTORY_TIME_SIZE		
#define ST_74_HISTORY_SEQ_NBR_ADDR						ST_74_EVENT_NUMBER_ADDR						+ ST_74_EVENT_NUMBER_SIZE		
#define ST_74_USER_ID_ADDR								ST_74_HISTORY_SEQ_NBR_ADDR					+ ST_74_HISTORY_SEQ_NBR_SIZE		
#define ST_74_HISTORY_CODE_ADDR							ST_74_USER_ID_ADDR							+ ST_74_USER_ID_SIZE				
#define ST_74_HISTORY_ARGUMENT_ADDR						ST_74_HISTORY_CODE_ADDR						+ ST_74_HISTORY_CODE_SIZE		

/*===========================================================================*\
 * ST_75: Event Log Control Table
\*===========================================================================*/
#define ST_75_STD_EVENTS_MONITORED_FLAGS_ADDR			ST_75_START_ADDR
#define ST_75_MFG_EVENTS_MONITORED_FLAGS_ADDR			ST_75_STD_EVENTS_MONITORED_FLAGS_ADDR		+ ST_75_STD_EVENTS_MONITORED_FLAGS_SIZE
#define ST_75_STD_TBLS_MONITORED_FLAGS_ADDR				ST_75_MFG_EVENTS_MONITORED_FLAGS_ADDR		+ ST_75_MFG_EVENTS_MONITORED_FLAGS_SIZE
#define ST_75_MFG_TBLS_MONITORED_FLAGS_ADDR				ST_75_STD_TBLS_MONITORED_FLAGS_ADDR			+ ST_75_STD_TBLS_MONITORED_FLAGS_SIZE
#define ST_75_STD_PROC_MONITORED_FLAGS_ADDR				ST_75_MFG_TBLS_MONITORED_FLAGS_ADDR			+ ST_75_MFG_TBLS_MONITORED_FLAGS_SIZE
#define ST_75_MFG_PROC_MONITORED_FLAGS_ADDR				ST_75_STD_PROC_MONITORED_FLAGS_ADDR			+ ST_75_STD_PROC_MONITORED_FLAGS_SIZE

/*===========================================================================*\
 * ST_76: Event Log Data Table
\*===========================================================================*/
#define ST_76_EVENT_FLAGS_ADDR							ST_76_START_ADDR							
#define ST_76_NBR_VALID_ENTRIES_ADDR					ST_76_EVENT_FLAGS_ADDR						+ ST_76_EVENT_FLAGS_SIZE			
#define ST_76_LAST_ENTRY_ELEMENT_ADDR					ST_76_NBR_VALID_ENTRIES_ADDR				+ ST_76_NBR_VALID_ENTRIES_SIZE	
#define ST_76_LAST_ENTRY_SEQ_NBR_ADDR					ST_76_LAST_ENTRY_ELEMENT_ADDR				+ ST_76_LAST_ENTRY_ELEMENT_SIZE	
#define ST_76_NBR_UNREAD_ENTRIES_ADDR					ST_76_LAST_ENTRY_SEQ_NBR_ADDR				+ ST_76_LAST_ENTRY_SEQ_NBR_SIZE	
#define ST_76_EVENT_TIME_ADDR							ST_76_NBR_UNREAD_ENTRIES_ADDR				+ ST_76_NBR_UNREAD_ENTRIES_SIZE	
#define ST_76_EVENT_NUMBER_ADDR							ST_76_EVENT_TIME_ADDR						+ ST_76_EVENT_TIME_SIZE			
#define ST_76_EVENT_SEQ_NBR_ADDR						ST_76_EVENT_NUMBER_ADDR						+ ST_76_EVENT_NUMBER_SIZE			
#define ST_76_USER_ID_ADDR								ST_76_EVENT_SEQ_NBR_ADDR					+ ST_76_EVENT_SEQ_NBR_SIZE		
#define ST_76_EVENT_CODE_ADDR							ST_76_USER_ID_ADDR							+ ST_76_USER_ID_SIZE				
#define ST_76_EVENT_ARGUMENT_ADDR						ST_76_EVENT_CODE_ADDR						+ ST_76_EVENT_CODE_SIZE			

/*===========================================================================*\
 * MT_1: EOS Specific Product Identification
\*===========================================================================*/
#define MT_1_FW_PART_NUMBER_ADDR						MT_1_START_ADDR							
#define MT_1_HW_PART_NUMBER_ADDR						MT_1_FW_PART_NUMBER_ADDR					+ MT_1_FW_PART_NUMBER_SIZE
#define MT_1_ED_IDENTITY_ADDR							MT_1_HW_PART_NUMBER_ADDR					+ MT_1_HW_PART_NUMBER_SIZE
#define MT_1_BOARD_ASSEMBLY_SERIAL_NUMBER_ADDR			MT_1_ED_IDENTITY_ADDR						+ MT_1_ED_IDENTITY_SIZE
#define MT_1_MFG_MODULE_SERIAL_NUMBER_ADDR				MT_1_BOARD_ASSEMBLY_SERIAL_NUMBER_ADDR		+ MT_1_BOARD_ASSEMBLY_SERIAL_NUMBER_SIZE

/*===========================================================================*\
 * MT_3: Auxiliary Configuration Table
\*===========================================================================*/
#define MT_3_NBR_POWER_OUTAGES_ADDR						MT_3_START_ADDR
#define MT_3_RESERVED_ADDR								MT_3_NBR_POWER_OUTAGES_ADDR					+ MT_3_NBR_POWER_OUTAGES_SIZE

/*===========================================================================*\
 * MT_23: Operating Voltage Limits Table
\*===========================================================================*/
#define MT_23_VOLT_LOW_LIMIT_ADDR						MT_23_START_ADDR
#define MT_23_VOLT_HIGH_LIMIT_ADDR						MT_23_VOLT_LOW_LIMIT_ADDR					+ MT_23_VOLT_LOW_LIMIT_SIZE	

/*===========================================================================*\
 * MT_4: MCU Diagnostics Table
\*===========================================================================*/
#define MT_4_MM100_TIME_RUNNING_ADDR					MT_4_START_ADDR								
#define MT_4_MCU_RESET_TOTAL_COUNTER_ADDR				MT_4_MM100_TIME_RUNNING_ADDR					+ MT_4_MM100_TIME_RUNNING_SIZE					
#define MT_4_MCU_RESET_POWER_FAILURE_ADDR				MT_4_MCU_RESET_TOTAL_COUNTER_ADDR				+ MT_4_MCU_RESET_TOTAL_COUNTER_SIZE
#define MT_4_MCU_RESET_FIRMWARE_UPDATE_ADDR				MT_4_MCU_RESET_POWER_FAILURE_ADDR				+ MT_4_MCU_RESET_POWER_FAILURE_SIZE
#define MT_4_MCU_RESET_WATCH_DOG_ADDR					MT_4_MCU_RESET_FIRMWARE_UPDATE_ADDR				+ MT_4_MCU_RESET_FIRMWARE_UPDATE_SIZE
#define MT_4_MCU_RESET_COLD_START_ADDR					MT_4_MCU_RESET_WATCH_DOG_ADDR					+ MT_4_MCU_RESET_WATCH_DOG_SIZE
#define MT_4_MCU_RESET_WARM_START_ADDR					MT_4_MCU_RESET_COLD_START_ADDR					+ MT_4_MCU_RESET_COLD_START_SIZE
#define MT_4_MCU_RESET_KT_CONSTANT_CHANGE_ADDR			MT_4_MCU_RESET_SYSTEM_FAULT_ADDR				+ MT_4_MCU_RESET_SYSTEM_FAULT_SIZE
#define MT_4_MCU_RESET_SYSTEM_FAULT_ADDR				MT_4_MCU_RESET_WARM_START_ADDR					+ MT_4_MCU_RESET_WARM_START_SIZE
#define MT_4_MCU_RESET_VLISTINSERT_COUNTER_ADDR			MT_4_MCU_RESET_KT_CONSTANT_CHANGE_ADDR			+ MT_4_MCU_RESET_TOTAL_COUNTER_SIZE				
#define MT_4_MCU_RESET_DUMMY_HANDLER_COUNTER_ADDR		MT_4_MCU_RESET_VLISTINSERT_COUNTER_ADDR			+ MT_4_MCU_RESET_VLISTINSERT_COUNTER_SIZE		
#define MT_4_MCU_RESET_ASSERT_TRIGGERED_COUNTER_ADDR	MT_4_MCU_RESET_DUMMY_HANDLER_COUNTER_ADDR		+ MT_4_MCU_RESET_DUMMY_HANDLER_COUNTER_SIZE		
#define MT_4_MCU_RESET_STACK_OVERFLOW_COUNTER_ADDR		MT_4_MCU_RESET_ASSERT_TRIGGERED_COUNTER_ADDR	+ MT_4_MCU_RESET_ASSERT_TRIGGERED_COUNTER_SIZE	
#define MT_4_RESERVED_ADDR								MT_4_MCU_RESET_STACK_OVERFLOW_COUNTER_ADDR		+ MT_4_MCU_RESET_STACK_OVERFLOW_COUNTER_SIZE		

/*===========================================================================*\
 * Mt_5: Table Integrity Diagnostic
\*===========================================================================*/
#define MT_5_ST11_ERROR_CODE_ADDR						MT_5_START_ADDR + 0
#define MT_5_ST13_ERROR_CODE_ADDR						MT_5_START_ADDR + 1
#define MT_5_ST21_ERROR_CODE_ADDR						MT_5_START_ADDR + 2
#define MT_5_ST22_ERROR_CODE_ADDR						MT_5_START_ADDR + 3
#define MT_5_ST27_ERROR_CODE_ADDR						MT_5_START_ADDR + 4
#define MT_5_ST41_ERROR_CODE_ADDR						MT_5_START_ADDR + 5
#define MT_5_ST42_ERROR_CODE_ADDR						MT_5_START_ADDR + 6
#define MT_5_ST43_ERROR_CODE_ADDR						MT_5_START_ADDR + 7
#define MT_5_ST44_ERROR_CODE_ADDR						MT_5_START_ADDR + 8
#define MT_5_ST51_ERROR_CODE_ADDR						MT_5_START_ADDR + 9
#define MT_5_ST54_ERROR_CODE_ADDR						MT_5_START_ADDR + 10
#define MT_5_ST61_ERROR_CODE_ADDR						MT_5_START_ADDR + 11
#define MT_5_ST62_ERROR_CODE_ADDR						MT_5_START_ADDR + 12
#define MT_5_ST71_ERROR_CODE_ADDR						MT_5_START_ADDR + 13
#define MT_5_ST73_ERROR_CODE_ADDR						MT_5_START_ADDR + 14
#define MT_5_ST75_ERROR_CODE_ADDR						MT_5_START_ADDR + 15
#define MT_5_MT73_ERROR_CODE_ADDR						MT_5_START_ADDR + 16
																										  								
/*===========================================================================*\
 * MT_53: Time Zone Offset Description
\*===========================================================================*/
#define MT_53_WIN_TIMEZONE_INDEX_ADDR					MT_53_START_ADDR
#define MT_53_WIN_TIMEZONE_NAME_ADDR					MT_53_WIN_TIMEZONE_INDEX_ADDR				+ MT_53_WIN_TIMEZONE_INDEX_SIZE	

/*===========================================================================*\
 * ST_73: History Log Control Table
\*===========================================================================*/
#define MT_73_STD_ALARMS_MONITORED_FLAGS_ADDR			MT_73_START_ADDR						
#define MT_73_MFG_ALARMS_MONITORED_FLAGS_ADDR			MT_73_STD_ALARMS_MONITORED_FLAGS_ADDR		+ MT_73_STD_ALARMS_MONITORED_FLAGS_SIZE	
#define MT_73_STD_TBLS_MONITORED_FLAGS_ADDR				MT_73_MFG_ALARMS_MONITORED_FLAGS_ADDR		+ MT_73_MFG_ALARMS_MONITORED_FLAGS_SIZE	
#define MT_73_MFG_TBLS_MONITORED_FLAGS_ADDR				MT_73_STD_TBLS_MONITORED_FLAGS_ADDR			+ MT_73_STD_TBLS_MONITORED_FLAGS_SIZE		
#define MT_73_STD_PROC_MONITORED_FLAGS_ADDR				MT_73_MFG_TBLS_MONITORED_FLAGS_ADDR			+ MT_73_MFG_TBLS_MONITORED_FLAGS_SIZE		
#define MT_73_MFG_PROC_MONITORED_FLAGS_ADDR				MT_73_STD_PROC_MONITORED_FLAGS_ADDR			+ MT_73_STD_PROC_MONITORED_FLAGS_SIZE		

/*===========================================================================*\
 * ST_64: Load Profile Control Table
\*===========================================================================*/
#define ST_64_LOAD_PROFILE_DATA_ADDR					ST_64_START_ADDR
#define ST_64_END_ADDR									0x7FFE2

#define LP_EXPECTED_BLOCK_END_TIME_ADDR					LP_BACKUP_START_ADDR																		 		 
#define LP_INTERVAL_TIME_ADDR							LP_EXPECTED_BLOCK_END_TIME_ADDR				+ LP_EXPECTED_BLOCK_END_TIME_SIZE	 		 
#define LP_LAST_INTERVAL_USED_ADDR						LP_INTERVAL_TIME_ADDR						+ LP_INTERVAL_TIME_SIZE			 		 
#define LP_LAST_BLOCK_USED_ADDR							LP_LAST_INTERVAL_USED_ADDR					+ LP_LAST_INTERVAL_USED_SIZE		 		 
#define LP_BACKUP_PULSE_RECORDER_ADDR					LP_LAST_BLOCK_USED_ADDR						+ LP_LAST_BLOCK_USED_SIZE
#define LP_LAST_INTERVAL_EXTENDED_STATUS_ADDR			LP_BACKUP_PULSE_RECORDER_ADDR				+ LP_BACKUP_PULSE_RECORDER_SIZE 		 
												
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
extern bool EEPROM_writer(uint32_t address, void* data_pointer, unsigned int data_length);
extern bool EEPROM_reader(uint32_t address, void* data_pointer, unsigned int data_length);
extern void EEPROM_erase_all(void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-08-13
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
#endif /* EEPROM_H_ */