#include "SPI2.h"
// 以下是SPI模块的初始化代码，配置成主机模式，访问SD卡 AT45DB161D							  
/*******************************************************************************
* Function Name  : SPI2_Configuration
* Description    : SPI2模块初始化，【包括相关IO口的初始化】
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_Init(void)
{	 
	RCC->APB2ENR|=1<<3;       //PORTB时钟使能   
	RCC->APB1ENR|=1<<14;      //SPI2时钟使能 

	GPIOB->CRH&=0X0000FFFF; 
	GPIOB->CRH|=0XBBB30000;	 //PB13,14,15复用 PB12(CS)推挽输出	    
	GPIOB->ODR|=0XF000;      //PB12,13,14,15上拉
	
	//下面一段为片选初始化，可以更改，不一定用PA4作片选
/*	GPIOB->CRH&=0XFFF0FFFF; 
	GPIOB->CRH|=0X00030000;//PB12推挽输出（CS片选）    
	GPIOB->ODR|=1<<12;      //PB12上拉	  */
		
	SPI2->CR1|=0<<10;//全双工模式	
	SPI2->CR1|=1<<9; //软件nss管理
	SPI2->CR1|=1<<8;  

	SPI2->CR1|=1<<2; //SPI主机
	SPI2->CR1|=0<<11;//8bit数据格式	
	SPI2->CR1|=1<<1; //空闲模式下SCK为1 CPOL=1
	SPI2->CR1|=1<<0; //数据采样从第二个时间边沿开始,CPHA=1  
	SPI2->CR1|=6<<3; //Fsck=Fcpu/256
	SPI2->CR1|=0<<7; //MSBfirst 
	SPI2->CR1|=1<<6; //SPI设备使能		
	SPI2_ReadWriteByte(0xff);//启动传输		 
}   
/*******************************************************************************
* Function Name  : SPI2_SetSpeed
* Description    : SPI2设置速度为高速
* Input          : u8 SpeedSet 
*                  如果速度设置输入0，则低速模式，非0则高速模式
*                  SPI_SPEED_HIGH   1
*                  SPI_SPEED_LOW    0
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_SetSpeed(u8 SpeedSet)
{
	SPI2->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI2_SPEED_2)//二分频
	{
		SPI2->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI2_SPEED_8)//八分频 
	{
		SPI2->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI2_SPEED_16)//十六分频
	{
		SPI2->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
	}else			 	 //256分频
	{
		SPI2->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz 低速模式
	}
	SPI2->CR1|=1<<6; //SPI设备使能	  
} 
/*******************************************************************************
* Function Name  : SPI1_ReadWriteByte
* Description    : SPI读写一个字节（发送完成后返回本次通讯读取的数据）
* Input          : u8 TxData 待发送的数
* Output         : None
* Return         : u8 RxData 收到的数
*******************************************************************************/
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	while((SPI2->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI2->DR=TxData;	 	  //发送一个byte 
	retry=0;
	while((SPI2->SR&1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI2->DR;          //返回收到的数据				    
}































