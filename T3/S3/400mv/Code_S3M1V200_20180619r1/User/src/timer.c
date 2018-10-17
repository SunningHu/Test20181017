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
#include "stm8l15x_tim1.h"
#include "stm8l15x_tim5.h"

//LED灯口线定义
#define		LED_PORT			GPIOD
#define		LED_PIN				GPIO_Pin_7



/*******************************************************
* Summary:    运行灯初始化,并快闪LED	
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
void	LedRun_Init(void)
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



/*******************************************************
* Summary:    定时器4初始化,定时1ms中断
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
void	TIMER4_Init(void)
{
      //默认TIM4时钟是关闭的，则需要先开启TIM4的时钟，之后才能对TIM4寄存器进行初始化，切记顺序不可反
      CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE); 
    

    /* 1、TIM4是个8位的定时器，最大计数可以达到255
       2、系统时钟是16MHZ的，定时器使用的是系统时钟源
       3、想得到1ms的时基，则需要将时钟分频
       4、系统时钟128分频，可以得到 24000000 / 128 = 187.5K的时钟
       5、时钟设定值公式为 (X * 1000) / 125000 = 1;
       6、得到X = 125，然后将 X - 1 就得到186，这个值就是我们对定时
         器的填充值   */

    TIM4_TimeBaseInit(TIM4_Prescaler_128,124);
    
    //清楚TIM4定时器溢出标志位
    TIM4_ClearFlag(TIM4_FLAG_Update);
    
    //使能TIM4定时器溢出中断
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);
    
    //启动TIM4定时器
    TIM4_Cmd(ENABLE); 
}



/*******************************************************
* Summary:    定时器1初始化,定时1ms中断
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
void	TIMER1_Init(void)
{
    //开启TIM1的时钟
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
    
    
    
    
    //使能TIM1定时器溢出中断
//    TIM1_ITConfig(TIM1_IT_Update, ENABLE);
//    
//    //启动TIM1定时器
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
//    TIM1->CCER2 |= 0x01; //OC3配置输出模式 
////    TIM1->OISR |= 0x10;  //空闲状态输出高电平
//    TIM1->CR1 |= 0x01;   //使能计数器
//    TIM1->BKR |= 0x80;   //打开主输出
    
      
}


 


/*******************************************************
* Summary:     定时器5输入捕获初始化
* Parameters:
*              void
*       
* Return :     void
*******************************************************/
void	TIMER5_Init(void)
{
    //TIM5  _CH2  配置上拉输入
    GPIO_Init(GPIOE, GPIO_Pin_0, GPIO_Mode_In_PU_No_IT);
    
    //开启TIM5时钟
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM5, ENABLE); 
    TIM5_DeInit();
    
 //-----------------------------方式一：库函数---------------------------------  
    
   //  TIM5->PSCR  = 0x01;
     TIM5_ICInit(TIM5_Channel_2,TIM5_ICPolarity_Falling,TIM5_ICSelection_DirectTI,TIM5_ICPSC_DIV4,0x00);
    
    //清楚TIM5定时器溢出标志位
     TIM5_ClearFlag(TIM5_FLAG_CC2);
     TIM5_ITConfig(TIM5_IT_CC2,ENABLE);
    //使能TIM4定时器溢出中断
    TIM5_CCxCmd(TIM5_Channel_2, ENABLE);
    TIM5->IER |= 0x01;
    
    //启动TIM5定时器
    TIM5_Cmd(ENABLE); 
 // --------------------------------------------------------------------------
 
    
 /*-----------------------------方式二：寄存器----------------------------- */   
    
//    TIM5->CCER1 &= ~0x10; 
//    TIM5->CCMR2 = 0x01;  //通道二配置为输入
//    TIM5->CCMR2 &= 0x0f;  //无滤波
//    TIM5->CCER1 |= 0x20;  //捕获下降沿
//    TIM5->CCMR2 |= 0x08;  //输入捕获预分频器  4个下降沿算一次
//    
//    TIM5->PSCR  = 0x01;
//    
// //   TIM5->CCER1 |= 0x10;   
//    TIM5->IER |= 0x04;
// //   TIM5->SR1 &= ~0x04;
////    TIM5->SR2 &= ~0x0e
////    TIM5->CCER1 &= 0xef;  //关闭输入捕获
//    
//      TIM5->CCER1 |= 0x10; 
//      TIM5->CR1 |= 0x01;
//     enableInterrupts();
      
/* -------------------------------------------------------------------------*/
    
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
