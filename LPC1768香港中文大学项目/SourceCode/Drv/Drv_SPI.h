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
* Description	 : SPI������ʼ��
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : ��SPI���ض��IC: FLASH\FM1702\ENC28J60\�ֿ�оƬ
***********************************************************************************************/
extern void SPI_Host_Init(void);

/***********************************************************************************************
* Function Name  : SPI_Send1Byte
* Description	 : SPI����1�ֽ�����
* Input 		 : data -- Ҫ���͵�����
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern uchar8 SPI_Send1Byte(uchar8 data);

/***********************************************************************************************
* Function Name  : SPI_Read1Byte
* Description	 : SPI��ȡ1�ֽ�����
* Input 		 : None
* Output		 : None
* Return		 : ��ȡ��������
***********************************************************************************************/
extern uchar8 SPI_Read1Byte(void);


/***********************************************************************************************
* Function Name  : FlashSPI_Enable
* Description	 : ʹ��FLASHƬѡ
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : ͬʱ�ر���������SPI��Ƭѡ
***********************************************************************************************/
extern void FlashSPI_Enable(void);

/***********************************************************************************************
* Function Name  : FlashSPI_Disable
* Description	 : �ر�FLASHƬѡ
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void FlashSPI_Disable(void);

/***********************************************************************************************
* Function Name  : FM1702SPI_Enable
* Description	 : ʹ��FLASHƬѡ--FM1702
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : ͬʱ�ر���������SPI��Ƭѡ
***********************************************************************************************/
extern void FM1702SPI_Enable(void);

/***********************************************************************************************
* Function Name  : FM1702SPI_Disable
* Description	 : �ر�FLASHƬѡ--FM1702
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void FM1702SPI_Disable(void);

/***********************************************************************************************
* Function Name  : NetSPI_Enable
* Description	 : ʹ��Ƭѡ--ENC28J60
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : ͬʱ�ر���������SPI��Ƭѡ
***********************************************************************************************/
extern void NetSPI_Enable(void);

/***********************************************************************************************
* Function Name  : NetSPI_Disable
* Description	 : �ر�Ƭѡ--ENC28J60
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void NetSPI_Disable(void);

/***********************************************************************************************
* Function Name  : ZiKuSPI_Enable
* Description	 : ʹ��Ƭѡ--�ֿ�оƬSPI
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : ͬʱ�ر���������SPI��Ƭѡ
***********************************************************************************************/
extern void ZiKuSPI_Enable(void);

/***********************************************************************************************
* Function Name  : ZiKuNetSPI_Disable
* Description	 : �ر�Ƭѡ--�ֿ�оƬSPI
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void ZiKuNetSPI_Disable(void);


#endif
/*=============================File end========================================================*/
