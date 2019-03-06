#include "main.h"					    			  
//��ʿ��Ʋ���
//֧�ָ�ʽ[XX:XX]/[XX:XX:XX]/[XX:XX.X]
//ͬʱҲ֧�ֶ�����ŵĵĸ�ʸ�ʽ
//��[XX:XX.XX][XX:XX.XX]Ҳ��֧�ֵ�	 
//����ԭ��@HYW
//2009.04.30
//V1.1	 		

//���˼��:
//��һ��:��lrc�ļ���tagȫ�����Ƴ���,��ŵ�tag_table(���֧��426��tag,����Ĳ��ֽ��������)
//�ڶ���:�ѵõ���tag,��˳������.�õ���׼��ʱ��tag,������tag_table����
//������:�ٰ���tag��ʱ��˳��,����tag,ͬʱ��ȡlrc�ļ��ĸ��,����������������ʱ��,����tag����ʵ�
//       ��ʱ���ĵ�ַbuffer1,buffer2��.�����͵õ����������
//ע��:buffer1+buffer2�Ĵ�СΪ6144���ֽ�,�������ֽ���ʧ.
  u8 bt30MS=0;                       //30ms���˵ı�־
/*					  
u16 lrc_tag_cnt=0; //��¼tag������
u8  lrctagtype=0;  //��¼lrc������ 0:[XX:XX] 1:[XX:XX.XX]	  
//����ļ�����Ϣ��ŵ�ַ		   
FileInfoStruct *LrcFile=&F_Info[1];//�����ݴ�ռ�.	 
u8 *tag_table=(u8 *)MCUBuffer;     //LRCʱ����ݴ��ַ,���潫���ڴ��һ��LRC��� 

//LRC��ʵ���Ϣ		 
typedef struct{ 
	u8   first;    //��־����Ƿ������
	u16  t_sec;    //�ݴ��ʵ�ʱ��(��)
	u8   t_ms;     //����32MS��������			 			    
    u16  lrcoff;   //����ļ���ȡ�ĳ���
	u8   tagcnt;   //��ʽ������
}LYRIC_STRUCT;
LYRIC_STRUCT lyric; //�����Ϣ

///////////////////////////////////////////////////////////////
unsigned short	code_value_table[4][256];
		  
u8 WriteByteLrcBuf(u16 addr,u8 data);//д��һ���ֽ�����
u8 ReadByteLrcBuf(u16 addr);         //��ȡһ���ֽ�����
u8 Get_Byte(u8 rst);//��sd�õ�һ���ֽ�
u8 Get_Num(void);   //�õ�һ������
u8 Get_Tag(void); //�õ�tag�б�
void Init_Tag(void);//�Եõ���tag����
 
//��lrcbufer�����ȡһ���ֽ� 
//��������,����0xff     
//addr:��ַ
//����ֵ:����������
u8 ReadByteLrcBuf(u16 addr)
{						  
	u8 *buffer1=(u8 *)iclip;//�õ���һ��buf��ŵĵ�ַ
	u8 *buffer2=(u8 *)code_value_table;//�ڶ���buf��ַ
										    
	if(addr<4096)return buffer1[addr];
	else 
	{				 
		if(addr>6144)return 0xff;
		return buffer2[addr];//��������
	}
}


//��lrcbufer����дһ���ֽ�,��ַ�Զ�����
//������Χ,����0xff
//data:Ҫд�������  
//addr:��ַ
//����ֵ:������־
//���洢6Kbyte��С�ĸ��.
u8 WriteByteLrcBuf(u16 addr,u8 data)
{						  
	u8 *buffer1=(u8 *)iclip;//�õ���һ��buf��ŵĵ�ַ
	u8 *buffer2=(u8 *)code_value_table;//�ڶ���buf��ַ	  							  
	if(addr<4096)buffer1[addr]=data; //д�����ݵ�buffer1
	else 
	{			 
		if(addr>6144)return 0xff;
		buffer2[addr-4097]=data;  //д�����ݵ�buffer2
	}
	return 1;
}

//����һ����
//tmin,tsec,tmms:Ҫ���Ƶĸ��tag
//����ֵ:0xff,����.1,���Ƴɹ�	  
u8 CopyString(u8 tmin,u8 tsec,u8 tmms)
{
	static u16 tagpos=0;   //���tag��־
	static u16 lrcbufpos=0;//��ʻ����ַ
	u8 temp;
	u8 temp1;
	u16 tagcnt=0;

	if(tmin==0xff){tagpos=0;lrcbufpos=0;return 1;}//��λ
	if(lrctagtype)tagcnt=(lrc_tag_cnt-1)*3;
	else tagcnt=(lrc_tag_cnt-1)*2;	
	if(tagpos>tagcnt)//��ȡ������
	{
		WriteByteLrcBuf(lrcbufpos,'\0');//д�������
		return 0xff;
	}	 
	if(tmin==tag_table[tagpos]&&tsec==tag_table[tagpos+1]&&lrctagtype==0)//[XX:XX]���͸��
	{	  
		tagpos+=2;
	}else if(tmin==tag_table[tagpos]&&tsec==tag_table[tagpos+1]&&tmms==tag_table[tagpos+2]&&lrctagtype==1) //����2
	{
		tagpos+=3;
	}else return 1;
	WriteByteLrcBuf(lrcbufpos++,'['); //дһ���ֽ�
	WriteByteLrcBuf(lrcbufpos++,tmin);//д�����
	WriteByteLrcBuf(lrcbufpos++,':'); 
	WriteByteLrcBuf(lrcbufpos++,tsec);//д������
	if(lrctagtype)//XX:XX.XX��ʽ
	{
		WriteByteLrcBuf(lrcbufpos++,'.'); 
		WriteByteLrcBuf(lrcbufpos++,tmms);//д������
	}
	WriteByteLrcBuf(lrcbufpos++,']');     //д�����Ľ�����־
	 
	while(1)//һֱ�ȵ��õ�tag������־
	{
		temp=Get_Byte(0);
		if(temp=='[') //������ζ����һ��tag�ĵ���
		{
			WriteByteLrcBuf(lrcbufpos,temp);//�ȼ�¼����,�Է�����
			temp1=Get_Byte(0);
			temp=temp1;//����
			temp1-='0';
			if(temp1<10)//���������,�϶�����һ��tag��
			{
				while(1)
				{
					temp1=Get_Byte(0);
					if(temp1==']'||temp1==0xff)break;//����tagĩβ
				}
			}else  //����tag,������
			{
				lrcbufpos++;
				WriteByteLrcBuf(lrcbufpos++,temp);//��¼����	  
				break;//������Ч������
			}
		}else if(temp!=' ')	//���Կո�
		{
			if(temp!=0XFF&&temp!=0X00)WriteByteLrcBuf(lrcbufpos++,temp);//��¼���� 
			break;//��������Ч����
		} 							   
	}						 
	while(1)//��ʼ�����ı�
	{
		temp=Get_Byte(0);//�õ�һ���ַ�
		if(temp!=0XFF&&temp!=0X00)WriteByteLrcBuf(lrcbufpos++,temp);//���ƺϷ��ַ�      
		if(temp==0X0A||temp==0XFF||temp==0X00)//�����ȡ����
		{
			WriteByteLrcBuf(lrcbufpos++,temp);
			break;
		}	    
	}
	return 1;
}   

//����LRC��ʵ�ram����
void CopyLrcToRam(void)
{					    		 
	u8 numx;
	u8 temp;
	u8 min;
	u8 sec;
	u8 mms;		
					    
	CopyString(0xff,0,0);//��λ
	Get_Byte(1);//��λ 
	while(1)
	{
		while(1)//һֱ�ȵ��õ�tag��ʼ��־
		{
			temp=Get_Byte(0);	 
			if(temp=='[')break;
			else if(temp==0xff) //�������ݶ�ȡ������
			{
				Get_Byte(1);//��λ ���¶�
			}			   
		}
		numx=Get_Num();//�õ���һ������	  
		if(Get_Byte(0)!=':'||numx==0xff)continue;
		min=numx;
		numx=Get_Num();//�õ��ڶ�������
		temp=Get_Byte(0);
		if(temp!='.'&&temp!=']'||numx==0xff)continue;
		sec=numx;	    
		if(temp!='.')  //[XX:XX]��ʽ
		{				     
			if(CopyString(min,sec,0)==0XFF)return ;//�������	 		    
			continue;
		}			   
		numx=Get_Num();//�õ�����������
		temp=Get_Byte(0);
		if(temp!=']'||numx==0xff)continue;//[XX:XX.XX]��ʽ
		mms=numx;
		if(CopyString(min,sec,mms)==0XFF)return ;//�������	 
	}
}

	
//��ʴ�ŵ�ַ,����JPEG���벿�������RAM	   
u8 *LRC_DATA=(u8 *)QtZzMCUBuffer; 

//�Ӹ���ļ��еõ�һ��LRC�ַ�
//��������0XFF
//rst:1,��λ,��ͷ��ʼ����
//    2,ƫ�Ƶ���һ����ַ
u8 Get_Byte(u8 rst)
{		 
	static u16 bytepos;
	u8 temp;					  
	if(rst)//��λ,����ȡ����
	{
		if(rst==1)//��λ
		{
			F_Open(LrcFile);//��LRC�ļ�
			F_Read(LrcFile,LRC_DATA);//����512���ֽ�	  
			bytepos=0;	  
		}
		else bytepos--;//ƫ�Ƶ���һ�� �Ը�[XX:XX.X]��ʽ�ĸ��
		return 0xff;
	}   
	if(bytepos>511)//����һ��������,����һ������
	{
		temp=F_Read(LrcFile,LRC_DATA);//����512���ֽ�	 
		bytepos=0; 
		if(temp==0)return 0xff;//��������	   
	} 
    return LRC_DATA[bytepos++];
}  
 
//�õ�һ������
//ʧ�ܷ���0xff
u8 Get_Num(void)
{
	u8 t1=0;
	u8 t2=0;
	t1=Get_Byte(0);//�õ���һλ����	 ʮλ
	t2=Get_Byte(0);//�õ��ڶ�λ����  ��λ
	if(t1<'9'&&t1>'0'&&t2==']')//����[XX:XX.X]���ָ�ʽ
	{
		Get_Byte(2);//ƫ�Ƶ���һ����ַ  �����޷����']'
		return (t1-'0')*10;
	}	 
	t1-='0';//�õ�ASCII��ֵ
	t2-='0';
	if(t1<10&&t2<10)//�õ��Ϸ�����
	{
		return t1*10+t2;
	}
	return 0XFF;
}	  
//�õ�����ļ�������ʱ��� tag
//����ֵ��1,�ɹ���ȡ��tag��0����ȡtagʧ��
u8 Get_Tag(void)
{	
	u16 i=0;														    
			 
	u8 numx;
	u8 temp;
	u8 min;
	u8 sec;
	u8 mms;			   
    lrc_tag_cnt=0;   //tag�����
    lrctagtype=0;    //�������	  
	Get_Byte(1);     //��λ 
	while(1)
	{
		if(lrc_tag_cnt>426)break;//���������洢�ռ�,�������ڱ���
		while(1)//һֱ�ȵ��õ�tag��ʼ��־
		{
			temp=Get_Byte(0);  	   		 
			if(temp=='[')break;
			else if(temp==0xff)
			{
				if(lrc_tag_cnt)return 1;//�������ݶ�ȡ�����ˣ�ƫ���ֽ��ݶ�Ϊ20	
				else return 0;//��ȡtagʧ��	�Ƿ���lrc�ļ�	 
			}
		}
		numx=Get_Num();//�õ���һ������	  
		if(Get_Byte(0)!=':'||numx==0xff)continue;
		min=numx;
		numx=Get_Num();//�õ��ڶ�������
		temp=Get_Byte(0);
		if(temp!='.'&&temp!=']'||numx==0xff)continue;
		sec=numx;		  					    
		if(temp!='.')//[XX:XX]��ʽ
		{
			tag_table[i]=min;
			tag_table[i+1]=sec;
			i+=2;
			lrctagtype=0; //xx:xx��ʽ
			lrc_tag_cnt++;//��¼tag��
			continue;
		}
		numx=Get_Num();//�õ�����������
		temp=Get_Byte(0);
		if(temp!=']'||numx==0xff)continue;//[XX:XX.XX]��ʽ
		mms=numx;
		lrctagtype=1;  //xx:xx.xx��ʽ
		lrc_tag_cnt++; //��¼tag��
		tag_table[i]=min;
		tag_table[i+1]=sec;
		tag_table[i+2]=mms;
		i+=3;									 
	}
	return 1;
}
 
//��ʼ������tag
//����������    
void Init_Tag(void)
{
	u16 t=0;		 
	u8 t1;
	u8 temp;
	u16 tagcnt=0;
	u8 offset;

	if(lrctagtype)//XX:XX.XX
	{
		tagcnt=(lrc_tag_cnt-1)*3;
		offset=3; //XX:XX 
	}else 	 
	{
		tagcnt=(lrc_tag_cnt-1)*2;
		offset=2;
	}	  							 
	do//������X��������
	{	
		t1=0;		  
		for(t=0;t<tagcnt;t+=offset)
		{
			if(tag_table[t]>tag_table[t+offset])//���������ں���,��������
			{
				temp=tag_table[t+offset];		  //���ӵ���
				tag_table[t+offset]=tag_table[t];
				tag_table[t]=temp;

				temp=tag_table[t+offset+1];		  //���ӵ���
				tag_table[t+offset+1]=tag_table[t+1];
				tag_table[t+1]=temp;
				if(offset==3)
				{
					temp=tag_table[t+5];		  //mm����
					tag_table[t+5]=tag_table[t+2];
					tag_table[t+2]=temp;   
				}	   
				t1=1; 	//�иı�
			}else if(tag_table[t+1]>tag_table[t+offset+1]&&tag_table[t]==tag_table[t+offset])//���������ں���,��������λ
			{
				temp=tag_table[t+offset+1];		  //���ӵ���
				tag_table[t+offset+1]=tag_table[t+1];
				tag_table[t+1]=temp;

				if(offset==3)
				{
					temp=tag_table[t+5];		  //mm����
					tag_table[t+5]=tag_table[t+2];
					tag_table[t+2]=temp;   
				}	   
				t1=1; 	//�иı�
			}else if(offset==3)	//XX:XX.XX
			{
				if(tag_table[t+2]>tag_table[t+5]&&tag_table[t]==tag_table[t+3]&&tag_table[t+1]==tag_table[t+4])//mm�����ں���,��������λ
				{						 
					temp=tag_table[t+5];		  //mm����
					tag_table[t+5]=tag_table[t+2];
					tag_table[t+2]=temp;
					t1=1; 	//�иı�
				}
			} 
		}
	}while(t1);	
	for(t=0;t<=tagcnt;t+=offset)
	{
		if(offset==3)printf("[%d:%d.%d]\n",tag_table[t],tag_table[t+1],tag_table[t+2]);
		else printf("[%d:%d]\n",tag_table[t],tag_table[t+1]);
	}
}   
//�ڵ�ǰ��Ŀ¼�в��Ҹ���ļ�,�ҵ������������lyric��
//���: 1.dircluster    ��ǰĿ¼���״غ�,����FAT12��FAT16�ĸ�Ŀ¼��Ϊ0
//����ҵ�,�򷵻�1
//û�ҵ�,����0
u8 FindLrcFile(unsigned long dircluster,unsigned char *strName)
{							  
	unsigned char  name[8];       
	u16 lrc_cnt=0;//��ǰĿ¼��LRC�ļ��ĸ���
	u16 i;
	u8 t;							    
	FileInfoStruct *FileInfo;

	//printf("START SEARCH LRC:%s\n\n",strName);
	for(i=0;i<8;i++)name[i]=strName[i];//ȡǰ��˸��ֽ�   	 						   
	lrc_cnt=0;	  
	FileInfo=&F_Info[2];//����LRC�ݴ�ռ�.
	Get_File_Info(Cur_Dir_Cluster,FileInfo,T_LRC,&lrc_cnt);//��ȡ��ǰ�ļ��������lrc�ļ�����  	  
	for(i=1;i<=lrc_cnt;i++)
	{
		Get_File_Info(Cur_Dir_Cluster,FileInfo,T_LRC,&i);//�õ����LRC����Ϣ	 
        for(t=0;t<8;t++)if(name[t]!=FileInfo->F_ShortName[t])break;                    
        if(t==8) //�ҵ�����ļ���
		{	
			//printf("Find Matched LRC\n");									  
			Get_File_Info(Cur_Dir_Cluster,LrcFile,T_LRC,&i);//��ȡ��Ϣ��LrcFile	    
			
			//printf("LRC Name:%s\n",LrcFile->F_Name);			
			//printf("LRC Size:%d\n",LrcFile->F_Size);						  
			
			t=Get_Tag();  //�õ�tag
			if(t==0)break;//��ȡtagʧ����
			//printf("Get LRC Tag OK\n");	 
			Init_Tag();	   //����tag
			//printf("Init Tag OK!!\n");	 	
			CopyLrcToRam();//���Ƹ�ʵ�ram�
			return 1;  				 
		}
	}   
	return 0;
}  

//LRC��ȡ��ʼ��
void Lrc_Read_Init(void)
{
	lyric.t_sec=0;
	lyric.t_ms=0;    //ʱ����� 
	lyric.tagcnt=0;  //���û�н���
	lyric.lrcoff=0;  //ƫ�Ƹ�λ
	lyric.first=1;   //���һ��		  
}

//��ȡһ����Ч�ĸ�ʼ�¼  
//����ʾ
void ReadLrcFileData(void)
{  
	static u8 mutiline=0;
	unsigned char *lbuf=(u8 *)MCUBuffer;//һ���ʵı�������  
	unsigned char c;
	u8 cc;	
	u16 t;	 							     
				   
	if(lyric.tagcnt>=lrc_tag_cnt) return;//��ʶ�ȡ�Ѿ�������
	 
	lyric.t_sec=0xffff;
	lyric.t_ms=0; //ʱ����� 
	//�ӵ�ǰ��λ�ö�ȡ����ļ��е�64���ֽ���������lbuf					   
	t=0;
	if(!lyric.first)//��ʻ�û�и���,��ʾ���
	{  		    
		while(1)//��ȡһ���ʵ�lbuf����
		{
			c=ReadByteLrcBuf(lyric.lrcoff++);//ȡ��һ���ֽ�    
			if(c=='[')
			{
				cc=ReadByteLrcBuf(lyric.lrcoff);//��̽��һ������
				if(cc<=9){lyric.lrcoff--;break;}//��̽�ɹ�	
			}
			if(c==0x0d||c==0x0a||c=='\0')break;//�����س�����/��������    
			lbuf[t]=c;//��¼
			t++;         
		}	    
	   	lbuf[t]='\0';//��ӽ����� 
		//POINT_COLOR=LRCCOLOR;
		if(mutiline)DrawBlock(0,154,239,219,BLACK);//�������и��
		else DrawBlock(0,154,239,170,BLACK);//ֻҪ����һ�о�ok��
		
		Show_Str_Mid(0,154,lbuf,WHITE,LRCCOLOR,0X03,240);//��ʾ���,�Զ�����
		if(t>28)mutiline=1;//Ҫ������ʾ 	   
		else mutiline=0;   //��Ҫ��������ʾ	  	  			   
	}        
	//��ȡ��һ����
	lyric.first=0;//��Ǹ�ʸ���      	    
	while(1)//�����Ϣ��ȡ��Ϣ
	{ 								  
		c=ReadByteLrcBuf(lyric.lrcoff++);//��ȡTAG�ĵ�һ���ֽ�    
		if(c=='[')//��ȡһ��TAG 
		{
			lyric.tagcnt++;//tag���
			lyric.t_sec=ReadByteLrcBuf(lyric.lrcoff++);//��ȡ����
			lyric.t_sec*=60;
			lyric.lrcoff++;//ƫ�Ƶ�����
			lyric.t_sec+=ReadByteLrcBuf(lyric.lrcoff++);//��ȡ���� 
			c=ReadByteLrcBuf(lyric.lrcoff++);
			if(c==']')break;//XX:XX��ʽ	    
			//XX:XX.XX ��ʽ	  
			lyric.t_ms=ReadByteLrcBuf(lyric.lrcoff++);  //��ȡmm 	  
		}else if(c==']')break;	 
	} 
}             
//check ok 08/09/06
// �����ʾ����  
//ͬʱ������ʾ����ʱ��   
//mnum ��ǰ�ļ������ 0~3
//ע��:ֻ֧�ֵ���ʱ���lrc������ʾ
void LyricDisplayCtrl(void)
{  
	static unsigned int time=0;//time=0xffff;
	static unsigned char ms=0; //������ǽ����˶��ٸ�10ms    
	unsigned int t1;  
	if(!bt30MS)  return;//30msδ��
	ms+=3;//ms++
	bt30MS=0;   
    t1=GetDecodeTime(); //��ȡ����ʱ��  
	if(t1!=time)//���Ӳ���
	{
		time=t1;
		ms=0;//ͬ��ʱ��   
		return;
	}  											  
	if(((t1==lyric.t_sec)&&(ms>=lyric.t_ms))||(t1>lyric.t_sec))//ʱ�����,��ʾ���
	{    
		ReadLrcFileData();  //��ȡ�������ݲ���ʾ
	}   
} */

	 
//��ʱ��3�жϷ������
//���30ms�ж�ʱ�䵽.
void TIM3_IRQHandler(void)
{ 					 		    
	if(TIM3->SR&0X0001)bt30MS=1;//����ж�	    		   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	 
}
//TIMER3�ж����ȼ��趨
//��mp3�������ȼ����						  	   
void NVIC_TIMER3Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//�洢��ӳ��,������    
#ifdef  VECT_TAB_RAM  									   
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else   							 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);		 
	//��0��,û���������ȼ�
	//��4��,û����Ӧ���ȼ�				  
	//��2��,��4������,4����Ӧ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ��ֵ���2�� �ܹ�5��		 
  	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel; //ʹ���ⲿ�ж�1
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//�׼�1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 //�ײ�0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure); 
	//����TIM3���ж� 												  		 		   	 
  	NVIC_Init(&NVIC_InitStructure);        
#endif  				  					   							 
}   
//��ʼ������������ŵ�����
//�����ж�,��ӳ�� 
void timer3_init(void)
{	
	//�˲������ֶ��޸�IO������
	RCC->APB1ENR|=1<<1;//TIM3ʱ��ʹ��    
  	NVIC_TIMER3Configuration();										 
	TIM3->ARR=300;   //�趨�������Զ���װֵ//�պ�30ms 
	TIM3->PSC=7199;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	//����������Ҫͬʱ���òſ���ʹ���ж�
	TIM3->DIER|=1<<0;   //��������ж�
	TIM3->DIER|=1<<6;   //�������ж�

	TIM3->CR1=0x0080;  //ARPEʹ�� 
	TIM3->CR1|=0x01;   //ʹ�ܶ�ʱ��3					 					  										  
}
































