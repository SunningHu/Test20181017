#include "DriveCom1.h"
#include "FunctionDoData.h"
#include "DriveEEprom.h"
#include "stm8l15x_syscfg.h"
extern Flag flag;
extern Value value;
extern Limit limit;


u8  g_ucUsart1Buf[100];		//���ڻ���
u8  g_ucUsart1Len = 0;		//���ڽ��յ������ݳ���

//Summary:    ��������ת����16��������������
union 
{
    float f;
    u8 d[4];
}fd1;

//Summary:    ����1��ʼ������MCGSͨѶ
void Usart3_Init(void)
{
   SYSCFG_REMAPDeInit();
   SYSCFG_REMAPPinConfig(REMAP_Pin_USART3TxRxPortF,ENABLE);
     
    //USART1ʱ�ӿ���
    CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE);
    
    //PC2,PC3����ʹ��
    GPIO_ExternalPullUpConfig(GPIOF, GPIO_Pin_0, ENABLE);
    GPIO_ExternalPullUpConfig(GPIOF, GPIO_Pin_1, ENABLE);
     
    //PC4���ƴ���1���ͣ�485��
    GPIO_Init(GPIOF, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow); 
    
    //����USART1������9600,8Bit,1 stop, ��У�飬ʹ�ܷ���+����
    USART_Init(USART3, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
    
    //ʹ�ܽ����ж�
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
   
    //ʹ�ܴ���1
    USART_Cmd(USART3, ENABLE);
}

//Summary:    ����1����
u8 Usart3_Receive(void)
{
    u8 i;
    for(i = 0; i < 100; i++)
    { 
      if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
      {
          i = 0;
          if(g_ucUsart1Len < sizeof(g_ucUsart1Buf))
          {
              g_ucUsart1Buf[g_ucUsart1Len] = USART_ReceiveData8(USART3);
              g_ucUsart1Len ++;
          }
          else
          {
              g_ucUsart1Len = 0;
              break;
          }
      }	
      usdelay(50);
    }
    if(g_ucUsart1Len >= 5)
        return 1;
    else
        return 0;	
}

//Summary:    ����1����
void Usart3_Send(void)
{  
    u8 i = 0;
    USART3_RS485_H;
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
    while(g_ucUsart1Len != 0)
    {		
        USART_SendData8(USART3, g_ucUsart1Buf[i]);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        i++;
        g_ucUsart1Len--;
    }
    usdelay(499);
    usdelay(999);
    usdelay(999);

    memset(g_ucUsart1Buf, 0, 100 * sizeof(char));
    g_ucUsart1Len = 0;
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART3_RS485_L; 
}

//Summary:    ����1���ݴ���
u8 Analysis_Usart1Buf(void)
{
  if(crc16(g_ucUsart1Buf, g_ucUsart1Len)!=0)
  { 
      return 0;
  }
  else if(g_ucUsart1Buf[0] != value.u8address)
  {
      return 0;
  }
//  else if(g_ucUsart1Buf[1] == 0x03)
//  {
//      SendUsart1Data();          //�����ݷ��͸�MCGS
//      return 1;
//  }
  else if(g_ucUsart1Buf[1] == 0x06)
  {
      ReceiveUsart1Data();       //���մ�MCGS���������
      return 1;
  }    
  else
  {
      return 0;
  }
}


//Summary:	���ô��ڼ���
void ResetUsart3(void)
{
    memset(g_ucUsart1Buf, 0, 100 * sizeof(char));
    g_ucUsart1Len = 0;
}

//Summary:  ���մ�MCGS���������
void ReceiveUsart1Data(void)
{
    switch(g_ucUsart1Buf[3])
    {
        case 0: 
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_1_OPEN;
              RELAY_2_OPEN;
              RELAY_3_OPEN;
              RELAY_4_OPEN;
              RELAY_5_OPEN;
              RELAY_6_OPEN;
              RELAY_7_OPEN;
              RELAY_8_OPEN;
              RELAY_9_OPEN;
              RELAY_10_OPEN;
              RELAY_11_OPEN;
              RELAY_12_OPEN;               
          }
          else
          {
              RELAY_1_CLOSE;
              RELAY_2_CLOSE;
              RELAY_3_CLOSE;
              RELAY_4_CLOSE;
              RELAY_5_CLOSE;
              RELAY_6_CLOSE;
              RELAY_7_CLOSE;
              RELAY_8_CLOSE;
              RELAY_9_CLOSE;
              RELAY_10_CLOSE;
              RELAY_11_CLOSE;
              RELAY_12_CLOSE;             
          }
          break;
        case 1:
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_1_OPEN;
          }
          else
          {
              RELAY_1_CLOSE;
          }
          break;
        case 2:
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_2_OPEN;
          }
          else
          {
              RELAY_2_CLOSE;
          }
          break;
        case 3: 
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_3_OPEN;
          }
          else
          {
              RELAY_3_CLOSE;
          }
          break;
        case 4: 
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_4_OPEN;
          }
          else
          {
              RELAY_4_CLOSE;
          }
          break;          
        case 5: 
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_5_OPEN;
          }
          else
          {
              RELAY_5_CLOSE;
          }
          break;          
        case 6: 
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_6_OPEN;
          }
          else
          {
              RELAY_6_CLOSE;
          }
          break;          
        case 7:
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_7_OPEN;
          }
          else
          {
              RELAY_7_CLOSE;
          }
          break;          
        case 8: 
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_8_OPEN;
          }
          else
          {
              RELAY_8_CLOSE;
          }
          break;
        case 9:
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_9_OPEN;
          }
          else
          {
              RELAY_9_CLOSE;
          }
          break;          
        case 10: 
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_10_OPEN;
          }
          else
          {
              RELAY_10_CLOSE;
          }
          break;          
        case 11: 
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_11_OPEN;
          }
          else
          {
              RELAY_11_CLOSE;
          }
          break;          
        case 12: 
          if(g_ucUsart1Buf[5] == 1) 
          {
              RELAY_12_OPEN;
          }
          else
          {
              RELAY_12_CLOSE;
          }
          break;          
        default:
        break;
    }
}



  


