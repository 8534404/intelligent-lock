#include "include.h"

/****************************************************************************
* Function Name  : void KEY_Init(void)
* Description    : ������ʼ������ PA0.15��PC5 ���ó�����

****************************************************************************/ 	    
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTCʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA15

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0
	
	//��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,DISABLE);
	
} 


/****************************************************************************
* Function Name  : u8 KEY_Scan(void)
* Description    : ������ʼ������ PA0.15��PC5 ���ó�����

****************************************************************************/ 	 
u8 KEY_Scan(void)
{	 
	if(KEY0==0||KEY1==0||WK_UP==1)
	{
		delay_ms(10);//ȥ���� 
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES; 
	} 	     
	return 0;// �ް�������
}

void Deal_KEY(u8 data)
{
	int i;
  if(data == KEY0_PRES)
	{
		for(i = 0;i<3;i++)
		{
			JQ8400_SendCmd(3,0,1,1);
		  delay_ms(800);
		}
		//����wifi����
	}
	else if(data == KEY1_PRES)
	{
		reset_sw = RESET_Switch();
		if(reset_sw == 0)
		{
			JQ8400_SendCmd(1,9,1,1);
			delay_ms(800);
			Mode = 0;
			re_flag = 0;
			Clear_Data(ALL);
			flash_empty(0);
			delay_ms(100);
			NVIC_SystemReset();//����
		}
		else
		{
			//���vertify����
			show_flag = 0;
		}
	}
}


u8 KEY_Scan3x4(void)
{
	volatile unsigned char key_Value = 0xff;
	static unsigned char key_flag1 = 1,key_flag2 = 1,key_flag3 = 1,key_flag4 = 1;
	
	GPIO_Write(GPIOB,((GPIOB->ODR & 0xFE1F) | 0x0020));
	if(key_flag1 && (Read_Lie1 == 1 || Read_Lie2 ==1 || Read_Lie3 == 1))
	{
		key_flag1 = 0;
		if(Read_Lie1 == 1)
		{
			key_Value = '1';
			printf("key=%c\r\n",key_Value);
						return key_Value;
		}
		else if(Read_Lie2 == 1)
		{
			key_Value = '2';
			printf("key=%c\r\n",key_Value);
						return key_Value;
		}
		else if(Read_Lie3 == 1)
		{
			key_Value = '3';
			printf("key=%c\r\n",key_Value);
						return key_Value;
		}
	}
	else if(Read_Lie1 == 0 && Read_Lie2 ==0 && Read_Lie3 == 0)key_flag1 = 1;
	else return 0;
	
	GPIO_Write(GPIOB,((GPIOB->ODR & 0xFE1F) | 0x0040));
	if(key_flag2 && (Read_Lie1 == 1 || Read_Lie2 ==1 || Read_Lie3 == 1))
	{
		key_flag2 = 0;
		if(Read_Lie1 == 1)
		{
			key_Value = '4';
			printf("key=%c\r\n",key_Value);
						return key_Value;
		}
		else if(Read_Lie2 == 1)
		{
			key_Value = '5';
			printf("key=%c\r\n",key_Value);
						return key_Value;
		}
		else if(Read_Lie3 == 1)
		{
			key_Value = '6';
			printf("key=%c\r\n",key_Value);
						return key_Value;
		}
	}
	else if(Read_Lie1 == 0 && Read_Lie2 ==0 && Read_Lie3 == 0)key_flag2 = 1;
	else return 0;
	
	
	GPIO_Write(GPIOB,((GPIOB->ODR & 0xFE1F) | 0x0080));
	if(key_flag3 && (Read_Lie1 == 1 || Read_Lie2 ==1 || Read_Lie3 == 1))
	{
		key_flag3 = 0;
		if(Read_Lie1 == 1)
		{
			key_Value = '7';
			printf("key=%c\r\n",key_Value);
						return key_Value;
		}
		else if(Read_Lie2 == 1)
		{
			key_Value = '8';
			printf("key=%c\r\n",key_Value);
						return key_Value;
		}
		else if(Read_Lie3 == 1)
		{
			key_Value = '9';
			printf("key=%c\r\n",key_Value);
						return key_Value;
		}
	}
	else if(Read_Lie1 == 0 && Read_Lie2 ==0 && Read_Lie3 == 0)key_flag3 = 1;
	else return 0;
	
	GPIO_Write(GPIOB,((GPIOB->ODR & 0xFE1F) | 0x0100));
	if(key_flag4 && (Read_Lie1 == 1 || Read_Lie2 ==1 || Read_Lie3 == 1))
	{
		key_flag4 = 0;
		if(Read_Lie1 == 1)
		{
			key_Value = '*';
			printf("key=%c\r\n",key_Value);
						return key_Value;
		}
		else if(Read_Lie2 == 1)
		{
			key_Value = '0';
			printf("key=%c\r\n",key_Value);
						return key_Value;
		}
		else if(Read_Lie3 == 1)
		{
			key_Value = '#';
			printf("key=%c\r\n",key_Value);
						return key_Value;
		}
	}
	else if(Read_Lie1 == 0 && Read_Lie2 ==0 && Read_Lie3 == 0)key_flag4 = 1;
	else return 0;
	
	return 0;
}
