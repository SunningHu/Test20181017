/*=============================================================================
//File:			timer.c	
//Author:		tpcon
//Version:	V0.1
//Date:			2014/04
//Dese:			定时器及时钟、延时相关函数
===============================================================================*/
#include "timer.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_tim4.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_rst.h"
#include "stm8l15x_iwdg.h"

//LED灯口线定义
#define		LED_PORT			GPIOB
#define		LED_PIN				GPIO_Pin_1

//Summary:	LED初始化,并快闪LED
void	LED_Init(void)
{
    u16	i;
    GPIO_Init(LED_PORT, LED_PIN, GPIO_Mode_Out_PP_Low_Slow);		//初始化LED端口		
    
    
    for(i = 20; i > 0; i--)
    {
        GPIO_ToggleBits(LED_PORT, LED_PIN);
        delay_ms(50);				
    }		
}

//Summary:	LED取反，闪烁一次
void	LED_Reverse(void)
{
    GPIO_ToggleBits(LED_PORT, LED_PIN);		
}


//Summary:		定时器4初始化,定时100ms中断
//Parameter:	None
//Return:			None
void	TIMER4_Init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE); //时钟ENABLE
    //TIM4_TimeBaseInit(TIM4_Prescaler_512, 194);	//主频1M，定时100ms，((100ms/1000ms)*(1000000/512) - 1) = 194
    TIM4_TimeBaseInit(TIM4_Prescaler_8192, 194);	//主频16M，(0.1*(16000000/8192) - 1) = 194
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);      //开中断
    TIM4_Cmd(ENABLE);
}

//Summary:	延时us
void 	delay_us(u16 us)
{  
  while(us --)
  {
      __asm
      (
          "NOP \n"
          "NOP \n"
          "NOP \n"
          "NOP \n"
          "NOP \n"
          "NOP \n"
          "NOP \n"
          "NOP \n"
          "NOP \n"
          "NOP \n"
          "NOP \n"
          "NOP \n"
          "NOP \n"
     );
  };
}

//Summary:	延时ms
void 	delay_ms(u16 ms)
{
  while(ms--)
  {
    delay_us(1000);
  }
}

#define     RELOAD_VALUE    254

//Summary:		时钟初始化
//Parameter:	None
//Return:			None
void	IWDG_Init(void)
{  
    CLK_LSICmd(ENABLE); //使能LSI时钟，IWDG看门狗用
    
    // Wait for LSI clock to be ready 
    while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
    
    // Check if the system has resumed from IWDG reset 
    if (RST_GetFlagStatus(RST_FLAG_IWDGF) != RESET)
    {
      // Clear IWDGF Flag 
     RST_ClearFlag(RST_FLAG_IWDGF);
   }
     /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
    
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    
    IWDG_SetPrescaler(IWDG_Prescaler_128);
    
    /* IWDG timeout equal to 214.7 ms (the timeout may varies due to LSI frequency dispersion) */
    /* IWDG timeout = (RELOAD_VALUE + 1) * Prescaler / LSI 
                    = (254 + 1) * 128 / 38 000 
                    = 858.9 ms */
    IWDG_SetReload((uint8_t)RELOAD_VALUE);
    
    /* Reload IWDG counter */
    IWDG_ReloadCounter(); 
}
