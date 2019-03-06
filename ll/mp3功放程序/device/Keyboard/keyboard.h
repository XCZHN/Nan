#ifndef __KEYBOARD_H
#define __KEYBOARD_H 
#include "stm32f10x_lib.h" 

#define PEN  (GPIOB->IDR&(1<<8)) //PB8
#define NPEN !(GPIOB->IDR&1<<8)    //!PEN

#define LS148_A0 (GPIOA->IDR&(1<<13)) //PA13
#define LS148_A1 (GPIOA->IDR&(1<<15)) //PA15
#define LS148_A2 (GPIOA->IDR&(1<<14)) //PA14

void keyboard_init(void);

#endif
