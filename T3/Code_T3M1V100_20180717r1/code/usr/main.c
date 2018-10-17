/***************************************************************
��Ʒ���ƣ�ȫ�Զ��궨ϵͳ
�������ţ�T2
������Ա��Ǯ��ˬ(Ƕ��ʽ���)���춬������(��λ�����)�����(Ӳ��)���ܿ�(�ṹ)
����ʱ�䣺2016.12.20-2017.05.01
***************************************************************/
 


/*****************************************
���б��������õ�ͷ�ļ���ȫ������main.h����
����
*****************************************/
#include "main.h"
#include "math.h"



//Summary:    ��ʱ�ӳ�ʼ��
void clk_init(void)
{
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);  
}


//Summary:    ��ʱ��TIM4��ʼ��
void tim4_init(void)
{
    //Ĭ��TIM4ʱ���ǹرյģ�����Ҫ�ȿ���TIM4��ʱ�ӣ�֮����ܶ�TIM4�Ĵ������г�ʼ�����м�˳�򲻿ɷ�
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE); 
    //1��TIM4�Ǹ�8λ�Ķ�ʱ�������������Դﵽ255
    //2��ϵͳʱ����16MHZ�ģ���ʱ��ʹ�õ���ϵͳʱ��Դ
    //3����õ�1ms��ʱ��������Ҫ��ʱ�ӷ�Ƶ
    //4��ϵͳʱ��128��Ƶ�����Եõ� 16000000 / 128 = 125K��ʱ��
    //5��ʱ���趨ֵ��ʽΪ (X * 1000) / 125000 = 1;
    //6���õ�X = 125��Ȼ�� X - 1 �͵õ�124�����ֵ�������ǶԶ�ʱ�������ֵ
    TIM4_TimeBaseInit(TIM4_Prescaler_128, 124);
    //���TIM4��ʱ�������־λ
    TIM4_ClearFlag(TIM4_FLAG_Update);
    //ʹ��TIM4��ʱ������ж�
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);
    //�������ж�
    enableInterrupts();
    //����TIM4��ʱ��
    TIM4_Cmd(ENABLE);     
}

//Summary:    GPIO�ڳ�ʼ��
void GPIO_init(void)
{
    GPIO_Init(LED_RUN_PORT, LED_RUN_PIN, GPIO_Mode_Out_PP_High_Slow); //  ���е�
    
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
    
    //���뿪��
    GPIO_Init(KEY_1_PORT, KEY_1_PIN, GPIO_Mode_In_PU_No_IT); //  key1
    GPIO_Init(KEY_2_PORT, KEY_2_PIN, GPIO_Mode_In_PU_No_IT); //  key2
    GPIO_Init(KEY_3_PORT, KEY_3_PIN, GPIO_Mode_In_PU_No_IT); //  key3
    GPIO_Init(KEY_4_PORT, KEY_4_PIN, GPIO_Mode_In_PU_No_IT); //  key4
    GPIO_Init(KEY_5_PORT, KEY_5_PIN, GPIO_Mode_In_PU_No_IT); //  key5
    GPIO_Init(KEY_6_PORT, KEY_6_PIN, GPIO_Mode_In_PU_No_IT); //  key6

}

/*******************************************************
* Summary:    �̵�����ʼ��
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







//Summary:    �õ�������ַ
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
   //�ϵ��ʼ��
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
