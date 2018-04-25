/**
 ******************************************************************************
 * \file lp.h
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

#ifndef LP_H_
#define LP_H_

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "stdint.h"
#include "compiler.h"
#include "regs.h"
   
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/
#define LP_DEFAULT_INTERVAL_PERIOD				((1000 * 300) / portTICK_RATE_MS)

#define LP_MAX_PULSE_REGISTER_VALUE				(uint16_t) 0xFFFF
#define LP_SAMPLING_MS_PERIOD					1000 //milliseconds
#define LP_SAMPLES_PER_HOUR						( (MILISECONDS_PER_SECOND / LP_SAMPLING_MS_PERIOD) * SECONDS_PER_HOUR )

/*===========================================================================*\
 * BIT MASK FOR USE IN ST_60 & ST_61 
\*===========================================================================*/
/* LP_FLAGS_BFLD BIT MASK */
#define LP_SET1_INHIBIT_OVF_FLAG_bm		0b0000000000000001
#define LP_SET1_INHIBIT_OVF_FLAG_bp		0
#define LP_SET2_INHIBIT_OVF_FLAG_bm		0b0000000000000010
#define LP_SET2_INHIBIT_OVF_FLAG_bp		1
#define LP_SET3_INHIBIT_OVF_FLAG_bm		0b0000000000000100
#define LP_SET3_INHIBIT_OVF_FLAG_bp		2
#define LP_SET4_INHIBIT_OVF_FLAG_bm		0b0000000000001000
#define LP_SET4_INHIBIT_OVF_FLAG_bp		3
#define BLK_END_READ_FLAG_bm 			0b0000000000010000
#define BLK_END_READ_FLAG_bp			4
#define BLK_END_PULSE_FLAG_bm			0b0000000000100000
#define BLK_END_PULSE_FLAG_bp			5
#define SCALAR_DIVISOR_FLAG_SET1_bm		0b0000000001000000
#define SCALAR_DIVISOR_FLAG_SET1_bp		6
#define SCALAR_DIVISOR_FLAG_SET2_bm		0b0000000010000000
#define SCALAR_DIVISOR_FLAG_SET2_bp		7
#define SCALAR_DIVISOR_FLAG_SET3_bm		0b0000000100000000
#define SCALAR_DIVISOR_FLAG_SET3_bp		8
#define SCALAR_DIVISOR_FLAG_SET4_bm		0b0000001000000000
#define SCALAR_DIVISOR_FLAG_SET4_bp		9
#define EXTENDED_INT_STATUS_FLAG_bm		0b0000010000000000
#define EXTENDED_INT_STATUS_FLAG_bp		10
#define SIMPLE_INT_STATUS_FLAG_bm		0b0000100000000000
#define SIMPLE_INT_STATUS_FLAG_bp		11
#define BLK_END_RP_INDICATOR_FLAG_bm	0b0001000000000000
#define BLK_END_RP_INDICATOR_FLAG_bp	12

/* LP_FMATS_BFLD BIT MASK */
#define INV_UINT8_FLAG_bm				0b00000001
#define INV_UINT8_FLAG_bp				0
#define INV_UINT16_FLAG_bm				0b00000010
#define INV_UINT16_FLAG_bp				1
#define INV_UINT32_FLAG_bm				0b00000100
#define INV_UINT32_FLAG_bp				2
#define INV_INT8_FLAG_bm				0b00001000
#define INV_INT8_FLAG_bp				3
#define INV_INT16_FLAG_bm				0b00010000
#define INV_INT16_FLAG_bp				4
#define INV_INT32_FLAG_bm				0b00100000
#define INV_INT32_FLAG_bp				5
#define INV_NI_FMAT1_FLAG_bm			0b01000000
#define INV_NI_FMAT1_FLAG_bp			6
#define INV_NI_FMAT2_FLAG_bm			0b10000000
#define INV_NI_FMAT2_FLAG_bp			7

/*===========================================================================*\
 * BIT MASK FOR USE IN ST_62
\*===========================================================================*/
#define END_RDG_FLAG_bm					0b00000001
#define END_RDG_FLAG_bp					0

/*===========================================================================*\
 * BIT MASK FOR USE IN ST_63 
\*===========================================================================*/
/* LP_FMATS_BFLD BIT MASK */
#define BLOCK_ORDER_bm					0b00000001
#define BLOCK_ORDER_bp					0
#define OVERFLOW_FLAG_bm				0b00000010
#define OVERFLOW_FLAG_bp				1
#define LIST_TYPE_bm					0b00000100
#define LIST_TYPE_bp					2
#define BLOCK_INHIBIT_OVERFLOW_FLAG_bm	0b00001000
#define BLOCK_INHIBIT_OVERFLOW_FLAG_bp	3
#define INTERVAL_ORDER_bm				0b00010000
#define INTERVAL_ORDER_bp				4
#define ACTIVE_MODE_FLAG_bm				0b00100000
#define ACTIVE_MODE_FLAG_bp				5
#define TEST_MODE_bm					0b01000000
#define TEST_MODE_bp					6

/*===========================================================================*\
 * BIT MASK FOR USE IN ST_64 
\*===========================================================================*/
/* EXTENDED_STATUS_BFLD BIT MASK */
#define CHANNEL_STATUS_ODD_bm			0b00001111
#define CHANNEL_STATUS_ODD_bp			0
#define CHANNEL_STATUS_EVEN_bm			0b11110000
#define CHANNEL_STATUS_EVEN_bp			4
#define COMMON_DST_FLAG_bm				0b00010000
#define COMMON_DST_FLAG_bp				4
#define COMMON_POWER_FAIL_FLAG_bm		0b00100000
#define COMMON_POWER_FAIL_FLAG_bp		5
#define COMMON_CLOCK_SET_FWD_FLAG_bm	0b01000000
#define COMMON_CLOCK_SET_FWD_FLAG_bp	6
#define COMMON_CLOCK_SET_BKWD_FLAG_bm	0b10000000
#define COMMON_CLOCK_SET_BKWD_FLAG_bp	7

/*===========================================================================*\
 * MM100 ST_60. Load Profile Dimension Limits Table (max values)
\*===========================================================================*/
#define MAX_LP_MEMORY_LEN				(uint32_t)	ST_64_MAX_SIZE
#define PERM_LP_FLAGS					(uint16_t)	( BLK_END_READ_FLAG_bm | SCALAR_DIVISOR_FLAG_SET1_bm | EXTENDED_INT_STATUS_FLAG_bm | SIMPLE_INT_STATUS_FLAG_bm )
#define PERM_LP_FMATS					(uint8_t)	( INV_INT16_FLAG_bm )
//#define MAX_NBR_BLKS_SET1				(uint16_t)	0 
#define MAX_NBR_BLK_INTS_SET1			(uint16_t)	48
#define MAX_NBR_CHNS_SET1				(uint8_t)	4
#define MAX_MAX_INT_TIME_SET1			(uint8_t)	60

/*===========================================================================*\
 * MM100 ST_61. Actual Load Profile Limiting Table (default values)
\*===========================================================================*/
//#define DEFAULT_LP_MEMORY_LEN			(uint32_t)	0
#define DEFAULT_LP_FLAGS				(uint16_t)	( BLK_END_READ_FLAG_bm | SCALAR_DIVISOR_FLAG_SET1_bm | EXTENDED_INT_STATUS_FLAG_bm | SIMPLE_INT_STATUS_FLAG_bm )
#define DEFAULT_LP_FMATS				(uint8_t)	( INV_INT16_FLAG_bm )
//#define DEFAULT_NBR_BLKS_SET1			(uint16_t)	0
#define DEFAULT_NBR_BLK_INTS_SET1		(uint16_t)	48
#define DEFAULT_NBR_CHNS_SET1			(uint8_t)	4
#define DEFAULT_MAX_INT_TIME_SET1		(uint8_t)	5

#define MAX_INT_TIME_SET1				(uint8_t)	60
#define MIN_INT_TIME_SET1				(uint8_t)	5

/*===========================================================================*\
 * MM100 ST_62.Load Profile Control Table  (default values)
\*===========================================================================*/
#define DEFAULT_DIVISOR_SET1			(uint16_t)	100
#define DEFAULT_SCALARS_SET1			(uint16_t)	5

/*===========================================================================*\
 * MM100 ST_63. Load Profile Status Table (default values)
\*===========================================================================*/
#define BLOCK_ORDER						0	// Blocks of Load Profile data are transported in descending order (N is newer than N+1).
#define OVERFLOW_FLAG					0	// Overflow has not occurred.
#define LIST_TYPE						0	// FIFO list as placed in Load Profile storage.
#define BLOCK_INHIBIT_OVERFLOW_FLAG		(DEFAULT_LP_FMATS & BLOCK_INHIBIT_OVERFLOW_FLAG_bm) >> BLOCK_INHIBIT_OVERFLOW_FLAG_bp	// The same value as ST_61.LP_SETn_INHIBIT_OVF_FLAG
#define INTERVAL_ORDER					0	// Intervals in each block of Load Profile are transported in ascending order (N is older than N+1).
#define ACTIVE_MODE_FLAG				1	// Data set is collecting data.
#define TEST_MODE_FLAG					0	// Data set is not collecting data.

#define DEFAULT_LP_SET_STATUS_FLAGS		(uint8_t)  ( (TEST_MODE_FLAG << 5) | (ACTIVE_MODE_FLAG << 5) | (INTERVAL_ORDER << 4) | (BLOCK_INHIBIT_OVERFLOW_FLAG << 3) | (LIST_TYPE << 2) | (OVERFLOW_FLAG << 1) | (BLOCK_ORDER << 0) ) 
#define DEFAULT_NBR_VALID_BLOCKS		(uint16_t) 0x00
#define	DEFAULT_LAST_BLOCK_ELEMENT		(uint16_t) 0x00	
#define DEFAULT_LAST_BLOCK_SEQ_NBR		(uint32_t) 0x00
#define DEFAULT_NBR_UNREAD_BLOCKS		(uint16_t) 0x00
#define DEFAULT_NBR_VALID_INT			(uint16_t) 0x00

/*===========================================================================*\
 * C12.19 Decade 6 Tables Size
\*===========================================================================*/

/* ST_60 Elements size */
#define ST_60_SIZE										13

/* ST_61 Elements size */
#define ST_61_SIZE										13

#define ST_61_LP_MEMORY_LEN_SIZE						4
#define ST_61_LP_FLAGS_SIZE								2
#define ST_61_LP_FMATS_SIZE								1
#define ST_61_NBR_BLKS_SET1_SIZE						2
#define ST_61_NBR_BLK_INTS_SET1_SIZE					2
#define ST_61_NBR_CHNS_SET1_SIZE						1
#define ST_61_MAX_INT_TIME_SET1_SIZE					1
#define ST_61_MAX_SIZE								   (ST_61_LP_MEMORY_LEN_SIZE		+ \
														ST_61_LP_FLAGS_SIZE				+ \
														ST_61_LP_FMATS_SIZE				+ \
														ST_61_NBR_BLKS_SET1_SIZE		+ \
														ST_61_NBR_BLK_INTS_SET1_SIZE	+ \
														ST_61_NBR_CHNS_SET1_SIZE		+ \
														ST_61_MAX_INT_TIME_SET1_SIZE	)

/* ST_62 Elements size */
#define ST_62_CHNL_FLAG_SET1_SIZE						MAX_NBR_CHNS_SET1
#define ST_62_LP_SOURCE_SELECT_SET1_SIZE				MAX_NBR_CHNS_SET1
#define ST_62_END_BLK_RDG_SOURCE_SELECT_SET1_SIZE		MAX_NBR_CHNS_SET1
#define ST_62_INT_FMT_CDE1_SIZE							1
#define ST_62_SCALARS_SET1_SIZE							(2 * MAX_NBR_CHNS_SET1)
#define ST_62_DIVISOR_SET1_SIZE							(2 * MAX_NBR_CHNS_SET1)
#define ST_62_MAX_SIZE								   (ST_62_CHNL_FLAG_SET1_SIZE					+ \					
														ST_62_LP_SOURCE_SELECT_SET1_SIZE			+ \
														ST_62_END_BLK_RDG_SOURCE_SELECT_SET1_SIZE	+ \
														ST_62_INT_FMT_CDE1_SIZE						+ \
														ST_62_SCALARS_SET1_SIZE						+ \
														ST_62_DIVISOR_SET1_SIZE						)

/* ST_63 Elements size */
#define ST_63_SIZE										13

#define ST_63_SET_STATUS_FLAGS_SIZE						1
#define ST_63_NBR_VALID_BLOCKS_SIZE						2
#define ST_63_LAST_BLOCK_ELEMENT_SIZE					2
#define ST_63_LAST_BLOCK_SEQ_NBR_SIZE					4
#define ST_63_NBR_UNREAD_BLOCKS_SIZE					2
#define ST_63_NBR_VALID_INT_SIZE						2
#define ST_63_MAX_SIZE								   (ST_63_SET_STATUS_FLAGS_SIZE		+ \
														ST_63_NBR_VALID_BLOCKS_SIZE		+ \
														ST_63_LAST_BLOCK_ELEMENT_SIZE	+ \
														ST_63_LAST_BLOCK_SEQ_NBR_SIZE	+ \
														ST_63_NBR_UNREAD_BLOCKS_SIZE	+ \
														ST_63_NBR_VALID_INT_SIZE		)

/* ST_64 Elements size */
#define ST_64_MAX_SIZE									262145

#define LP_EXPECTED_BLOCK_END_TIME_SIZE					TM_FMAT_SIZE
#define LP_INTERVAL_TIME_SIZE							TM_FMAT_SIZE
#define LP_LAST_INTERVAL_USED_SIZE						UINT16
#define LP_LAST_BLOCK_USED_SIZE							2
#define LP_BACKUP_PULSE_RECORDER_SIZE					(4 * MAX_NBR_CHNS_SET1)
#define LP_LAST_INTERVAL_EXTENDED_STATUS_SIZE			(MAX_NBR_CHNS_SET1/2) + 1	

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

/*---------------------------------------------------------------------------*\
 * CHANNEL_STATUS_ENUM: The contents of the even and odd channel status nibbles.
 * 0: No exception status indicated.
 * 1: Numeric overflow condition detected.
 * 2: Partial interval due to common state. The cause is indicated in the common status flags.
 * 3: Long interval due to common state. The cause is indicated in the common status flags.
 * 4: Skipped interval due to common state. The cause is indicated in the common status flags. 
	  The INT_DATA shall be set to zero (0).
 * 5: The interval contains test mode data.
 * 6: The configuration changed during this interval.
 * 7: Load profile recording stopped. If the stoppage condition spans an entire interval 
      duration then the value of INT_DATA shall be set to zero (0) in all applicable intervals
	  of the affected block until
\*---------------------------------------------------------------------------*/
typedef enum {
	
	CHANNEL_STATUS_0 = 0,
	CHANNEL_STATUS_1,
	CHANNEL_STATUS_2,
	CHANNEL_STATUS_3,
	CHANNEL_STATUS_4,
	CHANNEL_STATUS_5,
	CHANNEL_STATUS_6,
	CHANNEL_STATUS_7
	
} CHANNEL_STATUS_TYPE;

/*===========================================================================*\
 * ST_60 & ST_61 structure												       
\*===========================================================================*/
typedef struct {
	
	uint32_t lp_memory_len;
	uint16_t lp_flags;
	uint8_t lp_fmats;
	uint16_t nbr_blks_set1;
	uint16_t nbr_blk_ints_set1;
	uint8_t nbr_chns_set1;
	uint8_t max_int_time_set1;
	
} ST_60_TYPE;

/*===========================================================================*\
 * ST_62 structure												       
\*===========================================================================*/
typedef enum {
	
	UINT8		= 1,
	UINT16		   ,
	UINT32		= 4,
	INT8		= 8,
	INT16		= 16,
	INT32		= 32,
	NI_FMAT1	= 64,
	NI_FMAT2	= 128
	
} INT_FMT_ENUM_TYPE;

typedef struct {
	
	uint8_t chnl_flag;
	LP_SOURCE_ID_TYPE lp_source_select;
	SUMMATION_ID_TYPE end_blk_rdg_source_select;
	
} LP_SOURCE_SEL_TYPE;

typedef struct {
	
	LP_SOURCE_SEL_TYPE lp_sel_set1[MAX_NBR_CHNS_SET1];
	INT_FMT_ENUM_TYPE int_fmt_cde1;
	uint16_t scalars_set1[MAX_NBR_CHNS_SET1];
	uint16_t divisor_set1[MAX_NBR_CHNS_SET1];
	
} ST_62_TYPE;

typedef struct {
	
	uint64_t lp_int_channelPulses[MAX_NBR_CHNS_SET1];
	
} LP_INTERVAL_PULSES_ARRAY_TYPE;

/*===========================================================================*\
 * Load Profile current data structures											       
\*===========================================================================*/
typedef struct {

	uint16_t pulse_register[MAX_NBR_CHNS_SET1];
	uint16_t blk_number;
	uint16_t int_number;
	uint32_t int_end_time;
	uint32_t blk_end_time;
	uint32_t int_elapsed_time;
	uint8_t extended_int_status[(MAX_NBR_CHNS_SET1/2) + 1];
	uint8_t block_simple_flags[(MAX_NBR_BLK_INTS_SET1 + 7)/8];

} LP_INTERVAL_DATA;

typedef struct {
	
	bool is_active;
	bool eoi;													// End of Interval
	bool eob;													// End of Block

	LP_INTERVAL_DATA curr_data;
	LP_INTERVAL_DATA last_data;
	uint32_t pulse_recorder[MAX_NBR_CHNS_SET1];

} LP_STATUS;

/*===========================================================================*\
 * ST_63 structure												       
\*===========================================================================*/
typedef struct {
	
	uint8_t lp_set_status_flags;
	uint16_t nbr_valid_blocks;
	uint16_t last_block_element;
	uint32_t last_block_seq_nbr;
	uint16_t nbr_unread_blocks;
	uint16_t nbr_valid_int;
	
} ST_63_TYPE;

/*===========================================================================*\
 * ST_64 dynamic eeprom addresses
\*===========================================================================*/
typedef struct {
	
	uint32_t blk_end_time;
	uint32_t block_end_read;
	uint32_t simple_int_status;
	uint32_t extended_int_status;
	uint32_t int_data;
	uint32_t end_addr;
	
} ST_64_ADDR_TYPE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
extern ST_60_TYPE st_61;
extern ST_62_TYPE st_62;
extern ST_63_TYPE st_63;
extern ST_64_ADDR_TYPE st_64_addr;
extern LP_STATUS lp;

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/
extern void LP_APP_vRecorder_task(void);
extern void LP_APP_vInitialize(void);

extern bool LP_APP_bSt_60_execute_table_reading(uint8_t *table_buffer);
extern uint16_t LP_APP_usSt_60_get_size(void);

extern bool LP_APP_bLoad_st_61(void);
extern bool LP_APP_bSt_61_restore_default_values(void);
extern bool LP_APP_bSt_61_refresh_ram(void);
extern bool LP_APP_bSt_61_execute_table_reading(uint8_t *table_buffer);
extern bool LP_APP_bSt_61_execute_table_writing(uint8_t *table_buffer);
extern void LP_APP_vCopy_st_61_nvm(uint8_t* destination);
extern void LP_APP_vCopy_st_61_ram(uint8_t* destination);
extern void LP_APP_vSt_61_recover_nvm(void);
extern void LP_APP_vSt_61_recover_ram(void);
extern uint16_t LP_APP_usSt_61_get_size(void);

extern bool LP_APP_bLoad_st_62(void);
extern bool LP_APP_bSt_62_restore_default_values(void);
extern bool LP_APP_bSt_62_refresh_ram(void);
extern bool LP_APP_bSt_62_execute_table_reading(uint8_t *table_buffer);
extern bool LP_APP_bSt_62_execute_table_writing(uint8_t *table_buffer);
extern void LP_APP_vCopy_st_62_nvm(uint8_t* destination);
extern void LP_APP_vCopy_st_62_ram(uint8_t* destination);
extern void LP_APP_vSt_62_recover_nvm(void);
extern void LP_APP_vSt_62_recover_ram(void);
extern uint16_t LP_APP_usSt_62_get_size(void);

extern bool LP_APP_bLoad_st_63(void);
extern bool LP_APP_bSt_63_restore_default_values(void);
extern bool LP_APP_bSt_63_refresh_ram(void);
extern bool LP_APP_bSt_63_execute_table_reading(uint8_t *table_buffer);
extern uint16_t LP_APP_usSt_63_get_size(void);

extern uint32_t LP_APP_ulSt_64_get_size(void);

extern uint32_t LP_APP_ulLoad_nbr_blocks(void);
extern void LP_APP_vLoad_st_64_eeprom_addr(void);

extern void LP_APP_vStore_data_task(void);
extern bool LP_APP_bStart_load_profile(void);
extern void LP_APP_vStop_load_profile(void);
extern void LP_APP_vSynchronize_clock(uint32_t old_time, uint32_t new_time);
extern void LP_APP_vResume(void);
extern void LP_APP_vPower_fail_detection(void);

extern void LP_APP_vGet_data_block_fifo_table(uint16_t block_offset, uint16_t block_count, uint8_t* data_buffer);
extern void LP_APP_vGet_data_block_circ_table(uint16_t block_offset, uint16_t block_count, uint8_t* data_buffer);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Jacob Anaya Candia / 2015-08-13
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
 #endif /* LP_H_ */
