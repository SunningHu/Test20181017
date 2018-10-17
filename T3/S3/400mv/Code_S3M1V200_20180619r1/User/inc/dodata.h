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
#define     SIGTYPE_MA              3
#define     SIGTYPE_MV              2
#define     SIGTYPE_V               1
#define     SIGTYPE_Res             4
#define     SIGTYPE_Cu50            5 
#define     SIGTYPE_PT100           6
#define     SIGTYPE_K               7
#define     SIGTYPE_E               8
#define     SIGTYPE_J               9
#define     SIGTYPE_T               10
#define     SIGTYPE_R               11
#define     SIGTYPE_B               12
#define     SIGTYPE_S               13
#define     SIGTYPE_N               14
#define     SIGTYPE_Hz              15
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

#define     RJMODE_NONE     2           //冷端不显示
#define     RJMODE_AUTO     0           //冷端自动
#define     RJMODE_MAN      1           //冷端手动

#define     SWEEP_NONE      0
#define     SWEEP_SWEEP     1
#define     SWEEP_STEP      2

#define     PROGNM_NONE     0
#define     PROGNM_M        1
#define     PROGNM_N        2

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
    u8      u8RemoteFlag;                   //远程模式
    u8      u8VLoopFflag;
    u8      u8Hzfalg;                       //Hz测量 插拔显示1  进中断判断变量
    u8      u8MFlagHz;                       //Hz测量大小档   0 大档 1小档
    u8      u8ProgMode;                     //m,n顺序    
    u8      u8SRLHDot;                      //SOURCE RANGE DOT
    u8      u8SSigType;                     //SOURCE 信号类型
    u8      u8MSigType;                     //MEASURE 信号类型
    u8      u8SourceEnable;                 //SOURCE 开关标志
    u8      u8MeasureEnable;                //MEASURE 开关标志
    u8      u8SweepMode;                    //波形模式
    u8      u8RJMode;                       //冷端模式
    u8      u8TempType;                     //摄氏度和华氏度切换
    u8      u8SweepStatus;                  //SWEEP状态
    u8      u8MRLHDot;                      //MEASURE RANGE DOT
    u8      u8MRangeEnable;                 //MEASURE RANGE显示   
    u8      u8LCDBlight;                    //背光标志位
    u8      u8MHoldEnable;                  //保持标志位
    u8      u8AutoPowerEnable;              //自动关机标志
    u8      u8DotNum;                       //小数个数
    u8      u8KeyLongUpF;                    //按键长安抬起标志
    u16     u16SourceHz;                    // HZ输出
    s32     s32SourceValue;                 //SOURCE输出值
    s32     s32SRangeValue;                 //SOURCE RANGE值
    s32     s32MeasureValue;                //MEASURE信号值
    s32     s32MRangeValue;                 //MEASURE RANGE值
    s32     s32MRL;                         //MEASURE RL
    s32     s32MRH;                         //MEASURE RH
    s32     s32MSL;                         //MEASURE SL
    s32     s32MSH;                         //MEASURE SH   
    s32     s32SRL;                         //SOURCE RL
    s32     s32SRH;                         //SOURCE RH
    s32     s32SSL;                         //SOURCE SL
    s32     s32SSH;                         //SOURCE SH    
    s32     s32RJAuto;                      //自动冷端温度
    s32     s32RJSet;                       //手动冷端温度
    u16     u16KeyTime;                     //无按键时间计时器    
    u16     u16ADdata;                      //AD采集中间过度16位码值   
    float  fSourceValue;
}Data;

typedef struct _tagKeyConfig
{
   u8      u8WriteStutus;                  //输入状态
   u8      u8KeyPlaces;                    //位数
   u8      u8DotPlace;                     //小数点位置
   u8      u8KeyPN;                        //正负
}Edit;

typedef struct  _tagADCode
{
    u16     u16mACode;                      //mA码值   
    u16     u16VCode;                       //V码值 
    u16     u16mVCode;                      //mV码值 
    u16     u16NTCCode;                     //NTC码值
    u16     u16BatteryCode;                 //电池电压码值
    u16     u16ResCode1;
    s32     s32HzValue;
    s32     s32HzCode;
    s32     u16RRef;
}ADCode;

DODATA_EXT      Data        data ;             //参数
DODATA_EXT      ADCode      adCode ;           //AD采样码值
DODATA_EXT      Edit        edit ;             //输入配置
DODATA_EXT      s32         g_s32MSigRange[SIGTYPE_END][3];
DODATA_EXT      s32  g_s32SSigRange[SIGTYPE_END][3];
DODATA_EXT      s32         g_s32MSigRange1[SIGTYPE_END][3];
DODATA_EXT      s32  g_s32SSigRange1[SIGTYPE_END][3];
DODATA_EXT      s32         g_s32MSigRange2[SIGTYPE_END][3];
DODATA_EXT      s32  g_s32SSigRange2[SIGTYPE_END][3];


void    DoSource(u8 u8SigType, s32 s32SourceValue);
s32    DoMeasure(u8 u8SigType);
void   Dodata_PortInit(void);
s16     GetRJTemp(void);
u8     GetRJMode(void);
void     DoSourceCali(u8 u8CaliSig, u16* pu16CalPwm);
s16     GetRJAuto(void);
void    Dodata_1S(void);
void    Dodata_ADSample(void);
void    SetMSigPort(u8 u8SigType);
void    ResetData(void);
void    SetSSigPort(u8 u8SigType);
float Dodata_s32(u8 code[],u8 length, u8 u8PN, u8 u8Dot);
void    ADC_ResData(void);
float SeekTable_PT100_S(float s16Tempe, const float* fTable, u16 u16TblLen);
float SeekTable_Cu50_S(float s16Tempe, const float* fTable, u16 u16TblLen);
#endif
