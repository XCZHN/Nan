#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x_lib.h"	
// SPI总线速度设置*/
#define SPI_SPEED_2   0
#define SPI_SPEED_8   1
#define SPI_SPEED_16  2
#define SPI_SPEED_256 3 //281K

// SD传输数据结束后是否释放总线宏定义  
#define NO_RELEASE      0
#define RELEASE         1

#define	SD_CS_ENABLE()  GPIOA->ODR&=~(1<<4); //选中SD卡
#define SD_CS_DISABLE() GPIOA->ODR|=1<<4;	  //取消SD选中	  		    													  
void SPI1_Init(void);
void SPI1_SetSpeed(u8 SpeedSet);   
u8 SPI1_ReadWriteByte(u8 TxData);                //SPI总线读写一个字节
		 
#endif

