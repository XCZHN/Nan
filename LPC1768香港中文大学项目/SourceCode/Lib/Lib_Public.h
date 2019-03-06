/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                     
==                                                                                          
**  Version:	V1.0.0                                                                          
==
**  Filename:	Lib_Public.h                                                                       
==                                                                                         
**  Function:	常用函数库 
==
**  Data:       2013-12-20                                                                    
================================================================================================*/

#ifndef	_LIB_PUBLIC_H_
#define	_LIB_PUBLIC_H_

#include "Config.h"

/***********************************************************************************************
* Function Name  : UInt_To_Ascii
* Description	 : 将一个uint32数据转换成ascii码,例如: 123 --> "123"
* Input 		 : uiData -- uint32数据   ascii存储BUF
* Output		 : None
* Return		 : 生成的ASCII字符串数据实际长度 
***********************************************************************************************/
extern uchar8 UInt_To_Ascii(uint32 uiData, uchar8 ascii[]);

/***********************************************************************************************
* Function Name  : AsciiStr_To_UInt
* Description	 : 将一个ascii码格式的数字转换成UInt格式的数字
* Input 		 : pStr -- ascii格式字符串指针, len -- 字符串长度
* Output		 : None
* Return		 : 生成的UINT数值
* Other          : 例如: "123" --> 123    // 无须包含头文件 #include <math.h>
***********************************************************************************************/
extern uint32 AsciiStr_To_UInt(uchar8* pStr, uchar8 len);

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
extern void Bcd_To_Hex(uchar8* pHexBuf, const uchar8* pBcdBuf, uint32 BcdLen);

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
extern void Hex_To_Bcd(uchar8* pBcdBuf, const uchar8* pHexBuf, uint32 HexLen);

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
extern uint32 Hex_To_Ascii(uchar8* pAsciiBuf, const uchar8* pHexBuf, uint32 HexLen);

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
extern uint32 Ascii_To_Hex(uchar8* pHexBuf, const uchar8* pAsciiBuf, uint32 AsciiLen);

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
extern uint32 Bcd_To_Ascii(uchar8* pAsciiBuf, const uchar8* pBcdBuf, uint32 BcdLen);

/***********************************************************************************************
* Function Name	  : Find_CharAddr
* Description     : 在str_buf字符串中查找ch所在位置
* Input           : str -- 字符串指针, ch --所要寻找的字符, datalen -- 字符串长度
* Output     	  : 
* Return    	  : 目标字符所在字符串中的位置: -1无此字符，其他则为字符所在串的下标
***********************************************************************************************/
extern int Find_CharAddr(uchar8* str, uchar8 ch, uint32 datalen);

/***********************************************************************************************
* Function Name	  : int	Str_Sample(unsigned char *des, unsigned char *src)
* Description     : 在目标字符串数组中寻找指定字符串数组
* Input           : 目标数组指针, 指定数组指针
* Output     	  : 
* Return    	  : 寻找到匹配数组返回1,否则返回0  
***********************************************************************************************/
extern int	Str_Sample(unsigned char *des, unsigned char *src);

/***********************************************************************************************
* Function Name  : IPinfo_Valid
* Description	 : 判断IP地址的有效性
* Input 		 : ip -- ip指针
* Output		 : None
* Return		 : 
***********************************************************************************************/
extern int IPinfo_Valid(const char *ip);

/***********************************************************************************************
* Function Name  : IPinfo_Ascii2Hex
* Description	 : 转换IP信息---从ASCII的u8AsciiBuf中提取转换至HEX的u8HexBuf中
* Input 		 : 存放Ascii的数组指针,  ip数组指针
* Output		 : None
* Return		 : 生成的IP地址长度(4Byte)
***********************************************************************************************/
extern uchar8  IPinfo_Ascii2Hex(uchar8 *u8AsciiBuf, uchar8 *u8HexBuf);

/***********************************************************************************************
* Function Name  : IPinfo_Hex2Ascii
* Description	 : 转换IP信息---从HEX的u8HexBuf中提取转换至ASCII的u8AsciiBuf中
* Input 		 : u8AsciiBuf -- 存放区指针, u8HexBuf -- IP地址指针
* Output		 : None
* Return		 : 生成的IP地址长度
***********************************************************************************************/
extern uint32 IPinfo_Hex2Ascii(uchar8 *u8AsciiBuf, uchar8 *u8HexBuf);

/***********************************************************************************************
* Function Name  : U32ArraySort
* Description	 : 对u32数据类型的数组进行排序
* Input 		 : pU32Array -- 数组指针, u32Len -- 数组长度
* Output		 : None
* Return		 : 冒泡排序法 
***********************************************************************************************/
extern void U32ArraySort(uint32* pU32Array, uint32 u32Len);



#endif
/*============================================File end=============================================================*/


