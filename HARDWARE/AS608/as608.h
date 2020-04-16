#ifndef __AS608_H
#define __AS608_H
#include "include.h"

#define PS_Sta   PCin(4)//��ָ��ģ��״̬����
#define FigOnFlag GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) //�ߵ�ƽ˵����ָ��

#define CharBuffer1 0x01
#define CharBuffer2 0x02

#define MODULE_DBSize 100
#define ERR_OK					0x00		// �ɹ�
#define ERR_DATARECEIVE			0x01		//��ʾ���ݰ����մ���;
#define ERR_NOFINGER			0x02		//��ʾ��������û����ָ;
#define ERR_READIMAGE			0x03		//��ʾ¼��ָ��ͼ��ʧ��;
#define ERR_DRYFINGER			0x04		//��ʾָ��ͼ��̫�ɡ�̫��������������;
#define ERR_WETFINGER			0x05		//��ʾָ��ͼ��̫ʪ��̫��������������;
#define ERR_MIXFINGER			0x06		//��ʾָ��ͼ��̫�Ҷ�����������;
#define ERR_LACKCHAR			0x07		//��ʾָ��ͼ������,��������̫�٣������̫С��������������;
#define ERR_MACTHCHAR			0x08  		//��ʾָ�Ʋ�ƥ��;
#define ERR_SEARCHCHAR			0x09  		//��ʾû������ָ��;
#define ERR_MERGECHAR			0x0a  		//��ʾ�����ϲ�ʧ��;
#define ERR_OUTOFINDEX			0x0b  		//��ʾ����ָ�ƿ�ʱ��ַ��ų���ָ�ƿⷶΧ;
#define ERR_READTEMPLATE		0x0c  		//��ʾ��ָ�ƿ��ģ��������Ч;
#define ERR_UPCHAR				0x0d  		//��ʾ�ϴ�����ʧ��;
#define ERR_RECEIVENEXT			0x0e  		//��ʾģ�鲻�ܽ��պ������ݰ�;
#define ERR_UPIMAGE				0x0f  		//��ʾ�ϴ�ͼ��ʧ��;
#define ERR_DELETETEMPLATE		0x10  		//��ʾɾ��ģ��ʧ��;
#define ERR_CLEARDATABASE		0x11  		//��ʾ���ָ�ƿ�ʧ��;
#define ERR_SLEEP				0x12  		//��ʾ���ܽ���͹���״̬;
#define ERR_PIN					0x13  		//��ʾ�����ȷ;
#define ERR_RESET				0x14  		//��ʾϵͳ��λʧ��;
#define ERR_NORAWIMAGE			0x15  		//��ʾ��������û����Чԭʼͼ��������ͼ��;
#define ERR_IAP					0x16  		//��ʾ��������ʧ��;
#define ERR_SAMEIMAGE			0x17  		//��ʾ����ָ�ƻ����βɼ�֮����ָû���ƶ���;
#define ERR_ACCESSFLASH			0x18  		//��ʾ��дFLASH����;
#define ERR_GENERATERANDOM		0x19  		//���������ʧ��;
#define ERR_INVALIDREGISTER		0x1a  		//��Ч�Ĵ�����;
#define ERR_FULLDATABASE		0x1f  		//ָ�ƿ���;
#define ERR_PASSWORD			0x21  		//��������;
#define ERR_TEMPLATENOTNULL		0x22  		//ָ��ģ��ǿ�;
#define ERR_TEMPLATEISNULL		0x23  		//ָ��ģ��Ϊ��;
#define ERR_DATABASEISNULL		0x24  		//ָ�ƿ�Ϊ��;
#define ERR_ENROLLCOUNT			0x25  		//¼��������ô���;
#define ERR_OVERTIME			0x26  		//��ʱ;
#define ERR_TEMPLATEEXIST		0x27  		//ָ���Ѵ���;
#define ERR_TEMPLATERELATED		0x28  		//ָ��ģ���й���;


#define ERR_ADMINOVER        0X2A //����Ա����
#define ERR_FIGNOLEAVE     0X2B//ָ��û���뿪ģ��
#define ERR_MODECOMFAIL   0X2C//ģ��ϲ�ʧ��
#define ERR_GETINDEXFAIL  0X2D//��ȡ��һ����ע������ʧ��
#define ERR_SAVEFINGFAIL  0X2E //�洢ʧ��
#define NoIdFIG           0X2F  //������ָ��ʶ��
#define ERR_ISPUBLIC     0X30//��Ҫ¼��Ĺ���Աָ���Ѿ�������ָͨ��
//2AH��EFH									//Reserved;
#define ERR_F0RESPOND			0xf0  		//�к������ݰ���ָ��,��ȷ���պ���0xf0Ӧ��;
#define ERR_F1RESPOND			0xf1  		//�к������ݰ���ָ��,�������0xf1Ӧ��;
#define ERR_FLASHPACKAGECHECK	0xf2  		//��ʾ��д�ڲ�FLASHʱ,У��ʹ���;
#define ERR_FLASHPACKAGEFLAG	0xf3  		//��ʾ��д�ڲ�FLASHʱ,����ʶ����;
#define ERR_FLASHPACKAGELENGTH	0xf4  		//��ʾ��д�ڲ�FLASHʱ,�����ȴ���;
#define ERR_FLASHPACKAGECODE	0xf5		//��ʾ��д�ڲ�FLASHʱ,���볤��̫��;
#define ERR_FLASHPACKAGEBURN	0xf6		//��ʾ��д�ڲ�FLASHʱ,��дFLASHʧ��;
//F7-FF									    //Reserved;

#define CMD_RUNEXT 0x00
#define CMD_COMPLETE 0x01


extern u32 AS608Addr;//ģ���ַ

typedef struct  
{
	u16 pageID;//ָ��ID
	u16 mathscore;//ƥ��÷�
}SearchResult;

typedef struct
{
	u16 PS_max;//ָ���������
	u8  PS_level;//��ȫ�ȼ�
	u32 PS_addr;
	u8  PS_size;//ͨѶ���ݰ���С
	u8  PS_N;//�����ʻ���N
}SysPara;

void PS_StaGPIO_Init(void);//��ʼ��PA6��״̬����
	
u8 PS_GetImage(void); //¼��ͼ�� 
 
u8 PS_GenChar(u8 BufferID);//�������� 

u8 PS_Match(void);//��ȷ�ȶ���öָ������ 

u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,unsigned short *nPageID, unsigned short *nScore);//����ָ�� 
 
u8 PS_RegModel(void);//�ϲ�����������ģ�壩 
 
u8 PS_StoreChar(u8 BufferID,u16 PageID);//����ģ�� 

u8 PS_DeletChar(u16 PageID,u16 N);//ɾ��ģ�� 

u8 PS_Empty(void);//���ָ�ƿ� 

u8 PS_WriteReg(u8 RegNum,u8 DATA);//дϵͳ�Ĵ��� 
 
u8 PS_ReadSysPara(SysPara *p); //��ϵͳ�������� 

u8 PS_SetAddr(u32 addr);  //����ģ���ַ 

u8 PS_HandShake(u32 *PS_Addr); //��AS608ģ������ 

int AutoEnrollPro(unsigned char intype); 
int AutoIdentifyPro(unsigned char identype);
int DeelOnFigAndUSERPro(unsigned char usernum);
int EmptyUserFig(void);
#endif

