//MP3 ���ų��� GUI����
//QiYuan@SCUT
//2010/07/09
#include "main.h"

extern u8 invt;
//���˵�
void Load_Sys_ICO()
{
	Cleardisplay(ALL);
	if(key_point<4)
	{
		Show_str(0,0,"1.MP3 Player");		//��ʾfat���� 
	   	Show_str(1,0,"2.TXT Reader");		//��ʾfat����
		Show_str(2,0,"3.Show Time");		//��ʾfat����
		Show_str(3,0,"4.Show FFT");			//��ʾfat����
		Show_str(key_point,104,"<-");			//��ʾfat����	
	}
	else if(key_point>=4&&key_point<8)
	{
		LCD_write_String(0,0,"5.̰ʳ��",0);		//��ʾfat���� 
		LCD_write_String(1,0,"7.������",0);		//��ʾfat���� 
		LCD_write_String(2,0,"6.����˹����",0);		//��ʾfat���� 
		LCD_write_String(3,0,"8.ϵͳ��Ϣ",0);		//��ʾfat���� 
		Show_str(key_point%4,104,"<-");			//��ʾfat����	
	}
}


 //����MP3���Ž���
//sysfun������0~9bit������ǲ�ͬ״̬
void Load_Gui_Mp3(u16 funmark)
{	 
}


//��ʾ�ļ�
//fcnt:Ҫ��ʾ�ļ��Ķ���
//sel :ѡ�еڼ���1~4 
//����ֵ:Ҫ�򿪵��ļ����1~8(�����ļ���,��FileInfo�����)	    
//����9,����ֱ���˻���һ��Ŀ¼  		    
//��ʾ�ļ�
//check ok 09/04/19
u8 Show_File(u8 fcnt,u8 sel)
{
	u8 t;
	unsigned char Ico=0;					   
	//FileInfoStruct temp;
	//temp.F_Type=T_BMP;//�ļ���������

	sel-=1;										 
	for(t=0;t<fcnt;t++)
	{
		if(F_Info[t].F_Type==T_FILE) Ico=4;//�ļ���ͼ��
		else 
		{
			if(F_Info[t].F_Type&0x0F)Ico=0;//������ͼ��
			else if(F_Info[t].F_Type&0xF0)Ico=1;//TXT��ͼ��
		}
		if(t==sel) invt=1;
		else invt=0;
		Show_icon(t,0,Ico);//��ʾͼ��	 
		LCD_write_String(t,12,F_Info[t].F_Name,0);//������      
	}
	invt=0;
	while(1)//�ȴ���������
	{
		while(key==255);
		if(key!=255)							//�а�������  
		{
			switch(key)
			{
				case UP:	
							invt=0;Show_icon(sel,0,Ico);//��ʾͼ��
						   	if(sel==0)sel=3;	
							else sel--;
							invt=1;Show_icon(sel,0,Ico);//��ʾͼ��
							break;
				case DOWN:	
							invt=0;Show_icon(sel,0,Ico);//��ʾͼ��
							if(sel==3)sel=0;
							else sel++;
							invt=1;Show_icon(sel,0,Ico);//��ʾͼ��
							break;
				case KEY_A:{key=255;return sel+1;}//ȷ�ϼ�����
				case KEY_B://ѡ����"����"��ť 
					key=255;return 9;
				case LEFT:  //���Ϸ�ҳ���� 
					key=255;return 10;
				case RIGHT:  //���·�ҳ���� 
					key=255;return 11;
			}			  
		}
		key=255;//�Ѱ���ֵȡ����		
	}
}

//����ļ���������ļ�
//file_type:Ҫ������ļ�����
//����Ҫ�򿪵�Ŀ���ļ���� 1~n(����Ŀ���ļ������)
//�������ļ���!!!
//�������0������˳����
u16 File_Browse(u16 file_type)
{			 
	FileInfoStruct *FileInfo;
	u16 folder_cnt;//�ļ��еĸ���
	u16 des_cnt;   //Ŀ���ļ��ĸ���		    
	u8 fcnt;	   //��ʾ���ļ���Ŀ
	u16 temp; 					     									  

	u16 total_file;//���ļ���Ŀ
	u16 pos_cnt;//��ǰ�ļ��� 1~folder_cnt+des_cnt
	u8 temp_key;	  			  
							   			  
Reset:				    
	folder_cnt=0;des_cnt=0;fcnt=0;temp_key=0;pos_cnt=0;	 	
	Get_File_Info(Cur_Dir_Cluster,FileInfo,T_FILE,&folder_cnt);//�õ���ǰĿ¼���ļ��еĸ���	    
	Get_File_Info(Cur_Dir_Cluster,FileInfo,file_type,&des_cnt);//�õ���ǰĿ¼��Ŀ���ļ��ĸ���	 
									   
	total_file=des_cnt+folder_cnt;//�ܵ��ļ���Ŀ=�ļ�����Ŀ+Ŀ���ļ���Ŀ
	while(1)
	{
		if(pos_cnt<total_file)//�ļ����,δ�����ļ�ĩβ
		{	   
			FileInfo=&F_Info[fcnt];
			if(pos_cnt<folder_cnt)//���ļ���
			{
			    temp=pos_cnt+1;			    
				Get_File_Info(Cur_Dir_Cluster,FileInfo,T_FILE,&temp);
			}else				   //��Ŀ���ļ�
			{
				temp=pos_cnt-folder_cnt+1; 
				Get_File_Info(Cur_Dir_Cluster,FileInfo,file_type,&temp);
			}
			fcnt++;   //�Ѵ���ļ���Ŀ����
			pos_cnt++;//�ļ�λ�ñ�־����
		}	  
		if(fcnt==4||pos_cnt==total_file)//�����ĸ���,�����ļ����Ѿ�������
		{  
			temp_key=Show_File(fcnt,1);//��ʾ�˴εõ����ļ�
			switch(temp_key)
			{
				case 1://Ҫ��Ŀ���ļ�
				case 2:
				case 3:
				case 4:
					temp_key-=1;
					if(temp_key<fcnt)//��������Ч��Χ��
					{
						if(F_Info[temp_key].F_Type==T_FILE)//Ҫ�򿪵��Ǹ��ļ���
						{
							Cur_Dir_Cluster=F_Info[temp_key].F_StartCluster;//������ļ���		  
							goto Reset;
						}else    //Ҫ�򿪵���Ŀ���ļ�
						{		   
							return pos_cnt+temp_key-folder_cnt-fcnt+1;//����Ŀ���ļ����
						}
					}
					break;
				case 9:	//ѡ���˷��ذ�ť.�˳�����һ��Ŀ¼
					//����Ŀ¼�������,���·��ذ�ť,���˳����ģʽ	
					if(((fat_type==32)&&Cur_Dir_Cluster==root_dir_clu)||Cur_Dir_Cluster==0)return 0;//�˳����ģʽ	  					
					Cur_Dir_Cluster=Fat_Dir_Cluster;//�˵���Ŀ¼  
					Cleardisplay(ALL); //��13164��ʾ 			    	 
					goto Reset;	   	 
				case 10:  //���Ϸ�ҳ
					if(pos_cnt>4)pos_cnt-=(4+fcnt);//�ص���һҳ
					else pos_cnt=0;//��ͷ��ʼ��
					Cleardisplay(ALL); //��13164��ʾ 
					break;
				case 11:	//���·�ҳ
					if(pos_cnt==total_file)pos_cnt-=fcnt;//������
					Cleardisplay(ALL); //��13164��ʾ 
					break;

			}	 				    
			fcnt=0;//�����ʾ�ļ�������
		}
	}		
}

