#include <stm32f10x_lib.h>
#include "delay.h"
//使用SysTick的普通计数模式对延迟进行管理
//包括delay_us,delay_ms  
//正点原子@SCUT
//2008/12/14
//V1.2
//修正了中断中调用出现死循环的错误
//防止延时不准确,采用do while结构!	 
static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数
//初始化延迟函数
void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//选择内部时钟 HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}								    
//延时Nms
//注意Nms的范围
//Nms<=0xffffff*8/SYSCLK
//对72M条件下,Nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL=0x01 ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
}   
//延时us								   
void delay_us(u32 Nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=Nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}




































