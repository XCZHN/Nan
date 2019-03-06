#include "main.h"					    			  
//¸è´Ê¿ØÖÆ²¿·Ö
//Ö§³Ö¸ñÊ½[XX:XX]/[XX:XX:XX]/[XX:XX.X]
//Í¬Ê±Ò²Ö§³Ö¶à¸ö²¢ÅÅµÄµÄ¸è´Ê¸ñÊ½
//Èç[XX:XX.XX][XX:XX.XX]Ò²ÊÇÖ§³ÖµÄ	 
//ÕıµãÔ­×Ó@HYW
//2009.04.30
//V1.1	 		

//Éè¼ÆË¼Ïë:
//µÚÒ»²½:°ÑlrcÎÄ¼şµÄtagÈ«²¿¸´ÖÆ³öÀ´,´æ·Åµ½tag_table(×î´óÖ§³Ö426¸ötag,¶à³öµÄ²¿·Ö½«²»ÓèÀí»á)
//µÚ¶ş²½:°ÑµÃµ½µÄtag,°´Ë³ĞòÅÅÁĞ.µÃµ½±ê×¼µÄÊ±¼ätag,±£´æÔÚtag_tableÀïÃæ
//µÚÈı²½:ÔÙ°´ÕÕtagµÄÊ±¼äË³Ğò,±éÀútag,Í¬Ê±¶ÁÈ¡lrcÎÄ¼şµÄ¸è´Ê,µ±¶Áµ½·ûºÏÌõ¼şµÄÊ±ºò,¸´ÖÆtag¼°¸è´Êµ½
//       ¸è´Ê±£´æµÄµØÖ·buffer1,buffer2ÖĞ.ÕâÑù¾ÍµÃµ½ÁËÕû¸ö¸è´Ê
//×¢Òâ:buffer1+buffer2µÄ´óĞ¡Îª6144¸ö×Ö½Ú,³¬³ö²¿·Ö½«¶ªÊ§.
  u8 bt30MS=0;                       //30msµ½ÁËµÄ±êÖ¾
/*					  
u16 lrc_tag_cnt=0; //¼ÇÂ¼tagµÄ×ÜÊı
u8  lrctagtype=0;  //¼ÇÂ¼lrcµÄÀàĞÍ 0:[XX:XX] 1:[XX:XX.XX]	  
//¸è´ÊÎÄ¼şµÄĞÅÏ¢´æ·ÅµØÖ·		   
FileInfoStruct *LrcFile=&F_Info[1];//¿ª±ÙÔİ´æ¿Õ¼ä.	 
u8 *tag_table=(u8 *)MCUBuffer;     //LRCÊ±¼ä´ÁÔİ´æµØÖ·,ºóÃæ½«ÓÃÓÚ´æ·ÅÒ»¾äLRC¸è´Ê 

//LRC¸è´ÊµÄĞÅÏ¢		 
typedef struct{ 
	u8   first;    //±êÖ¾¸è´ÊÊÇ·ñ¸üĞÂÁË
	u16  t_sec;    //Ôİ´æ¸è´ÊµÄÊ±¼ä(Ãë)
	u8   t_ms;     //±£´æ32MS¼ÆÊıÂö³å			 			    
    u16  lrcoff;   //¸è´ÊÎÄ¼ş¶ÁÈ¡µÄ³¤¶È
	u8   tagcnt;   //¸è´Ê½áÊø±ê¼Ç
}LYRIC_STRUCT;
LYRIC_STRUCT lyric; //¸è´ÊĞÅÏ¢

///////////////////////////////////////////////////////////////
unsigned short	code_value_table[4][256];
		  
u8 WriteByteLrcBuf(u16 addr,u8 data);//Ğ´ÈëÒ»¸ö×Ö½ÚÊı¾İ
u8 ReadByteLrcBuf(u16 addr);         //¶ÁÈ¡Ò»¸ö×Ö½ÚÊı¾İ
u8 Get_Byte(u8 rst);//´ÓsdµÃµ½Ò»¸ö×Ö½Ú
u8 Get_Num(void);   //µÃµ½Ò»¸öÊı×Ö
u8 Get_Tag(void); //µÃµ½tagÁĞ±í
void Init_Tag(void);//¶ÔµÃµ½µÄtagÅÅĞò
 
//´ÓlrcbuferÀïÃæ¶ÁÈ¡Ò»¸ö×Ö½Ú 
//¶ÁÊı´íÎó,·µ»Ø0xff     
//addr:µØÖ·
//·µ»ØÖµ:¶Áµ½µÄÊı¾İ
u8 ReadByteLrcBuf(u16 addr)
{						  
	u8 *buffer1=(u8 *)iclip;//µÃµ½µÚÒ»¸öbuf´æ·ÅµÄµØÖ·
	u8 *buffer2=(u8 *)code_value_table;//µÚ¶ş¸öbufµØÖ·
										    
	if(addr<4096)return buffer1[addr];
	else 
	{				 
		if(addr>6144)return 0xff;
		return buffer2[addr];//·µ»ØÊı¾İ
	}
}


//ÏòlrcbuferÀïÃæĞ´Ò»¸ö×Ö½Ú,µØÖ·×Ô¶¯Ôö¼Ó
//³¬³ö·¶Î§,·µ»Ø0xff
//data:ÒªĞ´ÈëµÄÊı¾İ  
//addr:µØÖ·
//·µ»ØÖµ:²Ù×÷±êÖ¾
//×î¶à´æ´¢6Kbyte´óĞ¡µÄ¸è´Ê.
u8 WriteByteLrcBuf(u16 addr,u8 data)
{						  
	u8 *buffer1=(u8 *)iclip;//µÃµ½µÚÒ»¸öbuf´æ·ÅµÄµØÖ·
	u8 *buffer2=(u8 *)code_value_table;//µÚ¶ş¸öbufµØÖ·	  							  
	if(addr<4096)buffer1[addr]=data; //Ğ´ÈëÊı¾İµ½buffer1
	else 
	{			 
		if(addr>6144)return 0xff;
		buffer2[addr-4097]=data;  //Ğ´ÈëÊı¾İµ½buffer2
	}
	return 1;
}

//¸´ÖÆÒ»¾ä¸è´Ê
//tmin,tsec,tmms:Òª¸´ÖÆµÄ¸è´Êtag
//·µ»ØÖµ:0xff,½áÊø.1,¸´ÖÆ³É¹¦	  
u8 CopyString(u8 tmin,u8 tsec,u8 tmms)
{
	static u16 tagpos=0;   //¸è´Êtag±êÖ¾
	static u16 lrcbufpos=0;//¸è´Ê»º´æµØÖ·
	u8 temp;
	u8 temp1;
	u16 tagcnt=0;

	if(tmin==0xff){tagpos=0;lrcbufpos=0;return 1;}//¸´Î»
	if(lrctagtype)tagcnt=(lrc_tag_cnt-1)*3;
	else tagcnt=(lrc_tag_cnt-1)*2;	
	if(tagpos>tagcnt)//¶ÁÈ¡½áÊøÁË
	{
		WriteByteLrcBuf(lrcbufpos,'\0');//Ğ´Èë½áÊø·û
		return 0xff;
	}	 
	if(tmin==tag_table[tagpos]&&tsec==tag_table[tagpos+1]&&lrctagtype==0)//[XX:XX]ÀàĞÍ¸è´Ê
	{	  
		tagpos+=2;
	}else if(tmin==tag_table[tagpos]&&tsec==tag_table[tagpos+1]&&tmms==tag_table[tagpos+2]&&lrctagtype==1) //ÀàĞÍ2
	{
		tagpos+=3;
	}else return 1;
	WriteByteLrcBuf(lrcbufpos++,'['); //Ğ´Ò»¸ö×Ö½Ú
	WriteByteLrcBuf(lrcbufpos++,tmin);//Ğ´Èë·ÖÖÓ
	WriteByteLrcBuf(lrcbufpos++,':'); 
	WriteByteLrcBuf(lrcbufpos++,tsec);//Ğ´ÈëÃëÖÓ
	if(lrctagtype)//XX:XX.XX¸ñÊ½
	{
		WriteByteLrcBuf(lrcbufpos++,'.'); 
		WriteByteLrcBuf(lrcbufpos++,tmms);//Ğ´ÈëÃëÖÓ
	}
	WriteByteLrcBuf(lrcbufpos++,']');     //Ğ´Èë×îºóµÄ½áÊø±êÖ¾
	 
	while(1)//Ò»Ö±µÈµ½µÃµ½tag½áÊø±êÖ¾
	{
		temp=Get_Byte(0);
		if(temp=='[') //¿ÉÄÜÒâÎ¶×ÅÏÂÒ»¸ötagµÄµ½À´
		{
			WriteByteLrcBuf(lrcbufpos,temp);//ÏÈ¼ÇÂ¼ÏÂÀ´,ÒÔ·À²»ÊÇ
			temp1=Get_Byte(0);
			temp=temp1;//¸´ÖÆ
			temp1-='0';
			if(temp1<10)//Èç¹ûÊÇÊı¾İ,¿Ï¶¨ÊÇÏÂÒ»¸ötagÁË
			{
				while(1)
				{
					temp1=Get_Byte(0);
					if(temp1==']'||temp1==0xff)break;//¶Áµ½tagÄ©Î²
				}
			}else  //²»ÊÇtag,ÊÇÊı¾İ
			{
				lrcbufpos++;
				WriteByteLrcBuf(lrcbufpos++,temp);//¼ÇÂ¼ÏÂÀ´	  
				break;//¶Áµ½ÓĞĞ§Êı¾İÁË
			}
		}else if(temp!=' ')	//ºöÂÔ¿Õ¸ñ
		{
			if(temp!=0XFF&&temp!=0X00)WriteByteLrcBuf(lrcbufpos++,temp);//¼ÇÂ¼ÏÂÀ´ 
			break;//¶Áµ½ÁËÓĞĞ§Êı¾İ
		} 							   
	}						 
	while(1)//¿ªÊ¼¸´ÖÆÎÄ±¾
	{
		temp=Get_Byte(0);//µÃµ½Ò»¸ö×Ö·û
		if(temp!=0XFF&&temp!=0X00)WriteByteLrcBuf(lrcbufpos++,temp);//¸´ÖÆºÏ·¨×Ö·û      
		if(temp==0X0A||temp==0XFF||temp==0X00)//±¾¾ä¶ÁÈ¡½áÊø
		{
			WriteByteLrcBuf(lrcbufpos++,temp);
			break;
		}	    
	}
	return 1;
}   

//¸´ÖÆLRC¸è´Êµ½ramÀïÃæ
void CopyLrcToRam(void)
{					    		 
	u8 numx;
	u8 temp;
	u8 min;
	u8 sec;
	u8 mms;		
					    
	CopyString(0xff,0,0);//¸´Î»
	Get_Byte(1);//¸´Î» 
	while(1)
	{
		while(1)//Ò»Ö±µÈµ½µÃµ½tag¿ªÊ¼±êÖ¾
		{
			temp=Get_Byte(0);	 
			if(temp=='[')break;
			else if(temp==0xff) //ËùÓĞÊı¾İ¶ÁÈ¡½áÊøÁË
			{
				Get_Byte(1);//¸´Î» ÖØĞÂ¶Á
			}			   
		}
		numx=Get_Num();//µÃµ½µÚÒ»¸öÊı×Ö	  
		if(Get_Byte(0)!=':'||numx==0xff)continue;
		min=numx;
		numx=Get_Num();//µÃµ½µÚ¶ş¸öÊı×Ö
		temp=Get_Byte(0);
		if(temp!='.'&&temp!=']'||numx==0xff)continue;
		sec=numx;	    
		if(temp!='.')  //[XX:XX]¸ñÊ½
		{				     
			if(CopyString(min,sec,0)==0XFF)return ;//¸´ÖÆÍê±Ï	 		    
			continue;
		}			   
		numx=Get_Num();//µÃµ½µÚÈı¸öÊı×Ö
		temp=Get_Byte(0);
		if(temp!=']'||numx==0xff)continue;//[XX:XX.XX]¸ñÊ½
		mms=numx;
		if(CopyString(min,sec,mms)==0XFF)return ;//¸´ÖÆÍê±Ï	 
	}
}

	
//¸è´Ê´æ·ÅµØÖ·,½èÓÃJPEG½âÂë²¿·ÖÀïÃæµÄRAM	   
u8 *LRC_DATA=(u8 *)QtZzMCUBuffer; 

//´Ó¸è´ÊÎÄ¼şÖĞµÃµ½Ò»¸öLRC×Ö·û
//½áÊø·µ»Ø0XFF
//rst:1,¸´Î»,´ÓÍ·¿ªÊ¼¶ÁÊé
//    2,Æ«ÒÆµ½ÉÏÒ»¸öµØÖ·
u8 Get_Byte(u8 rst)
{		 
	static u16 bytepos;
	u8 temp;					  
	if(rst)//¸´Î»,²»¶ÁÈ¡Êı¾İ
	{
		if(rst==1)//¸´Î»
		{
			F_Open(LrcFile);//´ò¿ªLRCÎÄ¼ş
			F_Read(LrcFile,LRC_DATA);//¶Á³ö512¸ö×Ö½Ú	  
			bytepos=0;	  
		}
		else bytepos--;//Æ«ÒÆµ½ÉÏÒ»´Ø ¶Ô¸¶[XX:XX.X]¸ñÊ½µÄ¸è´Ê
		return 0xff;
	}   
	if(bytepos>511)//³¬¹ıÒ»¸öÉÈÇøÁË,¶ÁÏÂÒ»¸öÉÈÇø
	{
		temp=F_Read(LrcFile,LRC_DATA);//¶Á³ö512¸ö×Ö½Ú	 
		bytepos=0; 
		if(temp==0)return 0xff;//¶ÁÊı´íÎó	   
	} 
    return LRC_DATA[bytepos++];
}  
 
//µÃµ½Ò»¸öÊı×Ö
//Ê§°Ü·µ»Ø0xff
u8 Get_Num(void)
{
	u8 t1=0;
	u8 t2=0;
	t1=Get_Byte(0);//µÃµ½µÚÒ»Î»Êı¾İ	 Ê®Î»
	t2=Get_Byte(0);//µÃµ½µÚ¶şÎ»Êı¾İ  ¸÷Î»
	if(t1<'9'&&t1>'0'&&t2==']')//Óöµ½[XX:XX.X]ÕâÖÖ¸ñÊ½
	{
		Get_Byte(2);//Æ«ÒÆµ½ÉÏÒ»¸öµØÖ·  ·ñÔòÎŞ·¨¼ì²â']'
		return (t1-'0')*10;
	}	 
	t1-='0';//µÃµ½ASCIIÊıÖµ
	t2-='0';
	if(t1<10&&t2<10)//µÃµ½ºÏ·¨Êı¾İ
	{
		return t1*10+t2;
	}
	return 0XFF;
}	  
//µÃµ½¸è´ÊÎÄ¼şµÄËùÓĞÊ±¼ä´Á tag
//·µ»ØÖµ£º1,³É¹¦»ñÈ¡ÁËtag¡£0£¬»ñÈ¡tagÊ§°Ü
u8 Get_Tag(void)
{	
	u16 i=0;														    
			 
	u8 numx;
	u8 temp;
	u8 min;
	u8 sec;
	u8 mms;			   
    lrc_tag_cnt=0;   //tagÊıÇå¿Õ
    lrctagtype=0;    //ÀàĞÍÇå¿Õ	  
	Get_Byte(1);     //¸´Î» 
	while(1)
	{
		if(lrc_tag_cnt>426)break;//³¬¹ıÁË×î´ó´æ´¢¿Õ¼ä,ºóĞø²»ÔÚ±£´æ
		while(1)//Ò»Ö±µÈµ½µÃµ½tag¿ªÊ¼±êÖ¾
		{
			temp=Get_Byte(0);  	   		 
			if(temp=='[')break;
			else if(temp==0xff)
			{
				if(lrc_tag_cnt)return 1;//ËùÓĞÊı¾İ¶ÁÈ¡½áÊøÁË£¬Æ«ÒÆ×Ö½ÚÔİ¶¨Îª20	
				else return 0;//»ñÈ¡tagÊ§°Ü	·Ç·¨µÄlrcÎÄ¼ş	 
			}
		}
		numx=Get_Num();//µÃµ½µÚÒ»¸öÊı×Ö	  
		if(Get_Byte(0)!=':'||numx==0xff)continue;
		min=numx;
		numx=Get_Num();//µÃµ½µÚ¶ş¸öÊı×Ö
		temp=Get_Byte(0);
		if(temp!='.'&&temp!=']'||numx==0xff)continue;
		sec=numx;		  					    
		if(temp!='.')//[XX:XX]¸ñÊ½
		{
			tag_table[i]=min;
			tag_table[i+1]=sec;
			i+=2;
			lrctagtype=0; //xx:xx¸ñÊ½
			lrc_tag_cnt++;//¼ÇÂ¼tagÊı
			continue;
		}
		numx=Get_Num();//µÃµ½µÚÈı¸öÊı×Ö
		temp=Get_Byte(0);
		if(temp!=']'||numx==0xff)continue;//[XX:XX.XX]¸ñÊ½
		mms=numx;
		lrctagtype=1;  //xx:xx.xx¸ñÊ½
		lrc_tag_cnt++; //¼ÇÂ¼tagÊı
		tag_table[i]=min;
		tag_table[i+1]=sec;
		tag_table[i+2]=mms;
		i+=3;									 
	}
	return 1;
}
 
//³õÊ¼»¯ËùÓĞtag
//°´ÉıĞòÅÅÁĞ    
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
	do//½«Êı¾İXÉıĞòÅÅÁĞ
	{	
		t1=0;		  
		for(t=0;t<tagcnt;t+=offset)
		{
			if(tag_table[t]>tag_table[t+offset])//·ÖÖÓÊı´óÓÚºóÕß,µ÷»»ËùÓĞ
			{
				temp=tag_table[t+offset];		  //·ÖÖÓµ÷»»
				tag_table[t+offset]=tag_table[t];
				tag_table[t]=temp;

				temp=tag_table[t+offset+1];		  //ÃëÖÓµ÷»»
				tag_table[t+offset+1]=tag_table[t+1];
				tag_table[t+1]=temp;
				if(offset==3)
				{
					temp=tag_table[t+5];		  //mmµ÷»»
					tag_table[t+5]=tag_table[t+2];
					tag_table[t+2]=temp;   
				}	   
				t1=1; 	//ÓĞ¸Ä±ä
			}else if(tag_table[t+1]>tag_table[t+offset+1]&&tag_table[t]==tag_table[t+offset])//ÃëÖÓÊı´óÓÚºóÕß,µ÷»»ºóÁ½Î»
			{
				temp=tag_table[t+offset+1];		  //ÃëÖÓµ÷»»
				tag_table[t+offset+1]=tag_table[t+1];
				tag_table[t+1]=temp;

				if(offset==3)
				{
					temp=tag_table[t+5];		  //mmµ÷»»
					tag_table[t+5]=tag_table[t+2];
					tag_table[t+2]=temp;   
				}	   
				t1=1; 	//ÓĞ¸Ä±ä
			}else if(offset==3)	//XX:XX.XX
			{
				if(tag_table[t+2]>tag_table[t+5]&&tag_table[t]==tag_table[t+3]&&tag_table[t+1]==tag_table[t+4])//mmÊı´óÓÚºóÕß,µ÷»»ºóÁ½Î»
				{						 
					temp=tag_table[t+5];		  //mmµ÷»»
					tag_table[t+5]=tag_table[t+2];
					tag_table[t+2]=temp;
					t1=1; 	//ÓĞ¸Ä±ä
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
//ÔÚµ±Ç°µÄÄ¿Â¼ÖĞ²éÕÒ¸è´ÊÎÄ¼ş,ÕÒµ½ÖÃÏà¹ØÊı¾İÖÁlyricÖĞ
//Èë¿Ú: 1.dircluster    µ±Ç°Ä¿Â¼µÄÊ×´ØºÅ,ÈôÊÇFAT12»òFAT16µÄ¸ùÄ¿Â¼ÔòÎª0
//Èç¹ûÕÒµ½,Ôò·µ»Ø1
//Ã»ÕÒµ½,·µ»Ø0
u8 FindLrcFile(unsigned long dircluster,unsigned char *strName)
{							  
	unsigned char  name[8];       
	u16 lrc_cnt=0;//µ±Ç°Ä¿Â¼ÏÂLRCÎÄ¼şµÄ¸öÊı
	u16 i;
	u8 t;							    
	FileInfoStruct *FileInfo;

	//printf("START SEARCH LRC:%s\n\n",strName);
	for(i=0;i<8;i++)name[i]=strName[i];//È¡Ç°Ãæ°Ë¸ö×Ö½Ú   	 						   
	lrc_cnt=0;	  
	FileInfo=&F_Info[2];//¿ª±ÙLRCÔİ´æ¿Õ¼ä.
	Get_File_Info(Cur_Dir_Cluster,FileInfo,T_LRC,&lrc_cnt);//»ñÈ¡µ±Ç°ÎÄ¼ş¼ĞÏÂÃæµÄlrcÎÄ¼ş¸öÊı  	  
	for(i=1;i<=lrc_cnt;i++)
	{
		Get_File_Info(Cur_Dir_Cluster,FileInfo,T_LRC,&i);//µÃµ½Õâ¸öLRCµÄĞÅÏ¢	 
        for(t=0;t<8;t++)if(name[t]!=FileInfo->F_ShortName[t])break;                    
        if(t==8) //ÕÒµ½¸è´ÊÎÄ¼şÁË
		{	
			//printf("Find Matched LRC\n");									  
			Get_File_Info(Cur_Dir_Cluster,LrcFile,T_LRC,&i);//¶ÁÈ¡ĞÅÏ¢µ½LrcFile	    
			
			//printf("LRC Name:%s\n",LrcFile->F_Name);			
			//printf("LRC Size:%d\n",LrcFile->F_Size);						  
			
			t=Get_Tag();  //µÃµ½tag
			if(t==0)break;//¶ÁÈ¡tagÊ§°ÜÁË
			//printf("Get LRC Tag OK\n");	 
			Init_Tag();	   //¸´ÖÆtag
			//printf("Init Tag OK!!\n");	 	
			CopyLrcToRam();//¸´ÖÆ¸è´Êµ½ram¾
			return 1;  				 
		}
	}   
	return 0;
}  

//LRC¶ÁÈ¡³õÊ¼»¯
void Lrc_Read_Init(void)
{
	lyric.t_sec=0;
	lyric.t_ms=0;    //Ê±¼äÇå¿Õ 
	lyric.tagcnt=0;  //¸è´ÊÃ»ÓĞ½áÊø
	lyric.lrcoff=0;  //Æ«ÒÆ¸´Î»
	lyric.first=1;   //±ê¼ÇÒ»ÏÂ		  
}

//¶ÁÈ¡Ò»¾äÓĞĞ§µÄ¸è´Ê¼ÇÂ¼  
//²¢ÏÔÊ¾
void ReadLrcFileData(void)
{  
	static u8 mutiline=0;
	unsigned char *lbuf=(u8 *)MCUBuffer;//Ò»¾ä¸è´ÊµÄ±£´æÇø¼ä  
	unsigned char c;
	u8 cc;	
	u16 t;	 							     
				   
	if(lyric.tagcnt>=lrc_tag_cnt) return;//¸è´Ê¶ÁÈ¡ÒÑ¾­½áÊøÁË
	 
	lyric.t_sec=0xffff;
	lyric.t_ms=0; //Ê±¼äÇå¿Õ 
	//´Óµ±Ç°µÄÎ»ÖÃ¶ÁÈ¡¸è´ÊÎÄ¼şÖĞµÄ64¸ö×Ö½ÚÖÁ»º³åÇølbuf					   
	t=0;
	if(!lyric.first)//¸è´Ê»¹Ã»ÓĞ¸üĞÂ,ÏÔÊ¾¸è´Ê
	{  		    
		while(1)//¶ÁÈ¡Ò»¾ä¸è´Êµ½lbufÀïÃæ
		{
			c=ReadByteLrcBuf(lyric.lrcoff++);//È¡³öÒ»¸ö×Ö½Ú    
			if(c=='[')
			{
				cc=ReadByteLrcBuf(lyric.lrcoff);//ÊÔÌ½ÏÂÒ»¸öÊı¾İ
				if(cc<=9){lyric.lrcoff--;break;}//ÊÔÌ½³É¹¦	
			}
			if(c==0x0d||c==0x0a||c=='\0')break;//¶Áµ½»Ø³µ·ûºÅ/½áÊø·ûºÅ    
			lbuf[t]=c;//¼ÇÂ¼
			t++;         
		}	    
	   	lbuf[t]='\0';//Ìí¼Ó½áÊø·û 
		//POINT_COLOR=LRCCOLOR;
		if(mutiline)DrawBlock(0,154,239,219,BLACK);//²Á³ıËùÓĞ¸è´Ê
		else DrawBlock(0,154,239,170,BLACK);//Ö»Òª²Á³ıÒ»ĞĞ¾ÍokÁË
		
		Show_Str_Mid(0,154,lbuf,WHITE,LRCCOLOR,0X03,240);//ÏÔÊ¾¸è´Ê,×Ô¶¯»»ĞĞ
		if(t>28)mutiline=1;//Òª¶àĞĞÏÔÊ¾ 	   
		else mutiline=0;   //²»Òª¶àĞĞĞĞÏÔÊ¾	  	  			   
	}        
	//¶ÁÈ¡ÏÂÒ»¾ä¸è´Ê
	lyric.first=0;//±ê¼Ç¸è´Ê¸üĞÂ      	    
	while(1)//¸è´ÊĞÅÏ¢¶ÁÈ¡ĞÅÏ¢
	{ 								  
		c=ReadByteLrcBuf(lyric.lrcoff++);//¶ÁÈ¡TAGµÄµÚÒ»¸ö×Ö½Ú    
		if(c=='[')//¶ÁÈ¡Ò»¸öTAG 
		{
			lyric.tagcnt++;//tag±ê¼Ç
			lyric.t_sec=ReadByteLrcBuf(lyric.lrcoff++);//¶ÁÈ¡·ÖÖÓ
			lyric.t_sec*=60;
			lyric.lrcoff++;//Æ«ÒÆµ½ÃëÖÓ
			lyric.t_sec+=ReadByteLrcBuf(lyric.lrcoff++);//¶ÁÈ¡ÃëÖÓ 
			c=ReadByteLrcBuf(lyric.lrcoff++);
			if(c==']')break;//XX:XX¸ñÊ½	    
			//XX:XX.XX ¸ñÊ½	  
			lyric.t_ms=ReadByteLrcBuf(lyric.lrcoff++);  //¶ÁÈ¡mm 	  
		}else if(c==']')break;	 
	} 
}             
//check ok 08/09/06
// ¸è´ÊÏÔÊ¾¿ØÖÆ  
//Í¬Ê±¿ØÖÆÏÔÊ¾½âÂëÊ±¼ä   
//mnum µ±Ç°ÎÄ¼şµÄĞòºÅ 0~3
//×¢Òâ:Ö»Ö§³Öµ¥ÅÅÊ±¼äµÄlrcÕı³£ÏÔÊ¾
void LyricDisplayCtrl(void)
{  
	static unsigned int time=0;//time=0xffff;
	static unsigned char ms=0; //ÓÃÀ´±ê¼Ç½øĞĞÁË¶àÉÙ¸ö10ms    
	unsigned int t1;  
	if(!bt30MS)  return;//30msÎ´µ½
	ms+=3;//ms++
	bt30MS=0;   
    t1=GetDecodeTime(); //¶ÁÈ¡²¥·ÅÊ±¼ä  
	if(t1!=time)//ÃëÖÓ²»ºÏ
	{
		time=t1;
		ms=0;//Í¬²½Ê±¼ä   
		return;
	}  											  
	if(((t1==lyric.t_sec)&&(ms>=lyric.t_ms))||(t1>lyric.t_sec))//Ê±¼ä·ûºÏ,ÏÔÊ¾¸è´Ê
	{    
		ReadLrcFileData();  //¶ÁÈ¡¸èÇúÊı¾İ²¢ÏÔÊ¾
	}   
} */

	 
//¶¨Ê±Æ÷3ÖĞ¶Ï·şÎñ³ÌĞò
//±ê¼Ç30msÖĞ¶ÏÊ±¼äµ½.
void TIM3_IRQHandler(void)
{ 					 		    
	if(TIM3->SR&0X0001)bt30MS=1;//Òç³öÖĞ¶Ï	    		   
	TIM3->SR&=~(1<<0);//Çå³ıÖĞ¶Ï±êÖ¾Î» 	 
}
//TIMER3ÖĞ¶ÏÓÅÏÈ¼¶Éè¶¨
//ÔÚmp3ÀïÃæÓÅÏÈ¼¶×î¸ß						  	   
void NVIC_TIMER3Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//´æ´¢Æ÷Ó³Éä,²»ÓÃÀí    
#ifdef  VECT_TAB_RAM  									   
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else   							 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);		 
	//µÚ0×é,Ã»ÓĞÇÀ¶ÏÓÅÏÈ¼¶
	//µÚ4×é,Ã»ÓĞÏìÓ¦ÓÅÏÈ¼¶				  
	//µÚ2×é,ÓĞ4¸öÇÀ¶Ï,4¸öÏìÓ¦
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ÓÅÏÈ¼¶·Öµ½µÚ2×é ×Ü¹²5×é		 
  	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel; //Ê¹ÓÃÍâ²¿ÖĞ¶Ï1
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//½×¼¶1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 //½×²ã0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure); 
	//ÉèÖÃTIM3µÄÖĞ¶Ï 												  		 		   	 
  	NVIC_Init(&NVIC_InitStructure);        
#endif  				  					   							 
}   
//³õÊ¼»¯ºìÍâ½ÓÊÕÒı½ÅµÄÉèÖÃ
//¿ªÆôÖĞ¶Ï,²¢Ó³Éä 
void timer3_init(void)
{	
	//´Ë²¿·ÖĞèÊÖ¶¯ĞŞ¸ÄIO¿ÚÉèÖÃ
	RCC->APB1ENR|=1<<1;//TIM3Ê±ÖÓÊ¹ÄÜ    
  	NVIC_TIMER3Configuration();										 
	TIM3->ARR=300;   //Éè¶¨¼ÆÊıÆ÷×Ô¶¯ÖØ×°Öµ//¸ÕºÃ30ms 
	TIM3->PSC=7199;  //Ô¤·ÖÆµÆ÷7200,µÃµ½10KhzµÄ¼ÆÊıÊ±ÖÓ
	//ÕâÁ½¸ö¶«¶«ÒªÍ¬Ê±ÉèÖÃ²Å¿ÉÒÔÊ¹ÓÃÖĞ¶Ï
	TIM3->DIER|=1<<0;   //ÔÊĞí¸üĞÂÖĞ¶Ï
	TIM3->DIER|=1<<6;   //ÔÊĞí´¥·¢ÖĞ¶Ï

	TIM3->CR1=0x0080;  //ARPEÊ¹ÄÜ 
	TIM3->CR1|=0x01;   //Ê¹ÄÜ¶¨Ê±Æ÷3					 					  										  
}
































