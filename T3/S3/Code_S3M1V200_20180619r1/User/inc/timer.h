/*=============================================================================
//File:			timer.h	
//Author:		tpcon 
//Version:	V0.1
//Date:			2014/04
//Dese:			��ʱ����ʱ�ӡ���ʱ��غ���ͷ�ļ�
===============================================================================*/

#ifndef __TIMER_H
#define __TIMER_H

#include	"stm8l15x.h"

void 	delay_ms(u16 ms);
void 	delay_us(u16 us);
void	TIMER4_Init(void);
void	LedRun_Init(void);
void	LED_Reverse(void);
void	IWDG_Init(void);
void	TIMER1_Init(void);
void	TIMER5_Init(void);

#endif
