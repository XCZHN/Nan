/*===============================================================================================
==                                                                                              
**  Copyright(c) 2016  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                        
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_Voice.c                                                                      
==                                                                                         
**  Function:	����ģ���������� 
==                                                                                         
**  Data:       2014/09/18                                                                       
================================================================================================*/

#include "Drv_Voice.h"
#include "Drv_Uart.h"

extern void delay_ms(uint32 nms);


/***********************************************************************************************
* Function Name  : SendDataToVoice
* Description	 : ���ڸ�����ģ�鷢������
* Input 		 : pData -- ����ָ��, Len --���ݳ���
* Output		 : None
* Return		 : None
***********************************************************************************************/
void SendDataToVoice(uchar8* pData, uchar8 Len)
{	
	UART2_SendNxByte(pData, Len);
}

/***********************************************************************************************
* Function Name  : Voice_SendCmd
* Description	 : ������ģ�鷢��ָ��
* Input 		 : u8Cmd -- ָ��
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Voice_SendCmd(uchar8 u8Cmd)
{
	uchar8 VoiceBuf[10] = {0};

	VoiceBuf[0] = 0x7E;
	VoiceBuf[1] = 0x02;
	VoiceBuf[2] = u8Cmd;
	VoiceBuf[3] = 0xEF;

	SendDataToVoice(VoiceBuf, 4);
}

/***********************************************************************************************
* Function Name  : Voice_Init
* Description	 : ����ģ���ʼ������
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Voice_Init(void)
{	
	Voice_SendCmd(YY_Cmd_Reset);	
}

/***********************************************************************************************
* Function Name  : Set_VoiceLevel
* Description	 : �������������ȼ�
* Input 		 : VoiceLvel--���������ȼ�(0-1E)
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Set_VoiceLevel(uchar8 VoiceLvel)
{
	uchar8 VoiceBuf[10] = {0};

	VoiceBuf[0] = 0x7E;
	VoiceBuf[1] = 0x03;
	VoiceBuf[2] = 0x06;
	VoiceBuf[3] = VoiceLvel;
	VoiceBuf[4] = 0xEF;

	SendDataToVoice(VoiceBuf, 5);
}


/***********************************************************************************************
* Function Name  : Voice_Play
* Description	 : ����ָ������
* Input 		 : u16Voice -- �������
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Voice_Play(uint16 u16Voice)
{
	uchar8 VoiceBuf[10] = {0};

	VoiceBuf[0] = 0x7E;
	VoiceBuf[1] = 0x04;
	VoiceBuf[2] = 0x03;
	VoiceBuf[3] = (u16Voice & 0xFF00) >> 8;
	VoiceBuf[4] = u16Voice & 0xFF;
	VoiceBuf[5] = 0xEF;

	SendDataToVoice(VoiceBuf, 6);
}

/***********************************************************************************************
* Function Name  : Wait_VoiceModule_Free
* Description	 : �ȴ�����ģ��������״̬(��������һָ������)
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Wait_VoiceModule_Free(void)
{
	volatile uint32 time = 0;

	delay_ms(30);  //�ȴ����벥��״̬
	while(1 == Get_VoiceBusyBit)
	{
		time ++;
		if(time > 10000)   //��ֹ����
		{
			break;
		}
		else
		{
			delay_ms(1);
		}
	}
}



/*========================File end==========================================================================*/


