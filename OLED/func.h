#ifndef _FUNC_H
#define _FUNC_H
#include "include.h"


#define menuxone   0
#define menuyone   5
#define menuytwo   25
#define menuythree 45


void Volume_adjust(void);
void menu_page1_show(void);
void menu_page2_show(void);
void Recordquerypage(void);
void figsetpage(unsigned char modein);
void pwdsetpage(unsigned char moedein);
void ICetpage(void);
void Remotepage(void);
void information(unsigned char pwdin,unsigned char finin,unsigned char icin);
void ADD_DEL_FUNCTION(unsigned char type,unsigned short num);
void adminenterpage(void);
void enterpwdpage(u8 mode);
void Num_show(unsigned char mode,unsigned char type,unsigned short num);
void Enter_User_Num(unsigned char mode,unsigned short nums,unsigned char type,unsigned char motion);
void yesorno(void);
void Switch_show(unsigned char type);
void ManSet(void);
void UserSet(void);
void SYSSetONE(void);
void SYSSettwo(void);
void OpenWayModeOne(void);
void OpenWayModeTwo(void);
void openorclose(void);
void languagePage(void);
void wifisetPage(void);
void RESET_Switch_show(void);
void Display_timefun(unsigned char smin,unsigned char shour,unsigned char date,unsigned char month,unsigned int year,unsigned char mode);
void Model_select(unsigned char mode,unsigned short num,unsigned char type);
void Remoteset(void);
void DisplayThree(unsigned char disindex,unsigned char typein);
u8 Empty_Switch(u8 type);
void Huanxing(void);
 
 
 
 
 
 
 
 
#endif

