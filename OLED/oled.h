#ifndef _OLED_H
#define _OLED_H


//oled的引脚定义
//#define OLED_MODE 0

#define SIZE        16  
#define XLevelL		  0x00
#define XLevelH		  0x10
#define Max_Column	128 //列
#define Max_Row		  64  //行
#define	Brightness	0xFF  //亮度
#define X_WIDTH 	  128  //X_宽度
#define Y_WIDTH 	  64    //Y_宽度


#define OLED_POWERON GPIO_SetBits(GPIOD,GPIO_Pin_2) 
#define OLED_POWEROFF GPIO_ResetBits(GPIOD,GPIO_Pin_2) 

#define OLED_CMD 0
#define OLED_DATA 1
extern const unsigned char china[];
extern const unsigned char ascii[];
extern const unsigned char logo[];

void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_Init(void);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char mode);
void DisplayStr(unsigned char x,unsigned char y,char *chr,unsigned char mode,unsigned int size);
void DisplayInt(unsigned int intdata,unsigned char x,unsigned char y,unsigned char mode,unsigned char digit);
void DisplayOneChina(unsigned char x,unsigned char y,unsigned char pos,unsigned char mode);
void Displaychina(unsigned char x,unsigned char y,char *chr,unsigned char mode,unsigned int len);
void OLED_Refresh_Gram(void);
void OLED_Clear(void)  ;

void Display20china(unsigned char x,unsigned char y,char *chr,unsigned char mode,unsigned int len,unsigned int size);
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot);
void Boot_Animation(void);











#endif













