/*=============================================================================
//File:			battery.h	
//Author:		tpcon   
//Version:	    V0.1
//Date:			2014/04
//Dese:			电池管理
===============================================================================*/

#ifndef __BATTERY_H
#define __BATTERY_H

#include	"stm8l15x.h"

//快速和慢速充电标志
#define     CHARGE_FAST             0
#define     CHARGE_SLOW             1

//电池电压码值 
#define     BATTERY_LEVEL0   2949
#define     BATTERY_LEVEL1   3031
#define     BATTERY_LEVEL2   3113
#define     BATTERY_LEVEL3   3277

void	Battery_Init(void);
void	PowerOff(void);
void	LowBatteryPowerOff(void);
void    ViewBatteryLevel(u16 u16BatteryVoltage);

#endif
