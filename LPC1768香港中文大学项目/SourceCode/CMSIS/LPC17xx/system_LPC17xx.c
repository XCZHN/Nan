/**************************************************************************//**
 * @file     system_LPC17xx.c
 * @brief    CMSIS Cortex-M3 Device System Source File for
 *           NXP LPC17xx Device Series
 * @version  V1.11
 * @date     21. June 2011
 *
 * @note
 * Copyright (C) 2009-2011 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/
#include <stdint.h>
#include "LPC17xx.h"

#define CLOCK_SETUP           1
#define SCS_Val               0x00000020
#define CLKSRCSEL_Val         0x00000001
#define PLL0_SETUP            1
#define PLL0CFG_Val           0x00050063
#define PLL1_SETUP            1
#define PLL1CFG_Val           0x00000023
#define CCLKCFG_Val           0x00000003
#define USBCLKCFG_Val         0x00000000
#define PCLKSEL0_Val          0x00000000
#define PCLKSEL1_Val          0x00000000
#define PCONP_Val             0x042887DE
#define CLKOUTCFG_Val         0x00000000

#define FLASH_SETUP           1
#define FLASHCFG_Val          0x00004000

/*----------------------------------------------------------------------------
  Check the register settings
 *----------------------------------------------------------------------------*/
#define CHECK_RANGE(val, min, max)                ((val < min) || (val > max))
#define CHECK_RSVD(val, mask)                     (val & mask)

/* Clock Configuration -------------------------------------------------------*/
#if (CHECK_RSVD((SCS_Val),       ~0x00000030))
   #error "SCS: Invalid values of reserved bits!"
#endif

#if (CHECK_RANGE((CLKSRCSEL_Val), 0, 2))
   #error "CLKSRCSEL: Value out of range!"
#endif

#if (CHECK_RSVD((PLL0CFG_Val),   ~0x00FF7FFF))
   #error "PLL0CFG: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PLL1CFG_Val),   ~0x0000007F))
   #error "PLL1CFG: Invalid values of reserved bits!"
#endif

#if (PLL0_SETUP)            /* if PLL0 is used */
  #if (CCLKCFG_Val < 2)     /* CCLKSEL must be greater then 1 */
    #error "CCLKCFG: CCLKSEL must be greater then 1 if PLL0 is used!"
  #endif
#endif

#if (CHECK_RANGE((CCLKCFG_Val), 2, 255))
   #error "CCLKCFG: Value out of range!"
#endif

#if (CHECK_RSVD((USBCLKCFG_Val), ~0x0000000F))
   #error "USBCLKCFG: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PCLKSEL0_Val),   0x000C0C00))
   #error "PCLKSEL0: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PCLKSEL1_Val),   0x03000300))
   #error "PCLKSEL1: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PCONP_Val),      0x10100821))
   #error "PCONP: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((CLKOUTCFG_Val), ~0x000001FF))
   #error "CLKOUTCFG: Invalid values of reserved bits!"
#endif

/* Flash Accelerator Configuration -------------------------------------------*/
#if (CHECK_RSVD((FLASHCFG_Val), ~0x0000F000))
   #error "FLASHCFG: Invalid values of reserved bits!"
#endif

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define XTAL        (12000000UL)        /* Oscillator frequency               */
#define OSC_CLK     (      XTAL)        /* Main oscillator frequency          */
#define RTC_CLK     (   32768UL)        /* RTC oscillator frequency           */
#define IRC_OSC     ( 4000000UL)        /* Internal RC oscillator frequency   */


/* F_cco0 = (2 * M * F_in) / N  */
#define __M               (((PLL0CFG_Val      ) & 0x7FFF) + 1)
#define __N               (((PLL0CFG_Val >> 16) & 0x00FF) + 1)
#define __FCCO(__F_IN)    ((2ULL * __M * __F_IN) / __N)
#define __CCLK_DIV        (((CCLKCFG_Val      ) & 0x00FF) + 1)

/* Determine core clock frequency according to settings */
 #if (PLL0_SETUP)
    #if   ((CLKSRCSEL_Val & 0x03) == 1)	//=1,PLL0时钟源为外部主晶振
        #define __CORE_CLK (__FCCO(OSC_CLK) / __CCLK_DIV) //CPU时钟为__CORE_CLK
    #elif ((CLKSRCSEL_Val & 0x03) == 2)	//=2,选择RTC晶振为PLL0时钟源
        #define __CORE_CLK (__FCCO(RTC_CLK) / __CCLK_DIV)
    #else							    //=0,选择内部RC晶振作为PLL0时钟源
        #define __CORE_CLK (__FCCO(IRC_OSC) / __CCLK_DIV)
    #endif
 #else
    #if   ((CLKSRCSEL_Val & 0x03) == 1)
        #define __CORE_CLK (OSC_CLK         / __CCLK_DIV)
    #elif ((CLKSRCSEL_Val & 0x03) == 2)
        #define __CORE_CLK (RTC_CLK         / __CCLK_DIV)
    #else
        #define __CORE_CLK (IRC_OSC         / __CCLK_DIV)
    #endif
 #endif

uint32_t SystemCoreClock = __CORE_CLK;

void SystemInit (void)
{
#if (CLOCK_SETUP)                       /* Clock Setup                        */
  LPC_SC->SCS       = SCS_Val;			//SCS_Val=0x00000020,主晶振的选择范围为1-20MHZ，启动外部主晶振
  if (LPC_SC->SCS & (1 << 5)) 			//if OSCEN=1，即外部晶振已启动
  {             						
    while ((LPC_SC->SCS & (1<<6)) == 0);//检测晶振的工作状态位OSCSTA，等待晶振启动好
  }

  LPC_SC->CCLKCFG   = CCLKCFG_Val;      //设置CPU时钟分配,CCLKCFG_Val=0x00000003,即CPU时钟CCLK=PLLCLK/4
										//CPU时钟来源与PLL0输出信号，
  LPC_SC->PCLKSEL0  = PCLKSEL0_Val;     //外围设置时钟选择寄存器0 PCLKSEL0=0x00000000，表示所有外围设备时钟=CCLK/4
  //LPC_SC->PCLKSEL0=0x00010000;	
  LPC_SC->PCLKSEL1  = PCLKSEL1_Val;	    //外围设置时钟选择寄存器1 PCLKSEL1=0x00000000

#if (PLL0_SETUP)
  LPC_SC->CLKSRCSEL = CLKSRCSEL_Val;    //PLL0选择时钟源，CLKSRCSEL_Val=0x00000001(表示选择外部晶振作为PLL0的时钟源)

  LPC_SC->PLL0CFG   = PLL0CFG_Val;      //配置PLL0,PLL0CFG_Val=0x00050063,即MSEL0=0x63 ,NSEL0=5;PLL0输出频率Fcco=(2*M*Fin)/N ,其中Fin=32khz~50mhz,即外部晶振频率,M=MSEL0+1,N=NSEL0+1,
  LPC_SC->PLL0FEED  = 0xAA;
  LPC_SC->PLL0FEED  = 0x55;

  LPC_SC->PLL0CON   = 0x01;             //PLL0启动
  LPC_SC->PLL0FEED  = 0xAA;
  LPC_SC->PLL0FEED  = 0x55;
  while (!(LPC_SC->PLL0STAT & (1<<26)));//判断PLL0的锁定为'PLOCK0'是否被锁定

  LPC_SC->PLL0CON   = 0x03;             //PLL0连接
  LPC_SC->PLL0FEED  = 0xAA;
  LPC_SC->PLL0FEED  = 0x55;
  while (!(LPC_SC->PLL0STAT & ((1<<25) | (1<<24))));/* Wait for PLLC0_STAT & PLLE0_STAT */
#endif

#if (PLL1_SETUP)
  LPC_SC->PLL1CFG   = PLL1CFG_Val;
  LPC_SC->PLL1FEED  = 0xAA;
  LPC_SC->PLL1FEED  = 0x55;

  LPC_SC->PLL1CON   = 0x01;             /* PLL1 Enable                        */
  LPC_SC->PLL1FEED  = 0xAA;
  LPC_SC->PLL1FEED  = 0x55;
  while (!(LPC_SC->PLL1STAT & (1<<10)));/* Wait for PLOCK1                    */

  LPC_SC->PLL1CON   = 0x03;             /* PLL1 Enable & Connect              */
  LPC_SC->PLL1FEED  = 0xAA;
  LPC_SC->PLL1FEED  = 0x55;
  while (!(LPC_SC->PLL1STAT & ((1<< 9) | (1<< 8))));/* Wait for PLLC1_STAT & PLLE1_STAT */
#else
  LPC_SC->USBCLKCFG = USBCLKCFG_Val;    //如果PLL1没有被连接，那么USB时钟48MHZ来自PLL0由USB时钟分割而来
#endif									//USBCLKCFG_Val=5,那么PLL0输出必须为288MHZ;=7，PLL0=384mhz;=9,PLL0=480Mhz

  LPC_SC->PCONP     = PCONP_Val;        //外围设备电源控制，用于选择关闭和打开外围设备电源

  LPC_SC->CLKOUTCFG = CLKOUTCFG_Val;    //时钟输出配置，=0，表示选择CPU时钟作为CLKOUT源
#endif

#if (FLASH_SETUP == 1)                  //flash加速配置寄存器
  LPC_SC->FLASHCFG  = (LPC_SC->FLASHCFG & ~0x0000F000) | FLASHCFG_Val;
#endif
}

