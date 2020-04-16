#include "include.h"

extern u8 Hold_Open_Door;
ic IC;
fin Fin;
pwd Pwd;
flash Flash;
SumStr SumGen;
u8 voice_flag = 0,page_ic = 0;
u16 icreadcnt = 0,value_nums = 0,num1 = 0;
unsigned char Input[11],input_num;
static u8 page_turn_flag = 0,page_system = 0,page_showflag = 0;
static u8 page_flag = 0,pagesys_flag = 0,page_second = 0,page_third = 0,page_test = 0,page_test2 = 0;
 

u8 menu(void)
{
	unsigned char Key_Value = 0xff;
	u8 iRet = 0xff, iRet1 = 0xff, sta = 0,ic_sta,wifi_time=0,iret = 0xff,fin_num,open_way_flag=0,empty_sta=0xff;
	u16 times = 100, i, values = 0xffff;
	u32 buf = 0;
	u8 Pwd_iret = 0xff;
	u8 key_end = 0;//∞¥º¸∑∂ŒßΩ·Œ≤
	iRet = iRet;
	iRet1 = iRet1;
	sta = sta;
	iret = iret;
	fin_num = fin_num;
	times = times;
	ic_sta = ic_sta;
	wifi_time = wifi_time;
	values = values;
	buf = buf;
	i = i;
	Key_Value = KEY_Scan3x4();
	TimingDelay_flag = 0;//ø™∂® ±∆˜

	if((Page0 > '0' && Page1 == 0 && Page2 == 0 && Page3 == 0) || (Page0_0 > '0' && Page1_1 == 0 && Page2_2 == 0 && Page3_3 == 0 ))//÷˜≤Àµ•
	{
		if(page_showflag == 0)
		{
			OLED_Clear();
			page_showflag = 1;
		}
		if(Page0 == '1')
		{
			if(voice_flag == 0)
			{
				JQ8400_SendCmd(4,1,1,1);  //≤Àµ•µ⁄“ª“≥
				voice_flag = 1;
			}
			menu_page1_show();
			OLED_Refresh_Gram();
		}
		if(Page0_0 == '1')
		{
			if(voice_flag == 0)
			{
				JQ8400_SendCmd(8,2,1,1);
				voice_flag = 1;
			}
			menu_page2_show();
		  OLED_Refresh_Gram();
		}
		if(Key_Value == '*') //ÕÀ≥ˆ
		{
			JQ8400_SendCmd(2,9,1,1);
			delay_ms(300);
			show_flag = 0;
			Mode = 3;
			return 0;
		}

		if(Key_Value == '#') 
		{
			JQ8400_SendCmd(2,9,1,1);
			delay_ms(300);
			page_turn_flag = 1;
			OLED_Clear();
			voice_flag = 0;
			if(page_flag % 2 == 0)
			{
				Page0_0 = '1';
				Page0 = 1;
			}
			else 
			{
				Page0_0 = 0;
				Page0 = '1';
			}
			page_flag++;
			if(page_flag == 2)
				page_flag = 0;
		}
		if(Page0 == '1')
		{
			if(Key_Value > '0' && Key_Value <= '3')
			{
				OLED_Clear();
				Page1 = Key_Value;
				Page0 = 1;
				page_test = Page1;
				voice_flag = 0;
				page_showflag = 0;
				JQ8400_SendCmd(2,9,1,1);
				delay_ms(300);
			}
		}
		if(Page0_0 == '1')
		{
			if(Key_Value > '3' && Key_Value < '6')
			{
				OLED_Clear();
				Page1_1 = Key_Value;
				Page0_0 = 1;
				page_test = Page1_1;
				voice_flag = 0;
				page_showflag = 0;
				JQ8400_SendCmd(2,9,1,1);
				delay_ms(300);
			}
		
		}
	}
	else if((Page0 < '0' && Page1 > '0' && Page2 == 0 && Page3 == 0) || (Page0_0 < '0' && Page1_1 > '0' && Page2_2 == 0 && Page3_3 == 0)) //¥Œº∂≤Àµ•
	{
		  Page1 = page_test;
		  Page1_1 = page_test;
			if(Page1 == '1')//π‹¿Ì‘±”√ªß
			{
				if(voice_flag == 0)
				{
				   JQ8400_SendCmd(4,7,1,1);
					 voice_flag = 1;
				}
				page_second = 1;
				ManSet();
				OLED_Refresh_Gram();
				key_end='3';

			}
			else if(Page1 == '2')//∆’Õ®”√ªß
			{
				page_second = 2;
				if(voice_flag == 0)
				{
				   JQ8400_SendCmd(4,0,1,1);
					 voice_flag = 1;
				}
				UserSet(); 
				OLED_Refresh_Gram();
				key_end='2';
			}
			else if(Page1 == '3')  //‘∂≥Ã”√ªß…Ë÷√
			{
				page_second = 3;
				if(voice_flag == 0)
				{
				   JQ8400_SendCmd(8,1,1,1);
					 voice_flag = 1;
				}
			  Remoteset();
				OLED_Refresh_Gram();
				key_end='2';
			}
			else if(Page1_1 == '4' && page_system != 1)//œµÕ≥…Ë÷√
			{
				page_second = 4;
				if(voice_flag == 0)
				{
						JQ8400_SendCmd(5,2,1,1);
						voice_flag = 1;
				}
				SYSSetONE();
				OLED_Refresh_Gram();
				key_end='7';
			}
			else if(Page1_1 == '5') //º«¬º≤È—Ø
			{
				page_second = 5;
				if(voice_flag == 0)
				{
						JQ8400_SendCmd(4,2,1,1);
						voice_flag = 1;
				}
			  Recordquerypage();
				OLED_Refresh_Gram();
				key_end='2';
			}
			if(Key_Value == '*')
			{
					OLED_Clear();
					JQ8400_SendCmd(2,9,1,1);
					delay_ms(300);
					menu_now = TimingDelay;
				  ReSet_Flag(1);
					return 1;
			}
			if(Key_Value == '#' && Page1_1 == '4')
			{
				JQ8400_SendCmd(2,9,1,1);
				delay_ms(300);
				OLED_Clear();
				voice_flag = 0;
				if(pagesys_flag  % 2 == 0)
				{
					if(page_second == 4)
					{
						JQ8400_SendCmd(5,3,1,1);
					}
					key_end='7';
					page_system = 1;
					SYSSettwo();
					OLED_Refresh_Gram();
				}
				else
				{
					if(page_second == 4)
					{
						JQ8400_SendCmd(5,2,1,1);
					}
					key_end='3';
					page_system = 0;
					SYSSetONE();
					OLED_Refresh_Gram();
				}
				
				pagesys_flag++;
				if(pagesys_flag == 2)
					pagesys_flag = 0;
		  }   
			if(Key_Value > '0' ) 
			{
				JQ8400_SendCmd(2,9,1,1);
				delay_ms(300);
				page_third = SetPage(page_second,Key_Value);
/*/					if(page_second == 1)
//					{
//						if(Key_Value == '1')page_third = 1;
//						else if(Key_Value == '2')page_third = 2;
//					}
//					else if(page_second == 2)
//					{
//						if(Key_Value == '1')page_third = 3;
//						else if(Key_Value == '2')page_third = 4;
//						else if(Key_Value == '3')page_third = 5;
//					}
//					else if(page_second == 3)
//					{
//						if(Key_Value == '1')page_third = 6;
//						else if(Key_Value == '2')page_third = 7;
//					}
//					else if(page_second == 4)
//					{
//						if(Key_Value == '1')page_third = 8;
//						else if(Key_Value == '2')page_third = 9;
//						else if(Key_Value == '3')page_third = 10;
//						else if(Key_Value == '4')page_third = 11;
//						else if(Key_Value == '5')page_third = 12;
//						else if(Key_Value == '6')page_third = 13;
/					}     */
				OLED_Clear();
				Page2 = Key_Value;
				Page0 = 1;
				Page1 = 1;
				Page1_1 = 0;
				page_test2 = Page2;
				voice_flag = 0;
			}
	}
	else if(Page0 < '0' && Page1< '0' && Page2 != 0 && Page3 == 0)//¥Œ¥Œº∂≤Àµ•
	{
		Page2 =  page_test2;
		if(Page2 == '1')
		{
			if(page_second == 1)
			{
				key_end = '2';
				if(voice_flag == 0)
				{
					JQ8400_SendCmd(1,6,1,1);
					voice_flag = 1;
				}
				figsetpage(0);
				OLED_Refresh_Gram();
			}
			else if(page_second == 2)
			{
				if(voice_flag == 0)
				{
					JQ8400_SendCmd(4,3,1,1);
					voice_flag = 1;
				}
				key_end = '3';
				figsetpage(1);
				OLED_Refresh_Gram();
			}
			else if(page_second == 3)
			{
				if(voice_flag == 0)
				{
					JQ8400_SendCmd(4,9,1,1);
					voice_flag = 1;
				}
				
				key_end = '2';
				openorclose();
				OLED_Refresh_Gram();
			}
			else if(page_second == 4)
			{
				if(voice_flag == 0)
				{
					JQ8400_SendCmd(4,9,1,1);
					voice_flag = 1;
				}
				key_end = '1';
				yesorno();
				OLED_Refresh_Gram();
			}
			else if(page_second == 5)
			{
				key_end = '1';
				Model_select(0,1,1);
				OLED_Refresh_Gram();
			}
		}
		else if(Page2 =='2')
		{
			if(page_second == 1)
			{
				if(voice_flag == 0)
				{
					JQ8400_SendCmd(1,7,1,1);
					voice_flag = 1;
				}
				key_end = '2'; 
				pwdsetpage(0);
				OLED_Refresh_Gram();
			}
			else if(page_second == 2)
			{
				if(voice_flag == 0)
				{
					JQ8400_SendCmd(4,4,1,1);
					voice_flag = 1;
				}
				key_end = '3';
				pwdsetpage(1);
				OLED_Refresh_Gram();
			}
			else if(page_second == 3)
			{
				if(voice_flag == 0)
				{
					JQ8400_SendCmd(5,5,1,1);
					voice_flag = 1;
				}
				key_end = '2';
				wifisetPage();
				OLED_Refresh_Gram();
			}
			else if(page_second == 4)
			{
				if(voice_flag == 0)
				{
					JQ8400_SendCmd(5,1,1,1);
					voice_flag = 1;
				}
				key_end = '3';
				OpenWayModeOne();
				OLED_Refresh_Gram();
			}
			else if(page_second == 5)
			{
				key_end = '1';
				information(Pwd.Users_Num , Fin.Users_Num, IC.Users_Num);
				OLED_Refresh_Gram();
			}
		}
		else if(Page2 == '3')
		{
			if(page_second == 2)
			{
				if(voice_flag == 0)
				{
					JQ8400_SendCmd(4,5,1,1);
					voice_flag = 1;
				}
				key_end = '3';
				ICetpage();
				OLED_Refresh_Gram();
			}
			else if(page_second == 4)
			{
				if(voice_flag == 0)
				{
					JQ8400_SendCmd(4,9,1,1);
					voice_flag = 1;
				}
				key_end = '2';
				openorclose();
				OLED_Refresh_Gram();
			}
		}
		else if(Page2 == '4')
		{
			if(page_second == 4)
			{
				if(voice_flag == 0)
				{
					JQ8400_SendCmd(7,6,1,1);
					voice_flag = 1;
				}
				key_end = '3';
				Volume_adjust();
				OLED_Refresh_Gram();
			}
		}
		else if(Page2 == '5')
		{
			if(page_second == 4)
			{
				if(voice_flag == 0)
				{
					JQ8400_SendCmd(4,8,1,1);
					voice_flag = 1;
				}
				key_end = '2';
				languagePage();
				OLED_Refresh_Gram();
			}
		}
		else if(Page2 == '6')       //ª÷∏¥≥ˆ≥ß
		{
			if(page_second == 4)
			{
				reset_sw = RESET_Switch();
				if(reset_sw == 0)
				{
					JQ8400_SendCmd(1,9,1,1);
					delay_ms(800);
					
					Clear_Data(ALL);
					Mode = 0;
					flash_empty(0);
					ClearSum(0,2);//«Èø’À˘”–
					re_flag = 0;
					NVIC_SystemReset();//÷ÿ∆Ù
				}
				else 
				{
				  ReSet_Flag(2);
				}
				
			}
		}
		if(Key_Value == '*')
		{
			JQ8400_SendCmd(2,9,1,1);
			delay_ms(300);
			OLED_Clear();
			menu_now = TimingDelay;
			ReSet_Flag(2);
			return 1;
		}
	 if(Key_Value > '0' && Key_Value <= key_end)
	 {
			JQ8400_SendCmd(2,9,1,1);
			delay_ms(300);
			Page3 = Key_Value;
			Page0 = 1;
		  Page2 = 0;
			Page1_1 = 0;
		  voice_flag = 0;
	 }
	}
	else if(Page0 < '0' && Page1 < '0' && Page2 == 0 && Page3 != 0)// µœ÷¥Œº∂≤Àµ•
	{
		  if(Page3 == '1')
			{
				 if(page_third == 1)//π‹¿ÌÃÌº”÷∏Œ∆
				 {
					  if(page_showflag == 0)
						{
							OLED_Clear();
						  enterpwdpage(0);
							OLED_Refresh_Gram();
							page_showflag = 1;
							JQ8400_SendCmd(3,8,1,1);//«Î ‰»Î÷∏Œ∆
						}
						Num_show(Management, FINPT, Fin.Man_Save_Num);
					  OLED_Refresh_Gram();
						if(Pwd.Man_Num >= 2)
						{
							if(Fin.Man_Num >= 5)
							{
								JQ8400_SendCmd(7,3,1,1);//π‹¿Ì‘±“—¬˙
								delay_ms(1000);
								OLED_Clear();           
								menu_now = TimingDelay;
								ReSet_Flag(3);
							}
						}
						if(voice_flag == 3)
						{
							JQ8400_SendCmd(6,2,1,1);//÷∏Œ∆ø‚“—¬˙
							voice_flag = 0;
						}
				
						if((FigOnFlag == 1) && (Fin.Man_Num < 5))//ºÏ≤‚µΩ÷∏Œ∆
						{
							delay_ms(50);
							pwd_add(NONE,Key_Value);
							if(FigOnFlag == 1)//ºÏ≤‚µΩ÷∏Œ∆
							{
								menu_now = TimingDelay;
								iret=AutoEnrollPro(Management); //◊¢≤·“ª∏ˆ÷∏Œ∆
								if(iret==ERR_OVERTIME||iret==0xFE||iret==ERR_OUTOFINDEX)
								{								
									menu_now = TimingDelay;
									OLED_Clear();
									ReSet_Flag(3);
									voice_flag = 0;
								}
								else if(iret == ERR_OK)
								{
									AddSum(1,0);//÷∏Œ∆1 √‹¬Î2 IC 3   π‹¿Ì‘±0 ∆’Õ®”√ªß1
//									VoiceNum(0,0);	
									flash_save(31, Fin.Man_Num);
									flash_save(39, Fin.Man_Save_Num);
									if(Fin.Man_Num >= 5)
									{
//										Voice_1A_WT588S(0x000D);
										delay_ms(1500);
									}
									JQ8400_SendCmd(1,9,1,1);  //≤Ÿ◊˜≥…π¶
									delay_ms(1000);
									OLED_Clear();
									ReSet_Flag(3);
								}
								else if(iret == ERR_TEMPLATEEXIST)
								{
									JQ8400_SendCmd(2,0,1,1);  //≤Ÿ◊˜ ß∞‹
									delay_ms(1000);
									OLED_Clear();           
									menu_now = TimingDelay;
									ReSet_Flag(3);
								}
								menu_now = TimingDelay;
								voice_flag = 0;
							}
						}
				 }
				 else if(page_third == 2)//π‹¿ÌÃÌº”√‹¬Î
				 {    
						if(page_showflag == 0)
						{
							OLED_Clear();
							enterpwdpage(1);
							OLED_Refresh_Gram();
							page_showflag = 1;
						}
						Num_show(Management, PASSWORD, Pwd.Man_Save_Num);
					  OLED_Refresh_Gram();
						if(Pwd.Man_Num >= 2)
						{
							if(Fin.Man_Num >= 5)
							{
								JQ8400_SendCmd(7,3,1,1);//π‹¿Ì‘±“—¬˙
								delay_ms(1000);
								OLED_Clear();           						
								ReSet_Flag(3);
							}
						}
						
						if((((Key_Value >= '0') && (Key_Value <= '9'))|| (Key_Value == '#'))&& (Pwd.Man_Num < 2))//”–∞¥º¸ ‰»Î
						{
							if(Pwd.Man_Num >= 2)
							{
//								Voice_1A_WT588S(0x000E);
								delay_ms(2000);
							}	
							else 
							{
								if(Pwd.ManFullFlag == 1)
								{
									for(i = 0;i<2;i++)
									{
										Pwd.Man_Save_Num=i;
										break;
									}
								
								}
								Pwd_iret = pwd_add(Management,Key_Value);
								if(Pwd_iret == 0)
								{
									AddSum(2,0);//2 √‹¬Î  1 ∆’Õ®”√ªß
//									VoiceNum(0,0);
									flash_save(32, Pwd.Man_Num);
								  flash_save(40, Pwd.Man_Save_Num);
									Pwd_iret = 0xff;
									JQ8400_SendCmd(1,9,1,1);  //≤Ÿ◊˜≥…π¶
									delay_ms(1000);
									
									OLED_Clear();           
									ReSet_Flag(3);
								}
								else if(Pwd_iret != 4 && Pwd_iret < 6)
								{
									
								}
							}
							
							if(Pwd.Man_Num >= 2)
							{
//								Voice_1A_WT588S(0x000E);
								delay_ms(2000);
							}
							Pwd_iret = 0xff; 
							menu_now = TimingDelay;
							Key_Value = 0;
						}
				 }
				 else if(page_third == 3)//”√ªßÃÌº”÷∏Œ∆
				 {
						if(page_showflag == 0)
						{
							OLED_Clear();
						  enterpwdpage(0);
							OLED_Refresh_Gram();
							page_showflag = 1;
							JQ8400_SendCmd(3,8,1,1);//«Î ‰»Î÷∏Œ∆
						}
					 Num_show(User, FINPT, Fin.Users_Save_Num);
					 OLED_Refresh_Gram();
					 if(Pwd.Users_Num > PWD_USER_MAX)
					 {
						 if(Fin.Users_Num >= FIN_USER_MAX)
						 {
								//ø’º‰¬˙”Ô“Ù
								OLED_Clear();           
								ReSet_Flag(3);			 
						 }
					 }
					 if(voice_flag == 3)
					 {
						 
						 delay_ms(500);
					   voice_flag = 0;
					 }
					 if((FigOnFlag == 1) && (Fin.Users_Num < FIN_USER_MAX)) //ºÏ≤‚µΩ÷∏Œ∆
					 {
							delay_ms(50);
						  pwd_add(NONE,Key_Value);
						  if(FigOnFlag == 1)//ºÏ≤‚µΩ÷∏Œ∆
							{
								iret=AutoEnrollPro(User); //◊¢≤·“ª∏ˆ÷∏Œ∆
								if(iret==ERR_OVERTIME||iret==0xFE||iret==ERR_OUTOFINDEX)
								{								
									menu_now = TimingDelay;
									OLED_Clear();
									ReSet_Flag(3);
									voice_flag = 0;
								}
								else if(iret == ERR_OK)
								{
									AddSum(1,1);//÷∏Œ∆1 √‹¬Î2 IC 3   π‹¿Ì‘±0 ∆’Õ®”√ªß1
	//								VoiceNum(1,0);	
									flash_save(33, Fin.Users_Num);
									flash_save(37, Fin.Users_Save_Num);
									
									if(Fin.Users_Num >= FIN_USER_MAX)
									{
										//”√ªß÷∏Œ∆“—¬˙
										delay_ms(1500);
									}
									JQ8400_SendCmd(1,9,1,1);  //≤Ÿ◊˜≥…π¶
									delay_ms(1000);
									
									OLED_Clear();           
									ReSet_Flag(3);
								}
								else if(iret == ERR_TEMPLATEEXIST)
								{
									JQ8400_SendCmd(3,1,1,1);
									delay_ms(1000);
									
								  OLED_Clear();           
									ReSet_Flag(3);
								}
								else
								{
								  OLED_Clear();           
									ReSet_Flag(3);
								}
								delay_ms(500);
								menu_now = TimingDelay;
							}
					 }
				 }
				 else if(page_third == 4)//”√ªßÃÌº”√‹¬Î
				 {
					  if(page_showflag == 0)
						{
							OLED_Clear();
							enterpwdpage(1);
							OLED_Refresh_Gram();
							page_showflag = 1;
						}
            Num_show(User, PASSWORD, Pwd.Users_Save_Num);
					  OLED_Refresh_Gram();
				    if((((Key_Value >= '0') && (Key_Value <= '9')) || (Key_Value == '#')) && (Pwd.Users_Num < PWD_USER_MAX))//”–∞¥º¸ ‰»Î
						{
							if(Pwd.Users_Num >= PWD_USER_MAX)
							{
								voice_flag = 3;
								if(voice_flag == 3)
								{
//									JQ8400_SendCmd(2,9,1,1); ø’º‰¬˙
									delay_ms(600);
									voice_flag = 0;
								}
							}	
							else 
							{
								if(Pwd.UserFullFlag==1)
								{
									for(i=2;i<100;i++)
									{
										if(Pwd.Value[i][0]==0)
										{	
											Pwd.Users_Save_Num=i;
											break;
										}
									}
								}
								
								Pwd_iret = pwd_add(User,Key_Value);
								if(Pwd_iret == 0)
								{							
									AddSum(2,1);//2 √‹¬Î  1 ∆’Õ®”√ªß
//									VoiceNum(1,0);			
									flash_save(34, Pwd.Users_Num);
									flash_save(38, Pwd.Users_Save_Num);
									Pwd_iret = 0xff;
									
									JQ8400_SendCmd(1,9,1,1);  //≤Ÿ◊˜≥…π¶
									delay_ms(1000);
									
									OLED_Clear();           
									ReSet_Flag(3);
									
									if(Pwd.Users_Num >= PWD_USER_MAX)
									{
										voice_flag = 3;
										if(voice_flag == 3)
										{
											JQ8400_SendCmd(2,9,1,1);
											delay_ms(600);
											voice_flag = 0;
										}
									}
								}
								else if(Pwd_iret != 4 && Pwd_iret < 6)
								{
									voice_flag = 0;	
								}
							}
							Pwd_iret = 0xff;
							menu_now = TimingDelay;
							Key_Value = 0;
						}
				 }
				 else if(page_third == 5)//ÃÌº”ICø®
				 {
					  OLED_Clear();  
					  JQ8400_SendCmd(3,7,1,1);//«Î ‰»ÎICø®
						if(IC.Users_Num >= IC_USER_MAX)
						{
								//ø’º‰¬˙”Ô“
						    JQ8400_SendCmd(6,6,1,1);
								delay_ms(800);
								OLED_Clear();           
							  ReSet_Flag(3);	
						}
						if(voice_flag == 3)
						{
							//ø’º‰¬˙
							voice_flag = 0;
						}
						buf = 0;
						touch_flag = 1;
						Num_show(User, ICCARD, IC.Save_Num);
						OLED_Refresh_Gram();
						menu_now = TimingDelay;
						while(!buf)
						{
							buf = Regist_ID(); //
							if(buf)
								break;
							if(TimingDelay - menu_now > TOUCH_Timeout/2)
							{
								menu_now = TimingDelay;
								OLED_Clear();           
								pagesys_flag = 0;
								ReSet_Flag(3);
								break;
							}
						}
						touch_flag=0;
						if(buf != 0)
						{
							if(IC.UserFullFlag == 1)
							{
								for(i=0;i<200;i++)
								{
									if(IC.Value[i]==0)
									{
										IC.Save_Num=i;
										break;
									}
								}
							}
							for(i = 0; i < IC_USER_MAX; i++)
							{
								if(IC.Value[i] == buf)
										break;
							}
							if(i == IC_USER_MAX)
							{
								if(IC.Users_Num < IC_USER_MAX)
								{
									IC.Value[IC.Save_Num] = buf;
									SumGen.Save_Num = IC.Save_Num;
									AddSum(3,1);//3 
//										VoiceNum(1,0);	
									save_flash(ICCARD,IC.Save_Num);										
									IC.Users_Num++;
									flash_save(35, IC.Users_Num);
									
									JQ8400_SendCmd(1,9,1,1); 
									delay_ms(800);
									buf = 0;
									OLED_Clear();           
									pagesys_flag = 0;
									ReSet_Flag(3);
									if(IC.UserFullFlag==0)
									{
										IC.Save_Num++;
										if(IC.Save_Num>=100)
										{
											IC.UserFullFlag=1;
											flash_save(46,IC.UserFullFlag);		
										}
										flash_save(58, IC.Save_Num);
									}
								}
								if(IC.Users_Num >= IC_USER_MAX)
								{
									JQ8400_SendCmd(6,6,1,1); 
									delay_ms(1500);
									OLED_Clear();           
							    ReSet_Flag(3);
									pagesys_flag = 0;
								}	
							}
							else
							{
								 JQ8400_SendCmd(2,4,1,1);  //”√ªß“—¥Ê‘⁄
								 delay_ms(1500);
									OLED_Clear();           
								  ReSet_Flag(3);
									pagesys_flag = 0;
							}
							delay_ms(400);
							menu_now = TimingDelay;				
							voice_flag = 0;
							pwd_add(NONE,Key_Value);
						}
				 }
				 else if(page_third == 6)//“£øÿø™
				 {
					  JQ8400_SendCmd(1,9,1,1);
						delay_ms(1000);
					  Remote_flag = 0;
					  ReSet_Flag(3);
				 }
				 else if(page_third == 7)//¡¨Ω”wifi
				 {
				 
				 }
				 else if(page_third == 8)//…Ë÷√ ±º‰
				 {
					
				 }
				 else if(page_third == 9)//µ•—È÷§
				 {
						JQ8400_SendCmd(1,9,1,1);
					  Verify_Flag = 0;
					  flash_save(48, Verify_Flag);
					  delay_ms(1000);
					  ReSet_Flag(3);
				 }
				 else if(page_third == 10)//≥£ø™√≈ ø™
				 {
					  Hold_Open_Door = ON;
					  if(Hold_Open_Door == ON)
						{
						  JQ8400_SendCmd(1,9,1,1);
							delay_ms(1000);
						}
//						Open_Door();
						OLED_Clear();           
					  ReSet_Flag(3);
						pagesys_flag = 0;
						voice_flag = 0;	
				 }
				 else if(page_third == 11)//“Ù¡øµ˜Ω⁄  ∏ﬂ
				 {
				    Volume_Flag=2;
						JQ8400_SetVolume(2);
					  if(Volume_Flag == 2)
						{
							JQ8400_SendCmd(1,9,1,1);
							delay_ms(1000);
						}
						OLED_Clear();           
					  ReSet_Flag(3);
						pagesys_flag = 0;
						voice_flag = 0;	
				 }
				 else if(page_third == 12)//”Ô—‘  ÷–Œƒ
				 {
					  Language = Chinese;
						Volume_Flag=2;
						OLED_Clear();           
						ReSet_Flag(3);
						pagesys_flag = 0;
						voice_flag = 0;	
				 }
				 else if(page_third == 13) //ª÷∏¥≥ˆ≥ß ø™
				 {
				 
				 }
			}
			else if(Page3 == '2')
			{
				 if(page_third == 1)//π‹¿Ì…æ≥˝÷∏Œ∆
				 {
						if((Fin.Man_Num == 0)  && (Pwd.Man_Num == 1))
						{
//							Voice_1A_WT588S(0x0019);//π‹¿Ì‘±ø’
							JQ8400_SendCmd(7,3,1,1);//π‹¿Ì‘±“—¬˙
							delay_ms(1000);
							OLED_Clear();           
						  ReSet_Flag(3);
							pagesys_flag = 0;
							voice_flag = 0;								
						}
						else
						{
						   if(voice_flag == 0)
							 {
								  JQ8400_SendCmd(7,0,1,1);//π‹¿Ì‘±“—¬˙
								  voice_flag = 1;
							 }
							 JQ8400_SendCmd(3,8,1,1);//«Î ‰»Î÷∏Œ∆
							 delay_ms(800);
							 if((FigOnFlag == 1) && (Fin.Man_Num > 0)) //ºÏ≤‚µΩ÷∏Œ∆
							 {
								 delay_ms(50); 
							   if(FigOnFlag == 1)//ºÏ≤‚µΩ÷∏Œ∆
								 {
									  if(((Verify_Flag == 1) || (Verify_Flag == 2)) && ((Fin.Users_Num + Fin.Man_Num)<= 1))
										{
											JQ8400_SendCmd(6,3,1,1);   //÷∏Œ∆ø‚ø’
											delay_ms(1500);
										}
										else if((Verify_Flag == 4) && ((Fin.Users_Num + Fin.Man_Num) <= 2))
										{
											JQ8400_SendCmd(6,3,1,1);   //÷∏Œ∆ø‚ø’
											delay_ms(1500);
										}
										else
										{
											fin_num = AutoIdentifyPro(2);
											if((fin_num == ERR_OK) && (FIN_FLAG == 0))
											{
												if(DeelOnFigAndUSERPro(Fin.Read_Num) == 0)
												{
													DeleteDisSum(1,0);
//													JQ8400_VoliceNum(0,1);	
													ADD_DEL_FUNCTION(FINPT,Fin.Read_Num);
													OLED_Refresh_Gram();
													Fin.Value[Fin.Read_Num] = 0;
													save_flash(FINPT,Fin.Read_Num);
													flash_save(31, Fin.Man_Num);										
													delay_ms(100);	
												}
												if(Fin.Man_Num == 0)
												{
//													Voice_1A_WT588S(0x0091);//π‹¿Ì‘±÷∏Œ∆ø’
													delay_ms(1000);
												}
											}
											menu_now = TimingDelay;
										}
								 }
							 }
						}
				 }
				 else if(page_third == 2)//π‹¿Ì…æ≥˝√‹¬Î
				 {
				   JQ8400_SendCmd(7,8,1,1); //«Î ‰»Î√‹¬Î
					 ReSet_Flag(3);
				 }
				 else if(page_third == 3)//”√ªß…æ≥˝÷∏Œ∆
				 {
				   if(Fin.Users_Num == 0)
					 {
						  //”√ªßø’
						  JQ8400_SendCmd(6,3,1,1);
						  delay_ms(1000);
							OLED_Clear();           
						  ReSet_Flag(3);
							pagesys_flag = 0;
							voice_flag = 0;	
					 }
					 else
					 {
							if(voice_flag == 0)
							{
								 JQ8400_SendCmd(3,8,1,1);//«Î ‰»Î÷∏Œ∆
								 voice_flag = 1;
							}
							
							if((FigOnFlag == 1) && (Fin.Users_Num > 0)) //ºÏ≤‚µΩ÷∏Œ∆
							{
								delay_ms(50);
								if(FigOnFlag == 1)//ºÏ≤‚µΩ÷∏Œ∆
								{
									if(((Verify_Flag == 1) || (Verify_Flag == 2)) && ((Fin.Users_Num + Fin.Man_Num) <= 1))
									{
										// ”√ªß÷∏Œ∆ø’
										delay_ms(1500);
									}
									else if((Verify_Flag == 4) && ((Fin.Users_Num + Fin.Man_Num) <= 2))
									{
										// ”√ªß÷∏Œ∆ø’
										delay_ms(1500);
									}
									else
									{
										fin_num = AutoIdentifyPro(2);
										if((fin_num == ERR_OK) && (FIN_FLAG == 1))
										{
											if(DeelOnFigAndUSERPro(Fin.Read_Num) == 0)
											{
												DeleteDisSum(1,1);//
												Fin.Value[Fin.Read_Num] = 0;
												save_flash(FINPT,Fin.Read_Num);
												flash_save(33, Fin.Users_Num);	
												
//												JQ8400_VoliceNum(0,1);	
												ADD_DEL_FUNCTION(FINPT,Fin.Read_Num);
												OLED_Refresh_Gram();

												OLED_Clear();           
												ReSet_Flag(3);
												pagesys_flag = 0;
												voice_flag = 0;	
											}
											if(Fin.Users_Num == 0)
											{
												//”√ªß÷∏Œ∆ø’
												JQ8400_SendCmd(6,3,1,1);
												delay_ms(1000);
											}
										}
										else if(iret==ERR_OVERTIME)
										{
											OLED_Clear();           
											ReSet_Flag(3);
											pagesys_flag = 0;
											voice_flag = 0;	
										}
										else
										{
											JQ8400_SendCmd(2,3,1,1);
											OLED_Clear();           
											ReSet_Flag(3);
											pagesys_flag = 0;
											voice_flag = 0;	
										}									
									}
									menu_now = TimingDelay;
									voice_flag = 0;
								}
							}
					 }
				 }
				 else if(page_third == 4)//”√ªß…æ≥˝√‹¬Î
				 {
					 if(page_showflag == 0)
					 {
						 OLED_Clear(); 
					   enterpwdpage(1);
						 OLED_Refresh_Gram();
					 }
					 if(Pwd.Users_Num == 0)
					 {
						  JQ8400_SendCmd(6,3,1,1);
						  delay_ms(1000);
						 
							OLED_Clear();           
							ReSet_Flag(3);
							pagesys_flag = 0;
							voice_flag = 0;	
					 }
					 else 
					 {
							if(voice_flag == 0)
							{
							  JQ8400_SendCmd(7,8,1,1); //«Î ‰»Î√‹¬Î
							  voice_flag = 1;
							}
					 }
				 }
				 else if(page_third == 5)//…æ≥˝ICø®
				 {
					 unsigned char tempIC[] = {6,74,73};//«ÎÀ¢ø®
					 if(page_showflag == 0)
					 {
						  JQ8400_SendCmd(3,7,1,1);
							page_showflag = 1;
						  OLED_Clear(); 
						  Display20china(menuxone+36,menuyone,(char *)tempIC,1,3,20);//«ÎÀ¢ø®
						  OLED_Refresh_Gram(); 
						  delay_ms(1000);
					 }
					  
					 if(IC.Users_Num == 0)
					 {
							JQ8400_SendCmd(6,7,1,1); //ICø’
						  delay_ms(1200);
							OLED_Clear();           
							ReSet_Flag(3);
							pagesys_flag = 0;
							voice_flag = 0;	
						  page_ic = 1;
					 }  
					 buf = 1;
					 menu_now = TimingDelay;
					 while(buf ==1 && page_ic == 0)
					 {
							buf = Recong_ID();
					 }
					 if(buf == 0)
					 {
							if(((Verify_Flag == 2) || (Verify_Flag == 3)) && (IC.Users_Num <= 1))
							{
								JQ8400_SendCmd(6,7,1,1); //ICø’
								delay_ms(800);
								OLED_Clear();           
								ReSet_Flag(3);
								pagesys_flag = 0;
								voice_flag = 0;
							}
							else
							{
								DeleteDisSum(3,1);
								JQ8400_SendCmd(1,9,1,1);
								delay_ms(800);							
								IC.Value[IC.Read_Num] = 0;
								save_flash(ICCARD,IC.Read_Num);
								IC.Users_Num--;
								flash_save(35, IC.Users_Num);	
								OLED_Clear();           
								ReSet_Flag(3);
								pagesys_flag = 0;
								voice_flag = 0;
							}	
							voice_flag = 0;
							menu_now = TimingDelay;		
					 }
					 else if(buf == 2)
					 {
							JQ8400_SendCmd(2,3,1,1);    //”√ªß≤ª¥Ê‘⁄
						  delay_ms(1200);
						  voice_flag = 0;
						  menu_now = TimingDelay;
							OLED_Clear();           
							ReSet_Flag(3);
							pagesys_flag = 0;
							voice_flag = 0;
					 }
					 else
					 {
						 menu_now = TimingDelay;
						 OLED_Clear();  
					   ReSet_Flag(3);
					   pagesys_flag = 0;
						 voice_flag = 0;
					 }
				 }
				 else if(page_third == 6)//“£øÿπÿ
				 {
					  Remote_flag = 1;
						JQ8400_SendCmd(1,9,1,1);
						delay_ms(1000);
						ReSet_Flag(3);
				 }
				 else if(page_third == 7)//∂œø™wifi
				 {
				 
				 }
				 else if(page_third == 8)//…Ë÷√ ±º‰
				 {
					
				 }
				 else if(page_third == 9)//÷∏Œ∆+√‹¬Î
				 {			 
				   if((Fin.Users_Num + Fin.Man_Num) == 0)
					 {
							open_way_flag = FINPT;
					 }
					 else if((Pwd.Users_Num + Pwd.Man_Num) == 0)
					 {
							open_way_flag = PASSWORD;
					 }
					 if(open_way_flag == 0)
					 {
							Verify_Flag = Page3 - '1';
						  JQ8400_SendCmd(1,9,1,1);
						  delay_ms(1000);
					 }
					 else 
					 {
							Verify_Flag = 0;
					 }
					 flash_save(48,Verify_Flag);
					 menu_now = TimingDelay;
				 	 OLED_Clear();           
					 ReSet_Flag(3);
					 pagesys_flag = 0;
					 voice_flag = 0;	
				 }
				 else if(page_third == 10)//≥£ø™√≈ πÿ
				 {
					 if(Hold_Open_Door == OFF)
					 {
						 JQ8400_SendCmd(1,9,1,1);
						 delay_ms(1000);
					 }
					 Hold_Open_Door = OFF;
//					 Open_Door();
						OLED_Clear();           
						ReSet_Flag(3);
						pagesys_flag = 0;
						voice_flag = 0;	
				 }
				 else if(page_third == 11)//“Ù¡øµ˜Ω⁄  µÕ
				 {
						Volume_Flag=1;
						JQ8400_SetVolume(1);
						if(Volume_Flag == 1)
						{
							JQ8400_SendCmd(1,9,1,1);
							delay_ms(1000);
						}
						OLED_Clear();           
						ReSet_Flag(3);
						pagesys_flag = 0;
						voice_flag = 0;	
				 }
				 else if(page_third == 12)//”Ô—‘  ”¢”Ô
				 {
						Language = English;
						Volume_Flag=2;
						OLED_Clear();           
						ReSet_Flag(3);
						pagesys_flag = 0;
						voice_flag = 0;	
				 }
				 else if(page_third == 13) //ª÷∏¥≥ˆ≥ß πÿ
				 {
				 
				 }
			}
			else if(Page3 == '3')
			{
				 if(page_third == 3)//”√ªß«Âø’÷∏Œ∆
				 {
					  if(page_showflag == 0)
						{
						  JQ8400_SendCmd(3,5,1,1);
							page_showflag = 1;
						  OLED_Clear(); 
						  yesorno();
						  OLED_Refresh_Gram(); 	
						}
						empty_sta=Empty_Switch(DELUSER);
						if(empty_sta == 0)
						{
								JQ8400_SendCmd(1,9,1,1);  //≤Ÿ◊˜≥…π¶
							  delay_ms(1000);
							  EmptyUserFig(); 
								Clear_Data(FINPT);//
								ClearSum(1,1);
								OLED_Clear();           
							  ReSet_Flag(3);
								pagesys_flag = 0;
								voice_flag = 0;	
								empty_sta = 0xff;
						}
						 else
						 {
								OLED_Clear();           
								ReSet_Flag(3);
								pagesys_flag = 0;
								voice_flag = 0;	
						 }
				 }
				 else if(page_third == 4)//”√ªß«Âø’√‹¬Î
				 {
						if(page_showflag == 0)
						{
						  JQ8400_SendCmd(3,5,1,1);
							page_showflag = 1;
						  OLED_Clear(); 
						  yesorno();
						  OLED_Refresh_Gram(); 	
						}
				   empty_sta=Empty_Switch(DELUSER);
					 if(empty_sta == 0)
					 {
						 	Clear_Data(PASSWORD);//
							ClearSum(2,1);		
							JQ8400_SendCmd(1,9,1,1);  //≤Ÿ◊˜≥…π¶	
							delay_ms(1000);
							OLED_Clear();           
					  	ReSet_Flag(3);
							pagesys_flag = 0;
							voice_flag = 0;	
						  empty_sta = 0xff;
					 }
					 else
					 {
							OLED_Clear();           
							ReSet_Flag(3);
							pagesys_flag = 0;
							voice_flag = 0;	
					 }
				 }
				 else if(page_third == 5)//«Âø’ICø®
				 {
					 if(page_showflag == 0)
					 {
						  JQ8400_SendCmd(3,5,1,1);
							page_showflag = 1;
						  OLED_Clear(); 
						  yesorno();
						  OLED_Refresh_Gram(); 			
					 }
					 empty_sta=Empty_Switch(DELUSER);
					 if(empty_sta == 0)
					 {
							Clear_Data(ICCARD);//
							ClearSum(3,1);		
							JQ8400_SendCmd(1,9,1,1);  //≤Ÿ◊˜≥…π¶	
							delay_ms(800);
							OLED_Clear();           
							ReSet_Flag(3);
							pagesys_flag = 0;
							voice_flag = 0;	
						  empty_sta = 0xff;
					 }
					 else
					 {
						  JQ8400_SendCmd(2,0,1,1);  //≤Ÿ◊˜ ß∞‹
							OLED_Clear();           
							ReSet_Flag(3);
							voice_flag = 0;	
						  page_showflag = 0;
					 }
				 }
				 else if(page_third == 9)//÷∏Œ∆+ICø®
				 {
					 if((Fin.Users_Num + Fin.Man_Num) == 0)
					 {
							open_way_flag = FINPT;
					 }
					 else if(IC.Users_Num == 0)
					 {
							open_way_flag = ICCARD;
					 }
					 
					 if(open_way_flag == 0)
					 {
							Verify_Flag = Page3 - '1';
						  JQ8400_SendCmd(1,9,1,1);
						  delay_ms(1000);
					 }
					 else 
					 {
							Verify_Flag = 0;
					 }
					 flash_save(48,Verify_Flag);
					 menu_now = TimingDelay;
					 OLED_Clear();           
					 ReSet_Flag(3);
					 pagesys_flag = 0;
					 voice_flag = 0;	
				 }
				 else if(page_third == 11)//“Ù¡øµ˜Ω⁄  æ≤
				 {
						Volume_Flag=3;
						JQ8400_SetVolume(3);
					  if(Volume_Flag == 3)
						{
							JQ8400_SendCmd(1,9,1,1);
							delay_ms(1000);
						}
						OLED_Clear();           
					  ReSet_Flag(3);
						pagesys_flag = 0;
						voice_flag = 0;	
				 }
			}
			
			if(Key_Value == '*')
			{
				JQ8400_SendCmd(2,9,1,1);
				delay_ms(300);
				OLED_Clear();
				menu_now = TimingDelay;
				ReSet_Flag(3);
				pagesys_flag = 0;
				voice_flag = 0;
				return 1;
			}
	}
	
	if((Key_Value >= '0' && Key_Value <= '9') || Key_Value == '#' || Key_Value == '*')
	{
			menu_now = TimingDelay;
	}
	else
	{
		if(TimingDelay - menu_now > TOUCH_Timeout * 2)
		{
			Mode = 3;
			menu_now = TimingDelay;
			Pwd_iret = 0XFF;
			value_nums = 0;
			pwd_timeout = TOUCH_Timeout;
			OLED_Clear();
			return 0;
		}
	}
	Key_Value = 0;      
	return 1 ;
}

void ReSet_Flag(u8 page)
{
	if(page == 0)
	{
		voice_flag = 0;
		page_showflag = 0;
		show_flag = 0;
	}
	else if(page == 1)
	{
		Page0 = '1';
		Page1 = 0;
		Page0_0 = 0;
		Page1_1 = 0;
		page_flag = 0;
		page_system = 0;
		pagesys_flag = 0;
		voice_flag = 0;
		page_ic = 0;
	}
	else if(page == 2)
	{
		page_system = 0;
		Page0 = 0;
		Page1 = page_test;
		Page0_0 = 0;
		Page1_1 = 0;
		Page2 = 0;
		Page3 = 0;
		pagesys_flag = 0;
		voice_flag = 0;
		page_ic = 0;
	}
	else if(page == 3)
	{
		page_system = 0;
		Page0 = 0;
		Page1 = 0;
		Page0_0 = 0;
		Page1_1 = 0;
		Page2 =  page_test2;
		Page3 = 0;
		pagesys_flag = 0;
		voice_flag = 3;	
		page_showflag = 0;
		page_ic = 0;
	}
	else if(page == 4)
	{
		Page1 = 0;
		Page2 = 0;
		Page3 = 0;
		Page0_0 = 0;
		Page1_1 = 0;
		Page2_2 = 0;
		Page3_3 = 0;
		pagesys_flag = 0;
		voice_flag = 3;	
		page_showflag = 0;
		page_ic = 0;
		page_flag = 0;
		page_system = 0;
	}
}

unsigned char RESET_Switch(void)
{
	u8 Key_Value = 0xff;
	OLED_Clear();
	RESET_Switch_show();
	OLED_Refresh_Gram();
	JQ8400_SendCmd(3,5,1,1);
	delay_ms(1200);
  TimingDelay = 0;
	while(TimingDelay < TOUCH_Timeout)
	{
		Key_Value = KEY_Scan3x4();
		if(Key_Value == '*')
		{
			JQ8400_SendCmd(2,9,1,1);
			delay_ms(300);
			
			OLED_Clear();
		  return 1;
		}
		else if(Key_Value =='#')
		{
			OLED_Clear();
			return 0;
		}
	}
	return 1;
}

u8 SetPage(u8 data,u8 data2)
{  
  if(data == 1)
	{
		if(data2 == '1')return 1;
		else if(data2 == '2')return 2;
	}
	else if(data == 2)
	{
		if(data2 == '1')return 3;
		else if(data2 == '2')return 4;
		else if(data2 == '3')return 5;
	}
	else if(data == 3)
	{
		if(data2 == '1')return 6;
		else if(data2 == '2')return 7;
	}
	else if(data == 4)
	{
	  if(data2 == '1')return 8;
		else if(data2 == '2')return 9;
		else if(data2 == '3')return 10;
		else if(data2 == '4')return 11;
		else if(data2 == '5')return 12;
		else if(data2 == '6')return 13;
	}
}
