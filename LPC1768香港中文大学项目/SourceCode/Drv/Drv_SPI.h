/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_SPI.h                                                                       
==                                                                                         
**  Function:	SPI (Serial Peripheral Interface)
==                                                                                         
**  Data:       2016/05/26                                                                          
================================================================================================*/

#ifndef	_DRV_SPI_H_
#define	_DRV_SPI_H_

#include "Config.h"
#include "LPC17xx.h"


#define SPI_CLK_pin			15	 	// P0.15
#define SPI_MISO_pin		17	 	// P0.17
#define SPI_MOSI_pin		18	 	// P0.18

#define FLASH_CS_pin        4       //P1.4 
#define FM1702_CS_pin       9       //P1.9 
#define ENC28J60_CS_pin     0       //P1.0 
#define ZiKu_CS_pin         8       //P1.8

#define FLASH_CS            (1 << 4)   
#define FM1702_CS           (1 << 9) 
#define ENC28J60_CS         (1 << 0) 
#define ZiKu_CS             (1 << 8) 


#define SET_FLASH_CS_PIN		(LPC_GPIO1->FIOSET |= FLASH_CS) 
#define CLR_FLASH_CS_PIN		(LPC_GPIO1->FIOCLR |= FLASH_CS)

#define SET_FM1702_CS_PIN		(LPC_GPIO1->FIOSET |= FM1702_CS) 
#define CLR_FM1702_CS_PIN		(LPC_GPIO1->FIOCLR |= FM1702_CS) 

#define	SET_ENC28J60_CS_PIN	    (LPC_GPIO1->FIOSET |= ENC28J60_CS)
#define	CLR_ENC28J60_CS_PIN	    (LPC_GPIO1->FIOCLR |= ENC28J60_CS)

#define SET_ZiKu_CS_PIN		    (LPC_GPIO1->FIOSET |= ZiKu_CS) 
#define CLR_ZiKu_CS_PIN		    (LPC_GPIO1->FIOCLR |= ZiKu_CS) 


/***********************************************************************************************
* Function Name  : SPI_Host_Init
* Description	 : SPI主机初始化
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : 此SPI挂载多个IC: FLASH\FM1702\ENC28J60\字库芯片
***********************************************************************************************/
extern void SPI_Host_Init(void);

/***********************************************************************************************
* Function Name  : SPI_Send1Byte
* Description	 : SPI发出1字节数据
* Input 		 : data -- 要发送的数据
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern uchar8 SPI_Send1Byte(uchar8 data);

/***********************************************************************************************
* Function Name  : SPI_Read1Byte
* Description	 : SPI读取1字节数据
* Input 		 : None
* Output		 : None
* Return		 : 读取到的数据
***********************************************************************************************/
extern uchar8 SPI_Read1Byte(void);


/***********************************************************************************************
* Function Name  : FlashSPI_Enable
* Description	 : 使能FLASH片选
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : 同时关闭其它共用SPI的片选
***********************************************************************************************/
extern void FlashSPI_Enable(void);

/***********************************************************************************************
* Function Name  : FlashSPI_Disable
* Description	 : 关闭FLASH片选
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void FlashSPI_Disable(void);

/***********************************************************************************************
* Function Name  : FM1702SPI_Enable
* Description	 : 使能FLASH片选--FM1702
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : 同时关闭其它共用SPI的片选
***********************************************************************************************/
extern void FM1702SPI_Enable(void);

/***********************************************************************************************
* Function Name  : FM1702SPI_Disable
* Description	 : 关闭FLASH片选--FM1702
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void FM1702SPI_Disable(void);

/***********************************************************************************************
* Function Name  : NetSPI_Enable
* Description	 : 使能片选--ENC28J60
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : 同时关闭其它共用SPI的片选
***********************************************************************************************/
extern void NetSPI_Enable(void);

/***********************************************************************************************
* Function Name  : NetSPI_Disable
* Description	 : 关闭片选--ENC28J60
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void NetSPI_Disable(void);

/***********************************************************************************************
* Function Name  : ZiKuSPI_Enable
* Description	 : 使能片选--字库芯片SPI
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : 同时关闭其它共用SPI的片选
***********************************************************************************************/
extern void ZiKuSPI_Enable(void);

/***********************************************************************************************
* Function Name  : ZiKuNetSPI_Disable
* Description	 : 关闭片选--字库芯片SPI
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void ZiKuNetSPI_Disable(void);


#endif
/*=============================File end========================================================*/
