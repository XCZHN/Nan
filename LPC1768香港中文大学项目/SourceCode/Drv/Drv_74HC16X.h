/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                        
==                                                                                          
**  Version:	v2.0.0                                                                          
==
**  Filename:	Drv_74HC16X.h                                                                       
==                                                                                         
**  Function:	74HC164\74HC165�߼����� 
==                                                                                         
**  Data:       2015/08/21                                                                      
================================================================================================*/

#ifndef _DRV_74HC16X_H_
#define _DRV_74HC16X_H_


#include "Config.h"
#include "LPC17xx.h"


#define CLK164			    3		// P2.3
#define DATA164			    16		// P0.16
#define CLK165			    21		// P0.21
#define LOAD165			    19	 	// P0.19
#define DATA165			    22	 	// P0.22
#define DATA165_2			20	 	// P0.20

#define SET_CLK164_PIN		(LPC_GPIO2->FIOSET |= (1<<CLK164)) 
#define CLR_CLK164_PIN		(LPC_GPIO2->FIOCLR |= (1<<CLK164))

#define SET_DATA164_PIN		(LPC_GPIO0->FIOSET |= (1<<DATA164)) 
#define CLR_DATA164_PIN		(LPC_GPIO0->FIOCLR |= (1<<DATA164))


#define SET_CLK165_PIN		(LPC_GPIO0->FIOSET |= (1<<CLK165)) 
#define CLR_CLK165_PIN		(LPC_GPIO0->FIOCLR |= (1<<CLK165))

#define SET_LOAD165_PIN		(LPC_GPIO0->FIOSET |= (1<<LOAD165)) 
#define CLR_LOAD165_PIN		(LPC_GPIO0->FIOCLR |= (1<<LOAD165))


#define GET_HC165_DATA_STATE		 (LPC_GPIO0->FIOPIN&0x400000)  //����״̬	
#define GET_HC165_DATA2_STATE		 (LPC_GPIO0->FIOPIN&0x100000)  //̽��״̬	

/***********************************************************************************************
* Function Name  : Drv_74HC164_Init
* Description	 : 74HC164���Ƴ�ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Drv_74HC164_Init(void);

/***********************************************************************************************
* Function Name  : Drv_74HC165_Init
* Description	 : 74HC165���Ƴ�ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Drv_74HC165_Init(void);

/***********************************************************************************************
* Function Name  : ManualOpen1Box
* Description	 : ����һ������,������,������
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void ManualOpen1Box(uchar8 BoxNum);

/***********************************************************************************************
* Function Name  : CloseAllBox
* Description	 : ����������
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void CloseAllBox(void);

/***********************************************************************************************
* Function Name  : OpenOneBox
* Description	 : ����һ������,�������Զ�����
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void OpenOneBox(uchar8 BoxNum);

/***********************************************************************************************
* Function Name  : GetBoxDoorStatus
* Description	 : ��������״̬
* Input 		 : pStatusBuf -- ��ȡ��״̬�ֽڴ������ַ, u8BoxNum -- ��ȡ����������
* Output		 : ���ŵ�״̬,ÿ���ֽڶ�Ӧ8�����ŵ�״̬,��һ�ֽڵ����λ��Ӧ��1����״̬
                   ��һ�ֽ�bit0 -- 7 ��Ӧ1--8������, 0��ʾ�ر�,1��ʾ����
                   .............
                   ��N�ֽ�bit0 -- 7 ��Ӧ((N-1)*8+1)--(N*8)������, 0��ʾ�ر�,1��ʾ����
* Return		 : ��ȡ���ֽ���
***********************************************************************************************/
extern uchar8 GetBoxDoorStatus(uchar8* pStatusBuf, uchar8 u8BoxNum);

/***********************************************************************************************
* Function Name  : GetBoxInsideStatus
* Description	 : �����Ŵ���״̬
* Input 		 : pStatusBuf -- ��ȡ��״̬�ֽڴ������ַ, u8BoxNum -- ��ȡ����������
* Output		 : ���ŵ�״̬,ÿ���ֽڶ�Ӧ8�����ŵ�״̬,��һ�ֽڵ����λ��Ӧ��1����״̬
                   ��һ�ֽ�bit0 -- 7 ��Ӧ1--8������, 1��ʾ����Ʒ,0��ʾ����(û����Ʒ)
                   .............
                   ��N�ֽ�bit0 -- 7 ��Ӧ((N-1)*8+1)--(N*8)������, 1��ʾ����Ʒ,0��ʾ����(û����Ʒ)
* Return		 : ��ȡ���ֽ���
***********************************************************************************************/
extern uchar8 GetBoxInsideStatus(uchar8* pStatusBuf, uchar8 u8BoxNum);

#endif
/*========================File end===================================================================================================*/

