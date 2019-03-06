/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                        
==                                                                                          
**  Version:	V1.0.0                                                                          
==
**  Filename:	Drv_74HC16X.c                                                                       
==                                                                                         
**  Function:	74HC164\74HC165逻辑控制
==                                                                                         
**  Data:       2015/08/21                                                                      
================================================================================================*/

#include "Drv_74HC16X.h"

extern void delay_ms(uint32 nms);

/*******************************************************************************
* Function Name  : HC_delay_us
* Description    : 延时函数 -- 单位 us  (经示波器测试,此延时时间是精准的)
* Input          : - Nus: 延时us
* Output         : None
* Return         : None
* Notices		 : 参数最大值为 0xffffff / (HCLK / 8000000)
*******************************************************************************/            
void HC_delay_us(uint32 Nus)
{ 
	volatile uint32 i,k;
	while(Nus --)
	{    
		for(k=0; k<3; k++)  
		{  
			i++;
			if(i > 5)
			{
				i = 0;
			}
		}
	} 
}

/***********************************************************************************************
* Function Name  : Drv_74HC164_Init
* Description	 : 74HC164控制初始化
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Drv_74HC164_Init(void)
{	
	LPC_PINCON->PINSEL1 &= 0xFFFFFFFC;
	LPC_PINCON->PINSEL4 &= 0xFFFFFF3F;
	
	LPC_GPIO0->FIODIR |= (1<<DATA164);
	LPC_GPIO2->FIODIR |= (1<<CLK164);
	
	SET_CLK164_PIN; 
	CLR_DATA164_PIN;
}

/***********************************************************************************************
* Function Name  : Drv_74HC165_Init
* Description	 : 74HC165控制初始化
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Drv_74HC165_Init(void)
{
	LPC_PINCON->PINSEL1 &= 0xFFFFF3FF;
	LPC_PINCON->PINSEL1 &= 0xFFFFFF3F;

	LPC_PINCON->PINSEL1 &= 0xFFFFFCFF;  //P0.20
	LPC_PINCON->PINSEL1 &= 0xFFFFCFFF;  //P0.22
	
	LPC_GPIO0->FIODIR |= (1<<CLK165);
	LPC_GPIO0->FIODIR |= (1<<LOAD165);

	LPC_GPIO0->FIODIR &= (~(1<<DATA165));
	LPC_GPIO0->FIODIR &= (~(1<<DATA165_2));

	CLR_CLK165_PIN;
	SET_LOAD165_PIN;
}


/***********************************************************************************************
* Function Name  : ManualOpen1Box
* Description	 : 开启一个箱门,仅开锁,不关锁
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void ManualOpen1Box(uchar8 BoxNum)
{
	uchar8 i;
	
	SET_DATA164_PIN;
	for(i=0; i<BoxNum; i++)
	{
		CLR_CLK164_PIN;
		HC_delay_us(2);
		SET_CLK164_PIN;  //74HC164在上升沿读取数据
		HC_delay_us(2);

		CLR_DATA164_PIN;	
	}
}

/***********************************************************************************************
* Function Name  : CloseAllBox
* Description	 : 清所有箱门
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void CloseAllBox(void)
{
	uchar8 i;
	
	CLR_DATA164_PIN;
	for(i=0;i<195;i++)
	{
		SET_CLK164_PIN;
		HC_delay_us(2);
		CLR_CLK164_PIN;
		HC_delay_us(2);
	}
}

/***********************************************************************************************
* Function Name  : OpenOneBox
* Description	 : 开启一个箱门,开锁后自动关锁
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void OpenOneBox(uchar8 BoxNum)
{
	uchar8 i;
	
	SET_DATA164_PIN;
	for(i=0; i<BoxNum; i++)
	{
		CLR_CLK164_PIN;
		HC_delay_us(2);
		SET_CLK164_PIN;  //74HC164在上升沿读取数据
		HC_delay_us(2);

		CLR_DATA164_PIN;	
	}

	delay_ms(360);
	CloseAllBox();
}

/***********************************************************************************************
* Function Name  : GetBoxDoorStatus
* Description	 : 读箱门锁状态
* Input 		 : pStatusBuf -- 读取的状态字节存放区地址, u8BoxNum -- 读取的箱门数量
* Output		 : 箱门的状态,每个字节对应8个箱门的状态,第一字节的最低位对应第1号箱状态
                   第一字节bit0 -- 7 对应1--8号箱门, 0表示关闭,1表示开启
                   .............
                   第N字节bit0 -- 7 对应((N-1)*8+1)--(N*8)号箱门, 0表示关闭,1表示开启
* Return		 : 读取的字节数
***********************************************************************************************/
uchar8 GetBoxDoorStatus(uchar8* pStatusBuf, uchar8 u8BoxNum)
{		
	uchar8 i = 0, j = 0;
	uchar8 iRet = 0;
	uchar8 BoxGroup = 0;

	BoxGroup = u8BoxNum / 8;
	if(u8BoxNum % 8)
	{
		BoxGroup += 1;
	}
	iRet = BoxGroup;
	
	SET_LOAD165_PIN;
	SET_CLK165_PIN;
	HC_delay_us(2);
	CLR_LOAD165_PIN;
	HC_delay_us(2);
	SET_LOAD165_PIN;
	for(j=0; j<BoxGroup; j++)
	{
		pStatusBuf[j] = 0;
		for(i=0; i<8; i++)
		{
			pStatusBuf[j] = pStatusBuf[j]>>1;
			if(0x0 == GET_HC165_DATA_STATE)
			{
				pStatusBuf[j] |= 0x80;
			}
			CLR_CLK165_PIN;
			HC_delay_us(2);
			SET_CLK165_PIN;
			HC_delay_us(2);
		}
	}
	CLR_CLK165_PIN;

	return iRet;
}

/***********************************************************************************************
* Function Name  : GetBoxInsideStatus
* Description	 : 读箱门存物状态
* Input 		 : pStatusBuf -- 读取的状态字节存放区地址, u8BoxNum -- 读取的箱门数量
* Output		 : 箱门的状态,每个字节对应8个箱门的状态,第一字节的最低位对应第1号箱状态
                   第一字节bit0 -- 7 对应1--8号箱门, 1表示有物品,0表示空箱(没有物品)
                   .............
                   第N字节bit0 -- 7 对应((N-1)*8+1)--(N*8)号箱门, 1表示有物品,0表示空箱(没有物品)
* Return		 : 读取的字节数
***********************************************************************************************/
uchar8 GetBoxInsideStatus(uchar8* pStatusBuf, uchar8 u8BoxNum)
{
	uchar8 i = 0, j = 0;
	uchar8 iRet = 0;
	uchar8 BoxGroup = 0;

	BoxGroup = u8BoxNum / 8;
	if(u8BoxNum % 8)
	{
		BoxGroup += 1;
	}
	iRet = BoxGroup;

	SET_LOAD165_PIN;
	SET_CLK165_PIN;
	HC_delay_us(2);
	CLR_LOAD165_PIN;
	HC_delay_us(2);
	SET_LOAD165_PIN;
	for(j=0; j<BoxGroup; j++)
	{
		pStatusBuf[j] = 0;
		for(i=0; i<8; i++)
		{
			pStatusBuf[j] = pStatusBuf[j]>>1;
			if(0x0 == GET_HC165_DATA2_STATE)
			{
				pStatusBuf[j] |= 0x80;
			}
			CLR_CLK165_PIN;
			HC_delay_us(2);
			SET_CLK165_PIN;
			HC_delay_us(2);
		}
	}
	CLR_CLK165_PIN;

	return iRet; 
}


/*========================File end==========================================================================*/


