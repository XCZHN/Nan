/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_RTC.h                                                                       
==                                                                                         
**  Function:	RTC时钟模块驱动层程序
==                                                                                         
**  Data:       2014/08/11                                                                          
================================================================================================*/

#ifndef	_DRV_RTC_H_
#define	_DRV_RTC_H_

#include "Config.h"
#include "LPC17xx.h"

/* Private define ------------------------------------------------------------*/

typedef struct								//日期时间结构体
{
	uint8  u8Year;							//=实际年份 - 2000				
	uint8  u8Month;
	uint8  u8Day;
	uint8  u8Hour;
	uint8  u8Minute;
	uint8  u8Second;
	uint8  u8WeekDay;
	
}ST_TIME, *pST_TIME;

extern uchar8 gbRTC_TimeUpdate;

/***********************************************************************************************
* Function Name  : Chack_Date
* Description	 : 检测日期的有效性
* Input 		 : uchar8 u8Year 年, uchar8 u8Month 月, uchar8 u8Day 日
* Output		 : None
* Return		 : 0--合法的时间 ， 非0--非法时间
***********************************************************************************************/
extern int  Chack_Date(uchar8 u8Year , uchar8 u8Month , uchar8 u8Day);

/***********************************************************************************************
* Function Name  : Chack_Time
* Description	 : 检测时间的有效性
* Input 		 : uchar8 Hour 时, uchar8 Minute 分, uchar8 Second 秒
* Output		 : None
* Return		 : 0--合法的时间 ， 非0--非法时间
***********************************************************************************************/
extern int  Chack_Time(uchar8 u8Hour , uchar8 u8Minute , uchar8 u8Second);

/***********************************************************************************************
* Function Name  : Calc_Weekday
* Description    : 计算某一天是星期几
* Input          : iYear -- 年, iMonth -- 月, iDay -- 日
* Output         : None
* Return         : 0~7 分别对应: 星期天~星期六
***********************************************************************************************/
extern int Calc_Weekday(int iYear, int iMonth, int iDay);

/***********************************************************************************************
* Function Name  : RTC_Init
* Description    : RTC初始化
* Input          : None
* Output         : None
* Return         : None
***********************************************************************************************/
extern void RTC_Init(void);

/***********************************************************************************************
* Function Name  : RTC_GetDateTime
* Description    : 读取RTC的日期和时间
* Input          : pRtcTime -- 时间数据存放指针
* Output         : YY-MM-DD hh:mm:ss DayOfWeek (7Byte)
* Return         : 0 -- 读取成功, 其它 -- 错误代码(详见错误表)
***********************************************************************************************/
extern int RTC_GetDateTime(pST_TIME pRtcTime);

/***********************************************************************************************
* Function Name  : RTC_SetDateTime
* Description    : 设置RTC的日期和时间
* Input          : pRtcTime -- 时间数据存放指针
* Output         : YY-MM-DD hh:mm:ss DayOfWeek (7Byte)
* Return         : 0 -- 设置成功, 其它 -- 错误代码(详见错误表)
***********************************************************************************************/
extern int RTC_SetDateTime(pST_TIME pRtcTime);


#endif
/*=============================File end========================================================*/
