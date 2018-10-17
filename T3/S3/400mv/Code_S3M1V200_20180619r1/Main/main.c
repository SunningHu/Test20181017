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
* Summary:    ʱ�ӷ�Ƶ
* Parameters:
*             void
*       
* Return :    void
**************************************************************************/
void clk_init(void)
{
    //�ڲ�16MHZ��HSIʱ�ӣ�1��Ƶ
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); 
    
    //ʱ�����ⲿ����
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
    while(CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE)
    {};
}
  
void main(void)
{     
    KeyMsg keyMsg;         //�����ṹ��  
    u8      u8Msg; 
    
    disableInterrupts();   //�ر�ϵͳ���ж�  
    clk_init();                              
    eep_init();                                                    
    Dodata_PortInit();     //����������˿ڳ�ʼ��                 
    BatteryGPIO_Init();    //���IO�ڳ�ʼ��                    
    LedRun_Init();         //Run���Ƴ�ʼ��                     
    TIMER4_Init();         //��ʱ����ʼ��    
    TIMER1_Init();
    
    Usart_Init();          //���ڳ�ʼ��                              
    MADC_Init();           //ADC��ʼ��        
   
    DAC8811_Init();        //DAC8811��ʼ��                   
    InitKey();             //������ʼ��  
    bsp_InitTM7705();      //����˫����
    TPCON_LCD_Init();      //��ʾ��ʼ��
    Init_ViewFrame();      //��������
    ResetData();           //��������   
    InitCaliData();        //�궨������ʼ��  
    TIMER5_Init();         //��ʱ�����벶���ʼ��
    enableInterrupts();    //�������ж�
    while(1)
    {       

      
         //������Ϣ
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
           
          //SWEEPˢ��
          if(data.u8SweepStatus == 1)
             ViewFrame(0);
          if(data.u8RemoteFlag == 1)
             SetView(VIEW_REMOTE);
          else
             if(GetView() == VIEW_MAIN)          
                SetView(VIEW_MAIN);           
           ClearMsg(MSG_100MS); 
          
        }
       // ÿ����Ϣ
        u8Msg = GetMsg(MSG_1S, 1); 
        if(u8Msg != 0) 
        {    
          Dodata_ADSample();
          PowerOff(data.u8AutoPowerEnable);       //�Զ��ػ�     
          ClearMsg(MSG_1S);          
        }                              
        //ÿ��ˢ��                  
        u8Msg = GetMsg(MSG_PAINT, 1);                                          
        if(u8Msg != 0)                                                              
        {              
          ViewFrame(0);
          ClearMsg(MSG_PAINT); 
        }    
    }

}		

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
