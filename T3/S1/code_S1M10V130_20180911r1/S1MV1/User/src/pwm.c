/*=============================================================================
//File:			pwm.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			pwmÊä³ö
===============================================================================*/
#include "pwm.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_tim1.h"

#define     PWM_PORT            GPIOD
#define     PWM_PIN             GPIO_Pin_5

#define     TIM1_PERIOD                 10000
#define     TIM1_PRESCALER              1
#define     TIM1_REPTETION_COUNTER      0

u16     CCR3_Val = 0;

void    PWM_Init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
    GPIO_Init(PWM_PORT, PWM_PIN, GPIO_Mode_Out_PP_Low_Slow);
    TIM1_TimeBaseInit(TIM1_PRESCALER, TIM1_CounterMode_Up, TIM1_PERIOD, TIM1_REPTETION_COUNTER);
    TIM1_OC3Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               CCR3_Val, TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
    
    TIM1_OC3PreloadConfig(DISABLE);
    TIM1_ARRPreloadConfig(ENABLE);
    TIM1_CtrlPWMOutputs(ENABLE);
    TIM1_Cmd(ENABLE);
}

