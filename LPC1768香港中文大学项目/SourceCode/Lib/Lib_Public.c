/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                     
==                                                                                          
**  Version:	V1.0.0                                                                          
==
**  Filename:	Lib_Public.c                                                                       
==                                                                                         
**  Function:	常用函数库 
==
**  Data:       2013-12-20                                                                    
================================================================================================*/

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "Lib_Public.h"

/***********************************************************************************************
* Function Name  : UInt_To_Ascii
* Description	 : 将一个uint32数据转换成ascii码,例如: 123 --> "123"
* Input 		 : uiData -- uint32数据   ascii存储BUF
* Output		 : None
* Return		 : 生成的ASCII字符串数据实际长度 
***********************************************************************************************/
uchar8 UInt_To_Ascii(uint32 uiData, uchar8 ascii[])
{
	uchar8 j,i=0, buff[10];
	uint32 ui_temp;

	ui_temp = uiData;
	do
	{
		buff[i++] = (ui_temp%10 + '0');
		ui_temp = (ui_temp/10);
	}while(ui_temp);

	j = i;
	for(i=0; i<j; i++)  
	{
		ascii[i] = buff[j-i-1] ;	//反序				
	}
	return i;
}

/***********************************************************************************************
* Function Name  : AsciiStr_To_UInt
* Description	 : 将一个ascii码格式的数字转换成UInt格式的数字
* Input 		 : pStr -- ascii格式字符串指针, len -- 字符串长度
* Output		 : None
* Return		 : 生成的UINT数值
* Other          : 例如: "123" --> 123    // 无须包含头文件 #include <math.h>
***********************************************************************************************/
uint32 AsciiStr_To_UInt(uchar8* pStr, uchar8 len)
{
	uint32 uiNum = 0;
	uint32 uiTempNum = 0;
	uchar8 ucBuf[20] = {0};
	uchar8 i = 0;
	uchar8 Multipl = 0;

	if(!len)
	{
		return 0;	
	}
	else if(len > 10)
	{
		return 0xFFFFFFFF;
	}
	
	memcpy(ucBuf, pStr, len);

	for(i=0; i<len; i++)
	{
		if((ucBuf[i] >= '0')&&(ucBuf[i] <= '9'))
		{
			uiTempNum = ucBuf[i] - '0';
			
#if 0		//Linux库函数下找不到 <math.h> 文件
			uiNum += (uiTempNum * pow10(len -(i +1)));
#else
			Multipl = len -(i +1);
			while(Multipl--)
			{
				uiTempNum = uiTempNum *10;
			}
			uiNum += uiTempNum;
#endif
		}
		else
		{
			return 0xFFFFFFFF;	
		}
	}
	return uiNum;
}

/*************************************************************************************************
* 函数(Name)	  : void Bcd_To_Hex(uchar8* pHexBuf, const uchar8* pBcdBuf, uint32 BcdLen)
* 功能(Function)  : BCD码转换成HEX码
* 参数(Parameter) : pHexBuf -- Hex码存储区指针, pBcdBuf -- Bcd码区指针, BcdLen -- Bcd码长度
* 变量(Variable)  : NULL
* 错误(Error)	  : 无
* 返回(Return)	  : NULL
* 修订(Amendment) : Asen 2013/12/20
* 其他(Other)	  : 一个字节BCD码转换成一个字节的HEX码(例如: 12 --> 0x12)
***************************************************************************************************/
void Bcd_To_Hex(uchar8* pHexBuf, const uchar8* pBcdBuf, uint32 BcdLen)
{
	uchar8 temp0 = 0;
	uint32 i = 0;

	for(i=0; i<BcdLen; i++)
	{
		temp0 = pBcdBuf[i];
		pHexBuf[i] = ((temp0 >> 4)&0x0F)*10 + (temp0 &0x0F);
	}
}

/***************************************************************************************************
* 函数(Name)	  : void Hex_To_Bcd(uchar8* pBcdBuf, const uchar8* pHexBuf, uint32 HexLen)
* 功能(Function)  : HEX码转换成BCD码
* 参数(Parameter) : HexBuf -- Hex码存储区指针, pBcdBuf -- Bcd码区指针, HexLen -- Hex码长度
* 变量(Variable)  : NULL
* 错误(Error)	  : 无
* 返回(Return)	  : NULL
* 修订(Amendment) : Asen 2013/03/21
* 其他(Other)	  : 一个字节的HEX码转换成一个字节的BCD码(例如: 0x12 --> 12)
****************************************************************************************************/
void Hex_To_Bcd(uchar8* pBcdBuf, const uchar8* pHexBuf, uint32 HexLen)
{
	uchar8 temp0 = 0;
	uint32 i = 0;

	for(i=0; i<HexLen; i++)
	{
		temp0 = pHexBuf[i];
		pBcdBuf[i] = ((((temp0 /10) << 4)&0xF0) | (temp0 %10));
	}
}

/******************************************************************************************************
* 函数(Name)	  : uint32 Hex_To_Ascii(uchar8* pBcdBuf, const uchar8* pHexBuf, uint32 HexLen)
* 功能(Function)  : HEX码转换成ASCII码
* 参数(Parameter) : pHexBuf -- Hex码存储区指针, pAsciiBuf -- Ascii码区指针, HexLen -- Hex码长度
* 变量(Variable)  : NULL
* 错误(Error)	  : 无
* 返回(Return)	  : 生成的ASCII码的长度 (= HexLen * 2) 
* 修订(Amendment) : Asen 2013/03/21
* 其他(Other)	  : 一字节HEX码转换成2字节ASCII码(例如: 0xA2 --> "A2")
*******************************************************************************************************/
uint32 Hex_To_Ascii(uchar8* pAsciiBuf, const uchar8* pHexBuf, uint32 HexLen)
{
	uchar8  u8TempCH, u8TurnCH;
	uint32	u32AsciiLen = 0;
    int i = 0;
	
	for(i = 0; i < HexLen; i++)
	{
		u8TempCH = pHexBuf[i];
		u8TurnCH = (u8TempCH >> 4) & 0x0f;
		if (u8TurnCH > 9)
		{
			u8TurnCH = u8TurnCH -10 + 'A';
		}
		else
		{
			u8TurnCH = u8TurnCH + '0';
		}
		pAsciiBuf[u32AsciiLen++] = u8TurnCH;

		u8TurnCH = u8TempCH & 0x0f;
		if (u8TurnCH > 9)
		{
			u8TurnCH = u8TurnCH -10 + 'A';
		}
		else
		{
			u8TurnCH = u8TurnCH + '0';
		}
		pAsciiBuf[u32AsciiLen++] = u8TurnCH;
	}
	return  u32AsciiLen;
}

/*********************************************************************************************************
* 函数(Name)	  : uint32 Ascii_To_Hex(uchar8* pHexBuf, const uchar8* pAsciiBuf, uint32 AsciiLen)
* 功能(Function)  : HEX码转换成ASCII码
* 参数(Parameter) : pAsciiBuf -- Ascii码区指针, pHexBuf -- Hex码存储区指针, AsciiLen -- Ascii码长度
* 变量(Variable)  : NULL
* 错误(Error)	  : 无
* 返回(Return)	  : 生成的HEX码的长度 (= HexLen / 2) 
* 修订(Amendment) : Asen  2013/03/21
* 其他(Other)	  : 2字节ASCII码转换生成1字节的HEX码(例如:"12" --> 0x12)
**********************************************************************************************************/
uint32 Ascii_To_Hex(uchar8* pHexBuf, const uchar8* pAsciiBuf, uint32 AsciiLen)
{
	unsigned char temp, mid;
	unsigned int i = 0;
	unsigned int j = 0;

	for(i=0; i<AsciiLen; i++)
	{
		mid = pAsciiBuf[i];
		if( ('0'<= mid) && (mid <= '9') )
		{
			mid = mid - '0';
		}
		else if( ('a'<= mid) && (mid <= 'f') )
		{
			mid = mid - 'a' + 10;
		}
		else if( ('A'<= mid) && (mid <= 'F') )
		{
			mid = mid - 'A' + 10;
		}
		temp = mid << 4;

		i++;
		mid = pAsciiBuf[i];
		if( ('0'<= mid) && (mid <= '9') )
		{
			mid = mid - '0';
		}
		else if( ('a'<= mid) && (mid <= 'f') )
		{
			mid = mid - 'a' + 10;
		}
		else if( ('A'<= mid) && (mid <= 'F') )
		{
			mid = mid - 'A' + 10;
		}
		mid = mid & 0x0f;

		pHexBuf[j++] = temp + mid;
	}
	return	j;
}

/***********************************************************************************************************
* 函数(Name)	  : uint32 Bcd_To_Ascii(uchar8* pAsciiBuf, const uchar8* pBcdBuf, uint32 BcdLen)
* 功能(Function)  : Hex码转换成Ascii码
* 参数(Parameter) : BcdLen -- 待转换的BCD码的长度
* 变量(Variable)  : NULL
* 错误(Error)	  : 无
* 返回(Return)	  : 生成的ASCII码的长度 (= HexLen * 2) 
* 修订(Amendment) : Asen 2013/03/21
* 其他(Other)	  : 一字节的BCD码转换生成2字节的ASCII码(例如: 0x12 --> "12")
************************************************************************************************************/
uint32 Bcd_To_Ascii(uchar8* pAsciiBuf, const uchar8* pBcdBuf, uint32 BcdLen)
{
	uchar8 temp0 = 0;
	uint32 i = 0, j = 0;

	for(i=0; i<BcdLen; i++)
	{
		temp0 = pBcdBuf[i];
		pAsciiBuf[j++] = (((temp0 >>4) & 0x0f) + 0x30);
		pAsciiBuf[j++] = ((temp0 & 0x0f) + 0x30);		
	}
	return	j;
}

/***********************************************************************************************
* Function Name	  : Find_CharAddr
* Description     : 在str_buf字符串中查找ch所在位置
* Input           : str -- 字符串指针, ch --所要寻找的字符, datalen -- 字符串长度
* Output     	  : 
* Return    	  : 目标字符所在字符串中的位置: -1无此字符，其他则为字符所在串的下标
***********************************************************************************************/
int Find_CharAddr(uchar8* str, uchar8 ch, uint32 datalen)
{
	int	i = 0;
	while(i < datalen)
	{
		if( str[i] == ch)
		{
			return i;
		}
		i++;
	}
	return -1;
}

/***********************************************************************************************
* Function Name	  : int	Str_Sample(unsigned char *des, unsigned char *src)
* Description     : 在目标字符串数组中寻找指定字符串数组
* Input           : 目标数组指针, 指定数组指针
* Output     	  : 
* Return    	  : 寻找到匹配数组返回1,否则返回0  
***********************************************************************************************/
int	Str_Sample(unsigned char *des, unsigned char *src)
{
	uchar8 * str_des = des;
	uchar8 * str_src = src;
	uchar8 * temp_des;
	uchar8 * temp_src;
	int	i,j;
	int	len_des = strlen((const char*)str_des);
	int	len_src = strlen((const char*)str_src);
	int	temp = len_des - len_src;
	if( temp < 0)
	{
		return 0;
	}

	for ( i= 0; i <= temp; i++)
	{
		if( *str_des != *str_src)
		{
			str_des++;
		}
		else
		{
			temp_des = str_des;
			temp_src = str_src;
			for ( j = 0; j < len_src; j++)
			{
				if ( *temp_des != *temp_src)
				{
					break;
				}
				temp_des++;
				temp_src++;
			}
			if( j == len_src)	//找到了指定数组
			{		
				return 1;
			}
			str_des++;
		}
	}
	return 0;
}

/***********************************************************************************************
* Function Name  : IPinfo_Valid
* Description	 : 判断IP地址的有效性
* Input 		 : ip -- ip指针
* Output		 : None
* Return		 : 
***********************************************************************************************/
int IPinfo_Valid(const char *ip) 
{ 
    return 1; 
}


/***********************************************************************************************
* Function Name  : IPinfo_Ascii2Hex
* Description	 : 转换IP信息---从ASCII的u8AsciiBuf中提取转换至HEX的u8HexBuf中
* Input 		 : 存放Ascii的数组指针,  ip数组指针
* Output		 : None
* Return		 : 生成的IP地址长度(4Byte)
***********************************************************************************************/
uchar8  IPinfo_Ascii2Hex(uchar8 *u8AsciiBuf, uchar8 *u8HexBuf)
{
	uchar8 i,j;
	uchar8 x = 1;
	uchar8 Ip_Num[5] = {0};
	
	for(i=0; i<20; i++)							
	{
		if((u8AsciiBuf[i] == '.')||(u8AsciiBuf[i] == 0x00))
		{
			Ip_Num[x++] = i;
			if(x >= 5)
			{
				i = 30 ;
			}
		}
	}
	
	for(j=0; j<4; j++)
	{	
		if(j==0)
		{
			u8HexBuf[0] = 0;
			for(i=0; i<Ip_Num[j+1]; i++)
			{
				u8HexBuf[j] = u8HexBuf[j] * 10 + (u8AsciiBuf[i] - 0x30) ;
			}						
		}
		else
		{
			u8HexBuf[j] = 0 ;
			for(i=Ip_Num[j]+1; i<Ip_Num[j+1]; i++)
			{
				u8HexBuf[j] = u8HexBuf[j] * 10 + (u8AsciiBuf[i] - 0x30) ;
			}	
		}
	}

	return 4;
}

/***********************************************************************************************
* Function Name  : IPinfo_Hex2Ascii
* Description	 : 转换IP信息---从HEX的u8HexBuf中提取转换至ASCII的u8AsciiBuf中
* Input 		 : u8AsciiBuf -- 存放区指针, u8HexBuf -- IP地址指针
* Output		 : None
* Return		 : 生成的IP地址长度
***********************************************************************************************/
uint32 IPinfo_Hex2Ascii(uchar8 *u8AsciiBuf, uchar8 *u8HexBuf)
{
	uchar8	temp,mid;
	uint32  i = 0;
	uint32  j = 0;
	for(i= 0; i <4; i++)  
	{
		temp = u8HexBuf[i];
		if( temp > 99)
		{
			mid = temp / 100;
			temp = temp %100;
			u8AsciiBuf[j++] = mid+'0';
			u8AsciiBuf[j++] = temp /10+'0';
			u8AsciiBuf[j++] = temp %10+'0';
		}
		else if(temp > 9)
		{
			u8AsciiBuf[j++] = temp /10+'0';
			u8AsciiBuf[j++] = temp %10+'0';
		}
		else
		{
			u8AsciiBuf[j++] = temp+'0';
		}
		if( i != 3)
		{
			u8AsciiBuf[j++] = '.';
		}
	}
	return j;
}

/***********************************************************************************************
* Function Name  : U32ArraySort
* Description	 : 对u32数据类型的数组进行排序
* Input 		 : pU32Array -- 数组指针, u32Len -- 数组长度
* Output		 : None
* Return		 : 冒泡排序法 
***********************************************************************************************/
void U32ArraySort(uint32* pU32Array, uint32 u32Len)
{
	uint32 i = 0;
	uint32 j = 0;
	uint32 u32Temp = 0;
	
	for(i = 0; i < u32Len; i++)
	{  
        //比较两个相邻的元素  
        for(j = 0; j < (u32Len-i-1); j++)
		{  
            if(pU32Array[j] > pU32Array[j+1])
			{  
                u32Temp = pU32Array[j];  
                pU32Array[j] = pU32Array[j+1];  
                pU32Array[j+1] = u32Temp;  
            }  
        }  
    }  
}




/*============================================File end=============================================================*/



