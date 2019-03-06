/*===============================================================================================
==                                                                                              
**  Copyright(c) 2015  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Lockers.c                                                                       
==                                                                                         
**  Function:	储物箱处理程序 
==                                                                                         
**  Data:       2015/09/18                                                                     
================================================================================================*/

#include "Includes.h"

//-------------------------------------------------Variable definitions--------------------------- 


/***********************************************************************************************
* Function Name  : CheckBoxOnLockersNum
* Description	 : 检查箱号所在的柜号
* Input 		 : BoxNum -- 箱号, pExtBoxNum -- 在扩展柜中的新箱号
* Output		 : None
* Return		 : 箱号所在的柜号(0--主柜, 其它--副柜编号) 
* Note  		 : None 
***********************************************************************************************/
uchar8 CheckBoxOnLockersNum(uchar8 BoxNum, uchar8* pExtBoxNum)
{
	uchar8 LockersNum = 0;
	uchar8 ExtL01StartBoxNo = 0;   //1号副柜的起始箱号
	uchar8 ExtL02StartBoxNo = 0;   //2号副柜的起始箱号
	uchar8 ExtL03StartBoxNo = 0;   //3号副柜的起始箱号
	uchar8 ExtendBoxNum = 1;       //此箱号在扩展箱中的新箱号

	if((0 == SysConfig.stLockers.u8ExtL01BoxNum)&&(0 == SysConfig.stLockers.u8ExtL02BoxNum))
	{
		*pExtBoxNum = BoxNum;
		return 0;
	}
	
	ExtL01StartBoxNo = SysConfig.stLockers.u8HostLockerBoxs +1;   
	ExtL02StartBoxNo = ExtL01StartBoxNo + SysConfig.stLockers.u8ExtL01BoxNum;
	ExtL03StartBoxNo = ExtL02StartBoxNo + SysConfig.stLockers.u8ExtL02BoxNum;

	if(BoxNum >= (ExtL03StartBoxNo + SysConfig.stLockers.u8ExtL03BoxNum))
	{
		LockersNum = 4;
		ExtendBoxNum = 0;  //超出扩展范围,无效箱号
	}
	else if(BoxNum >= ExtL03StartBoxNo)
	{
		LockersNum = 3;
		ExtendBoxNum = (BoxNum - ExtL03StartBoxNo) +1;
	}
	else if(BoxNum >= ExtL02StartBoxNo)
	{
		LockersNum = 2;
		ExtendBoxNum = (BoxNum - ExtL02StartBoxNo) +1;
	}
	else if(BoxNum >= ExtL01StartBoxNo)
	{
		LockersNum = 1;
		ExtendBoxNum = (BoxNum - ExtL01StartBoxNo) +1;
	}
	else  //在主柜上
	{
		ExtendBoxNum = BoxNum; 
	}
	
	*pExtBoxNum = ExtendBoxNum;
	return LockersNum;
}

/***********************************************************************************************
* Function Name  : CheckEmptyBoxNum
* Description	 : 检查空箱数
* Input 		 : None
* Output		 : None
* Return		 : 空箱数
* Note  		 :  
***********************************************************************************************/
uchar8 CheckEmptyBoxNum(void)
{
	int i = 0;
	uchar8 EmptyBoxNum = 0;

	for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
	{
		if((BOX_STATUS_IDLE == SysConfig.stBox[i].u8Status)&&(LSTATUS_FREE == SysConfig.stBox[i].bLocking))
		{
			EmptyBoxNum ++;
		}
	}
	gstSysRunInfo.u8EmptyBoxCount = EmptyBoxNum;
		
	return EmptyBoxNum;
}


/***********************************************************************************************
* Function Name  : ParseBoxDoorStatus
* Description	 : 解析箱门状态,并将状态存放到 gstSysRunInfo.BoxDoorStatus
* Input 		 : LockerNum--柜号,　pStatusBuf--状态数组 
* Output		 : None
* Return		 : None
* Note  		 :  
***********************************************************************************************/
void ParseBoxDoorStatus(uchar8 LockerNum, uchar8* pStatusBuf)
{
	uchar8 i = 0;
	uchar8 BoxByteAddr = 0;  //箱号所在字节
	uchar8 BoxBitAddr = 0;   //箱号所在字节的位地址
	uchar8 BoxCount = 0;
	uchar8 BoxStatusAddr = 0;

	if(0 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8HostLockerBoxs;
		BoxStatusAddr = 0;
		for(i=1; i<= BoxCount; i++)		
		{
			BoxByteAddr = ((i-1) /8);						   //计算箱号状态所在字节
			BoxBitAddr = ((i-1) %8);						   //计算箱号状态所在对应的位
			gstSysRunInfo.BoxDoorStatus[BoxStatusAddr + i] = ((pStatusBuf[BoxByteAddr] >> BoxBitAddr) &0x01);
		}	
	}
	else if(1 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8ExtL01BoxNum;
		BoxStatusAddr = SysConfig.stLockers.u8HostLockerBoxs + 1;
		memcpy((uchar8*)&gstSysRunInfo.BoxDoorStatus[BoxStatusAddr], pStatusBuf, BoxCount);
	}
	else if(2 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8ExtL02BoxNum;
		BoxStatusAddr = SysConfig.stLockers.u8HostLockerBoxs + SysConfig.stLockers.u8ExtL01BoxNum + 1;
		memcpy((uchar8*)&gstSysRunInfo.BoxDoorStatus[BoxStatusAddr], pStatusBuf, BoxCount);
	}
	else if(3 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8ExtL03BoxNum;
			
		BoxStatusAddr = SysConfig.stLockers.u8HostLockerBoxs + 1; 
		BoxStatusAddr += SysConfig.stLockers.u8ExtL01BoxNum; 
		BoxStatusAddr += SysConfig.stLockers.u8ExtL02BoxNum;
		memcpy((uchar8*)&gstSysRunInfo.BoxDoorStatus[BoxStatusAddr], pStatusBuf, BoxCount);
	}
}

/***********************************************************************************************
* Function Name  : ParseBoxInsideStatus
* Description	 : 解析箱内状态,并将状态存放到 gstSysRunInfo.BoxInsideStatus
* Input 		 : LockerNum--柜号,　pStatusBuf--状态数组 
* Output		 : None
* Return		 : None
* Note  		 :  
***********************************************************************************************/
void ParseBoxInsideStatus(uchar8 LockerNum, uchar8* pStatusBuf)
{
	uchar8 i = 0;
	uchar8 BoxByteAddr = 0;  //箱号所在字节
	uchar8 BoxBitAddr = 0;   //箱号所在字节的位地址
	uchar8 BoxCount = 0;
	uchar8 BoxStatusAddr = 0;

	if(0 == LockerNum)
	{
		for(i=1; i<= SysConfig.stLockers.u8HostLockerBoxs; i++)		
		{
			BoxByteAddr = ((i-1) /8);						   //计算箱号状态所在字节
			BoxBitAddr = ((i-1) %8);						   //计算箱号状态所在对应的位
			gstSysRunInfo.BoxInsideStatus[i] = ((pStatusBuf[BoxByteAddr] >> BoxBitAddr) &0x01);
		}	
	}
	else if(1 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8ExtL01BoxNum;
		BoxStatusAddr = SysConfig.stLockers.u8HostLockerBoxs +1;
		memcpy((uchar8*)&gstSysRunInfo.BoxInsideStatus[BoxStatusAddr], pStatusBuf, BoxCount);
	}
	else if(2 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8ExtL02BoxNum;
		BoxStatusAddr = SysConfig.stLockers.u8HostLockerBoxs + SysConfig.stLockers.u8ExtL01BoxNum +1;
		memcpy((uchar8*)&gstSysRunInfo.BoxInsideStatus[BoxStatusAddr], pStatusBuf, BoxCount);
	}
	else if(3 == LockerNum)
	{
		BoxCount = SysConfig.stLockers.u8ExtL03BoxNum;
			
		BoxStatusAddr = SysConfig.stLockers.u8HostLockerBoxs +1; 
		BoxStatusAddr += SysConfig.stLockers.u8ExtL01BoxNum; 
		BoxStatusAddr += SysConfig.stLockers.u8ExtL02BoxNum;
		memcpy((uchar8*)&gstSysRunInfo.BoxInsideStatus[BoxStatusAddr], pStatusBuf, BoxCount);
	}
}

/***********************************************************************************************
* Function Name  : ScanLocalBoxStatus
* Description	 : 扫描本机箱门,箱内状态
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 :
***********************************************************************************************/
void ScanLocalBoxStatus(void)
{
	static uint32 LastScanTime;
	static uchar8 ScanStep;
		
	switch(ScanStep)
	{
		case 0: //限制执行时间间隔,以免扫描太过频繁
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 600)
			{
				LastScanTime = gstSysRunInfo.u32TimeCount_ms;
				ScanStep ++;
			}
		}
		break;
		
		case 1: //扫描箱门状态
		{
			memset((uchar8*)&gstSysRunInfo.TempBuf[0], 0x0, sizeof(gstSysRunInfo.TempBuf));
			gstSysRunInfo.u8Temp = GetBoxDoorStatus((uchar8*)&gstSysRunInfo.TempBuf[0], SysConfig.stLockers.u8HostLockerBoxs);
			ParseBoxDoorStatus(0, (uchar8*)gstSysRunInfo.TempBuf);
			ScanStep = 0;
		}
		break;

		default:
		{
			ScanStep = 0;
		}
		break;
	}
}

/***********************************************************************************************
* Function Name  : ScanExtendBoxStatus
* Description	 : 扩展副柜开箱门\检查状态
* Input 		 : bReset -- 复位标志(0有效)
* Output		 : None
* Return		 : None
* Note  		 : 当前支持扩展3个副柜  
***********************************************************************************************/
void ScanExtendBoxStatus(uchar8 bReset)
{
	static uint32 LastScanTime;
	static uchar8 HandleStep;

	if(0 == bReset)
	{
		HandleStep = 0;	
		LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		return;
	}
	
	if((SYS_STATUS_KEY == gstSysRunInfo.CurrentStatus)&&(6 == gstKeyboard.u8AdminMenuStep))
	{
		return;	
	}
	else if(gstSysRunInfo.bRS485Busy)
	{
		LastScanTime = gstSysRunInfo.u32TimeCount_ms;
		return;
	}

	switch(HandleStep)
	{
		case 0: //扫描状态(限制执行时间间隔,以免扫描太过频繁)
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 300)
			{
				LastScanTime = gstSysRunInfo.u32TimeCount_ms;
				if(SysConfig.stLockers.u8ExtL01BoxNum)
				{
					HandleStep ++;
				}
			}
			else
			{
				HandleStep = 0;	
			}
		}
		break;

		case 1: //扫描扩展锁控板1的箱门状态 
		{
			Clean_RS485RxdBuf();

			gstSysRunInfo.TempBuf[0] = 1;
			gstSysRunInfo.TempBuf[1] = SysConfig.stLockers.u8ExtL01BoxNum;
			SendCmdToExtendLockers(1, CMD_GET_BOXDOOR_STATE, (uchar8*)&gstSysRunInfo.TempBuf[0], 2);

			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
			HandleStep ++;
		}
		break;

		case 2: //等一会再读取RS485返回的数据 
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 200)  //实测在19200BPS下,接受回应答的时间为135ms
			{
				if(RS485DataCnt > CMD_PACKET_MIN_LEN)
				{					
					memset((uchar8*)&stCmdSlavePacket, 0x0, sizeof(stCmdSlavePacket));
					if(CMD_GET_BOXDOOR_STATE == SlavePacketDecoding(RS485DataBuf, RS485DataCnt, &stCmdSlavePacket))
					{
						if(1 == stCmdSlavePacket.iLockerNum)  //检查机号
						{
							//应答指令中的数据结构为 执行状态1+起始箱门号1+箱门数1+箱门状态N 	
							ParseBoxDoorStatus(1, (uchar8*)&stCmdSlavePacket.DataBuf[3]);
						}						
					}
				}	
				
				if(SysConfig.stLockers.u8ExtL02BoxNum)
				{
					HandleStep ++;
				}
				else
				{
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;
					HandleStep =0;
				}
			}
		}
		break;
		
		case 3: //扫描扩展锁控板2的箱门状态 
		{
			Clean_RS485RxdBuf();

			gstSysRunInfo.TempBuf[0] = 1;
			gstSysRunInfo.TempBuf[1] = SysConfig.stLockers.u8ExtL02BoxNum;
			SendCmdToExtendLockers(2, CMD_GET_BOXDOOR_STATE, (uchar8*)&gstSysRunInfo.TempBuf[0], 2);

			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
			HandleStep ++;
		}
		break;		

		case 4: //等一会再读取RS485返回的数据 
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 200)
			{
				if(RS485DataCnt > CMD_PACKET_MIN_LEN)
				{					
					memset((uchar8*)&stCmdSlavePacket, 0x0, sizeof(stCmdSlavePacket));
					if(CMD_GET_BOXDOOR_STATE == SlavePacketDecoding(RS485DataBuf, RS485DataCnt, &stCmdSlavePacket))
					{
						if(2 == stCmdSlavePacket.iLockerNum)  //检查机号
						{
							//应答指令中的数据结构为 执行状态1+起始箱门号1+箱门数1+箱门状态N 	
							ParseBoxDoorStatus(2, (uchar8*)&stCmdSlavePacket.DataBuf[3]);
						}
					}
				}					
				
				if(SysConfig.stLockers.u8ExtL03BoxNum)
				{
					HandleStep ++;
				}
				else
				{
					LastScanTime = gstSysRunInfo.u32TimeCount_ms;
					HandleStep =0;
				}
			}
		}
		break;

		case 5: //扫描扩展锁控板3的箱门状态 
		{
			Clean_RS485RxdBuf();

			gstSysRunInfo.TempBuf[0] = 1;
			gstSysRunInfo.TempBuf[1] = SysConfig.stLockers.u8ExtL03BoxNum;
			SendCmdToExtendLockers(3, CMD_GET_BOXDOOR_STATE, (uchar8*)&gstSysRunInfo.TempBuf[0], 2);

			LastScanTime = gstSysRunInfo.u32TimeCount_ms;
			HandleStep ++;
		}
		break;		

		case 6: //等一会再读取RS485返回的数据 
		{
			if(gstSysRunInfo.u32TimeCount_ms - LastScanTime > 200)
			{
				LastScanTime = gstSysRunInfo.u32TimeCount_ms;
				if(RS485DataCnt)
				{					
					memset((uchar8*)&stCmdSlavePacket, 0x0, sizeof(stCmdSlavePacket));
					if(CMD_GET_BOXDOOR_STATE == SlavePacketDecoding(RS485DataBuf, RS485DataCnt, &stCmdSlavePacket))
					{
						if(3 == stCmdSlavePacket.iLockerNum)  //检查机号
						{
							//应答指令中的数据结构为 执行状态1+起始箱门号1+箱门数1+箱门状态N 	
							ParseBoxDoorStatus(3, (uchar8*)&stCmdSlavePacket.DataBuf[3]);
						}
					}
				}	
				LastScanTime = gstSysRunInfo.u32TimeCount_ms;
				HandleStep = 0;
			}
		}
		break;
			
		default:
		{
			HandleStep = 0;	
		}
		break;
	}
}

/***********************************************************************************************
* Function Name  : ScanAllBoxStatus
* Description	 : 扫描所有箱门状态
* Input 		 : void
* Output		 : None
* Return		 : None
* Note  		 : 包括扩展箱柜的箱门状态
***********************************************************************************************/
void ScanAllBoxStatus(void)
{
	if(SysConfig.stLockers.bCheckBoxDoorStatus)
	{
		ScanLocalBoxStatus();
		ScanExtendBoxStatus(1);
	}
}

/***********************************************************************************************
* Function Name  : Choice1BoxOpen
* Description	 : 自动开启一个箱门, 不检测箱门是否开启成功
* Input 		 : Box -- 待开启的箱门号
* Output		 : None
* Return		 : 执行结果 0--开箱成功, 其它--开箱失败
* Notes          : 自动判断是在主柜还是副柜上
***********************************************************************************************/
uchar8 Choice1BoxOpen(uint8 BoxNum)
{
	uchar8 ExtendLockerNum = 0;
	uchar8 ExtendNewBoxNum = 0;
	uchar8 SendBuf[20] = {0};
		
	if(BoxNum <= SysConfig.stLockers.u8HostLockerBoxs)
	{
		OpenOneBox(BoxNum);
	}
	else
	{
		ExtendLockerNum = CheckBoxOnLockersNum(BoxNum, &ExtendNewBoxNum);
		if(ExtendLockerNum) 
		{
			SendBuf[0] = ExtendNewBoxNum;
			gstSysRunInfo.bRS485Busy = 1;
			delay_ms(220);
			SendCmdToExtendLockers(ExtendLockerNum, CMD_OPEN_BOX, SendBuf, 1);
			gstSysRunInfo.bRS485Busy = 0;
			ScanExtendBoxStatus(0);
		}
	}
	return 0;
}

/***********************************************************************************************
* Function Name  : Choice1BoxOpen_CheckResult
* Description	 : 自动开启一个箱门,并且检测箱门状态,开启失败则再尝试一次
* Input 		 : Box -- 待开启的箱门号
* Output		 : None
* Return		 : 执行结果 0--开箱成功, 其它--开箱失败
* Notes          : 自动判断是在主柜还是副柜上
***********************************************************************************************/
uchar8 Choice1BoxOpen_CheckResult(uint8 BoxNum)
{
	uchar8 status = 0;
	uchar8 SendBuf[20] = {0};
	uchar8 WaitCut = 0;
	uchar8 ExtendLockerNum = 0;
	uchar8 ExtendNewBoxNum = 0;
		
	if(BoxNum <= SysConfig.stLockers.u8HostLockerBoxs)
	{
		OpenOneBox(BoxNum);
		delay_ms(800);
		memset((uchar8*)&gstSysRunInfo.TempBuf[0], 0x0, sizeof(gstSysRunInfo.TempBuf));
		gstSysRunInfo.u8Temp = GetBoxDoorStatus((uchar8*)&gstSysRunInfo.TempBuf[0], SysConfig.stLockers.u8HostLockerBoxs);
		ParseBoxDoorStatus(0, (uchar8*)gstSysRunInfo.TempBuf);
		if(0 == gstSysRunInfo.BoxDoorStatus[BoxNum])  //开启失败再尝试一次
		{
			OpenOneBox(BoxNum);
			delay_ms(800);
			memset((uchar8*)&gstSysRunInfo.TempBuf[0], 0x0, sizeof(gstSysRunInfo.TempBuf));
			gstSysRunInfo.u8Temp = GetBoxDoorStatus((uchar8*)&gstSysRunInfo.TempBuf[0], SysConfig.stLockers.u8HostLockerBoxs);
			ParseBoxDoorStatus(0, (uchar8*)gstSysRunInfo.TempBuf);
			if(0 == gstSysRunInfo.BoxDoorStatus[BoxNum])  //开启失败 
			{
				status = 0xEE;		
			}
		}
	}
	else
	{
		gstSysRunInfo.bRS485Busy = 1;
		delay_ms(220);

		ExtendLockerNum = CheckBoxOnLockersNum(BoxNum, &ExtendNewBoxNum);
		if(ExtendLockerNum) 
		{
			Clean_RS485RxdBuf();
			SendBuf[0] = ExtendNewBoxNum;
			SendCmdToExtendLockers(ExtendLockerNum, CMD_OPEN_BOX_RETURN_STATUS, SendBuf, 1);
			WaitCut = 30;
			status = 0xEE;	
			while(WaitCut --)
			{						
				if(RS485DataCnt >= 14)
				{					
					memset((uchar8*)&stCmdSlavePacket, 0x0, sizeof(stCmdSlavePacket));
					if(CMD_OPEN_BOX_RETURN_STATUS == SlavePacketDecoding(RS485DataBuf, RS485DataCnt, &stCmdSlavePacket))
					{
						Clean_RS485RxdBuf();
						if((ExtendLockerNum == stCmdSlavePacket.iLockerNum)&&(CMD_OPEN_BOX_RETURN_STATUS == stCmdSlavePacket.u8Cmd))	//检查机号和命令号
						{
							if(0 == stCmdSlavePacket.DataBuf[0])
							{
								//开锁成功
								status = 0; 
							}
							else
							{
								status = 0xEE;	
							}
							break;
						}
					}
				}
				delay_ms(100);
			}
		}
		gstSysRunInfo.bRS485Busy = 0;
		ScanExtendBoxStatus(0);
	}
	return status;
}

/***********************************************************************************************
* Function Name  : SelectOneBoxToOpen
* Description	 : 选择开启某个箱门
* Input 		 : boxId -- 待开启的箱门号, bCheckResult:0不检测开门结果,非0则检测开门结果
* Output		 : None
* Return		 : 执行结果 0--开箱成功, 其它--开箱失败
* Notes          : 自动判断是在主柜还是副柜上
***********************************************************************************************/
uchar8 SelectOneBoxToOpen(uint8 boxId, uchar8 bCheckResult)
{
	uchar8 status = 0;

	if(bCheckResult)
	{
		status = Choice1BoxOpen_CheckResult(boxId);
		if(OK != status)
		{
			status = Choice1BoxOpen_CheckResult(boxId);
		}
	}
	else
	{
		status = Choice1BoxOpen(boxId);
	}
	
	return status;
}

/***********************************************************************************************
* Function Name  : HandleTimeoutAlert
* Description	 : 寄存关门超时提醒
* Input 		 : 
* Output		 : None
* Return		 : None
***********************************************************************************************/
void HandleTimeoutAlert(void)
{
	static uint32 iLastHandleTime;
	static uchar8 step;

	switch(step)
	{
		case 0: //倒计时
		{
			if((gstSysRunInfo.u32TimeCount_ms - iLastHandleTime) >= 1000) 
			{
				iLastHandleTime = gstSysRunInfo.u32TimeCount_ms; 

				if(gstSysRunInfo.iHandleTimeout)
				{
					gstSysRunInfo.iHandleTimeout --;
					if(0 == gstSysRunInfo.iHandleTimeout)
					{
						//检查寄存时打开的门关了没有
						if(1 == gstSysRunInfo.BoxDoorStatus[gstSysRunInfo.u8UseBox])
						{
							if(SYS_STATUS_IDLE == gstSysRunInfo.CurrentStatus)
							{
								gstSysRunInfo.CurrentStatus = SYS_STATUS_Message;
								Beep(BEEP_TIME_200MS, BEEP_TIME_200MS, 0xFFFFFFFF);
								GUI_LockersNotice(LOCKERS_PlsCloseBoxDoor);
								iLastHandleTime = gstSysRunInfo.u32TimeCount_ms; 
								step ++;
							}	
						}
					}
				}
			}
		}
		break;

		case 1: //显示信息停留
		{
			//检查寄存时打开的门关了没有
			if((0 == gstSysRunInfo.BoxDoorStatus[gstSysRunInfo.u8UseBox])
				||((gstSysRunInfo.u32TimeCount_ms - iLastHandleTime) > (SysConfig.stLockers.iNoticeTime * 1000)))
			{
				Beep(BEEP_TIME_200MS, BEEP_TIME_200MS, 1);	
				GUI_Admin();
				gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;
				step = 0;
			}
		}
		break;
		
		default:
		{
			step = 0;
		}
		break;
	}	
}

/***********************************************************************************************
* Function Name  : UpdateEmptyBoxList
* Description	 : 更新空箱列表
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 将空箱重新装载到空箱列表中 
***********************************************************************************************/
void UpdateEmptyBoxList(void)
{
	int i = 0, j = 0;

	memset((uchar8*)&gstSysRunInfo.EmptyBox[0], 0x0, sizeof(gstSysRunInfo.EmptyBox));

	gstSysRunInfo.u8EmptyBoxCount = 0;
	for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
	{
		if((BOX_STATUS_IDLE == SysConfig.stBox[i].u8Status)&&(LSTATUS_FREE == SysConfig.stBox[i].bLocking))
		{
			gstSysRunInfo.EmptyBox[j++] = i;
			gstSysRunInfo.u8EmptyBoxCount ++;
		}
	}
}

/***********************************************************************************************
* Function Name  : AddOneEmptyBox
* Description	 : 往空箱队列末尾增加一个空箱箱号
* Input 		 : box--箱号
* Output		 : None
* Return		 : None
* Note  		 : None 
***********************************************************************************************/
void AddOneEmptyBox(uchar8 box)
{
	uchar8 i = 0;

	//先查询下空箱列表中有没有
	for(i=0; i<sizeof(gstSysRunInfo.EmptyBox); i++)
	{
		if(box == gstSysRunInfo.EmptyBox[i])
		{
			return;
		}
	}
	
	//空箱队列移位
	for(i=0; i<SysConfig.stLockers.u8BoxCount; i++)
	{
		if(0 == gstSysRunInfo.EmptyBox[i])
		{
			gstSysRunInfo.EmptyBox[i] = box;
			gstSysRunInfo.u8EmptyBoxCount +=1;
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : AddOneEmptyBox
* Description	 : 往空箱队列中删除一个空箱箱号
* Input 		 : box--箱号
* Output		 : None
* Return		 : None
* Note  		 : None 
***********************************************************************************************/
void DeleteOneEmptyBox(uchar8 box)
{
	uchar8 i = 0;
	uint32 Addr = 0;

	//查找
	for(i=0; i<sizeof(gstSysRunInfo.EmptyBox); i++)
	{
		if(box == gstSysRunInfo.EmptyBox[i])
		{
			Addr = i;
			break;
		}
	}
	
	if(i == sizeof(gstSysRunInfo.EmptyBox))
	{
		//没找到
		return;
	}
	else
	{
		//删除
		for(i=Addr; i<(sizeof(gstSysRunInfo.EmptyBox)-1); i++)
		{
			gstSysRunInfo.EmptyBox[i] = gstSysRunInfo.EmptyBox[i+1];
		}
		//确保最后一个为0
		gstSysRunInfo.EmptyBox[(sizeof(gstSysRunInfo.EmptyBox)-1)] = 0;
		gstSysRunInfo.u8EmptyBoxCount -= 1;
	}
}

/***********************************************************************************************
* Function Name  : CheckBoxStatusChange
* Description	 : 检查箱门\箱内状态改变
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : 当检查到有改变时立即上传至服务器
***********************************************************************************************/
void CheckBoxStatusChange(void)
{
	static uchar8 BoxDoorStatus[MAX_BOX_NUM+1] = {0};     
	static uchar8 BoxInsideStatus[MAX_BOX_NUM+1] = {0};   

	if(memcmp(&BoxDoorStatus[1], (uchar8*)&gstSysRunInfo.BoxDoorStatus[1], SysConfig.stLockers.u8BoxCount))
	{
		memcpy(&BoxDoorStatus[1], (uchar8*)&gstSysRunInfo.BoxDoorStatus[1], SysConfig.stLockers.u8BoxCount);
	}
	else if(memcmp(&BoxInsideStatus[1], (uchar8*)&gstSysRunInfo.BoxInsideStatus[1], SysConfig.stLockers.u8BoxCount))
	{
		memcpy(&BoxInsideStatus[1], (uchar8*)&gstSysRunInfo.BoxInsideStatus[1], SysConfig.stLockers.u8BoxCount);
	}
}

/***********************************************************************************************
* Function Name  : CheckValidityDate
* Description	 : 检查某个日期的有效性
* Input 		 : pDate -- 日期
* Output		 : None
* Return		 : 0x00 -- 未设置有效期, 其它为有效期限(天),小于0表示已过期天数
***********************************************************************************************/
int	CheckValidityDate(uchar8* pDate)
{
	int iRet = VALIDITY_USING;
	uint32 iCurrentTime = 0;
	uint32 iValidityTime = 0;

	if((0 == pDate[0])||(0xFF == pDate[0]))
	{
		//未设置使用有效期
		return 0;
	}
	else if((99 == pDate[0])&&(12 == pDate[1]))
	{
		//永久有效
		return 0;
	}

	iCurrentTime = (gstSysRunInfo.stRtcTime.u8Year * 365) + (gstSysRunInfo.stRtcTime.u8Month * 30) + gstSysRunInfo.stRtcTime.u8Day;
	iValidityTime = (pDate[0] * 365) + (pDate[1] *30) + pDate[2];

	iRet = iValidityTime - iCurrentTime;
	return iRet;
}


/***********************************************************************************************
* Function Name  : VoicePlay_DoorNotClose
* Description	 : 播放"xx号箱门未关,请关门!"
* Input 		 : u8DoorNum -- 门号
* Output		 : None
* Return		 : None
***********************************************************************************************/
void VoicePlay_DoorNotClose(uchar8 u8DoorNum)
{
#if GUI_ENGLISH
	Voice_Play(3);
	Voice_Play(u8DoorNum+10);
	Voice_Play(38);
#else
	uchar8 NumBit = 0;
	uchar8 PlayNum = 0;

	if(u8DoorNum > 99)
	{
		return;
	}
	else if(u8DoorNum > 9)
	{
		NumBit = 2;
	}
	else
	{
		NumBit = 1;	
	}

	switch(NumBit)
	{
		case 2:
		{	
			PlayNum = (u8DoorNum / 10) %10;	
			if(1 == PlayNum)
			{
				Voice_Play(YY_Num_Shi);
			}
			else if(PlayNum > 1)
			{
				Voice_Play(PlayNum + YY_Num_0);
				Voice_Play(YY_Num_Shi);
			}
			else if(0 == PlayNum)
			{
				if((3 == NumBit)&&(u8DoorNum % 10))
				{
					Voice_Play(YY_Num_0);
				}
			}
		}
		case 1:
		{
			PlayNum = (u8DoorNum % 10);
			if(PlayNum)
			{
				Voice_Play(PlayNum + YY_Num_0);
			}
			Voice_Play(YY_HaoXiangMengWeiGuan);
		}
		break;
	}
#endif
}

/***********************************************************************************************
* Function Name  : VoicePlay_xBoxOpen
* Description	 : 播放"xx号箱门已开!"
* Input 		 : u8Box -- 箱门号
* Output		 : None
* Return		 : None
***********************************************************************************************/
void VoicePlay_xBoxOpen(uchar8 u8Box)
{
#if GUI_ENGLISH
	Voice_Play(3);
	Voice_Play(u8Box+10);
	Voice_Play(5);
#else
	uchar8 NumBit = 0;
	uchar8 PlayNum = 0;

	if(u8Box > 99)
	{
		return;
	}
	else if(u8Box > 9)
	{
		NumBit = 2;
	}
	else
	{
		NumBit = 1;	
	}

	switch(NumBit)
	{
		case 2:
		{	
			PlayNum = (u8Box / 10) %10;	
			if(1 == PlayNum)
			{
				Voice_Play(YY_Num_Shi);
			}
			else if(PlayNum > 1)
			{
				Voice_Play(PlayNum + YY_Num_0);
				Voice_Play(YY_Num_Shi);
			}
			else if(0 == PlayNum)
			{
				if((3 == NumBit)&&(u8Box % 10))
				{
					Voice_Play(YY_Num_0);
				}
			}
		}
		case 1:
		{
			PlayNum = (u8Box % 10);
			if(PlayNum)
			{
				Voice_Play(PlayNum + YY_Num_0);
			}
			Voice_Play(YY_HaoXiangMengYiKai);
		}
		break;
	}
#endif
}


/*========================File end======================================================================*/

