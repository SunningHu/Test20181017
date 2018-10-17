/**
  ******************************************************************************
  * @file    main.c
  * @author  Microcontroller Division
  * @version V1.2.0
  * @date    09/2010
  * @brief   Main program body
  ******************************************************************************
**/ 

#include "main.h"

/**************************************************************************
* Summary:    时钟分频
* Parameters:
*             void
*       
* Return :    void
**************************************************************************/
void clk_init(void)
{
    //内部16MHZ的HSI时钟，1分频
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); 
    
    //时钟用外部晶震
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
    while(CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE)
    {};
}
  
void main(void)
{     
    KeyMsg keyMsg;         //按键结构体  
    u8      u8Msg; 
    
    disableInterrupts();   //关闭系统总中断  
    clk_init();                              
    eep_init();                                                    
    Dodata_PortInit();     //测量、输出端口初始化                 
    BatteryGPIO_Init();    //电池IO口初始化                    
    LedRun_Init();         //Run闪灯初始化                     
    TIMER4_Init();         //定时器初始化    
    TIMER1_Init();
    
    Usart_Init();          //串口初始化                              
    MADC_Init();           //ADC初始化        
   
    DAC8811_Init();        //DAC8811初始化                   
    InitKey();             //按键初始化  
    bsp_InitTM7705();      //开了双极性
    TPCON_LCD_Init();      //显示初始化
    Init_ViewFrame();      //开机画面
    ResetData();           //参数重置   
    InitCaliData();        //标定参数初始化  
    TIMER5_Init();         //定时器输入捕获初始化
    enableInterrupts();    //开启总中断
    while(1)
    {       

      
         //按键消息
        if(GetKeyMsg())						 										
        {
            data.u16KeyTime = 0;
            LCDBlink(OFF);
            GetKeyMsgEx(&keyMsg);
            ViewFrame(&keyMsg);
        }  
      
        u8Msg = GetMsg(MSG_100MS, 1); 
        if(u8Msg != 0) 
        {   
          if(data.u8MFlagHz == 0)
              ADC_Hzdata();        
           ADC_ResData(); 
           ADC_Cmd(ADC1, ENABLE);ADC_ChannelCmd(ADC1, ADC_Channel_1, ENABLE);ADC_ChannelCmd(ADC1, ADC_Channel_24, DISABLE);ADC_BatteryADdata();
           ADC_ChannelCmd(ADC1, ADC_Channel_24, ENABLE);ADC_ChannelCmd(ADC1, ADC_Channel_1, DISABLE);ADC_NTCdata();
           PostMsg(MSG_ADOK, 1);          
           
          //SWEEP刷屏
          if(data.u8SweepStatus == 1)
             ViewFrame(0);
          if(data.u8RemoteFlag == 1)
             SetView(VIEW_REMOTE);
          else
             if(GetView() == VIEW_MAIN)          
                SetView(VIEW_MAIN);           
           ClearMsg(MSG_100MS); 
          
        }
       // 每秒消息
        u8Msg = GetMsg(MSG_1S, 1); 
        if(u8Msg != 0) 
        {    
          Dodata_ADSample();
          PowerOff(data.u8AutoPowerEnable);       //自动关机     
          ClearMsg(MSG_1S);          
        }                              
        //每秒刷屏                  
        u8Msg = GetMsg(MSG_PAINT, 1);                                          
        if(u8Msg != 0)                                                              
        {              
          ViewFrame(0);
          ClearMsg(MSG_PAINT); 
        }    
    }

}		

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
