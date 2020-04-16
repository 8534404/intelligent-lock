#include "include.h"

u32 AS608Addr = 0XFFFFFFFF; //Ĭ��
SearchResult match;
unsigned char g_flagloop; 
u16 man_num = 0,user_num = 5;

//��ʼ��PA6Ϊ��������		    
//��������Ӧ״̬(������Ӧʱ����ߵ�ƽ�ź�)
void PS_StaGPIO_Init(void)
{   
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  //��ʼ����״̬����GPIOA
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO	
}

//���ڷ���һ���ֽ�
static void MYUSART_SendData(u8 data)
{
	while((UART5->SR&0X40)==0); 
	UART5->DR = data;
}
//���Ͱ�ͷ
static void SendHead(void)
{
	MYUSART_SendData(0xEF);
	MYUSART_SendData(0x01);
}
//���͵�ַ
static void SendAddr(void)
{
	MYUSART_SendData(AS608Addr>>24);
	MYUSART_SendData(AS608Addr>>16);
	MYUSART_SendData(AS608Addr>>8);
	MYUSART_SendData(AS608Addr);
}
//���Ͱ���ʶ,
static void SendFlag(u8 flag)
{
	MYUSART_SendData(flag);
}
//���Ͱ�����
static void SendLength(int length)
{
	MYUSART_SendData(length>>8);
	MYUSART_SendData(length);
}
//����ָ����
static void Sendcmd(u8 cmd)
{
	MYUSART_SendData(cmd);
}
//����У���
static void SendCheck(u16 check)
{
	MYUSART_SendData(check>>8);
	MYUSART_SendData(check);
}
//�ж��жϽ��յ�������û��Ӧ���
//waittimeΪ�ȴ��жϽ������ݵ�ʱ�䣨��λ1ms��
//����ֵ�����ݰ��׵�ַ
static u8 *JudgeStr(u16 waittime)
{
	char *data;
	u8 str[8];
	str[0]=0xef;str[1]=0x01;str[2]=AS608Addr>>24;
	str[3]=AS608Addr>>16;str[4]=AS608Addr>>8;
	str[5]=AS608Addr;str[6]=0x07;str[7]='\0';
	UART5_RX_STA=0;
	while(--waittime)
	{
		delay_ms(1);
		if(UART5_RX_STA&0X8000)//���յ�һ������
		{
			UART5_RX_STA=0;
			data=strstr((const char*)UART5_RX_BUF,(const char*)str);
			if(data)
				return (u8*)data;	
		}
	}
	return 0;
}
//¼��ͼ�� PS_GetImage
//����:̽����ָ��̽�⵽��¼��ָ��ͼ�����ImageBuffer�� 
//ģ�鷵��ȷ����
u8 PS_GetImage(void)
{
  u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x01);
  temp =  0x01+0x03+0x01;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//�������� PS_GenChar
//����:��ImageBuffer�е�ԭʼͼ������ָ�������ļ�����CharBuffer1��CharBuffer2			 
//����:BufferID --> charBuffer1:0x01	charBuffer1:0x02												
//ģ�鷵��ȷ����
u8 PS_GenChar(u8 BufferID)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x04);
	Sendcmd(0x02);
	MYUSART_SendData(BufferID);
	temp = 0x01+0x04+0x02+BufferID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//��ȷ�ȶ���öָ������ PS_Match
//����:��ȷ�ȶ�CharBuffer1 ��CharBuffer2 �е������ļ� 
//ģ�鷵��ȷ����
u8 PS_Match(void)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x03);
	temp = 0x01+0x03+0x03;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//����ָ�� PS_Search
//����:��CharBuffer1��CharBuffer2�е������ļ����������򲿷�ָ�ƿ�.�����������򷵻�ҳ�롣			
//����:  BufferID @ref CharBuffer1	CharBuffer2
//˵��:  ģ�鷵��ȷ���֣�ҳ�루����ָ��ģ�壩
u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,unsigned short *nPageID, unsigned short *nScore)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x08);
	Sendcmd(0x04);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage>>8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum>>8);
	MYUSART_SendData(PageNum);
	temp = 0x01+0x08+0x04+BufferID
	+(StartPage>>8)+(u8)StartPage
	+(PageNum>>8)+(u8)PageNum;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure = data[9];
		*nPageID  =(data[10]<<8)+data[11];
		*nScore=(data[12]<<8)+data[13];	
	}
	else
		ensure = 0xff;
	return ensure;	
}
//�ϲ�����������ģ�壩PS_RegModel
//����:��CharBuffer1��CharBuffer2�е������ļ��ϲ����� ģ��,�������CharBuffer1��CharBuffer2	
//˵��:  ģ�鷵��ȷ����
u8 PS_RegModel(void)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x05);
	temp = 0x01+0x03+0x05;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;		
}
//����ģ�� PS_StoreChar
//����:�� CharBuffer1 �� CharBuffer2 �е�ģ���ļ��浽 PageID ��flash���ݿ�λ�á�			
//����:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID��ָ�ƿ�λ�úţ�
//˵��:  ģ�鷵��ȷ����
u8 PS_StoreChar(u8 BufferID,u16 PageID)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x06);
	Sendcmd(0x06);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	temp = 0x01+0x06+0x06+BufferID
	+(PageID>>8)+(u8)PageID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;	
}
//ɾ��ģ�� PS_DeletChar
//����:  ɾ��flash���ݿ���ָ��ID�ſ�ʼ��N��ָ��ģ��
//����:  PageID(ָ�ƿ�ģ���)��Nɾ����ģ�������
//˵��:  ģ�鷵��ȷ����
u8 PS_DeletChar(u16 PageID,u16 N)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x07);
	Sendcmd(0x0C);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	MYUSART_SendData(N>>8);
	MYUSART_SendData(N);
	temp = 0x01+0x07+0x0C
	+(PageID>>8)+(u8)PageID
	+(N>>8)+(u8)N;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//���ָ�ƿ� PS_Empty
//����:  ɾ��flash���ݿ�������ָ��ģ��
//����:  ��
//˵��:  ģ�鷵��ȷ����
u8 PS_Empty(void)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x0D);
	temp = 0x01+0x03+0x0D;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//дϵͳ�Ĵ��� PS_WriteReg
//����:  дģ��Ĵ���
//����:  �Ĵ������RegNum:4\5\6
//˵��:  ģ�鷵��ȷ����
u8 PS_WriteReg(u8 RegNum,u8 DATA)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x05);
	Sendcmd(0x0E);
	MYUSART_SendData(RegNum);
	MYUSART_SendData(DATA);
	temp = RegNum+DATA+0x01+0x05+0x0E;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	if(ensure==0)
		printf("\r\n���ò����ɹ���");
	return ensure;
}

//����ģ���ַ PS_SetAddr
//����:  ����ģ���ַ
//����:  PS_addr
//˵��:  ģ�鷵��ȷ����
u8 PS_SetAddr(u32 PS_addr)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x07);
	Sendcmd(0x15);
	MYUSART_SendData(PS_addr>>24);
	MYUSART_SendData(PS_addr>>16);
	MYUSART_SendData(PS_addr>>8);
	MYUSART_SendData(PS_addr);
	temp = 0x01+0x07+0x15
	+(u8)(PS_addr>>24)+(u8)(PS_addr>>16)
	+(u8)(PS_addr>>8) +(u8)PS_addr;				
	SendCheck(temp);
	AS608Addr=PS_addr;//������ָ�������ַ
  data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;	
		AS608Addr = PS_addr;
	if(ensure==0x00)
		printf("\r\n���õ�ַ�ɹ���");
	return ensure;
}

//��AS608���� PS_HandShake
//����: PS_Addr��ַָ��
//˵��: ģ�鷵�µ�ַ����ȷ��ַ��	
u8 PS_HandShake(u32 *PS_Addr)
{
	SendHead();
	SendAddr();
	MYUSART_SendData(0X01);
	MYUSART_SendData(0X00);
	MYUSART_SendData(0X00);	
	delay_ms(200);
	if(UART5_RX_STA&0X8000)//���յ�����
	{		
		if(//�ж��ǲ���ģ�鷵�ص�Ӧ���				
				UART5_RX_BUF[0]==0XEF
				&&UART5_RX_BUF[1]==0X01
				&&UART5_RX_BUF[6]==0X07
			)
			{
				*PS_Addr=(UART5_RX_BUF[2]<<24) + (UART5_RX_BUF[3]<<16)
								+(UART5_RX_BUF[4]<<8) + (UART5_RX_BUF[5]);
				UART5_RX_STA=0;
				return 0;
			}
		UART5_RX_STA=0;					
	}
	return 1;		
}


int AutoEnrollPro(unsigned char intype)
{
	unsigned short tempcnt = 0,Key_Value,i;
	unsigned char circnt = 0;//ѭ������
	unsigned short uMatchIndex, uMatchScore;    // ����������ֵ������
	unsigned char tempdelaycnt = 20; 
	
	int iRet;
	u8 test_cir = 0;
	if(intype == 1)
	{
		if(Fin.Users_Num >= (MODULE_DBSize - 5))
		{
			//�û�ָ����
			return ERR_OUTOFINDEX;
		}
		
		if(Fin.UserFullFlag == 1)
		{
				for(i=5;i<MODULE_DBSize;i++)
				{
					if(Fin.Value[i]==0)
					{
						Fin.Users_Save_Num=i;
						break;
					}
				}
		}
	}
	else
	{
		if(Fin.Man_Num >= 5 || man_num >= 5)   
		{  
			//����Ա����
			delay_ms(1000);
			return ERR_ADMINOVER;
		}	
		if(Fin.ManFullFlag == 1)
		{
			for(i=0;i<5;i++)
			{
				if(Fin.Value[i]==0)
				{
					Fin.Man_Save_Num=i;
					break;
				}
			}
		}	
		
	}
	g_flagloop = true;
	//��Ҫ2��ע��
	for(circnt = 1;circnt <= 2;circnt++)
	{	
		while(g_flagloop)	
		{
			//��ȡע��ͼ��
			if((iRet = PS_GetImage()) == ERR_OK)	
			{   //  �粻֧�ֿɻ���cmd_GetImage
					break;
			}
		
			//ע�ᳬʱ
			if((TimingDelay-menu_now) > TOUCH_Timeout/2)	
			{
					g_flagloop = false; 			  
					return ERR_OVERTIME;
			}
			Key_Value = KEY_Scan3x4();  			
			if(Key_Value=='*')
			{
				return 0xFE;
			}
		}
			
		//���������ļ�
		test_cir = (u8)circnt;
		iRet = PS_GenChar(test_cir);
		if(iRet != ERR_OK)
		{
			//���ʧ��
			return iRet;
		}
		
		//����ָ�ƿ����Ƿ�������ָ��
		iRet = PS_Search(circnt, 0, MODULE_DBSize,&uMatchIndex, &uMatchScore);
		if(iRet == ERR_OK	)//��������ָ��
		{
			for(tempcnt = 0;tempcnt < 5;tempcnt++)
			{
				//�͹���Աָ���ظ�
				if(Fin.Value[tempcnt] == (uMatchIndex))
				{
					g_flagloop = false;										
					JQ8400_SendCmd(2,4,1,1);
					delay_ms(1500);
					return ERR_TEMPLATEEXIST;
				}
			}
			if(intype == 0)//�����ӵ�ʱ����Ա���� ���Ҵ�ָ���Ѿ�¼���� 
			{
					g_flagloop = false;
					//ָ���Ѵ���
					return ERR_ISPUBLIC;
			}		
		}
		else if( (iRet != ERR_SEARCHCHAR))//��������ERR_SEARCHCHARû��������
		{
			//���ʧ��
			return iRet;
		}	
			
		//�ж���ָ�Ƿ��뿪
		if(circnt != 2)
		{
			menu_now = TimingDelay;//�������ֵ
			if(circnt!=2)
			{
				JQ8400_SendCmd(1,8,1,1);
			}
			 g_flagloop = true;
			 menu_now = TimingDelay;
			 while(g_flagloop)	
			 {
				if((iRet = PS_GetImage()) == ERR_NOFINGER)	
				{
						tempdelaycnt = 20;
						
						break;
				}
				else 
				{
					JQ8400_SendCmd(1,2,1,1);
					delay_ms(1000);
				}
				if((TimingDelay - menu_now) > TOUCH_Timeout)	
				{
					return ERR_OVERTIME;
				}
				tempdelaycnt++;
				if(tempdelaycnt >= 15)//3s
				{
					tempdelaycnt = 0;
				}
				Key_Value = KEY_Scan3x4(); 
				if(Key_Value=='*')
				{
					return 0xFE;
				}
			 }
		}
	}	
	iRet = PS_RegModel();
	if(iRet != ERR_OK)
	{
		//���ʧ��
		return iRet;
	}
	//�洢ģ��
	if(intype == 1)
	{
		if(user_num >= 99)
		{
			user_num = 0;
		}
		iRet = PS_StoreChar(1,user_num);
	}
	else
	{
		if(man_num >= 5)
		{
			man_num = 0;
		}
		iRet = PS_StoreChar(1,man_num);
	}
	if(iRet != ERR_OK)
	{
		//���ʧ��
		return iRet;
	}
	if(intype == 0)
	{
		Fin.Man_Num++;
		if(Fin.ManFullFlag == 0)//��û����
		{
			SumGen.Save_Num = Fin.Man_Save_Num;
			Fin.Value[Fin.Man_Save_Num] = (man_num);
			save_flash(FINPT,Fin.Man_Save_Num);
			if(Fin.Man_Save_Num == 4)//����Ѿ��������������
			{
				Fin.ManFullFlag = 1;//�Ѿ��洢����
				flash_save(44,Fin.ManFullFlag);
				for(i=0;i<5;i++)
				{
					if(Fin.Value[i]==0)
					{
						Fin.Man_Save_Num=i;
						break;
					}
				}
			}
			else
			{
				Fin.Man_Save_Num++;
				man_num++;
			}			
		}
		else  //�Ѿ��洢����
		{
			for(i=0;i<5;i++)
			{
				if(Fin.Value[i]==0)
				{
					Fin.Man_Save_Num=i;
					break;
				}
			}
			SumGen.Save_Num  = Fin.Man_Save_Num;
			Fin.Value[Fin.Man_Save_Num] = (man_num);
			save_flash(FINPT,Fin.Man_Save_Num);	
		}
	}
	else//��ͨ�û�
	{
		Fin.Users_Num = user_num;			
		if(Fin.UserFullFlag == 0)//��û����
		{
			SumGen.Save_Num  = Fin.Users_Save_Num;
			Fin.Value[Fin.Users_Save_Num] = (user_num);
			save_flash(FINPT,Fin.Users_Save_Num);
			if(Fin.Users_Save_Num == (MODULE_DBSize-1))//����Ѿ��������������
			{
				Fin.UserFullFlag = 1;//�Ѿ��洢����
				flash_save(45,Fin.UserFullFlag);
				for(i=5;i<MODULE_DBSize;i++)
				{
					if(Fin.Value[i]==0)
					{
						Fin.Users_Save_Num=i;
						break;
					}
				}
			}
			else
			{
				Fin.Users_Save_Num++;
				user_num++;
			}
		}
		else //�Ѿ��洢����
		{
			for(i=5;i<MODULE_DBSize;i++)
			{
				if(Fin.Value[i]==0)
				{
					Fin.Users_Save_Num=i;
					break;
				}
			}
			SumGen.Save_Num  = Fin.Users_Save_Num;
			Fin.Value[Fin.Users_Save_Num] = (user_num);
			save_flash(FINPT,Fin.Users_Save_Num);
		}
	}
	return 0;
}

//=================================================================================
// ʵ��ָ��ʶ����, ��������ʶ��ͱ���ʶ�� 
// ����:
//  identype ʶ������ 0 ����ʶ��  1 ����ʶ��(�����û��Ѵ�������) 2����ʶ�� �����û��Ѵ�������

//figidadd ʶ��ָ�Ƶĵ�ַ
// ����ֵ:
//     0xFF : ������ʶ��
//     0xFE : ʧ�ܶ��
//     0  : �ɹ�
//     ����: ʧ��
//=================================================================================
unsigned char startIdentifyflag = 0;//��ʼ����ָ��ʶ����
int AutoIdentifyPro(unsigned char identype)
{
	int iRet = 0;
	unsigned tempcnt = 0;
	unsigned short uMatchIndex, uMatchScore;    // ����������ֵ������
	if(identype > 0)
	{
		startIdentifyflag = 1;
	}
	else if(identype == 0)
	{
		if(startIdentifyflag == 0)
		{
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))//�������ָ��
			{
					startIdentifyflag = 1;
			}
		}
	}
	if(startIdentifyflag == 1)//ʶ��ʼ
	{
		FIN_FLAG=3;
		g_flagloop = true;
//		TimingDelay_flag = 0;//����ʱ��
		do
		{
			menu_now = TimingDelay;
			while(g_flagloop)
			{
				 if((iRet = PS_GetImage()) == ERR_OK)
				 {
						break;
				 }
				 if((TimingDelay-menu_now) > TOUCH_Timeout/2)	
				 {
						if(identype == 1)
						{
								//����ʧ�� ������
						}
						iRet = ERR_OVERTIME;
						g_flagloop = false;
						startIdentifyflag = 0;
						return  iRet;
				 }
				 delay_ms(100);
			}
			iRet =  PS_GenChar(CharBuffer1);
			if(iRet != ERR_OK)
			{
				if(identype != 0)
				{
					//����ʧ�� ������
				}
				startIdentifyflag = 0;
				return iRet;
			}
			
			iRet = PS_Search(CharBuffer1, 0, MODULE_DBSize,&uMatchIndex, &uMatchScore);
			if(iRet == ERR_OK)
			{
				if(identype == 1)
				{
					//�û��Ѵ���
				}
				for(tempcnt =0;tempcnt <sizeof(Fin.Value);tempcnt++)
				{
					if(Fin.Value[tempcnt] == (uMatchIndex))
					{
						Fin.Read_Num = uMatchIndex;
						if(uMatchIndex<5)
							FIN_FLAG=0; //����Ա
						else 
							FIN_FLAG=1; //��ͨ�û�
						break;							
					}
				}
				startIdentifyflag = 0;
				return ERR_OK;
			}
			else if(iRet == ERR_SEARCHCHAR)
			{
				if(identype == 0)
				{
					//�û�bu����
				}
				startIdentifyflag = 0;
				return ERR_SEARCHCHAR;
			}
			else
			{
				if(identype != 0)
				{
					//����ʧ��,������
				}
				startIdentifyflag = 0;
				
				return iRet;
			}
		}while(0);	
	}
	return NoIdFIG;
}

int DeelOnFigAndUSERPro(unsigned char usernum)
{
	int iRet;
	do{
		iRet = PS_DeletChar(Fin.Read_Num,1);
	  if(iRet != 0)break;
	}while(0);
	
	if(iRet == 0)
	{
		if(Fin.Read_Num < 5)
		{
			Fin.Man_Num--;
		}
		else 
		{
			Fin.Users_Num--;
		}
	  
		Fin.Value[usernum] = 0x00;
	}
	else
	{
		//ɾ��ʧ��
	  delay_ms(2000);
	}
	return iRet;
}

//=================================================================================
// ����û�ָ��
// ����:
// 
// ����ֵ:
//     
//     0  : �ɹ�
//     ����: ʧ��
//=================================================================================
int EmptyUserFig(void)
{
	int iRet;
	unsigned char tempcnt = 0;
	do{
		for(tempcnt = 5;tempcnt < MODULE_DBSize;tempcnt++)	
		{
			if(Fin.Value[tempcnt] != 0)//��ַ����ָ�ƴ���
			{
				iRet = PS_DeletChar(Fin.Value[tempcnt]-1,1);//�Ե�ַ����ɾ��
				if(iRet != 0)   break;
			}
		}
	} while(0);
    
	if(iRet == 0)
	{	
		delay_ms(200);
	}
	else
	{
		//����ʧ��
		delay_ms(200);
	}
	
	return iRet;
}





























