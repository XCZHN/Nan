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
**  Function:	���ú����� 
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
* Description	 : ��һ��uint32����ת����ascii��,����: 123 --> "123"
* Input 		 : uiData -- uint32����   ascii�洢BUF
* Output		 : None
* Return		 : ���ɵ�ASCII�ַ�������ʵ�ʳ��� 
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
		ascii[i] = buff[j-i-1] ;	//����				
	}
	return i;
}

/***********************************************************************************************
* Function Name  : AsciiStr_To_UInt
* Description	 : ��һ��ascii���ʽ������ת����UInt��ʽ������
* Input 		 : pStr -- ascii��ʽ�ַ���ָ��, len -- �ַ�������
* Output		 : None
* Return		 : ���ɵ�UINT��ֵ
* Other          : ����: "123" --> 123    // �������ͷ�ļ� #include <math.h>
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
			
#if 0		//Linux�⺯�����Ҳ��� <math.h> �ļ�
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
* ����(Name)	  : void Bcd_To_Hex(uchar8* pHexBuf, const uchar8* pBcdBuf, uint32 BcdLen)
* ����(Function)  : BCD��ת����HEX��
* ����(Parameter) : pHexBuf -- Hex��洢��ָ��, pBcdBuf -- Bcd����ָ��, BcdLen -- Bcd�볤��
* ����(Variable)  : NULL
* ����(Error)	  : ��
* ����(Return)	  : NULL
* �޶�(Amendment) : Asen 2013/12/20
* ����(Other)	  : һ���ֽ�BCD��ת����һ���ֽڵ�HEX��(����: 12 --> 0x12)
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
* ����(Name)	  : void Hex_To_Bcd(uchar8* pBcdBuf, const uchar8* pHexBuf, uint32 HexLen)
* ����(Function)  : HEX��ת����BCD��
* ����(Parameter) : HexBuf -- Hex��洢��ָ��, pBcdBuf -- Bcd����ָ��, HexLen -- Hex�볤��
* ����(Variable)  : NULL
* ����(Error)	  : ��
* ����(Return)	  : NULL
* �޶�(Amendment) : Asen 2013/03/21
* ����(Other)	  : һ���ֽڵ�HEX��ת����һ���ֽڵ�BCD��(����: 0x12 --> 12)
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
* ����(Name)	  : uint32 Hex_To_Ascii(uchar8* pBcdBuf, const uchar8* pHexBuf, uint32 HexLen)
* ����(Function)  : HEX��ת����ASCII��
* ����(Parameter) : pHexBuf -- Hex��洢��ָ��, pAsciiBuf -- Ascii����ָ��, HexLen -- Hex�볤��
* ����(Variable)  : NULL
* ����(Error)	  : ��
* ����(Return)	  : ���ɵ�ASCII��ĳ��� (= HexLen * 2) 
* �޶�(Amendment) : Asen 2013/03/21
* ����(Other)	  : һ�ֽ�HEX��ת����2�ֽ�ASCII��(����: 0xA2 --> "A2")
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
* ����(Name)	  : uint32 Ascii_To_Hex(uchar8* pHexBuf, const uchar8* pAsciiBuf, uint32 AsciiLen)
* ����(Function)  : HEX��ת����ASCII��
* ����(Parameter) : pAsciiBuf -- Ascii����ָ��, pHexBuf -- Hex��洢��ָ��, AsciiLen -- Ascii�볤��
* ����(Variable)  : NULL
* ����(Error)	  : ��
* ����(Return)	  : ���ɵ�HEX��ĳ��� (= HexLen / 2) 
* �޶�(Amendment) : Asen  2013/03/21
* ����(Other)	  : 2�ֽ�ASCII��ת������1�ֽڵ�HEX��(����:"12" --> 0x12)
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
* ����(Name)	  : uint32 Bcd_To_Ascii(uchar8* pAsciiBuf, const uchar8* pBcdBuf, uint32 BcdLen)
* ����(Function)  : Hex��ת����Ascii��
* ����(Parameter) : BcdLen -- ��ת����BCD��ĳ���
* ����(Variable)  : NULL
* ����(Error)	  : ��
* ����(Return)	  : ���ɵ�ASCII��ĳ��� (= HexLen * 2) 
* �޶�(Amendment) : Asen 2013/03/21
* ����(Other)	  : һ�ֽڵ�BCD��ת������2�ֽڵ�ASCII��(����: 0x12 --> "12")
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
* Description     : ��str_buf�ַ����в���ch����λ��
* Input           : str -- �ַ���ָ��, ch --��ҪѰ�ҵ��ַ�, datalen -- �ַ�������
* Output     	  : 
* Return    	  : Ŀ���ַ������ַ����е�λ��: -1�޴��ַ���������Ϊ�ַ����ڴ����±�
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
* Description     : ��Ŀ���ַ���������Ѱ��ָ���ַ�������
* Input           : Ŀ������ָ��, ָ������ָ��
* Output     	  : 
* Return    	  : Ѱ�ҵ�ƥ�����鷵��1,���򷵻�0  
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
			if( j == len_src)	//�ҵ���ָ������
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
* Description	 : �ж�IP��ַ����Ч��
* Input 		 : ip -- ipָ��
* Output		 : None
* Return		 : 
***********************************************************************************************/
int IPinfo_Valid(const char *ip) 
{ 
    return 1; 
}


/***********************************************************************************************
* Function Name  : IPinfo_Ascii2Hex
* Description	 : ת��IP��Ϣ---��ASCII��u8AsciiBuf����ȡת����HEX��u8HexBuf��
* Input 		 : ���Ascii������ָ��,  ip����ָ��
* Output		 : None
* Return		 : ���ɵ�IP��ַ����(4Byte)
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
* Description	 : ת��IP��Ϣ---��HEX��u8HexBuf����ȡת����ASCII��u8AsciiBuf��
* Input 		 : u8AsciiBuf -- �����ָ��, u8HexBuf -- IP��ַָ��
* Output		 : None
* Return		 : ���ɵ�IP��ַ����
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
* Description	 : ��u32�������͵������������
* Input 		 : pU32Array -- ����ָ��, u32Len -- ���鳤��
* Output		 : None
* Return		 : ð������ 
***********************************************************************************************/
void U32ArraySort(uint32* pU32Array, uint32 u32Len)
{
	uint32 i = 0;
	uint32 j = 0;
	uint32 u32Temp = 0;
	
	for(i = 0; i < u32Len; i++)
	{  
        //�Ƚ��������ڵ�Ԫ��  
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



