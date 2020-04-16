#include "include.h"

u8 Mode = 0,FIN_FLAG = 0,comu = 0,ic_switch=OFF,Verify_Flag,Touch_flag = 0,re_flag = 0,respond_flag = 0,
Page0,Page1,Page2,Page3,Page0_0,Page1_1,Page2_2,Page3_3, value_num=0,Error_Flag = 0x00,ic_f = 0xff;
u8 key_value = 0xff,t,show_flag = 0,reset_sw = 0,timer3_open = 0,TimingDelay_flag = 0;
u8 Remote_Error,Remote_status,Remote_flag;
unsigned char Language;
u8 Temp_Pwd[22];
unsigned short nIndex_temp[2];
unsigned short nIndex_num;
unsigned int timingdelay = 0, TimingDelay = 0 , menu_now = 0,lowerpower_now = 0,Low_Timeout = TOUCH_Timeout;

int main(void)
{	
	unsigned char Key_Value = 0;
	u8 i,Temp[22],iret = 0xff;
	delay_init();
	uart_init(115200);
	uart5_init(57600);
	TIM3_Int_Init(99,7199);//10ms
	KEY_Init();
	RTC_Init();
	Rc522_Init();
	Remote_Init();			//红外接收初始化	 
	OLED_Init();
	JQ8400_Init();
	HC_SR501_Init();
	
	Boot_Animation();
	delay_ms(500);
	Read_Status();

	while(1)
	{
		if(Mode == 0)//复位模式
		{
			Register_Admins();
		}
		else if(Mode == 1)//管理员模式
		{
			menu();
			if(Mode == 3)
			{
				voice_flag = 0;
				Pwd.Status = 0;
				Fin.Status = 0;
				lowerpower_now = TimingDelay;							 
				Touch_flag=1;
				Low_Timeout =100;
				delay_ms(300);
				continue;
			}
		}
		else if(Mode == 2)
		{
			TimingDelay_flag = 0;//开定时器
			if(show_flag == 0)
			{
				OLED_Clear();
				Huanxing();
				OLED_Refresh_Gram();
				show_flag = 1;
			}
			Enter_pwd(User);							 		//密码
		  ic_f = Recong_ID_zhuye();         //ID卡
			if(ic_f == 0 && Verify_Flag == 0){
					Mode = 3;
			}
			if(Remote_flag == 0)
			{
				Deal_Remote(Remote_Scan(),1);
			}
			
			if((GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4) != 0 || respond_flag == 1) && Fin.Flag == 0)
			{
				if(Error_Flag == 1)
				{
					JQ8400_SendCmd(1,5,1,1);//系统锁定
					delay_ms(1000);
					Mode = 3;
					show_flag = 0;
				}
				else if(Error_Flag == 0)
				{
					respond_flag = 0;
					value_num = 0;
					value_num = 0;
					read_flash(FINPT);
					iret = AutoIdentifyPro(2); //识别指纹 
					if(iret == ERR_OK)
					{
							menu_now = TimingDelay;
							Fin.Status = 1;																				
					}	
					else if(iret == ERR_SEARCHCHAR)
					{
						Fin.Error++;
						JQ8400_SendCmd(2,3,1,1);
						delay_ms(1000);
					}
					else if(iret==ERR_OVERTIME||Key_Value == '*')
					{  
						 Mode = 3;
						 show_flag = 0;
					}
					else if(iret != NoIdFIG)
					{
						//请重试
						Mode = 3;
					}
					iret = NoIdFIG;
				}
			}
			
			Open_Way(Verify_Flag);
			if(TimingDelay - lowerpower_now > TOUCH_Timeout)
			{
				Mode = 3;	
				show_flag = 0;
				Low_Timeout=TOUCH_Timeout;	
			}	
			if(Mode == 3)
			{
				Touch_flag=0;
				value_num = 0;
				voice_flag = 0;
				respond_flag=0;
				lowerpower_now = TimingDelay;
			 	TimingDelay_flag = 1;
			}			
		}
		else if(Mode == 3)
		{
			TimingDelay_flag = 1;//关定时器
			ReSet_Flag(4);
			if(show_flag == 0)
			{
				OLED_Clear();
				Display_timefun(calendar.min,calendar.hour,calendar.w_date,calendar.w_month,calendar.w_year,1);
				OLED_Refresh_Gram();
				show_flag = 1;
			}
			Display_timefun(calendar.min,calendar.hour,calendar.w_date,calendar.w_month,calendar.w_year,1);
			OLED_Refresh_Gram();
		  Deal_KEY(KEY_Scan());
			Key_Value = KEY_Scan3x4();
			if(Remote_flag == 0)
			{
				Deal_Remote(Remote_Scan(),0);
			}
			if( Key_Value > '0' && (Error_Flag == 0 || Error_Flag == 1))
			{
				Mode = 2;
				Key_Value = 0;
				show_flag = 0;
			}
			else if( Key_Value > '0' && Error_Flag == 2)
			{
				JQ8400_SendCmd(7,2,1,1);//菜单已锁定
				delay_ms(1000);
			}
		}
		else if(Mode == 4)//验证管理员
		{
			if(show_flag == 0)
			{
				OLED_Clear();
				adminenterpage();
				OLED_Refresh_Gram();
				show_flag = 1;
				JQ8400_SendCmd(3,6,1,1);
				delay_ms(1200);
				voice_flag = 0;
			}
			Enter_pwd(Management);
			if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 1)
			{
				iret = AutoIdentifyPro(2); //识别指纹			
				if(iret == ERR_OK && FIN_FLAG==0)//识别到了 并且是管理员
				{
						Mode =	1;//进入管理员模式																				
				}
				else if((iret == ERR_SEARCHCHAR) || (iret == ERR_OK && FIN_FLAG==1) ) //没有该指纹或者指纹为普通用户
				{
					
					Fin.Man_Error++;	
					JQ8400_SendCmd(3,4,1,1);
					delay_ms(1500);
				}
				else 
				{							
					memset(Temp, 0, sizeof(Temp));										 
					value_num = 0;								
					Mode = 3;
					JQ8400_SendCmd(3,4,1,1);
					delay_ms(1500);			
				}
				menu_now = 	TimingDelay	;				
			}
		  if(TimingDelay - menu_now > TOUCH_Timeout * 2)
			{
				memset(Temp, 0, sizeof(Temp));						 
				value_num = 0;
				Mode = 3;
				show_flag = 0;
			}
			if(Mode == 1)
			{
				Error_Flag &= ~(0x01);    //解除系统锁定
				Reset_Error_Flag(User);
			}
		}
		else if(Mode == 5)//菜单已锁定
		{
		  JQ8400_SendCmd(7,2,1,1);//菜单已锁定
		  delay_ms(1000);
			Mode = 3;
			timer3_open = 1;
		}
		else if(Mode == 6)//系统锁定
		{
			JQ8400_SendCmd(1,5,1,1);//系统锁定
			delay_ms(1000);
			Mode = 3;
			timer3_open = 1;
		}
		else if(Mode == 11)
		{
			  //实体按键
		}
		if(Fin.Error + Pwd.Error + IC.Error + Remote_Error > 4 && (((Error_Flag >> 0) & 1) == 0))
		{           
			Error_Flag |= 0x01;
			show_flag = 0;
			Mode = 6;
		}
		if(Pwd.Man_Error + Fin.Man_Error > 4 && (((Error_Flag >> 1) & 1) == 0))
		{            
			Error_Flag |= 0x02;
			show_flag = 0;
			Mode = 5;
		}
	}
}

