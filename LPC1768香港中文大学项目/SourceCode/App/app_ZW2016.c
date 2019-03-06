/*===============================================================================================
==                                                                                              
**  Copyright(c) 2015  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	App_SZZDL.c                                                                       
==                                                                                         
**  Function:	�����Ƕ������ܼ����ɷ����޹�˾ ָ�ƼĴ��Ӧ�ó���                                                               
==
**  Data:       2015-12-05
================================================================================================*/

#include "Includes.h"

/*===============================================================================================
����汾�޸ļ�¼:

=��= V1.0.0 =��=  
1. ��ʼ�汾

================================================================================================*/



//-----------------------------------���������--------------------------------------/
typedef enum 							    
{
	HandleStep_Start = 0,				//��ʼִ��	
	HandleStep_Message,					//��ʾ��Ϣ	
	HandleStep_JiCunKey,                //�Ĵ������
	HandleStep_AwaitFingerprint,		//�ȴ���ָ��ȡ��
	HandleStep_GetFingerprint, 			//��ָ��ȡ��
	HandleStep_JiCunOpenBox,		    //�Ĵ濪�����
	HandleStep_JiCunBoxCheck,           //���ż��
	HandleStep_QuWuOpenBox, 			//ȡ�￪�����
	HandleStep_QuWuBoxCheck,			//���ż��
	HandleStep_JiCunByCard,             //ˢ���Ĵ�
	HandleStep_WriteRecord,             //д��¼��Ϣ
	HandleStep_WaitingCardAway,         //�ȴ����뿪
	HandleStep_GetCard,                 //����ˢ��
	HandleStep_
	
}E_HANDLE_STEP;


typedef	struct								 
{
	uint32 iCardNum;             //����
	uint32 iLastTime;            //�ϴ�ִ��ʱ��

	uchar8 HandleStep;           //ִ�й켣
	uchar8 BoxNum;               //���  
	uchar8 TryCout; 			 //���Լ���
	
}ST_LOCKERS_OPERATION;

volatile ST_LOCKERS_OPERATION stLockersOperation;

/***********************************************************************************************
* Function Name  : FingerprintLockers_Init
* Description	 : ָ������洢���ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : ���б����ĳ�ʼ��
***********************************************************************************************/
void  FingerprintLockers_Init(void)
{
	if((HandleStep_JiCunOpenBox == stLockersOperation.HandleStep)||(HandleStep_JiCunBoxCheck == stLockersOperation.HandleStep)
		||(HandleStep_QuWuOpenBox == stLockersOperation.HandleStep)||(HandleStep_QuWuBoxCheck == stLockersOperation.HandleStep))
	{
		//���⼸��״̬�²������˳�����!
		return;
	}
	else
	{
		if(stLockersOperation.HandleStep)
		{
			GUI_Admin();
		}
		memset((uchar8*)&stLockersOperation, 0x0, sizeof(ST_LOCKERS_OPERATION));
		
		CloseAllBox();
		Fingerprint_Search(ZW_StopHandle);
		Fingerprint_Login(ZW_StopHandle);
		
	}
}

/***********************************************************************************************
* Function Name  : AdminFingerprint_Login
* Description	 : ����Ա��ָ�Ƶ�¼
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : �൱�ڴ�������"#-->*-->000-->Enter"
***********************************************************************************************/
void AdminFingerprint_Login(void)
{	
	gstKeyboard.u8AdminMenuStep = 4;
	gstKeyboard.u8SecondMenuStep = 4;
	gstSysRunInfo.CurrentStatus = SYS_STATUS_KEY;
	gstKeyboard.iLastKeyTime = gstSysRunInfo.u32TimeCount_ms;
	User_Login(0xCC);
}

/***********************************************************************************************
* Function Name  : JiCunKey_Login
* Description	 : ���¼Ĵ����Ĳ���
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : None
***********************************************************************************************/
void  JiCunKey_Login(void)
{
	gstSysRunInfo.CurrentStatus = SYS_STATUS_JiCun;
	stLockersOperation.HandleStep = HandleStep_JiCunKey;
}

/***********************************************************************************************
* Function Name  : Lockers_FixedBox
* Description	 : ָ�ƹ̶���洢��
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : ��ָ�ƿ���
***********************************************************************************************/
void Lockers_FixedBox(void)
{
	int i = 0, j = 0;
	uint16 iRet = 0;
	static uchar8 bScreen;
	
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
						Fingerprint_PowerOn();
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
				//GUI_ShowMessage("ָ�ƶ�ȡ��ʱ!", LCD_Color_Blue);
				//Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
				stLockersOperation.HandleStep = HandleStep_Start;
			}
			else
			{
				iRet = Fingerprint_Search(0);
				if(0xE000 == iRet) //�������󴥷�
				{
					gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;	
					stLockersOperation.HandleStep = HandleStep_Start;
				}
				else if((iRet >= 0xA000)&&(iRet <= 0xA00F))
				{
					//������������
				}
				else if(iRet < MAX_ZW_UNM)
				{
					//�ҵ�ƥ���ָ�ƺ�
					//Buzzer(330);
					for(j=0; j<MAX_UserFingerprintNum; j++)
					{
						if(SysConfig.stBox[0].TouchID[j] == iRet)
						{
							FingerprintLockers_Init();
							AdminFingerprint_Login();
							return;
						}
					}
					
					for(i=1; i<=SysConfig.stLockers.iBoxCount; i++)
					{
						for(j=0; j<MAX_UserFingerprintNum; j++)
						{
							if(SysConfig.stBox[i].TouchID[j] == iRet)
							{
								stLockersOperation.BoxNum = i;
								stLockersOperation.TryCout = 0;
								stLockersOperation.HandleStep = HandleStep_QuWuOpenBox;
								return;
							}
						}
					}

					if(i > SysConfig.stLockers.iBoxCount)
					{
						GUI_ShowMessage("��ָ��δ������!", LCD_Color_Blue);
						bScreen = 1;
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
						stLockersOperation.HandleStep = HandleStep_Message;
					}
				}
				else if(iRet > 0xE000)
				{
					if(0xFFFF == iRet)
					{
						//Voice_Play(YY_ZhiWenWeiShouQuan);						
						GUI_ShowMessage("����ָδ��Ȩ!", LCD_Color_Red);
						Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
						bScreen = 1;
					}
					stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
					stLockersOperation.HandleStep = HandleStep_Message;
				}
			}
		}
		break;

		case HandleStep_QuWuOpenBox:
		{		
			if(BOX_STATUS_BLOCKING == SysConfig.stBox[stLockersOperation.BoxNum].u8Status)
			{
				GUI_BoxByLocking(stLockersOperation.BoxNum);
				Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);	
				stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
				stLockersOperation.HandleStep = HandleStep_Message;
			}
			else
			{
				GUI_ShowUserOpenBox(stLockersOperation.BoxNum);
				VoicePlay_OpenDoor(stLockersOperation.BoxNum);
				Choice1BoxOpen(stLockersOperation.BoxNum);
				stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
				stLockersOperation.HandleStep = HandleStep_WriteRecord;
			}
			bScreen = 1;
		}
		break;

		case HandleStep_WriteRecord:
		{						
			//���濪���¼			
			memset((uchar8*)&stRecord, 0x0, sizeof(ST_RECORD)); 		
			stRecord.BoxNum = stLockersOperation.BoxNum;
			stRecord.u8Type = OpenBoxType_Fingerprint;
			stRecord.u8SaveOrTake = 0; 
			stRecord.HandleInfo[0] = stLockersOperation.BoxNum;
			StoreOpenBoxRecord(&stRecord);			

			stLockersOperation.iLastTime = gstSysRunInfo.u32TimeCount_ms;
			stLockersOperation.HandleStep = HandleStep_Message;					
		}
		break;
		
		case HandleStep_Message:
		{
			if((gstSysRunInfo.u32TimeCount_ms - stLockersOperation.iLastTime > 2123)&&(GET_Fingerprint_STATE))
			{
				if(SYS_STATUS_KEY != gstSysRunInfo.CurrentStatus)
				{
					if(bScreen)
					{
						bScreen = 0;
						GUI_Admin();
					}
					gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;	
				}
				stLockersOperation.HandleStep = HandleStep_Start;
				Fingerprint_PowerOff();
			}			
			else
			{
				//��ֹ��ָ��ͷ�ź����쳣��һֱ��������
				if(gstSysRunInfo.u32TimeCount_ms - stLockersOperation.iLastTime > 3210)
				{
					if(bScreen)
					{
						bScreen = 0;
						GUI_Admin();
					}
					stLockersOperation.HandleStep = HandleStep_Start;
					gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;
					Fingerprint_PowerOff();
				}
				
				//Ԥ����ָ��ͷ����һֱͣ���ڴ���ָ�Ƶ���������ɰ���\ˢ���ȹ���ʧЧ!
				if((SYS_STATUS_JiCun == gstSysRunInfo.CurrentStatus)||(SYS_STATUS_QuWu == gstSysRunInfo.CurrentStatus))
				{
					gstSysRunInfo.CurrentStatus = SYS_STATUS_Message;
				}
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

/***********************************************************************************************
* Function Name  : Lockers_OpenBox
* Description	 : ָ�ƴ洢�������
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 0.ϵͳ����ʱ�Ŷ���--���ϵͳ��
***********************************************************************************************/
void  Lockers_OpenBox(void)
{
	Lockers_FixedBox();
}





/*===================================File end===================================================*/


