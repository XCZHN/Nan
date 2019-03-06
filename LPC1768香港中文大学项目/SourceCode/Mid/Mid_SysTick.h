/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Mid_SysTick.h                                                                     
==                                                                                         
**  Function:	ϵͳ�δ�ʱ�Ӵ�������
==                                                                                         
**  Data:       2014/08/15                                                                      
================================================================================================*/

#ifndef	_APP_SYS_TICK_H_
#define	_APP_SYS_TICK_H_


#include "Config.h"
#include "LPC17xx.h"


#define TICK_COUNT_1MS			1
#define TICK_COUNT_5MS			5
#define TICK_COUNT_10MS			10
#define TICK_COUNT_25MS			25
#define TICK_COUNT_35MS			35
#define TICK_COUNT_50MS			50
#define TICK_COUNT_100MS		100
#define TICK_COUNT_200MS		200
#define TICK_COUNT_400MS		400
#define TICK_COUNT_500MS		500
#define TICK_COUNT_1S			1000

//---------------------------TIMER0----------���μ���������------------------------------------
#define MAX_CIRCLU_TIMER			8
//#define TIMER_ENTER_CRITICAL()		NVIC_DisableIRQ(TMR0_IRQn);
//#define TIMER_EXIT_CRITICAL()		NVIC_EnableIRQ(TMR0_IRQn);

struct STimer						//System timer structure.
{
	struct STimer	*pNext;
	BOOL	State;					//Timer����״̬��־ 
	BOOL	Flag;					//Timer�Զ���װ�ر�־
	uint32	Timeout;				//Timer����ֵ
	uint32	TimeCount;				//��ǰ����ֵ
	void	*Arg;					//�ص���������
	void (* callback)(void *Arg);	//�ص�����ָ��
};

/*******************************************************************************
* Function Name  : SysTick_Init
* Description    : ϵͳ�δ�ʱ�ӳ�ʼ��
* Input          : u32IntervalTime -- �δ���ʱ��(us)
* Output         : None
* Return         : None
* Attention		 : 1000 <= u32IntervalTime <= 5000 
*******************************************************************************/
extern void SysTick_Init(uint32 u32IntervalTime);

/*******************************************************************************
* Function Name  : SysTick_Enable
* Description    : ϵͳ�δ�ʱ�Ӽ���
* Input          : None
* Output         : None
* Return         : None
* Attention		 : 0 < u32IntervalTime < 5000 
*******************************************************************************/
extern void SysTick_Enable(void);

/*******************************************************************************
* Function Name  : SysTick_Disable
* Description    : ϵͳ�δ�ʱ�ӹر�
* Input          : None
* Output         : None
* Return         : None
* Attention		 : 0 < u32IntervalTime < 5000 
*******************************************************************************/
extern void SysTick_Disable(void);

/*******************************************************************************
* Function Name  : SysTick_Stop
* Description    : ϵͳ�δ�ʱ����ͣ
* Input          : None
* Output         : None
* Return         : None
* Attention		 :  
*******************************************************************************/
extern void SysTick_Stop(void);

/*******************************************************************************
* Function Name  : SysTimerInit
* Description	 : ����TIMER���г�ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
extern void SysTimerInit(void);

/*******************************************************************************
* Function Name  : TimerCreate
* Description	 : �ڻ���TIMER�����д���һ��ģ�ⶨʱ������
* Input 		 : State--����״̬�� Flag--�Զ���װ�ر�ǣ� 
				   Timeout--����ֵ�� callback--�ص�����ָ��
* Output		 : None
* Return		 : ��������Timerָ��
*******************************************************************************/
extern struct STimer *TimerCreate(uint8 State, uint8 Flag, uint32 Timeout, void (* callback)(void *Arg), void *Arg);

/*******************************************************************************
* Function Name  : TimerCallback
* Description	 : ������ʱ���ص�������ʵ�ֻ���Timer���еļ�������
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
extern void TimerCallback(void);


#endif
/*========================File end======================================================================*/

