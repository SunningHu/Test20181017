#ifndef _FUNCTIONDODATA_H__
#define _FUNCTIONDODATA_H__

#include "stm8l15x.h"


#define READ_MAXTIME   50      //最大读取次数
#define READ_VALIDTIME 10      //有效读取次数

typedef struct
{
    u8 u8Type;       //数据类型标志位
    u8 u8count;      //通讯计数标志位
    u8 u8ReStart;
      
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
    
    u8  u8CailPointFlag[25];   //各标定点状态  0待标定  1 成功  2失败  3死机
    
    
    u8 u8address;              //本机地址，由拨码开关决定，范围0-64
    
//////////////////////////////////////////以下为与MCGS通讯所用变量
    u16 g_bStart;              //开始标志，0为未开始，1为开始                       use
    u16 g_bCaliFinish;         //标定状态，0为准备标定，1为标定完成，2为正在标定
    u16 g_u8MeterNum;          //正在标定板号，0-48                                 use
    u16 g_u8MeterMax;          //结束标定标号，0-48
    u16 g_bCaliPoint;          //标定点，标定数据类型                               use
    u16 g_u16MacData;          //整数码值                                           use
    float g_fMacData;         //浮点数码值                                         use
    u16 g_u16ResultMeter;     //标定结果，0为未完成，1为成功，2为失败               use
/////////////////////////////////////////扩展标志位
    u16  g_productType;        //产品类型  0 S1  1 S2
    u16  g_functionType;        // 0 检测  1 标定
  
}Value;


typedef struct
{
//    float fPCorrAOut_dw[2];      //有源电流输出标定点判断范围  下限     [0]为min，[1]为max   2-5        6-9
//    float fPCorrAOut_up[2];      //有源电流输出标定点判断范围  上限     [0]为min，[1]为max   10-13      14-17
//    float fNCorrAOut_dw[2];      //无源电流输出标定点判断范围  下限     [0]为min，[1]为max   18-21      22-25
//    float fNCorrAOut_up[2];      //无源电流输出标定点判断范围  上限     [0]为min，[1]为max   26-29      30-33
//    float fCorrVOut20_dw[2];     //20mv输出标定点判断范围      下限     [0]为min，[1]为max   34-37      38-41
//    float fCorrVOut20_up[2];     //20mv输出标定点判断范围      上限     [0]为min，[1]为max   42-45      46-49
//    float fCorrVOut100_dw[2];    //100mv输出标定点判断范围     下限     [0]为min，[1]为max   50-53      54-57
//    float fCorrVOut100_up[2];    //100mv输出标定点判断范围     上限     [0]为min，[1]为max   58-61      62-65
//    float fCorrVOut_dw[2];       //V输出标定点判断范围         下限     [0]为min，[1]为max   66-69      70-73
//    float fCorrVOut_up[2];       //V输出标定点判断范围         上限     [0]为min，[1]为max   74-77      78-81
//    u16    u16CorrAIn_dw[2];      //电流测量标定点判断范围      下限     [0]为min，[1]为max   82-83      84 85
//    u16    u16CorrAIn_up[2];      //电流测量标定点判断范围      上限     [0]为min，[1]为max   86 87      88 89
//    u16    u16CorrVIn20_dw[2];    //20mv测量标定点判断范围      下限     [0]为min，[1]为max   90 91      92 93 
//    u16    u16CorrVIn20_up[2];    //20mv测量标定点判断范围      上限     [0]为min，[1]为max   94 95      96 97 
//    u16    u16CorrVIn100_dw[2];   //100mv测量标定点判断范围     下限     [0]为min，[1]为max   98 99      100 101
//    u16    u16CorrVIn100_up[2];   //100mv测量标定点判断范围     上限     [0]为min，[1]为max   102 103    104 105
//    u16    u16CorrVIn_dw[2];      //V测量标定点判断范围         下限     [0]为min，[1]为max   106 107    108 109
//    u16    u16CorrVIn_up[2];      //V测量标定点判断范围         上限     [0]为min，[1]为max   110 111    112 113

////////////////////////////////////数据稳定 前后数据差值
    u16    u16MaxOKCount;            //判断信号是否稳定最大次数           EEPROM地址  0 1

    
}Limit;




void usdelay(u16 us);
void msdelay(u16 ms);
void sdelay(u16 s);
u16 crc16(volatile u8 *puchMsg, u8 usDataLen);
void PCorrAOutDown(void);
void PCorrAOutUp(void);
void NCorrAOutDown(void);
void NCorrAOutUp(void);
void CorrVOut20Down(void);
void CorrVOut20Up(void);
void CorrVOut100Down(void);
void CorrVOut100Up(void);
void CorrVOutDown(void);
void CorrVOutUp(void);
void CorrAInDown(void);
void CorrAInUp(void);
void CorrVIn20Down(void);
void CorrVIn20Up(void);
void CorrVIn100Down(void);
void CorrVIn100Up(void);
void CorrVInDown(void);
void CorrVInUp(void);
void CorrROutDown(void);
void CorrROutUp(void);
void PCorrComm(void);

void PTestTemp(void);
void PTestAOutDown(void);
void PTestAOutUp(void);
void NTestAOutDown(void);
void NTestAOutUp(void);
void TestVOut20Down(void);
void TestVOut20Up(void);
void TestVOut100Down(void);
void TestVOut100Up(void);
void TestVOutDown(void);
void TestVOutUp(void);
void TestROutDown(void);
void TestROutUp(void);
void TestAInDown(void);
void TestAInUp(void);
void TestVIn20Down(void);
void TestVIn20Up(void);
void TestVIn100Down(void);
void TestVIn100Up(void);
void TestVInDown(void);
void TestVInUp(void);
void TestRInDown(void);
void TestRInUp(void);

float S32tofloat(s32 s32Data);
float S32tofloatCheck(s32 s32Data,u8 dotNUm);

extern const u16  uCRC16Table[];

extern void RelayInit(void);


#endif