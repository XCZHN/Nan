/*===============================================================================================
==                                                                                              
**  Copyright(c) 2014  ZhenYao Science and technology co., LTD.   All rights reserved.
==                                                                                           
**	Author:		Asen                                                                     
==                                                                                          
**  Version:	v2.0.0                                                                          
==
**  Filename:	App_GUI.c                                                                       
==                                                                                         
**  Function:	显示界面程序
==
**  Data:       2017/04/18                                                                      
================================================================================================*/


#include "App_GUI.h"



#ifdef GUI_ENGLISH
#include "app_Notice_EN.h"

#else
#include "app_Notice_CH.h"

#endif



LCM_STR_UI LCM_Unit_UI[UNIT_UI_Num];
LCM_STR_UI PromptBigBox = {40, 10, 200, 310};
LCM_STR_UI Promptbox = {50, 50, 189, 289};

char cStr_Message[100];
uchar8 MenuIconBuf[UNIT_UI_Num];

/*******************************************************************************************************
* Description  : 显示字符串 
********************************************************************************************************/
void  LCD_DisplayStr(uint16 s_x, uint16 s_y, uchar8 Font, const char* pStr, uint16 FColor)
{
	DisplayString(Font, s_x, s_y, pStr, FColor);
}

/*******************************************************************************
* Function Name  : GUI_PromptBox(const char *pStr_Title)
* Description    : 绘制小提示框
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_PromptBox(const char *pStr_Title)
{
	u32 strlength=(strlen((char*)pStr_Title))/2;
	Promptbox.pStr = (char*)pStr_Title;
	
	LCD_Fill(Promptbox.s_x, Promptbox.s_y, Promptbox.e_x, Promptbox.e_y, LCD_Color_Back);
	LCD_Fill(Promptbox.s_x, Promptbox.s_y, Promptbox.s_x+FONT_SIZE24, Promptbox.e_y, LCD_Color_Blue);
	LCD_DisplayStr(Promptbox.s_x+4, ((Promptbox.e_y - Promptbox.s_y) - FONT_SIZE16*strlength)/2 + FONT_SIZE16*strlength + Promptbox.s_y, FONT_SIZE16, Promptbox.pStr, LCD_Color_White);
}

/*******************************************************************************
* Function Name  : GUI_PromptBigBox(const char *pStr_Title)
* Description    : 绘制大提示框
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_PromptBigBox(const char *pStr_Title)
{
	u32 strlength=(strlen((char*)pStr_Title))/2;
	PromptBigBox.pStr = (char*)pStr_Title;
	
	LCD_Fill(PromptBigBox.s_x, PromptBigBox.s_y, PromptBigBox.e_x, PromptBigBox.e_y, LCD_Color_Back);
	LCD_Fill(PromptBigBox.s_x, PromptBigBox.s_y, PromptBigBox.s_x+FONT_SIZE24, PromptBigBox.e_y, LCD_Color_Blue);
	LCD_DisplayStr(PromptBigBox.s_x+4, ((PromptBigBox.e_y - PromptBigBox.s_y) - FONT_SIZE16*strlength)/2 + FONT_SIZE16*strlength + PromptBigBox.s_y, 
		           FONT_SIZE16, PromptBigBox.pStr, LCD_Color_White);
}

/*******************************************************************************
* Function Name  : GUI_UpdateDisplay(LCM_UI *ppLCM_UI,char start, char end)
* Description    : 更新界面显示
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_UpdateDisplay(LCM_STR_UI *pStrUi,char start, char end)
{
	u32 i;
	
	for(i=start;i<=end;i++)
	{
		LCD_ShowIcon(pStrUi[i].s_x, pStrUi[i].e_y, pStrUi[i].e_x-1, pStrUi[i].s_y-1, pStrUi[i].PicNum);
		LCD_DisplayStr(pStrUi[i].s_x+48+6, pStrUi[i].s_y, pStrUi[i].Font, pStrUi[i].pStr, pStrUi[i].FColor);
	}
}

/*******************************************************************************
* Function Name  : GUI_ShowListMenu
* Description    : 显示列表菜单 
* Input          : pMenu: 菜单指针, ListCnt:菜单选项数(不包含标题栏)
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_ShowListMenu(const char* pMenu[], uchar8 ListCnt)
{
	u32 strlength, i;
	u16 x_addr = 0;
	strlength = (strlen(pMenu[0]))/2;

	LCD_Fill(0, 0, FONT_SIZE24, LCD_WIDE, LCD_Color_Blue);
	LCD_Fill(FONT_SIZE24, 0, LCD_HIGH, LCD_WIDE, LCD_Color_White);
	LCD_DisplayStr(4, (LCD_WIDE - FONT_SIZE16*strlength)/2+FONT_SIZE16*strlength, FONT_SIZE16, pMenu[0], LCD_Color_White);

	x_addr = FONT_SIZE24 +FONT_SIZE16 +6; 	
	for(i=0; i<8; i++)
	{
		LCD_DrawLine(x_addr, LCD_WIDE_START, x_addr, LCD_WIDE_END, LCD_Color_GrayBlue);
		x_addr += 27;
	}
	
	if(ListCnt > 8)
	{
		ListCnt = 8;
	}
	
	x_addr = FONT_SIZE24 +4; 	
	for(i=0; i<ListCnt; i++)
	{
		LCD_DisplayStr(x_addr, 280, FONT_SIZE16, pMenu[1+i], LCD_Color_Blue);
		x_addr += 27;
	}
}

/*******************************************************************************
* Function Name  : GUI_ShowIconMenu
* Description    : 显示菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_ShowIconMenu(const char* pMenu[], uchar8* pIconBuf, uchar8 TabCnt)
{
	u32 strlength, i;
	u16 x_size,y_size;
	
	strlength =(strlen(pMenu[0]))/2;
	
	memset(&LCM_Unit_UI[0], 0, UNIT_UI_Num);
	
	LCD_Fill(0,0,FONT_SIZE24,LCD_WIDE,LCD_Color_Blue);
	LCD_Fill(FONT_SIZE24,0,LCD_HIGH,LCD_WIDE,LCD_Color_White);
	LCD_DisplayStr(4, (LCD_WIDE - FONT_SIZE16*strlength)/2+FONT_SIZE16*strlength, FONT_SIZE16, (char *)pMenu[0], LCD_Color_White);

	x_size =(u16)(LCD_HIGH-FONT_SIZE24-2*FONT_SIZE16-2*HALF_ICON_Xsize)/3;
	y_size =(u16)(LCD_WIDE-4*HALF_ICON_Ysize)/5;

	//第一排菜单
	for(i=1;i<5;i++)
	{
		LCM_Unit_UI[i].s_x =(u16)( x_size+FONT_SIZE24);
		LCM_Unit_UI[i].s_y =(u16)(y_size+HALF_ICON_Ysize)*(5-i);
		LCM_Unit_UI[i].e_x =LCM_Unit_UI[i].s_x+HALF_ICON_Xsize;
		LCM_Unit_UI[i].e_y =LCM_Unit_UI[i].s_y-HALF_ICON_Ysize;
		LCM_Unit_UI[i].FColor =LCD_Color_Black;
		LCM_Unit_UI[i].Font =FONT_SIZE16;
		LCM_Unit_UI[i].pStr =(char *)pMenu[i];
		
		LCM_Unit_UI[i].PicNum = pIconBuf[i];	

		if(i >= TabCnt)
		{
			break;
		}
	}

	if(TabCnt >= 5)
	{
		//第二排菜单
		for(i=5;i<9;i++)
		{
			LCM_Unit_UI[i].s_x =(u16)(x_size*2+HALF_ICON_Xsize+FONT_SIZE24+FONT_SIZE16);
			LCM_Unit_UI[i].s_y =(u16)(y_size+HALF_ICON_Ysize)*(5-i+4);
			LCM_Unit_UI[i].e_x =LCM_Unit_UI[i].s_x+HALF_ICON_Xsize;
			LCM_Unit_UI[i].e_y =LCM_Unit_UI[i].s_y-HALF_ICON_Ysize;
			LCM_Unit_UI[i].FColor =LCD_Color_Black;
			LCM_Unit_UI[i].Font =FONT_SIZE16;
			LCM_Unit_UI[i].pStr =(char *)pMenu[i];
			
			LCM_Unit_UI[i].PicNum = pIconBuf[i];

			if(i >= TabCnt)
			{
				break;
			}
		}
	}

	GUI_UpdateDisplay(&LCM_Unit_UI[0], 1, TabCnt);
}

/*******************************************************************************
* Function Name  : GUI_ShowMenu
* Description    : 显示菜单界面
* Input          : pMenu:菜单列表, TabCnt:菜单选项数量
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_ShowMenu(const char* pMenu[], uchar8* pIconBuf, uchar8 TabCnt)
{
#ifdef GUI_ENGLISH
	GUI_ShowListMenu(pMenu, TabCnt);
#else
	GUI_ShowIconMenu(pMenu, pIconBuf, TabCnt);
#endif
}

/*******************************************************************************
* Function Name  : GUI_ShowMessage
* Description    : 提示消息
* Input          : pstr--要显示的字符串指针, color--颜色
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_ShowMessage(const char* pstr, uint16 color)
{
	u32 strlength = strlen(pstr); 

	GUI_PromptBox(pStr_OperationTip);

	LCD_DisplayStr((Promptbox.e_x-Promptbox.s_x)/2+Promptbox.s_x-FONT_SIZE24/2, (Promptbox.e_y-Promptbox.s_y)/2+Promptbox.s_y+((FONT_SIZE24*strlength)/2)/2, FONT_SIZE24, (char*)pstr, color);

}

/*******************************************************************************
* Function Name  : GUI_ShowTowRowMessage
* Description    : 提示两行消息
* Input          : pstr--要显示的字符串指针, color--颜色
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_ShowTowRowMessage(const char* pstr, const char* pstr02, uint16 color)
{
	u32 strlength = strlen(pstr); 
	u32 strlength02 = strlen(pstr02); 

	GUI_PromptBox(pStr_OperationTip);

	LCD_DisplayStr(100, (Promptbox.e_y-Promptbox.s_y)/2+Promptbox.s_y+((FONT_SIZE24*strlength)/2)/2, FONT_SIZE24, (char*)pstr, color);
	LCD_DisplayStr(150, (Promptbox.e_y-Promptbox.s_y)/2+Promptbox.s_y+((FONT_SIZE24*strlength02)/2)/2, FONT_SIZE24, (char*)pstr02, color);
}

/*******************************************************************************
* Function Name  : GUI_ShowSysVersion
* Description    : 显示系统版本
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_ShowSysVersion(void)
{
	extern const char  ProgramVer_Tab[];
	extern const char  Firmware_Tab[];
	u32 strlength = strlen((char*)ProgramVer_Tab); 

	GUI_PromptBox(pStr_SysVersion);

	LCD_DisplayStr(Promptbox.s_x + FONT_SIZE32, Promptbox.e_y-FONT_SIZE16, FONT_SIZE24, ProgramVer_Tab, LCD_Color_Red);
	LCD_DisplayStr(Promptbox.s_x +(FONT_SIZE24)*3, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, Firmware_Tab, LCD_Color_Blue);
	LCD_DisplayStr(Promptbox.s_x + 110, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, pStr_Company, LCD_Color_Red);
}

/*******************************************************************************
* Function Name  : GUI_ChoiceKeyboardType
* Description    : 提示选择键盘的类型
* Input          : None
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
void GUI_ChoiceKeyboardType(void)
{
	GUI_PromptBox(pMenu_KeyType[0]);

    LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16), Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, (char*)pMenu_KeyType[1], LCD_Color_Blue);
	LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16)*2, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, (char*)pMenu_KeyType[2], LCD_Color_Blue);
    LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16)*3, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, (char*)pMenu_KeyType[3], LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_InputUseDate
* Description    : 显示输入某个箱的使用日期
* Input          : box -- 箱号
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
void GUI_InputUseDate(uchar8 box, uchar8* pDateStr)
{
	char DisplayBuf[100] = {0};
	strcpy(DisplayBuf, pStr_PlsEnterValidity);
	
	GUI_PromptBox(DisplayBuf);
	LCD_DisplayStr(110, 250, FONT_SIZE32, (char*)pDateStr, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_InputBoxID
* Description    : 输入箱号
* Input          : None
* Output         : None
* Return         : None
* Notes          : 在一个提示框中输入数字
*******************************************************************************/
void GUI_InputBoxID(uchar8 BoxID)
{
	char DisplayBuf[100] = {0};
	u32 strlength = 0; 
	
	GUI_PromptBox(pStr_PlsInputBoxID);
	
	strlength = UInt_To_Ascii(BoxID, (uchar8*)DisplayBuf);
	LCD_DisplayStr((Promptbox.e_x-Promptbox.s_x)/2+Promptbox.s_x -24, ((Promptbox.e_y-Promptbox.s_y)/2) + Promptbox.s_y + ((FONT_SIZE64*strlength)/2)/2, FONT_SIZE64, DisplayBuf, LCD_Color_Red);
}

/*******************************************************************************
* Function Name  : GUI_AdminSetBoxNum
* Description    : 管理员设置箱数
* Input          : type -- 类型(0--待选, 1--主柜, 2--副柜1...); BoxNum -- 箱数
* Output         : None
* Return         : None
* Notes          : 在一个提示框中输入数字
*******************************************************************************/
void GUI_AdminSetBoxNum(uchar8 type, uchar8 BoxNum)
{
	char DisplayBuf[10] = {0};
	u32 strlength = 0; 

	if(0 == type)
	{
		GUI_PromptBox(pStr_PlsChoiceType);
	
		memset(cStr_Message, 0x0, sizeof(cStr_Message));
		strcpy(cStr_Message, pMenu_SetBoxNum[1]);
		LCD_DisplayStr(Promptbox.s_x+40, Promptbox.e_y-FONT_SIZE32, FONT_SIZE24, cStr_Message, LCD_Color_Blue);		

		memset(cStr_Message, 0x0, sizeof(cStr_Message));
		strcpy(cStr_Message, pMenu_SetBoxNum[2]);
		LCD_DisplayStr(Promptbox.s_x+90, Promptbox.e_y-FONT_SIZE32, FONT_SIZE24, cStr_Message, LCD_Color_Blue);		
	}
	else if(1 == type)  //主柜
	{	
		memset(cStr_Message, 0x0, sizeof(cStr_Message));
		strcpy(cStr_Message, pStr_PlsInput);
		strcat(cStr_Message, &pMenu_SetBoxNum[1][2]);
		GUI_PromptBox(cStr_Message);
		
		strlength = UInt_To_Ascii(BoxNum, (uchar8*)DisplayBuf);
		LCD_DisplayStr((Promptbox.e_x-Promptbox.s_x)/2+Promptbox.s_x -24, ((Promptbox.e_y-Promptbox.s_y)/2) + Promptbox.s_y + ((FONT_SIZE64*strlength)/2)/2, FONT_SIZE64, DisplayBuf, LCD_Color_Red);
	}
	else if(2 == type)  //提示选择副柜号
	{
		GUI_PromptBox(pStr_PlsChoiceType);

		memset(cStr_Message, 0x0, sizeof(cStr_Message));
		strcpy(cStr_Message, (char*)&pMenu_SetBoxNum[3][0]);
		LCD_DisplayStr(Promptbox.s_x+34, Promptbox.e_y-FONT_SIZE32, FONT_SIZE16, cStr_Message, LCD_Color_Blue);		

		memset(cStr_Message, 0x0, sizeof(cStr_Message));
		strcpy(cStr_Message, (char*)&pMenu_SetBoxNum[4][0]);
		LCD_DisplayStr(Promptbox.s_x+66, Promptbox.e_y-FONT_SIZE32, FONT_SIZE16, cStr_Message, LCD_Color_Blue); 	

		memset(cStr_Message, 0x0, sizeof(cStr_Message));
		strcpy(cStr_Message, (char*)&pMenu_SetBoxNum[5][0]);
		LCD_DisplayStr(Promptbox.s_x+98, Promptbox.e_y-FONT_SIZE32, FONT_SIZE16, cStr_Message, LCD_Color_Blue); 	
	}
	else if((type >= 3)&&(type <= 6)) //副柜
	{
		memset(cStr_Message, 0x0, sizeof(cStr_Message));
		strcpy(cStr_Message, pStr_PlsInput);
		strcat(cStr_Message, (char*)&pMenu_SetBoxNum[type][2]);
		GUI_PromptBox(cStr_Message);
		
		strlength = UInt_To_Ascii(BoxNum, (uchar8*)DisplayBuf);
		LCD_DisplayStr((Promptbox.e_x-Promptbox.s_x)/2+Promptbox.s_x -24, ((Promptbox.e_y-Promptbox.s_y)/2) + Promptbox.s_y + ((FONT_SIZE64*strlength)/2)/2, FONT_SIZE64, DisplayBuf, LCD_Color_Red);
	}
}

/*******************************************************************************
* Function Name  : GUI_Open1Box
* Description    : 开箱提示
* Input          : u8BoxNum -- 箱号  AddNote -- 附加加提示信息
* Output         : None
* Return         : None
* Notes          : AddNote : 1 -- 请随手关门
                             2 -- 按<#>停止开箱
*******************************************************************************/
void GUI_Open1Box(uchar8 u8BoxNum, uchar8 AddNote)
{
	char DisplayBuf[100] = {0};
	u32 strlength = 0; 

	GUI_PromptBox(pStr_OperationTip);

	strlength = strlen((char*)SysConfig.stBox[u8BoxNum].BoxSN);
	if(strlength > MAX_BoxSnLen)
	{
		strlength = MAX_BoxSnLen;
	}
	memcpy(DisplayBuf, (char*)SysConfig.stBox[u8BoxNum].BoxSN, strlength);
	LCD_DisplayStr(100, 260, FONT_SIZE32, DisplayBuf, LCD_Color_Red);
	
	LCD_DisplayStr(100, 250-(strlength*FONT_SIZE16), FONT_SIZE24, (char*)pStr_Open1Box, LCD_Color_Blue);

	if(1 == AddNote)
	{
		LCD_DisplayStr(160, 280, FONT_SIZE16, pStr_PlsCloseBox, LCD_Color_Blue);
	}
	else if(2 == AddNote)
	{
		LCD_DisplayStr(160, 280, FONT_SIZE16, pStr_StopOpenBox, LCD_Color_Blue);
	}
}

/*******************************************************************************
* Function Name  : GUI_Open1BoxFaild
* Description    : 提示开箱失败
* Input          : u8BoxNum -- 箱号  AddNote -- 附加加提示信息
* Output         : None
* Return         : None
* Notes          : AddNote : 1 -- 请随手关门
                             2 -- 按<#>停止开箱
*******************************************************************************/
void GUI_Open1BoxFaild(uchar8 u8BoxNum, uchar8 AddNote)
{
	char DisplayBuf[100] = {0};
	u32 strlength = 0; 

	GUI_PromptBox(pStr_OperationTip);

	strlength = strlen((char*)SysConfig.stBox[u8BoxNum].BoxSN);
	if(strlength > MAX_BoxSnLen)
	{
		strlength = MAX_BoxSnLen;
	}
	memcpy(DisplayBuf, (char*)SysConfig.stBox[u8BoxNum].BoxSN, strlength);
	LCD_DisplayStr(100, 280, FONT_SIZE32, DisplayBuf, LCD_Color_Red);
	
	LCD_DisplayStr(100, 270-(strlength*FONT_SIZE16), FONT_SIZE24, (char*)pStr_xBoxOpenFail, LCD_Color_Blue);

	if(1 == AddNote)
	{
		LCD_DisplayStr(160, 280, FONT_SIZE16, pStr_PlsCloseBox, LCD_Color_Blue);
	}
	else if(2 == AddNote)
	{
		LCD_DisplayStr(160, 280, FONT_SIZE16, pStr_StopOpenBox, LCD_Color_Blue);
	}
}

/*******************************************************************************
* Function Name  : GUI_BoxBeLocked
* Description    : 提示箱已被锁,请联系管理员
* Input          : u8BoxNum -- 箱号 
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_BoxBeLocked(uchar8 box)
{
	char DisplayBuf[100] = {0};
	GUI_PromptBox(pStr_OperationTip);

	memcpy(DisplayBuf, (char*)SysConfig.stBox[box].BoxSN, MAX_BoxSnLen);
	LCD_DisplayStr(100, 260, FONT_SIZE32, DisplayBuf, LCD_Color_Red);	
	LCD_DisplayStr(100, 250-(3*FONT_SIZE16), FONT_SIZE24, pStr_BoxBeLocked, LCD_Color_Blue);

	LCD_DisplayStr(160, 280, FONT_SIZE24, pStr_PlsCallAdmin, LCD_Color_Blue);
}

/*******************************************************************************************************
* Description  : 显示时间
********************************************************************************************************/
void  GUI_Time(void)
{
	char DisplayBuf[100] = {0};
	char tempBuf[20] = {0};

#ifdef GUI_ENGLISH	
	LCD_Fill((LCD_HIGH - FONT_SIZE32), 0, LCD_HIGH, LCD_WIDE, LCD_Color_White);
	sprintf(DisplayBuf, "20%02d-%02d-%02d    %d:%02d:%02d     ", gstSysRunInfo.stRtcTime.u8Year, gstSysRunInfo.stRtcTime.u8Month, gstSysRunInfo.stRtcTime.u8Day,
		                                                           gstSysRunInfo.stRtcTime.u8Hour, gstSysRunInfo.stRtcTime.u8Minute,gstSysRunInfo.stRtcTime.u8Second);
#else
	LCD_Fill((LCD_HIGH - FONT_SIZE32), 0, LCD_HIGH, LCD_WIDE, LCD_Color_White);
	sprintf(DisplayBuf, "20%02d年%02d月%02d日  %d时%02d分%02d秒  ", gstSysRunInfo.stRtcTime.u8Year, gstSysRunInfo.stRtcTime.u8Month, gstSysRunInfo.stRtcTime.u8Day,
		                                                           gstSysRunInfo.stRtcTime.u8Hour, gstSysRunInfo.stRtcTime.u8Minute,gstSysRunInfo.stRtcTime.u8Second);
#endif

	if(1 == gstSysRunInfo.stRtcTime.u8WeekDay)
	{
		strcpy(tempBuf, pMenu_Week[1]);
	}
	else if(2 == gstSysRunInfo.stRtcTime.u8WeekDay)
	{
		strcpy(tempBuf, pMenu_Week[2]);
	}
	else if(3 == gstSysRunInfo.stRtcTime.u8WeekDay)
	{
		strcpy(tempBuf, pMenu_Week[3]);
	}
	else if(4 == gstSysRunInfo.stRtcTime.u8WeekDay)
	{
		strcpy(tempBuf, pMenu_Week[4]);
	}
	else if(5 == gstSysRunInfo.stRtcTime.u8WeekDay)
	{
		strcpy(tempBuf, pMenu_Week[5]);
	}
	else if(6 == gstSysRunInfo.stRtcTime.u8WeekDay)
	{
		strcpy(tempBuf, pMenu_Week[6]);
	}
	else 
	{
		strcpy(tempBuf, pMenu_Week[0]);
	}
	strcat(DisplayBuf, tempBuf);
	LCD_DisplayStr((LCD_HIGH - FONT_SIZE24), (LCD_WIDE - FONT_SIZE24), FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_SetTime
* Description    : 设置时间
* Input          : pDateStr --YY-MM-DD字符串指针 , pTimeStr--HH;MM;SS字符串指针
* Output         : None
* Return         : None
* Notes          : 在下边框显示当前的日期和时间
*******************************************************************************/
void GUI_SetTime(uchar8* pDateStr, uchar8* pTimeStr)
{	
	char DisplayBuf[100] = {0};

	GUI_PromptBox(pMenu_SetTime[0]);

	strcpy(DisplayBuf, (char*)pMenu_SetTime[1]);
	strcat(DisplayBuf, (char*)pDateStr);
    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strcpy(DisplayBuf, (char*)pMenu_SetTime[2]);
	strcat(DisplayBuf, (char*)pTimeStr);	
	LCD_DisplayStr(Promptbox.s_x +FONT_SIZE32*2, Promptbox.e_y -FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);

	GUI_Time();	
}

/*******************************************************************************
* Function Name  : GUI_SetDebugMode
* Description    : 调试模式开关设置
* Input          : bDebugMode -- 当前开关状态
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
void GUI_SetDebugMode(uchar8 bDebugMode)
{
	char DisplayBuf[100] = {0};
	
	GUI_PromptBox(pMenu_SetDebugMode[0]);

	strcpy(DisplayBuf, (char*)pMenu_SetDebugMode[1]);
	if(bDebugMode)
	{
		strcat(DisplayBuf, (char*)&pMenu_SetDebugMode[2][2]);
	}
	else
	{
		strcat(DisplayBuf, (char*)&pMenu_SetDebugMode[3][2]);
	}
    LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16), Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strcpy(DisplayBuf, (char*)pMenu_SetDebugMode[2]);
	strcat(DisplayBuf, (char*)pMenu_SetDebugMode[3]);
	LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16)*2, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_SetBoxDoorCheckSwitch
* Description    : 设置箱门状态检测开关
* Input          : bSwitch -- 当前开关状态
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
void GUI_SetBoxDoorCheckSwitch(uchar8 bSwitch)
{
	char DisplayBuf[100] = {0};
	
	GUI_PromptBox(pMenu_SetCheckDoorSwitch[0]);

	strcpy(DisplayBuf, (char*)pMenu_SetCheckDoorSwitch[1]);
	if(bSwitch)
	{
		strcat(DisplayBuf, (char*)&pMenu_SetCheckDoorSwitch[2][2]);
	}
	else
	{
		strcat(DisplayBuf, (char*)&pMenu_SetCheckDoorSwitch[3][2]);
	}
    LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16), Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strcpy(DisplayBuf, (char*)pMenu_SetCheckDoorSwitch[2]);
	strcat(DisplayBuf, (char*)pMenu_SetCheckDoorSwitch[3]);
	LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16)*2, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_SetBoxDoorCheckSwitch
* Description    : 设置箱门状态检测开关
* Input          : bSwitch -- 当前开关状态
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
void GUI_SetIDCardSwitch(uchar8 bSwitch)
{
	char DisplayBuf[100] = {0};
	
	GUI_PromptBox(pMenu_IDCaardSwitch[0]);

	strcpy(DisplayBuf, (char*)pMenu_IDCaardSwitch[1]);
	if(bSwitch)
	{
		strcat(DisplayBuf, (char*)&pMenu_IDCaardSwitch[2][2]);
	}
	else
	{
		strcat(DisplayBuf, (char*)&pMenu_IDCaardSwitch[3][2]);
	}
    LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16), Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strcpy(DisplayBuf, (char*)pMenu_IDCaardSwitch[2]);
	strcat(DisplayBuf, (char*)pMenu_IDCaardSwitch[3]);
	LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16)*2, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_SetWorkMode
* Description    : 工作模式设置
* Input          : mode -- 当前模式
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
void GUI_SetWorkMode(uchar8 mode)
{
	char DisplayBuf[100] = {0};
	uchar8 len = 0;
	
	GUI_ShowListMenu(pMenu_WorkMode, 7);

	sprintf(DisplayBuf, "<%d>", (mode +1));
	len = strlen(pMenu_WorkMode[0]);
    LCD_DisplayStr(FONT_SIZE24 +4, 280 - (len*8), FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_InputVoiceLevel
* Description    : 设置音量等级
* Input          : VoiceLevel -- 音量等级
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
void GUI_InputVoiceLevel(uchar8 VoiceLevel)
{
	char DisplayBuf[100] = {0};
	
	GUI_PromptBox(pMenu_VoiceLevel[0]);

	LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16), Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, (char*)pMenu_VoiceLevel[1], LCD_Color_Blue);
	sprintf(DisplayBuf, "%02d", VoiceLevel);
	LCD_DisplayStr(140, 165, FONT_SIZE32, DisplayBuf, LCD_Color_Red);
}

/*******************************************************************************
* Function Name  : GUI_InputMachineNum
* Description    : 输入机号
* Input          : iMachineNum -- 当前输入的机号
* Output         : None
* Return         : None
* Notes          : 在一个提示框中输入数字
*******************************************************************************/
void GUI_InputMachineNum(uint32 iMachineNum)
{
	char DisplayBuf[100] = {0};
	uchar8 strlength = 0;

	GUI_PromptBox(pStr_PlsInputMachineNum);

	strlength = strlen((char*)pStr_Old);
	strcpy(DisplayBuf, (char*)pStr_Old);
	UInt_To_Ascii(SysConfig.stHardware.u32MachineNum, (uchar8*)&DisplayBuf[strlength]);	
    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strlength = strlen((char*)pStr_New);
	strcpy(DisplayBuf, (char*)pStr_New);
	UInt_To_Ascii(iMachineNum, (uchar8*)&DisplayBuf[strlength]);	
	LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32*2, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Red);
}

/*******************************************************************************
* Function Name  :  GUI_SetNetSwitch
* Description    : 联网授权
* Input          : NetSwitch -- 当前开关状态
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
void GUI_SetNetSwitch(uchar8 NetSwitch)
{
	char DisplayBuf[100] = {0};
	
	GUI_PromptBox(pMenu_SetNetSwitch[0]);

	strcpy(DisplayBuf, (char*)pMenu_SetNetSwitch[1]);
	if(NetSwitch)
	{

		strcat(DisplayBuf, (char*)&pMenu_SetNetSwitch[2][2]);
	}
	else
	{
		strcat(DisplayBuf, (char*)&pMenu_SetNetSwitch[3][2]);
	}
    LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16), Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strcpy(DisplayBuf, (char*)pMenu_SetNetSwitch[2]);
	strcat(DisplayBuf, (char*)pMenu_SetNetSwitch[3]);
	LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16)*2, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_ChoiceNetInfo
* Description    : 网络信息选择
* Input          : None
* Output         : None
* Return         : None
* Notes          : 1.本机IP 2.服务器IP 3.服务器端口号
*******************************************************************************/
void GUI_ChoiceNetInfo(void)
{	
	GUI_PromptBox(pMenu_TCPIP[0]);
	
    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, (char*)pMenu_TCPIP[1], LCD_Color_Blue);
	LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32*2, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, (char*)pMenu_TCPIP[2], LCD_Color_Blue);
    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32*3, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, (char*)pMenu_TCPIP[3], LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_InputIp
* Description    : 设置本机IP地址
* Input          : type -- 1.本机IP, 2.服务器IP
                   pInputIpStr--输入的IP地址(字符串格式)
* Output         : None
* Return         : None
* Notes          : 同时显示旧的IP
*******************************************************************************/
void GUI_InputIp(uchar8 type, uchar8* pInputIpStr)
{	
	char DisplayBuf[100] = {0};
	uchar8 strlength = 0;

	if(1 == type)
	{
		GUI_PromptBox(&pMenu_TCPIP[1][2]);
		strlength = strlen((char*)pStr_Old);
		strcpy(DisplayBuf, (char*)pStr_Old);
		IPinfo_Hex2Ascii((uchar8*)&DisplayBuf[strlength], (uchar8*)SysConfig.stNet.IP);
	    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
	}
	else if(2 == type)
	{
		GUI_PromptBox(&pMenu_TCPIP[2][2]);
		strlength = strlen((char*)pStr_Old);
		strcpy(DisplayBuf, (char*)pStr_Old);
		IPinfo_Hex2Ascii((uchar8*)&DisplayBuf[strlength], (uchar8*)SysConfig.stNet.ServerIP);
	    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
	}
	else
	{
		GUI_PromptBox(pMenu_TCPIP[0]);
	}

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strlength = strlen((char*)pStr_New);
	strcpy(DisplayBuf, (char*)pStr_New);
	strcat(DisplayBuf, (char*)pInputIpStr);
	LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32*2, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_InputServerPort
* Description    : 设置服务器端口号
* Input          : iPort--输入的端口号
* Output         : None
* Return         : None
* Notes          : 同时显示旧的端口号
*******************************************************************************/
void GUI_InputServerPort(uint32 iPort)
{	
	char DisplayBuf[100] = {0};
	uchar8 strlength = 0;

	GUI_PromptBox(&pMenu_TCPIP[3][2]);

	strlength = strlen((char*)pStr_Old);
	strcpy(DisplayBuf, (char*)pStr_Old);
	UInt_To_Ascii(SysConfig.stNet.u16UDP_ServerPort, (uchar8*)&DisplayBuf[strlength]);	
    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strlength = strlen((char*)pStr_New);
	strcpy(DisplayBuf, (char*)pStr_New);
	UInt_To_Ascii(iPort, (uchar8*)&DisplayBuf[strlength]);	
	LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32*2, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_Registration
* Description    : 系统注册
* Input          : pInputStr--输入的字符串, InputType--当前输入法(0-数字,INPUT_TYPE_ABC-字母)
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_Registration(uchar8* pInputStr, uchar8 InputType)
{
	char DisplayBuf[100] = {0};

	GUI_PromptBox(pStr_OperationTip);

	strcpy(DisplayBuf, (char*)pStr_InputType);
	if(INPUT_TYPE_ABC == InputType)
	{
		strcat(DisplayBuf, "<ABC>");	
	}
	else
	{
		strcat(DisplayBuf, "<123>");	
	}
	LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Red);

	strcpy(DisplayBuf, pStr_PlsEnterPwd);
    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32*2, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strcat(DisplayBuf, (char*)pInputStr);	
	LCD_DisplayStr(Promptbox.s_x+90, Promptbox.e_y-FONT_SIZE08, FONT_SIZE24, DisplayBuf, LCD_Color_Red);
}

/*******************************************************************************
* Function Name  : GUI_ShowNotice
* Description    : 显示提示信息
* Input          : Type--信息类型
* Output         : None
* Return         : None
* Notes          : 根据类型选择要显示的信息
*******************************************************************************/
void GUI_ShowNotice(uchar8 Type)
{
	switch(Type)
	{
		case NOTICE_SET_OK:
		{
			GUI_PromptBox(pStr_OperationTip);
			LCD_DisplayStr(110, 230, FONT_SIZE24, pStr_SetOK, LCD_Color_Red);
		}
		break;

		case NOTICE_SET_FAIL:
		{
			GUI_PromptBox(pStr_OperationTip);
			LCD_DisplayStr(110, 230, FONT_SIZE24, pStr_SetFailed, LCD_Color_Red);
		}
		break;

		case NOTICE_PWD_ERR:
		{
			GUI_PromptBox(pStr_OperationTip);
			LCD_DisplayStr(110, 230, FONT_SIZE24, pStr_PasswordError, LCD_Color_Red);
		}
		break;

		case NOTICE_SYS_INIT_ASK:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 265, FONT_SIZE24, pStr_AskSysInit, LCD_Color_Red);
		}
		break;

		case NOTICE_SYS_INIT_WAIT:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 265, FONT_SIZE24, pStr_SysInitWait, LCD_Color_Red);
		}
		break;
		
		case NOTICE_SYS_INIT_OK:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 265, FONT_SIZE24, pStr_SysInitOK, LCD_Color_Red);
		}
		break;

		case NOTICE_BOX_ID_ERR:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 250, FONT_SIZE24, pStr_BoxIdErr, LCD_Color_Red);
		}
		break;
		
		case NOTICE_PLS_WAIT:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 250, FONT_SIZE24, pStr_PlsWait, LCD_Color_Red);
		}
		break;

		case NOTICE_ASK_OPEN_ALL_BOX:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 265, FONT_SIZE24, pStr_AskOpenAllBox, LCD_Color_Red);
		}
		break;

		case NOTICE_ASK_CLR_CARD:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 265, FONT_SIZE24, pStr_AskClrCard, LCD_Color_Red);
		}
		break;

		case NOTICE_ASK_CLR_CARD_OK:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 265, FONT_SIZE24, pStr_ClrCardOk, LCD_Color_Red);
		}
		break;

		case NOTICE_ASK_CLR_BOX:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 280, FONT_SIZE24, pStr_AskClrBox, LCD_Color_Red);
		}
		break;

		case NOTICE_ASK_CLR_ALLBOX:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 280, FONT_SIZE24, pStr_AskClrAllBox, LCD_Color_Red);
		}
		break;

		case NOTICE_OPENING_BOX:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 265, FONT_SIZE24, pStr_OpeningBox, LCD_Color_Red);
		}
		break;

		case NOTICE_LOCKING_BOX_OK:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 250, FONT_SIZE24, pStr_LockingBoxOk, LCD_Color_Red);
		}
		break;

		case NOTICE_UNLOCKING_BOX_OK:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 250, FONT_SIZE24, pStr_UnlockingBoxOk, LCD_Color_Red);
		}
		break;
		
		default:
			break;
	}
}


/*******************************************************************************************************
* Description  : 显示主界面  
********************************************************************************************************/
void  GUI_Admin(void)
{	
	LCD_Clear(LCD_Color_White);
	LCD_ShowFillPic(PHOTO_Desktop);

	LCD_ShowPartPic(HALF_PHOTO_X_2, HALF_PHOTO_Y_2, HALF_PHOTO_X_2+HALF_PHOTO_Xsize, HALF_PHOTO_Y_2+HALF_PHOTO_Ysize, HALF_PHOTO_ZNXG);

	if(0 == gstSysRunInfo.u8EmptyBoxCount)
	{
		LCD_Fill(65, 80, 120, 250, LCD_Color_Back);
		LCD_DisplayStr(80, 230, FONT_SIZE24, pStr_LockersIsFull, LCD_Color_Red);	
	}
}

/*******************************************************************************************************
* Description  : 显示空箱数
********************************************************************************************************/
void  GUI_EmptyBoxCount(void)
{
	char DisplayBuf[100] = {0};
	uchar8 len = 0;
	extern const char *pStr_NowEmptyBoxNum;
	
	LCD_Fill((LCD_HIGH - FONT_SIZE32), 0, LCD_HIGH, LCD_WIDE, LCD_Color_White);
	strcpy(DisplayBuf, pStr_NowEmptyBoxNum);
	len = strlen(DisplayBuf);
	sprintf(&DisplayBuf[len], "%d ", gstSysRunInfo.u8EmptyBoxCount);
	LCD_DisplayStr((LCD_HIGH - FONT_SIZE24), (LCD_WIDE - FONT_SIZE24), FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************************************
* Description  : 显示待机主界面  
* Notes        : NULL
********************************************************************************************************/
void GUI_OnStandby(void)
{
	static uint32 iLastUpdateTime, iCount, iCount02;

	if((gstSysRunInfo.u32TimeCount_ms - iLastUpdateTime) >= 1000) 
	{
		iLastUpdateTime = gstSysRunInfo.u32TimeCount_ms;
		
		iCount ++;
		if(1 == (iCount % 4))
		{
			GUI_EmptyBoxCount();
		}
		else if((0 == (iCount % 4))||(3 == (iCount % 4)))
		{
			GUI_Time();	 
		}
		
		iCount02 ++;
		if(WORK_MODE_ANY_FINGER_CARD == SysConfig.stLockers.u8WorkMode)
		{
			if(1 == (iCount02 % 4))
			{
				LCD_Fill(95, 40, 130, 280, LCD_Color_GrayBlue);
				LCD_DisplayStr(100, 260, FONT_SIZE24, "存物请按[寄存].", LCD_Color_White);
			}
			else if(3 == (iCount02 % 4))
			{
				LCD_Fill(95, 40, 130, 280, LCD_Color_GrayBlue);
				LCD_DisplayStr(100, 265, FONT_SIZE24, "取物请刷卡或按指纹.", LCD_Color_White);
			}
		}
		else
		{
			if(1 == (iCount02 % 4))
			{
				LCD_ShowPartPic(HALF_PHOTO_X_2,HALF_PHOTO_Y_2, HALF_PHOTO_X_2+HALF_PHOTO_Xsize, HALF_PHOTO_Y_2+HALF_PHOTO_Ysize, HALF_PHOTO_FetchByCard);
			}
			else if(3 == (iCount02 % 4))
			{
				LCD_ShowPartPic(HALF_PHOTO_X_2,HALF_PHOTO_Y_2, HALF_PHOTO_X_2+HALF_PHOTO_Xsize, HALF_PHOTO_Y_2+HALF_PHOTO_Ysize, HALF_PHOTO_StorePlsKey);
			}
		}		
	}			
}

/*******************************************************************************************************
* Description  : 按工作状态显示界面
********************************************************************************************************/
void  GUI_WorkStatus(void)
{
	switch(gstSysRunInfo.CurrentStatus)
	{	
		case SYS_STATUS_IDLE:
		{
			GUI_OnStandby();	
		}
		break;

		default:
			break;
	}
}


/*******************************************************************************
* Function Name  : GUI_ShowPassword
* Description    : 提示框显示输入密码
* Input          : PsdNum -- 输入的密码位数
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_ShowPassword(uchar8 PsdNum)
{	
	char Display[20] = {0};
	GUI_PromptBox(pStr_PlsInputPsw);

	if(0 == PsdNum)
	{
		Display[0] = '_';
	}
	else
	{
		memset(Display, '*', PsdNum);
	}
	LCD_DisplayStr(100, 230, FONT_SIZE24, Display, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_ChangePassword
* Description    : 修改高级管理员密码
* Input          : pStr_OldPassword--旧密码(ASCII格式), 新输入的密码字符串
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_ChangePassword(uchar8* pStr_OldPassword, uchar8* pInputStr)
{
	char DisplayBuf[100] = {0};

	GUI_PromptBox(pMenu_ChoiceAdmin[0]);

	strcpy(DisplayBuf, (char*)pStr_Old);
	strcat(DisplayBuf, (char*)pStr_OldPassword);	
    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strcpy(DisplayBuf, (char*)pStr_New);
	strcat(DisplayBuf, (char*)pInputStr);	
	LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32*2, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_InputName
* Description    : 录入名称
* Input          : pInputStr--输入的字符串, InputType--当前输入法(0-数字,INPUT_TYPE_ABC-字母)
                   pNotice -- 附带提示信息
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_InputName(uchar8* pInputStr, uchar8 InputType, const char* pNotice)
{
	char DisplayBuf[100] = {0};

	GUI_PromptBox(pStr_OperationTip);

	strcpy(DisplayBuf, (char*)pStr_InputType);
	if(INPUT_TYPE_ABC == InputType)
	{
		strcat(DisplayBuf, "<ABC>");	
	}
	else
	{
		strcat(DisplayBuf, "<123>");	
	}
	LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Red);

	strcpy(DisplayBuf, pNotice);
    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32*2, Promptbox.e_y-FONT_SIZE08, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strcat(DisplayBuf, (char*)pInputStr);	
	LCD_DisplayStr(Promptbox.s_x+90, Promptbox.e_y-FONT_SIZE08, FONT_SIZE24, DisplayBuf, LCD_Color_Red);
}

/*******************************************************************************
* Function Name  : GUI_AddValidCard
* Description    : 提示小框显示--增加预授权卡
* Input          : iCardNum--卡号, addmsg--附带提示信息
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_AddValidCard(uint32 iCardNum, uchar8 addmsg)
{
	char DisplayBuf[20] = {0};
	GUI_PromptBox(pMenu_Admin_Manage[6]);

	UInt_To_Ascii(iCardNum, (uchar8*)DisplayBuf);
    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE64, Promptbox.e_y-FONT_SIZE64, FONT_SIZE24, DisplayBuf, LCD_Color_Blue);
	if(1 == addmsg)
	{
		memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
		strcpy(DisplayBuf, "<OK>");
		LCD_DisplayStr(150, 220, FONT_SIZE24, DisplayBuf, LCD_Color_Red);
	}
	else if(ALREADY_EXIST == addmsg)
	{		
		LCD_DisplayStr(150, 220, FONT_SIZE24, pMenu_CardAccredit[4], LCD_Color_Red);
	}
	else if(0xFF == addmsg)
	{		
		LCD_DisplayStr(150, 220, FONT_SIZE24, pMenu_CardAccredit[5], LCD_Color_Red);
	}
	else
	{
		LCD_DisplayStr(150, 220, FONT_SIZE24, pStr_PlsCard, LCD_Color_Red);
	}
}

/*******************************************************************************
* Function Name  : GUI_AddCard2Box
* Description    : 提示大框显示--授权卡
* Input          : box--箱号, iCardNum--卡号, CardCount--当前授权卡数量
* Output         : None
* Return         : None
* Notes          : 如果是授权用户卡(box=0),则在底部显示"按<#>切换至下一信箱号"
*******************************************************************************/
void GUI_AddCard2Box(uchar8 box, uint32 iCardNum, uchar8 CardCount)
{
	char DisplayBuf[100] = {0};
	char tempBuf[30] = {0};
	u32  strlength = 0;

	GUI_PromptBox(pStr_PlsCard);

	if(0 == box) //管理卡
	{
	    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32, Promptbox.e_y-FONT_SIZE24, FONT_SIZE16, pMenu_CardAccredit[1], LCD_Color_Blue);

		if(iCardNum)
		{
			memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
			UInt_To_Ascii(iCardNum, (uchar8*)DisplayBuf);
		    //LCD_DisplayStr(Promptbox.s_x+FONT_SIZE64, Promptbox.e_y-FONT_SIZE24, FONT_SIZE24, DisplayBuf, LCD_Color_Blue);
			
			memset(tempBuf, 0x0, sizeof(tempBuf));		
			if(ALREADY_EXIST == CardCount)
			{
				strcpy(tempBuf, pMenu_CardAccredit[4]);
			}
			else if(MAX_AdminCardNum == CardCount)
			{
				strcpy(tempBuf, pMenu_CardAccredit[5]);
			}
			else if(CardCount < MAX_AdminCardNum)
			{
				sprintf(tempBuf, "<%d--OK>", CardCount+1);
			}
			else
			{
				strcpy(tempBuf, "<Error>");
			}
			strcat(DisplayBuf, tempBuf);
			LCD_DisplayStr(Promptbox.s_x+FONT_SIZE64, Promptbox.e_y-FONT_SIZE24, FONT_SIZE16, DisplayBuf, LCD_Color_Red);
		}
		else
		{
			LCD_DisplayStr(Promptbox.s_x+FONT_SIZE64, Promptbox.e_y-FONT_SIZE24, FONT_SIZE24, pMenu_CardAccredit[3], LCD_Color_Red);
		}

	}
	else
	{
		strcpy(DisplayBuf, pMenu_CardAccredit[2]);
		strlength = strlen(DisplayBuf);
		sprintf(&DisplayBuf[strlength], " %02d", box);
	    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32, Promptbox.e_y-FONT_SIZE24, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);

		if(iCardNum)
		{
			memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
			strlength = UInt_To_Ascii(iCardNum, (uchar8*)DisplayBuf);
		    //LCD_DisplayStr(Promptbox.s_x+FONT_SIZE64, Promptbox.e_y-FONT_SIZE24, FONT_SIZE24, DisplayBuf, LCD_Color_Blue);
			
			memset(tempBuf, 0x0, sizeof(tempBuf));		
			if(ALREADY_EXIST == CardCount)
			{
				strcpy(tempBuf, pMenu_CardAccredit[4]);
			}
			else if(MAX_BoxCardNum == CardCount)
			{
				strcpy(tempBuf, pMenu_CardAccredit[5]);
			}
			else if(CardCount < MAX_BoxCardNum)
			{
				sprintf(tempBuf, "<%d--OK>", CardCount+1);
			}
			else
			{
				strcpy(tempBuf, "<Error>");
			}
			strcat(DisplayBuf, tempBuf);
			LCD_DisplayStr(Promptbox.s_x+FONT_SIZE64, 120, FONT_SIZE16, DisplayBuf, LCD_Color_Red);
		}
		else
		{
			LCD_DisplayStr(Promptbox.s_x+FONT_SIZE64, 220, FONT_SIZE24, pMenu_CardAccredit[3], LCD_Color_Red);
		}
		
		memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
		strcpy(DisplayBuf, pMenu_CardAccredit[6]);
		LCD_DisplayStr(Promptbox.e_x-FONT_SIZE32, Promptbox.e_y-FONT_SIZE24, FONT_SIZE16, DisplayBuf, LCD_Color_Red);
	}
}

/*******************************************************************************
* Function Name  : GUI_ClrBoxNotice
* Description    : 清箱提示信息消息
* Input          : u8BoxNum -- 箱号
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_ClrBoxNotice(uchar8 u8Box)
{
	char DisplayBuf[100] = {0};
	u32 strlength = 0; 

	GUI_PromptBox(pStr_OperationTip);

	if(MARK_CHOICE_ALL_BOX == u8Box)
	{
		LCD_DisplayStr(100, 260, FONT_SIZE24, pStr_Open1Box, LCD_Color_Red);		
	}
	else
	{
		strlength = strlen((char*)SysConfig.stBox[u8Box].BoxSN);
		if(strlength > MAX_BoxSnLen)
		{
			strlength = MAX_BoxSnLen;
		}
		memcpy(DisplayBuf, (char*)SysConfig.stBox[u8Box].BoxSN, strlength);
		strcpy(&DisplayBuf[strlength], pStr_Open1Box);	
		LCD_DisplayStr(100, 260, FONT_SIZE24, DisplayBuf, LCD_Color_Red);
	}
	LCD_DisplayStr(160, 280, FONT_SIZE16, (char*)pStr_ClrBoxNotice, LCD_Color_Red);
}

/*******************************************************************************
* Function Name  : GUI_AskLockinBox
* Description    : 提示--确定锁箱?
* Input          : u8BoxNum -- 箱号
* Output         : None
* Return         : None
* Notes          : MARK_CHOICE_ALL_BOX -- 代表选中所有箱号
*******************************************************************************/
void GUI_AskLockinBox(uchar8 u8Box)
{
	char DisplayBuf[100] = {0};
	u32 strlength = 0; 

	GUI_PromptBox(pStr_OperationTip);

	strlength = UInt_To_Ascii(u8Box, (uchar8*)DisplayBuf);
	LCD_DisplayStr((Promptbox.e_x-Promptbox.s_x)/2+Promptbox.s_x -24, ((Promptbox.e_y-Promptbox.s_y)/2) + Promptbox.s_y + ((FONT_SIZE64*strlength)/2)/2, FONT_SIZE64, DisplayBuf, LCD_Color_Red);
	if(MARK_CHOICE_ALL_BOX == u8Box)
	{
		LCD_DisplayStr(160, 280, FONT_SIZE16, (char*)pStr_LockinAllBox, LCD_Color_Red);			
	}
	else
	{		
		LCD_DisplayStr(160, 280, FONT_SIZE16, (char*)pStr_Locking1Box, LCD_Color_Red);
	}
}

/*******************************************************************************
* Function Name  : GUI_AskUnlockinBox
* Description    : 提示--确定解除锁X箱?
* Input          : u8BoxNum -- 箱号
* Output         : None
* Return         : None
* Notes          : MARK_CHOICE_ALL_BOX -- 代表选中所有箱号
*******************************************************************************/
void GUI_AskUnlockinBox(uchar8 u8Box)
{
	char DisplayBuf[100] = {0};
	u32 strlength = 0; 

	GUI_PromptBox(pStr_OperationTip);

	strlength = UInt_To_Ascii(u8Box, (uchar8*)DisplayBuf);
	LCD_DisplayStr((Promptbox.e_x-Promptbox.s_x)/2+Promptbox.s_x -24, ((Promptbox.e_y-Promptbox.s_y)/2) + Promptbox.s_y + ((FONT_SIZE64*strlength)/2)/2, FONT_SIZE64, DisplayBuf, LCD_Color_Red);
	if(MARK_CHOICE_ALL_BOX == u8Box)
	{
		LCD_DisplayStr(160, 280, FONT_SIZE16, (char*)pStr_UnlockingAllBox, LCD_Color_Red);			
	}
	else
	{		
		LCD_DisplayStr(160, 280, FONT_SIZE16, (char*)pStr_Unlocking1Box, LCD_Color_Red);
	}
}

/*******************************************************************************
* Function Name  : GUI_QueryBoxRecord
* Description    : 开箱记录查询
* Input          : BoxNum -- 箱号, CurrentPage -- 当前页(0--N)
* Output         : None
* Return         : None
* Notes          : 每页显示4笔开箱记录
*******************************************************************************/
void GUI_QueryBoxRecord(uchar8 BoxNum, uchar8 CurrentPage)
{
	uint32 strlength = 0 ,curnum =0,linenum =0; 
	char DisplayBuf[100] = {0};
	uint32 RecordNum = 0;
	uint32 iQueryRet = 0;
	ST_RECORD BoxRecordQuery;

	memcpy(DisplayBuf, (char*)&SysConfig.stBox[BoxNum].BoxSN[0], MAX_BoxSnLen);
	strcat(DisplayBuf, pMenu_RecordQuery[1]);
	GUI_PromptBigBox(DisplayBuf);
		
    if((0 == BoxNum)||(BoxNum > SysConfig.stLockers.u8BoxCount))
	{
		LCD_DisplayStr(PromptBigBox.e_x-FONT_SIZE24*4, PromptBigBox.e_y- FONT_SIZE16*6, FONT_SIZE32, pMenu_RecordQuery[2], LCD_Color_Blue);
	}
	else
	{
		if(0 == SysConfig.stBox[BoxNum].iRecordCount)
		{
			LCD_DisplayStr(PromptBigBox.e_x-FONT_SIZE24*4, PromptBigBox.e_y- FONT_SIZE16*6, FONT_SIZE32, pMenu_RecordQuery[3], LCD_Color_Blue);
		}
		else
		{
			memset(DisplayBuf,0x00,sizeof(DisplayBuf));
			strcpy(DisplayBuf, (char*)pStr_QueryPrompt);
			strlength = strlen(pStr_QueryPrompt);
			sprintf(DisplayBuf+strlength, "   No.:%d", CurrentPage+1);
			LCD_DisplayStr(PromptBigBox.e_x-FONT_SIZE24, PromptBigBox.e_y- FONT_SIZE16, FONT_SIZE16,DisplayBuf, LCD_Color_Blue);

			for(curnum =0; curnum<4; curnum ++) // 4行3列
			{
				RecordNum = CurrentPage *4 + curnum;
				if(RecordNum >= SysConfig.stBox[BoxNum].iRecordCount)
				{
					break;
				}
				RecordNum = SysConfig.stBox[BoxNum].iRecordCount - RecordNum; //倒翻记录
				memset((uchar8*)&BoxRecordQuery, 0x00, sizeof(ST_RECORD));		
				iQueryRet = QueryBoxRecord(BoxNum, RecordNum -1, &BoxRecordQuery);
				if(0xFFFFFFFF == iQueryRet)
				{
					memset(DisplayBuf,0x00,sizeof(DisplayBuf));
					strcpy(DisplayBuf, "**-**-** **-**-**");
				}	
				else
				{
					memset(DisplayBuf,0x00,sizeof(DisplayBuf));
					sprintf(DisplayBuf, "20%02d-%02d-%02d %02d:%02d:%02d  ", 
						    BoxRecordQuery.Time[0],BoxRecordQuery.Time[1],BoxRecordQuery.Time[2],BoxRecordQuery.Time[3],BoxRecordQuery.Time[4],BoxRecordQuery.Time[5]);
					switch(BoxRecordQuery.u8Type)
					{
						case OpenBoxType_Admin:
						{
							strcat(DisplayBuf, pMenu_OpenBoxType[1]);
						}
						break;
						
						case OpenBoxType_Password:
						{
							strcat(DisplayBuf, pMenu_OpenBoxType[2]);
						}
						break;

						case OpenBoxType_Card:
						{
							strcat(DisplayBuf, pMenu_OpenBoxType[3]);
						}
						break;

						case OpenBoxType_CardPwd:
						{
							strcat(DisplayBuf, pMenu_OpenBoxType[4]);
						}
						break;

						case OpenBoxType_Fingerprint:
						{
							strcat(DisplayBuf, pMenu_OpenBoxType[5]);
						}
						break;
		
						case OpenBoxType_NetCmd:
						{
							strcat(DisplayBuf, pMenu_OpenBoxType[6]);
						}
						break;

						case OpenBoxType_Postman:
						{
							strcat(DisplayBuf, pMenu_OpenBoxType[7]);
							strcat(DisplayBuf, (char*)BoxRecordQuery.HandleInfo);
						}
						break;
					}
					
					if(1 == BoxRecordQuery.u8SaveOrTake)
					{
						strcat(DisplayBuf, pAccessStatus[1]);
					}
					else if(2 == BoxRecordQuery.u8SaveOrTake)
					{
						strcat(DisplayBuf, pAccessStatus[2]);
					}
					else
					{
						strcat(DisplayBuf, pAccessStatus[3]);
					}
				}		
			
				LCD_DisplayStr(PromptBigBox.s_x+FONT_SIZE32+FONT_SIZE24*linenum, PromptBigBox.e_y- FONT_SIZE16, FONT_SIZE16,DisplayBuf, LCD_Color_Red);
				linenum++;		
			}
		}
	}
}

/*******************************************************************************
* Function Name  : GUI_QueryLockBoxList
* Description    : 显示锁箱列表
* Input          : CurrentPage -- 当前页(0--N)
* Output         : None
* Return         : None
* Notes          : 
*******************************************************************************/
void GUI_QueryLockBoxList(uchar8 CurrentPage)
{
	u32 strlength = 0 ,curnum =0,listnum =0,linenum =0; 
	char DisplayBuf[100]={0};
	uchar8 LockBoxList[MAX_BOX_NUM] = {0};
	uchar8 LockBoxCount = 0;
	int i =0;
	uint32 iAddr = 0;
	
	GUI_PromptBigBox(pStr_QueryLockBox);

	strcpy(DisplayBuf, (char*)pStr_QueryPrompt);
	strlength = strlen(pStr_QueryPrompt);
	sprintf(DisplayBuf+strlength, "   No.:%d", CurrentPage+1);
	LCD_DisplayStr(PromptBigBox.e_x-FONT_SIZE24, PromptBigBox.e_y- FONT_SIZE16, FONT_SIZE16,DisplayBuf, LCD_Color_Blue);

	//读取锁箱列表
	for(i=1; i<=SysConfig.stLockers.u8BoxCount; i++)
	{
		if(LSTATUS_LOCKING == SysConfig.stBox[i].bLocking)
		{
			LockBoxList[LockBoxCount++] = SysConfig.stBox[i].u8BoxID;
		}
	}

	if(0 == LockBoxCount)
	{
		strlength = strlen(pStr_WithoutBoxBeLock);
		LCD_DisplayStr((PromptBigBox.e_x-PromptBigBox.s_x)/2+PromptBigBox.s_x-FONT_SIZE24/2, (PromptBigBox.e_y-PromptBigBox.s_y)/2+PromptBigBox.s_y+((FONT_SIZE24*strlength)/2)/2, FONT_SIZE24, pStr_WithoutBoxBeLock, LCD_Color_Red);
	}
	else
	{
		for(curnum =1; curnum<=24; curnum +=6) // 4行6列
		{
			memset(DisplayBuf,0x00,sizeof(DisplayBuf));
			for(listnum=0;listnum<6;listnum++)
			{
				iAddr = (CurrentPage * 24) + (linenum *6) + listnum ;
				if(0 == LockBoxList[iAddr])
				{
					break;
				}
				else
				{
					sprintf(&DisplayBuf[listnum*6], "%02d    ", LockBoxList[iAddr]);
				}
			}
			LCD_DisplayStr(PromptBigBox.s_x+FONT_SIZE32+FONT_SIZE24*linenum, PromptBigBox.e_y- FONT_SIZE16, FONT_SIZE16,DisplayBuf, LCD_Color_Red);
			linenum++;
		}
	}
}

/*******************************************************************************
* Function Name  : GUI_QueryBoxStatusList
* Description    : 查询箱状态列表(箱门状态以及箱内状态)
* Input          : CurrentPage -- 当前页(0--N)
* Output         : None
* Return         : None
* Notes          : 每页显示4个箱的状态
*******************************************************************************/
void GUI_QueryBoxStatusList(uchar8 CurrentPage)
{
	uint32 iAddr = 0;
	uint32 strlength = 0 ,curnum =0,linenum =0; 
	char DisplayBuf[100]={0};
	uchar8 bChangeColor = 0;
	uchar8 len = 0;
	
	GUI_PromptBigBox(pStr_QueryBoxStatus);

	strcpy(DisplayBuf, (char*)pStr_QueryPrompt);
	strlength =strlen(pStr_QueryPrompt);
	sprintf(DisplayBuf+strlength, "   No.:%d", CurrentPage+1);
	LCD_DisplayStr(PromptBigBox.e_x-FONT_SIZE24, PromptBigBox.e_y- FONT_SIZE16, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
	
	for(curnum =1; curnum<=4; curnum ++) // 4行3列
	{
		iAddr = (CurrentPage *4) + curnum; //计算箱号
		if(iAddr > SysConfig.stLockers.u8BoxCount)
		{
			break;
		}
		memset(DisplayBuf, 0x00, sizeof(DisplayBuf));
		memcpy(DisplayBuf, (char*)&SysConfig.stBox[iAddr].BoxSN[0], MAX_BoxSnLen);
		strcat(DisplayBuf, pStr_NxBox);
		strlength = strlen(DisplayBuf);
		memset(&DisplayBuf[strlength], ' ', (40 - strlength));

		bChangeColor = 0;
		if(0 == gstSysRunInfo.BoxDoorStatus[iAddr])
		{
			strcpy(DisplayBuf+10, pStr_BoxStatus[0]);
		}
		else
		{
			strcpy(DisplayBuf+10, pStr_BoxStatus[1]);
			bChangeColor = 1;
		}
		len = strlen(DisplayBuf);
		DisplayBuf[len] = 0x20;
		if(BOX_STATUS_OCCUPY == SysConfig.stBox[iAddr].u8Status)
		{
			strcpy(DisplayBuf+20, pStr_BoxStatus[3]);
			bChangeColor = 1;
		}
		else
		{
			strcpy(DisplayBuf+20, pStr_BoxStatus[2]);
		}

		if(0 == bChangeColor)
		{
			LCD_DisplayStr(PromptBigBox.s_x+FONT_SIZE32+FONT_SIZE24*linenum, PromptBigBox.e_y- FONT_SIZE16, FONT_SIZE16,DisplayBuf, LCD_Color_Blue);
		}
		else
		{
			LCD_DisplayStr(PromptBigBox.s_x+FONT_SIZE32+FONT_SIZE24*linenum, PromptBigBox.e_y- FONT_SIZE16, FONT_SIZE16,DisplayBuf, LCD_Color_Red);
		}
		linenum++;
	}
}

/*******************************************************************************
* Function Name  : GUI_SetCardSector
* Description    : 设置IC卡的使用扇区号
* Input          : sector -- 输入的卡扇区号
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
void GUI_SetCardSector(uchar8 sector)
{
	char DisplayBuf[30] = {0};
	char tempBuf[10] = {0};
	
	GUI_PromptBox(pMenu_CardSector[0]);

	LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16), Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, (char*)pMenu_CardSector[1], LCD_Color_Red);
	strcpy(DisplayBuf, (char*)pMenu_CardSector[2]);
	sprintf(tempBuf, "%d", SysConfig.stCardKey.u8Sector);
	strcat(DisplayBuf, tempBuf);
	LCD_DisplayStr(120, 260, FONT_SIZE24, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	memset(tempBuf, 0x0, sizeof(tempBuf));
	strcpy(DisplayBuf, (char*)pMenu_CardSector[3]);
	if(sector)
	{
		sprintf(tempBuf, "%d", sector);
	}
	else
	{
		tempBuf[0] = '_';
	}
	strcat(DisplayBuf, tempBuf);
	LCD_DisplayStr(150, 260, FONT_SIZE24, DisplayBuf, LCD_Color_Blue);	
}

/*******************************************************************************
* Function Name  : GUI_InputAllowOpenBoxNum
* Description    : 设置寄存时允许的最大开箱数量
* Input          : boxNum -- 开箱数量
* Output         : None
* Return         : None
* Notes          : None
*******************************************************************************/
void GUI_InputAllowOpenBoxNum(uchar8 boxNum)
{
	char DisplayBuf[30] = {0};
	char tempBuf[10] = {0};
	
	GUI_PromptBox(pMenu_AllowOpenBoxNum[0]);

	LCD_DisplayStr(Promptbox.s_x+(FONT_SIZE24+FONT_SIZE16), Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, (char*)pMenu_AllowOpenBoxNum[1], LCD_Color_Red);
	strcpy(DisplayBuf, (char*)pMenu_AllowOpenBoxNum[2]);
	sprintf(tempBuf, "%d", SysConfig.stLockers.u8AllowBeOpenedBoxNum);
	strcat(DisplayBuf, tempBuf);
	LCD_DisplayStr(120, 260, FONT_SIZE24, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	memset(tempBuf, 0x0, sizeof(tempBuf));
	strcpy(DisplayBuf, (char*)pMenu_AllowOpenBoxNum[3]);
	if(boxNum)
	{
		sprintf(tempBuf, "%d", boxNum);
	}
	else
	{
		tempBuf[0] = '_';
	}
	strcat(DisplayBuf, tempBuf);
	LCD_DisplayStr(150, 260, FONT_SIZE24, DisplayBuf, LCD_Color_Blue);	
}

/*******************************************************************************
* Function Name  : GUI_SetPickUpAwaitTime
* Description    : 设置限时取件等待时间
* Input          : time_s -- 时间(S)
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_SetPickUpAwaitTime(uint32 time_s)
{
	char DisplayBuf[10] = {0};
	uchar8 len = 0;
	
	GUI_PromptBox(pMenu_PickUpAwaitTime[0]);

	LCD_DisplayStr(100, 260, FONT_SIZE16, (char*)pMenu_PickUpAwaitTime[1], LCD_Color_Blue);
	sprintf(DisplayBuf, "%d", SysConfig.stLockers.iMinFetchTime);
	LCD_DisplayStr(100, 260-(11*8), FONT_SIZE24, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strcpy(DisplayBuf, pMenu_PickUpAwaitTime[2]);
	len = strlen(DisplayBuf);	
	sprintf(&DisplayBuf[len], "%d", time_s);
	LCD_DisplayStr(140, 260, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_SetPickUpTimeout
* Description    : 设置限时取件超时时间
* Input          : time_s -- 时间(S)
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_SetPickUpTimeout(uint32 time_s)
{
	char DisplayBuf[10] = {0};
	uchar8 len = 0;
	
	GUI_PromptBox(pMenu_PickUpTimeout[0]);

	LCD_DisplayStr(100, 260, FONT_SIZE16, pMenu_PickUpTimeout[1], LCD_Color_Blue);
	sprintf(DisplayBuf, "%d", SysConfig.stLockers.iMaxFetchTime);
	LCD_DisplayStr(100, 260-(11*8), FONT_SIZE24, DisplayBuf, LCD_Color_Blue);
	
	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strcpy(DisplayBuf, pMenu_PickUpTimeout[2]);
	len = strlen(DisplayBuf);	
	sprintf(&DisplayBuf[len], "%d", time_s);
	LCD_DisplayStr(140, 260, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_SetUIntValue
* Description    : 设置一个参数的整型值
* Input          : pMenuTitle -- 提示菜单标题, uiOldValue -- 旧值, uiNewValue -- 新值
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_SetUIntValue(const char *pMenuTitle, uint32 uiOldValue, uint32 uiNewValue)
{
	char DisplayBuf[10] = {0};
	uchar8 len = 0;
	
	GUI_PromptBox(pMenuTitle);

	LCD_DisplayStr(100, 260, FONT_SIZE16, (char*)pMenu_SetUIntValue[1], LCD_Color_Blue);
	sprintf(DisplayBuf, "%d", uiOldValue);
	LCD_DisplayStr(100, 260-(11*8), FONT_SIZE24, DisplayBuf, LCD_Color_Blue);

	memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
	strcpy(DisplayBuf, pMenu_SetUIntValue[2]);
	len = strlen(DisplayBuf);	
	sprintf(&DisplayBuf[len], "%d", uiNewValue);
	LCD_DisplayStr(140, 260, FONT_SIZE16, DisplayBuf, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_ChoiceAdminMenu
* Description    : 选择管理等级菜单
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_ChoiceAdminMenu(void)
{
	GUI_PromptBox(pMenu_ChoiceAdmin[0]);

    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, pMenu_ChoiceAdmin[2], LCD_Color_Blue);
	LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32*2, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, pMenu_ChoiceAdmin[3], LCD_Color_Blue);
    LCD_DisplayStr(Promptbox.s_x+FONT_SIZE32*3, Promptbox.e_y-FONT_SIZE16, FONT_SIZE16, pStr_PlsChoiceType, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_AdminCardOpertion
* Description    : 管理员登录-->(查询)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_AdminCardOpertion(void)
{
	memset(MenuIconBuf, 0x0, sizeof(MenuIconBuf));
	MenuIconBuf[1] = ICON_Wifi;
	MenuIconBuf[2] = ICON_RecycleBin;

	GUI_ShowMenu(&pMenu_AdminCardOpertion[0], MenuIconBuf, 2);
}

/*******************************************************************************
* Function Name  : GUI_AdminMenu
* Description    : 用户管理员登录主界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_AdminMenu(void)
{
	memset(MenuIconBuf, 0x0, sizeof(MenuIconBuf));
	MenuIconBuf[1] = ICONS_003;
	MenuIconBuf[2] = ICONS_002;
	MenuIconBuf[3] = ICONS_004;
	MenuIconBuf[4] = ICONS_001;
	MenuIconBuf[5] = ICON_Find;
	MenuIconBuf[6] = ICON_Airplane;

	GUI_ShowIconMenu(&pMenu_Admin[0], MenuIconBuf, 6);
}

/*******************************************************************************
* Function Name  : GUI_Admin_SysSetMenu
* Description    : 管理员登录-->(系统)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_Admin_SysSetMenu(void)
{
	memset(MenuIconBuf, 0x0, sizeof(MenuIconBuf));
	MenuIconBuf[1] = ICON_Teacup;
	MenuIconBuf[2] = ICON_Clock;
	MenuIconBuf[3] = ICON_Loudspeaker;
	MenuIconBuf[4] = ICON_Airplane;
	MenuIconBuf[5] = ICON_ExclamationMark;
	MenuIconBuf[6] = ICON_NetBll;
	MenuIconBuf[7] = ICON_Hospital;
	MenuIconBuf[8] = 0;

	GUI_ShowIconMenu(&pMenu_Admin_SysSet[0], MenuIconBuf, 7);
}

/*******************************************************************************
* Function Name  : GUI_Admin_ManageMenu
* Description    : 管理员登录-->(管理)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_Admin_ManageMenu(void)
{
	memset(MenuIconBuf, 0x0, sizeof(MenuIconBuf));
	MenuIconBuf[1] = ICON_Lock;
	MenuIconBuf[2] = ICON_Wifi;
	MenuIconBuf[3] = ICON_House;
	MenuIconBuf[4] = ICON_Car;
	MenuIconBuf[5] = ICON_DateList;
	MenuIconBuf[6] = ICON_Teacup;
	MenuIconBuf[7] = ICON_RecycleBin;

	GUI_ShowMenu(&pMenu_Admin_Manage[0], MenuIconBuf, 7);
}

/*******************************************************************************
* Function Name  : GUI_Admin_BoxMenu
* Description    : 管理员登录-->(箱门)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_Admin_BoxMenu(void)
{
	memset(MenuIconBuf, 0x0, sizeof(MenuIconBuf));
	MenuIconBuf[1] = ICON_Microphone;
	MenuIconBuf[2] = ICON_Lockers;
	MenuIconBuf[3] = ICON_Bulb;
	MenuIconBuf[4] = ICON_Car;
	MenuIconBuf[5] = ICON_Lock;
	MenuIconBuf[6] = ICON_RotationLock;

	GUI_ShowMenu(&pMenu_Admin_Box[0], MenuIconBuf, 6);
}

/*******************************************************************************
* Function Name  : GUI_Admin_LockersMenu
* Description    : 管理员登录-->(寄存)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_Admin_LockersMenu(void)
{
	memset(MenuIconBuf, 0x0, sizeof(MenuIconBuf));
	MenuIconBuf[1] = ICON_DateList;
	MenuIconBuf[2] = ICON_ExclamationMark;
	MenuIconBuf[3] = ICON_Teacup;
	MenuIconBuf[4] = ICON_Microphone;

	GUI_ShowMenu(&pMenu_Admin_Lockers[0], MenuIconBuf, 4);
}

/*******************************************************************************
* Function Name  : GUI_Admin_QueryMenu
* Description    : 管理员登录-->(查询)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_Admin_QueryMenu(void)
{
	memset(MenuIconBuf, 0x0, sizeof(MenuIconBuf));
	MenuIconBuf[1] = ICON_Lockers;
	MenuIconBuf[2] = ICON_ZY;
	MenuIconBuf[3] = ICON_Airplane;
	MenuIconBuf[4] = ICON_House;
	MenuIconBuf[5] = ICON_Car;
	MenuIconBuf[6] = ICON_Lock;
	MenuIconBuf[7] = ICON_DateList;

	GUI_ShowMenu(&pMenu_Admin_Query[0], MenuIconBuf, 7);
}

/*******************************************************************************
* Function Name  : GUI_TopAdminMenu
* Description    : 高级管理员登录-->(系统)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_TopAdminMenu(void)
{
	memset(MenuIconBuf, 0x0, sizeof(MenuIconBuf));
	MenuIconBuf[1] = ICONS_003;
	MenuIconBuf[2] = ICON_Airplane;

	GUI_ShowIconMenu(&pMenu_TopAdmin[0], MenuIconBuf, 2);
}

/*******************************************************************************
* Function Name  : GUI_TopAdmin_SysSetMenu
* Description    : 高级管理员登录-->(系统)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_TopAdmin_SysSetMenu(void)
{
	memset(MenuIconBuf, 0x0, sizeof(MenuIconBuf));
	MenuIconBuf[1] = ICON_Lock;
	MenuIconBuf[2] = ICON_Wifi;
	MenuIconBuf[3] = ICON_ZY;
	MenuIconBuf[4] = ICON_Gear;
	MenuIconBuf[5] = ICON_Lockers;
	MenuIconBuf[6] = ICON_Bulb;
	MenuIconBuf[7] = ICON_Teacup;

	GUI_ShowIconMenu(&pMenu_TopAdmin_Set[0], MenuIconBuf, 7);
}

/*******************************************************************************
* Function Name  : GUI_SuperAdmin_SysSetMenu
* Description    : 超级管理员登录-->(系统)菜单界面
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_SuperAdmin_SysSetMenu(void)
{
	memset(MenuIconBuf, 0x0, sizeof(MenuIconBuf));
	MenuIconBuf[1] = ICON_Lock;
	MenuIconBuf[2] = ICON_Hospital;
	MenuIconBuf[3] = ICON_Ascii;
	MenuIconBuf[4] = ICON_Bulb;
	MenuIconBuf[5] = ICON_Lockers;

	GUI_ShowIconMenu(&pMenu_SuperAdmin_Set[0], MenuIconBuf, 5);
}

/*******************************************************************************
* Function Name  : GUI_LockersNotice
* Description    : 寄存柜存取提示信息
* Input          : Type--信息类型
* Output         : None
* Return         : None
* Notes          : 根据类型选择要显示的信息
*******************************************************************************/
void GUI_LockersNotice(uchar8 Type)
{
	switch(Type)
	{
		case LOCKERS_SysError:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_SysError, LCD_Color_Red);
		}
		break;

		case LOCKERS_AdminCard:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 220, FONT_SIZE24, pStr_AdminCard, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_CardNotConsign:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_CardNotConsign, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_NotExtractTime:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_NotExtractTime, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_OpenBoxFaild:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(100, 260, FONT_SIZE24, pStr_OpenBoxFaild, LCD_Color_Red);
			LCD_DisplayStr(150, 280, FONT_SIZE24, pStr_PlsCallAdmin, LCD_Color_Red);
		}
		break;

		case LOCKERS_PlsCard:
		{			
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_PlsCard, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_IsFull:
		{			
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_LockersIsFull, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_OpeningBox:
		{			
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_OpeningBox, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_CardUnauthorized:
		{			
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_CardNotRegister, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_NonSysCard:
		{			
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_NonSysCard, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_Overdue:
		{			
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_Overdue, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_LimitOfUse:
		{			
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(100, 260, FONT_SIZE24, pStr_LimitOfUse, LCD_Color_Red);
			LCD_DisplayStr(150, 260, FONT_SIZE24, pStr_PlsGotoOtherLockers, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_WriteCardFaild:
		{			
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(100, 260, FONT_SIZE24, pStr_WriteCardFaild, LCD_Color_Red);
			LCD_DisplayStr(150, 260, FONT_SIZE24, pStr_PlsConsignAgain, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_RepairCard:
		{			
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(100, 260, FONT_SIZE24, pStr_ConsignError, LCD_Color_Red);
			LCD_DisplayStr(150, 280, FONT_SIZE24, pStr_RestoreCard, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_ClrCardStoreInfo:
		{			
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(100, 260, FONT_SIZE24, pStr_OpenBoxFaild, LCD_Color_Red);
			LCD_DisplayStr(150, 260, FONT_SIZE24, pStr_PlsCardAgain, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_PlsCloseBoxDoor:
		{			
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_PlsCloseBoxDoor, LCD_Color_Red);
		}
		break;

		case LOCKERS_PlsFingerOrCard:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_PlsFingerOrCard, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_WaitTimeout:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_WaitTimeout, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_NoDeposit:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_NoDeposit, LCD_Color_Red);
		}
		break;
		
		case LOCKERS_NetError:
		{
			GUI_PromptBox(pStr_OperationTip);	
			LCD_DisplayStr(110, 260, FONT_SIZE24, pStr_NetError, LCD_Color_Red);
		}
		break;
	}
}

/*******************************************************************************
* Function Name  : GUI_PlsCloseBoxDoor
* Description    : 提示箱未关,请先关好箱门
* Input          : box -- 开启的箱号 
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_PlsCloseBoxDoor(uchar8 box)
{
	char DisplayBuf[100] = {0};
	GUI_PromptBox(pStr_OperationTip);

	memcpy(DisplayBuf, (char*)SysConfig.stBox[box].BoxSN, MAX_BoxSnLen);
	LCD_DisplayStr(100, 260, FONT_SIZE32, DisplayBuf, LCD_Color_Red);	
	LCD_DisplayStr(100, 250-(2*FONT_SIZE16), FONT_SIZE24, pStr_xBoxNotClose, LCD_Color_Blue);

	LCD_DisplayStr(160, 240, FONT_SIZE24, pStr_PlsCloseThisBox, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_UserConsignFail
* Description    : 提示开箱失败,请重新寄存
* Input          : box -- 开启失败的箱号 
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_UserConsignFail(uchar8 box)
{
	char DisplayBuf[100] = {0};
	GUI_PromptBox(pStr_OperationTip);

	memcpy(DisplayBuf, (char*)SysConfig.stBox[box].BoxSN, MAX_BoxSnLen);
	LCD_DisplayStr(100, 280, FONT_SIZE32, DisplayBuf, LCD_Color_Red);	
	LCD_DisplayStr(100, 270-(2*FONT_SIZE16), FONT_SIZE24, pStr_xBoxOpenFail, LCD_Color_Blue);

	LCD_DisplayStr(160, 260, FONT_SIZE24, pStr_PlsConsignAgain, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_UserTakeAwwyFail
* Description    : 提示用户取件开箱失败,请联系管理员
* Input          : box -- 开启失败的箱号 
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_UserTakeAwwyFail(uchar8 box)
{
	char DisplayBuf[100] = {0};
	GUI_PromptBox(pStr_OperationTip);

	memcpy(DisplayBuf, (char*)SysConfig.stBox[box].BoxSN, MAX_BoxSnLen);
	LCD_DisplayStr(100, 280, FONT_SIZE32, DisplayBuf, LCD_Color_Red);	
	LCD_DisplayStr(100, 270-(2*FONT_SIZE16), FONT_SIZE24, pStr_xBoxOpenFail, LCD_Color_Blue);

	LCD_DisplayStr(160, 280, FONT_SIZE24, pStr_PlsCallAdmin, LCD_Color_Blue);
}

/*******************************************************************************
* Function Name  : GUI_ShowIDPosition
* Description    : 提示用户的物品在Nx号柜的Nx号箱
* Input          : u8Locker--柜号, u8Box--箱号,
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_ShowIDPosition(uchar8 u8Locker, uchar8 u8Box)
{
	char DisplayBuf[100] = {0};
	uchar8 strlength = 0; 
	GUI_PromptBox(pStr_OperationTip);

	if(0 == u8Locker)
	{
		memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
		strcpy(DisplayBuf, pStr_ThisLockers);
		LCD_DisplayStr(90, 255, FONT_SIZE24, DisplayBuf, LCD_Color_Blue);		
		memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
		sprintf(DisplayBuf, "%02d", u8Box);
		LCD_DisplayStr(80, 200, FONT_SIZE64, DisplayBuf, LCD_Color_Red);		
		LCD_DisplayStr(90, 130, FONT_SIZE24, pStr_xBox , LCD_Color_Blue);

		strlength = strlen(pStr_PlsCardToPickUp);
		LCD_DisplayStr(145, (Promptbox.e_y-Promptbox.s_y)/2+Promptbox.s_y+((FONT_SIZE24*strlength)/2)/2, FONT_SIZE24, pStr_PlsCardToPickUp, LCD_Color_Blue);
	}
	else
	{
		strlength = strlen(pStr_IdPosition);
		LCD_DisplayStr(90, (Promptbox.e_y-Promptbox.s_y)/2+Promptbox.s_y+((FONT_SIZE24*strlength)/2)/2, FONT_SIZE24, pStr_IdPosition, LCD_Color_Blue);
		
		DisplayBuf[0] = (u8Locker+0x40);
		LCD_DisplayStr(125, 270, FONT_SIZE64, DisplayBuf, LCD_Color_Red);
		
		memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
		strcpy(DisplayBuf, pStr_xLockers);
		strlength = strlen(DisplayBuf);
		LCD_DisplayStr(145, 230, FONT_SIZE24, DisplayBuf, LCD_Color_Blue);
		
		memset(DisplayBuf, 0x0, sizeof(DisplayBuf));
		sprintf(DisplayBuf, "%02d", u8Box);
		LCD_DisplayStr(125, 200, FONT_SIZE64, DisplayBuf, LCD_Color_Red);		
		LCD_DisplayStr(145, 130, FONT_SIZE24, pStr_xBox , LCD_Color_Blue);
	}
}

/*******************************************************************************
* Function Name  : GUI_PackageInOtherLockers
* Description    : 提示用户的物品别的箱上
* Input          : pLockersNmae--柜名, u8Box--箱号,
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_PackageInOtherLockers(const char* pLockersNmae, uchar8 u8Box)
{
	char DisplayBuf[100] = {0};
	char tempBuf[10] = {0};
	uchar8 len = 0; 
	uchar8 font = FONT_SIZE24;
	GUI_PromptBox(pStr_OperationTip);

	LCD_DisplayStr(90, 255, FONT_SIZE24, pStr_IdPosition, LCD_Color_Blue);	
	len = strlen(pLockersNmae);
	if(0 == len)
	{
		font = FONT_SIZE24;
		strcpy(DisplayBuf, pStr_OnOtherLockers);
	}
	else
	{
		font = FONT_SIZE32;
		if(len > MAX_NameLen)
		{
			len = MAX_NameLen;
		}
		memcpy(DisplayBuf, pLockersNmae, len);
		sprintf(tempBuf, "--%d", u8Box);
		strcat(DisplayBuf, tempBuf);
	}
	LCD_DisplayStr(130, 255, font, DisplayBuf, LCD_Color_Red);		                   
}

/*******************************************************************************
* Function Name  : GUI_LockersParameters
* Description    : 显示箱柜参数
* Input          : void
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_LockersParameters(void)
{
	char LockersArg[9][50];	
	const char* pLockers[9];
	char i = 0;
	for(i=0; i<9; i++)
	{
		pLockers[i] = LockersArg[i];
	}

	strcpy(&LockersArg[0][0], "Lockers Parameters");
	sprintf(&LockersArg[1][0], "SysCode:%s", SysConfig.stClientInfo.IdCode);
	sprintf(&LockersArg[2][0], "ID: %d", SysConfig.stHardware.u32MachineNum);
	sprintf(&LockersArg[3][0], "Name: %s", SysConfig.stLockers.Name);
	sprintf(&LockersArg[4][0], "WorkMode: %d", SysConfig.stLockers.u8WorkMode);
	sprintf(&LockersArg[5][0], "OpenBoxes: %d", SysConfig.stLockers.u8AllowBeOpenedBoxNum);
	sprintf(&LockersArg[6][0], "CT-min: %d CT-max: %d", SysConfig.stLockers.iMinFetchTime, SysConfig.stLockers.iMaxFetchTime);
	sprintf(&LockersArg[7][0], "Timeout: %d", SysConfig.stLockers.iHandleTimeout);
	sprintf(&LockersArg[8][0], "T-Alarm: %d", SysConfig.stLockers.iNoticeTime);

	GUI_ShowListMenu(pLockers, 8);
}

/*******************************************************************************
* Function Name  : GUI_NetParameters
* Description    : 显示箱柜网络参数
* Input          : void
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_NetParameters(void)
{
	char LockersArg[9][50];	
	const char* pLockers[9];
	char i = 0;
	for(i=0; i<9; i++)
	{
		pLockers[i] = LockersArg[i];
	}

	strcpy(&LockersArg[0][0], "Network Parameters");
	sprintf(&LockersArg[1][0], "IP:%d.%d.%d.%d", SysConfig.stNet.IP[0], SysConfig.stNet.IP[1],SysConfig.stNet.IP[2],SysConfig.stNet.IP[3]);
	sprintf(&LockersArg[2][0], "Mark:%d.%d.%d.%d", SysConfig.stNet.NetMask[0], SysConfig.stNet.NetMask[1],SysConfig.stNet.NetMask[2],SysConfig.stNet.NetMask[3]);
	sprintf(&LockersArg[3][0], "Getaway:%d.%d.%d.%d", SysConfig.stNet.Gateway[0], SysConfig.stNet.Gateway[1],SysConfig.stNet.Gateway[2],SysConfig.stNet.Gateway[3]);

	sprintf(&LockersArg[4][0], "UDP-Port: %d, TCP-Port: %d", SysConfig.stNet.u16UDP_ListenPort, SysConfig.stNet.u16TCP_ListenPort);
	sprintf(&LockersArg[5][0], "ServerIP:%d.%d.%d.%d", SysConfig.stNet.ServerIP[0], SysConfig.stNet.ServerIP[1],SysConfig.stNet.ServerIP[2],SysConfig.stNet.ServerIP[3]);
	sprintf(&LockersArg[6][0], "S-UDP: %d S-TCP: %d", SysConfig.stNet.u16UDP_ServerPort, SysConfig.stNet.u16TCP_ServerPort);

	GUI_ShowListMenu(pLockers, 6);
}

/*******************************************************************************
* Function Name  : GUI_UserAccess
* Description    : 用户寄存物--提示框显示
* Input          : Type---内容标号
* Output         : None
* Return         : None
*******************************************************************************/
void GUI_UserAccess(uint32 Type)
{
	u32 strlength;
	const char *str;

	str = pMenu_UserAccess[Type];
	strlength =strlen((char*)str); 

	GUI_PromptBox(pStr_OperationTip);

	LCD_DisplayStr((Promptbox.e_x-Promptbox.s_x)/2+Promptbox.s_x-FONT_SIZE24/2, (Promptbox.e_y-Promptbox.s_y)/2+Promptbox.s_y+((FONT_SIZE24*strlength)/2)/2, FONT_SIZE24, (char*)str, LCD_Color_Blue);
}

/*===================================File end===================================================*/ 

