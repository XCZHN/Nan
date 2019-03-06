/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Mid_SysConfig.c                                                                     
==                                                                                         
**  Function:	系统参数配置文件
==                                                                                         
**  Data:       2014/08/11                                                                     
================================================================================================*/

#include "Includes.h"

volatile ST_SYSTEM_RUN_INFO gstSysRunInfo;
volatile ST_SYSTEM_CONFIG   SysConfig;

const uchar8 SuperPassword[8] = {"913067"};
const uchar8 SeniorPassword[8] = {"999999"};
const uchar8 UserPassword[8] = {"888888"};


struct STimer *pTimer_DelayTick = NULL;
struct STimer *pTimer_TimeCount = NULL;
struct STimer *pTimer_GetRTC = NULL;

/*******************************************************************************
* Function Name  : void GPIO_ConfigInit(void)
* Description    : GPIO初始化
* Input          : None
* Output         : None
* Return         : None
* Attention		 : SYSTICK的时钟固定为HCLK时钟的1/8
*******************************************************************************/
void GPIO_ConfigInit(void)
{
	LPC_PINCON->PINSEL0 = 0xC0000050;
    LPC_PINCON->PINSEL1 = 0x0000003C; 
	LPC_PINCON->PINSEL2 = 0x00000000;
    LPC_PINCON->PINSEL3 = 0x00000000;
    //LPC_PINCON->PINSEL4 = 0x000A000A;
	LPC_PINCON->PINSEL4 = 0x0000000A;  //rxd2,txd2为GPIO  
}


/*******************************************************************************
* Function Name  : Get_MCU_ID
* Description    : 读取MCU的唯一ID
* Input          : pMcuID -- ID存放区指针
* Output         : None
* Return         : None
* Attention		 : STM32 96位(12字节)的产品唯一身份标识 基地址：0x1FFF F7E8
*******************************************************************************/            
void Get_MCU_ID(uchar8* pMcuID)
{
	uint32 CpuID[3] = {0};
		
	//获取CPU唯一ID
//	(*IAP_Entry)();
//	CpuID[0] =*(vu32*)(0x1ffff7e8);
//	CpuID[1] =*(vu32*)(0x1ffff7ec);
//	CpuID[2] =*(vu32*)(0x1ffff7f0);
	
	pMcuID[0] = CpuID[0] & 0xFF;
	pMcuID[1] = (CpuID[0] & 0xFF00) >>8;
	pMcuID[2] = (CpuID[0] & 0xFF0000) >>16;
	pMcuID[3] = (CpuID[0] & 0xFF000000) >>24;
	
	pMcuID[4] = CpuID[1] & 0xFF;
	pMcuID[5] = (CpuID[1] & 0xFF00) >>8;
	pMcuID[6] = (CpuID[1] & 0xFF0000) >>16;
	pMcuID[7] = (CpuID[1] & 0xFF000000) >>24;

	
	pMcuID[8] = CpuID[2] & 0xFF;
	pMcuID[9] = (CpuID[2] & 0xFF00) >>8;
	pMcuID[10] = (CpuID[2] & 0xFF0000) >>16;
	pMcuID[11] = (CpuID[2] & 0xFF000000) >>24;
}

/*******************************************************************************
* Function Name  : Calc_CRC
* Description    : 计算CRC
* Input          : pBuf -- 数据指针, len -- 数据长度
* Output         : None
* Return         : CRC8
* Attention		 : 参数最大值为 0xffffff / (HCLK / 8000000)
*******************************************************************************/            
uchar8  Calc_CRC(uchar8 *pBuf, uint32 len)
{
	uchar8 Crc8 = 0;
	uint32 i = 0;

	for(i=0; i<len; i++)
	{
		Crc8 ^= pBuf[i];	
	}
	
	return  Crc8;
}

/*******************************************************************************
* Function Name  : delay_us
* Description    : 延时函数 -- 单位 us  (经示波器测试,此延时时间是精准的)
* Input          : - Nus: 延时us
* Output         : None
* Return         : None
* Notices		 : 参数最大值为 0xffffff / (HCLK / 8000000)
*******************************************************************************/            
void delay_us(uint32 Nus)
{ 
	volatile uint32 i,k;
	while(Nus --)
	{    
		for(k=0; k<3; k++)  
		{  
			i++;
			if(i > 5)
			{
				i = 0;
			}
		}
	} 
}


/*******************************************************************************
* Function Name  : delay_ms
* Description    : 延时函数 -- 单位 ms
* Input          : - nms: 延时ms
* Output         : None
* Return         : None
* Attention		 : 参数最大值 nms<=0xffffff*8*1000/SYSCLK 对72M条件下,nms<=1864 
*******************************************************************************/  
void delay_ms(uint32 nms)
{ 
	uint32 iCount = 0;
//	SysTick_Disable();
	gstSysRunInfo.u32TimingDelay = nms;
//	SysTick_Stop();
	
	while(gstSysRunInfo.u32TimingDelay != 0)
	{
		iCount ++;
		if(iCount > 1000)
		{
			iCount = 0;
			WDT_Feed();
		}
	}
}

/*******************************************************************************
* Function Name  : TimingDelay_Decrement
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void TimingDelay_Decrement(void *Arg)
{
	if(gstSysRunInfo.u32TimingDelay != 0x00)
	{ 
		gstSysRunInfo.u32TimingDelay --;
	}
}

/*******************************************************************************
* Function Name  : TimeCount
* Description    : 计时器
* Input          : None
* Output         : None
* Return         : None
* Notes          : pTimer_TimeCount = TimerCreate(1, 1, TICK_COUNT_10MS, TimeCount, NULL);
*******************************************************************************/
void TimeCount(void *Arg)
{
	gstSysRunInfo.u32TimeCount_ms += TICK_COUNT_10MS;
}

/*******************************************************************************
* Function Name  : GetRTCTime
* Description    : 读取RTC时间
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GetRTCTime(void *Arg)
{
	if(gbRTC_TimeUpdate == 1)
	{	 
		gbRTC_TimeUpdate = 0;
		RTC_GetDateTime((pST_TIME)&gstSysRunInfo.stRtcTime);

		if((gstSysRunInfo.TimeBuf[2] != gstSysRunInfo.stRtcTime.u8Day)||(gstSysRunInfo.TimeBuf[3] != gstSysRunInfo.stRtcTime.u8Hour))
		{
			CheckSysValidityDate();
		}
	
		gstSysRunInfo.TimeBuf[0] = gstSysRunInfo.stRtcTime.u8Year;
		gstSysRunInfo.TimeBuf[1] = gstSysRunInfo.stRtcTime.u8Month;
		gstSysRunInfo.TimeBuf[2] = gstSysRunInfo.stRtcTime.u8Day;
		gstSysRunInfo.TimeBuf[3] = gstSysRunInfo.stRtcTime.u8Hour;
		gstSysRunInfo.TimeBuf[4] = gstSysRunInfo.stRtcTime.u8Minute;
		gstSysRunInfo.TimeBuf[5] = gstSysRunInfo.stRtcTime.u8Second;
		gstSysRunInfo.TimeBuf[6] = gstSysRunInfo.stRtcTime.u8WeekDay;

		gstSysRunInfo.u32RtcTime_S = (gstSysRunInfo.stRtcTime.u8Hour *3600) + (gstSysRunInfo.stRtcTime.u8Minute *60) + gstSysRunInfo.stRtcTime.u8Second;

		//检查时间的有效性
		if((Chack_Date(gstSysRunInfo.stRtcTime.u8Year, gstSysRunInfo.stRtcTime.u8Month, gstSysRunInfo.stRtcTime.u8Day))
			||(Chack_Time(gstSysRunInfo.stRtcTime.u8Hour, gstSysRunInfo.stRtcTime.u8Minute, gstSysRunInfo.stRtcTime.u8Second)))
		{
			gstSysRunInfo.stRtcTime.u8Year = 16;
			gstSysRunInfo.stRtcTime.u8Month = 8;
			gstSysRunInfo.stRtcTime.u8Day = 10;
			gstSysRunInfo.stRtcTime.u8Hour = 8;
			gstSysRunInfo.stRtcTime.u8Minute = 30;
			gstSysRunInfo.stRtcTime.u8Second = 0;
			gstSysRunInfo.stRtcTime.u8WeekDay = Calc_Weekday(gstSysRunInfo.stRtcTime.u8Year, gstSysRunInfo.stRtcTime.u8Month, gstSysRunInfo.stRtcTime.u8Day);
			
			RTC_SetDateTime((pST_TIME)&gstSysRunInfo.stRtcTime);	
		}
	}
}


/***********************************************************************************************
* Function Name  : Store_Parameters
* Description	 : 将参数存储至Flash(自动备份)
* Input 		 : Addr -- 存储地址， pBuf -- 数据指针， BufLen -- 数据长度
* Output		 : None
* Return		 : 0正常 ，非0参考错误代码
***********************************************************************************************/
int Store_Parameters(uint32 Addr, uchar8* pBuf, uint32 BufLen)
{	
	uint32 iBackupAddr = 0;
		
	I2C_EE_BufferWrite(pBuf, Addr, BufLen);

	//先计算出此参数相对于SysConfig起始地址的偏移量
	iBackupAddr = Addr - Addr_SysConfig;
	iBackupAddr += Addr_SysConfig_Backup;
	//写备份数据
	I2C_EE_BufferWrite(pBuf, iBackupAddr, BufLen);
	
	return 0;
}

/***********************************************************************************************
* Function Name  : Write_Parameters
* Description	 : 写参数
* Input 		 : Addr -- 存储地址， pBuf -- 数据指针， BufLen -- 数据长度
* Output		 : None
* Return		 : 0正常 ，非0参考错误代码
***********************************************************************************************/
int Write_Parameters(uchar8 * pBuf, uint32 Addr, uint32 ReadLen)
{
	I2C_EE_BufferWrite(pBuf, Addr, ReadLen);
	
	return 0;
}

/***********************************************************************************************
* Function Name  : Read_Parameters
* Description	 : 从Flash读取参数
* Input 		 : Addr -- 存储地址， pBuf -- 数据指针， BufLen -- 数据长度
* Output		 : None
* Return		 : 0正常 ，非0参考错误代码
***********************************************************************************************/
int Read_Parameters(uint32 Addr,uchar8 * pBuf,uint32 ReadLen)
{
	I2C_EE_BufferRead(pBuf, Addr, ReadLen);
	
	return 0;
}

/***********************************************************************************************
* Function Name  : Init_HardwareArg
* Description	 : 系统配置参数初始化  
* Input 		 : NULL
* Output		 : None
* Return		 : NULL
* Notes          : 恢复出厂设置
***********************************************************************************************/
void Init_HardwareArg(void)
{
	//清空信息
	memset((uchar8*)&SysConfig.stHardware, 0x0, sizeof(ST_SYS_HW));

	//取MCU的ID作为MAC和机号
	//CreateSysMAC(gstSysRunInfo.stRtcTime.u8Second, (uchar8*)&SysConfig.stNet.MAC[0]);
	memcpy((uchar8*)&SysConfig.stHardware.u32MachineNum, (uchar8*)&SysConfig.stNet.MAC[1], 4);
	
	//硬件配置
	SysConfig.stHardware.u32ConfigVer = 17070401;			
	SysConfig.stHardware.u32Baudrate = 115200;
	SysConfig.stHardware.bNetwork = 0;   
	SysConfig.stHardware.u8VoiceLevel = VOICE_LEVEL_MAX; 
	SysConfig.stHardware.bABCSwitch = VALIDITY_USING;
	SysConfig.stHardware.u32BeatInterval = 0;
    SysConfig.stHardware.u8RS485Type = COMM_TYPE_RS485_HOST;
		
	//保存参数
	Store_Parameters(Addr_Hardware, (uchar8*)&SysConfig.stHardware, sizeof(ST_SYS_HW));
}

/***********************************************************************************************
* Function Name  : Init_NetworkArg
* Description	 : 系统配置参数初始化  
* Input 		 : NULL
* Output		 : None
* Return		 : NULL
* Notes          : 恢复出厂设置
***********************************************************************************************/
void Init_NetworkArg(void)
{
	//清空信息
	memset((uchar8*)&SysConfig.stNet, 0x0, sizeof(ST_NETWORK));

	//网络参数
	SysConfig.stNet.IP[0] = 192;
	SysConfig.stNet.IP[1] = 168;
	SysConfig.stNet.IP[2] = 1;
	SysConfig.stNet.IP[3] = 201;
	CreateSysMAC(0xAC, (uchar8*)&SysConfig.stNet.MAC[0]);

	SysConfig.stNet.Gateway[0] = 192;
	SysConfig.stNet.Gateway[1] = 168;
	SysConfig.stNet.Gateway[2] = 1;
	SysConfig.stNet.Gateway[3] = 1;

    SysConfig.stNet.NetMask[0] = 255;
	SysConfig.stNet.NetMask[1] = 255;
	SysConfig.stNet.NetMask[2] = 255;
	SysConfig.stNet.NetMask[3] = 0;

	SysConfig.stNet.u16UDP_ListenPort = 6500;
	SysConfig.stNet.bAutoConnectServer = 0;
	SysConfig.stNet.u16UDP_ServerPort = 5600;

	SysConfig.stNet.ServerIP[0] = 192;
	SysConfig.stNet.ServerIP[1] = 168;
	SysConfig.stNet.ServerIP[2] = 1;
	SysConfig.stNet.ServerIP[3] = 200;

	//保存参数
	Store_Parameters(Addr_NetInfo, (uchar8*)&SysConfig.stNet, sizeof(ST_NETWORK));
}

/***********************************************************************************************
* Function Name  : Init_LockersArg
* Description	 : 系统配置参数初始化  
* Input 		 : NULL
* Output		 : None
* Return		 : NULL
* Notes          : 恢复出厂设置
***********************************************************************************************/
void Init_LockersArg(void)
{
	int i = 0;
	uint32 Addr = 0;
	
	//清空信息
	memset((uchar8*)&SysConfig.stLockers, 0x0, sizeof(ST_LOCKERS));

	//信包箱信息	
	memcpy((uchar8*)&SysConfig.stLockers.SuperPassword[0], SuperPassword, 6);
	memcpy((uchar8*)&SysConfig.stLockers.SeniorPassword[0], SeniorPassword, 6);
	memcpy((uchar8*)&SysConfig.stLockers.AdminPassword[0], UserPassword, 3);
	memcpy((char*)&SysConfig.stLockers.Name[0], "001", 3);

	SysConfig.stLockers.u8BoxCount = Default_Box_Count;
	SysConfig.stLockers.u8HostLockerBoxs = Default_Box_Count;
	SysConfig.stLockers.u8ExtL01BoxNum = 0;
	SysConfig.stLockers.u8ExtL02BoxNum = 0;
	SysConfig.stLockers.u8ExtL03BoxNum = 0;
	SysConfig.stLockers.bCheckBoxDoorStatus = 0; 
	
	SysConfig.stLockers.iMinFetchTime = 15;
	SysConfig.stLockers.iMaxFetchTime = 0;
	SysConfig.stLockers.u8AllowBeOpenedBoxNum = 3;
	SysConfig.stLockers.iHandleTimeout = 15;
	SysConfig.stLockers.iNoticeTime = 60;

	//保存参数
	Store_Parameters(Addr_Lockers, (uchar8*)&SysConfig.stLockers, sizeof(ST_LOCKERS));
	
	//初始化箱信息
	for(i=0; i<=MAX_BOX_NUM; i++)
	{
		memset((uchar8*)&SysConfig.stBox[i], 0x0, sizeof(ST_BOX));
		
		SysConfig.stBox[i].u8BoxID = i;
		SysConfig.stBox[i].u8Status = BOX_STATUS_IDLE;
		sprintf((char*)SysConfig.stBox[i].BoxSN, "%02d", i);

		Addr = Addr_SysConfig +  ((uint32)&SysConfig.stBox[i] - (uint32)&SysConfig);
		Store_Parameters(Addr, (uchar8*)&SysConfig.stBox[i], sizeof(ST_BOX));		
		WDT_Feed();
	}
}

/***********************************************************************************************
* Function Name  : Init_CardKey
* Description	 : 卡密钥初始化
* Input 		 : void
* Output		 : None
* Return		 : NULL
***********************************************************************************************/
void Init_CardKey(void)
{
	uint32 Addr = 0;
	memset((uchar8*)&SysConfig.stCardKey, 0x0, sizeof(ST_CARD_KEY));

	SysConfig.stCardKey.u8Sector = 10;
	SysConfig.stCardKey.u8KeyType = AuthKey_A;
	memset((uchar8*)&SysConfig.stCardKey.Password[0], 0x06, sizeof(SysConfig.stCardKey.Password));
	
	Addr = Addr_SysConfig +  ((uint32)&SysConfig.stCardKey - (uint32)&SysConfig);
	Store_Parameters(Addr, (uchar8*)&SysConfig.stCardKey, sizeof(ST_CARD_KEY));		
}


/***********************************************************************************************
* Function Name  : SysConfig_Init
* Description	 : 系统配置参数初始化 -- 恢复出厂设置
* Input 		 : NULL
* Output		 : None
* Return		 : NULL
***********************************************************************************************/
int	 SysConfig_Init(void)
{
	uchar8 buf[256] = {0};
	uint32 Addr = 0;
	extern const char START_MESSAGE[];

	Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
	
	//写FLASH标记信息,防止系统启动时自动初始化
	strcpy((char*)buf, START_MESSAGE);
	Write_Parameters(buf, Addr_SysMarkerInfo, strlen((char*)buf));   
	

	//有效期不能被清零
	memset(buf, 0x0, 10);
	memcpy(buf, (uchar8*)SysConfig.stClientInfo.ValidityDate, sizeof(SysConfig.stClientInfo.ValidityDate));
	
	//清空0客户信息
	memset((uchar8*)&SysConfig.stClientInfo, 0x0, sizeof(ST_CLIENT_INFO));
	memcpy((char*)&SysConfig.stClientInfo.IdCode[0], "ABCD01", 6);
	//还原有效期
	memcpy((uchar8*)SysConfig.stClientInfo.ValidityDate, buf, sizeof(SysConfig.stClientInfo.ValidityDate));
	//保存客户信息
	Addr = Addr_SysConfig +  ((uint32)&SysConfig.stClientInfo - (uint32)&SysConfig);
	Store_Parameters(Addr, (uchar8*)&SysConfig.stClientInfo, sizeof(ST_CLIENT_INFO));	
	

	//硬件配置
	Init_HardwareArg();

	//网络参数
	Init_NetworkArg();
	
	//信包箱信息
	Init_LockersArg();

	//卡信息
	Init_CardKey();
	
	//记录信息
	memset((uchar8*)&SysConfig.stRecord, 0x0, sizeof(ST_RECORD_COUNT));
	Addr = Addr_SysConfig +  ((uint32)&SysConfig.stRecord - (uint32)&SysConfig);
	Store_Parameters(Addr, (uchar8*)&SysConfig.stRecord, sizeof(ST_RECORD_COUNT));	

	//清除记录
	//Clear_Record();
	FLASH_ChipErase();

	Buzzer(800);	
	return 0;
}

/***********************************************************************************************
* Function Name  : Chcek_HardwareArg
* Description	 : 检查系统硬件配置参数 
* Input 		 : pstHW -- 参数指针
* Variable       : None
* Output		 : None
* Return		 : 0--无误，非0--有误(详见错误代码表)
* Notes          :  
***********************************************************************************************/
int	Chcek_HardwareArg(ST_SYS_HW* pstHW)
{	
	int iRet = 0;

	if(pstHW->u32MachineNum > 0xFFFFFFEE)
	{
		iRet = 1;
	}
	
	return iRet;
}

/***********************************************************************************************
* Function Name  : Chcek_NetworkArg
* Description	 : 检查网络配置参数 
* Input 		 : pstNet -- 参数指针
* Variable       : None
* Output		 : None
* Return		 : 0--无误，非0--有误(详见错误代码表)
* Notes          :  
***********************************************************************************************/
int	Chcek_NetworkArg(ST_NETWORK* pstNet)
{
	int iRet = 0;

	if((0xFF == pstNet->MAC[0])&&(0xFF == pstNet->MAC[1])&&(0xFF == pstNet->MAC[5]))
	{
		iRet = 1;	
	}
	else if(pstNet->u16UDP_ListenPort > 65000)
	{
		iRet = 2;	
	}
	
	return iRet;
}

/***********************************************************************************************
* Function Name  : Chcek_LockersArg
* Description	 : 检查系统硬件配置参数 
* Input 		 : pstLockers -- 参数指针
* Variable       : None
* Output		 : None
* Return		 : 0--无误，非0--有误(详见错误代码表)
* Notes          :  
***********************************************************************************************/
int	Chcek_LockersArg(ST_LOCKERS* pstLockers)
{
	int iRet = 0;

	if(pstLockers->u8BoxCount > (MAX_BOX_NUM + 1))
	{
		iRet = 1;	
	}
	
	return iRet;
}


/***********************************************************************************************
* Function Name  : Get_HardwareArg
* Description	 : 读取系统硬件配置参数 
* Input 		 : NULL
* Variable       : SysConfig.stHardware  
* Output		 : None
* Return		 : 0--成功，非0--失败(详见错误代码表)
* Notes          : 自动比对备份数据
***********************************************************************************************/
int	Get_HardwareArg(void)
{
	uint32 iAddr = 0;
	int iRet0 = 0;
	int iRet1 = 0;
	ST_SYS_HW stHardware;
	memset((uchar8*)&stHardware, 0x0, sizeof(ST_SYS_HW));
	
	iAddr = Addr_Hardware;
	Read_Parameters(iAddr, (uchar8*)&SysConfig.stHardware, sizeof(ST_SYS_HW));

	//再从备份地址读取此参数
	iAddr = Addr_SysConfig_Backup + Addr_Hardware_Offset;
	Read_Parameters(iAddr, (uchar8*)&stHardware, sizeof(ST_SYS_HW));

	//比对两份数据
	if(0 != memcmp((uchar8*)&stHardware, (uchar8*)&SysConfig.stHardware, sizeof(ST_SYS_HW)))
	{
		iRet0 = Chcek_HardwareArg((ST_SYS_HW*)&SysConfig.stHardware);
		iRet1 = Chcek_HardwareArg(&stHardware);
		if(0 == iRet0)
		{
			//以原数据为准,覆盖备份数据
			Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
			iAddr = Addr_SysConfig_Backup + Addr_Hardware_Offset;
			Write_Parameters((uchar8*)&SysConfig.stHardware, iAddr, sizeof(ST_SYS_HW));
		}
		else if(0 == iRet1)
		{
			//以备份数据为准,覆盖原始数据
			Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 4);
			memcpy((uchar8*)&SysConfig.stHardware, (uchar8*)&stHardware, sizeof(ST_SYS_HW));
			Write_Parameters((uchar8*)&SysConfig.stHardware, Addr_Hardware, sizeof(ST_SYS_HW));			
		}
		else
		{
			//两份数据都有问题,初始化此参数
			Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
			Init_HardwareArg();
		}
	}
	return 0;
}

/***********************************************************************************************
* Function Name  : Get_NetworkArg
* Description	 : 读取系统网络配置参数 
* Input 		 : NULL
* Variable       : SysConfig.stNet  
* Output		 : None
* Return		 : 0--成功，非0--失败(详见错误代码表)
* Notes          : 自动比对备份数据
***********************************************************************************************/
int	Get_NetworkArg(void)
{
	uint32 iAddr = 0;
	int iRet0 = 0;
	int iRet1 = 0;
	ST_NETWORK stNet;
	memset((uchar8*)&stNet, 0x0, sizeof(ST_NETWORK));
	
	iAddr = Addr_NetInfo;
	Read_Parameters(iAddr, (uchar8*)&SysConfig.stNet, sizeof(ST_NETWORK));

	//再从备份地址读取此参数
	iAddr = Addr_SysConfig_Backup + Addr_NetInfo_Offset;
	Read_Parameters(iAddr, (uchar8*)&stNet, sizeof(ST_NETWORK));

	//比对两份数据
	if(0 != memcmp((uchar8*)&stNet, (uchar8*)&SysConfig.stNet, sizeof(ST_NETWORK)))
	{
		iRet0 = Chcek_NetworkArg((ST_NETWORK*)&SysConfig.stNet);
		iRet1 = Chcek_NetworkArg(&stNet);
		if(0 == iRet0)
		{
			//以原数据为准,覆盖备份数据
			Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
			iAddr = Addr_SysConfig_Backup + Addr_NetInfo_Offset;
			Write_Parameters((uchar8*)&SysConfig.stNet, iAddr, sizeof(ST_NETWORK));
		}
		else if(0 == iRet1)
		{
			//以备份数据为准,覆盖原始数据
			Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 4);
			memcpy((uchar8*)&SysConfig.stNet, (uchar8*)&stNet, sizeof(ST_NETWORK));
			Write_Parameters((uchar8*)&SysConfig.stNet, Addr_NetInfo, sizeof(ST_NETWORK));			
		}
		else
		{
			//两份数据都有问题,初始化此参数
			Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
			Init_NetworkArg();
		}	
	}
	
	return 0;
}

/***********************************************************************************************
* Function Name  : Get_LockersArg
* Description	 : 读取箱柜配置参数以及每个箱的信息
* Input 		 : NULL
* Variable       : SysConfig 
* Output		 : None
* Return		 : 0--成功，非0--失败(详见错误代码表)
* Notes          : 自动比对备份数据
***********************************************************************************************/
int	Get_LockersArg(void)
{
	uint32 iAddr = 0;
	int iRet0 = 0;
	int iRet1 = 0;
	ST_LOCKERS stLockers;
	memset((uchar8*)&stLockers, 0x0, sizeof(ST_LOCKERS));
	
	iAddr = Addr_Lockers;
	Read_Parameters(iAddr, (uchar8*)&SysConfig.stLockers, sizeof(ST_LOCKERS));

	//再从备份地址读取此参数
	iAddr = Addr_SysConfig_Backup + Addr_Lockers_Offset;
	Read_Parameters(iAddr, (uchar8*)&stLockers, sizeof(ST_LOCKERS));

	//比对两份数据
	if(0 != memcmp((uchar8*)&stLockers, (uchar8*)&SysConfig.stLockers, sizeof(ST_LOCKERS)))
	{	
		iRet0 = Chcek_LockersArg((ST_LOCKERS*)&SysConfig.stLockers);
		iRet1 = Chcek_LockersArg(&stLockers);
		if(0 == iRet0)
		{
			//以原数据为准,覆盖备份数据
			Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
			iAddr = Addr_SysConfig_Backup + Addr_Lockers_Offset;
			Write_Parameters((uchar8*)&SysConfig.stLockers, iAddr, sizeof(ST_LOCKERS));
		}
		else if(0 == iRet1)
		{
			//以备份数据为准,覆盖原始数据
			Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 4);
			memcpy((uchar8*)&SysConfig.stLockers, (uchar8*)&stLockers, sizeof(ST_LOCKERS));
			Write_Parameters((uchar8*)&SysConfig.stLockers, Addr_Lockers, sizeof(ST_LOCKERS));			
		}
		else
		{
			//两份数据都有问题,初始化此参数
			Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 5);
			Init_LockersArg();
		}
	}

	//读取每个箱的信息
	iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[0] - (uint32)&SysConfig);
	Read_Parameters(iAddr, (uchar8*)&SysConfig.stBox[0], (sizeof(ST_BOX)*(MAX_BOX_NUM +1)));


	//防错设置
	if(SysConfig.stLockers.u8BoxCount > MAX_BOX_NUM)
	{
		SysConfig.stLockers.u8BoxCount = MAX_BOX_NUM;
	}

	
	return 0;
}

/***********************************************************************************************
* Function Name  : Get_CardKey
* Description	 : 读取卡密钥信息
* Input 		 : NULL
* Variable       : SysConfig.stRecord  
* Output		 : None
* Return		 : 0--成功，非0--失败(详见错误代码表)
***********************************************************************************************/
int	Get_CardKey(void)
{
	uint32 iAddr = 0;
	
	iAddr = Addr_SysConfig + ((uint32)&SysConfig.stCardKey - (uint32)&SysConfig);
	Read_Parameters(iAddr, (uchar8*)&SysConfig.stCardKey, sizeof(ST_CARD_KEY));

	return 0;
}

/***********************************************************************************************
* Function Name  : Get_ClientInfo
* Description	 : 读取使用单位信息
* Input 		 : NULL
* Variable       : SysConfig.stClientInfo  
* Output		 : None
* Return		 : 0--成功，非0--失败(详见错误代码表)
* Notes          : 自动比对备份数据
***********************************************************************************************/
int	Get_ClientInfo(void)
{
	uint32 iAddr = 0;
	ST_CLIENT_INFO stClientInfo;
	memset((uchar8*)&stClientInfo, 0x0, sizeof(ST_CLIENT_INFO));
	
	iAddr = Addr_SysConfig + ((uint32)&SysConfig.stClientInfo - (uint32)&SysConfig);
	Read_Parameters(iAddr, (uchar8*)&SysConfig.stClientInfo, sizeof(ST_CLIENT_INFO));

	//再从备份地址读取此参数
	iAddr = Addr_SysConfig_Backup + ((uint32)&SysConfig.stClientInfo - (uint32)&SysConfig);
	Read_Parameters(iAddr, (uchar8*)&stClientInfo, sizeof(ST_CLIENT_INFO));

	//比对两份数据
	if(0 != memcmp((uchar8*)&stClientInfo, (uchar8*)&SysConfig.stClientInfo, sizeof(ST_CLIENT_INFO)))
	{
		//
	}
	
	return 0;
}

/***********************************************************************************************
* Function Name  : Get_RecordPoint
* Description	 : 读取记录统计信息
* Input 		 : NULL
* Variable       : SysConfig.stRecord  
* Output		 : None
* Return		 : 0--成功，非0--失败(详见错误代码表)
* Notes          : 自动比对备份数据
***********************************************************************************************/
int	Get_RecordPoint(void)
{
	uint32 iAddr = 0;
	uint32 iLen = sizeof(ST_RECORD_COUNT);
	
	iAddr = Addr_SysConfig + ((uint32)&SysConfig.stRecord - (uint32)&SysConfig);
	Read_Parameters(iAddr, (uchar8*)&SysConfig.stRecord, iLen);

	return 0;
}

/***********************************************************************************************
* Function Name  : ReadSysConfig
* Description	 : 读取系统配置参数 
* Input 		 : NULL
* Variable       : SysConfig  
* Output		 : None
* Return		 : 0--成功，非0--失败(详见错误代码表)
***********************************************************************************************/
int	 ReadSysConfig(void)
{
	uchar8 TempBuf[200] = {0};
	uint32 iLen = 0;
	extern const char START_MESSAGE[];
	iLen = strlen(START_MESSAGE);
    if(iLen > 100)
	{
		iLen = 100;
	}

	gstSysRunInfo.u32Temp = sizeof(SysConfig);
	if(gstSysRunInfo.u32Temp > 10240)
	{
		//参数结构体大于10K则警报
		Buzzer(4321);
		delay_ms(4321);
	}
	
	Read_Parameters(Addr_SysMarkerInfo, TempBuf, iLen);
	if(memcmp(TempBuf, START_MESSAGE, iLen))   //没有内存标记信息视为首次使用,自动初始化设备
	{
		Beep(BEEP_TIME_200MS, BEEP_TIME_200MS, 5);
		delay_ms(321);
		UART0_SendStr("For the first time use, automatic initialization system!\r\n");
		
		GUI_ShowNotice(NOTICE_SYS_INIT_WAIT);
		SysConfig_Init();
		//提示设置成功
		GUI_ShowNotice(NOTICE_SYS_INIT_OK);
		
		delay_ms(321);
	}


	//配置结构体清零	
	memset((uchar8*)&SysConfig, 0x0, sizeof(SysConfig));

	//--------------------读取硬件参数--------------------------
	Get_HardwareArg();

	//--------------------读取网络参数--------------------------
	Get_NetworkArg();
	
	//--------------读取箱柜配置参数以及每个箱的信息------------
	Get_LockersArg();

	//--------------读取IC卡信息------------
	Get_CardKey();
	
	//--------------------读取使用单位信息-----------------------
	Get_ClientInfo();

	//-------------------读取记录统计信息---------------------
	Get_RecordPoint();
	
	return 0;
}

/***********************************************************************************************
* Function Name  : CheckSysValidityDate
* Description	 : 有效期检查
* Input 		 : NULL
* Variable       : SysConfig  
* Output		 : None
* Return		 : 0x00 -- 未设置有效期, 其它为有效期限(天),小于0表示已过期天数
***********************************************************************************************/
int	CheckSysValidityDate(void)
{
	int iRet = VALIDITY_USING;
	uint32 iCurrentTime = 0;
	uint32 iValidityTime = 0;

	if((0 == SysConfig.stClientInfo.ValidityDate[0])||(0xFF == SysConfig.stClientInfo.ValidityDate[0]))
	{
		//未设置使用有效期
		gstSysRunInfo.bLockedSys = 0;
		return 0;
	}
	else if((99 == SysConfig.stClientInfo.ValidityDate[0])&&(12 == SysConfig.stClientInfo.ValidityDate[1]))
	{
		//永久有效
		gstSysRunInfo.bLockedSys = 0;
		return 0;
	}

	iCurrentTime = (gstSysRunInfo.stRtcTime.u8Year * 365) + (gstSysRunInfo.stRtcTime.u8Month * 30) + gstSysRunInfo.stRtcTime.u8Day;
	iValidityTime = (SysConfig.stClientInfo.ValidityDate[0] * 365) + (SysConfig.stClientInfo.ValidityDate[1] *30) + SysConfig.stClientInfo.ValidityDate[2];

	iRet = iValidityTime - iCurrentTime;
	if(iRet < 0)
	{
		gstSysRunInfo.bLockedSys = 1;
	}
	else
	{
		gstSysRunInfo.bLockedSys = 0;		
	}

	//只要设置了有效期间要禁止时间往回改
	gstSysRunInfo.bForbidTimeGoBack = 1;
	
	return iRet;
}

/*========================File end======================================================================*/

