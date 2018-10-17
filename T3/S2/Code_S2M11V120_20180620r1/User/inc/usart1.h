/*=============================================================================
//File:			usart1.h	
//Author:		tpcon 
//Version:	    V0.1
//Date:			2014/04
//Dese:			串口相关函数头文件
===============================================================================*/
#ifndef __USART1_H
#define __USART1_H

void	Usart_Init(void);
u8 		Usart_Receive(void);
u8 		Usart_Send(void);
void	ResetUsart(void);

#endif
