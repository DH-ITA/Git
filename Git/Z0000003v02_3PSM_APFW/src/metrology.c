/**
 ******************************************************************************
 * \file metrology.c
 ******************************************************************************
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
 * This file contains all standard code file functions for metrology using ATSAM4CM core.
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
#include "arm_math.h"
#include "c1218.h"
#include "regs.h"
#include "tou.h"
#include "sm.h"
#include "system.h"
#include "metrology.h"
#include "config_meter.h"
#include "timer.h"

/*
 ******************************************************************************
 * File level pragmas                                                
 ******************************************************************************
 */ 

/*
 ******************************************************************************
 * Constant and Macro Definitions using #define
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
 */
METROLOGY_TYPE VMetrology;
INSTANTANEOUS_VALUES_TYPE xInstantaneous;

/*
 ************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ************************************************************************
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
static void METROLOGY_APP_vDsp_initialization(void);
static bool METROLOGY_APP_vDsp_ctrl_verification(void);
static void METROLOGY_APP_vLoad_dsp_ctrl_eeprom(void);
static void METROLOGY_APP_vLoad_dsp_ctrl_core_0(void);

static double METROLOGY_APP_dCalculate_PF_angle(int64_t p, int64_t q);
static double METROLOGY_APP_dCalculate_Ix_rms(uint64_t acc_ix, uint32_t nbr_samples);
static double METROLOGY_APP_dCalculate_Vx_rms(uint64_t acc_vx, uint32_t nbr_samples);
static double METROLOGY_APP_dCalculate_Px_rms(int64_t acc_px, uint32_t nbr_samples);
static double METROLOGY_APP_dCalculate_Qx_rms(int64_t acc_qx, uint32_t nbr_samples);

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */

/**
 ******************************************************************************
 * \fn void METROLOGY_APP_vInitialize(void)
 * \brief Initialize metrology task
 *
 * \param void
 * \retval
 ******************************************************************************
 */
void METROLOGY_APP_vInitialize(void) {
	
	/* Load Metrology Config*/
	METROLOGY_APP_vLoad_dsp_ctrl_eeprom();
	METROLOGY_APP_vDsp_ctrl_verification();

	/* Metrology initialization */
	METROLOGY_APP_vDsp_initialization();
	
	PIOC->PIO_MDER = PIO_MDER_P6;
}

/**
 ******************************************************************************
 * \fn void Set_DSP_CTRL_ST_CTRL( DSP_CTRL_ST_CTRL_TYPE iSStatus )
 * \brief set dsp ctrl st ctrl status.
 *
 * \param iSStatus: new status
 * \retval void
 ******************************************************************************
 */
void Set_DSP_CTRL_ST_CTRL(DSP_CTRL_ST_CTRL_TYPE iSStatus) {
	
    VMetrology.DSP_CTRL.STATE_CTRL.BIT.ST_CTRL = iSStatus;
    ( ( DSP_CTRL_TYPE * )( mem_reg_in ) )->STATE_CTRL.BIT.ST_CTRL = iSStatus;
}

/**
 ******************************************************************************
 * \fn void	METROLOGY_APP_vDsp_initialization(void)
 * \brief Initialize dsp
 *
 * \param void
 * \retval always true
 ******************************************************************************
 */
void METROLOGY_APP_vDsp_initialization(void) {
	
    uint32_t i;

    Set_DSP_CTRL_ST_CTRL( IsReset );

    for ( i = 1; i < ( DSP_CONTROL_SIZE ); i++ ) {
        *( ( uint32_t * )( mem_reg_in ) + i ) = *( ( &VMetrology.DSP_CTRL.STATE_CTRL.WORD ) + i );
    }
	
    Set_DSP_CTRL_ST_CTRL( IsInit );
    delay_ms( 100 );
    Set_DSP_CTRL_ST_CTRL( IsRun );

    ( ( DSP_ACC_TYPE * )( mem_acc_out ) )->ACC_T0 = VMetrology.DSP_ACC.ACC_T0;
    ( ( DSP_ACC_TYPE * )( mem_acc_out ) )->ACC_T1 = VMetrology.DSP_ACC.ACC_T1;
    ( ( DSP_ACC_TYPE * )( mem_acc_out ) )->ACC_T2 = VMetrology.DSP_ACC.ACC_T2;
}

/**
 ******************************************************************************
 * \fn void METROLOGY_APP_vCheck_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void METROLOGY_APP_vCheck_task(void) {
	
	if ( TIMER_is_ms_timer_expired(METROLOGY_CHECK_TIMER) ) {
		
		meter.metrology_over_timer++;

		if ( meter.metrology_over_timer >= 3 ) {
			
			meter.metrology_over_timer = 0;
			METROLOGY_APP_vLoad_dsp_ctrl_eeprom();
			METROLOGY_APP_vDsp_ctrl_verification();
			Init_Core1();
			METROLOGY_APP_vDsp_initialization();
		}

// 		METROLOGY_APP_vLoad_dsp_ctrl_core_0();
// 
// 		if ( !METROLOGY_APP_vDsp_ctrl_verification() ) {
// 
// 			Init_Core1();
// 			METROLOGY_APP_vDsp_initialization();
// 		}
		
		TIMER_start_ms_timer(METROLOGY_CHECK_TIMER, TIMER_ms_to_tic(1000), null_action);
	}
}

/**
 ******************************************************************************
 * \fn void	METROLOGY_APP_vLoad_dsp_ctrl_eeprom(void)
 * \brief DSP Load EEPROM Data
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void METROLOGY_APP_vLoad_dsp_ctrl_eeprom(void) {
	
    uint32_t i;
    uint8_t *ptr;
	
	if ( meter.power_failure == false ) {

		ptr = (uint8_t*)(&VMetrology.DSP_CTRL.STATE_CTRL.WORD);
	
		for ( i = 0; i < DSP_CONTROL_SIZE; i++ ) {

			EEPROM_reader( DSP_CTRL_STATE_CTRL_ADDR + ( i * 4 ), ptr + ( i * 4 ) , 4 );
		}
	}
}

/**
 ******************************************************************************
 * \fn void	METROLOGY_APP_vLoad_dsp_ctrl_core_0(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void METROLOGY_APP_vLoad_dsp_ctrl_core_0(void) {
	
    uint32_t i;
    uint8_t *ptr;
	
	if ( meter.power_failure == false ) {

		for ( i = 0; i < DSP_CONTROL_SIZE; i++ )
		{
			*( ( uint32_t * )( &VMetrology.DSP_CTRL.STATE_CTRL.WORD ) + i ) = *( ( uint32_t * )( mem_reg_in ) + i );
		}
	}
}

/**
 ******************************************************************************
 * \fn bool	METROLOGY_APP_vDsp_ctrl_verification(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_vDsp_ctrl_verification(void) {

	static bool data_ok = true;

	/* Default state is running */
	if ( VMetrology.DSP_CTRL.STATE_CTRL.WORD != (uint32_t)DSP_CTRL_ST_CTRL_RUN ) {

		VMetrology.DSP_CTRL.STATE_CTRL.WORD = (uint32_t)DSP_CTRL_ST_CTRL_RUN;
		EEPROM_writer(DSP_CTRL_STATE_CTRL_ADDR, &VMetrology.DSP_CTRL.STATE_CTRL.WORD, 4);
		data_ok = false;
	}

	/* Enable 1, 2 or 3 phases by default */
	if ( VMetrology.DSP_CTRL.FEATURE_CTRL0.WORD != (uint32_t)DEFAULT_DSP_CTRL_FEATURE_CTRL0 ) {

		VMetrology.DSP_CTRL.FEATURE_CTRL0.WORD = (uint32_t)DEFAULT_DSP_CTRL_FEATURE_CTRL0;
		EEPROM_writer(DSP_CTRL_FEATURE_CTRL0_ADDR, &VMetrology.DSP_CTRL.FEATURE_CTRL0.WORD, 4);
		data_ok = false;
	}

	/* Disable creep detection and harmonics by default */
	if ( VMetrology.DSP_CTRL.FEATURE_CTRL1.WORD != (uint32_t)DEFAULT_DSP_CTRL_FEATURE_CTRL1 ) {

		VMetrology.DSP_CTRL.FEATURE_CTRL1.WORD = (uint32_t)DEFAULT_DSP_CTRL_FEATURE_CTRL1;
		EEPROM_writer(DSP_CTRL_FEATURE_CTRL1_ADDR, &VMetrology.DSP_CTRL.FEATURE_CTRL1.WORD, 4);
		data_ok = false;
	}
	
	/* Current transformer meter by default */
	if ( VMetrology.DSP_CTRL.METER_TYPE.WORD != (uint32_t)DEFAULT_DSP_CTRL_METER_TYPE ) {

		VMetrology.DSP_CTRL.METER_TYPE.WORD = (uint32_t)DEFAULT_DSP_CTRL_METER_TYPE;
		EEPROM_writer(DSP_CTRL_METER_TYPE_ADDR, &VMetrology.DSP_CTRL.METER_TYPE.WORD, 4);
		data_ok = false;
	}
	
	/* 60Hz system by default */
	if ( VMetrology.DSP_CTRL.M.WORD != (uint32_t)DEFAULT_DSP_CTRL_M ) {

		VMetrology.DSP_CTRL.M.WORD = (uint32_t)DEFAULT_DSP_CTRL_M;
		EEPROM_writer(DSP_CTRL_M_ADDR, &VMetrology.DSP_CTRL.M.WORD, 4);
		data_ok = false;
	}
	
	/* ATMEL recommended value */
	if ( VMetrology.DSP_CTRL.N_MAX.WORD != (uint32_t)DEFAULT_DSP_CTRL_N_MAX ) {

		VMetrology.DSP_CTRL.N_MAX.WORD = (uint32_t)DEFAULT_DSP_CTRL_N_MAX;
		EEPROM_writer(DSP_CTRL_N_MAX_ADDR, &VMetrology.DSP_CTRL.N_MAX.WORD, 4);
		data_ok = false;
	}
	
	if ( meter.system_state.initialiting ) {
		
		/* Enable pulse at PC6 with active energy by default */
		if ( VMetrology.DSP_CTRL.PULSE0_CTRL.WORD != (uint32_t)DEFAULT_DSP_CTRL_PULSE0_CTRL ) {

			VMetrology.DSP_CTRL.PULSE0_CTRL.WORD = (uint32_t)DEFAULT_DSP_CTRL_PULSE0_CTRL;
			EEPROM_writer(DSP_CTRL_PULSE0_CTRL_ADDR, &VMetrology.DSP_CTRL.PULSE0_CTRL.WORD, 4);
			data_ok = false;
		}
		
		/* Disable pulse at PC7 by default */
		if ( VMetrology.DSP_CTRL.PULSE1_CTRL.WORD != (uint32_t)DEFAULT_DSP_CTRL_PULSE1_CTRL ) {

			VMetrology.DSP_CTRL.PULSE1_CTRL.WORD = (uint32_t)DEFAULT_DSP_CTRL_PULSE1_CTRL;
			EEPROM_writer(DSP_CTRL_PULSE1_CTRL_ADDR, &VMetrology.DSP_CTRL.PULSE1_CTRL.WORD, 4);
			data_ok = false;
		}

		/* Disable pulse at PC9 by default */
		if ( VMetrology.DSP_CTRL.PULSE2_CTRL.WORD != (uint32_t)DEFAULT_DSP_CTRL_PULSE2_CTRL ) {

			VMetrology.DSP_CTRL.PULSE2_CTRL.WORD = (uint32_t)DEFAULT_DSP_CTRL_PULSE2_CTRL;
			EEPROM_writer(DSP_CTRL_PULSE2_CTRL_ADDR, &VMetrology.DSP_CTRL.PULSE2_CTRL.WORD, 4);
			data_ok = false;
		}

		/* Active Power Pulse Constant */
		if ( VMetrology.DSP_CTRL.P_K_T != (uint32_t)DEFAULT_DSP_CTRL_P_K_T ) {

			VMetrology.DSP_CTRL.P_K_T = (uint32_t)DEFAULT_DSP_CTRL_P_K_T;
			EEPROM_writer(DSP_CTRL_P_K_T_ADDR, &VMetrology.DSP_CTRL.P_K_T, 4);
			data_ok = false;
		}
		
		/* Reactive Power Pulse Constant */
		if ( VMetrology.DSP_CTRL.Q_K_T != (uint32_t)DEFAULT_DSP_CTRL_Q_K_T ) {
			
			VMetrology.DSP_CTRL.Q_K_T = (uint32_t)DEFAULT_DSP_CTRL_Q_K_T;
			EEPROM_writer(DSP_CTRL_Q_K_T_ADDR, &VMetrology.DSP_CTRL.Q_K_T, 4);
			data_ok = false;
		}
		
		/* Current Pulse Constant */
		if ( VMetrology.DSP_CTRL.I_K_T != (uint32_t)DEFAULT_DSP_CTRL_I_K_T ) {
			
			VMetrology.DSP_CTRL.I_K_T = (uint32_t)DEFAULT_DSP_CTRL_I_K_T;
			EEPROM_writer(DSP_CTRL_I_K_T_ADDR, &VMetrology.DSP_CTRL.I_K_T, 4);
			data_ok = false;
		}
	}
	
	/* No creep detection */
	if ( VMetrology.DSP_CTRL.CREEP_THR_P != (uint32_t)DEFAULT_CREEP_THRESHOLD_P ) {
		
		VMetrology.DSP_CTRL.CREEP_THR_P = (uint32_t)DEFAULT_CREEP_THRESHOLD_P;
		EEPROM_writer(DSP_CTRL_CREEP_THR_P_ADDR, &VMetrology.DSP_CTRL.CREEP_THR_P, 4);
		data_ok = false;
	}
	
	/* No creep detection */
	if ( VMetrology.DSP_CTRL.CREEP_THR_Q != (uint32_t)DEFAULT_CREEP_THRESHOLD_Q ) {

		VMetrology.DSP_CTRL.CREEP_THR_Q = (uint32_t)DEFAULT_CREEP_THRESHOLD_Q;
		EEPROM_writer(DSP_CTRL_CREEP_THR_Q_ADDR, &VMetrology.DSP_CTRL.CREEP_THR_Q, 4);
		data_ok = false;
	}
	
	/* No creep detection */
	if ( VMetrology.DSP_CTRL.CREEP_THR_I != (uint32_t)DEFAULT_CREEP_THRESHOLD_I((double)(DEFAULT_DSP_CTRL_K_Ix/K_Ix_MULTIPLIER)) ) {
		
		VMetrology.DSP_CTRL.CREEP_THR_I = (uint32_t)DEFAULT_CREEP_THRESHOLD_I((double)(DEFAULT_DSP_CTRL_K_Ix/K_Ix_MULTIPLIER));
		EEPROM_writer(DSP_CTRL_CREEP_THR_I_ADDR, &VMetrology.DSP_CTRL.CREEP_THR_I, 4);
		data_ok = false;
	}
	
	/* No power offset adjustment by default */
	if ( VMetrology.DSP_CTRL.POWER_OFFSET_CTRL.WORD != (uint32_t)DEFAULT_DSP_CTRL_POWER_OFFSET_CTRL ) {

		VMetrology.DSP_CTRL.POWER_OFFSET_CTRL.WORD = (uint32_t)DEFAULT_DSP_CTRL_POWER_OFFSET_CTRL;
		EEPROM_writer(DSP_CTRL_POWER_OFFSET_CTRL_ADDR, &VMetrology.DSP_CTRL.POWER_OFFSET_CTRL.WORD, 4);
		data_ok = false;
	}

	/* No power offset adjustment */
	if ( VMetrology.DSP_CTRL.POWER_OFFSET_CTRL.BIT.ACTIVE_PO_EN == 0 ) {

		if ( VMetrology.DSP_CTRL.POWER_OFFSET_P != (uint32_t)DEFAULT_DSP_CTRL_POWER_OFFSET_P ) {

			VMetrology.DSP_CTRL.POWER_OFFSET_P = (uint32_t)DEFAULT_DSP_CTRL_POWER_OFFSET_P;
			EEPROM_writer(DSP_CTRL_POWER_OFFSET_P_ADDR, &VMetrology.DSP_CTRL.POWER_OFFSET_P, 4);
			data_ok = false;
		}
	}
	
	/* No power offset adjustment */
	if ( VMetrology.DSP_CTRL.POWER_OFFSET_CTRL.BIT.REACTIVE_PO_EN == 0 ) {

		if ( VMetrology.DSP_CTRL.POWER_OFFSET_Q != DEFAULT_DSP_CTRL_POWER_OFFSET_Q ) {

			VMetrology.DSP_CTRL.POWER_OFFSET_Q = (uint32_t)DEFAULT_DSP_CTRL_POWER_OFFSET_Q;
			EEPROM_writer(DSP_CTRL_POWER_OFFSET_Q_ADDR, &VMetrology.DSP_CTRL.POWER_OFFSET_Q, 4);
			data_ok = false;
		}
	}
	
	/* If not valid swell value */
	if ( VMetrology.DSP_CTRL.SWELL_THR_VA != (uint32_t)DEFAULT_DSP_CTRL_SWELL_THRESHOLD_Vx ) {

		VMetrology.DSP_CTRL.SWELL_THR_VA = (uint32_t)DEFAULT_DSP_CTRL_SWELL_THRESHOLD_Vx;
		EEPROM_writer(DSP_CTRL_SWELL_THR_VA_ADDR, &VMetrology.DSP_CTRL.SWELL_THR_VA, 4);
		data_ok = false;
	}
	
	/* If not valid swell value */
	if ( VMetrology.DSP_CTRL.SWELL_THR_VB != (uint32_t)DEFAULT_DSP_CTRL_SWELL_THRESHOLD_Vx ) {

		VMetrology.DSP_CTRL.SWELL_THR_VB = (uint32_t)DEFAULT_DSP_CTRL_SWELL_THRESHOLD_Vx;
		EEPROM_writer(DSP_CTRL_SWELL_THR_VB_ADDR, &VMetrology.DSP_CTRL.SWELL_THR_VB, 4);
		data_ok = false;
	}

	/* If not valid swell value */
	if ( VMetrology.DSP_CTRL.SWELL_THR_VC != (uint32_t)DEFAULT_DSP_CTRL_SWELL_THRESHOLD_Vx ) {

		VMetrology.DSP_CTRL.SWELL_THR_VC = (uint32_t)DEFAULT_DSP_CTRL_SWELL_THRESHOLD_Vx;
		EEPROM_writer(DSP_CTRL_SWELL_THR_VC_ADDR, &VMetrology.DSP_CTRL.SWELL_THR_VC, 4);
		data_ok = false;
	}
	
	/* If not valid sag value */
	if ( VMetrology.DSP_CTRL.SAG_THR_VA != (uint32_t)DEFAULT_DSP_CTRL_SAG_THRESHOLD_Vx ) {

		VMetrology.DSP_CTRL.SAG_THR_VA = (uint32_t)DEFAULT_DSP_CTRL_SAG_THRESHOLD_Vx;
		EEPROM_writer(DSP_CTRL_SAG_THR_VA_ADDR, &VMetrology.DSP_CTRL.SAG_THR_VA, 4);
		data_ok = false;
	}
	
	/* If not valid sag value */
	if ( VMetrology.DSP_CTRL.SAG_THR_VB != (uint32_t)DEFAULT_DSP_CTRL_SAG_THRESHOLD_Vx ) {

		VMetrology.DSP_CTRL.SAG_THR_VB = (uint32_t)DEFAULT_DSP_CTRL_SAG_THRESHOLD_Vx;
		EEPROM_writer(DSP_CTRL_SAG_THR_VB_ADDR, &VMetrology.DSP_CTRL.SAG_THR_VB, 4);
		data_ok = false;
	}
	
	/* If not valid sag value */
	if ( VMetrology.DSP_CTRL.SAG_THR_VC != (uint32_t)DEFAULT_DSP_CTRL_SAG_THRESHOLD_Vx ) {

		VMetrology.DSP_CTRL.SAG_THR_VC = (uint32_t)DEFAULT_DSP_CTRL_SAG_THRESHOLD_Vx;
		EEPROM_writer(DSP_CTRL_SAG_THR_VC_ADDR, &VMetrology.DSP_CTRL.SAG_THR_VC, 4);
		data_ok = false;
	}
	
	/* Reserved must be zero */
	if ( ( VMetrology.DSP_CTRL.RESERVED1 | VMetrology.DSP_CTRL.RESERVED2 | VMetrology.DSP_CTRL.RESERVED3 | \
	       VMetrology.DSP_CTRL.RESERVED4 | VMetrology.DSP_CTRL.RESERVED5 | VMetrology.DSP_CTRL.RESERVED6 | \
		   VMetrology.DSP_CTRL.RESERVED7 | VMetrology.DSP_CTRL.RESERVED8 | VMetrology.DSP_CTRL.RESERVED9 | \
		   VMetrology.DSP_CTRL.RESERVED10 ) != 0 ) {
		
		VMetrology.DSP_CTRL.RESERVED1 = 0;
		EEPROM_writer(DSP_CTRL_RESERVED1_ADDR, &VMetrology.DSP_CTRL.RESERVED1, 4);
		VMetrology.DSP_CTRL.RESERVED2 = 0;
		EEPROM_writer(DSP_CTRL_RESERVED2_ADDR, &VMetrology.DSP_CTRL.RESERVED2, 4);
		VMetrology.DSP_CTRL.RESERVED3 = 0;
		EEPROM_writer(DSP_CTRL_RESERVED3_ADDR, &VMetrology.DSP_CTRL.RESERVED3, 4);
		VMetrology.DSP_CTRL.RESERVED4 = 0;
		EEPROM_writer(DSP_CTRL_RESERVED4_ADDR, &VMetrology.DSP_CTRL.RESERVED4, 4);
		VMetrology.DSP_CTRL.RESERVED5 = 0;
		EEPROM_writer(DSP_CTRL_RESERVED5_ADDR, &VMetrology.DSP_CTRL.RESERVED5, 4);
		VMetrology.DSP_CTRL.RESERVED6 = 0;
		EEPROM_writer(DSP_CTRL_RESERVED6_ADDR, &VMetrology.DSP_CTRL.RESERVED6, 4);
		VMetrology.DSP_CTRL.RESERVED7 = 0;
		EEPROM_writer(DSP_CTRL_RESERVED7_ADDR, &VMetrology.DSP_CTRL.RESERVED7, 4);
		VMetrology.DSP_CTRL.RESERVED8 = 0;
		EEPROM_writer(DSP_CTRL_RESERVED8_ADDR, &VMetrology.DSP_CTRL.RESERVED8, 4);
		VMetrology.DSP_CTRL.RESERVED9 = 0;
		EEPROM_writer(DSP_CTRL_RESERVED9_ADDR, &VMetrology.DSP_CTRL.RESERVED9, 4);
		VMetrology.DSP_CTRL.RESERVED10 = 0;
		EEPROM_writer(DSP_CTRL_RESERVED10_ADDR, &VMetrology.DSP_CTRL.RESERVED10, 4);
		data_ok = false;
	}
	
	/* ATSENSE ADC Control Register 0 */
	if ( VMetrology.DSP_CTRL.ATsense_CTRL_20_23.WORD != (uint32_t)DEFAULT_DSP_CTRL_ATSENSE_CTRL_20_23_MASK ) {

		VMetrology.DSP_CTRL.ATsense_CTRL_20_23.WORD = (uint32_t)DEFAULT_DSP_CTRL_ATSENSE_CTRL_20_23_MASK;
		EEPROM_writer(DSP_CTRL_ATSENSE_CTRL_20_23_ADDR, &VMetrology.DSP_CTRL.ATsense_CTRL_20_23.WORD, 4);
		data_ok = false;
	}

	/* ATSENSE ADC Control Register 1 */
	if ( VMetrology.DSP_CTRL.ATsense_CTRL_24_27.WORD != (uint32_t)DEFAULT_DSP_CTRL_ATSENSE_CTRL_24_27_MASK ) {

		VMetrology.DSP_CTRL.ATsense_CTRL_24_27.WORD = (uint32_t)DEFAULT_DSP_CTRL_ATSENSE_CTRL_24_27_MASK;
		EEPROM_writer(DSP_CTRL_ATSENSE_CTRL_24_27_ADDR, &VMetrology.DSP_CTRL.ATsense_CTRL_24_27.WORD, 4);
		data_ok = false;
	}

	/* Enable EMAFE for phase A, B and C */
	while ( VMetrology.DSP_CTRL.ATsense_CTRL_28_2B.WORD != (uint32_t)DEFAULT_DSP_CTRL_ATSENSE_CTRL_28_2B_MASK ) {

		VMetrology.DSP_CTRL.ATsense_CTRL_28_2B.WORD = (uint32_t)DEFAULT_DSP_CTRL_ATSENSE_CTRL_28_2B_MASK;
		EEPROM_writer(DSP_CTRL_ATSENSE_CTRL_28_2B_ADDR, &VMetrology.DSP_CTRL.ATsense_CTRL_28_2B.WORD, 4);
		data_ok = false;
	}
	
	if ( (VMetrology.DSP_CTRL.CAL_M_IA == 0xFFFFFFFF) || (VMetrology.DSP_CTRL.CAL_M_IA == 0) ) {

		VMetrology.DSP_CTRL.CAL_M_IA = (uint32_t)0x20000000;
		EEPROM_writer(DSP_CTRL_CAL_M_IA_ADDR, &VMetrology.DSP_CTRL.CAL_M_IA, 4);
		data_ok = false;
	}

	if ( (VMetrology.DSP_CTRL.CAL_M_IB == 0xFFFFFFFF) || (VMetrology.DSP_CTRL.CAL_M_IB == 0) ) {

		VMetrology.DSP_CTRL.CAL_M_IB = (uint32_t)0x20000000;
		EEPROM_writer(DSP_CTRL_CAL_M_IB_ADDR, &VMetrology.DSP_CTRL.CAL_M_IB, 4);
		data_ok = false;
	}

	if ( (VMetrology.DSP_CTRL.CAL_M_IC == 0xFFFFFFFF) || (VMetrology.DSP_CTRL.CAL_M_IC == 0) ) {

		VMetrology.DSP_CTRL.CAL_M_IC = (uint32_t)0x20000000;
		EEPROM_writer(DSP_CTRL_CAL_M_IC_ADDR, &VMetrology.DSP_CTRL.CAL_M_IC, 4);
		data_ok = false;
	}
	
	if ( (VMetrology.DSP_CTRL.CAL_M_VA == 0xFFFFFFFF) || (VMetrology.DSP_CTRL.CAL_M_VA == 0) ) {

		VMetrology.DSP_CTRL.CAL_M_VA = (uint32_t)0x20000000;
		EEPROM_writer(DSP_CTRL_CAL_M_VA_ADDR, &VMetrology.DSP_CTRL.CAL_M_VA, 4);
		data_ok = false;
	}

	if ( (VMetrology.DSP_CTRL.CAL_M_VB == 0xFFFFFFFF) || (VMetrology.DSP_CTRL.CAL_M_VB == 0) ) {

		VMetrology.DSP_CTRL.CAL_M_VB = (uint32_t)0x20000000;
		EEPROM_writer(DSP_CTRL_CAL_M_VB_ADDR, &VMetrology.DSP_CTRL.CAL_M_VB, 4);
		data_ok = false;
	}

	if ( (VMetrology.DSP_CTRL.CAL_M_VC == 0xFFFFFFFF) || (VMetrology.DSP_CTRL.CAL_M_VC == 0) ) {

		VMetrology.DSP_CTRL.CAL_M_VC = (uint32_t)0x20000000;
		EEPROM_writer(DSP_CTRL_CAL_M_VC_ADDR, &VMetrology.DSP_CTRL.CAL_M_VC, 4);
		data_ok = false;
	}
	
	if ( VMetrology.DSP_CTRL.CAL_PH_IA == 0xFFFFFFFF ) {

		VMetrology.DSP_CTRL.CAL_PH_IA = 0;
		EEPROM_writer(DSP_CTRL_CAL_PH_IA_ADDR, &VMetrology.DSP_CTRL.CAL_PH_IA, 4);
		data_ok = false;
	}

 	if ( VMetrology.DSP_CTRL.CAL_PH_IB == 0xFFFFFFFF ) {

		VMetrology.DSP_CTRL.CAL_PH_IB = 0;
		EEPROM_writer(DSP_CTRL_CAL_PH_IB_ADDR, &VMetrology.DSP_CTRL.CAL_PH_IB, 4);
		data_ok = false;
	}

 	if ( VMetrology.DSP_CTRL.CAL_PH_IC == 0xFFFFFFFF ) {

		VMetrology.DSP_CTRL.CAL_PH_IC = 0;
		EEPROM_writer(DSP_CTRL_CAL_PH_IC_ADDR, &VMetrology.DSP_CTRL.CAL_PH_IC, 4);
		data_ok = false;
	}
	
	if ( VMetrology.DSP_CTRL.CAL_PH_VA != 0 ) {

		VMetrology.DSP_CTRL.CAL_PH_VA = 0;
		EEPROM_writer(DSP_CTRL_CAL_PH_VA_ADDR, &VMetrology.DSP_CTRL.CAL_PH_VA, 4);
		data_ok = false;
	}
	
	if ( VMetrology.DSP_CTRL.CAL_PH_VB != 0 ) {

		VMetrology.DSP_CTRL.CAL_PH_VB = 0;
		EEPROM_writer(DSP_CTRL_CAL_PH_VB_ADDR, &VMetrology.DSP_CTRL.CAL_PH_VB, 4);
		data_ok = false;
	}
	
	if ( VMetrology.DSP_CTRL.CAL_PH_VC != 0 ) {

		VMetrology.DSP_CTRL.CAL_PH_VC = 0;
		EEPROM_writer(DSP_CTRL_CAL_PH_VC_ADDR, &VMetrology.DSP_CTRL.CAL_PH_VC, 4);
		data_ok = false;
	}
	
	/* Default current constant */
	if ( VMetrology.DSP_CTRL.K_IA != (uint32_t)DEFAULT_DSP_CTRL_K_Ix ) {

		VMetrology.DSP_CTRL.K_IA = (uint32_t)DEFAULT_DSP_CTRL_K_Ix;
		EEPROM_writer(DSP_CTRL_K_IA_ADDR, &VMetrology.DSP_CTRL.K_IA, 4);
		data_ok = false;
	}
	
	/* Default current constant */
	if ( VMetrology.DSP_CTRL.K_IB != (uint32_t)DEFAULT_DSP_CTRL_K_Ix ) {
		
		VMetrology.DSP_CTRL.K_IB = (uint32_t)DEFAULT_DSP_CTRL_K_Ix;
		EEPROM_writer(DSP_CTRL_K_IB_ADDR, &VMetrology.DSP_CTRL.K_IB, 4);
		data_ok = false;
	}
	
	/* Default current constant */
	if ( VMetrology.DSP_CTRL.K_IC != (uint32_t)DEFAULT_DSP_CTRL_K_Ix ) {

		VMetrology.DSP_CTRL.K_IC = (uint32_t)DEFAULT_DSP_CTRL_K_Ix;
		EEPROM_writer(DSP_CTRL_K_IC_ADDR, &VMetrology.DSP_CTRL.K_IC, 4);
		data_ok = false;
	}
	
	/* Default voltage constant */
	if ( VMetrology.DSP_CTRL.K_VA != (uint32_t)DEFAULT_DSP_CTRL_K_Vx ) {

		VMetrology.DSP_CTRL.K_VA = (uint32_t)DEFAULT_DSP_CTRL_K_Vx;
		EEPROM_writer(DSP_CTRL_K_VA_ADDR, &VMetrology.DSP_CTRL.K_VA, 4);
		data_ok = false;
	}
	
	/* Default voltage constant */
	if ( VMetrology.DSP_CTRL.K_VB != (uint32_t)DEFAULT_DSP_CTRL_K_Vx ) {

		VMetrology.DSP_CTRL.K_VB = (uint32_t)DEFAULT_DSP_CTRL_K_Vx;
		EEPROM_writer(DSP_CTRL_K_VB_ADDR, &VMetrology.DSP_CTRL.K_VB, 4);
		data_ok = false;
	}
	
	/* Default voltage constant */
	if ( VMetrology.DSP_CTRL.K_VC != (uint32_t)DEFAULT_DSP_CTRL_K_Vx ) {

		VMetrology.DSP_CTRL.K_VC = (uint32_t)DEFAULT_DSP_CTRL_K_Vx;
		EEPROM_writer(DSP_CTRL_K_VC_ADDR, &VMetrology.DSP_CTRL.K_VC, 4);
		data_ok = false;
	}

	return data_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bCalibrate_m_ia(uint32_t mA_in)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bCalibrate_m_ia(uint32_t mA_in) {
	
	double cal_m_i;
	double i_rms;

	cal_m_i = (double)(mA_in * pow(2, 29));
	cal_m_i /= METROLOGY_APP_dCalculate_Ix_rms(VMetrology.DSP_ACC.I_A, VMetrology.DSP_ST.N);

	return METROLOGY_APP_bWrite_cal_m_ia((uint32_t)cal_m_i);
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bCalibrate_m_va(uint32_t mV_in)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bCalibrate_m_va(uint32_t mV_in) {
	
	double cal_m_v;
	double v_rms;

	cal_m_v = (double)(mV_in * pow(2, 29));
	cal_m_v /= METROLOGY_APP_dCalculate_Vx_rms(VMetrology.DSP_ACC.V_A, VMetrology.DSP_ST.N);

	return METROLOGY_APP_bWrite_cal_m_va((uint32_t)cal_m_v);
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bCalibrate_ph_ia(uint32_t ph_error)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bCalibrate_ph_ia(uint32_t ph_error) {
	
	volatile double cal_ph_i;

	cal_ph_i = (double)((int32_t)ph_error * pow(2, 31)) / (double)(100*1.7320508*CONST_Pi) * 60 / 60;
	cal_ph_i /= 1000;
	cal_ph_i *= (-1);

	return METROLOGY_APP_bWrite_cal_ph_ia((int32_t)cal_ph_i);
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bWrite_cal_m_ia(uint32_t adjust_value)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bWrite_cal_m_ia(uint32_t adjust_value) {
	
	bool is_ok = false;
	uint32_t comp_value;

	#if NBR_PHASES >= 1
	
		VMetrology.DSP_CTRL.CAL_M_IA = (uint32_t)(adjust_value);
		EEPROM_writer(DSP_CTRL_CAL_M_IA_ADDR, (uint8_t*)(&VMetrology.DSP_CTRL.CAL_M_IA), 4);
		((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_IA = VMetrology.DSP_CTRL.CAL_M_IA;
	
		EEPROM_reader(DSP_CTRL_CAL_M_IA_ADDR, (uint8_t*)(&comp_value), 4);
	
		is_ok = (comp_value == adjust_value) ? true : false;
		
	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bWrite_cal_m_va(uint32_t adjust_value)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bWrite_cal_m_va(uint32_t adjust_value) {
	
	bool is_ok = false;
	uint32_t comp_value;

	#if NBR_PHASES >= 1
	
		VMetrology.DSP_CTRL.CAL_M_VA = (uint32_t)adjust_value;
		EEPROM_writer(DSP_CTRL_CAL_M_VA_ADDR, (uint8_t*)(&VMetrology.DSP_CTRL.CAL_M_VA), 4);
		((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_VA = VMetrology.DSP_CTRL.CAL_M_VA;
	
		EEPROM_reader(DSP_CTRL_CAL_M_VA_ADDR, (uint8_t*)(&comp_value), 4);
	
		is_ok = (comp_value == adjust_value) ? true : false;
		
	#endif
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bWrite_cal_ph_ia(uint32_t adjust_value)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bWrite_cal_ph_ia(uint32_t adjust_value) {
	
	bool is_ok = false;
	uint32_t comp_value;
	
	#if NBR_PHASES >= 1
	
		VMetrology.DSP_CTRL.CAL_PH_IA = (uint32_t)adjust_value;
		VMetrology.DSP_CTRL.CAL_PH_VA = 0;
		EEPROM_writer(DSP_CTRL_CAL_PH_IA_ADDR, (uint8_t*)(&VMetrology.DSP_CTRL.CAL_PH_IA), 4);
		EEPROM_writer(DSP_CTRL_CAL_PH_VA_ADDR, (uint8_t*)(&VMetrology.DSP_CTRL.CAL_PH_VA), 4);
		((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_PH_IA = VMetrology.DSP_CTRL.CAL_PH_IA;
		((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_PH_VA = VMetrology.DSP_CTRL.CAL_PH_VA;
	
		EEPROM_reader(DSP_CTRL_CAL_PH_IA_ADDR, (uint8_t*)(&comp_value), 4);
	
		is_ok = (comp_value == adjust_value) ? true : false;
		
	#endif
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bBack_up_cal_m_ia(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bBack_up_cal_m_ia(void) {
	
	bool is_ok = false;
	
	#if NBR_PHASES >= 1

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {

		EEPROM_reader(DSP_CTRL_CAL_M_IA_ADDR, (uint8_t*)(&nvm_value), 4);
		
		if ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_IA ) {
			
			EEPROM_writer(DSP_CTRL_CAL_M_IA_COPY_ADDR, &nvm_value, sizeof(nvm_value));
			nvm_value = 0;
			EEPROM_reader(DSP_CTRL_CAL_M_IA_COPY_ADDR, &nvm_value, sizeof(nvm_value));

			is_ok = ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_IA ) ? true : false;
		}

		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bBack_up_cal_m_va(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bBack_up_cal_m_va(void) {
	
	bool is_ok = false;
	
	#if NBR_PHASES >= 1

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {

		EEPROM_reader(DSP_CTRL_CAL_M_VA_ADDR, (uint8_t*)(&nvm_value), 4);
		
		if ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_VA ) {
			
			EEPROM_writer(DSP_CTRL_CAL_M_VA_COPY_ADDR, &nvm_value, sizeof(nvm_value));
			nvm_value = 0;
			EEPROM_reader(DSP_CTRL_CAL_M_VA_COPY_ADDR, &nvm_value, sizeof(nvm_value));

			is_ok = ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_VA ) ? true : false;
		}

		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bBack_up_cal_ph_ia(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bBack_up_cal_ph_ia(void) {
	
	bool is_ok = false;
	
	#if NBR_PHASES >= 1

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {

		EEPROM_reader(DSP_CTRL_CAL_PH_IA_ADDR, (uint8_t*)(&nvm_value), 4);
		
		if ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_PH_IA ) {
			
			EEPROM_writer(DSP_CTRL_CAL_PH_IA_COPY_ADDR, &nvm_value, sizeof(nvm_value));
			nvm_value = 0;
			EEPROM_reader(DSP_CTRL_CAL_PH_IA_COPY_ADDR, &nvm_value, sizeof(nvm_value));

			is_ok = ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_PH_IA ) ? true : false;
		}

		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bRestore_cal_m_ia_backup(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bRestore_cal_m_ia_backup(void) {
	
	bool is_ok = false; 
	
	#if NBR_PHASES >= 1

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {
		
		EEPROM_reader(DSP_CTRL_CAL_M_IA_COPY_ADDR, (uint8_t*)(&nvm_value), 4);
		is_ok = METROLOGY_APP_bWrite_cal_m_ia(nvm_value);
		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bRestore_cal_m_va_backup(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bRestore_cal_m_va_backup(void) {
	
	bool is_ok = false; 
	
	#if NBR_PHASES >= 1

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {
		
		EEPROM_reader(DSP_CTRL_CAL_M_VA_COPY_ADDR, (uint8_t*)(&nvm_value), 4);
		is_ok = METROLOGY_APP_bWrite_cal_m_va(nvm_value);
		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bRestore_cal_ph_ia_backup(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bRestore_cal_ph_ia_backup(void) {
	
	bool is_ok = false; 
	
	#if NBR_PHASES >= 1

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {
		
		EEPROM_reader(DSP_CTRL_CAL_PH_IA_COPY_ADDR, (uint8_t*)(&nvm_value), 4);
		is_ok = METROLOGY_APP_bWrite_cal_ph_ia(nvm_value);
		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bCalibrate_m_ib(uint32_t mA_in)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bCalibrate_m_ib(uint32_t mA_in) {
	
	double cal_m_i;
	double i_rms;

	cal_m_i = (double)(mA_in * pow(2, 29));
	cal_m_i /= METROLOGY_APP_dCalculate_Ix_rms(VMetrology.DSP_ACC.I_B, VMetrology.DSP_ST.N);

	return METROLOGY_APP_bWrite_cal_m_ib((uint32_t)cal_m_i);
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bCalibrate_m_vb(uint32_t mV_in)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bCalibrate_m_vb(uint32_t mV_in) {
	
	double cal_m_v;
	double v_rms;

	cal_m_v = (double)(mV_in * pow(2, 29));
	cal_m_v /= METROLOGY_APP_dCalculate_Vx_rms(VMetrology.DSP_ACC.V_B, VMetrology.DSP_ST.N);

	return METROLOGY_APP_bWrite_cal_m_vb((uint32_t)cal_m_v);
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bCalibrate_ph_ib(uint32_t ph_error)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bCalibrate_ph_ib(uint32_t ph_error) {
	
	volatile double cal_ph_i;

	cal_ph_i = (double)((int32_t)ph_error * pow(2, 31)) / (double)(100*1.7320508*CONST_Pi) * 60 / 60;
	cal_ph_i /= 1000;
	cal_ph_i *= (-1);

	return METROLOGY_APP_bWrite_cal_ph_ib((int32_t)cal_ph_i);
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bWrite_cal_m_ib(uint32_t adjust_value)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bWrite_cal_m_ib(uint32_t adjust_value) {
	
	bool is_ok = false;
	uint32_t comp_value;

	#if NBR_PHASES >= 2
	
		VMetrology.DSP_CTRL.CAL_M_IB = (uint32_t)(adjust_value);
		EEPROM_writer(DSP_CTRL_CAL_M_IB_ADDR, (uint8_t*)(&VMetrology.DSP_CTRL.CAL_M_IB), 4);
		((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_IB = VMetrology.DSP_CTRL.CAL_M_IB;
	
		EEPROM_reader(DSP_CTRL_CAL_M_IB_ADDR, (uint8_t*)(&comp_value), 4);
	
		is_ok = (comp_value == adjust_value) ? true : false;
		
	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bWrite_cal_m_vb(uint32_t adjust_value)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bWrite_cal_m_vb(uint32_t adjust_value) {
	
	bool is_ok = false;
	uint32_t comp_value;

	#if NBR_PHASES >= 2
	
		VMetrology.DSP_CTRL.CAL_M_VB = (uint32_t)adjust_value;
		EEPROM_writer(DSP_CTRL_CAL_M_VB_ADDR, (uint8_t*)(&VMetrology.DSP_CTRL.CAL_M_VB), 4);
		((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_VB = VMetrology.DSP_CTRL.CAL_M_VB;
	
		EEPROM_reader(DSP_CTRL_CAL_M_VB_ADDR, (uint8_t*)(&comp_value), 4);
	
		if (comp_value == adjust_value) {
			
			is_ok = true;
		}
		
	#endif
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bWrite_cal_ph_ib(uint32_t adjust_value)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bWrite_cal_ph_ib(uint32_t adjust_value) {
	
	bool is_ok = false;
	uint32_t comp_value;
	
	#if NBR_PHASES >= 2
	
		VMetrology.DSP_CTRL.CAL_PH_IB = (uint32_t)adjust_value;
		VMetrology.DSP_CTRL.CAL_PH_VB = 0;
		EEPROM_writer(DSP_CTRL_CAL_PH_IB_ADDR, (uint8_t*)(&VMetrology.DSP_CTRL.CAL_PH_IB), 4);
		EEPROM_writer(DSP_CTRL_CAL_PH_VB_ADDR, (uint8_t*)(&VMetrology.DSP_CTRL.CAL_PH_VB), 4);
		((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_PH_IB = VMetrology.DSP_CTRL.CAL_PH_IB;
		((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_PH_VB = VMetrology.DSP_CTRL.CAL_PH_VB;
	
		EEPROM_reader(DSP_CTRL_CAL_PH_IB_ADDR, (uint8_t*)(&comp_value), 4);
	
		is_ok = (comp_value == adjust_value) ? true : false;
		
	#endif
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bBack_up_cal_m_ib(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bBack_up_cal_m_ib(void) {
	
	bool is_ok = false;
	
	#if NBR_PHASES >= 2

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {

		EEPROM_reader(DSP_CTRL_CAL_M_IB_ADDR, (uint8_t*)(&nvm_value), 4);
		
		if ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_IB ) {
			
			EEPROM_writer(DSP_CTRL_CAL_M_IB_COPY_ADDR, &nvm_value, sizeof(nvm_value));
			nvm_value = 0;
			EEPROM_reader(DSP_CTRL_CAL_M_IB_COPY_ADDR, &nvm_value, sizeof(nvm_value));

			is_ok = ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_IB ) ? true : false;
		}

		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bBack_up_cal_m_vb(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bBack_up_cal_m_vb(void) {
	
	bool is_ok = false;
	
	#if NBR_PHASES >= 2

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {

		EEPROM_reader(DSP_CTRL_CAL_M_VB_ADDR, (uint8_t*)(&nvm_value), 4);
		
		if ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_VB ) {
			
			EEPROM_writer(DSP_CTRL_CAL_M_VB_COPY_ADDR, &nvm_value, sizeof(nvm_value));
			nvm_value = 0;
			EEPROM_reader(DSP_CTRL_CAL_M_VB_COPY_ADDR, &nvm_value, sizeof(nvm_value));

			is_ok = ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_VB ) ? true : false;
		}

		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bBack_up_cal_ph_ib(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bBack_up_cal_ph_ib(void) {
	
	bool is_ok = false;
	
	#if NBR_PHASES >= 2

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {

		EEPROM_reader(DSP_CTRL_CAL_PH_IB_ADDR, (uint8_t*)(&nvm_value), 4);
		
		if ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_PH_IB ) {
			
			EEPROM_writer(DSP_CTRL_CAL_PH_IB_COPY_ADDR, &nvm_value, sizeof(nvm_value));
			nvm_value = 0;
			EEPROM_reader(DSP_CTRL_CAL_PH_IB_COPY_ADDR, &nvm_value, sizeof(nvm_value));

			is_ok = ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_PH_IB ) ? true : false;
		}

		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bRestore_cal_m_ib_backup(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bRestore_cal_m_ib_backup(void) {
	
	bool is_ok = false; 
	
	#if NBR_PHASES >= 2

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {
		
		EEPROM_reader(DSP_CTRL_CAL_M_IB_COPY_ADDR, (uint8_t*)(&nvm_value), 4);
		is_ok = METROLOGY_APP_bWrite_cal_m_ib(nvm_value);
		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bRestore_cal_m_vb_backup(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bRestore_cal_m_vb_backup(void) {
	
	bool is_ok = false; 
	
	#if NBR_PHASES >= 2

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {
		
		EEPROM_reader(DSP_CTRL_CAL_M_VB_COPY_ADDR, (uint8_t*)(&nvm_value), 4);
		is_ok = METROLOGY_APP_bWrite_cal_m_vb(nvm_value);
		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bRestore_cal_ph_ib_backup(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bRestore_cal_ph_ib_backup(void) {
	
	bool is_ok = false; 
	
	#if NBR_PHASES >= 2

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {
		
		EEPROM_reader(DSP_CTRL_CAL_PH_IB_COPY_ADDR, (uint8_t*)(&nvm_value), 4);
		is_ok = METROLOGY_APP_bWrite_cal_ph_ib(nvm_value);
		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bCalibrate_m_ic(uint32_t mA_in)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bCalibrate_m_ic(uint32_t mA_in) {
	
	double cal_m_i;
	double i_rms;

	cal_m_i = (double)(mA_in * pow(2, 29));
	cal_m_i /= METROLOGY_APP_dCalculate_Ix_rms(VMetrology.DSP_ACC.I_C, VMetrology.DSP_ST.N);

	return METROLOGY_APP_bWrite_cal_m_ic((uint32_t)cal_m_i);
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bCalibrate_m_vc(uint32_t mV_in)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bCalibrate_m_vc(uint32_t mV_in) {
	
	double cal_m_v;
	double v_rms;

	cal_m_v = (double)(mV_in * pow(2, 29));
	cal_m_v /= METROLOGY_APP_dCalculate_Vx_rms(VMetrology.DSP_ACC.V_C, VMetrology.DSP_ST.N);

	return METROLOGY_APP_bWrite_cal_m_vc((uint32_t)cal_m_v);
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bCalibrate_ph_ic(uint32_t ph_error)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bCalibrate_ph_ic(uint32_t ph_error) {
	
	volatile double cal_ph_i;

	cal_ph_i = (double)((int32_t)ph_error * pow(2, 31)) / (double)(100*1.7320508*CONST_Pi) * 60 / 60;
	cal_ph_i /= 1000;
	cal_ph_i *= (-1);

	return METROLOGY_APP_bWrite_cal_ph_ic((int32_t)cal_ph_i);
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bWrite_cal_m_ic(uint32_t adjust_value)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bWrite_cal_m_ic(uint32_t adjust_value) {
	
	bool is_ok = false;
	uint32_t comp_value;

	#if NBR_PHASES >= 3
	
		VMetrology.DSP_CTRL.CAL_M_IC = (uint32_t)(adjust_value);
		EEPROM_writer(DSP_CTRL_CAL_M_IC_ADDR, (uint8_t*)(&VMetrology.DSP_CTRL.CAL_M_IC), 4);
		((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_IC = VMetrology.DSP_CTRL.CAL_M_IC;
	
		EEPROM_reader(DSP_CTRL_CAL_M_IC_ADDR, (uint8_t*)(&comp_value), 4);
	
		is_ok = (comp_value == adjust_value) ? true : false;

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bWrite_cal_m_vc(uint32_t adjust_value)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bWrite_cal_m_vc(uint32_t adjust_value) {
	
	bool is_ok = false;
	uint32_t comp_value;

	#if NBR_PHASES >= 3
	
		VMetrology.DSP_CTRL.CAL_M_VC = (uint32_t)adjust_value;
		EEPROM_writer(DSP_CTRL_CAL_M_VC_ADDR, (uint8_t*)(&VMetrology.DSP_CTRL.CAL_M_VC), 4);
		((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_VC = VMetrology.DSP_CTRL.CAL_M_VC;
	
		EEPROM_reader(DSP_CTRL_CAL_M_VC_ADDR, (uint8_t*)(&comp_value), 4);
	
		is_ok = (comp_value == adjust_value) ? true : false;
		
	#endif
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bWrite_cal_ph_ic(uint32_t adjust_value)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bWrite_cal_ph_ic(uint32_t adjust_value) {
	
	bool is_ok = false;
	uint32_t comp_value;
	
	#if NBR_PHASES >= 3
	
		VMetrology.DSP_CTRL.CAL_PH_IC = (uint32_t)adjust_value;
		VMetrology.DSP_CTRL.CAL_PH_VC = 0;
		EEPROM_writer(DSP_CTRL_CAL_PH_IC_ADDR, (uint8_t*)(&VMetrology.DSP_CTRL.CAL_PH_IC), 4);
		EEPROM_writer(DSP_CTRL_CAL_PH_VC_ADDR, (uint8_t*)(&VMetrology.DSP_CTRL.CAL_PH_VC), 4);
		((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_PH_IC = VMetrology.DSP_CTRL.CAL_PH_IC;
		((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_PH_VC = VMetrology.DSP_CTRL.CAL_PH_VC;
	
		EEPROM_reader(DSP_CTRL_CAL_PH_IC_ADDR, (uint8_t*)(&comp_value), 4);
	
		is_ok = (comp_value == adjust_value) ? true : false;
		
	#endif
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bBack_up_cal_m_ic(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bBack_up_cal_m_ic(void) {
	
	bool is_ok = false;
	
	#if NBR_PHASES >= 3

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {

		EEPROM_reader(DSP_CTRL_CAL_M_IC_ADDR, (uint8_t*)(&nvm_value), 4);
		
		if ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_IC ) {
			
			EEPROM_writer(DSP_CTRL_CAL_M_IC_COPY_ADDR, &nvm_value, sizeof(nvm_value));
			nvm_value = 0;
			EEPROM_reader(DSP_CTRL_CAL_M_IC_COPY_ADDR, &nvm_value, sizeof(nvm_value));

			is_ok = ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_IC ) ? true : false;
		}

		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bBack_up_cal_m_vc(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bBack_up_cal_m_vc(void) {
	
	bool is_ok = false;
	
	#if NBR_PHASES >= 3

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {

		EEPROM_reader(DSP_CTRL_CAL_M_VC_ADDR, (uint8_t*)(&nvm_value), 4);
		
		if ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_VC ) {
			
			EEPROM_writer(DSP_CTRL_CAL_M_VC_COPY_ADDR, &nvm_value, sizeof(nvm_value));
			nvm_value = 0;
			EEPROM_reader(DSP_CTRL_CAL_M_VC_COPY_ADDR, &nvm_value, sizeof(nvm_value));

			is_ok = ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_M_VC ) ? true : false;
		}

		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bBack_up_cal_ph_ic(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bBack_up_cal_ph_ic(void) {
	
	bool is_ok = false; 
	
	#if NBR_PHASES >= 3

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {

		EEPROM_reader(DSP_CTRL_CAL_PH_IC_ADDR, (uint8_t*)(&nvm_value), 4);
		
		if ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_PH_IC ) {
			
			EEPROM_writer(DSP_CTRL_CAL_PH_IC_COPY_ADDR, &nvm_value, sizeof(nvm_value));
			nvm_value = 0;
			EEPROM_reader(DSP_CTRL_CAL_PH_IC_COPY_ADDR, &nvm_value, sizeof(nvm_value));

			is_ok = ( nvm_value == ((DSP_CTRL_TYPE*)(mem_reg_in))->CAL_PH_IC ) ? true : false;
		}

		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bRestore_cal_m_ic_backup(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bRestore_cal_m_ic_backup(void) {
	
	bool is_ok = false; 
	
	#if NBR_PHASES >= 3

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {
		
		EEPROM_reader(DSP_CTRL_CAL_M_IC_COPY_ADDR, (uint8_t*)(&nvm_value), 4);
		is_ok = METROLOGY_APP_bWrite_cal_m_ic(nvm_value);
		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bRestore_cal_m_vc_backup(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bRestore_cal_m_vc_backup(void) {
	
	bool is_ok = false; 
	
	#if NBR_PHASES >= 3

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {
		
		EEPROM_reader(DSP_CTRL_CAL_M_VC_COPY_ADDR, (uint8_t*)(&nvm_value), 4);
		is_ok = METROLOGY_APP_bWrite_cal_m_vc(nvm_value);
		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool METROLOGY_APP_bRestore_cal_ph_ic_backup(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool METROLOGY_APP_bRestore_cal_ph_ic_backup(void) {
	
	bool is_ok = false; 
	
	#if NBR_PHASES >= 3

	uint8_t i = 0;
	uint32_t nvm_value;

	while ( (!is_ok) && (i < 3) ) {
		
		EEPROM_reader(DSP_CTRL_CAL_PH_IC_COPY_ADDR, (uint8_t*)(&nvm_value), 4);
		is_ok = METROLOGY_APP_bWrite_cal_ph_ic(nvm_value);
		i++;
	}

	#endif

	return is_ok;
}

/**
 ******************************************************************************
 * \fn void METROLOGY_APP_vChange_kt(uint32_t kt_value)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void METROLOGY_APP_vChange_kt(uint32_t kt_value) {
		
	VMetrology.DSP_CTRL.P_K_T = (uint32_t)(kt_value);
	VMetrology.DSP_CTRL.I_K_T = VMetrology.DSP_CTRL.P_K_T;
	VMetrology.DSP_CTRL.Q_K_T = VMetrology.DSP_CTRL.P_K_T;

	EEPROM_writer(DSP_CTRL_P_K_T_ADDR, &VMetrology.DSP_CTRL.P_K_T, 4);
	( ( DSP_CTRL_TYPE * )( mem_reg_in ) )->P_K_T = VMetrology.DSP_CTRL.P_K_T;
	
	EEPROM_writer(DSP_CTRL_I_K_T_ADDR, &VMetrology.DSP_CTRL.I_K_T, 4);
	( ( DSP_CTRL_TYPE * )( mem_reg_in ) )->I_K_T = VMetrology.DSP_CTRL.I_K_T;
	
	EEPROM_writer(DSP_CTRL_Q_K_T_ADDR, &VMetrology.DSP_CTRL.Q_K_T, 4);
	( ( DSP_CTRL_TYPE * )( mem_reg_in ) )->Q_K_T = VMetrology.DSP_CTRL.Q_K_T;
}

/** 
 ******************************************************************************
 * \fn void METROLOGY_APP_vEnable_Q_energy_pulse_mode(void)
 * \brief change to reactive power
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void METROLOGY_APP_vEnable_Q_energy_pulse_mode(void) {
	
	uint32_t temp_compare;
	
	VMetrology.DSP_CTRL.PULSE0_CTRL.WORD = ( DSP_CTRL_PULSEx_CTRL_PCx_WIDTH(DEFAULT_PULSEx_CTRL_PCx_WIDTH)		| \
											 DSP_CTRL_PULSEx_CTRL_PCx_TYPE_Q_T			| \
											 DSP_CTRL_PULSEx_CTRL_PCx_POLARITY_LOW		| \
											 DSP_CTRL_PULSEx_CTRL_PCx_OVERRIDE_NORMAL	| \
											 DSP_CTRL_PULSEx_CTRL_PCx_ACC_HOLD_CONTINUE	| \
											 DSP_CTRL_PULSEx_CTRL_PCx_DETENT_ABS		| \
											 DSP_CTRL_PULSEx_CTRL_PCx_ENABLE_En			);

	EEPROM_reader( DSP_CTRL_PULSE0_CTRL_ADDR, &temp_compare, 4);

	if (temp_compare != VMetrology.DSP_CTRL.PULSE0_CTRL.WORD) {
		
		delay_ms(1);
		EEPROM_writer(DSP_CTRL_PULSE0_CTRL_ADDR, &VMetrology.DSP_CTRL.PULSE0_CTRL.WORD, 4);
		( ( DSP_CTRL_TYPE * )( mem_reg_in ) )->PULSE0_CTRL.WORD = VMetrology.DSP_CTRL.PULSE0_CTRL.WORD;
	}
}

/** 
 ******************************************************************************
 * \fn void METROLOGY_APP_vEnable_P_energy_pulse_mode(void)
 * \brief change to active power
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void METROLOGY_APP_vEnable_P_energy_pulse_mode(void) {
	
	uint32_t temp_compare;
	
	VMetrology.DSP_CTRL.PULSE0_CTRL.WORD = ( DSP_CTRL_PULSEx_CTRL_PCx_WIDTH(DEFAULT_PULSEx_CTRL_PCx_WIDTH)		| \
											 DSP_CTRL_PULSEx_CTRL_PCx_TYPE_P_T			| \
											 DSP_CTRL_PULSEx_CTRL_PCx_POLARITY_LOW		| \
											 DSP_CTRL_PULSEx_CTRL_PCx_OVERRIDE_NORMAL	| \
											 DSP_CTRL_PULSEx_CTRL_PCx_ACC_HOLD_CONTINUE	| \
											 DSP_CTRL_PULSEx_CTRL_PCx_DETENT_ABS		| \
											 DSP_CTRL_PULSEx_CTRL_PCx_ENABLE_En			);

	EEPROM_reader(DSP_CTRL_PULSE0_CTRL_ADDR, &temp_compare, 4);
	
	if (temp_compare != VMetrology.DSP_CTRL.PULSE0_CTRL.WORD) {
		
		delay_ms(1);
		EEPROM_writer(DSP_CTRL_PULSE0_CTRL_ADDR, &VMetrology.DSP_CTRL.PULSE0_CTRL.WORD, 4);
		( ( DSP_CTRL_TYPE * )( mem_reg_in ) )->PULSE0_CTRL.WORD = VMetrology.DSP_CTRL.PULSE0_CTRL.WORD;
	}
}

/**
******************************************************************************
* \fn QUADRANT_TYPE METROLOGY_APP_xGet_total_energy_quadrant(void)
* \brief Returns the current quadrant of the total energy
*
* \param void
* \retval RMS value
******************************************************************************
*/
QUADRANT_TYPE METROLOGY_APP_xGet_total_energy_quadrant(void) {
	
	QUADRANT_TYPE quadrant;
	double angle;
	
	angle = METROLOGY_APP_dGet_instantaneous_value(ANGLE_PF_T_ID);
	
	quadrant = QUADRANT_1;
	
	if ( (angle >= 0) && (angle < 90000) ) {
		quadrant = QUADRANT_1;
	}
	else if ( (angle >= 90000) && (angle < 180000) ) {
		quadrant = QUADRANT_2;
	}
	else if ( (angle >= 180000) && (angle < 270000) ) {
		quadrant = QUADRANT_3;
	}
	else if ( (angle >= 270000) && (angle < 360000) ) {
		quadrant = QUADRANT_4;
	}
	
	return quadrant;
}

/**
******************************************************************************
* \fn void METROLOGY_APP_vRefresh_inst_values(void)
* \brief 
*
* \param void
* \retval void
******************************************************************************
*/
void METROLOGY_APP_vRefresh_inst_values(void) {

	INSTANTANEOUS_VALUES_TYPE xInst;
	METROLOGY_TYPE xMetrology;

	xMetrology = VMetrology;
	memset(&xInst, 0, sizeof(xInst));
	
#if NBR_PHASES >= 1
	xInst.I_a = METROLOGY_APP_dCalculate_Ix_rms(xMetrology.DSP_ACC.I_A, xMetrology.DSP_ST.N);
	xInst.V_a = METROLOGY_APP_dCalculate_Vx_rms(xMetrology.DSP_ACC.V_A, xMetrology.DSP_ST.N);
	xInst.P_a = METROLOGY_APP_dCalculate_Px_rms(xMetrology.DSP_ACC.P_A, xMetrology.DSP_ST.N);
	xInst.Q_a = METROLOGY_APP_dCalculate_Qx_rms(xMetrology.DSP_ACC.Q_A, xMetrology.DSP_ST.N);
	xInst.S_a = sqrt( pow(xInst.P_a, 2) + pow(xInst.Q_a, 2) );
	xInst.Angle_PF_a = METROLOGY_APP_dCalculate_PF_angle( (int64_t)xInst.P_a, (int64_t)xInst.Q_a );
	xInst.PF_a = (uint64_t)fabs((xInst.P_a / xInst.S_a) * 1000);

	if		( (xInst.Angle_PF_a >= 0)		&& (xInst.Angle_PF_a < 90000) )		{ xInst.Quadrant_a = (double)QUADRANT_1; }
	else if ( (xInst.Angle_PF_a >= 90000)	&& (xInst.Angle_PF_a < 180000) )	{ xInst.Quadrant_a = (double)QUADRANT_2; }
	else if ( (xInst.Angle_PF_a >= 180000)	&& (xInst.Angle_PF_a < 270000) )	{ xInst.Quadrant_a = (double)QUADRANT_3; }
	else if ( (xInst.Angle_PF_a >= 270000)	&& (xInst.Angle_PF_a < 360000) )	{ xInst.Quadrant_a = (double)QUADRANT_4; }
#endif
	
#if NBR_PHASES >= 2
	xInst.I_b = METROLOGY_APP_dCalculate_Ix_rms(xMetrology.DSP_ACC.I_B, xMetrology.DSP_ST.N);
	xInst.V_b = METROLOGY_APP_dCalculate_Vx_rms(xMetrology.DSP_ACC.V_B, xMetrology.DSP_ST.N);
	xInst.P_b = METROLOGY_APP_dCalculate_Px_rms(xMetrology.DSP_ACC.P_B, xMetrology.DSP_ST.N);
	xInst.Q_b = METROLOGY_APP_dCalculate_Qx_rms(xMetrology.DSP_ACC.Q_B, xMetrology.DSP_ST.N);
	xInst.S_b = sqrt( pow(xInst.P_b, 2) + pow(xInst.Q_b, 2) );
	xInst.Angle_PF_b = METROLOGY_APP_dCalculate_PF_angle( (int64_t)xInst.P_b, (int64_t)xInst.Q_b );
	xInst.PF_b = (uint64_t)fabs((xInst.P_b / xInst.S_b) * 1000);

	if		( (xInst.Angle_PF_b >= 0)		&& (xInst.Angle_PF_b < 90000) )		{ xInst.Quadrant_b = (double)QUADRANT_1; }
	else if ( (xInst.Angle_PF_b >= 90000)	&& (xInst.Angle_PF_b < 180000) )	{ xInst.Quadrant_b = (double)QUADRANT_2; }
	else if ( (xInst.Angle_PF_b >= 180000)	&& (xInst.Angle_PF_b < 270000) )	{ xInst.Quadrant_b = (double)QUADRANT_3; }
	else if ( (xInst.Angle_PF_b >= 270000)	&& (xInst.Angle_PF_b < 360000) )	{ xInst.Quadrant_b = (double)QUADRANT_4; }
#endif
	
#if NBR_PHASES >= 3
	xInst.I_c = METROLOGY_APP_dCalculate_Ix_rms(xMetrology.DSP_ACC.I_C, xMetrology.DSP_ST.N);
	xInst.V_c = METROLOGY_APP_dCalculate_Vx_rms(xMetrology.DSP_ACC.V_C, xMetrology.DSP_ST.N);
	xInst.P_c = METROLOGY_APP_dCalculate_Px_rms(xMetrology.DSP_ACC.P_C, xMetrology.DSP_ST.N);
	xInst.Q_c = METROLOGY_APP_dCalculate_Qx_rms(xMetrology.DSP_ACC.Q_C, xMetrology.DSP_ST.N);
	xInst.S_c = sqrt( pow(xInst.P_c, 2) + pow(xInst.Q_c, 2) );
	xInst.Angle_PF_c = METROLOGY_APP_dCalculate_PF_angle( (int64_t)xInst.P_c, (int64_t)xInst.Q_c );
	xInst.PF_c = (uint64_t)fabs((xInst.P_c / xInst.S_c) * 1000);

	if		( (xInst.Angle_PF_c >= 0)		&& (xInst.Angle_PF_c < 90000) )		{ xInst.Quadrant_c = (double)QUADRANT_1; }
	else if ( (xInst.Angle_PF_c >= 90000)	&& (xInst.Angle_PF_c < 180000) )	{ xInst.Quadrant_c = (double)QUADRANT_2; }
	else if ( (xInst.Angle_PF_c >= 180000)	&& (xInst.Angle_PF_c < 270000) )	{ xInst.Quadrant_c = (double)QUADRANT_3; }
	else if ( (xInst.Angle_PF_c >= 270000)	&& (xInst.Angle_PF_c < 360000) )	{ xInst.Quadrant_c = (double)QUADRANT_4; }
#endif

	xInst.P_t = xInst.P_a + xInst.P_b + xInst.P_c;
	xInst.Q_t = xInst.Q_a + xInst.Q_b + xInst.Q_c;
	xInst.S_t = sqrt( pow(xInst.P_t, 2) + pow(xInst.Q_t, 2) );
	xInst.Angle_PF_t = METROLOGY_APP_dCalculate_PF_angle( (int64_t)xInst.P_t, (int64_t)xInst.Q_t );
	xInst.PF_t = (double)fabs((xInst.P_t / xInst.S_t) * 1000);
	xInst.Freq_t = (double)((VMetrology.DSP_ST.FREQ * 1000) >> FREQ_Q);

	if		( (xInst.Angle_PF_t >= 0)		&& (xInst.Angle_PF_t < 90000) )		{ xInst.Quadrant_t = (double)QUADRANT_1; }
	else if ( (xInst.Angle_PF_t >= 90000)	&& (xInst.Angle_PF_t < 180000) )	{ xInst.Quadrant_t = (double)QUADRANT_2; }
	else if ( (xInst.Angle_PF_t >= 180000)	&& (xInst.Angle_PF_t < 270000) )	{ xInst.Quadrant_t = (double)QUADRANT_3; }
	else if ( (xInst.Angle_PF_t >= 270000)	&& (xInst.Angle_PF_t < 360000) )	{ xInst.Quadrant_t = (double)QUADRANT_4; }

	volatile uint32_t ZC_N_REF = 0;
	volatile uint32_t ZC_N_VA = VMetrology.DSP_ST.ZC_N_VA;
	volatile uint32_t ZC_N_VB = VMetrology.DSP_ST.ZC_N_VB;
	volatile uint32_t ZC_N_VC = VMetrology.DSP_ST.ZC_N_VC;

	ZC_N_REF = ZC_N_VA;
	ZC_N_REF = (ZC_N_VA < ZC_N_REF) ? ZC_N_VA : ZC_N_REF;
	ZC_N_REF = (ZC_N_VB < ZC_N_REF) ? ZC_N_VB : ZC_N_REF;
	ZC_N_REF = (ZC_N_VC < ZC_N_REF) ? ZC_N_VC : ZC_N_REF;

	xInst.Angle_V_a = (double)(ZC_N_VA - ZC_N_REF ) * 360 * xInst.Freq_t / (double)(VMetrology.DSP_ST.N * 4096);
	xInst.Angle_V_b = (double)(ZC_N_VB - ZC_N_REF ) * 360 * xInst.Freq_t / (double)(VMetrology.DSP_ST.N * 4096);
	xInst.Angle_V_c = (double)(ZC_N_VC - ZC_N_REF ) * 360 * xInst.Freq_t / (double)(VMetrology.DSP_ST.N * 4096);

	if (xInst.Angle_V_a != 0) {

		xInst.Angle_V_b -= xInst.Angle_V_a; 
		xInst.Angle_V_c -= xInst.Angle_V_a;
		xInst.Angle_V_a -= xInst.Angle_V_a;

		if ((xInst.Angle_V_b < 0) && (xInst.V_b > 1000)) {
			xInst.Angle_V_b += 360000; 
		}
		else {
			xInst.Angle_V_b = 0;
		}

		if ((xInst.Angle_V_c < 0) && (xInst.V_c > 1000)) {
			xInst.Angle_V_c += 360000;
		}
		else {
			xInst.Angle_V_c = 0;
		}
	}

	xInst.Angle_I_a = xInst.Angle_V_a - xInst.Angle_PF_a;
	xInst.Angle_I_a = (xInst.Angle_I_a < 0) ? (360000 + xInst.Angle_I_a) : xInst.Angle_I_a;

	xInst.Angle_I_b = xInst.Angle_V_b - xInst.Angle_PF_b;
	xInst.Angle_I_b = (xInst.Angle_I_b < 0) ? (360000 + xInst.Angle_I_b) : xInst.Angle_I_b;

	xInst.Angle_I_c = xInst.Angle_V_c - xInst.Angle_PF_c;
	xInst.Angle_I_c = (xInst.Angle_I_c < 0) ? (360000 + xInst.Angle_I_c) : xInst.Angle_I_c;

	xInstantaneous = xInst;
}

/** 
 ******************************************************************************
 * \fn double METROLOGY_APP_dGet_instantaneous_value(RMS_TYPE inst_id)
 * \brief 
 *
 * 
 * \param 
 * \retval 
 ******************************************************************************
 */
double METROLOGY_APP_dGet_instantaneous_value(RMS_TYPE inst_id) {
	
	double inst_val = 0;

	switch (inst_id) {
		
		case P_T_ID:
			inst_val = xInstantaneous.P_t;
			break;

		case P_A_ID:
			inst_val = xInstantaneous.P_a;
			break;

		case P_B_ID:
			inst_val = xInstantaneous.P_b;
			break;

		case P_C_ID:
			inst_val = xInstantaneous.P_c;
			break;

		case Q_T_ID:
			inst_val = xInstantaneous.Q_t;
			break;

		case Q_A_ID:
			inst_val = xInstantaneous.Q_a;
			break;

		case Q_B_ID:
			inst_val = xInstantaneous.Q_b;
			break;

		case Q_C_ID:
			inst_val = xInstantaneous.Q_c;
			break;

		case S_T_ID:
			inst_val = xInstantaneous.S_t;
			break;

		case S_A_ID:
			inst_val = xInstantaneous.S_a;
			break;

		case S_B_ID:
			inst_val = xInstantaneous.S_b;
			break;

		case S_C_ID:
			inst_val = xInstantaneous.S_c;
			break;

		case V_A_ID:
			inst_val = xInstantaneous.V_a;
			break;

		case V_B_ID:
			inst_val = xInstantaneous.V_b;
			break;

		case V_C_ID:
			inst_val = xInstantaneous.V_c;
			break;

		case V_AB_ID:
			inst_val = xInstantaneous.V_ab;
			break;

		case V_BC_ID:
			inst_val = xInstantaneous.V_bc;
			break;

		case V_CA_ID:
			inst_val = xInstantaneous.V_ca;
			break;

		case I_A_ID:
			inst_val = xInstantaneous.I_a;
			break;

		case I_B_ID:
			inst_val = xInstantaneous.I_b;
			break;

		case I_C_ID:
			inst_val = xInstantaneous.I_c;
			break;

		case ANGLE_V_A_ID:
			inst_val = xInstantaneous.Angle_V_a;
			break;

		case ANGLE_V_B_ID:
			inst_val = xInstantaneous.Angle_V_b;
			break;

		case ANGLE_V_C_ID:
			inst_val = xInstantaneous.Angle_V_c;
			break;

		case ANGLE_I_A_ID:
			inst_val = xInstantaneous.Angle_I_a;
			break;

		case ANGLE_I_B_ID:
			inst_val = xInstantaneous.Angle_I_b;
			break;

		case ANGLE_I_C_ID:
			inst_val = xInstantaneous.Angle_I_c;
			break;

		case ANGLE_PF_T_ID:
			inst_val = xInstantaneous.Angle_PF_t;
			break;

		case ANGLE_PF_A_ID:
			inst_val = xInstantaneous.Angle_PF_a;
			break;

		case ANGLE_PF_B_ID:
			inst_val = xInstantaneous.Angle_PF_b;
			break;

		case ANGLE_PF_C_ID:
			inst_val = xInstantaneous.Angle_PF_c;
			break;

		case PF_T_ID:
			inst_val = xInstantaneous.PF_t;
			break;

		case PF_A_ID:
			inst_val = xInstantaneous.PF_a;
			break;

		case PF_B_ID:
			inst_val = xInstantaneous.PF_b;
			break;

		case PF_C_ID:
			inst_val = xInstantaneous.PF_c;
			break;

		case QUADRANT_T_ID:
			inst_val = xInstantaneous.Quadrant_t;
			break;

		case QUADRANT_A_ID:
			inst_val = xInstantaneous.Quadrant_a;
			break;

		case QUADRANT_B_ID:
			inst_val = xInstantaneous.Quadrant_b;
			break;

		case QUADRANT_C_ID:
			inst_val = xInstantaneous.Quadrant_c;
			break;

		case FREQ_T_ID:
			inst_val = xInstantaneous.Freq_t;
			break;

		case FREQ_A_ID:
			inst_val = xInstantaneous.Freq_a;
			break;

		case FREQ_B_ID:
			inst_val = xInstantaneous.Freq_b;
			break;

		case FREQ_C_ID:
			inst_val = xInstantaneous.Freq_c;
			break;
			
		default:
			inst_val = 0;
			break;
	}

	return inst_val;
}

/** 
 ******************************************************************************
 * \fn double METROLOGY_APP_dGet_instantaneous_value(RMS_TYPE inst_id)
 * \brief 
 *
 * 
 * \param 
 * \retval 
 ******************************************************************************
 */
void METROLOGY_APP_vCopy_instantaneous_struct(INSTANTANEOUS_VALUES_TYPE* inst_struct_ptr) {

	*inst_struct_ptr = xInstantaneous;
}

/** 
 ******************************************************************************
 * \fn double METROLOGY_APP_dCalculate_Ix_rms(uint64_t acc_ix, uint32_t nbr_samples)
 * \brief 
 * 
 * \param 
 * \retval 
 ******************************************************************************
 */
double METROLOGY_APP_dCalculate_Ix_rms(uint64_t acc_ix, uint32_t nbr_samples) {
	
	static volatile double ix_rms;
	
	ix_rms = (double)(acc_ix);
	ix_rms /= nbr_samples;  
	ix_rms /= pow(2, 40);						 	       
	ix_rms = sqrt(ix_rms);					
	ix_rms *= CURRENT_CONV_FACTOR;      
	ix_rms *= 1000;
	ix_rms = (ix_rms < CREEP_THRESHOLD_I) ? 0 : ix_rms;
	
	return ix_rms;
}

/** 
 ******************************************************************************
 * \fn double METROLOGY_APP_dCalculate_Vx_rms(uint64_t acc_vx, uint32_t nbr_samples)
 * \brief 
 *
 * \param 
 * \retval 
 ******************************************************************************
 */
double METROLOGY_APP_dCalculate_Vx_rms(uint64_t acc_vx, uint32_t nbr_samples) {
	
	static volatile double vx_rms;
	
	vx_rms = (double)(acc_vx);
	vx_rms /= nbr_samples;  
	vx_rms /= pow(2, 40);						 	       
	vx_rms = sqrt(vx_rms);					
	vx_rms *= VOLTAGE_CONV_FACTOR;      
	vx_rms *= 1000;
	vx_rms = (vx_rms < CREEP_THRESHOLD_V) ? 0 : vx_rms;
	
	return vx_rms;
}

/**
******************************************************************************
* \fn double METROLOGY_APP_dCalculate_Px_rms(int64_t acc_px, uint32_t nbr_samples)
* \brief returns calculated power
*
* \param void
* \retval calculated power
******************************************************************************
*/
double METROLOGY_APP_dCalculate_Px_rms(int64_t acc_px, uint32_t nbr_samples) {
	
	static volatile double px_rms;

	px_rms = (double)(acc_px);
	px_rms /= nbr_samples;
	px_rms /= pow(2, 40);
	px_rms *= CURRENT_CONV_FACTOR * VOLTAGE_CONV_FACTOR;
	px_rms *= 1000;
	px_rms = ( fabs(px_rms) < CREEP_THRESHOLD_PQ) ? 0 : px_rms;

	return px_rms;
}

/**
******************************************************************************
* \fn double METROLOGY_APP_dCalculate_Qx_rms(int64_t acc_qx, uint32_t nbr_samples)
* \brief returns calculated power
*
* \param void
* \retval calculated power
******************************************************************************
*/
double METROLOGY_APP_dCalculate_Qx_rms(int64_t acc_qx, uint32_t nbr_samples) {
	
	static volatile double qx_rms;

	qx_rms = (double)(acc_qx);
	qx_rms /= nbr_samples;
	qx_rms /= pow(2, 40);
	qx_rms *= CURRENT_CONV_FACTOR * VOLTAGE_CONV_FACTOR;
	qx_rms *= 1000;
	qx_rms = ( fabs(qx_rms) < CREEP_THRESHOLD_PQ) ? 0 : qx_rms;

	return qx_rms;
}

/**
******************************************************************************
* \fn double METROLOGY_APP_dCalculate_PF_angle(int64_t p, int64_t q)
* \brief returns calculated angle
*
* \param void
* \retval calculated angle
******************************************************************************
*/
double METROLOGY_APP_dCalculate_PF_angle(int64_t p, int64_t q) {
	
	double angle;
	
	angle = ( (180 * atan((double)q / (double)p)) / (double)CONST_Pi );
	angle = fabs(angle);
	
	if ( (p >= 0) && (q >= 0) ) {
		angle =   0 + angle;
	}
	else if ( (p <= 0) && (q >= 0) ) {
		angle = 180 - angle;
	}
	else if ( (p <= 0) && (q <= 0) ) {
		angle = 180 + angle;
	}
	else if ( (p >= 0) && (q <= 0) ) {
		angle = 360 - angle;
	}
	
	if ( (angle <= 0) || (angle >= 360) || ( (p == 0) && (q == 0) ) ) {
		angle = 0;
	}
	
	angle *= 1000;
	
	return angle;
}
	
/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY
 * ----------------------------------------------------------------------------
 * - Revision 04 / Jacob Anaya Candia / 2017-06-16
 *   - Change 3:  Description.
 *   - Change 2:  Description.
 *   - Change 1:  Description.
  ******************************************************************************
  */
