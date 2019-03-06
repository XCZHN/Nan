/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                     
==                                                                                          
**  Version:	v2.0.0                                                                          
==
**  Filename:	App_KeyUI.c                                                                       
==                                                                                         
**  Function:	���̲����������
==
**  Data:       2014/08/11                                                                       
================================================================================================*/

#ifndef	_APP_KEY_INTERFACE_H_
#define	_APP_KEY_INTERFACE_H_

#include "Config.h"   

typedef struct
{
	uchar8 bHaveKey;			//�а������
	uchar8 u8KeyValue;			//��⵽�ļ�ֵ
	uchar8 u8Box;			    //ѡ�е����
	uchar8 u8NumValue;			//��⵽�����ּ���ֵ
	uchar8 u8KeyCount;			//������ͳ��
	uchar8 u8TempValue;         //��ʱֵ
	uchar8 u8ValueType;         //��ʱֵ����
	uchar8 u8Select;			//�ڲ˵���ѡ����к�
	uchar8 u8InputType;
	uchar8 bAddInto;			//���밴���˵����
	uchar8 u8AdminMenuStep;		//��¼���˵��켣
	uchar8 u8SecondMenuStep;	//��¼�����˵��켣
	uchar8 u8ThreeMenuStep;	    //��¼�����˵��켣
	uchar8 u8FourMenuStep;	    //��¼�ļ��˵��켣
	uchar8 u8FiveMenuStep;	    //��¼�弶�˵��켣

	uchar8 IpBuf[6];
	uchar8 KeyBuf[16];
	uchar8 tempBuf[20];

	uint16 u16TouchID;          //ָ�ƺ�

	uint32 iCardUID;            //����
	uint32 iTempValue;          //��ʱֵ
	uint32 iLastKeyTime;        //�ϴΰ���ʱ��
	
}ST_KEYBOARD;


extern	volatile ST_KEYBOARD gstKeyboard;


/***********************************************************************************************
* Function Name  : Admin_Login
* Description	 : �û��ʺŵ�¼����
* Input 		 : 0--��������, 0xCC--ˢ������
* Output		 : None
* Return		 : None 
* Notes          : ��������: #-->*-->000-->Enter-->PWD
                   ˢ������: Card-->Enter-->PWD
***********************************************************************************************/
extern void Admin_Login(uchar8 Type);

/***********************************************************************************************
* Function Name  : KeyboardFunction
* Description	 : ���̲�������
* Input 		 : None
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
extern void  KeyboardFunction(void);

/***********************************************************************************************
* Function Name  : DepositKeyDown
* Description	 : �Ĵ������
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : ϵͳ����״̬�²�ɨ��[�Ĵ�]��������
***********************************************************************************************/
extern void DepositKeyDown(void);

#endif  //_APP_KEY_INTERFACE_H_
/*===================================File end===================================================*/


