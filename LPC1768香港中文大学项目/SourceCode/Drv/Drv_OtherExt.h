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
**  Function:	������չ����ģ���������Ƴ���: �̵�������������ָʾ�ơ�������   
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
* Description	 : LED��ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void LED_Init(void);

/***********************************************************************************************
* Function Name  : Beep_Init
* Description	 : ��������ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Beep_Init(void);

/***********************************************************************************************
* Function Name  : ExtendFunction_Init
* Description	 : ��չ���ܳ�ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : RS485����ͨ��PA0�ܽŵ�ƽѡ��
***********************************************************************************************/
extern void ExtendFunction_Init(void);

#endif
/*========================File end===================================================================================================*/

