#ifndef _RED_H
#define _RED_H

#define RDATA (GPIOC->IDR&(1<<13))  //红外接口//PC13

void NVIC_Configuration_RED(void);
void exti_init(void);
void red_init(void)	;	
void EXTI1_IRQHandler(void);
u8 redchange(u8 rednum);
void key_handle_red(void);
extern u8 key_red; //红外接收到数据  
#endif  
