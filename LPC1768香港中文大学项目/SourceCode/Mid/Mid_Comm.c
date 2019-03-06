/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                     
==                                                                                          
**  Version:	V1.0.0                                                                          
==
**  Filename:	Mid_Comm.c                                                                       
==                                                                                         
**  Function:	通讯接收解码、发送组包编码处理
==
**  Others:     
==
**  Data:       2014/04/28                                                          
================================================================================================*/

#include "Includes.h"


ST_RXD_DECODING  Rxd_Decoding;
ST_CMD_PACKET	gstCmdPacket;
ST_CMD_SLAVE_PACKET  stCmdSlavePacket;

TURN_DATA  TD_Comm;


/***********************************************************************************************
* Function Name  : CmdData_Encrypt
* Description	 : 根据密文版本加密命令中的加密数据
* Input 		 : KeyVer -- 密文版本, pCmdData -- 需解密数据的指针, u16SataLen -- 数据长度
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
uchar8 CmdData_Encrypt(uchar8 KeyVer, uchar8* pCmdData, uint16 u16SataLen)
{
	switch(KeyVer)
	{
		case 0x10:
		default:
		{
			//数据无需加密
		}
		break;
	}

	return 0;
}

/***********************************************************************************************
* Function Name  : CmdData_Decoding
* Description	 : 根据密文版本解密命令中的加密数据
* Input 		 : KeyVer -- 密文版本, pCmdData -- 需解密数据的指针, u16SataLen -- 数据长度
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
uchar8 CmdData_Decoding(uchar8 KeyVer, uchar8* pCmdData, uint16 u16SataLen)
{
	switch(KeyVer)
	{
		case 0x10:
		default:
		{
			//数据没有加密,无需解密
		}
		break;
	}

	return 0;
}

/***********************************************************************************************
* Function Name  : Comm_Decoding
* Description	 : 按协议解析命令
* Input 		 : u8data -- 接收数据
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
void Comm_Decoding(uchar8 u8data)
{	
	Rxd_Decoding.u8CmdData = u8data;
		
	switch(Rxd_Decoding.u8Step)	
	{
		case 0: 		// Check header
		{
			if((CMD_HEADER == Rxd_Decoding.u8CmdData)&&(!gstCmdPacket.bGetDataComplete))
			{
				memset((uchar8*)&Rxd_Decoding, 0x0, sizeof(Rxd_Decoding));
				memset((uchar8*)&gstCmdPacket, 0x0, sizeof(gstCmdPacket));
				Rxd_Decoding.u8Step ++;
			}
			else
			{
				Rxd_Decoding.u8Step = 0;
			}
		}
		break;		
		
		case 1: 		// Check header2
		{
			if(CMD_TYPE_ASK == Rxd_Decoding.u8CmdData)
			{
				gstCmdPacket.u8AskOrReplay = CMD_TYPE_ASK;
				Rxd_Decoding.u8Step ++;
			}
			else if(CMD_TYPE_REPLY == Rxd_Decoding.u8CmdData)
			{
				gstCmdPacket.u8AskOrReplay = CMD_TYPE_REPLY;
				Rxd_Decoding.u8Step ++;
			}
			else
			{
				Rxd_Decoding.u8Step = 0;
			}
		}
		break;		

		case 2:
		{
			if(CMD_VERSION == Rxd_Decoding.u8CmdData)
			{				
				Rxd_Decoding.u8Crc = Rxd_Decoding.u8CmdData;
				Rxd_Decoding.u8Step ++;
			}
			else
			{
				Rxd_Decoding.u8Step = 0;
			}
		}
		break;

		case 3:         //设备类型
		{
			gstCmdPacket.u8MachineType = Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Crc ^= Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Step ++;
		}
		break;
		
		case 4: 		//机号--byte0
		{
			TD_Comm.numb32 = 0;
			TD_Comm.numb8[0] = Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Crc ^= Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Step ++;
		}
		break;	
		
		case 5: 		//机号--byte1
		{
			TD_Comm.numb8[1] = Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Crc ^= Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Step ++;
		}
		break;		

		case 6: 		//机号--byte2
		{
			TD_Comm.numb8[2] = Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Crc ^= Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Step ++;
		}
		break;		
		
		case 7: 		//机号--byte3
		{
			TD_Comm.numb8[3] = Rxd_Decoding.u8CmdData;
			gstCmdPacket.u32MachineNum = TD_Comm.numb32;
			Rxd_Decoding.u8Crc ^= Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Step ++;
		}
		break;		
		
		case 8:        //密文版本
		{
			Rxd_Decoding.u8KeyVer = Rxd_Decoding.u8CmdData;
			if(CMD_KEY_VERSION == Rxd_Decoding.u8KeyVer)
			{
				Rxd_Decoding.u8Crc ^= Rxd_Decoding.u8CmdData;
				Rxd_Decoding.u8Step ++;
			}
			else
			{
				Rxd_Decoding.u8Step = 0;
			}
		}
		break;
		
		case 9: 		//Command
		{
			gstCmdPacket.u8Command = Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Crc ^= Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Step ++;		
		}
		break;
		
		case 10:		// length1
		{
			gstCmdPacket.u16length = 0;
			gstCmdPacket.u16length = Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Crc ^= Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Step ++;
		}
		break;
		
		case 11: 		// length2
		{
			gstCmdPacket.u16length += (Rxd_Decoding.u8CmdData *256);	
			Rxd_Decoding.u8Crc ^= Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Step ++;
			if(gstCmdPacket.u16length == 0)
			{ /* 没有数据则跳过取数据环节 */
				Rxd_Decoding.u8Step ++;				
			}
			else if (gstCmdPacket.u16length >= MAX_CMD_DATA_LENGTH)
			{
				Rxd_Decoding.u8Step = 0;
			}
		}
		break;

		case 12:	    //取数据环节
		{
			gstCmdPacket.DataBuf[Rxd_Decoding.u16DataCnt++] = Rxd_Decoding.u8CmdData;
			Rxd_Decoding.u8Crc ^= Rxd_Decoding.u8CmdData;
			if (Rxd_Decoding.u16DataCnt >= gstCmdPacket.u16length)		//检查是否取完数据了
			{
				Rxd_Decoding.u8Step ++;	
			}	
		}
		break;

		case 13:       //CRC
		{			
#ifndef CMD_CALC_CRC								   		//是否校验CRC
			Rxd_Decoding.u8Step ++;
#else
			if (Rxd_Decoding.u8Crc == Rxd_Decoding.u8CmdData)							
			{
			   Rxd_Decoding.u8Step ++;
			}
			else
			{
				Rxd_Decoding.u8Step  = 0;
			}			
#endif						
		}
		break;
		
		case 14:	 // Check end
		{						
			if (CMD_END == Rxd_Decoding.u8CmdData)  		//如果接收到了一笔完整的数据则进入命令处理 
			{
				if(Rxd_Decoding.u8KeyVer)					//如果数据加密了，则须先解密
				{
					CmdData_Decoding(Rxd_Decoding.u8KeyVer, (uchar8*)&gstCmdPacket.DataBuf, gstCmdPacket.u16length);
				}
				gstCmdPacket.bGetDataComplete = TRUE;
			}
			Rxd_Decoding.u8Step = 0;
		}
		break;	

		default:
		{
			memset((uchar8*)&Rxd_Decoding, 0x0, sizeof(Rxd_Decoding));
		}
		break;
	}
}

/***********************************************************************************************
* Function Name  : Comm_PacketValidityCheck
* Description	 : 数据包有效性检查
* Input 		 : pPacket -- 数据包指针, u32Len -- 数据长度
* Output		 : None
* Return		 : None 
* Others		 : CMD_VALIDITY_PACKET -- 有效, 无效则返回错误代码
***********************************************************************************************/
int Comm_PacketValidityCheck(uchar8* pPacket, uint32 u32Len)
{
	uchar8 crc = 0;
	uint16 DataLen = 0;

	if(u32Len < CMD_PACKET_MIN_LEN)
	{
		return CMD_NULLITY_PACKET;
	}
	
	if(CMD_HEADER != pPacket[0])
	{
		return 1;
	}
	else if((CMD_TYPE_ASK != pPacket[1])&&(CMD_TYPE_REPLY != pPacket[1]))
	{
		return 1;
	}
	else if(CMD_VERSION != pPacket[2])
	{
		return 2;
	}
	else if(CMD_KEY_VERSION != pPacket[8])
	{
		return 4;
	}

	//取出数据长度
	DataLen = pPacket[10];
	DataLen += ((pPacket[11] << 8)&0xFF00);
	if(CMD_END != pPacket[13 + DataLen])
	{
		return 7;
	}
	
	//校验CRC和包尾
#ifdef CMD_CALC_CRC								   		 
	crc = Calc_CRC(&pPacket[2], (u32Len -4));
	if(crc != pPacket[u32Len -2])
	{
		return 5;
	}
#endif
	else if(CMD_END != pPacket[u32Len -1])
	{
		return 6;
	}

	return CMD_VALIDITY_PACKET;
}

/***********************************************************************************************
* Function Name  : Comm_PacketDeal
* Description	 : 数据包处理
* Input 		 : pPacket -- 数据包指针, u32Len -- 数据长度, CommType -- 通信方式
* Output		 : None
* Return		 : None 
* Others		 : CMD_VALIDITY_PACKET -- 有效, 无效则返回错误代码
***********************************************************************************************/
int Comm_PacketDeal(uchar8* pPacket, uint32 u32Len, uchar8 CommType)
{
	uint16 DataLen;

	if(CMD_VALIDITY_PACKET != Comm_PacketValidityCheck(pPacket, u32Len))
	{
		return CMD_NULLITY_PACKET;
	}
	
	//数据包有效
	gstCmdPacket.u8AskOrReplay = pPacket[1];
	memcpy((uchar8*)&gstCmdPacket.u32MachineNum, &pPacket[4], 4);
	gstCmdPacket.u8Command = pPacket[9];
	DataLen = pPacket[10];
	DataLen += ((pPacket[11] << 8)&0xFF00);
	gstCmdPacket.u16length = DataLen;
	memcpy((uchar8*)&gstCmdPacket.DataBuf[0], &pPacket[12], DataLen);
	
	gstSysRunInfo.CommType = CommType; 
	gstCmdPacket.bGetDataComplete = 1;

	return CMD_VALIDITY_PACKET;
}

/***********************************************************************************************
* Function Name  : CommDeal_Reply
* Description	 : 通讯指令执行后的回复协议接口
* Input 		 : u8Cmd:命令字 pBuf:数据指针, u8Status:执行状态 len:数据长度
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
void CommDeal_Reply(uchar8 u8Cmd, uchar8 u8Status, uchar8* pBuf, uint32 u32Len)
{
	uint32 u32SendLen = 0;

	if((CMD_GET_NUM != u8Cmd)&&(FALSE == gstSysRunInfo.bTxdEnable))
	{
		gstSysRunInfo.CommType = COMM_TYPE_FREE;
		return;
	}
	
	if(u32Len > (sizeof(gstCmdPacket.DataBuf) - 15))
	{
		gstSysRunInfo.CommType = COMM_TYPE_FREE;
		return;
	}

	gstCmdPacket.DataBuf[u32SendLen++] = CMD_HEADER; 
	gstCmdPacket.DataBuf[u32SendLen++] = CMD_TYPE_REPLY;
	gstCmdPacket.DataBuf[u32SendLen++] = CMD_VERSION;
	
	gstCmdPacket.DataBuf[u32SendLen++] = SysConfig.stHardware.u8MachineType;		
	memcpy(&gstCmdPacket.DataBuf[u32SendLen], (uchar8*)&SysConfig.stHardware.u32MachineNum, 4); 	
	u32SendLen += 4;

	gstCmdPacket.DataBuf[u32SendLen++] = Rxd_Decoding.u8KeyVer;

	gstCmdPacket.DataBuf[u32SendLen++] = u8Cmd;
	gstCmdPacket.DataBuf[u32SendLen++] = (u32Len +1) & 0xFF;				//回复的状态字包含在数据中的第一个字节里面
	gstCmdPacket.DataBuf[u32SendLen++] = ((u32Len +1) & 0xFF00) >> 8;
	gstCmdPacket.DataBuf[u32SendLen++] = u8Status;
	memcpy(&gstCmdPacket.DataBuf[u32SendLen], pBuf, u32Len); 	
	
	if(Rxd_Decoding.u8KeyVer)					//判断数据是否加密传输
	{
		CmdData_Encrypt(Rxd_Decoding.u8KeyVer, &gstCmdPacket.DataBuf[u32SendLen -1], u32Len);  //加密时把处理状态字也加入数据包中
	}
	u32SendLen += u32Len;
	
	gstCmdPacket.DataBuf[u32SendLen++] = Calc_CRC(&gstCmdPacket.DataBuf[2], (u32SendLen - 2));
	gstCmdPacket.DataBuf[u32SendLen++] = CMD_END; 


	if(COMM_TYPE_TCP_CLIENT == gstSysRunInfo.CommType)
	{	
		//
	}
	else if(COMM_TYPE_TCP_SERVER == gstSysRunInfo.CommType) 
	{
		//
	}
	else if(COMM_TYPE_UDP_CLIENT == gstSysRunInfo.CommType) 
	{
		UdpClient_SendData(gstCmdPacket.DataBuf, u32SendLen);
	}
	else if(COMM_TYPE_UDP_SERVER == gstSysRunInfo.CommType) 
	{
		UdpServer_SendData(gstCmdPacket.DataBuf, u32SendLen);
	}
	else if(COMM_TYPE_RS485 == gstSysRunInfo.CommType)
	{
		RS485_Txd_Enable;
		UART2_SendNxByte(gstCmdPacket.DataBuf, u32SendLen);
		delay_ms(1);
		RS485_Txd_Disable;
	}
	else if(COMM_TYPE_UART_03 == gstSysRunInfo.CommType)
	{
		UART3_SendNxByte(gstCmdPacket.DataBuf, u32SendLen);	
	}
	else
	{
		UART1_SendNxByte(gstCmdPacket.DataBuf, u32SendLen);
	}
	gstSysRunInfo.LastCommType = gstSysRunInfo.CommType;   //记录最后一次通讯成功的方式
	gstSysRunInfo.CommType = COMM_TYPE_FREE;			   //发送完成后释放端口
}


/***********************************************************************************************
* Function Name  : Comm_ApplyFor
* Description	 : 主动申请的通讯
* Input 		 : u8Cmd:命令字 pBuf:数据指针, u32Len:数据长度 CommType:通讯方式
* Output		 : None
* Return		 : None 
* Others		 : None
***********************************************************************************************/
void Comm_ApplyFor(uchar8 u8Cmd, uchar8* pBuf, uint32 u32Len, uchar8 CommType)
{
	uint32 u32SendLen = 0;
	
	if(u32Len > (sizeof(gstCmdPacket.DataBuf) - 15))
	{
		gstSysRunInfo.CommType = COMM_TYPE_FREE;
		return;
	}

	gstCmdPacket.DataBuf[u32SendLen++] = CMD_HEADER; 
	gstCmdPacket.DataBuf[u32SendLen++] = CMD_TYPE_ASK;
	gstCmdPacket.DataBuf[u32SendLen++] = CMD_VERSION;
	
	gstCmdPacket.DataBuf[u32SendLen++] = SysConfig.stHardware.u8MachineType;		
	memcpy(&gstCmdPacket.DataBuf[u32SendLen], (uchar8*)&SysConfig.stHardware.u32MachineNum, 4); 	
	u32SendLen += 4;

	gstCmdPacket.DataBuf[u32SendLen++] = Rxd_Decoding.u8KeyVer;

	gstCmdPacket.DataBuf[u32SendLen++] = u8Cmd;
	gstCmdPacket.DataBuf[u32SendLen++] = u32Len & 0xFF;				
	gstCmdPacket.DataBuf[u32SendLen++] = (u32Len & 0xFF00) >> 8;
	memcpy(&gstCmdPacket.DataBuf[u32SendLen], pBuf, u32Len); 	
	
	if(Rxd_Decoding.u8KeyVer)					//判断数据是否加密传输
	{
		CmdData_Encrypt(Rxd_Decoding.u8KeyVer, &gstCmdPacket.DataBuf[u32SendLen -1], u32Len);  //加密时把处理状态字也加入数据包中
	}
	u32SendLen += u32Len;
	
	gstCmdPacket.DataBuf[u32SendLen++] = Calc_CRC(&gstCmdPacket.DataBuf[2], (u32SendLen - 2));
	gstCmdPacket.DataBuf[u32SendLen++] = CMD_END; 


	if(COMM_TYPE_TCP_CLIENT == CommType)
	{		
		//
	}
	else if(COMM_TYPE_TCP_SERVER == CommType) 
	{
		//
	}	
	else if(COMM_TYPE_UDP_CLIENT == CommType) 
	{
		UdpClient_SendData(gstCmdPacket.DataBuf, u32SendLen);
	}
	else if(COMM_TYPE_UDP_SERVER == CommType) 
	{
		UdpServer_SendData(gstCmdPacket.DataBuf, u32SendLen);
	}
	else if(COMM_TYPE_UART_03 == CommType)
	{
		UART3_SendNxByte(gstCmdPacket.DataBuf, u32SendLen);	
	}
	else
	{
		//不通过串口上传心跳包
	}
}

/***********************************************************************************************
* Function Name  : SendCmdToExtendLockers
* Description	 : 发指令到副柜上
* Input 		 : LockersNo -- 副柜机号, u8Cmd--命令字, pBuf --命令数据, len--数据长度
* Output		 : None
* Return		 : None 
* Note  		 : None 
***********************************************************************************************/
void SendCmdToExtendLockers(uint32 LockersNo, uchar8 u8Cmd, uchar8* pBuf, uint16 len)
{
	uint32 u32SendLen = 0;
	uchar8 SendData[256] = {0};

	if(((COMM_TYPE_RS485_HOST != SysConfig.stHardware.u8RS485Type))||(len > (sizeof(SendData) - 15)))
	{
		return;
	}

	SendData[u32SendLen++] = CMD_HEADER; 
	SendData[u32SendLen++] = CMD_TYPE_ASK;
	SendData[u32SendLen++] = CMD_VERSION;
	
	SendData[u32SendLen++] = 0xB0;		   //副柜的类型
	SendData[u32SendLen++] = LockersNo;    //副柜机号(不足位补0)
	SendData[u32SendLen++] = 0;
	SendData[u32SendLen++] = 0;
	SendData[u32SendLen++] = 0;

	SendData[u32SendLen++] = CMD_KEY_VERSION;

	SendData[u32SendLen++] = u8Cmd;
	SendData[u32SendLen++] = (len & 0xFF);				 
	SendData[u32SendLen++] = (len & 0xFF00) >> 8;
	memcpy(&SendData[u32SendLen], pBuf, len); 	
	u32SendLen += len;
	
	SendData[u32SendLen++] = Calc_CRC(&SendData[2], (u32SendLen - 2));
	SendData[u32SendLen++] = CMD_END; 

	RS485_Txd_Enable;
	delay_ms(1);
	UART0_SendNxByte(SendData, u32SendLen);
	delay_ms(3);
	RS485_Txd_Disable;
}

/***********************************************************************************************
* Function Name  : SlavePacketDecoding
* Description	 : 按协议解析从机回复的数据包
* Input 		 : pPacket -- 待解析数据区指针, Len--数据长度 
* Output		 : None
* Return		 : 0--无效数据包, 其它--有效数据包的命令字 
* Others		 : None
***********************************************************************************************/
uchar8 SlavePacketDecoding(uchar8* pPacket, uint32 Len, ST_CMD_SLAVE_PACKET* pstCmdPacket)
{	
	int i = 0;
	uchar8 crc = 0;
	uint16 DataLen = 0;
	
	for(i=0; i<Len; i++)
	{
		if((CMD_HEADER == pPacket[i])&&(CMD_TYPE_REPLY == pPacket[i +1]))
		{
			break;
		}
	}
	
	if((Len-i) < CMD_PACKET_MIN_LEN)
	{
		return 0;
	}

	if(CMD_VERSION != pPacket[i +2])
	{
		return 0;
	}
	else if(CMD_KEY_VERSION != pPacket[i +8])
	{
		return 0;
	}

	//取出数据长度
	DataLen = pPacket[i +10];
	DataLen += ((pPacket[i +11] << 8)&0xFF00);
	
	//校验CRC和包尾
#ifdef CMD_CALC_CRC								   		 
	crc = Calc_CRC(&pPacket[i +2], (10 + DataLen));
	if(crc != pPacket[i +12 + DataLen])
	{
		return 0;
	}
#endif
	if(CMD_END != pPacket[i +13 + DataLen])
	{
		return 0;
	}
	
	memcpy((uchar8*)&pstCmdPacket->iLockerNum, &pPacket[i+4], 4);
	pstCmdPacket->u8Cmd = pPacket[i +9]; 
	memcpy((uchar8*)pstCmdPacket->DataBuf, &pPacket[i+12], DataLen);
	pstCmdPacket->u16length = DataLen; 
	
	SetLedBlink(TICK_COUNT_100MS, 2);  //做通讯指示灯
	
	return pstCmdPacket->u8Cmd;
}


/*================================File end===========================================================================*/


