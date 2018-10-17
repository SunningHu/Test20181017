#ifndef _DRIVECOM1_H__
#define _DRIVECOM1_H__

#include "stm8l15x.h"
#include <string.h>


#define USART1_RS485_H   GPIO_SetBits(GPIOC, GPIO_Pin_4)
#define USART1_RS485_L   GPIO_ResetBits(GPIOC, GPIO_Pin_4)

void Usart1_Init(void);
u8 Usart1_Receive(void);
void Usart1_Send(void);
u8 Analysis_Usart1Buf(void);
void ResetUsart1(void);
void ReceiveUsart1Data(void);
void SendUsart1Data(void);
void SendManageData(void);
void ReceiveManageData(void);
void SendCheckData(void);
void SendCailData(void);
 void ResetCheckParam(void);


#endif