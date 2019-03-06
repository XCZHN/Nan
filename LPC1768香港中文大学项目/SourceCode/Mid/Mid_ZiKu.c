/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Mid_ZiKu.c                                                                       
==                                                                                         
**  Function:	�ֿ�
==                                                                                         
**  Data:       2016/05/30                                                                          
================================================================================================*/


#include "Mid_ZiKu.h"
#include "Drv_SPI.h"


/***********************************************************************************************
* Function Name  : ZiKu_GetChineseChar
* Description	 : ���ֿ�оƬ�ж�ȡһ�����ֵĵ�������
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : ���ֿ�оƬ�ж�ȡһ�����ֵĵ�������
***********************************************************************************************/
void ZiKu_GetChineseChar(uchar8 data[],ulong addr,uint len)
{
	uchar8 addr_h,addr_m,addr_l;
	uint i;

	addr_h=(addr>>16)&0xff;
	addr_m=(addr>>8)&0xff;
	addr_l=addr&0xff;

	ZiKuSPI_Enable();
	SPI_Send1Byte(0x0b);
	SPI_Send1Byte(addr_h);
	SPI_Send1Byte(addr_m);
	SPI_Send1Byte(addr_l);
	SPI_Send1Byte(0x55);
	
	for(i=0;i<len;i++)
	{
		data[i]=SPI_Read1Byte();
	}
	ZiKuNetSPI_Disable();
}






/*========================File end=====================================================================*/

