/**
  ******************************************************************************
  * @file    main.c
  * @author  Microcontroller Division
  * @version V1.2.0
  * @date    09/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */
 
/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l15x_clk.h"
#include "timer.h"
#include "key.h"
#include "usart1.h"
#include "stm8l15x_usart.h"
#include "battery.h"
#include "measure.h"
#include "lcd.h"
#include "stm8l15x_i2c.h"
#include "gui.h"
#include "viewMain.h"
#include "pwm.h"
#include "dodata.h"
#include "viewFrame.h"
#include "domsg.h"
#include "adcmd.h"
#include "stm8l15x_iwdg.h"

/**
  * @brief main entry point.
  * @par Parameters None
  * @retval void None
  * @par Required preconditions: None
  */
void main(void)
{     
    KeyMsg keyMsg;
    u8      u8Msg;;
    
    disableInterrupts();    
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);       //16M/1 = 16M主频
    Battery_Init();     //电池口线初始化
    LED_Init();         //闪灯
    TIMER4_Init();      //定时器初始化 
    Usart_Init();       //串口初始化
    PWM_Init();         //PWM初始化
    Measure_ADC_Init(); //ADC初始化
    InitCaliData();     //标定参数初始化
    Dodata_PortInit();  //口线初始化
    InitKey();          //按键初始化    
    TPCON_LCD_Init();   //显示初始化
    Init_ViewFrame();   //开机画面
    IWDG_Init();        //IWDG初始化
    ResetData();        //参数重置
    enableInterrupts();
    
    while(1)
    {    
        IWDG_ReloadCounter();     //喂狗  
        

        
        //每秒消息
        u8Msg = GetMsg(MSG_1S, 1); 
        if(u8Msg != 0) 
        {
          
          PowerOff();       //自动关机
          SetLCDBright(data.u16KeyTime);
          ClearMsg(MSG_1S);          
        }
              
        //按键消息
        if(GetKeyMsg())						 										
        {
            data.u16KeyTime = 0;
            LCDBlink(OFF);
            GetKeyMsgEx(&keyMsg);
            ViewFrame(&keyMsg);	   
        }  
        
        //SWEEP刷屏
        u8Msg = GetMsg(MSG_100MS, 1);                                          
        if(u8Msg != 0 )                                                              
        {
          if(data.u8SweepStatus == 1)
          {
                ViewFrame(0);   
          }
          else
          {
            if(data.u8RemoteFlag == 1)
             SetView(VIEW_REMOTE);
           else
             if(GetView() == VIEW_MAIN)          
                SetView(VIEW_MAIN);          
          }
          ClearMsg(MSG_100MS);                                                         
        }
        else        //每秒刷屏
        {
          u8Msg = GetMsg(MSG_PAINT, 0);                                          
          if(u8Msg != 0)                                                              
          {           
            ViewFrame(0);
            ClearMsg(MSG_PAINT); 
          }
        }
    }

}		

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
