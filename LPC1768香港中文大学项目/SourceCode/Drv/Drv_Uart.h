/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_Uart.h                                                                       
==                                                                                         
**  Function:	串口通信驱动层程序
==                                                                                         
**  Data:       2014/08/11                                                                          
================================================================================================*/

#ifndef	_DRV_UART_H_
#define	_DRV_UART_H_


#include "Config.h"
#include "LPC17xx.h"


//========================================================================================
#define CS485			    14		            // P1.14

#define	SET_CS485_PIN		(LPC_GPIO1->FIOSET |= (1<<CS485))
#define	CLR_CS485_PIN		(LPC_GPIO1->FIOCLR |= (1<<CS485))

#define	RS485_Txd_Enable	SET_CS485_PIN	     //打开发送使能 
#define	RS485_Txd_Disable	CLR_CS485_PIN	     //关闭发送使能					
#define	RS485_Rxd_Enable	CLR_CS485_PIN   	 //开接收使能,即关闭发送使能					
#define	RS485_Rxd_Disable	SET_CS485_PIN	     //关闭接收使能	



extern uchar8 RS485DataBuf[512];
extern uint32 RS485DataCnt;

extern uchar8 FingerprintDataBuf[768];
extern uint32 FingerprintDataCnt;

/***********************************************************************************************
* Function Name  : Clean_RS485RxdBuf
* Description	 : 清除RS485接收缓存区
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Clean_RS485RxdBuf(void);

/***********************************************************************************************
* Function Name  : ReadRS485Data
* Description	 : 读取UART2的数据
* Input 		 : pDataBuf -- 数据存放指针, GetLen --读取长度, TimeOut_ms -- 超时时间(ms)
* Output		 : None
* Return		 : 实际读取到的数据长度
***********************************************************************************************/
extern int ReadRS485Data(uchar8* pDataBuf, uint32 GetLen, uint32 TimeOut_ms);

/***********************************************************************************************
* Function Name  : UART0_Init
* Description    : 串口0初始化
* Input          : u32Baudrate -- 通讯波特率
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART0_Init(uint32 u32Baudrate);


/***********************************************************************************************
* Function Name  : UART0_SendByte
* Description    : 串口0发送1字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART0_SendByte(uchar8 Data);

/***********************************************************************************************
* Function Name  : UART0_SendNxByte
* Description    : 串口0发送N字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART0_SendNxByte(uchar8* pStrBuf,uint16 len);

/***********************************************************************************************
* Function Name  : UART0_SendStr
* Description    : 串口0发送字符串 
* Input          : pStr -- 待发送字符串指针
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART0_SendStr(uchar8* pStr);


/***********************************************************************************************
* Function Name  : UART1_Init
* Description    : 串口1初始化
* Input          : u32Baudrate -- 通讯波特率
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART1_Init(uint32 u32Baudrate);

/***********************************************************************************************
* Function Name  : UART1_SendByte
* Description    : 串口1发送1字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART1_SendByte(uchar8 Data);

/***********************************************************************************************
* Function Name  : UART1_SendNxByte
* Description    : 串口1发送1字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART1_SendNxByte(uchar8* pStrBuf,uint16 len);

/***********************************************************************************************
* Function Name  : UART2_Init
* Description    : 串口2初始化
* Input          : u32Baudrate -- 通讯波特率
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART2_Init(uint32 u32Baudrate);

/***********************************************************************************************
* Function Name  : UART2_SendByte
* Description    : 串口2发送1字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART2_SendByte(uchar8 Data);

/***********************************************************************************************
* Function Name  : UART2_SendNxByte
* Description    : 串口2发送1字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART2_SendNxByte(uchar8* pStrBuf,uint16 len);

/***********************************************************************************************
* Function Name  : UART3_Init
* Description    : 串口3初始化
* Input          : u32Baudrate -- 通讯波特率
* Output         : None
* Return         : None
***********************************************************************************************/
void UART3_Init(uint32 u32Baudrate);

/***********************************************************************************************
* Function Name  : UART3_SendNxByte
* Description    : 串口3发送N字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART3_SendNxByte(uchar8* pStrBuf,uint16 len);

/***********************************************************************************************
* Function Name  : RS485_SendNxByte
* Description    : RS485发送N字节数据
* Input          : pDataBuf -- 待发送数据, len -- 数据长度
* Output         : None
* Return         : None
***********************************************************************************************/
extern void RS485_SendNxByte(uchar8* pDataBuf, uint16 len);

#endif
/*=============================File end========================================================*/
