//MP3 ���ų��� MP3����
//QiYuan@SCUT
//2010/07/09
#include "main.h"  
extern u8 VS_VOLT;
extern u8 VS_BASS;
extern u8 M_Type;
u8	MCUBuffer[10*64];  
//short	QtZzMCUBuffer[10*64];
//long iclip[1024];//4k BYTES
u8 voltemp[6]={50,7,7,0,0,0};//�����Ĵ���,��һ��ˢ��ʱ��Ĭ������ 
//u8 INITDSP=0;  //0~4 eeprom���� ������Ч  
u16 f_kbps=0;//�����ļ�λ��	
//������,ʱ��,������ ����ʵ��Ϣ��ʾ����
void MP3_PROG(u32 pos,u32 lenth)
{	
	static u16 playtime=0;//����ʱ����	     
	float propos=0;	
	u16 time=0;// ʱ�����
	u16 temp=0;
	u8 t=0;
	if(f_kbps==0xffff)//δ���¹�
	{
		playtime=0;
		f_kbps=GetHeadInfo();	   //��ñ�����

		Show_mininum(6,93,f_kbps/10);//��ʾ����λ
		show_minichar(6,105,f_kbps%10);//��ʾ���λ  
		 
		time=(lenth/f_kbps)/125;//��ʱ��
		//��ʾ��ʱ��
		if(f_kbps)time=(lenth/f_kbps)/125;//�õ�������   (�ļ�����(�ֽ�)/(1000/8)/������=����������    	  
		else time=0;//�Ƿ�λ��

		show_minichar(7,50,11); 
        Show_mininum(7,56,time/60);//��ʾ����
        show_minichar(7,68,10);
        Show_mininum(7,74,time%60);//��ʾ����  
	}
	if(lenth)
	{
		propos=(float)pos*512/lenth;//�õ��ٷֱ� 	
		if(propos>1)propos=1;//����propos�ķ�Χ
	}else return;//�ļ�Ϊ��,����    
  	if(f_kbps)//��λ�ʲ�Ϊ0 ���ļ�
	{
		time=(lenth/f_kbps)/125;//��ʱ��
		time=propos*time;       //�õ���ǰ����ʱ��
	}else time=GetDecodeTime(); //��λ��Ϊ0���ļ�,�õ�����ʱ��
 	
	propos=propos*80;//�õ�ʵ�ʳ���
	if(propos>80) return;//Խ�紦��
	Set_page(6);//��λ��(ҳ0~7)
    Set_column(12);//��λ��    
    for(t=0;t<propos;t++)Write_data(0xFC);
		  
	if(time!=playtime)//1sʱ�䵽,������ʾ����
	{	  
		temp=GetHeadInfo(); //��ñ�����   				 
		if(temp)
		{
			f_kbps=temp;//����KBPS	
			time=(((float)lenth/f_kbps)/125)*(propos/80);//����ʱ��	 
		}	
		playtime=time;//����ʱ�� 	 
		//��ʾ����ʱ��			 
		//LCD_ShowNum(30,70,time/60,2,MP3COLOR4,GREEN,0); //����	   
		//LCD_ShowNum(54,70,time%60,2,MP3COLOR4,GREEN,0);//���� 
		//��ʾ��ǰ����ʱ��
	    Show_mininum(7,20,time/60);//��ʾ����
        show_minichar(7,32,10);//��ʾð��
        Show_mininum(7,38,time%60);//��ʾ����  
	}   		 
}  
 
 /*
//check ok 08/09/06
// �����ʾ����  
//ͬʱ������ʾ����ʱ��   
//mnum ��ǰ�ļ������ 0~3
//ע��:ֻ֧�ֵ���ʱ���lrc������ʾ
void LyricDisplayCtrl(uchar mnum)
{  
	static unsigned int time=0xffff;
	static unsigned char ms=0;  
	unsigned long lenth=0; 
	unsigned int t1;  
	if(!bt30MS)  return;//30msδ��
	ms+=3;//ms++
	bt30MS=0;  
	t1=GetDecodeTime(); //��ȡ����ʱ��  
	if(t1!=time)//���Ӳ���
	{
		time=t1;ms=0;//ͬ��ʱ��  
		timer=time;  //�޸�ͬ��ʱ�������(����Ǹ�ʵ�׼ȷ��ʱ��,������ʾ��ʱ��!) 
		             //���������˵Ļ�,���Ҳ����,���Կ������ʾ���˸��
		//�õ�������
		t1=GetHeadInfo();  
        //��ʾ��������             
        if(t1!=file_bps&&t1>0)//ֻ�е�kbps�仯ʱ��ִ��,���ϵͳ�ٶ�
        {              
            //��ʾ������Kbps
		    Show_mininum(6,93,t1/10);//��ʾ����λ
		    show_minichar(6,105,t1%10);//��ʾ���λ   
            file_bps=t1;//����˴���Ϣ����;
            //printf("BIT Rate is:%d\n",file_bps); 
            lenth=m_c[mnum].FileLen/125;//�õ��ļ�����xxxx Kbps
            t1=lenth/t1;
            show_minichar(7,50,11); 
            Show_mininum(7,56,t1/60);//��ʾ����
            show_minichar(7,68,10);
            Show_mininum(7,74,t1%60);//��ʾ����  
		}
		return;
	}
	if(!ENLRC||!LYRIC)return;//�޸�ʻ��߸�ʱ���ֹʹ��  
	if(((t1==lyric.t_sec)&&(ms>=lyric.t_ms))||(t1>lyric.t_sec))
	{    
		ReadLrcFileData();  //��ȡ�������ݲ���ʾ
	}   
}   	 */ 


//��ʾ��������
//num:�������           
void music_name(FileInfoStruct *FileName)
{
    //u8 i;
    /*while(m_c[mnum].LongName[i]!='\0')//�Ѷ����ַ��м���ʾ
    {
        i++;
        if(i>21){i=22;break;}
    }     */             
    //i=66-3*i;//��13164Һ������
    LCD_write_String(1,0,FileName->F_Name,2);//�޸��/�����ʾ��ʹ��    
    
}

							  	 
//����ѡ���ĸ��� 					  
//index:��ǰ���ŵĸ�������
//total:�ܹ������ļ��ĸ���
//�������ֵļ������п��ƶ����������
u16 Mus_Play(FileInfoStruct *FileName,u16 index,u16 total)
{	 
    unsigned long bfactor=0; //��������,���Ե�ַ
    unsigned long bcluster=0;//����غ�
	u8 pause=1; 
	u32 file_pos=0;          //��¼�ļ����ŵĳ��� 
    u8 count=0; 			 //����������,���ڻ���
    u16 i,n;
	u8 keycnt;	 //����������
	u8 FFT_LRC=0;//Ĭ��FFT

 	u8 *BUFFER=(u8*)MCUBuffer;//ָ��JPEG�Ŀ�ʼ512�ֽ�
	u8 *spedate=(u8*)MCUBuffer+512;//��ǰƵ�׷����Ľ���Ĵ���
  	u8 *topval=(u8*)MCUBuffer+526; //FFT������ߵ��¼��
	u8 *flyval=(u8*)MCUBuffer+540; //FFT�����������¼��
			  	   
	u8 mark=1;		//���
	u8 timer_temp=0;		//Ƶ�׶�ʱ��,Ϊ�˲���ȫ��ͬ������˸ĳ�temp
	u32 topstay1;   //��ߵ�Ƶ����ʱ1
	u32 topstay2;	//��ߵ�Ƶ����ʱ2
	//bit0~9������ǹ��� ,Ĭ��ȫ��ѭ��
	//bit0,1:ѭ��ģʽ;bit2,3,4:��������;bit5,6:��������
	//bit7:���ص������;bit8:�Ƿ��и��;bit9:��Ч��/ȡ���
	u16 sysfun=1;
	u16 temp;		  
	u8 forpreclust;//���������Ҫ�Ĵ���   								 
	timer3_init();//��ʼ�������ʾ��ʱ��  	 
	if(!voltemp[5])sysfun|=1<<7;   
RESTART: 			 		  
	forpreclust=FileName->F_Size/(cluster_size*sector_size*50);//ÿ��ǰ��/���� 2%����Ҫ�Ĵ���	   
	if(forpreclust<2)forpreclust=2;//����ҲҪ������
	//fft����,�������
	topstay1=0;
	topstay1=topstay1;
	topstay2=0;
	topstay2=topstay2;
	file_pos=0;	  
	keycnt=0;
	keycnt=keycnt;
	f_kbps=0xffff;//�ļ�λ������Ϊ������ֵ
	count=0;	  //����������	  	  
    bfactor=fatClustToSect(FileName->F_StartCluster);//�õ���ʼ�ض�Ӧ������
	bcluster=FileName->F_StartCluster;				 //�õ��ļ���ʼ�غ�	 
	//�ӵ�ǰĿ¼���Ҹ���ļ�,��,����.	 
	/*if(FindLrcFile(Cur_Dir_Cluster,FileName->F_ShortName))
	{							   
		sysfun|=1<<8;     //����и��
		Lrc_Read_Init();  //��ʼ����ʶ�ȡ
	}else sysfun&=~(1<<8);//���û�и��*/	 		  
	Mp3Reset();       //Ӳ��λ
	Vs1003SoftReset();//��λVS1003
	LoadPatch();      //����Ƶ�׷������� 
 	//Load_Gui_Mp3(sysfun);//���ز��Ž���
	Play_GUI();
	MP3_msg(pause); 
	//Show_Str_Mid(0,24,FileName->F_Name,MP3COLOR4,WHITE,0x01,240); //��ʾ��������
	music_name(FileName);  //��ʾ����
	timer_temp=5;//���õ�һ�������ܵ�ֵ,��ʼ����Ϣ��
	timer_temp=timer_temp;						 	
	//FFT�������
	for(i=0;i<14;i++)//����������
	{
		topval[i]=0;		 	    
		flyval[i]=0;
		spedate[i]=0;
	}								  
	key=255;		 	    	
	while(1)   //�������ֵ���ѭ��
    {
        if(SD_ReadSingleBlock(bfactor,BUFFER))break;//��ȡһ������������ 
        i=0;  	  
        count++;//���������� 
		do      //������ѭ��
        {  
			while(VS_DREQ==0&&((sysfun&0X0400)==0)) //�ȴ���/����Ƶ����ʾ/����ͣ/����û�а��� 
			{
				if(FileName->F_Type!=T_MID)//����MID��ʽ�ģ�����
				{
					if(FFT_LRC==0)//Ƶ��ģʽ,���ڷ�MID��ʽ��Ч
					{
						if(mark)
						{
							GetSpec(spedate);//�õ�Ƶ������
							MP3_PROG(file_pos,FileName->F_Size);//MP3������ʾ/����ʱ��  
						}
						//else LCD_ShowFFT(spedate,topval,flyval,&topstay1,&topstay2);//Ƶ����ʾ     	  
						/*timer_temp++; 
						if(timer_temp>=3)//FFT��ߵ���������� 
						{
							Top_Delet_FFT(flyval,&topstay1,&topstay2);//ɾ���߶�
							timer_temp=0; 
						}*/   
					}else//���ģʽ
					{					   											    
						if(mark&&sysfun&(1<<8))	
						{
						 	//LyricDisplayCtrl();//���ڸ��,����ʾ
						}
						else if(mark==0)MP3_PROG(file_pos,FileName->F_Size);//MP3������ʾ/����ʱ�� 				  
					}	
					mark=!mark;
				}else//MID��ʽ�ļ� 
				{	  
					if(NPEN)break;
					MP3_PROG(file_pos,FileName->F_Size);//MID������ʾ/����ʱ�� 	
				}	
			}
			if(VS_DREQ!=0&&((sysfun&0X0400)==0))  //����ͣ,�����ݸ�VS1003
			{			   	 
	            for(n=0;n<32;n++)Vs1003_DATA_Write(BUFFER[i++]);//������������    
			} 							
			if(key!=255)//�а�������
			{	 
				switch(key)
				{

					case UP://��һ��
						index--;
						if(index==0)index=total;
						Get_File_Info(Cur_Dir_Cluster,FileName,T_MP3|T_WMA|T_WAV|T_MID,&index);
						goto RESTART;
					case DOWN://��һ��
						index++;
						if(index>total)index=1;
						Get_File_Info(Cur_Dir_Cluster,FileName,T_MP3|T_WMA|T_WAV|T_MID,&index);
						goto RESTART;
					case KEY_C://������ͣ
						temp=(sysfun>>10)&0x01;
						temp=!temp;	  	   
						if(temp)
						{
						   	pause=0;
							Show_icon(3,0,12+pause);//��ʾ������ͣ //����
						}
						else
						{
						   	pause=1;
							Show_icon(3,0,12+pause);//��ʾ������ͣ //����	
						}						    
						sysfun&=0XFBFF;
						sysfun|=temp<<10;
						break;
					case KEY_B://�˳�
					   	Vs1003SoftReset();//��λ
						return 1; 

					case LEFT://������
						if(VS_VOLT<=4)VS_VOLT=0;
						else VS_VOLT=VS_VOLT-4;
						Show_num2(0,12,VS_VOLT/4);//��ʾ����
						VS_SET_VOL(VS_VOLT,VS_BASS);
						break;
					case RIGHT://������	
						if(VS_VOLT>=251)VS_VOLT=255;
						else VS_VOLT=VS_VOLT+4;
						Show_num2(0,12,VS_VOLT/4);//��ʾ����
						VS_SET_VOL(VS_VOLT,VS_BASS);
						break;
					 case KEY_D://���ص���//Supper Bass ����
					 		 
						if(VS_BASS==0)
						{
							VS_BASS=255;
							VS_SET_VOL(VS_VOLT,VS_BASS);
							Show_Bass(1);//��ʾBass
						}
						else 
						{
							VS_BASS=0;
							VS_SET_VOL(VS_VOLT,VS_BASS);
							Show_Bass(0);//����ʾBass
						}
						break;
				}  
				key=255;	  						  
			} 		           
        }while(i<511);//ѭ������512���ֽ� 
		file_pos++;//����512���ֽ�	  
        bfactor++; //������

        //prgpos+=512;//����������   
        //Pro_msg(prgpos,mnum);//��ʾ������    
		if(file_pos>(FileName->F_Size/512))break;//�ļ����ȵ���  �����ˣ�����
        if(count>=cluster_size)//һ���ؽ���,����
        {
            count=0;  		   
            bcluster=FAT_NextCluster(bcluster);	   
			//�ļ����� 
			if(((fat_type==16)&&bcluster==0xffff)||bcluster==0x0ffffff8||bcluster==0x0fffffff)break;//error
			bfactor=fatClustToSect(bcluster);
	    } 
    } 						  
	//init_pt2314(NONE_Audio);//��������  			   
	Mp3Reset();       //Ӳ��λ
	Vs1003SoftReset();//��λVS1003

	temp=sysfun&0x03;//0/1				 
	if(temp==1)//ȫ��ѭ��
	{
		index++; 						   
    	if(index>total)index=1;
		key=0xffff;	
		Get_File_Info(Cur_Dir_Cluster,FileName,T_MP3|T_WMA|T_WAV|T_MID,&index);			  
	}								   
	goto RESTART;//���»ص����ﲥ��,����/ȫ��ѭ��	 
}

//���������
//֧��FAT32/16�ļ�ϵͳ�����.
//check:09/04/18
void Mus_Viewer(void)
{		      
	FileInfoStruct *FileInfo;		 
	u16 mus_cnt=0;//��ǰĿ¼�������ļ��ĸ���
	u16 index=0;  //��ǰѡ����ļ����	 
	Cleardisplay(ALL);   //����
	//�Ӹ�Ŀ¼��ʼ���
	if(fat_type==32)Cur_Dir_Cluster=root_dir_clu;//��Ŀ¼�غ�
 	else Cur_Dir_Cluster=0;						   
	Fat_Dir_Cluster=Cur_Dir_Cluster;//��Ŀ¼���ǵ�ǰĿ¼
	while(1)
	{
BROS:	 	     
		index=File_Browse(T_MP3|T_WMA|T_WAV|T_MID);//����ļ���    
		if(!index){State=1;return;}//�������,�˳��˹���!!!!!
		mus_cnt=0;	 
		Get_File_Info(Cur_Dir_Cluster,FileInfo,T_MP3|T_WMA|T_WAV|T_MID,&mus_cnt);//��ȡ��ǰ�ļ��������Ŀ���ļ����� 
		FileInfo=&F_Info[0];//�����ݴ�ռ�.
		while(1)
		{
			Get_File_Info(Cur_Dir_Cluster,FileInfo,T_MP3|T_WMA|T_WAV|T_MID,&index);//�õ����׸�������Ϣ	 
			Cleardisplay(ALL); //��13164��ʾ  
			Mus_Play(FileInfo,index,mus_cnt);			   	   
			goto BROS;	   
		}
	}			 
}



















