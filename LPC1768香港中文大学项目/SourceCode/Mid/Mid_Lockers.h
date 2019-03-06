/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Lockers.h                                                                       
==                                                                                         
**  Function:	�����䴦����� 
==                                                                                         
**  Data:       2015/09/18                                                                       
================================================================================================*/

#ifndef	_LOCKERS_H_
#define	_LOCKERS_H_


#include "Includes.h"


/***********************************************************************************************
* Function Name  : CheckEmptyBoxNum
* Description	 : ��������
* Input 		 : None
* Output		 : None
* Return		 : ������
* Note  		 :  
***********************************************************************************************/
extern uchar8 CheckEmptyBoxNum(void);

/***********************************************************************************************
* Function Name  : ScanAllBoxStatus
* Description	 : ɨ����������״̬
* Input 		 : void
* Output		 : None
* Return		 : None
* Note  		 : ������չ��������״̬
***********************************************************************************************/
extern void ScanAllBoxStatus(void);

/***********************************************************************************************
* Function Name  : SelectOneBoxToOpen
* Description	 : ѡ����ĳ������
* Input 		 : boxId -- �����������ź�, bCheckResult:0����⿪�Ž��,��0���⿪�Ž��
* Output		 : None
* Return		 : ִ�н�� 0--����ɹ�, ����--����ʧ��
* Notes          : �Զ��ж����������Ǹ�����
***********************************************************************************************/
extern uchar8 SelectOneBoxToOpen(uint8 boxId, uchar8 bCheckResult);

/***********************************************************************************************
* Function Name  : HandleTimeoutAlert
* Description	 : �Ĵ���ų�ʱ����
* Input 		 : 
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void HandleTimeoutAlert(void);

/***********************************************************************************************
* Function Name  : UpdateEmptyBoxList
* Description	 : ���¿����б�
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : ����������װ�ص������б��� 
***********************************************************************************************/
extern void UpdateEmptyBoxList(void);

/***********************************************************************************************
* Function Name  : AddOneEmptyBox
* Description	 : ���������ĩβ����һ���������
* Input 		 : box--���
* Output		 : None
* Return		 : None
* Note  		 : None 
***********************************************************************************************/
extern void AddOneEmptyBox(uchar8 box);

/***********************************************************************************************
* Function Name  : AddOneEmptyBox
* Description	 : �����������ɾ��һ���������
* Input 		 : box--���
* Output		 : None
* Return		 : None
* Note  		 : None 
***********************************************************************************************/
extern void DeleteOneEmptyBox(uchar8 box);


/***********************************************************************************************
* Function Name  : CheckBoxStatusChange
* Description	 : �������\����״̬�ı�
* Input 		 : None
* Output		 : None
* Return		 : None
* Note  		 : ����鵽�иı�ʱ�����ϴ���������
***********************************************************************************************/
extern void CheckBoxStatusChange(void);

/***********************************************************************************************
* Function Name  : CheckValidityDate
* Description	 : ���ĳ�����ڵ���Ч��
* Input 		 : pDate -- ����
* Output		 : None
* Return		 : 0x00 -- δ������Ч��, ����Ϊ��Ч����(��),С��0��ʾ�ѹ�������
***********************************************************************************************/
extern int	CheckValidityDate(uchar8* pDate);


/***********************************************************************************************
* Function Name  : VoicePlay_DoorNotClose
* Description	 : ����"xx������δ��,�����!"
* Input 		 : u8DoorNum -- �ź�
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void VoicePlay_DoorNotClose(uchar8 u8DoorNum);

/***********************************************************************************************
* Function Name  : VoicePlay_xBoxOpen
* Description	 : ����"xx�������ѿ�!"
* Input 		 : u8Box -- ���ź�
* Output		 : None
* Return		 : None
***********************************************************************************************/
extern void VoicePlay_xBoxOpen(uchar8 u8Box);


#endif
/*========================File end======================================================================*/

