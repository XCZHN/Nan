/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_Wiegand.c                                                                       
==                                                                                         
**  Function:	韦根(Wiegand)接收协议
==                                                                                         
**  Data:       2016/07/11                                                                         
================================================================================================*/


#include "Drv_Wiegand.h"
#include "Drv_WDT.h"
#include "Drv_Uart.h"


volatile ST_WG_TYPE gstWG;

/***********************************************************************************************
* Function Name  : Wiegand_Init
* Description	 : 独立看门狗初始化
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void Wiegand_Init(void)
{ 
	LPC_GPIOINT->IO2IntEnF |= (1<<WG_D0);//使能
	LPC_GPIOINT->IO2IntEnF |= (1<<WG_D1);//使能
	NVIC_EnableIRQ(EINT3_IRQn);          //开启外部中断3

	LPC_GPIO2->FIODIR &=~ (1<<WG_D0);//输入
	LPC_GPIO2->FIODIR &=~ (1<<WG_D1);//输入

	memset((uchar8*)&gstWG, 0x0, sizeof(ST_WG_TYPE));
}

void L_MOVE(void)
{
	uchar8 i = 0;
	
	for(i=5; i>0; i--)
	{
		if(gstWG.crc_f2)
		{
			gstWG.crc_f2 = gstWG.DataBuf[i]&0x80;
			gstWG.DataBuf[i] = gstWG.DataBuf[i]<<1;
			gstWG.DataBuf[i] += 1;
		}
		else
		{
			gstWG.crc_f2 = gstWG.DataBuf[i]&0x80;
			gstWG.DataBuf[i] = gstWG.DataBuf[i]<<1;
		}
	}
}

void R_MOVE(void)
{
	uchar8 i = 0;
	
	gstWG.crc_f2 = 0;
	for(i=0; i<6; i++)
	{
		if(gstWG.crc_f2)
		{
			gstWG.crc_f2 = gstWG.DataBuf[i]&0x01;
			gstWG.DataBuf[i] = gstWG.DataBuf[i]>>1;
			gstWG.DataBuf[i] += 0x80;
		}
		else
		{
			gstWG.crc_f2 = gstWG.DataBuf[i]&0x01;
			gstWG.DataBuf[i] = gstWG.DataBuf[i]>>1;
		}		
	}
}

/***********************************************************************************************
* Function Name  : ID_REQUEST
* Description	 : 韦根接收ID卡号--解析数据
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
uchar8 ID_REQUEST(void)
{
	uint32 i,j;
	uint32 iTimeCount = 0;
	
	for(j=0; j<100; j++)
	{
		WDT_Feed();
		if(((LPC_GPIO2->FIOPIN0&0x30)!=0x30)&&((LPC_GPIO2->FIOPIN0&0x30) !=0))		
		{
			gstWG.BitCnt = 0;
			if(LPC_GPIO2->FIOPIN0&0x10)
			{
				gstWG.crc_f1 = 1;
			}
			else 
			{
				gstWG.crc_f1 = 0;
			}

			iTimeCount = 0;
			while((LPC_GPIO2->FIOPIN0&0x30) != 0x30)
			{
				WDT_Feed();
				iTimeCount ++;
				if(iTimeCount > WG_TIME_OUT)
				{
					return 0;
				}
			}
			gstWG.BitCnt +=1;
			memset((uchar8*)gstWG.DataBuf, 0x0, 6);
			
			for(i=0;i<70000;i++)
			{
				WDT_Feed();
				if(!(LPC_GPIO2->FIOPIN0&0x10))
				{
					gstWG.crc_f2 = 0;
					L_MOVE();

					iTimeCount = 0;
					while((LPC_GPIO2->FIOPIN0&0x30)!=0x30)
					{
						WDT_Feed();
						iTimeCount ++;
						if(iTimeCount > WG_TIME_OUT)
						{
							return 0;
						}
					}
					i = 0;
					gstWG.BitCnt += 1;
				}
				else if(!(LPC_GPIO2->FIOPIN0&0x20))
				{
					gstWG.crc_f2 = 1;
					L_MOVE();

					iTimeCount = 0;
					while((LPC_GPIO2->FIOPIN0&0x30) !=0x30)
					{
						WDT_Feed();
						iTimeCount ++;
						if(iTimeCount > WG_TIME_OUT)
						{
							return 0;
						}
					}
					i = 0;
					gstWG.BitCnt += 1;
				}
			}
	
			if(gstWG.BitCnt == 26)
			{
				R_MOVE();
				for(i=0; i<12; i++)
				{
					WDT_Feed();
					if(i < 8)
					{
						if(gstWG.DataBuf[3]&(1<<i))
						{
							gstWG.crc_f1 = !gstWG.crc_f1;
						}
						if(gstWG.DataBuf[5]&(1<<i))
						{
							gstWG.crc_f2 = !gstWG.crc_f2;
						}
					}
					else
					{
						if(gstWG.DataBuf[4]&(1<<(i-4)))
						{
							gstWG.crc_f1 = !gstWG.crc_f1;
						}
						if(gstWG.DataBuf[4]&(1<<(i-8)))
						{
							gstWG.crc_f2 = !gstWG.crc_f2;
						}
					}
					
				}
				if(gstWG.crc_f1|(!gstWG.crc_f2))
				{
					return 0;
				}
			}
			else if(gstWG.BitCnt == 34)
			{
				R_MOVE();
				for(i=0; i<16; i++)
				{
					WDT_Feed();
					if(i < 8)
					{
						if(gstWG.DataBuf[2]&(1<<i))
						{
							gstWG.crc_f1 = !gstWG.crc_f1;
						}
						if(gstWG.DataBuf[4]&(1<<i))
						{
							gstWG.crc_f2 = !gstWG.crc_f2;
						}
					}
					else
					{
						if(gstWG.DataBuf[3]&(1<<(i-8)))
						{
							gstWG.crc_f1 = !gstWG.crc_f1;
						}
						if(gstWG.DataBuf[5]&(1<<(i-8)))
						{
							gstWG.crc_f2 = !gstWG.crc_f2;
						}
					}
					
				}
				if(gstWG.crc_f1|(!gstWG.crc_f2))
				{
					return 0;
				}
			}
			else if(gstWG.BitCnt == 35)
			{
				R_MOVE();
				for(i=0; i<16; i++)
				{
					WDT_Feed();
					if(i < 8)
					{
						if(gstWG.DataBuf[2]&(1<<i))
						{
							gstWG.crc_f1 = !gstWG.crc_f1;
						}
						if(gstWG.DataBuf[4]&(1<<i))
						{
							gstWG.crc_f2 = !gstWG.crc_f2;
						}
					}
					else
					{
						if(gstWG.DataBuf[3]&(1<<(i-8)))
						{
							gstWG.crc_f1 = !gstWG.crc_f1;
						}
						if(gstWG.DataBuf[5]&(1<<(i-8)))
						{
							gstWG.crc_f2 = !gstWG.crc_f2;
						}
					}
					
				}
				if(gstWG.crc_f1|(!gstWG.crc_f2))
				{
				//	return 0;
				}
			}
			else
			{
				return 0;
			}
			return 1;
		}
	}
	return 0;
}

/***********************************************************************************************
* Function Name  : EINT3_IRQHandler
* Description	 : 外部中断3接收程序
* Input 		 : None
* Output		 : None
* Return		 : None
***********************************************************************************************/
void EINT3_IRQHandler(void)
{
	Disable_IRQ();
	LPC_GPIOINT->IO2IntClr |= (1<<WG_D0);
	LPC_GPIOINT->IO2IntClr |= (1<<WG_D1);
	
	if(1 == ID_REQUEST())
	{
		gstWG.bSignalFlag = 1;
	}
	Enable_IRQ();
}

/***********************************************************************************************
* Function Name  : Wiegand_ReardCardID
* Description	 : Wiegand接口接收读取ID卡号
* Input 		 : None
* Output		 : None
* Return		 : 接收到的ID卡号
***********************************************************************************************/
uint32 Wiegand_ReardCardID(void)
{
	uint32 iCardID = 0;

	if(gstWG.bSignalFlag)
	{
		gstWG.bSignalFlag = 0;

		iCardID = gstWG.DataBuf[3];
		iCardID <<= 8;
		iCardID += gstWG.DataBuf[4];
		iCardID <<= 8;
		iCardID += gstWG.DataBuf[5];
	}
	return iCardID;
}


/*========================File end=====================================================================*/

