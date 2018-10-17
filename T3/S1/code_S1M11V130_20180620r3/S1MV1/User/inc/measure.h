/*=============================================================================
//File:			measure.h	
//Author:		tpcon  
//Version:	    V0.1
//Date:			2014/04
//Dese:			����
===============================================================================*/

#ifndef __MEASURE_H
#define __MEASURE_H

#include	"stm8l15x.h"

#define     SIG_MA          0
#define     SIG_20MV        1
#define     SIG_100MV       2
#define     SIG_V           3

#define     MAX_BUF                 10      //ADC����10��ȡƽ��ֵ

void	Measure_ADC_Init(void);
void	SignalPort_Init(void);
void	SwitchSignal(u8 u8SigType);

#endif
