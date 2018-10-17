/*=============================================================================
//File:			timer.c	
//Author:		tpcon
//Version:	V0.1
//Date:			2014/04
//Dese:			��ʱ����ʱ�ӡ���ʱ��غ���
===============================================================================*/
#include "timer.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_tim4.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_rst.h"
#include "stm8l15x_iwdg.h"

//LED�ƿ��߶���
#define		LED_PORT			GPIOB
#define		LED_PIN				GPIO_Pin_1

//Summary:	LED��ʼ��,������LED
void	LED_Init(void)
{
    u16	i;
    GPIO_Init(LED_PORT, LED_PIN, GPIO_Mode_Out_PP_Low_Slow);		//��ʼ��LED�˿�		
    
    
    for(i = 20; i > 0; i--)
    {
        GPIO_ToggleBits(LED_PORT, LED_PIN);
        delay_ms(50);				
    }		
}

//Summary:	LEDȡ������˸һ��
void	LED_Reverse(void)
{
    GPIO_ToggleBits(LED_PORT, LED_PIN);		
}


//Summary:		��ʱ��4��ʼ��,��ʱ100ms�ж�
//Parameter:	None
//Return:			None
void	TIMER4_Init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE); //ʱ��ENABLE
    //TIM4_TimeBaseInit(TIM4_Prescaler_512, 194);	//��Ƶ1M����ʱ100ms��((100ms/1000ms)*(1000000/512) - 1) = 194
    TIM4_TimeBaseInit(TIM4_Prescaler_8192, 194);	//��Ƶ16M��(0.1*(16000000/8192) - 1) = 194
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);      //���ж�
    TIM4_Cmd(ENABLE);
}

//Summary:	��ʱus
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

//Summary:	��ʱms
void 	delay_ms(u16 ms)
{
  while(ms--)
  {
    delay_us(1000);
  }
}

#define     RELOAD_VALUE    254

//Summary:		ʱ�ӳ�ʼ��
//Parameter:	None
//Return:			None
void	IWDG_Init(void)
{  
    CLK_LSICmd(ENABLE); //ʹ��LSIʱ�ӣ�IWDG���Ź���
    
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
