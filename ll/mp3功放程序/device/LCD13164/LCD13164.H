#ifndef __LCD13164_H
#define __LCD13164_H 
#include "stm32f10x_lib.h"	
#define uchar u8
#define uint unsigned int
/*
LCD_SDA--->PA8
LCD_SCK--->PA9
LCD_CS---->PA10
LCD_RS---->PA11
LCD_RST--->PA12
LCD_LIGHT->PB3
*/
//-----------------LCD端口定义----------------
#define	LCD_SDA	(1<<8)  //数据端口			
#define	LCD_SCK	(1<<9)  //时钟端口			
#define	LCD_CS	(1<<10) //片选端口  			
#define	LCD_RS	(1<<11) //数据/命令选择端口	
#define	LCD_RST	(1<<12) //硬复位
#define	LCD_LIGHT	(1<<3) //背光 PB3			


#define LCD_SDA_SET(x) GPIOA->ODR=(GPIOA->ODR&~LCD_SDA)|(x ? LCD_SDA:0)
#define LCD_SCK_SET(x) GPIOA->ODR=(GPIOA->ODR&~LCD_SCK)|(x ? LCD_SCK:0)													    
#define LCD_RS_SET(x)  GPIOA->ODR=(GPIOA->ODR&~LCD_RS) |(x ? LCD_RS:0) 
#define LCD_RST_SET(x) GPIOA->ODR=(GPIOA->ODR&~LCD_RST)|(x ? LCD_RST:0)	
#define LCD_CS_SET(x)  GPIOA->ODR=(GPIOA->ODR&~LCD_CS) |(x ? LCD_CS:0)	
#define LCD_LIGHT_SET(x) GPIOB->ODR=(GPIOB->ODR&~LCD_LIGHT) |(x ? LCD_LIGHT:0)												    
 
void Send_Data(unsigned char datain);    
void Write_data(unsigned char data); 
void Write_comd(unsigned char comd); 
void Set_column(unsigned char column);
void Set_page(unsigned char page);            
void Set_row(unsigned char row);
void Cleardisplay(u8 row);      
void Show_char(unsigned char x,unsigned char y,unsigned char chr);
void Show_str(unsigned char x,unsigned char y,const unsigned char * str);
void Show_num(unsigned char x,unsigned char y,unsigned int num);
void LCD_init(void); 






void Show_icon(unsigned char x,unsigned char y,unsigned char Ico);
void Show_font(unsigned char x,unsigned char y,unsigned char *chr);
void draw_picture(const unsigned char *p);
u8 LCD_write_String(u8 x, u8 y,u8 *p,u8 enchg);
void LCD_write_cstr(u8 x, u8 y,const u8 *str);
void Show_num2(unsigned char x,unsigned char y,unsigned char num);
void show_minichar(unsigned char x,unsigned char y,unsigned num);
void Show_mininum(unsigned char x,unsigned char y,unsigned char num);
void lcd_refresh(void);
u8 lcd_read(u8 x,u8 y);
void lcd_write(uchar x,uchar y,uchar data);
void WritePixel(uchar x,uchar y,uchar val);
uchar ReadPixel(uchar x,uchar y);
void Big_pinxl(uchar x,uchar y,uchar val);
void clearlcdram(void);
void save_show(void);
void MP3_msg(uchar pause);
void Play_GUI(void);
void Pro_msg(unsigned long npos,uchar mnum);
void Vs1003_GUI(void);
void Show_Bass(uchar i);

#endif         

