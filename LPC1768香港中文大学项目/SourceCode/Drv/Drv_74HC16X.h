/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                        
==                                                                                          
**  Version:	v2.0.0                                                                          
==
**  Filename:	Drv_74HC16X.h                                                                       
==                                                                                         
**  Function:	74HC164\74HC165逻辑控制 
==                                                                                         
**  Data:       2015/08/21                                                                      
================================================================================================*/

#ifndef _DRV_74HC16X_H_
#define _DRV_74HC16X_H_


#include "Config.h"
#include "LPC17xx.h"


#define CLK164			    3		// P2.3
#define DATA164			    16		// P0.16
#define CLK165			    21		// P0.21
#define LOAD165			    19	 	// P0.19
#define DATA165			    22	 	// P0.22
#define DATA165_2			20	 	// P0.20

#define SET_CLK164_PIN		(LPC_GPIO2->FIOSET |= (1<<CLK164)) 
#define CLR_CLK164_PIN		(LPC_GPIO2->FIOCLR |= (1<<CLK164))

#define SET_DATA164_PIN		(LPC_GPIO0->FIOSET |= (1<<DATA164)) 
#define CLR_DATA164_PIN		(LPC_GPIO0->FIOCLR |= (1<<DATA164))


#define SET_CLK165_PIN		(LPC_GPIO0->FIOSET |= (1<<CLK165)) 
#define CLR_CLK165_PIN		(LPC_GPIO0->FIOCLR |= (1<<CLK165))

#define SET_LOAD165_PIN		(LPC_GPIO0->FIOSET |= (1<<LOAD165)) 
#define CLR_LOAD165_PIN		(LPC_GPIO0->FIOCLR |= (1<<LOAD165))


#define GET_HC165_DATA_STATE		 (LPC_GPIO0->FIOPIN&0x400000)  //箱门状态	
#define GET_HC165_DATA2_STATE		 (LPC_GPIO0->FIOPIN&0x100000)  //探物状态	

/***********************************************************************************************
* Function Name  : Drv_74HC164_Init
* Description	 : 74HC164控制初始化
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Drv_74HC164_Init(void);

/***********************************************************************************************
* Function Name  : Drv_74HC165_Init
* Description	 : 74HC165控制初始化
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Drv_74HC165_Init(void);

/***********************************************************************************************
* Function Name  : ManualOpen1Box
* Description	 : 开启一个箱门,仅开锁,不关锁
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void ManualOpen1Box(uchar8 BoxNum);

/***********************************************************************************************
* Function Name  : CloseAllBox
* Description	 : 清所有箱门
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void CloseAllBox(void);

/***********************************************************************************************
* Function Name  : OpenOneBox
* Description	 : 开启一个箱门,开锁后自动关锁
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void OpenOneBox(uchar8 BoxNum);

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
extern uchar8 GetBoxDoorStatus(uchar8* pStatusBuf, uchar8 u8BoxNum);

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
extern uchar8 GetBoxInsideStatus(uchar8* pStatusBuf, uchar8 u8BoxNum);

#endif
/*========================File end===================================================================================================*/

