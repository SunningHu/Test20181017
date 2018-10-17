/*=============================================================================
//File:			pwm.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			pwmÊä³ö
===============================================================================*/
#include "stm8l15x_gpio.h"
#include "stm8l15x_tim2.h"
#include "pwm.h"

#define     PWM_PORT            GPIOB
#define     PWM_PIN             GPIO_Pin_2

#define     TIM2_PERIOD                 10000
#define     TIM2_PRESCALER              1
#define     TIM2_REPTETION_COUNTER      0

u16     CCR3_Val = 0;

void    PWM_Init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    GPIO_Init(PWM_PORT, PWM_PIN, GPIO_Mode_Out_PP_Low_Slow);
    TIM2_TimeBaseInit(TIM2_Prescaler_2, TIM2_CounterMode_Up, TIM2_PERIOD);
    TIM2_OC2Init(TIM2_OCMode_PWM1, TIM2_OutputState_Enable,
               CCR3_Val, TIM2_OCPolarity_Low,TIM2_OCIdleState_Set);
    
    TIM2_OC2PreloadConfig(DISABLE);
    TIM2_ARRPreloadConfig(ENABLE);
    TIM2_CtrlPWMOutputs(ENABLE);
    TIM2_Cmd(ENABLE);
}

