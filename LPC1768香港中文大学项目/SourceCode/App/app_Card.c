/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                     
==                                                                                          
**  Version:	V1.0.0                                                                          
==
**  Filename:	Card.c                                                                       
==                                                                                         
**  Function:	IC��ID����ز���   
==
**  Data:       2015/09/11                                                                       
================================================================================================*/

#include "Includes.h"

#include "App_GUI.h"


//-----------------------------------���������--------------------------------------/
typedef enum 							    
{
	CardStep_Start = 0,				//��ʼִ��	
	CardStep_WaitingAway,           //�ȴ����뿪
	CardStep_Message,				//��ʾ��Ϣ	
	CardStep_OnlineAsk,             //��������
	CardStep_WaitServerReply,       //�ȴ���λ����
	CardStep_
	
}E_CARD_HANDLE_STEP;


uchar8 CardData[50] = {0};
volatile ST_USER_CARD stUserCard;


/***********************************************************************************************
* Function Name  : Get_IC_ID_CardSN
* Description	 : ��ȡIC��ID���Ŀ����к�
* Input 		 : None
* Output		 : None
* Return		 : �����Ŀ���
***********************************************************************************************/
uint32 Get_IC_ID_CardSN(void)
{
	uint32 iCardID = 0;

	iCardID = RF_GetCardUID();
	if(0 == iCardID)
	{
		iCardID = Wiegand_ReardCardID();
	}
	
	return iCardID;
}

/***********************************************************************************************
* Function Name  : CheckCardValid
* Description	 : ���Ԥ��Ȩ��
* Input 		 : iCardID -- ����
* Output		 : None
* Return		 : 0 <= -- ��Ч(�˿������ڵ����λ��), ����--��Ч
***********************************************************************************************/
int CheckCardValid(uint32 iCardID)
{
	int i = 0;
	uint32 iNum = 0;
	uint32 iAddr = Addr_PreAuthorization;	
	
//	for(i=0; i<SysConfig.stLockers.iValidCardCount; i++)
	for(i=0; i<MAX_VALID_CAED_COUNT; i++)
	{
		WDT_Feed();
		I2C_EE_BufferRead((uchar8*)&iNum, iAddr, 4);	
		if(iNum == iCardID)
		{
			return i;
		}
		iAddr += 4; 
	}
	return -1;
}

/***********************************************************************************************
* Function Name  : AddOneValidCardNum
* Description	 : ����һ��Ԥ��Ȩ����
* Input 		 : iCardNum -- ����
* Output		 : None
* Return		 : ��ӽ��
***********************************************************************************************/
uchar8 AddOneValidCardNum(uint32 iCardNum)
{
	uint32 iAddr = Addr_PreAuthorization;	
	uint32 iNum = 0;
	int i = 0;

	//�Ȳ������б�����û��
	for(i=0; i<SysConfig.stLockers.iValidCardCount; i++)
	{
		WDT_Feed();
		I2C_EE_BufferRead((uchar8*)&iNum, iAddr, 4);	
		if(iNum == iCardNum)
		{
			return ALREADY_EXIST;
		}
		iAddr += 4; 
	}

	if(SysConfig.stLockers.iValidCardCount < MAX_VALID_CAED_COUNT)
	{
		//�����¼�洢��ַ
		iAddr = Addr_PreAuthorization + (SysConfig.stLockers.iValidCardCount * 4);
		I2C_EE_BufferWrite((uchar8*)&iCardNum, iAddr, 4);
		delay_ms(21);

		//�����¼����  
		SysConfig.stLockers.iValidCardCount ++;
		iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.iValidCardCount - (uint32)&SysConfig);
		Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.iValidCardCount, 4);

		return 1;
	}
	return 0xFF;
}

/***********************************************************************************************
* Function Name  : AdminCard_Login
* Description	 : ˢ������¼
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : �൱�ڴ�������"#-->*-->000-->Enter"
***********************************************************************************************/
void AdminCard_Login(void)
{
	gstKeyboard.u8AdminMenuStep = 4;
	gstKeyboard.u8SecondMenuStep = 4;
	gstSysRunInfo.CurrentStatus = SYS_STATUS_KEY;
	gstKeyboard.iLastKeyTime = gstSysRunInfo.u32TimeCount_ms;
	Admin_Login(0xCC);
}

/***********************************************************************************************
* Function Name  : SuperCard_SetAdmin
* Description	 : ˢ���������ù���Ա�˺�
* Input 		 : None
* Output		 : None
* Return		 : None 
***********************************************************************************************/
void SuperCard_SetAdmin(void)
{
	uchar8 status = 0;
	char i = 0;
	uint32 uiNum = 0;
	uint32 iAddr = 0;

	//��ȡ����������
	memset(CardData, 0x0, sizeof(CardData));
	status = RF_ReadCard((uchar8*)&gstKeyboard.iCardUID, 1, AuthKey_A, (uchar8*)SysConfig.stCardKey.Password, CardData);
	if(OK == status)
	{

		if(0 == memcmp("ADMIN", &CardData[0], strlen("ADMIN")))
		{
			Beep(BEEP_TIME_200MS, BEEP_TIME_200MS, 2);
			//����Ա��¼����
			iAddr = 22;
			memcpy((uchar8*)&SysConfig.stLockers.AdminPassword[0], &CardData[iAddr], MAX_PasswordLen);
			iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.AdminPassword[0] - (uint32)&SysConfig);
			Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.AdminPassword[0], MAX_PasswordLen);

			//����
			iAddr = 28;
			for(i=0; i<MAX_AdminCardNum; i++)
			{
				memcpy((uchar8*)&uiNum, &CardData[iAddr], 4);
				iAddr += 4;
				SysConfig.stLockers.AdminCard[i] = uiNum;
			}
			GUI_ShowNotice(NOTICE_SET_OK);

			//��������
			iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.AdminCard[0] - (uint32)&SysConfig);
			Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.AdminCard[0], sizeof(SysConfig.stLockers.AdminCard));
		}
		else if(0 == memcmp("TOP-M", &CardData[0], strlen("TOP-M")))
		{
			//�߼�����Ա��¼����
			iAddr = 22;
			memcpy((uchar8*)&SysConfig.stLockers.SeniorPassword[0], &CardData[iAddr], MAX_PasswordLen);
			iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers.SeniorPassword[0] - (uint32)&SysConfig);
			Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers.SeniorPassword[0], MAX_PasswordLen);

			Buzzer(300);
			GUI_ShowNotice(NOTICE_SET_OK);
		}
		else
		{
			GUI_ShowMessage("Super Card Wrong !!", LCD_Color_Red);
			Beep(BEEP_TIME_200MS, BEEP_TIME_200MS, 5);
		}
			
	}
	else
	{
		GUI_ShowMessage("Super CardID !", LCD_Color_Red);
		Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
	}	
}

/***********************************************************************************************
* Function Name  : ArgCardOperation
* Description	 : ��ȡ�������ÿ���ִ�в�������
* Input 		 : void
* Output		 : None
* Return		 : None 
* Notes          : ���ÿ��е�ĳЩ�������ԼӺ�д�ؿ���
***********************************************************************************************/
void ArgCardOperation(void)
{

	static uint32 iLastOperationTime;
	uchar8 status = 0;
	uint32 iCardID01 = 0;
	uint32 iAddr = 0;
	ST_PARAMET_CARD stLockersCard;
	ST_NETWORK stNetCard;
	memset((void*)&stLockersCard, 0x0, sizeof(ST_PARAMET_CARD));
	memset((void*)&stNetCard, 0x0, sizeof(ST_NETWORK));

	if(gstSysRunInfo.u32TimeCount_ms - iLastOperationTime > 220)
	{
		iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
		iCardID01 = Get_IC_ID_CardSN();
		if((iCardID01)&&(iCardID01 != gstKeyboard.iCardUID))
		{
			//��ȡ����������
			memset(CardData, 0x0, sizeof(CardData));
			status = RF_ReadCard((uchar8*)&iCardID01, 1, AuthKey_A, (uchar8*)SysConfig.stCardKey.Password, CardData);
			if(OK == status)
			{				
				//�������ÿ�����
				if(0 == memcmp("LOCKER", &CardData[0], 6))
				{
					iAddr = 6;

					//��<<-0->>����--ϵͳ��ʶ��
					memcpy((uchar8*)&stLockersCard.SysIdCode[0], &CardData[iAddr], 6);
					iAddr += 6;

					//��<<-1->>����
					iAddr = 16;
					//���ַ
					memcpy((uchar8*)&stLockersCard.iID, &CardData[iAddr], 4);
					iAddr += 4;
					//������
					memcpy((uchar8*)&stLockersCard.Name[0], &CardData[iAddr], 6);
					iAddr += 6;
					//����ģʽ
					stLockersCard.u8WorkMode = 	CardData[iAddr++];
					//��������
					stLockersCard.u8AllowOpenBoxNum = CardData[iAddr++];

					//��<<--2-->>����
					iAddr = 32;
					//��С�Ĵ�ʱ��
					memcpy((uchar8*)&stLockersCard.iMinTime, &CardData[iAddr], 4);
					iAddr += 4;
					//���Ĵ�ʱ��
					memcpy((uchar8*)&stLockersCard.iMaxTime, &CardData[iAddr], 4);
					iAddr += 4;
					//���ų�ʱʱ��
					memcpy((uchar8*)&stLockersCard.iCloseTimeout, &CardData[iAddr], 4);
					iAddr += 4;
					//����ά��ʱ��
					memcpy((uchar8*)&stLockersCard.iAlarmTime, &CardData[iAddr], 4);
					iAddr += 4;

					//����ϵͳ����
					SysConfig.stHardware.u32MachineNum = stLockersCard.iID;
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stHardware.u32MachineNum - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stHardware.u32MachineNum, 4);
					
					memcpy((uchar8*)&SysConfig.stClientInfo.IdCode[0], (uchar8*)&stLockersCard.SysIdCode[0], SYS_ID_CODE_LEN);
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stClientInfo.IdCode[0] - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)SysConfig.stClientInfo.IdCode, sizeof(SysConfig.stClientInfo.IdCode));
					
					SysConfig.stLockers.u8WorkMode =stLockersCard.u8WorkMode;
					SysConfig.stLockers.u8AllowBeOpenedBoxNum = stLockersCard.u8AllowOpenBoxNum;
					memcpy((uchar8*)&SysConfig.stLockers.Name[0], (uchar8*)&stLockersCard.Name[0], MAX_NameLen); 

					SysConfig.stLockers.iMinFetchTime = stLockersCard.iMinTime;
					SysConfig.stLockers.iMaxFetchTime = stLockersCard.iMaxTime;
					SysConfig.stLockers.iHandleTimeout = stLockersCard.iCloseTimeout;
					SysConfig.stLockers.iNoticeTime = stLockersCard.iAlarmTime;
					
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stLockers - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stLockers, sizeof(ST_LOCKERS));

					//��ʾ���ò���
					GUI_LockersParameters();
					
					//��һЩ�����ԼӺ�д�ؿ���
					stLockersCard.iID += 1;
					status = strlen((char*)stLockersCard.Name);
					iAddr = AsciiStr_To_UInt((uchar8*)&stLockersCard.Name[1], status-1);
					iAddr += 1;
					UInt_To_Ascii(iAddr, (uchar8*)&stLockersCard.Name[1]);

					//��<<-1->>����
					iAddr = 16;
					//���ַ
					memcpy(&CardData[iAddr], (uchar8*)&stLockersCard.iID, 4);
					iAddr += 4;
					//������
					memcpy(&CardData[iAddr], (uchar8*)&stLockersCard.Name[0], 6);
					iAddr += 6;

					status = RF_WriteCard_1Block((uchar8*)&iCardID01, 5, AuthKey_A, (uchar8*)SysConfig.stCardKey.Password, &CardData[16]);
					if(OK != status)
					{
						status = RF_WriteCard_1Block((uchar8*)&iCardID01, 5, AuthKey_A, (uchar8*)SysConfig.stCardKey.Password, &CardData[16]);
						if(OK != status)
						{
							Beep(BEEP_TIME_200MS, BEEP_TIME_200MS, 2);

						}
						else
						{
							Buzzer(300);
						}
					}
					else
					{
						Buzzer(300);
					}
				}
				else if(0 == memcmp("NET", &CardData[0], 3))
				{
					//����ȡԭ�в���
					memcpy((uchar8*)&stNetCard, (uchar8*)&SysConfig.stNet, sizeof(ST_NETWORK));

					//��ȡ�������ÿ���Ϣ
					//��1--IP
					iAddr = 16;
					memcpy((uchar8*)&stNetCard.IP[0], &CardData[iAddr], 4);
					iAddr += 4;
					//��������
					memcpy((uchar8*)&stNetCard.NetMask[0], &CardData[iAddr], 4);
					iAddr += 4;
					//����
					memcpy((uchar8*)&stNetCard.Gateway[0], &CardData[iAddr], 4);
					iAddr += 4;
					//UDP�˿�
					stNetCard.u16UDP_ListenPort = CardData[iAddr++];
					stNetCard.u16UDP_ListenPort += (CardData[iAddr++] << 8);
					//TCP�˿�
					stNetCard.u16TCP_ListenPort = CardData[iAddr++];
					stNetCard.u16TCP_ListenPort += (CardData[iAddr++] << 8);

					//��2
					iAddr = 32;
					//������IP
					memcpy((uchar8*)&stNetCard.ServerIP[0], &CardData[iAddr], 4);
					iAddr += 4;
					//�������˿�
					stNetCard.u16UDP_ServerPort = CardData[iAddr++];
					stNetCard.u16UDP_ServerPort += (CardData[iAddr++] << 8);
					stNetCard.u16TCP_ServerPort = CardData[iAddr++];
					stNetCard.u16TCP_ServerPort += (CardData[iAddr++] << 8);

					//������Ϣ
					memcpy((uchar8*)&SysConfig.stNet, (uchar8*)&stNetCard, sizeof(ST_NETWORK));
					Store_Parameters(Addr_NetInfo, (uchar8*)&SysConfig.stNet , sizeof(ST_NETWORK));
					//��Ȼ�������������,�ǾͿ�������
					SysConfig.stHardware.bNetwork = VALIDITY_USING;
					iAddr = Addr_SysConfig + ((uint32)&SysConfig.stHardware.bNetwork - (uint32)&SysConfig);
					Store_Parameters(iAddr, (uchar8*)&SysConfig.stHardware.bNetwork, 1);
					

					//��ʾ�������
					GUI_NetParameters();
					
					//��IP�Լ�1��д�ؿ���
					if(stNetCard.IP[3] >= 250)
					{
						stNetCard.IP[3] = 51;
						stNetCard.IP[2] += 1;
					}
					else
					{
						stNetCard.IP[3] += 1;
					}
					
					//IPַ�ڿ�1��
					iAddr = 16;
					memcpy(&CardData[iAddr], (uchar8*)&stNetCard.IP[0], 4);
					status = RF_WriteCard_1Block((uchar8*)&iCardID01, 5, AuthKey_A, (uchar8*)SysConfig.stCardKey.Password, &CardData[16]);
					if(OK != status)
					{
						status = RF_WriteCard_1Block((uchar8*)&iCardID01, 5, AuthKey_A, (uchar8*)SysConfig.stCardKey.Password, &CardData[16]);
						if(OK != status)
						{
							Beep(BEEP_TIME_200MS, BEEP_TIME_200MS, 2);

						}
						else
						{
							Buzzer(300);
						}
					}
					else
					{
						Buzzer(300);
					}					
				}
				else
				{
					Beep(BEEP_TIME_200MS, BEEP_TIME_200MS, 4);

				}
			}
			else
			{
				Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
			}
			delay_ms(5000);
			GUI_Admin();
			memset((uchar8*)&gstKeyboard, 0x0, sizeof(gstKeyboard));
			gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;
		}
	}
}

/***********************************************************************************************
* Function Name  : UserTakesByCardID
* Description	 : ˢ����������
* Input 		 : handle
* Output		 : None
* Return		 : None 
* Notes          : ϵͳ����ʱ�Ŷ��� 
***********************************************************************************************/
void UserTakesByCardID(uchar8 handle)
{
	static uchar8 HandleStep;
	static uint32 iLastOperationTime;
	static uchar8 TryCount;
	int i = 0;
	uchar8 status = 0;
	uint32 iCardID01 = 0;
	uint32 iAddr = 0;
	uint32 iTime01, iTime02;

	if(0 == handle)
	{
		HandleStep = 0;
		TryCount = 0;
		return;
	}
	
	if((SYS_STATUS_IDLE == gstSysRunInfo.CurrentStatus)||(SYS_STATUS_CARD == gstSysRunInfo.CurrentStatus))
	{
		switch(HandleStep)
		{
			case CardStep_Start:
			{
				if(gstSysRunInfo.u32TimeCount_ms - iLastOperationTime > 220)
				{
					iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
					//����
					gstKeyboard.iCardUID = Get_IC_ID_CardSN();
					if(gstKeyboard.iCardUID)
					{
						Buzzer(300);
						gstSysRunInfo.CurrentStatus = SYS_STATUS_CARD;

						//������ǲ��ǳ�����
						if(SUPER_CARD_ID == gstKeyboard.iCardUID)
						{
							SuperCard_SetAdmin();
							delay_ms(3000);
							iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
							HandleStep = CardStep_WaitingAway;
							return;
						}

						//������ǲ��ǹ���
						for(i=0; i<MAX_AdminCardNum; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stLockers.AdminCard[i])
							{
								AdminCard_Login();
								return;
							}
						}
						
						//������ǲ��ǼĴ濨
						for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stBox[i].iUserCardID)
							{
								gstKeyboard.u8Box = i;
								
								//��ʱȡ�������ж�
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
											TryCount = 0;
											iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
											HandleStep = CardStep_WaitingAway;
											return;
										}
									}
								}
								
								GUI_LockersNotice(LOCKERS_OpeningBox);
								status = SelectOneBoxToOpen(i, SysConfig.stLockers.bCheckBoxDoorStatus);;
								if(status)  
								{
									GUI_UserTakeAwwyFail(i);
									Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
									
									//��ס����										
									SysConfig.stBox[i].bLocking = LSTATUS_LOCKING;										
									iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[i].u8Status - (uint32)&SysConfig);
									Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[i].u8Status, 1);
								}
								else
								{	
									//�����ɹ�
									GUI_Open1Box(i, 1);
									VoicePlay_xBoxOpen(i);
									Voice_Play(YY_QuWuTiShi);

									//��������¼�Ŀ���
									SysConfig.stBox[i].iUserCardID = 0;
									iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[i].iUserCardID - (uint32)&SysConfig);
									Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[i].iUserCardID, 4);									

									//������״̬			
									SysConfig.stBox[i].bLocking = LSTATUS_FREE;
									SysConfig.stBox[i].u8Status = BOX_STATUS_IDLE; 									
									iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[i].bLocking - (uint32)&SysConfig);
									Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[i].bLocking, 1);
									iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[i].u8Status - (uint32)&SysConfig);
									Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[i].u8Status, 1);

									//����һ�ʿ����¼
									StoreRecord_OpenBoxByCard(i, gstKeyboard.iCardUID, 2);

									//��������ӵ������б�
									AddOneEmptyBox(i);
								}					
								break;
							}
						}
						if(i > SysConfig.stLockers.u8BoxCount)
						{
							//��ʾ��δ�Ĵ�								
							GUI_LockersNotice(LOCKERS_CardNotConsign);
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						}
						TryCount = 0;
						iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
						HandleStep = CardStep_WaitingAway;
					}
				}
			}
			break;

			case CardStep_WaitingAway: //�ȴ����뿪
			{	
				if(gstSysRunInfo.u32TimeCount_ms - iLastOperationTime > 220)
				{
					iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
					iCardID01 = Get_IC_ID_CardSN();
					if((0 == iCardID01)||(iCardID01 != gstKeyboard.iCardUID))
					{
						TryCount ++;
						if(TryCount >= 2)
						{
							HandleStep = CardStep_Message;
						}
					}
				}
			}
			break;

			case CardStep_Message:
			{
				if(gstSysRunInfo.u32TimeCount_ms - iLastOperationTime > 1234)
				{
					if(VALIDITY_USING == SysConfig.stLockers.bEnableIdCard)
					{
						delay_ms(1000);
						Get_IC_ID_CardSN(); //�ͷ���ʱ�����д���Ŀ���
					}
					GUI_Admin();
					gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;	
					TryCount = 0;
					HandleStep = 0;							
				}
			}
			break;
			
			default:
			{
				HandleStep = CardStep_Start;
			}
			break;
		}
	}
}


/***********************************************************************************************
* Function Name  : UserTakesWriteCard
* Description	 : �û�ˢ��ȡ��--д��ģʽ
* Input 		 : handle
* Output		 : None
* Return		 : None 
* Notes          : ϵͳ����ʱ�Ŷ��� 
***********************************************************************************************/
void UserTakesWriteCard(uchar8 handle)
{
	static uchar8 HandleStep;
	static uint32 iLastOperationTime;
	static uchar8 TryCount;
	int i = 0;
	uchar8 status = 0;
	uint32 iCardID01 = 0;
	uint32 iAddr = 0;
	uint32 iTime01, iTime02;

	if(0 == handle)
	{
		HandleStep = 0;
		TryCount = 0;
		return;
	}
	
	if((SYS_STATUS_IDLE == gstSysRunInfo.CurrentStatus)||(SYS_STATUS_CARD == gstSysRunInfo.CurrentStatus))
	{
		switch(HandleStep)
		{
			case CardStep_Start:
			{
				if(gstSysRunInfo.u32TimeCount_ms - iLastOperationTime > 220)
				{
					iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
					
					//����
					gstKeyboard.iCardUID = Get_IC_ID_CardSN();
					if(gstKeyboard.iCardUID)
					{
						//������ǲ��ǳ�����
						if(SUPER_CARD_ID == gstKeyboard.iCardUID)
						{
							SuperCard_SetAdmin();
							delay_ms(3000);
							iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
							HandleStep = CardStep_WaitingAway;
							return;
						}
						
						//��ȡ����������
						memset(CardData, 0x0, sizeof(CardData));
						status = RF_ReadCard((uchar8*)&gstKeyboard.iCardUID, SysConfig.stCardKey.u8Sector, AuthKey_A, 
							              (uchar8*)SysConfig.stCardKey.Password, CardData);

						Buzzer(300);
						gstSysRunInfo.CurrentStatus = SYS_STATUS_CARD;

						//������ǲ��ǹ���
						for(i=0; i<MAX_AdminCardNum; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stLockers.AdminCard[i])
							{
								AdminCard_Login();
								return;
							}
						}

						
						//������ǲ��ǼĴ濨
						for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stBox[i].iUserCardID)
							{	
								gstKeyboard.u8Box = i;
								//��ʱȡ�������ж�
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
											TryCount = 0;
											iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
											HandleStep = CardStep_WaitingAway;
											return;
										}
									}
								}

								//������ϵļĴ��¼��Ϣ
								memset(CardData, 0x0, sizeof(CardData));
								iAddr = 0;
								memcpy(&CardData[iAddr], (uchar8*)&SysConfig.stLockers.Name[0], MAX_NameLen);
								iAddr += MAX_NameLen;
								CardData[iAddr++] = gstKeyboard.u8Box;
							    CardData[iAddr++] = USER_TAKEAWAY;
								memcpy(&CardData[iAddr], (uchar8*)&gstSysRunInfo.TimeBuf[0], 6);
								iAddr += 6;
								iAddr += 1;
								CardData[iAddr++] = Calc_CRC(&CardData[0], 15);

								//д��
								status = RF_WriteCard_1Block((uchar8*)&gstKeyboard.iCardUID, (SysConfig.stCardKey.u8Sector *4 +2), 
								                             AuthKey_A, (uchar8*)SysConfig.stCardKey.Password, CardData);


								
								
								GUI_LockersNotice(LOCKERS_OpeningBox);
								status = SelectOneBoxToOpen(i, SysConfig.stLockers.bCheckBoxDoorStatus);;
								if(status)  
								{
									GUI_UserTakeAwwyFail(i);
									Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
									
									//��ס����										
									SysConfig.stBox[i].bLocking = LSTATUS_LOCKING;										
									iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[i].u8Status - (uint32)&SysConfig);
									Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[i].u8Status, 1);
								}
								else
								{	
									//�����ɹ�
									GUI_Open1Box(i, 1);
									VoicePlay_xBoxOpen(i);
									Voice_Play(YY_QuWuTiShi);

									//��������¼�Ŀ���
									SysConfig.stBox[i].iUserCardID = 0;
									iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[i].iUserCardID - (uint32)&SysConfig);
									Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[i].iUserCardID, 4);									

									//������״̬										
									SysConfig.stBox[i].bLocking = LSTATUS_FREE;
									SysConfig.stBox[i].u8Status = BOX_STATUS_IDLE; 									
									iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[i].bLocking - (uint32)&SysConfig);
									Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[i].bLocking, 1);
									iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[i].u8Status - (uint32)&SysConfig);
									Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[i].u8Status, 1);

									//����һ�ʿ����¼
									StoreRecord_OpenBoxByCard(i, gstKeyboard.iCardUID, 2);

									//��������ӵ������б�
									AddOneEmptyBox(i);
								}					
								break;
							}
						}
						if(i > SysConfig.stLockers.u8BoxCount)
						{
							memset((uchar8*)&stUserCard, 0x0, sizeof(ST_USER_CARD));							
							//������ȡ��Ϣ--����´˿��ǲ����ڱ����мĴ�ʱ����ʧ�ܵ�д�˿��Ĵ���Ϣ
							iAddr = 0;
							memcpy((uchar8*)&stUserCard.SysCode[0], &CardData[iAddr], SYS_ID_CODE_LEN);
							iAddr = 32;
							memcpy((uchar8*)&stUserCard.OnLocker[0], &CardData[iAddr], MAX_NameLen);
							iAddr += MAX_NameLen;
							stUserCard.u8OnBox = CardData[iAddr++];
							stUserCard.bStoreOrTake = CardData[iAddr++];
							memcpy((uchar8*)&stUserCard.OnTime[0], &CardData[iAddr], 6);

							if(0 == memcmp((uchar8*)&stUserCard.SysCode[0], (uchar8*)&SysConfig.stClientInfo.IdCode[0], SYS_ID_CODE_LEN))
							{
								if(USER_STORE == stUserCard.bStoreOrTake)
								{
									if(0 == memcmp((uchar8*)stUserCard.OnLocker, (uchar8*)SysConfig.stLockers.Name, MAX_NameLen))
									{
										//������ϵļĴ��¼��Ϣ
										memset(CardData, 0x0, sizeof(CardData));
										status = RF_WriteCard_1Block((uchar8*)&gstKeyboard.iCardUID, (SysConfig.stCardKey.u8Sector *4 +2), 
										                             AuthKey_A, (uchar8*)SysConfig.stCardKey.Password, CardData);
										GUI_LockersNotice(LOCKERS_RepairCard);
									}
									else 
									{
										GUI_PackageInOtherLockers((char*)stUserCard.OnLocker, stUserCard.u8OnBox);	
										Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
									}
									TryCount = 0;
									iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
									HandleStep = CardStep_WaitingAway;
									return;
								}
							}
							
							
							//��ʾ��δ�Ĵ�								
							GUI_LockersNotice(LOCKERS_CardNotConsign);
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						}
						TryCount = 0;
						iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
						HandleStep = CardStep_WaitingAway;
					}
				}
			}
			break;

			case CardStep_WaitingAway: //�ȴ����뿪
			{	
				if(gstSysRunInfo.u32TimeCount_ms - iLastOperationTime > 220)
				{
					iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
					iCardID01 = Get_IC_ID_CardSN();
					if((0 == iCardID01)||(iCardID01 != gstKeyboard.iCardUID))
					{
						TryCount ++;
						if(TryCount >= 2)
						{
							iLastOperationTime = gstSysRunInfo.u32TimeCount_ms - (NOTICE_STAY_TIME /2);
							HandleStep = CardStep_Message;
						}
					}
				}
			}
			break;

			case CardStep_Message:
			{
				if(gstSysRunInfo.u32TimeCount_ms - iLastOperationTime > NOTICE_STAY_TIME)
				{
					GUI_Admin();
					gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;	
					TryCount = 0;
					HandleStep = 0;							
				}
			}
			break;
			
			default:
			{
				HandleStep = CardStep_Start;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : UserTakes_OnlineAsk
* Description	 : �û�ȡ��--��������ģʽ
* Input 		 : handle
* Output		 : None
* Return		 : None 
* Notes          : ϵͳ����ʱ�Ŷ��� 
***********************************************************************************************/
void UserTakes_OnlineAsk(uchar8 handle)
{
	static uchar8 HandleStep;
	static uint32 iLastOperationTime;
	static uchar8 TryCount;
	int i = 0;
	uchar8 status = 0;
	uint32 iCardID01 = 0;
	uint32 iAddr = 0;
	uint32 iTime01, iTime02;

	if(0 == handle)
	{
		HandleStep = 0;
		TryCount = 0;
		return;
	}
	
	if((SYS_STATUS_IDLE == gstSysRunInfo.CurrentStatus)||(SYS_STATUS_CARD == gstSysRunInfo.CurrentStatus))
	{
		switch(HandleStep)
		{
			case CardStep_Start:
			{
				if(gstSysRunInfo.u32TimeCount_ms - iLastOperationTime > 220)
				{
					iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
					//����
					gstKeyboard.iCardUID = Get_IC_ID_CardSN();
					if(gstKeyboard.iCardUID)
					{
						//Buzzer(300);
						gstSysRunInfo.CurrentStatus = SYS_STATUS_CARD;

						//������ǲ��ǳ�����
						if(SUPER_CARD_ID == gstKeyboard.iCardUID)
						{
							SuperCard_SetAdmin();
							delay_ms(3000);
							iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
							HandleStep = CardStep_WaitingAway;
							return;
						}

						//������ǲ��ǹ���
						for(i=0; i<MAX_AdminCardNum; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stLockers.AdminCard[i])
							{
								AdminCard_Login();
								return;
							}
						}
						
						//������ǲ��ǼĴ濨
						for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
						{
							WDT_Feed();
							if(gstKeyboard.iCardUID == SysConfig.stBox[i].iUserCardID)
							{
								gstKeyboard.u8Box = i;
								
								//��ʱȡ�������ж�
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
											TryCount = 0;
											iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
											HandleStep = CardStep_WaitingAway;
											return;
										}
									}
								}

								//�����������ȡ��
								GUI_ShowNotice(NOTICE_PLS_WAIT);
								memset((uchar8*)&stOnlineAsk, 0x0, sizeof(ST_ONLINEE_ASK));
								HandleStep = CardStep_OnlineAsk;
								return;
							}
						}
						if(i > SysConfig.stLockers.u8BoxCount)
						{
							//��ʾ��δ�Ĵ�								
							GUI_LockersNotice(LOCKERS_CardNotConsign);
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						}
						//���������ѯ���ĸ���
						gstKeyboard.u8Box = 0;
						GUI_ShowNotice(NOTICE_PLS_WAIT);
						memset((uchar8*)&stOnlineAsk, 0x0, sizeof(ST_ONLINEE_ASK));
						HandleStep = CardStep_OnlineAsk;
					}
				}
			}
			break;

			case CardStep_OnlineAsk:
			{
				stOnlineAsk.bReplyOk = 0;
				stOnlineAsk.iCardID = gstKeyboard.iCardUID;
				stOnlineAsk.iWaitingTime = gstSysRunInfo.u32TimeCount_ms;
				UserTakeAway_OnlineAsk(gstKeyboard.iCardUID, gstKeyboard.u8Box);
				HandleStep = CardStep_WaitServerReply;			 
			}
			break;

			case CardStep_WaitServerReply:
			{
				if(gstSysRunInfo.u32TimeCount_ms - stOnlineAsk.iWaitingTime < WAITING_TIMEOUT)
				{
					if(SERVER_REPLY_MASK == stOnlineAsk.bReplyOk)
					{
						if((ALLOW_CONSIGN == stOnlineAsk.bAllowConsign)&&(gstKeyboard.u8Box))
						{
						    //����ȡ��
							GUI_LockersNotice(LOCKERS_OpeningBox);
							status = SelectOneBoxToOpen(gstKeyboard.u8Box, SysConfig.stLockers.bCheckBoxDoorStatus);;
							if(status)  
							{
								GUI_UserTakeAwwyFail(gstKeyboard.u8Box);
								Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
								
								//��ס����										
								SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_LOCKING;										
								iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].u8Status - (uint32)&SysConfig);
								Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].u8Status, 1);
							}
							else
							{	
								//�����ɹ�
								GUI_Open1Box(gstKeyboard.u8Box, 1);
								VoicePlay_xBoxOpen(gstKeyboard.u8Box);
								Voice_Play(YY_QuWuTiShi);

								//��������¼�Ŀ���
								SysConfig.stBox[gstKeyboard.u8Box].iUserCardID = 0;
								iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].iUserCardID - (uint32)&SysConfig);
								Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].iUserCardID, 4);									

								//������״̬			
								SysConfig.stBox[gstKeyboard.u8Box].bLocking = LSTATUS_FREE;
								SysConfig.stBox[gstKeyboard.u8Box].u8Status = BOX_STATUS_IDLE; 									
								iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].bLocking - (uint32)&SysConfig);
								Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].bLocking, 1);
								iAddr = Addr_SysConfig + ((uint32)&SysConfig.stBox[gstKeyboard.u8Box].u8Status - (uint32)&SysConfig);
								Store_Parameters(iAddr, (uchar8*)&SysConfig.stBox[gstKeyboard.u8Box].u8Status, 1);

								//����һ�ʿ����¼
								StoreRecord_OpenBoxByCard(gstKeyboard.u8Box, gstKeyboard.iCardUID, 2);

								//��������ӵ������б�
								AddOneEmptyBox(gstKeyboard.u8Box);
							}
	
						}
//						else if
//						{
//							for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
//							{
//								if(gstKeyboard.iCardUID == SysConfig.stBox[i].iUserCardID)
//								{
//									gstKeyboard.u8Box=i;
//								}
//							}
//						}	
						else if(NOTALLOW_CONSIGN == stOnlineAsk.bAllowConsign)
						{
							GUI_PackageInOtherLockers((char*)stOnlineAsk.OnLockers, stOnlineAsk.u8OnBox);	
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						}
						else
						{
							GUI_ShowMessage((char*)stOnlineAsk.Notice, LCD_Color_Red);	
							Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						}
						TryCount = 0;
						iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
						HandleStep = CardStep_WaitingAway;
				}
				else
				{
					stOnlineAsk.u8TryCnt ++;
					if(stOnlineAsk.u8TryCnt < 3)
					{
						//δ�յ�������Ӧ��,�ٴγ���	
						HandleStep = CardStep_OnlineAsk;
					}
					else
					{
						//���������Ӧ�� 
						GUI_LockersNotice(LOCKERS_NetError);
						//Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
						HandleStep = CardStep_WaitingAway;
					}
				}
			}
		  }
			break;
			
			case CardStep_WaitingAway: //�ȴ����뿪
			{	
				if(gstSysRunInfo.u32TimeCount_ms - iLastOperationTime > 220)
				{
					iLastOperationTime = gstSysRunInfo.u32TimeCount_ms;
					iCardID01 = Get_IC_ID_CardSN();
					if((0 == iCardID01)||(iCardID01 != gstKeyboard.iCardUID))
					{
						TryCount ++;
						if(TryCount >= 2)
						{
							HandleStep = CardStep_Message;
						}
					}
				}
			}
			break;

			case CardStep_Message:
			{
				if(gstSysRunInfo.u32TimeCount_ms - iLastOperationTime > 1234)
				{
					if(VALIDITY_USING == SysConfig.stLockers.bEnableIdCard)
					{
						delay_ms(1000);
						Get_IC_ID_CardSN(); //�ͷ���ʱ�����д���Ŀ���
					}
					GUI_Admin();
					gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;	
					TryCount = 0;
					HandleStep = 0;							
				}
			}
			break;
			
			default:
			{
				HandleStep = CardStep_Start;
			}
			break;
		}
	}
}

/***********************************************************************************************
* Function Name  : CardOperation
* Description	 : ˢ��ȡ����������
* Input 		 : handle
* Output		 : None
* Return		 : None 
* Notes          : ϵͳ����ʱ�Ŷ��� 
***********************************************************************************************/
void CardOperation(uchar8 handle)
{
	if(WORK_MODE_WRITE_CARD == SysConfig.stLockers.u8WorkMode)
	{
		UserTakesWriteCard(handle);
	}
	if(WORK_MODE_ONLINE == SysConfig.stLockers.u8WorkMode)
	{

		UserTakes_OnlineAsk(handle);
	}
	else
	{
		UserTakesByCardID(handle);
	}
}

/*===================================File end===================================================*/


