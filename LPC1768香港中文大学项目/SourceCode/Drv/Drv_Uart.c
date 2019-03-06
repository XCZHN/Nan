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
**  Function:	����ͨ����������� 
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
* Description	 : �洢ָ��ͷ������
* Input 		 : u8Data -- װ�������
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
* Description	 : ���RS485���ջ�����
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
* Description	 : �洢RS485������
* Input 		 : u8Data -- װ�������
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
* Description    : RS485��ʼ��
* Input          : u32Baudrate -- ͨѶ������
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
* Description    : ����0��ʼ��
* Input          : u32Baudrate -- ͨѶ������
* Output         : None
* Return         : None
***********************************************************************************************/
void UART0_Init(uint32 u32Baudrate)
{
    uint32 Fdiv;

    LPC_SC->PCONP = LPC_SC->PCONP|0x08;

	LPC_PINCON->PINSEL0 |= (1 << 4);             /* Pin P0.2 used as TXD0 (Com0) */
    LPC_PINCON->PINSEL0 |= (1 << 6);             /* Pin P0.3 used as RXD0 (Com0) */
  
    //8Ϊ����λ������ż����λ��1λֹͣλ
     
    LPC_UART0->LCR = 0x83;
    Fdiv = (SystemCoreClock/4 / 16 ) / u32Baudrate ;                    
    LPC_UART0->DLM = Fdiv / 256;
    LPC_UART0->DLL = Fdiv % 256;
    LPC_UART0->LCR = 0x03;           //������������                
	LPC_UART0->IER = 0x01;			 //������������ж�             
	LPC_UART0->FCR = 0x07;           //����UARTn RX��TX FIFO�Ĳ�������������һ���ֽڴ����ж�

//	NVIC_SetPriority(UART1_IRQn, NVIC_PRIORTY_UART0); // �����ж����ȼ�
	NVIC_EnableIRQ(UART0_IRQn);

	RS485_Init();
}

void UART0_IRQHandler(void)                	//����1�жϷ������
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
* Description    : ����0����1�ֽ�����
* Input          : Data -- ����������
* Output         : None
* Return         : None
***********************************************************************************************/
void UART0_SendByte(uchar8 Data)
{
	LPC_UART0->THR = Data;			       		// ��������
	while((LPC_UART0->LSR&0x40)==0 );	   		// �ȴ����ݷ������
}

/***********************************************************************************************
* Function Name  : UART0_SendNxByte
* Description    : ����0����N�ֽ�����
* Input          : Data -- ����������
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
* Description    : ����0�����ַ��� 
* Input          : pStr -- �������ַ���ָ��
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
* Description    : RS485����N�ֽ�����
* Input          : pDataBuf -- ����������, len -- ���ݳ���
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
* Description    : ����1��ʼ��
* Input          : u32Baudrate -- ͨѶ������
* Output         : None
* Return         : None
***********************************************************************************************/
void UART1_Init(uint32 u32Baudrate)
{
    uint32 Fdiv;

    LPC_SC->PCONP= LPC_SC->PCONP |0x10;

	LPC_PINCON->PINSEL4 |= (2 << 0);		// �������� P2.0 Ϊ TXD1
	LPC_PINCON->PINSEL4 |= (2 << 2);		// �������� P2.1 Ϊ RXD1

    /*
     * 8Ϊ����λ������ż����λ��1λֹͣλ
     */
    LPC_UART1->LCR = 0x83;
    Fdiv = (SystemCoreClock/4 / 16 ) / u32Baudrate;                
    LPC_UART1->DLM = Fdiv / 256;
    LPC_UART1->DLL = Fdiv % 256;
    LPC_UART1->LCR = 0x03;                                          
	LPC_UART1->IER = 0x01;	
	LPC_UART1->FCR = 0x07;          //����UARTn RX��TX FIFO�Ĳ�������������һ���ֽڴ����ж�

    NVIC_EnableIRQ(UART1_IRQn);
}

/***********************************************************************************************
* Function Name  : UART1_SendByte
* Description    : ����1����1�ֽ�����
* Input          : Data -- ����������
* Output         : None
* Return         : None
***********************************************************************************************/
void UART1_SendByte(uchar8 Data)
{
	LPC_UART1->THR = Data;			       		// ��������
	while((LPC_UART1->LSR&0x40)==0 );	   		// �ȴ����ݷ������
}

/***********************************************************************************************
* Function Name  : UART1_SendNxByte
* Description    : ����1����1�ֽ�����
* Input          : Data -- ����������
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

void UART1_IRQHandler(void)                	//����1�жϷ������
{
	cRxdByte = LPC_UART1->RBR;

	//UART1_SendByte(cRxdByte); //Test
	StoreFingerData(cRxdByte);
} 


/***********************************************************************************************
* Function Name  : UART2_Init
* Description    : ����2��ʼ��
* Input          : u32Baudrate -- ͨѶ������
* Output         : None
* Return         : None
***********************************************************************************************/
void UART2_Init(uint32 u32Baudrate)
{
    uint32 Fdiv;

   	LPC_SC->PCONP = LPC_SC->PCONP |(1<<24);	      //��UART2��Դ����λ
   	
	LPC_PINCON->PINSEL4 |= (2 << 16);		// �������� P2.8 Ϊ TXD2
	LPC_PINCON->PINSEL4 |= (2 << 18);		// �������� P2.9 Ϊ RXD2

    LPC_UART2->LCR = 0x83;
    Fdiv = (SystemCoreClock/4 / 16 ) / u32Baudrate;                
    LPC_UART2->DLM = Fdiv / 256;
    LPC_UART2->DLL = Fdiv % 256;
    LPC_UART2->LCR = 0x03;                                          
	LPC_UART2->IER = 0x01;	
	LPC_UART2->FCR = 0x07;          //����UARTn RX��TX FIFO�Ĳ�������������һ���ֽڴ����ж�

    NVIC_EnableIRQ(UART2_IRQn);
}

void UART2_IRQHandler(void)                	//����2�жϷ������
{	
	cRxdByte = LPC_UART2->RBR;
} 

/***********************************************************************************************
* Function Name  : UART2_SendByte
* Description    : ����2����1�ֽ�����
* Input          : Data -- ����������
* Output         : None
* Return         : None
***********************************************************************************************/
void UART2_SendByte(uchar8 Data)
{ 
	LPC_UART2->THR = Data;			       		// ��������
	while((LPC_UART2->LSR&0x40)==0 );	   		// �ȴ����ݷ������
}

/***********************************************************************************************
* Function Name  : UART2_SendNxByte
* Description    : ����2����N�ֽ�����
* Input          : Data -- ����������
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
* Description    : ����3��ʼ��
* Input          : u32Baudrate -- ͨѶ������
* Output         : None
* Return         : None
***********************************************************************************************/
void UART3_Init(uint32 u32Baudrate)
{
    uint32 Fdiv;

   	LPC_SC->PCONP = LPC_SC->PCONP |(1<<25);	      //��UART3��Դ����λ
   	
	LPC_PINCON->PINSEL1 |= (0x03 << 18);		  //�������� P0.25 Ϊ TXD2
	LPC_PINCON->PINSEL1 |= (0x03 << 20);		  //�������� P0.26 Ϊ RXD2

    LPC_UART3->LCR = 0x83;
    Fdiv = (SystemCoreClock/4 / 16 ) / u32Baudrate;                
    LPC_UART3->DLM = Fdiv / 256;
    LPC_UART3->DLL = Fdiv % 256;
    LPC_UART3->LCR = 0x03;                                          
	LPC_UART3->IER = 0x01;	
	LPC_UART3->FCR = 0x07;          //����UARTn RX��TX FIFO�Ĳ�������������һ���ֽڴ����ж�

    NVIC_EnableIRQ(UART3_IRQn);
}

/***********************************************************************************************
* Function Name  : UART3_SendByte
* Description    : ����3����1�ֽ�����
* Input          : Data -- ����������
* Output         : None
* Return         : None
***********************************************************************************************/
void UART3_SendByte(uchar8 Data)
{ 
	LPC_UART3->THR = Data;			       		// ��������
	while((LPC_UART3->LSR&0x40)==0 );	   		// �ȴ����ݷ������										 	
}

/***********************************************************************************************
* Function Name  : UART3_SendNxByte
* Description    : ����3����N�ֽ�����
* Input          : Data -- ����������
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

