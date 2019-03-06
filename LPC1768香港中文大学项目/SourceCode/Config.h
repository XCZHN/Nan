/*===============================================================================================
==                                                                                              
**  Copyright(c) 2015  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Config.h                                                                       
==                                                                                         
**  Function:	System configuration Header files                                                                     
==
**  Data:       2013-05-15
================================================================================================*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string.h>
#include <stdio.h>

#ifndef OK
#define OK    0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned char  uchar8;	
typedef unsigned char  u8;
typedef unsigned char  uint8;
typedef unsigned char  BOOL;
typedef signed   char  int8;

typedef unsigned short uint;
typedef unsigned short uint16;
typedef unsigned short u16;
typedef signed   short int16;

typedef unsigned int   ulong;
typedef unsigned int   uint32;
typedef unsigned int   u32;
typedef signed   int   int32;
typedef float          fp32;
typedef double         fp64;


#define Disable_IRQ(void) __disable_irq(void)	  
#define Enable_IRQ(void) __enable_irq(void)	  

#define HIGH(x) (x &0xff00)>>8
#define LOW(x) 	(x &0xff)

typedef union 
{
	uint32	numb32;
	uchar8	numb8[4];

}TURN_DATA;


#endif


