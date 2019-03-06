/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_Eeprom.h                                                                       
==                                                                                         
**  Function:	EEPROM--I2C通讯存储芯片驱动程序 
==                                                                                         
**  Data:       2014/09/05                                                                         
================================================================================================*/

#ifndef	_DRV_EEPROM_H_
#define	_DRV_EEPROM_H_

#include "Config.h"
#include "LPC17xx.h"

/* Exported constants --------------------------------------------------------*/
#define	EP_SDA_pin			27				//P0.27
#define	EP_SCL_pin			28				//P0.28

#define	EP_SDA_H	        (LPC_GPIO0->FIOSET |= (1<<EP_SDA_pin))
#define	EP_SDA_L	        (LPC_GPIO0->FIOCLR |= (1<<EP_SDA_pin))

#define	EP_SCL_H	        (LPC_GPIO0->FIOSET |= (1<<EP_SCL_pin))
#define	EP_SCL_L	        (LPC_GPIO0->FIOCLR |= (1<<EP_SCL_pin))

#define	Get_EP_SDA	        (LPC_GPIO0->FIOPIN &0x08000000)

//------------------------------------------------------------------------------
#define	EP_SUCCEED			0						//成功。
#define	EP_COMM_ERR			1						//通讯错误
#define	EP_AD_OVER_ERR		2						//地址溢出。

#define	EP_Max_ErrCount		3						//收不到ACK信号最大次数---操作尝试次数
#define	EP_DelayTime		10


/***********************************************************************************************/
//#define	EEPROM_MAX_AD		 		0x2000UL		//24C64的最大地址 0x2000
//#define	EEPROM_1PAGE_NUM			32				//一页的大小为32Byte

#define	EEPROM_MAX_AD		 		0x8000UL		//24C256的最大地址 0x8000
#define	EEPROM_1PAGE_NUM			64				//一页的大小为64Byte

/***********************************************************************************************
* Function Name  : I2C_Eeprom_Init
* Description	 : brief  Initializes peripherals used by the I2C EEPROM driver.
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void I2C_Eeprom_Init(void);

/***********************************************************************************************
* Function Name  : I2C_EE_ByteWrite
* Description	 : brief  Writes one byte to the I2C EEPROM.
* Input 		 : @param pBuffer : pointer to the buffer  containing the data to be written to the EEPROM.
				   @param WriteAddr : EEPROM's internal address to write to.
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern uchar8 I2C_EE_ByteWrite(uint32 WriteAddr, uchar8 writedata);

/***********************************************************************************************
* Function Name  : I2C_EE_ByteRead
* Description	 : brief  Read one byte to the I2C EEPROM.
* Input 		 : @param pBuffer : pointer to the buffer  containing the data to be written to the EEPROM.
				   @param WriteAddr : EEPROM's internal address to write to.
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern uchar8 I2C_EE_ByteRead(uint32 WriteAddr, uchar8* readdata);

/***********************************************************************************************
* Function Name  : I2C_EE_PageWrite
* Description	 : brief  Writes more than one byte to the EEPROM with a single WRITE cycle. 
				   The number of byte can't exceed the EEPROM page size.
* Input 		 : @param pBuffer : pointer to the buffer containing the data to be written to the EEPROM.
				   @param WriteAddr : EEPROM's internal address to write to.
				   @param NumByteToWrite : number of bytes to write to the EEPROM.
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern uchar8 I2C_EE_PageWrite(uchar8* pBuffer, uint32 WriteAddr, uint32 NumByteToWrite);

/***********************************************************************************************
* Function Name  : I2C_EE_BufferWrite
* Description	 : @param pBuffer : pointer to the buffer  containing the data to be written to the EEPROM.
* Input 		 : @param WriteAddr : EEPROM's internal address to write to.
				   @param NumByteToWrite : number of bytes to write to the EEPROM.
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern uchar8 I2C_EE_BufferWrite(uchar8* pBuffer, uint32 WriteAddr, uint32 NumByteToWrite);
  
/***********************************************************************************************
* Function Name  : I2C_EE_BufferRead
* Description	 : brief  Reads a block of data from the EEPROM.
* Input 		 : @param pBuffer : pointer to the buffer that receives the data read from the EEPROM.
				   @param ReadAddr : EEPROM's internal address to read from.
				   @param NumByteToRead : number of bytes to read from the EEPROM.
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern uchar8 I2C_EE_BufferRead(uchar8* pBuffer, uint32 ReadAddr, uint32 NumByteToRead);


#endif
/*=============================File end========================================================*/
