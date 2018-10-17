#ifndef _DRIVECOM1_H__
#define _DRIVECOM1_H__

#include "stm8l15x.h"
#include <string.h>


#define USART3_RS485_H   GPIO_SetBits(GPIOF, GPIO_Pin_4)
#define USART3_RS485_L   GPIO_ResetBits(GPIOF, GPIO_Pin_4)

void Usart3_Init(void);
u8 Usart3_Receive(void);
void Usart3_Send(void);
u8 Analysis_Usart1Buf(void);
void ResetUsart3(void);
void ReceiveUsart1Data(void);


#endif