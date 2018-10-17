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

#define     MEASURE             0x02        //0
#define     LOOP24V             0x80        //7
#define     MEASURE_MINUS       0x01        //0
#define     MEASURE_m           0x20        //13
#define     MEASURE_A           0x80        //13
#define     MEASURE_V           0x40        //13
#define     MEASURE_K           0x04        //14
#define     MEASURE_Hz          0x08        //14
#define     MEASURE_Pt100       0x04        //13
#define     MEASURE_Cu50        0x08        //13
#define     MEASURE_P11         0x10        //13
#define     MEASURE_F           0x20        //7
#define     MEASURE_R           0x10        //7
#define     MEASURE_RJA         0x01        //13
#define     MEASURE_RJM         0x02        //13
#define     MEASURE_SSD         0x40        //7
#define     MEASURE_RL          0x10        //0
#define     MEASURE_RH          0x20        //0
#define     MEASURE_SL          0x40        //0
#define     MEASURE_SH          0x80        //0
#define     MEASURE_1           0x01        //7
#define     HOLD                0x04        //0
#define     AUTOPOWER           0x08        //0
#define     CELL_N              0x08        //7
#define     CELL_1              0x40        //6
#define     CELL_2              0x20        //6
#define     CELL_3              0x02        //7
#define     CELL_4              0x04        //7
#define     CELL_5              0x80        //6
#define     MEASURE_K1          0x08        //8
#define     MEASURE_K2          0x80        //8
#define     MEASURE_K3          0x08        //9
#define     MEASURE_K4          0x80        //9
#define     MEASURE_K5          0x08        //10
#define     MEASURE_K6          0x04        //8
#define     MEASURE_K7          0x40        //8
#define     MEASURE_K8          0x04        //9
#define     MEASURE_K9          0x40        //9
#define     MEASURE_K10         0x04        //10
#define     MEASURE_K11         0x02        //8
#define     MEASURE_K12         0x20        //8
#define     MEASURE_K13         0x02        //9
#define     MEASURE_K14         0x20        //9
#define     MEASURE_K15         0x02        //10
#define     MEASURE_K16         0x01        //8
#define     MEASURE_K17         0x10        //8
#define     MEASURE_K18         0x01        //9
#define     MEASURE_K19         0x10        //9
#define     MEASURE_K20         0x01        //10
#define     MEASURE_K21         0x20        //12
#define     MEASURE_K22         0x02        //12
#define     MEASURE_K23         0x20        //11
#define     MEASURE_K24         0x02        //11
#define     MEASURE_K25         0x20        //10
#define     MEASURE_K26         0x40        //12
#define     MEASURE_K27         0x04        //12
#define     MEASURE_K28         0x40        //11
#define     MEASURE_K29         0x04        //11
#define     MEASURE_K30         0x40        //10
#define     MEASURE_K31         0x80        //12
#define     MEASURE_K32         0x08        //12
#define     MEASURE_K33         0x80        //11
#define     MEASURE_K34         0x08        //11
#define     MEASURE_K35         0x80        //10

#define     HG                  0x01        //14

#define     SOURCE              0x01        //35
#define     SOURCE_1            0x80        //34
#define     SOURCE_ON           0x10        //34
#define     SOURCE_OFF          0x20        //34
#define     SOURCE_MINUS        0x40        //34
#define     SOURCE_RL           0x02        //35
#define     SOURCE_RH           0x04        //35
#define     SOURCE_SL           0x08        //35
#define     SOURCE_SH           0x02        //14
#define     SOURCE_m            0x02        //23
#define     SOURCE_V            0x04        //23
#define     SOURCE_A            0x08        //23
#define     SOURCE_P12          0x01        //23
#define     SOURCE_HZ           0x08        //24
#define     SOURCE_K            0x08        //25
#define     SOURCE_SSD          0x40        //14
#define     SOURCE_R            0x10        //14
#define     SOURCE_SINK         0x80        //14
#define     SOURCE_F            0x20        //14
#define     SOURCE_RJA          0x10        //22
#define     SOURCE_RJM          0x20        //22
#define     SOURCE_Pt100        0x40        //22
#define     SOURCE_Cu50         0x80        //22

#define     SOURCE_L1            0x08       //15
#define     SOURCE_L2            0x08       //18
#define     SOURCE_L3            0x80       //18
#define     SOURCE_L4            0x08       //19
#define     SOURCE_L5            0x80       //19
#define     SOURCE_L6            0x04       //15
#define     SOURCE_L7            0x04       //18
#define     SOURCE_L8            0x40       //18
#define     SOURCE_L9            0x04       //19
#define     SOURCE_L10           0x40       //19
#define     SOURCE_L11           0x02       //15
#define     SOURCE_L12           0x02       //18
#define     SOURCE_L13           0x20       //18
#define     SOURCE_L14           0x02       //19
#define     SOURCE_L15           0x20       //19
#define     SOURCE_L16           0x01       //15
#define     SOURCE_L17           0x01       //18
#define     SOURCE_L18           0x10       //18
#define     SOURCE_L19           0x01       //19
#define     SOURCE_L20           0x10       //19
#define     SOURCE_L21           0x02       //22
#define     SOURCE_L22           0x20       //21
#define     SOURCE_L23           0x02       //21
#define     SOURCE_L24           0x20       //20
#define     SOURCE_L25           0x02       //20
#define     SOURCE_L26           0x04       //22
#define     SOURCE_L27           0x40       //21
#define     SOURCE_L28           0x04       //21
#define     SOURCE_L29           0x40       //20
#define     SOURCE_L30           0x04       //20
#define     SOURCE_L31           0x08       //22
#define     SOURCE_L32           0x80       //21
#define     SOURCE_L33           0x08       //21
#define     SOURCE_L34           0x80       //20
#define     SOURCE_L35           0x08       //20

#define     STEP                0x08        //29
#define     SWEEP               0x02        //29
#define     TIME                0x01        //29
#define     TIME_L1             0x10        //28
#define     TIME_L2             0x20        //28
#define     TIME_L3             0x40        //28
#define     TIME_L4             0x80        //28
#define     COUNT               0x04        //29
#define     STEP_N              0x08        //28
#define     STEP_M              0x08        //26
#define     SEC                 0x08        //27

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
void	Write_MEASURE_RJA(u8 u8Stat);
void	Write_MEASURE_RJM(u8 u8Stat);
void	Write_SOURSE_RJA(u8 u8Stat);
void	Write_SOURSE_RJM(u8 u8Stat);
void	Write_Line(u8 u8Stat);
void	Write_SOURCE_TempType(u8 u8Stat);
void	Write_MEASURE_TempType(u8 u8Stat);

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
void	Write_Value_MEASURE(s32* ps32MValue, u8 u8Dot, s32 s32L, s32 s32H);
void	Write_Value_SOURCE(s32* ps32SValue, u8 u8Dot, s32 s32L, s32 s32H);
void	Write_SWEEPMode(u8 u8Sweep);
void	Write_RJModeS(u8 u8RJMode);
void	Write_RJModeM(u8 u8RJMode);
void	Write_Value_m(u16* pu16Value, u16 u16L, u16 u16H);
void	Write_Value_TIME_COUNT_n(u16* pu16Value, u16 u16L, u16 u16H);
void	Write_Time_t(u8 u8t);
void	Write_BatteryLine(u8 u8Stat);

void	Edit_Value_MEASURE(u8 editNum[], u8 u8KeyPlace, u8 dot);
void	Edit_Value_SOURCE(u8 editNum[], u8 u8KeyPlace, u8 dot);
 s32    Write_Value_fSOURCE(float* psfSValue, u8 fDot,u8 u8Dot, s32 s32L, s32 s32H);
 void	Write_HOLD_ON_OFF(u8 u8Stat);
 s32  Write_UP_SOURCE(s32* psSValue, u8 fDot,u8 u8Dot, s32 s32L, s32 s32H);
 s32  Write_DOWN_SOURCE(s32* psSValue, u8 fDot,u8 u8Dot, s32 s32L, s32 s32H);
 void	Write_AUTOPOWER(u8 u8Stat);
#endif
