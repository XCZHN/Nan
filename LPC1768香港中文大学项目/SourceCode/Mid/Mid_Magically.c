/*===============================================================================================
==                                                                                              
**  Copyright(c) 2015  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Mid_Magically.c                                                                     
==                                                                                         
**  Function:	
==                                                                                         
**  Data:       2015/09/18                                                                     
================================================================================================*/

#include "Includes.h"

struct STimer *pTimer_LedBlink = NULL;
struct STimer *pTimer_Beep = NULL;
struct STimer *pTimer_ReadRTC = NULL;
struct STimer *pTimer_OpenBox = NULL;
	
/*******************************************************************************
* Function Name  : SetLedBlink
* Description    : ����ָʾ����˸Ƶ�ʺʹ���
* Input          : u32Tick_ms -- Ƶ��(ms), u32Times -- ����
* Output         : None
* Return         : None
*******************************************************************************/
void SetLedBlink(uint32 u32Tick_ms, uint32 u32Times)
{
	if(0xFFFFFFFF == u32Times)
	{
		gstSysRunInfo.u32LedBlinkTimes = u32Times;		 
	}
	else
	{
		gstSysRunInfo.u32LedBlinkTimes = u32Times *2;	
	}
	pTimer_LedBlink->Timeout = u32Tick_ms / 2;
	pTimer_LedBlink->State = 1;
}

/*******************************************************************************
* Function Name  : SignalLedBlink
* Description    : ָʾ����˸
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SignalLedBlink(void *Arg)
{
	static uchar8 bLedON;
	
	if(gstSysRunInfo.u32LedBlinkTimes)
	{ 
		if(bLedON)
		{
			LED1_ON;
		}
		else
		{
			LED1_OFF;
		}
		bLedON = !bLedON;
		if(0xFFFFFFFF != gstSysRunInfo.u32LedBlinkTimes)
		{
			gstSysRunInfo.u32LedBlinkTimes --;
		}
	}
	else
	{
		LED1_OFF;
		pTimer_LedBlink->State = 0;
	}
}

BOOL BeepFlag = FALSE;
BOOL BeepState = 0;
uint32 OnTick, iOnTick, OffTick, iOffTick, Retries;

/***********************************************************************************************
* Function Name  : Beep
* Description	 : ������� 
* Input 		 : OnTickVal -- ����������ʱ��,OffTickVal--�������ص�ʱ��, Times--��Ĵ���
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Beep(uint32 OnTickVal, uint32 OffTickVal, uint32 Times)
{
	if((0 == OnTickVal )||(0 == Times))
	{
		BEEP_OFF;
		BeepFlag = FALSE;
		return;
	}
	OnTick = OnTickVal;
	OffTick = OffTickVal;
	Retries = Times;

	iOnTick = 0;
	iOffTick = 0;
	//TimerSet(pBeepTimer, TRUE, TRUE, TICK_COUNT_200MS);
	pTimer_Beep->TimeCount = pTimer_Beep->Timeout;
	BeepState = 1;
	BEEP_ON;
	BeepFlag = TRUE;
}

/***********************************************************************************************
* Function Name  : BeepCallback
* Description	 : ��������ص�ִ�к���(��ʱ������)
* Input 		 : 
* Output		 : None
* Return		 : None
***********************************************************************************************/
void BeepCallback(void *Arg)
{
	if(BeepFlag == TRUE)
	{
		if(BeepState == 1)
		{
			iOnTick++;
			if(iOnTick >= OnTick)
			{
				BEEP_OFF;
				BeepState = 0;
			}		
		}
		else
		{
			iOffTick++;
			if(iOffTick >= OffTick)
			{
				Retries--;
				if(Retries == 0)
				{
					BeepFlag = FALSE;
					return;
				}
				else
				{
					BEEP_ON;
					BeepState = 1;
					iOnTick = 0;
					iOffTick = 0;
				}
			}
		}	
	}
}

/***********************************************************************************************
* Function Name  : Buzzer
* Description	 : �������
* Input 		 : time_ms -- ���ʱ��(ms)
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Buzzer(uint32 time_ms)
{
	Beep((time_ms / TICK_COUNT_10MS), 0, 1);
}


/*******************************************************************************
* Function Name  : Tick_ReadRTC
* Description    : ��ʱ��ȡRTCʱ��
* Input          : None
* Output         : None
* Return         : None
* Notes          : ֻ���ñ�־gbRTC_TimeUpdate = 1,��ʾ��Ҫ��ȡʱ����
*******************************************************************************/
void Tick_ReadRTC(void *Arg)
{
	gbRTC_TimeUpdate = 1;
}

/***********************************************************************************************
* Function Name  : CheckRegistrationCode
* Description	 : У��ע����
* Input 		 : pRegistrationCode -- ע���� 
* Output		 : None
* Return		 : 0xEB -- ע������Ч
***********************************************************************************************/
uchar8 CheckRegistrationCode(uchar8* pRegistrationCode)
{
	uchar8 Today[8] = {0};
	uchar8 StopDate[8] = {0};
	uint32 iData = 0;
	uchar8 Code[20] = {0};

	Ascii_To_Hex(Code, pRegistrationCode, 10);

	//����
	for(iData=0; iData<5; iData++)
	{
		Code[iData] ^= 0x05;
	}

	//У��crc
	Code[10] = Code[0] ^ Code[1] ^ Code[2] ^ Code[3];
	if(Code[10] != Code[4])
	{
		return 0xE0;
	}

	iData = Code[0]*256 + Code[1];
	Today[0] = (iData >> 9) & 0xFF;
	Today[1] = ((iData & 0x01E0)>>5) & 0xFF;
	Today[2] = (iData & 0x1F);

	iData = Code[2]*256 + Code[3];
	StopDate[0] = (iData >> 9) & 0xFF;
	StopDate[1] = ((iData & 0x01E0)>>5) & 0xFF;
	StopDate[2] = (iData & 0x1F);

	//�ж�ʱ�����Ч��
	if((Today[0] != gstSysRunInfo.stRtcTime.u8Year)||(Today[1] != gstSysRunInfo.stRtcTime.u8Month)||(Today[2] != gstSysRunInfo.stRtcTime.u8Day))
	{
		return 0xE1;		
	}

	SysConfig.stClientInfo.ValidityDate[0] = StopDate[0];
	SysConfig.stClientInfo.ValidityDate[1] = StopDate[1];
	SysConfig.stClientInfo.ValidityDate[2] = StopDate[2];

	iData = Addr_SysConfig + ((uint32)&SysConfig.stClientInfo.ValidityDate[0] - (uint32)&SysConfig);
	Store_Parameters(iData, (uchar8*)&SysConfig.stClientInfo.ValidityDate[0], sizeof(SysConfig.stClientInfo.ValidityDate));
	
	return 0xEB;
}

/*========================File end======================================================================*/

