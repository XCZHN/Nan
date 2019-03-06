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
**  Function:	系统滴答时钟处理任务
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

//---------------------------TIMER0----------环形计算器队列------------------------------------
#define MAX_CIRCLU_TIMER			8
//#define TIMER_ENTER_CRITICAL()		NVIC_DisableIRQ(TMR0_IRQn);
//#define TIMER_EXIT_CRITICAL()		NVIC_EnableIRQ(TMR0_IRQn);

struct STimer						//System timer structure.
{
	struct STimer	*pNext;
	BOOL	State;					//Timer运行状态标志 
	BOOL	Flag;					//Timer自动重装载标志
	uint32	Timeout;				//Timer计数值
	uint32	TimeCount;				//当前计数值
	void	*Arg;					//回调函数参数
	void (* callback)(void *Arg);	//回调函数指针
};

/*******************************************************************************
* Function Name  : SysTick_Init
* Description    : 系统滴答时钟初始化
* Input          : u32IntervalTime -- 滴答间隔时间(us)
* Output         : None
* Return         : None
* Attention		 : 1000 <= u32IntervalTime <= 5000 
*******************************************************************************/
extern void SysTick_Init(uint32 u32IntervalTime);

/*******************************************************************************
* Function Name  : SysTick_Enable
* Description    : 系统滴答时钟激活
* Input          : None
* Output         : None
* Return         : None
* Attention		 : 0 < u32IntervalTime < 5000 
*******************************************************************************/
extern void SysTick_Enable(void);

/*******************************************************************************
* Function Name  : SysTick_Disable
* Description    : 系统滴答时钟关闭
* Input          : None
* Output         : None
* Return         : None
* Attention		 : 0 < u32IntervalTime < 5000 
*******************************************************************************/
extern void SysTick_Disable(void);

/*******************************************************************************
* Function Name  : SysTick_Stop
* Description    : 系统滴答时钟暂停
* Input          : None
* Output         : None
* Return         : None
* Attention		 :  
*******************************************************************************/
extern void SysTick_Stop(void);

/*******************************************************************************
* Function Name  : SysTimerInit
* Description	 : 环形TIMER队列初始化
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
extern void SysTimerInit(void);

/*******************************************************************************
* Function Name  : TimerCreate
* Description	 : 在环形TIMER队列中创建一个模拟定时计数器
* Input 		 : State--运行状态， Flag--自动重装载标记， 
				   Timeout--计数值， callback--回调函数指针
* Output		 : None
* Return		 : 所创建的Timer指针
*******************************************************************************/
extern struct STimer *TimerCreate(uint8 State, uint8 Flag, uint32 Timeout, void (* callback)(void *Arg), void *Arg);

/*******************************************************************************
* Function Name  : TimerCallback
* Description	 : 定计数时器回调函数，实现环形Timer队列的计数功能
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
extern void TimerCallback(void);


#endif
/*========================File end======================================================================*/

