/*=============================================================================
//File:			dac8552.h	
//Author:		HuZhenjun 
//Version:	    V0.1
//Date:			2016/06
//Dese:			DACÊä³ö
===============================================================================*/



#ifndef __DAC8552_H__
#define __DAC8552_H__
#include "stm8l15x.h"

void dac8552_Init(void);
void Write_DAC8552(uint8_t Command, uint16_t Resdata);
void dac8552_Init(void);

#endif