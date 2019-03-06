/*===============================================================================================
==                                                                                              
**  Copyright(c) 2015  shenzhen zhenyao technology co., LTD. All rights reserved.
==                                                                                           
**	Author:		zenghl                                                                          
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Fingerprint.c                                                                      
==                                                                                         
**  Function:	指纹识别应用层文件  
==
**  Data:       2015-11-26
================================================================================================*/

#include "Includes.h"


uchar8 FingerprintData[520] = {0};  //存储指纹数据(指纹号2B + 512B指纹数据)
uchar8 FingerprintReplyData[20] = {0};


/***********************************************************************************************
* Function Name  : Fingerprint_Init
* Description	 : 指纹模块初始化
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Fingerprint_Init(void)
{
	LPC_PINCON->PINSEL4 &= 0xFFFFFFCF;
	LPC_GPIO2->FIODIR &= ~(1<<Fingerprint_pin);
}

/***********************************************************************************************
* Function Name  : SendDataToFingerprint
* Description	 : 发送指令到指纹模块
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 发送前先清除接收存储区  
***********************************************************************************************/
void SendDataToFingerprint(uchar8* pBuf, uint16 len)
{
	//先清除下接收存储区
	memset(FingerprintDataBuf, 0x0, sizeof(FingerprintDataBuf));
	FingerprintDataCnt =0;

	UART1_SendNxByte(pBuf, len);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint
* Description	 : 发送指令到指纹模块
* Input 		 : PackageID :包标识(0x01--命令包, 0x02--数据包, 0x07--应答包, 0x08--结束包) 
                   cmd : 指令码
                   pPackageBuf : 附带数据包
                   PackageLen : 数据包长度
* Output		 : None
* Return		 : None
* Note  		 : 发送数据后悔清除接收存储区,以便于后续接收数据
***********************************************************************************************/
void SendCmdToFingerprint(uchar8 PackageID, uchar8 cmd, uchar8* pPackageBuf, uint16 PackageLen)
{
	uchar8 SendBuf[256] = {0};
	uint16 SendLen = 0;
	uint16 Crc = 0;
	uint16 i = 0;

	//清除下接收存储区
	memset(FingerprintDataBuf, 0xFF, sizeof(FingerprintDataBuf));
	FingerprintDataCnt =0;

	//包头
	SendBuf[SendLen++] = 0xEF;
	SendBuf[SendLen++] = 0x01;

	//模块地址
	SendBuf[SendLen++] = 0xFF;
	SendBuf[SendLen++] = 0xFF;
	SendBuf[SendLen++] = 0xFF;
	SendBuf[SendLen++] = 0xFF;

    //包标识
	SendBuf[SendLen++] = PackageID;


	//指令码
	if(ZW_PackageID_CMD == PackageID)
	{
		//包长度(指令+数据)
		SendBuf[SendLen++] = ((PackageLen +3)&0xFF00) >> 8;
		SendBuf[SendLen++] = (PackageLen +3) &0xFF;

		SendBuf[SendLen++] = cmd;
	}
	else
	{
		//包长度(数据)
		SendBuf[SendLen++] = ((PackageLen +2)&0xFF00) >> 8;
		SendBuf[SendLen++] = (PackageLen +2) &0xFF;
	}

	//数据
	if(PackageLen)
	{
		memcpy(&SendBuf[SendLen], pPackageBuf, PackageLen);
		SendLen += PackageLen;
	}

	//校验码
	for(i=6; i<SendLen; i++)
	{
		Crc += SendBuf[i];
	}
	SendBuf[SendLen++] = (Crc & 0xFF00) >> 8;
	SendBuf[SendLen++] = Crc & 0xFF;
	
	//发送指令包
	UART1_SendNxByte(SendBuf, SendLen);
}

/***********************************************************************************************
* Function Name  : CheckFingerprintReply
* Description	 : 指纹模块的回复数据
* Input 		 : CmdDataLen : 回复指令中的数据长度                 
* Output		 : pCmdDataBuf : 回复指令中的数据
* Return		 : 指令执行确认码
* Note  		 : 
***********************************************************************************************/
uchar8 CheckFingerprintReply(uint16 CmdDataLen, uchar8* pCmdDataBuf)
{
	uchar8 Ret = 0xFF;
	uint16 Crc = 0;
	uint16 i = 0;
		
	if(FingerprintDataCnt >= (12 + CmdDataLen))
 	{
 		if((FingerprintDataBuf[0]==0xEF)&&(FingerprintDataBuf[1]==0x01)&&(FingerprintDataBuf[2]==0xFF)
			&&(FingerprintDataBuf[3]==0xFF)&&(FingerprintDataBuf[4]==0xFF)&&(FingerprintDataBuf[5]==0xFF))
 		{
 			CmdDataLen = FingerprintDataBuf[7] * 256;
			CmdDataLen += FingerprintDataBuf[8];
			CmdDataLen -= 3;  //减去确认码1B和校验和2B
				
 			Crc = FingerprintDataBuf[6];  //包标识
			Crc += FingerprintDataBuf[7];
			Crc += FingerprintDataBuf[8];
			Crc += FingerprintDataBuf[9]; //确认码

			for(i=0; i<CmdDataLen; i++)
			{
				Crc += FingerprintDataBuf[10 + i];
				//提取数据部分
				pCmdDataBuf[i] = FingerprintDataBuf[10 + i];
			}
			
			if(Crc == (FingerprintDataBuf[10+i]*256+FingerprintDataBuf[11+i]))
			{
	 			Ret = FingerprintDataBuf[9]; //返回状态码
			}
			else
			{
				Ret = 0xCC;
			} 		
		}
	}

	return Ret;
}

/***********************************************************************************************
* Function Name  : AwaitingerprintResult
* Description	 : 等待指纹模块的回复指令执行结果
* Input 		 : AwaitTime_ms -- 等待时间(ms)
* Output		 : None
* Return		 : 指令执行确认码(0x01--收包有错...)
* Note  		 : 
***********************************************************************************************/
uchar8 AwaitingerprintResult(uint32 WaitTimeout_ms)
{
	uchar8 Ret = 0xFF;
	uint16 Crc = 0;
	uint16 DataLen = 0;
	uint16 i = 0;

	while(WaitTimeout_ms --)
	{
		if(FingerprintDataCnt >= 12)
	 	{
	 		if((FingerprintDataBuf[0]==0xEF)&&(FingerprintDataBuf[1]==0x01)&&(FingerprintDataBuf[2]==0xFF)
				&&(FingerprintDataBuf[3]==0xFF)&&(FingerprintDataBuf[4]==0xFF)&&(FingerprintDataBuf[5]==0xFF))
	 		{
	 			DataLen = FingerprintDataBuf[7] * 256;
				DataLen += FingerprintDataBuf[8];
				DataLen -= 3;  //减去确认码1B和校验和2B
					
	 			Crc = FingerprintDataBuf[6];  //包标识
				Crc += FingerprintDataBuf[7];
				Crc += FingerprintDataBuf[8];
				Crc += FingerprintDataBuf[9]; //确认码

				for(i=0; i<DataLen; i++)
				{
					Crc += FingerprintDataBuf[10 + i];
				}
				
				if(Crc == (FingerprintDataBuf[10+i]*256+FingerprintDataBuf[11+i]))
				{
		 			Ret = FingerprintDataBuf[9];
				}
				else
				{
					Ret = 0xCC;
				}
	 		}
			return Ret;
		}
		else
		{
			delay_ms(1);
		}
	}

	return Ret;
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_GetImg
* Description	 : 获取图像
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 发送:EF 01	FF FF FF FF 01 00 03 01 00 05
*				   返回:EF 01	FF FF FF FF 07 00 03 xx(确认码1字节) sum(校验和2字节)
*				   确认码=0x00 表示录入成功
*				   确认码=0x01 表示收包有错
*				   确认码=0x02 表示传感器上无手指
*				   确认码=0x03 表示录入不成功
***********************************************************************************************/
void SendCmdToFingerprint_GetImg(void)
{
	uchar8 SendBuf[12] = {0};
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_GetImg, SendBuf, 0);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_GenChar
* Description	 : 图像生成特征
* Input 		 : BufferID---缓存ID
* Output		 : None
* Return		 : None
* Note  		 : 发送:EF 01	FF FF FF FF 01 00 04 02 BufferID sum
*				   返回:EF 01	FF FF FF FF 07 00 03 xx(确认码1字节) sum(校验和2字节)
*				   确认码=0x00 表示生成特征成功
*				   确认码=0x01 表示收包有错
*				   确认码=0x06 表示指纹图像太乱而生不成特征
*				   确认码=0x07 表示指纹图像正常，但特征点太少而生不成特征
*				   确认码=0x15 表示图像缓冲区内没有有效原始图而生不成图像
***********************************************************************************************/
void SendCmdToFingerprint_GenChar(uchar8 BufferID)
{
	uchar8 SendBuf[12] = {0};
	SendBuf[0] = BufferID;
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_GenChar, SendBuf, 1);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_RegModel
* Description	 : 特征合成模板
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 发送:EF 01	FF FF FF FF 01 00 03 05 00 09
*				   返回:EF 01	FF FF FF FF 07 00 03 xx(确认码1字节) sum(校验和2字节)
*				   确认码=0x00 表示合并成功
*				   确认码=0x01 表示收包有错
*				   确认码=0x0a 表示合并失败(两枚指纹不属于同一手指)
***********************************************************************************************/
void SendCmdToFingerprint_RegModel(void)
{
	uchar8 SendBuf[12] = {0};
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_RegModel, SendBuf, 0);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_Store
* Description	 : 存储模板
* Input 		 : BufferID---缓存ID   PageID--存储位置ID
* Output		 : None
* Return		 : None
* Note  		 :  发送:EF 01	FF FF FF FF 01(包标识01) 00 06(包长度0006) 06(指令代码06) BufferID(缓冲区号1字节) PageID(位置号2字节) 
     				 sum(检验和2字节)
*					返回:EF 01	FF FF FF FF 07 00 03 xx(确认码1字节) sum(校验和2字节)
*					确认码=0x00 表示存储成功
*					确认码=0x01 表示收包有错
*					确认码=0x0b 表示PageID超出指纹库范围
*					确认码=0x11 表示写FLASH出错
***********************************************************************************************/
void SendCmdToFingerprint_Store(uchar8 BufferID, uint16 PageID)
{
	uchar8 SendBuf[12] = {0};
	SendBuf[0] = BufferID;
	SendBuf[1] = ((PageID&0xFF00)>>8); 
	SendBuf[2] = (PageID&0x00FF);
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_Store, SendBuf, 3);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_DeletChar
* Description	 : 删除模板
* Input 		 : PageID--存储位置ID  num--删除字节数
* Output		 : None
* Return		 : None
* Note  		 : 发送:EF 01	FF FF FF FF 01 00 07(包长度) 0C(命令) PageID(页码2字节) N(删除个数2字节) sum(检验和2字节)
*				   返回:EF 01	FF FF FF FF 07 00 03 xx(确认码1字节) sum(校验和2字节)
*				   确认码=0x00 表示删除模板成功
*				   确认码=0x01 表示收包有错
*				   确认码=0x02 表示删除模板失败
***********************************************************************************************/
void SendCmdToFingerprint_DeletChar(uint16 PageID, uint16 num)
{
	uchar8 SendBuf[12] = {0};
	SendBuf[0]= ((PageID & 0xFF00)>>8);
	SendBuf[1]= (PageID & 0x00FF);	
	SendBuf[2]= ((num & 0xFF00)>>8);
	SendBuf[3]= (num & 0x00FF);	
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_DeletChar, SendBuf, 4);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_Empty
* Description	 : 清空指纹库
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 发送:EF 01	FF FF FF FF 01 00 03 0d 00 11
*				   返回:EF 01	FF FF FF FF 07 00 03 xx(确认码1字节) sum(校验和2字节)
*				   确认码=0x00 表示清空成功
*				   确认码=0x01 表示收包有错
*				   确认码=0x02 表示清空失败
***********************************************************************************************/
void SendCmdToFingerprint_Empty(void)
{
	uchar8 SendBuf[12] = {0};
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_Empty, SendBuf, 0);
}

/***********************************************************************************************
* Function Name  : SendCmdToFingerprint_Search
* Description	 : 搜索指纹
* Input 		 : BufferID--缓存ID  StartPage--起始页   PageNum--页数
* Output		 : None
* Return		 : None
* Note  		 : 发送:EF 01	FF FF FF FF 01(包标识01) 00 08(包长度0008) 04(指令代码04) BufferID(缓冲区号1字节) StartPage(起始页2字节) 
*    		       PageNum(页数2字节) sum(检验和2字节)
*				   返回:EF 01	FF FF FF FF 07 00 07 xx(确认码1字节) PageID(页码2字节) MatchScore(得分2字节) sum(校验和2字节)
*				   确认码=0x00 表示搜索到
*				   确认码=0x01 表示收包有错
*				   确认码=0x09 表示没搜索到，此时页码与得分为0
***********************************************************************************************/
void SendCmdToFingerprint_Search(uchar8 BufferID, uint16 StartPage, uint16 PageNum)
{
	uchar8 SendBuf[12] = {0};
	SendBuf[0]= BufferID;
	SendBuf[1]= ((StartPage & 0xFF00)>>8);
	SendBuf[2]= (StartPage & 0x00FF);	
	SendBuf[3]= ((PageNum & 0xFF00)>>8);
	SendBuf[4]= (PageNum & 0x00FF);	
	SendCmdToFingerprint(ZW_PackageID_CMD, ZW_CMD_Search, SendBuf, 5);
}

/***********************************************************************************************
* Function Name  : Fingerprint_Search
* Description	 : 指纹搜索
* Input 		 : handle -- (=0xFF操作超时取消指纹录入)
* Output		 : None
* Return		 : 搜索到匹配的指纹返回指纹号(1--200), 
                   0xA000-0xA00F 对应当前执行步骤0-16
				   0xE0xx-0xEFxx 错误代号, 0xFFFF--未找到匹配指纹号
* Note  		 : 内部包含了操作提示
***********************************************************************************************/
uint16 Fingerprint_Search(uint16 handle)
{
	static uint32 LastScanTime;
	static uchar8 HandleStep;
	uchar8 cRet = 0xFF;
	uint16 iHandleRet = 0xA000;

	if(ZW_StopHandle == handle)  //因操作超时取消指纹录入
	{
		HandleStep = 0;
		return 0xFFFF;
	}

	switch(HandleStep)
	{	
		case 0: //检测是否有手指触摸
		{
			if(0x00 == GET_Fingerprint_STATE)
			{
			//	delay_ms(100);
			}
			
			if(0x00 == GET_Fingerprint_STATE)
			{
				HandleStep ++;	
			}
			else
			{
				HandleStep = 0;
				return 0xE000;
			}
		}
		break;

		case 1: //第一次采集
		{ 
			SendCmdToFingerprint_GetImg();
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
			HandleStep++;
		}
		break;

		case 2:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 1100)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);
	 			if(0x00 == cRet)
	 			{
	 				HandleStep++;
	 			}
				else if(0xFF != cRet)
				{
					//接收到数据包不完整
					HandleStep = 0;
					//GUI_UserAccess(9); //9--指纹录入错误
					return 0xE100;
				}
			}
			else //超时-- 接收到数据包不完整or超时未回应
			{
				HandleStep = 0;
				//GUI_UserAccess(9); //9--指纹录入错误
				return 0xE200;
			}
		}
		break;

		case 3: //图像生成特征码1
		{
			SendCmdToFingerprint_GenChar(1);
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 4:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 1000)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);
	 			if(0x00 == cRet)
	 			{
	 				HandleStep++;
	 			}
				else if(0xFF != cRet)
				{
					//生产特征码失败
					HandleStep = 0;
					//GUI_UserAccess(9); //9--指纹录入错误
					return 0xE300;
				}
			}
			else //超时--接收到数据包不完整or超时未回应
			{
				HandleStep = 0;
				//GUI_UserAccess(9); //9--指纹录入错误
				return 0xE400;
			}
		}
		break;

		case 5://检查指纹是否已授权
		{
			SendCmdToFingerprint_Search(1, 0, MAX_ZW_UNM);//检测当前MAX_ZW_UNM枚指纹
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 6:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 2300)
			{
			 	cRet = CheckFingerprintReply(4, FingerprintReplyData);
	 			if(0x09 == cRet) //未搜索到
	 			{
	 				HandleStep = 0;
					return 0xFFFF;
	 			}
				else if(0x00 == cRet)//搜索到指纹
				{
					HandleStep = 0;
					return (FingerprintReplyData[0]*256+FingerprintReplyData[1]);
				}
				else if(0xFF != cRet)
				{
					//接收到数据包不完整
					HandleStep = 0;
					//GUI_UserAccess(13);//指纹未授权
					return 0xE600;
				}
			}
			else //超时--接收到数据包不完整or超时未回应	
			{								
				HandleStep = 0;
				//GUI_UserAccess(13);//指纹未授权
				return 0xE700;
			}
		}
		break;

		default:
		{
			HandleStep = 0;
		}
		break;
	}	
	
	iHandleRet += HandleStep; 
	return iHandleRet;
}

/***********************************************************************************************
* Function Name  : Fingerprint_Login
* Description	 : 指纹注册
* Input 		 : FingerNum -- 生成存储指纹号(=0xFFFF操作超时取消指纹录入)
* Output		 : None
* Return		 : 0xA000-0xA00F 对应当前执行步骤0-16
                   >=0xE000--出现异常,终止执行, 
                   0xABCD--录入指纹成功
* Note  		 : 内部包含了操作提示
***********************************************************************************************/
uint16 Fingerprint_Login(uint16 FingerNum)
{
	static uint32 LastScanTime, Timeout;
	static uchar8 HandleStep;
	uchar8 cRet = 0xFF;

	if(FingerNum >= MAX_ZW_UNM)  
	{
		HandleStep = 0;
		return 0xA000;
	}
	
	switch(HandleStep)
	{
		case 0: //检测是否有手指触摸
		{
			if(0x00 == GET_Fingerprint_STATE)
			{
				delay_ms(321);
			}
			
			if(0x00 == GET_Fingerprint_STATE)
			{
				LastScanTime = gstSysRunInfo.u32TimeCount_ms;
				HandleStep++;				
			}
			else
			{
				if(gstSysRunInfo.u32TimeCount_ms-LastScanTime > 3500)
				{
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;
					Voice_Play(YY_QingAnZhiWen);
				}
				HandleStep =0;
			}
		}
		break;

		case 1: //第一次采集
		{ 
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
			GUI_UserAccess(2);
			SendCmdToFingerprint_GetImg();
			HandleStep++;
		}
		break;

		case 2:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 1100)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);				
	 			if(0x00 == cRet)
	 			{
	 				HandleStep++;
	 			}
				else if(0xFF != cRet)
				{
					HandleStep = 0;
					GUI_UserAccess(9); //9--指纹录入错误
					return 0xE200;
				}
			}
			else //超时--接收到数据包不完整or超时未回应	
			{
				HandleStep = 0;
				GUI_UserAccess(9); //9--指纹录入错误
				return 0xE300;
			}
		}
		break;

		case 3: //图像生成特征码1
		{
			SendCmdToFingerprint_GenChar(1);
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 4:
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime < 1000)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);				
	 			if(0x00 == cRet)
	 			{
	 				Timeout = gstSysRunInfo.u32TimeCount_ms;
	 				HandleStep++;
					GUI_UserAccess(3); //提示"请移开手指"
					if(0x00 == GET_Fingerprint_STATE)
					{
						Voice_Play(YY_QingYiKaiShouZhi);
					}
	 			}
				else if(0xFF != cRet)
				{
					//生成特征码失败
					HandleStep = 0;
					GUI_UserAccess(9); //9--指纹录入错误
					return 0xE400;
				}
			}
			else //超时--接收到数据包不完整or超时未回应
			{										
				HandleStep = 0;
				GUI_UserAccess(9); //9--指纹录入错误
				return 0xE500;
			}
		}
		break;

		case 5: //等待手指移开
		{
			if((GET_Fingerprint_STATE)||(gstSysRunInfo.u32TimeCount_ms - Timeout > 3210))
			{
				HandleStep++;
			}
			else
			{
				if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 2000)
				{
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;
					GUI_UserAccess(3); //提示"请移开手指"
					Voice_Play(YY_QingYiKaiShouZhi);
				}
			}
		}
		break;
		
		case 6: //提示再按指纹
		{
			if(GET_Fingerprint_STATE)
			{
				GUI_UserAccess(4); //提示"请再按指纹"
				Voice_Play(YY_QingZaiAnZhiWen);
			}
			HandleStep++;
		}
		break;

		case 7://第二次采集 
		{
			if(0x00 == GET_Fingerprint_STATE)
			{
				delay_ms(220);
				if(0x00 == GET_Fingerprint_STATE)
				{
					GUI_UserAccess(5); //第二次采集指纹
					SendCmdToFingerprint_GetImg();
					HandleStep++;
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;				
				}
			}
		}
		break;

		case 8: //解析第二次采集回应数据 
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime < 1100)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);				
	 			if(0x00 == cRet)
	 			{
	 				HandleStep++;
	 			}
				else if(0xFF != cRet)
				{
					//接收到数据包不完整
					HandleStep = 0;
					GUI_UserAccess(9);
					return 0xE800;
				}
			}
			else //超时--接收到数据包不完整or超时未回应
			{										
				HandleStep = 0;
				GUI_UserAccess(9); //9--指纹录入错误
				return 0xE900;
			}
		}
		break;

		case 9: //图像生成特征码2
		{
			SendCmdToFingerprint_GenChar(2);
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 10:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 700)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);				
	 			if(0x00 == cRet)
	 			{
	 				HandleStep++;
	 			}
				else if(0xFF != cRet)
				{
					//生成特征码2失败
					HandleStep = 0;
					GUI_UserAccess(9);
					return 0xEA00;
				}
			}
			else //超时--接收到数据包不完整or超时未回应
			{										
				HandleStep = 0;
				GUI_UserAccess(9); //9--指纹录入错误
				return 0xEC00;
			}
		}
		break;

		case 11: //特征合成模板
		{
			SendCmdToFingerprint_RegModel();
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 12:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 500)
			{
			 	cRet = CheckFingerprintReply(0, FingerprintReplyData);				
	 			if(0x00 == cRet)
	 			{
	 				HandleStep++;
	 			}
				else if(0xFF != cRet)
				{
					//合成模块失败
					HandleStep = 0;
					GUI_UserAccess(10); //10--指纹合并失败
					return 0xED00;
				}
			}
			else //超时--接收到数据包不完整or超时未回应
			{										
				HandleStep = 0;
				GUI_UserAccess(10); //10--指纹合并失败 
				return 0xEF00;
			}
		}
		break;

		case 13://检查指纹是否已授权
		{
			SendCmdToFingerprint_Search(1, 0, MAX_ZW_UNM);//检测当前MAX_ZW_UNM枚指纹
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 14:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 700)
			{
			 	cRet = CheckFingerprintReply(4, FingerprintReplyData);				
	 			if(0x09 == cRet) //未搜索到
	 			{
	 				HandleStep++;
	 			}
				else if(0x00 == cRet) //指纹已授权
				{
					HandleStep = 0;
					GUI_UserAccess(12);
					return 0xF000;
				}
				else if(0xFF != cRet)
				{
					HandleStep = 0;
					GUI_UserAccess(9);
					return 0xF100;
			 	}
			}
			else //超时--接收到数据包不完整or超时未回应
			{										
				HandleStep = 0;
				GUI_UserAccess(9);	
				return 0xF300;
			}
		}
		break;

		case 15://存储指纹
		{
			SendCmdToFingerprint_Store(1, FingerNum);
			HandleStep++;
			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		}
		break;

		case 16:
		{
			if(gstSysRunInfo.u32TimeCount_ms-LastScanTime < 500)
			{
				cRet = CheckFingerprintReply(0, FingerprintReplyData);				
				if(0x00 == cRet)
	 			{
	 				HandleStep = 0;
					GUI_UserAccess(6);
					return 0xABCD; //指纹录入成功,完成操作
	 			}
				else if(0xFF != cRet)
				{
					HandleStep = 0;
					GUI_UserAccess(7);
					return 0xF500;
				}
			}
			else //超时--接收到数据包不完整or超时未回应
			{
				HandleStep = 0;
				GUI_UserAccess(7); //指纹存储失败
				return 0xF600;
			}
		}
		break;
		
		default:
		{
			HandleStep =0;
		}
		break;
	}

	return (HandleStep +0xA000);
}

/***********************************************************************************************
* Function Name  : Fingerprint_GetChar
* Description	 : 读取一个指纹数据
* Input 		 : FingerID -- 存储的指纹号 
* Output		 : None
* Return		 : 0x01--获取成功, 其它 --获取失败, 
* Note  		 :  
***********************************************************************************************/
uchar8 Fingerprint_GetChar(uint16 FingerID)
{
	uchar8 u8Ret = 0;
	uchar8 Buf[12] = {0};
	uint16 Len = 0;
		
	memset(FingerprintData, 0x0, sizeof(FingerprintData));
	
	//读出模块到Buffer1中
	Buf[Len++] = 1;
	Buf[Len++] = (FingerID & 0xFF00) >>8;
	Buf[Len++] = FingerID & 0xFF;
	SendCmdToFingerprint(0x01, ZW_CMD_LoadChar, Buf, Len);
	u8Ret = AwaitingerprintResult(321);
	if(0 == u8Ret)
	{
		//上传特征或模板
		Len = 0;
		memset(Buf, 0x0, sizeof(Buf));
		Buf[Len++] = 1;  //Buffer1
		SendCmdToFingerprint(0x01, ZW_CMD_UpChar, Buf, Len);
		u8Ret = AwaitingerprintResult(520);
		delay_ms(520);  //等待指纹数据
		if(0 == u8Ret)
		{
			if(568 == FingerprintDataCnt)
			{
				//提取指纹数据
				FingerprintData[0] = (FingerID &0xFF00) >>8;
				FingerprintData[1] = FingerID & 0xFF;
				memcpy(&FingerprintData[2], &FingerprintDataBuf[21], 128);
				memcpy(&FingerprintData[130], &FingerprintDataBuf[160], 128);
				memcpy(&FingerprintData[258], &FingerprintDataBuf[299], 128);
				memcpy(&FingerprintData[386], &FingerprintDataBuf[438], 128);

				u8Ret = 1;
			}
			else
			{
				u8Ret = 0xFF;
			}
		}
		else
		{
			u8Ret = 0xFF;
		}
	}
	
	return u8Ret;
}

/*========================File end==============================================================================*/

