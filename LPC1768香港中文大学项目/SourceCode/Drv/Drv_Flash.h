/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_SpiFlash.c                                                                       
==                                                                                         
**  Function:	SPIͨѶFLASHоƬ��д���� 
==                                                                                         
**  Data:       2014/09/02                                                                      
================================================================================================*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FLASH_H_
#define _FLASH_H_

/* Includes ------------------------------------------------------------------*/
#include "Config.h"
#include "LPC17xx.h"


/* Exported macro ------------------------------------------------------------*/
//#define CS45			4		// P1.4
//#define SET_CS45_PIN		(LPC_GPIO1->FIOSET |= (1<<CS45)) 
//#define CLR_CS45_PIN		(LPC_GPIO1->FIOCLR |= (1<<CS45))

/*
// Select SPI FLASH: Chip Select pin low  
#define FLASH_CS_LOW()       CLR_CS45_PIN
// Deselect SPI FLASH: Chip Select pin high 
#define FLASH_CS_HIGH()      SET_CS45_PIN
*/


#define FLASH_ID    				0xC84014C8

#define FLASH_MAX_ADDR				 0x800000UL		//8MByte Flash ����ֽڵ�ַ
#define FLASH_MAX_BlockByte			(64*1024)
#define FLASH_MAX_SectorByte		(4*1024)
#define FLASH_MAX_PageByte			 256


#define FLASH_FREE_SEC_ADDR			(FLASH_MAX_ADDR - FLASH_MAX_SectorByte) //Ԥ��һ�������Ŀռ�

/*--------------------FLASH�洢��ַ�滮----------------------------------------*/

#define FLASH_ADDR_FONT				0x0UL		   //0~2MByteΪ�ֿ�洢��
#define FLASH_ADDR_PHOTO			0x200000UL	   //2~4MByteΪͼƬ�洢��
#define FLASH_ADDR_USER			    0x400000UL	   //4~8MByteΪ�û����ݴ洢��


/*--------------------FLASH�����������----------------------------------------*/
#define FLASH_ERR_ADDR				0xF0		  //������ַ����
#define FLASH_ERR_LEN				0xF1		  //д�����ݳ��ȳ���

/*******************************************************************************
* Function Name  : FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
extern u8 FLASH_SendByte(u8 byte);

/*******************************************************************************
* Function Name  : FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
extern u8 FLASH_ReadByte(void);

/*******************************************************************************
* Function Name  : FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : 0 -- OK, ����--�������
*******************************************************************************/
extern int FLASH_Init(void);

/*******************************************************************************
* Function Name  : FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
extern u16 FLASH_SendHalfWord(u16 HalfWord);

/*******************************************************************************
* Function Name  : FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void FLASH_WaitForWriteEnd(void);

/*******************************************************************************
* Function Name  : FLASH_WriteEnable
* Description    : ��FLASHдʹ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void FLASH_WriteEnable(void);

/*******************************************************************************
* Function Name  : FLASH_WriteDisable
* Description    : �ر�FLASHдʹ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void FLASH_WriteDisable(void);

/*******************************************************************************
* Function Name  : FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
extern u32 FLASH_ReadID(void);

/*******************************************************************************
* Function Name  : FLASH_ID_Check
* Description    : У��FlashоƬID
* Input          : None
* Output         : None
* Return         : 0 -- OK, ���� -- �쳣
*******************************************************************************/
extern int FLASH_ID_Check(u32 FlashID);

/*******************************************************************************
* Function Name  : FLASH_SectorErase
* Description    : ����FLASHָ����ַ����sector����
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
extern void FLASH_SectorErase(u32 SectorAddr);

/*******************************************************************************
* Function Name  : FLASH_BlockErase
* Description    : ����FLASHָ����ַ����Block����
* Input          : BlockAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
extern void FLASH_BlockErase(u32 BlockAddr);

/*******************************************************************************
* Function Name  : FLASH_ChipErase
* Description    : ������ƬFLASH 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void FLASH_ChipErase(void);

/*******************************************************************************
* Function Name  : FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "FLASH_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
extern void FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);

/*******************************************************************************
* Function Name  : FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
extern void FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);

/*******************************************************************************
* Function Name  : FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
extern void FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);

/*******************************************************************************
* Function Name  : FLASH_FastReadBuffer
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
* Others         : ���ٶ�ȡ����
*******************************************************************************/
extern void FLASH_FastReadBuffer(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);

/*******************************************************************************
* Function Name  : FLASH_AutoWriteBuffer
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : 0 -- �ɹ�, ��0--���������
* Others         : �Զ���ҳд,дǰ�����flash���в�������
*******************************************************************************/
extern int FLASH_AutoWriteBuffer(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);


#endif /* __FLASH_H */
/*=============================File end========================================================*/

