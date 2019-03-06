#include "main.h"	
#include "unicodeToGB2312.h"   
u16 UnicodeToGB2312(u16 unicode)//�ö��ֲ����㷨
{
   	int mid, low, high, len;
	len = sizeof(UnicodeToGB2312_Tab)/sizeof(UnicodeToGB2312_Tab[0]);
	low = 0;
	high = len - 1;
	if( (unicode >= 0x20 && unicode <= 0x5b) || (unicode >= 0x5d && unicode <= 0x7e))
	   return unicode;
	while(low <= high)
	{
	    mid = (low + high) / 2;
		if(UnicodeToGB2312_Tab[mid][0] > unicode)
		    high = mid - 1;
		else if(UnicodeToGB2312_Tab[mid][0] < unicode)
		    low =  mid + 1;
		else
		    return 	UnicodeToGB2312_Tab[mid][1];
	}
	return 0 ; //û�ҵ�
}						 
//ע�⣬����������unicode�벢�������еĶ��ܽ�� 
//��͵������ļ������治����unicode���ַ�
//����ת��unit code-> gb2312
void UniToGB(u8 *pbuf)
{   					  
	unsigned int  code;
	unsigned char i,m=0; 
	for(i=0;i<80;i++)//�80���ַ�
	{	  
		code= pbuf[i*2+1]*256+pbuf[i*2]; 
		if((code==0)||(code==0xffff))break;
		if((code&0xff00)==0)//��ĸ
		{
			if((code>=0x20)&&(code<=0x7e))
			{
				pbuf[m++]=(unsigned char)code;              
			}else pbuf[m++]='?';//�޷�ʶ����ã����� 
			continue;
		}
		if(code>=0x4e00) //�Ǻ���
		{   
			code=UnicodeToGB2312(code);//��unicodeת��Ϊgb2312 
			pbuf[m++]=code>>8;
			pbuf[m++]=(u8)code; 
		}else pbuf[m++]='?';//�޷�ʶ����ã�����  
	}    
	pbuf[m]='\0';//��ӽ�������  			      
}
