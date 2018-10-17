#ifndef _DRIVECOM2_H__
#define _DRIVECOM2_H__

#include "stm8l15x.h"
#include <string.h>


void Usart2_Init(void);
void Usart2_Receive(void);
void Usart2_Send(u8 *buf, u8 size);
void Analysis_Usart2Buf(void);
void Usart2_SendCmd(u8 cmd);

void Cmd1000(void);
void Cmd1100(void);
void Cmd1101(void);
void Cmd1102(void);
void Cmd1103(void);
void Cmd1104(void);
void Cmd1105(void);
void Cmd1106(void);
void Cmd1107(void);
void Cmd1108(void);
void Cmd1109(void);
void Cmd1300(void);
void Cmd1301(void);
void Cmd1302(void);
void Cmd1303(void);

void Cmd12Send(float fdata);
void Cmd14Send(s16 s16data);

void Usart2_SendCmd_Test(u8 cmd);



#endif

