#ifndef _DRIVECOM3_H__
#define _DRIVECOM3_H__

#include "stm8l15x.h"
#include <string.h>






void Usart3_Init(void);
void Usart3_Receive(void);
void Usart3_Send(u8 *buf, u8 size);
void Analysis_Usart3Buf(void);
void Usart3_SendCmd_Test(u8 cmd);
float StringToFloatVmA(char* str);
float StringToFloatmV(char* str);





#endif