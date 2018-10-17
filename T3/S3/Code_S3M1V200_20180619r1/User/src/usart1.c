/*=============================================================================
//File:			usart1.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			������غ���
===============================================================================*/
#include "stm8l15x.h"
#include "stm8l15x_usart.h"
#include "timer.h"
#include <string.h>
#include "dodata.h"

u8  g_ucUsartBuf[32];		//���ڻ���
u8  g_ucUsartLen = 0;		//���ڽ��յ������ݳ���

extern void	Write_Value_SOURCE(s32* ps32SValue, u8 u8Dot, s32 s32L, s32 s32H);

/********************************************************************
* Summary:    ���ڳ�ʼ��  ������9600��,8λ����λ��1λֹͣλ����У��
* Parameters:
*             void
*       
* Return :    void
********************************************************************/
void	Usart_Init(void)
{
    //USART1ʱ�ӿ���
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);   
    
    //ʹ��������������
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_2, ENABLE);       
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_3, ENABLE);
    
    //����USART1������9600,8Bit,1 stop, ��У�飬ʹ�ܷ���+����
    USART_Init(USART1, 9600, USART_WordLength_8b, USART_StopBits_1, 
              USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
    
    //ʹ�ܽ����ж�
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
    
    //ʹ�ܴ���1
    USART_Cmd(USART1, ENABLE);
}


//Summary:		���ڽ�������
//Parameter:	        void
//Return:	        1�ɹ���0 ʧ��
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

//Summary:		���ͷ��ص�����
//Parameter:	        void
//Return:	        1�ɹ���0 ʧ��
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

//Summary:	���ô��ڼ���
void	ResetUsart(void)
{
    memset(g_ucUsartBuf, 0, sizeof(g_ucUsartBuf));
    g_ucUsartLen = 0;
}