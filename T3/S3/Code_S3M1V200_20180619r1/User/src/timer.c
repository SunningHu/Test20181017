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
#include "stm8l15x_tim1.h"
#include "stm8l15x_tim5.h"

//LED�ƿ��߶���
#define		LED_PORT			GPIOD
#define		LED_PIN				GPIO_Pin_7



/*******************************************************
* Summary:    ���еƳ�ʼ��,������LED	
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
void	LedRun_Init(void)
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



/*******************************************************
* Summary:    ��ʱ��4��ʼ��,��ʱ1ms�ж�
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
void	TIMER4_Init(void)
{
      //Ĭ��TIM4ʱ���ǹرյģ�����Ҫ�ȿ���TIM4��ʱ�ӣ�֮����ܶ�TIM4�Ĵ������г�ʼ�����м�˳�򲻿ɷ�
      CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE); 
    

    /* 1��TIM4�Ǹ�8λ�Ķ�ʱ�������������Դﵽ255
       2��ϵͳʱ����16MHZ�ģ���ʱ��ʹ�õ���ϵͳʱ��Դ
       3����õ�1ms��ʱ��������Ҫ��ʱ�ӷ�Ƶ
       4��ϵͳʱ��128��Ƶ�����Եõ� 24000000 / 128 = 187.5K��ʱ��
       5��ʱ���趨ֵ��ʽΪ (X * 1000) / 125000 = 1;
       6���õ�X = 125��Ȼ�� X - 1 �͵õ�186�����ֵ�������ǶԶ�ʱ
         �������ֵ   */

    TIM4_TimeBaseInit(TIM4_Prescaler_128,124);
    
    //���TIM4��ʱ�������־λ
    TIM4_ClearFlag(TIM4_FLAG_Update);
    
    //ʹ��TIM4��ʱ������ж�
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);
    
    //����TIM4��ʱ��
    TIM4_Cmd(ENABLE); 
}



/*******************************************************
* Summary:    ��ʱ��1��ʼ��,��ʱ1ms�ж�
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
void	TIMER1_Init(void)
{
    //����TIM1��ʱ��
    GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE); 
   
    TIM1_DeInit();
       
    TIM1_TimeBaseInit(0,TIM1_CounterMode_Up,48000,0);
    
    TIM1_OC2Init(TIM1_OCMode_Toggle, TIM1_OutputState_Disable,TIM1_OutputNState_Enable,
               0, TIM1_OCPolarity_Low,TIM1_OCNPolarity_Low,TIM1_OCIdleState_Set,TIM1_OCNIdleState_Set);

    TIM1_OC2PreloadConfig(DISABLE);
    TIM1_ARRPreloadConfig(DISABLE);
    TIM1_CtrlPWMOutputs(ENABLE);
    TIM1_Cmd(ENABLE);
   
   
//    TIM1->CCMR2 &= ~0x70;
//    TIM1->CCMR2 |= 0x30;
//    TIM1->CCR2H = 0;
//    TIM1->CCR2L = 0;
////    TIM1->ARRH = 0xBB;
////    TIM1->ARRL = 0x80;
//    
//    TIM1->CCER1 = 0x30;
//    
// //   TIM1->CR1 |= 0x01;
//    TIM1->EGR |= 0x01;
//    TIM1->BKR |= 0x80;
    
    
    
    
    //ʹ��TIM1��ʱ������ж�
//    TIM1_ITConfig(TIM1_IT_Update, ENABLE);
//    
//    //����TIM1��ʱ��
//    TIM1_Cmd(DISABLE); 
    
//    TIM1_DeInit();
//    TIM1->PSCRH  &= 0x00;
//    TIM1->PSCRL |= 0x01;  
//    
//    TIM1->ARRH  = 0x2b;
//    TIM1->ARRL  = 0x33;    
//    TIM1->CCR3H = 0x15;
//    TIM1->CCR3L = 0x99;    
//    TIM1->CCMR3 |= 0x60; //PWM 1
//    TIM1->CR1 &= 0x8f;    
// //   TIM1->CCMR3 |= 0x08;
//    TIM1->CCER2 |= 0x01; //OC3�������ģʽ 
////    TIM1->OISR |= 0x10;  //����״̬����ߵ�ƽ
//    TIM1->CR1 |= 0x01;   //ʹ�ܼ�����
//    TIM1->BKR |= 0x80;   //�������
    
      
}


 


/*******************************************************
* Summary:     ��ʱ��5���벶���ʼ��
* Parameters:
*              void
*       
* Return :     void
*******************************************************/
void	TIMER5_Init(void)
{
    //TIM5  _CH2  ������������
    GPIO_Init(GPIOE, GPIO_Pin_0, GPIO_Mode_In_PU_No_IT);
    
    //����TIM5ʱ��
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM5, ENABLE); 
    TIM5_DeInit();
    
 //-----------------------------��ʽһ���⺯��---------------------------------  
    
   //  TIM5->PSCR  = 0x01;
     TIM5_ICInit(TIM5_Channel_2,TIM5_ICPolarity_Falling,TIM5_ICSelection_DirectTI,TIM5_ICPSC_DIV4,0x00);
    
    //���TIM5��ʱ�������־λ
     TIM5_ClearFlag(TIM5_FLAG_CC2);
     TIM5_ITConfig(TIM5_IT_CC2,ENABLE);
    //ʹ��TIM4��ʱ������ж�
    TIM5_CCxCmd(TIM5_Channel_2, ENABLE);
    TIM5->IER |= 0x01;
    
    //����TIM5��ʱ��
    TIM5_Cmd(ENABLE); 
 // --------------------------------------------------------------------------
 
    
 /*-----------------------------��ʽ�����Ĵ���----------------------------- */   
    
//    TIM5->CCER1 &= ~0x10; 
//    TIM5->CCMR2 = 0x01;  //ͨ��������Ϊ����
//    TIM5->CCMR2 &= 0x0f;  //���˲�
//    TIM5->CCER1 |= 0x20;  //�����½���
//    TIM5->CCMR2 |= 0x08;  //���벶��Ԥ��Ƶ��  4���½�����һ��
//    
//    TIM5->PSCR  = 0x01;
//    
// //   TIM5->CCER1 |= 0x10;   
//    TIM5->IER |= 0x04;
// //   TIM5->SR1 &= ~0x04;
////    TIM5->SR2 &= ~0x0e
////    TIM5->CCER1 &= 0xef;  //�ر����벶��
//    
//      TIM5->CCER1 |= 0x10; 
//      TIM5->CR1 |= 0x01;
//     enableInterrupts();
      
/* -------------------------------------------------------------------------*/
    
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
