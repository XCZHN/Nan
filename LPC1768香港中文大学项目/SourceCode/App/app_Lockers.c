/*===============================================================================================
==                                                                                              
**  Copyright(c) 2017  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	app_Lockers.c                                                                       
==                                                                                         
**  Function:	寄存柜存取操作流程                                                             
==
**  Data:       2017-06-27
================================================================================================*/

#include "Includes.h"
#include "app_Lockers.h"


//-----------------------------------寄存操作步骤表--------------------------------------/
typedef enum 							    
{
	LockersStep_Start = 0,			//开始执行	
	LockersStep_ReadCard,           //读卡
	LockersStep_OpenBox,            //开箱
	LockersStep_WaitCardAway,       //等待卡离开
	LockersStep_Notice, 			//显示提示信息	
	LockersStep_OnlineAsk,          //在线请求
	LockersStep_WaitServerReply,    //等待上位机答复
	LockersStep_GetFingerprint,     //等待录指纹
	LockersStep_
	
}LOCKERS_STEP;

//-----------------------------------操作步骤表--------------------------------------/
typedef enum 							    
{
	HandleStep_Start = 0,				//开始执行	
	HandleStep_Message,					//显示消息	
	HandleStep_GetFingerprint, 			//按指纹取件
	HandleStep_QuWuOpenBox, 			//取物开箱操作
	HandleStep_
	
}FINGER_HANDLE_STEP;

typedef	struct								 
{
	uint32 iCardNum;             //卡号
	uint32 iLastTime;            //上次执行时间

	uchar8 HandleStep;           //执行轨迹
//	uchar8 Box;                  //箱号  
	uchar8 TryCout; 			 //尝试计数
	
}ST_LOCKERS_OPERATION;

volatile ST_LOCKERS_OPERATION stLockersOperation;

uchar8 UserCardData[50] = {0};
volatile ST_CONSIGN stUserConsign;
volatile ST_ONLINEE_ASK stOnlineAsk;

/***********************************************************************************************
* Function Name  : UserConsign_OnlineAsk
* Description	 : 用户在线请求寄存
* Input 		 : iCardID -- 卡ID号
* Output		 : None
* Return		 : None 
***********************************************************************************************/
void UserConsign_OnlineAsk(uint32 iCardID, uchar8 box)
{
	uchar8 SendBuf[50] = {0};
	uint32 SendLen = 0;

	memcpy(SendBuf, (uchar8*)&iCardID, 4);
	SendLen += 4;
	SendBuf[SendLen++] = box;
	SendLen += 7; //预留
	Comm_ApplyFor(CMD_USER_ASK_CONSIGN, SendBuf, SendLen, COMM_TYPE_UDP_CLIENT);	
}

/***********************************************************************************************
* Function Name  : UserTakeAway_OnlineAsk
* Description	 : 用户在线请求取件
* Input 		 : iCardID -- 卡ID号
* Output		 : None
* Return		 : None 
***********************************************************************************************/
void UserTakeAway_OnlineAsk(uint32 iCardID, uchar8 box)
{
	uchar8 SendBuf[50] = {0};
	uint32 SendLen = 0;

	memcpy(SendBuf, (uchar8*)&iCardID, 4);
	SendLen += 4;
	SendBuf[SendLen++] = box;
	SendLen += 7; //预留

	Comm_ApplyFor(CMD_USER_ASK_TAKEAWAY, SendBuf, SendLen, COMM_TYPE_UDP_CLIENT);	
}

/***********************************************************************************************
* Function Name  : UserConsign_ServerReply
* Description	 : 用户在线寄存--服务器应答
* Input 		 : pReplyBuf -- 应答数据包, Len -- 应答数据长度
* Output		 : None
* Return		 : None 
* Notes          : 应答数据: 4Byte卡号 + 寄存标记(1B) + 所在柜名称(6B) + 所在箱号(1B) + 附带提示(20B)
***********************************************************************************************/
void UserConsign_ServerReply(uchar8 *pReplyBuf, uint16 Len)
{
	uint32 iCard = 0;
	uchar8 addr = 0;
	
	if(40 == Len)
	{
		memcpy((uchar8*)&iCard, &pReplyBuf[addr], 4);
		addr += 4;
		if(iCard == stOnlineAsk.iCardID)
		{
			stOnlineAsk.u8OnBox = pReplyBuf[addr++];
			stOnlineAsk.bAllowConsign = pReplyBuf[addr++];
			
			memcpy((uchar8*)&stOnlineAsk.OnLockers[0], &pReplyBuf[addr], 6);
			addr += 6;
			stOnlineAsk.u8OnBox = pReplyBuf[addr++];
			memcpy((uchar8*)&stOnlineAsk.Notice[0], &pReplyBuf[addr], 20);
		}		
		
		stOnlineAsk.bReplyOk = SERVER_REPLY_MASK;
	}
}

/******************************************************************************************************
* Function Name  : ServerReply_CommHandle
* Description    : 服务器应答指令执行
* Input          : CMD--命令字, pRecvBuf－－命令数据, dLen -- 数据部分长度
* Variable       : gstCmdPacket -- 通讯指令数据包
* Output         : None
* Return         : None
******************************************************************************************************/
void ServerReply_CommHandle(uchar8 CMD, uchar8* pRecvBuf, uint16 dLen)
{	
	stOnlineAsk.u8Cmd = CMD;
	
	switch(CMD)
	{
		case CMD_USER_ASK_CONSIGN:
		case CMD_USER_ASK_TAKEAWAY:
		{
			UserConsign_ServerReply(pRecvBuf, dLen);	
		}
		break;
	}
}

/***********************************************************************************************
* Function Name  : ConsignMode_AnyCard
* Description	 : 用户刷卡寄存--任意卡模式
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 刷卡后分配空箱
***********************************************************************************************/
void ConsignMode_AnyCard(void)
{
	int i = 0;
	uchar8 ret = 0;
	uint32 iAddr = 0;
	uint32 iCardID01 = 0;
	uchar8 OpenBoxCnt = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8AdminMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8SecondMenuStep)
		{
			case LockersStep_Start: //按下寄存键后检查寄存条件
			{				
				//系统过期被锁,禁用寄存功能
				if(gstSysRunInfo.bLockedSys)
				{
					GUI_LockersNotice(LOCKERS_SysError);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);

					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					return;
				}

				//检查箱门是否关好
				if(SysConfig.stLockers.bCheckBoxDoorStatus)
				{
					gstKeyboard.u8Box = 0;
					for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
					{
						if((1 == gstSysRunInfo.BoxDoorStatus[i])&&(LSTATUS_LOCKING != SysConfig.stBox[i].bLocking))
						{
							if(0 == gstKeyboard.u8Box)
							{	
								gstKeyboard.u8Box = i;
							}
							OpenBoxCnt ++;
						}
					}
					if(OpenBoxCnt > SysConfig.stLockers.u8AllowBeOpenedBoxNum)
					{
						GUI_PlsCloseBoxDoor(gstKeyboard.u8Box);
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						VoicePlay_DoorNotClose(gstKeyboard.u8Box);

						stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
						gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
						return;
					}
				}
				
				//检查有没有空箱	
				if((gstSysRunInfo.EmptyBox[0]>0)&&(gstSysRunInfo.EmptyBox[0] <= SysConfig.stLockers.u8BoxCount))
				{	
					GUI_LockersNotice(LOCKERS_PlsCard);
					Voice_Play(YY_PlsCard);
					gstKeyboard.u8SecondMenuStep = LockersStep_ReadCard;
				}
				else
				{
					//提示箱已满								
					GUI_LockersNotice(LOCKERS_IsFull);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
				}				
			}
			break;	

			case LockersStep_ReadCard:  
			{
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > 300)
				{
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.iCardUID = Get_IC_ID_CardSN();
					if(gstKeyboard.iCardUID)
					{	
						stUserConsign.TryCount = 0;

						if(SUPER_CARD_ID == gstKeyboard.iCardUID)
						{
							GUI_ShowMessage("Super Card !!", LCD_Color_Red);
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);	
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
							gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
							return;
						}
						
						//检查下是不是管理卡
						for(i=0; i<MAX_AdminCardNum; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stLockers.AdminCard[i])
							{
								GUI_LockersNotice(LOCKERS_AdminCard);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
								return;
							}
						}

						//先检查下此卡是否已存放物品
						for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stBox[i].iUserCardID)
							{
								GUI_ShowIDPosition(0, i);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
								return;									
							}
						}

						Buzzer(200);
						gstKeyboard.u8SecondMenuStep = LockersStep_OpenBox;
					}
					else
					{
						stUserConsign.iTimeout += 300;
						if(stUserConsign.iTimeout > 8000)  //超时不刷卡自动退出(gstKeyboard.bAddInto = 1, 因此自动取消键退出无效)
						{
							stUserConsign.iTimeout = 0;
							gstKeyboard.u8SecondMenuStep = 0;
							gstKeyboard.u8AdminMenuStep = 0;
						}

					}
				}
			}
			break;

			case LockersStep_OpenBox:  
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8Box = gstSysRunInfo.EmptyBox[0];

				GUI_LockersNotice(LOCKERS_OpeningBox);
				ret = SelectOneBoxToOpen(gstKeyboard.u8Box, SysConfig.stLockers.bCheckBoxDoorStatus);;			
				if(ret)  //开箱失败
				{
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					GUI_UserConsignFail(gstKeyboard.u8Box);	
					Voice_Play(YY_MenGuZhanQingJiCun);
											
					//锁住此箱										
					SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

					//更新空箱列表
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}
				else
				{	
					//开启成功
					GUI_Open1Box(gstKeyboard.u8Box, 1);
					VoicePlay_xBoxOpen(gstKeyboard.u8Box);
					Voice_Play(YY_CunWuTiShi);
					gstSysRunInfo.u8UseBox = gstKeyboard.u8Box;
					gstSysRunInfo.iHandleTimeout = SysConfig.stLockers.iHandleTimeout;
					
					//记录此箱记录的卡号和密码/更改箱状态
					SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = gstKeyboard.iCardUID;
					SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_OCCUPY;										
					memcpy((uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].LastUsedTime[0], (uchar8*)gstSysRunInfo.TimeBuf, 6);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));
					
					//保存一笔开箱取件记录
					StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 1);

					//更新空箱列表
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}	
				stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
				gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
				gstKeyboard.bAddInto = 1;							
			}					
			break;

			case LockersStep_WaitCardAway:  
			{	
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > 220)
				{
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					iCardID01 = Get_IC_ID_CardSN();
					if((0 == iCardID01)||(iCardID01 != gstKeyboard.iCardUID))
					{
						stUserConsign.TryCount ++;
						if(stUserConsign.TryCount >= 3)
						{
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms - (NOTICE_STAY_TIME /2);
							gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
							gstKeyboard.bAddInto = 1;
						}
					}
				}
			}
			break;		

			case LockersStep_Notice:  
			{
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > NOTICE_STAY_TIME)
				{
					gstKeyboard.u8SecondMenuStep = 0;
					gstKeyboard.u8AdminMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8SecondMenuStep = 0;
			}
			break;
		}
	}			
}

/***********************************************************************************************
* Function Name  : ConsignMode_PreAuthorization
* Description	 : 用户刷卡寄存--预授权卡模式
* Input 		 : None
* Output		 : None
* Return		 : None 
***********************************************************************************************/
void ConsignMode_PreAuthorization(void)
{
	int i = 0;
	uchar8 ret = 0;
	uint32 iAddr = 0;
	uint32 iCardID01 = 0;
	uchar8 OpenBoxCnt = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8AdminMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8SecondMenuStep)
		{
			case LockersStep_Start: //按下寄存键后检查寄存条件
			{				
				//系统过期被锁,禁用寄存功能
				if(gstSysRunInfo.bLockedSys)
				{
					GUI_LockersNotice(LOCKERS_SysError);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);

					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					return;
				}

				//检查箱门是否关好
				if(SysConfig.stLockers.bCheckBoxDoorStatus)
				{
					gstKeyboard.u8Box = 0;
					for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
					{
						if((1 == gstSysRunInfo.BoxDoorStatus[i])&&(LSTATUS_LOCKING != SysConfig.stBox[i].bLocking))
						{
							if(0 == gstKeyboard.u8Box)
							{	
								gstKeyboard.u8Box = i;
							}
							OpenBoxCnt ++;
						}
					}
					if(OpenBoxCnt > SysConfig.stLockers.u8AllowBeOpenedBoxNum)
					{
						GUI_PlsCloseBoxDoor(gstKeyboard.u8Box);
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						VoicePlay_DoorNotClose(gstKeyboard.u8Box);

						stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
						gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
						return;
					}
				}
				
				//检查有没有空箱	
				if((gstSysRunInfo.EmptyBox[0]>0)&&(gstSysRunInfo.EmptyBox[0] <= SysConfig.stLockers.u8BoxCount))
				{	
					GUI_LockersNotice(LOCKERS_PlsCard);
					Voice_Play(YY_PlsCard);
					gstKeyboard.u8SecondMenuStep = LockersStep_ReadCard;
				}
				else
				{
					//提示箱已满								
					GUI_LockersNotice(LOCKERS_IsFull);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
				}				
			}
			break;	

			case LockersStep_ReadCard:  
			{
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > 300)
				{
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.iCardUID = Get_IC_ID_CardSN();
					if(gstKeyboard.iCardUID)
					{	
						if(SUPER_CARD_ID == gstKeyboard.iCardUID)
						{
							GUI_ShowMessage("Super Card !!", LCD_Color_Red);
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);	
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
							gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
							return;
						}

						stUserConsign.TryCount = 0;
						//检查下是不是管理卡
						for(i=0; i<MAX_AdminCardNum; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stLockers.AdminCard[i])
							{
								GUI_LockersNotice(LOCKERS_AdminCard);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
								return;
							}
						}

						//先检查下此卡是否已存放物品
						for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stBox[i].iUserCardID)
							{
								GUI_ShowIDPosition(0, i);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
								return;									
							}
						}

						//检查是不是预授权卡
						GUI_ShowNotice(NOTICE_PLS_WAIT);
						delay_ms(500);
						if(CheckCardValid(gstKeyboard.iCardUID) >= 0)
						{
							Buzzer(200);
							gstKeyboard.u8SecondMenuStep = LockersStep_OpenBox;
						}
						else
						{
							GUI_LockersNotice(LOCKERS_CardUnauthorized);
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
							gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;							
						}
					}
					else
					{
						stUserConsign.iTimeout += 300;
						if(stUserConsign.iTimeout > 8000)  //超时不刷卡自动退出(gstKeyboard.bAddInto = 1, 因此自动取消键退出无效)
						{
							stUserConsign.iTimeout = 0;
							gstKeyboard.u8SecondMenuStep = 0;
							gstKeyboard.u8AdminMenuStep = 0;
						}

					}
				}
			}
			break;

			case LockersStep_OpenBox:  
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8Box = gstSysRunInfo.EmptyBox[0];

				GUI_LockersNotice(LOCKERS_OpeningBox);
				ret = SelectOneBoxToOpen(gstKeyboard.u8Box, SysConfig.stLockers.bCheckBoxDoorStatus);;			
				if(ret)  //开箱失败
				{
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					GUI_UserConsignFail(gstKeyboard.u8Box);	
					Voice_Play(YY_MenGuZhanQingJiCun);
											
					//锁住此箱										
					SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

					//更新空箱列表
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}
				else
				{	
					//开启成功
					GUI_Open1Box(gstKeyboard.u8Box, 1);
					VoicePlay_xBoxOpen(gstKeyboard.u8Box);
					Voice_Play(YY_CunWuTiShi);
					gstSysRunInfo.u8UseBox = gstKeyboard.u8Box;
					gstSysRunInfo.iHandleTimeout = SysConfig.stLockers.iHandleTimeout;
					
					//记录此箱记录的卡号和密码/更改箱状态
					SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = gstKeyboard.iCardUID;
					SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_OCCUPY;										
					memcpy((uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].LastUsedTime[0], (uchar8*)gstSysRunInfo.TimeBuf, 6);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));
					
					//保存一笔开箱取件记录
					StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 1);

					//更新空箱列表
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}	
				stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
				gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
				gstKeyboard.bAddInto = 1;							
			}					
			break;

			case LockersStep_WaitCardAway:  
			{	
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > 220)
				{
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					iCardID01 = Get_IC_ID_CardSN();
					if((0 == iCardID01)||(iCardID01 != gstKeyboard.iCardUID))
					{
						stUserConsign.TryCount ++;
						if(stUserConsign.TryCount >= 3)
						{
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms - (NOTICE_STAY_TIME /2);
							gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
							gstKeyboard.bAddInto = 1;
						}
					}
				}
			}
			break;		

			case LockersStep_Notice:  
			{
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > NOTICE_STAY_TIME)
				{
					gstKeyboard.u8SecondMenuStep = 0;
					gstKeyboard.u8AdminMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8SecondMenuStep = 0;
			}
			break;
		}
	}			
}

/***********************************************************************************************
* Function Name  : ConsignMode_WriteCard
* Description	 : 用户刷卡寄存--写卡模式
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 会将寄存信息写入卡扇区中
***********************************************************************************************/
void ConsignMode_WriteCard(void)
{
	int i = 0;
	uchar8 ret = 0;
	uint32 iAddr = 0;
	uint32 iCardID01 = 0;
	uchar8 OpenBoxCnt = 0;
	uint16 i16Temp = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8AdminMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8SecondMenuStep)
		{
			case LockersStep_Start: //按下寄存键后检查寄存条件
			{				
				//系统过期被锁,禁用寄存功能
				if(gstSysRunInfo.bLockedSys)
				{
					GUI_LockersNotice(LOCKERS_SysError);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);

					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					return;
				}

				//检查箱门是否关好
				if(SysConfig.stLockers.bCheckBoxDoorStatus)
				{
					gstKeyboard.u8Box = 0;
					for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
					{
						if((1 == gstSysRunInfo.BoxDoorStatus[i])&&(LSTATUS_LOCKING != SysConfig.stBox[i].bLocking))
						{
							if(0 == gstKeyboard.u8Box)
							{	
								gstKeyboard.u8Box = i;
							}
							OpenBoxCnt ++;
						}
					}
					if(OpenBoxCnt > SysConfig.stLockers.u8AllowBeOpenedBoxNum)
					{
						GUI_PlsCloseBoxDoor(gstKeyboard.u8Box);
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						VoicePlay_DoorNotClose(gstKeyboard.u8Box);

						stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
						gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
						return;
					}
				}
				
				//检查有没有空箱	
				if((gstSysRunInfo.EmptyBox[0]>0)&&(gstSysRunInfo.EmptyBox[0] <= SysConfig.stLockers.u8BoxCount))
				{	
					GUI_LockersNotice(LOCKERS_PlsCard);
					Voice_Play(YY_PlsCard);
					gstKeyboard.u8SecondMenuStep = LockersStep_ReadCard;
				}
				else
				{
					//提示箱已满								
					GUI_LockersNotice(LOCKERS_IsFull);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
				}				
			}
			break;	

			case LockersStep_ReadCard:  
			{
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > 300)
				{
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.iCardUID = Get_IC_ID_CardSN();
					if(gstKeyboard.iCardUID)
					{	
						if(SUPER_CARD_ID == gstKeyboard.iCardUID)
						{
							GUI_ShowMessage("Super Card !!", LCD_Color_Red);
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);	
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
							gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
							return;
						}

						stUserConsign.TryCount = 0;
						//检查下是不是管理卡
						for(i=0; i<MAX_AdminCardNum; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stLockers.AdminCard[i])
							{
								GUI_LockersNotice(LOCKERS_AdminCard);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
								return;
							}
						}
						
						//先检查下此卡是否已存放物品
						for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stBox[i].iUserCardID)
							{
								GUI_ShowIDPosition(0, i);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
								return;									
							}
						}
						
						//读取卡扇区数据
						memset(UserCardData, 0x0, sizeof(UserCardData));
						ret = RF_ReadCard((uchar8*)&gstKeyboard.iCardUID, SysConfig.stCardKey.u8Sector, AuthKey_A, 
							              (uchar8*)SysConfig.stCardKey.Password, UserCardData);
						if(0 == ret)
						{
							memset((uchar8*)&stUserCard, 0x0, sizeof(ST_USER_CARD));
							iAddr = 0;
							memcpy((uchar8*)&stUserCard.SysCode[0], &UserCardData[iAddr], SYS_ID_CODE_LEN);
							iAddr += SYS_ID_CODE_LEN;
							i16Temp = UserCardData[iAddr]*256 + UserCardData[iAddr +1];
							stUserCard.ValidDate[0] = (i16Temp >> 9) & 0xFF;
							stUserCard.ValidDate[1] = ((i16Temp & 0x01E0)>>5) & 0xFF;
							stUserCard.ValidDate[2] = (i16Temp & 0x1F);	
							
							iAddr += 2;
							memcpy((uchar8*)&stUserCard.iEnableStartLockers, &UserCardData[iAddr], 4);
							iAddr += 4;
							memcpy((uchar8*)&stUserCard.iEnableEndLockers, &UserCardData[iAddr], 4);
							iAddr += 4;
							
							memcpy((uchar8*)&stUserCard.JobID[0], &UserCardData[iAddr], CARD_JobIdLen);
							iAddr += CARD_JobIdLen;
							memcpy((uchar8*)&stUserCard.Name[0], &UserCardData[iAddr], CARD_NameLen);
							iAddr += CARD_NameLen;

							//解析存取信息
							memcpy((uchar8*)&stUserCard.OnLocker[0], &UserCardData[iAddr], MAX_NameLen);
							iAddr += MAX_NameLen;
							stUserCard.u8OnBox = UserCardData[iAddr++];
							stUserCard.bStoreOrTake = UserCardData[iAddr++];
							memcpy((uchar8*)&stUserCard.OnTime[0], &UserCardData[iAddr], 6);
							iAddr += 6;

							if(memcmp((uchar8*)&stUserCard.SysCode[0], (uchar8*)&SysConfig.stClientInfo.IdCode[0], SYS_ID_CODE_LEN))
							{
								GUI_LockersNotice(LOCKERS_NonSysCard);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;	
								return;								
							}
							
							//检查是否已在别的柜子中存放物品
							if((USER_STORE == stUserCard.bStoreOrTake)
								&&(memcmp((uchar8*)&stUserCard.OnLocker[0], (uchar8*)&SysConfig.stLockers.Name[0], MAX_NameLen)))
							{
								GUI_PackageInOtherLockers((char*)stUserCard.OnLocker, stUserCard.u8OnBox);	
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;	
								return;												
							}

							//检查寄存条件
							if(CheckValidityDate((uchar8*)stUserCard.ValidDate) < 0)
							{
								GUI_LockersNotice(LOCKERS_Overdue);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;	
								return;												
							}
							else if((stUserCard.iEnableStartLockers > SysConfig.stHardware.u32MachineNum)||(stUserCard.iEnableEndLockers < SysConfig.stHardware.u32MachineNum))
							{
								GUI_LockersNotice(LOCKERS_LimitOfUse);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;	
								return;												
							}
							else
							{
								gstKeyboard.u8Box = gstSysRunInfo.EmptyBox[0];
								//符合寄存条件--在指定扇区写寄存信息
								memset(UserCardData, 0x0, sizeof(UserCardData));
								iAddr = 0;
								memcpy(&UserCardData[iAddr], (uchar8*)&SysConfig.stLockers.Name[0], MAX_NameLen);
								iAddr += MAX_NameLen;
								UserCardData[iAddr++] = gstKeyboard.u8Box;
							    UserCardData[iAddr++] = USER_STORE;
								memcpy(&UserCardData[iAddr], (uchar8*)&gstSysRunInfo.TimeBuf[0], 6);
								iAddr += 6;
								iAddr += 1;
								UserCardData[iAddr++] = Calc_CRC(&UserCardData[0], 15);

								//写卡
								ret = RF_WriteCard_1Block((uchar8*)&gstKeyboard.iCardUID, (SysConfig.stCardKey.u8Sector *4 +2), 
								                          AuthKey_A, (uchar8*)SysConfig.stCardKey.Password, UserCardData);
								if(0 == ret)
								{
									Buzzer(200);
									gstKeyboard.u8SecondMenuStep = LockersStep_OpenBox;
								}
								else
								{
									GUI_LockersNotice(LOCKERS_WriteCardFaild);
									Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
									stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
									gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;	
									return;										
								}
							}
						}
						else
						{
							GUI_LockersNotice(LOCKERS_CardUnauthorized);
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
							gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;	
							return;								
						}							
					}
					else
					{
						stUserConsign.iTimeout += 300;
						if(stUserConsign.iTimeout > 8000)  //超时不刷卡自动退出(gstKeyboard.bAddInto = 1, 因此自动取消键退出无效)
						{
							stUserConsign.iTimeout = 0;
							gstKeyboard.u8SecondMenuStep = 0;
							gstKeyboard.u8AdminMenuStep = 0;
						}

					}
				}
			}
			break;

			case LockersStep_OpenBox:  
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8Box = gstSysRunInfo.EmptyBox[0];

				GUI_LockersNotice(LOCKERS_OpeningBox);
				ret = SelectOneBoxToOpen(gstKeyboard.u8Box, SysConfig.stLockers.bCheckBoxDoorStatus);;			
				if(ret)  //开箱失败
				{
					//提示再刷卡--清除卡记录的寄存信息
					Voice_Play(YY_PlsCard);					
					GUI_LockersNotice(LOCKERS_ClrCardStoreInfo);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 100);
					for(i=0; i<100; i++)
					{
						iCardID01 = Get_IC_ID_CardSN();
						if(iCardID01)
						{
							memset(UserCardData, 0x0, sizeof(UserCardData));
							iAddr = 0;
							memcpy(&UserCardData[iAddr], (uchar8*)&SysConfig.stLockers.Name[0], MAX_NameLen);
							iAddr += MAX_NameLen;
							UserCardData[iAddr++] = gstKeyboard.u8Box;
						    UserCardData[iAddr++] = USER_STORE_FAILD;
							memcpy(&UserCardData[iAddr], (uchar8*)&gstSysRunInfo.TimeBuf[0], 6);
							iAddr += 6;
							iAddr += 1;
							UserCardData[iAddr++] = Calc_CRC(&UserCardData[0], 15);

							//写卡
							ret = RF_WriteCard_1Block((uchar8*)&gstKeyboard.iCardUID, (SysConfig.stCardKey.u8Sector *4 +2), 
							                          AuthKey_A, (uchar8*)SysConfig.stCardKey.Password, UserCardData);
							break;
						}
						delay_ms(100);
						if(50 == i)
						{
							Voice_Play(YY_PlsCard);	
						}
					}

					//提示开箱失败,请重新寄存
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					GUI_UserConsignFail(gstKeyboard.u8Box);	
					Voice_Play(YY_MenGuZhanQingJiCun);
											
					//锁住此箱										
					SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

					//更新空箱列表
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}
				else
				{	
					//开启成功
					GUI_Open1Box(gstKeyboard.u8Box, 1);
					VoicePlay_xBoxOpen(gstKeyboard.u8Box);
					Voice_Play(YY_CunWuTiShi);
					gstSysRunInfo.u8UseBox = gstKeyboard.u8Box;
					gstSysRunInfo.iHandleTimeout = SysConfig.stLockers.iHandleTimeout;
					
					//记录此箱记录的卡号和密码/更改箱状态
					SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = gstKeyboard.iCardUID;
					SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_OCCUPY;										
					memcpy((uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].LastUsedTime[0], (uchar8*)gstSysRunInfo.TimeBuf, 6);

					//姓名和工号
					memcpy((uchar8*)SysConfig.stBox[gstKeyboard.u8Box].UserName, (uchar8*)stUserCard.Name, MAX_UserNameLen);
					memcpy((uchar8*)SysConfig.stBox[gstKeyboard.u8Box].UserID, (uchar8*)stUserCard.JobID, CARD_JobIdLen);
					
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));
					
					//保存一笔开箱取件记录
					StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 1);

					//更新空箱列表
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}	
				stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
				gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
				gstKeyboard.bAddInto = 1;							
			}					
			break;

			case LockersStep_WaitCardAway:  
			{	
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > 220)
				{
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					iCardID01 = Get_IC_ID_CardSN();
					if((0 == iCardID01)||(iCardID01 != gstKeyboard.iCardUID))
					{
						stUserConsign.TryCount ++;
						if(stUserConsign.TryCount >= 3)
						{
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms - (NOTICE_STAY_TIME /2);
							gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
							gstKeyboard.bAddInto = 1;
						}
					}
				}
			}
			break;		

			case LockersStep_Notice:  
			{
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > NOTICE_STAY_TIME)
				{
					gstKeyboard.u8SecondMenuStep = 0;
					gstKeyboard.u8AdminMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8SecondMenuStep = 0;
			}
			break;
		}
	}			
}
	
/***********************************************************************************************
* Function Name  : ConsignMode_OnlineAsk
* Description	 : 用户刷卡寄存--在线验证模式
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 刷卡后分配空箱
***********************************************************************************************/
void ConsignMode_OnlineAsk(void)
{
	int i = 0;
	uchar8 ret = 0;
	uint32 iAddr = 0;
	uint32 iCardID01 = 0;
	uchar8 OpenBoxCnt = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8AdminMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8SecondMenuStep)
		{
			case LockersStep_Start: //按下寄存键后检查寄存条件
			{	
				memset((uchar8*)&stOnlineAsk, 0x0, sizeof(ST_ONLINEE_ASK));
				
				//系统过期被锁,禁用寄存功能
				if(gstSysRunInfo.bLockedSys)
				{
					GUI_LockersNotice(LOCKERS_SysError);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);

					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					return;
				}

				//检查箱门是否关好
				if(SysConfig.stLockers.bCheckBoxDoorStatus)
				{
					gstKeyboard.u8Box = 0;
					for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
					{
						if((1 == gstSysRunInfo.BoxDoorStatus[i])&&(LSTATUS_LOCKING != SysConfig.stBox[i].bLocking))
						{
							if(0 == gstKeyboard.u8Box)
							{	
								gstKeyboard.u8Box = i;
							}
							OpenBoxCnt ++;
						}
					}
					if(OpenBoxCnt > SysConfig.stLockers.u8AllowBeOpenedBoxNum)
					{
						GUI_PlsCloseBoxDoor(gstKeyboard.u8Box);
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						VoicePlay_DoorNotClose(gstKeyboard.u8Box);

						stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
						gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
						return;
					}
				}
				
				//检查有没有空箱	
				if((gstSysRunInfo.EmptyBox[0]>0)&&(gstSysRunInfo.EmptyBox[0] <= SysConfig.stLockers.u8BoxCount))
				{	
					GUI_LockersNotice(LOCKERS_PlsCard);
					Voice_Play(YY_PlsCard);
					gstKeyboard.u8SecondMenuStep = LockersStep_ReadCard;
				}
				else
				{
					//提示箱已满								
					GUI_LockersNotice(LOCKERS_IsFull);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
				}				
			}
			break;	

			case LockersStep_ReadCard:  
			{
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > 300)
				{
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.iCardUID = Get_IC_ID_CardSN();
					if(gstKeyboard.iCardUID)
					{	
						if(SUPER_CARD_ID == gstKeyboard.iCardUID)
						{
							GUI_ShowMessage("Super Card !!", LCD_Color_Red);
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);	
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
							gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
							return;
						}

						stUserConsign.TryCount = 0;
						//检查下是不是管理卡
						for(i=0; i<MAX_AdminCardNum; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stLockers.AdminCard[i])
							{
								GUI_LockersNotice(LOCKERS_AdminCard);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
								return;
							}
						}

						//先检查下此卡是否已存放物品
						for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stBox[i].iUserCardID)
							{
								GUI_ShowIDPosition(0, i);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
								return;									
							}
						}

						//检查是不是预授权卡
						Buzzer(350);
						GUI_ShowNotice(NOTICE_PLS_WAIT);
						gstKeyboard.u8SecondMenuStep = LockersStep_OnlineAsk;
					}
					else
					{
						stUserConsign.iTimeout += 300;
						if(stUserConsign.iTimeout > 8000)  //超时不刷卡自动退出(gstKeyboard.bAddInto = 1, 因此自动取消键退出无效)
						{
							stUserConsign.iTimeout = 0;
							gstKeyboard.u8SecondMenuStep = 0;
							gstKeyboard.u8AdminMenuStep = 0;
						}

					}
				}
			}
			break;

			case LockersStep_OnlineAsk: 
			{
				stOnlineAsk.bReplyOk = 0;
				stOnlineAsk.iCardID = gstKeyboard.iCardUID;
				stOnlineAsk.iWaitingTime = gstSysRunInfo.u32TimeCount_ms;
				stOnlineAsk.u8OnBox = gstSysRunInfo.EmptyBox[0];
				UserConsign_OnlineAsk(stOnlineAsk.iCardID, stOnlineAsk.u8OnBox);
				
			    gstKeyboard.u8SecondMenuStep = LockersStep_WaitServerReply;
			}
			break;

			case LockersStep_WaitServerReply:
			{
				if(gstSysRunInfo.u32TimeCount_ms - stOnlineAsk.iWaitingTime < WAITING_TIMEOUT)
				{
					if(SERVER_REPLY_MASK == stOnlineAsk.bReplyOk)
					{
						if(ALLOW_CONSIGN == stOnlineAsk.bAllowConsign)
						{
						    gstKeyboard.u8SecondMenuStep = LockersStep_OpenBox;
						}
						else if(NOTALLOW_CONSIGN == stOnlineAsk.bAllowConsign)
						{
							GUI_PackageInOtherLockers((char*)stOnlineAsk.OnLockers, stOnlineAsk.u8OnBox);	
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
							gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;	
							return;												
						}
						else
						{
							//memset((char*)stOnlineAsk.Notice, 0x0, sizeof(stOnlineAsk.Notice));
							//strcpy((char*)stOnlineAsk.Notice, "该卡片状态异常!");
							GUI_ShowMessage((char*)stOnlineAsk.Notice, LCD_Color_Red);	
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
							gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;	
							return;												
						}
					}
				}
				else
				{
					stOnlineAsk.u8TryCnt ++;
					if(stOnlineAsk.u8TryCnt < 3)
					{
						//未收到服务器应答,再次尝试	
						gstKeyboard.u8SecondMenuStep = LockersStep_OnlineAsk;
					}
					else
					{
						//多次申请无应答 
						//gstKeyboard.u8SecondMenuStep = LockersStep_OpenBox;
						GUI_LockersNotice(LOCKERS_NetError);
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
						gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					}
				}
			}
			break;
			
			case LockersStep_OpenBox:  
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8Box = gstSysRunInfo.EmptyBox[0];

				GUI_LockersNotice(LOCKERS_OpeningBox);
				ret = SelectOneBoxToOpen(gstKeyboard.u8Box, SysConfig.stLockers.bCheckBoxDoorStatus);;			
				if(ret)  //开箱失败
				{
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					GUI_UserConsignFail(gstKeyboard.u8Box);	
					Voice_Play(YY_MenGuZhanQingJiCun);
											
					//锁住此箱										
					SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

					//更新空箱列表
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}
				else
				{	
					//开启成功
					GUI_Open1Box(gstKeyboard.u8Box, 1);
					VoicePlay_xBoxOpen(gstKeyboard.u8Box);
					Voice_Play(YY_CunWuTiShi);
					gstSysRunInfo.u8UseBox = gstKeyboard.u8Box;
					gstSysRunInfo.iHandleTimeout = SysConfig.stLockers.iHandleTimeout;
					
					//记录此箱记录的卡号和密码/更改箱状态
					SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = gstKeyboard.iCardUID;
					SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_OCCUPY;										
					memcpy((uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].LastUsedTime[0], (uchar8*)gstSysRunInfo.TimeBuf, 6);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));
					
					//保存一笔开箱取件记录
					StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 1);

					//更新空箱列表
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}	
				stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
				gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
				gstKeyboard.bAddInto = 1;							
			}					
			break;

			case LockersStep_WaitCardAway:  
			{	
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > 220)
				{
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					iCardID01 = Get_IC_ID_CardSN();
					if((0 == iCardID01)||(iCardID01 != gstKeyboard.iCardUID))
					{
						stUserConsign.TryCount ++;
						if(stUserConsign.TryCount >= 3)
						{
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms - (NOTICE_STAY_TIME /2);
							gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
							gstKeyboard.bAddInto = 1;
						}
					}
				}
			}
			break;		

			case LockersStep_Notice:  
			{
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > NOTICE_STAY_TIME)
				{
					gstKeyboard.u8SecondMenuStep = 0;
					gstKeyboard.u8AdminMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8SecondMenuStep = 0;
			}
			break;
		}
	}			
}


/***********************************************************************************************
* Function Name  : ConsignMode_AnyFinger
* Description	 : 用户刷卡寄存--任意指纹 
* Input 		 : None
* Output		 : None
* Return		 : None 
***********************************************************************************************/
void ConsignMode_AnyFinger(void)
{
	int i = 0;
	uchar8 ret = 0;
	uint32 iAddr = 0;
	uchar8 OpenBoxCnt = 0;
	uint32 iRet = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8AdminMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8SecondMenuStep)
		{
			case LockersStep_Start: //按下寄存键后检查寄存条件
			{				
				//系统过期被锁,禁用寄存功能
				if(gstSysRunInfo.bLockedSys)
				{
					GUI_LockersNotice(LOCKERS_SysError);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);

					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					return;
				}

				//检查箱门是否关好
				if(SysConfig.stLockers.bCheckBoxDoorStatus)
				{
					gstKeyboard.u8Box = 0;
					for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
					{
						if((1 == gstSysRunInfo.BoxDoorStatus[i])&&(LSTATUS_LOCKING != SysConfig.stBox[i].bLocking))
						{
							if(0 == gstKeyboard.u8Box)
							{	
								gstKeyboard.u8Box = i;
							}
							OpenBoxCnt ++;
						}
					}
					if(OpenBoxCnt > SysConfig.stLockers.u8AllowBeOpenedBoxNum)
					{
						GUI_PlsCloseBoxDoor(gstKeyboard.u8Box);
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						VoicePlay_DoorNotClose(gstKeyboard.u8Box);

						stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
						gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
						return;
					}
				}
				
				//检查有没有空箱	
				if((gstSysRunInfo.EmptyBox[0]>0)&&(gstSysRunInfo.EmptyBox[0] <= SysConfig.stLockers.u8BoxCount))
				{	
					GUI_LockersNotice(LOCKERS_PlsFingerOrCard);
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_GetFingerprint;
				}
				else
				{
					//提示箱已满								
					GUI_LockersNotice(LOCKERS_IsFull);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
				}				
			}
			break;	

			case LockersStep_GetFingerprint:  
			{
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > 11000)
				{
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					//读取指纹超时
					Fingerprint_Login(ZW_StopHandle);
					GUI_LockersNotice(LOCKERS_WaitTimeout);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;	
				}
				else
				{
					iRet = Fingerprint_Login(gstSysRunInfo.EmptyBox[0]);
					if(0xABCD == iRet)
					{
						stUserConsign.TryCount = 0;
						gstKeyboard.u8SecondMenuStep = LockersStep_OpenBox;
					}
					else if(iRet >= 0xE000)
					{
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);	
						if(0xF000==iRet)
						{
							Voice_Play(YY_ZhiWenYiShouQuan);
						}
						else
						{
							Voice_Play(YY_ZhiWenShouQuanShiBai);
						}
						Fingerprint_Login(0xFFFF);
						stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
						gstKeyboard.u8SecondMenuStep = LockersStep_Notice;	
					}					
				}
			}
			break;

			case LockersStep_OpenBox:  
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8Box = gstSysRunInfo.EmptyBox[0];

				GUI_LockersNotice(LOCKERS_OpeningBox);
				ret = SelectOneBoxToOpen(gstKeyboard.u8Box, SysConfig.stLockers.bCheckBoxDoorStatus);;			
				if(ret)  //开箱失败
				{
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					GUI_UserConsignFail(gstKeyboard.u8Box);	
					Voice_Play(YY_MenGuZhanQingJiCun);
											
					//锁住此箱										
					SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

					//清除指纹
					SendCmdToFingerprint_DeletChar(gstKeyboard.u8Box, 1);

					//更新空箱列表
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}
				else
				{	
					//开启成功
					GUI_Open1Box(gstKeyboard.u8Box, 1);
					VoicePlay_xBoxOpen(gstKeyboard.u8Box);
					Voice_Play(YY_CunWuTiShi);
					gstSysRunInfo.u8UseBox = gstKeyboard.u8Box;
					gstSysRunInfo.iHandleTimeout = SysConfig.stLockers.iHandleTimeout;
					
					//记录此箱记录的卡号和密码/更改箱状态
					SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = gstKeyboard.iCardUID;
					SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_OCCUPY;	
					SysConfig.stBox[gstKeyboard.u8Box].iTouchID = gstKeyboard.u8Box; 
					memcpy((uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].LastUsedTime[0], (uchar8*)gstSysRunInfo.TimeBuf, 6);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));
					
					//保存一笔开箱取件记录
					if(gstKeyboard.iCardUID)
					{
						StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 1);
					}
					else
					{
						StoreRecord_OpenBoxByFinger(gstKeyboard.u8Box, 1);
					}

					//更新空箱列表
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}	
				stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
				gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
				gstKeyboard.bAddInto = 1;							
			}					
			break;	

			case LockersStep_Notice:  
			{
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > NOTICE_STAY_TIME)
				{
					if(0x00 == GET_Fingerprint_STATE)
					{
						delay_ms(NOTICE_STAY_TIME);
					}
					gstKeyboard.u8SecondMenuStep = 0;
					gstKeyboard.u8AdminMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8SecondMenuStep = 0;
			}
			break;
		}
	}			
}

/***********************************************************************************************
* Function Name  : ConsignMode_AnyFingerOrCard
* Description	 : 用户刷卡寄存--任意指纹或卡模式
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 刷卡后分配空箱
***********************************************************************************************/
void ConsignMode_AnyFingerOrCard(void)
{
	int i = 0;
	uchar8 ret = 0;
	uint32 iAddr = 0;
	uint32 iCardID01 = 0;
	uchar8 OpenBoxCnt = 0;
	uint32 iRet = 0;
	
	if(Key_Cancel == gstKeyboard.u8KeyValue)
	{	
		gstKeyboard.u8SecondMenuStep = 0;
		gstKeyboard.u8AdminMenuStep = 0;
		gstKeyboard.bAddInto = 1;
	}
	else
	{
		switch(gstKeyboard.u8SecondMenuStep)
		{
			case LockersStep_Start: //按下寄存键后检查寄存条件
			{				
				//系统过期被锁,禁用寄存功能
				if(gstSysRunInfo.bLockedSys)
				{
					GUI_LockersNotice(LOCKERS_SysError);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);

					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					return;
				}

				//检查箱门是否关好
				if(SysConfig.stLockers.bCheckBoxDoorStatus)
				{
					gstKeyboard.u8Box = 0;
					for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
					{
						if((1 == gstSysRunInfo.BoxDoorStatus[i])&&(LSTATUS_LOCKING != SysConfig.stBox[i].bLocking))
						{
							if(0 == gstKeyboard.u8Box)
							{	
								gstKeyboard.u8Box = i;
							}
							OpenBoxCnt ++;
						}
					}
					if(OpenBoxCnt > SysConfig.stLockers.u8AllowBeOpenedBoxNum)
					{
						GUI_PlsCloseBoxDoor(gstKeyboard.u8Box);
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						VoicePlay_DoorNotClose(gstKeyboard.u8Box);

						stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
						gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
						return;
					}
				}
				
				//检查有没有空箱	
				if((gstSysRunInfo.EmptyBox[0]>0)&&(gstSysRunInfo.EmptyBox[0] <= SysConfig.stLockers.u8BoxCount))
				{	
					GUI_LockersNotice(LOCKERS_PlsFingerOrCard);
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_GetFingerprint;
				}
				else
				{
					//提示箱已满								
					GUI_LockersNotice(LOCKERS_IsFull);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
				}				
			}
			break;	

			case LockersStep_GetFingerprint:  
			{
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > 11000)
				{
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					//读取指纹超时
					Fingerprint_Login(ZW_StopHandle);
					GUI_LockersNotice(LOCKERS_WaitTimeout);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;	
				}
				else
				{
					iRet = Fingerprint_Login(gstSysRunInfo.EmptyBox[0]);
					if(0xABCD == iRet)
					{
						stUserConsign.TryCount = 0;
						gstKeyboard.u8SecondMenuStep = LockersStep_OpenBox;
					}
					else if(iRet >= 0xE000)
					{
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);	
						if(0xF000==iRet)
						{
							Voice_Play(YY_ZhiWenYiShouQuan);
						}
						else
						{
							Voice_Play(YY_ZhiWenShouQuanShiBai);
						}
						Fingerprint_Login(0xFFFF);
						stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
						gstKeyboard.u8SecondMenuStep = LockersStep_Notice;	
					}

					//读卡
					if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.uiReadCardTime > 300)
					{
						stUserConsign.uiReadCardTime = gstSysRunInfo.u32TimeCount_ms;
						gstKeyboard.iCardUID = Get_IC_ID_CardSN();
						if(gstKeyboard.iCardUID)
						{	
							stUserConsign.TryCount = 0;

							if(SUPER_CARD_ID == gstKeyboard.iCardUID)
							{
								GUI_ShowMessage("Super Card !!", LCD_Color_Red);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);	
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
								return;
							}
							
							//检查下是不是管理卡
							for(i=0; i<MAX_AdminCardNum; i++)
							{
								WDT_Feed();
								if(gstKeyboard.iCardUID == SysConfig.stLockers.AdminCard[i])
								{
									GUI_LockersNotice(LOCKERS_AdminCard);
									Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
									stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
									gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
									return;
								}
							}

							//先检查下此卡是否已存放物品
							for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
							{
								WDT_Feed();
								if(gstKeyboard.iCardUID == SysConfig.stBox[i].iUserCardID)
								{
									GUI_ShowIDPosition(0, i);
									Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
									stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
									gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
									return;									
								}
							}
							Buzzer(200);
							gstKeyboard.u8SecondMenuStep = LockersStep_OpenBox;
						}
					}
				}
			}
			break;

			case LockersStep_OpenBox:  
			{
				gstKeyboard.bAddInto = 0;
				gstKeyboard.u8Box = gstSysRunInfo.EmptyBox[0];

				GUI_LockersNotice(LOCKERS_OpeningBox);
				ret = SelectOneBoxToOpen(gstKeyboard.u8Box, SysConfig.stLockers.bCheckBoxDoorStatus);;			
				if(ret)  //开箱失败
				{
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					GUI_UserConsignFail(gstKeyboard.u8Box);	
					Voice_Play(YY_MenGuZhanQingJiCun);
											
					//锁住此箱										
					SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

					//清除指纹
					SendCmdToFingerprint_DeletChar(gstKeyboard.u8Box, 1);

					//更新空箱列表
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}
				else
				{	
					//开启成功
					GUI_Open1Box(gstKeyboard.u8Box, 1);
					VoicePlay_xBoxOpen(gstKeyboard.u8Box);
					Voice_Play(YY_CunWuTiShi);
					gstSysRunInfo.u8UseBox = gstKeyboard.u8Box;
					gstSysRunInfo.iHandleTimeout = SysConfig.stLockers.iHandleTimeout;
					
					//记录此箱记录的卡号和密码/更改箱状态
					SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = gstKeyboard.iCardUID;
					SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_OCCUPY;	
					SysConfig.stBox[gstKeyboard.u8Box].iTouchID = gstKeyboard.u8Box; 
					memcpy((uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].LastUsedTime[0], (uchar8*)gstSysRunInfo.TimeBuf, 6);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));
					
					//保存一笔开箱取件记录
					if(gstKeyboard.iCardUID)
					{
						StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 1);
					}
					else
					{
						StoreRecord_OpenBoxByFinger(gstKeyboard.u8Box, 1);
					}

					//更新空箱列表
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}	
				stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
				gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;
				gstKeyboard.bAddInto = 1;							
			}					
			break;

			case LockersStep_WaitCardAway:  
			{	
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > 220)
				{
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					iCardID01 = Get_IC_ID_CardSN();
					if((0 == iCardID01)||(iCardID01 != gstKeyboard.iCardUID))
					{
						stUserConsign.TryCount ++;
						if(stUserConsign.TryCount >= 3)
						{
							stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms - (NOTICE_STAY_TIME /2);
							gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
							gstKeyboard.bAddInto = 1;
						}
					}
				}
			}
			break;		

			case LockersStep_Notice:  
			{
				if(gstSysRunInfo.u32TimeCount_ms - stUserConsign.iLastHandleTime > NOTICE_STAY_TIME)
				{
					if(0x00 == GET_Fingerprint_STATE)
					{
						delay_ms(NOTICE_STAY_TIME);
					}
					gstKeyboard.u8SecondMenuStep = 0;
					gstKeyboard.u8AdminMenuStep = 0;
					gstKeyboard.bAddInto = 1;
				}
			}
			break;
			
			default:
			{
				gstKeyboard.u8SecondMenuStep = 0;
			}
			break;
		}
	}			
}

/***********************************************************************************************
* Function Name  : User_Consign
* Description	 : 用户寄存操作流程
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 按寄存模式选择对应的操作流程
***********************************************************************************************/
void User_Consign(void)
{
	switch(SysConfig.stLockers.u8WorkMode)
	{
		case WORK_MODE_VALID_CARD:
		{
			ConsignMode_PreAuthorization();
		}
		break;

		case WORK_MODE_WRITE_CARD:
		{
			ConsignMode_WriteCard();
		}
		break;
		
		case WORK_MODE_ONLINE:
		{
			ConsignMode_OnlineAsk();
		}
		break;

		case WORK_MODE_ANY_FINGER:
		{
			ConsignMode_AnyFinger();
		}
		break;

		case WORK_MODE_ANY_FINGER_CARD:
		{
			ConsignMode_AnyFingerOrCard();
		}
		break;
		
		default:
		{
			ConsignMode_AnyCard();
		}
		break;
	}
}


/***********************************************************************************************
* Function Name  : Lockers_TakeAwayByFinger
* Description	 : 按指纹取件
* Input 		 : None
* Output		 : None
* Return		 : None 
***********************************************************************************************/
void Lockers_TakeAwayByFinger(void)
{
	uint32 iAddr = 0;	
	uint16 iRet = 0;
	static uchar8 bScreen;
	uchar8 i = 0;
	uint32 iTime01, iTime02;
	
	switch(stLockersOperation.HandleStep)
	{
		case HandleStep_Start: //仅在系统空闲状态下读卡
		{
			if(SYS_STATUS_IDLE == gstSysRunInfo.CurrentStatus)
			{	
				if(0x00 == GET_Fingerprint_STATE) //检查是否感应到指纹
				{	
					delay_ms(100);
					if(0x00 == GET_Fingerprint_STATE) //检查是否感应到指纹
					{
						stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
						gstSysRunInfo.CurrentStatus = SYS_STATUS_QuWu;							
						stLockersOperation.HandleStep = HandleStep_GetFingerprint;
					}
				}
			}	
		}
		break;
		
		case HandleStep_GetFingerprint:
		{
			if(gstSysRunInfo.u32TimeCount_ms - stLockersOperation.iLastTime > 5000)
			{
				stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
				//读取指纹超时
				Fingerprint_Search(ZW_StopHandle);
				stLockersOperation.HandleStep = HandleStep_Message;					
			}
			else
			{
				iRet = Fingerprint_Search(0);
				if(0xE000 == iRet) //可能是误触发
				{
					gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;	
					stLockersOperation.HandleStep = HandleStep_Start;
				}
				else if(iRet > 0xE000)
				{
					if(0xFFFF == iRet)
					{
						Voice_Play(YY_ZhiWenWeiShouQuan);	
						GUI_LockersNotice(LOCKERS_NoDeposit);
						bScreen = 1;
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);  //未避免指纹头感应到错误信号,移至此!
					}
					stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
					stLockersOperation.HandleStep = HandleStep_Message;
				}					
				else if((iRet >= 0xA000)&&(iRet <= 0xA00F))
				{
					//正在跑流程中
				}
				else if((iRet)&&(iRet <= SysConfig.stLockers.u8BoxCount))
				{
					//找到匹配的指纹号(指纹号与箱号相同)
					Buzzer(330);
					gstKeyboard.u8Box = iRet;

					//限时取件条件判断
					i = gstKeyboard.u8Box;
					if(SysConfig.stLockers.iMinFetchTime)
					{
						iTime01 = (SysConfig.stBox[i].LastUsedTime[0]*365) + (SysConfig.stBox[i].LastUsedTime[1]*30) + SysConfig.stBox[i].LastUsedTime[2];
						iTime02 = (gstSysRunInfo.stRtcTime.u8Year * 365) + (gstSysRunInfo.stRtcTime.u8Month * 30) + gstSysRunInfo.stRtcTime.u8Day;
						if(iTime01 == iTime02)
						{
							iTime01 = (SysConfig.stBox[i].LastUsedTime[3]*3600) + (SysConfig.stBox[i].LastUsedTime[4]*60) + SysConfig.stBox[i].LastUsedTime[5];	
							iTime02 = (gstSysRunInfo.stRtcTime.u8Hour * 3600) + (gstSysRunInfo.stRtcTime.u8Minute * 60) + gstSysRunInfo.stRtcTime.u8Second;
							if(iTime02 - iTime01 < SysConfig.stLockers.iMinFetchTime)
							{
								GUI_LockersNotice(LOCKERS_NotExtractTime);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
								bScreen = 1;
								stLockersOperation.HandleStep = HandleStep_Message;
								return;
							}
						}
					}

					stLockersOperation.TryCout = 0;
					stLockersOperation.HandleStep = HandleStep_QuWuOpenBox;
				}
			}
		}
		break;

		case HandleStep_QuWuOpenBox:
		{
			iRet = SelectOneBoxToOpen(gstKeyboard.u8Box, SysConfig.stLockers.bCheckBoxDoorStatus);			
			if(iRet)  //开箱失败,提示寄存失败并进行锁箱操作
			{
				GUI_UserTakeAwwyFail(gstKeyboard.u8Box);
				Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
				Voice_Play(YY_XiangMenGuZhan);
				
				//锁住此箱										
				SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
				iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
				Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

				//保存开箱记录
				StoreRecord_OpenBoxByFinger(gstKeyboard.u8Box, 2);
				
				stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
				stLockersOperation.HandleStep = HandleStep_Message;											
			}
			else //开箱成功
			{
				GUI_Open1Box(gstKeyboard.u8Box, 1);
				VoicePlay_xBoxOpen(gstKeyboard.u8Box);
				Voice_Play(YY_QuWuTiShi);

				//清除此箱记录的指纹
				SysConfig.stBox[gstKeyboard.u8Box].iTouchID = 0;
				iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].iTouchID - (uint32)&SysConfig);
				Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].iTouchID, 2);									

				//更改箱状态			
				SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_FREE;
				SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_IDLE; 									
				iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
				Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);
				iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].u8Status - (uint32)&SysConfig);
				Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].u8Status, 1);

				//清除指纹
				SendCmdToFingerprint_DeletChar(gstKeyboard.u8Box, 1);
				
				//保存开箱记录
				StoreRecord_OpenBoxByFinger(gstKeyboard.u8Box, 2);
				
				//更新空箱列表
				AddOneEmptyBox(gstKeyboard.u8Box);
									
				stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
				stLockersOperation.HandleStep = HandleStep_Message;			
			}
			bScreen = 1;
		}
		break;
		
		case HandleStep_Message:
		{
			if(gstSysRunInfo.u32TimeCount_ms - stLockersOperation.iLastTime > NOTICE_STAY_TIME)
			{
				if(SYS_STATUS_KEY != gstSysRunInfo.CurrentStatus)
				{
					if(0x00 == GET_Fingerprint_STATE)
					{
						delay_ms(NOTICE_STAY_TIME);
					}

					if(bScreen)
					{
						bScreen = 0;
						GUI_Admin();
					}
					gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;	
				}
				stLockersOperation.HandleStep = HandleStep_Start;
			}
		}
		break;
		
		default:
		{
			stLockersOperation.HandleStep = 0;
		}
		break;		
	}
}





/*===================================File end===================================================*/


