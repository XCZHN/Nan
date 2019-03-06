//MP3 ���ų��� FFT����
//QiYuan@SCUT
//2010/07/09

//dianΪ����, dian=pow(2,m)   
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
void butterfly(void);  //��������
void fft()
{       
	//////////////λ����////////////////
	int i,j,k;		//i,jΪ��ʱ������k�ǵ��ú�Ĵ���
	m=7;			//128��FFT   
	N=dian[m]; 
	for(i=0;i<N;i++)
	{
		k=0;
		for(j=0;j<m;j++)
		{
			if(((i>>j)&0x01)==1)	//�жϵ�jλ�Ƿ�Ϊ1
			k=k+(1<<(m-j-1));		//����ǣ����ú��K��Ӧ�ǵ�10-j-1λΪ1,����i�ĵڶ�λΪ1,��k�ĵ���λΪ1
		}
		XI[k]=XR[i];
	}
	
	for(i=0;i<N;i++)
	{
		XR[i]=XI[i];
		XI[i]=0;		//�鲿��0
	}    
	     
	butterfly();	
	
	///////////�����Ƶ�ʵ��ֵ//////////  
	for(i=0;i<N;i++)
	{
	    XR[i]=sqrt((XR[i]*XR[i])+(XI[i]*XI[i]));
	}
	
} /* END FFT */ 

	/////////////��������////////////////
void butterfly()
{
	float TR,TI,WR,WI;
	unsigned int i,j,k,p,A,B;		//*** AΪ��ѭ���ļ���; BΪ��ѭ���ļ���,CΪ��ѭ���ļ���
	A = m;
	for(i=0;i<A;i++)				//��ѭ��
	{
		B = 1<<i;					//��B=pow(2,i)		
		for(j=0;j<B;j++)			//��ѭ��
		{
			p=j*(1<<(A-i-1));		//����FFT�����Ƶ�ʽ��,��ת���ӵ�ϵ��
			for(k=j;k<N;k=k+2*B)	//��ѭ��
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
	Cleardisplay(ALL);   //����
	LCD_write_String(2,0,"Audio Player",0);//�޸��/�����ʾ��ʹ��
	LCD_write_String(3,0,"128�� FFT Showing ^_^",0);//�޸��/�����ʾ��ʹ��
	ADC_DMA_int();
	while(1)
	{
		if(adc_over==1)
		{	
			adc_over=0;
			fft();
            fft_show();     
			TIM2->CR1|=1<<0;   //ʹ�ܶ�ʱ��2
		}


		if(key!=255)
		{
			if(key==KEY_B)	//�˳�
			{
				State=1;
				adc_count=0;
				TIM2->CR1&=~(1<<0);   //ʹ�ܶ�ʱ��2
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


