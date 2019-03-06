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
**  Function:	��ʱ��
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
* Description	 : d��ʱ��0��ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Time0_Init(void);

/***********************************************************************************************
* Function Name  : TIMER0_IRQHandler
* Description	 : ��ʱ��0�жϷ������ʹ�ö�ʱ��ʵ��0.05���Ӷ�ʱ��
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void TIMER0_IRQHandler(void);


#endif
/*========================File end=====================================================================*/

