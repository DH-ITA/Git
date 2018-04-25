//=========================================================
//=========================================================
#ifndef _TARGET_H_INCLUDED
#define _TARGET_H_INCLUDED
/* FreeRTOS mini Real-Time Kernel */
#include "FreeRTOS.h"
#include "StackMacros.h"
#include "croutine.h"
#include "list.h"
#include "mpu_wrappers.h"
#include "portable.h"
#include "projdefs.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

/* SOC Related Includes */
#include "compiler.h"
#include "stdio_serial.h"
#include "string.h"
#include "core_cmInstr.h"
#include "core_cm4.h"
#include "math.h"
#include "asf.h"
#include "conf_board.h"
#include "pio.h"
#include "ipc.h"
//#include "main\DemoMeter.h"
#include "board.h"
#include "conf_clock.h"
#include "delay.h"

#include "metrology.h"

#include "shared_memory.h"

#include "eeprom.h"

/* -------- CCFG_SYSIO : (MATRIX Offset: 0x0114) System I/O Configuration register -------- */
#define CCFG_SYSIO_SYSIO4 (0x1u << 4) /**< \brief (CCFG_SYSIO) PB4 or TDI Assignment */
#define CCFG_SYSIO_SYSIO5 (0x1u << 5) /**< \brief (CCFG_SYSIO) PB5 or TDO/TRACESWO Assignment */
#define CCFG_SYSIO_SYSIO6 (0x1u << 6) /**< \brief (CCFG_SYSIO) PB6 or TMS/SWDIO Assignment */
#define CCFG_SYSIO_SYSIO7 (0x1u << 7) /**< \brief (CCFG_SYSIO) PB7 or TCK/SWCLK Assignment */
#define CCFG_SYSIO_SYSIO10 (0x1u << 10) /**< \brief (CCFG_SYSIO) PB10 or DDM Assignment */
#define CCFG_SYSIO_SYSIO11 (0x1u << 11) /**< \brief (CCFG_SYSIO) PB11 or DDP Assignment */
#define CCFG_SYSIO_SYSIO12 (0x1u << 12) /**< \brief (CCFG_SYSIO) PB12 or ERASE Assignment */
/* -------- CCFG_SMCNFCS : (MATRIX Offset: 0x011C) SMC Chip Select NAND Flash Assignment Register -------- */
#define CCFG_SMCNFCS_SMC_NFCS0 (0x1u << 0) /**< \brief (CCFG_SMCNFCS) SMC NAND Flash Chip Select 0 Assignment */
#define CCFG_SMCNFCS_SMC_NFCS1 (0x1u << 1) /**< \brief (CCFG_SMCNFCS) SMC NAND Flash Chip Select 1 Assignment */
#define CCFG_SMCNFCS_SMC_NFCS2 (0x1u << 2) /**< \brief (CCFG_SMCNFCS) SMC NAND Flash Chip Select 2 Assignment */
#define CCFG_SMCNFCS_SMC_NFCS3 (0x1u << 3) /**< \brief (CCFG_SMCNFCS) SMC NAND Flash Chip Select 3 Assignment */


/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* _DEMOMETER_H_INCLUDED */
