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
**  Function:	���ú����� 
==
**  Data:       2013-12-20                                                                    
================================================================================================*/

#ifndef	_LIB_PUBLIC_H_
#define	_LIB_PUBLIC_H_

#include "Config.h"

/***********************************************************************************************
* Function Name  : UInt_To_Ascii
* Description	 : ��һ��uint32����ת����ascii��,����: 123 --> "123"
* Input 		 : uiData -- uint32����   ascii�洢BUF
* Output		 : None
* Return		 : ���ɵ�ASCII�ַ�������ʵ�ʳ��� 
***********************************************************************************************/
extern uchar8 UInt_To_Ascii(uint32 uiData, uchar8 ascii[]);

/***********************************************************************************************
* Function Name  : AsciiStr_To_UInt
* Description	 : ��һ��ascii���ʽ������ת����UInt��ʽ������
* Input 		 : pStr -- ascii��ʽ�ַ���ָ��, len -- �ַ�������
* Output		 : None
* Return		 : ���ɵ�UINT��ֵ
* Other          : ����: "123" --> 123    // �������ͷ�ļ� #include <math.h>
***********************************************************************************************/
extern uint32 AsciiStr_To_UInt(uchar8* pStr, uchar8 len);

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
extern void Bcd_To_Hex(uchar8* pHexBuf, const uchar8* pBcdBuf, uint32 BcdLen);

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
extern void Hex_To_Bcd(uchar8* pBcdBuf, const uchar8* pHexBuf, uint32 HexLen);

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
extern uint32 Hex_To_Ascii(uchar8* pAsciiBuf, const uchar8* pHexBuf, uint32 HexLen);

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
extern uint32 Ascii_To_Hex(uchar8* pHexBuf, const uchar8* pAsciiBuf, uint32 AsciiLen);

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
extern uint32 Bcd_To_Ascii(uchar8* pAsciiBuf, const uchar8* pBcdBuf, uint32 BcdLen);

/***********************************************************************************************
* Function Name	  : Find_CharAddr
* Description     : ��str_buf�ַ����в���ch����λ��
* Input           : str -- �ַ���ָ��, ch --��ҪѰ�ҵ��ַ�, datalen -- �ַ�������
* Output     	  : 
* Return    	  : Ŀ���ַ������ַ����е�λ��: -1�޴��ַ���������Ϊ�ַ����ڴ����±�
***********************************************************************************************/
extern int Find_CharAddr(uchar8* str, uchar8 ch, uint32 datalen);

/***********************************************************************************************
* Function Name	  : int	Str_Sample(unsigned char *des, unsigned char *src)
* Description     : ��Ŀ���ַ���������Ѱ��ָ���ַ�������
* Input           : Ŀ������ָ��, ָ������ָ��
* Output     	  : 
* Return    	  : Ѱ�ҵ�ƥ�����鷵��1,���򷵻�0  
***********************************************************************************************/
extern int	Str_Sample(unsigned char *des, unsigned char *src);

/***********************************************************************************************
* Function Name  : IPinfo_Valid
* Description	 : �ж�IP��ַ����Ч��
* Input 		 : ip -- ipָ��
* Output		 : None
* Return		 : 
***********************************************************************************************/
extern int IPinfo_Valid(const char *ip);

/***********************************************************************************************
* Function Name  : IPinfo_Ascii2Hex
* Description	 : ת��IP��Ϣ---��ASCII��u8AsciiBuf����ȡת����HEX��u8HexBuf��
* Input 		 : ���Ascii������ָ��,  ip����ָ��
* Output		 : None
* Return		 : ���ɵ�IP��ַ����(4Byte)
***********************************************************************************************/
extern uchar8  IPinfo_Ascii2Hex(uchar8 *u8AsciiBuf, uchar8 *u8HexBuf);

/***********************************************************************************************
* Function Name  : IPinfo_Hex2Ascii
* Description	 : ת��IP��Ϣ---��HEX��u8HexBuf����ȡת����ASCII��u8AsciiBuf��
* Input 		 : u8AsciiBuf -- �����ָ��, u8HexBuf -- IP��ַָ��
* Output		 : None
* Return		 : ���ɵ�IP��ַ����
***********************************************************************************************/
extern uint32 IPinfo_Hex2Ascii(uchar8 *u8AsciiBuf, uchar8 *u8HexBuf);

/***********************************************************************************************
* Function Name  : U32ArraySort
* Description	 : ��u32�������͵������������
* Input 		 : pU32Array -- ����ָ��, u32Len -- ���鳤��
* Output		 : None
* Return		 : ð������ 
***********************************************************************************************/
extern void U32ArraySort(uint32* pU32Array, uint32 u32Len);



#endif
/*============================================File end=============================================================*/


