/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                        
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_Keyboard.h                                                                      
==                                                                                         
**  Function:	��������������� 
==                                                                                         
**  Data:       2014/09/18                                                                       
================================================================================================*/

#ifndef _DRV_KEYBOARD_H_
#define _DRV_KEYBOARD_H_


#include "Config.h"
#include "LPC17xx.h"


#define KB_PIN01     (1 << 4)  //P0.4
#define KB_PIN02     (1 << 5)  //P0.5
#define KB_PIN03     (1 << 6)  //P0.6
#define KB_PIN04     (1 << 7)  //P0.7
#define KB_PIN05     (1 << 8)  //P0.8
#define KB_PIN06     (1 << 9)  //P0.9
#define KB_PIN07     (1 << 10) //P0.10
#define KB_PIN08     (1 << 11) //P0.11

//#define TOUCH_KB_BUS     (KB_PIN01 | KB_PIN02 | KB_PIN03 | KB_PIN04 | KB_PIN05 | KB_PIN06 | KB_PIN07 | KB_PIN08)  
//ΪʡȥƵ������,ֱ�Ӹ������
#define TOUCH_KB_BUS  0x00000FF0


#define CSKEY			       28		// P4.28

#define SET_CSKEY_PIN		   (LPC_GPIO4->FIOSET |= (1<<CSKEY)) 
#define CLR_CSKEY_PIN		   (LPC_GPIO4->FIOCLR |= (1<<CSKEY))

#define KB_ENABLE              CLR_CSKEY_PIN
#define KB_DISNABLE            SET_CSKEY_PIN

//--------------------���̰�������---------------------------------------------------------------
#define Key_NoDown         			0x00   //û�м�����
#define Key_Null         			0xFF   //û�м�����


#define Key_1                       0x31
#define Key_2                       0x32
#define Key_3                       0x33
#define Key_4                       0x34
#define Key_5                       0x35
#define Key_6                       0x36
#define Key_7                       0x37
#define Key_8                       0x38
#define Key_9                       0x39
#define Key_0                       0x30

#define Key_Star                    0xF0   //"*"
#define Key_Pwd                     0xFD   //����
#define Key_Jin                     0xFE   //"#"
#define Key_Shift                   0xAB   //�л�
#define Key_F1                      0xF1   //"F1"
#define Key_F2                      0xF2   //"F2"

#define Key_Up         	            0xA1   
#define Key_Down         	        0xA2   
#define Key_Left        			0xA3  
#define Key_Right                 	0xA4   
#define Key_Enter                   0xA5   
#define Key_Back                    0xEE   //����
#define Key_Cancel                  0xEF   //ȡ��

#define Key_JiCun                   0xFD
#define Key_QuJian                  0xFD


/***********************************************************************************************
* Function Name  : Keyboard_Init
* Description	 : ���̳�ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Keyboard_Init(void);


/***********************************************************************************************
* Function Name  : Key_Scan
* Description	 : ����ɨ��
* Input 		 : None
* Output		 : None
* Return		 : ����ֵ
***********************************************************************************************/
extern uchar8 Key_Scan(void);

/***********************************************************************************************
* Function Name  : Key_Get_KeyDown
* Description	 : ��ȡ����
* Input 		 : None
* Output		 : None
* Return		 : ����ֵ
***********************************************************************************************/
extern uchar8 Key_Get_KeyDown(void);

/***********************************************************************************************
* Function Name  : NumKeyToNumValue
* Description	 : Key 0-9 ���ּ���־ת��������ֵ
* Input 		 : ����ֵ����
* Output		 : None
* Return		 : ������Ӧ��ֵ
***********************************************************************************************/
extern uchar8  NumKeyToNumValue(uchar8 u8KeyNum);

#endif
/*========================File end===================================================================================================*/

