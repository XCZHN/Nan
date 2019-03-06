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
* Description    : 设置指示灯闪烁频率和次数
* Input          : u32Tick_ms -- 频率(ms), u32Times -- 次数
* Output         : None
* Return         : None
*******************************************************************************/
extern void SetLedBlink(uint32 u32Tick_ms, uint32 u32Times);

/*******************************************************************************
* Function Name  : SignalLedBlink
* Description    : 指示灯闪烁
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void SignalLedBlink(void *Arg);


/***********************************************************************************************
* Function Name  : Beep
* Description	 : 响蜂鸣器 
* Input 		 : OnTickVal -- 蜂鸣器开的时间,OffTickVal--蜂鸣器关的时间, Times--响的次数
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Beep(uint32 OnTickVal, uint32 OffTickVal, uint32 Times);

/***********************************************************************************************
* Function Name  : BeepCallback
* Description	 : 响蜂鸣器回调执行函数(定时器处理)
* Input 		 : 
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void BeepCallback(void *Arg);

/***********************************************************************************************
* Function Name  : Buzzer
* Description	 : 响蜂鸣器
* Input 		 : time_ms -- 响的时间(ms)
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Buzzer(uint32 time_ms);

/*******************************************************************************
* Function Name  : Tick_ReadRTC
* Description    : 定时读取RTC时钟
* Input          : None
* Output         : None
* Return         : None
* Notes          : 只是置标志gbRTC_TimeUpdate = 1,表示需要读取时间了
*******************************************************************************/
extern void Tick_ReadRTC(void *Arg);

/***********************************************************************************************
* Function Name  : CheckRegistrationCode
* Description	 : 校验注册码
* Input 		 : pRegistrationCode -- 注册码 
* Output		 : None
* Return		 : 0xEB -- 注册码有效
***********************************************************************************************/
extern uchar8 CheckRegistrationCode(uchar8* pRegistrationCode);

#endif
/*========================File end======================================================================*/

