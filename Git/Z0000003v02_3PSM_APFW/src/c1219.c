/**
 ******************************************************************************
 * \file c1219_ptr.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Eos Tech S. de A. de C.V. All rights reserved.</b><br><br>
 * Eos Tech Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup File content title
 * @section DESCRIPTION DESCRIPTION:
 *
 * File content description.
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
 * - Revision 01 / Jacob Anaya Candia / 2016-12-20
 *   - Change 1:  PCR#: Description.
 *   - Change 2:  PCR#: Description.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include <asf.h>
#include "c1219.h"
#include "evnt.h"
#include "c1218.h"
#include "secr.h"
#include "rtc_driver.h"
#include "sm.h"
#include "system.h"
#include "rst.h"
#include "timer.h"
#include "flash.h"
#include "nic.h"
#include "op.h"
#include "crc.h"
#include "watchdog.h"
#include "gral.h"
#include "led.h"
#include "rx8900.h"
#include "stdlib.h"

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
 * Brief description for each block of constant definition
\*===========================================================================*/

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
C1219_TYPE c1219[NBR_C1219_INSTANCES];

/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
 */

static OTA_IMAGE_TYPE ota_image;
static uint16_t proc_id;
static uint8_t proc_seq_nbr;

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
static void C1219_full_read(C1219_TYPE *c1219_ptr);
static void C1219_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_full_write(C1219_TYPE *c1219_ptr);
static void C1219_pwrite_offset(C1219_TYPE *c1219_ptr);

/*===========================================================================*\
 * Standard Tables
\*===========================================================================*/
static void C1219_st_0_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_0_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_1_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_1_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_1_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_1_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_2_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_2_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_3_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_3_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_6_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_6_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_6_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_6_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_7_full_write(C1219_TYPE *c1219_ptr);

static void C1219_st_8_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_8_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_10_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_10_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_11_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_11_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_11_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_11_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_12_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_12_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_13_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_13_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_13_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_13_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_15_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_15_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_16_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_16_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_20_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_20_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_21_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_21_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_21_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_21_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_22_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_22_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_22_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_22_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_23_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_23_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_23_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_23_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_24_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_24_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_24_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_24_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_25_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_25_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_25_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_25_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_26_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_26_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_26_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_26_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_27_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_27_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_27_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_27_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_28_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_28_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_40_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_40_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_41_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_41_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_41_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_41_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_42_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_42_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_42_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_42_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_43_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_43_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_43_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_43_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_44_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_44_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_44_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_44_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_50_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_50_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_51_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_51_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_51_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_51_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_52_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_52_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_53_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_53_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_53_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_53_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_54_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_54_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_54_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_54_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_55_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_55_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_60_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_60_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_61_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_61_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_61_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_61_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_62_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_62_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_62_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_62_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_63_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_63_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_64_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_70_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_70_pread_offset(C1219_TYPE *c1219_ptr);
 
static void C1219_st_71_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_71_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_71_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_71_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_72_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_72_pread_offset(C1219_TYPE *c1219_ptr);
 
static void C1219_st_73_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_73_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_73_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_73_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_74_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_74_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_74_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_74_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_75_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_75_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_75_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_75_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_st_76_full_read(C1219_TYPE *c1219_ptr);
static void C1219_st_76_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_st_76_full_write(C1219_TYPE *c1219_ptr);
static void C1219_st_76_pwrite_offset(C1219_TYPE *c1219_ptr);

/*===========================================================================*\
 * Manufacturer Tables
\*===========================================================================*/
static void C1219_mt_1_full_read(C1219_TYPE *c1219_ptr);			// NET50 Specific Product Identification
static void C1219_mt_1_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_mt_1_full_write(C1219_TYPE *c1219_ptr);
static void C1219_mt_1_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_mt_3_full_read(C1219_TYPE *c1219_ptr);			// Manufacturer Configuration Table
static void C1219_mt_3_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_mt_3_full_write(C1219_TYPE *c1219_ptr);
static void C1219_mt_3_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_mt_4_full_read(C1219_TYPE *c1219_ptr);			// MCU Trouble Data
static void C1219_mt_4_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_mt_4_full_write(C1219_TYPE *c1219_ptr);
static void C1219_mt_4_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_mt_5_full_read(C1219_TYPE *c1219_ptr);			// Table Integrity Data
static void C1219_mt_5_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_mt_5_full_write(C1219_TYPE *c1219_ptr);
static void C1219_mt_5_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_mt_22_full_read(C1219_TYPE *c1219_ptr);			// Present Register Data (BCD Format)
static void C1219_mt_22_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_mt_23_full_read(C1219_TYPE *c1219_ptr);			// Present Register Status Table
static void C1219_mt_23_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_mt_24_full_read(C1219_TYPE *c1219_ptr);			// DSP Accumulated Quantities
static void C1219_mt_24_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_mt_25_full_read(C1219_TYPE *c1219_ptr);			// DSP Calibration Constants Register
static void C1219_mt_25_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_mt_53_full_read(C1219_TYPE *c1219_ptr);			// Time Offset Data
static void C1219_mt_53_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_mt_53_full_write(C1219_TYPE *c1219_ptr);
static void C1219_mt_53_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_mt_72_full_read(C1219_TYPE *c1219_ptr);			// Alarms Identification Table
static void C1219_mt_72_pread_offset(C1219_TYPE *c1219_ptr);

static void C1219_mt_73_full_read(C1219_TYPE *c1219_ptr);			// Alarms Control Table
static void C1219_mt_73_pread_offset(C1219_TYPE *c1219_ptr);
static void C1219_mt_73_full_write(C1219_TYPE *c1219_ptr);
static void C1219_mt_73_pwrite_offset(C1219_TYPE *c1219_ptr);

static void C1219_mt_202_pwrite_offset(C1219_TYPE *c1219_ptr);		// Image Data Table

/*===========================================================================*\
 * Standard Procedures
\*===========================================================================*/
static void C1219_sp_0_execute(C1219_TYPE *c1219_ptr);				// Cold Start
static void C1219_sp_1_execute(C1219_TYPE *c1219_ptr);				// Warm Start
static void C1219_sp_3_execute(C1219_TYPE *c1219_ptr);				// Clear Data
static void C1219_sp_5_execute(C1219_TYPE *c1219_ptr);				// Update Last Read Entry
static void C1219_sp_6_execute(C1219_TYPE *c1219_ptr);				// Change End Device Mode
static void C1219_sp_7_execute(C1219_TYPE *c1219_ptr);				// Clear Standard Status Flags
static void C1219_sp_8_execute(C1219_TYPE *c1219_ptr);				// Clear Manufacturer Status Flags
static void C1219_sp_9_execute(C1219_TYPE *c1219_ptr);				// Remote Reset
static void C1219_sp_10_execute(C1219_TYPE *c1219_ptr);			// Set Date and/or Time
static void C1219_sp_11_execute(C1219_TYPE *c1219_ptr);			// Diagnostic Procedure

/*===========================================================================*\
 * Manufacturer Procedures
\*===========================================================================*/
static void C1219_mp_6_execute(C1219_TYPE *c1219_ptr);				// Change End Device Test Mode Period
static void C1219_mp_9_execute(C1219_TYPE *c1219_ptr);				// Energy Reset
static void C1219_mp_100_execute(C1219_TYPE *c1219_ptr);			// Change End Device to Boot Mode
static void C1219_mp_101_execute(C1219_TYPE *c1219_ptr);			// Change Constant Kt
static void C1219_mp_102_execute(C1219_TYPE *c1219_ptr);			// Change Energy Pulse Mode
static void C1219_mp_103_execute(C1219_TYPE *c1219_ptr);			// Change Voltage Level Threshold
static void C1219_mp_104_execute(C1219_TYPE *c1219_ptr);			// Change Time Level Threshold for Voltage Monitor
static void C1219_mp_105_execute(C1219_TYPE *c1219_ptr);			// Configuration Mode
static void C1219_mp_110_execute(C1219_TYPE *c1219_ptr);			// Change Calibration Constant
static void C1219_mp_111_execute(C1219_TYPE *c1219_ptr);			// Back up Calibration Constants
static void C1219_mp_112_execute(C1219_TYPE *c1219_ptr);			// Restore Calibration Constants
static void C1219_mp_113_execute(C1219_TYPE *c1219_ptr);			// Load Calibration Constants Default Values
static void C1219_mp_120_execute(C1219_TYPE *c1219_ptr);			// Back up Configuration Table
static void C1219_mp_121_execute(C1219_TYPE *c1219_ptr);			// Restore Configuration Table
static void C1219_mp_122_execute(C1219_TYPE *c1219_ptr);			// Load Configuration Table Default Values
static void C1219_mp_123_execute(C1219_TYPE *c1219_ptr);			// Back up All Configuration Tables
static void C1219_mp_124_execute(C1219_TYPE *c1219_ptr);			// Restore All Configuration Tables
static void C1219_mp_125_execute(C1219_TYPE *c1219_ptr);			// Load All Configuration Tables Default Values
static void C1219_mp_126_execute(C1219_TYPE *c1219_ptr);			// Erase External Memory Data
static void C1219_mp_130_execute(C1219_TYPE *c1219_ptr);			// Test I2C EEPROM Memories
static void C1219_mp_150_execute(C1219_TYPE *c1219_ptr);			// Flash Image CRC Calculation
static void C1219_mp_200_execute(C1219_TYPE *c1219_ptr);			// Image Initiation Process
static void C1219_mp_201_execute(C1219_TYPE *c1219_ptr);			// Image Activation Process

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
*/

/**
 ******************************************************************************
 * \fn void C1219_full_read_service(C1218_DLL_TYPE *c1218_dll_ptr, C1218_APL_TYPE c1218_apl)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_full_read_service(C1218_DLL_TYPE *c1218_dll_ptr, C1218_APL_TYPE c1218_apl) {

	static uint8_t instance;

	for ( instance = 0; instance < NBR_C1219_INSTANCES; instance++ ) {
		
		if (c1219[instance].busy_instance == false) break; 
	}
	
	if ( c1219[instance].busy_instance == false ) {
	
		c1219[instance].busy_instance = true;
		
		c1219[instance].c1218_dll = c1218_dll_ptr;
		c1219[instance].c1218_apl = c1218_apl;
		
		/* Register Event */
		EVNT_APP_vRegister_event(END_DEVICE_ACCESSED_FOR_READ, 0, true, c1218_apl.user_id, rtc.utc_timestamp);
		
		if ( c1219[instance].c1218_dll->uart == OP_UART ) {
			EVNT_APP_vRegister_event(END_DEVICE_ACCESSED_FOR_READ_OP, 0, false, c1218_apl.user_id, rtc.utc_timestamp);
		}
		else if ( c1219[instance].c1218_dll->uart == NIC_UART ) {
			EVNT_APP_vRegister_event(END_DEVICE_ACCESSED_FOR_READ_NIC, 0, false, c1218_apl.user_id, rtc.utc_timestamp);
		}
		
		c1219[instance].table_id = 0;
		c1219[instance].table_id |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 1] << 8);
		c1219[instance].table_id |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 2] << 0);
		
		SECURITY_USER_INFO_TYPE user_info = {
			.tbl_proc_id			= (c1219[instance].table_id & TBL_PROC_NBR_bm) >> TBL_PROC_NBR_bp,
			.mfg_flag				= (bool)((c1219[instance].table_id & STD_VS_MFG_FLAG_bm) >> STD_VS_MFG_FLAG_bp),
			.access_permissions		= c1218_apl.access_permissions
		};
		memcpy(&user_info.password, &c1218_apl.password, 20);
	
		if ( SECR_APP_bCheck_access_to_read_table(user_info) ) {
			
			switch ( c1219[instance].table_id ) {	
				
				case ST_0_ID:
					C1219_st_0_full_read(&c1219[instance]);
					break;
				
				case ST_1_ID:
					C1219_st_1_full_read(&c1219[instance]);
					break;
					
				case ST_2_ID:
					C1219_st_2_full_read(&c1219[instance]);
					break;
					
				case ST_3_ID:
					C1219_st_3_full_read(&c1219[instance]);
					break;
				
				case ST_6_ID:
					C1219_st_6_full_read(&c1219[instance]);
					break;
				
				case ST_8_ID:
					C1219_st_8_full_read(&c1219[instance]);
					break;
				
				case ST_10_ID:
					C1219_st_10_full_read(&c1219[instance]);
					break;
					
				case ST_11_ID:
					C1219_st_11_full_read(&c1219[instance]);
					break;
					
				case ST_12_ID:
					C1219_st_12_full_read(&c1219[instance]);
					break;
					
				case ST_13_ID:
					C1219_st_13_full_read(&c1219[instance]);
					break;
					
				case ST_15_ID:
					C1219_st_15_full_read(&c1219[instance]);
					break;
					
				case ST_16_ID:
					C1219_st_16_full_read(&c1219[instance]);
					break;
				
				case ST_20_ID:
					C1219_st_20_full_read(&c1219[instance]);
					break;
					
				case ST_21_ID:
					C1219_st_21_full_read(&c1219[instance]);
					break;
					
				case ST_22_ID:
					C1219_st_22_full_read(&c1219[instance]);
					break;
					
				case ST_23_ID:
					C1219_st_23_full_read(&c1219[instance]);
					break;
					
				case ST_24_ID:
					C1219_st_24_full_read(&c1219[instance]);
					break;
					
				case ST_25_ID:
					C1219_st_25_full_read(&c1219[instance]);
					break;
					
				case ST_26_ID:
					C1219_st_26_full_read(&c1219[instance]);
					break;
					
				case ST_27_ID:
					C1219_st_27_full_read(&c1219[instance]);
					break;
					
				case ST_28_ID:
					C1219_st_28_full_read(&c1219[instance]);
					break;
					
				case ST_40_ID:
					C1219_st_40_full_read(&c1219[instance]);
					break;
					
				case ST_41_ID:
					C1219_st_41_full_read(&c1219[instance]);
					break;
					
				case ST_42_ID:
					C1219_st_42_full_read(&c1219[instance]);
					break;
					
				case ST_43_ID:
					C1219_st_43_full_read(&c1219[instance]);
					break;
					
				case ST_44_ID:
					C1219_st_44_full_read(&c1219[instance]);
					break;

				case ST_50_ID:
					C1219_st_50_full_read(&c1219[instance]);
					break;

				case ST_51_ID:
					C1219_st_51_full_read(&c1219[instance]);
					break;

				case ST_52_ID:
					C1219_st_52_full_read(&c1219[instance]);
					break;
					
				case ST_53_ID:
					C1219_st_53_full_read(&c1219[instance]);
					break;
					
				case ST_54_ID:
					C1219_st_54_full_read(&c1219[instance]);
					break;
					
				case ST_55_ID:
					C1219_st_55_full_read(&c1219[instance]);
					break;
					
				case ST_60_ID:
					C1219_st_60_full_read(&c1219[instance]);
					break;
					
				case ST_61_ID:
					C1219_st_61_full_read(&c1219[instance]);
					break;
					
				case ST_62_ID:
					C1219_st_62_full_read(&c1219[instance]);
					break;
					
				case ST_63_ID:
					C1219_st_63_full_read(&c1219[instance]);
					break;
					
				case ST_70_ID:
					C1219_st_70_full_read(&c1219[instance]);
					break;
					
				case ST_71_ID:
					C1219_st_71_full_read(&c1219[instance]);
					break;
					
				case ST_72_ID:
					C1219_st_72_full_read(&c1219[instance]);
					break;
					
				case ST_73_ID:
					C1219_st_73_full_read(&c1219[instance]);
					break;
					
				case ST_74_ID:
					C1219_st_74_full_read(&c1219[instance]);
					break;
				
				case ST_75_ID:
					C1219_st_75_full_read(&c1219[instance]);
					break;
					
				case ST_76_ID:
					C1219_st_76_full_read(&c1219[instance]);
					break;
					
				case MT_1_ID:
					C1219_mt_1_full_read(&c1219[instance]);
					break;
					
				case MT_3_ID:
					C1219_mt_3_full_read(&c1219[instance]);
					break;

				case MT_4_ID:
					C1219_mt_4_full_read(&c1219[instance]);
					break;
					
				case MT_5_ID:
					C1219_mt_5_full_read(&c1219[instance]);
					break;
				
				case MT_22_ID:
					C1219_mt_22_full_read(&c1219[instance]);
					break;

				case MT_23_ID:
					C1219_mt_23_full_read(&c1219[instance]);
					break;
					
				case MT_24_ID:
					C1219_mt_24_full_read(&c1219[instance]);
					break;
								
				case MT_25_ID:
					C1219_mt_25_full_read(&c1219[instance]);
					break;
				
				case MT_53_ID:
					C1219_mt_53_full_read(&c1219[instance]);
					break;

				case MT_72_ID:
					C1219_mt_72_full_read(&c1219[instance]);
					break;
				
				case MT_73_ID:
					C1219_mt_73_full_read(&c1219[instance]);
					break;

				default:
					C1218_send_iar(c1219[instance].c1218_dll);
					break;
			}	
		}
		else {
			C1218_send_isc(c1219[instance].c1218_dll);
		}

		c1219[instance].busy_instance = false;
	}
}

/**
 ******************************************************************************
 * \fn void C1219_pread_offset_service(C1218_DLL_TYPE c1218_dll_ptr, C1218_APL_TYPE c1218_apl)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_pread_offset_service(C1218_DLL_TYPE *c1218_dll_ptr, C1218_APL_TYPE c1218_apl) {

	static uint8_t instance;

	for ( instance = 0; instance < NBR_C1219_INSTANCES; instance++ ) {
		
		if (c1219[instance].busy_instance == false) break;
	}
	
	if ( c1219[instance].busy_instance == false ) {

		c1219[instance].busy_instance = true;

		c1219[instance].c1218_dll = c1218_dll_ptr;
		c1219[instance].c1218_apl = c1218_apl;
		
		/* Register Event */
		EVNT_APP_vRegister_event(END_DEVICE_ACCESSED_FOR_READ, 0, true, c1218_apl.user_id, rtc.utc_timestamp);
		
		if ( c1219[instance].c1218_dll->uart == OP_UART ) {
			EVNT_APP_vRegister_event(END_DEVICE_ACCESSED_FOR_READ_OP, 0, false, c1218_apl.user_id, rtc.utc_timestamp);
		}
		else if ( c1219[instance].c1218_dll->uart == NIC_UART ) {
			EVNT_APP_vRegister_event(END_DEVICE_ACCESSED_FOR_READ_NIC, 0, false, c1218_apl.user_id, rtc.utc_timestamp);
		}
		
		c1219[instance].table_id = 0;
		c1219[instance].table_id |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 1] << 8);
		c1219[instance].table_id |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 2] << 0);
		
		c1219[instance].offset = 0;
		c1219[instance].offset |= (uint32_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 3] << 16);
		c1219[instance].offset |= (uint32_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 4] <<  8);
		c1219[instance].offset |= (uint32_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 5] <<  0);
		
		c1219[instance].count = 0;
		c1219[instance].count |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 6] << 8);
		c1219[instance].count |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 7] << 0);
		
		SECURITY_USER_INFO_TYPE user_info = {
			.tbl_proc_id			= (c1219[instance].table_id & TBL_PROC_NBR_bm) >> TBL_PROC_NBR_bp,
			.mfg_flag				= (bool)((c1219[instance].table_id & STD_VS_MFG_FLAG_bm) >> STD_VS_MFG_FLAG_bp),
			.access_permissions		= c1218_apl.access_permissions
		};
		memcpy(&user_info.password, &c1218_apl.password, 20);
		
		if ( SECR_APP_bCheck_access_to_read_table(user_info) ) {
			
			switch ( c1219[instance].table_id ) {
				
				case ST_0_ID:
					C1219_st_0_pread_offset(&c1219[instance]);
					break;
					
				case ST_1_ID:
					C1219_st_1_pread_offset(&c1219[instance]);
					break;
				
				case ST_2_ID:
					C1219_st_2_pread_offset(&c1219[instance]);
					break;
					
				case ST_3_ID:
					C1219_st_3_pread_offset(&c1219[instance]);
					break;
				
				case ST_6_ID:
					C1219_st_6_pread_offset(&c1219[instance]);
					break;
					
				case ST_8_ID:
					C1219_st_8_pread_offset(&c1219[instance]);
					break;
				
				case ST_10_ID:
					C1219_st_10_pread_offset(&c1219[instance]);
					break;
					
				case ST_11_ID:
					C1219_st_11_pread_offset(&c1219[instance]);
					break;
					
				case ST_12_ID:
					C1219_st_12_pread_offset(&c1219[instance]);
					break;
					
				case ST_13_ID:
					C1219_st_13_pread_offset(&c1219[instance]);
					break;
					
				case ST_15_ID:
					C1219_st_15_pread_offset(&c1219[instance]);
					break;
					
				case ST_16_ID:
					C1219_st_16_pread_offset(&c1219[instance]);
					break;
				
				case ST_20_ID:
					C1219_st_20_pread_offset(&c1219[instance]);
					break;
				
				case ST_21_ID:
					C1219_st_21_pread_offset(&c1219[instance]);
					break;
					
				case ST_22_ID:
					C1219_st_22_pread_offset(&c1219[instance]);
					break;
				
				case ST_23_ID:
					C1219_st_23_pread_offset(&c1219[instance]);
					break;
					
				case ST_24_ID:
					C1219_st_24_pread_offset(&c1219[instance]);
					break;
				
				case ST_25_ID:
					C1219_st_25_pread_offset(&c1219[instance]);
					break;
					
				case ST_26_ID:
					C1219_st_26_pread_offset(&c1219[instance]);
					break;
					
				case ST_27_ID:
					C1219_st_27_pread_offset(&c1219[instance]);
					break;
					
				case ST_28_ID:
					C1219_st_28_pread_offset(&c1219[instance]);
					break;
				
				case ST_40_ID:
					C1219_st_40_pread_offset(&c1219[instance]);
					break;
					
				case ST_41_ID:
					C1219_st_41_pread_offset(&c1219[instance]);
					break;
				
				case ST_42_ID:
					C1219_st_42_pread_offset(&c1219[instance]);
					break;
					
				case ST_43_ID:
					C1219_st_43_pread_offset(&c1219[instance]);
					break;
					
				case ST_44_ID:
					C1219_st_44_pread_offset(&c1219[instance]);
					break;

				case ST_50_ID:
					C1219_st_50_pread_offset(&c1219[instance]);
					break;

				case ST_51_ID:
					C1219_st_51_pread_offset(&c1219[instance]);
					break;

				case ST_52_ID:
					C1219_st_52_pread_offset(&c1219[instance]);
					break;
					
				case ST_53_ID:
					C1219_st_53_pread_offset(&c1219[instance]);
					break;
					
				case ST_54_ID:
					C1219_st_54_pread_offset(&c1219[instance]);
					break;
					
				case ST_55_ID:
					C1219_st_55_pread_offset(&c1219[instance]);
					break;
					
				case ST_60_ID:
					C1219_st_60_pread_offset(&c1219[instance]);
					break;
					
				case ST_61_ID:
					C1219_st_61_pread_offset(&c1219[instance]);
					break;
				
				case ST_62_ID:
					C1219_st_62_pread_offset(&c1219[instance]);
					break;
					
				case ST_63_ID:
					C1219_st_63_pread_offset(&c1219[instance]);
					break;
					
				case ST_64_ID:
					C1219_st_64_pread_offset(&c1219[instance]);
					break;
					
				case ST_70_ID:
					C1219_st_70_pread_offset(&c1219[instance]);
					break;
					
				case ST_71_ID:
					C1219_st_71_pread_offset(&c1219[instance]);
					break;
					
				case ST_72_ID:
					C1219_st_72_pread_offset(&c1219[instance]);
					break;
					
				case ST_73_ID:
					C1219_st_73_pread_offset(&c1219[instance]);
					break;
					
				case ST_74_ID:
					C1219_st_74_pread_offset(&c1219[instance]);
					break;
					
				case ST_75_ID:
					C1219_st_75_pread_offset(&c1219[instance]);
					break;
					
				case ST_76_ID:
					C1219_st_76_pread_offset(&c1219[instance]);
					break;
					
				case MT_1_ID:
					C1219_mt_1_pread_offset(&c1219[instance]);
					break;
					
				case MT_3_ID:
					C1219_mt_3_pread_offset(&c1219[instance]);
					break;
					
				case MT_4_ID:
					C1219_mt_4_pread_offset(&c1219[instance]);
					break;
					
				case MT_5_ID:
					C1219_mt_5_pread_offset(&c1219[instance]);
					break;
				
				case MT_22_ID:
					C1219_mt_22_pread_offset(&c1219[instance]);
					break;

				case MT_23_ID:
					C1219_mt_23_pread_offset(&c1219[instance]);
					break;
				
				case MT_24_ID:
					C1219_mt_24_pread_offset(&c1219[instance]);
					break;
				
				case MT_25_ID:
					C1219_mt_25_pread_offset(&c1219[instance]);
					break;

				case MT_53_ID:
					C1219_mt_53_pread_offset(&c1219[instance]);
					break;

				case MT_72_ID:
					C1219_mt_72_pread_offset(&c1219[instance]);
					break;

				case MT_73_ID:
					C1219_mt_73_pread_offset(&c1219[instance]);
					break;

				default:
					C1218_send_iar(c1219[instance].c1218_dll);
					break;
			}
		}
		else {
			C1218_send_isc(c1219[instance].c1218_dll);
		}

		c1219[instance].busy_instance = false;
	}
}

/**
 ******************************************************************************
 * \fn void C1219_full_write_service(C1218_DLL_TYPE *c1218_dll_ptr, C1218_APL_TYPE c1218_apl)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_full_write_service(C1218_DLL_TYPE *c1218_dll_ptr, C1218_APL_TYPE c1218_apl) {	

	static uint8_t instance;
	static uint8_t cksum_rx;
	static uint8_t cksum_calc;

	for ( instance = 0; instance < NBR_C1219_INSTANCES; instance++ ) {
		
		if (c1219[instance].busy_instance == false) break;
	}
	
	if ( c1219[instance].busy_instance == false ) {
		
		c1219[instance].busy_instance = true;

		c1219[instance].c1218_dll = c1218_dll_ptr;
		c1219[instance].c1218_apl = c1218_apl;
		
		/* Register Event */
		EVNT_APP_vRegister_event(END_DEVICE_ACCESSED_FOR_WRITE, 0, true, c1218_apl.user_id, rtc.utc_timestamp);
		
		if ( c1219[instance].c1218_dll->uart == OP_UART ) {
			EVNT_APP_vRegister_event(END_DEVICE_ACCESSED_FOR_WRITE_OP, 0, 0, c1218_apl.user_id, rtc.utc_timestamp);
		}
		else if ( c1219[instance].c1218_dll->uart == NIC_UART ) {
			EVNT_APP_vRegister_event(END_DEVICE_ACCESSED_FOR_WRITE_NIC, 0, 0, c1218_apl.user_id, rtc.utc_timestamp);
		}
		
		c1219[instance].table_id = 0;
		c1219[instance].table_id |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 1] << 8);
		c1219[instance].table_id |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 2] << 0);

		c1219[instance].count = 0;
		c1219[instance].count |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 3] << 8);
		c1219[instance].count |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 4] << 0);
				
		cksum_rx = c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + c1219[instance].count + 5];
		cksum_calc = CRC_2s_complement(&c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 5], c1219[instance].count);
		
		EVNT_APP_vRegister_event(TABLE_WRITTEN_TO, c1219[instance].table_id, true, c1218_apl.user_id, rtc.utc_timestamp);

		if (cksum_rx == cksum_calc) {
			
			SECURITY_USER_INFO_TYPE user_info = {
				.tbl_proc_id			= (c1219[instance].table_id & TBL_PROC_NBR_bm) >> TBL_PROC_NBR_bp,
				.mfg_flag				= (bool)((c1219[instance].table_id & STD_VS_MFG_FLAG_bm) >> STD_VS_MFG_FLAG_bp),
				.access_permissions		= c1218_apl.access_permissions
			};
			memcpy(&user_info.password, &c1218_apl.password, 20);
			
			if ( SECR_APP_bCheck_access_to_write_table(user_info) ) {
					
				switch ( c1219[instance].table_id ) {
										
					case ST_1_ID:
						C1219_st_1_full_write(&c1219[instance]);
						break;
					
					case ST_6_ID:
						C1219_st_6_full_write(&c1219[instance]);
						break;
							
					case ST_7_ID:
						C1219_st_7_full_write(&c1219[instance]);
						break;
						
					case ST_11_ID:
						C1219_st_11_full_write(&c1219[instance]);
						break;
						
					case ST_13_ID:
						C1219_st_13_full_write(&c1219[instance]);
						break;
						
					case ST_21_ID:
						C1219_st_21_full_write(&c1219[instance]);
						break;
						
					case ST_22_ID:
						C1219_st_22_full_write(&c1219[instance]);
						break;
						
					case ST_23_ID:
						C1219_st_23_full_write(&c1219[instance]);
						break;
						
					case ST_24_ID:
						C1219_st_24_full_write(&c1219[instance]);
						break;
						
					case ST_25_ID:
						C1219_st_25_full_write(&c1219[instance]);
						break;
						
					case ST_26_ID:
						C1219_st_26_full_write(&c1219[instance]);
						break;
						
					case ST_27_ID:
						C1219_st_27_full_write(&c1219[instance]);
						break;
						
					case ST_41_ID:
						C1219_st_41_full_write(&c1219[instance]);
						break;
						
					case ST_42_ID:
						C1219_st_42_full_write(&c1219[instance]);
						break;
						
					case ST_43_ID:
						C1219_st_43_full_write(&c1219[instance]);
						break;
						
					case ST_44_ID:
						C1219_st_44_full_write(&c1219[instance]);
						break;

					case ST_51_ID:
						C1219_st_51_full_write(&c1219[instance]);
						break;
						
					case ST_53_ID:
						C1219_st_53_full_write(&c1219[instance]);
						break;

					case ST_54_ID:
						C1219_st_54_full_write(&c1219[instance]);
						break;
						
					case ST_61_ID:
						C1219_st_61_full_write(&c1219[instance]);
						break;
					
					case ST_62_ID:
						C1219_st_62_full_write(&c1219[instance]);
						break;
						
					case ST_71_ID:
						C1219_st_71_full_write(&c1219[instance]);				
						break;
						
					case ST_73_ID:
						C1219_st_73_full_write(&c1219[instance]);
						break;
						
					case ST_74_ID:
						C1219_st_74_full_write(&c1219[instance]);
						break;
						
					case ST_75_ID:
						C1219_st_75_full_write(&c1219[instance]);
						break;
						
					case ST_76_ID:
						C1219_st_76_full_write(&c1219[instance]);
						break;
						
					case MT_1_ID:
						C1219_mt_1_full_write(&c1219[instance]);
						break;
						
					case MT_3_ID:
						C1219_mt_3_full_write(&c1219[instance]);
						break;
						
					case MT_4_ID:
						C1219_mt_4_full_write(&c1219[instance]);
						break;
						
					case MT_5_ID:
						C1219_mt_5_full_write(&c1219[instance]);
						break;
						
					case MT_53_ID:
						C1219_mt_53_full_write(&c1219[instance]);
						break;

					case MT_73_ID:
						C1219_mt_73_full_write(&c1219[instance]);
						break;
					
					default:
						C1218_send_iar(c1219[instance].c1218_dll);
						break;
				}
			}
			else {
				C1218_send_isc(c1219[instance].c1218_dll);
			}
		}
		else {
			C1218_send_err(c1219[instance].c1218_dll);
		}

		c1219[instance].busy_instance = false;
	}
}

/**
 ******************************************************************************
 * \fn void C1219_pwrite_offset_service(C1218_DLL_TYPE *c1218_dll_ptr, C1218_APL_TYPE c1218_apl)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_pwrite_offset_service(C1218_DLL_TYPE *c1218_dll_ptr, C1218_APL_TYPE c1218_apl) {
	
	static uint8_t instance;
	static uint8_t cksum_rx;
	static uint8_t cksum_calc;

	for ( instance = 0; instance < NBR_C1219_INSTANCES; instance++ ) {
		
		if (c1219[instance].busy_instance == false) break;
	}
	
	if ( c1219[instance].busy_instance == false ) {
		
		c1219[instance].busy_instance = true;

		c1219[instance].c1218_dll = c1218_dll_ptr;
		c1219[instance].c1218_apl = c1218_apl;
		
		/* Register Event */
		EVNT_APP_vRegister_event(END_DEVICE_ACCESSED_FOR_WRITE, 0, true, c1218_apl.user_id, rtc.utc_timestamp);
		
		if ( c1219[instance].c1218_dll->uart == OP_UART ) {
			EVNT_APP_vRegister_event(END_DEVICE_ACCESSED_FOR_WRITE_OP, 0, 0, c1218_apl.user_id, rtc.utc_timestamp);
		}
		else if ( c1219[instance].c1218_dll->uart == NIC_UART ) {
			EVNT_APP_vRegister_event(END_DEVICE_ACCESSED_FOR_WRITE_NIC, 0, 0, c1218_apl.user_id, rtc.utc_timestamp);
		}
		
		c1219[instance].table_id = 0;
		c1219[instance].table_id |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 1] << 8);
		c1219[instance].table_id |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 2] << 0);
		
		c1219[instance].offset = 0;
		c1219[instance].offset |= (uint32_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 3] << 16);
		c1219[instance].offset |= (uint32_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 4] <<  8);
		c1219[instance].offset |= (uint32_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 5] <<  0);
		
		c1219[instance].count = 0;
		c1219[instance].count |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 6] << 8);
		c1219[instance].count |= (uint16_t)(c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 7] << 0);
		
		cksum_rx = c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + c1219[instance].count + 8];
		cksum_calc = CRC_2s_complement(&c1219[instance].c1218_dll->rx_data[SERVICE_INDEX + 8], c1219[instance].count);
		
		EVNT_APP_vRegister_event(TABLE_WRITTEN_TO, c1219[instance].table_id, true, c1218_apl.user_id, rtc.utc_timestamp);
		
		if (cksum_rx == cksum_calc) {
			
			SECURITY_USER_INFO_TYPE user_info = {
				.tbl_proc_id			= (c1219[instance].table_id & TBL_PROC_NBR_bm) >> TBL_PROC_NBR_bp,
				.mfg_flag				= (bool)((c1219[instance].table_id & STD_VS_MFG_FLAG_bm) >> STD_VS_MFG_FLAG_bp),
				.access_permissions		= c1218_apl.access_permissions
			};
			memcpy(&user_info.password, &c1218_apl.password, 20);
			
			if ( SECR_APP_bCheck_access_to_write_table(user_info) ) {
				
				switch ( c1219[instance].table_id ) {
					
					case ST_1_ID:
						C1219_st_1_pwrite_offset(&c1219[instance]);
						break;
					
					case ST_6_ID:
						C1219_st_6_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_11_ID:
						C1219_st_11_pwrite_offset(&c1219[instance]);
						break;
					
					case ST_13_ID:
						C1219_st_13_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_21_ID:
						C1219_st_21_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_22_ID:
						C1219_st_22_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_23_ID:
						C1219_st_23_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_24_ID:
						C1219_st_24_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_25_ID:
						C1219_st_25_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_26_ID:
						C1219_st_26_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_27_ID:
						C1219_st_27_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_41_ID:
						C1219_st_41_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_42_ID:
						C1219_st_42_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_43_ID:
						C1219_st_43_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_44_ID:
						C1219_st_44_pwrite_offset(&c1219[instance]);
						break;

					case ST_51_ID:
						C1219_st_51_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_53_ID:
						C1219_st_53_pwrite_offset(&c1219[instance]);
						break;

					case ST_54_ID:
						C1219_st_54_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_61_ID:
						C1219_st_61_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_62_ID:
						C1219_st_62_pwrite_offset(&c1219[instance]);
						break;
					
					case ST_71_ID:
						C1219_st_71_pwrite_offset(&c1219[instance]);
						break;
					
					case ST_73_ID:
						C1219_st_73_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_74_ID:
						C1219_st_74_pwrite_offset(&c1219[instance]);
						break;
					
					case ST_75_ID:
						C1219_st_75_pwrite_offset(&c1219[instance]);
						break;
						
					case ST_76_ID:
						C1219_st_76_pwrite_offset(&c1219[instance]);
						break;		
						
					case MT_1_ID:
						C1219_mt_1_pwrite_offset(&c1219[instance]);
						break;
						
					case MT_3_ID:
						C1219_mt_3_pwrite_offset(&c1219[instance]);
						break;
						
					case MT_4_ID:
						C1219_mt_4_pwrite_offset(&c1219[instance]);
						break;
						
					case MT_5_ID:
						C1219_mt_5_pwrite_offset(&c1219[instance]);
						break;
						
					case MT_53_ID:
						C1219_mt_53_pwrite_offset(&c1219[instance]);
						break;

					case MT_73_ID:
						C1219_mt_73_pwrite_offset(&c1219[instance]);
						break;
						
					case MT_202_ID:
						C1219_mt_202_pwrite_offset(&c1219[instance]);
						break;
								    
					default:
						C1218_send_iar(c1219[instance].c1218_dll);
						break;
				}
			}
			else {
				C1218_send_isc(c1219[instance].c1218_dll);
			}		
		}
		else {
			C1218_send_err(c1219[instance].c1218_dll);
		}

		c1219[instance].busy_instance = false;
	}
}

/**
 ******************************************************************************
 * \fn void C1219_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_full_read(C1219_TYPE *c1219_ptr) {
	
	static volatile uint16_t response_index;

	response_index = 0;	
	c1219_ptr->c1218_dll->tx_data[response_index++] = C1218_OK;
	c1219_ptr->c1218_dll->tx_data[response_index++] = (uint8_t)((c1219_ptr->table_size & 0xFF00) >> 8);
	c1219_ptr->c1218_dll->tx_data[response_index++] = (uint8_t)((c1219_ptr->table_size & 0x00FF) >> 0);
		
	c1219_ptr->execute_table_reading(&c1219_ptr->table_data[0]);
	memcpy(&c1219_ptr->c1218_dll->tx_data[response_index], &c1219_ptr->table_data[0], c1219_ptr->table_size);
	response_index += c1219_ptr->table_size;
	
	c1219_ptr->c1218_dll->tx_data[response_index++] = CRC_2s_complement(&c1219_ptr->c1218_dll->tx_data[3], c1219_ptr->table_size);
	c1219_ptr->c1218_dll->tx_data_length = response_index;
	
	C1218_send_response(c1219_ptr->c1218_dll);
}

/**
 ******************************************************************************
 * \fn void C1219_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_pread_offset(C1219_TYPE *c1219_ptr) {
	
	static volatile uint16_t response_index;

	if ( (c1219_ptr->offset + (uint32_t)c1219_ptr->count) <= (uint32_t)c1219_ptr->table_size ) {
		
		response_index = 0;
		c1219_ptr->c1218_dll->tx_data[response_index++] = C1218_OK;
		c1219_ptr->c1218_dll->tx_data[response_index++] = (uint8_t)((c1219_ptr->count & 0xFF00) >> 8);
		c1219_ptr->c1218_dll->tx_data[response_index++] = (uint8_t)((c1219_ptr->count & 0x00FF) >> 0);
		
		c1219_ptr->execute_table_reading(&c1219_ptr->table_data[0]);
		memcpy(&c1219_ptr->c1218_dll->tx_data[response_index], &c1219_ptr->table_data[c1219_ptr->offset], c1219_ptr->count);
		response_index += c1219_ptr->count;
		
		c1219_ptr->c1218_dll->tx_data[response_index++] = CRC_2s_complement(&c1219_ptr->c1218_dll->tx_data[3], c1219_ptr->count);
		c1219_ptr->c1218_dll->tx_data_length = response_index;
		
		C1218_send_response(c1219_ptr->c1218_dll);
	}
	else {
		
		C1218_send_onp(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_full_write(C1219_TYPE *c1219_ptr) {
	
	if ( c1219_ptr->count == c1219_ptr->table_size ) {
		
		if ( c1219_ptr->execute_table_writing(&c1219_ptr->c1218_dll->rx_data[5]) ) {
			C1218_send_ok(c1219_ptr->c1218_dll);
		}
		else {
			C1218_send_dlk(c1219_ptr->c1218_dll);
		}
	}
	else {
		C1218_send_onp(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	if ( (c1219_ptr->offset + (uint32_t)c1219_ptr->count) <= (uint32_t)c1219_ptr->table_size ) {
		
		c1219_ptr->execute_table_reading(&c1219_ptr->table_data[0]);
		memcpy(&c1219_ptr->table_data[c1219_ptr->offset], &c1219_ptr->c1218_dll->rx_data[8], c1219_ptr->count);
		
		if ( c1219_ptr->execute_table_writing(&c1219_ptr->table_data[0]) ) {
			C1218_send_ok(c1219_ptr->c1218_dll);
		}
		else {
			C1218_send_dlk(c1219_ptr->c1218_dll);
		}
	}
	else {
		C1218_send_onp(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_st_0_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_0_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_0_get_size();
	c1219_ptr->execute_table_reading = GRAL_APP_bSt_0_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_0_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_0_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_0_get_size();
	c1219_ptr->execute_table_reading = GRAL_APP_bSt_0_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_1_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_1_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_1_get_size();
	c1219_ptr->execute_table_reading = GRAL_APP_bSt_1_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_1_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_1_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_1_get_size();
	c1219_ptr->execute_table_reading = GRAL_APP_bSt_1_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_1_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_1_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_1_get_size();
	c1219_ptr->execute_table_writing = GRAL_APP_bSt_1_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_1_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_1_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_1_get_size();
	c1219_ptr->execute_table_reading = GRAL_APP_bSt_1_execute_table_reading;
	c1219_ptr->execute_table_writing = GRAL_APP_bSt_1_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_2_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_2_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_2_get_size();
	c1219_ptr->execute_table_reading = GRAL_APP_bSt_2_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_2_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_2_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_2_get_size();
	c1219_ptr->execute_table_reading = GRAL_APP_bSt_2_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_3_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_3_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_3_get_size();
	c1219_ptr->execute_table_reading = GRAL_APP_bSt_3_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_3_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_3_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_3_get_size();
	c1219_ptr->execute_table_reading = GRAL_APP_bSt_3_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_6_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_6_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_6_get_size();
	c1219_ptr->execute_table_reading = GRAL_APP_bSt_6_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_6_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_6_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_6_get_size();
	c1219_ptr->execute_table_reading = GRAL_APP_bSt_6_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_6_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_6_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_6_get_size();
	c1219_ptr->execute_table_writing = GRAL_APP_bSt_6_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_6_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_6_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = GRAL_APP_usSt_6_get_size();
	c1219_ptr->execute_table_reading = GRAL_APP_bSt_6_execute_table_reading;
	c1219_ptr->execute_table_writing = GRAL_APP_bSt_6_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_8_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_8_full_read(C1219_TYPE *c1219_ptr)
{
	uint8_t k;
	
	k = 0;
	c1219_ptr->c1218_dll->tx_data_length = c1219_ptr->c1218_dll->st_08_length + 4;
	
	c1219_ptr->c1218_dll->tx_data[k++] = SERVICE_R_OK;									
	c1219_ptr->c1218_dll->tx_data[k++] = (uint8_t)((c1219_ptr->c1218_dll->st_08_length & 0xFF00) >> 8);		
	c1219_ptr->c1218_dll->tx_data[k++] = (uint8_t)((c1219_ptr->c1218_dll->st_08_length & 0x00FF) >> 0);
	
	memcpy(&c1219_ptr->c1218_dll->tx_data[READ_R_DATA_INDEX], &c1219_ptr->c1218_dll->st_08_data[0], c1219_ptr->c1218_dll->st_08_length);
	k += c1219_ptr->c1218_dll->st_08_length;
	
	c1219_ptr->c1218_dll->tx_data[k++] = CRC_2s_complement(&c1219_ptr->c1218_dll->tx_data[3], c1219_ptr->c1218_dll->st_08_length);
	C1218_send_response(c1219_ptr->c1218_dll);
}

/**
 ******************************************************************************
 * \fn void C1219_st_8_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_8_pread_offset(C1219_TYPE *c1219_ptr)
{
	uint8_t k;
	
	if ( (c1219_ptr->offset + (uint32_t)c1219_ptr->count) <= c1219_ptr->c1218_dll->st_08_length )
	{
		k = 0;
		c1219_ptr->c1218_dll->tx_data_length = c1219_ptr->count + 4;
	
		c1219_ptr->c1218_dll->tx_data[k++] = SERVICE_R_OK;								
		c1219_ptr->c1218_dll->tx_data[k++] = (uint8_t)((c1219_ptr->count & 0xFF00) >> 8);
		c1219_ptr->c1218_dll->tx_data[k++] = (uint8_t)((c1219_ptr->count & 0x00FF) >> 0);	
	
		memcpy(&c1219_ptr->c1218_dll->tx_data[READ_R_DATA_INDEX], &c1219_ptr->c1218_dll->st_08_data[c1219_ptr->offset], c1219_ptr->count);
		k += c1219_ptr->count;
	
		c1219_ptr->c1218_dll->tx_data[k++] = CRC_2s_complement(&c1219_ptr->c1218_dll->tx_data[3], c1219_ptr->count);
		C1218_send_response(c1219_ptr->c1218_dll);
	}
	else
	{
		C1218_send_onp(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_st_10_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_10_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_10_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_10_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_10_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_10_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_10_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_10_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_11_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_11_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_11_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_11_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_11_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_11_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_11_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_11_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_11_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_11_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_11_get_size();
	c1219_ptr->execute_table_writing = REGS_APP_bSt_11_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_11_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_11_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_11_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_11_execute_table_reading;
	c1219_ptr->execute_table_writing = REGS_APP_bSt_11_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_12_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_12_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_12_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_12_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_12_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_12_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_12_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_12_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_13_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_13_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_13_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_13_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_13_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_13_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_13_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_13_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_13_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_13_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_13_get_size();
	c1219_ptr->execute_table_writing = REGS_APP_bSt_13_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_13_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_13_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_13_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_13_execute_table_reading;
	c1219_ptr->execute_table_writing = REGS_APP_bSt_13_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_15_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_15_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_15_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_15_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_15_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_15_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_15_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_15_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_16_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_16_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_16_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_16_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_16_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_16_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_16_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_16_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_20_full_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_20_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_20_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_20_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_20_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_20_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_20_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_20_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_21_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_21_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_21_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_21_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_21_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_21_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_21_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_21_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_21_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_21_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_21_get_size();
	c1219_ptr->execute_table_writing = REGS_APP_bSt_21_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_21_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_21_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_21_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_21_execute_table_reading;
	c1219_ptr->execute_table_writing = REGS_APP_bSt_21_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_22_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_22_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_22_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_22_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_22_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_22_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_22_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_22_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_22_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_22_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_22_get_size();
	c1219_ptr->execute_table_writing = REGS_APP_bSt_22_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_22_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_22_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_22_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_22_execute_table_reading;
	c1219_ptr->execute_table_writing = REGS_APP_bSt_22_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_23_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_23_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_23_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_23_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_23_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_23_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_23_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_23_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_23_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_23_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_23_get_size();
	c1219_ptr->execute_table_writing = REGS_APP_bSt_23_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_23_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_23_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_23_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_23_execute_table_reading;
	c1219_ptr->execute_table_writing = REGS_APP_bSt_23_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_24_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_24_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_24_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_24_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_24_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_24_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_24_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_24_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_24_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_24_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_24_get_size();
	c1219_ptr->execute_table_writing = REGS_APP_bSt_24_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_24_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_24_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_24_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_24_execute_table_reading;
	c1219_ptr->execute_table_writing = REGS_APP_bSt_24_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_25_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_25_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_25_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_25_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_25_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_25_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_25_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_25_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_25_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_25_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_25_get_size();
	c1219_ptr->execute_table_writing = REGS_APP_bSt_25_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_25_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_25_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_25_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_25_execute_table_reading;
	c1219_ptr->execute_table_writing = REGS_APP_bSt_25_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_26_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_26_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_26_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_26_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_26_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_26_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_26_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_26_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_26_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_26_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_26_get_size();
	c1219_ptr->execute_table_writing = REGS_APP_bSt_26_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_26_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_26_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_26_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_26_execute_table_reading;
	c1219_ptr->execute_table_writing = REGS_APP_bSt_26_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_27_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_27_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_27_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_27_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_27_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_27_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_27_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_27_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_27_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_27_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_27_get_size();
	c1219_ptr->execute_table_writing = REGS_APP_bSt_27_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_27_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_27_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_27_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_27_execute_table_reading;
	c1219_ptr->execute_table_writing = REGS_APP_bSt_27_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_28_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_28_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_28_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_28_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_28_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_28_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = REGS_APP_usSt_28_get_size();
	c1219_ptr->execute_table_reading = REGS_APP_bSt_28_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_40_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_40_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_40_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_40_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_40_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_40_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_40_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_40_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_41_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_41_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_41_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_41_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_41_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_41_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_41_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_41_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_41_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_41_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_41_get_size();
	c1219_ptr->execute_table_writing = SECR_APP_bSt_41_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_41_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_41_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_41_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_41_execute_table_reading;
	c1219_ptr->execute_table_writing = SECR_APP_bSt_41_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_42_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_42_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_42_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_42_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_42_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_42_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_42_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_42_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_42_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_42_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_42_get_size();
	c1219_ptr->execute_table_writing = SECR_APP_bSt_42_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_42_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_42_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_42_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_42_execute_table_reading;
	c1219_ptr->execute_table_writing = SECR_APP_bSt_42_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_43_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_43_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_43_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_43_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_43_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_43_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_43_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_43_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_43_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_43_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_43_get_size();
	c1219_ptr->execute_table_writing = SECR_APP_bSt_43_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_43_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_43_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_43_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_43_execute_table_reading;
	c1219_ptr->execute_table_writing = SECR_APP_bSt_43_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_44_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_44_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_44_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_44_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_44_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_44_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_44_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_44_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_44_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_44_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_44_get_size();
	c1219_ptr->execute_table_writing = SECR_APP_bSt_44_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_44_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_44_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SECR_APP_usSt_44_get_size();
	c1219_ptr->execute_table_reading = SECR_APP_bSt_44_execute_table_reading;
	c1219_ptr->execute_table_writing = SECR_APP_bSt_44_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_50_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_50_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_sSt_50_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_st_50_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_50_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_50_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_sSt_50_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_st_50_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_51_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_51_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_51_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_51_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_51_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_51_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_51_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_51_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_51_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_51_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_51_get_size();
	c1219_ptr->execute_table_writing = TOU_APP_bSt_51_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_51_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_51_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_51_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_51_execute_table_reading;
	c1219_ptr->execute_table_writing = TOU_APP_bSt_51_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_52_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_52_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_52_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_52_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_52_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_52_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_52_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_52_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_53_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_53_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_53_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_53_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_53_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_53_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_53_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_53_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_53_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_53_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_53_get_size();
	c1219_ptr->execute_table_writing = TOU_APP_bSt_53_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_53_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_53_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_53_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_53_execute_table_reading;
	c1219_ptr->execute_table_writing = TOU_APP_bSt_53_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_54_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_54_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_54_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_54_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_54_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_54_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_54_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_54_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_54_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_54_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_54_get_size();
	c1219_ptr->execute_table_writing = TOU_APP_bSt_54_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_54_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_54_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_54_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_54_execute_table_reading;
	c1219_ptr->execute_table_writing = TOU_APP_bSt_54_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_55_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_55_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_55_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_55_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_55_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_55_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = TOU_APP_usSt_55_get_size();
	c1219_ptr->execute_table_reading = TOU_APP_bSt_55_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_60_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_60_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = LP_APP_usSt_60_get_size();
	c1219_ptr->execute_table_reading = LP_APP_bSt_60_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_60_pread_offset(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_60_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = LP_APP_usSt_60_get_size();
	c1219_ptr->execute_table_reading = LP_APP_bSt_60_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_61_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_61_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = LP_APP_usSt_61_get_size();
	c1219_ptr->execute_table_reading = LP_APP_bSt_61_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_61_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_61_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = LP_APP_usSt_61_get_size();
	c1219_ptr->execute_table_reading = LP_APP_bSt_61_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_61_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_61_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = LP_APP_usSt_61_get_size();
	c1219_ptr->execute_table_writing = LP_APP_bSt_61_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_61_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_61_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = LP_APP_usSt_61_get_size();
	c1219_ptr->execute_table_reading = LP_APP_bSt_61_execute_table_reading;
	c1219_ptr->execute_table_writing = LP_APP_bSt_61_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_62_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_62_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = LP_APP_usSt_62_get_size();
	c1219_ptr->execute_table_reading = LP_APP_bSt_62_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_62_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_62_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = LP_APP_usSt_62_get_size();
	c1219_ptr->execute_table_reading = LP_APP_bSt_62_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_62_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_62_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = LP_APP_usSt_62_get_size();
	c1219_ptr->execute_table_writing = LP_APP_bSt_62_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_62_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_62_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = LP_APP_usSt_62_get_size();
	c1219_ptr->execute_table_reading = LP_APP_bSt_62_execute_table_reading;
	c1219_ptr->execute_table_writing = LP_APP_bSt_62_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_63_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_63_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = LP_APP_usSt_63_get_size();
	c1219_ptr->execute_table_reading = LP_APP_bSt_63_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_63_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_63_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = LP_APP_usSt_63_get_size();
	c1219_ptr->execute_table_reading = LP_APP_bSt_63_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_64_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_64_pread_offset(C1219_TYPE *c1219_ptr) {
	
	static uint16_t i, j, k, l = 0;
	static uint16_t block_size;
	static uint16_t block_start, block_end = 0;
	
	uint32_t st_64_size = LP_APP_ulSt_64_get_size();
	
	if ( ( (c1219_ptr->offset + c1219_ptr->count) <= st_64_size ) && ( c1219_ptr->count <= (C1218_DLL_SIZE * C1218_NUM_PACKETS) ) )
	{
		block_size = st_64_size/st_61.nbr_blks_set1;
		
		for ( i = 0; i < st_61.nbr_blks_set1; i++ )
		{
			if ( (c1219_ptr->offset >= (i + 0)*block_size ) && (c1219_ptr->offset <= (i + 1)*block_size ) ) { block_start = i; }
			if ( ( (c1219_ptr->offset + c1219_ptr->count) > (i + 0)*block_size ) && ( (c1219_ptr->offset + c1219_ptr->count) <= (i + 1)*block_size ) ) { block_end = i; }
		}
		
		k = 3;
		if ( (st_63.lp_set_status_flags & LIST_TYPE_bm) == LIST_TYPE_bm )
		{	
			LP_APP_vGet_data_block_circ_table( block_start, (block_end - block_start + 1), &c1219_ptr->c1218_dll->tx_data[k] );
		}
		else
		{
			LP_APP_vGet_data_block_fifo_table( block_start, (block_end - block_start + 1), &c1219_ptr->c1218_dll->tx_data[k] );
		}
		
		c1219_ptr->offset -= block_size*block_start;

		memcpy(&c1219_ptr->c1218_dll->tx_data[READ_R_DATA_INDEX], &c1219_ptr->c1218_dll->tx_data[ READ_R_DATA_INDEX + c1219_ptr->offset], c1219_ptr->count);
		
		c1219_ptr->c1218_dll->tx_data[0] = C1218_OK;
		c1219_ptr->c1218_dll->tx_data[1] = (uint8_t)(c1219_ptr->count >> 8);
		c1219_ptr->c1218_dll->tx_data[2] = (uint8_t)(c1219_ptr->count >> 0);
		c1219_ptr->c1218_dll->tx_data[3 + c1219_ptr->count] = CRC_2s_complement(&c1219_ptr->c1218_dll->tx_data[READ_R_DATA_INDEX], c1219_ptr->count);
		
		c1219_ptr->c1218_dll->tx_data_length = c1219_ptr->count + 4;
		C1218_send_response(c1219_ptr->c1218_dll);
	}
	else
	{
		C1218_send_onp(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_st_70_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_70_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_70_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_ubSt_70_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_70_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_70_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_70_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_ubSt_70_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_71_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_71_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_71_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_71_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_71_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_71_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_71_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_71_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_71_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_71_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_71_get_size();
	c1219_ptr->execute_table_writing = EVNT_APP_bSt_71_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_71_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_71_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_71_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_71_execute_table_reading;
	c1219_ptr->execute_table_writing = EVNT_APP_bSt_71_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_72_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_72_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_72_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_72_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_72_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_72_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_72_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_72_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_73_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_73_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_73_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_73_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_73_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_73_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_73_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_73_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_71_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_73_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_73_get_size();
	c1219_ptr->execute_table_writing = EVNT_APP_bSt_73_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_73_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_73_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_73_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_73_execute_table_reading;
	c1219_ptr->execute_table_writing = EVNT_APP_bSt_73_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_74_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_74_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_74_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_74_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_74_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_74_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_74_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_74_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_74_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_74_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_74_get_size();
	c1219_ptr->execute_table_writing = EVNT_APP_bSt_74_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_74_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_74_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_74_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_74_execute_table_reading;
	c1219_ptr->execute_table_writing = EVNT_APP_bSt_74_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_75_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_75_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_75_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_75_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_75_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_75_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_75_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_75_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_75_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_75_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_75_get_size();
	c1219_ptr->execute_table_writing = EVNT_APP_bSt_75_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_75_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_75_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_75_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_75_execute_table_reading;
	c1219_ptr->execute_table_writing = EVNT_APP_bSt_75_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_76_full_read(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_76_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_76_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_76_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_76_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_76_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_76_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_76_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_76_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_76_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_76_get_size();
	c1219_ptr->execute_table_writing = EVNT_APP_bSt_76_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_st_76_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_76_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usSt_76_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bSt_76_execute_table_reading;
	c1219_ptr->execute_table_writing = EVNT_APP_bSt_76_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_1_full_read(C1219_TYPE *c1219_ptr)
 * \brief NET50 Specific Product Identification
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_1_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_1_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_1_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_1_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief NET50 Specific Product Identification
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_1_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_1_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_1_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_1_full_write(C1219_TYPE *c1219_ptr)
 * \brief NET50 Specific Product Identification
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_1_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_1_get_size();
	c1219_ptr->execute_table_writing = SM_APP_bMt_1_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_1_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief NET50 Specific Product Identification
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_1_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_1_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_1_execute_table_reading;
	c1219_ptr->execute_table_writing = SM_APP_bMt_1_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_3_full_read(C1219_TYPE *c1219_ptr)
 * \brief Manufacturer Configuration Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_3_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_3_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_3_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_3_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief Manufacturer Configuration Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_3_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_3_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_3_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_3_full_write(C1219_TYPE *c1219_ptr)
 * \brief Manufacturer Configuration Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_3_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_3_get_size();
	c1219_ptr->execute_table_writing = SM_APP_bMt_3_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_3_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief Manufacturer Configuration Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_3_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_3_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_3_execute_table_reading;
	c1219_ptr->execute_table_writing = SM_APP_bMt_3_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_4_full_read(C1219_TYPE *c1219_ptr)
 * \brief MCU Trouble Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_4_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_4_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_4_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_4_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief MCU Trouble Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_4_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_4_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_4_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_4_full_write(C1219_TYPE *c1219_ptr)
 * \brief MCU Trouble Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_4_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_4_get_size();
	c1219_ptr->execute_table_writing = SM_APP_bMt_4_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_4_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief MCU Trouble Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_4_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_4_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_4_execute_table_reading;
	c1219_ptr->execute_table_writing = SM_APP_bMt_4_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_5_full_read(C1219_TYPE *c1219_ptr)
 * \brief Table Integrity Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_5_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_5_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_5_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_5_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief Table Integrity Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_5_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_5_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_5_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_5_full_write(C1219_TYPE *c1219_ptr)
 * \brief Table Integrity Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_5_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_5_get_size();
	c1219_ptr->execute_table_writing = SM_APP_bMt_5_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_5_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief Table Integrity Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_5_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_5_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_5_execute_table_reading;
	c1219_ptr->execute_table_writing = SM_APP_bMt_5_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_22_full_read(C1219_TYPE *c1219_ptr)
 * \brief Present Register Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_22_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_22_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_22_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_22_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief Present Register Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_22_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_22_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_22_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_23_full_read(C1219_TYPE *c1219_ptr)
 * \brief Present Register Status Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_23_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_23_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_23_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_23_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief Present Register Status Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_23_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_23_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_23_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_24_full_read(C1219_TYPE *c1219_ptr)
 * \brief DSP Accumulated Quantities
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_24_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_24_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_24_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_24_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief DSP Accumulated Quantities
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_24_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_24_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_24_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_25_full_read(C1219_TYPE *c1219_ptr)
 * \brief DSP Calibration Constants Register
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_25_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_25_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_25_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_25_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief DSP Calibration Constants Register
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_25_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_25_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_25_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_53_full_read(C1219_TYPE *c1219_ptr)
 * \brief Time Offset Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_53_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_53_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_53_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_53_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief Time Offset Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_53_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_53_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_53_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_53_full_write(C1219_TYPE *c1219_ptr)
 * \brief Time Offset Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_53_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_53_get_size();
	c1219_ptr->execute_table_writing = SM_APP_bMt_53_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_53_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief Time Offset Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_53_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = SM_APP_usMt_53_get_size();
	c1219_ptr->execute_table_reading = SM_APP_bMt_53_execute_table_reading;
	c1219_ptr->execute_table_writing = SM_APP_bMt_53_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_72_full_read(C1219_TYPE *c1219_ptr)
 * \brief Alarms Identification Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_72_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usMt_72_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bMt_72_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_72_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief Alarms Identification Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_72_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usMt_72_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bMt_72_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_73_full_read(C1219_TYPE *c1219_ptr)
 * \brief Alarms Control Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_73_full_read(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usMt_73_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bMt_73_execute_table_reading;
	C1219_full_read(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_73_pread_offset(C1219_TYPE *c1219_ptr)
 * \brief Alarms Control Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_73_pread_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usMt_73_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bMt_73_execute_table_reading;
	C1219_pread_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_73_full_write(C1219_TYPE *c1219_ptr)
 * \brief Alarms Control Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_73_full_write(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usMt_73_get_size();
	c1219_ptr->execute_table_writing = EVNT_APP_bMt_73_execute_table_writing;
	C1219_full_write(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_73_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief Alarms Control Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_73_pwrite_offset(C1219_TYPE *c1219_ptr) {
	
	c1219_ptr->table_size = EVNT_APP_usMt_73_get_size();
	c1219_ptr->execute_table_reading = EVNT_APP_bMt_73_execute_table_reading;
	c1219_ptr->execute_table_writing = EVNT_APP_bMt_73_execute_table_writing;
	C1219_pwrite_offset(c1219_ptr);
}

/**
 ******************************************************************************
 * \fn void C1219_mt_202_pwrite_offset(C1219_TYPE *c1219_ptr)
 * \brief Image Data Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mt_202_pwrite_offset(C1219_TYPE *c1219_ptr)
{
	static bool write_ok = false;
	static uint32_t flash_offset;
	static uint32_t  *mt144_page;
	static uint16_t table_count;
	static uint8_t table_buffer[128];
	static uint32_t image_start_addr = IMAGE_APP_START_ADDR;
	
	flash_offset = ((uint32_t)c1219_ptr->c1218_dll->rx_buffer[9]<<16);
	flash_offset = flash_offset |((uint32_t)c1219_ptr->c1218_dll->rx_buffer[10]<<8);
	flash_offset = flash_offset |((uint32_t)c1219_ptr->c1218_dll->rx_buffer[11]);
	flash_offset = flash_offset + (0x01040000);
	
	table_count = (uint16_t)c1219_ptr->c1218_dll->rx_buffer[12] <<8;
	table_count = (uint16_t)c1219_ptr->c1218_dll->rx_buffer[13];
   
	mt144_page = (uint32_t *) flash_offset;
   
	if (flash_offset >= 0x50000) { // Definir la direccion de inicio
	
		 flash_unlock(image_start_addr, image_start_addr + (TOTAL_IMAGE_SECTOR_SIZE) - 1, 0, 0);  
		
		 memcpy(&table_buffer,&c1219_ptr->c1218_dll->rx_buffer[14],table_count);
		 
		 flash_write((uint32_t)mt144_page, (void*)table_buffer, table_count, 0); 
		 
		 write_ok = true;
	}     

	if ( write_ok ) {
		
		c1219_ptr->c1218_dll->tx_data_length = 0;
		c1219_ptr->c1218_dll->tx_data[c1219_ptr->c1218_dll->tx_data_length++] = C1218_OK;
		C1218_send_response(c1219_ptr->c1218_dll);
	}
	else {
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_st_7_full_write(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_st_7_full_write(C1219_TYPE *c1219_ptr) {

	proc_id  = (uint16_t)(c1219_ptr->c1218_dll->rx_data[PROCEDURE_ID_INDEX + 0] << 0);
	proc_id |= (uint16_t)(c1219_ptr->c1218_dll->rx_data[PROCEDURE_ID_INDEX + 1] << 8);
	proc_seq_nbr = c1219_ptr->c1218_dll->rx_data[PROCEDURE_SEQ_NBR_INDEX];

	EVNT_APP_vRegister_event(PROCEDURE_INVOKED, proc_id, true, c1219_ptr->c1218_apl.user_id, rtc.utc_timestamp);
	
 	SECURITY_USER_INFO_TYPE user_info = {
 		.tbl_proc_id			= (proc_id & TBL_PROC_NBR_bm) >> TBL_PROC_NBR_bp,
 		.mfg_flag				= (bool)( (proc_id & STD_VS_MFG_FLAG_bm) >> STD_VS_MFG_FLAG_bp ),
 		.access_permissions		= c1219_ptr->c1218_apl.access_permissions
 	};
	memcpy(&user_info.password[0], &c1219_ptr->c1218_apl.password[0], 20);

	if ( SECR_APP_bCheck_access_to_execute_procedure(user_info) ) {
		
		switch ( proc_id ) {
			
			case SP_0_ID:
				C1219_sp_0_execute(c1219_ptr);
				break;
				
			case SP_1_ID:
				C1219_sp_1_execute(c1219_ptr);
				break;

			case SP_3_ID:
				C1219_sp_3_execute(c1219_ptr);
				break;
				
			case SP_5_ID:
				C1219_sp_5_execute(c1219_ptr);
				break;
			
			case SP_6_ID:
				C1219_sp_6_execute(c1219_ptr);
				break;
				
			case SP_7_ID:
				C1219_sp_7_execute(c1219_ptr);
				break;
				
			case SP_8_ID:
				C1219_sp_8_execute(c1219_ptr);
				break;
				
			case SP_9_ID:
				C1219_sp_9_execute(c1219_ptr);
				break;
			
			case SP_10_ID:
				C1219_sp_10_execute(c1219_ptr);
				break;
			
			case SP_11_ID:
				C1219_sp_11_execute(c1219_ptr);
				break;

			case MP_6_ID:
				C1219_mp_6_execute(c1219_ptr);
				break;

			case MP_9_ID:
				C1219_mp_9_execute(c1219_ptr);
				break;
				
			case MP_100_ID:
				C1219_mp_100_execute(c1219_ptr);
				break;

			case MP_101_ID:
				C1219_mp_101_execute(c1219_ptr);
				break;
				
			case MP_102_ID:
				C1219_mp_102_execute(c1219_ptr);
				break;

			case MP_103_ID:
				C1219_mp_103_execute(c1219_ptr);
				break;
				
			case MP_104_ID:
				C1219_mp_104_execute(c1219_ptr);
				break;

			case MP_110_ID:
				C1219_mp_110_execute(c1219_ptr);
				break;

			case MP_111_ID:
				C1219_mp_111_execute(c1219_ptr);
				break;

			case MP_112_ID:
				C1219_mp_112_execute(c1219_ptr);
				break;

			case MP_113_ID:
				C1219_mp_113_execute(c1219_ptr);
				break;
			
			case MP_120_ID:
				C1219_mp_120_execute(c1219_ptr);
				break;

			case MP_121_ID:
				C1219_mp_121_execute(c1219_ptr);
				break;

			case MP_122_ID:
				C1219_mp_122_execute(c1219_ptr);
				break;

			case MP_123_ID:
				C1219_mp_123_execute(c1219_ptr);
				break;

			case MP_124_ID:
				C1219_mp_124_execute(c1219_ptr);
				break;

			case MP_125_ID:
				C1219_mp_125_execute(c1219_ptr);
				break;
											
			case MP_126_ID:
				C1219_mp_126_execute(c1219_ptr);
				break;

			case MP_130_ID:
				C1219_mp_130_execute(c1219_ptr);
				break;

			case MP_150_ID:
				C1219_mp_150_execute(c1219_ptr);
				break;
					
			case MP_200_ID:
				C1219_mp_200_execute(c1219_ptr);
				break;
								
			case MP_201_ID:
				C1219_mp_201_execute(c1219_ptr);
				break;
				
			default:
				C1218_send_iar(c1219_ptr->c1218_dll);
				break;
		}
	}
	else {
		C1218_send_isc(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_sp_0_execute(C1219_TYPE *c1219_ptr)
 * \brief Cold Start
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_sp_0_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if (c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + SP_0_PARAMETERS_SIZE)) {
			
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_ACCEPTED_NOT_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);

		meter.system_state.busy = true;
		meter.system_state.changing_configuration = true;
		
		vTaskSuspend(tou_os.task_id);
		vTaskSuspend(registers_os.storage_task_id);
				
		GRAL_APP_bSt_3_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		REGS_APP_bSt_11_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		REGS_APP_bSt_13_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		REGS_APP_bSt_21_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		REGS_APP_bSt_22_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		REGS_APP_vClear_st_23();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		REGS_APP_vClear_st_24();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		REGS_APP_vClear_st_25();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		REGS_APP_vClear_st_26();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		REGS_APP_bSt_27_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		TOU_APP_bSt_51_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		TOU_APP_bSt_53_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		LP_APP_bSt_61_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		LP_APP_bSt_62_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		LP_APP_bSt_63_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		EVNT_APP_bSt_71_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		EVNT_APP_bSt_73_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		EVNT_APP_vClear_st_74(c1219_ptr->c1218_apl.user_id);
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		EVNT_APP_bSt_75_restore_default_values();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		EVNT_APP_vClear_st_76(c1219_ptr->c1218_apl.user_id);
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		SM_APP_vMt_23_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		SM_APP_vMt_53_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();

		EVNT_APP_bMt_73_restore_default_values();
		PFAIL_APP_vTask();
		NIC_task();
		OP_task();
		WATCHDOG_APP_vRestart();
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		while ( (op_c1218_apl.communication_state != BASE_STATE) || (nic_c1218_apl.communication_state != BASE_STATE) ) {
			
			NIC_task();
			PFAIL_APP_vTask();
			WATCHDOG_APP_vRestart();

			OP_task();
			PFAIL_APP_vTask();
			WATCHDOG_APP_vRestart();
		}
		
		SYSTEM_RESTART_TYPE mcu;
		mcu.restart_cause = COLD_START;
		mcu.save_information = false;
		RST_APP_vExecute_mcu_reset(mcu);
	}
	else {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_sp_1_execute(C1219_TYPE *c1219_ptr)
 * \brief Warm Start
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_sp_1_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k;
	
	if (c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + SP_1_PARAMETERS_SIZE)) {
			
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
		
		delay_ms(500);
		
		SYSTEM_RESTART_TYPE mcu;
		mcu.restart_cause = WARM_START;
		mcu.save_information = true;
		RST_APP_vExecute_mcu_reset(mcu);
	}
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_sp_3_execute(C1219_TYPE *c1219_ptr)
 * \brief Clear Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_sp_3_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;

	if (c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + SP_3_PARAMETERS_SIZE)) {
		
		meter.system_state.busy = true;
		meter.system_state.changing_configuration = true;
		REGS_APP_vClear_st_23();
		REGS_APP_vClear_st_24();
		REGS_APP_vClear_st_25();
		REGS_APP_vClear_st_26();		
		meter.system_state.busy = false;
		meter.system_state.changing_configuration = false;
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
	}
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_sp_5_execute(C1219_TYPE *c1219_ptr)
 * \brief Update Last Read Entry
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_sp_5_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k;
	uint16_t entries_read;
	
	if (c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + SP_5_PARAMETERS_SIZE) ) {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		
		entries_read  = (uint16_t)(c1219_ptr->c1218_dll->rx_data[SP_5_ENTRIES_READ_INDEX]);
		entries_read |= (uint16_t)(c1219_ptr->c1218_dll->rx_data[SP_5_ENTRIES_READ_INDEX + 1] << 8);
		
		switch ( c1219_ptr->c1218_dll->rx_data[SP_5_LIST_INDEX] ) {
			
			case EVENT_LOG_TBL:
				EEPROM_writer( ST_74_NBR_UNREAD_ENTRIES_ADDR, &entries_read, 2 );
				c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
				break;
				
			case SELF_READ_DATA_TBL:
				if ( entries_read < 0xFF ) {
					EEPROM_writer( ST_26_NBR_UNREAD_ENTRIES_ADDR, &entries_read, 1 );
					c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
				}
				else {
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				}
						
				break;
				
			case LP_DATA_SET1_TBL:
				EEPROM_writer( ST_63_NBR_UNREAD_BLOCKS_ADDR, &entries_read, 2 );
				c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
				break;
				
			case LP_DATA_SET2_TBL:
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				break;
				
			case LP_DATA_SET3_TBL:
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				break;
				
			case LP_DATA_SET4_TBL:
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				break;
				
			case LP_DATA_SET_1_2_3_4_TBL:
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				break;
				
			case HISTORY_LOG_TBL:
				EEPROM_writer( ST_76_NBR_UNREAD_ENTRIES_ADDR, &entries_read, 2 );
				c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
				break;
				
			case QUALITY_LOG_TBL:
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				break;
				
			case TD_ASYNC_TBL:
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				break;
				
			case FD_ASYNC_TBL:
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				break;
				
			case TIME_DOMAIN_TBL:
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				break;
				
			case FD_SPECTRA_TBL:
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				break;
				
			case ALL_LIST_NO_EVENT_LOG_TBL:
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				break;
				
			default:
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				break;
		}

		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
	}
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_sp_6_execute(C1219_TYPE *c1219_ptr)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_sp_6_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k, ed_mode_temp;	
	
	if (c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + SP_6_PARAMETERS_SIZE)) {
		
		if (((uint8_t)(c1219_ptr->c1218_dll->rx_data[8])> 0) && (((uint8_t)(c1219_ptr->c1218_dll->rx_data[8])< 15))) {
			
			ed_mode_temp = (uint8_t)(c1219_ptr->c1218_dll->rx_data[8]);
			
			if ( (ed_mode_temp == METERING_FLAG_bm) || (ed_mode_temp == TEST_MODE_FLAG_bm) ){
				
				if ( ed_mode_temp == METERING_FLAG_bm ) {
					
					GRAL_APP_bEnable_metering_mode(c1219_ptr->c1218_apl.user_id, c1219_ptr->c1218_dll->uart);
				}
				
				if ( ed_mode_temp == TEST_MODE_FLAG_bm ) {
					
					GRAL_APP_bEnable_test_mode(c1219_ptr->c1218_apl.user_id, c1219_ptr->c1218_dll->uart);
				}
								
				k = 0;
				c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
				c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
				c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
				c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
				c1219_ptr->c1218_dll->st_08_data[k++] = ed_mode_temp;
				c1219_ptr->c1218_dll->st_08_length = k;
				
				C1218_send_ok(c1219_ptr->c1218_dll);
			}
			else{
				
				k = 0;
				c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
				c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
				c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				c1219_ptr->c1218_dll->st_08_length = k;
				
				C1218_send_err(c1219_ptr->c1218_dll);
			}
		}
		else {
			
			k = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
			c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
			c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
			c1219_ptr->c1218_dll->st_08_length = k;
		
			C1218_send_err(c1219_ptr->c1218_dll);
		}
	}
}

/**
 ******************************************************************************
 * \fn void C1219_sp_7_execute(C1219_TYPE *c1219_ptr)
 * \brief Clear Standard Status Flags
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_sp_7_execute(C1219_TYPE *c1219_ptr)
{
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + SP_7_PARAMETERS_SIZE) ) {
		
		GRAL_APP_vClear_st_3_ed_std_status1();
		GRAL_APP_vClear_st_3_ed_std_status2();
		NIC_APP_vDeassert_attention_request();
		
		uint16_t ed_std_status1 = GRAL_APP_usGet_st_3_ed_std_status1();
		uint8_t ed_std_status2 = GRAL_APP_ucGet_st_3_ed_std_status2();
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(ed_std_status1);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(ed_std_status1 >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = ed_std_status2;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_sp_8_execute(C1219_TYPE *c1219_ptr)
 * \brief Clear Manufacturer Status Flags
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_sp_8_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + SP_8_PARAMETERS_SIZE) ) {
		
		GRAL_APP_vClear_st_3_ed_mfg_status();
		NIC_APP_vDeassert_attention_request();
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		
		GRAL_APP_vCopy_st_3_ed_mfg_status(&c1219_ptr->c1218_dll->st_08_data[k]);
		k += DIM_MFG_STATUS_USED;

		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
	} 
	else {
	
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_sp_9_execute(C1219_TYPE *c1219_ptr)
 * \brief Remote reset
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_sp_9_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + SP_9_PARAMETERS_SIZE) ) {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		uint8_t curr_season = TOU_APP_ucGet_current_season();
		uint32_t timestamp = rtc.utc_timestamp;

		REGS_APP_vCopy_st_23();
		
		if ( (c1219_ptr->c1218_dll->rx_data[SP_9_ACTION_FLAG_INDEX_bm] & SP_9_SELF_READ_FLAG_bm) == SP_9_SELF_READ_FLAG_bm )
		{
			REGS_APP_vSelf_read(c1219_ptr->c1218_apl.user_id, curr_season, timestamp);
		}
		
		if ( (c1219_ptr->c1218_dll->rx_data[SP_9_ACTION_FLAG_INDEX_bm] & SP_9_DEMAND_RESET_FLAG_bm) == SP_9_DEMAND_RESET_FLAG_bm )
		{
			REGS_APP_vDemand_reset(c1219_ptr->c1218_apl.user_id, curr_season, timestamp);
			
			if ( c1219_ptr->c1218_dll->uart == OP_UART ) {
				EVNT_APP_vRegister_event(DEMAND_RESET_OCCURRED_OP, 0, false, c1219_ptr->c1218_apl.user_id, timestamp);
			}
			else if ( c1219_ptr->c1218_dll->uart == NIC_UART ) {
				EVNT_APP_vRegister_event(DEMAND_RESET_OCCURRED_NIC, 0, false, c1219_ptr->c1218_apl.user_id, timestamp);
			}
		}
		
		if ( (c1219_ptr->c1218_dll->rx_data[SP_9_ACTION_FLAG_INDEX_bm] & SP_9_SEASON_CHANGE_FLAG_bm) == SP_9_SEASON_CHANGE_FLAG_bm )
		{
			TOU_APP_vSeason_change(c1219_ptr->c1218_apl.user_id, curr_season, timestamp);
			
			curr_season = (c1219_ptr->c1218_dll->rx_data[SP_9_ACTION_FLAG_INDEX_bm] & SP_9_NEW_SEASON_bm) >> SP_9_NEW_SEASON_bp;
			TOU_APP_vTask();
			
			EVNT_APP_vRegister_event(SEASON_CHANGES, curr_season, true, c1219_ptr->c1218_apl.user_id, timestamp);
		}
		
		C1218_send_ok(c1219_ptr->c1218_dll);
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_sp_10_execute(C1219_TYPE *c1219_ptr)
 * \brief  Set Date and/or Time Procedure
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_sp_10_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	uint8_t set_mask;
	RTC_TYPE rtc_temp, rtc_backup;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + SP_10_PARAMETERS_SIZE) ) {		
		set_mask = c1219_ptr->c1218_dll->rx_data[SET_MASK_INDEX] & SET_MASK_BM;
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		
		if ( rtc.is_time_relative == 1 ) {
			
			c1219_ptr->c1218_dll->st_08_data[k++] = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = 0;
		} 
		else {
			
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(rtc.utc_timestamp);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(rtc.utc_timestamp >> 8);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(rtc.utc_timestamp >> 16);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(rtc.utc_timestamp >> 24);
		}
		
		if ( (set_mask & (SET_TIME_BM |SET_DATE_BM)) == (SET_TIME_BM |SET_DATE_BM) ) {
				
			rtc_temp.utc_timestamp = (uint32_t)c1219_ptr->c1218_dll->rx_data[TIMESTAMP_INDEX];
			rtc_temp.utc_timestamp += (uint32_t)(c1219_ptr->c1218_dll->rx_data[TIMESTAMP_INDEX + 1]) << 8;
			rtc_temp.utc_timestamp += (uint32_t)(c1219_ptr->c1218_dll->rx_data[TIMESTAMP_INDEX + 2]) << 16;
			rtc_temp.utc_timestamp += (uint32_t)(c1219_ptr->c1218_dll->rx_data[TIMESTAMP_INDEX + 3]) << 24;

			RTC_APP_vLoad_tm_zn_dst(&rtc_temp);
			rtc_temp.timestamp = rtc_temp.utc_timestamp + rtc_temp.time_zone_offset;
			RTC_APP_vBreak_timestamp(&rtc_temp, rtc_temp.timestamp);
			rtc_temp.dst_applied_flag = TOU_APP_ucLoad_dst_dates(&rtc_temp);
			rtc_temp.is_dst = 0;
			rtc_temp.dst_step = 0;
			
			if ( (rtc_temp.timestamp >= rtc_temp.dst_to_effective) && (rtc_temp.timestamp < rtc_temp.dst_from_effective) ) {
				
				rtc_temp.is_dst = 1;
				rtc_temp.dst_step = 1;
				rtc_temp.timestamp += rtc_temp.dst_time_amount;
				RTC_APP_vBreak_timestamp(&rtc_temp, rtc_temp.timestamp);
			}
			
			if ( rtc_temp.timestamp >= rtc_temp.dst_from_effective ) {
				rtc_temp.dst_step = 2;
			}
			
			rtc_temp.is_time_relative = 0;			
			
			Disable_global_interrupt();
			rtc_backup = rtc;
			rtc = rtc_temp;
			rtc.tick_count = 0;
			Enable_global_interrupt();

			/* Register Clock Sync as Event before change */
			EVNT_APP_vRegister_event(TIME_CHANGED_A, 0, true, c1219_ptr->c1218_apl.user_id, rtc_backup.utc_timestamp);
			
			if (c1219_ptr->c1218_dll->uart == OP_UART) {
				EVNT_APP_vRegister_event(TIME_CHANGED_A_OP, 0, false, c1219_ptr->c1218_apl.user_id, rtc_backup.utc_timestamp);
			}
			else if (c1219_ptr->c1218_dll->uart == NIC_UART) {
				EVNT_APP_vRegister_event(TIME_CHANGED_A_NIC, 0, false, c1219_ptr->c1218_apl.user_id, rtc_backup.utc_timestamp);
			}
			
			/* Register Clock Sync as Event after change */
			EVNT_APP_vRegister_event(TIME_CHANGED_B, 0, true, c1219_ptr->c1218_apl.user_id, rtc.utc_timestamp);
			
			if (c1219_ptr->c1218_dll->uart == OP_UART) {
				EVNT_APP_vRegister_event(TIME_CHANGED_B_OP, 0, false, c1219_ptr->c1218_apl.user_id, rtc.utc_timestamp);
			}
			else if (c1219_ptr->c1218_dll->uart == NIC_UART) {
				EVNT_APP_vRegister_event(TIME_CHANGED_B_NIC, 0, false, c1219_ptr->c1218_apl.user_id, rtc.utc_timestamp);
			}
			
			if ( (rtc_backup.is_dst == 0) && (rtc_backup.is_dst != rtc_temp.is_dst) ) {
				
				tou_os.register_dst_on = true;
				tou_os.user = c1219_ptr->c1218_apl.user_id;
				tou_os.event_timestamp = rtc.utc_timestamp;
			}
			else if ( (rtc_backup.is_dst == 1) && (rtc_backup.is_dst != rtc_temp.is_dst) ) {
				
				tou_os.register_dst_off = true;
				tou_os.user = c1219_ptr->c1218_apl.user_id;
				tou_os.event_timestamp = rtc.utc_timestamp;
			}
			
			/* Initialize Time of Use*/
			tou_os.rtc_change_mask = (SECOND_CHANGE_bm | MINUTE_CHANGE_bm | HOUR_CHANGE_bm | DAY_CHANGE_bm | MONTH_CHANGE_bm | YEAR_CHANGE_bm);
			TOU_APP_vTask();
			
			/* Start Load Profile */
			LP_APP_vSynchronize_clock(rtc_backup.utc_timestamp, rtc.utc_timestamp);	
			
			/* Time/Date External RTC Sync */
			RTC_APP_vBreak_timestamp(&rtc_temp, rtc.utc_timestamp);

			switch ( RTC_TYPE_USED ) {

				case RTC_RX8900:
					RX8900_APP_bWrite_date_time(rtc_temp.htime.second, rtc_temp.htime.minute, rtc_temp.htime.hour, 
												rtc_temp.day_of_week, rtc_temp.htime.day, rtc_temp.htime.month, rtc_temp.htime.year);
					break;

				case RTC_SAM4CM:
					rtc_set_time(RTC, rtc_temp.htime.hour, rtc_temp.htime.minute, rtc_temp.htime.second);
					rtc_set_date(RTC, rtc_temp.htime.year, rtc_temp.htime.month, rtc_temp.htime.day, rtc_temp.day_of_week);
					break;

				default:
					break;
			}		
			
		}
		
		if ( rtc.is_time_relative == 1 ) {
			c1219_ptr->c1218_dll->st_08_data[k++] = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = 0;
		} 
		else {
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(rtc.utc_timestamp);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(rtc.utc_timestamp >> 8);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(rtc.utc_timestamp >> 16);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(rtc.utc_timestamp >> 24);
		}
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_sp_10_execute(C1219_TYPE *c1219_ptr)
 * \brief Diagnostic Procedure
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_sp_11_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + SP_11_PARAMETERS_SIZE) ) {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
		SM_APP_vExecute_diagnostics_procedure();
	}
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_100_execute(C1219_TYPE *c1219_ptr)
 * \brief Change End Device to Boot Mode
 *
 * \param void
 * \retval void
 ******************************************************************************
*/
void C1219_mp_100_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_100_PARAMETERS_SIZE)) {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;

		C1218_send_ok(c1219_ptr->c1218_dll);
		
		//Set the boot origin to AA Boot mode
		FLASH_fwu_boot_mode();
	
	}
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;

		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_6_execute(C1219_TYPE *c1219_ptr)
 * \brief Change End Device Test Mode Period
 *
 * \param void
 * \retval void
 ******************************************************************************
*/
void C1219_mp_6_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_6_PARAMETERS_SIZE)) {
		
		uint8_t volatile setting_time_min = 0;
		uint32_t volatile setting_time_ms = 0;
		
		setting_time_min = ((uint32_t) c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX]);		
		setting_time_ms = (uint32_t)(setting_time_min * 60 *1000);
		meter.ed_mode.test_mode.active_period = (uint32_t)TIMER_ms_to_tic(setting_time_ms);
		
		if ( meter.ed_mode.test_mode.enabled == true ) {
			TIMER_start_ms_timer(SM_TEST_MODE_TIMER, meter.ed_mode.test_mode.active_period, SM_APP_vSet_test_mode_expired_flag);
		}
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(setting_time_min & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(setting_time_min >> 8);
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);

	}
	else {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
				
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_101_execute(C1219_TYPE *c1219_ptr)
 * \brief Change Constant Kt
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_101_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	uint32_t new_kt_value;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_101_PARAMETERS_SIZE) ) {
		
		new_kt_value = ((uint32_t) c1219_ptr->c1218_dll->rx_data[8]) << 24;
		new_kt_value |= ((uint32_t) c1219_ptr->c1218_dll->rx_data[9]) << 16;
		new_kt_value |= ((uint32_t) c1219_ptr->c1218_dll->rx_data[10]) << 8;
		new_kt_value |= ((uint32_t) c1219_ptr->c1218_dll->rx_data[11]);
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t) (proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t) (proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;

		C1218_send_ok(c1219_ptr->c1218_dll);
		
		METROLOGY_APP_vChange_kt(new_kt_value);
		
		delay_ms(500);
		SYSTEM_RESTART_TYPE mcu;
		mcu.restart_cause = KT_CONSTANT_CHANGE;
		mcu.save_information = true;
		RST_APP_vExecute_mcu_reset(mcu);
	}
	else {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_102_execute(C1219_TYPE *c1219_ptr)
 * \brief Change Energy Pulse Mode
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_102_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
		
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_102_PARAMETERS_SIZE) ) {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;

		switch ( c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX] ) {

			case MP_102_KWH_MODE:
				METROLOGY_APP_vEnable_P_energy_pulse_mode();
				c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
				C1218_send_ok(c1219_ptr->c1218_dll);
				break;

			case MP_102_KVARH_MODE:
				METROLOGY_APP_vEnable_Q_energy_pulse_mode();
				c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
				C1218_send_ok(c1219_ptr->c1218_dll);
				break;
			
			default:
				c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
				C1218_send_err(c1219_ptr->c1218_dll);
				break;
		}
	
		c1219_ptr->c1218_dll->st_08_length = k;
	}
	else {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_110_execute(C1219_TYPE *c1219_ptr)
 * \brief Change Calibration Constant
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_110_execute(C1219_TYPE *c1219_ptr) {

	uint8_t k;
	uint32_t proc_input = 0;
	uint8_t id_value;
	
	if ( ( c1219_ptr->c1218_dll->rx_data_bytes >= (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_110_PARAMETERS_MIN_SIZE ) )    &&
	     ( c1219_ptr->c1218_dll->rx_data_bytes <= (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_110_PARAMETERS_MAX_SIZE ) ) )  {	

		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t) (proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t) (proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_ACCEPTED_NOT_COMPLETED;		
		c1219_ptr->c1218_dll->st_08_length = k;
		
		id_value = (uint8_t)(c1219_ptr->c1218_dll->rx_data[CALIBRATION_CONSTANT_SELECT_INDEX]);
		
		proc_input  = (uint32_t)(c1219_ptr->c1218_dll->rx_data[CALIBRATION_CONSTANT_VALUE_INDEX + 0] << 0);
		proc_input |= (uint32_t)(c1219_ptr->c1218_dll->rx_data[CALIBRATION_CONSTANT_VALUE_INDEX + 1] << 8);
		proc_input |= (uint32_t)(c1219_ptr->c1218_dll->rx_data[CALIBRATION_CONSTANT_VALUE_INDEX + 2] << 16);
		proc_input |= (uint32_t)(c1219_ptr->c1218_dll->rx_data[CALIBRATION_CONSTANT_VALUE_INDEX + 3] << 24);
		
		switch ( id_value ) {
				
			case ADJUST_M_IA_ID:
				
				if ( NBR_PHASES >= 1 ) {

					if ( METROLOGY_APP_bWrite_cal_m_ia(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_M_IA_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;

			case ADJUST_M_VA_ID:
			
				if ( NBR_PHASES >= 1 ) {

					if ( METROLOGY_APP_bWrite_cal_m_va(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_M_VA_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;

			case ADJUST_PH_IA_ID:
			
				if ( NBR_PHASES >= 1 ) {

					if ( METROLOGY_APP_bWrite_cal_ph_ia(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader( DSP_CTRL_CAL_PH_IA_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;

			case AUTO_ADJUST_M_IA_ID:

				if ( NBR_PHASES >= 1 ) {

					if ( METROLOGY_APP_bCalibrate_m_ia(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_M_IA_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;

			case AUTO_ADJUST_M_VA_ID:

				if ( NBR_PHASES >= 1 ) {

					if ( METROLOGY_APP_bCalibrate_m_va(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_M_VA_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;

			case AUTO_ADJUST_PH_IA_ID:

				if ( NBR_PHASES >= 1 ) {

					if ( METROLOGY_APP_bCalibrate_ph_ia(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_PH_IA_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;
				
			case ADJUST_M_IB_ID:
				
				if ( NBR_PHASES >= 2 ) {

					if ( METROLOGY_APP_bWrite_cal_m_ib(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_M_IB_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;
				
			case ADJUST_M_VB_ID:

				if ( NBR_PHASES >= 2 ) {

					if ( METROLOGY_APP_bWrite_cal_m_vb(proc_input) ) {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_M_VB_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;

			case ADJUST_PH_IB_ID:
			
				if ( NBR_PHASES >= 2 ) {

					if ( METROLOGY_APP_bWrite_cal_ph_ib(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_PH_IB_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;

			case AUTO_ADJUST_M_IB_ID:

				if ( NBR_PHASES >= 2 ) {

					if ( METROLOGY_APP_bCalibrate_m_ib(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_M_IB_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;

			case AUTO_ADJUST_M_VB_ID:

				if ( NBR_PHASES >= 2 ) {

					if ( METROLOGY_APP_bCalibrate_m_vb(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_M_VB_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;

			case AUTO_ADJUST_PH_IB_ID:

				if ( NBR_PHASES >= 2 ) {

					if ( METROLOGY_APP_bCalibrate_ph_ib(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_PH_IB_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;
			
			case ADJUST_M_IC_ID:
			
				if ( NBR_PHASES >= 3 ) {

					if ( METROLOGY_APP_bWrite_cal_m_ic(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_M_IC_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;
				
			case ADJUST_M_VC_ID:

				if ( NBR_PHASES >= 3 ) {

					if ( METROLOGY_APP_bWrite_cal_m_vc(proc_input) ) {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_M_VC_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;
			
			case ADJUST_PH_IC_ID:

				if ( NBR_PHASES >= 3 ) {

					if ( METROLOGY_APP_bWrite_cal_ph_ic(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_PH_IC_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;

			case AUTO_ADJUST_M_IC_ID:

				if ( NBR_PHASES >= 3 ) {

					if ( METROLOGY_APP_bCalibrate_m_ic(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_M_IC_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;

			case AUTO_ADJUST_M_VC_ID:

				if ( NBR_PHASES >= 3 ) {

					if ( METROLOGY_APP_bCalibrate_m_vc(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_M_VC_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;

			case AUTO_ADJUST_PH_IC_ID:

				if ( NBR_PHASES >= 3 ) {

					if ( METROLOGY_APP_bCalibrate_ph_ic(proc_input) ) {

						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
						EEPROM_reader(DSP_CTRL_CAL_PH_IC_ADDR, (uint8_t*)(&c1219_ptr->c1218_dll->st_08_data[k]), 4);
						k += 4;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_ok(c1219_ptr->c1218_dll);
					}
					else {
						k = 3;
						c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
						c1219_ptr->c1218_dll->st_08_length = k;
						C1218_send_err(c1219_ptr->c1218_dll);
					}
				}
				else {
					
					k = 3;
					c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
					c1219_ptr->c1218_dll->st_08_length = k;
					C1218_send_err(c1219_ptr->c1218_dll);
				}
				break;
				
			default:
				break;
		}
	}
	else
	{
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_111_execute(C1219_TYPE *c1219_ptr)
 * \brief Back up Calibration Constants
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_111_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_111_PARAMETERS_SIZE) ) {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;

		#if NBR_PHASES >= 1
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bBack_up_cal_m_ia();
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bBack_up_cal_m_va();
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bBack_up_cal_ph_ia();
		#endif

		#if NBR_PHASES >= 2
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bBack_up_cal_m_ib();
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bBack_up_cal_m_vb();
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bBack_up_cal_ph_ib();
		#endif

		#if NBR_PHASES >= 3
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bBack_up_cal_m_ic();
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bBack_up_cal_m_vc();
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bBack_up_cal_ph_ic();
		#endif

		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_112_execute(C1219_TYPE *c1219_ptr)
 * \brief Restore Calibration Constants
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_112_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_112_PARAMETERS_SIZE) ) {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;

		#if NBR_PHASES >= 1
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bRestore_cal_m_ia_backup();
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bRestore_cal_m_va_backup();
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bRestore_cal_ph_ia_backup();
		#endif

		#if NBR_PHASES >= 2										   
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bRestore_cal_m_ib_backup();
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bRestore_cal_m_vb_backup();
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bRestore_cal_ph_ib_backup();
		#endif

		#if NBR_PHASES >= 3								   
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bRestore_cal_m_ic_backup();
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bRestore_cal_m_vc_backup();
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bRestore_cal_ph_ic_backup();
		#endif

		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
	}
	else {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_113_execute(C1219_TYPE *c1219_ptr)
 * \brief Load Calibration Constants Default Values
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_113_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_113_PARAMETERS_SIZE) ) {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;

		#if NBR_PHASES >= 1
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bWrite_cal_m_ia(0x20000000);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bWrite_cal_m_va(0x20000000);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bWrite_cal_ph_ia(0);
		#endif

		#if NBR_PHASES >= 2
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bWrite_cal_m_ib(0x20000000);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bWrite_cal_m_vb(0x20000000);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bWrite_cal_ph_ib(0);
		#endif

		#if NBR_PHASES >= 3
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bWrite_cal_m_ic(0x20000000);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bWrite_cal_m_vc(0x20000000);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)METROLOGY_APP_bWrite_cal_ph_ic(0);
		#endif

		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);	
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_120_execute(C1219_TYPE *c1219_ptr)
 * \brief Back up Configuration Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_120_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	bool is_ok = false;
	uint16_t table_selected;
	uint16_t backup_area_id;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_120_PARAMETERS_SIZE) ) {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_ACCEPTED_NOT_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		table_selected  = ( c1219_ptr->c1218_dll->rx_data[TABLE_ID_INDEX + 0] << 0 );
		table_selected |= ( c1219_ptr->c1218_dll->rx_data[TABLE_ID_INDEX + 1] << 8 );
		backup_area_id  = c1219_ptr->c1218_dll->rx_data[TABLE_ID_INDEX + 2];

		uint8_t table_data[600];
		
		switch (table_selected) {
			
			case ST_1_ID:
				is_ok = SM_APP_bBack_up_st_1(backup_area_id);
				break;
			
			case ST_6_ID:
				is_ok = SM_APP_bBack_up_st_6(backup_area_id);
				break;
			
			case ST_11_ID:
				is_ok = SM_APP_bBack_up_st_11(backup_area_id);
				break;
			
			case ST_13_ID:
				is_ok = SM_APP_bBack_up_st_13(backup_area_id);
				break;
			
			case ST_21_ID:
				is_ok = SM_APP_bBack_up_st_21(backup_area_id);
				break;
			
			case ST_22_ID:
				is_ok = SM_APP_bBack_up_st_22(backup_area_id);
				break;
			
			case ST_27_ID:
				is_ok = SM_APP_bBack_up_st_27(backup_area_id);
				break;
			
			case ST_41_ID:
				is_ok = SM_APP_bBack_up_st_41(backup_area_id);
				break;
			
			case ST_42_ID:
				is_ok = SM_APP_bBack_up_st_42(backup_area_id);
				break;
			
			case ST_43_ID:
				is_ok = SM_APP_bBack_up_st_43(backup_area_id);
				break;
			
			case ST_44_ID:
				is_ok = SM_APP_bBack_up_st_44(backup_area_id);
				break;
			
			case ST_51_ID:
				is_ok = SM_APP_bBack_up_st_51(backup_area_id);
				break;
			
			case ST_53_ID:
				is_ok = SM_APP_bBack_up_st_53(backup_area_id);				
				break;

			case ST_54_ID:
				is_ok = SM_APP_bBack_up_st_54(backup_area_id);
				break;
			
			case ST_61_ID:
				is_ok = SM_APP_bBack_up_st_61(backup_area_id);
				break;
			
			case ST_62_ID:
				is_ok = SM_APP_bBack_up_st_62(backup_area_id);
				break;
			
			case ST_71_ID:
				is_ok = SM_APP_bBack_up_st_71(backup_area_id);
				break;
			
			case ST_73_ID:
				is_ok = SM_APP_bBack_up_st_73(backup_area_id);
				break;
			
			case ST_75_ID:
				is_ok = SM_APP_bBack_up_st_75(backup_area_id);
				break;
			
			case MT_1_ID:
				is_ok = SM_APP_bBack_up_mt_1(backup_area_id);
				break;

			case MT_3_ID:
				is_ok = SM_APP_bBack_up_mt_3(backup_area_id);
				break;
			
			case MT_23_ID:
				is_ok = SM_APP_bBack_up_mt_23(backup_area_id);
				break;
			
			case MT_53_ID:
				is_ok = SM_APP_bBack_up_mt_53(backup_area_id);
				break;
			
			case MT_73_ID:
				is_ok = SM_APP_bBack_up_mt_73(backup_area_id);
				break;
			
			default:
				break;
		}
		
		if (is_ok) {
			
			k = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
			c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
			c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
			c1219_ptr->c1218_dll->st_08_length = k;

			C1218_send_ok(c1219_ptr->c1218_dll);
		}
		else {
			
			k = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
			c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
			c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
			c1219_ptr->c1218_dll->st_08_length = k;

			C1218_send_err(c1219_ptr->c1218_dll);
		}
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_121_execute(C1219_TYPE *c1219_ptr)
 * \brief Restore Configuration Table
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_121_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	bool is_ok;
	uint16_t table_selected;
	uint16_t backup_area_id;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_121_PARAMETERS_SIZE) ) {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_ACCEPTED_NOT_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		table_selected  = ( c1219_ptr->c1218_dll->rx_data[TABLE_ID_INDEX + 0] << 0 );
		table_selected |= ( c1219_ptr->c1218_dll->rx_data[TABLE_ID_INDEX + 1] << 8 );
		backup_area_id  = c1219_ptr->c1218_dll->rx_data[TABLE_ID_INDEX + 2];

		uint8_t table_data[600];
		uint16_t backup_crc = 0, calc_crc = 1;
		uint16_t table_size = 0;
		
		switch (table_selected) {
			
			case ST_1_ID:
				SM_APP_bRestore_st_1_from_backup(backup_area_id);
				break;
			
			case ST_6_ID:
				SM_APP_bRestore_st_6_from_backup(backup_area_id);
				break;
			
			case ST_11_ID:
				SM_APP_bRestore_st_11_from_backup(backup_area_id);
				break;
			
			case ST_13_ID:
				SM_APP_bRestore_st_13_from_backup(backup_area_id);
				break;
			
			case ST_21_ID:
				SM_APP_bRestore_st_21_from_backup(backup_area_id);
				break;
			
			case ST_22_ID:
				SM_APP_bRestore_st_22_from_backup(backup_area_id);
				break;
			
			case ST_27_ID:
				SM_APP_bRestore_st_27_from_backup(backup_area_id);
				break;
			
			case ST_41_ID:
				SM_APP_bRestore_st_41_from_backup(backup_area_id);
				break;
			
			case ST_42_ID:
				SM_APP_bRestore_st_42_from_backup(backup_area_id);
				break;
			
			case ST_43_ID:
				SM_APP_bRestore_st_43_from_backup(backup_area_id);
				break;
			
			case ST_44_ID:
				SM_APP_bRestore_st_44_from_backup(backup_area_id);
				break;
			
			case ST_51_ID:
				SM_APP_bRestore_st_51_from_backup(backup_area_id);
				break;
			
			case ST_53_ID:
				SM_APP_bRestore_st_53_from_backup(backup_area_id);
				break;

			case ST_54_ID:
				SM_APP_bRestore_st_54_from_backup(backup_area_id);
				break;
			
			case ST_61_ID:
				SM_APP_bRestore_st_61_from_backup(backup_area_id);
				break;
			
			case ST_62_ID:
				SM_APP_bRestore_st_62_from_backup(backup_area_id);
				break;
			
			case ST_71_ID:
				SM_APP_bRestore_st_71_from_backup(backup_area_id);
				break;
			
			case ST_73_ID:
				SM_APP_bRestore_st_73_from_backup(backup_area_id);
				break;
			
			case ST_75_ID:
				SM_APP_bRestore_st_75_from_backup(backup_area_id);
				break;
			
			case MT_1_ID:
				SM_APP_bRestore_mt_1_from_backup(backup_area_id);
				break;

			case MT_3_ID:
				SM_APP_bRestore_mt_3_from_backup(backup_area_id);
				break;
			
			case MT_23_ID:
				SM_APP_bRestore_mt_23_from_backup(backup_area_id);
				break;
			
			case MT_53_ID:
				SM_APP_bRestore_mt_53_from_backup(backup_area_id);
				break;
			
			case MT_73_ID:
				SM_APP_bRestore_mt_73_from_backup(backup_area_id);
				break;
			
			default:
				is_ok = false;
				C1218_send_err(c1219_ptr->c1218_dll);
				break;
		}
		
		if (is_ok) {
			
			k = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
			c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
			c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
			c1219_ptr->c1218_dll->st_08_length = k;

			C1218_send_ok(c1219_ptr->c1218_dll);
		}
		else {
			
			k = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
			c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
			c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
			c1219_ptr->c1218_dll->st_08_length = k;

			C1218_send_err(c1219_ptr->c1218_dll);
		}
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_122_execute(C1219_TYPE *c1219_ptr)
 * \brief Load Configuration Table Default Values
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_122_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	bool is_ok;
	uint16_t table_selected;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_122_PARAMETERS_SIZE) ) {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_ACCEPTED_NOT_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		table_selected  = ( c1219_ptr->c1218_dll->rx_data[TABLE_ID_INDEX + 0] << 0 );
		table_selected |= ( c1219_ptr->c1218_dll->rx_data[TABLE_ID_INDEX + 1] << 8 );
		
		switch (table_selected) {
			
			case ST_1_ID:
				GRAL_APP_bSt_1_restore_default_values();
				is_ok = true;
				break;
				
			case ST_3_ID:
				GRAL_APP_bSt_3_restore_default_values();
				is_ok = true;
				break;
				
			case ST_6_ID:
				GRAL_APP_bSt_6_restore_default_values();
				is_ok = true;
				break;
			
			case ST_11_ID:
				REGS_APP_bSt_11_restore_default_values();
				is_ok = true;
				break;
				
			case ST_13_ID:
				REGS_APP_bSt_13_restore_default_values();
				is_ok = true;
				break;
				
			case ST_21_ID:
				REGS_APP_bSt_21_restore_default_values();
				is_ok = true;
				break;
			
			case ST_22_ID:
				REGS_APP_bSt_22_restore_default_values();
				is_ok = true;
				break;
				
			case ST_23_ID:
				REGS_APP_vClear_st_23();
				is_ok = true;
				break;
				
			case ST_24_ID:
				REGS_APP_vClear_st_24();
				is_ok = true;
				break;
				
			case ST_25_ID:
				REGS_APP_vClear_st_25();
				is_ok = true;
				break;
				
			case ST_26_ID:
				REGS_APP_vClear_st_26();
				is_ok = true;
				break;
				
			case ST_27_ID:
				REGS_APP_bSt_27_restore_default_values();
				is_ok = true;
				break;
			
			case ST_41_ID:
				SECR_APP_bSt_41_restore_default_values();
				is_ok = true;
				break;
				
			case ST_42_ID:
				SECR_APP_bSt_42_restore_default_values();
				is_ok = true;
				break;
				
			case ST_43_ID:
				SECR_APP_bSt_43_restore_default_values();
				is_ok = true;
				break;
			
			case ST_44_ID:
				SECR_APP_bSt_44_restore_default_values();
				is_ok = true;
				break;
				
			case ST_51_ID:
				TOU_APP_bSt_51_restore_default_values();
				is_ok = true;
				break;
				
			case ST_53_ID:
				TOU_APP_bSt_53_restore_default_values();
				is_ok = true;
				break;
				
			case ST_61_ID:
				LP_APP_bSt_61_restore_default_values();
				is_ok = true;
				break;
				
			case ST_62_ID:
				LP_APP_bSt_62_restore_default_values();
				is_ok = true;
				break;
				
			case ST_63_ID:
				LP_APP_bSt_63_restore_default_values();
				is_ok = true;
				break;
				
			case ST_71_ID:
				EVNT_APP_bSt_71_restore_default_values();
				is_ok = true;
				break;
				
			case ST_73_ID:
				EVNT_APP_bSt_73_restore_default_values();
				is_ok = true;
				break;
				
			case ST_74_ID:
				EVNT_APP_vClear_st_74(c1219_ptr->c1218_apl.user_id);
				is_ok = true;
				break;
				
			case ST_75_ID:
				EVNT_APP_bSt_75_restore_default_values();
				is_ok = true;
				break;
				
			case ST_76_ID:
				EVNT_APP_vClear_st_76(c1219_ptr->c1218_apl.user_id);
				is_ok = true;
				break;
				
			case MT_1_ID:
				SM_APP_vMt_1_restore_default_values();
				is_ok = true;
				break;
				
			case MT_23_ID:
				SM_APP_vMt_23_restore_default_values();
				is_ok = true;
				break;
				
			case MT_4_ID:
				SM_APP_vMt_4_restore_default_values();
				is_ok = true;
				break;
				
			case MT_53_ID:
				SM_APP_vMt_53_restore_default_values();
				is_ok = true;
				break;
				
			case MT_73_ID:
				EVNT_APP_bMt_73_restore_default_values();
				is_ok = true;
				break;	
				
			default:
				is_ok = false;
				break;
		}
		
		if (is_ok) {
			
			k = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
			c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
			c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
			c1219_ptr->c1218_dll->st_08_length = k;
		} 
		else {
			
			k = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
			c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
			c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
			c1219_ptr->c1218_dll->st_08_length = k;
		}
		
		C1218_send_ok(c1219_ptr->c1218_dll);
	}
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_123_execute(C1219_TYPE *c1219_ptr)
 * \brief Back up All Configuration Tables
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_123_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_123_PARAMETERS_SIZE) ) {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_ACCEPTED_NOT_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;

		C1218_send_ok(c1219_ptr->c1218_dll);

		uint8_t backup_area_id  = c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX];
		uint8_t current_tries;
		bool backup_ok;
		uint8_t j = 0;

		bool (*back_up_function[30])(uint16_t);
		uint8_t backup_result[30];

		memset( &backup_result, 0, sizeof(backup_result) );

		back_up_function[j++] = SM_APP_bBack_up_st_1;
		back_up_function[j++] = SM_APP_bBack_up_st_6;
		back_up_function[j++] = SM_APP_bBack_up_st_11;
		back_up_function[j++] = SM_APP_bBack_up_st_13;
		back_up_function[j++] = SM_APP_bBack_up_st_21;
		back_up_function[j++] = SM_APP_bBack_up_st_22;
		back_up_function[j++] = SM_APP_bBack_up_st_27;
		back_up_function[j++] = SM_APP_bBack_up_st_41;
		back_up_function[j++] = SM_APP_bBack_up_st_42;
		back_up_function[j++] = SM_APP_bBack_up_st_43;
		back_up_function[j++] = SM_APP_bBack_up_st_44;
		back_up_function[j++] = SM_APP_bBack_up_st_51;
		back_up_function[j++] = SM_APP_bBack_up_st_53;
		back_up_function[j++] = SM_APP_bBack_up_st_54;
		back_up_function[j++] = SM_APP_bBack_up_st_61;
		back_up_function[j++] = SM_APP_bBack_up_st_62;
		back_up_function[j++] = SM_APP_bBack_up_st_71;
		back_up_function[j++] = SM_APP_bBack_up_st_73;
		back_up_function[j++] = SM_APP_bBack_up_st_75;
		back_up_function[j++] = SM_APP_bBack_up_mt_1;
		back_up_function[j++] = SM_APP_bBack_up_mt_3;
		back_up_function[j++] = SM_APP_bBack_up_mt_23;
		back_up_function[j++] = SM_APP_bBack_up_mt_53;
		back_up_function[j++] = SM_APP_bBack_up_mt_73;
		
		for ( uint8_t i = 0; i < j; i++) {
			
			current_tries = 0;
			backup_ok = false;

			while( (!backup_ok) && (current_tries < 3) ) {

				backup_ok = back_up_function[i](backup_area_id);
				backup_result[i] = backup_ok ? 1 : 0;
				current_tries++;

				NIC_task();
				WATCHDOG_APP_vRestart();
				
				OP_task();
				WATCHDOG_APP_vRestart();

				PFAIL_APP_vTask();
				WATCHDOG_APP_vRestart();
			}
		}
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;

		memcpy( &c1219_ptr->c1218_dll->st_08_data[k], &backup_result[0], j);
		k += j;

		c1219_ptr->c1218_dll->st_08_length = k;	
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_124_execute(C1219_TYPE *c1219_ptr)
 * \brief Restore All Configuration Tables
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_124_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_124_PARAMETERS_SIZE) ) {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_ACCEPTED_NOT_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;		
		
		C1218_send_ok(c1219_ptr->c1218_dll);

		uint8_t backup_area_id  = c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX];
		uint8_t current_tries;
		bool backup_ok;
		uint8_t j = 0;

		bool (*restore_function[30])(uint16_t);
		uint8_t backup_result[30];

		memset( &backup_result, 0, sizeof(backup_result) );

		meter.system_state.changing_configuration = true;

		restore_function[j++] = SM_APP_bRestore_st_1_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_6_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_11_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_13_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_21_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_22_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_27_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_41_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_42_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_43_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_44_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_51_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_53_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_54_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_61_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_62_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_71_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_73_from_backup;
		restore_function[j++] = SM_APP_bRestore_st_75_from_backup;
		restore_function[j++] = SM_APP_bRestore_mt_1_from_backup;
		restore_function[j++] = SM_APP_bRestore_mt_3_from_backup;
		restore_function[j++] = SM_APP_bRestore_mt_23_from_backup;
		restore_function[j++] = SM_APP_bRestore_mt_53_from_backup;
		restore_function[j++] = SM_APP_bRestore_mt_73_from_backup;
		
		for ( uint8_t i = 0; i < j; i++) {
			
			current_tries = 0;
			backup_ok = false;

			while( (!backup_ok) && (current_tries < 3) ) {

				backup_ok = restore_function[i](backup_area_id);
				backup_result[i] = backup_ok ? 1 : 0;
				current_tries++;

				NIC_task();
				WATCHDOG_APP_vRestart();
				
				OP_task();
				WATCHDOG_APP_vRestart();

				PFAIL_APP_vTask();
				WATCHDOG_APP_vRestart();
			}
		}

		GRAL_APP_bSt_3_restore_default_values();
		NIC_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();
		OP_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();

		REGS_APP_vClear_st_23();
		NIC_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();
		OP_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();

		REGS_APP_vClear_st_24();
		NIC_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();
		OP_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();

		REGS_APP_vClear_st_25();
		NIC_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();
		OP_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();

		REGS_APP_vClear_st_26();
		NIC_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();
		OP_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();

		LP_APP_bSt_63_restore_default_values();
		NIC_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();
		OP_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();

		EVNT_APP_vClear_st_74(c1219_ptr->c1218_apl.user_id);
		NIC_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();
		OP_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();

		EVNT_APP_vClear_st_76(c1219_ptr->c1218_apl.user_id);
		NIC_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();
		OP_task();
		PFAIL_APP_vTask();
		WATCHDOG_APP_vRestart();

		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		memcpy( &c1219_ptr->c1218_dll->st_08_data[k], &backup_result[0], j);
		k += j;
		c1219_ptr->c1218_dll->st_08_length = k;

		while ( (op_c1218_apl.communication_state != BASE_STATE) || (nic_c1218_apl.communication_state != BASE_STATE) ) {
			
			NIC_task();
			PFAIL_APP_vTask();
			WATCHDOG_APP_vRestart();

			OP_task();
			PFAIL_APP_vTask();
			WATCHDOG_APP_vRestart();
		}
		
		SYSTEM_RESTART_TYPE mcu;
		mcu.restart_cause = COLD_START;
		mcu.save_information = false;
		RST_APP_vExecute_mcu_reset(mcu);
		
		
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_125_execute(C1219_TYPE *c1219_ptr)
 * \brief Load All Configuration Tables Default Values
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_125_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_125_PARAMETERS_SIZE) ) {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_ACCEPTED_NOT_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		/////////

		C1218_send_ok(c1219_ptr->c1218_dll);
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;	
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_126_execute(C1219_TYPE *c1219_ptr)
 * \brief Erase External Memory Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_126_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_126_PARAMETERS_SIZE) ) {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_ACCEPTED_NOT_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
		
		meter.system_state.busy = true;
		meter.system_state.changing_configuration = true;

		// Erase I2C memory
		EEPROM_erase_all();
		
		// Load mm100 C1219 Tables
		SM_APP_vLoad_c1219_tables();
		
		RST_HAL_vExecute_software_reset();
		
		meter.system_state.busy = false;
		meter.system_state.changing_configuration = false;
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;	
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_130_execute(C1219_TYPE *c1219_ptr)
 * \brief Test I2C EEPROM Memories
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_130_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k;
	uint8_t eeprom_ok;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_130_PARAMETERS_SIZE ) ) {
		
		eeprom_ok = SM_APP_ucGet_nonvol_mem_state();	
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t) (proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t) (proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_data[k++] = eeprom_ok;
		c1219_ptr->c1218_dll->st_08_length = k;

		C1218_send_ok(c1219_ptr->c1218_dll);
	}
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
				
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_103_execute(C1219_TYPE *c1219_ptr)
 * \brief Change Voltage Level Threshold
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_103_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_103_PARAMETERS_SIZE) ) {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		meter.voltage_lower_limit  = (uint32_t)(c1219_ptr->c1218_dll->rx_data[SET_LOW_VOLTAGE_INDEX + 0]);
		meter.voltage_lower_limit |= (uint32_t)(c1219_ptr->c1218_dll->rx_data[SET_LOW_VOLTAGE_INDEX + 1] << 8);
		meter.voltage_lower_limit |= (uint32_t)(c1219_ptr->c1218_dll->rx_data[SET_LOW_VOLTAGE_INDEX + 2] << 16);
		meter.voltage_lower_limit |= (uint32_t)(c1219_ptr->c1218_dll->rx_data[SET_LOW_VOLTAGE_INDEX + 3] << 24);
		
		meter.voltage_upper_limit  = (uint32_t)(c1219_ptr->c1218_dll->rx_data[SET_HIGH_VOLTAGE_INDEX + 0]);
		meter.voltage_upper_limit |= (uint32_t)(c1219_ptr->c1218_dll->rx_data[SET_HIGH_VOLTAGE_INDEX + 1] << 8);
		meter.voltage_upper_limit |= (uint32_t)(c1219_ptr->c1218_dll->rx_data[SET_HIGH_VOLTAGE_INDEX + 2] << 16);
		meter.voltage_upper_limit |= (uint32_t)(c1219_ptr->c1218_dll->rx_data[SET_HIGH_VOLTAGE_INDEX + 3] << 24);
		
		SM_APP_vMt_23_write_eeprom_values();
		
		C1218_send_ok(c1219_ptr->c1218_dll);
	}
	else {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_104_execute(C1219_TYPE *c1219_ptr)
 * \brief Change Time Level Threshold for Voltage Monitor
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_104_execute(C1219_TYPE *c1219_ptr) {
		
	uint8_t k = 0;

	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_104_PARAMETERS_SIZE) ) {
		
		uint16_t setting_time_sec = 0;
		
		setting_time_sec = (uint16_t)c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX];
		setting_time_sec |= (uint16_t)c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX +1] << 8;
		
		meter.detect_voltage_change_period = (uint32_t)TIMER_ms_to_tic(setting_time_sec * 1000);
					
		TIMER_reset_ms_timer(SM_PHA_VOLTAGE_MONITOR_TIMER);
		TIMER_reset_ms_timer(SM_PHB_VOLTAGE_MONITOR_TIMER);
		TIMER_reset_ms_timer(SM_PHC_VOLTAGE_MONITOR_TIMER);
						
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(setting_time_sec & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(setting_time_sec >> 8);
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
	} 
	else {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_105_execute(C1219_TYPE *c1219_ptr)
 * \brief Configuration Mode
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_105_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_105_PARAMETERS_SIZE) ) {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);

		uint8_t configuration_mode_enabled = c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX];

		switch ( configuration_mode_enabled ) {
			
			case MP_105_DISABLE_CONF_MODE:

				if ( meter.system_state.changing_configuration ) {

					while ( (op_c1218_apl.communication_state != BASE_STATE) || (nic_c1218_apl.communication_state != BASE_STATE) ) {
						
						NIC_task();
						PFAIL_APP_vTask();
						WATCHDOG_APP_vRestart();

						OP_task();
						PFAIL_APP_vTask();
						WATCHDOG_APP_vRestart();
					}
					
					SYSTEM_RESTART_TYPE mcu;
					mcu.restart_cause = COLD_START;
					mcu.save_information = false;
					RST_APP_vExecute_mcu_reset(mcu);
				}
				break;

			case MP_105_ENABLE_CONF_MODE:
				meter.system_state.changing_configuration = true;
				break;

			default:
				break;
		}
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}


/**
 ******************************************************************************
 * \fn void C1219_mp_9_execute(C1219_TYPE *c1219_ptr)
 * \brief Reset Energy
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_9_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_102_PARAMETERS_SIZE) ) {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_ACCEPTED_NOT_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
		
		if ( !meter.ed_mode.test_mode.enabled ) {
			REGS_APP_vReset_summations();
		}
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;	
	} 
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_200_execute(C1219_TYPE *c1219_ptr)
 * \brief Image Initiation Process
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_200_execute(C1219_TYPE *c1219_ptr)
{ 
	volatile uint8_t k;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_200_PARAMETERS_SIZE)) {
		
		ota_image.type = c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX];
		ota_image.size = (uint32_t)c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 1];
		ota_image.size |= ((uint32_t)c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 2]) << 8;
		ota_image.size |= ((uint32_t)c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 3]) << 16;
		ota_image.size |= ((uint32_t)c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 4]) << 24;
		memcpy(&ota_image.part_number, &c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 5], 8 );
		ota_image.new_version = c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 13];
		ota_image.new_revision = c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 14];
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_ACCEPTED_NOT_COMPLETED;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_ok(c1219_ptr->c1218_dll);
				
	    ota_image.memory_busy = 1;
	    
		FLASH_erase_image_sector();
	    
	    ota_image.memory_busy = 0;
	    
	    k = 0;
	    c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
	    c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
	    c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
	    c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
	    c1219_ptr->c1218_dll->st_08_length = k;
	    
	    ota_image.initiation = 1;
	    ota_image.activation = 0;
	}
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/**
 ******************************************************************************
 * \fn void C1219_mp_201_execute(C1219_TYPE *c1219_ptr)
 * \brief Image Activation Process
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_201_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	static volatile uint32_t crc_1, crc_2;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_201_PARAMETERS_SIZE)) {
		
		if ( (ota_image.initiation == 1) && (ota_image.type == c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX]) ) {
			
			ota_image.type = c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX];
			ota_image.size = (uint32_t)c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 1];
			ota_image.size |= ((uint32_t)c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 2]) << 8;
			ota_image.size |= ((uint32_t)c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 3]) << 16;
			ota_image.size |= ((uint32_t)c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 4]) << 24;
			memcpy(&ota_image.part_number, &c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 5], 8 );
			ota_image.new_version = c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 13];
			ota_image.new_revision = c1219_ptr->c1218_dll->rx_data[PROCEDURE_PARAMETER_INDEX + 14];
			
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
			c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
			c1219_ptr->c1218_dll->st_08_data[k++] = PROC_ACCEPTED_NOT_COMPLETED;
			c1219_ptr->c1218_dll->st_08_length = k;
				
			C1218_send_ok(c1219_ptr->c1218_dll);
	
			crc_1 = CRC_image_crc16_calc();
			crc_2 = crc_1 & 0x0000FFFF;
			crc_1 >>= 16;
	
			if ( crc_1 != crc_2 ) {
				
				k = 0;
				c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
				c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
				c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
				c1219_ptr->c1218_dll->st_08_data[k++] = PROC_CONFLICT_WITH_SET_UP;
				c1219_ptr->c1218_dll->st_08_length = k;
			}
			else {
				
				ota_image.memory_busy = 0;
				
				k = 0;
				c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
				c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
				c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
				c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
				c1219_ptr->c1218_dll->st_08_length = k;
				
				FLASH_boot_origin(0x55);
				
				ota_image.activation = 1;
				
				FLASH_fwu_reset();
				
		    }
			ota_image.initiation = 0;
			ota_image.type = 0;
			ota_image.size = 0;
			ota_image.memory_busy = 0;
		}
		else {
			
			k = 0;
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
			c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
			c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
			c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
			c1219_ptr->c1218_dll->st_08_length = k;
			
			C1218_send_err(c1219_ptr->c1218_dll);
		}
	}
	else {
		
		k = 0;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	} 
}

/**
 ******************************************************************************
 * \fn void C1219_mp_150_execute(C1219_TYPE *c1219_ptr)
 * \brief Flash Image CRC Calculation
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void C1219_mp_150_execute(C1219_TYPE *c1219_ptr) {
	
	uint8_t k = 0;
	static volatile uint32_t crc;
	
	if ( c1219_ptr->c1218_dll->rx_data_bytes == (C1218_FULL_WRITE_OVERHEAD_SIZE + PROCEDURE_OVERHEAD_SIZE + MP_150_PARAMETERS_SIZE) ) {
	
		crc = CRC_app_crc16_calc();
	
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = PROC_COMPLETED;
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(crc >> 16);		// LSB Calculated CRC
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(crc >> 24);		// MSB Calculated CRC
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(crc >> 0);		// LSB Flash CRC
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(crc >> 8);		// MSB Flash CRC
		c1219_ptr->c1218_dll->st_08_length = k;
	
		C1218_send_ok(c1219_ptr->c1218_dll);
	}
	else {
		
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id & 0x00FF);
		c1219_ptr->c1218_dll->st_08_data[k++] = (uint8_t)(proc_id >> 8);
		c1219_ptr->c1218_dll->st_08_data[k++] = proc_seq_nbr;
		c1219_ptr->c1218_dll->st_08_data[k++] = INVALID_PARAMETER;
		c1219_ptr->c1218_dll->st_08_length = k;
		
		C1218_send_err(c1219_ptr->c1218_dll);
	}
}

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */