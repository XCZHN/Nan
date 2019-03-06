/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_Uart.c                                                                       
==                                                                                         
**  Function:	串口通信驱动层程序 
==                                                                                         
**  Data:       2014/08/11                                                                       
================================================================================================*/

#include "Drv_Uart.h"
#include "Mid_SysConfig.h"



uchar8 cRxdByte;

uchar8 RS485DataBuf[512];
uint32 RS485DataCnt;

uchar8 FingerprintDataBuf[768];
uint32 FingerprintDataCnt;

/***********************************************************************************************
* Function Name  : StoreFingerData
* Description	 : 存储指纹头的数据
* Input 		 : u8Data -- 装入的数据
* Output		 : None
* Return		 : None
***********************************************************************************************/
void StoreFingerData(uchar8 u8Data)
{
	if(FingerprintDataCnt >= (sizeof(FingerprintDataBuf) -1))
	{
		FingerprintDataCnt = 0;
	}
	FingerprintDataBuf[FingerprintDataCnt++] = u8Data;
}

/***********************************************************************************************
* Function Name  : Clean_RS485RxdBuf
* Description	 : 清除RS485接收缓存区
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Clean_RS485RxdBuf(void)
{
	memset(RS485DataBuf, 0x0, sizeof(RS485DataBuf));
}

/***********************************************************************************************
* Function Name  : Store_Rs485Data
* Description	 : 存储RS485的数据
* Input 		 : u8Data -- 装入的数据
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Store_Rs485Data(uchar8 u8Data)
{
	if(RS485DataCnt >= (sizeof(RS485DataBuf) -1))
	{
		RS485DataCnt = 0;
	}
	RS485DataBuf[RS485DataCnt++] = u8Data;
}

/***********************************************************************************************
* Function Name  : RS485_Init
* Description    : RS485初始化
* Input          : u32Baudrate -- 通讯波特率
* Output         : None
* Return         : None
***********************************************************************************************/
void RS485_Init(void)
{
	LPC_GPIO1->FIODIR |= (1<<CS485);

	RS485_Txd_Disable;
}

/***********************************************************************************************
* Function Name  : UART0_Init
* Description    : 串口0初始化
* Input          : u32Baudrate -- 通讯波特率
* Output         : None
* Return         : None
***********************************************************************************************/
void UART0_Init(uint32 u32Baudrate)
{
    uint32 Fdiv;

    LPC_SC->PCONP = LPC_SC->PCONP|0x08;

	LPC_PINCON->PINSEL0 |= (1 << 4);             /* Pin P0.2 used as TXD0 (Com0) */
    LPC_PINCON->PINSEL0 |= (1 << 6);             /* Pin P0.3 used as RXD0 (Com0) */
  
    //8为数据位，无奇偶检验位，1位停止位
     
    LPC_UART0->LCR = 0x83;
    Fdiv = (SystemCoreClock/4 / 16 ) / u32Baudrate ;                    
    LPC_UART0->DLM = Fdiv / 256;
    LPC_UART0->DLL = Fdiv % 256;
    LPC_UART0->LCR = 0x03;           //锁定除数访问                
	LPC_UART0->IER = 0x01;			 //允许产生接收中断             
	LPC_UART0->FCR = 0x07;           //控制UARTn RX和TX FIFO的操作。接收数据一个字节触发中断

//	NVIC_SetPriority(UART1_IRQn, NVIC_PRIORTY_UART0); // 设置中断优先级
	NVIC_EnableIRQ(UART0_IRQn);

	RS485_Init();
}

void UART0_IRQHandler(void)                	//串口1中断服务程序
{
	cRxdByte = LPC_UART0->RBR;

	Store_Rs485Data(cRxdByte);
} 

uchar8 UART0_GetChar(void) 
{
  	while (!(LPC_UART0->LSR & 0x01));
  	return (LPC_UART0->RBR);
}

/***********************************************************************************************
* Function Name  : UART0_SendByte
* Description    : 串口0发送1字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
void UART0_SendByte(uchar8 Data)
{
	LPC_UART0->THR = Data;			       		// 发送数据
	while((LPC_UART0->LSR&0x40)==0 );	   		// 等待数据发送完毕
}

/***********************************************************************************************
* Function Name  : UART0_SendNxByte
* Description    : 串口0发送N字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
void UART0_SendNxByte(uchar8* pStrBuf, uint16 len)
{
	uint16 i;
	for(i=0; i<len; i++)
	{
		UART0_SendByte(pStrBuf[i]);
	}
}

/***********************************************************************************************
* Function Name  : UART0_SendStr
* Description    : 串口0发送字符串 
* Input          : pStr -- 待发送字符串指针
* Output         : None
* Return         : None
***********************************************************************************************/
void UART0_SendStr(uchar8* pStr)
{
	uint16 iLen = strlen((char*)pStr);
	UART0_SendNxByte(pStr, iLen);
}

/***********************************************************************************************
* Function Name  : RS485_SendNxByte
* Description    : RS485发送N字节数据
* Input          : pDataBuf -- 待发送数据, len -- 数据长度
* Output         : None
* Return         : None
***********************************************************************************************/
void RS485_SendNxByte(uchar8* pDataBuf, uint16 len)
{
	RS485_Txd_Enable;
	UART0_SendNxByte(pDataBuf, len);
	RS485_Txd_Disable;
}


/***********************************************************************************************
* Function Name  : UART1_Init
* Description    : 串口1初始化
* Input          : u32Baudrate -- 通讯波特率
* Output         : None
* Return         : None
***********************************************************************************************/
void UART1_Init(uint32 u32Baudrate)
{
    uint32 Fdiv;

    LPC_SC->PCONP= LPC_SC->PCONP |0x10;

	LPC_PINCON->PINSEL4 |= (2 << 0);		// 设置引脚 P2.0 为 TXD1
	LPC_PINCON->PINSEL4 |= (2 << 2);		// 设置引脚 P2.1 为 RXD1

    /*
     * 8为数据位，无奇偶检验位，1位停止位
     */
    LPC_UART1->LCR = 0x83;
    Fdiv = (SystemCoreClock/4 / 16 ) / u32Baudrate;                
    LPC_UART1->DLM = Fdiv / 256;
    LPC_UART1->DLL = Fdiv % 256;
    LPC_UART1->LCR = 0x03;                                          
	LPC_UART1->IER = 0x01;	
	LPC_UART1->FCR = 0x07;          //控制UARTn RX和TX FIFO的操作。接收数据一个字节触发中断

    NVIC_EnableIRQ(UART1_IRQn);
}

/***********************************************************************************************
* Function Name  : UART1_SendByte
* Description    : 串口1发送1字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
void UART1_SendByte(uchar8 Data)
{
	LPC_UART1->THR = Data;			       		// 发送数据
	while((LPC_UART1->LSR&0x40)==0 );	   		// 等待数据发送完毕
}

/***********************************************************************************************
* Function Name  : UART1_SendNxByte
* Description    : 串口1发送1字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
void UART1_SendNxByte(uchar8* pStrBuf,uint16 len)
{
	uint16 i;
	for(i=0; i<len; i++)
	{
		UART1_SendByte(pStrBuf[i]);
	}
}

void UART1_IRQHandler(void)                	//串口1中断服务程序
{
	cRxdByte = LPC_UART1->RBR;

	//UART1_SendByte(cRxdByte); //Test
	StoreFingerData(cRxdByte);
} 


/***********************************************************************************************
* Function Name  : UART2_Init
* Description    : 串口2初始化
* Input          : u32Baudrate -- 通讯波特率
* Output         : None
* Return         : None
***********************************************************************************************/
void UART2_Init(uint32 u32Baudrate)
{
    uint32 Fdiv;

   	LPC_SC->PCONP = LPC_SC->PCONP |(1<<24);	      //打开UART2电源控制位
   	
	LPC_PINCON->PINSEL4 |= (2 << 16);		// 设置引脚 P2.8 为 TXD2
	LPC_PINCON->PINSEL4 |= (2 << 18);		// 设置引脚 P2.9 为 RXD2

    LPC_UART2->LCR = 0x83;
    Fdiv = (SystemCoreClock/4 / 16 ) / u32Baudrate;                
    LPC_UART2->DLM = Fdiv / 256;
    LPC_UART2->DLL = Fdiv % 256;
    LPC_UART2->LCR = 0x03;                                          
	LPC_UART2->IER = 0x01;	
	LPC_UART2->FCR = 0x07;          //控制UARTn RX和TX FIFO的操作。接收数据一个字节触发中断

    NVIC_EnableIRQ(UART2_IRQn);
}

void UART2_IRQHandler(void)                	//串口2中断服务程序
{	
	cRxdByte = LPC_UART2->RBR;
} 

/***********************************************************************************************
* Function Name  : UART2_SendByte
* Description    : 串口2发送1字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
void UART2_SendByte(uchar8 Data)
{ 
	LPC_UART2->THR = Data;			       		// 发送数据
	while((LPC_UART2->LSR&0x40)==0 );	   		// 等待数据发送完毕
}

/***********************************************************************************************
* Function Name  : UART2_SendNxByte
* Description    : 串口2发送N字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
void UART2_SendNxByte(uchar8* pStrBuf,uint16 len)
{
	uint16 i;
	for(i=0; i<len; i++)
	{
		UART2_SendByte(pStrBuf[i]);
	}
}
/***********************************************************************************************
* Function Name  : UART3_Init
* Description    : 串口3初始化
* Input          : u32Baudrate -- 通讯波特率
* Output         : None
* Return         : None
***********************************************************************************************/
void UART3_Init(uint32 u32Baudrate)
{
    uint32 Fdiv;

   	LPC_SC->PCONP = LPC_SC->PCONP |(1<<25);	      //打开UART3电源控制位
   	
	LPC_PINCON->PINSEL1 |= (0x03 << 18);		  //设置引脚 P0.25 为 TXD2
	LPC_PINCON->PINSEL1 |= (0x03 << 20);		  //设置引脚 P0.26 为 RXD2

    LPC_UART3->LCR = 0x83;
    Fdiv = (SystemCoreClock/4 / 16 ) / u32Baudrate;                
    LPC_UART3->DLM = Fdiv / 256;
    LPC_UART3->DLL = Fdiv % 256;
    LPC_UART3->LCR = 0x03;                                          
	LPC_UART3->IER = 0x01;	
	LPC_UART3->FCR = 0x07;          //控制UARTn RX和TX FIFO的操作。接收数据一个字节触发中断

    NVIC_EnableIRQ(UART3_IRQn);
}

/***********************************************************************************************
* Function Name  : UART3_SendByte
* Description    : 串口3发送1字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
void UART3_SendByte(uchar8 Data)
{ 
	LPC_UART3->THR = Data;			       		// 发送数据
	while((LPC_UART3->LSR&0x40)==0 );	   		// 等待数据发送完毕										 	
}

/***********************************************************************************************
* Function Name  : UART3_SendNxByte
* Description    : 串口3发送N字节数据
* Input          : Data -- 待发送数据
* Output         : None
* Return         : None
***********************************************************************************************/
void UART3_SendNxByte(uchar8* pStrBuf,uint16 len)
{
	uint16 i;
	for(i=0; i<len; i++)
	{
		UART3_SendByte(pStrBuf[i]);
	}
}

void UART3_IRQHandler(void)                	
{
	cRxdByte = LPC_UART3->RBR;

	if((COMM_TYPE_FREE == gstSysRunInfo.CommType)||(COMM_TYPE_UART_03 == gstSysRunInfo.CommType))
	{
		if(COMM_TYPE_FREE == gstSysRunInfo.CommType)
		{
			gstSysRunInfo.CommType = COMM_TYPE_UART_03;
		}
		Comm_Decoding(cRxdByte);
	}
} 

/*========================File end=====================================================================*/

