#ifndef __SPI2_H
#define __SPI2_H
#include "stm32f10x_lib.h"	
// SPI2总线速度设置*/
#define SPI2_SPEED_2   0
#define SPI2_SPEED_8   1
#define SPI2_SPEED_16  2
#define SPI2_SPEED_256 3 //281K

// SD传输数据结束后是否释放总线宏定义  
#define NO_RELEASE      0
#define RELEASE         1

#define	SD_CS_ENABLE()  GPIOB->ODR&=~(1<<12); //选中SD卡
#define SD_CS_DISABLE() GPIOB->ODR|=1<<12;	  //取消SD选中	  		    													  
void SPI2_Init(void);
void SPI2_SetSpeed(u8 SpeedSet);   
u8 SPI2_ReadWriteByte(u8 TxData);                //SPI总线读写一个字节
		 
#endif

