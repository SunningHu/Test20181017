#ifndef _FUNCTIONDODATA_H__
#define _FUNCTIONDODATA_H__

#include "stm8l15x.h"




#define RELAY_1_PORT    GPIOE
#define RELAY_1_PIN        GPIO_Pin_5     // 子板1电源供电
#define RELAY_2_PORT    GPIOE
#define RELAY_2_PIN        GPIO_Pin_4     // 子板2电源供电
#define RELAY_3_PORT    GPIOE
#define RELAY_3_PIN        GPIO_Pin_3     // 子板3电源供电
#define RELAY_4_PORT    GPIOE
#define RELAY_4_PIN        GPIO_Pin_2     // 子板4电源供电
#define RELAY_5_PORT    GPIOE
#define RELAY_5_PIN        GPIO_Pin_1     // 子板5电源供电
#define RELAY_6_PORT    GPIOE
#define RELAY_6_PIN        GPIO_Pin_0     // 子板6电源供电
#define RELAY_7_PORT    GPIOC
#define RELAY_7_PIN        GPIO_Pin_2     // 子板7电源供电
#define RELAY_8_PORT    GPIOC
#define RELAY_8_PIN        GPIO_Pin_3     // 子板8电源供电
#define RELAY_9_PORT    GPIOC
#define RELAY_9_PIN        GPIO_Pin_4     // 子板9电源供电
#define RELAY_10_PORT   GPIOC
#define RELAY_10_PIN       GPIO_Pin_5     // 子板10电源供电
#define RELAY_11_PORT   GPIOC
#define RELAY_11_PIN       GPIO_Pin_6     // 子板11电源供电
#define RELAY_12_PORT   GPIOC
#define RELAY_12_PIN       GPIO_Pin_7     // 子板12电源供电




#define RELAY_1_OPEN         GPIO_SetBits(RELAY_1_PORT, RELAY_1_PIN)      //打开子板1电源供电
#define RELAY_1_CLOSE        GPIO_ResetBits(RELAY_1_PORT, RELAY_1_PIN)        //关闭子板1电源供电
#define RELAY_2_OPEN         GPIO_SetBits(RELAY_2_PORT, RELAY_2_PIN)      //打开子板2电源供电
#define RELAY_2_CLOSE        GPIO_ResetBits(RELAY_2_PORT, RELAY_2_PIN)        //关闭子板2电源供电
#define RELAY_3_OPEN         GPIO_SetBits(RELAY_3_PORT, RELAY_3_PIN)      //打开子板3电源供电
#define RELAY_3_CLOSE        GPIO_ResetBits(RELAY_3_PORT, RELAY_3_PIN)        //关闭子板3电源供电
#define RELAY_4_OPEN         GPIO_SetBits(RELAY_4_PORT, RELAY_4_PIN)      //打开子板4电源供电
#define RELAY_4_CLOSE        GPIO_ResetBits(RELAY_4_PORT, RELAY_4_PIN)        //关闭子板4电源供电
#define RELAY_5_OPEN         GPIO_SetBits(RELAY_5_PORT, RELAY_5_PIN)      //打开子板5电源供电
#define RELAY_5_CLOSE        GPIO_ResetBits(RELAY_5_PORT, RELAY_5_PIN)        //关闭子板5电源供电
#define RELAY_6_OPEN         GPIO_SetBits(RELAY_6_PORT, RELAY_6_PIN)      //打开子板6电源供电
#define RELAY_6_CLOSE        GPIO_ResetBits(RELAY_6_PORT, RELAY_6_PIN)        //关闭子板6电源供电
#define RELAY_7_OPEN         GPIO_SetBits(RELAY_7_PORT, RELAY_7_PIN)      //打开子板7电源供电
#define RELAY_7_CLOSE        GPIO_ResetBits(RELAY_7_PORT, RELAY_7_PIN)        //关闭子板7电源供电
#define RELAY_8_OPEN         GPIO_SetBits(RELAY_8_PORT, RELAY_8_PIN)      //打开子板8电源供电
#define RELAY_8_CLOSE        GPIO_ResetBits(RELAY_8_PORT, RELAY_8_PIN)        //关闭子板8电源供电
#define RELAY_9_OPEN         GPIO_SetBits(RELAY_9_PORT, RELAY_9_PIN)      //打开子板9电源供电
#define RELAY_9_CLOSE        GPIO_ResetBits(RELAY_9_PORT, RELAY_9_PIN)        //关闭子板9电源供电
#define RELAY_10_OPEN        GPIO_SetBits(RELAY_10_PORT, RELAY_10_PIN)    //打开子板10电源供电
#define RELAY_10_CLOSE       GPIO_ResetBits(RELAY_10_PORT, RELAY_10_PIN)      //关闭子板10电源供电
#define RELAY_11_OPEN        GPIO_SetBits(RELAY_11_PORT, RELAY_11_PIN)    //打开子板11电源供电
#define RELAY_11_CLOSE       GPIO_ResetBits(RELAY_11_PORT, RELAY_11_PIN)      //关闭子板11电源供电
#define RELAY_12_OPEN        GPIO_SetBits(RELAY_12_PORT, RELAY_12_PIN)    //打开子板12电源供电
#define RELAY_12_CLOSE       GPIO_ResetBits(RELAY_12_PORT, RELAY_12_PIN)      //关闭子板12电源供电




typedef struct
{
    u8 u8Type;       //数据类型标志位
    u8 u8count;      //通讯计数标志位
      
}Flag;

typedef struct
{
    float fPCorrAOut[2];       //有源电流输出标定值，[0]为下限，[1]为上限
    float fNCorrAOut[2];       //无源电流输出标定值，[0]为下限，[1]为上限
    float fCorrVOut20[2];      //20mV电压输出标定值，[0]为下限，[1]为上限
    float fCorrVOut100[2];     //100mV电压输出标定值，[0]为下限，[1]为上限
    float fCorrVOut[2];        //V电压输出标定值，[0]为下限，[1]为上限
    float fCorrROut[2];        //欧姆输出标定值，[0]为下限，[1]为上限
    u16 u16CorrAIn[2];         //电流测量标定值，[0]为下限，[1]为上限
    u16 u16CorrVIn20[2];       //20mV电压测量标定值，[0]为下限，[1]为上限
    u16 u16CorrVIn100[2];      //100mV电压测量标定值，[0]为下限，[1]为上限
    u16 u16CorrVIn[2];         //V电压测量标定值，[0]为下限，[1]为上限
    u16 u16CorrRIn[2];         //V电压测量标定值，[0]为下限，[1]为上限
    
    s32 s32TestTemp;           //冷端温度。
    s32 s32PTestAOut[2];       //有源电流输出检测值，[0]为下限，[1]为上限
    s32 s32NTestAOut[2];       //无源电流输出检测值，[0]为下限，[1]为上限
    s32 s32TestVOut20[2];      //20mV电压输出检测值，[0]为下限，[1]为上限
    s32 s32TestVOut100[2];     //100mV电压输出检测值，[0]为下限，[1]为上限
    s32 s32TestVOut[2];        //V电压输出检测值，[0]为下限，[1]为上限
    s32 s32TestROut[2];        //欧姆输出检测值，[0]为下限，[1]为上限
    s32 s32TestAIn[2];         //电流测量检测值，[0]为下限，[1]为上限
    s32 s32TestVIn20[2];       //20mV电压测量检测值，[0]为下限，[1]为上限
    s32 s32TestVIn100[2];      //100mV电压测量检测值，[0]为下限，[1]为上限
    s32 s32TestVIn[2];         //V电压测量检测值，[0]为下限，[1]为上限
    s32 s32TestRIn[2];         //V电压测量检测值，[0]为下限，[1]为上限    
    
    
    
    
    u8 u8address;              //本机地址，由拨码开关决定，范围0-64
    
//////////////////////////////////////////以下为与MCGS通讯所用变量
    u16 g_bMode;               //手/自动，0为手动，1为自动
    u16 g_bStart;              //开始标志，0为未开始，1为开始                       use
    u16 g_bStop;               //急停标志，0为不急停，1为急停
    u16 g_bCaliFinish;         //标定状态，0为准备标定，1为标定完成，2为正在标定
    u16 g_u8MeterNum;          //正在标定板号，0-48                                 use
    u16 g_u8MeterMax;          //结束标定标号，0-48
    u16 g_bCaliPoint;          //标定点，标定数据类型                               use
    u16 g_u16MacData;          //整数码值                                           use
    float g_fMacData;         //浮点数码值                                         use
    u16 g_u16ReadTimes;       //读取次数，1-30次
    u16 g_u16ResultMeter;     //标定结果，0为未完成，1为成功，2为失败               use
/////////////////////////////////////////扩展标志位
    u16  g_productType;        //产品类型  0 S1  1 S2
    u16  g_functionType;        // 0 检测  1 标定
  
}Value;


typedef struct
{
    float fPCorrAOut_dw[2];      //有源电流输出标定点判断范围  下限     [0]为min，[1]为max   2-5        6-9
    float fPCorrAOut_up[2];      //有源电流输出标定点判断范围  上限     [0]为min，[1]为max   10-13      14-17
    float fNCorrAOut_dw[2];      //无源电流输出标定点判断范围  下限     [0]为min，[1]为max   18-21      22-25
    float fNCorrAOut_up[2];      //无源电流输出标定点判断范围  上限     [0]为min，[1]为max   26-29      30-33
    float fCorrVOut20_dw[2];     //20mv输出标定点判断范围      下限     [0]为min，[1]为max   34-37      38-41
    float fCorrVOut20_up[2];     //20mv输出标定点判断范围      上限     [0]为min，[1]为max   42-45      46-49
    float fCorrVOut100_dw[2];    //100mv输出标定点判断范围     下限     [0]为min，[1]为max   50-53      54-57
    float fCorrVOut100_up[2];    //100mv输出标定点判断范围     上限     [0]为min，[1]为max   58-61      62-65
    float fCorrVOut_dw[2];       //V输出标定点判断范围         下限     [0]为min，[1]为max   66-69      70-73
    float fCorrVOut_up[2];       //V输出标定点判断范围         上限     [0]为min，[1]为max   74-77      78-81
    u16    u16CorrAIn_dw[2];      //电流测量标定点判断范围      下限     [0]为min，[1]为max   82-83      84 85
    u16    u16CorrAIn_up[2];      //电流测量标定点判断范围      上限     [0]为min，[1]为max   86 87      88 89
    u16    u16CorrVIn20_dw[2];    //20mv测量标定点判断范围      下限     [0]为min，[1]为max   90 91      92 93 
    u16    u16CorrVIn20_up[2];    //20mv测量标定点判断范围      上限     [0]为min，[1]为max   94 95      96 97 
    u16    u16CorrVIn100_dw[2];   //100mv测量标定点判断范围     下限     [0]为min，[1]为max   98 99      100 101
    u16    u16CorrVIn100_up[2];   //100mv测量标定点判断范围     上限     [0]为min，[1]为max   102 103    104 105
    u16    u16CorrVIn_dw[2];      //V测量标定点判断范围         下限     [0]为min，[1]为max   106 107    108 109
    u16    u16CorrVIn_up[2];      //V测量标定点判断范围         上限     [0]为min，[1]为max   110 111    112 113

////////////////////////////////////数据稳定 前后数据差值
    u16    u16MaxOKCount;            //判断信号是否稳定最大次数           EEPROM地址  0 1

    
}Limit;




void usdelay(u16 us);
void msdelay(u16 ms);
void sdelay(u16 s);
u16 crc16(volatile u8 *puchMsg, u8 usDataLen);




extern const u16  uCRC16Table[];

extern void RelayInit(void);


#endif