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
**  Function:	�Ĵ���ȡ��������                                                             
==
**  Data:       2017-06-27
================================================================================================*/

#include "Includes.h"
#include "app_Lockers.h"


//-----------------------------------�Ĵ���������--------------------------------------/
typedef enum 							    
{
	LockersStep_Start = 0,			//��ʼִ��	
	LockersStep_ReadCard,           //����
	LockersStep_OpenBox,            //����
	LockersStep_WaitCardAway,       //�ȴ����뿪
	LockersStep_Notice, 			//��ʾ��ʾ��Ϣ	
	LockersStep_OnlineAsk,          //��������
	LockersStep_WaitServerReply,    //�ȴ���λ����
	LockersStep_GetFingerprint,     //�ȴ�¼ָ��
	LockersStep_
	
}LOCKERS_STEP;

//-----------------------------------���������--------------------------------------/
typedef enum 							    
{
	HandleStep_Start = 0,				//��ʼִ��	
	HandleStep_Message,					//��ʾ��Ϣ	
	HandleStep_GetFingerprint, 			//��ָ��ȡ��
	HandleStep_QuWuOpenBox, 			//ȡ�￪�����
	HandleStep_
	
}FINGER_HANDLE_STEP;

typedef	struct								 
{
	uint32 iCardNum;             //����
	uint32 iLastTime;            //�ϴ�ִ��ʱ��

	uchar8 HandleStep;           //ִ�й켣
//	uchar8 Box;                  //���  
	uchar8 TryCout; 			 //���Լ���
	
}ST_LOCKERS_OPERATION;

volatile ST_LOCKERS_OPERATION stLockersOperation;

uchar8 UserCardData[50] = {0};
volatile ST_CONSIGN stUserConsign;
volatile ST_ONLINEE_ASK stOnlineAsk;

/***********************************************************************************************
* Function Name  : UserConsign_OnlineAsk
* Description	 : �û���������Ĵ�
* Input 		 : iCardID -- ��ID��
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
	SendLen += 7; //Ԥ��
	Comm_ApplyFor(CMD_USER_ASK_CONSIGN, SendBuf, SendLen, COMM_TYPE_UDP_CLIENT);	
}

/***********************************************************************************************
* Function Name  : UserTakeAway_OnlineAsk
* Description	 : �û���������ȡ��
* Input 		 : iCardID -- ��ID��
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
	SendLen += 7; //Ԥ��

	Comm_ApplyFor(CMD_USER_ASK_TAKEAWAY, SendBuf, SendLen, COMM_TYPE_UDP_CLIENT);	
}

/***********************************************************************************************
* Function Name  : UserConsign_ServerReply
* Description	 : �û����߼Ĵ�--������Ӧ��
* Input 		 : pReplyBuf -- Ӧ�����ݰ�, Len -- Ӧ�����ݳ���
* Output		 : None
* Return		 : None 
* Notes          : Ӧ������: 4Byte���� + �Ĵ���(1B) + ���ڹ�����(6B) + �������(1B) + ������ʾ(20B)
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
* Description    : ������Ӧ��ָ��ִ��
* Input          : CMD--������, pRecvBuf������������, dLen -- ���ݲ��ֳ���
* Variable       : gstCmdPacket -- ͨѶָ�����ݰ�
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
* Description	 : �û�ˢ���Ĵ�--���⿨ģʽ
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : ˢ����������
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
			case LockersStep_Start: //���¼Ĵ������Ĵ�����
			{				
				//ϵͳ���ڱ���,���üĴ湦��
				if(gstSysRunInfo.bLockedSys)
				{
					GUI_LockersNotice(LOCKERS_SysError);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);

					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					return;
				}

				//��������Ƿ�غ�
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
				
				//�����û�п���	
				if((gstSysRunInfo.EmptyBox[0]>0)&&(gstSysRunInfo.EmptyBox[0] <= SysConfig.stLockers.u8BoxCount))
				{	
					GUI_LockersNotice(LOCKERS_PlsCard);
					Voice_Play(YY_PlsCard);
					gstKeyboard.u8SecondMenuStep = LockersStep_ReadCard;
				}
				else
				{
					//��ʾ������								
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
						
						//������ǲ��ǹ���
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

						//�ȼ���´˿��Ƿ��Ѵ����Ʒ
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
						if(stUserConsign.iTimeout > 8000)  //��ʱ��ˢ���Զ��˳�(gstKeyboard.bAddInto = 1, ����Զ�ȡ�����˳���Ч)
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
				if(ret)  //����ʧ��
				{
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					GUI_UserConsignFail(gstKeyboard.u8Box);	
					Voice_Play(YY_MenGuZhanQingJiCun);
											
					//��ס����										
					SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

					//���¿����б�
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}
				else
				{	
					//�����ɹ�
					GUI_Open1Box(gstKeyboard.u8Box, 1);
					VoicePlay_xBoxOpen(gstKeyboard.u8Box);
					Voice_Play(YY_CunWuTiShi);
					gstSysRunInfo.u8UseBox = gstKeyboard.u8Box;
					gstSysRunInfo.iHandleTimeout = SysConfig.stLockers.iHandleTimeout;
					
					//��¼�����¼�Ŀ��ź�����/������״̬
					SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = gstKeyboard.iCardUID;
					SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_OCCUPY;										
					memcpy((uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].LastUsedTime[0], (uchar8*)gstSysRunInfo.TimeBuf, 6);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));
					
					//����һ�ʿ���ȡ����¼
					StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 1);

					//���¿����б�
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
* Description	 : �û�ˢ���Ĵ�--Ԥ��Ȩ��ģʽ
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
			case LockersStep_Start: //���¼Ĵ������Ĵ�����
			{				
				//ϵͳ���ڱ���,���üĴ湦��
				if(gstSysRunInfo.bLockedSys)
				{
					GUI_LockersNotice(LOCKERS_SysError);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);

					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					return;
				}

				//��������Ƿ�غ�
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
				
				//�����û�п���	
				if((gstSysRunInfo.EmptyBox[0]>0)&&(gstSysRunInfo.EmptyBox[0] <= SysConfig.stLockers.u8BoxCount))
				{	
					GUI_LockersNotice(LOCKERS_PlsCard);
					Voice_Play(YY_PlsCard);
					gstKeyboard.u8SecondMenuStep = LockersStep_ReadCard;
				}
				else
				{
					//��ʾ������								
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
						//������ǲ��ǹ���
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

						//�ȼ���´˿��Ƿ��Ѵ����Ʒ
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

						//����ǲ���Ԥ��Ȩ��
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
						if(stUserConsign.iTimeout > 8000)  //��ʱ��ˢ���Զ��˳�(gstKeyboard.bAddInto = 1, ����Զ�ȡ�����˳���Ч)
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
				if(ret)  //����ʧ��
				{
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					GUI_UserConsignFail(gstKeyboard.u8Box);	
					Voice_Play(YY_MenGuZhanQingJiCun);
											
					//��ס����										
					SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

					//���¿����б�
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}
				else
				{	
					//�����ɹ�
					GUI_Open1Box(gstKeyboard.u8Box, 1);
					VoicePlay_xBoxOpen(gstKeyboard.u8Box);
					Voice_Play(YY_CunWuTiShi);
					gstSysRunInfo.u8UseBox = gstKeyboard.u8Box;
					gstSysRunInfo.iHandleTimeout = SysConfig.stLockers.iHandleTimeout;
					
					//��¼�����¼�Ŀ��ź�����/������״̬
					SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = gstKeyboard.iCardUID;
					SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_OCCUPY;										
					memcpy((uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].LastUsedTime[0], (uchar8*)gstSysRunInfo.TimeBuf, 6);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));
					
					//����һ�ʿ���ȡ����¼
					StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 1);

					//���¿����б�
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
* Description	 : �û�ˢ���Ĵ�--д��ģʽ
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : �Ὣ�Ĵ���Ϣд�뿨������
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
			case LockersStep_Start: //���¼Ĵ������Ĵ�����
			{				
				//ϵͳ���ڱ���,���üĴ湦��
				if(gstSysRunInfo.bLockedSys)
				{
					GUI_LockersNotice(LOCKERS_SysError);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);

					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					return;
				}

				//��������Ƿ�غ�
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
				
				//�����û�п���	
				if((gstSysRunInfo.EmptyBox[0]>0)&&(gstSysRunInfo.EmptyBox[0] <= SysConfig.stLockers.u8BoxCount))
				{	
					GUI_LockersNotice(LOCKERS_PlsCard);
					Voice_Play(YY_PlsCard);
					gstKeyboard.u8SecondMenuStep = LockersStep_ReadCard;
				}
				else
				{
					//��ʾ������								
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
						//������ǲ��ǹ���
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
						
						//�ȼ���´˿��Ƿ��Ѵ����Ʒ
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
						
						//��ȡ����������
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

							//������ȡ��Ϣ
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
							
							//����Ƿ����ڱ�Ĺ����д����Ʒ
							if((USER_STORE == stUserCard.bStoreOrTake)
								&&(memcmp((uchar8*)&stUserCard.OnLocker[0], (uchar8*)&SysConfig.stLockers.Name[0], MAX_NameLen)))
							{
								GUI_PackageInOtherLockers((char*)stUserCard.OnLocker, stUserCard.u8OnBox);	
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
								gstKeyboard.u8SecondMenuStep = LockersStep_WaitCardAway;	
								return;												
							}

							//���Ĵ�����
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
								//���ϼĴ�����--��ָ������д�Ĵ���Ϣ
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

								//д��
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
						if(stUserConsign.iTimeout > 8000)  //��ʱ��ˢ���Զ��˳�(gstKeyboard.bAddInto = 1, ����Զ�ȡ�����˳���Ч)
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
				if(ret)  //����ʧ��
				{
					//��ʾ��ˢ��--�������¼�ļĴ���Ϣ
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

							//д��
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

					//��ʾ����ʧ��,�����¼Ĵ�
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					GUI_UserConsignFail(gstKeyboard.u8Box);	
					Voice_Play(YY_MenGuZhanQingJiCun);
											
					//��ס����										
					SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

					//���¿����б�
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}
				else
				{	
					//�����ɹ�
					GUI_Open1Box(gstKeyboard.u8Box, 1);
					VoicePlay_xBoxOpen(gstKeyboard.u8Box);
					Voice_Play(YY_CunWuTiShi);
					gstSysRunInfo.u8UseBox = gstKeyboard.u8Box;
					gstSysRunInfo.iHandleTimeout = SysConfig.stLockers.iHandleTimeout;
					
					//��¼�����¼�Ŀ��ź�����/������״̬
					SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = gstKeyboard.iCardUID;
					SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_OCCUPY;										
					memcpy((uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].LastUsedTime[0], (uchar8*)gstSysRunInfo.TimeBuf, 6);

					//�����͹���
					memcpy((uchar8*)SysConfig.stBox[gstKeyboard.u8Box].UserName, (uchar8*)stUserCard.Name, MAX_UserNameLen);
					memcpy((uchar8*)SysConfig.stBox[gstKeyboard.u8Box].UserID, (uchar8*)stUserCard.JobID, CARD_JobIdLen);
					
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));
					
					//����һ�ʿ���ȡ����¼
					StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 1);

					//���¿����б�
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
* Description	 : �û�ˢ���Ĵ�--������֤ģʽ
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : ˢ����������
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
			case LockersStep_Start: //���¼Ĵ������Ĵ�����
			{	
				memset((uchar8*)&stOnlineAsk, 0x0, sizeof(ST_ONLINEE_ASK));
				
				//ϵͳ���ڱ���,���üĴ湦��
				if(gstSysRunInfo.bLockedSys)
				{
					GUI_LockersNotice(LOCKERS_SysError);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);

					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					return;
				}

				//��������Ƿ�غ�
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
				
				//�����û�п���	
				if((gstSysRunInfo.EmptyBox[0]>0)&&(gstSysRunInfo.EmptyBox[0] <= SysConfig.stLockers.u8BoxCount))
				{	
					GUI_LockersNotice(LOCKERS_PlsCard);
					Voice_Play(YY_PlsCard);
					gstKeyboard.u8SecondMenuStep = LockersStep_ReadCard;
				}
				else
				{
					//��ʾ������								
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
						//������ǲ��ǹ���
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

						//�ȼ���´˿��Ƿ��Ѵ����Ʒ
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

						//����ǲ���Ԥ��Ȩ��
						Buzzer(350);
						GUI_ShowNotice(NOTICE_PLS_WAIT);
						gstKeyboard.u8SecondMenuStep = LockersStep_OnlineAsk;
					}
					else
					{
						stUserConsign.iTimeout += 300;
						if(stUserConsign.iTimeout > 8000)  //��ʱ��ˢ���Զ��˳�(gstKeyboard.bAddInto = 1, ����Զ�ȡ�����˳���Ч)
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
							//strcpy((char*)stOnlineAsk.Notice, "�ÿ�Ƭ״̬�쳣!");
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
						//δ�յ�������Ӧ��,�ٴγ���	
						gstKeyboard.u8SecondMenuStep = LockersStep_OnlineAsk;
					}
					else
					{
						//���������Ӧ�� 
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
				if(ret)  //����ʧ��
				{
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					GUI_UserConsignFail(gstKeyboard.u8Box);	
					Voice_Play(YY_MenGuZhanQingJiCun);
											
					//��ס����										
					SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

					//���¿����б�
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}
				else
				{	
					//�����ɹ�
					GUI_Open1Box(gstKeyboard.u8Box, 1);
					VoicePlay_xBoxOpen(gstKeyboard.u8Box);
					Voice_Play(YY_CunWuTiShi);
					gstSysRunInfo.u8UseBox = gstKeyboard.u8Box;
					gstSysRunInfo.iHandleTimeout = SysConfig.stLockers.iHandleTimeout;
					
					//��¼�����¼�Ŀ��ź�����/������״̬
					SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = gstKeyboard.iCardUID;
					SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_OCCUPY;										
					memcpy((uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].LastUsedTime[0], (uchar8*)gstSysRunInfo.TimeBuf, 6);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));
					
					//����һ�ʿ���ȡ����¼
					StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 1);

					//���¿����б�
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
* Description	 : �û�ˢ���Ĵ�--����ָ�� 
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
			case LockersStep_Start: //���¼Ĵ������Ĵ�����
			{				
				//ϵͳ���ڱ���,���üĴ湦��
				if(gstSysRunInfo.bLockedSys)
				{
					GUI_LockersNotice(LOCKERS_SysError);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);

					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					return;
				}

				//��������Ƿ�غ�
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
				
				//�����û�п���	
				if((gstSysRunInfo.EmptyBox[0]>0)&&(gstSysRunInfo.EmptyBox[0] <= SysConfig.stLockers.u8BoxCount))
				{	
					GUI_LockersNotice(LOCKERS_PlsFingerOrCard);
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_GetFingerprint;
				}
				else
				{
					//��ʾ������								
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
					//��ȡָ�Ƴ�ʱ
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
				if(ret)  //����ʧ��
				{
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					GUI_UserConsignFail(gstKeyboard.u8Box);	
					Voice_Play(YY_MenGuZhanQingJiCun);
											
					//��ס����										
					SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

					//���ָ��
					SendCmdToFingerprint_DeletChar(gstKeyboard.u8Box, 1);

					//���¿����б�
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}
				else
				{	
					//�����ɹ�
					GUI_Open1Box(gstKeyboard.u8Box, 1);
					VoicePlay_xBoxOpen(gstKeyboard.u8Box);
					Voice_Play(YY_CunWuTiShi);
					gstSysRunInfo.u8UseBox = gstKeyboard.u8Box;
					gstSysRunInfo.iHandleTimeout = SysConfig.stLockers.iHandleTimeout;
					
					//��¼�����¼�Ŀ��ź�����/������״̬
					SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = gstKeyboard.iCardUID;
					SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_OCCUPY;	
					SysConfig.stBox[gstKeyboard.u8Box].iTouchID = gstKeyboard.u8Box; 
					memcpy((uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].LastUsedTime[0], (uchar8*)gstSysRunInfo.TimeBuf, 6);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));
					
					//����һ�ʿ���ȡ����¼
					if(gstKeyboard.iCardUID)
					{
						StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 1);
					}
					else
					{
						StoreRecord_OpenBoxByFinger(gstKeyboard.u8Box, 1);
					}

					//���¿����б�
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
* Description	 : �û�ˢ���Ĵ�--����ָ�ƻ�ģʽ
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : ˢ����������
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
			case LockersStep_Start: //���¼Ĵ������Ĵ�����
			{				
				//ϵͳ���ڱ���,���üĴ湦��
				if(gstSysRunInfo.bLockedSys)
				{
					GUI_LockersNotice(LOCKERS_SysError);
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);

					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_Notice;
					return;
				}

				//��������Ƿ�غ�
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
				
				//�����û�п���	
				if((gstSysRunInfo.EmptyBox[0]>0)&&(gstSysRunInfo.EmptyBox[0] <= SysConfig.stLockers.u8BoxCount))
				{	
					GUI_LockersNotice(LOCKERS_PlsFingerOrCard);
					stUserConsign.iLastHandleTime = gstSysRunInfo.u32TimeCount_ms;
					gstKeyboard.u8SecondMenuStep = LockersStep_GetFingerprint;
				}
				else
				{
					//��ʾ������								
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
					//��ȡָ�Ƴ�ʱ
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

					//����
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
							
							//������ǲ��ǹ���
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

							//�ȼ���´˿��Ƿ��Ѵ����Ʒ
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
				if(ret)  //����ʧ��
				{
					Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
					GUI_UserConsignFail(gstKeyboard.u8Box);	
					Voice_Play(YY_MenGuZhanQingJiCun);
											
					//��ס����										
					SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

					//���ָ��
					SendCmdToFingerprint_DeletChar(gstKeyboard.u8Box, 1);

					//���¿����б�
					DeleteOneEmptyBox(gstKeyboard.u8Box);
				}
				else
				{	
					//�����ɹ�
					GUI_Open1Box(gstKeyboard.u8Box, 1);
					VoicePlay_xBoxOpen(gstKeyboard.u8Box);
					Voice_Play(YY_CunWuTiShi);
					gstSysRunInfo.u8UseBox = gstKeyboard.u8Box;
					gstSysRunInfo.iHandleTimeout = SysConfig.stLockers.iHandleTimeout;
					
					//��¼�����¼�Ŀ��ź�����/������״̬
					SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = gstKeyboard.iCardUID;
					SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_OCCUPY;	
					SysConfig.stBox[gstKeyboard.u8Box].iTouchID = gstKeyboard.u8Box; 
					memcpy((uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].LastUsedTime[0], (uchar8*)gstSysRunInfo.TimeBuf, 6);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box], sizeof(ST_BOX));
					
					//����һ�ʿ���ȡ����¼
					if(gstKeyboard.iCardUID)
					{
						StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 1);
					}
					else
					{
						StoreRecord_OpenBoxByFinger(gstKeyboard.u8Box, 1);
					}

					//���¿����б�
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
* Description	 : �û��Ĵ��������
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : ���Ĵ�ģʽѡ���Ӧ�Ĳ�������
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
* Description	 : ��ָ��ȡ��
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
		case HandleStep_Start: //����ϵͳ����״̬�¶���
		{
			if(SYS_STATUS_IDLE == gstSysRunInfo.CurrentStatus)
			{	
				if(0x00 == GET_Fingerprint_STATE) //����Ƿ��Ӧ��ָ��
				{	
					delay_ms(100);
					if(0x00 == GET_Fingerprint_STATE) //����Ƿ��Ӧ��ָ��
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
				//��ȡָ�Ƴ�ʱ
				Fingerprint_Search(ZW_StopHandle);
				stLockersOperation.HandleStep = HandleStep_Message;					
			}
			else
			{
				iRet = Fingerprint_Search(0);
				if(0xE000 == iRet) //�������󴥷�
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
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);  //δ����ָ��ͷ��Ӧ�������ź�,������!
					}
					stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
					stLockersOperation.HandleStep = HandleStep_Message;
				}					
				else if((iRet >= 0xA000)&&(iRet <= 0xA00F))
				{
					//������������
				}
				else if((iRet)&&(iRet <= SysConfig.stLockers.u8BoxCount))
				{
					//�ҵ�ƥ���ָ�ƺ�(ָ�ƺ��������ͬ)
					Buzzer(330);
					gstKeyboard.u8Box = iRet;

					//��ʱȡ�������ж�
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
			if(iRet)  //����ʧ��,��ʾ�Ĵ�ʧ�ܲ������������
			{
				GUI_UserTakeAwwyFail(gstKeyboard.u8Box);
				Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
				Voice_Play(YY_XiangMenGuZhan);
				
				//��ס����										
				SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
				iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
				Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);

				//���濪���¼
				StoreRecord_OpenBoxByFinger(gstKeyboard.u8Box, 2);
				
				stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
				stLockersOperation.HandleStep = HandleStep_Message;											
			}
			else //����ɹ�
			{
				GUI_Open1Box(gstKeyboard.u8Box, 1);
				VoicePlay_xBoxOpen(gstKeyboard.u8Box);
				Voice_Play(YY_QuWuTiShi);

				//��������¼��ָ��
				SysConfig.stBox[gstKeyboard.u8Box].iTouchID = 0;
				iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].iTouchID - (uint32)&SysConfig);
				Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].iTouchID, 2);									

				//������״̬			
				SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_FREE;
				SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_IDLE; 									
				iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
				Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);
				iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].u8Status - (uint32)&SysConfig);
				Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].u8Status, 1);

				//���ָ��
				SendCmdToFingerprint_DeletChar(gstKeyboard.u8Box, 1);
				
				//���濪���¼
				StoreRecord_OpenBoxByFinger(gstKeyboard.u8Box, 2);
				
				//���¿����б�
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


