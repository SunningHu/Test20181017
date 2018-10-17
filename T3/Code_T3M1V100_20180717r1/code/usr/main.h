#ifndef _MAIN_H__
#define _MAIN_H__
#include "stm8l15x.h"
#include "DriveCom1.h"
#include "FunctionFont.h"
#include "FunctionDoData.h"
#include "DriveEEprom.h"

#define KEY_1_PORT   GPIOB
#define KEY_1_PIN    GPIO_Pin_7  //���뿪��1
#define KEY_2_PORT   GPIOB
#define KEY_2_PIN    GPIO_Pin_6  //���뿪��2
#define KEY_3_PORT   GPIOB
#define KEY_3_PIN    GPIO_Pin_5  //���뿪��3
#define KEY_4_PORT   GPIOB
#define KEY_4_PIN    GPIO_Pin_4  //���뿪��4
#define KEY_5_PORT   GPIOB
#define KEY_5_PIN    GPIO_Pin_3  //���뿪��5
#define KEY_6_PORT   GPIOB
#define KEY_6_PIN    GPIO_Pin_2  //���뿪��6

#define LED_RUN_PORT    GPIOA
#define LED_RUN_PIN     GPIO_Pin_2   // ���е�





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


u8 g_u8Address;

extern Flag flag;
extern Value value;
extern Limit limit;
#endif
