//MP3 功放程序 GUI部分
//QiYuan@SCUT
//2010/07/09
#include "main.h"

extern u8 invt;
//主菜单
void Load_Sys_ICO()
{
	Cleardisplay(ALL);
	if(key_point<4)
	{
		Show_str(0,0,"1.MP3 Player");		//显示fat类型 
	   	Show_str(1,0,"2.TXT Reader");		//显示fat类型
		Show_str(2,0,"3.Show Time");		//显示fat类型
		Show_str(3,0,"4.Show FFT");			//显示fat类型
		Show_str(key_point,104,"<-");			//显示fat类型	
	}
	else if(key_point>=4&&key_point<8)
	{
		LCD_write_String(0,0,"5.贪食蛇",0);		//显示fat类型 
		LCD_write_String(1,0,"7.推箱子",0);		//显示fat类型 
		LCD_write_String(2,0,"6.俄罗斯方块",0);		//显示fat类型 
		LCD_write_String(3,0,"8.系统信息",0);		//显示fat类型 
		Show_str(key_point%4,104,"<-");			//显示fat类型	
	}
}


 //加载MP3播放界面
//sysfun变量。0~9bit用来标记不同状态
void Load_Gui_Mp3(u16 funmark)
{	 
}


//显示文件
//fcnt:要显示文件的多少
//sel :选中第几个1~4 
//返回值:要打开的文件编号1~8(包括文件夹,在FileInfo里面的)	    
//返回9,代表直接退回上一层目录  		    
//显示文件
//check ok 09/04/19
u8 Show_File(u8 fcnt,u8 sel)
{
	u8 t;
	unsigned char Ico=0;					   
	//FileInfoStruct temp;
	//temp.F_Type=T_BMP;//文件属性设置

	sel-=1;										 
	for(t=0;t<fcnt;t++)
	{
		if(F_Info[t].F_Type==T_FILE) Ico=4;//文件夹图标
		else 
		{
			if(F_Info[t].F_Type&0x0F)Ico=0;//音乐类图标
			else if(F_Info[t].F_Type&0xF0)Ico=1;//TXT类图标
		}
		if(t==sel) invt=1;
		else invt=0;
		Show_icon(t,0,Ico);//显示图标	 
		LCD_write_String(t,12,F_Info[t].F_Name,0);//不换行      
	}
	invt=0;
	while(1)//等待按键操作
	{
		while(key==255);
		if(key!=255)							//有按键按下  
		{
			switch(key)
			{
				case UP:	
							invt=0;Show_icon(sel,0,Ico);//显示图标
						   	if(sel==0)sel=3;	
							else sel--;
							invt=1;Show_icon(sel,0,Ico);//显示图标
							break;
				case DOWN:	
							invt=0;Show_icon(sel,0,Ico);//显示图标
							if(sel==3)sel=0;
							else sel++;
							invt=1;Show_icon(sel,0,Ico);//显示图标
							break;
				case KEY_A:{key=255;return sel+1;}//确认键，打开
				case KEY_B://选择了"返回"按钮 
					key=255;return 9;
				case LEFT:  //向上翻页操作 
					key=255;return 10;
				case RIGHT:  //向下翻页操作 
					key=255;return 11;
			}			  
		}
		key=255;//把按键值取消掉		
	}
}

//浏览文件夹下面的文件
//file_type:要浏览的文件类型
//返回要打开的目标文件编号 1~n(在总目标文件里面的)
//不包括文件夹!!!
//如果返回0则代表退出浏览
u16 File_Browse(u16 file_type)
{			 
	FileInfoStruct *FileInfo;
	u16 folder_cnt;//文件夹的个数
	u16 des_cnt;   //目标文件的个数		    
	u8 fcnt;	   //显示的文件数目
	u16 temp; 					     									  

	u16 total_file;//总文件数目
	u16 pos_cnt;//当前文件号 1~folder_cnt+des_cnt
	u8 temp_key;	  			  
							   			  
Reset:				    
	folder_cnt=0;des_cnt=0;fcnt=0;temp_key=0;pos_cnt=0;	 	
	Get_File_Info(Cur_Dir_Cluster,FileInfo,T_FILE,&folder_cnt);//得到当前目录下文件夹的个数	    
	Get_File_Info(Cur_Dir_Cluster,FileInfo,file_type,&des_cnt);//得到当前目录下目标文件的个数	 
									   
	total_file=des_cnt+folder_cnt;//总的文件数目=文件夹数目+目标文件数目
	while(1)
	{
		if(pos_cnt<total_file)//文件标尺,未到达文件末尾
		{	   
			FileInfo=&F_Info[fcnt];
			if(pos_cnt<folder_cnt)//找文件夹
			{
			    temp=pos_cnt+1;			    
				Get_File_Info(Cur_Dir_Cluster,FileInfo,T_FILE,&temp);
			}else				   //找目标文件
			{
				temp=pos_cnt-folder_cnt+1; 
				Get_File_Info(Cur_Dir_Cluster,FileInfo,file_type,&temp);
			}
			fcnt++;   //已存的文件数目增加
			pos_cnt++;//文件位置标志增加
		}	  
		if(fcnt==4||pos_cnt==total_file)//找满四个了,或者文件夹已经找完了
		{  
			temp_key=Show_File(fcnt,1);//显示此次得到的文件
			switch(temp_key)
			{
				case 1://要打开目标文件
				case 2:
				case 3:
				case 4:
					temp_key-=1;
					if(temp_key<fcnt)//按键在有效范围内
					{
						if(F_Info[temp_key].F_Type==T_FILE)//要打开的是个文件夹
						{
							Cur_Dir_Cluster=F_Info[temp_key].F_StartCluster;//打开这个文件夹		  
							goto Reset;
						}else    //要打开的是目标文件
						{		   
							return pos_cnt+temp_key-folder_cnt-fcnt+1;//返回目标文件编号
						}
					}
					break;
				case 9:	//选择了返回按钮.退出到上一层目录
					//在主目录浏览里面,按下返回按钮,则退出浏览模式	
					if(((fat_type==32)&&Cur_Dir_Cluster==root_dir_clu)||Cur_Dir_Cluster==0)return 0;//退出浏览模式	  					
					Cur_Dir_Cluster=Fat_Dir_Cluster;//退到父目录  
					Cleardisplay(ALL); //清13164显示 			    	 
					goto Reset;	   	 
				case 10:  //向上翻页
					if(pos_cnt>4)pos_cnt-=(4+fcnt);//回到上一页
					else pos_cnt=0;//从头开始了
					Cleardisplay(ALL); //清13164显示 
					break;
				case 11:	//向下翻页
					if(pos_cnt==total_file)pos_cnt-=fcnt;//重新找
					Cleardisplay(ALL); //清13164显示 
					break;

			}	 				    
			fcnt=0;//清除显示文件计数器
		}
	}		
}

