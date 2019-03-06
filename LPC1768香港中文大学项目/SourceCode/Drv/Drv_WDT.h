/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_WDT.h                                                                       
==                                                                                         
**  Function:	看门狗定时器(Watch Dog Timer)
==                                                                                         
**  Data:       2016/05/24                                                                          
================================================================================================*/

#ifndef	_DRV_WDT_H_
#define	_DRV_WDT_H_

#include "Config.h"
#include "LPC17xx.h"

/***********************************************************************************************
* Function Name  : WDT_Init
* Description	 : 独立看门狗初始化
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void WDT_Init(void);

/***********************************************************************************************
* Function Name  : WDT_Feed
* Description	 : 独立看门狗--喂狗
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void WDT_Feed(void);


#endif
/*=============================File end========================================================*/
