/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : HF1.c
**     Project     : Remote
**     Processor   : MK20DX128VFT5
**     Component   : HardFault
**     Version     : Component 01.010, Driver 01.00, CPU db: 3.00.000
**     Repository  : My Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-09-30, 13:38, # CodeGen: 1
**     Abstract    :
**          Component to simplify hard faults for ARM/Kinetis.
**     Settings    :
**          Component name                                 : HF1
**          SDK                                            : KSDK1
**     Contents    :
**         HardFaultHandler - void HF1_HardFaultHandler(void);
**
**     License   : Open Source (LGPL)
**     Copyright : Erich Styger, 2013-2016, all rights reserved.
**     Web       : www.mcuoneclipse.com
**     This an open source software for Processor Expert.
**     This is a free software and is opened for education, research and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file HF1.c
** @version 01.00
** @brief
**          Component to simplify hard faults for ARM/Kinetis.
*/         
/*!
**  @addtogroup HF1_module HF1 module documentation
**  @{
*/         

/* MODULE HF1. */

#include "HF1.h"

/*
** ===================================================================
**     Method      :  HF1_HandlerC (component HardFault)
**
**     Description :
**         Additional handler which decodes the processor status
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
/**
 * This is called from the HardFaultHandler with a pointer the Fault stack
 * as the parameter. We can then read the values from the stack and place them
 * into local variables for ease of reading.
 * We then read the various Fault Status and Address Registers to help decode
 * cause of the fault.
 * The function ends with a BKPT instruction to force control back into the debugger
 */
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
void HF1_HandlerC(uint32_t *hardfault_args)
{
  /*lint -save  -e550 Symbol not accessed. */
  static volatile unsigned long stacked_r0;
  static volatile unsigned long stacked_r1;
  static volatile unsigned long stacked_r2;
  static volatile unsigned long stacked_r3;
  static volatile unsigned long stacked_r12;
  static volatile unsigned long stacked_lr;
  static volatile unsigned long stacked_pc;
  static volatile unsigned long stacked_psr;
  static volatile unsigned long _CFSR;
  static volatile unsigned long _HFSR;
  static volatile unsigned long _DFSR;
  static volatile unsigned long _AFSR;
  static volatile unsigned long _BFAR;
  static volatile unsigned long _MMAR;
  stacked_r0 = ((unsigned long)hardfault_args[0]);
  stacked_r1 = ((unsigned long)hardfault_args[1]);
  stacked_r2 = ((unsigned long)hardfault_args[2]);
  stacked_r3 = ((unsigned long)hardfault_args[3]);
  stacked_r12 = ((unsigned long)hardfault_args[4]);
  stacked_lr = ((unsigned long)hardfault_args[5]);
  stacked_pc = ((unsigned long)hardfault_args[6]);
  stacked_psr = ((unsigned long)hardfault_args[7]);

  /* Configurable Fault Status Register */
  /* Consists of MMSR, BFSR and UFSR */
  _CFSR = (*((volatile unsigned long *)(0xE000ED28)));

  /* Hard Fault Status Register */
  _HFSR = (*((volatile unsigned long *)(0xE000ED2C)));

  /* Debug Fault Status Register */
  _DFSR = (*((volatile unsigned long *)(0xE000ED30)));

  /* Auxiliary Fault Status Register */
  _AFSR = (*((volatile unsigned long *)(0xE000ED3C)));

  /* Read the Fault Address Registers. These may not contain valid values.
   * Check BFARVALID/MMARVALID to see if they are valid values
   * MemManage Fault Address Register
   */
  _MMAR = (*((volatile unsigned long *)(0xE000ED34)));
  /* Bus Fault Address Register */
  _BFAR = (*((volatile unsigned long *)(0xE000ED38)));

#if 0 /* experimental, seems not to work properly with GDB in KDS V3.2.0 */
#ifdef __GNUC__ /* might improve stack, see https://www.element14.com/community/message/199113/l/gdb-assisted-debugging-of-hard-faults#199113 */
  __asm volatile (
      "tst lr,#4     \n" /* check which stack pointer we are using */
      "ite eq        \n"
      "mrseq r0, msp \n" /* use MSP */
      "mrsne r0, psp \n" /* use PSP */
      "mov sp, r0    \n" /* set stack pointer so GDB shows proper stack frame */
  );
#endif
#endif
  __asm("BKPT #0\n") ; /* cause the debugger to stop */
  /*lint -restore */
}

/*
** ===================================================================
**     Method      :  HF1_HardFaultHandler (component HardFault)
**     Description :
**         Hard Fault Handler
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
__attribute__((naked))
#if KSDK1_SDK_VERSION_USED != KSDK1_SDK_VERSION_NONE
void HardFault_Handler(void)
#else
void HF1_HardFaultHandler(void)
#endif
{
  __asm volatile (
    " movs r0,#4      \n"  /* load bit mask into R0 */
    " mov r1, lr      \n"  /* load link register into R1 */
    " tst r0, r1      \n"  /* compare with bitmask */
    " beq _MSP        \n"  /* if bitmask is set: stack pointer is in PSP. Otherwise in MSP */
    " mrs r0, psp     \n"  /* otherwise: stack pointer is in PSP */
    " b _GetPC        \n"  /* go to part which loads the PC */
  "_MSP:              \n"  /* stack pointer is in MSP register */
    " mrs r0, msp     \n"  /* load stack pointer into R0 */
  "_GetPC:            \n"  /* find out where the hard fault happened */
    " ldr r1,[r0,#20] \n"  /* load program counter into R1. R1 contains address of the next instruction where the hard fault happened */
    " b HF1_HandlerC   \n"  /* decode more information. R0 contains pointer to stack frame */
  );
}

/* END HF1. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
