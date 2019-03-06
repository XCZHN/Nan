//FAT�ļ�ϵͳ����
//2010/06/18 revise
//QiYuan
#include "main.h"

#define FSIZE 12
u32 file_cluster;
u32 file_size;
extern u8  SD_Type;//SD��������
DWORD root_dir_clu=2;    //��Ŀ¼�غ�
DWORD FirstDataSector=2;	//�������ĵ�һ������
WORD  sector_size=512;	//ÿ�����ֽ���
DWORD FATsectors=0;		//FAT����ռ������
BYTE   cluster_size=8;		//ÿ��������
DWORD fat_address=0;		//��һ��FAT��(FAT1)��������
DWORD root_address=0;		//��Ŀ¼��������
DWORD root_dir_size=0;	//fat16��Ŀ¼��ռ������
DWORD Root_Entries=0;		//��Ŀ¼��Ŀ¼���� fat32Ϊ0
BYTE  fat_type=32;			//fat����,Ĭ��fat32

DWORD Cur_Dir_Cluster=0;//��ǰĿ¼�غ�
DWORD Fat_Dir_Cluster=0;//��Ŀ¼�غ� ��FAT�ļ�������CopyDirentruyItem�������޸�!


//�ļ���Ϣ������		   		    															   
FileInfoStruct F_Info[8]={0};
//�ⲿȫ�ֱ���

//FAT ���ݻ�����,���ܺ�jpg_buffer����.������FAT32�ļ�ϵͳ����,���ܳ���!!
u8 fat_buffer[512]={0};
u8 LongNameBuffer[MAX_LONG_NAME_SIZE]={0};//���ļ����Ļ�����
BOOL LongNameFlag = 0;//�Ƿ���ڳ��ļ����ı�־

u32 FONT16CLUSTER=0;//16������״�
u32 FONT12CLUSTER=0;//12������״�

u32 sys_ico[9]={0};  //ϵͳͼ�껺����!���ܴ۸�!	 
u32 sys_ico1[9]={0};  //ϵͳͼ�껺����!���ܴ۸�!
u32 file_ico[8]={0}; //�ļ�ͼ�껺���� folder;mus;pic;book;//�ļ��ж���
u32 File_DIR[6]={0};	//��Ŀ¼�µĸ����ļ���0:SYSTEM 1:TXT 2:MP3 3:PIC 4:RECEIVE

const unsigned char *folder[12]=
{
"SYSTEM     ",
"FONT       ",
"SYSICO     ",
"GAME       ",
"LEVEL1     ",
"LEVEL2     ",
"LEVEL3     ",
"MP3        ",
"TXT        ",
"RECEIVE    ",
"SEND       ",
"PIC        "
};	

//ϵͳ�ļ�����
const unsigned char *sysfile[19]=
{			  
"FONT16  FON",
"FONT12  FON",
//ϵͳ������ͼ��
"MUSIC   BMP",
"PICTURE BMP",
"TXT     BMP",
"ALARM   BMP",
"TIME    BMP",
"SETTING BMP",  
"GAME    BMP",
"RADIO   BMP",
"NEXT    BMP",
//ϵͳ�ļ�ͼ��	12��ʼ
"FOLDER  BMP",
"MUS     BMP",
"PIC     BMP",
"BOOK    BMP",
"DRAW    BMP",
"CALCU   BMP",
"QQ1     BMP",
"QQ2     BMP"
};

//�ڶ�ҳͼ��
const unsigned char *sysfile1[9]=
{			  
//ϵͳ������ͼ��
"LIGHT   BMP",
"SETUP   BMP",
"QQ      BMP",
"JISUAN  BMP",  
"DAT     BMP",
"MESS    BMP",
"PICT    BMP",
"SEND    BMP",
"LAST    BMP",
};


u32 get_DW(u16 i)		 //�õ�buffer��ƫ��iΪ��ʼ��˫��
{
 	u32 temp = 0;
	temp += fat_buffer[i+3]<<24;	
	temp += fat_buffer[i+2]<<16;
	temp += fat_buffer[i+1]<<8;
	temp += fat_buffer[i];
	return temp;
}
//FAT��ʼ��������SD�ĳ�ʼ������֮ǰӦ�ȵ���sd�ĳ�ʼ��
u8 FAT_Init(void)//Initialize of FAT  need initialize SD first
{  		   
	DBR 		*DBR_Point  = 0;
	DBR_BPB     *BPB_Point = 0;			  
	u32 BPB_address=0;
	u32 cap=0;//����
	Cleardisplay(ALL);
	Show_str(0,0,"Insert SD card...");
	while(SD_Init());  //�ȴ�SD����ʼ����ȷ���,NO_ERRORʱ����Ϊ0   
    SD_CS_ENABLE();	
	Cleardisplay(ALL);    
	Show_str(0,0,"Initial OK !");
	if(SD_Type==SD_TYPE_V2HC)		//��ʾ�汾��
	{
		Show_str(1,0,"SD_Type : SDHC");	
	}
	else if(SD_Type==SD_TYPE_V1)
	{
		Show_str(1,0,"SD_Type : SD1.0");	
	}
	else
	{
	}
	 
	(SD_ReadSingleBlock(0,fat_buffer));//��MBR
	if(fat_buffer[0x1fe]!=0x55 || fat_buffer[0x1ff]!=0xaa)	//�ж�SD�������Ƿ����
	{
		return 1;
	}

	if(get_DW(0x1C6))  		BPB_address = get_DW(0x1C6); //�õ�������ʼ��ַ
	else if(get_DW(0x1D6))	BPB_address = get_DW(0x1D6); //�õ�������ʼ��ַ
	else if(get_DW(0x1E6))	BPB_address = get_DW(0x1E6); //�õ�������ʼ��ַ
	else if(get_DW(0x1F6))  BPB_address = get_DW(0x1F6); //�õ�������ʼ��ַ//�����������(ֻ�����ҵ�SDHC 4GB)

	SD_ReadSingleBlock(BPB_address, fat_buffer);//�õ�������Ϣ
	DBR_Point =  (DBR*)fat_buffer;
	BPB_Point = (DBR_BPB *)DBR_Point->bsBPB;
	if((get_DW(0x20)>0x800000))	//�������4G						
	{													
		cap=(u16)((get_DW(0x20)/(1024*1024))*512);	 	//��ȡ����
	}
	else  cap=(u16)((get_DW(0x20)*512)/(1024*1024));	//��ȡ����
	Show_num(2,0,cap);
	Show_str(2,32,"MB");
	if(fat_buffer[0x52]=='F'&&fat_buffer[0x53]=='A'&&fat_buffer[0x54]=='T'&&fat_buffer[0x55]=='3'&&fat_buffer[0x56]=='2')		
	{
		fat_type = 32;	//FAT32
		FATsectors	  = BPB_Point->bpbBigFATsecs;//һ��FAT��ռ�õ�������
		root_dir_clu = BPB_Point->bpbRootClust;//��Ŀ¼���ڴغ�
	}
	else 
	{
		fat_type=16;	//FAT16
		FATsectors	= BPB_Point->bpbFATsecs;  //һ��FAT��ռ�õ������� 
		root_dir_clu = 2; //��Ŀ¼��ʼ�غ�
	}
	Show_str(2,60,"FAT");		//��ʾfat���� 
	Show_num(2,84,fat_type);

	///////////////////////FAT��ʼ���ɹ���õ�����Ϣ////////////////////////////
	sector_size	= BPB_Point->bpbBytesPerSec;      //ÿ�����ֽ���
	cluster_size	= (BYTE)BPB_Point->bpbSecPerClust;//ÿ��������
	fat_address	= BPB_address+BPB_Point->bpbResSectors;//��һ��FAT������������=DBR����������+��������

	Root_Entries	= BPB_Point->bpbRootDirEnts;		//��Ŀ¼���� ��FAT32Ϊ0
	root_dir_size	= (Root_Entries*32)/512;		//��Ŀ¼ռ�õ����������������ҲΪ0
	root_address = fat_address+BPB_Point->bpbFATs*FATsectors;//��һ��Ŀ¼(��Ŀ¼)������=��һ��FAT������������+(FAT����)*(һ��FAT��ռ������)
	FirstDataSector	= root_address+root_dir_size;//��һ������������(����FAT32��˵���Ǹ�Ŀ¼����������)

	return 0; 
}     

//����һ�شغ�
//���뵱ǰ�غ�(2~n)��������һ�صĴغ�
//Return the cluster number of next cluster of file
//Suitable for system which has limited RAM
unsigned long FAT_NextCluster(unsigned long cluster)
{
	DWORD sector;
	DWORD offset;  

	if(fat_type==32)offset = cluster/128;//FAT32��FAT����,���ĸ��ֽڱ�ʾһ���ص�ַ.512/4=128
	else offset = cluster/256;			 //FAT16��FAT����,�������ֽڱ�ʾһ���ص�ַ.512/2=256
	if(cluster<2)return 0x0ffffff8;		 //��0,1�������ڴ��
	sector=fat_address+offset;//����ʵ��������
	if(SD_ReadSingleBlock(sector,fat_buffer))return 0x0ffffff8;//��ȡFAT��,��������ʱ����0x0ffffff8
	if(fat_type==32) //�����FAT32�ļ�ϵͳ
	{
		offset=cluster%128;//����λ��
		sector=((unsigned long *)fat_buffer)[offset];	
	}
	else //�����FAT16�ļ�ϵͳ
	{
		offset=cluster%256;//����λ��
		sector=((unsigned short *)fat_buffer)[offset];
	}
	return (unsigned long)sector;//return the cluste number
}

//��ǰһ�شغ�
//cluster:��ǰ�غ�
//startcluster:�ļ���ʼ�Ĵغ�
//����ֵ:clusterǰһ���صĴغ�
//����0xfffffff8�����
unsigned long FAT_PrevCluster(unsigned long cluster,unsigned long startcluster)
{
	DWORD temp;
	DWORD tempcluster;					  
	tempcluster=startcluster;//����ʼ�غſ�ʼ����
	if(cluster==startcluster)return 0x0ffffff8;//���Ҵ���
	while(1)
	{ 
		temp=FAT_NextCluster(tempcluster);  //���ҵ�ǰ�ص���һ��
		if(temp==cluster)return tempcluster;//��һ�ؾ͵������մ�,����ǰһ��,�������մص���һ��
		else tempcluster=temp;				//������һ�ز���
		if(temp==0x0ffffff8)return 0x0ffffff8;//���Ҵ���
	}
}

//���غ�ת��Ϊ������
u32 fatClustToSect(u32 cluster)
{
	return root_address+(DWORD)(cluster-2)*(DWORD)cluster_size;	 
}

//�ļ�����
//����ֵ:��Ӧ������   0,mp3;1,wma;2,wav,3,mid;4,lrc;5,txt;6,c;7,h;8,jpg;9,jpeg;10,bmp;11,file;12,FON;   
const unsigned char *filetype[13]={"MP3","WMA","WAV","MID","LRC","TXT","C  ","H  ","JPG","JPE","BMP","   ","FON"};
//������չ������
//����:exName �ļ���չ��
u16 FileType_Tell(u8 * exName)
{
    u8 i;
    u8 t;   
    for(i=0;i<13;i++)
    { 
        for(t=0;t<3;t++)if(exName[t]!=filetype[i][t])break; 
        if(t==3)break;	//���ҵ���Ӧ���ļ�����
    } 
    return 1<<i;//�����ļ�����????????????????what's the matter???????????
}  

//�����ļ���¼����Ϣ
void CopyDirentruyItem(FileInfoStruct *Desti,Short_DIR *Source)
{
	BYTE i;
	for(i=0;i<8;i++)Desti->F_ShortName[i]=Source->deName[i];//���ƶ��ļ���
	Desti->F_Type         = FileType_Tell(Source->deExtension);	//������չ��
	Desti->F_StartCluster = Source->deStartCluster + (((unsigned long)Source->deHighClust)<<16);//���ù�
	Desti->F_Size         = Source->deFileSize;	   //�����ļ���С
	Desti->F_Attr         = Source->deAttributes;  //�ļ�����
	Desti->F_CurClust     = 0;//����...
	Desti->F_Offset       = 0;//ƫ��0 	

	//FAT�ĴغŲ�����0(��Ŀ¼�غ�)
	if(fat_type==32&&Desti->F_StartCluster==0)  
	{																  
		Desti->F_StartCluster=root_dir_clu;//�ı�����غ�.ʹ����ڸ�Ŀ¼���ڴغ�!!
	}   
	if(LongNameFlag)//���ڳ��ļ���
	{
		LongNameBuffer[MAX_LONG_NAME_SIZE-1] = 0;
		LongNameBuffer[MAX_LONG_NAME_SIZE-2] = 0;
		UniToGB(LongNameBuffer);  //��Unicode����ת��ΪASICII��
		for(i=0;i<80;i++)Desti->F_Name[i] = LongNameBuffer[i];//���Ƴ��ļ���
	}else //���ļ���
	{	
		if(Source->deName[0]==0x2e)//�õ�һ����Ŀ¼(�޸�Ϊ:":\")
		{	
			//���游Ŀ¼�غ�   
			Fat_Dir_Cluster=Desti->F_StartCluster; 	 
			Desti->F_Name[0]=':';
			Desti->F_Name[1]=0x5c;//'\'
			Desti->F_Name[2]='\0';//���������
		}else	    			  //��ͨ�ļ�
		{
			for(i=0;i<11;i++)Desti->F_Name[i] = Source->deName[i];//���ƶ��ļ���  
			Desti->F_Name[11]='\0';//���������
		}
	} 															   	  								  
	return ;
}


//���Ŀ���ļ��������һ���ļ���
//dir_clust:��ǰĿ¼���ڴغ�
//FileInfo :Ŀ���ļ���ʵ�����(FileInfoStruct��)
//type     :Ҫ���ҵ��ļ�����:1<<0,mp3;1<<1,wma;1<<2,wav,1<<3,mid;1<<4,1<<lrc;
//                           1<<5,txt;1<<6,jpg;1<<7,jpeg;1<<8,bmp;1<<9,file; 
//count    :0,���ص�ǰĿ¼��,�������ļ��ĸ���;��Ϊ��ʱ,���ص�count���ļ�����ϸ��Ϣ
//����ֵ   :1,�����ɹ�.0,����ʧ��
u8 Get_File_Info(u32 dir_clust,FileInfoStruct *FileInfo,u16 type,u16 *count)
{ 			  	   
	DWORD sector;
	DWORD cluster=dir_clust;
	DWORD tempclust;
	unsigned char cnt;
	unsigned int offset;		 
	unsigned short cont=0;//�ļ�������־ <65536
	unsigned char j; //long name fat_buffer offset;
	unsigned char *p;//long name fat_buffer pointer
	Short_DIR *item = 0;
	Long_DIR *we =0;	  
	cont=0;
	LongNameFlag = 0;//��ճ��ļ�����־

	//SD_Init();//��ʼ��SD����������γ�֮���������ʹ��
	//goto SD;
	if(cluster==0 && fat_type==16)//FAT16��Ŀ¼��ȡ
	{			 
		for(cnt=0;cnt<root_dir_size;cnt++)
		{
			if(SD_ReadSingleBlock(root_address+cnt,fat_buffer))return 1;//��������
//SD:			for(offset=0;offset<512;offset++)fat_buffer[offset]=temp_buf[offset];	   
			for(offset=0;offset<512;offset+=32)
			{
				item=(Short_DIR *)(&fat_buffer[offset]);//ָ��ת��
				//�ҵ�һ�����õ��ļ�
				if((item->deName[0]!=0x2E)&&(item->deName[0]!=0x00)&&(item->deName[0]!=0xe5)
				||((item->deName[0]==0x2E)&&(item->deName[1]==0x2E)))//�ҵ�һ���Ϸ��ļ�.����".",ʹ��".."
				{		   
					if(item->deAttributes == 0x0f)//�ҵ�һ�����ļ���
					{
						we = (Long_DIR *)(&fat_buffer[offset]);
						j = 26 *( (we->weCnt-1) & WIN_CNT);//���ļ����ĳ���
                        if(j<MAX_LONG_NAME_SIZE-25)
						{
							p = &LongNameBuffer[j];//ƫ�Ƶ�Ŀ���ַ
							for (j=0;j<10;j++)	*p++ = we->wePart1[j];			
							for (j=0;j<12;j++)	*p++ = we->wePart2[j];
							for (j=0;j<4;j++)	*p++ = we->wePart3[j];	
							if (we->weCnt & 0x40) (*(unsigned int *)p) = 0;  				
							if ((we->weCnt & WIN_CNT) == 1) LongNameFlag = 1;//���һ�����ļ����ҵ���	
						}	    
					}else 
					{  	 										 									 
						if(type&FileType_Tell(item->deExtension))//�ҵ�һ��Ŀ���ļ�
						{
							cont++;//�ļ�����(�ҵ����ļ���Ŀ)����
						 }
						 //���Ҹ�Ŀ¼��,type���͵��ļ�����
						if(*count&&cont==*count)
						{
							//printf("\ncount:%d",*count);
							CopyDirentruyItem(FileInfo,item);//����Ŀ¼��,��ȡ��ϸ��Ϣ 

							return 1;//�ҵ�Ŀ���ļ��ɹ�	 
						}
						LongNameFlag=0;//��ճ��ļ���
					}
				}
			}
		}  				 
	}
	else//�����ļ���/FAT32ϵͳ
	{
		tempclust=cluster;
		while(1)
		{
			sector=fatClustToSect(tempclust);  //�غ�ת��Ϊ������
			for(cnt=0;cnt<cluster_size;cnt++)
			{
				if(SD_ReadSingleBlock(sector+cnt,fat_buffer))return 1;
				for(offset=0;offset<512;offset+=32)//ÿ��ƫ��32���ֽ�
				{
					item=(Short_DIR *)(&fat_buffer[offset]);
					if((item->deName[0]!=0x2E)&&(item->deName[0]!=0x00)&&(item->deName[0]!=0xe5)
					||((item->deName[0]==0x2E)&&(item->deName[1]==0x2E)))//�ҵ�һ���Ϸ��ļ�.����".",ʹ��".."
					{				
						if(item->deAttributes == 0x0f) //�õ�һ�����ļ���
						{
							we = (Long_DIR *)(&fat_buffer[offset]);
							j = 26 *( (we->weCnt-1) & WIN_CNT);	//52+26=78+2=80
							if(j<MAX_LONG_NAME_SIZE-25)
							{
								p = &LongNameBuffer[j];//pָ���ļ����Ĵ�ŵ�ַ
								for (j=0;j<10;j++)	*p++ = we->wePart1[j];			
								for (j=0;j<12;j++)	*p++ = we->wePart2[j];
								for (j=0;j<4;j++)	*p++ = we->wePart3[j];	
								if (we->weCnt & 0x40) (*(unsigned int *)p) = 0;				
								if ((we->weCnt & WIN_CNT) == 1) LongNameFlag = 1;	
							}										    			 
						}	  
						else  //���ļ���
						{
							if(type&FileType_Tell(item->deExtension))//�ҵ�һ��Ŀ���ļ�
							{
								cont++;//�ļ�����(�ҵ����ļ���Ŀ)����
							 }
							 //���Ҹ�Ŀ¼��,type���͵��ļ�����
							if(*count&&cont==*count)  //����if(*count==cont)�����𣿣�������������������
							{
								CopyDirentruyItem(FileInfo,item);//����Ŀ¼��,��ȡ��ϸ��Ϣ  
								return 1;//�ҵ�Ŀ���ļ��ɹ�	 
							}
							LongNameFlag=0;//��ճ��ļ���
						}
					}
				}
			}						  
			tempclust=FAT_NextCluster(tempclust);//������һ���غ�
			if(tempclust==0x0fffffff||tempclust==0x0ffffff8 ||(fat_type==16&&tempclust==0xffff))break;
		}  
	}
	if(*count==0)
	{
		*count=cont;//�õ��ܹ��ļ���Ŀ
		return 1;   //�����ɹ�,�ҵ�cont�������������ļ���
	}else return 0; //����ʧ��,û�ҵ��ļ�,���߳���
}


//���ļ�
void F_Open(FileInfoStruct *FileInfo)
{
	FileInfo->F_CurClust=FileInfo->F_StartCluster;//��ǰ��Ϊ�״�
	FileInfo->F_Offset=0;//ƫ������Ϊ0
}
//��ȡ512���ֽ�
//FileInfo:Ҫ��ȡ���ļ�
//buf     :���ݻ�����
//����ֵ  :0,����ʧ��,1,�����ɹ�
unsigned char F_Read(FileInfoStruct *FileInfo,u8 *buf)
{	
	DWORD sector;			   		  
	sector=fatClustToSect(FileInfo->F_CurClust);//�õ���ǰ�غŶ�Ӧ��������	   	 		    
	if(SD_ReadSingleBlock(sector+FileInfo->F_Offset,buf))return 0;//��������   
	FileInfo->F_Offset++; //�����ż�1
	if(FileInfo->F_Offset==cluster_size)	//�صľ�ͷ,����
	{
		FileInfo->F_Offset=0;		    
		FileInfo->F_CurClust=FAT_NextCluster(FileInfo->F_CurClust);//��ȡ��һ���غ�
		if((fat_type==16&&FileInfo->F_CurClust==0xffff) \
		||FileInfo->F_CurClust==0x0ffffff8||FileInfo->F_CurClust == 0x0fffffff)return 0;//error	    
	} 
	return 1;//��ȡ�ɹ�
}

//��ȡǰ512���ֽ�
//FileInfo:Ҫ��ȡ���ļ�
//buf     :���ݻ�����
//����ֵ  :0,����ʧ��,1,�����ɹ�
unsigned char F_PrevRead(FileInfoStruct *FileInfo,u8 *buf)
{	
	DWORD sector;  
	if(FileInfo->F_Offset>1)FileInfo->F_Offset-=2;//δ���أ�ֱ��������һ����  //>1???-2??
	else 					  //������,������һ��
	{
		FileInfo->F_CurClust=FAT_PrevCluster(FileInfo->F_CurClust,FileInfo->F_StartCluster);//��ȡ��һ���غ�
		if((fat_type==16&&FileInfo->F_CurClust==0xffff) \
		||FileInfo->F_CurClust==0x0ffffff8||FileInfo->F_CurClust == 0x0fffffff)return 0;//��������	 
		if(FileInfo->F_Offset==1)FileInfo->F_Offset=cluster_size-1;//�������һ������ 
		else FileInfo->F_Offset=cluster_size-2;//�������ڶ���  
	}  
	sector=fatClustToSect(FileInfo->F_CurClust);//�õ���ǰ�غŶ�Ӧ��������	   
	if(SD_ReadSingleBlock(sector+FileInfo->F_Offset,buf))return 0;//��������   
	return 1;//��ȡ�ɹ�
}

//�Ƚ������ַ�����Ȳ�
//���,����1,�����,����0;
u8 mystrcmp(u8*s1,u8*s2)
{
	u8 len1,len2;
	len1=len2=0;
	while(*s1!='\0')
	{
		len1++;s1++;
	}
	while(*s2!='\0')
	{
		len2++;s2++;
	}
	if(len1!=len2)return 0;//����� 
	s1-=len1;s2-=len1;
	while(*s1!='\0')
	{
		if(*s1!=*s2)return 0;//����� 
		s1++;s2++;
	}
	return 1;
}	 
//����ϵͳ�ļ�
//��ָ��Ŀ¼��,��Ѱһ��ָ�����͵�ָ�����ֵ��ļ�
//cluster:�ļ��еĴغ�!!!
//Name   :�ļ�������
//type   :�ļ�����
//����ֵ :���ļ��Ŀ�ʼ�غ�
u32 F_Search(u32 cluster,unsigned char *Name,u16 type)
{
	DWORD sector;			 
	DWORD tempclust;
	unsigned char cnt;
	unsigned int offset; 

	Short_DIR *item = 0;	  
	FileInfoStruct FileInfo;    			    
	if(cluster==0 && fat_type==16)//FAT16��Ŀ¼��ȡ
	{			 
		for(cnt=0;cnt<root_dir_size;cnt++)
		{
			if(SD_ReadSingleBlock(root_address+cnt,fat_buffer))return 1;//��������    
			for(offset=0;offset<512;offset+=32)
			{
				item=(Short_DIR *)(&fat_buffer[offset]);//ָ��ת��
				//�ҵ�һ�����õ��ļ�
				if((item->deName[0] != 0x00) && (item->deName[0] != 0xe5))//�ҵ�һ���Ϸ��ļ�
				{		   
					if(item->deAttributes != AM_LFN)//���Գ��ļ���	 
					{  	 				 
						CopyDirentruyItem(&FileInfo,item);//����Ŀ¼��,��ȡ��ϸ��Ϣ	   
						if(FileInfo.F_Type&type)//�ҵ�һ�����ʵ�������
						{
						   // printf("File Name:%s\n",FileInfo.F_Name);
	
							//�ҵ����ļ�,��������ļ����״�
							if(mystrcmp(Name,FileInfo.F_Name))return FileInfo.F_StartCluster; 
						} 	  
						LongNameFlag=0;//��ճ��ļ���
					}
				}
			}
		}  				 
	}
	else//�����ļ���/FAT32ϵͳ
	{
		tempclust=cluster;
		while(1)
		{	
			sector=fatClustToSect(tempclust);
			for(cnt=0;cnt<cluster_size;cnt++)
			{
				if(SD_ReadSingleBlock(sector+cnt,fat_buffer))return 1;
				for(offset=0;offset<512;offset+=32)
				{
					item=(Short_DIR *)(&fat_buffer[offset]);
					if((item->deName[0] != 0x00) && (item->deName[0] != 0xe5))
					{				
						if(item->deAttributes != AM_LFN) //���Գ��ļ���		    
						{  	 				 
							CopyDirentruyItem(&FileInfo,item);//����Ŀ¼��,��ȡ��ϸ��Ϣ	  
							if(FileInfo.F_Type&type)//�ҵ�һ�����ʵ�������
							{	 
								//�ҵ����ļ�,��������ļ����״�
								if(mystrcmp(Name,FileInfo.F_Name))
								{	
								 	file_cluster=FileInfo.F_StartCluster;
									file_size=FileInfo.F_Size;	
								  	return FileInfo.F_StartCluster; 
								}
								
							} 	  
							LongNameFlag=0;//��ճ��ļ���
						}
					}
				}
			}						  
			tempclust=FAT_NextCluster(tempclust);//������һ���غ�
			if(tempclust==0x0fffffff||tempclust==0x0ffffff8 ||(fat_type==16&&tempclust==0xffff))break;
		}  
	}
	return 0;//ʧ��						    
}





//��ȡϵͳ�ļ��Ĵ洢��ַ
//�β�����,���޷�����!!!
//����0��ʧ�ܡ�����1���ɹ�
//sel:bit7��0,����ϵͳ�ļ�
//sel:bit7��1,������Ϸ�ļ�
//sel:bit 0~6 ���
u8 SysInfoGet(u8 sel)
{			 
	u32 cluster=0;
	u32 syscluster=0;
	u8 t=0;	  
//	u8 imgx[9];
//	u8 sizex;  //ͼƬ��С Ҳ�����ѶȵĴ�С	 
	//�õ���Ŀ¼�Ĵغ�
	if(fat_type==32)   //�����FAT32�ļ�ϵͳ
		cluster = root_dir_clu;
	else  //����
		cluster=0;	

	cluster = F_Search(cluster,(unsigned char *)folder[0],T_FILE);//����SYSTEM�ļ���
	if(cluster==0)return 0;//ϵͳ�ļ��ж�ʧ	
	syscluster=cluster;//����ϵͳ�ļ������ڴغ�
	File_DIR[0]=cluster;	//SYSTEM
	
		
	cluster=F_Search(root_dir_clu,(unsigned char *)folder[8],T_FILE);//�ڸ��ļ����²���TXT�ļ���
	if(cluster==0)return 0;//�����ļ��ж�ʧ	
	File_DIR[1]=cluster;
	
	cluster=F_Search(root_dir_clu,(unsigned char *)folder[7],T_FILE);//�ڸ��ļ����²���MP3�ļ���
	if(cluster==0)return 0;//�����ļ��ж�ʧ	
	File_DIR[2]=cluster;	
	
	cluster=F_Search(root_dir_clu,(unsigned char *)folder[9],T_FILE);//�ڸ��ļ����²���RECEIVE�ļ���
	if(cluster==0)return 0;//�����ļ��ж�ʧ	
	File_DIR[4]=cluster;
	
	cluster=F_Search(root_dir_clu,(unsigned char *)folder[11],T_FILE);//�ڸ��ļ����²���PIC�ļ���
	if(cluster==0)return 0;//�����ļ��ж�ʧ	
	File_DIR[5]=cluster;			
				   
/*	
	if(sel&0x80)//������Ϸ�ļ�
	{	
		sizex=sel&0x7F;//�õ�size 3*3��4*4��5*5									  
		cluster=F_Search(cluster,(unsigned char *)folder[3],T_FILE);//��system�ļ����²���game�ļ���
		if(cluster==0)return 0;  		
					   
		cluster=F_Search(cluster,(unsigned char *)folder[sizex+1],T_FILE);//��game�ļ����²���LEVEL(sizex-2)�ļ���	
		if(cluster==0)return 0;//ϵͳ�ļ��ж�ʧ	   	  			   
		sizex*=sizex;//ȡƽ��
		for(t=0;t<sizex+1;t++) //X*XͼƬ��ַ��ȡ
		{
			Get_Game_Pic_Str(t,imgx);//�õ�ͼƬ���		  
			Pic_Addr[t]=F_Search(cluster,imgx,T_BMP);//��LEVEL1�ļ����²���BMPͼƬ
			if(Pic_Addr[t]==0)return 0;//ϵͳ�ļ��ж�ʧ	 	
		}	 
		//printf("level ok\n");
	}
*/
//	else//����ϵͳ�ļ�
//	{	 										 
		//�Ȳ�������
		cluster=F_Search(syscluster,(unsigned char *)folder[1],T_FILE);//��system�ļ����²���FONT�ļ���
		if(cluster==0)return 0;//�����ļ��ж�ʧ	  
		
		FONT16CLUSTER=F_Search(cluster,(unsigned char *)sysfile[0],T_FON);//��system�ļ����²���FONT16�����ļ�
		if(FONT16CLUSTER==0)return 0;//FONT16�����ļ���ʧ		 
			  
		FONT12CLUSTER=F_Search(cluster,(unsigned char *)sysfile[1],T_FON);//��system�ļ����²���FONT12�����ļ�
		if(FONT12CLUSTER==0)return 0;//FONT12�����ļ���ʧ						  	 
			
		cluster=F_Search(syscluster,(unsigned char *)folder[2],T_FILE);//��system�ļ����²���SYSICO�ļ���
		if(cluster==0)return 0; 
		for(t=0;t<9;t++)//����ϵͳͼ��,�Ÿ�
		{
			sys_ico[t]=F_Search(cluster,(unsigned char *)sysfile[t+2],T_BMP);//��SYSICO�ļ����²���ϵͳͼ��
			if(sys_ico[t]==0)return 0;//ʧ��		 
		}
		for(t=0;t<9;t++)//���ҵڶ�ҳ��������ͼ��
		{
			sys_ico1[t]=F_Search(cluster,(unsigned char *)sysfile1[t],T_BMP);//��SYSICO�ļ����²���ϵͳͼ��
			if(sys_ico1[t]==0)return 0;//ʧ��		
		}
		for(t=11;t<19;t++)//����ϵͳͼ��,6��
		{
			file_ico[t-11]=F_Search(cluster,(unsigned char *)sysfile[t],T_BMP);//��SYSICO�ļ����²����ļ�ͼ��
			if(file_ico[t-11]==0)return 0;//ʧ�� 	  
		}
		
//	}
	return 1;//�ɹ�

}



//��ָ��dir_cluster���������Ϊfile_num��file_type�����ļ�
//Name���ļ�����
//����: �ļ���ָ��
u8* get_file_name(u8 file_num,u32 dir_cluster,u16 file_type)
{
	u8 *return_filename;
	u8 num=1;
	DWORD tempclust=dir_cluster;
	DWORD sector;
	u8 cnt;u16 offset;
	Short_DIR *item = 0;	  
	FileInfoStruct FileInfo;  
	while(1)
	{
		sector=fatClustToSect(tempclust);
		for(cnt=0;cnt<cluster_size;cnt++)
		{
			if(SD_ReadSingleBlock(sector+cnt,fat_buffer))return 0;
			for(offset=0;offset<512;offset+=32)
			{
				item=(Short_DIR *)(&fat_buffer[offset]);
				if((item->deName[0] != 0x00) && (item->deName[0] != 0xe5))
				{				
					if(item->deAttributes != AM_LFN) //���Գ��ļ���		    
					{  	 				 
						CopyDirentruyItem(&FileInfo,item);//����Ŀ¼��,��ȡ��ϸ��Ϣ	  
						if(FileInfo.F_Type&file_type)//�ҵ�һ��TXT����
						{	
							if(num==file_num) 
							{
								return_filename=FileInfo.F_Name;
								return return_filename;	//�õ��ļ��ļ�												
							}

							num++;	
						} 	  
						LongNameFlag=0;//��ճ��ļ���
					}
				}
			}
		}						  
		tempclust=FAT_NextCluster(tempclust);//������һ���غ�
		if(tempclust==0x0fffffff||tempclust==0x0ffffff8 ||(fat_type==16&&tempclust==0xffff))break;	
	}
	return 0;
}

u8 get_file_num(u32 dir_cluster,u16 type)
{    
	u16 num;
	FileInfoStruct *FileInfo=0;
	num=0;		//ָ����ָ������0
	while(!Get_File_Info(dir_cluster,FileInfo,type,&num));		//��ȡTXT�ļ�����
	return (u8)num;
}


 //Cluster �ص���� ����Ϊ�ôغ�����Ӧ��������ַ
//�õ�Cluster
unsigned long GetSysFileSector(u8 uoh,unsigned int Cluster)
{
	unsigned long foffset=0;
	unsigned int i; 
	
	if(uoh==16)foffset=FONT16CLUSTER;//�õ�unicode����״�  
    else if(uoh==12)foffset=FONT12CLUSTER; 
	i=Cluster;
	while(i)
	{
	    //printf("foffset:%d\n",foffset);
	    foffset=FAT_NextCluster(foffset);
	    i--;
	}
	//printf("final foffset:%d\n",foffset);
	return (root_address+ (foffset-2)*8); //�����ַ�����������
}    

//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,ascii��
//mat  ���ݴ�ŵ�ַ FSIZE*2 bytes��С
void Get_HzMat(unsigned char *code,unsigned char *mat)
{
	unsigned char qh,wh;
	unsigned char i;
	unsigned int  sector,cluster,secoff;
	unsigned long foffset; 
	if(*code<0xa1)if(*code++<0xa1)//�� ���ú���
	{   
	    code--;
	    for(i=0;i<(FSIZE*2);i++)*mat++=0xff;//�������
	    return; //��������
	}          
	qh=(*code++)-0xa1;	 //���������8λ
	wh=(*code)-0xa1;   	 //���������8λ

	foffset=((unsigned long)94*qh+wh)*(FSIZE*2);//�õ��ֿ��е��ֽ�ƫ����   
	//LCD_ShowNum(20,1,wh,4,WHITE,BLUE);
	sector=foffset/sector_size;              //�õ��ܵ�������������(��Ҫд��(unsigned int ) 
 	secoff= (unsigned int) foffset%sector_size;//�����ڵ��ֽ���ƫ��
	wh=(unsigned char)sector%cluster_size;		//����ƫ�� 
	cluster=(unsigned int)sector/cluster_size;//�õ��ܵĴ���
	
	foffset=GetSysFileSector(FSIZE,cluster); //ȡ���ֿ��clusor������������ַ 
	 
	if(sector_size-secoff>=(FSIZE*2))//ȷ���Ƿ������?
	{        
		 SD_Read_Bytes(foffset+wh,mat,secoff,(FSIZE*2));  //��ȡ(FSIZE*2)�ֽ�
	}
	else
	{
		i=sector_size-secoff; //��ȡ�������ѿ�����
		SD_Read_Bytes(foffset+wh,mat,secoff,i);
		if(++wh>cluster_size)
		{  
			wh=0; 
			foffset=GetSysFileSector(0,++cluster);//��ȡ�������Ѿ����
		} 
		SD_Read_Bytes(foffset+wh,mat+i,0,(FSIZE*2)-i); //��ȡʣ�������
	} 
}  

