/*=============================================================================
//File:			battery.h	
//Author:		tpcon   
//Version:	    V0.1
//Date:			2014/04
//Dese:			��ع���
===============================================================================*/

#ifndef __BATTERY_H
#define __BATTERY_H

#include	"stm8l15x.h"

//���ٺ����ٳ���־
#define     CHARGE_FAST             0
#define     CHARGE_SLOW             1

//��ص�ѹ��ֵ 
#define     BATTERY_LEVEL0   2949
#define     BATTERY_LEVEL1   3031
#define     BATTERY_LEVEL2   3113
#define     BATTERY_LEVEL3   3277

void	Battery_Init(void);
void	PowerOff(void);
void	LowBatteryPowerOff(void);
void    ViewBatteryLevel(u16 u16BatteryVoltage);

#endif
