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
**  Function:	����ͨ�����������
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

#define	RS485_Txd_Enable	SET_CS485_PIN	     //�򿪷���ʹ�� 
#define	RS485_Txd_Disable	CLR_CS485_PIN	     //�رշ���ʹ��					
#define	RS485_Rxd_Enable	CLR_CS485_PIN   	 //������ʹ��,���رշ���ʹ��					
#define	RS485_Rxd_Disable	SET_CS485_PIN	     //�رս���ʹ��	



extern uchar8 RS485DataBuf[512];
extern uint32 RS485DataCnt;

extern uchar8 FingerprintDataBuf[768];
extern uint32 FingerprintDataCnt;

/***********************************************************************************************
* Function Name  : Clean_RS485RxdBuf
* Description	 : ���RS485���ջ�����
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void Clean_RS485RxdBuf(void);

/***********************************************************************************************
* Function Name  : ReadRS485Data
* Description	 : ��ȡUART2������
* Input 		 : pDataBuf -- ���ݴ��ָ��, GetLen --��ȡ����, TimeOut_ms -- ��ʱʱ��(ms)
* Output		 : None
* Return		 : ʵ�ʶ�ȡ�������ݳ���
***********************************************************************************************/
extern int ReadRS485Data(uchar8* pDataBuf, uint32 GetLen, uint32 TimeOut_ms);

/***********************************************************************************************
* Function Name  : UART0_Init
* Description    : ����0��ʼ��
* Input          : u32Baudrate -- ͨѶ������
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART0_Init(uint32 u32Baudrate);


/***********************************************************************************************
* Function Name  : UART0_SendByte
* Description    : ����0����1�ֽ�����
* Input          : Data -- ����������
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART0_SendByte(uchar8 Data);

/***********************************************************************************************
* Function Name  : UART0_SendNxByte
* Description    : ����0����N�ֽ�����
* Input          : Data -- ����������
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART0_SendNxByte(uchar8* pStrBuf,uint16 len);

/***********************************************************************************************
* Function Name  : UART0_SendStr
* Description    : ����0�����ַ��� 
* Input          : pStr -- �������ַ���ָ��
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART0_SendStr(uchar8* pStr);


/***********************************************************************************************
* Function Name  : UART1_Init
* Description    : ����1��ʼ��
* Input          : u32Baudrate -- ͨѶ������
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART1_Init(uint32 u32Baudrate);

/***********************************************************************************************
* Function Name  : UART1_SendByte
* Description    : ����1����1�ֽ�����
* Input          : Data -- ����������
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART1_SendByte(uchar8 Data);

/***********************************************************************************************
* Function Name  : UART1_SendNxByte
* Description    : ����1����1�ֽ�����
* Input          : Data -- ����������
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART1_SendNxByte(uchar8* pStrBuf,uint16 len);

/***********************************************************************************************
* Function Name  : UART2_Init
* Description    : ����2��ʼ��
* Input          : u32Baudrate -- ͨѶ������
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART2_Init(uint32 u32Baudrate);

/***********************************************************************************************
* Function Name  : UART2_SendByte
* Description    : ����2����1�ֽ�����
* Input          : Data -- ����������
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART2_SendByte(uchar8 Data);

/***********************************************************************************************
* Function Name  : UART2_SendNxByte
* Description    : ����2����1�ֽ�����
* Input          : Data -- ����������
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART2_SendNxByte(uchar8* pStrBuf,uint16 len);

/***********************************************************************************************
* Function Name  : UART3_Init
* Description    : ����3��ʼ��
* Input          : u32Baudrate -- ͨѶ������
* Output         : None
* Return         : None
***********************************************************************************************/
void UART3_Init(uint32 u32Baudrate);

/***********************************************************************************************
* Function Name  : UART3_SendNxByte
* Description    : ����3����N�ֽ�����
* Input          : Data -- ����������
* Output         : None
* Return         : None
***********************************************************************************************/
extern void UART3_SendNxByte(uchar8* pStrBuf,uint16 len);

/***********************************************************************************************
* Function Name  : RS485_SendNxByte
* Description    : RS485����N�ֽ�����
* Input          : pDataBuf -- ����������, len -- ���ݳ���
* Output         : None
* Return         : None
***********************************************************************************************/
extern void RS485_SendNxByte(uchar8* pDataBuf, uint16 len);

#endif
/*=============================File end========================================================*/
