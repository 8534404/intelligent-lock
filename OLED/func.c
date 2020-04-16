#include "include.h"

extern unsigned char Language;

u8 Empty_Switch(u8 type)
{
	unsigned char Key_Value = 0xff;
	int  now;
	now=TimingDelay;
	while(TimingDelay-now<TOUCH_Timeout/2)
	{
		Key_Value=KEY_Scan3x4();
		if(Key_Value=='*')
		{
			return 1;
		}	
		else if(Key_Value=='#')
		{	
			return 0;
		}	
		if(Key_Value == '#')
		{
		  JQ8400_SendCmd(2,9,1,1);
			delay_ms(300);
			now = TimingDelay;
			if(type == RESETCMD)
			{

			}
			else if(type == DELUSER)
			{
			}
		}
	}
	return 2;
}
////////////////////////////////////////////////////////////	
//		
//	主菜单  2020.1.3
//
///////////////////////////////////////////////////////////

//菜单第一页
void menu_page1_show(void)
{
	unsigned char tempadminset[] = {11,12,13,24,25};//管理员设置
	unsigned char tempuser[] = {34,35,24,25};//用户设置
	unsigned char tempremote[] = {95,96,34,35,24,25}; //远程用户设置
	
	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempadminset,1,5,18);//管理员设置
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempuser,1,4,18);//用户设置
		
		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		Display20china(menuxone+20,menuythree,(char *)tempremote,1,6,18);//远程用户设置
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Admin setting",1,8);
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"User setting",1,8);
		
		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		DisplayStr(menuxone+16,menuythree,(char *)"Remote setting",1,8);
	}
}

//菜单第二页
void menu_page2_show(void)
{
	unsigned char tempsysset[] = {26,27,24,25};//系统设置
	unsigned char temprecord[] = {20,21,22,23};//记录查询

	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"4.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempsysset,1,4,20);//系统设置
		
		DisplayStr(menuxone,menuytwo,(char *)"5.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)temprecord,1,4,20);//记录查询
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"4.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"System setting",1,8);
		
		DisplayStr(menuxone,menuytwo,(char *)"5.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"Record query",1,8);
	}
}

////////////////////////////////////////////////////////////	
//		
//	次级菜单 2020.1.3
//
///////////////////////////////////////////////////////////

//管理员设置界面
void ManSet(void)
{
	unsigned char tempfirt[] = {11,12,16,17};//
	unsigned char tempseconde[] = {11,12,9,10};//

	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt,1,4,20);//管理指纹
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,4,20);//管理密码
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Admin FP",1,8);
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"Admin PWD",1,8);
	}
}

//用户设置界面
void UserSet(void)
{
	unsigned char tempfirt[] = {16,17,24,25};//
	unsigned char tempseconde[] = {9,10,24,25};//
	unsigned char tempthird[] = {91,73,24,25};//
	
	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt,1,4,20);//指纹设置
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,4,20);//密码设置
		
		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		Display20china(menuxone+20,menuythree,(char *)tempthird,1,4,20);//IC卡设置
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"FP setting",1,8);
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"PWD setting",1,8);
		
		DisplayStr(menuxone,menuythree,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuythree,(char *)"IC setting",1,8);
	}
}

//远程用户设置
void Remoteset(void)
{
	unsigned char tempfirt[] = {97,98,34,35,24,25};//
	unsigned char tempseconde[] = {92,93,24,25};//

	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+16,menuyone,(char *)tempfirt,1,6,18);
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+16,menuytwo,(char *)tempseconde,1,4,18);
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Remote setting",1,8);
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"WIFI setting",1,8);
	}
	
}

//系统设置第一页
void SYSSetONE(void)
{
	unsigned char tempfirt[] = {24,25,46,47};//
	unsigned char tempseconde[] = {59,29,87,88};//
	unsigned char tempthird[] = {89,59,29,85};//

	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt,1,4,20);//设置时间
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,4,20);//开门方式
		
		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		Display20china(menuxone+20,menuythree,(char *)tempthird,1,4,20);//常开门
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Time setting",1,8);
	
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"Opening method",1,8);
		
		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		DisplayStr(menuxone+16,menuythree,(char *)"Normal open",1,8);
	}
}

//系统设置第二页
void SYSSettwo(void)
{
	unsigned char tempfirt[] = {67,99,100,101};//
	unsigned char tempseconde[] = {66,70,71,72};//
	unsigned char tempthird[] = {60,61,62,63};//
	
	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"4.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt,1,4,20);//音量调节
		
		DisplayStr(menuxone,menuytwo,(char *)"5.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,4,20);//语言选择
		
		DisplayStr(menuxone,menuythree,(char *)"6.",1,8);
		Display20china(menuxone+20,menuythree,(char *)tempthird,1,4,20);//防撬设置
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"4.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Volume adjust",1,7);
		
		DisplayStr(menuxone,menuytwo,(char *)"5.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"Language select",1,7);
		
		DisplayStr(menuxone,menuythree,(char *)"6.",1,8);
		DisplayStr(menuxone+16,menuythree,(char *)"Factory reset",1,7);
	}
}


////////////////////////////////////////////////////////////	
//		
//	次次级菜单 2020.1.3
//
///////////////////////////////////////////////////////////

//指纹设置   modein = 1为普通用户
void figsetpage(unsigned char modein)
{
	unsigned char tempfirt[] = {36,37,16,17};//
	unsigned char tempseconde[] = {38,39,16,17};//
	unsigned char tempthird[] = {40,41,16,17};//
	
	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt,1,4,20);//添加指纹
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,4,20);//删除指纹
		
		if(modein == 1)
		{
			DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
			Display20china(menuxone+20,menuythree,(char *)tempthird,1,4,20);//清空指纹
		}
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Add FP",1,8);
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"Del FP",1,8);
		
		if(modein == 1)
		{
			DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
			DisplayStr(menuxone+16,menuythree,(char *)"Empty FP",1,8);
		}
	}
}

//密码界面  modein = 1为普通用户
void pwdsetpage(unsigned char moedein)
{
	unsigned char tempfirt[] = {36,37,9,10};//
	unsigned char tempseconde[] = {38,39,9,10};//
	unsigned char tempthird[] = {40,41,9,10};//

	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt,1,4,20);//添加密码

		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,4,20);//删除密码

		if(moedein == 1)
		{
			DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
			Display20china(menuxone+20,menuythree,(char *)tempthird,1,4,20);//清空密码
		}
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Add password",1,8);
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"Del password",1,8);
		
		if(moedein == 1)
		{
			DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
			DisplayStr(menuxone+16,menuythree,(char *)"Empty password",1,8);
		}
	}
}

//IC卡界面
void ICetpage(void)
{
	
	unsigned char tempseconde[] = {36,37,91,73};//
	unsigned char tempthird[] = {38,39,91,73};//
	unsigned char tempfirt1[] = {40,41,91,73};//	
	
	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempseconde,1,4,20);//添加IC卡

		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempthird,1,4,20);//删除IC卡


		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		Display20china(menuxone+20,menuythree,(char *)tempfirt1,1,4,20);//清空IC卡
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Add IC card",1,8);
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"Del IC card",1,8);
		
		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		DisplayStr(menuxone+16,menuythree,(char *)"Empty IC card",1,8);
	}
}

//遥控界面
void Remotepage(void)
{
	unsigned char tempfirt1[] = {36,37,97,98};//
	unsigned char tempseconde[] = {38,39,97,98};//
	unsigned char tempthird[] = {40,41,97,98};//	
	
	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt1,1,4,20);//添加遥控

		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,4,20);//删除遥控


		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		Display20china(menuxone+20,menuythree,(char *)tempthird,1,4,20);//清空遥控
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Add remote",1,8);
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"Del remote",1,8);
		
		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		DisplayStr(menuxone+16,menuythree,(char *)"Empty remote",1,8);
	}
}

//连接wifi
void wifisetPage(void)
{
	unsigned char tempfirt[] = {56,57,92,93};//
	unsigned char tempseconde[] = {58,59,92,93};//
	
	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt,1,4,20);//连接
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,4,20);//断开
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Connet WIFI",1,8);
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"Disconnet WIFI",1,8);
	}
}

//开门方式 第一页
void OpenWayModeOne(void)
{
	unsigned char tempfirt[] = {50,51,52};//
	unsigned char tempseconde[] = {16,17,94,9,10};//
	unsigned char tempthird[] = {16,17,94,91};//
	
	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt,1,3,20);//单验证
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,5,20);//指纹 + 密码
		
		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		Display20china(menuxone+20,menuythree,(char *)tempthird,1,4,20);//指纹 + IC
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Single verify",1,8);
	
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"FP+PWD",1,8);
		
		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		DisplayStr(menuxone+16,menuythree,(char *)"FP+IC",1,8);	
	}
}


//开 或 关
void openorclose(void)
{
	unsigned char tempfirt[] = {59};//
	unsigned char tempseconde[] = {83};//

	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt,1,1,20);//开
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,1,20);//关
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Open",1,8);//开
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"Close",1,8);//关
	}
}

//音量调节
void Volume_adjust(void)
{
	unsigned char tempfirt[] = {104,67};//
	unsigned char tempseconde[] = {105,67};//
	unsigned char tempthird[] = {106,67};//
	
	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt,1,2,20);//高
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,2,20);//低
		
		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		Display20china(menuxone+20,menuythree,(char *)tempthird,1,2,20);//静
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"High pitch",1,8);//高
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"Low pitch",1,8);//低
		
		DisplayStr(menuxone,menuythree,(char *)"3.",1,8);
		DisplayStr(menuxone+16,menuythree,(char *)"Mute",1,8);//静
	}
}

//中文 or 英文
void languagePage(void)
{
	unsigned char tempfirt[] = {53,55};//
	unsigned char tempseconde[] = {54,55};//

	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt,1,2,20);//中文
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,2,20);//英文
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Chinese",1,8);
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"English",1,8);
	}
}



//////////////////////////////////////////////////////////////
//																													//
//                    菜单完结                              //
//                                                          //
//////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////	
//		
//	请输入xxxxxx 2020.1.3
//
///////////////////////////////////////////////////////////

//请输入管理员的 密码或指纹
void adminenterpage(void)
{
	unsigned char tempfirt[] = {6,7,8,11,12,13,18};//
	unsigned char tempseconde[] = {9,10,19,16,17};//
	
	if(Language == Chinese)
	{
		Displaychina(menuxone+8,0,(char *)tempfirt,1,7);//请输入管理员的 
		Displaychina(menuxone+24,16,(char *)tempseconde,1,5);//密码或指纹
	}
	else if(Language == English)
	{
		DisplayStr(menuxone+8,0,(char *)"Please enter ",1,8);
		DisplayStr(menuxone,20,(char *)"admin FP or PWD ",1,8);
	}
}

//请输密码     mode = 1密码   mode = 0指纹
void enterpwdpage(u8 mode)
{
	unsigned char tempfirt[] = {6,7,8,9,10};//
  unsigned char tempsec[] = {6,7,8,16,17};//
	
	if(Language == Chinese)
	{
		if(mode == 1)
			Display20china(menuxone+16,menuyone,(char *)tempfirt,1,5,20);//请输入密码
		else if(mode == 0)
			Display20china(menuxone+16,menuyone,(char *)tempsec,1,5,20);//请输入指纹
	}
	else if(Language == English)
	{
		if(mode == 1)
			DisplayStr(menuxone+10,5,(char *)"Enter password ",1,8);
		else if (mode == 0)
			DisplayStr(menuxone+10,5,(char *)"Enter fingerprint ",1,8);
	}
}

//请输入用户编号   
void Enter_User_Num(u8 mode,u16 nums,u8 type,u8 motion)
{
	unsigned char tempadminnum[] = {6,7,8,11,12,13,14,15};
	unsigned char tempnumnum[] = {6,7,8,34,35,14,15};
	unsigned char temppwd[] = {19,9,10};//或密码
	unsigned char tempfin[] = {19,16,17};///指纹
	unsigned char tempic[] = {19,74,91};///或刷IC
	
	if(Language == Chinese)
	{
		if(mode==0)//请输入管理员编号
		{
			Displaychina(menuxone,menuyone,(char *)tempadminnum,1,8);//
		}
		else if(mode == 1)//请输入用户编号
		{
			Displaychina(menuxone+8,menuyone,(char *)tempnumnum,1,7);//
		}
		
		if(motion == 0)
		{
			if(type == 1)//或指纹
			{
				Displaychina(menuxone,menuytwo,(char *)tempfin,1,3);//
			}
			else if(type == 2)//或密码
			{
				Displaychina(menuxone,menuytwo,(char *)temppwd,1,3);//
			}
			else if(type == 3)//或刷IC
			{
				Displaychina(menuxone,menuytwo,(char *)tempic,1,3);//
			//	DisplayStr(menuxone+32,menuytwo,(char *)"IC",1);//
			}
		}
		DisplayStr(menuxone+48,menuytwo,(char *)":",1,8);//
		if(nums<1000)
		DisplayInt(nums,menuxone+64,menuytwo,1,3);
	}
	else if(Language == English)
	{
		if(mode==0)//请输入管理员编号 
		{
			DisplayStr(menuxone+8,8,(char *)"Enter",1,8);
			DisplayStr(menuxone,28,(char *)"admin ID",1,8);
		}
		else if(mode == 1)//请输入用户编号
		{
			DisplayStr(menuxone+8,8,(char *)"Enter",1,8);
			DisplayStr(menuxone+8,28,(char *)"user ID",1,8);
		}
		
		if(motion == 0)
		{
			if(type == 1)//或指纹
			{
				DisplayStr(menuxone+56,8,(char *)"FP or",1,8);
			}
			else if(type == 2)//或密码
			{
				DisplayStr(menuxone+56,8,(char *)"PWD or",1,8);
			}
			else if(type == 3)//或刷IC
			{
				DisplayStr(menuxone+56,8,(char *)"IC or",1,8);//
			}
		}
		DisplayStr(menuxone+64,28,(char *)":",1,8);//
		if(nums<1000)
		DisplayInt(nums,menuxone+80,28,1,3);
	}
}


//用户信息
void information(u8 pwdin,u8 finin,u8 icin)
{
	unsigned char tempfirt[] = {9,10,34,35};//
	unsigned char tempseconde[] = {16,17,34,35};//
	unsigned char tempthird[] = {34,35};//
	
	if(Language == Chinese)
	{
		Displaychina(menuxone,menuyone,(char *)tempfirt,1,4);//密码用户
		DisplayStr(menuxone+64,menuyone,(char *)":   : 28",1,8);
		DisplayInt(pwdin,menuxone+80,menuyone,1,2);
		
		Displaychina(menuxone,menuytwo,(char *)tempseconde,1,4);//指纹用户
		DisplayStr(menuxone+64,menuytwo,(char *)":   : 95",1,8);
		DisplayInt(finin,menuxone+80,menuytwo,1,2);
		
		DisplayStr(menuxone,menuythree,(char *)"IC    :     :200",1,8);
		Displaychina(menuxone+16,menuythree,(char *)tempthird,1,2);//用户
		DisplayInt(icin,menuxone+72,menuythree,1,3);
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"PWD user",1,8);//密码用户
		DisplayStr(menuxone+64,menuyone,(char *)":   : 28",1,8);
		DisplayInt(pwdin,menuxone+80,menuyone,1,2);
		
		DisplayStr(menuxone,menuytwo,(char *)"FP user",1,8);//指纹用户
		DisplayStr(menuxone+64,menuytwo,(char *)":   : 95",1,8);
		DisplayInt(finin,menuxone+80,menuytwo,1,2);
		
		DisplayStr(menuxone,menuythree,(char *)"IC card :   :100",1,8);
		//Displaychina(menuxone+16,menuythree,(char *)tempthird,1,2);//用户
		DisplayInt(icin,menuxone+80,menuythree,1,2);
	}
}

//已删除
void ADD_DEL_FUNCTION(u8 type,u16 num)
{
	unsigned char tempfirt[] = {77,38,39};//
	unsigned char temptype = 0;
	OLED_Clear();
	
	if(Language == Chinese)
	{
		Displaychina(menuxone+24,menuytwo,(char *)tempfirt,1,3);//已删除
		DisplayStr(menuxone+72,menuytwo,(char *)":",1,8);
		if(type==FINPT)
		{
			temptype = 'F';
		}
		else if(type==PASSWORD)
		{
			temptype = 'P';
		}
		else if(type==ICCARD)
		{
			temptype = 'I';
		}
		OLED_ShowChar(menuxone+80,menuytwo,temptype,1);
		DisplayInt(num,menuxone+88,menuytwo,1,3);
	}
	else if(Language == English)
	{
		DisplayStr(menuxone+35,15,(char *)"Delete",1,8);
		if(type==FINPT)
		{
			DisplayStr(menuxone+48,menuytwo+15,(char *)"F",1,8);//F
		}
		else if(type==PASSWORD)
		{
			DisplayStr(menuxone+48,menuytwo+15,(char *)"P",1,8);//P
		}
		else if(type==ICCARD)
		{
			DisplayStr(menuxone+48,menuytwo+15,(char *)"I",1,8);//I
		}
		DisplayInt(num,menuxone+56,menuytwo+15,1,3);
	}
	OLED_Refresh_Gram();
	delay_ms(800);
	OLED_Clear();
}


//编号显示
void Num_show(u8 mode,u8 type,u16 num)
{
	unsigned char tempIC[] = {6,74,73};//请刷卡
	unsigned char temppwd[] = {6,7,8,9,10};//密码
	unsigned char tempfin[] = {6,7,8,16,17};///指纹
	unsigned char tempadminnum[] = {11,12,13,14,15};//管理员编号
	unsigned char tempusenum[] = {34,35,14,15};//用户编号
	
	if(type==PASSWORD)
	{
		if(Language == Chinese)
		{
			Display20china(menuxone+16,menuyone,(char *)temppwd,1,5,20);//密码
			DisplayStr(menuxone+80,menuytwo,(char *)":P",1,8);//F
		}
		else if(Language == English)
		{
			DisplayStr(menuxone+15,10,(char *)"Enter password",1,8);//F
		}
	}
	else if(type==FINPT)
	{
		if(Language == Chinese)
		{
			Display20china(menuxone+16,menuyone,(char *)tempfin,1,5,20);//指纹
			DisplayStr(menuxone+80,menuytwo,(char *)":F",1,8);//F
		}
		else if(Language == English)
		{
			DisplayStr(menuxone+15,10,(char *)"Enter Finger",1,8);//F
		}
	}
	else if(type==ICCARD)
	{
		if(Language == Chinese)
		{
			Display20china(menuxone+36,menuyone,(char *)tempIC,1,3,20);//请刷卡
			DisplayStr(menuxone+80,menuytwo,(char *)":I",1,8);//F
		}
		else if(Language == English)
		{
			DisplayStr(menuxone+15,10,(char *)"Swipe IC card",1,8);//F
		}
	}
	
	if(mode==Management)
	{
		if(Language ==Chinese)
		{
			Displaychina(menuxone,menuytwo,(char *)tempadminnum,1,5);//管理员编号
		}
		else if(Language == English)
		{
			DisplayStr(menuxone+15,26,(char *)"Admin ID:",1,8);//F
		}
	}
	else if(mode==User)
	{
		if(Language ==Chinese)
		{
			Display20china(menuxone,menuytwo,(char *)tempusenum,1,4,20);//用户编号
		}
		else if(Language == English)
		{
			DisplayStr(menuxone+15,26,(char *)"User ID:",1,8);//F
		}
	}
	
	DisplayInt(num,menuxone+96,menuytwo,1,3);
}

//* 取消   # 确认
void yesorno(void)
{
	unsigned char tempno[] = {42,43};
  unsigned char tempyes[] = {44,45};

	if(Language == Chinese)
	{
		DisplayStr(0,40,(char *)"*",1,8);//
		Display20china(16,40,(char *)tempno,1,2,20);//
		
		DisplayStr(76,40,(char *)"#",1,8);//
		Display20china(92,40,(char *)tempyes,1,2,20);//
	}
	else if(Language == English)
	{
		DisplayStr(0,40,(char *)"*",1,7);//
		DisplayStr(8,40,(char *)"cancel",1,7);//
		
		DisplayStr(68,40,(char *)"#",1,7);//
		DisplayStr(76,40,(char *)"confirm",1,7);//
	}
}


void Switch_show(unsigned char type) //清空 遥控没弄
{
	unsigned char tempfin[] = {40,41,16,17};
	unsigned char tempfpwd[] = {40,41,9,10};
	unsigned char tempic[] = {40,41,91,73};
	unsigned char tempremote[] = {40,41,97,98};
	
	if(type == 1)
	{
		if(Language == Chinese)
			Display20china(26,10,(char *)tempfin,1,4,20);//
		else if(Language == English)
			DisplayStr(35,10,(char *)"Empty FP",1,8);//
	}
	else if(type == 2)
	{
		if(Language == Chinese)
			Display20china(26,10,(char *)tempfpwd,1,4,20);//
		else if(Language == English)
			DisplayStr(31,10,(char *)"Empty PWD",1,8);//
	}
	else if(type == 3)
	{
		if(Language == Chinese)
			Display20china(26,10,(char *)tempic,1,4,20);//
		else if(Language == English)
			DisplayStr(31,10,(char *)"Empty IC",1,8);//
	}
	else if(type ==4)
	{
		if(Language == Chinese)
			Display20china(26,10,(char *)tempremote,1,4,20);//
		else if(Language == English)
			DisplayStr(16,10,(char *)"Empty Remote",1,8);//
	}
	yesorno();
}

//已配置WIFI
//是否重新配置
//*取消 # 确认
void Config_WIFI(void) 
{
	unsigned char tempfin[] = {79,80,81,82,78,25};
	
	if(Language == Chinese)
		Displaychina(16,10,(char *)tempfin,1,6);//
	else if(Language == English)
		DisplayStr(7,10,(char *)"Reconfigure WIFI",1,7);//
	yesorno();
}
void RESET_Switch_show(void)
{
	unsigned char tempfin[] = {60,61,62,63,24,25};
	
	if(Language == Chinese)
		Displaychina(16,10,(char *)tempfin,1,6);//
	else if(Language == English)
		DisplayStr(0,10,(char *)"Factory data reset",1,7);//
	yesorno();
}

void Display_timefun(unsigned char smin,unsigned char shour,unsigned char date,unsigned char month,unsigned int year,unsigned char mode)
{
		unsigned short xstartone = 7;
		unsigned short ystartone = 15,ystarttwo = 35;
		unsigned char tempfirt[] = {107,108,109,110};    //欢迎回家
	  unsigned char tempsec[3];
		
		tempsec[0] = 111;
	  tempsec[1] = 112;
		
		switch(calendar.week)
		{
			case 0:
				tempsec[2] = 119;
				break;
			case 1:
				tempsec[2] = 113;
			  break;
			case 2:
				tempsec[2] = 114;
				break;
			case 3:
				tempsec[2] = 115;
				break;
			case 4:
				tempsec[2] = 116;
				break;
			case 5:
				tempsec[2] = 117;
				break;
			case 6:
				tempsec[2] = 118;
				break;
		}
		
		DisplayInt(year%100,xstartone,ystartone,1,2);
		DisplayStr(xstartone+16,ystartone,(char *)"-",1,8);
		DisplayInt(month,xstartone+24,ystartone,1,2);
		DisplayStr(xstartone+40,ystartone,(char *)"-",1,8);
		DisplayInt(date,xstartone+48,ystartone,1,2);

		DisplayInt(shour,xstartone+74,ystartone,1,2);
		DisplayStr(xstartone+90,ystartone,(char *)":",1,8);
		DisplayInt(smin,xstartone+98,ystartone,1,2);
	
		if(mode == 0)
			Displaychina(xstartone+17,ystarttwo,(char *)tempfirt,1,4);//
		else if(mode == 1)
		{
		  Displaychina(xstartone+25,ystarttwo,(char *)tempsec,1,3);//
		}
}


//开门记录 用户编号
 void Model_select(u8 mode,u16 num,u8 type)
{
		unsigned char tempadmin[] = {11,12,34,35};
		unsigned char tempuser[] = {64,65,34,35};
		unsigned char temptemp[] = {75,76,34,35};
		unsigned short xstartone = 7;
		unsigned short ystarttwo = 35;
		
		if(mode == 2)
		{
			if(Language == Chinese)
				Displaychina(xstartone+24,ystarttwo,(char *)temptemp,1,4);//		临时用户
			else if(Language == English)
				DisplayStr(xstartone+24,ystarttwo,(char *)"Temp User",1,7);//
		}
		else
		{
			if(mode == 0)
			{
				if(Language == Chinese)
					Displaychina(xstartone,ystarttwo,(char *)tempadmin,1,4);//	
				else if(Language == English)
					DisplayStr(xstartone+16,ystarttwo,(char *)"Admin",1,8);//
			}
			if(mode == 1)
			{
				if(Language == Chinese)
					Displaychina(xstartone,ystarttwo,(char *)tempuser,1,4);//	
				else if(Language == English)
					DisplayStr(xstartone+24,ystarttwo,(char *)"User",1,8);//
			}
			DisplayStr(xstartone+64,ystarttwo,(char *)":",1,8);
			DisplayStr(xstartone+80,ystarttwo,(char *)&type,1,8);
			DisplayInt(num,xstartone+88,ystarttwo,1,3);
		}
}

//记录查询界面
void Recordquerypage(void)
{
	unsigned char tempfirt[] = {28,29,20,21};//开门记录
	unsigned char tempseconde[] = {30,31,32,33};//注册信息
	
	if(Language == Chinese)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		Display20china(menuxone+20,menuyone,(char *)tempfirt,1,4,20);//开门记录
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		Display20china(menuxone+20,menuytwo,(char *)tempseconde,1,4,20);//注册信息
	}
	else if(Language == English)
	{
		DisplayStr(menuxone,menuyone,(char *)"1.",1,8);
		DisplayStr(menuxone+16,menuyone,(char *)"Door record",1,8);
		
		DisplayStr(menuxone,menuytwo,(char *)"2.",1,8);
		DisplayStr(menuxone+16,menuytwo,(char *)"Message",1,8);
	
	}
}

//欢迎回家
void WelcomeHome(void)
{
	unsigned char tempfirt[] = {107,108,109,110};//开门记录
  if(Language == Chinese)
	{
		Display20china(menuxone+20,menuytwo,(char *)tempfirt,1,4,20);//注册信息
	}
	else if(Language == English)
	{
		DisplayStr(menuxone+16,menuytwo,(char *)"Welcome Home",1,8);
	}
}

//唤醒后页面
void Huanxing(void)
{
	unsigned char tempfirt[] = {6,7,8,34,35,18};//
	unsigned char tempseconde[] = {9,10,19,16,17,19,91,73};//
	
	if(Language == Chinese)
	{
		Displaychina(menuxone+16,0,(char *)tempfirt,1,6);//请输入用户的 
		Displaychina(menuxone,16,(char *)tempseconde,1,8);//密码或指纹或IC卡
	}
	else if(Language == English)
	{
		DisplayStr(menuxone+8,0,(char *)"Please enter ",1,8);
		DisplayStr(menuxone,20,(char *)" FP or PWD or IC card ",1,8);
	}

}
//no more
