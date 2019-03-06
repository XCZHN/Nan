/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_Timer.c                                                                       
==                                                                                         
**  Function:	定时器
==                                                                                         
**  Data:       2016/05/24                                                                          
================================================================================================*/

#ifndef _TIMER_H_
#define _TIMER_H_



#include "Config.h"
#include "LPC17xx.h"

extern uint32 gTimeCount;

/***********************************************************************************************
* Function Name  : Time0_Init
* Description	 : d定时器0初始化
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Time0_Init(void);

/***********************************************************************************************
* Function Name  : TIMER0_IRQHandler
* Description	 : 定时器0中断服务程序。使用定时器实现0.05秒钟定时。
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void TIMER0_IRQHandler(void);


#endif
/*========================File end=====================================================================*/

