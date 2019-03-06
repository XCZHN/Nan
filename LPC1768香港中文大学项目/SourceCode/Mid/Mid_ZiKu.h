/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Mid_ZiKu.h                                                                       
==                                                                                         
**  Function:	字库
==                                                                                         
**  Data:       2016/05/30                                                                          
================================================================================================*/

#ifndef	_MID_ZIKU_H_
#define	_MID_ZIKU_H_

#include "Config.h"


/***********************************************************************************************
* Function Name  : ZiKu_GetChineseChar
* Description	 : 从字库芯片中读取一个汉字的点阵数据
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          : 从字库芯片中读取一个汉字的点阵数据
***********************************************************************************************/
extern void ZiKu_GetChineseChar(uchar8 data[],ulong addr,uint len);

#endif
/*=============================File end========================================================*/
