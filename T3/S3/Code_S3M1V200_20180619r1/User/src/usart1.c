/*=============================================================================
//File:			usart1.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			串口相关函数
===============================================================================*/
#include "stm8l15x.h"
#include "stm8l15x_usart.h"
#include "timer.h"
#include <string.h>
#include "dodata.h"

u8  g_ucUsartBuf[32];		//串口缓存
u8  g_ucUsartLen = 0;		//串口接收到的数据长度

extern void	Write_Value_SOURCE(s32* ps32SValue, u8 u8Dot, s32 s32L, s32 s32H);

/********************************************************************
* Summary:    串口初始化  波特率9600，,8位数据位，1位停止位，无校验
* Parameters:
*             void
*       
* Return :    void
********************************************************************/
void	Usart_Init(void)
{
    //USART1时钟开启
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);   
    
    //使能引脚上拉电阻
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_2, ENABLE);       
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_3, ENABLE);
    
    //串口USART1波特率9600,8Bit,1 stop, 无校验，使能发送+接收
    USART_Init(USART1, 9600, USART_WordLength_8b, USART_StopBits_1, 
              USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
    
    //使能接受中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
    
    //使能串口1
    USART_Cmd(USART1, ENABLE);
}


//Summary:		串口接收数据
//Parameter:	        void
//Return:	        1成功，0 失败
u8 Usart_Receive(void)
{
    u8 i;
   static u8 clkFlag = 0;     
    for(i = 0; i < 50; i++)
    {
      if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
      {
          i = 0;
          
          if(clkFlag == 0)
          {
            clkFlag = 1;
            CLK_SYSCLKSourceSwitchCmd(ENABLE);
            CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
            CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
            while(CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI)
            {};           
          }      
            
          USART_ClearITPendingBit(USART1,USART_IT_RXNE);
          if(g_ucUsartLen < sizeof(g_ucUsartBuf))
          {
              g_ucUsartBuf[g_ucUsartLen] = USART_ReceiveData8(USART1);
              g_ucUsartLen ++;
          }
          else
          {
              g_ucUsartLen = 0;
              break;
          }
      }	
      delay_us(100);
    }
    
    if(g_ucUsartLen >= 5)
        return 1;
    else
        return 0;		
}

//Summary:		发送返回的数据
//Parameter:	        void
//Return:	        1成功，0 失败
u8 Usart_Send(void)
{  
    u8 i = 0;
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    while(g_ucUsartLen != 0)
    {				
        USART_SendData8(USART1, g_ucUsartBuf[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        i++;
        g_ucUsartLen--;
    }
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    return  1;
}

//Summary:	重置串口计数
void	ResetUsart(void)
{
    memset(g_ucUsartBuf, 0, sizeof(g_ucUsartBuf));
    g_ucUsartLen = 0;
}