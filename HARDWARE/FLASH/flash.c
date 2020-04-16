#include "include.h"

extern flash Flash;
extern fin Fin;
extern pwd Pwd;
extern ic IC;

u8 Volume_Flag=1;
  
void flash_read(uint8_t pagination,uint16_t num)
{
	u16 i=0;
	uint16_t flash_tmp;
	uint32_t	FLASH_APP1_READ_ADDR;
	//flag=0;
	FLASH_APP1_READ_ADDR=FLASH_APP1_START_ADDR+STM_PAGE_SIZE*pagination;
	FLASH_Unlock();
	
	while((FLASH_APP1_READ_ADDR + 2*i)< FLASH_APP1_READ_ADDR+512*4)
	{
		flash_tmp=*(__IO uint16_t*)(FLASH_APP1_READ_ADDR + 2*i);
		if(flash_tmp!=0xffff)
			Flash.read_data[i]=flash_tmp;
		else
			Flash.read_data[i]=0;
		i++;
		if(i>num)
			break;
	}
	FLASH_Lock();
}

uint32_t FLASH_APP1_SAVE_test;
void flash_write(uint8_t pagination,uint16_t num)
{
	u16 i=0;
//	u16 write_data[30]={0,1,2,3,4,5,6};

	uint32_t FLASH_APP1_SAVE_ADDR;
   	FLASH_APP1_SAVE_ADDR=FLASH_APP1_START_ADDR + STM_PAGE_SIZE*pagination;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY |FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(FLASH_APP1_SAVE_ADDR);
	
	while(FLASH_ProgramHalfWord(FLASH_APP1_SAVE_ADDR,Flash.write_data[i])==FLASH_COMPLETE)
	{
		FLASH_APP1_SAVE_ADDR=FLASH_APP1_SAVE_ADDR+2;
		FLASH_APP1_SAVE_test=FLASH_APP1_SAVE_ADDR;
		 i++;
		if(i>num)
			break;
	}	
	FLASH_Lock();
}

uint32_t FLASH_APP1_Clear_ADDR;
void flash_empty(u8 mode)
{
    u8 i;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    for(i=mode;i<8;i++)  
    {    
      FLASH_ErasePage(FLASH_APP1_START_ADDR + STM_PAGE_SIZE*i);
    }
    FLASH_Lock();
}

void read_flash(u8 type)
{
	u16 i,j;
	if(type == FINPT || type == ALL)
	{
		flash_read(FINPT,400);
		for(i=0;i<100;i++)
		{
			Fin.Value[i]=Flash.read_data[i];
		}
	}
	if(type == PASSWORD || type == ALL)
	{
		flash_read(PASSWORD,400);
		for(j=0;j<40;j++)
		{
			for(i=0;i<11;i++)
			{
				Pwd.Value[j][i]=Flash.read_data[j*11+i];
			}
		}
	}
	if(type == ICCARD || type == ALL)	
	{
		flash_read(ICCARD,400);
		for(i=0;i<200;i+=2)
		{
			IC.Value[i/2]=Flash.read_data[i]*65536+Flash.read_data[i+1];
		}
	}
}

void save_flash(u8 type,u8 num)
{
		u16 i,j;
	
		flash_read(type,400);
		for(i=0;i<400;i++)
			Flash.write_data[i]=Flash.read_data[i];	
		if(type == FINPT)
		{
			Flash.write_data[num]=Fin.Value[num];
		}
		else if(type == PASSWORD)
		{
			for(j=0;j<11;j++)
			{
				Flash.write_data[num*11+j]=Pwd.Value[num][j];
			}
		}
		else if(type == ICCARD)
		{
			Flash.write_data[num*2]=(uint16_t)(IC.Value[num]/65536);
			Flash.write_data[num*2+1]=(uint16_t)IC.Value[num];
		}
		flash_write(type,400);
}

u8 Record_Save_num = 0,Record_Save_Flag = 0;
u16 Record[8],Record_Show;
void Time_save(u8 time[5],u16 type,u8 mode)
{
	u16 save_buf[5] = {0};
	save_buf[0] = save_buf[0];
	save_buf[0]=((u16)time[1]<<8)+(u16)time[2];
	save_buf[1]=((u16)time[3]<<8)+(u16)time[4];
	save_buf[2]=((u16)mode<<16)+(u16)type;
}

void Record_Save(u8 Time[5],u8 mode,u16 type)
{
	u16 Data[7],i,j,page,number;
	j = j;

	for(i=0;i<5;i++)
	Data[i]=Time[i]; 
	Data[5]=mode;
	Data[6]=type;
	page=Record_Save_num%100/50+5;
	number=Record_Save_num%100%50;
	flash_read(page,350);
	for(i=0;i<350;i++)
	{
		Flash.write_data[i]=Flash.read_data[i];
	}			
	for(i=0;i<7;i++)
		Flash.write_data[number*7+i]=Data[i];		
	flash_write(page,350);
	Record_Save_num++;
	flash_save(36,Record_Save_num);
}

void Record_Read(u16 num)
{
		u8 i,page;	
		page=Record_Save_num%100/50+5;
		flash_read(page,num*7);
		for(i=0;i<7;i++)
		{
			Record[i]=Flash.read_data[((num-1)%100%50)*7+i];
		}
}

void flash_save(uint8_t num,u16 value)
{
	u8 i;
	flash_read(0,60);
	for(i=0;i<=60;i++)
		Flash.write_data[i]=Flash.read_data[i];	
	Flash.write_data[num]=value;
	flash_write(0,60);
}

void Read_SumStr(void)
{
	u16 i;
	flash_read(7,SumMax);	
	for(i=0;i<SumMax;i++)
	{
		SumGen.Value[i]=Flash.read_data[i];
	}
}
void SumStr_Save(u16 num)
{
	u16 i;
	flash_read(7,SumMax);	
	for(i=0;i<SumMax;i++)
	{
		Flash.write_data[i]=Flash.read_data[i];
	}
	Flash.write_data[num]=SumGen.Value[num];
	flash_write(7,SumMax);
}
void SumStr_Savezero(void)
{
	u16 i;
	for(i=0;i<SumMax;i++)
	{
		Flash.write_data[i]=0;
	}
	flash_write(7,SumMax);
}

void Clear_Data(u8 type)
{
  u16 i;
  if(type==FINPT)
  {
    Fin.Users_Num=0;
    flash_save(33,Fin.Users_Num);
    Fin.UserFullFlag = 0;
    flash_save(45,Fin.UserFullFlag);
    Fin.Users_Save_Num=5;
    flash_save(37,Fin.Users_Save_Num);
    for(i=5;i<100;i++)
    {
      Flash.write_data[i]=0;  
      Fin.Value[i]=0;
    }
    flash_read(4,5);
    for(i=0;i<5;i++)
    Flash.write_data[i]=Flash.read_data[i];
    flash_write(4,210);
  }
  else if(type==PASSWORD)
  {
    for(i=2;i<30;i++)
    memset((void *)Pwd.Value[i],0,sizeof(Pwd.Value[i])); 
    Pwd.Users_Num=0;
    flash_save(34,Pwd.Users_Num);
    Pwd.Users_Save_Num=2;
    flash_save(38,Pwd.Users_Save_Num);
    Pwd.UserFullFlag=0;
    flash_save(43,Pwd.UserFullFlag);
    for(i=0;i<400;i++)
      Flash.write_data[i]=0;  
    flash_write(1,440);
  }
  else if(type==ICCARD)  
  {  
    memset((void *)IC.Value,0,sizeof(IC.Value));
    IC.Users_Num=0;
    IC.Save_Num=0;
    IC.UserFullFlag=0;
    flash_save(35,IC.Users_Num);  
    flash_save(46,IC.Save_Num);  
    flash_save(49,IC.UserFullFlag);  
    for(i=0;i<400;i++)
    Flash.write_data[i]=0;  
    flash_write(3,400);
    
  }
  else if(type==ALL)
  {
    memset((void *)Fin.Value,0,sizeof(Fin.Value));
    for(i=0;i<30;i++)
      memset((void *)Pwd.Value[i],0,sizeof(Pwd.Value[i])); 
    memset((void *)IC.Value,0,sizeof(IC.Value));
    memset((void *)Flash.write_data,0,sizeof(Flash.write_data));
    Fin.Users_Num=0;
    Fin.Users_Save_Num=5;
    Fin.UserFullFlag = 0;
    flash_save(45,Fin.UserFullFlag);
    Fin.Man_Num=0;
    Fin.Man_Save_Num=0;
    Fin.Read_Num=0;
    
    Pwd.Users_Num=0;
    Pwd.Users_Save_Num=2;
    Pwd.Man_Num=0;
    Pwd.Man_Save_Num=0;
    Pwd.Read_Num=0;
    
    IC.Users_Num=0;
    IC.Save_Num=0;
    IC.Read_Num=0;
    
    Fin.ManFullFlag = 0;
    Fin.UserFullFlag = 0;
    Pwd.ManFullFlag = 0;
    Pwd.UserFullFlag = 0;
    IC.UserFullFlag = 0;
  }
}

extern u8 ic_switch,Verify_Flag;
void Flag_Save(void)
{
	Record_Save_num = 0; //???????
	ic_switch= OFF;
	Fin.Man_Num = 0;
	Pwd.Man_Num = 0;
	Fin.Man_Save_Num = 0;
	Pwd.Man_Save_Num = 0;
	Pwd.Users_Num = 0;
	Fin.Users_Num = 0;
	Pwd.Users_Save_Num = 2;
	Fin.Users_Save_Num = 5;
	IC.Users_Num = 0;
	Record_Save_num = 0;
	Pwd.ManFullFlag = 0;
	Pwd.UserFullFlag = 0;
	Fin.UserFullFlag = 0;
	Fin.ManFullFlag = 0;
	IC.UserFullFlag = 0;
	SumGen.Man_Num = 0; 
	SumGen.Users_Num = 0;
	SumGen.Man_Save_Num = 1;
	SumGen.Users_Save_Num = 10;
	SumGen.ManFullFlag = 0;
	SumGen.UserFullFlag = 0;
	Verify_Flag = 0;
	Flash.write_data[31] = Fin.Man_Num;
	Flash.write_data[32] = Pwd.Man_Num;
	Flash.write_data[33] = Fin.Users_Num;
	Flash.write_data[34] = Pwd.Users_Num;
	Flash.write_data[35] = IC.Users_Num;
	Flash.write_data[36] = Record_Save_num;
	Flash.write_data[37] = Fin.Users_Save_Num;
	Flash.write_data[38] = Pwd.Users_Save_Num;
	Flash.write_data[39] = Fin.Man_Save_Num;
	Flash.write_data[40] = Pwd.Man_Save_Num;
	Flash.write_data[41] = ic_switch;
	Flash.write_data[42] = Pwd.ManFullFlag;
	Flash.write_data[43] = Pwd.UserFullFlag;
	Flash.write_data[44] = Fin.ManFullFlag;
	Flash.write_data[45] = Fin.UserFullFlag;
	Flash.write_data[46] = IC.UserFullFlag;
	Flash.write_data[48] = Verify_Flag;
	Flash.write_data[49] = SumGen.Man_Num;
	Flash.write_data[50] = SumGen.Users_Num;
	Flash.write_data[51] = SumGen.Man_Save_Num;
	Flash.write_data[52] = SumGen.Users_Save_Num;
	Flash.write_data[53] = SumGen.ManFullFlag;
	Flash.write_data[54] = SumGen.UserFullFlag;	
	Flash.write_data[58] = IC.Save_Num;	
	Flash.write_data[59] = Language;	
	Flash.write_data[60] = Volume_Flag;	
	flash_write(0, 60);
}

void Flag_Read(void)
{
	Fin.Man_Num = (u8)Flash.read_data[31];
	Pwd.Man_Num = (u8)Flash.read_data[32];
	Fin.Users_Num = (u8)Flash.read_data[33];
	Pwd.Users_Num = (u8)Flash.read_data[34];
	IC.Users_Num = (u8)Flash.read_data[35];
	Record_Save_num = (u8)Flash.read_data[36];
	Fin.Users_Save_Num = (u8)Flash.read_data[37];
	Pwd.Users_Save_Num = (u8)Flash.read_data[38];
	Fin.Man_Save_Num = (u8)Flash.read_data[39];
	Pwd.Man_Save_Num = (u8)Flash.read_data[40];
	ic_switch=(u8)Flash.read_data[41];
	Pwd.ManFullFlag = Flash.read_data[42];
	Pwd.UserFullFlag = Flash.read_data[43];
	Fin.ManFullFlag	=	Flash.read_data[44];
	Fin.UserFullFlag =	Flash.read_data[45];
	IC.UserFullFlag = Flash.read_data[46];
	Verify_Flag = Flash.read_data[48];
	SumGen.Man_Num = Flash.read_data[49];
	SumGen.Users_Num = Flash.read_data[50];
	SumGen.Man_Save_Num = Flash.read_data[51];
	SumGen.Users_Save_Num = Flash.read_data[52];
	SumGen.ManFullFlag = Flash.read_data[53];
	SumGen.UserFullFlag = Flash.read_data[54];		
	IC.Save_Num = Flash.read_data[58];
	Language = Flash.read_data[59];
	Volume_Flag = Flash.read_data[60];
}

extern unsigned char time_buf[4];
extern unsigned char date_buf[6];
u8  time_real[10] = {1, 9, 0, 9, 1, 0, 1, 2, 0, 7};
void Read_Status(void)
{
	OLED_Clear();
	flash_read(0, 60);
	if(Flash.read_data[30] != 0)
	{
		//关灯
		Mode = 3;
		Flag_Read();
		if(Volume_Flag == 2)
		{
			JQ8400_SetVolume(2);//高音
		}
		else if(Volume_Flag == 1)
		{
				JQ8400_SetVolume(1);//中音	
		}
		else if(Volume_Flag == 0)
		{
			 JQ8400_SetVolume(0);//低音
		}
		re_flag = 3;	
		TimingDelay_flag = 1;  //关定时器
		read_flash(FINPT);
		read_flash(ICCARD);
		read_flash(PASSWORD);
		Read_SumStr();
	}
	else
	{				
		Volume_Flag = 1;//默认为中音
		if(Volume_Flag==2)
		 JQ8400_SetVolume(2);//高
		else if(Volume_Flag==1)
		 JQ8400_SetVolume(1);//中
		else if(Volume_Flag==0)
		 JQ8400_SetVolume(0);//低
		delay_ms(10);
		Mode = 0;
		memset((void *)Flash.write_data, 0, sizeof(Flash.write_data));
		memset((void *)Flash.read_data, 0, sizeof(Flash.read_data));
		re_flag = 1;      //原先为1
		flash_empty(0);
		PS_Empty();
		Flag_Save();						
		Language = Chinese;		
		show_flag = 0;
		Remote_flag = 1;
		JQ8400_SendCmd(3,9,1,1);
		delay_ms(1500);
	}
    Reset_Error_Flag(ALL);//清空错误标志位
}





