#include "main.h"  
//����ԭ��@scut 08/09/12
//ʵ�ֹ���:�����˹����,��� 12*16
//�ο�����:���ľ�����˹�������

extern u16 timer4_count;

extern u8 GAME_OVER;  //��Ϸ������־
extern u8 PASS;       //��ͨ�صı�־
extern u8 pauseen;    //��ͣ 
extern u8 GAME_PLAY;//��Ϸģʽ    

//����       
extern u8 LEVELEPM_Snake; //���漶�� 
extern u8 LEVELEPM_Russia;
extern u16 MAXSCORE_Snake;  //��߷ּ�¼
extern u16 MAXSCORE_Russia;

extern struct SNAKE	
{ 
	unsigned char Direction;//�ƶ�����
#define TO_RIGHT RIGHT
#define TO_LEFT LEFT
#define TO_UP UP
#define TO_DOWN DOWN
#define PAUSE KEY_C
  
#define LEVAND   0X06
#define LEVSUB   0X07 
#define SAVELEV  0x08  
#define BACK     0X09
	unsigned int  Dot_Cnt;//������
	unsigned char Next_X;//Ŀ���
	unsigned char Next_Y;
} Snake_Data;//�����ṹ�����  

extern u8 LEVEL;         //�ȼ��ݴ���   
extern u16 score;          //�����ݴ���


void fk_reffk(void);				//ˢ�·���
void fk_refnew(void);				//ˢ��Ԥ������
void fk_refline(uchar yy);			//ˢ��1�б���
uchar fk_chk(void);					//��ͻ���
void fk_new(void);					//�����·���
void fk_add(void);					//����ϲ�
void Play_Russia(void);  
void Russia_msg(void); 

#define MAX_LEVEL_Russia 8 //��߼���  ���㹫˾ ��ʱ :90-LEVEL*10;�����ʱ90*5ms,��С10*5ms
//�ҵ�Һ���� 12*16��С(ÿ���СΪ4*4)�Ŀռ������˶��ռ� 
#define FULLMAP	0x0fff				/*����*/
#define LINEGUAN	15				/*20�й�һ��*/
#define NEWX	13					/*Ԥ������Xλ��*/
#define NEWY	12					/*Ԥ������Yλ��*/
#define LINEMAX	15					/*��Ļ���15��*/
 
u8 fk_run=0;//����˹���鿪ʼ��־     

//����˹�������ݽṹ��
struct Russia
{
    u16 fk_map[LINEMAX+4];		        //����ӳ��
    u8  fk_x,fk_y,fk_r;				//�������ҡ��߶ȡ�����
    u8 fk_type;						//������״
    u8 fk_oldx,fk_oldy,fk_oldr;		//�����ϴ����ҡ��߶ȡ�����
    u8 fk_newtype,fk_newr;			//�·�����״������  
}Russia_Data;  

//ģ��,�ܹ�����ģ��
const u8 fk_mod[7][4][4]={			//����ģ�ͺ�,4������,4��
    7,2,0,0,1,3,1,0,2,7,0,0,2,3,2,0,    //_|_
    2,3,1,0,3,6,0,0,2,3,1,0,3,6,0,0,    //_|~
    1,3,2,0,6,3,0,0,1,3,2,0,6,3,0,0,    //~|_
    1,1,3,0,4,7,0,0,3,2,2,0,7,1,0,0,    //|__
    3,1,1,0,1,7,0,0,2,2,3,0,7,4,0,0,    //__|
    1,1,1,1,15,0,0,0,1,1,1,1,15,0,0,0,  //____
    3,3,0,0,3,3,0,0,3,3,0,0,3,3,0,0     //����
};      
//ˢ�·���
//�������ܺ�ʱ��
void fk_reffk(void)					 
{
	u8 i,j;
	u8 temp; 
	for (i=0;i<4;i++)//���ԭ���ķ���
	{
		temp=(fk_mod[Russia_Data.fk_type][Russia_Data.fk_oldr][i]);
		for (j=Russia_Data.fk_oldx;j<Russia_Data.fk_oldx+4;j++)
		{
			if(temp&0x01)Big_pinxl(j,Russia_Data.fk_oldy+i,0);//���һ��4*4�ĵ� 
			temp>>=1;
		}
	} 
	for (i=0;i<4;i++)//��ʾ�µķ���
	{
		temp=(fk_mod[Russia_Data.fk_type][Russia_Data.fk_r][i]);
		for (j=Russia_Data.fk_x;j<Russia_Data.fk_x+4;j++)
		{
			if(temp&0x01)Big_pinxl(j,Russia_Data.fk_y+i,1);//���һ��4*4�ĵ� 
			temp>>=1;
		}
	}
	Russia_Data.fk_oldx=Russia_Data.fk_x;Russia_Data.fk_oldy=Russia_Data.fk_y;Russia_Data.fk_oldr=Russia_Data.fk_r;	//�����·���λ�� 
	lcd_refresh();//ˢ��lcd�Դ�
} 
 






//ˢ��Ԥ������
void fk_refnew(void)				
{
	u8 i,j;
	u8 temp;
	//Ԥ������
	for (i=0;i<4;i++)
	{
		temp=fk_mod[Russia_Data.fk_newtype][Russia_Data.fk_newr][i];
		for (j=NEWX;j<NEWX+4;j++)
		{ 
		    Big_pinxl(j,NEWY+i,temp&0x01); 
			temp>>=1;
		}
	}  
	lcd_refresh();//ˢ��lcd�Դ�
} 
 
//ˢ��1�б���
void fk_refline(uchar yy)					
{
	u8 i;
	u16 temp; 
	temp=Russia_Data.fk_map[yy];
	for (i=0;i<12;i++)
	{ 
	    Big_pinxl(i,yy,temp&0x01); 
	    temp >>= 1;
	} 
	lcd_refresh();
}  
//��ͻ���        
//check ok
uchar fk_chk(void)					
{
	u8 i;
	u8 neq=0; 
	for (i=0;i<4;i++)
	{    
		if (((((uint)fk_mod[Russia_Data.fk_type][Russia_Data.fk_r][i])<<Russia_Data.fk_x)+(Russia_Data.fk_map[Russia_Data.fk_y+i]))!=((((uint)fk_mod[Russia_Data.fk_type][Russia_Data.fk_r][i])<<Russia_Data.fk_x)|(Russia_Data.fk_map[Russia_Data.fk_y+i])))
		neq=1;  
	}
	return(neq);
} 	  
//check ok 
//�����·���
void fk_new(void)						
{       
    srand(timer4_count);//�õ�����
	srand(rand()+Russia_Data.fk_x+Russia_Data.fk_y+Russia_Data.fk_r);
	Russia_Data.fk_oldx=Russia_Data.fk_x=5;        //�·��������
	Russia_Data.fk_oldy=Russia_Data.fk_y=LINEMAX; 
	
	Russia_Data.fk_type=Russia_Data.fk_newtype;//���ϴε���Ϣ�������Ҫ��ʾ��
	Russia_Data.fk_oldr=Russia_Data.fk_newr; 	
	Russia_Data.fk_r=Russia_Data.fk_oldr; 
    Russia_Data.fk_newtype = rand()%7;
	Russia_Data.fk_newr=rand()%4;  
	fk_refnew();//ˢ��Ԥ������  
}         
//����ϲ�
//check ok
void fk_add(void)					
{
	uchar i,j;
	uchar full=0x00;
	uchar fulltemp;
	uchar fullline=0x00;   
	for (i=0;i<4;i++)//����ϲ�
	{
		Russia_Data.fk_map[Russia_Data.fk_y+i]|=(uint)(fk_mod[Russia_Data.fk_type][Russia_Data.fk_r][i])<<Russia_Data.fk_x;//��map�����ÿһ�е���Ӧλͬģ�ͱȽ�
		full <<= 1;
		if ((Russia_Data.fk_y+i>=0)&&(Russia_Data.fk_map[Russia_Data.fk_y+i]==0xffff))full|=0x01;//ͳ�����е����� 
	}
	if (full!=0)//������
	{
		for (j=0;j<3;j++)		//������˸3��
		{ 
			fulltemp=full;
			for (i=0;i<4;i++) //4��
			{
				if ((fulltemp&0x08)!=0)//�����������
				{
					Russia_Data.fk_map[Russia_Data.fk_y+i]^=FULLMAP;
				}
				fk_refline(Russia_Data.fk_y+i);//ˢ��һ�е���ʾ����(�ܹ��Ĵ�,Ҳ����һ��ͼ�ε���󳤶�)
				fulltemp<<=1;
			}
			delay_ms(200);
		}
		fulltemp=full;
		for (i=Russia_Data.fk_y;i<LINEMAX+4+fullline;i++)//Russia_Data.fk_y ���ϵ����е�������
		{
			if ((i<LINEMAX+4)&&(Russia_Data.fk_map[i-fullline]!=Russia_Data.fk_map[i]))
			{
				Russia_Data.fk_map[i-fullline]=Russia_Data.fk_map[i];
				fk_refline(i-fullline);
			}
			if ((i>=LINEMAX+4)&&(Russia_Data.fk_map[i-fullline] != ~FULLMAP))
			{
				Russia_Data.fk_map[i-fullline]=~FULLMAP;//����ӳ�� ���
				fk_refline(i-fullline);
			}
			if ((fulltemp&0x08)!=0)fullline++;//���б�־,ͳ�����е����� 
			fulltemp <<= 1;
		} 
		fulltemp=1;//����ͳ��,��һ�е�2��,���е��ķ�,���еð˷�,���е�ʮ����
		while(fullline)
		{
		    fulltemp*=2;
		    fullline--;
		} 
		score+=fulltemp; 
		Russia_msg();       //��ʾ����
		fk_new();			//�����·���
	}
	else if (Russia_Data.fk_y==LINEMAX)fk_run=0;//�����λ����ײ�Ҳ�����������Ϸ���� 
	else fk_new(); 
}  
//�����ʼ�� 
void fk_init(void)					
{
    uchar i;
    Cleardisplay(4);//LCD���� 
    clearlcdram();//���sram  
    Russia_Data.fk_map[0]=0xffff;            //����ӳ��   
    for (i=0;i<LINEMAX+4;i++)
    {
        Russia_Data.fk_map[i]=~FULLMAP;          //����ӳ��
        fk_refline(i);               //ˢ��1�б���
    } 
    fk_run = 1; 
    fk_new();                        //�����·���
}             
//����˹������ʾ�����ȼ���Ϣ
void Russia_msg(void)
{                 
    Show_num(1,109,LEVEL);  
    Show_num(3,103,score/100);
    Show_num(3,115,score%100); 
}     
void Russia_GUI(void)
{ 
    uchar i=0;     
	for(i=0;i<64;i++)WritePixel(48,i,1);//��һ������,д���Դ�
	for(i=0;i<64;i++)WritePixel(95,i,1);//�ڶ�������,д�뻺��
	for(i=0;i<47;i++)WritePixel(i+48,44,1);//����������,д�뻺�� 
    LCD_write_cstr(0,99,"LEVEL");
    if(pauseen)LCD_write_cstr(2,99,"PAUSE");
    else LCD_write_cstr(2,99,"SCORE"); 
    Russia_msg();  
}
//����˹����������
//����Ϸ
void Play_Russia(void)						
{ 
	uchar timecount=0;//��ʱ��  
	uchar overtime=0;
	Timer4_Init();
    if(LEVELEPM_Russia>MAX_LEVEL_Russia)LEVELEPM_Russia=4;//��ʼ���ȼ� 
    else LEVEL=LEVELEPM_Russia; //��ȡeeprom����
    if(MAXSCORE_Russia>9999)MAXSCORE_Russia=0;//��ʼ������   	 
	overtime=90-LEVEL*10;//���10ms��ʱ
	
	score=0;  //�������
	pauseen=0;//�ر���ͣ 
	Russia_GUI(); 
	GAME_PLAY=1;//��ʼ��Ϸ
	fk_run=1;  
	while(fk_run)//��ѭ��,һֱ�浽��,�����û��˳�
	{ 
	    if(key!=255)
	    { 
	        timecount=0;//��ʱ����� 
	       	switch(key)
			{ 
				case TO_DOWN:		//���� 
				{
		 			if (fk_run)	//��Ϸ������
					{
						if((Russia_Data.fk_y>0)&&!(fk_chk()))Russia_Data.fk_y--;//��һһ��
						if(fk_chk()){Russia_Data.fk_y++;break;}//��ͻȡ��,ֱ���˳�
						if((Russia_Data.fk_y>0)&&!(fk_chk()))Russia_Data.fk_y--;//��һһ��
						if(fk_chk())Russia_Data.fk_y++;//��ͻȡ��
						break;
					}
					else fk_new();				//�����·��� 
					break;  
				} 
				case TO_RIGHT:		//����  
				{  
					if (fk_run)					//��Ϸ������
					{
						if(Russia_Data.fk_x<12)
						{
							Russia_Data.fk_x++;
							if(fk_chk())Russia_Data.fk_x--;//�г�ͻȡ������
						}
					}     
					break;   
				}
				case TO_LEFT:		//����
				{
					if (fk_run)						//��Ϸ������
					{
						if(Russia_Data.fk_x>0)
						{
							Russia_Data.fk_x--;
							if(fk_chk())Russia_Data.fk_x++;//�г�ͻȡ������
						}
					} 
					break;
				}
				case TO_UP:		//��ת  
				{
					if (fk_run)						//��Ϸ����ת
					{
						Russia_Data.fk_r++;
						if(fk_chk())Russia_Data.fk_r--;//�г�ͻȡ������
						Russia_Data.fk_r&=0x03;
					}
					else fk_run=1;//��ʼ��ʱ  
					break;
				}                    
            case KEY_D://ˮƽ����
			{
			    if(LEVEL)LEVEL--;
			    else LEVEL=0;  
			    overtime=90-LEVEL*10;//����ʱ��,��ʱ10ms
			    Russia_msg();//������ʾ 
			    break;
			}

				case PAUSE:pauseen=!pauseen;Russia_GUI();break; 
				case KEY_B:{GAME_PLAY=0;State=1;return ;}//�������˳�
			}  
			fk_reffk();	//ˢ����ʾ
			continue;
	    }
	    if(timecount==overtime)  
	    {    
	        timecount=0;//��ʱ�����  
	        if(pauseen)continue;//��ͣ,����˴β���,���¿�ʼ
	         
	        if (fk_run)//��Ϸ������
            {
			    if(Russia_Data.fk_y>0)
			    {
			        Russia_Data.fk_y--;
			        if(fk_chk())
			        {
			            Russia_Data.fk_y++;//�г�ͻȡ������,ִ����ײ���
					    fk_add();	//����ϲ�	 
			        }
			    }
			    else fk_add();		//����ϲ�   
			    fk_reffk();//ˢ��  
			    continue;
	        } 
	    }   
	    delay_ms(5);    
	    timecount++;//��ʱ������
    }
    Game_Over_Show(1);//��Ϸ����
}

  
