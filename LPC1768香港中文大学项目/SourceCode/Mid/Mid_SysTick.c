/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v2.0.0                                                                          
==
**  Filename:	Mid_SysTick.c                                                                  
==                                                                                         
**  Function:	ϵͳ�δ�ʱ�Ӵ�������
==                                                                                         
**  Data:       2014/08/15                                                                    
================================================================================================*/

#include "Mid_SysTick.h"


#ifndef NULL
#define NULL  0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

static uchar8 bSysTickEnable;


/* SysTick counter state */
#define SysTick_Counter_Disable        ((uint32)0xFFFFFFFE)
#define SysTick_Counter_Enable         ((uint32)0x00000001)
#define SysTick_Counter_Clear          ((uint32)0x00000000)

/*******************************************************************************
* Function Name  : SysTick_CounterCmd
* Description    : Enables or disables the SysTick counter.
* Input          : - SysTick_Counter: new state of the SysTick counter.
*                    This parameter can be one of the following values:
*                       - SysTick_Counter_Disable: Disable counter
*                       - SysTick_Counter_Enable: Enable counter
*                       - SysTick_Counter_Clear: Clear counter value to 0
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_CounterCmd(uint32 SysTick_Counter)
{
	if (SysTick_Counter == SysTick_Counter_Enable)
	{
		SysTick->CTRL |= SysTick_Counter_Enable;
	}
	else if (SysTick_Counter == SysTick_Counter_Disable) 
	{
		SysTick->CTRL &= SysTick_Counter_Disable;
	}
	else /* SysTick_Counter == SysTick_Counter_Clear */
	{
		SysTick->RELOAD = SysTick_Counter_Clear;
	}    
}

/*******************************************************************************
* Function Name  : SysTick_Init
* Description    : ϵͳ�δ�ʱ�ӳ�ʼ��
* Input          : u32IntervalTime -- �δ���ʱ��(us)
* Output         : None
* Return         : None
* Attention		 : 1000 <= u32IntervalTime <= 5000 
*******************************************************************************/
void SysTick_Init(uint32 u32IntervalTime)
{
	if(1000 != u32IntervalTime)
	{
		u32IntervalTime = 1000;
	}

	SysTick_Config(0x05F5E100 / u32IntervalTime);  //SystemCoreClock = 0x05F5E100  1ms����һ���ж�
	SysTick_CounterCmd(SysTick_Counter_Disable);
	bSysTickEnable = 0;
}

/*******************************************************************************
* Function Name  : SysTick_Enable
* Description    : ϵͳ�δ�ʱ�Ӽ���
* Input          : None
* Output         : None
* Return         : None
* Attention		 : 0 < u32IntervalTime < 5000 
*******************************************************************************/
void SysTick_Enable(void)
{
	bSysTickEnable = 1;
	/* Enable the SysTick Counter */
	SysTick_CounterCmd(SysTick_Counter_Enable);
}

/*******************************************************************************
* Function Name  : SysTick_Disable
* Description    : ϵͳ�δ�ʱ�ӹر�
* Input          : None
* Output         : None
* Return         : None
* Attention		 : 0 < u32IntervalTime < 5000 
*******************************************************************************/
void SysTick_Disable(void)
{
	bSysTickEnable = 0;
	/* Disable SysTick Counter */
	SysTick_CounterCmd(SysTick_Counter_Disable);
	/* Clear SysTick Counter */
	SysTick_CounterCmd(SysTick_Counter_Clear);
}

/*******************************************************************************
* Function Name  : SysTick_Stop
* Description    : ϵͳ�δ�ʱ����ͣ
* Input          : None
* Output         : None
* Return         : None
* Attention		 :  
*******************************************************************************/
void SysTick_Stop(void)
{
	bSysTickEnable = 0;
	/* Disable SysTick Counter */
	SysTick_CounterCmd(SysTick_Counter_Disable);
}


/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
	TimerCallback();
}




/***************====��������TIMER���У�����1����ʱ�������������====*******************************/
struct STimer TimerPool[MAX_CIRCLU_TIMER];
struct STimer *TimerFreeList;
struct STimer *TimerList;

/*******************************************************************************
* Function Name  : SysTimerInit
* Description	 : ����TIMER���г�ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void SysTimerInit(void)
{
	int i;

	TimerFreeList=NULL;
	for(i=0; i<MAX_CIRCLU_TIMER; i++)
	{
		TimerPool[i].pNext = TimerFreeList;
		TimerFreeList = &TimerPool[i];
	}

	TimerList = NULL;
}

/*******************************************************************************
* Function Name  : TimerCreate
* Description	 : �ڻ���TIMER�����д���һ��ģ�ⶨʱ������
* Input 		 : State--����״̬�� Flag--�Զ���װ�ر�ǣ� 
				   Timeout--����ֵ�� callback--�ص�����ָ��
* Output		 : None
* Return		 : ��������Timerָ��
*******************************************************************************/
struct STimer *TimerCreate(uint8 State, uint8 Flag, uint32 Timeout, void (* callback)(void *Arg), void *Arg)
{
	struct STimer *pTimer;
	
	if(bSysTickEnable)
	{
		SysTick_Disable();
	}
	
	if((pTimer = TimerFreeList) != NULL)
	{
		TimerFreeList = TimerFreeList->pNext;
		
		pTimer->State = State;
		pTimer->Flag = Flag;
		pTimer->Timeout = Timeout;
		pTimer->TimeCount = Timeout;
		pTimer->callback = callback;
		pTimer->Arg = Arg;
		
		pTimer->pNext = TimerList;
		TimerList = pTimer;
	}
	
	if(bSysTickEnable)
	{
		SysTick_Enable();
	}
	return pTimer;
}

void TimerDelete(struct STimer *pTimer)
{
	struct STimer *pTimert;
	
	if(bSysTickEnable)
	{
		SysTick_Disable();
	}

	if(pTimer == TimerList)
	{
		TimerList = TimerList->pNext;
	}
	else
	{
		for(pTimert=TimerList; pTimert!=NULL; pTimert=pTimert->pNext)
		{
			if(pTimert->pNext == pTimer)
			{
				pTimert->pNext = pTimer->pNext;
				break;
			}
		}
	}
	pTimer->pNext = TimerFreeList;
	TimerFreeList = pTimer;
	if(bSysTickEnable)
	{
		SysTick_Enable();
	}

}

void TimerSet(struct STimer *pTimer, uint8 State, uint8 Flag, uint32 Timeout)
{
	if(bSysTickEnable)
	{
		SysTick_Disable();
	}

	SysTick_Disable();
	pTimer->State = State;
	pTimer->Flag = Flag;
	pTimer->Timeout = Timeout;
	pTimer->TimeCount = Timeout;
	if(bSysTickEnable)
	{
		SysTick_Enable();
	}

}

/*******************************************************************************
* Function Name  : TimerCallback
* Description	 : ������ʱ���ص�������ʵ�ֻ���Timer���еļ�������
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void TimerCallback(void)
{
	struct STimer *pTimert;
	
	pTimert = TimerList;
	while(pTimert)
	{
		if(pTimert->State == TRUE)			//���������б�־
		{
			pTimert->TimeCount--;
			if(pTimert->TimeCount == 0)
			{
				pTimert->callback(pTimert->Arg);
				if(pTimert->Flag == TRUE)	// Auto-Reload
				{
					pTimert->TimeCount = pTimert->Timeout;
				}
				else
				{
					TimerDelete(pTimert);
				}
			}
		}
		pTimert = pTimert->pNext;
	}
}
//===========================����Timer���д������=======================================



/*========================File end======================================================================*/

