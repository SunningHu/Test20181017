#ifndef _MAIN_H__
#define _MAIN_H__
#include "stm8l15x.h"
#include "DriveCom1.h"
#include "DriveCom2.h"
#include "DriveCom3.h"
#include "FunctionFont.h"
#include "FunctionDoData.h"
#include "DriveEEprom.h"

#define KEY_1_PORT   GPIOB
#define KEY_1_PIN    GPIO_Pin_7  //拨码开关1
#define KEY_2_PORT   GPIOB
#define KEY_2_PIN    GPIO_Pin_6  //拨码开关2
#define KEY_3_PORT   GPIOB
#define KEY_3_PIN    GPIO_Pin_5  //拨码开关3
#define KEY_4_PORT   GPIOB
#define KEY_4_PIN    GPIO_Pin_4  //拨码开关4
#define KEY_5_PORT   GPIOB
#define KEY_5_PIN    GPIO_Pin_3  //拨码开关5
#define KEY_6_PORT   GPIOB
#define KEY_6_PIN    GPIO_Pin_2  //拨码开关6

#define LED_RUN_PORT    GPIOA
#define LED_RUN_PIN     GPIO_Pin_2   // 运行灯
#define RELAY_M_PORT    GPIOC
#define RELAY_M_PIN     GPIO_Pin_6   // 测量继电器
#define RELAY_S_PORT    GPIOC
#define RELAY_S_PIN     GPIO_Pin_7   // 输出继电器
#define RELAY_MA_PORT   GPIOE
#define RELAY_MA_PIN    GPIO_Pin_7   // 标定欧姆继电器
#define RELAY_W_PORT    GPIOE
#define RELAY_W_PIN     GPIO_Pin_6   // 预留继电器



#define RELAY_M_OPEN         GPIO_ResetBits(RELAY_M_PORT, RELAY_M_PIN)    //打开测量继电器
#define RELAY_M_CLOSE        GPIO_SetBits(RELAY_M_PORT, RELAY_M_PIN)      //关闭测量继电器
#define RELAY_S_OPEN         GPIO_ResetBits(RELAY_S_PORT, RELAY_S_PIN)    //打开输出继电器
#define RELAY_S_CLOSE        GPIO_SetBits(RELAY_S_PORT, RELAY_S_PIN)      //关闭输出继电器
#define RELAY_MA_OPEN        GPIO_ResetBits(RELAY_MA_PORT, RELAY_MA_PIN)  //打开标定欧姆继电器
#define RELAY_MA_CLOSE       GPIO_SetBits(RELAY_MA_PORT, RELAY_MA_PIN)    //关闭标定欧姆继电器
#define RELAY_W_OPEN         GPIO_ResetBits(RELAY_W_PORT, RELAY_W_PIN)    //打开预留继电器
#define RELAY_W_CLOSE        GPIO_SetBits(RELAY_W_PORT, RELAY_W_PIN)      //关闭预留继电器




u8 time_1ms_flag;
u8 time_10ms_flag;
u8 time_100ms_flag;
u8 time_1000ms_flag;
u8 time_10s_flag;
u8 time_interval_flag;


void clk_init(void);
void tim4_init(void);
void GPIO_init(void);
void RelayInit(void);
void GetAddress(void);
void CalPoint(void);
void CodeRelayTab(void);
void LimitInit(void);

u8 g_u8Address;

extern Flag flag;
extern Value value;
extern Limit limit;
#endif
