#ifndef __FLASH_H
#define __FLASH_H			    
#include "include.h" 

void flash_read(uint8_t pagination,uint16_t num);
void flash_write(uint8_t pagination,uint16_t num);
void flash_empty(u8 mode);
void Time_save(u8 time[5],u16 type,u8 mode);
void Record_Save(u8 Time[5],u8 mode,u16 type);
void Record_Read(u16 num);
void flash_save(uint8_t num,u16 value);
void Read_SumStr(void);
void SumStr_Save(u16 num);
void SumStr_Savezero(void);
void Clear_Data(u8 type);
void Flag_Save(void);
void Flag_Read(void);
void save_flash(u8 type,u8 num);

void Read_Status(void);

void read_flash(u8 type);

void SumStr_Save(u16 num);

#endif
















