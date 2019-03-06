//MP3 功放程序 FFT部分
//QiYuan@SCUT
//2010/07/09

//dian为点数, dian=pow(2,m)   
#include<main.h>
#define PI 3.141592653
extern u8 adc_count;
extern u8 key;
extern u8 State;
float XR[128]={0},XI[128]={0}; 
unsigned char m;
unsigned int N;
const int dian[]={1,2,4,8,16,32,64,128,256,512,1024,2048};
void fft_show(void);
void fft_show2(u8 temp,u8 column);
void butterfly(void);  //蝶形运算
void fft()
{       
	//////////////位倒置////////////////
	int i,j,k;		//i,j为临时变量，k是倒置后的次序
	m=7;			//128点FFT   
	N=dian[m]; 
	for(i=0;i<N;i++)
	{
		k=0;
		for(j=0;j<m;j++)
		{
			if(((i>>j)&0x01)==1)	//判断第j位是否为1
			k=k+(1<<(m-j-1));		//如果是，则倒置后的K对应是第10-j-1位为1,例如i的第二位为1,则k的第七位为1
		}
		XI[k]=XR[i];
	}
	
	for(i=0;i<N;i++)
	{
		XR[i]=XI[i];
		XI[i]=0;		//虚部置0
	}    
	     
	butterfly();	
	
	///////////计算各频率点幅值//////////  
	for(i=0;i<N;i++)
	{
	    XR[i]=sqrt((XR[i]*XR[i])+(XI[i]*XI[i]));
	}
	
} /* END FFT */ 

	/////////////蝶形运算////////////////
void butterfly()
{
	float TR,TI,WR,WI;
	unsigned int i,j,k,p,A,B;		//*** A为外循环的级数; B为中循环的级数,C为内循环的级数
	A = m;
	for(i=0;i<A;i++)				//外循环
	{
		B = 1<<i;					//即B=pow(2,i)		
		for(j=0;j<B;j++)			//中循环
		{
			p=j*(1<<(A-i-1));		//这是FFT最难推的式子,旋转因子的系数
			for(k=j;k<N;k=k+2*B)	//内循环
			{
				TR=XR[k];TI=XI[k];
				WR=	XR[k+B]*cos((2*PI*p)/N)+XI[k+B]*sin((2*PI*p)/N);
				WI=-XR[k+B]*sin((2*PI*p)/N)+XI[k+B]*cos((2*PI*p)/N);
				XR[k]=TR+WR;
				XI[k]=TI+WI;
				XR[k+B]=TR-WR;
				XI[k+B]=TI-WI;
			}
		}
	}
} 

void Show_FFT()
{
	key=255;
	Cleardisplay(ALL);   //清屏
	LCD_write_String(2,0,"Audio Player",0);//无歌词/歌词显示不使能
	LCD_write_String(3,0,"128点 FFT Showing ^_^",0);//无歌词/歌词显示不使能
	ADC_DMA_int();
	while(1)
	{
		if(adc_over==1)
		{	
			adc_over=0;
			fft();
            fft_show();     
			TIM2->CR1|=1<<0;   //使能定时器2
		}


		if(key!=255)
		{
			if(key==KEY_B)	//退出
			{
				State=1;
				adc_count=0;
				TIM2->CR1&=~(1<<0);   //使能定时器2
				return;
			}
		}
	}
}



void fft_show()
{
	u8 i=0;
	for(i=1;i<64;i++)
	{
	 	fft_show2((u8)XR[i],i*2-1);
	} 
	   
}

void fft_show2(u8 temp,u8 column)
{
	u8 i=0,j=0;
	u8 data=0;
	for(j=1;j<5;j++)
	{
		if(temp<8*j)
		{
			for(i=0;i<(temp-(8*(j-1)));i++)
			{
				data+=(1<<(7-i));	
			}
			Set_page(4-j);
			Set_column(column);
			Write_data(data);
			Write_data(data);
			data=0;
			if(temp<24)
			{
				Set_page(0);
				Set_column(column);
				Write_data(0);Write_data(0);
				if(temp<16)	
				{
				  	Set_page(1);
					Set_column(column);
					Write_data(0);Write_data(0);		
					if(temp<8)
					{
						Set_page(2);
						Set_column(column);
						Write_data(0);Write_data(0);
					}		
				}
			}
			break;
		} 
		else
		{
			Set_page(4-j);
			Set_column(column);
			Write_data(0xff);	
			Write_data(0xff);
		}	
	}
}


