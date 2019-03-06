#ifndef __COMM_H__
#define __COMM_H__
//ͨ�ú궨������
typedef char  CHAR;
typedef short SHORT;
typedef long  LONG;
	    	    			   
typedef unsigned long   DWORD;	//4�ֽ�
typedef int             BOOL;	//2�ֽ�
typedef u8              BYTE;	//1�ֽ�
typedef unsigned short  WORD;	//2�ֽ�



#define FALSE 0
#define TRUE  1 

#define MAX_LONG_NAME_SIZE 80	/* 26*n+2   n=3 */	//���ļ�����󳤶�

//�ļ���Ϣ�ṹ��
#define T_MP3  1<<0
#define T_WMA  1<<1
#define T_WAV  1<<2
#define T_MID  1<<3	

#define T_LRC  1<<4
#define T_TXT  1<<5
#define T_C    1<<6
#define T_H    1<<7

#define T_JPG  1<<8
#define T_JPEG 1<<9
#define T_BMP  1<<10
#define T_FILE 1<<11
#define T_FON  1<<12



#define Music_Mode 0
#define Ebook_Mode 1							
#define Radio_Mode 2
#define Set_Mode   3 

#define PREV 104   //����
#define NEXT 240   //����
#define CYCL 168   //ѭ��ģʽ
#define QUIT 56    //�˳�
#define PLAY 248   //����
#define PLAYOVER 0 //���Ž��� 

//�����������ң����£ã�
#define UP 7
#define DOWN 4
#define LEFT 6
#define RIGHT 5
#define KEY_A 3
#define KEY_B 2
#define KEY_C 1
#define KEY_D 0

#endif





