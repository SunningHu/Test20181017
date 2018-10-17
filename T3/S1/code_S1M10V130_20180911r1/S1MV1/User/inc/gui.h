/*=============================================================================
//File:			gui.h	
//Author:		tpcon 
//Version:	    V0.1
//Date:			2014/04
//Dese:			“∫æß∆¡œ‘ æ
===============================================================================*/

#ifndef __GUI_H
#define __GUI_H

#include	"stm8l15x.h"

#define     OFF                 0
#define     ON                  1

#define     MEASURE             0x08
#define     LOOP24V             0x04
#define     MEASURE_MINUS       0x02
#define     MV20                0x01
#define     MV100               0x08
#define     MEASURE_RL          0x80
#define     MEASURE_RH          0x40
#define     MEASURE_SL          0x20
#define     MEASURE_SH          0x10
#define     MEASURE_MV          0x06
#define     MEASURE_V           0x04
#define     MEASURE_MA          0x0A
#define     MEASURE_HZ          0x40
#define     MEASURE_KHZ         0x41
#define     MEASURE_SSD         0x20
#define     MEASURE_R           0x80
#define     MA4_20              0x04
#define     K                   0x02
#define     E                   0x01
#define     J                   0x01
#define     T                   0x02
#define     R                   0x04
#define     B                   0x08
#define     S                   0x80
#define     N                   0x40
#define     PT100               0x20
#define     RJA                 0x10
#define     RJM                 0x01
#define     L18                 0x10
#define     L17                 0x04
#define     L16                 0x02
#define     L15                 0x01
#define     POWERFLG            0x08
#define     SOURCE              0x80
#define     SOURCE_ON           0x40
#define     SOURCE_OFF          0x20
#define     SOURCE_MINUS        0x10
#define     SOURCE_RL           0x80
#define     SOURCE_RH           0x40
#define     SOURCE_SL           0x20
#define     SOURCE_SH           0x10
#define     SOURCE_MV           0x06
#define     SOURCE_V            0x04
#define     SOURCE_MA           0x0A
#define     SOURCE_HZ           0x10
#define     SOURCE_K            0x01
#define     SOURCE_SSD          0x10
#define     SOURCE_R            0x10
#define     STEP                0x01
#define     SWEEP               0x04
#define     TIME                0x02
#define     TIME_L1             0x80
#define     TIME_L2             0x40
#define     TIME_L3             0x20
#define     TIME_L4             0x10
#define     COUNT               0x08
#define     STEP_N              0x10
#define     STEP_M              0x10
#define     SEC                 0x10

void	Write_MEASURE(u8 u8Stat);
void	Write_24VLOOP(u8 u8Stat);
void	Write_MEASURE_MINUS(u8 u8Stat);
void	Write_20mV(u8 u8Stat);
void	Write_100mV(u8 u8Stat);
void	Write_MEASURE_RL(u8 u8Stat);
void	Write_MEASURE_RH(u8 u8Stat);
void	Write_MEASURE_SL(u8 u8Stat);
void	Write_MEASURE_SH(u8 u8Stat);
void	Write_MEASURE_mV(u8 u8Stat);
void	Write_MEASURE_V(u8 u8Stat);
void	Write_MEASURE_mA(u8 u8Stat);
void	Write_MEASURE_Hz(u8 u8Stat);
void	Write_MEASURE_kHz(u8 u8Stat);
void	Write_MEASURE_SSD(u8 u8Stat);
void	Write_MEASURE_R(u8 u8Stat);
void	Write_4_20mA(u8 u8Stat);
void	Write_K(u8 u8Stat);
void	Write_E(u8 u8Stat);
void	Write_J(u8 u8Stat);
void	Write_T(u8 u8Stat);
void	Write_R(u8 u8Stat);
void	Write_B(u8 u8Stat);
void	Write_S(u8 u8Stat);
void	Write_N(u8 u8Stat);
void	Write_PT100(u8 u8Stat);
void	Write_RJA(u8 u8Stat);
void	Write_RJM(u8 u8Stat);

void	Write_BatteryLevel(u8 u8Level);
void	Write_PowerFlg(u8 u8Stat);
void	Write_SOURCE_ON_OFF(u8 u8Stat);
void	Write_SOURCE_MINUS(u8 u8Stat);
void	Write_SOURCE_RL(u8 u8Stat);
void	Write_SOURCE_RH(u8 u8Stat);
void	Write_SOURCE_SL(u8 u8Stat);
void	Write_SOURCE_SH(u8 u8Stat);
void	Write_SOURCE_mV(u8 u8Stat);
void	Write_SOURCE_V(u8 u8Stat);
void	Write_SOURCE_mA(u8 u8Stat);
void	Write_SOURCE_Hz(u8 u8Stat);
void	Write_SOURCE_kHz(u8 u8Stat);
void	Write_SOURCE_SSD(u8 u8Stat);
void	Write_SOURCE_R(u8 u8Stat);
void	Write_STEP(u8 u8Stat);
void	Write_SWEEP(u8 u8Stat);
void	Write_TIME(u8 u8Stat);
void	Write_TIME_L1(u8 u8Stat);
void	Write_TIME_L2(u8 u8Stat);
void	Write_TIME_L3(u8 u8Stat);
void	Write_TIME_L4(u8 u8Stat);
void	Write_COUNT(u8 u8Stat);
void	Write_n(u8 u8Stat);
void	Write_m(u8 u8Stat);
void	Write_s(u8 u8Stat);
void	Write_Digtial_MEASURE(u8 u8Pos, u8 u8Digtial, u8 Dot);
void	Write_Digtial_SOURCE(u8 u8Pos, u8 u8Digtial, u8 Dot);
void	Write_Digtial_TIME_COUNT_n(u8 u8Pos, u8 u8Digtial);
void	Write_Digtial_m(u8 u8Pos, u8 u8Digtial);

void    Write_SigType(u8 u8SSigType, u8 u8MSigType);
void    Write_MEASURE_Unit(u8 u8SigType);
void	Write_MEASURE_RangeFlg(u8 u8RangeFlg);
void    Write_SOURCE_Unit(u8 u8SigType);
void	Write_SOURCE_RangeFlg(u8 u8RangeFlg);
void	Write_Value_MEASURE(s16* ps16MValue, u8 u8Dot, s16 s16L, s16 s16H);
void	Write_Value_SOURCE(s16* ps16SValue, u8 u8Dot, s16 s16L, s16 s16H);
void	Write_SWEEPMode(u8 u8Sweep);
void	Write_RJMode(u8 u8RJMode);
void	Write_Value_m(u16* pu16Value, u16 u16L, u16 u16H);
void	Write_Value_TIME_COUNT_n(u16* pu16Value, u16 u16L, u16 u16H);
void	Write_Time_t(u8 u8t);

#endif
