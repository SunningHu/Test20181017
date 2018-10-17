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

u8  g_ucUsartBuf[32];		//���ڻ���
u8  g_ucUsartLen = 0;		//���ڽ��յ������ݳ���

//Summary:		���ڳ�ʼ��
//						������9600��,8λ����λ��1λֹͣλ����У��
//Parameter:	None
//Return:			None
void	Usart_Init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_2, ENABLE);       //ʹ��������������
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_3, ENABLE);
    USART_Init(USART1, 57600, USART_WordLength_8b, USART_StopBits_1, 
              USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

//Summary:		���ڽ�������
//Parameter:	None
//Return:			1	�ɹ���0 ʧ��
u8 Usart_Receive(void)
{
    u8 i;
    
    for(i = 0; i < 50; i++)
    {
      if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
      {
          i = 0;
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
//Parameter:	None
//Return:			1	�ɹ���0 ʧ��
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