//===================================================================
#ifdef __cplusplus
extern "C" {
#endif
//===================================================================
#ifndef SHARED_MEMORY_H_INCLUDED
#define SHARED_MEMORY_H_INCLUDED
//===================================================================
//#include "device.h"
#include "target.h"

//====================SHARED MEMORY SETTINGS=========================
#define mem_reg_in (0x20100000)
#define mem_reg_out (mem_reg_in + 55*4)//DSP_CONTROL_SIZE*4)
#define mem_acc_out (mem_reg_out + 35*4)//DSP_ST_SIZE*4)
#define mem_har_out (mem_acc_out + 55*8)//DSP_ACC_SIZE*8)

/* Configure IPC channels */
#define IPC_CORE0 IPC0
#define ID_IPC_CORE0 (IRQn_Type)ID_IPC0
#define IPC_CORE1 IPC1
#define ID_IPC_CORE1 (IRQn_Type)ID_IPC1
/* IPC channel for shared memory */
#define IPC_INIT_IRQ        (0x1u << 20)
#define IPC_STATUS_IRQ        	(0x1U << 16)
#define IPC_HALF_CYCLE_IRQ		(0x1U << 5)
#define IPC_FULL_CYCLE_IRQ		(0x1U << 4)
#define IPC_INTEGRATION_IRQ		(0x1U << 0)

/* Uncomment to use direct access of control/mode registers during dsp startup only, instead of IPC. */
#define _USE_DIRECT_METHOD_INSTEAD_OF_IPC_  // use this to defeat IPC method during dsp startup phase only.

#define WRITE_CONTROL_DATA      1

//===================================================================

//extern  __IO uint32_t* Reg_In_CONTROL;
//extern	__IO uint32_t* Reg_Out_MODE;
//extern	__IO uint32_t* Acc_Out_I_T;
//---------------------------------------------------------
//void    IPC_Handler(void);
void shared_mem_init( void );
void Init_Core1( void );
void deassert_c1_reset( void );
void fill_sram1_memory( void );
void configure_coprocessor( void );

//=========================================================



#endif

#ifdef __cplusplus
}
#endif