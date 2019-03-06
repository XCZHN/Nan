/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	ComDeal.c                                                                       
==                                                                                         
**  Function:	串口通信应用层文件  
==
**  Data:       2014-08-18
================================================================================================*/

#include "Includes.h"


/*****************************************************************************************************
* 功能(Function)  : 读取系统程序版本号
*****************************************************************************************************/
uchar8 Comm_Get_Version(void)
{
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;
	extern const char  ProgramVer_Tab[];

	SendLen = strlen(ProgramVer_Tab);
	memcpy(SendBuf, ProgramVer_Tab, SendLen);

	CommDeal_Reply(CMD_GET_VERSION, 0, SendBuf, SendLen);	
	return 0;
}

/*****************************************************************************************************
* 功能(Function)  : 远程重启
*****************************************************************************************************/
uchar8 Comm_RestSys(void)
{
	uchar8 SendBuf[10] = {0};
	uint32 SendLen = 0;

	//解决TCP模式下还没来得及回复就重启了
	if((COMM_TYPE_TCP_CLIENT == gstSysRunInfo.CommType)||(COMM_TYPE_TCP_SERVER == gstSysRunInfo.CommType))
	{		
		CommDeal_Reply(CMD_SET_REST, 0, SendBuf, SendLen);
		gstSysRunInfo.bNetResetSys = VALIDITY_USING;
	}
	else
	{
		CommDeal_Reply(CMD_SET_REST, 0, SendBuf, SendLen);
		delay_ms(1234);		
		NVIC_SystemReset();
	}
	
	return 0;
}

/*****************************************************************************************************
* 功能(Function)  : 设置机号
*****************************************************************************************************/
uchar8 Comm_Set_MachineNum(const uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;//HW_Err_Eeprom;
	TURN_DATA mid;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;
	uint32 iAddr = 0;

	if(4 != Len)
	{
		return Err_DataLen;
	}
	
	mid.numb32 = 0;
	mid.numb8[0] = pRecvBuf[0];
	mid.numb8[1] = pRecvBuf[1];
	mid.numb8[2] = pRecvBuf[2];
	mid.numb8[3] = pRecvBuf[3];

	if(mid.numb32 > 0xFFFFFFEE)
	{
		CommDeal_Reply(CMD_SET_NUM, CMD_SET_NUM, SendBuf, SendLen);	
	}
	else
	{
		SysConfig.stHardware.u32MachineNum = mid.numb32;
		iAddr = Addr_SysConfig + ((uint32)&SysConfig.stHardware.u32MachineNum - (uint32)&SysConfig);
		status = Store_Parameters(iAddr, (uchar8*)&SysConfig.stHardware.u32MachineNum, sizeof(SysConfig.stHardware.u32MachineNum));
		CommDeal_Reply(CMD_SET_NUM, status, SendBuf, SendLen);
	}
	
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 读取机号(方便网络通信时PC获取未知机号) 
*****************************************************************************************************/
uchar8 Comm_Get_MachineNum(void)
{
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = sizeof(SysConfig.stHardware.u32MachineNum);

	memcpy(SendBuf, (uchar8*)&SysConfig.stHardware.u32MachineNum, SendLen);
	CommDeal_Reply(CMD_GET_NUM, 0, SendBuf, SendLen);
	return 0;
}

/*****************************************************************************************************
* 功能(Function)  : 读取系统时间
*****************************************************************************************************/
uchar8 Comm_Get_Time(void)
{
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 7;
	
	memcpy(&SendBuf[0], (uchar8*)&gstSysRunInfo.TimeBuf[0], SendLen);
	CommDeal_Reply(CMD_GET_TIME, 0, SendBuf, SendLen);	
	return 0;
}

/*****************************************************************************************************
* 功能(Function)  : 设置系统时间
*****************************************************************************************************/
uchar8 Comm_Set_Time(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 time_buf[8] = {0};  					 //年-月-日 时:分:秒 周
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;
	uint32 time1, time2, time_gap;
	ST_TIME RtcTime;

	if(7 != Len)
	{
		return Err_DataLen;
	}
		
	memcpy(time_buf, &pRecvBuf[0], 7);
	RtcTime.u8Year    = pRecvBuf[0];
	RtcTime.u8Month   = pRecvBuf[1];
	RtcTime.u8Day     = pRecvBuf[2];
	RtcTime.u8Hour    = pRecvBuf[3];
	RtcTime.u8Minute  = pRecvBuf[4];
	RtcTime.u8Second  = pRecvBuf[5];
	RtcTime.u8WeekDay = pRecvBuf[6];

	if((0 == Chack_Date(time_buf[0], time_buf[1], time_buf[2])) && (0 == Chack_Time(time_buf[3], time_buf[4], time_buf[5])))
	{
		if((gstSysRunInfo.TimeBuf[0] != time_buf[0])||(gstSysRunInfo.TimeBuf[1] != time_buf[1])||(gstSysRunInfo.TimeBuf[2] != time_buf[2]))
		{//日期不同则更新DS13002时间		
			RTC_SetDateTime(&RtcTime);							
		}
		else
		{//日期相同，则时间差大于3S才更新DS1302的时间,避免频繁擦写RTC
			time1 = (time_buf[3] * 3600) + (time_buf[4] * 60) + time_buf[5];
			time2 = (gstSysRunInfo.TimeBuf[3] * 3600) + (gstSysRunInfo.TimeBuf[4] * 60) + gstSysRunInfo.TimeBuf[5];
			time_gap = time2 - time1;
			if(time_gap >= 3)
			{
				RTC_SetDateTime(&RtcTime);						
			}			
		}
		status = 0;
	}
	else
	{
		status = Err_Time;	
	}

	CommDeal_Reply(CMD_SET_TIME, status, SendBuf, SendLen);   
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 初始化系统
*****************************************************************************************************/
uchar8 Comm_SysInit(void)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};

	CommDeal_Reply(CMD_SYS_INIT, status, SendBuf, 0);  

	SysConfig_Init();

	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 设置有线网络开关 
*****************************************************************************************************/
uchar8 Comm_Set_NetworkSwitch(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[10] = {0};
	uint32 SendLen = 0;
	uint32 iAddr = 0;

	if(1 != Len)  
	{
		return Err_DataLen;
	}

	SysConfig.stHardware.bNetwork = pRecvBuf[0];
	iAddr = Addr_SysConfig + ((uint32)&SysConfig.stHardware.bNetwork - (uint32)&SysConfig);	
	Store_Parameters(iAddr, (uchar8*)&SysConfig.stHardware.bNetwork, 1);

	CommDeal_Reply(CMD_SET_NET_SWITCH, status, SendBuf, SendLen); 
	
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 控制蜂鸣器
*****************************************************************************************************/
uchar8 Comm_RingBuzzer(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 u32BeepTime = 0;

	if(4 != Len)
	{
		return Err_DataLen;
	}

	memcpy((uchar8*)&u32BeepTime, pRecvBuf, 4);
	Buzzer(u32BeepTime);
	CommDeal_Reply(CMD_RING_BUZZER, status, SendBuf, 0);  
	
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 从Eeprom指定地址读数据  
*****************************************************************************************************/
uchar8 Comm_EepromRead(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[512] = {0};
	uint32 SendLen = 0;
	TURN_DATA TD;	
	uint32 ReadAddr = 0;
	uint32 ReadLen = 0;
	memcpy(&TD.numb8[0], &pRecvBuf[0], 4);
	ReadAddr = TD.numb32;
	memcpy(&TD.numb8[0], &pRecvBuf[4], 4);
	if(TD.numb32 > (sizeof(SendBuf) - 8))
	{
		TD.numb32 = sizeof(SendBuf) - 8;
	}
	ReadLen = TD.numb32;

	memcpy(&SendBuf[0], &pRecvBuf[0], 4);   //将读FLASH的地址和长度回复给上位机
	memcpy(&SendBuf[4], &TD.numb8[0], 4);
	SendLen = 8;
		
	status = I2C_EE_BufferRead(&SendBuf[8], ReadAddr, ReadLen);
	SendLen += ReadLen;
	CommDeal_Reply(CMD_EEPROM_READ, status, SendBuf, SendLen); 
	
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 往Eeprom指定地址写数据 
*****************************************************************************************************/
uchar8 Comm_EepromWrite(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[512] = {0};
	uint32 SendLen = 0;
	TURN_DATA TD;
	uint32 WriteAddr = 0;
	uint32 WriteLen = 0;
	memcpy(&TD.numb8[0], &pRecvBuf[0], 4);
	WriteAddr = TD.numb32;
	memcpy(&TD.numb8[0], &pRecvBuf[4], 4);
	WriteLen = TD.numb32;
	if((WriteAddr + WriteLen) < EEPROM_MAX_AD)
	{
		memcpy(SendBuf, &pRecvBuf[8], WriteLen);
		status = I2C_EE_BufferWrite(SendBuf, WriteAddr, WriteLen);
	}
	else
	{
		status = Err_EepromWrite;
	}
	memcpy(SendBuf, &pRecvBuf[0], 8);   //将写FLASH的地址和长度回复给上位机
	SendLen = 8;
	CommDeal_Reply(CMD_EEPROM_WRITE, status, SendBuf, SendLen); 
		
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 设置本机IP地址  
*****************************************************************************************************/
uchar8 Comm_Set_IP(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;

	if(4 == Len)
	{
		memset((uchar8*)&SysConfig.stNet.IP[0], 0x0, sizeof(SysConfig.stNet.IP));
		memcpy((uchar8*)&SysConfig.stNet.IP[0], pRecvBuf, 4);
		memcpy((uchar8*)&SysConfig.stNet.Gateway[0], pRecvBuf, 3);  //网关自动修改过来
		SysConfig.stNet.Gateway[3] = 1;
		Store_Parameters(Addr_NetInfo, (uchar8*)&SysConfig.stNet , sizeof(ST_NETWORK));
	}
	else if(12 == Len)
	{
		memset((uchar8*)&SysConfig.stNet.IP[0], 0x0, sizeof(SysConfig.stNet.IP));
		memcpy((uchar8*)&SysConfig.stNet.IP[0], &pRecvBuf[0], 4);

		memset((uchar8*)&SysConfig.stNet.Gateway[0], 0x0, sizeof(SysConfig.stNet.Gateway));
		memcpy((uchar8*)&SysConfig.stNet.Gateway[0], &pRecvBuf[4], 4);

		memset((uchar8*)&SysConfig.stNet.NetMask[0], 0x0, sizeof(SysConfig.stNet.NetMask));
		memcpy((uchar8*)&SysConfig.stNet.NetMask[0], &pRecvBuf[8], 4);		

		Store_Parameters(Addr_NetInfo, (uchar8*)&SysConfig.stNet , sizeof(ST_NETWORK));
	}
	else
	{
		return Err_DataLen;
	}

	CommDeal_Reply(CMD_SET_IP, status, SendBuf, SendLen); 
	
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 读取IP地址  
*****************************************************************************************************/
uchar8 Comm_Get_IP(void)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 22;
	 	
	memcpy(SendBuf, (uchar8*)&SysConfig.stNet.IP[0], 4);
	memcpy(&SendBuf[4], (uchar8*)&SysConfig.stNet.Gateway[0], 4); 
	memcpy(&SendBuf[8], (uchar8*)&SysConfig.stNet.NetMask[0], 4); 
	memcpy(&SendBuf[12], (uchar8*)&SysConfig.stNet.u16UDP_ListenPort, 2); 
	memcpy(&SendBuf[14], (uchar8*)&SysConfig.stNet.u16TCP_ListenPort, 2); 
	memcpy(&SendBuf[16], (uchar8*)&SysConfig.stNet.MAC[0], 6); 

	CommDeal_Reply(CMD_GET_IP, status, SendBuf, SendLen); 
	
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 设置服务器IP地址及端口号  
*****************************************************************************************************/
uchar8 Comm_Set_ServerIP(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;

	if(8 != Len)  
	{
		return Err_DataLen;
	}
	
	memcpy((uchar8*)&SysConfig.stNet.ServerIP[0], pRecvBuf, 4);
	memcpy((uchar8*)&SysConfig.stNet.u16UDP_ServerPort, &pRecvBuf[4], 2);
	Store_Parameters(Addr_NetInfo, (uchar8*)&SysConfig.stNet, sizeof(ST_NETWORK));

	CommDeal_Reply(CMD_SET_SERVER_IPINFO, status, SendBuf, SendLen); 
	
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 读取服务器IP地址及端口号    
*****************************************************************************************************/
uchar8 Comm_Get_ServerIP(void)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 8;
	 	
	memcpy(SendBuf, (uchar8*)&SysConfig.stNet.ServerIP[0], 4);
	memcpy(&SendBuf[4], (uchar8*)&SysConfig.stNet.u16UDP_ServerPort, 2);

	CommDeal_Reply(CMD_GET_SERVER_IPINFO, status, SendBuf, SendLen); 
	
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 寻卡
*****************************************************************************************************/
uchar8 Comm_FindCard(void)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 4;

	status = RF_Read_CardUID(&SendBuf[0]);
	CommDeal_Reply(CMD_FIND_CARD, status, SendBuf, SendLen);	
	if(OK == status)
	{
		Buzzer(50);
	}
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 设置IC卡密钥信息
*****************************************************************************************************/
uchar8 Comm_Set_CardKey(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[50] = {0};
	uint32 SendLen = 0;
	uint32 u32StorAddr = 0;
	ST_CARD_KEY stCardKey;

	if(sizeof(ST_CARD_KEY) != Len)  
	{
		return Err_DataLen;
	}
		
	memset((uchar8*)&stCardKey, 0x0, sizeof(ST_CARD_KEY));
	memcpy((uchar8*)&stCardKey, pRecvBuf, sizeof(ST_CARD_KEY));
	if((stCardKey.u8Sector > 0)&&(stCardKey.u8Sector < 16))
	{
		memcpy((uchar8*)&SysConfig.stCardKey, pRecvBuf, sizeof(ST_CARD_KEY));
		u32StorAddr = Addr_SysConfig + ((uint32)&SysConfig.stCardKey - (uint32)&SysConfig);
		Store_Parameters(u32StorAddr, (uchar8*)&SysConfig.stCardKey, sizeof(ST_CARD_KEY));
	}
	else
	{
		status = Err_Data;	
	}
	CommDeal_Reply(CMD_SET_CARD_KEY, status, SendBuf, SendLen);	 

	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 读取IC卡密钥信息
*****************************************************************************************************/
uchar8 Comm_Get_CardKey(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;

	SendLen = sizeof(ST_CARD_KEY);	
	memcpy(SendBuf, (uchar8*)&SysConfig.stCardKey, SendLen);
	CommDeal_Reply(CMD_GET_CARD_KEY, status, SendBuf, SendLen);	 

	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 播放指定语音
*****************************************************************************************************/
uchar8 Comm_PlayVoice(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;
	uint16 VoiceAddr = 0;

	if(1 != Len)  
	{
		return Err_DataLen;
	}

	VoiceAddr = pRecvBuf[0];
	Voice_Play(VoiceAddr);

	CommDeal_Reply(CMD_PLAY_VOICE, status, SendBuf, SendLen); 
		
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 播放指定语音
*****************************************************************************************************/
uchar8 Comm_SetVoiceLevel(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;
	uint32 iAddr = 0;

	CommDeal_Reply(CMD_SET_VOICE_LEVEL, status, SendBuf, SendLen); 

	SysConfig.stHardware.u8VoiceLevel = pRecvBuf[0];
	iAddr = Addr_SysConfig + ((uint32)&SysConfig.stHardware - (uint32)&SysConfig);
	status = Store_Parameters(iAddr, (uchar8*)&SysConfig.stHardware, sizeof(SysConfig.stHardware));
	Set_VoiceLevel(SysConfig.stHardware.u8VoiceLevel);
	
	
	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 语音模块指令测试
*****************************************************************************************************/
uchar8 Comm_VoiceCmdTest(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;
	uchar8 cmd = pRecvBuf[0];

	CommDeal_Reply(CMD_VOICE_CMD_TEST, status, SendBuf, SendLen); 
	Voice_SendCmd(cmd);
	
	return status;
}

/*******************************************************************************
* Function Name  : Comm_Set_Lockers
* Description    : 设置寄存柜信息
* Input          : ...
* Output         : None
* Return         : None
*******************************************************************************/
uchar8 Comm_Set_Lockers(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;
	uint32 iAddr = 0;
	uchar8 lenddr = 0;

	if(30 != Len)  
	{
		return Err_DataLen;
	}

	if(pRecvBuf[0] >= WORK_MODE_)
	{
		status = Err_Data;
	}
	else
	{
		memcpy((uchar8*)&SysConfig.stClientInfo.IdCode[0], &pRecvBuf[lenddr], SYS_ID_CODE_LEN);
		iAddr = Addr_SysConfig + ((uint32)&SysConfig.stClientInfo.IdCode[0] - (uint32)&SysConfig);
		Store_Parameters(iAddr, (uchar8*)SysConfig.stClientInfo.IdCode, sizeof(SysConfig.stClientInfo.IdCode));
		lenddr += SYS_ID_CODE_LEN;
		
		SysConfig.stLockers.u8WorkMode = pRecvBuf[lenddr++];
		SysConfig.stLockers.u8AllowBeOpenedBoxNum = pRecvBuf[lenddr++];
		memcpy((uchar8*)&SysConfig.stLockers.Name[0], &pRecvBuf[lenddr], MAX_NameLen); 
		lenddr += MAX_NameLen;

		memcpy((uchar8*)&SysConfig.stLockers.iMinFetchTime, &pRecvBuf[lenddr], 4); 
		lenddr += 4;
		memcpy((uchar8*)&SysConfig.stLockers.iMaxFetchTime, &pRecvBuf[lenddr], 4);  
		lenddr += 4;
		memcpy((uchar8*)&SysConfig.stLockers.iHandleTimeout, &pRecvBuf[lenddr], 4);  
		lenddr += 4;
		memcpy((uchar8*)&SysConfig.stLockers.iNoticeTime, &pRecvBuf[lenddr], 4);  
		lenddr += 4;

		iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers - (uint32)&SysConfig);
		Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers, sizeof(ST_LOCKERS));
	}
	
	CommDeal_Reply(CMD_SET_LOCKERS, status, SendBuf, SendLen); 

	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 设置箱数
*****************************************************************************************************/
uchar8 Comm_SetBoxNum(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;
	uint32 u32StorAddr = 0;

	if(6 != Len)  
	{
		return Err_DataLen;
	}

	if((pRecvBuf[0] <= MAX_BOX_NUM)&&(pRecvBuf[0] == (pRecvBuf[1] + pRecvBuf[2] + pRecvBuf[3] + pRecvBuf[4] + pRecvBuf[5])))
	{
		SysConfig.stLockers.u8BoxCount = pRecvBuf[0];
		SysConfig.stLockers.u8HostLockerBoxs = pRecvBuf[1];
		SysConfig.stLockers.u8ExtL01BoxNum = pRecvBuf[2];
		SysConfig.stLockers.u8ExtL02BoxNum = pRecvBuf[3];
		SysConfig.stLockers.u8ExtL03BoxNum = pRecvBuf[4];
		status = 0;

		u32StorAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.u8BoxCount - (uint32)&SysConfig);
		Store_Parameters(u32StorAddr, (uchar8*)&SysConfig.stLockers.u8BoxCount, 4);
		
		u32StorAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.u8HostLockerBoxs - (uint32)&SysConfig);
		Store_Parameters(u32StorAddr, (uchar8*)&SysConfig.stLockers.u8HostLockerBoxs, 5);
	}
	else
	{
		status = 0x05;
	}

	CommDeal_Reply(CMD_SET_BOX_NUM, status, SendBuf, SendLen); 
	return status;	
}

/*****************************************************************************************************
* 功能(Function)  : 开箱门锁
*****************************************************************************************************/
uchar8 Comm_OpenOneBox(uchar8 cmd, uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;
	uchar8 BoxNum = 0;	
	ST_RECORD stRecord;
	memset((uchar8*)&stRecord, 0x0, sizeof(ST_RECORD));

	if(1 != Len)  
	{
		return Err_DataLen;
	}

	BoxNum = pRecvBuf[0]; 
	if(BoxNum > SysConfig.stLockers.u8BoxCount)
	{
		CommDeal_Reply(cmd, 0xFF, SendBuf, SendLen); 
	}
	else
	{		
		status = SelectOneBoxToOpen(BoxNum, SysConfig.stLockers.bCheckBoxDoorStatus);
		CommDeal_Reply(cmd, status, SendBuf, SendLen); 
			
		//保存一笔开箱记录
		stRecord.u8BoxID = BoxNum;
		stRecord.u8Type = OpenBoxType_NetCmd;
		stRecord.u8Result = status;
		StoreOpenBoxRecord(&stRecord);
	}

	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 检查箱门状态
*****************************************************************************************************/
uchar8 Comm_Get_BoxDoorStatus(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[300] = {0};
	uint32 SendLen = 0;
	uchar8 StartBoxNo = 1;       //起始箱号
	uchar8 GetBoxCount = 0;      //箱数

	StartBoxNo = pRecvBuf[0];
	GetBoxCount = pRecvBuf[1];

	if((0 == StartBoxNo)||(StartBoxNo + GetBoxCount) > (MAX_BOX_NUM +1))
	{
		StartBoxNo = 1;
		GetBoxCount = MAX_BOX_NUM;
	}
	SendBuf[0] = StartBoxNo;
	SendBuf[1] = GetBoxCount;
	SendLen = 2;

	memcpy(&SendBuf[2], (uchar8*)&gstSysRunInfo.BoxDoorStatus[StartBoxNo], GetBoxCount);
	SendLen += GetBoxCount;
	CommDeal_Reply(CMD_GET_BOXDOOR_STATE, status, SendBuf, SendLen);	 

	return 0;
}

/*****************************************************************************************************
* 功能(Function)  : 检查包箱状态
*****************************************************************************************************/
uchar8 Comm_CheckBoxEmpty(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[300] = {0};
	uint32 SendLen = 0;
	uchar8 StartBoxNo = 1;		 //起始箱号
	uchar8 GetBoxCount = 0; 	 //箱数

	StartBoxNo = pRecvBuf[0];
	GetBoxCount = pRecvBuf[1];

	if((0 == StartBoxNo)||(StartBoxNo + GetBoxCount) > (MAX_BOX_NUM +1))
	{
		StartBoxNo = 1;
		GetBoxCount = MAX_BOX_NUM;
	}
	SendBuf[0] = StartBoxNo;
	SendBuf[1] = GetBoxCount;
	SendLen = 2;

	memcpy(&SendBuf[2], (uchar8*)&gstSysRunInfo.BoxInsideStatus[StartBoxNo-1], GetBoxCount);
	SendLen += GetBoxCount;
	CommDeal_Reply(CMD_GET_BOX_STATE, status, SendBuf, SendLen);	 

	return 0;
}

/*****************************************************************************************************
* 功能(Function)  : 设置指定箱号信息
*****************************************************************************************************/
uchar8 Comm_SetBoxInfo(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[50] = {0};
	uint32 SendLen = 0;
	uint32 u32StorAddr = 0;
	ST_BOX stBox;

	if(sizeof(ST_BOX) != Len)  
	{
		return Err_DataLen;
	}
		
	memset((uchar8*)&stBox, 0x0, sizeof(ST_BOX));
	memcpy((uchar8*)&stBox, pRecvBuf, sizeof(ST_BOX));
	if(stBox.u8BoxID == SysConfig.stBox[stBox.u8BoxID].u8BoxID)
	{
		memcpy((uchar8*)&SysConfig.stBox[stBox.u8BoxID], (uchar8*)&stBox, sizeof(ST_BOX));
		u32StorAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[stBox.u8BoxID] - (uint32)&SysConfig);
		Store_Parameters(u32StorAddr, (uchar8*)&SysConfig.stBox[stBox.u8BoxID], sizeof(ST_BOX));
	}
	else
	{
		status = Err_BoxInfo;	
	}
	CommDeal_Reply(CMD_SET_BOX_INFO, status, SendBuf, SendLen);	 

	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 读取指定箱号信息
*****************************************************************************************************/
uchar8 Comm_GetBoxInfo(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 BoxNum = 1;
	uchar8 SendBuf[300] = {0};
	uint32 SendLen = 0;

	if(pRecvBuf[0] <= MAX_BOX_NUM)
	{
		BoxNum = pRecvBuf[0];
	}
	
	memcpy(SendBuf, (uchar8*)&SysConfig.stBox[BoxNum], sizeof(ST_BOX));
	SendLen = sizeof(ST_BOX);
	CommDeal_Reply(CMD_GET_BOX_INFO, status, SendBuf, SendLen);	 

	return status;
}

/*****************************************************************************************************
* Description	 : 下发预授权卡号表 
* Notes          : 第一个卡号的相对位置(4Byte)+ 卡号数量N(2Byte) + 卡号(4B*NB)
*****************************************************************************************************/
uchar8 Comm_Set_ValidCardList(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[30] = {0};
	uint32 SendLen = 0;
	uint32 iAddr = 0;
	uint32 iStartCardAddr = 0;
	uint16 iCardNum = 0;
		
	memcpy((uchar8*)&iStartCardAddr, &pRecvBuf[0], 4);
	memcpy((uchar8*)&iCardNum, &pRecvBuf[4], 2);

	if(((iCardNum *4) +6) != Len)  
	{
		return Err_DataLen;
	}

	if((iStartCardAddr + iCardNum) > MAX_VALID_CAED_COUNT)
	{
		return 0xFF;
	}

	iAddr = Addr_PreAuthorization + (iStartCardAddr * 4);
	I2C_EE_BufferWrite(&pRecvBuf[6], iAddr, (iCardNum * 4));
	
	CommDeal_Reply(CMD_SET_VALID_CARD_LIST, status, SendBuf, SendLen);	 

	return 0;
}

/*****************************************************************************************************
* Description	 : 下发预授权卡号表 
* Notes          : 回复:第一个卡号的相对位置(4Byte)+ 卡号数量N(2Byte) + 卡号(4B*NB)
*****************************************************************************************************/
uchar8 Comm_Get_ValidCardList(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[512] = {0};
	uint32 SendLen = 0;
	uint32 iAddr = 0;
	uint32 iStartCardAddr = 0;
	uint16 iCardNum = 0;
		
	memcpy((uchar8*)&iStartCardAddr, &pRecvBuf[0], 4);
	memcpy((uchar8*)&iCardNum, &pRecvBuf[4], 2);

	if(6 != Len)  
	{
		return Err_DataLen;
	}

	if(((iStartCardAddr + iCardNum) > MAX_VALID_CAED_COUNT)||(iCardNum > 100))
	{
		return 0xFF;
	}

	memcpy(&SendBuf[0], &pRecvBuf[0], 6);
	iAddr = Addr_PreAuthorization + (iStartCardAddr * 4);
	I2C_EE_BufferRead(&SendBuf[6], iAddr, (iCardNum * 4));
	SendLen = 6 + (iCardNum * 4);
	
	CommDeal_Reply(CMD_GET_VALID_CARD_LIST, status, SendBuf, SendLen);	 

	return 0;
}

/*****************************************************************************************************
* 功能(Function)  : 授权一个箱的开箱卡号
*****************************************************************************************************/
uchar8 Comm_Set_BoxUserCard(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[50] = {0};
	uint32 SendLen = 0;
	uchar8 u8CardCnt = 0;
	uchar8 u8Box = 0;  
	uint32 iCardID = 0;
	uchar8 i = 0;
	uint32 iAddr = 0;
		
	u8Box = pRecvBuf[0]; 
	u8CardCnt = pRecvBuf[1];
	
	if((2 + (u8CardCnt *4)) != Len)  
	{
		return Err_DataLen;
	}
	else if((u8Box)&&(u8CardCnt > MAX_BoxCardNum))
	{
		return Err_DataLen;
	}
	else
	{
		if(0 == u8Box)  //管理员卡
		{
			for(i=0; i<MAX_AdminCardNum; i++)
			{
				if(i < u8CardCnt)
				{
					memcpy((uchar8*)&iCardID, &pRecvBuf[i*4 +2], 4);
					SysConfig.stLockers.AdminCard[i] = iCardID;
				}
				else
				{
					SysConfig.stLockers.AdminCard[i] = 0;	
				}
			}
			//保存数据
			iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.AdminCard[0] - (uint32)&SysConfig);
			Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.AdminCard[0], sizeof(SysConfig.stLockers.AdminCard));
		}
		else  //开箱卡
		{
			memcpy((uchar8*)&iCardID, &pRecvBuf[2], 4);
			SysConfig.stBox[u8Box].iUserCardID = iCardID;	

			//保存数据
			iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[u8Box].iUserCardID - (uint32)&SysConfig);
			Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[u8Box].iUserCardID, 4);
		}
	}

	CommDeal_Reply(CMD_SET_BoxUserCard, status, SendBuf, SendLen);	 

	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 读取一个箱的开箱卡号
*****************************************************************************************************/
uchar8 Comm_Get_BoxUserCard(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[200] = {0};
	uint32 SendLen = 0;
	uchar8 u8Box = 0;  
	uchar8 i = 0;
	uint32 iCardID = 0;
		
	u8Box = pRecvBuf[0]; 
	
	if(1 != Len)  
	{
		return Err_DataLen;
	}

	SendBuf[0] = u8Box;
	if(0 == u8Box)
	{
		SendBuf[1] = MAX_AdminCardNum;
		SendLen = 2;
		for(i=0; i<MAX_AdminCardNum; i++)
		{
			iCardID = SysConfig.stLockers.AdminCard[i];
			memcpy(&SendBuf[SendLen], (uchar8*)&iCardID, 4);
			SendLen += 4;
		}
	}
	else
	{
		SendBuf[1] = MAX_BoxCardNum;
		SendLen = 2;
		for(i=0; i<MAX_BoxCardNum; i++)
		{
			iCardID = SysConfig.stBox[u8Box].iUserCardID;
			memcpy(&SendBuf[SendLen], (uchar8*)&iCardID, 4);
			SendLen += 4;
		}
	}
	
	CommDeal_Reply(CMD_GET_BoxUserCard, status, SendBuf, SendLen);	 

	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 给所有箱绑定一个开箱卡号
*****************************************************************************************************/
uchar8 Comm_Get_AllBoxUserCard(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[512] = {0};
	uint32 SendLen = 0;
	uchar8 u8StartBox = 0;  
	uchar8 u8BoxCnt = 0;  
	
	uint32 iCardID = 0;
	uchar8 i =0;
		
	u8StartBox = pRecvBuf[0]; 
	u8BoxCnt = pRecvBuf[1];
	
	if(2 != Len)  
	{
		return Err_DataLen;
	}
	else if((u8StartBox + u8BoxCnt) > MAX_BOX_NUM)
	{
		return Err_DataLen;
	}
	else
	{
		SendBuf[0] = u8StartBox;
		SendBuf[1] = u8BoxCnt;
		SendLen = 2;
		for(i=0; i<u8BoxCnt; i++)
		{
			iCardID = SysConfig.stBox[u8StartBox].iUserCardID;
			memcpy(&SendBuf[SendLen], (uchar8*)&iCardID, 4);
			SendLen += 4;
			u8StartBox += 1;
			WDT_Feed();
		}
	}

	CommDeal_Reply(CMD_GET_AllBoxCard, status, SendBuf, SendLen);	 

	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 锁箱,可锁多个箱号
*****************************************************************************************************/
uchar8 Comm_LockNxBox(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[50] = {0};
	uint32 SendLen = 0;
	uchar8 u8BoxCnt = 0;  
	uchar8 Box = 1;  
	
	uchar8 i =0;
	uint32 iAddr = 0;
		
	u8BoxCnt = pRecvBuf[0];
	
	if((1 + u8BoxCnt) != Len)  
	{
		return Err_DataLen;
	}
	else if(u8BoxCnt > MAX_BOX_NUM)
	{
		return Err_DataLen;
	}
	else
	{
		for(i=0; i<u8BoxCnt; i++)
		{
			Box = pRecvBuf[1 + i];
			if(Box <= MAX_BOX_NUM)   //当box=0时表示锁管理员账号
			{
				SysConfig.stBox[Box].bLocking = LSTATUS_LOCKING;
				//保存数据
				iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[Box].bLocking - (uint32)&SysConfig);
				Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[Box].bLocking, 1);
			}
		}
		UpdateEmptyBoxList();       
	}

	CommDeal_Reply(CMD_LockBox, status, SendBuf, SendLen);	 

	return status;
}

/*****************************************************************************************************
* 功能(Function)  : 解锁箱,可解锁多个箱号
*****************************************************************************************************/
uchar8 Comm_UnlockNxBox(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[50] = {0};
	uint32 SendLen = 0;
	uchar8 u8BoxCnt = 0;  
	uchar8 Box = 1;  
	
	uchar8 i =0;
	uint32 iAddr = 0;
		
	u8BoxCnt = pRecvBuf[0];
	
	if((1 + u8BoxCnt) != Len)  
	{
		return Err_DataLen;
	}
	else if(u8BoxCnt > MAX_BOX_NUM)
	{
		return Err_DataLen;
	}
	else
	{
		for(i=0; i<u8BoxCnt; i++)
		{
			Box = pRecvBuf[1 + i];
			if(Box <= MAX_BOX_NUM)   //当box=0时表示解锁管理员账号
			{
				SysConfig.stBox[Box].bLocking = BOX_STATUS_IDLE;
				//保存数据
				iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[Box].bLocking - (uint32)&SysConfig);
				Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[Box].bLocking, 1);
			}
		}
		UpdateEmptyBoxList();       
	}

	CommDeal_Reply(CMD_UnlockBox, status, SendBuf, SendLen);	 

	return status;
}

/*****************************************************************************************************
* Description    : 读取开箱记录统计数据
* Notes          : 上传记录总容量\当前记录号\有效起始记录号
*****************************************************************************************************/
uchar8 Comm_Get_RecordPoint(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[100] = {0};
	uint32 SendLen = 0;
	uint32 iAddr = 0;

	if(0 != Len)  
	{
		return Err_DataLen;
	}

	iAddr = MAX_RECORD_NUM;
	memcpy(&SendBuf[SendLen], (uchar8*)&iAddr, 4);
	SendLen += 4;
	memcpy(&SendBuf[SendLen], (uchar8*)&SysConfig.stRecord.iRecordCount, 4);
	SendLen += 4;
	if(SysConfig.stRecord.iRecordCount < MAX_RECORD_NUM)
	{
		SendBuf[SendLen++] = 0;
		SendBuf[SendLen++] = 0;
		SendBuf[SendLen++] = 0;
		SendBuf[SendLen++] = 0;
	}
	else
	{
		iAddr = MAX_RECORD_NUM - (SysConfig.stRecord.iRecordCount % MAX_RECORD_NUM);
		memcpy(&SendBuf[SendLen], (uchar8*)&iAddr, 4);
		SendLen += 4;
	}
	memcpy(&SendBuf[SendLen], (uchar8*)&SysConfig.stRecord.iUploadCount, 4);
	SendLen += 4;
	
	CommDeal_Reply(CMD_GET_RecordPoint, status, SendBuf, SendLen);	 

	return status;
}

/*****************************************************************************************************
* Description    : 读取开箱记录
* Notes          : 可读取多笔开箱记录
*****************************************************************************************************/
uchar8 Comm_Get_Record(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[512] = {0};
	uint32 SendLen = 0;
	uchar8 u8BoxNum = 0;
	uchar8 i = 0;
	uint32 iAddr = 0;
	 		
	u8BoxNum = pRecvBuf[0]; 
	if((0 == SysConfig.stRecord.iRecordCount)||(SysConfig.stRecord.iRecordCount <= SysConfig.stRecord.iUploadCount))
	{
		status = Err_NoRecord;
		CommDeal_Reply(CMD_GET_RECORD, status, SendBuf, SendLen);

		//纠错
		if(SysConfig.stRecord.iRecordCount <= SysConfig.stRecord.iUploadCount)
		{
			SysConfig.stRecord.iUploadCount = SysConfig.stRecord.iRecordCount;
			iAddr = Addr_SysConfig + ((uint32)&SysConfig.stRecord.iUploadCount - (uint32)&SysConfig);
			Store_Parameters(iAddr, (uchar8*)&SysConfig.stRecord.iUploadCount, 4);
		}
	}
	else
	{
		//去除被覆盖了的记录
		if(SysConfig.stRecord.iRecordCount - SysConfig.stRecord.iUploadCount > MAX_RECORD_NUM)
		{
			SysConfig.stRecord.iUploadCount = SysConfig.stRecord.iRecordCount - MAX_RECORD_NUM;
		}

		//检查下未上传记录数量有木有上位机要的那么多
		if(u8BoxNum > (SysConfig.stRecord.iRecordCount - SysConfig.stRecord.iUploadCount))
		{
			u8BoxNum = SysConfig.stRecord.iRecordCount - SysConfig.stRecord.iUploadCount;
		}

		//防溢出
		if(u8BoxNum > 10)
		{
			u8BoxNum = 10;
		}
		
		SendLen = 0;
		SendBuf[SendLen++] = ONE_RECORD_LEN;
		SendBuf[SendLen++] = u8BoxNum;
		for(i=0; i<u8BoxNum; i++)
		{
			ReadOneRecord_ToArrayBuf(SysConfig.stRecord.iUploadCount, &SendBuf[SendLen]);	
			SendLen += ONE_RECORD_LEN;
			SysConfig.stRecord.iUploadCount += 1;
			WDT_Feed();
		}
		CommDeal_Reply(CMD_GET_RECORD, status, SendBuf, SendLen);	 

		iAddr = Addr_SysConfig + ((uint32)&SysConfig.stRecord.iUploadCount - (uint32)&SysConfig);
		Store_Parameters(iAddr, (uchar8*)&SysConfig.stRecord.iUploadCount, 4);	
	}

	return status;
}

/*****************************************************************************************************
* Description    : 读取开箱记录
* Notes          : 可读取多笔开箱记录
*****************************************************************************************************/
uchar8 Comm_Get_RecordAgain(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[512] = {0};
	uint32 SendLen = 0;
	uint32 iStartRecordNum = 0;
	uchar8 u8BoxNum = 0;
	uchar8 i = 0;
	 		
	memcpy((uchar8*)&iStartRecordNum, &pRecvBuf[0], 4);
	u8BoxNum = pRecvBuf[4]; 

	//防溢出
	if(u8BoxNum > 10)
	{
		u8BoxNum = 10;
	}
	
	SendLen = 0;
	SendBuf[SendLen++] = ONE_RECORD_LEN;
	SendBuf[SendLen++] = u8BoxNum;
	for(i=0; i<u8BoxNum; i++)
	{
		ReadOneRecord_ToArrayBuf(iStartRecordNum, &SendBuf[SendLen]);	
		SendLen += ONE_RECORD_LEN;
		iStartRecordNum += 1;
		WDT_Feed();
	}
	CommDeal_Reply(CMD_GET_RECORD_AGAIN, status, SendBuf, SendLen);	 

	return status;
}

/*****************************************************************************************************
* Description    : 清除开箱记录
* Notes          : 同时将上传记录等于记录指针
*****************************************************************************************************/
uchar8 Comm_Clr_AllRecord(uchar8 *pRecvBuf, uint16 Len)
{
	uchar8 status = 0;
	uchar8 SendBuf[20] = {0};
	uint32 SendLen = 0;

	if(0 != Len)  
	{
		return Err_DataLen;
	}
	
	//先回复再执行清数据,以免上位机等太久
	CommDeal_Reply(CMD_CLR_AllRecord, status, SendBuf, SendLen);	 

	//清除记录
	Clear_Record();

	return status;
}

/******************************************************************************************************
* Function Name  : Comm_Handle
* Description    : 通讯命令执行
* Input          : CMD--命令字,pCmdData－－命令数据, dLen -- 数据部分长度
* Variable       : gstCmdPacket -- 通讯指令数据包
* Output         : None
* Return         : None
******************************************************************************************************/
void  Comm_Handle(uchar8 CMD, uchar8* pCmdData, uint16 dLen)
{	
	uchar8 Ret = 0;

	switch(CMD)
	{
		case CMD_GET_VERSION:
			Ret = Comm_Get_Version();
			break;
			
		case CMD_SET_REST:
			Ret = Comm_RestSys();
			break;
			
		case CMD_SET_NUM:
			Ret = Comm_Set_MachineNum(pCmdData, dLen);
			break;

		case CMD_GET_TIME:
			Ret = Comm_Get_Time();
			break;

		case CMD_SET_TIME:
			Ret = Comm_Set_Time(pCmdData, dLen);
			break;

		case CMD_SYS_INIT:
			Ret = Comm_SysInit();
			break;

		case CMD_SET_NET_SWITCH:
			Ret = Comm_Set_NetworkSwitch(pCmdData, dLen);
			break;

		case CMD_SET_IP:
			Ret = Comm_Set_IP(pCmdData, dLen);
			break;

		case CMD_GET_IP:
			Ret = Comm_Get_IP();
			break;

		case CMD_SET_SERVER_IPINFO:
			Ret = Comm_Set_ServerIP(pCmdData, dLen);
			break;

		case CMD_GET_SERVER_IPINFO:
			Ret = Comm_Get_ServerIP();
			break;
									
		case CMD_RING_BUZZER:
			Ret = Comm_RingBuzzer(pCmdData, dLen);
			break;
			
		case CMD_EEPROM_WRITE:
			Comm_EepromWrite(pCmdData, dLen);
			break;
		
		case CMD_EEPROM_READ:
			Comm_EepromRead(pCmdData, dLen);
			break;
			
		case CMD_FIND_CARD:
			Ret = Comm_FindCard();
			break;
				
		case CMD_SET_CARD_KEY:
			Comm_Set_CardKey(pCmdData, dLen);
			break;

		case CMD_GET_CARD_KEY:
			Comm_Get_CardKey(pCmdData, dLen);
			break;
			
		case CMD_PLAY_VOICE:
			Ret = Comm_PlayVoice(pCmdData, dLen);
			break;

		case CMD_SET_VOICE_LEVEL:
			Ret = Comm_SetVoiceLevel(pCmdData, dLen);
			break;

		case CMD_VOICE_CMD_TEST:
			Ret = Comm_VoiceCmdTest(pCmdData, dLen);
			break;

		case CMD_SET_BOX_NUM:
			Ret = Comm_SetBoxNum(pCmdData, dLen);
			break;

		case CMD_OPEN_BOX_RETURN_STATUS:
		case CMD_OPEN_BOX:
			Ret = Comm_OpenOneBox(CMD, pCmdData, dLen);
			break;

		case CMD_GET_BOXDOOR_STATE:
			Ret = Comm_Get_BoxDoorStatus(pCmdData, dLen);
			break;

		case CMD_GET_BOX_STATE:
			Ret = Comm_CheckBoxEmpty(pCmdData, dLen);
			break;

		case CMD_SET_BOX_INFO:
			Ret = Comm_SetBoxInfo(pCmdData, dLen);
			break;

		case CMD_GET_BOX_INFO:
			Ret = Comm_GetBoxInfo(pCmdData, dLen);
			break;

		case CMD_SET_LOCKERS:
			Ret = Comm_Set_Lockers(pCmdData, dLen);
			break;
			
		case CMD_SET_VALID_CARD_LIST:
			Comm_Set_ValidCardList(pCmdData, dLen);
			break;

		case CMD_GET_VALID_CARD_LIST:
			Comm_Get_ValidCardList(pCmdData, dLen);
			break;

		case CMD_SET_BoxUserCard:   
			Ret = Comm_Set_BoxUserCard(pCmdData, dLen);
			break;
			
		case CMD_GET_BoxUserCard:   
			Ret = Comm_Get_BoxUserCard(pCmdData, dLen);
			break;

		case CMD_GET_AllBoxCard:             
			Ret = Comm_Get_AllBoxUserCard(pCmdData, dLen);
			break;
			
		case CMD_LockBox:
			Ret = Comm_LockNxBox(pCmdData, dLen);
			break;

		case CMD_UnlockBox:
			Ret = Comm_UnlockNxBox(pCmdData, dLen);
			break;
			
		case CMD_GET_RecordPoint:
			Ret = Comm_Get_RecordPoint(pCmdData, dLen);
			break;
			
		case CMD_GET_RECORD:
			Ret = Comm_Get_Record(pCmdData, dLen);
			break;

		case CMD_GET_RECORD_AGAIN:
			Ret = Comm_Get_RecordAgain(pCmdData, dLen);
			break;

		case CMD_CLR_AllRecord:
			Ret = Comm_Clr_AllRecord(pCmdData, dLen);
			break;
			
		default:
			break;
	}
	
	if(Err_DataLen == Ret) //指令执行时检测到数据长度不符
	{		
		CommDeal_Reply(CMD, Err_DataLen, pCmdData, 0);	 
	}
}

/******************************************************************************************************
* Function Name  : Thread_CommHandle
* Description    : 处理接收到的数据，验证机型机号通过后调用命令执行
* Input          : None
* Variable       : gstCmdPacket -- 通讯指令数据包
* Output         : None
* Return         : None
******************************************************************************************************/
void Thread_CommHandle(void)
{
	if(gstCmdPacket.bGetDataComplete)				//如果接收到有完整的命令数据包则进行处理
	{
		SetLedBlink(TICK_COUNT_100MS, 1);           //做通讯指示灯

		gstSysRunInfo.iLastCommTime = gstSysRunInfo.u32TimeCount_ms;
		
		if((CMD_GET_NUM == gstCmdPacket.u8Command)&&(0 == gstCmdPacket.u32MachineNum))
		{
			Comm_Get_MachineNum();
			SetLedBlink(TICK_COUNT_100MS, 3);			//做通讯指示灯
		}
		else
		{
			if((SysConfig.stHardware.u32MachineNum == gstCmdPacket.u32MachineNum)||(0xFFFFFFFF == gstCmdPacket.u32MachineNum)||(0xFFFFFFEE == gstCmdPacket.u32MachineNum)) //验证机号
			{
				if(0xFFFFFFFF == gstCmdPacket.u32MachineNum)	  
				{
					gstSysRunInfo.bTxdEnable = FALSE;	
				}
				else
				{
					gstSysRunInfo.bTxdEnable = TRUE;
				}	
				
				if(CMD_TYPE_REPLY == gstCmdPacket.u8AskOrReplay)
				{
					ServerReply_CommHandle(gstCmdPacket.u8Command, (uchar8*)gstCmdPacket.DataBuf, gstCmdPacket.u16length); 
				}
				else
				{
					Comm_Handle(gstCmdPacket.u8Command, (uchar8*)gstCmdPacket.DataBuf, gstCmdPacket.u16length); 	 
				}
				SetLedBlink(TICK_COUNT_100MS, 3);		//做通讯指示灯
			}
			else
			{
				gstSysRunInfo.CommType = COMM_TYPE_FREE;
			}
		}			
		gstCmdPacket.bGetDataComplete = FALSE;
	}	
}

/*========================File end==============================================================================*/

