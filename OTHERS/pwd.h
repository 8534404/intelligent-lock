#ifndef __PWD_H
#define __PWD_H	    

#include "include.h"

void Reset_Error_Flag(u8 mode);
u8 pwd_add(u8 mode,u8 Key_Value);
u8	pwd_verify(u8 mode,unsigned char *buf);
u8 Enter_pwd(u8 mode);
void Open_Way(u8 mode);

void Register_Admins(void);
void AddSum(unsigned char idtype,unsigned char usertype);
unsigned char DeleteDisSum(unsigned char idtype,unsigned char usertype);
unsigned char  DistinguishSum(unsigned char idtype);
unsigned char ClearSum(unsigned char idtype,unsigned char usertype);
unsigned char DeletenumSum(unsigned char idtype,unsigned char usertype,unsigned short delnum);


#endif
