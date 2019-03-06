#ifndef __main__H
#define __main__H

#include "stm32f10x_lib.h" 
#include <math.h>
#include <stdlib.h> //abs函数
#include <stdio.h>
#include <string.h>
#include "sys.h"
#include "delay.h"
#include "vs1003.h"
#include "MMC_SD.h"
#include "lcd13164.h"
#include "fat.h"
#include "untogb.h"
#include "keyboard.h"
#include "my_fft.h"
#include "ADC.h"
#include "snake.h"
#include "russia.h"
#define ALL 255

extern u8 key;
extern u8 State;
//FFT部分
extern unsigned char m;
extern unsigned int N; 
extern float XR[],XI[]; 
extern u8 adc_over;   //128次adc转换完成

extern u8 key_point;
extern u8 fat_buffer[];
extern u8 pbuffer[];
extern u8	MCUBuffer[];
//extern short	QtZzMCUBuffer[];
//extern long iclip[];//4k BYTES
extern void Load_Sys_ICO(void);
extern u16 File_Browse(u16 file_type);

extern void timer3_init(void);
extern u8 FindLrcFile(unsigned long dircluster,unsigned char *strName);
extern void Lrc_Read_Init(void);

extern void Mus_Viewer(void);
extern void Show_FFT(void);

#endif
