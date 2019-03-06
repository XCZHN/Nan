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
**  Function:	语音模块驱动程序 
==                                                                                         
**  Data:       2014/09/18                                                                       
================================================================================================*/

#include "Drv_Voice.h"
#include "Drv_Uart.h"

extern void delay_ms(uint32 nms);


/***********************************************************************************************
* Function Name  : SendDataToVoice
* Description	 : 串口给语音模块发送数据
* Input 		 : pData -- 数据指针, Len --数据长度
* Output		 : None
* Return		 : None
***********************************************************************************************/
void SendDataToVoice(uchar8* pData, uchar8 Len)
{	
	UART2_SendNxByte(pData, Len);
}

/***********************************************************************************************
* Function Name  : Voice_SendCmd
* Description	 : 给语音模块发送指令
* Input 		 : u8Cmd -- 指令
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
* Description	 : 语音模块初始化操作
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
* Description	 : 设置语音音量等级
* Input 		 : VoiceLvel--语音音量等级(0-1E)
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
* Description	 : 播放指定语音
* Input 		 : u16Voice -- 语音编号
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
* Description	 : 等待语言模块进入空闲状态(播放完上一指令语言)
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Wait_VoiceModule_Free(void)
{
	volatile uint32 time = 0;

	delay_ms(30);  //等待进入播放状态
	while(1 == Get_VoiceBusyBit)
	{
		time ++;
		if(time > 10000)   //防止死锁
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


