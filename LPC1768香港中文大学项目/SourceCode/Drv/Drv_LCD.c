/*******************************************************************
**文件名称：
**功能：TFT驱动
*******************************************************************/
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "Config.h"
#include "Drv_LCD.h"
#include "Mid_ZiKu.h"

#include "Deskstop.h"
#include "jicun.h"
#include "jicun1.h"
#include "jicun2.h"
/*
#include "xinxiang.h"
#include "xinxiang1.h"
#include "xinxiang2.h"  //开箱门请读卡
#include "xinxiang3.h"
*/
#include "system.h"		  //0
#include "system01.h"	  //1
#include "system02.h"	  //2
#include "system03.h"	  //3
#include "system04.h"	  //4
#include "system05.h"	  //5
#include "BoxDoor.h"	  //6
#include "BoxDoor01.h"	  //7
#include "BoxDoor02.h"	  //8
#include "BoxDoor03.h"	  //9
#include "BoxDoor04.h"	  //10
#include "BoxDoor05.h"	  //11
#include "manage.h"		  //12
#include "manage01.h"	  //13
#include "manage02.h"	  //14
#include "manage03.h"	  //15
#include "manage04.h"	  //16
#include "manage05.h"	  //17
#include "user.h"		  //18
#include "user01.h"		  //19
#include "user02.h"		  //20
#include "user03.h"		  //21
#include "user04.h"		  //22
#include "user05.h"		  //23
#include "exit.h"		  //24




extern void delay_ms(uint32 nms);
uchar8 DisplayBuf[520] = {0};

/*********************************************************
* 函数名称：void LCD_WR_REG(uchar8 Cmd)
* 功能描述：写寄存器
* D0-D7:  P0.23~P0.30
* D8-D15: P1.24~P1.31
**********************************************************/
void LCD_WR_REG(uchar8 Cmd)
{
	CLR_CS_PIN;
	CLR_RS_PIN;
	LPC_GPIO1->FIODIR |= 0xffff0000;				//控制16位总线为输出 (0 = INPUT, 1 = OUTPUT)
	LPC_GPIO1->FIOSET = Cmd << AD_PIN_NUM;			//先写数据中为1的位到锁存器
   	Cmd   = ~Cmd;						
	LPC_GPIO1->FIOCLR = Cmd << AD_PIN_NUM;   		//后写数据中为0的位到锁存器
	CLR_WR_PIN;
	SET_WR_PIN;
	SET_CS_PIN;
}

/*********************************************************
* 函数名称：void LCD_WR_DATA(uint RegValue)
* 功能描述：写16位数据
**********************************************************/
void LCD_WR_DATA(uint RegValue)
{
	CLR_CS_PIN;
	SET_RS_PIN;
	LPC_GPIO1->FIODIR |= 0xffff0000;				//控制16位总线为输出 (0 = INPUT, 1 = OUTPUT)
	LPC_GPIO1->FIOSET = RegValue << AD_PIN_NUM;	//先写数据中为1的位到锁存器
   	RegValue   = ~RegValue;						
	LPC_GPIO1->FIOCLR = RegValue << AD_PIN_NUM; 	//后写数据中为0的位到锁存器
	CLR_WR_PIN;
	SET_WR_PIN;
	SET_CS_PIN;
}

/*********************************************************
* 函数名称：void LCD_WriteReg(uchar8 LCD_Reg,uint LCD_RegValue)
* 功能描述：寄存器赋值
**********************************************************/
void LCD_WriteReg(uchar8 LCD_Reg,uint LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);
}

/*********************************************************
* 函数名称：uint LCD_ReadReg(uchar8 LCD_Reg)
* 功能描述：读寄存器数据
**********************************************************/
uchar8 LCD_ReadReg(uchar8 LCD_Reg)
{
	ulong temp;
	uint result;
	
	CLR_CS_PIN;
	CLR_RS_PIN;
	LPC_GPIO1->FIODIR |= 0xffff0000;			//控制16位总线为输出 (0 = INPUT, 1 = OUTPUT)
	LPC_GPIO1->FIOSET = LCD_Reg << 16;			//先写数据中为1的位到锁存器
   	LCD_Reg   = ~LCD_Reg;						
	LPC_GPIO1->FIOCLR = LCD_Reg << 16;   		//后写数据中为0的位到锁存器
	CLR_WR_PIN;
	SET_WR_PIN;
	
	SET_RS_PIN;
	LPC_GPIO1->FIOSET = 0xffff0000;				//16位数据总线置高
	LPC_GPIO1->FIODIR &= 0x0000ffff;
	CLR_RD_PIN;
	SET_RD_PIN;

	CLR_RD_PIN;
	temp= LPC_GPIO1->FIOPIN;
	SET_RD_PIN;	
	result=(uchar8)(temp>>16);
	SET_CS_PIN;
	return result;
}


/***********************************************************************************************
* Function Name  : LCD_Init
* Description	 : LCD初始化
* Input 		 : None
* Output		 : None
* Return		 : None
* Notes          :  
***********************************************************************************************/
void LCD_Init(void)
{
	LPC_PINCON->PINSEL0 &= 0xFFFFFFFC;       //WR--P0.0
	LPC_GPIO0->FIODIR |= (1 << WR);
	LPC_PINCON->PINSEL0 &= 0xFFFFFFF3;       //RD--P0.1
	LPC_GPIO0->FIODIR |= (1 << RD);

	LPC_PINCON->PINSEL4 &= 0xFFCFFFFF;       //RSTDSP--P2.10
	LPC_GPIO2->FIODIR |= (1 << RSTDSP);
	LPC_PINCON->PINSEL4 &= 0xFF3FFFFF;       //DISP--P2.11
	LPC_GPIO2->FIODIR |= (1 << DISP);
	LPC_PINCON->PINSEL4 &= 0xFCFFFFFF;       //RS--P2.12
	LPC_GPIO2->FIODIR |= (1 << RS);
	LPC_PINCON->PINSEL4 &= 0xF3FFFFFF;       //CSDSP--P2.13
	LPC_GPIO2->FIODIR |= (1 << CSDSP);

	SET_RST_PIN;
	SET_RD_PIN;
	SET_WR_PIN;
	SET_CS_PIN;
	SET_RS_PIN;
	SET_DISP_PIN;


	SET_RST_PIN;
	delay_ms(1);
	CLR_RST_PIN;
	delay_ms(100);
	SET_RST_PIN;
	delay_ms(120);
	
	LCD_WR_REG(0xCF);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xAA);
	LCD_WR_DATA(0xE0);

	LCD_WR_REG(0xED);
	LCD_WR_DATA(0x67);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0x81);

	LCD_WR_REG(0xE8);
	LCD_WR_DATA(0x85);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x78);

	LCD_WR_REG(0xCB);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x02);

	LCD_WR_REG(0xF7);
	LCD_WR_DATA(0x20);

	LCD_WR_REG(0xEA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA(0x21);

	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x11);

	LCD_WR_REG(0xC5);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x3C);

	LCD_WR_REG(0xC7);
	LCD_WR_DATA(0xB7);

	LCD_WR_REG(0x36);
	LCD_WR_DATA(0x88);//0x88,0x48

	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);

	LCD_WR_REG(0xF2);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0x26);
	LCD_WR_DATA(0x01);

	LCD_WR_REG(0xB7);
	LCD_WR_DATA(0x06);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x19);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0xA9);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x26);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x56);
	LCD_WR_DATA(0x50);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x0F);

	LCD_WR_REG(0x11);
	delay_ms(120);
	LCD_WR_REG(0x29);
	LCD_WR_REG(0x2C);
}

/*********************************************************
* 函数名称：ILI9486_EnterSleep_Code(void) 
* 功能描述：
**********************************************************/
void ILI9486_EnterSleep_Code(void) 
{ 
	LCD_WR_REG(0x28);
	LCD_WR_REG(0x10);
} 
 
/*********************************************************
* 函数名称：ILI9486_ExitSleep_Code(void) 
* 功能描述：
**********************************************************/
void ILI9486_ExitSleep_Code(void) 
{ 
 	LCD_WR_REG(0x11);
	delay_ms(120);
	LCD_WR_REG(0x29);
} 

/*********************************************************
* 函数名称：void LCD_WriteRAM_Prepare(void)
* 功能描述：开始写GRAM
**********************************************************/
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(0x2C);
}	 

/*********************************************************
* 函数名称：vvoid LCD_WriteRAM(u16 RGB_Code)
* 功能描述：LCD写GRAM
**********************************************************/
void LCD_WriteRAM(uint RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);//写十六位GRAM
}

/*********************************************************
* 函数名称：void LCD_DisplayOn(void)
* 功能描述：LCD开启显示
**********************************************************/
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0x29);
}	 

/*********************************************************
* 函数名称：void LCD_DisplayOff(void)
* 功能描述：LCD关闭显示
**********************************************************/
void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0x28);
}  

/*********************************************************
* 函数名称：void LCD_SetCursor(uint Xpos, uint Ypos)
* 功能描述：设置显示区域
*Column address(行地址0~319)
*Page address(列地址0~479)
**********************************************************/
void LCD_SetCursor(uint Xspos, uint Xepos, uint Yspos, uint Yepos)
{
	LCD_WR_REG(0x2a);
	LCD_WR_DATA(HIGH(Xspos));
	LCD_WR_DATA(LOW(Xspos));
	LCD_WR_DATA(HIGH(Xepos));
	LCD_WR_DATA(LOW(Xepos));
	
	LCD_WR_REG(0x2B);
	LCD_WR_DATA(HIGH(Yspos));
	LCD_WR_DATA(LOW(Yspos));
	LCD_WR_DATA(HIGH(Yepos));
	LCD_WR_DATA(LOW(Yepos));
	LCD_WR_REG(0x2C);
}

/*********************************************************
* 函数名称：void LCD_Clear(uint Color)
* 功能描述：清屏函数
* Color:要清屏的填充色
**********************************************************/
void LCD_Clear(uint Color)
{
	ulong index=0;	

	LCD_SetCursor(0,239,0,319);
	LCD_WriteRAM_Prepare();	  
	CLR_CS_PIN;
	SET_RS_PIN;
	LPC_GPIO1->FIODIR |= 0xffff0000;				//控制16位总线为输出 (0 = INPUT, 1 = OUTPUT)
	for(index=0;index<76800;index++)
	{
		LPC_GPIO1->FIOCLR=0xffff0000;
		LPC_GPIO1->FIOSET = Color << AD_PIN_NUM;	//先写数据中为1的位到锁存器
		CLR_WR_PIN;
		SET_WR_PIN;
	}
	SET_CS_PIN;
}  

/*********************************************************
* 函数名称：void LCD_Fill(uint xsta,uint ysta,uint xend,uint yend,uint color)
* 功能描述：在指定区域内填充指定颜色
* 区域大小：(xend-xsta)*(yend-ysta)
**********************************************************/
void LCD_Fill(uint xsta,uint ysta,uint xend,uint yend,uint color)
{
	ulong index=0;
	ulong len;	

	LCD_SetCursor(xsta,xend,ysta,yend);
	LCD_WriteRAM_Prepare();	  
	CLR_CS_PIN;
	SET_RS_PIN;
	LPC_GPIO1->FIODIR |= 0xffff0000;				//控制16位总线为输出 (0 = INPUT, 1 = OUTPUT)
	len=(xend-xsta+1)*(yend-ysta+1)*2;
	for(index=0;index<len;index++)
	{
		LPC_GPIO1->FIOCLR=0xffff0000;
		LPC_GPIO1->FIOSET = color << AD_PIN_NUM;	//先写数据中为1的位到锁存器
		CLR_WR_PIN;
		SET_WR_PIN;
	}
	SET_CS_PIN;
	LCD_SetCursor(0,239,0,319);
}

/*********************************************************
* 函数名称：void LCD_DrawPoint(uint x,uint y)
* 功能描述：在指定位置画点
* x:0~319
* y:0~479
* POINT_COLOR:此点的颜色
**********************************************************/
void LCD_DrawPoint(uint x,uint y,uint color)
{
	LCD_SetCursor(x,x,y,y);
	LCD_WR_REG(0x2c);
	LCD_WR_DATA(color); 	    
} 

/*********************************************************
* 函数名称：void LCD_DrawLine(uint x1, uint y1, uint x2, uint y2)
* 功能描述：画线
* x1,y1:起点坐标
* x2,y2:终点坐标  
**********************************************************/
void LCD_DrawLine(uint x1, uint y1, uint x2, uint y2,uint color)
{
	uint t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol;
	
	delta_x=x2-x1; 					//计算坐标增量 
	delta_y=y2-y1;
	uRow=x1; 
	uCol=y1; 
	
	if(delta_x>0)
		incx=1; 					//设置单步方向 
	else if(delta_x==0)
		incx=0;						//垂直线 
	else 
	{	incx=-1;
		delta_x=-delta_x;
	}
	 
	if(delta_y>0)
		incy=1; 
	else if(delta_y==0)
		incy=0;						//水平线 
	else
	{	incy=-1;
		delta_y=-delta_y;
	}
	
	if(delta_x>delta_y)
		distance=delta_x; 			//选取基本增量坐标轴 
	else 
		distance=delta_y;
	
	for(t=0;t<=distance+1;t++ )		//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol,color);	//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		}
	}
}

/*********************************************************
* 函数名称：LCD_DrawRectangle(uint x1, uint y1, uint x2, uint y2)
* 功能描述：画矩形
* x1,y1:起点坐标
* x2,y2:终点坐标  
**********************************************************/
void LCD_DrawRectangle(uint x1, uint y1, uint x2, uint y2,uint color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}

/*********************************************************
* 函数名称：void Draw_Circle(uint x0,uint y0,uchar8 r)
* 功能描述：在指定位置画一个指定大小的圆
* (x,y):中心点
* r    :半径
**********************************************************/
void Draw_Circle(uint x0,uint y0,uchar8 r,uint color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             				//判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);           //3           
		LCD_DrawPoint(x0+b,y0-a,color);           //0           
		LCD_DrawPoint(x0-a,y0+b,color);           //1       
		LCD_DrawPoint(x0-b,y0-a,color);           //7           
		LCD_DrawPoint(x0-a,y0-b,color);           //2             
		LCD_DrawPoint(x0+b,y0+a,color);           //4               
		LCD_DrawPoint(x0+a,y0-b,color);           //5
		LCD_DrawPoint(x0+a,y0+b,color);           //6 
		LCD_DrawPoint(x0-b,y0+a,color);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)
			di+=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b,color);
	}
} 

/************************************************************
*函数名称：	SET_FontColor(uint FC,uint BC)
*功能描述：	设置当前调色板
*<FC> 前景色调色板，2 字节(16 bit，65K color)，复位默认值是0xFFFF (白色)；
*<BC> 背景色调色板，2 字节(16 bit，65K color)，复位默认值是0x001F (蓝色)；
*16bit 调色板定义是5R6G5B 模式；
*一旦设定好，除非重新设定，就会一直保持下来，直到显示终端硬件复位后恢复默认值
************************************************************/
void SET_FontColor(uint FC,uint BC)
{
	
}

/************************************************************
*函数名称：	SET_FontDistance(uchar8 D_X,uchar8 D_Y)
*功能描述：	设置字符显示间距
*<D_X> X 方向的字符间距（列间距），取值范围 0x00-0x7F，复位默认值是0x00；
*<D_Y> Y 方向的字符间距（行间距），取值范围 0x00-0x7F，复位默认值是0x00；
************************************************************/
void SET_FontDistance(uchar8 D_X,uchar8 D_Y)
{

}

/************************************************************
*函数名称：	void Get_HzMat(const char *code,unsigned char *mat,uchar8 size)
*功能描述：	获取点阵数据
************************************************************/
void Get_HzMat(const char *code,unsigned char *mat,uchar8 size)
{
	ulong address,BaseAdd;
	uchar8 MSB,LSB;
	MSB=*code;
	LSB=*(code+1);
	if(!(size%12))
	{				   					//字体大小为11*12(6*12)，22*24(12*24)，44*48(24*48)，88*96(48*96)
	   	if(((uchar8)((*code)+0x50)>0x48)||((uchar8)((*code)+0x50)==0x48))
		{								//字符
			BaseAdd=0x66d40;
			if((MSB>=0x20)&&(MSB<=0x7E))
			{
				address=(MSB-0x20)*12+BaseAdd;
			}
			else
			{
				address=BaseAdd;
			}
			ZiKu_GetChineseChar(mat,address,12);
		}
		else
		{								//汉字
			BaseAdd=0x3cf80;
			if((MSB>=0xa1)&&(MSB<=0xa3)&&(LSB>=0xa1))
			{
				address=((MSB-0xa1)*94+(LSB-0xa1))*24+BaseAdd;
			}
			else if((MSB==0xa9)&&(LSB>=0xA1))
			{
				address=(282+(LSB-0xA1))*24+BaseAdd;
			}
			else if((MSB>=0xb0)&&(MSB<=0xf7)&&(LSB>=0xa1))
			{
				address=((MSB-0xb0)*94+(LSB-0xa1)+376)*24+BaseAdd;	
			}
			ZiKu_GetChineseChar(mat,address,24);
		}
	}
	else
	{									//字体大小为15*16，30*32，60*64
		if(((uchar8)((*code)+0x50)>0x48)||((uchar8)((*code)+0x50)==0x48))
		{								//字符
			BaseAdd=0x3b7c0;
			if((MSB>=0x20)&&(MSB<=0x7E))
			{
				address=(MSB-0x20)*16+BaseAdd;
			}
			else
			{
				address=BaseAdd;
			}
			ZiKu_GetChineseChar(mat,address,16);
		}
		else
		{								//汉字
			BaseAdd=0x0;
			if((MSB>=0xa4)&&(MSB<=0xa8)&&(LSB>=0xa1))
			{
				address=BaseAdd;
			}
			else if((MSB>=0xa1)&&(MSB<=0xa9)&&(LSB>=0xa1))
			{
				address=((MSB-0xa1)*94+(LSB-0xa1))*32+BaseAdd;
			}
			else if((MSB>=0xb0)&&(MSB<=0xf7)&&(LSB>=0xa1))
			{
				address=((MSB-0xb0)*94+(LSB-0xa1)+846)*32+BaseAdd;
			}
			else
			{
				address=BaseAdd;
			}
			ZiKu_GetChineseChar(mat,address,32);
		}
	}
}

/************************************************************
*函数名称：	void Show_Font(uint x,uint y,const char *str,uchar8 Font,uint color)
*功能描述：	显示一个汉字或字符
************************************************************/
void Show_Font(uint x,uint y,const char *str,uchar8 Font,uint color)
{
	uchar8 temp,t,t1,row;
	uint y0=y;
	uchar8 dzk[32];
	
	Get_HzMat(str,dzk,Font);
   	if(*str<0x80)
	{								//字符显示	
		if(!(Font%12))
		{							//6*12,12*24,24*48号字符显示
			for(t=0;t<12;t++)		//字符共12个字节
			{   												   
				temp=dzk[t];
				row=6;		   		//每行6个点
			    for(t1=0;t1<row;t1++)
				{
					if((Font/12)==1)
					{			   //6*12号字体
						if(temp&0x80)
						{
							LCD_DrawPoint(x,y,color);
						}   
						y-=1;
						if((y0-y)==6)
						{
							y=y0;
							x=x+1;
							break;
						}
					}
					else if((Font/12)==2)
					{			   //12*24号字体
						if(temp&0x80)
						{
							LCD_DrawPoint(x,y,color);
							LCD_DrawPoint(x,y-1,color);
							LCD_DrawPoint(x+1,y,color);
							LCD_DrawPoint(x+1,y-1,color);
						}   
						y-=2;
						if((y0-y)==12)
						{
							y=y0;
							x=x+2;
							break;
						}
					}
					else
					{			   //24*48号字体
						if(temp&0x80)
						{
							LCD_DrawPoint(x,y,color);
							LCD_DrawPoint(x,y-1,color);
							LCD_DrawPoint(x,y-2,color);
							LCD_DrawPoint(x,y-3,color);
							LCD_DrawPoint(x+1,y,color);
							LCD_DrawPoint(x+1,y-1,color);
							LCD_DrawPoint(x+1,y-2,color);
							LCD_DrawPoint(x+1,y-3,color);
							LCD_DrawPoint(x+2,y,color);
							LCD_DrawPoint(x+2,y-1,color);
							LCD_DrawPoint(x+2,y-2,color);
							LCD_DrawPoint(x+2,y-3,color);
							LCD_DrawPoint(x+3,y,color);
							LCD_DrawPoint(x+3,y-1,color);
							LCD_DrawPoint(x+3,y-2,color);
							LCD_DrawPoint(x+3,y-3,color);
						}   
						y-=4;
						if((y0-y)==24)
						{
							y=y0;
							x=x+4;
							break;
						}
					}
					temp<<=1;
				}  	 
		    }
		}
		else
		{
			for(t=0;t<16;t++)	   //字符共16字节组成
			{   												   
				temp=dzk[t];
				row=8;		   	   //每行8个点
			    for(t1=0;t1<row;t1++)
				{
					if((Font/16)==1)
					{			   //8*16号字体
						if(temp&0x80)
						{
							//LCD_DrawPoint(x,y,color);
							LCD_DrawPoint(x+3,y,color);
						}   
						y-=1;
						if((y0-y)==8)
						{
							y=y0;
							x=x+1;
							break;
						}
					}
					else if((Font/16)==2)
					{			   //16*32号字体
						if(temp&0x80)
						{
							LCD_DrawPoint(x,y,color);
							LCD_DrawPoint(x,y-1,color);
							LCD_DrawPoint(x+1,y,color);
							LCD_DrawPoint(x+1,y-1,color);
						}   
						y-=2;
						if((y0-y)==16)
						{
							y=y0;
							x=x+2;
							break;
						}
					}
					else
					{			   //32*64号字体
						if(temp&0x80)
						{
							LCD_DrawPoint(x,y,color);
							LCD_DrawPoint(x,y-1,color);
							LCD_DrawPoint(x,y-2,color);
							LCD_DrawPoint(x,y-3,color);
							LCD_DrawPoint(x+1,y,color);
							LCD_DrawPoint(x+1,y-1,color);
							LCD_DrawPoint(x+1,y-2,color);
							LCD_DrawPoint(x+1,y-3,color);
							LCD_DrawPoint(x+2,y,color);
							LCD_DrawPoint(x+2,y-1,color);
							LCD_DrawPoint(x+2,y-2,color);
							LCD_DrawPoint(x+2,y-3,color);
							LCD_DrawPoint(x+3,y,color);
							LCD_DrawPoint(x+3,y-1,color);
							LCD_DrawPoint(x+3,y-2,color);
							LCD_DrawPoint(x+3,y-3,color);
						}   
						y-=4;
						if((y0-y)==32)
						{
							y=y0;
							x=x+4;
							break;
						}
					}
					temp<<=1;
				}  	 
		    }
		}
	}
	else
	{								//汉字显示
		if(!(Font%12))
		{							//11*12,22*24,44*48号字体显示
			for(t=0;t<24;t++)
			{   												   
				temp=dzk[t];
				if(t%2)
					row=3;		   //每行11个点，第一个字节占8bit,第2字节占3bit
				else
					row=8;                       
			    for(t1=0;t1<row;t1++)
				{
					if((Font/12)==1)
					{			   //11*12号字体
						if(temp&0x80)
						{
							LCD_DrawPoint(x,y,color);
						}   
						y-=1;
						if((y0-y)==11)
						{
							y=y0;
							x=x+1;
							break;
						}
					}
					else if((Font/12)==2)
					{			   //22*24号字体
						if(temp&0x80)
						{
							LCD_DrawPoint(x,y,color);
							LCD_DrawPoint(x,y-1,color);
							LCD_DrawPoint(x+1,y,color);
							LCD_DrawPoint(x+1,y-1,color);
						}   
						y-=2;
						if((y0-y)==22)
						{
							y=y0;
							x=x+2;
							break;
						}
					}
					else
					{			   //48*48号字体
						if(temp&0x80)
						{
							LCD_DrawPoint(x,y,color);
							LCD_DrawPoint(x,y-1,color);
							LCD_DrawPoint(x,y-2,color);
							LCD_DrawPoint(x,y-3,color);
							LCD_DrawPoint(x+1,y,color);
							LCD_DrawPoint(x+1,y-1,color);
							LCD_DrawPoint(x+1,y-2,color);
							LCD_DrawPoint(x+1,y-3,color);
							LCD_DrawPoint(x+2,y,color);
							LCD_DrawPoint(x+2,y-1,color);
							LCD_DrawPoint(x+2,y-2,color);
							LCD_DrawPoint(x+2,y-3,color);
							LCD_DrawPoint(x+3,y,color);
							LCD_DrawPoint(x+3,y-1,color);
							LCD_DrawPoint(x+3,y-2,color);
							LCD_DrawPoint(x+3,y-3,color);
						}   
						y-=4;
						if((y0-y)==44)
						{
							y=y0;
							x=x+4;
							break;
						}
					}
					temp<<=1;
				}  	 
		    }
		}
		else
		{
			for(t=0;t<32;t++)
			{   												   
				temp=dzk[t]; 
				if(t%2)
					row=7;		   //每行15个点，第一个字节占8bit,第2字节占7bit
				else
					row=8;                      
			    for(t1=0;t1<row;t1++)
				{
					if((Font/16)==1)
					{			   //15*16号字体
						if(temp&0x80)
						{
							LCD_DrawPoint(x,y,color);
						}   
						y-=1;
						if((y0-y)==15)
						{
							y=y0;
							x=x+1;
							break;
						}
					}
					else if((Font/16)==2)
					{			   //30*32号字体
						if(temp&0x80)
						{
							LCD_DrawPoint(x,y,color);
							LCD_DrawPoint(x,y-1,color);
							LCD_DrawPoint(x+1,y,color);
							LCD_DrawPoint(x+1,y-1,color);
						}   
						y-=2;
						if((y0-y)==30)
						{
							y=y0;
							x=x+2;
							break;
						}
					}
					else
					{			   //60*64号字体
						if(temp&0x80)
						{
							LCD_DrawPoint(x,y,color);
							LCD_DrawPoint(x,y-1,color);
							LCD_DrawPoint(x,y-2,color);
							LCD_DrawPoint(x,y-3,color);
							LCD_DrawPoint(x+1,y,color);
							LCD_DrawPoint(x+1,y-1,color);
							LCD_DrawPoint(x+1,y-2,color);
							LCD_DrawPoint(x+1,y-3,color);
							LCD_DrawPoint(x+2,y,color);
							LCD_DrawPoint(x+2,y-1,color);
							LCD_DrawPoint(x+2,y-2,color);
							LCD_DrawPoint(x+2,y-3,color);
							LCD_DrawPoint(x+3,y,color);
							LCD_DrawPoint(x+3,y-1,color);
							LCD_DrawPoint(x+3,y-2,color);
							LCD_DrawPoint(x+3,y-3,color);
						}   
						y-=4;
						if((y0-y)==60)
						{
							y=y0;
							x=x+4;
							break;
						}
					}
					temp<<=1;
				}  	 
		    }
		}
    }
}

/************************************************************
*函数名称：标准库显示
*功能描述：文本显示
*<X> <Y> 显示字符串的起始位置（第一个字符左上角坐标位置）；
*<String> 要显示的字符串，汉字采用GB2312 编码，
*显示颜色由函数void SET_FontColor(uint FC,uint BC)指令设定，
*显示字符间距由函数void SET_FontDistance(uchar8 D_X,uchar8 D_Y)指令设置
************************************************************/
void DisplayString(uchar8 Font,uint x,uint y,const char *string,uint color)
{
	uchar8 bHz=0;     					//字符或者中文

	while(*string!=0)					//数据未结束
	{ 
		if(!bHz)
	    {
			if(*string>0x80)
			{
		    	bHz=1;					//中文 
			}
		    else             			//字符
		    {      
			    Show_Font(x,y,string,Font,color);
				string++;
				if(Font==64)
				{						//64号字体
					y-=32;
				}
				else if(Font==48)
				{						//48号字体
					y-=24;
				}
				else if(Font==32)
				{					   	//32号字体
					y-=16;
				}
				else if(Font==24)
				{						//24号字体
					y-=12;
				}
				else if(Font==16)
				{						//16号字体
					y-=8;
				} 
				else
				{						//12号字体
				    y-=6;				//下一个字符偏移量(字符为汉字的一半)
				}
		    }
	    }
	    else							//中文 
	    {     
	    	bHz=0;						//有汉字库    
		    Show_Font(x,y,string,Font,color);
		    string+=2; 
			if(Font==64)
			{							//64号字体
				y-=64;
			}
			else if(Font==48)
			{							//48号字体
				y-=48;
			}
			else if(Font==32)
			{						   	//32号字体
				y-=32;
			}
			else if(Font==24)
			{							//24号字体
				y-=24;
			}
			else if(Font==16)
			{							//16号字体
				y-=16;
			}
		    else
			{							//12号字体
			    y-=12;					//下一个汉字偏移
			}
	    }						 
	}
}

void DisplayASCII(uchar8 Font,uint x,uint y,const char ASCII,uint color)
{
	uchar8 temp,t,t1,row;
	uint y0=y;
	uchar8 dzk[32];

	Get_HzMat(&ASCII,dzk,Font);
	if(!(Font%12))
	{								//6*12,12*24,24*48号字符显示
		for(t=0;t<12;t++)			//字符共12个字节
		{   												   
			temp=dzk[t];
			row=6;		   			//每行6个点
		    for(t1=0;t1<row;t1++)
			{
				if((Font/12)==1)
				{			   		//6*12号字体
					if(temp&0x80)
					{
						LCD_DrawPoint(x,y,color);
					}   
					y-=1;
					if((y0-y)==6)
					{
						y=y0;
						x=x+1;
						break;
					}
				}
				else if((Font/12)==2)
				{			   		//12*24号字体
					if(temp&0x80)
					{
						LCD_DrawPoint(x,y,color);
						LCD_DrawPoint(x,y-1,color);
						LCD_DrawPoint(x+1,y,color);
						LCD_DrawPoint(x+1,y-1,color);
					}   
					y-=2;
					if((y0-y)==12)
					{
						y=y0;
						x=x+2;
						break;
					}
				}
				else
				{			   		//24*48号字体
					if(temp&0x80)
					{
						LCD_DrawPoint(x,y,color);
						LCD_DrawPoint(x,y-1,color);
						LCD_DrawPoint(x,y-2,color);
						LCD_DrawPoint(x,y-3,color);
						LCD_DrawPoint(x+1,y,color);
						LCD_DrawPoint(x+1,y-1,color);
						LCD_DrawPoint(x+1,y-2,color);
						LCD_DrawPoint(x+1,y-3,color);
						LCD_DrawPoint(x+2,y,color);
						LCD_DrawPoint(x+2,y-1,color);
						LCD_DrawPoint(x+2,y-2,color);
						LCD_DrawPoint(x+2,y-3,color);
						LCD_DrawPoint(x+3,y,color);
						LCD_DrawPoint(x+3,y-1,color);
						LCD_DrawPoint(x+3,y-2,color);
						LCD_DrawPoint(x+3,y-3,color);
					}   
					y-=4;
					if((y0-y)==24)
					{
						y=y0;
						x=x+4;
						break;
					}
				}
				temp<<=1;
			}  	 
	    }
	}
	else
	{
		for(t=0;t<16;t++)	   		//字符共16字节组成
		{   												   
			temp=dzk[t];
			row=8;		   	   		//每行8个点
		    for(t1=0;t1<row;t1++)
			{
				if((Font/16)==1)
				{			   		//8*16号字体
					if(temp&0x80)
					{
						LCD_DrawPoint(x,y,color);
					}   
					y-=1;
					if((y0-y)==8)
					{
						y=y0;
						x=x+1;
						break;
					}
				}
				else if((Font/16)==2)
				{			   		//16*32号字体
					if(temp&0x80)
					{
						LCD_DrawPoint(x,y,color);
						LCD_DrawPoint(x,y-1,color);
						LCD_DrawPoint(x+1,y,color);
						LCD_DrawPoint(x+1,y-1,color);
					}   
					y-=2;
					if((y0-y)==16)
					{
						y=y0;
						x=x+2;
						break;
					}
				}
				else
				{			   		//32*64号字体
					if(temp&0x80)
					{
						LCD_DrawPoint(x,y,color);
						LCD_DrawPoint(x,y-1,color);
						LCD_DrawPoint(x,y-2,color);
						LCD_DrawPoint(x,y-3,color);
						LCD_DrawPoint(x+1,y,color);
						LCD_DrawPoint(x+1,y-1,color);
						LCD_DrawPoint(x+1,y-2,color);
						LCD_DrawPoint(x+1,y-3,color);
						LCD_DrawPoint(x+2,y,color);
						LCD_DrawPoint(x+2,y-1,color);
						LCD_DrawPoint(x+2,y-2,color);
						LCD_DrawPoint(x+2,y-3,color);
						LCD_DrawPoint(x+3,y,color);
						LCD_DrawPoint(x+3,y-1,color);
						LCD_DrawPoint(x+3,y-2,color);
						LCD_DrawPoint(x+3,y-3,color);
					}   
					y-=4;
					if((y0-y)==32)
					{
						y=y0;
						x=x+4;
						break;
					}
				}
				temp<<=1;
			}  	 
	    }
	}
}

/************************************************************
*函数名称：标准库显示
*功能描述：文本显示
*<X> <Y> 显示字符串的起始位置（第一个字符左上角坐标位置）；
*<String> 要显示的字符串，汉字采用GB2312 编码，
*显示颜色由参数指定
*显示字符间距由函数void SET_FontDistance(uchar8 D_X,uchar8 D_Y)指令设置
************************************************************/
void PrtString(uchar8 Font,uint X,uint Y,uint Fcolor,uint Bcolor,const char *string)
{
	
}

void PrtASCII(uchar8 Font,uint X,uint Y,uint Fcolor,uint Bcolor,uchar8 ASCII)
{

}


/***********************************************************************************************
* Function Name  : LCD_ShowFillPic
* Description	 : LCD显示一张图片
* Input 		 : PicID--图片编号
***********************************************************************************************/
void LCD_ShowFillPic(uchar8 PicID)
{
	uint32 i, index =0;	
	uint16 t;

	LCD_SetCursor(0, 239, 0, 319);
	LCD_WriteRAM_Prepare();	  
	CLR_CS_PIN;
	SET_RS_PIN;
	LPC_GPIO1->FIODIR |= 0xffff0000;				//控制16位总线为输出 (0 = INPUT, 1 = OUTPUT)
	for(index=0; index<300; index++)
	{
		memcpy(DisplayBuf, matrix+index*512, 512);
		for(i=0; i<256; i++)
		{
			t = (DisplayBuf[2*i]*256 + DisplayBuf[2*i+1]);
			LPC_GPIO1->FIOCLR=0xffff0000;
			LPC_GPIO1->FIOSET = t << AD_PIN_NUM;	//先写数据中为1的位到锁存器
			CLR_WR_PIN;
			SET_WR_PIN;
		}
	}
	SET_CS_PIN;
}

/***********************************************************************************************
* Function Name  : LCD_ShowPartPic
* Description	 : LCD显示一张不完整的图片
* Input 		 : PicID--图片编号
***********************************************************************************************/
void LCD_ShowPartPic(uint xsta, uint ysta, uint xend, uint yend, uchar8 PicID)
{
	ulong i = 0, index = 0;	
	uint t = 0;
	const uchar8* pPicAddr = 0;
	
	if(PicID ==0)
	{//标题栏:欢迎使用智能信报箱
		xsta = 8;
		ysta = 51;
		xend = 20;
		yend = 299;
	//	pPicAddr = xinxiang;
	}
	else if(PicID ==1)
	{//信息栏:（读卡或输密码开箱）
		xsta = 114;
		ysta = 157;
		xend = 20;
		yend = 299;
	//	pPicAddr = xinxiang1;
	}
	else if(PicID ==2)
	{//标题栏:欢迎使用智能寄存柜
		xsta = 8;
		ysta = 51;
		xend = 20;
		yend = 299;
		pPicAddr = jicun;
	}
	else if(PicID ==3)
	{//信息栏:（存物请按寄存）
		xsta = 114;
		ysta = 157;
		xend = 20;
		yend = 299;
		pPicAddr = jicun1;
	}
	else if(PicID ==4)
	{//信息栏:（取物请按指纹）
		xsta = 120;
		ysta = 163;
		xend = 20;
		yend = 299;
	//	pPicAddr = jicun2;
	}
	else if(PicID ==5)
	{//信息栏:（开箱门请读卡）
		xsta = 114;
		ysta = 157;
		xend = 20;
		yend = 299;
		pPicAddr = jicun2;
	}
	else if(PicID ==6)
	{//信息栏:（开箱门请输入密码）
		xsta = 114;
		ysta = 157;
		xend = 20;
		yend = 299;
	//	pPicAddr = xinxiang3;
	} 
	else
	{
		return;
	}

	LCD_SetCursor(xsta, ysta, xend, yend);
	LCD_WriteRAM_Prepare();	  
	CLR_CS_PIN;
	SET_RS_PIN;
	LPC_GPIO1->FIODIR |= 0xffff0000;				//控制16位总线为输出 (0 = INPUT, 1 = OUTPUT)
	for(index=0; index<80; index++)
	{
		memcpy(DisplayBuf, pPicAddr+index*308, 308);
		for(i=0;i<154;i++)
		{
			t = (DisplayBuf[2*i]*256 + DisplayBuf[2*i+1]);
			
			LPC_GPIO1->FIOCLR = 0xffff0000;
			LPC_GPIO1->FIOSET = t << AD_PIN_NUM;	//先写数据中为1的位到锁存器
			CLR_WR_PIN;
			SET_WR_PIN;
		}
	}
	SET_CS_PIN;	
}

/***********************************************************************************************
* Function Name  : LCD_ShowIcon
* Description	 : LCD显示图标
* Input 		 : IconID--图标编号
***********************************************************************************************/
void LCD_ShowIcon(uint xsta,uint ysta,uint xend,uint yend,uchar8 IconID)
{
	ulong i,index=0;	
	uint t;

	LCD_SetCursor(xsta,xend,ysta,yend);
	LCD_WriteRAM_Prepare();	  
	CLR_CS_PIN;
	SET_RS_PIN;
	LPC_GPIO1->FIODIR |= 0xffff0000;				//控制16位总线为输出 (0 = INPUT, 1 = OUTPUT)
	for(index=0;index<9;index++)
	{
		if(IconID==0)
			memcpy(DisplayBuf,system+index*512,512);
		else if(IconID==1)
			memcpy(DisplayBuf,system01+index*512,512);
		else if(IconID==2)
			memcpy(DisplayBuf,system02+index*512,512);
		else if(IconID==3)
			memcpy(DisplayBuf,system03+index*512,512);
		else if(IconID==4)
			memcpy(DisplayBuf,system04+index*512,512);
		else if(IconID==5)
			memcpy(DisplayBuf,system05+index*512,512);
		else if(IconID==6)
			memcpy(DisplayBuf,BoxDoor+index*512,512);
		else if(IconID==7)
			memcpy(DisplayBuf,BoxDoor01+index*512,512);
		else if(IconID==8)
			memcpy(DisplayBuf,BoxDoor02+index*512,512);
		else if(IconID==9)
			memcpy(DisplayBuf,BoxDoor03+index*512,512);
		else if(IconID==10)
			memcpy(DisplayBuf,BoxDoor04+index*512,512);
		else if(IconID==11)
			memcpy(DisplayBuf,BoxDoor05+index*512,512);
		else if(IconID==12)
			memcpy(DisplayBuf,manage+index*512,512);
		else if(IconID==13)
			memcpy(DisplayBuf,manage01+index*512,512);
		else if(IconID==14)
			memcpy(DisplayBuf,manage02+index*512,512);
		else if(IconID==15)
			memcpy(DisplayBuf,manage03+index*512,512);
		else if(IconID==16)
			memcpy(DisplayBuf,manage04+index*512,512);
		else if(IconID==17)
			memcpy(DisplayBuf,manage05+index*512,512);
		else if(IconID==18)
			memcpy(DisplayBuf,user+index*512,512);
		else if(IconID==19)
			memcpy(DisplayBuf,user01+index*512,512);
		else if(IconID==20)
			memcpy(DisplayBuf,user02+index*512,512);
		else if(IconID==21)
			memcpy(DisplayBuf,user03+index*512,512);
		else if(IconID==22)
			memcpy(DisplayBuf,user04+index*512,512);
		else if(IconID==23)
			memcpy(DisplayBuf,user05+index*512,512);
		else if(IconID==24)
			memcpy(DisplayBuf,exit+index*512,512);
		else
			memset(DisplayBuf,0x00,512);

		for(i=0;i<256;i++)
		{
			t=(DisplayBuf[2*i]*256+DisplayBuf[2*i+1]);
			LPC_GPIO1->FIOCLR=0xffff0000;
			LPC_GPIO1->FIOSET = t << AD_PIN_NUM;	//先写数据中为1的位到锁存器
			CLR_WR_PIN;
			SET_WR_PIN;
		}
	}
	SET_CS_PIN;
}

