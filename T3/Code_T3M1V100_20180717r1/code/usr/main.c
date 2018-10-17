/***************************************************************
产品名称：全自动标定系统
开发代号：T2
开发人员：钱泽爽(嵌入式软件)，徐冬、胡镇俊(上位机软件)，赵李丹(硬件)，周凯(结构)
开发时间：2016.12.20-2017.05.01
***************************************************************/
 


/*****************************************
所有变量，引用的头文件，全部都在main.h里面
定义
*****************************************/
#include "main.h"
#include "math.h"



//Summary:    主时钟初始化
void clk_init(void)
{
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);  
}


//Summary:    定时器TIM4初始化
void tim4_init(void)
{
    //默认TIM4时钟是关闭的，则需要先开启TIM4的时钟，之后才能对TIM4寄存器进行初始化，切记顺序不可反
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE); 
    //1、TIM4是个8位的定时器，最大计数可以达到255
    //2、系统时钟是16MHZ的，定时器使用的是系统时钟源
    //3、想得到1ms的时基，则需要将时钟分频
    //4、系统时钟128分频，可以得到 16000000 / 128 = 125K的时钟
    //5、时钟设定值公式为 (X * 1000) / 125000 = 1;
    //6、得到X = 125，然后将 X - 1 就得到124，这个值就是我们对定时器的填充值
    TIM4_TimeBaseInit(TIM4_Prescaler_128, 124);
    //清楚TIM4定时器溢出标志位
    TIM4_ClearFlag(TIM4_FLAG_Update);
    //使能TIM4定时器溢出中断
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);
    //开启总中断
    enableInterrupts();
    //启动TIM4定时器
    TIM4_Cmd(ENABLE);     
}

//Summary:    GPIO口初始化
void GPIO_init(void)
{
    GPIO_Init(LED_RUN_PORT, LED_RUN_PIN, GPIO_Mode_Out_PP_High_Slow); //  运行灯
    
    GPIO_Init(RELAY_1_PORT, RELAY_1_PIN, GPIO_Mode_Out_OD_HiZ_Fast); 
    GPIO_Init(RELAY_2_PORT, RELAY_2_PIN, GPIO_Mode_Out_OD_HiZ_Fast); 
    GPIO_Init(RELAY_3_PORT, RELAY_3_PIN, GPIO_Mode_Out_OD_HiZ_Fast);
    GPIO_Init(RELAY_4_PORT, RELAY_4_PIN, GPIO_Mode_Out_OD_HiZ_Fast); 
    GPIO_Init(RELAY_5_PORT, RELAY_5_PIN, GPIO_Mode_Out_OD_HiZ_Fast); 
    GPIO_Init(RELAY_6_PORT, RELAY_6_PIN, GPIO_Mode_Out_OD_HiZ_Fast); 
    GPIO_Init(RELAY_7_PORT, RELAY_7_PIN, GPIO_Mode_Out_OD_HiZ_Fast);
    GPIO_Init(RELAY_8_PORT, RELAY_8_PIN, GPIO_Mode_Out_OD_HiZ_Fast); 
    GPIO_Init(RELAY_9_PORT, RELAY_9_PIN, GPIO_Mode_Out_OD_HiZ_Fast); 
    GPIO_Init(RELAY_10_PORT, RELAY_10_PIN, GPIO_Mode_Out_OD_HiZ_Fast); 
    GPIO_Init(RELAY_11_PORT, RELAY_11_PIN, GPIO_Mode_Out_OD_HiZ_Fast);
    GPIO_Init(RELAY_12_PORT, RELAY_12_PIN, GPIO_Mode_Out_OD_HiZ_Fast);     
    
    //拨码开关
    GPIO_Init(KEY_1_PORT, KEY_1_PIN, GPIO_Mode_In_PU_No_IT); //  key1
    GPIO_Init(KEY_2_PORT, KEY_2_PIN, GPIO_Mode_In_PU_No_IT); //  key2
    GPIO_Init(KEY_3_PORT, KEY_3_PIN, GPIO_Mode_In_PU_No_IT); //  key3
    GPIO_Init(KEY_4_PORT, KEY_4_PIN, GPIO_Mode_In_PU_No_IT); //  key4
    GPIO_Init(KEY_5_PORT, KEY_5_PIN, GPIO_Mode_In_PU_No_IT); //  key5
    GPIO_Init(KEY_6_PORT, KEY_6_PIN, GPIO_Mode_In_PU_No_IT); //  key6

}

/*******************************************************
* Summary:    继电器初始化
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
void RelayInit(void)
{
//    RELAY_1_CLOSE;
//    RELAY_2_CLOSE;
//    RELAY_3_CLOSE;
//    RELAY_4_CLOSE;
//    RELAY_5_CLOSE;  
//    RELAY_6_CLOSE;
//    RELAY_7_CLOSE;
//    RELAY_8_CLOSE;
//    RELAY_9_CLOSE;
//    RELAY_10_CLOSE;
//    RELAY_11_CLOSE;  
//    RELAY_12_CLOSE;
 
}







//Summary:    得到本机地址
void GetAddress(void)
{
    u8 i = 0;
    if(GPIO_ReadInputDataBit(KEY_1_PORT, KEY_1_PIN) == RESET) 
    {
        i = i + 1;
    }
    if(GPIO_ReadInputDataBit(KEY_2_PORT, KEY_2_PIN) == RESET) 
    {
        i = i + 2;
    }
    if(GPIO_ReadInputDataBit(KEY_3_PORT, KEY_3_PIN) == RESET) 
    {
        i = i + 4;
    }
    if(GPIO_ReadInputDataBit(KEY_4_PORT, KEY_4_PIN) == RESET) 
    {
        i = i + 8;
    }
    if(GPIO_ReadInputDataBit(KEY_5_PORT, KEY_5_PIN) == RESET) 
    {
        i = i + 16;
    }
    if(GPIO_ReadInputDataBit(KEY_6_PORT, KEY_6_PIN) == RESET) 
    {
        i = i + 32;
    }
    value.u8address = i;
} 




/*****************************************************************************************************************/
void main(void)                                                          
{
   //上电初始化
    clk_init();
    GPIO_init();    
    tim4_init();
    RelayInit();
    Usart3_Init();
//    GetAddress();
    value.u8address = 19;
    while (1)
    { 
       ;
       sdelay(2);
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
