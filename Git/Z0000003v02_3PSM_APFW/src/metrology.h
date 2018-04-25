/**
 ******************************************************************************
 * \file metrology.h
 *****************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Softek Global eDesign S. de R.L. de C.V. All rights reserved.</b><br><br>
 * Softek Global eDesign Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup Metrology Meter Library
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all exportable functions for metrology Library.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 *   - mm - medidor modular.
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *     - CFE: Especificacion CFE G0100-05. Marzo 2010.
 *
 *   - Applicable Standards:
 *     - ANSI C12.18.
 *     - ANSI C12.19.
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
#ifndef METROLOGY_H_INCLUDED
#define METROLOGY_H_INCLUDED

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "compiler.h"
#include "pfail.h"
#include "arm_math.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */
#define	CONST_Pi										3.1415926
#define CREEP_THRESHOLD_PQ								((NOM_VOLTAGE * (CREEP_THRESHOLD_I * 0.8))/1000)	// Minimal quantity of instantaneous PQ for energy integration (mW or mVAR)

/*===========================================================================*\
 * Main Metrology Buffers Size
\*===========================================================================*/
#define DSP_CONTROL_SIZE								( sizeof(DSP_CTRL_TYPE) / sizeof(uint32_t) )
#define DSP_ST_SIZE										( sizeof(DSP_ST_TYPE) / sizeof(uint32_t) )
#define DSP_ACC_SIZE									( sizeof(DSP_ACC_TYPE) / sizeof(uint64_t) )

/*---------------------------------------------------------------------------*\
 * Metrology_Reg_In
 *
 * Metrology Control Registers (WORD and BIT definitions)
\*---------------------------------------------------------------------------*/

/*===========================================================================*\
 * STATE_CTRL
 * DSP Metrology State Control Register
\*===========================================================================*/
/* WORD options */
#define DSP_CTRL_ST_CTRL_Pos							0
#define	DSP_CTRL_ST_CTRL_RST 							((0x0u) << DSP_CTRL_ST_CTRL_Pos)							// Reset State
#define	DSP_CTRL_ST_CTRL_INIT 							((0x1u) << DSP_CTRL_ST_CTRL_Pos)							// Initialize State
#define	DSP_CTRL_ST_CTRL_RUN 							((0x2u) << DSP_CTRL_ST_CTRL_Pos)							// Running State
#define	DSP_CTRL_ST_CTRL_HOLD							((0x3u) << DSP_CTRL_ST_CTRL_Pos)							// Holding State

/*===========================================================================*\
 * FEATURE_CTRL0
 * DSP Metrology Feature Control Register 0
\*===========================================================================*/
/* WORD options */
#define DEFAULT_DSP_CTRL_FEATURE_CTRL0				    (DSP_CTRL_FEATURE_CTRL0_SYNCH_BaseOnA								| \
														 ( (NBR_PHASES >= 1) ? DSP_CTRL_FEATURE_CTRL0_PHASE_A_EN_En : 0 )	| \
														 ( (NBR_PHASES >= 2) ? DSP_CTRL_FEATURE_CTRL0_PHASE_B_EN_En : 0 )	| \
														 ( (NBR_PHASES >= 3) ? DSP_CTRL_FEATURE_CTRL0_PHASE_C_EN_En : 0 )	)

/* -------- SYNCH: Active Voltage Channel Selection -------- */
#define DSP_CTRL_FEATURE_CTRL0_SYNCH_Pos				4
#define	DSP_CTRL_FEATURE_CTRL0_SYNCH_BaseOnACT 			((0x0u) << DSP_CTRL_FEATURE_CTRL0_SYNCH_Pos)						
#define	DSP_CTRL_FEATURE_CTRL0_SYNCH_BaseOnA			((0x1u) << DSP_CTRL_FEATURE_CTRL0_SYNCH_Pos)						
#define	DSP_CTRL_FEATURE_CTRL0_SYNCH_BaseOnB 			((0x2u) << DSP_CTRL_FEATURE_CTRL0_SYNCH_Pos)					
#define	DSP_CTRL_FEATURE_CTRL0_SYNCH_BaseOnC			((0x3u) << DSP_CTRL_FEATURE_CTRL0_SYNCH_Pos)

/* -------- PHASE_A_EN: Enable Phase A -------- */
#define DSP_CTRL_FEATURE_CTRL0_PHASE_A_EN_Pos			8
#define	DSP_CTRL_FEATURE_CTRL0_PHASE_A_EN_Dis 			((0x0u) << DSP_CTRL_FEATURE_CTRL0_PHASE_A_EN_Pos)
#define	DSP_CTRL_FEATURE_CTRL0_PHASE_A_EN_En			((0x1u) << DSP_CTRL_FEATURE_CTRL0_PHASE_A_EN_Pos)

/* -------- PHASE_B_EN: Enable Phase B -------- */
#define DSP_CTRL_FEATURE_CTRL0_PHASE_B_EN_Pos			9
#define	DSP_CTRL_FEATURE_CTRL0_PHASE_B_EN_Dis 			((0x0u) << DSP_CTRL_FEATURE_CTRL0_PHASE_B_EN_Pos)
#define	DSP_CTRL_FEATURE_CTRL0_PHASE_B_EN_En			((0x1u) << DSP_CTRL_FEATURE_CTRL0_PHASE_B_EN_Pos)

/* -------- PHASE_C_EN: Enable Phase C -------- */
#define DSP_CTRL_FEATURE_CTRL0_PHASE_C_EN_Pos			10
#define	DSP_CTRL_FEATURE_CTRL0_PHASE_C_EN_Dis 			((0x0u) << DSP_CTRL_FEATURE_CTRL0_PHASE_C_EN_Pos)
#define	DSP_CTRL_FEATURE_CTRL0_PHASE_C_EN_En			((0x1u) << DSP_CTRL_FEATURE_CTRL0_PHASE_C_EN_Pos)

/* -------- MAX_INT_SELECT: Max Integration Period Select -------- */
#define DSP_CTRL_FEATURE_CTRL0_MAX_INT_SELECT_Pos		12
#define	DSP_CTRL_FEATURE_CTRL0_MAX_INT_SELECT_1S 		((0x0u) << DSP_CTRL_FEATURE_CTRL0_MAX_INT_SELECT_Pos)
#define	DSP_CTRL_FEATURE_CTRL0_MAX_INT_SELECT_NS		((0x1u) << DSP_CTRL_FEATURE_CTRL0_MAX_INT_SELECT_Pos)

/*===========================================================================*\
 * FEATURE_CTRL1
 * DSP Metrology Feature Control Register 1
\*===========================================================================*/
/* WORD options */
#define DEFAULT_DSP_CTRL_FEATURE_CTRL1				    (DSP_CTRL_FEATURE_CTRL1_CREEP_I_EN_En | \
														 DSP_CTRL_FEATURE_CTRL1_CREEP_Q_EN_En | \
														 DSP_CTRL_FEATURE_CTRL1_CREEP_P_EN_En )

/* -------- CREEP_I_EN: Current Creep Threshold Function Enable -------- */
#define DSP_CTRL_FEATURE_CTRL1_CREEP_I_EN_Pos			0
#define	DSP_CTRL_FEATURE_CTRL1_CREEP_I_EN_Dis 			((0x0u) << DSP_CTRL_FEATURE_CTRL1_CREEP_I_EN_Pos)
#define	DSP_CTRL_FEATURE_CTRL1_CREEP_I_EN_En			((0x1u) << DSP_CTRL_FEATURE_CTRL1_CREEP_I_EN_Pos)

/* -------- CREEP_Q_EN: Reactive Power Creep Threshold Function Enable -------- */
#define DSP_CTRL_FEATURE_CTRL1_CREEP_Q_EN_Pos			1
#define	DSP_CTRL_FEATURE_CTRL1_CREEP_Q_EN_Dis 			((0x0u) << DSP_CTRL_FEATURE_CTRL1_CREEP_Q_EN_Pos)
#define	DSP_CTRL_FEATURE_CTRL1_CREEP_Q_EN_En			((0x1u) << DSP_CTRL_FEATURE_CTRL1_CREEP_Q_EN_Pos)

/* -------- CREEP_P_EN: Active Power Creep Threshold Feature Enable -------- */
#define DSP_CTRL_FEATURE_CTRL1_CREEP_P_EN_Pos			2
#define	DSP_CTRL_FEATURE_CTRL1_CREEP_P_EN_Dis 			((0x0u) << DSP_CTRL_FEATURE_CTRL1_CREEP_P_EN_Pos)
#define	DSP_CTRL_FEATURE_CTRL1_CREEP_P_EN_En			((0x1u) << DSP_CTRL_FEATURE_CTRL1_CREEP_P_EN_Pos)

/* -------- HARMONIC_m_REQ, Request number of harmonic for analysis -------- */
#define DSP_CTRL_FEATURE_CTRL1_HARMONIC_m_REQ_Pos		8
#define	DSP_CTRL_FEATURE_CTRL1_HARMONIC_m_REQ(value) 	((value) << DSP_CTRL_FEATURE_CTRL1_HARMONIC_m_REQ_Pos)

/* -------- HARMONIC_EN: Enable Harmonic Analysis -------- */
#define DSP_CTRL_FEATURE_CTRL1_HARMONIC_EN_Pos			15
#define	DSP_CTRL_FEATURE_CTRL1_HARMONIC_EN_Dis 			((0x0u) << DSP_CTRL_FEATURE_CTRL1_HARMONIC_EN_Pos) 
#define	DSP_CTRL_FEATURE_CTRL1_HARMONIC_EN_En			((0x1u) << DSP_CTRL_FEATURE_CTRL1_HARMONIC_EN_Pos) 

/* -------- I_MAX_RESET: Reset All I_x_MAX values -------- */
#define DSP_CTRL_FEATURE_CTRL1_I_MAX_RESET_Pos			19
#define	DSP_CTRL_FEATURE_CTRL1_I_MAX_RESET_Dis 			((0x0u) << DSP_CTRL_FEATURE_CTRL1_I_MAX_RESET_Pos)
#define	DSP_CTRL_FEATURE_CTRL1_I_MAX_RESET_En			((0x1u) << DSP_CTRL_FEATURE_CTRL1_I_MAX_RESET_Pos)

/* -------- V_MAX_RESET: Reset All V_x_MAX values -------- */
#define DSP_CTRL_FEATURE_CTRL1_V_MAX_RESET_Pos			23
#define	DSP_CTRL_FEATURE_CTRL1_V_MAX_RESET_Dis 			((0x0u) << DSP_CTRL_FEATURE_CTRL1_V_MAX_RESET_Pos)
#define	DSP_CTRL_FEATURE_CTRL1_V_MAX_RESET_En			((0x1u) << DSP_CTRL_FEATURE_CTRL1_V_MAX_RESET_Pos)

/*===========================================================================*\
 * METER_TYPE
 * DSP Meter Type Settings Register
\*===========================================================================*/
/* WORD options */
#define DEFAULT_DSP_CTRL_METER_TYPE						(SENSOR_CONFIGURE_IA(SENSOR_TYPE_CT)	| \
														 SENSOR_CONFIGURE_VA(SENSOR_TYPE_VRD)	| \
														 SENSOR_CONFIGURE_IB(SENSOR_TYPE_CT)	| \
														 SENSOR_CONFIGURE_VB(SENSOR_TYPE_VRD)	| \
														 SENSOR_CONFIGURE_IC(SENSOR_TYPE_CT)	| \
														 SENSOR_CONFIGURE_VC(SENSOR_TYPE_VRD)	)
/* BIT options */
#define SENSOR_TYPE_IA_Pos								0
#define SENSOR_TYPE_VA_Pos								2
#define SENSOR_TYPE_IB_Pos								4
#define SENSOR_TYPE_VB_Pos								6
#define SENSOR_TYPE_IC_Pos								8
#define SENSOR_TYPE_VC_Pos								10

#define SENSOR_TYPE_CT									0	// Current Sensor
#define SENSOR_TYPE_SHUNT								1	// Current Sensor
#define SENSOR_TYPE_ROGOWSKI							2	// Current Sensor
#define SENSOR_TYPE_VRD									3	// Voltage Sensor

#define SENSOR_CONFIGURE_IA(x)							(x << SENSOR_TYPE_IA_Pos)
#define SENSOR_CONFIGURE_VA(x)							(x << SENSOR_TYPE_VA_Pos)	
#define SENSOR_CONFIGURE_IB(x)							(x << SENSOR_TYPE_IB_Pos)
#define SENSOR_CONFIGURE_VB(x)							(x << SENSOR_TYPE_VB_Pos)	
#define SENSOR_CONFIGURE_IC(x)							(x << SENSOR_TYPE_IC_Pos)
#define SENSOR_CONFIGURE_VC(x)							(x << SENSOR_TYPE_VC_Pos)

/*===========================================================================*\
 * M
 * DSP Number of Cycles for Integration Period Register
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_M								NOM_FREQ
#define NOM_FREQ										60

/*===========================================================================*\
 * N_MAX
 * DSP Max Number of Samples in an Integration Period
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_N_MAX							NOM_SAMPLES
#define NOM_SAMPLES										4400

/*===========================================================================*\
 * PULSEx_CTRL
 * DSP Pulse Control Register x (x = 0, 1, 2)
\*===========================================================================*/
#define DEFAULT_PULSEx_CTRL_PCx_WIDTH					2000				// Equivalent to 2000/400 = 5ms. Pulse Width is defined as: DEFAULT_PULSEx_CTRL_PCx_WIDTH/400

/* PULSE0_CTRL is used to control the pulse output from PC6 */
#define DEFAULT_DSP_CTRL_PULSE0_CTRL					(DSP_CTRL_PULSEx_CTRL_PCx_WIDTH(DEFAULT_PULSEx_CTRL_PCx_WIDTH)		| \
														 DSP_CTRL_PULSEx_CTRL_PCx_TYPE_P_T			| \
														 DSP_CTRL_PULSEx_CTRL_PCx_POLARITY_LOW		| \
														 DSP_CTRL_PULSEx_CTRL_PCx_OVERRIDE_NORMAL	| \
														 DSP_CTRL_PULSEx_CTRL_PCx_ACC_HOLD_CONTINUE	| \
														 DSP_CTRL_PULSEx_CTRL_PCx_DETENT_ABS		| \
														 DSP_CTRL_PULSEx_CTRL_PCx_ENABLE_En)

/* PULSE1_CTRL is used to control the pulse output from PC7 */
#define DEFAULT_DSP_CTRL_PULSE1_CTRL					(DSP_CTRL_PULSEx_CTRL_PCx_WIDTH(DEFAULT_PULSEx_CTRL_PCx_WIDTH)		| \
														 DSP_CTRL_PULSEx_CTRL_PCx_TYPE_Q_T			| \
														 DSP_CTRL_PULSEx_CTRL_PCx_POLARITY_LOW		| \
														 DSP_CTRL_PULSEx_CTRL_PCx_OVERRIDE_TEMP_DIS	| \
														 DSP_CTRL_PULSEx_CTRL_PCx_ACC_HOLD_CONTINUE	| \
														 DSP_CTRL_PULSEx_CTRL_PCx_DETENT_ABS		| \
														 DSP_CTRL_PULSEx_CTRL_PCx_ENABLE_Dis)

/* PULSE2_CTRL is used to control the pulse output from PC9 */
#define DEFAULT_DSP_CTRL_PULSE2_CTRL					(DSP_CTRL_PULSEx_CTRL_PCx_WIDTH(DEFAULT_PULSEx_CTRL_PCx_WIDTH)		| \
														 DSP_CTRL_PULSEx_CTRL_PCx_TYPE_I_T			| \
														 DSP_CTRL_PULSEx_CTRL_PCx_POLARITY_LOW		| \
														 DSP_CTRL_PULSEx_CTRL_PCx_OVERRIDE_TEMP_DIS	| \
														 DSP_CTRL_PULSEx_CTRL_PCx_ACC_HOLD_CONTINUE	| \
														 DSP_CTRL_PULSEx_CTRL_PCx_DETENT_ABS		| \
														 DSP_CTRL_PULSEx_CTRL_PCx_ENABLE_Dis)

/* -------- PCx_WIDTH: Pulse Width -------- */
#define DSP_CTRL_PULSEx_CTRL_PCx_WIDTH_Pos				0
#define	DSP_CTRL_PULSEx_CTRL_PCx_WIDTH(value) 			((value) << DSP_CTRL_PULSEx_CTRL_PCx_WIDTH_Pos)

/* -------- PCx_TYPE: Pulse Type Selection -------- */
#define DSP_CTRL_PULSEx_CTRL_PCx_TYPE_Pos				16
#define	DSP_CTRL_PULSEx_CTRL_PCx_TYPE_P_T	 			((0x0u) << DSP_CTRL_PULSEx_CTRL_PCx_TYPE_Pos)
#define	DSP_CTRL_PULSEx_CTRL_PCx_TYPE_P_T_F	 			((0x1u) << DSP_CTRL_PULSEx_CTRL_PCx_TYPE_Pos)
#define	DSP_CTRL_PULSEx_CTRL_PCx_TYPE_Q_T	 			((0x2u) << DSP_CTRL_PULSEx_CTRL_PCx_TYPE_Pos)
#define	DSP_CTRL_PULSEx_CTRL_PCx_TYPE_Q_T_F	 			((0x3u) << DSP_CTRL_PULSEx_CTRL_PCx_TYPE_Pos)
#define	DSP_CTRL_PULSEx_CTRL_PCx_TYPE_I_T	 			((0x4u) << DSP_CTRL_PULSEx_CTRL_PCx_TYPE_Pos)
#define	DSP_CTRL_PULSEx_CTRL_PCx_TYPE_I_T_F	 			((0x5u) << DSP_CTRL_PULSEx_CTRL_PCx_TYPE_Pos)

/* -------- PCx_POLARITY: Pulse Polarity -------- */
#define DSP_CTRL_PULSEx_CTRL_PCx_POLARITY_Pos			24
#define	DSP_CTRL_PULSEx_CTRL_PCx_POLARITY_LOW 			((0x0u) << DSP_CTRL_PULSEx_CTRL_PCx_POLARITY_Pos)
#define	DSP_CTRL_PULSEx_CTRL_PCx_POLARITY_HIGH			((0x1u) << DSP_CTRL_PULSEx_CTRL_PCx_POLARITY_Pos)

/* -------- PCx_OVERRIDE: Pulse Override Control -------- */
#define DSP_CTRL_PULSEx_CTRL_PCx_OVERRIDE_Pos			25
#define	DSP_CTRL_PULSEx_CTRL_PCx_OVERRIDE_NORMAL 		((0x0u) << DSP_CTRL_PULSEx_CTRL_PCx_OVERRIDE_Pos)
#define	DSP_CTRL_PULSEx_CTRL_PCx_OVERRIDE_TEMP_DIS		((0x1u) << DSP_CTRL_PULSEx_CTRL_PCx_OVERRIDE_Pos)

/* -------- PCx_ACC_HOLD: Pulse Accumulation Hold Control -------- */
#define DSP_CTRL_PULSEx_CTRL_PCx_ACC_HOLD_Pos			26
#define	DSP_CTRL_PULSEx_CTRL_PCx_ACC_HOLD_CONTINUE 		((0x0u) << DSP_CTRL_PULSEx_CTRL_PCx_ACC_HOLD_Pos)
#define	DSP_CTRL_PULSEx_CTRL_PCx_ACC_HOLD_HELD_STATIC	((0x1u) << DSP_CTRL_PULSEx_CTRL_PCx_ACC_HOLD_Pos)

/* -------- PCx_DETENT: Total Absolute Values for P and Q values -------- */
#define DSP_CTRL_PULSEx_CTRL_PCx_DETENT_Pos				28
#define	DSP_CTRL_PULSEx_CTRL_PCx_DETENT_NET		 		((0x0u) << DSP_CTRL_PULSEx_CTRL_PCx_DETENT_Pos)
#define	DSP_CTRL_PULSEx_CTRL_PCx_DETENT_ABS				((0x1u) << DSP_CTRL_PULSEx_CTRL_PCx_DETENT_Pos)
#define	DSP_CTRL_PULSEx_CTRL_PCx_DETENT_DEL		 		((0x2u) << DSP_CTRL_PULSEx_CTRL_PCx_DETENT_Pos)
#define	DSP_CTRL_PULSEx_CTRL_PCx_DETENT_GEN				((0x3u) << DSP_CTRL_PULSEx_CTRL_PCx_DETENT_Pos)

/* -------- PCx_ENABLE: Output Pulse Enable -------- */
#define DSP_CTRL_PULSEx_CTRL_PCx_ENABLE_Pos				31
#define	DSP_CTRL_PULSEx_CTRL_PCx_ENABLE_Dis 			((0x0u) << DSP_CTRL_PULSEx_CTRL_PCx_ENABLE_Pos)
#define	DSP_CTRL_PULSEx_CTRL_PCx_ENABLE_En				((0x1u) << DSP_CTRL_PULSEx_CTRL_PCx_ENABLE_Pos)

/*===========================================================================*\
 * P_K_t
 * DSP Meter Active Power Pulse Constant
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_P_K_T							WORD_PULSE_CONSTANT_PKT

/*---------------------------------------------------------------------------*\
 * P_K_t defines the meter test constant which is the amount of active energy
 * signified by one output pulse. The unit of P_K_t is Watt-h. P_K_t is stored
 * in uQ8.24 format.
 * If the pulse constant is set to 800 imp/kWh (1.25 Wh/imp), then:
 * P_K_t = round[1000/800*2^24] = 20971520 = 0x01400000.
\*---------------------------------------------------------------------------*/
#define WORD_PULSE_CONSTANT_PKT							CALCULATE_WORD_PKT(PULSE_CONSTANT_PKT)
#define PULSE_PER_Wh									1/PULSE_CONSTANT_PKT
#define PULSE_PER_kWh									(PULSE_PER_Wh * 1000)
#define CALCULATE_WORD_PKT(x)							roundf( (double)((double)x * (double)0x01000000) )

/*===========================================================================*\
 * Q_K_t
 * DSP Meter Reactive Power Pulse Constant
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_Q_K_T							WORD_PULSE_CONSTANT_QKT

/*---------------------------------------------------------------------------*\
 * Q_K_t defines the meter test constant which is the amount of reactive energy
 * signified by one output pulse. The unit of Q_K_t is Var-h. Q_K_t is stored
 * in uQ8.24 format.
 * If the pulse constant is set to 800 imp/kVarh (1.25 Varh/imp), then:
 * Q_K_t = round[1000/800*2^24] = 20971520 = 0x01400000.
\*---------------------------------------------------------------------------*/
#define WORD_PULSE_CONSTANT_QKT							CALCULATE_WORD_QKT(PULSE_CONSTANT_QKT)
#define PULSE_PER_VARh									1/PULSE_CONSTANT_QKT
#define PULSE_PER_kVARh									(PULSE_PER_VARh * 1000)
#define CALCULATE_WORD_QKT(x)							roundf( (double)((double)x * (double)0x01000000) )

/*===========================================================================*\
 * I_K_t
 * DSP Meter Current Pulse Constant
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_I_K_T							WORD_PULSE_CONSTANT_IKT

/*---------------------------------------------------------------------------*\
 * I_K_t defines the meter test constant which is the amount of current
 * signified by one output pulse. The unit of I_K_t is Amp2-h. I_K_t is stored
 * in uQ8.24 format.
 * If the pulse constant is set to 800 imp/kAmp2-h (1.25 Amp2-h/imp), then:
 * I_K_t = round[1000/800*2^24] = 20971520 = 0x01400000.
\*---------------------------------------------------------------------------*/
#define WORD_PULSE_CONSTANT_IKT							CALCULATE_WORD_IKT(PULSE_CONSTANT_IKT)
#define PULSE_PER_Amp2h									1/PULSE_CONSTANT_IKT
#define PULSE_PER_kAmp2h								(PULSE_PER_Amp2h * 1000)
#define CALCULATE_WORD_IKT(x)							roundf( (double)( (double)x * (double)0x01000000) )

/*===========================================================================*\
 * CREEP_THRESHOLD_P
 * DSP Creep Threshold for Active Energy
\*===========================================================================*/

/*---------------------------------------------------------------------------*\
 * CREEP_THRESHOLD_P is the creep active energy (Watt-hour) in a full cycle
 * (20ms for 50H, 16.667ms for 60Hz).
 * CREEP_THRESHOLD_P is stored in uQ2.30 format.
 * If the creep active power is set to 11 Watt-hour, and the system frequency 
 * is 60Hz, then:
 * CREEP_THRESHOLD_P = round[(11/(60*3600))*2^30] = 54681 = 0x0000D599.
\*---------------------------------------------------------------------------*/
#define DEFAULT_CREEP_THRESHOLD_P						roundf( (double)( (double)(CREEP_THRESHOLD_PQ / 1000) * (double)pow(2, 30) / (double)(NOM_FREQ *3600) ) )

/*===========================================================================*\
 * CREEP_THRESHOLD_Q
 * DSP Creep Threshold for Reactive Energy
\*===========================================================================*/

/*---------------------------------------------------------------------------*\
 * CREEP_THRESHOLD_Q is the creep reactive energy (Watt-hour) in a full cycle
 * (20ms for 50H, 16.667ms for 60Hz).
 * CREEP_THRESHOLD_Q is stored in uQ2.30 format.
 * If the creep active power is set to 11 Var-hour, and the system frequency 
 * is 60Hz, then:
 * CREEP_THRESHOLD_P = round[(11/(60*3600))*2^30] = 54681 = 0x0000D599.
\*---------------------------------------------------------------------------*/
#define DEFAULT_CREEP_THRESHOLD_Q						roundf( (double)( (double)(CREEP_THRESHOLD_PQ / 1000) * (double)pow(2, 30) / (double)(NOM_FREQ *3600) ) )

/*===========================================================================*\
 * CREEP_THRESHOLD_Q
 * DSP Creep Threshold for Reactive Energy
\*===========================================================================*/

/*---------------------------------------------------------------------------*\
 * CREEP_THRESHOLD_I is stored in uQ12.20 format and the unit is mA.
 * CREEP_THRESHOLD_I is the RMS value of creep current for ATSENSE input. It
 * mean that you must divide it by K_Ix.
 * If the creep threshold is set to 10mA, K_Ix = 617.28396, then:
 * is 60Hz, then:
 * CREEP_THRESHOLD_I = round[(10/617.28396)*2^20] = 16987 = 0x0000425B.
\*---------------------------------------------------------------------------*/
#define DEFAULT_CREEP_THRESHOLD_I(K_Ix)					roundf( (double)( (double)CREEP_THRESHOLD_I * (double)pow(2, 20) / (double)K_Ix) )

/*===========================================================================*\
 * POWER_OFFSET_CTRL
 * DSP Power Offset Control Register
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_POWER_OFFSET_CTRL				(DSP_CTRL_POWER_OFFSET_CTRL_REACTIVE_PO_EN_Dis	| \
														 DSP_CTRL_POWER_OFFSET_CTRL_ACTIVE_PO_EN_Dis	)

/* -------- REACTIVE_PO_EN: Reactive Power Offset Feature Enable -------- */
#define DSP_CTRL_POWER_OFFSET_CTRL_REACTIVE_PO_EN_Pos	27
#define	DSP_CTRL_POWER_OFFSET_CTRL_REACTIVE_PO_EN_Dis 	((0x0u) << DSP_CTRL_POWER_OFFSET_CTRL_REACTIVE_PO_EN_Pos)
#define	DSP_CTRL_POWER_OFFSET_CTRL_REACTIVE_PO_EN_En	((0x1u) << DSP_CTRL_POWER_OFFSET_CTRL_REACTIVE_PO_EN_Pos)

/* -------- ACTIVE_PO_EN: Active Power Offset Feature Enable -------- */
#define DSP_CTRL_POWER_OFFSET_CTRL_ACTIVE_PO_EN_Pos		31
#define	DSP_CTRL_POWER_OFFSET_CTRL_ACTIVE_PO_EN_Dis 	((0x0u) << DSP_CTRL_POWER_OFFSET_CTRL_ACTIVE_PO_EN_Pos)
#define	DSP_CTRL_POWER_OFFSET_CTRL_ACTIVE_PO_EN_En		((0x1u) << DSP_CTRL_POWER_OFFSET_CTRL_ACTIVE_PO_EN_Pos)

/*===========================================================================*\
 * POWER_OFFSET_P
 * DSP Active Power Offset Register
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_POWER_OFFSET_P					0

/*===========================================================================*\
 * POWER_OFFSET_Q
 * DSP Reactive Power Offset Register
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_POWER_OFFSET_Q					0

/*===========================================================================*\
 * SWELL_THRESHOLD_Vx
 * DSP Voltage Swell Threshold
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_SWELL_THRESHOLD_Vx				0

/*===========================================================================*\
 * SAG_THRESHOLD_Vx
 * DSP Voltage Sag Threshold
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_SAG_THRESHOLD_Vx				0

/*===========================================================================*\
 * K_Ix
 * DSP Current Conversion Factor
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_K_Ix							roundf( CURRENT_CONV_FACTOR * K_Ix_MULTIPLIER )
#define K_Ix_MULTIPLIER									1024

/*===========================================================================*\
 * K_Vx
 * DSP Voltage Conversion Factor
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_K_Vx							roundf( VOLTAGE_CONV_FACTOR * K_Vx_MULTIPLIER )
#define K_Vx_MULTIPLIER									1024

/*===========================================================================*\
 * CAPTURE_CTRL
 * DSP Waveform Capture Control Register
\*===========================================================================*/

/* -------- CAPTURE_TYPE: Selects whether capture is one-shot in nature -------- */
#define DSP_CTRL_CAPTURE_CTRL_CAPTURE_TYPE_Pos			0
#define	DSP_CTRL_CAPTURE_CTRL_CAPTURE_TYPE_ONE_SHOT 	((0x0u) << DSP_CTRL_CAPTURE_CTRL_CAPTURE_TYPE_Pos)
#define	DSP_CTRL_CAPTURE_CTRL_CAPTURE_TYPE_CONTINUOUS	((0x1u) << DSP_CTRL_CAPTURE_CTRL_CAPTURE_TYPE_Pos)

/* -------- CAPTURE_SOURCE: Capture Source Select -------- */
#define DSP_CTRL_CAPTURE_CTRL_CAPTURE_SOURCE_Pos		4
#define	DSP_CTRL_CAPTURE_CTRL_CAPTURE_SOURCE_OPTION0 	((0x0u) << DSP_CTRL_CAPTURE_CTRL_CAPTURE_SOURCE_Pos)
#define	DSP_CTRL_CAPTURE_CTRL_CAPTURE_SOURCE_OPTION1	((0x1u) << DSP_CTRL_CAPTURE_CTRL_CAPTURE_SOURCE_Pos)
#define	DSP_CTRL_CAPTURE_CTRL_CAPTURE_SOURCE_OPTION2	((0x2u) << DSP_CTRL_CAPTURE_CTRL_CAPTURE_SOURCE_Pos)

/* -------- CH_SEL_IA: Capture Channel Select -------- */
#define DSP_CTRL_CAPTURE_CTRL_CH_SEL_IA_Pos				8
#define	DSP_CTRL_CAPTURE_CTRL_CH_SEL_IA_Dis 			((0x0u) << DSP_CTRL_CAPTURE_CTRL_CH_SEL_IA_Pos)
#define	DSP_CTRL_CAPTURE_CTRL_CH_SEL_IA_En				((0x1u) << DSP_CTRL_CAPTURE_CTRL_CH_SEL_IA_Pos)

/* -------- CH_SEL_VA: Capture Channel Select -------- */
#define DSP_CTRL_CAPTURE_CTRL_CH_SEL_VA_Pos				9
#define	DSP_CTRL_CAPTURE_CTRL_CH_SEL_VA_Dis 			((0x0u) << DSP_CTRL_CAPTURE_CTRL_CH_SEL_VA_Pos)
#define	DSP_CTRL_CAPTURE_CTRL_CH_SEL_VA_En				((0x1u) << DSP_CTRL_CAPTURE_CTRL_CH_SEL_VA_Pos)

/* -------- CH_SEL_IB: Capture Channel Select -------- */
#define DSP_CTRL_CAPTURE_CTRL_CH_SEL_IB_Pos				10
#define	DSP_CTRL_CAPTURE_CTRL_CH_SEL_IB_Dis 			((0x0u) << DSP_CTRL_CAPTURE_CTRL_CH_SEL_IB_Pos)
#define	DSP_CTRL_CAPTURE_CTRL_CH_SEL_IB_En				((0x1u) << DSP_CTRL_CAPTURE_CTRL_CH_SEL_IB_Pos)

/* -------- CH_SEL_VB: Capture Channel Select -------- */
#define DSP_CTRL_CAPTURE_CTRL_CH_SEL_VB_Pos				11
#define	DSP_CTRL_CAPTURE_CTRL_CH_SEL_VB_Dis 			((0x0u) << DSP_CTRL_CAPTURE_CTRL_CH_SEL_VB_Pos)
#define	DSP_CTRL_CAPTURE_CTRL_CH_SEL_VB_En				((0x1u) << DSP_CTRL_CAPTURE_CTRL_CH_SEL_VB_Pos)

/* -------- CH_SEL_IC: Capture Channel Select -------- */
#define DSP_CTRL_CAPTURE_CTRL_CH_SEL_IC_Pos				12
#define	DSP_CTRL_CAPTURE_CTRL_CH_SEL_IC_Dis 			((0x0u) << DSP_CTRL_CAPTURE_CTRL_CH_SEL_IC_Pos)
#define	DSP_CTRL_CAPTURE_CTRL_CH_SEL_IC_En				((0x1u) << DSP_CTRL_CAPTURE_CTRL_CH_SEL_IC_Pos)

/* -------- CH_SEL_VC: Capture Channel Select -------- */
#define DSP_CTRL_CAPTURE_CTRL_CH_SEL_VC_Pos				13
#define	DSP_CTRL_CAPTURE_CTRL_CH_SEL_VC_Dis 			((0x0u) << DSP_CTRL_CAPTURE_CTRL_CH_SEL_VC_Pos)
#define	DSP_CTRL_CAPTURE_CTRL_CH_SEL_VC_En				((0x1u) << DSP_CTRL_CAPTURE_CTRL_CH_SEL_VC_Pos)

/* -------- CAPTURE_EN: Enable waveform capture -------- */
#define DSP_CTRL_CAPTURE_CTRL_CAPTURE_EN_Pos			31
#define	DSP_CTRL_CAPTURE_CTRL_CAPTURE_EN_Dis 			((0x0u) << DSP_CTRL_CAPTURE_CTRL_CAPTURE_EN_Pos)
#define	DSP_CTRL_CAPTURE_CTRL_CAPTURE_EN_En				((0x1u) << DSP_CTRL_CAPTURE_CTRL_CAPTURE_EN_Pos)

/*===========================================================================*\
 * ATSENSE_CTRL_20_23
 * DSP ATSENSE ADC Control Register 0
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_ATSENSE_CTRL_20_23_MASK		(( (NBR_PHASES >= 1) ? (DSP_CTRL_ATSENSE_CTRL_20_23_I1_ON_Mask | DSP_CTRL_ATSENSE_CTRL_20_23_V1_ON_Mask) : 0 )	| \
														 ( (NBR_PHASES >= 2) ? (DSP_CTRL_ATSENSE_CTRL_20_23_I2_ON_Mask) : 0 )											)

/* -------- I0_ON: Enable channel I0 -------- */
#define	DSP_CTRL_ATSENSE_CTRL_20_23_I0_ON_Pos			0
#define	DSP_CTRL_ATSENSE_CTRL_20_23_I0_ON_Mask			(0x1<<DSP_CTRL_ATSENSE_CTRL_20_23_I0_ON_Pos)

/* -------- TEMPMEAS: Channel I0/Temperature multiplex select bit -------- */
#define	DSP_CTRL_ATSENSE_CTRL_20_23_TEMPMEAS_Pos		1
#define	DSP_CTRL_ATSENSE_CTRL_20_23_TEMPMEAS_Mask		(0x1<<DSP_CTRL_ATSENSE_CTRL_20_23_TEMPMEAS_Pos)

/* -------- I0_GAIN: Channel I0 -------- */
#define	DSP_CTRL_ATSENSE_CTRL_20_23_I0_GAIN_Pos			4
#define	DSP_CTRL_ATSENSE_CTRL_20_23_I0_GAIN_Mask		(0x3<<DSP_CTRL_ATSENSE_CTRL_20_23_I0_GAIN_Pos)

/* -------- I1_ON: Enable channel I1 -------- */
#define	DSP_CTRL_ATSENSE_CTRL_20_23_I1_ON_Pos			8
#define	DSP_CTRL_ATSENSE_CTRL_20_23_I1_ON_Mask			(0x1<<DSP_CTRL_ATSENSE_CTRL_20_23_I1_ON_Pos)

/* -------- I1_GAIN: Channel I1 gain -------- */
#define	DSP_CTRL_ATSENSE_CTRL_20_23_I1_GAIN_Pos			12
#define	DSP_CTRL_ATSENSE_CTRL_20_23_I1_GAIN_Mask		(0x3<<DSP_CTRL_ATSENSE_CTRL_20_23_I1_GAIN_Pos)

/* -------- V1_ON: Enable channel V1 -------- */
#define	DSP_CTRL_ATSENSE_CTRL_20_23_V1_ON_Pos			16
#define	DSP_CTRL_ATSENSE_CTRL_20_23_V1_ON_Mask			(0x1<<DSP_CTRL_ATSENSE_CTRL_20_23_V1_ON_Pos)

/* -------- I2_ON: Enable channel I2 -------- */
#define	DSP_CTRL_ATSENSE_CTRL_20_23_I2_ON_Pos			24
#define	DSP_CTRL_ATSENSE_CTRL_20_23_I2_ON_Mask			(0x1<<DSP_CTRL_ATSENSE_CTRL_20_23_I2_ON_Pos)

/* -------- I2_GAIN: Channel I2 gain -------- */
#define	DSP_CTRL_ATSENSE_CTRL_20_23_I2_GAIN_Pos			28
#define	DSP_CTRL_ATSENSE_CTRL_20_23_I2_GAIN_Mask		(0x3<<DSP_CTRL_ATSENSE_CTRL_20_23_I2_GAIN_Pos)

/*===========================================================================*\
 * ATSENSE_CTRL_24_27
 * DSP ATSENSE ADC Control Register 1
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_ATSENSE_CTRL_24_27_MASK		(DSP_CTRL_ATSENSE_CTRL_24_27_ONBIAS_Mask																		| \
														 DSP_CTRL_ATSENSE_CTRL_24_27_ONREF_Mask																			| \
														 DSP_CTRL_ATSENSE_CTRL_24_27_ONLDO_Mask																			| \
														 ( (NBR_PHASES >= 2) ? (DSP_CTRL_ATSENSE_CTRL_24_27_V2_ON_Mask) : 0 )											| \
														 ( (NBR_PHASES >= 3) ? (DSP_CTRL_ATSENSE_CTRL_24_27_I3_ON_Mask | DSP_CTRL_ATSENSE_CTRL_24_27_V3_ON_Mask) : 0 )	)

/* -------- V2_ON: Enable channel V2 -------- */
#define	DSP_CTRL_ATSENSE_CTRL_24_27_V2_ON_Pos			0
#define	DSP_CTRL_ATSENSE_CTRL_24_27_V2_ON_Mask			(0x1<<DSP_CTRL_ATSENSE_CTRL_24_27_V2_ON_Pos)

/* -------- I3_ON: Enable channel I3 -------- */
#define	DSP_CTRL_ATSENSE_CTRL_24_27_I3_ON_Pos			8
#define	DSP_CTRL_ATSENSE_CTRL_24_27_I3_ON_Mask			(0x1<<DSP_CTRL_ATSENSE_CTRL_24_27_I3_ON_Pos)

/* -------- I3_GAIN: Channel I3 gain -------- */
#define	DSP_CTRL_ATSENSE_CTRL_24_27_I3_GAIN_Pos			12
#define	DSP_CTRL_ATSENSE_CTRL_24_27_I3_GAIN_Mask		(0x3<<DSP_CTRL_ATSENSE_CTRL_24_27_I3_GAIN_Pos)

/* -------- V3_ON: Enable channel V3 -------- */
#define	DSP_CTRL_ATSENSE_CTRL_24_27_V3_ON_Pos			16
#define	DSP_CTRL_ATSENSE_CTRL_24_27_V3_ON_Mask			(0x1<<DSP_CTRL_ATSENSE_CTRL_24_27_V3_ON_Pos)

/* -------- ONBIAS: Enable bias -------- */
#define	DSP_CTRL_ATSENSE_CTRL_24_27_ONBIAS_Pos			24
#define	DSP_CTRL_ATSENSE_CTRL_24_27_ONBIAS_Mask			(0x1<<DSP_CTRL_ATSENSE_CTRL_24_27_ONBIAS_Pos)

/* -------- ONREF: Enable internal reference -------- */
#define	DSP_CTRL_ATSENSE_CTRL_24_27_ONREF_Pos			25
#define	DSP_CTRL_ATSENSE_CTRL_24_27_ONREF_Mask			(0x1<<DSP_CTRL_ATSENSE_CTRL_24_27_ONREF_Pos)

/* -------- ONLDO: Enable internal LDO -------- */
#define	DSP_CTRL_ATSENSE_CTRL_24_27_ONLDO_Pos			26
#define	DSP_CTRL_ATSENSE_CTRL_24_27_ONLDO_Mask			(0x1<<DSP_CTRL_ATSENSE_CTRL_24_27_ONLDO_Pos)

/*===========================================================================*\
 * ATSENSE_CTRL_28_2B
 * DSP ATSENSE ADC Control Register 2
\*===========================================================================*/
#define DEFAULT_DSP_CTRL_ATSENSE_CTRL_28_2B_MASK		DSP_CTRL_ATSENSE_CTRL_28_2B_OSR_64

/* -------- OSR: Over Sampling Ratio -------- */
#define	DSP_CTRL_ATSENSE_CTRL_28_2B_OSR_Pos				0
#define	DSP_CTRL_ATSENSE_CTRL_28_2B_OSR_8		 		((0x0u) << DSP_CTRL_ATSENSE_CTRL_28_2B_OSR_Pos)
#define	DSP_CTRL_ATSENSE_CTRL_28_2B_OSR_16				((0x1u) << DSP_CTRL_ATSENSE_CTRL_28_2B_OSR_Pos)
#define	DSP_CTRL_ATSENSE_CTRL_28_2B_OSR_32	 			((0x2u) << DSP_CTRL_ATSENSE_CTRL_28_2B_OSR_Pos)
#define	DSP_CTRL_ATSENSE_CTRL_28_2B_OSR_64				((0x3u) << DSP_CTRL_ATSENSE_CTRL_28_2B_OSR_Pos)

/* -------- MSB_MODE: Mode selection -------- */
#define DSP_CTRL_ATSENSE_CTRL_28_2B_MSB_MODE_Pos		4
#define	DSP_CTRL_ATSENSE_CTRL_28_2B_MSB_MODE_32_BITS 	((0x0u) << DSP_CTRL_ATSENSE_CTRL_28_2B_MSB_MODE_Pos)
#define	DSP_CTRL_ATSENSE_CTRL_28_2B_MSB_MODE_16_BITS	((0x1u) << DSP_CTRL_ATSENSE_CTRL_28_2B_MSB_MODE_Pos)

/*---------------------------------------------------------------------------*\
 * Metrology_Reg_Out
 *
 * Metrology Status Registers (WORD and BIT definitions)
\*---------------------------------------------------------------------------*/

/*===========================================================================*\
 * STATUS
 * DSP Metrology Status Register
\*===========================================================================*/
#define	DSP_ST_STATUS_HALT								0
#define	DSP_ST_STATUS_RESET								1
#define	DSP_ST_STATUS_INIT_DSP							2
#define	DSP_ST_STATUS_DSP_READY							3
#define	DSP_ST_STATUS_INIT_ATSENSE						4
#define	DSP_ST_STATUS_ATSENSE_READY						5
#define	DSP_ST_STATUS_READY								6
#define	DSP_ST_STATUS_DSP_SETTLING						7
#define	DSP_ST_STATUS_DSP_RUNNING						8

#define FREQ_Q											12

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs                          
 ******************************************************************************
 */

/*===========================================================================*\
 * STATE CTRL STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t ST_CTRL		: 4;	// status control
		uint32_t				: 28;
	} BIT;

} STATE_CTRL_TYPE;

/*===========================================================================*\
 * STATE CTRL ENUM
\*===========================================================================*/
typedef	enum {

	IsReset	= 0,
	IsInit	= 1,
	IsRun	= 2,
	IsHold	= 3,
} DSP_CTRL_ST_CTRL_TYPE;

/*===========================================================================*\
 * FEATURE CTRL0 STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t ACC_BUF_EN		: 1;	// ACC BUFF ENABLE
		uint32_t				: 3;
		uint32_t SYNCH			: 2;
		uint32_t				: 2;
		uint32_t PHASE_A_EN		: 1;	// ENABLE
		uint32_t PHASE_B_EN		: 1;	// ENABLE
		uint32_t PHASE_C_EN		: 1;	// ENABLE
		uint32_t 				: 1;
		uint32_t MAX_INT_SELECT	: 1;
		uint32_t				: 19;
	} BIT;

} FEATURE_CTRL0_TYPE;

/*===========================================================================*\
 * FEATURE CTRL1 STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t CREEP_I_EN		: 1;	//ENABLE
		uint32_t CREEP_Q_EN		: 1;	//ENABLE
		uint32_t CREEP_P_EN		: 1;	//ENABLE
		uint32_t 				: 5;
		uint32_t HARMONIC_m_REQ	: 6;
		uint32_t 				: 1;
		uint32_t HARMONIC_EN	: 1;
		uint32_t 				: 3;
		uint32_t I_MAX_RESET	: 1;
		uint32_t 				: 3;
		uint32_t V_MAX_RESET	: 1;
		uint32_t 				: 8;
	} BIT;

} FEATURE_CTRL1_TYPE;

/*===========================================================================*\
 * METER TYPE STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t SENSOR_TYPE_I_A : 2;	
		uint32_t SENSOR_TYPE_V_A : 2;	
		uint32_t SENSOR_TYPE_I_B : 2;	
		uint32_t SENSOR_TYPE_V_B : 2;	
		uint32_t SENSOR_TYPE_I_C : 2;	
		uint32_t SENSOR_TYPE_V_C : 2;	
		uint32_t 				 : 20;
	} BIT;

} METER_TYPE_TYPE;

/*===========================================================================*\
 * INTEGRATION PERIOD STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t MV				: 12;
		uint32_t				: 20;
	} BIT;

} M_TYPE_TYPE;

/*===========================================================================*\
 * SAMPLE NUMBER STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t MAX			: 24;
		uint32_t				: 8;
	} BIT;

} N_MAX_TYPE_TYPE;

/*===========================================================================*\
 * PULSE CTRL STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t PCx_WIDTH		: 16;
		uint32_t PCx_TYPE		: 8;
		uint32_t PCx_POLARITY	: 1;
		uint32_t PCx_OVERRIDE	: 1;
		uint32_t PCx_ACC_HOLD	: 1;
		uint32_t 				: 1;
		uint32_t PCx_DETENT		: 2;
		uint32_t 				: 1;
		uint32_t PCx_ENABLE		: 1;
	} BIT;

} PULSE_CTRL_TYPE;

/*===========================================================================*\
 * POWER OFFSET CTRL STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t				: 27;
		uint32_t REACTIVE_PO_EN : 1;
		uint32_t				: 3;
		uint32_t ACTIVE_PO_EN	: 1;
	} BIT;

} POWER_OS_CTRL_TYPE;

/*===========================================================================*\
 * CAPTURE CTRL STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t CAPTURE_TYPE	: 2;
		uint32_t 				: 2;
		uint32_t CAPTURE_SOURCE	: 3;
		uint32_t 				: 1;
		uint32_t CH_SEL_IA		: 1;
		uint32_t CH_SEL_VA		: 1;
		uint32_t CH_SEL_IB		: 1;
		uint32_t CH_SEL_VB		: 1;
		uint32_t CH_SEL_IC		: 1;
		uint32_t CH_SEL_VC		: 1;
		uint32_t 				: 17;
		uint32_t CAPTURE_EN		: 1;
	} BIT;

} CAPTURE_CTRL_TYPE;

/*===========================================================================*\
 * CAPTURE BUFFER SIZE STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t CP_BUF_SIZE	: 24;
		uint32_t				: 8;
	} BIT;

} CAPTURE_BUF_SIZE_TYPE;

/*===========================================================================*\
 * ATSENSE_CTRL_20_23 STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t I0_ON			: 1; 
		uint32_t TEMPMEAS		: 1; 
		uint32_t 				: 2; 
		uint32_t I0_GAIN		: 2; 
		uint32_t 				: 2; 
		uint32_t I1_ON			: 1; 
		uint32_t 				: 3; 
		uint32_t I1_GAIN		: 2; 
		uint32_t 				: 2; 
		uint32_t V1_ON			: 1; 
		uint32_t 				: 7; 
		uint32_t I2_ON			: 1; 
		uint32_t 				: 3; 
		uint32_t I2_GAIN		: 2; 
		uint32_t 				: 2; 
	} BIT;

} ATsens_CTRL_20_23_TYPE;

/*===========================================================================*\
 * ATSENSE_CTRL_24_27 STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t V2_ON			: 1;
		uint32_t				: 7;
		uint32_t I3_ON			: 1;
		uint32_t 				: 3;
		uint32_t I3_GAIN		: 2;
		uint32_t 				: 2;
		uint32_t V3_ON			: 1;
		uint32_t 				: 7;
		uint32_t ONBIAS			: 1;
		uint32_t ONREF			: 1;
		uint32_t ONLDO			: 1;
		uint32_t 				: 5;
	} BIT;

} ATsens_CTRL_24_27_TYPE;

/*===========================================================================*\
 * ATSENSE_CTRL_28_2B STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t OSR			: 2; 
		uint32_t 				: 2; 
		uint32_t MSB_MODE		: 1; 
		uint32_t 				: 27;
	} BIT;

} ATsens_CTRL_28_2B_TYPE;

/*===========================================================================*\
 * DSP_CTRL STRUCT
\*===========================================================================*/
typedef	struct {

	STATE_CTRL_TYPE STATE_CTRL;					//uQ32.0
	FEATURE_CTRL0_TYPE FEATURE_CTRL0;
	FEATURE_CTRL1_TYPE FEATURE_CTRL1;
	METER_TYPE_TYPE METER_TYPE;

	M_TYPE_TYPE M;
	N_MAX_TYPE_TYPE N_MAX;
	PULSE_CTRL_TYPE PULSE0_CTRL;
	PULSE_CTRL_TYPE PULSE1_CTRL;
	PULSE_CTRL_TYPE	PULSE2_CTRL;

	uint32_t P_K_T;								//uQ8.24	Wh/pulse
	uint32_t Q_K_T;								//uQ8.24	Wh/pulse
	uint32_t I_K_T;								//uQ8.24	Wh/pulse

	uint32_t CREEP_THR_P;
	uint32_t CREEP_THR_Q;
	uint32_t CREEP_THR_I;

	POWER_OS_CTRL_TYPE POWER_OFFSET_CTRL;

	int32_t POWER_OFFSET_P;						//sQ1.30
	int32_t POWER_OFFSET_Q;						//sQ1.30

	uint32_t SWELL_THR_VA;
	uint32_t SWELL_THR_VB;
	uint32_t SWELL_THR_VC;

	uint32_t SAG_THR_VA;
	uint32_t SAG_THR_VB;
	uint32_t SAG_THR_VC;

	uint32_t K_IA;								//uQ22.10
	uint32_t K_VA;								//uQ22.10
	uint32_t K_IB;
	uint32_t K_VB;
	uint32_t K_IC;
	uint32_t K_VC;

	uint32_t RESERVED1;

	int32_t CAL_M_IA;							//sQ2.29
	int32_t	CAL_M_VA;
	int32_t	CAL_M_IB;
	int32_t	CAL_M_VB;
	int32_t	CAL_M_IC;
	int32_t	CAL_M_VC;

	int32_t	RESERVED2;

	int32_t	CAL_PH_IA;							//sQ0.31
	int32_t	CAL_PH_VA;
	int32_t	CAL_PH_IB;
	int32_t	CAL_PH_VB;
	int32_t	CAL_PH_IC;
	int32_t	CAL_PH_VC;

	int32_t	RESERVED3;
	uint32_t RESERVED4;							//uQ32.0
	uint32_t RESERVED5;
	uint32_t RESERVED6;
			 
	uint32_t RESERVED7;
	uint32_t RESERVED8;
	uint32_t RESERVED9;

	ATsens_CTRL_20_23_TYPE ATsense_CTRL_20_23;
	ATsens_CTRL_24_27_TYPE ATsense_CTRL_24_27;
	ATsens_CTRL_28_2B_TYPE ATsense_CTRL_28_2B;

	uint32_t RESERVED10;

} DSP_CTRL_TYPE;

/*===========================================================================*\
 * VERSION STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t FIRMWARE_VER	: 24;
		uint32_t				: 8;
	} BIT;

} VERSION_TYPE;

/*===========================================================================*\
 * STATUS STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t ST				: 4; 
		uint32_t				: 28;
	} BIT;

} STATUS_TYPE;

/*===========================================================================*\
 * STATE FLAG STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t PH_A_PRESENT		: 1;
		uint32_t PH_B_PRESENT		: 1;
		uint32_t PH_C_PRESENT		: 1;
		uint32_t ACTIVE_CHANNLE		: 2;
		uint32_t FREQ_CLOCK			: 1;
		uint32_t 					: 1;
		uint32_t ATSENSE_FAIL		: 1;
		uint32_t HARMONIC_M_CONF	: 8;
		uint32_t CREEP_DETECT_IA	: 1;
		uint32_t CREEP_DETECT_IB	: 1;
		uint32_t CREEP_DETECT_IC	: 1;
		uint32_t CREEP_DETECT_P		: 1;
		uint32_t CREEP_DETECT_Q		: 1;
		uint32_t 					: 3;
		uint32_t SAG_DET_VA			: 1;
		uint32_t SAG_DET_VB			: 1;
		uint32_t SAG_DET_VC			: 1;
		uint32_t SWELL_DET_VA		: 1;
		uint32_t SWELL_DET_VB		: 1;
		uint32_t SWELL_DET_VC		: 1;
		uint32_t 					: 2;
	} BIT;

} STATE_FLAG_TYPE;

/*===========================================================================*\
 * ATSENSE_CAL_41_44 STRUCT
\*===========================================================================*/
typedef	union {

	uint32_t WORD;
	struct {
		uint32_t REF_TL_11_8		: 4;
		uint32_t 					: 4;
		uint32_t REF_TL_7_0			: 8;
		uint32_t TEMP_TL_11_8		: 4;
		uint32_t 					: 4;
		uint32_t TEMP_TL_7_0		: 8;
	} BIT;

} ATsense_CAL_41_44_TYPE;

/*===========================================================================*\
 * ATSENSE_CAL_45_48 STRUCT
\*===========================================================================*/
typedef	union {
	
	uint32_t WORD;
	struct {
		uint32_t REF_TH_11_8		: 4; 
		uint32_t 					: 4;
		uint32_t REF_TH_7_0			: 8;
		uint32_t TEMP_TH_11_8		: 4; 
		uint32_t 					: 4;
		uint32_t TEMP_TH_7_0		: 8;
	} BIT;

} ATsense_CAL_45_48_TYPE;

/*===========================================================================*\
 * DSP_STATUS STRUCT
\*===========================================================================*/
typedef	struct
{
	VERSION_TYPE VERSION;				
	STATUS_TYPE STATUS;
	STATE_FLAG_TYPE STATE_FLAG;
	uint32_t CP_ST;
	uint32_t INTERVAL_NUM;
	uint32_t N;
	int32_t	PH_OFFSET;					
	uint32_t FREQ;						
	uint32_t FREQ_VA;					
	uint32_t FREQ_VB;
	uint32_t FREQ_VC;
	uint32_t RESERVED0;
	int32_t	TEMPERATURE;				
	int32_t	I_A_MAX;					
	int32_t	I_B_MAX;
	int32_t	I_C_MAX;
	int32_t	I_Ni_MAX;
	int32_t	I_Nm_MAX;
	int32_t	V_A_MAX;
	int32_t	V_B_MAX;
	int32_t	V_C_MAX;
	uint32_t RESERVED1;
	uint32_t RESERVED2;
	uint32_t RESERVED3;
	uint32_t RESERVED4;
	uint32_t RESERVED5;
	uint32_t RESERVED6;
	uint32_t RESERVED7;
	uint32_t RESERVED8;
	uint32_t RESERVED9;
	uint32_t ZC_N_VA;
	uint32_t ZC_N_VB;
	uint32_t ZC_N_VC;
	ATsense_CAL_41_44_TYPE ATsense_CAL_41_44;
	ATsense_CAL_45_48_TYPE ATsense_CAL_45_48;

} DSP_ST_TYPE, *p_DSP_ST;

/*===========================================================================*\
 * DSP_ACCUMULATED STRUCT
\*===========================================================================*/
typedef	struct {

	uint64_t I_A;						
	uint64_t I_B;
	uint64_t I_C;
	uint64_t I_Ni;
	uint64_t I_Nm;
	uint64_t I_A_F;
	uint64_t I_B_F;
	uint64_t I_C_F;
	uint64_t I_Ni_F;
	uint64_t I_A_2;
	uint64_t I_B_2;
	uint64_t I_C_2;
	uint64_t RESERVED0;
	uint64_t RESERVED1;
	uint64_t RESERVED2;
	int64_t	P_A;						
	int64_t	P_B;
	int64_t	P_C;
	int64_t	P_A_F;
	int64_t	P_B_F;
	int64_t	P_C_F;
	uint64_t RESERVED3;
	uint64_t RESERVED4;
	uint64_t RESERVED5;
	int64_t	Q_A;
	int64_t	Q_B;
	int64_t	Q_C;
	int64_t	Q_A_F;
	int64_t	Q_B_F;
	int64_t	Q_C_F;
	uint64_t RESERVED6;
	uint64_t RESERVED7;
	uint64_t RESERVED8;
	uint64_t V_A;						
	uint64_t V_B;
	uint64_t V_C;
	uint64_t RESERVED9;
	uint64_t V_A_F;
	uint64_t V_B_F;
	uint64_t V_C_F;
	uint64_t RESERVED10;
	uint64_t V_AB;
	uint64_t V_BC;
	uint64_t V_CA;
	uint64_t V_AB_F;
	uint64_t V_BC_F;
	uint64_t V_CA_F;
	uint64_t RESERVED11;
	uint64_t RESERVED12;
	uint64_t RESERVED13;
	uint64_t ACC_T0;					
	uint64_t ACC_T1;
	uint64_t ACC_T2;
	uint64_t RESERVED14;
	uint64_t RESERVED15;

} DSP_ACC_TYPE;


/*===========================================================================*\
 * DSP_HAR STRUCT
\*===========================================================================*/
typedef struct {

	uint32_t I_A_R;
	uint32_t V_A_R;
	uint32_t I_B_R;
	uint32_t V_B_R;
	uint32_t I_C_R;
	uint32_t V_C_R;
	uint32_t I_A_I;
	uint32_t V_A_I;
	uint32_t I_B_I;
	uint32_t V_B_I;
	uint32_t I_C_I;
	uint32_t V_C_I;
	uint32_t RESERVED0;
	uint32_t RESERVED1;
	uint32_t RESERVED2;

} DSP_HAR_TYPE;

/*===========================================================================*\
 * METROLOGY STRUCT
\*===========================================================================*/
typedef	struct {

	DSP_CTRL_TYPE DSP_CTRL;
	DSP_ST_TYPE DSP_ST;
	DSP_ACC_TYPE DSP_ACC;
	DSP_HAR_TYPE DSP_HAR;

} METROLOGY_TYPE, *p_Metrology;

/*===========================================================================*\
 * QUADRANTS ENUM
\*===========================================================================*/
typedef	enum {

	QUADRANT_1 = 1,
	QUADRANT_2,
	QUADRANT_3,
	QUADRANT_4

} QUADRANT_TYPE;

/*===========================================================================*\
 * INSTRUMENTATION ID'S ENUM
\*===========================================================================*/
typedef enum {

	P_T_ID,
	P_A_ID,
	P_B_ID,
	P_C_ID,
	Q_T_ID,
	Q_A_ID,
	Q_B_ID,
	Q_C_ID,
	S_T_ID,
	S_A_ID,
	S_B_ID,
	S_C_ID,
	V_A_ID,
	V_B_ID,
	V_C_ID,
	V_AB_ID,
	V_BC_ID,
	V_CA_ID,
	I_A_ID,
	I_B_ID,
	I_C_ID,
	ANGLE_V_A_ID,
	ANGLE_V_B_ID,
	ANGLE_V_C_ID,
	ANGLE_I_A_ID,
	ANGLE_I_B_ID,
	ANGLE_I_C_ID,
	ANGLE_PF_T_ID,
	ANGLE_PF_A_ID,
	ANGLE_PF_B_ID,
	ANGLE_PF_C_ID,
	PF_T_ID,
	PF_A_ID,
	PF_B_ID,
	PF_C_ID,
	QUADRANT_T_ID,
	QUADRANT_A_ID,
	QUADRANT_B_ID,
	QUADRANT_C_ID,
	FREQ_T_ID,
	FREQ_A_ID,
	FREQ_B_ID,
	FREQ_C_ID,
	INST_NUM_MAX

} RMS_TYPE;

/*===========================================================================*\
 * INSTRUMENTATION VALUES STRUCT
\*===========================================================================*/
typedef struct {

	double P_t;
	double P_a;
	double P_b;
	double P_c;
	double Q_t;
	double Q_a;
	double Q_b;
	double Q_c;
	double S_t;
	double S_a;
	double S_b;
	double S_c;
	double V_a;
	double V_b;
	double V_c;
	double V_ab;
	double V_bc;
	double V_ca;
	double I_a;
	double I_b;
	double I_c;
	double Angle_V_a;
	double Angle_V_b;
	double Angle_V_c;
	double Angle_I_a;
	double Angle_I_b;
	double Angle_I_c;
	double Angle_PF_t;
	double Angle_PF_a;
	double Angle_PF_b;
	double Angle_PF_c;
	double PF_t;
	double PF_a;
	double PF_b;
	double PF_c;
	double Quadrant_t;
	double Quadrant_a;
	double Quadrant_b;
	double Quadrant_c;
	double Freq_t;
	double Freq_a;
	double Freq_b;
	double Freq_c;

} INSTANTANEOUS_VALUES_TYPE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
extern METROLOGY_TYPE VMetrology;

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
 */
extern void METROLOGY_APP_vInitialize(void);

extern void Set_DSP_CTRL_ST_CTRL( DSP_CTRL_ST_CTRL_TYPE iSStatus );

extern void METROLOGY_APP_vEnable_P_energy_pulse_mode(void);
extern void METROLOGY_APP_vEnable_Q_energy_pulse_mode(void);

extern bool METROLOGY_APP_bCalibrate_m_ia(uint32_t mA_in);
extern bool METROLOGY_APP_bCalibrate_m_va(uint32_t mV_in);
extern bool METROLOGY_APP_bCalibrate_ph_ia(uint32_t ph_error);
extern bool METROLOGY_APP_bWrite_cal_m_ia(uint32_t adjust_value);
extern bool METROLOGY_APP_bWrite_cal_m_va(uint32_t adjust_value);
extern bool METROLOGY_APP_bWrite_cal_ph_ia(uint32_t adjust_value);
extern bool METROLOGY_APP_bBack_up_cal_m_ia(void);
extern bool METROLOGY_APP_bBack_up_cal_m_va(void);
extern bool METROLOGY_APP_bBack_up_cal_ph_ia(void);
extern bool METROLOGY_APP_bRestore_cal_m_ia_backup(void);
extern bool METROLOGY_APP_bRestore_cal_m_va_backup(void);
extern bool METROLOGY_APP_bRestore_cal_ph_ia_backup(void);

extern bool METROLOGY_APP_bCalibrate_m_ib(uint32_t mA_in);
extern bool METROLOGY_APP_bCalibrate_m_vb(uint32_t mV_in);
extern bool METROLOGY_APP_bCalibrate_ph_ib(uint32_t ph_error);
extern bool METROLOGY_APP_bWrite_cal_m_ib(uint32_t adjust_value);
extern bool METROLOGY_APP_bWrite_cal_m_vb(uint32_t adjust_value);
extern bool METROLOGY_APP_bWrite_cal_ph_ib(uint32_t adjust_value);
extern bool METROLOGY_APP_bBack_up_cal_m_ib(void);
extern bool METROLOGY_APP_bBack_up_cal_m_vb(void);
extern bool METROLOGY_APP_bBack_up_cal_ph_ib(void);
extern bool METROLOGY_APP_bRestore_cal_m_ib_backup(void);
extern bool METROLOGY_APP_bRestore_cal_m_vb_backup(void);
extern bool METROLOGY_APP_bRestore_cal_ph_ib_backup(void);

extern bool METROLOGY_APP_bCalibrate_m_ic(uint32_t mA_in);
extern bool METROLOGY_APP_bCalibrate_m_vc(uint32_t mV_in);
extern bool METROLOGY_APP_bCalibrate_ph_ic(uint32_t ph_error);
extern bool METROLOGY_APP_bWrite_cal_m_ic(uint32_t adjust_value);
extern bool METROLOGY_APP_bWrite_cal_m_vc(uint32_t adjust_value);
extern bool METROLOGY_APP_bWrite_cal_ph_ic(uint32_t adjust_value);
extern bool METROLOGY_APP_bBack_up_cal_m_ic(void);
extern bool METROLOGY_APP_bBack_up_cal_m_vc(void);
extern bool METROLOGY_APP_bBack_up_cal_ph_ic(void);
extern bool METROLOGY_APP_bRestore_cal_m_ic_backup(void);
extern bool METROLOGY_APP_bRestore_cal_m_vc_backup(void);
extern bool METROLOGY_APP_bRestore_cal_ph_ic_backup(void);

extern void METROLOGY_APP_vChange_kt(uint32_t kt_value);
extern QUADRANT_TYPE METROLOGY_APP_xGet_total_energy_quadrant(void);
extern double METROLOGY_APP_dGet_instantaneous_value(RMS_TYPE inst_id);
extern void METROLOGY_APP_vCopy_instantaneous_struct(INSTANTANEOUS_VALUES_TYPE* inst_struct_ptr);
extern void METROLOGY_APP_vRefresh_inst_values(void);
extern void METROLOGY_APP_vCheck_task(void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
#endif /* METROLOGY_H_INCLUDED */
