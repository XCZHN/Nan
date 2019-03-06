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
**  Function:	深圳智动力精密技术股份有限公司 指纹寄存柜应用程序                                                               
==
**  Data:       2015-12-05
================================================================================================*/

#include "Includes.h"

/*===============================================================================================
程序版本修改记录:

=★= V1.0.0 =★=  
1. 初始版本

================================================================================================*/



//-----------------------------------操作步骤表--------------------------------------/
typedef enum 							    
{
	HandleStep_Start = 0,				//开始执行	
	HandleStep_Message,					//显示消息	
	HandleStep_JiCunKey,                //寄存键按下
	HandleStep_AwaitFingerprint,		//等待按指纹取件
	HandleStep_GetFingerprint, 			//按指纹取件
	HandleStep_JiCunOpenBox,		    //寄存开箱操作
	HandleStep_JiCunBoxCheck,           //箱门检测
	HandleStep_QuWuOpenBox, 			//取物开箱操作
	HandleStep_QuWuBoxCheck,			//箱门检测
	HandleStep_JiCunByCard,             //刷卡寄存
	HandleStep_WriteRecord,             //写记录信息
	HandleStep_WaitingCardAway,         //等待卡离开
	HandleStep_GetCard,                 //请求刷卡
	HandleStep_
	
}E_HANDLE_STEP;


typedef	struct								 
{
	uint32 iCardNum;             //卡号
	uint32 iLastTime;            //上次执行时间

	uchar8 HandleStep;           //执行轨迹
	uchar8 BoxNum;               //箱号  
	uchar8 TryCout; 			 //尝试计数
	
}ST_LOCKERS_OPERATION;

volatile ST_LOCKERS_OPERATION stLockersOperation;

/***********************************************************************************************
* Function Name  : FingerprintLockers_Init
* Description	 : 指纹随机存储柜初始化
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 所有变量的初始化
***********************************************************************************************/
void  FingerprintLockers_Init(void)
{
	if((HandleStep_JiCunOpenBox == stLockersOperation.HandleStep)||(HandleStep_JiCunBoxCheck == stLockersOperation.HandleStep)
		||(HandleStep_QuWuOpenBox == stLockersOperation.HandleStep)||(HandleStep_QuWuBoxCheck == stLockersOperation.HandleStep))
	{
		//在这几种状态下不可以退出操作!
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
* Description	 : 管理员按指纹登录
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 相当于代替输入"#-->*-->000-->Enter"
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
* Description	 : 按下寄存键后的操作
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
* Description	 : 指纹固定箱存储柜
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 按指纹开箱
***********************************************************************************************/
void Lockers_FixedBox(void)
{
	int i = 0, j = 0;
	uint16 iRet = 0;
	static uchar8 bScreen;
	
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
				//读取指纹超时
				Fingerprint_Search(ZW_StopHandle);
				//GUI_ShowMessage("指纹读取超时!", LCD_Color_Blue);
				//Beep(BEEP_TIME_100MS, BEEP_TIME_100MS, 3);
				stLockersOperation.HandleStep = HandleStep_Start;
			}
			else
			{
				iRet = Fingerprint_Search(0);
				if(0xE000 == iRet) //可能是误触发
				{
					gstSysRunInfo.CurrentStatus = SYS_STATUS_IDLE;	
					stLockersOperation.HandleStep = HandleStep_Start;
				}
				else if((iRet >= 0xA000)&&(iRet <= 0xA00F))
				{
					//正在跑流程中
				}
				else if(iRet < MAX_ZW_UNM)
				{
					//找到匹配的指纹号
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
						GUI_ShowMessage("此指纹未分配箱!", LCD_Color_Blue);
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
						GUI_ShowMessage("此手指未授权!", LCD_Color_Red);
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
			//保存开箱记录			
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
				//防止因指纹头信号线异常而一直卡在这里
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
				
				//预防因指纹头问题一直停留在处理指纹的流程上造成按键\刷卡等功能失效!
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
* Description	 : 指纹存储柜开箱操作
* Input 		 : None
* Output		 : None
* Return		 : None 
* Notes          : 0.系统空闲时才读卡--检查系统卡
***********************************************************************************************/
void  Lockers_OpenBox(void)
{
	Lockers_FixedBox();
}





/*===================================File end===================================================*/


