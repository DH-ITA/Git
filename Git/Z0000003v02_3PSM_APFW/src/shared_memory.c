
//===================================================================

#include "target.h"
#include "system.h"
#include "sm.h"
//-------------------------------------------------------------------

extern uint8_t core1_image_start_p;
extern uint8_t core1_image_end_p;
extern uint8_t core1_image_start_s;
extern uint8_t core1_image_end_s;

#define ioport_set_pin_peripheral_mode(pin, mode) \
do {\
	ioport_set_pin_mode(pin, mode);\
	ioport_disable_pin(pin);\
} while (0)
//===================================================================
//description	::	core interrupt process
//function		::	IPC_Handler
//input			::	none
//output		::	none
//call			::	ipc_get_status,ipc_clear_interrupt
//effect		::	VMetrology
//===================================================================
void IPC0_Handler( void )
{
    uint32_t i = 0;

    if ( ipc_get_status( IPC_CORE0, IPC_INIT_IRQ ) )
    {
        ipc_clear_interrupt( IPC_CORE0, IPC_INIT_IRQ );
    }

    if ( ipc_get_status( IPC_CORE0, IPC_STATUS_IRQ ) )
    {
        ipc_clear_interrupt( IPC_CORE0, IPC_STATUS_IRQ );
    }

    if ( ipc_get_status( IPC_CORE0, IPC_HALF_CYCLE_IRQ ) )
    {
        ipc_clear_interrupt( IPC_CORE0, IPC_HALF_CYCLE_IRQ );
    }

    if ( ipc_get_status( IPC_CORE0, IPC_FULL_CYCLE_IRQ ) )
    {
        ipc_clear_interrupt( IPC_CORE0, IPC_FULL_CYCLE_IRQ );
    }

    if ( ipc_get_status( IPC_CORE0, IPC_INTEGRATION_IRQ ) )
    {
        ipc_clear_interrupt( IPC_CORE0, IPC_INTEGRATION_IRQ );
		
		meter.metrology_over_timer = 0;

        for ( i = 0; i < DSP_ST_SIZE; i++ )
        {
            *( ( uint32_t * )( &VMetrology.DSP_ST.VERSION.WORD ) + i ) = *( ( uint32_t * )( mem_reg_out ) + i );
        }
        if ( ( ( volatile DSP_ST_TYPE * ) ( mem_reg_out ) )->STATUS.BIT.ST == DSP_ST_STATUS_DSP_RUNNING )
        {
            for ( i = 0; i < ( DSP_ACC_SIZE * 2 ); i = i + 2 )
            {
                *( ( uint32_t * )( &VMetrology.DSP_ACC.I_A ) + i ) = *( ( uint32_t * )( mem_acc_out ) + i );
                *( ( uint32_t * )( &VMetrology.DSP_ACC.I_A ) + i + 1 ) = *( ( uint32_t * )( mem_acc_out ) + i + 1 );
            }
        }

		meter_os.metrology_ready = true;
    }
}

//===================================================================
//description	::	share memory initialize
//function		::	shared_mem_init
//input			::	none
//output		::	none
//call			::	pmc_enable_periph_clk,ipc_enable_interrupt
//effect		::	Reg_In_CONTROL,Reg_Out_MODE,Acc_Out_I_T
//===================================================================
void	shared_mem_init( void )
{
    //Enable IPC0, IPC1 clock
    pmc_enable_periph_clk( ID_IPC_CORE0 );
    pmc_enable_periph_clk( ID_IPC_CORE1 );

    //ipc_enable_interrupt(IPC_CORE0, IPC_IRQ);
    ipc_enable_interrupt( IPC_CORE0, IPC_INIT_IRQ | IPC_STATUS_IRQ | IPC_HALF_CYCLE_IRQ | IPC_FULL_CYCLE_IRQ | IPC_INTEGRATION_IRQ );
    NVIC_DisableIRQ( ID_IPC_CORE0 );
    NVIC_ClearPendingIRQ( ID_IPC_CORE0 );
    NVIC_EnableIRQ( ID_IPC_CORE0 );
}
//===================================================================
//description   ::  inition c1 core
//function      ::  Init_Core1
//input         ::  none
//output        ::  none
//call          ::  shared_mem_init,configure_coprocessor,
//                  fill_sram1_memory,deassert_c1_reset
//effect        ::
//===================================================================
void    Init_Core1( void )
{
    //if( RSTC_SR_RSTTYP_UserReset == rstc_get_reset_cause( RSTC ) )
    {
        /* Asserts co-processor reset */
        /* Asserts co-processor peripheral reset */
        RSTC->RSTC_CPMR = RSTC_CPMR_CPKEY(0x5AU);
		nop();
        delay_ms( 200 );
    }

    shared_mem_init();

#ifdef CONF_BOARD_ENABLE_COPROCESSOR
    configure_coprocessor();
    fill_sram1_memory();
    deassert_c1_reset();
#endif
#ifdef CONF_BOARD_UART_USER
	ioport_set_pin_peripheral_mode( PIO_PC0_IDX, IOPORT_MODE_MUX_A);
	ioport_set_pin_peripheral_mode( PIO_PC1_IDX, IOPORT_MODE_MUX_A);
#endif
#ifdef ENABLE_CACHE //defined in project settings
    //Matrix0 settings
    MATRIX0->MATRIX_SCFG[2] = MATRIX_SCFG_DEFMSTR_TYPE( 2 ) | MATRIX_SCFG_FIXED_DEFMSTR( 6 );

    //Enable cache
    CMCC0->CMCC_CTRL = CMCC_CTRL_CEN;
#else
    CMCC0->CMCC_CTRL &= ~CMCC_CTRL_CEN;
#endif
}
//===================================================================
//description   ::  release core1 reset
//function      ::  deassert_c1_reset
//input         ::  none
//output        ::  none
//call          ::  none
//effect        ::
//===================================================================
void    deassert_c1_reset( void )
{
    //deasserts Coprocessor reset
    REG_RSTC_CPMR |= RSTC_CPMR_CPROCEN | RSTC_CPMR_CPKEY(0x5AU);
}
//===================================================================
//description   ::  fill core1 code to sram1
//function      ::  fill_sram1_memory
//input         ::  none
//output        ::  none
//call          ::  memcpy
//effect        ::
//===================================================================
void	fill_sram1_memory( void )
{
    /*
    	extern char SRAM1_BIN;
    	memcpy( ( char* )IRAM1_ADDR, ( char* )&SRAM1_BIN, IRAM1_SIZE );
    */
	/*This modification is done because we are only using SAM4CMP - RHR*/
//     if ( ( chipID == SAM4CMS16C_CHIP_ID ) || ( chipID == SAM4CMS8C_CHIP_ID  ) )
//     {
// 	    memcpy( ( char * )IRAM1_ADDR, ( char * )&core1_image_start_s, ( int )&core1_image_end_s - ( int )&core1_image_start_s );
//     }
//     else
//     {
	    memcpy( ( char * )IRAM1_ADDR, ( char * )&core1_image_start_p, ( int )&core1_image_end_p - ( int )&core1_image_start_p );
//     }
}
//===================================================================
//description   ::  configure core1 processor
//function      ::  configure_coprocessor
//input         ::  none
//output        ::  none
//call          ::  none
//effect        ::
//===================================================================
void	configure_coprocessor( void )
{
    unsigned int  read_reg = 0;

    /* Enables Coprocessor Bus Master Clock */
    PMC->PMC_SCER = PMC_SCER_CPBMCK | PMC_SCER_CPKEY_PASSWD;

    /* Enables the Co-processor Clocks */
    PMC->PMC_SCER = PMC_SCER_CPCK | PMC_SCER_CPKEY_PASSWD;

    /* Set co-processor clock pre-scaler and source */
    read_reg = REG_PMC_MCKR;
    read_reg &= ~PMC_MCKR_CPPRES_Msk;
	/*This modification is done because we are only using SAM4CMP - RHR*/
//     if ( ( chipID == SAM4CMS16C_CHIP_ID ) || ( chipID == SAM4CMS8C_CHIP_ID  ) )
//     {
// 	    // 51.2MHz for single-phase AFE
// 	    read_reg |= PMC_MCKR_CPPRES( CONFIG_CPCLK_PRES );
//     }
//     else
//     {
	    // 102.4MHz for poly-phase AFE
	    read_reg |= PMC_MCKR_CPPRES( CONFIG_CPCLK_PRES - 1 );
//     }
    REG_PMC_MCKR = read_reg;

    /* Choose co-processor main clock source */
    read_reg = REG_PMC_MCKR;
    read_reg &= ~PMC_MCKR_CPCSS_Msk;
    read_reg |= ( CONFIG_CPCLK_SOURCE << PMC_MCKR_CPCSS_Pos );
    REG_PMC_MCKR = read_reg;

    /* Release co-processor peripheral reset */
    RSTC->RSTC_CPMR |= ( RSTC_CPMR_CPKEY( 0x5Au ) | RSTC_CPMR_CPEREN );

    /* Enable Core 1 SRAM1 and SRAM2 memories */
    pmc_enable_periph_clk( 42 ); /* ID_SRAM1_2 */
}

