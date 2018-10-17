/*=============================================================================
//File:			DAC8811.h	
//Author:		huzhenjun 
//Version:	        V0.1
//Date:			2017/06
//Dese:			DAC8811相关函数头文件
===============================================================================*/

#ifndef __DAC8811_H
#define __DAC8811_H

#include	"stm8l15x.h"

void DAC8811_Init(void);
void DAC8811_OUTV(float fData);
void DAC8811_Write(u16 u16Data);
#endif