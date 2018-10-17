/*=============================================================================
//File:			dodata.h	
//Author:		tpcon  
//Version:	    V0.1
//Date:			2014/04
//Dese:			数据处理
===============================================================================*/

#ifndef __DODATA_H
#define __DODATA_H

#include	"stm8l15x.h"

#ifdef   _DODATA_C_
    #define DODATA_EXT
#else
    #define DODATA_EXT    extern
#endif

#define     SIGTYPE_24VLOOP         0
#define     SIGTYPE_MA              1
#define     SIGTYPE_4_20MA          2
#define     SIGTYPE_V               3
#define     SIGTYPE_20MV            4
#define     SIGTYPE_100MV           5
#define     SIGTYPE_K               6
#define     SIGTYPE_E               7
#define     SIGTYPE_J               8
#define     SIGTYPE_T               9
#define     SIGTYPE_R               10
#define     SIGTYPE_B               11
#define     SIGTYPE_S               12
#define     SIGTYPE_N               13
#define     SIGTYPE_PT100           14
#define     SIGTYPE_PT              15
#define     SIGTYPE_END             16

//RANGE序号
#define     RANGE_NONE      0
#define     RANGE_RL        1
#define     RANGE_RH        2
#define     RANGE_SL        3
#define     RANGE_SH        4
#define     RANGE_END       5

#define     SIGVALUE        0           //信号值
#define     RANGEVALUE      1           //转换值

#define     RJMODE_NONE     0           //冷端不显示
#define     RJMODE_AUTO     1           //冷端自动
#define     RJMODE_MAN      2           //冷端手动

#define     SWEEP_NONE      0
#define     SWEEP_SWEEP     1
#define     SWEEP_STEP      2

#define     TIME_T1         0
#define     TIME_T2         1
#define     TIME_T3         2
#define     TIME_T4         3
#define     TIME_TEND       4
#define     TIME_SWEEP      5
#define     TIME_SWEEPSTEP1 6
#define     TIME_SWEEPSTEP2 7
#define     TIME_SWEEPSTEP3 8
#define     TIME_SWEEPSTEP4 9

#define     MAMODE_MA       0       //有源电流
#define     MAMODE_SINK     1       //无源电流



typedef struct _tagConfig  
{
    u8      u8SinkFlag;                      //无源 1 是 0否
    u8      u8RemoteFlag;                   //远程模式 1是 0否
    s16      s16Ri;                        //电阻输出电流
    u8      u8SRLHDot;                      //SOURCE RANGE DOT
    u8      u8SSigType;                     //SOURCE 信号类型
    u8      u8MSigType;                     //MEASURE 信号类型
    u8      u8SourceEnable;                 //SOURCE 开关标志
    u8      u8MeasureEnable;                //MEASURE 开关标志
    u8      u8SweepMode;                    //波形模式
    u8      u8RJMode;                       //冷端模式
    u8      u8SweepStatus;                  //SWEEP状态
    u8      u8MRLHDot;                      //MEASURE RANGE DOT
    u8      u8MRangeEnable;                 //MEASURE RANGE显示
    s16     s16SourceValue;                 //SOURCE输出值
    s16     s16SRangeValue;                 //SOURCE RANGE值
    s16     s16MeasureValue;                //MEASURE信号值
    s16     s16MRangeValue;                 //MEASURE RANGE值
    s16     s16MRL;                         //MEASURE RL
    s16     s16MRH;                         //MEASURE RH
    s16     s16MSL;                         //MEASURE SL
    s16     s16MSH;                         //MEASURE SH   
    s16     s16SRL;                         //SOURCE RL
    s16     s16SRH;                         //SOURCE RH
    s16     s16SSL;                         //SOURCE SL
    s16     s16SSH;                         //SOURCE SH    
    s16     s16RJAuto;                      //自动冷端温度
    s16     s16RJSet;                       //手动冷端温度
    u16     u16KeyTime;                     //无按键时间计时器      
    u16     u16ADdata;
}Data;


typedef struct  _tagADCode
{
    u16     u16mACode;                      //mA码值   
    u16     u16VCode;                       //V码值 
    u16     u16mVCode;                      //mV码值 
    u16     u16NTCCode;                     //NTC码值
    u16     u16BatteryCode;                 //电池电压码值
    u16     u16RCode;                       //电阻两端电压
    u16     u16RRef;                        //电流电压
}ADCode;

DODATA_EXT      Data        data ;             //参数
DODATA_EXT      ADCode      adCode ;           //AD采样码值
DODATA_EXT      s16         g_s16MSigRange[SIGTYPE_END][3];
DODATA_EXT      s16  g_s16SSigRange[SIGTYPE_END][3];



void    DoSource(u8 u8SigType, s16 s16SourceValue);
s16    DoMeasure(u8 u8SigType);
void   Dodata_PortInit(void);
s16     GetRJTemp(void);
u8  GetRJMode(void);
void     DoSourceCali(u8 u8CaliSig, u16* pu16CalPwm);
s16     GetRJAuto(void);
void    Dodata_1S(void);
void    Dodata_ADSample(void);
void    SetMSigPort(u8 u8SigType);
void ResetData(void);
void    SetSSigPort(u8 u8SigType);

#endif
