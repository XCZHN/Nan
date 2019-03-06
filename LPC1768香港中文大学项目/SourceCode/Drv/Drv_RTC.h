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
**  Function:	RTCʱ��ģ�����������
==                                                                                         
**  Data:       2014/08/11                                                                          
================================================================================================*/

#ifndef	_DRV_RTC_H_
#define	_DRV_RTC_H_

#include "Config.h"
#include "LPC17xx.h"

/* Private define ------------------------------------------------------------*/

typedef struct								//����ʱ��ṹ��
{
	uint8  u8Year;							//=ʵ����� - 2000				
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
* Description	 : ������ڵ���Ч��
* Input 		 : uchar8 u8Year ��, uchar8 u8Month ��, uchar8 u8Day ��
* Output		 : None
* Return		 : 0--�Ϸ���ʱ�� �� ��0--�Ƿ�ʱ��
***********************************************************************************************/
extern int  Chack_Date(uchar8 u8Year , uchar8 u8Month , uchar8 u8Day);

/***********************************************************************************************
* Function Name  : Chack_Time
* Description	 : ���ʱ�����Ч��
* Input 		 : uchar8 Hour ʱ, uchar8 Minute ��, uchar8 Second ��
* Output		 : None
* Return		 : 0--�Ϸ���ʱ�� �� ��0--�Ƿ�ʱ��
***********************************************************************************************/
extern int  Chack_Time(uchar8 u8Hour , uchar8 u8Minute , uchar8 u8Second);

/***********************************************************************************************
* Function Name  : Calc_Weekday
* Description    : ����ĳһ�������ڼ�
* Input          : iYear -- ��, iMonth -- ��, iDay -- ��
* Output         : None
* Return         : 0~7 �ֱ��Ӧ: ������~������
***********************************************************************************************/
extern int Calc_Weekday(int iYear, int iMonth, int iDay);

/***********************************************************************************************
* Function Name  : RTC_Init
* Description    : RTC��ʼ��
* Input          : None
* Output         : None
* Return         : None
***********************************************************************************************/
extern void RTC_Init(void);

/***********************************************************************************************
* Function Name  : RTC_GetDateTime
* Description    : ��ȡRTC�����ں�ʱ��
* Input          : pRtcTime -- ʱ�����ݴ��ָ��
* Output         : YY-MM-DD hh:mm:ss DayOfWeek (7Byte)
* Return         : 0 -- ��ȡ�ɹ�, ���� -- �������(��������)
***********************************************************************************************/
extern int RTC_GetDateTime(pST_TIME pRtcTime);

/***********************************************************************************************
* Function Name  : RTC_SetDateTime
* Description    : ����RTC�����ں�ʱ��
* Input          : pRtcTime -- ʱ�����ݴ��ָ��
* Output         : YY-MM-DD hh:mm:ss DayOfWeek (7Byte)
* Return         : 0 -- ���óɹ�, ���� -- �������(��������)
***********************************************************************************************/
extern int RTC_SetDateTime(pST_TIME pRtcTime);


#endif
/*=============================File end========================================================*/
