#include "include.h"

#define OLED_CS_L  GPIO_ResetBits(GPIOC,GPIO_Pin_6)
#define OLED_CS_H  GPIO_SetBits(GPIOC,GPIO_Pin_6)

#define OLED_DC_H  GPIO_SetBits(GPIOC,GPIO_Pin_7)
#define OLED_DC_L  GPIO_ResetBits(GPIOC,GPIO_Pin_7)//DCÃüÁî	

#define OLED_SCL_L GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define OLED_SCL_H GPIO_SetBits(GPIOC,GPIO_Pin_9)

#define OLED_SDA_L GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define OLED_SDA_H GPIO_SetBits(GPIOC,GPIO_Pin_8)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)

//OLEDµÄÏÔ´æ
//´æ·Å¸ñÊ½ÈçÏÂ.
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
//ÏòSSD1106Ğ´ÈëÒ»¸ö×Ö½Ú¡£
//dat:ÒªĞ´ÈëµÄÊı¾İ/ÃüÁî
//cmd:Êı¾İ/ÃüÁî±êÖ¾ 0,±íÊ¾ÃüÁî;1,±íÊ¾Êı¾İ;
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
 // GPIO_SetBits(GPIOB,GPIO_Pin_5);//DCÃüÁî	
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
		OLED_Command (0xb0+i);    //ÉèÖÃÒ³µØÖ·£¨0~7£©
		OLED_Command (0x00);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞµÍµØÖ·
		OLED_Command (0x10);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞ¸ßµØÖ·   
		for(n=0;n<128;n++)OLED_Write_Data(OLED_GRAM[n][i]); 
	}  
	
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_Command(0xb0+y);
	OLED_Command(((x&0xf0)>>4)|0x10);
	OLED_Command((x&0x0f)|0x01); 
}   	  
//¿ªÆôOLEDÏÔÊ¾    
void OLED_Display_On(void)
{
	OLED_Command(0X8D);  //SET DCDCÃüÁî
	OLED_Command(0X14);  //DCDC ON
	OLED_Command(0XAF);  //DISPLAY ON
}
//¹Ø±ÕOLEDÏÔÊ¾     
void OLED_Display_Off(void)
{
	OLED_Command(0X8D);  //SET DCDCÃüÁî
	OLED_Command(0X10);  //DCDC OFF
	OLED_Command(0XAE);  //DISPLAY OFF
}		   			 
//ÇåÆÁº¯Êı,ÇåÍêÆÁ,Õû¸öÆÁÄ»ÊÇºÚÉ«µÄ!ºÍÃ»µãÁÁÒ»Ñù!!!	  
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
	OLED_Refresh_Gram();//¸üĞÂÏÔÊ¾ 
}

//»­µã 
//x:0~127
//y:0~63
//t:1 Ìî³ä 0,Çå¿Õ				   
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t)
{
	unsigned char pos,bx,temp=0;

	if(x>127||y>63)return;//³¬³ö·¶Î§ÁË.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

//x1,y1,x2,y2 Ìî³äÇøÓòµÄ¶Ô½Ç×ø±ê
//È·±£x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,Çå¿Õ;1,Ìî³ä	  
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
	OLED_Refresh_Gram();//¸üĞÂÏÔÊ¾
}

//ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û,°üÀ¨²¿·Ö×Ö·û
//x:0~127
//y:0~63
//mode:0,·´°×ÏÔÊ¾;1,Õı³£ÏÔÊ¾				 
//size:Ñ¡Ôñ×ÖÌå 12/16/24
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

//ÏÔÊ¾×Ö·û´®
void DisplayStr(unsigned char x,unsigned char y,char *chr,unsigned char mode,unsigned int size)
{
	unsigned int len = strlen(chr);
	while(len--)
	{
		OLED_ShowChar(x,y,*chr++,mode);
		x += size;
	}
}

//ÏÔÊ¾ÕûÊı
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

////ÏÔÊ¾ºº×Ö
////x,y:Æğµã×ø±ê  
////pos:Êı×éÎ»ÖÃºº×ÖÏÔÊ¾
////size:×ÖÌå´óĞ¡ 
////mode:0,·´°×ÏÔÊ¾;1,Õı³£ÏÔÊ¾
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


//ÏÔÊ¾BMPÍ¼Æ¬128¡Á64
//ÆğÊ¼µã×ø±ê(x,y),xµÄ·¶Î§0¡«127£¬yÎªÒ³µÄ·¶Î§0¡«7
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

//GND  ½ÓµçÔ´µØ
//VCC  ½Ó5V»ò3.3vµçÔ´
//D0   ½ÓPD6£¨SCL£©
//D1   ½ÓPD7£¨SDA£©
//RES  ½ÓPD4
//DC   ½ÓPD5
//CS   ½ÓPD3 
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ê1?üA???úê±?ó
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //í?íìê?3?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//?ù?è50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  
 	GPIO_SetBits(GPIOA,GPIO_Pin_4);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ê1?üA???úê±?ó
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_6|GPIO_Pin_9;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //í?íìê?3?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//?ù?è50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  
 	GPIO_SetBits(GPIOC,GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_6|GPIO_Pin_9);	

	OLED_RST_Clr();//¸´Î»
	delay_ms(200);
	OLED_RST_Set();
		
	OLED_Command(0xAE); //¹Ø±ÕÏÔÊ¾
	OLED_Command(0xD5); //ÉèÖÃÊ±ÖÓ·ÖÆµÒò×Ó,Õğµ´ÆµÂÊ
	OLED_Command(80);   //[3:0],·ÖÆµÒò×Ó;[7:4],Õğµ´ÆµÂÊ
	OLED_Command(0xA8); //ÉèÖÃÇı¶¯Â·Êı
	OLED_Command(0X3F); //Ä¬ÈÏ0X3F(1/64) 
	OLED_Command(0xD3); //ÉèÖÃÏÔÊ¾Æ«ÒÆ
	OLED_Command(0X00); //Ä¬ÈÏÎª0
  OLED_Command(0x40); //ÉèÖÃÏÔÊ¾¿ªÊ¼ĞĞ [5:0],ĞĞÊı.												    
	OLED_Command(0x8D); //µçºÉ±ÃÉèÖÃ
	OLED_Command(0x14); //bit2£¬¿ªÆô/¹Ø±Õ
	OLED_Command(0x20); //ÉèÖÃÄÚ´æµØÖ·Ä£Ê½
	OLED_Command(0x02); //[1:0],00£¬ÁĞµØÖ·Ä£Ê½;01£¬ĞĞµØÖ·Ä£Ê½;10,Ò³µØÖ·Ä£Ê½;Ä¬ÈÏ10;
	OLED_Command(0xA1); //¶ÎÖØ¶¨ÒåÉèÖÃ,bit0:0,0->0;1,0->127;
	OLED_Command(0xC0); //ÉèÖÃCOMÉ¨Ãè·½Ïò;bit3:0,ÆÕÍ¨Ä£Ê½;1,ÖØ¶¨ÒåÄ£Ê½ COM[N-1]->COM0;N:Çı¶¯Â·Êı
	OLED_Command(0xDA); //ÉèÖÃCOMÓ²¼şÒı½ÅÅäÖÃ
	OLED_Command(0x12); //[5:4]ÅäÖÃ	 
	OLED_Command(0x81); //¶Ô±È¶ÈÉèÖÃ
	OLED_Command(0xEF); //1~255;Ä¬ÈÏ0X7F (ÁÁ¶ÈÉèÖÃ,Ô½´óÔ½ÁÁ)
	OLED_Command(0xD9); //ÉèÖÃÔ¤³äµçÖÜÆÚ
	OLED_Command(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_Command(0xDB); //ÉèÖÃVCOMH µçÑ¹±¶ÂÊ
	OLED_Command(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_Command(0xA4); //È«¾ÖÏÔÊ¾¿ªÆô;bit0:1,¿ªÆô;0,¹Ø±Õ;(°×ÆÁ/ºÚÆÁ)
	OLED_Command(0xA6); //ÉèÖÃÏÔÊ¾·½Ê½;bit0:1,·´ÏàÏÔÊ¾;0,Õı³£ÏÔÊ¾	    						    
	OLED_Clear();
	OLED_Command(0xAF); //¿ªÆôÏÔÊ¾	
}

void Boot_Animation(void)
{
		static u8 x=0,y=0;
		for(x = 63;x>=18;x--){
				OLED_DrawPoint(108-0.7*x,x,1);//??? ??˜v3/3
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
