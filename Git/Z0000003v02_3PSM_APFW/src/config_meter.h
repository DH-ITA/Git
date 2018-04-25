/**
 ******************************************************************************
 * \file config_meter.h
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
 *   - Format: F_P06-DP_14
 *   - Revision: 02
 *   - Date: 16/MAY/2017
 *   - Author: Ernesto Cocoletzi.
 *   - Aprove: Mariano Centeno Camarena.
 ******************************************************************************
 *
 * @defgroup File change log
 * @section FILE_CHANGE_LOG FILE CHANGE LOG:
 *
 ******************************************************************************
 *   - Revision 01 / 2017-06-16
 *   - PCR#00000001/Jacob Anaya Candia: Initial Release.
 *   - PCR#00000002/Author: Description.
 ******************************************************************************
 *   - Revision 02 / YYYY-MM-DD
 *   - PCR#00000003/Author: Description.
 *   - PCR#00000004/Author: Description.
 ******************************************************************************
 */
#ifndef CONFIG_METER_H_
#define CONFIG_METER_H_

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
 * Product Identification
\*===========================================================================*/
#define MENUFACTURER_ID				(char*)"TEMP"			// Specific ED Manufacturer ID		( STRING[4] )
#define MANUFACTURER_NAME			(char*)"TEMP"			// Manufacturer's name				( STRING[4] )
#define ED_MODEL					(char*)"    3PSM"		// Meter model name					( STRING[8] )

#define FW_PART_NUMBER				(char*)"Z0000003"		// Firmware Part Number				( STRING[8] )
#define FW_VERSION_NUMBER			(uint8_t) 2 			// Firmware version number
#define FW_REVISION_NUMBER			(uint8_t) 0				// Firmware revision number

#define HW_PART_NUMBER				(char*)"Z0000002"		// Hardware Part Number				( STRING[8] )
#define HW_VERSION_NUMBER			(uint8_t) 1				// Hardware version number
#define HW_REVISION_NUMBER			(uint8_t) 0				// Hardware revision number

/*===========================================================================*\
 * Metrology Config
\*===========================================================================*/
/*---------------------------------------------------------------------------*\
 * Number of Phases
 * This firmware can be configured as monophasic, biphasic or triphasic meter.
\*---------------------------------------------------------------------------*/
#define NBR_PHASES					3						

/*---------------------------------------------------------------------------*\
 * Nominal Voltage
 * Phase x-N Nominal Voltage, where x can be A, B or C. 
\*---------------------------------------------------------------------------*/
#define NOM_VOLTAGE					120000					// [mV]

/*---------------------------------------------------------------------------*\
 * Nominal Current
 * Phase x-N Nominal Current, where x can be A, B or C. 
\*---------------------------------------------------------------------------*/
#define NOM_CURRENT					30000					// [mA]

/*---------------------------------------------------------------------------*\
 * Active Power Pulse Constant Test
 * This value is used as default but can be changed with a procedure.
\*---------------------------------------------------------------------------*/
#define PULSE_CONSTANT_PKT			0.3			

/*---------------------------------------------------------------------------*\
 * Reactive Power Pulse Constant Test
 * This value is used as default but can be changed with a procedure.
\*---------------------------------------------------------------------------*/
#define PULSE_CONSTANT_QKT			PULSE_CONSTANT_PKT		

/*---------------------------------------------------------------------------*\
 * Current Power Pulse Constant Test
 * This value is used as default but can be changed with a procedure.
\*---------------------------------------------------------------------------*/
#define PULSE_CONSTANT_IKT			PULSE_CONSTANT_PKT						

/*---------------------------------------------------------------------------*\
 * Creep Threshold of Phase Current
 * Metered active/reactive energy in any line cycle interval which current is
 * less than the creep threshold is set to zero and is not accumulated.
\*---------------------------------------------------------------------------*/
#define CREEP_THRESHOLD_I			50						// [mA]

/*---------------------------------------------------------------------------*\
 * Creep Threshold of Phase Voltage
\*---------------------------------------------------------------------------*/
#define CREEP_THRESHOLD_V			12000					// [mV]

/*---------------------------------------------------------------------------*\
 * Current Sensor Type
 * Selects the type of current sensor used.
\*---------------------------------------------------------------------------*/
#define CURRENT_SENSOR_TYPE			SENSOR_TYPE_CT			// SENSOR_TYPE_CT, SENSOR_TYPE_SHUNT or SENSOR_TYPE_ROGOWSKI 

/*---------------------------------------------------------------------------*\
 * Current Conversion Factor
 * The current conversion factor is used when converting an internal current
 * quantity to an external equivalent RMS current quantity. See the
 * "Atmel_46005_SE_SAM4CMP16_Metrology_Datasheet_060514.pdf" and/or 
 * "SAM4CMP16_METROLOGY_CONFIGURE_CALIBRATE_CALCULATE_v1.2.00.xlsx" for more
 * information.
\*---------------------------------------------------------------------------*/
#define CURRENT_CONV_FACTOR			771.60494						

/*---------------------------------------------------------------------------*\
 * Voltage Conversion Factor
 * The voltage conversion factor is used when converting an internal current
 * quantity to an external equivalent RMS current quantity. See the
 * "Atmel_46005_SE_SAM4CMP16_Metrology_Datasheet_060514.pdf" and/or 
 * "SAM4CMP16_METROLOGY_CONFIGURE_CALIBRATE_CALCULATE_v1.2.00.xlsx" for more
 * information.
\*---------------------------------------------------------------------------*/															
#define VOLTAGE_CONV_FACTOR			1651			

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each block of Macro definitions
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
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
 #endif /* CONFIG_METER_H_ */