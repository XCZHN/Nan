/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                        
==                                                                                          
**  Version:	v2.0.0                                                                          
==
**  Filename:	Drv_OtherExt.h                                                                       
==                                                                                         
**  Function:	其它扩展功能模块驱动控制程序: 继电器、蜂鸣器、指示灯、按键等   
==                                                                                         
**  Data:       2013/03/06                                                                       
================================================================================================*/

#ifndef _DRV_OTHER_EXT_H_
#define _DRV_OTHER_EXT_H_


#include "Config.h"
#include "LPC17xx.h"


/*----------------- Led define ----------------------------------*/
#define	LED1_pin	           25		//P3.25


#define	LED1_ON		          (LPC_GPIO3->FIOCLR |= (1<<LED1_pin)) 	
#define	LED1_OFF	          (LPC_GPIO3->FIOSET |= (1<<LED1_pin))


/*----------------- Beep define ----------------------------------*/
#define	BEEP_pin	           26		//3.26

#define	BEEP_ON			      (LPC_GPIO3->FIOSET |= (1<<BEEP_pin))
#define	BEEP_OFF			  (LPC_GPIO3->FIOCLR |= (1<<BEEP_pin))


/*----------------- Key define ----------------------------------*/
#define	MKey_pin	           6		//P2.6

#define GET_MKey_STATE		 (LPC_GPIO2->FIOPIN&0x40)   


/***********************************************************************************************
* Function Name  : LED_Init
* Description	 : LED初始化
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void LED_Init(void);

/***********************************************************************************************
* Function Name  : Beep_Init
* Description	 : 蜂鸣器初始化
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Beep_Init(void);

/***********************************************************************************************
* Function Name  : ExtendFunction_Init
* Description	 : 扩展功能初始化
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : RS485主从通过PA0管脚电平选择
***********************************************************************************************/
extern void ExtendFunction_Init(void);

#endif
/*========================File end===================================================================================================*/

