#include "include.h"

#define OLED_CS_L  GPIO_ResetBits(GPIOC,GPIO_Pin_6)
#define OLED_CS_H  GPIO_SetBits(GPIOC,GPIO_Pin_6)

#define OLED_DC_H  GPIO_SetBits(GPIOC,GPIO_Pin_7)
#define OLED_DC_L  GPIO_ResetBits(GPIOC,GPIO_Pin_7)//DC命令	

#define OLED_SCL_L GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define OLED_SCL_H GPIO_SetBits(GPIOC,GPIO_Pin_9)

#define OLED_SDA_L GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define OLED_SDA_H GPIO_SetBits(GPIOC,GPIO_Pin_8)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
unsigned char OLED_GRAM[128][8];	 
		
		
	    	    
//#if OLED_MODE==0
//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_Command(unsigned char dat)
{	
	unsigned char i;	
  
	OLED_DC_L;  	  
	OLED_CS_L;
	for(i=0;i<8;i++)
	{			  
		OLED_SCL_L;
		if(dat&0x80)
			OLED_SDA_H;
		else 
		  OLED_SDA_L;
		dat<<=1;  
		OLED_SCL_H;
		 
	}				 		  
	OLED_CS_H;
	OLED_DC_H;   	 
 // GPIO_SetBits(GPIOB,GPIO_Pin_5);//DC命令	
} 

void OLED_Write_Data(unsigned char dat)
{
      unsigned char i;
      OLED_DC_H; 
      OLED_CS_L;
       for(i=0;i<8;i++)
        {
				  OLED_SCL_L;
					if(dat&0x80)
			     OLED_SDA_H;
		        else 
		      OLED_SDA_L;
		        dat<<=1;  
		        OLED_SCL_H;
				}	
			OLED_DC_H; 
				
      OLED_CS_L;
}


void OLED_Refresh_Gram(void)
{
	unsigned char i,n;	
	
	for(i=0;i<8;i++)  
	{  
		OLED_Command (0xb0+i);    //设置页地址（0~7）
		OLED_Command (0x00);      //设置显示位置—列低地址
		OLED_Command (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_Write_Data(OLED_GRAM[n][i]); 
	}  
	
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_Command(0xb0+y);
	OLED_Command(((x&0xf0)>>4)|0x10);
	OLED_Command((x&0x0f)|0x01); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_Command(0X8D);  //SET DCDC命令
	OLED_Command(0X14);  //DCDC ON
	OLED_Command(0XAF);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_Command(0X8D);  //SET DCDC命令
	OLED_Command(0X10);  //DCDC OFF
	OLED_Command(0XAE);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	unsigned char i,n;		
	
	for(i=0;i<8;i++)  
	{    
		for(n=0;n<128;n++)
		{
			OLED_GRAM[n][i]=0;
		}
	}
	OLED_Refresh_Gram();//更新显示 
}

//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t)
{
	unsigned char pos,bx,temp=0;

	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot)  
{  
	unsigned char x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)
		{ 	
			OLED_DrawPoint(x,y,dot);	
		}
	}													    
	OLED_Refresh_Gram();//更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char mode)
{      			    
	unsigned char *temp;
	unsigned int index;
	unsigned char t,t1,dat;
	unsigned char tempx = x,tempy0 = y,tempy1 = y;
	 
	index = 16*(chr - 32);

	temp = (unsigned char *)&ascii;
	temp += index; 	 
	
    for(t=0;t<16;t++)
    {   
			dat = *temp++;
			for(t1=0;t1<8;t1++)
			{
				if(dat&0x80)
					OLED_DrawPoint(tempx,tempy1,mode);
				else 
					OLED_DrawPoint(tempx,tempy1,!mode);
				dat<<=1;
				tempy1++;
				if(tempy1-tempy0 == 16)
				{
					tempy1 = tempy0;
					tempx++;
					break;
				}
			}  	 
    }          
}

//显示字符串
void DisplayStr(unsigned char x,unsigned char y,char *chr,unsigned char mode,unsigned int size)
{
	unsigned int len = strlen(chr);
	while(len--)
	{
		OLED_ShowChar(x,y,*chr++,mode);
		x += size;
	}
}

//显示整数
void DisplayInt(unsigned int intdata,unsigned char x,unsigned char y,unsigned char mode,unsigned char digit)
{
	if(digit==3)
	{
		OLED_ShowChar(x,y,(intdata/100%10)+48,mode);
		OLED_ShowChar(x + 8,y,(intdata/10%10)+48,mode);
		OLED_ShowChar(x + 16,y,(intdata%10)+48,mode);
	}
	else if(digit==2)
	{
		OLED_ShowChar(x,y,(intdata/10%10)+48,mode);
		OLED_ShowChar(x + 8,y,(intdata%10)+48,mode);
	}
	else if(digit==1)
	{
		OLED_ShowChar(x,y,(intdata%10)+48,mode);
	}
}

////显示汉字
////x,y:起点坐标  
////pos:数组位置汉字显示
////size:字体大小 
////mode:0,反白显示;1,正常显示
void DisplayOneChina(unsigned char x,unsigned char y,unsigned char pos,unsigned char mode)
{									//	menuxone+20,			menuyone,					tempICset,			1,

	unsigned char *temp;
	unsigned int index;
	unsigned char t,t1,dat;
	unsigned char tempx = x,tempy0 = y,tempy1 = y;
	 
	index = pos * 32;

	temp = (unsigned char *)&china;
	temp += index; 	 
	
    for(t=0;t<32;t++)
    {   
			dat = *temp++;
			for(t1=0;t1<8;t1++)
			{
				if(dat&0x80)
					OLED_DrawPoint(tempx,tempy1,mode);
				else 
					OLED_DrawPoint(tempx,tempy1,!mode);
				dat<<=1;
				tempy1++;
				if(tempy1-tempy0 == 16)
				{
					tempy1 = tempy0;
					tempx++;
					break;
				}
			}  	 
    }     
}

void Displaychina(unsigned char x,unsigned char y,char *chr,unsigned char mode,unsigned int len)
{
	while(len--)
	{
		DisplayOneChina(x,y,*chr++,mode);
		x += 16;
	}
}

void Display20china(unsigned char x,unsigned char y,char *chr,unsigned char mode,unsigned int len,unsigned int size)
{										//menuxone+20,				menuyone,	tempICset,			1,									4
	while(len--)
	{
		DisplayOneChina(x,y,*chr++,mode);
		x += size;
	}
}


//显示BMP图片128×64
//起始点坐标(x,y),x的范围0～127，y为页的范围0～7
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 	unsigned short j=0;
 	unsigned char x,y;
	
  	if(y1%8==0)y=y1/8;      
  	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    	for(x=x0;x<x1;x++)
	    {      
	    	OLED_Write_Data(BMP[j++]);	    	
	    }
	}
} 

//GND  接电源地
//VCC  接5V或3.3v电源
//D0   接PD6（SCL）
//D1   接PD7（SDA）
//RES  接PD4
//DC   接PD5
//CS   接PD3 
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //�1?麬???�?�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�?盱�?3?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//?�?�50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  
 	GPIO_SetBits(GPIOA,GPIO_Pin_4);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //�1?麬???�?�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_6|GPIO_Pin_9;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�?盱�?3?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//?�?�50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  
 	GPIO_SetBits(GPIOC,GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_6|GPIO_Pin_9);	

	OLED_RST_Clr();//复位
	delay_ms(200);
	OLED_RST_Set();
		
	OLED_Command(0xAE); //关闭显示
	OLED_Command(0xD5); //设置时钟分频因子,震荡频率
	OLED_Command(80);   //[3:0],分频因子;[7:4],震荡频率
	OLED_Command(0xA8); //设置驱动路数
	OLED_Command(0X3F); //默认0X3F(1/64) 
	OLED_Command(0xD3); //设置显示偏移
	OLED_Command(0X00); //默认为0
  OLED_Command(0x40); //设置显示开始行 [5:0],行数.												    
	OLED_Command(0x8D); //电荷泵设置
	OLED_Command(0x14); //bit2，开启/关闭
	OLED_Command(0x20); //设置内存地址模式
	OLED_Command(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_Command(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_Command(0xC0); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_Command(0xDA); //设置COM硬件引脚配置
	OLED_Command(0x12); //[5:4]配置	 
	OLED_Command(0x81); //对比度设置
	OLED_Command(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_Command(0xD9); //设置预充电周期
	OLED_Command(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_Command(0xDB); //设置VCOMH 电压倍率
	OLED_Command(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_Command(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_Command(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示	    						    
	OLED_Clear();
	OLED_Command(0xAF); //开启显示	
}

void Boot_Animation(void)
{
		static u8 x=0,y=0;
		for(x = 63;x>=18;x--){
				OLED_DrawPoint(108-0.7*x,x,1);//??? ??榲3/3
				OLED_DrawPoint(17 +0.7*x,x,1);
				y = 64-x;
				OLED_DrawPoint(64-0.7*y,y,1);
				OLED_DrawPoint(64+0.7*y,y,1);
				delay_ms(2);
			  OLED_Refresh_Gram();//?????OLED
		}
		
		for(x = 30;x <= 94;x++){
				OLED_DrawPoint(125-x,47,1);
				OLED_DrawPoint(x,18,1);
				delay_ms(2);
				OLED_Refresh_Gram();//?????OLED
		}
		delay_ms(100);
		
}
//no more
