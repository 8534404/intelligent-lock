#include "include.h"

#define OLED_CS_L  GPIO_ResetBits(GPIOC,GPIO_Pin_6)
#define OLED_CS_H  GPIO_SetBits(GPIOC,GPIO_Pin_6)

#define OLED_DC_H  GPIO_SetBits(GPIOC,GPIO_Pin_7)
#define OLED_DC_L  GPIO_ResetBits(GPIOC,GPIO_Pin_7)//DC����	

#define OLED_SCL_L GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define OLED_SCL_H GPIO_SetBits(GPIOC,GPIO_Pin_9)

#define OLED_SDA_L GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define OLED_SDA_H GPIO_SetBits(GPIOC,GPIO_Pin_8)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)

//OLED���Դ�
//��Ÿ�ʽ����.
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
//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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
 // GPIO_SetBits(GPIOB,GPIO_Pin_5);//DC����	
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
		OLED_Command (0xb0+i);    //����ҳ��ַ��0~7��
		OLED_Command (0x00);      //������ʾλ�á��е͵�ַ
		OLED_Command (0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_Write_Data(OLED_GRAM[n][i]); 
	}  
	
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_Command(0xb0+y);
	OLED_Command(((x&0xf0)>>4)|0x10);
	OLED_Command((x&0x0f)|0x01); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_Command(0X8D);  //SET DCDC����
	OLED_Command(0X14);  //DCDC ON
	OLED_Command(0XAF);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_Command(0X8D);  //SET DCDC����
	OLED_Command(0X10);  //DCDC OFF
	OLED_Command(0XAE);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
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
	OLED_Refresh_Gram();//������ʾ 
}

//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t)
{
	unsigned char pos,bx,temp=0;

	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
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
	OLED_Refresh_Gram();//������ʾ
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24
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

//��ʾ�ַ���
void DisplayStr(unsigned char x,unsigned char y,char *chr,unsigned char mode,unsigned int size)
{
	unsigned int len = strlen(chr);
	while(len--)
	{
		OLED_ShowChar(x,y,*chr++,mode);
		x += size;
	}
}

//��ʾ����
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

////��ʾ����
////x,y:�������  
////pos:����λ�ú�����ʾ
////size:�����С 
////mode:0,������ʾ;1,������ʾ
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


//��ʾBMPͼƬ128��64
//��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
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

//GND  �ӵ�Դ��
//VCC  ��5V��3.3v��Դ
//D0   ��PD6��SCL��
//D1   ��PD7��SDA��
//RES  ��PD4
//DC   ��PD5
//CS   ��PD3 
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //�1?�A???��?�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�?���?3?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//?�?�50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  
 	GPIO_SetBits(GPIOA,GPIO_Pin_4);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //�1?�A???��?�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_6|GPIO_Pin_9;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�?���?3?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//?�?�50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  
 	GPIO_SetBits(GPIOC,GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_6|GPIO_Pin_9);	

	OLED_RST_Clr();//��λ
	delay_ms(200);
	OLED_RST_Set();
		
	OLED_Command(0xAE); //�ر���ʾ
	OLED_Command(0xD5); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_Command(80);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_Command(0xA8); //��������·��
	OLED_Command(0X3F); //Ĭ��0X3F(1/64) 
	OLED_Command(0xD3); //������ʾƫ��
	OLED_Command(0X00); //Ĭ��Ϊ0
  OLED_Command(0x40); //������ʾ��ʼ�� [5:0],����.												    
	OLED_Command(0x8D); //��ɱ�����
	OLED_Command(0x14); //bit2������/�ر�
	OLED_Command(0x20); //�����ڴ��ַģʽ
	OLED_Command(0x02); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_Command(0xA1); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_Command(0xC0); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_Command(0xDA); //����COMӲ����������
	OLED_Command(0x12); //[5:4]����	 
	OLED_Command(0x81); //�Աȶ�����
	OLED_Command(0xEF); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_Command(0xD9); //����Ԥ�������
	OLED_Command(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_Command(0xDB); //����VCOMH ��ѹ����
	OLED_Command(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_Command(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_Command(0xA6); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						    
	OLED_Clear();
	OLED_Command(0xAF); //������ʾ	
}

void Boot_Animation(void)
{
		static u8 x=0,y=0;
		for(x = 63;x>=18;x--){
				OLED_DrawPoint(108-0.7*x,x,1);//??? ??�v3/3
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
