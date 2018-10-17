/*=============================================================================
//File:			measure.h	
//Author:		tpcon  
//Version:	    V0.1
//Date:			2014/04
//Dese:			测量
===============================================================================*/

#ifndef __MEASURE_H
#define __MEASURE_H

#include	"stm8l15x.h"

#define     SIG_MA          0
#define     SIG_20MV        1
#define     SIG_100MV       2
#define     SIG_V           3

#define     MAX_BUF                 15      //ADC采样10次取平均值

void	MADC_Init(void);
void    adc_handle(void);
void    ADC_BatteryADdata(void);
void    ADC_NTCdata(void);
void     ADC_Hzdata(void);

#endif
