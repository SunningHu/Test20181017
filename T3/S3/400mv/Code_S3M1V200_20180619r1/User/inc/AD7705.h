/*=============================================================================
//File:			TM7705.h	
//Author:		胡镇俊
//Version:	        V0.1
//Date:			2016/07
//Dese:			TM7705 驱动模块(2通道带PGA的16位ADC)软件SPI
===============================================================================*/

#ifndef _AD7705_H
#define _AD7705_H

#include	"stm8l15x.h"

extern void 	delay_ms(u16 ms);
void bsp_InitTM7705(void);
void TM7705_CalibSelf(unsigned char  _ch);
void TM7705_SytemCalibZero(unsigned char  _ch);
void TM7705_SytemCalibFull(unsigned char  _ch);
uint16_t TM7705_ReadAdc(uint8_t _ch);
void bsp_InitTM7705(void);
void TM7705_WriteReg(unsigned char _RegID, unsigned long _RegValue);
unsigned long TM7705_ReadReg(unsigned char _RegID);
unsigned int TM7705_Dodata(unsigned int readdata);
void bsp_DelayMS(unsigned int n);
#endif

/*******************************************************************************************************/
