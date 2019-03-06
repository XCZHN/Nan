/*====================================================================================
==                                                                                              
**  Copyright(c) 2014-09  Asen. All rights reserved.                                 
==                                                                                           
**	Author:		Asen                                                                        
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_FM1702SL.c                                                                       
==                                                                                         
**  Function:	读卡芯片 FM1702SL/FM1701 驱动控制程序
==                                                                                         
**  Data:       2015/09/22                                                                       
=====================================================================================*/

#include <string.h>
//#include "Mid_SysConfig.h"

#include "Drv_RFM.h"
#include "Drv_SPI.h"

uchar8	g_Card_RevBuffer[40];			// command send/receive buffer
uchar8  g_Card_tagtype[2];				// 卡片标识字符 
uchar8	g_Card_UID[5];					// 存放物理卡号

extern void delay_ms(uint32 nms);
	
void  FM_Delay_us(uint32 xus)
{
	volatile int i, j;
	for(i = 0; i < xus; i++)	
	{
		for(j = 0; j < 3; j++);
	}
}

void  FM_Delay_ms(uint32 xms)
{	
	delay_ms(xms);
}

void FM_SPI_TxdByte(uchar8 da)
{
	SPI_Send1Byte(da);
}

uchar8 FM_SPI_RxdByte(void)
{
	uchar8 da = SPI_Read1Byte();
	return	da;
}

uchar8 FM_Read_Reg(uchar8 reg_add)
{
	uchar8 valu = 0;
	uchar8 mid_data = reg_add;
	 
	FM1702SPI_Enable();
	mid_data <<= 1;		//read
	mid_data |= 0x80;
	FM_SPI_TxdByte(mid_data);
	valu = FM_SPI_RxdByte();
	FM1702SPI_Disable();
	FM_Delay_us(0);

	return valu;
}

void FM_Write_Reg(uchar8 reg_add, uchar8 reg_data)
{
	uchar8	mid_data = reg_add;
	 
	FM1702SPI_Enable();
	mid_data <<= 1;		//write
	mid_data &= 0x7f;
	FM_SPI_TxdByte(mid_data);
	FM_SPI_TxdByte(reg_data);
	FM1702SPI_Disable();
	FM_Delay_us(0);
}

void FM1702SL_Config(void)
{
	int i = 0;
	
	FM_Write_Reg(FM1702SL_REG_Page_Sel,0x80);		    /* select FM1715 control bus */

	for(i=0; i<30; i++)    			
	{
		if(0 == FM_Read_Reg(FM1702SL_REG_Command))
		{
			FM_Write_Reg(FM1702SL_REG_Page_Sel,0x00);
		}
	}
	FM_Write_Reg(FM1702SL_REG_TimerClock,0x0b);

	FM_Write_Reg(FM1702SL_REG_TimerControl,0x02);
	FM_Write_Reg(FM1702SL_REG_TimerReload,0x42);

	FM_Write_Reg(FM1702SL_REG_InterruptEn,0x7f);

	FM_Write_Reg(FM1702SL_REG_Int_Req,0x7f);
	FM_Write_Reg(FM1702SL_REG_MFOUTSelect,0x02);

	FM_Write_Reg(FM1702SL_REG_TxControl,0x5b);
	FM_Write_Reg(FM1702SL_REG_RxControl2,0x01);
	FM_Write_Reg(FM1702SL_REG_RxWait,0x07);

	FM_Write_Reg(FM1702SL_REG_TypeSH,0x00);
}


uchar8	RF_Init(void)
{
	uchar8	timeout = 0;

	LPC_PINCON->PINSEL2 &= 0xFF3FFFFF;
	LPC_GPIO1->FIODIR |= (1 << FM_RST_pin);
	FM_RST_L;

	FM_Delay_ms(5);		
	FM_RST_H;			//reset
	FM_Delay_ms(25);		
	FM_RST_L;
	FM_Delay_ms(15);	//等待成功复位

	while(FM_Read_Reg(FM1702SL_REG_Command) != 0)		// 等待Command = 0,FM1702复位成功 
	{
		FM_Delay_ms(5);
		timeout++;
		if(timeout > 20)
		{
			return	1;
		}
	}

	FM1702SL_Config();

	return 0;
}


//----------------------------------------------------------------------------
uchar8 Clear_FIFO(void)
{
	uchar8	temp;
	int	i;
	temp = FM_Read_Reg(FM1702SL_REG_Control);		/* 清空FIFO */
	temp = (temp | 0x01);
	FM_Write_Reg(FM1702SL_REG_Control,temp);
	for(i=0; i<RF_TimeOut; i++) 					/* 检查FIFO是否被清空 */
	{
		temp = FM_Read_Reg(FM1702SL_REG_FIFO_Length);
		if(temp == 0)
		{
			return 0;
		}
	}
	return 1;
}

void Write_FIFO(uchar8 count, uchar8 *buff)
{
	int	i;

	for(i=0; i<count; i++)
	{
		FM_Write_Reg(FM1702SL_REG_FIFO,*(buff + i));
	}
}

uchar8	Read_FIFO(uchar8  *buff)
{
	uchar8	len = 0;
	uchar8	i = 0;

	len = FM_Read_Reg(FM1702SL_REG_FIFO_Length);
	if(len == 0)
	{
		return 0;
	}

	if(len >= 24)
	{
		len = 24;
	}

	for(i = 0; i < len; i++)
	{
		*(buff + i) = FM_Read_Reg(FM1702SL_REG_FIFO);
	}

	return	len;
}

uchar8 Judge_Req(uchar8 *buff)
{
	uchar8	temp1, temp2;

	temp1 = *buff;
	temp2 = *(buff + 1);
	if((temp1 == 0x02) || (temp1 == 0x04) || (temp1 == 0x05) || (temp1 == 0x08) || (temp1 == 0x53) || (temp1 == 0x03))
	{
		if (temp2 == 0x00)
		{
			return TRUE;
		}
	}
	return FALSE;
}

uchar8 Command_Send(uchar8 count, uchar8 *buff, uchar8 Comm_Set)
{
	uint32	j;
	uchar8	temp, temp1;

	FM_Write_Reg(FM1702SL_REG_Command, 0x00);
	Clear_FIFO();
	Write_FIFO(count, buff);

	FM_Write_Reg(FM1702SL_REG_Command, Comm_Set);		// 命令执行 

	for(j = 0; j < RF_TimeOut; j++) 	// 检查命令执行否 
	{
		temp  = FM_Read_Reg(FM1702SL_REG_Command);
		temp1 = FM_Read_Reg(FM1702SL_REG_Int_Req) & 0x80;
		if((temp == 0x00) || (temp1 == 0x80))
		{
			return TRUE;
		}
	}

	return FALSE;
}


uchar8	MIF_Request(uchar8 mode)
{
	uchar8	temp;

	//FM_Write_Reg(FM1702SL_REG_CRCPresetLSB,0x63);		
	//FM_Write_Reg(FM1702SL_REG_CWConductance,0x3f);
	
	g_Card_RevBuffer[0] = mode;								//select Request mode			
	FM_Write_Reg(FM1702SL_REG_Bit_Frame,0x07);				//send 7 bit
	FM_Write_Reg(FM1702SL_REG_ChannelRedundancy,0x03);  	//closed CRC
	temp = FM_Read_Reg(FM1702SL_REG_Control);
	temp = temp & (0xf7);
	FM_Write_Reg(FM1702SL_REG_Control,temp);				//Control reset value is 00
	temp = Command_Send(1, g_Card_RevBuffer, Transceive);   //Transceive=0x1E	 发送接收命令 
	if(temp == FALSE)
	{
		return FM1702_NOTAGERR;
	}

	Read_FIFO(g_Card_RevBuffer);							// 从FIFO中读取应答信息到RevBuffer[]中
	temp = Judge_Req(g_Card_RevBuffer);						// 判断应答信号是否正确 
	if(temp == TRUE)
	{
		g_Card_tagtype[0] = g_Card_RevBuffer[0];
		g_Card_tagtype[1] = g_Card_RevBuffer[1];
		return FM1702_OK;
	}

	return FM1702_REQERR;
}

uchar8 Check_UID(void)
{
	uchar8	temp;
	uchar8	i;
	
	temp = 0x00;
	for(i = 0; i < 5; i++)
	{
		temp = temp ^ g_Card_UID[i];
	}

	if(temp == 0)
	{
		return TRUE;
	}
	return FALSE;
}

void Save_UID(uchar8 row, uchar8 col, uchar8 length)
{
	uchar8	i;
	uchar8	temp;
	uchar8	temp1;
	
	if((row == 0x00) && (col == 0x00))
	{
		for(i = 0; i < length; i++)
		{
			g_Card_UID[i] = g_Card_RevBuffer[i];
		}
	}
	else
	{
		temp = g_Card_RevBuffer[0];
		temp1 = g_Card_UID[row - 1];
		switch(col)
		{
			case 0:		temp1 = 0x00; row = row + 1; break;
			case 1:		temp = temp & 0xFE; temp1 = temp1 & 0x01; break;
			case 2:		temp = temp & 0xFC; temp1 = temp1 & 0x03; break;
			case 3:		temp = temp & 0xF8; temp1 = temp1 & 0x07; break;
			case 4:		temp = temp & 0xF0; temp1 = temp1 & 0x0F; break;
			case 5:		temp = temp & 0xE0; temp1 = temp1 & 0x1F; break;
			case 6:		temp = temp & 0xC0; temp1 = temp1 & 0x3F; break;
			case 7:		temp = temp & 0x80; temp1 = temp1 & 0x7F; break;
			default:	break;
		}

		g_Card_RevBuffer[0] = temp;
		g_Card_UID[row - 1] = temp1 | temp;
		for(i = 1; i < length; i++)
		{
			g_Card_UID[row - 1 + i] = g_Card_RevBuffer[i];
		}
	}
}

void Set_BitFraming(uchar8 row, uchar8 col)
{
	switch(row)
	{
		case 0:		g_Card_RevBuffer[1] = 0x20; break;
		case 1:		g_Card_RevBuffer[1] = 0x30; break;
		case 2:		g_Card_RevBuffer[1] = 0x40; break;
		case 3:		g_Card_RevBuffer[1] = 0x50; break;
		case 4:		g_Card_RevBuffer[1] = 0x60; break;
		default:	break;
	}

	switch(col)
	{
		case 0:		FM_Write_Reg(FM1702SL_REG_Bit_Frame,0x00); break;
		case 1:		FM_Write_Reg(FM1702SL_REG_Bit_Frame,0x11); g_Card_RevBuffer[1] = (g_Card_RevBuffer[1] | 0x01); break;
		case 2:		FM_Write_Reg(FM1702SL_REG_Bit_Frame,0x22); g_Card_RevBuffer[1] = (g_Card_RevBuffer[1] | 0x02); break;
		case 3:		FM_Write_Reg(FM1702SL_REG_Bit_Frame,0x33); g_Card_RevBuffer[1] = (g_Card_RevBuffer[1] | 0x03); break;
		case 4:		FM_Write_Reg(FM1702SL_REG_Bit_Frame,0x44); g_Card_RevBuffer[1] = (g_Card_RevBuffer[1] | 0x04); break;
		case 5:		FM_Write_Reg(FM1702SL_REG_Bit_Frame,0x55); g_Card_RevBuffer[1] = (g_Card_RevBuffer[1] | 0x05); break;
		case 6:		FM_Write_Reg(FM1702SL_REG_Bit_Frame,0x66); g_Card_RevBuffer[1] = (g_Card_RevBuffer[1] | 0x06); break;
		case 7:		FM_Write_Reg(FM1702SL_REG_Bit_Frame,0x77); g_Card_RevBuffer[1] = (g_Card_RevBuffer[1] | 0x07); break;
		default:	break;
	}
}

uchar8	MIF_AntiColl(void)
{
	uchar8	temp;
	uchar8	i;
	uchar8	row, col;
	uchar8	pre_row;
	
	row = 0;
	col = 0;
	pre_row = 0;
	FM_Write_Reg(FM1702SL_REG_CRCPresetLSB,0x63);
	FM_Write_Reg(FM1702SL_REG_CWConductance,0x3f);
	g_Card_RevBuffer[0] = RF_CMD_ANTICOL;
	g_Card_RevBuffer[1] = 0x20;
	FM_Write_Reg(FM1702SL_REG_ChannelRedundancy,0x03);				//closed CRC,start parity
	temp = Command_Send(2, g_Card_RevBuffer, Transceive);
	while(1)
	{
		if(temp == FALSE)
		{
			return(FM1702_NOTAGERR);
		}
		/*
		temp = FM_Read_Reg(FM1702SL_REG_ErrorFlag);
 		if((temp & 0x02) == 0x02)  return(FM1702_PARITYERR);
		if((temp & 0x04) == 0x04)  return(FM1702_FRAMINGERR);
		*/
		temp = FM_Read_Reg(FM1702SL_REG_FIFO_Length);
		if(temp == 0)
		{
			return FM1702_BYTECOUNTERR;
		}

		Read_FIFO(g_Card_RevBuffer);
		Save_UID(row, col, temp);								    /* 将收到的UID放入UID数组中 */
		
		temp = FM_Read_Reg(FM1702SL_REG_ErrorFlag);				    /* 判断接収数据是否出错 */
		temp = temp & 0x01;
		if(temp == 0x00)
		{
			temp = Check_UID();										/* 校验收到的UID */
			if(temp == FALSE)
			{
				return(FM1702_SERNRERR);
			}

			return(FM1702_OK);
		}
		else
		{
			temp = FM_Read_Reg(FM1702SL_REG_CollPos);				/* 读取冲突检测寄存器 */
			row = temp / 8;
			col = temp % 8;
			g_Card_RevBuffer[0] = RF_CMD_ANTICOL;
			Set_BitFraming(row + pre_row, col);						/* 设置待发送数据的字节数 */
			pre_row = pre_row + row;
			for(i = 0; i < pre_row + 1; i++)
			{
				g_Card_RevBuffer[i + 2] = g_Card_UID[i];
			}

			if(col != 0x00)
			{
				row = pre_row + 1;
			}
			else
			{
				row = pre_row;
			}

			temp = Command_Send(row + 2, g_Card_RevBuffer, Transceive);
		}
	}
}

uchar8 MIF_Select(void)
{
	uchar8	temp, i;
	
	FM_Write_Reg(FM1702SL_REG_CRCPresetLSB, 0x63);
	FM_Write_Reg(FM1702SL_REG_CWConductance, 0x3f);
	g_Card_RevBuffer[0] = RF_CMD_SELECT;
	g_Card_RevBuffer[1] = 0x70;
	for(i = 0; i < 5; i++)
	{
		g_Card_RevBuffer[i + 2] = g_Card_UID[i];
	}

	FM_Write_Reg(FM1702SL_REG_ChannelRedundancy, 0x0f);	
	temp = Command_Send(7, g_Card_RevBuffer, Transceive);
	if(temp == FALSE)
	{
		return(FM1702_NOTAGERR);
	}
	else
	{
		temp = FM_Read_Reg(FM1702SL_REG_ErrorFlag);
		if((temp & 0x02) == 0x02) 
			return(FM1702_PARITYERR);
		
		if((temp & 0x04) == 0x04) 
			return(FM1702_FRAMINGERR);
		
		if((temp & 0x08) == 0x08) 
			return(FM1702_CRCERR);
		
		temp = FM_Read_Reg(FM1702SL_REG_FIFO_Length);
		if(temp != 1) 
			return(FM1702_BYTECOUNTERR);
		
		Read_FIFO(g_Card_RevBuffer);	/* 从FIFO中读取应答信息 */
		temp = *g_Card_RevBuffer;

//		if((temp == 0x18) || (temp == 0x08) || (temp == 0x88) || (temp == 0x53))	/* 判断应答信号是否正确 这里是指定MF1卡，为了兼容模拟MF1卡，所以屏蔽*/
			return(FM1702_OK);
//		else
//			return(FM1702_SELERR);
	}
}

uchar8 MIF_ChgAvailableKey(uchar8 *uncoded, uchar8 *coded)   
{
    uchar8	cnt = 0;
    uchar8	ln  = 0;     
    uchar8	hn  = 0;      
    for (cnt = 0; cnt < 6; cnt++)
    {
        ln = uncoded[cnt] & 0x0F;
        hn = uncoded[cnt] >> 4;
        coded[cnt * 2 + 1] = (~ln << 4) | ln;
        coded[cnt * 2 ] = (~hn << 4) | hn;
    }
    return FM1702_OK;
}

uchar8 MIF_Load_Key(uchar8 *keybuf)
{
	uchar8	status;
	uchar8	coded_keys[14];
	memset(coded_keys, 0x0, sizeof(coded_keys));

	MIF_ChgAvailableKey(keybuf, coded_keys);
	status = Command_Send(0x0c, coded_keys, LoadKey);
	if(status == FALSE)
	{
		return FM1702_LOADKEYERR;
	}

	status=(FM_Read_Reg(FM1702SL_REG_ErrorFlag)) & 0x40;
	if(status == 0x40)
	{
		return FM1702_AUTHERR;
	}
	return FM1702_OK;
}

uchar8 MIF_Authentication(uchar8 *pCardUID, uchar8 SecNR, uchar8 keyType)
{
	
	uchar8	i;
	uchar8	temp, temp1;

	uchar8 temp0;
	if(SecNR >= 0x20)
	{
		temp0 = SecNR -0x20;
		SecNR = 0x20 + temp0 * 4;
	}
	
	FM_Write_Reg(FM1702SL_REG_CRCPresetLSB,0x63);
	FM_Write_Reg(FM1702SL_REG_CWConductance,0x3f);

	if((keyType != RF_CMD_AUTH_LA) && (keyType != RF_CMD_AUTH_LB))
	{			
		return FM1702_AUTHERR;
	}
	else
	{
		g_Card_RevBuffer[0] = keyType;
	}
	g_Card_RevBuffer[1] = SecNR * 4 + 3;
	for(i = 0; i < 4; i++)
	{
		g_Card_RevBuffer[2 + i] = pCardUID[i];
	}

	FM_Write_Reg(FM1702SL_REG_ChannelRedundancy,0x0f);	//start crc,parity verify
	temp = Command_Send(6, g_Card_RevBuffer, Authent1);
	if(temp == FALSE)
	{
		return 0x99;
	}

	temp = FM_Read_Reg(FM1702SL_REG_ErrorFlag);			//ErrorFlag address is 0x0A.
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	temp = Command_Send(0, g_Card_RevBuffer, Authent2);	
	if(temp == FALSE)
	{
		return 0x88;
	}

	temp = FM_Read_Reg(FM1702SL_REG_ErrorFlag);
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	temp1 = FM_Read_Reg(FM1702SL_REG_Control);
	temp1 = temp1 & 0x08;	
	if(temp1 == 0x08)
	{
		return FM1702_OK;
	}

	return FM1702_AUTHERR;
}


uchar8 MIF_Read(uchar8 *databuf, uchar8 Block_Adr)
{
	uchar8	temp;
	
	FM_Write_Reg(FM1702SL_REG_CRCPresetLSB,0x63);
	FM_Write_Reg(FM1702SL_REG_CWConductance,0x3f);
	FM_Write_Reg(FM1702SL_REG_ModConductance,0x3f);
	FM_Write_Reg(FM1702SL_REG_ChannelRedundancy,0x0f);

	databuf[0] = RF_CMD_READ;
	databuf[1] = Block_Adr;
	temp = Command_Send(2, databuf, Transceive);
	if(temp == 0)
	{
		return FM1702_NOTAGERR;
	}

	temp = FM_Read_Reg(FM1702SL_REG_ErrorFlag);
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	temp = FM_Read_Reg(FM1702SL_REG_FIFO_Length);
	if(temp == 0x10)	
	{
		Read_FIFO(databuf);
		return FM1702_OK;
	}
	else if(temp == 0x04)	
	{
		Read_FIFO(databuf);
		return FM1702_OK;
	}
	else
	{
		return FM1702_BYTECOUNTERR;
	}
}

uchar8 MIF_Write(uchar8 *databuf, uchar8 Block_Adr)
{
	uchar8	temp;
	uchar8	*F_buff;
	uchar8	temp_buf[10];

	FM_Write_Reg(FM1702SL_REG_CRCPresetLSB,0x63);
	FM_Write_Reg(FM1702SL_REG_CWConductance,0x3f);
	F_buff = temp_buf;
	FM_Write_Reg(FM1702SL_REG_ChannelRedundancy,0x07);    /* Note: this line is for 1702, different from RC500*/
	*F_buff = RF_CMD_WRITE;
	*(F_buff + 1) = Block_Adr;
	temp = Command_Send(2, F_buff, Transceive);
	if(temp == FALSE)
	{
		return(FM1702_NOTAGERR);
	}

	temp = FM_Read_Reg(FM1702SL_REG_FIFO_Length);
	if(temp == 0)
	{
		return(FM1702_BYTECOUNTERR);
	}

	Read_FIFO(F_buff);
	temp = *F_buff;
	switch(temp)
	{
		case 0x00:	return(FM1702_NOTAUTHERR);	
		case 0x04:	return(FM1702_EMPTY);
		case 0x0a:	break;
		case 0x01:	return(FM1702_CRCERR);
		case 0x05:	return(FM1702_PARITYERR);
		default:	return(FM1702_WRITEERR);
	}

	temp = Command_Send(16, databuf, Transceive);
	if(temp == TRUE)
	{
		return FM1702_OK;
	}
	else
	{
		temp = FM_Read_Reg(FM1702SL_REG_ErrorFlag);
		if(0x0 == temp)
		{
			return FM1702_OK;
		}
		else if((temp & 0x02) == 0x02)
		{
			return(FM1702_PARITYERR);
		}
		else if((temp & 0x04) == 0x04)
		{
			return(FM1702_FRAMINGERR);
		}
		else if((temp & 0x08) == 0x08)
		{
			return(FM1702_CRCERR);
		}
		else
		{
			return(FM1702_WRITEERR);
		}
	}
}

uchar8	MIF_Halt(void)
{
	uchar8	temp;
	
	FM_Write_Reg(FM1702SL_REG_CRCPresetLSB,0x63);
	FM_Write_Reg(FM1702SL_REG_CWConductance,0x3f);
	FM_Write_Reg(FM1702SL_REG_ChannelRedundancy,0x03);
	*g_Card_RevBuffer = RF_CMD_HALT;
	*(g_Card_RevBuffer + 1) = 0x00;
	temp = Command_Send(2, g_Card_RevBuffer, Transmit);
	if(temp == TRUE)
	{
		return FM1702_OK;
	}
	else
	{
		temp = FM_Read_Reg(FM1702SL_REG_ErrorFlag);
		if((temp & 0x02) == 0x02)
		{
			return(FM1702_PARITYERR);
		}

		if((temp & 0x04) == 0x04)
		{
			return(FM1702_FRAMINGERR);
		}
		return FM1702_NOTAGERR;
	}
}

uchar8 MIF_Increment(uchar8 *buff, uchar8 Block_Adr)
{
	uchar8	temp;
	uchar8 *F_buff;
	
	FM_Write_Reg(FM1702SL_REG_CRCPresetLSB,0x63);
	FM_Write_Reg(FM1702SL_REG_CWConductance,0x3f);
	F_buff = buff + 4;
	*F_buff = RF_CMD_INC;
	*(F_buff + 1) = Block_Adr;
	FM_Write_Reg(FM1702SL_REG_ChannelRedundancy,0x07);
	temp = Command_Send(2, F_buff, Transceive);
	if(temp == FALSE)
	{
		return FM1702_NOTAGERR;
	}

	temp = FM_Read_Reg(FM1702SL_REG_FIFO_Length);
	if(temp == 0)
	{
		return FM1702_BYTECOUNTERR;
	}

	Read_FIFO(F_buff);
	temp = *F_buff;
	switch(temp)
	{
	case 0x00:	/* break; */return(FM1702_NOTAUTHERR);
	case 0x04:	return(FM1702_EMPTY);
	case 0x0a:	break;
	case 0x01:	return(FM1702_CRCERR);
	case 0x05:	return(FM1702_PARITYERR);
	default:	return(FM1702_INCRERR);
	}
        
	temp = Command_Send(4, buff, Transmit);
	if(temp == FALSE)
	{
		return FM1702_INCRERR;
	}

	return FM1702_OK;
}

uchar8 MIF_Decrement(uchar8 *buff, uchar8 Block_Adr)
{
	uchar8	temp;
	uchar8 *F_buff;
	
	FM_Write_Reg(FM1702SL_REG_CRCPresetLSB,0x63);
	FM_Write_Reg(FM1702SL_REG_CWConductance,0x3f);
	F_buff = buff + 4;
	*F_buff = RF_CMD_DEC;
	*(F_buff + 1) = Block_Adr;
	FM_Write_Reg(FM1702SL_REG_ChannelRedundancy,0x07);
	temp = Command_Send(2, F_buff, Transceive);
	if(temp == FALSE)
	{
		return FM1702_NOTAGERR;
	}

	temp = FM_Read_Reg(FM1702SL_REG_FIFO_Length);
	if(temp == 0)
	{
		return FM1702_BYTECOUNTERR;
	}

	Read_FIFO(F_buff);
	temp = *F_buff;
	switch(temp)
	{
	case 0x00:	/* break; */return(FM1702_NOTAUTHERR);	
	case 0x04:	return(FM1702_EMPTY);
	case 0x0a:	break;
	case 0x01:	return(FM1702_CRCERR);
	case 0x05:	return(FM1702_PARITYERR);
	default:	return(FM1702_DECRERR);
	}

	temp = Command_Send(4, buff, Transmit);
	if(temp == FALSE)
	{
		return(FM1702_DECRERR);
	}

	return FM1702_OK;
}

uchar8 MIF_Restore(uchar8 Block_Adr)
{	
	uchar8	temp, i;
	
	FM_Write_Reg(FM1702SL_REG_CRCPresetLSB,0x63);
	FM_Write_Reg(FM1702SL_REG_CWConductance,0x3f);
	FM_Write_Reg(FM1702SL_REG_ChannelRedundancy,0x07);
	*g_Card_RevBuffer = RF_CMD_RESTORE;
	*(g_Card_RevBuffer + 1) = Block_Adr;
	temp = Command_Send(2, g_Card_RevBuffer, Transceive);
	if(temp == FALSE)
	{
		return FM1702_NOTAGERR;
	}

	temp = FM_Read_Reg(FM1702SL_REG_FIFO_Length);
	if(temp == 0)
	{
		return FM1702_BYTECOUNTERR;
	}

	Read_FIFO(g_Card_RevBuffer);
	temp = *g_Card_RevBuffer;
	switch(temp)
	{
	case 0x00:	/* break; */return(FM1702_NOTAUTHERR);	
	case 0x04:	return(FM1702_EMPTY);
	case 0x0a:	break;
	case 0x01:	return(FM1702_CRCERR);
	case 0x05:	return(FM1702_PARITYERR);
	default:	return(FM1702_RESTERR);
	}

	for(i = 0; i < 4; i++) g_Card_RevBuffer[i] = 0x00;
	temp = Command_Send(4, g_Card_RevBuffer, Transmit);
	if(temp == FALSE)
	{
		return FM1702_RESTERR;
	}

	return FM1702_OK;
}

uchar8 MIF_Transfer(uchar8 Block_Adr)
{
	uchar8	temp;
	FM_Write_Reg(FM1702SL_REG_CRCPresetLSB,0x63);
	FM_Write_Reg(FM1702SL_REG_CWConductance,0x3f);
	FM_Write_Reg(FM1702SL_REG_ChannelRedundancy,0x07);	
	g_Card_RevBuffer[0] = RF_CMD_TRANSFER;
	g_Card_RevBuffer[1] = Block_Adr;
	temp = Command_Send(2, g_Card_RevBuffer, Transceive);
	if(temp == FALSE)
	{
		return FM1702_NOTAGERR;
	}

	temp = FM_Read_Reg(FM1702SL_REG_FIFO_Length);
	if(temp == 0)
	{
		return FM1702_BYTECOUNTERR;
	}

	Read_FIFO(g_Card_RevBuffer);
	temp = *g_Card_RevBuffer;
	switch(temp)
	{
	case 0x00:	/* break; */return(FM1702_NOTAUTHERR);	
	case 0x04:	return(FM1702_EMPTY);
	case 0x0a:	return(FM1702_OK);
	case 0x01:	return(FM1702_CRCERR);
	case 0x05:	return(FM1702_PARITYERR);
	default:	return(FM1702_TRANSERR);
	}
}

uchar8 RF_ActiveCard(uchar8* pUID, uint8 u8Section, uchar8 u8KeyType, uchar8* pKey)
{
	uchar8	temp;
	
	MIF_Halt();	/* Halt */
	temp = MIF_Request(RF_CMD_REQUEST_STD);		
	if(temp != FM1702_OK)
	{
		return(FM1702_REQERR);
	}

	temp = MIF_AntiColl();				
	if(temp != FM1702_OK)
	{
		return(FM1702_ANTICOLLERR);
	}

	if(memcmp(g_Card_UID, pUID, 4) !=0)
	{
		return 0xFF;
	}
	
	temp = MIF_Select();				
	if(temp != FM1702_OK)
	{
		return(FM1702_SELERR);
	}

	MIF_Load_Key(pKey);	 	
	temp = MIF_Authentication(pUID, u8Section, u8KeyType);	
	if(temp != FM1702_OK)
	{
		return(FM1702_AUTHERR);
	}

	return FM1702_OK;
}



/*---------------------Read/Write Card operation----------------------------------------*/
uchar8	RF_Find_Card(void)
{
	uint8 i = 0;
	
	for (i=0; i<RF_TRY_COUNT; i++)
	{
		if (MIF_Request(RF_CMD_REQUEST_ALL) == 0)		 	   
		{
			return 0;
		}
		else
		{
			MIF_Halt();
		}
	}
	return 0xFF;
}

uchar8	RF_Read_CardUID(uchar8* pUID)
{
	uint8 i = 0;
	
	for (i=0; i<RF_TRY_COUNT; i++)
	{
		if (MIF_Request(RF_CMD_REQUEST_ALL) == 0)  
		{
			if (MIF_AntiColl() == 0)  
			{
				memcpy(pUID, g_Card_UID, 4);
				return 0;
			}			
		}
		else
		{
			MIF_Halt();
			continue ;
		}
	}
	return 0xFF;
}

uint32	RF_GetCardUID(void)
{
	uint32 iCardID = 0;
	uint8 i = 0;
	for (i=0; i<RF_TRY_COUNT; i++)
	{
		if (MIF_Request(RF_CMD_REQUEST_ALL) == 0)  
		{
			if (MIF_AntiColl() == 0)  
			{
				memcpy((uchar8*)&iCardID, g_Card_UID, 4);
				return iCardID;
			}			
		}
		else
		{
			MIF_Halt();
			continue ;
		}
	}
	return iCardID;
}

uint8 RF_ReadCard_1Block(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey, uchar8* pData)
{
	uint8 i = 0;

	if (u8Block > CARD_MAX_BLOCK)  return 0xff;
	
	for (i=0; i<RF_TRY_COUNT; i++)
	{
		if (MIF_Request(RF_CMD_REQUEST_ALL) != 0)		 	   continue;
		if (MIF_AntiColl() != 0)						 	   continue;
		if (memcmp(g_Card_UID, pUID, 4) !=0)				   continue;
		if (MIF_Select() != 0)							 	   continue;
		if (MIF_Load_Key(pKey) !=0)						 	   continue;
		if (MIF_Authentication(pUID, (u8Block/4), u8KeyType))  continue;
		if (MIF_Read(pData, u8Block) != 0)				 	   continue;
		MIF_Halt();
		break;
	}
	if (i == RF_TRY_COUNT)  return 0xff;

	return 0;
}

uint8 RF_WriteCard_1Block(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey, uchar8* pData)
{
	uint8 i = 0;

	if ((u8Block == 0)||(u8Block >= CARD_MAX_BLOCK))  return 0xff;
	
	for (i=0; i<RF_TRY_COUNT; i++)
	{
		if (MIF_Request(RF_CMD_REQUEST_ALL) != 0)		 	   continue;
		if (MIF_AntiColl() != 0)						 	   continue;		
		if (memcmp(g_Card_UID, pUID, 4) !=0)				   continue;
		if (MIF_Select() != 0)							 	   continue;
		if (MIF_Load_Key(pKey) !=0)						 	   continue;
		if (MIF_Authentication(pUID, (u8Block/4), u8KeyType))  continue;
		if (MIF_Write(pData, u8Block) != 0)				 	   continue;
		MIF_Halt();
		break;
	}
	if (i == RF_TRY_COUNT)  return 0xff;

	return 0;
}

uint8 RF_ReadCard(uchar8* pUID, uint8 sector, uchar8 u8KeyType, uchar8* pKey, uchar8* pData)
{
	uint32 i;
	uchar8 keya[6];
	
	memset(keya, 0xFF, sizeof(keya));
	if(sector > CARD_MAX_SECTION)  return 0xff;
	
	for (i=0; i<RF_TRY_COUNT; i++)  
	{
		if (MIF_Request(RF_CMD_REQUEST_ALL) != 0)			continue;
		if (MIF_AntiColl() != 0)							continue;
		if (memcmp(g_Card_UID, pUID, 4) !=0)				continue;
		if (MIF_Select() != 0)								continue;
		if (MIF_Load_Key(pKey) !=0)							continue;
		if (MIF_Authentication(pUID, sector, u8KeyType))  	continue;
		if (MIF_Read(pData, sector*4) != 0)					continue;
		if (MIF_Read(&pData[16], sector*4 +1) != 0)			continue;
		if (MIF_Read(&pData[32], sector*4 +2) != 0)			continue;
		MIF_Halt();
		break;
	}
	if (i == RF_TRY_COUNT)  return 0xff;

	return 0;
}

uint8 RF_WriteCard(uchar8* pUID, uint8 sector, uchar8 u8KeyType, uchar8* pKey, uchar8* pData)
{
	uint32 i;

	if(sector > CARD_MAX_SECTION)  return 0xff;
	
	for (i=0; i<RF_TRY_COUNT; i++)
	{
		if (MIF_Request(RF_CMD_REQUEST_ALL) != 0)		  continue;
		if (MIF_AntiColl() != 0)						  continue;
		if (memcmp(g_Card_UID, pUID, 4) !=0)			  continue;
		if (MIF_Select() != 0)							  continue;
		if (MIF_Load_Key(pKey) !=0)						  continue;
		if (MIF_Authentication(pUID, sector, u8KeyType))  continue;
		if (MIF_Write(pData, sector*4) != 0)			  continue;
		FM_Delay_ms(3);
		if (MIF_Write(&pData[16], sector*4 +1) != 0)	  continue;
		FM_Delay_ms(3);
		if (MIF_Write(&pData[32], sector*4 +2) != 0)	  continue;
		MIF_Halt();
		break;
	}
	if (i == RF_TRY_COUNT)  return 0xff;

	return 0;
}

uint8 RF_BlockValueIncrement(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey, uchar8* pData)
{
	uint8 i = 0;
	uint8 u8Ret = 0;

	if (u8Block > CARD_MAX_BLOCK)  return 0xff;
	
	for (i=0; i<RF_TRY_COUNT; i++)
	{
		u8Ret = RF_ActiveCard(pUID, (u8Block /4), u8KeyType, pKey);
		if(0 == u8Ret)
		{
			break;
		}
	}
	if (i == RF_TRY_COUNT)  
	{
		return u8Ret;
	}
	else
	{
		u8Ret = MIF_Increment(pData, u8Block);
		MIF_Transfer(u8Block);
	}
	return u8Ret;
}

uint8 RF_BlockValueDecrement(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey, uchar8* pData)
{
	uint8 i = 0;
	uint8 u8Ret = 0;

	if (u8Block > CARD_MAX_BLOCK)  return 0xff;
	
	for (i=0; i<RF_TRY_COUNT; i++)
	{
		u8Ret = RF_ActiveCard(pUID, (u8Block /4), u8KeyType, pKey);
		if(0 == u8Ret)
		{
			break;
		}
	}
	if (i == RF_TRY_COUNT)  
	{
		return u8Ret;
	}
	else
	{
		u8Ret = MIF_Decrement(pData, u8Block);
		MIF_Transfer(u8Block);
	}
	return u8Ret;
}

uint8 RF_BlockRestore(uchar8* pUID, uint8 u8Block, uchar8 u8KeyType, uchar8* pKey)
{
	uint8 i = 0;
	uint8 u8Ret = 0;

	if (u8Block > CARD_MAX_BLOCK)  return 0xff;
	
	for (i=0; i<RF_TRY_COUNT; i++)
	{
		u8Ret = RF_ActiveCard(pUID, (u8Block /4), u8KeyType, pKey);
		if(0 == u8Ret)
		{
			break;
		}
	}
	if (i == RF_TRY_COUNT)  
	{
		return u8Ret;
	}
	else
	{
		u8Ret = MIF_Restore(u8Block);
	}
	return u8Ret;
}

/*========================File end===================================================================================================*/


