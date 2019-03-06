/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  Intelligent Admin electronic systems co., LTD. All rights reserved.
==                                                                                           
**	Author:		Asen                                                                           
==                                                                                          
**  Version:	v1.0.0                                                                          
==
**  Filename:	Drv_LCD_ILI9341.h                                                                       
==                                                                                         
**  Function:	LCD显示驱动
==                                                                                         
**  Data:       2016/05/30                                                                          
================================================================================================*/

#ifndef	_DRV_LCD_H_
#define	_DRV_LCD_H_

#include "Config.h"
#include "LPC17xx.h"

/**************************************************************				
*P1口的引脚功能记录
P1.0	~	P1.15 	|	不可用								
P1.16	~	P1.31 	|	总线AD0	~ AD15						IO
****************************************************************/		
#define	AD_PIN_NUM		    16		//P1.16~P1.31定义外部16位数据总线D0-D15的引脚起始号

#define WR  			    0		// P0.0
#define RD  			    1		// P0.1
#define RSTDSP              10		// P2.10
#define DISP  			    11		// P2.11
#define RS  			    12		// P2.12
#define CSDSP               13		// P2.13

#define	SET_WR_PIN			(LPC_GPIO0->FIOSET |= (1<<WR))
#define	CLR_WR_PIN			(LPC_GPIO0->FIOCLR |= (1<<WR))

#define	SET_RD_PIN			(LPC_GPIO0->FIOSET |= (1<<RD))
#define	CLR_RD_PIN			(LPC_GPIO0->FIOCLR |= (1<<RD))

#define	SET_RST_PIN			(LPC_GPIO2->FIOSET |= (1<<RSTDSP))
#define	CLR_RST_PIN			(LPC_GPIO2->FIOCLR |= (1<<RSTDSP))

#define	SET_DISP_PIN		(LPC_GPIO2->FIOSET |= (1<<DISP))
#define	CLR_DISP_PIN		(LPC_GPIO2->FIOCLR |= (1<<DISP))

#define	SET_RS_PIN			(LPC_GPIO2->FIOSET |= (1<<RS))
#define	CLR_RS_PIN			(LPC_GPIO2->FIOCLR |= (1<<RS))

#define	SET_CS_PIN			(LPC_GPIO2->FIOSET |= (1<<CSDSP))
#define	CLR_CS_PIN			(LPC_GPIO2->FIOCLR |= (1<<CSDSP))


#define LCD_WIDTH          239
#define LCD_LENGTH         319

/***********************************************************************************************
* Function Name  : LCD_Init
* Description	 : LCD初始化
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          :  
***********************************************************************************************/
extern void LCD_Init(void);

void LCD_Clear(uint Color);
void LCD_Fill(uint xsta,uint ysta,uint xend,uint yend,uint color);
void DisplayString(uchar8 Font,uint x,uint y,const char *string,uint color);
void DisplayASCII(uchar8 Font,uint x,uint y,const char ASCII,uint color);

/*********************************************************
* 函数名称：void LCD_DrawLine(uint x1, uint y1, uint x2, uint y2)
* 功能描述：画线
* x1,y1:起点坐标
* x2,y2:终点坐标  
**********************************************************/
extern void LCD_DrawLine(uint x1, uint y1, uint x2, uint y2,uint color);

/***********************************************************************************************
* Function Name  : LCD_ShowFillPic
* Description	 : LCD显示一张图片
* Input 		 : PicID--图片编号
***********************************************************************************************/
extern void LCD_ShowFillPic(uchar8 PicID);

/***********************************************************************************************
* Function Name  : LCD_ShowPartPic
* Description	 : LCD显示一张不完整的图片
* Input 		 : PicID--图片编号
***********************************************************************************************/
extern void LCD_ShowPartPic(uint xsta,uint ysta,uint xend,uint yend, uchar8 PicID);

/***********************************************************************************************
* Function Name  : LCD_ShowIcon
* Description	 : LCD显示图标
* Input 		 : IconID--图标编号
***********************************************************************************************/
extern void LCD_ShowIcon(uint xsta,uint ysta,uint xend,uint yend,uchar8 IconID);


#endif
/*=============================File end========================================================*/
