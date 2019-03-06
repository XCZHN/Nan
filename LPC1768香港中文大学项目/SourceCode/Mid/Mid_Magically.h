/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Mid_Magically.h                                                                       
==                                                                                         
**  Function:	 
==                                                                                         
**  Data:       2015/09/18                                                                       
================================================================================================*/

#ifndef	_MID_MAGICAL_H_
#define	_MID_MAGICAL_H_

#include "Config.h"   


//TimerCreate(TRUE, TRUE, TICK_COUNT_10MS, BeepCallback, NULL);
#define BEEP_TIME_50MS  	5
#define BEEP_TIME_100MS  	10
#define BEEP_TIME_200MS		20
#define BEEP_TIME_500MS		100
#define BEEP_TIME_1S		200


extern struct STimer *pTimer_LedBlink;
extern struct STimer *pTimer_Beep;
extern struct STimer *pTimer_ReadRTC;
extern struct STimer *pTimer_OpenBox;

/*******************************************************************************
* Function Name  : SetLedBlink
* Description    : ����ָʾ����˸Ƶ�ʺʹ���
* Input          : u32Tick_ms -- Ƶ��(ms), u32Times -- ����
* Output         : None
* Return         : None
*******************************************************************************/
extern void SetLedBlink(uint32 u32Tick_ms, uint32 u32Times);

/*******************************************************************************
* Function Name  : SignalLedBlink
* Description    : ָʾ����˸
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void SignalLedBlink(void *Arg);


/***********************************************************************************************
* Function Name  : Beep
* Description	 : ������� 
* Input 		 : OnTickVal -- ����������ʱ��,OffTickVal--�������ص�ʱ��, Times--��Ĵ���
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Beep(uint32 OnTickVal, uint32 OffTickVal, uint32 Times);

/***********************************************************************************************
* Function Name  : BeepCallback
* Description	 : ��������ص�ִ�к���(��ʱ������)
* Input 		 : 
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void BeepCallback(void *Arg);

/***********************************************************************************************
* Function Name  : Buzzer
* Description	 : �������
* Input 		 : time_ms -- ���ʱ��(ms)
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Buzzer(uint32 time_ms);

/*******************************************************************************
* Function Name  : Tick_ReadRTC
* Description    : ��ʱ��ȡRTCʱ��
* Input          : None
* Output         : None
* Return         : None
* Notes          : ֻ���ñ�־gbRTC_TimeUpdate = 1,��ʾ��Ҫ��ȡʱ����
*******************************************************************************/
extern void Tick_ReadRTC(void *Arg);

/***********************************************************************************************
* Function Name  : CheckRegistrationCode
* Description	 : У��ע����
* Input 		 : pRegistrationCode -- ע���� 
* Output		 : None
* Return		 : 0xEB -- ע������Ч
***********************************************************************************************/
extern uchar8 CheckRegistrationCode(uchar8* pRegistrationCode);

#endif
/*========================File end======================================================================*/

