#include "DriveCom2.h"
#include "FunctionDoData.h"
extern Flag flag;
extern Value value;
extern Limit limit;

u8  g_ucUsart2Buf[100];		//���ڻ���
u8  g_ucUsart2Len = 0;		//���ڽ��յ������ݳ���


//Summary:    ��������ת����16��������������
union 
{
    float f;
    u8 d[4];
}fd;

//Summary:    ����2��ʼ�������źŷ�����ͨѶ
void Usart2_Init(void)
{
    //USART2ʱ�ӿ���
    CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE);
    
    //PE3,PE4����ʹ��
    GPIO_ExternalPullUpConfig(GPIOE, GPIO_Pin_3, ENABLE);
    GPIO_ExternalPullUpConfig(GPIOE, GPIO_Pin_4, ENABLE);
    
    //����USART2������57600,8Bit,1 stop, ��У�飬ʹ�ܷ���+����
    if(value.g_productType == 0)
       USART_Init(USART2, 57600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
    else
       USART_Init(USART2, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
    
    //ʹ�ܽ����ж�
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
   
    //ʹ�ܴ���2
    USART_Cmd(USART2, ENABLE);
    
}

//Summary:    ����2����
void Usart2_Receive(void)
{
    u8 i;
    for(i = 0; i < 100; i++)
    { 
      if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
      {
          i = 0;
          if(g_ucUsart2Len < sizeof(g_ucUsart2Buf))
          {
              g_ucUsart2Buf[g_ucUsart2Len] = USART_ReceiveData8(USART2);
              g_ucUsart2Len ++;
          }
          else
          {
              g_ucUsart2Len = 0;
              break;
          }
      }	
      usdelay(50);
    }
}

//Summary:    ����2����
void Usart2_Send(u8 *buf, u8 size)
{  
    u8 i = 0;
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    while(i < size)
    {		
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData8(USART2, *(buf + i));
        i++;
    }
    usdelay(999);
    memset(g_ucUsart2Buf, 0, 100 * sizeof(char));
    g_ucUsart2Len = 0;
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

//Summary:    ����2���ݴ���
void Analysis_Usart2Buf(void)
{ 
  if(g_ucUsart2Buf[0] == 0x01)
  {
    if(g_ucUsart2Buf[1] == 0x10)
    {
        if(g_ucUsart2Buf[3] == 0x08)
        {
                flag.u8count = 0;//��������
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;
                value.g_bCaliPoint = 1;          
        }
        else
        {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //�궨ʧ��            
        }
    }
    else if(g_ucUsart2Buf[1] == 0x12)
    {
        if(g_ucUsart2Buf[2] == 0x00)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                flag.u8count = 0;//��������
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;
                value.g_bCaliPoint = 2;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //�궨ʧ��  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x01)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//��������
                value.g_bCaliPoint = 3;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //�궨ʧ��  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x02)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;               
                flag.u8count = 0;//��������
                value.g_bCaliPoint = 4;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //�궨ʧ��  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x03)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//��������
                value.g_bCaliPoint = 5;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //�궨ʧ��  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x04)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//��������
                value.g_bCaliPoint = 6;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //�궨ʧ��  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x05)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {             
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//��������
                value.g_bCaliPoint = 7;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //�궨ʧ��  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x06)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//��������
                value.g_bCaliPoint = 8;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //�궨ʧ��  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x07)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//��������
                 value.g_bCaliPoint = 9;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //�궨ʧ��  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x08)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//��������
                 value.g_bCaliPoint = 10;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //�궨ʧ��  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x09)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                   
                flag.u8count = 0;//��������
                value.g_bCaliPoint = 11;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;                 
                value.g_u16ResultMeter = 2;  //�궨ʧ��  
            }
        }
        else 
        {
            value.g_u16ResultMeter = 2;  //�궨ʧ��  
        }
    }
    else if(g_ucUsart2Buf[1] == 0x13)
    {
        if(g_ucUsart2Buf[2] == 0x00)
        {
            switch(value.g_bCaliPoint)
            {
                case 11: //20mA���ޱ궨ֵ
                    value.u16CorrAIn[0] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                case 12: //20mA���ޱ궨ֵ
                    value.u16CorrAIn[1] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                default:;
            }  
        }
        else if(g_ucUsart2Buf[2] == 0x01)
        {
            switch(value.g_bCaliPoint)
            {
                case 13: //20mV���ޱ궨ֵ
                    value.u16CorrVIn20[0] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                case 14: //20mV���ޱ궨ֵ
                    value.u16CorrVIn20[1] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                default:;
            }
        }
        else if(g_ucUsart2Buf[2] == 0x02)
        {
            switch(value.g_bCaliPoint)
            {
                case 15: //100mV���ޱ궨ֵ
                    value.u16CorrVIn100[0] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                case 16: //100mV���ޱ궨ֵ
                    value.u16CorrVIn100[1] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                default:;
            }
        }
        else if(g_ucUsart2Buf[2] == 0x03)
        {
            switch(value.g_bCaliPoint)
            {
                case 17: //V���ޱ궨ֵ
                    value.u16CorrVIn[0] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                case 18: //V���ޱ궨ֵ
                    value.u16CorrVIn[1] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                default:;
            }
        }
    }
    else if(g_ucUsart2Buf[1] == 0x14)
    {
        if(g_ucUsart2Buf[2] == 0x00)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;
                flag.u8count = 0;//��������
                value.g_bCaliPoint = 12;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //�궨ʧ��  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x01)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;
                flag.u8count = 0;//��������                
                value.g_bCaliPoint = 13;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;                 
                value.g_u16ResultMeter = 2;  //�궨ʧ��               
            }
        }
        else if(g_ucUsart2Buf[2] == 0x02)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;
                flag.u8count = 0;//��������
                value.g_bCaliPoint = 14;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;               
                value.g_u16ResultMeter = 2;  //�궨ʧ��                 
            }          
        }
        else if(g_ucUsart2Buf[2] == 0x03)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1; 
                flag.u8count = 0;//��������              
                value.g_bCaliPoint = 15;               
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;                
                value.g_u16ResultMeter = 2;  //�궨ʧ��                
            }
        }
        else if(g_ucUsart2Buf[2] == 0x04)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;    
                flag.u8count = 0;//��������           
                value.g_bCaliPoint = 16;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;               
                value.g_u16ResultMeter = 2;  //�궨ʧ��                 
            }
        }
        else if(g_ucUsart2Buf[2] == 0x05)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1; 
                flag.u8count = 0;//��������               
                value.g_bCaliPoint = 17;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;                 
                value.g_u16ResultMeter = 2;  //�궨ʧ��               
            }
        }
        else if(g_ucUsart2Buf[2] == 0x06)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;  
                flag.u8count = 0;//��������              
                value.g_bCaliPoint = 18;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;                
                value.g_u16ResultMeter = 2;  //�궨ʧ��                
            }
        }
        else if(g_ucUsart2Buf[2] == 0x07)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//��������
                if(value.g_u16ResultMeter == 0)
                     value.g_u16ResultMeter = 1;  //�궨�ɹ�
                value.g_bStart = 0;
                value.g_bCaliFinish = 1;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;               
                value.g_u16ResultMeter = 2;  //�궨ʧ��                
            }
        }
    }
    else
    {
//        value.g_u16ResultMeter = 2;  //�궨ʧ��
    }
  }
  else if(g_ucUsart2Buf[0] == 0x02)
  {
    if(g_ucUsart2Buf[1] == 0x10)
    {
      
    }
    else if(g_ucUsart2Buf[1] == 0x11)
    {
      
    }
    else if(g_ucUsart2Buf[1] == 0x12)
    {
      
    }    
    else if(g_ucUsart2Buf[1] == 0x13)
    {
      //  switch(value.g_bCaliPoint)
        switch(13)
        {
            case 0:
                value.s32TestTemp = (s32)((((u32)g_ucUsart2Buf[15]) << 24) | (((u32)g_ucUsart2Buf[16]) << 16) | (((u32)g_ucUsart2Buf[17]) << 8) | (((u32)g_ucUsart2Buf[18])));
                break;          
            case 13:
                value.s32TestAIn[0] = (s32)((((u32)g_ucUsart2Buf[11]) << 24) | (((u32)g_ucUsart2Buf[12]) << 16) | (((u32)g_ucUsart2Buf[13]) << 8) | (((u32)g_ucUsart2Buf[14])));
                break;
            case 14:
                value.s32TestAIn[1] = (s32)((((u32)g_ucUsart2Buf[11]) << 24) | (((u32)g_ucUsart2Buf[12]) << 16) | (((u32)g_ucUsart2Buf[13]) << 8) | (((u32)g_ucUsart2Buf[14])));
                break;
            case 15:
                value.s32TestVIn20[0] = (s32)((((u32)g_ucUsart2Buf[11]) << 24) | (((u32)g_ucUsart2Buf[12]) << 16) | (((u32)g_ucUsart2Buf[13]) << 8) | (((u32)g_ucUsart2Buf[14])));
                break;
            case 16:
                value.s32TestVIn20[1] = (s32)((((u32)g_ucUsart2Buf[11]) << 24) | (((u32)g_ucUsart2Buf[12]) << 16) | (((u32)g_ucUsart2Buf[13]) << 8) | (((u32)g_ucUsart2Buf[14])));
                break;
            case 17:
                value.s32TestVIn100[0] = (s32)((((u32)g_ucUsart2Buf[11]) << 24) | (((u32)g_ucUsart2Buf[12]) << 16) | (((u32)g_ucUsart2Buf[13]) << 8) | (((u32)g_ucUsart2Buf[14])));
                break;
            case 18:
                value.s32TestVIn100[1] = (s32)((((u32)g_ucUsart2Buf[11]) << 24) | (((u32)g_ucUsart2Buf[12]) << 16) | (((u32)g_ucUsart2Buf[13]) << 8) | (((u32)g_ucUsart2Buf[14])));
                break;
            case 19:
                value.s32TestVIn[0] = (s32)((((u32)g_ucUsart2Buf[11]) << 24) | (((u32)g_ucUsart2Buf[12]) << 16) | (((u32)g_ucUsart2Buf[13]) << 8) | (((u32)g_ucUsart2Buf[14])));
                break;
            case 20:
                value.s32TestVIn[1] = (s32)((((u32)g_ucUsart2Buf[11]) << 24) | (((u32)g_ucUsart2Buf[12]) << 16) | (((u32)g_ucUsart2Buf[13]) << 8) | (((u32)g_ucUsart2Buf[14])));
                break;
            case 21:
                value.s32TestRIn[0] = (s32)((((u32)g_ucUsart2Buf[11]) << 24) | (((u32)g_ucUsart2Buf[12]) << 16) | (((u32)g_ucUsart2Buf[13]) << 8) | (((u32)g_ucUsart2Buf[14])));
                break;
            case 22:
                value.s32TestRIn[1] = (s32)((((u32)g_ucUsart2Buf[11]) << 24) | (((u32)g_ucUsart2Buf[12]) << 16) | (((u32)g_ucUsart2Buf[13]) << 8) | (((u32)g_ucUsart2Buf[14])));
                break;
          
            default:;
        }     
    }     
  }
    memset(g_ucUsart2Buf, 0, 100 * sizeof(char));
    g_ucUsart2Len = 0;
}

//Summary:    ����2��������
void Usart2_SendCmd(u8 cmd)
{
    switch(cmd)
    {
        case 0:
            Cmd1100();//��Դ20mA���������
            break;
        case 1:
            Cmd1101();//��Դ20mA���������
            break;
        case 2:
            Cmd1102();//��Դ20mA���������
            break;
        case 3:
            Cmd1103();//��Դ20mA���������
            break;
        case 4:
            Cmd1104();//20mV���������
            break;
        case 5:
            Cmd1105();//20mV���������
            break;
        case 6:
            Cmd1106();//100mV���������
            break;
        case 7:
            Cmd1107();//100mV���������
            break;
        case 8:
            Cmd1108();//V���������
            break;
        case 9:
            Cmd1109();//V���������
            break;
        case 10:
            Cmd1300();//20mA������
            break;
        case 11:
            Cmd1301();//20mV������
            break;
        case 12:
            Cmd1302();//100mV������
            break;
        case 13:
            Cmd1303();//V������
            break;
        case 14:
            Cmd1000();//ͨѶ���� ���汾��
            break;            
        default:;
    }
    Usart2_Send(g_ucUsart2Buf, g_ucUsart2Len);
//    memset(g_ucUsart2Buf, 0, 100 * sizeof(char));
//    g_ucUsart2Len = 0;
}



//Summary:    ���汾�� ͨѶ����
void Cmd1000(void)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x10;
    g_ucUsart2Buf[2] = 0x00;
    g_ucUsart2Buf[3] = 0x00;
    g_ucUsart2Buf[4] = 0x00;
    crc = crc16(g_ucUsart2Buf,5);
    g_ucUsart2Buf[5] = crc >> 8;
    g_ucUsart2Buf[6] = crc & 0x00ff;
}          
          

//Summary:    ��Դ20mA���������
void Cmd1100(void)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x11;
    g_ucUsart2Buf[2] = 0x00;
    g_ucUsart2Buf[3] = 0x07;
    g_ucUsart2Buf[4] = 0xD0;
    crc = crc16(g_ucUsart2Buf,5);
    g_ucUsart2Buf[5] = crc >> 8;
    g_ucUsart2Buf[6] = crc & 0x00ff;
}

//Summary:    ��Դ20mA���������
void Cmd1101(void)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x11;
    g_ucUsart2Buf[2] = 0x01;
    g_ucUsart2Buf[3] = 0x1F;
    g_ucUsart2Buf[4] = 0x40;
    crc = crc16(g_ucUsart2Buf,5);
    g_ucUsart2Buf[5] = crc >> 8;
    g_ucUsart2Buf[6] = crc & 0x00ff;
}

//Summary:    ��Դ20mA���������
void Cmd1102(void)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x11;
    g_ucUsart2Buf[2] = 0x02;
    g_ucUsart2Buf[3] = 0x07;
    g_ucUsart2Buf[4] = 0xD0;
    crc = crc16(g_ucUsart2Buf,5);
    g_ucUsart2Buf[5] = crc >> 8;
    g_ucUsart2Buf[6] = crc & 0x00ff;
}

//Summary:    ��Դ20mA���������
void Cmd1103(void)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x11;
    g_ucUsart2Buf[2] = 0x03;
    g_ucUsart2Buf[3] = 0x1F;
    g_ucUsart2Buf[4] = 0x40;
    crc = crc16(g_ucUsart2Buf,5);
    g_ucUsart2Buf[5] = crc >> 8;
    g_ucUsart2Buf[6] = crc & 0x00ff;
}

//Summary:    20mV���������
void Cmd1104(void)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x11;
    g_ucUsart2Buf[2] = 0x04;
    g_ucUsart2Buf[3] = 0x07;
    g_ucUsart2Buf[4] = 0xD0;
    crc = crc16(g_ucUsart2Buf,5);
    g_ucUsart2Buf[5] = crc >> 8;
    g_ucUsart2Buf[6] = crc & 0x00ff;
}

//Summary:    20mV���������
void Cmd1105(void)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x11;
    g_ucUsart2Buf[2] = 0x05;
    g_ucUsart2Buf[3] = 0x1F;
    g_ucUsart2Buf[4] = 0x40;
    crc = crc16(g_ucUsart2Buf,5);
    g_ucUsart2Buf[5] = crc >> 8;
    g_ucUsart2Buf[6] = crc & 0x00ff;
}

//Summary:    100mV���������
void Cmd1106(void)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x11;
    g_ucUsart2Buf[2] = 0x06;
    g_ucUsart2Buf[3] = 0x07;
    g_ucUsart2Buf[4] = 0xD0;
    crc = crc16(g_ucUsart2Buf,5);
    g_ucUsart2Buf[5] = crc >> 8;
    g_ucUsart2Buf[6] = crc & 0x00ff;
}

//Summary:    100mV���������
void Cmd1107(void)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x11;
    g_ucUsart2Buf[2] = 0x07;
    g_ucUsart2Buf[3] = 0x1F;
    g_ucUsart2Buf[4] = 0x40;
    crc = crc16(g_ucUsart2Buf,5);
    g_ucUsart2Buf[5] = crc >> 8;
    g_ucUsart2Buf[6] = crc & 0x00ff;
}

//Summary:    V���������
void Cmd1108(void)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x11;
    g_ucUsart2Buf[2] = 0x08;
    g_ucUsart2Buf[3] = 0x05;
    g_ucUsart2Buf[4] = 0x35;
    crc = crc16(g_ucUsart2Buf,5);
    g_ucUsart2Buf[5] = crc >> 8;
    g_ucUsart2Buf[6] = crc & 0x00ff;
}

//Summary:    V���������
void Cmd1109(void)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x11;
    g_ucUsart2Buf[2] = 0x09;
    g_ucUsart2Buf[3] = 0x17;
    g_ucUsart2Buf[4] = 0x70;
    crc = crc16(g_ucUsart2Buf,5);
    g_ucUsart2Buf[5] = crc >> 8;
    g_ucUsart2Buf[6] = crc & 0x00ff;
}


//Summary:    20mA������
void Cmd1300(void)
{
    u16 crc;  
    g_ucUsart2Len = 5;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x13;
    g_ucUsart2Buf[2] = 0x00;
    crc = crc16(g_ucUsart2Buf,3);
    g_ucUsart2Buf[3] = crc >> 8;
    g_ucUsart2Buf[4] = crc & 0x00ff;
}

//Summary:    20mV������
void Cmd1301(void)
{
    u16 crc;  
    g_ucUsart2Len = 5;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x13;
    g_ucUsart2Buf[2] = 0x01;
    crc = crc16(g_ucUsart2Buf,3);
    g_ucUsart2Buf[3] = crc >> 8;
    g_ucUsart2Buf[4] = crc & 0x00ff;
}

//Summary:    100mV������
void Cmd1302(void)
{
    u16 crc;  
    g_ucUsart2Len = 5;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x13;
    g_ucUsart2Buf[2] = 0x02;
    crc = crc16(g_ucUsart2Buf,3);
    g_ucUsart2Buf[3] = crc >> 8;
    g_ucUsart2Buf[4] = crc & 0x00ff;
}

//Summary:    V������
void Cmd1303(void)
{
    u16 crc;  
    g_ucUsart2Len = 5;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x13;
    g_ucUsart2Buf[2] = 0x03;
    crc = crc16(g_ucUsart2Buf,3);
    g_ucUsart2Buf[3] = crc >> 8;
    g_ucUsart2Buf[4] = crc & 0x00ff;
}

//Summary:    ����궨д����������
void Cmd12Send(float fdata)
{
    u16 crc;  
    g_ucUsart2Len = 9;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x12;
    switch(value.g_bCaliPoint)
    {
        case 1: //��Դ20mA�������д
            g_ucUsart2Buf[2] = 0x00;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 2: //��Դ20mA�������д
            g_ucUsart2Buf[2] = 0x01;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 3: //��Դ20mA�������д
            g_ucUsart2Buf[2] = 0x02;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 4: //��Դ20mA�������д
            g_ucUsart2Buf[2] = 0x03;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 5: //20mV�������д
            g_ucUsart2Buf[2] = 0x04;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 6: //20mV�������д
            g_ucUsart2Buf[2] = 0x05;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 7: //100mV�������д
            g_ucUsart2Buf[2] = 0x06;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 8: //100mV�������д
            g_ucUsart2Buf[2] = 0x07;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 9: //V�������д
            g_ucUsart2Buf[2] = 0x08;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 10: //V�������д
            g_ucUsart2Buf[2] = 0x09;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        default:;
    }
    
    crc = crc16(g_ucUsart2Buf,7);
    g_ucUsart2Buf[7] = crc >> 8;
    g_ucUsart2Buf[8] = crc & 0x00ff;
    Usart2_Send(g_ucUsart2Buf, g_ucUsart2Len);
}

//Summary:    �����궨д����������
void Cmd14Send(s16 s16data)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x14;
    switch(value.g_bCaliPoint)
    {
        case 11: //20mA�������д
            g_ucUsart2Buf[2] = 0x00;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 12: //20mA�������д
            g_ucUsart2Buf[2] = 0x01;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 13: //20mV�������д
            g_ucUsart2Buf[2] = 0x02;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 14: //20mV�������д
            g_ucUsart2Buf[2] = 0x03;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 15: //100mV�������д
            g_ucUsart2Buf[2] = 0x04;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 16: //100mV�������д
            g_ucUsart2Buf[2] = 0x05;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 17: //V�������д
            g_ucUsart2Buf[2] = 0x06;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 18: //V�������д
            g_ucUsart2Buf[2] = 0x07;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        default:;
    }
    
    crc = crc16(g_ucUsart2Buf,5);
    g_ucUsart2Buf[5] = crc >> 8;
    g_ucUsart2Buf[6] = crc & 0x00ff;
    Usart2_Send(g_ucUsart2Buf, g_ucUsart2Len);
}



/********************************************************************
* Summary:    ���ò�������
* Parameters:
*             cmd   ���ʹ���    0/24vloop 1/V 2/mV 3/mA 4/ŷķ
*       
* Return :    void  
********************************************************************/
static void	CmdMeasureType(u8 cmd)
{
  u16 u16Crc;
  g_ucUsart2Len = 7;
 
  switch(cmd)
  {
    case 1:g_ucUsart2Buf[4] = 1;break; //mA
    case 3:g_ucUsart2Buf[4] = 3;break; //V
    case 4:g_ucUsart2Buf[4] = 4;break; //20mV
    case 5:g_ucUsart2Buf[4] = 5;break; //100mV
    case 14:g_ucUsart2Buf[4] = 14;break; //ŷķ
    default:g_ucUsart2Buf[4] = 16;break; //end
  }  
    
  
  g_ucUsart2Buf[0] = 0x02;         // ��ַ
  g_ucUsart2Buf[1] = 0x12;         // ����
  g_ucUsart2Buf[2] = 0x01;         // ����
  g_ucUsart2Buf[3] = 0x00;         // 00

  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[5] = u16Crc / 0x100;
  g_ucUsart2Buf[6] = u16Crc % 0x100;
  
    
}


/********************************************************************
* Summary:    �򿪲���
* Parameters:
*             cmd   0 �� 1 ��  
*       
* Return :    void  
********************************************************************/
static void	CmdMeasureOpen(u8 cmd)
{
  u16 u16Crc;
  g_ucUsart2Len = 7;
 
  if(cmd < 2)
    g_ucUsart2Buf[4] = cmd;
  else
    g_ucUsart2Buf[4] = 1;

  
  g_ucUsart2Buf[0] = 0x02;         // ��ַ
  g_ucUsart2Buf[1] = 0x12;         // ����
  g_ucUsart2Buf[2] = 0x02;         // ����
  g_ucUsart2Buf[3] = 0x00;         // 00

  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[5] = u16Crc / 0x100;
  g_ucUsart2Buf[6] = u16Crc % 0x100;
  

}


/********************************************************************
* Summary:    ����S2�������
* Parameters:
*             cmd   1/mA 2/��ԴmA  3/V  4/20mV  5/100mV   14/ŷķ
*       
* Return :    void  
********************************************************************/
static void	CmdSourceType(u8 cmd)
{
  u16 u16Crc;
  g_ucUsart2Len = 7;
 
  switch(cmd)
  {
    case 1:g_ucUsart2Buf[3] = 0x00;g_ucUsart2Buf[4] = 1;break;     //��ԴmA   
    case 2:g_ucUsart2Buf[3] = 0x01;g_ucUsart2Buf[4] = 1;break;     //��ԴmA
    case 3:g_ucUsart2Buf[3] = 0x00;g_ucUsart2Buf[4] = 3;break;     //V
    case 4:g_ucUsart2Buf[3] = 0x00;g_ucUsart2Buf[4] = 4;break;     //20mV
    case 5:g_ucUsart2Buf[3] = 0x00;g_ucUsart2Buf[4] = 5;break;     //100mV
    case 14:g_ucUsart2Buf[3] = 0x00;g_ucUsart2Buf[4] = 14;break;     //ŷķ
    default:g_ucUsart2Buf[3] = 0x00;g_ucUsart2Buf[4] = 16;break;  //end
  }  

  
  g_ucUsart2Buf[0] = 0x02;         // ��ַ
  g_ucUsart2Buf[1] = 0x11;         // ����
  g_ucUsart2Buf[2] = 0x01;         // ����


  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[5] = u16Crc / 0x100;
  g_ucUsart2Buf[6] = u16Crc % 0x100;

}


/********************************************************************
* Summary:    ����S2���ֵ
* Parameters:
*             cmd u16
*       
* Return :    void  
********************************************************************/
static void	CmdSourceValue(u16 cmd)
{
  u16 u16Crc;
  g_ucUsart2Len = 9;
 

   g_ucUsart2Buf[5] = cmd / 256;
   g_ucUsart2Buf[6] = cmd % 256;
  

  
  g_ucUsart2Buf[0] = 0x02;         // ��ַ
  g_ucUsart2Buf[1] = 0x11;         // ����
  g_ucUsart2Buf[2] = 0x02;         // ����
  g_ucUsart2Buf[3] = 0x00;         // 00
  g_ucUsart2Buf[4] = 0x00;         // 00


  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[7] = u16Crc / 0x100;
  g_ucUsart2Buf[8] = u16Crc % 0x100;
  

}

/********************************************************************
* Summary:    �����
* Parameters:
*             cmd  0�ر�  1��
*       
* Return :    void  
********************************************************************/
static void	CmdSourceOpen(u8 cmd)
{
  u16 u16Crc;
  g_ucUsart2Len = 7;
 
  if(cmd < 2)
    g_ucUsart2Buf[4] = cmd;
  else
    g_ucUsart2Buf[4] = 0;
    
  g_ucUsart2Buf[0] = 0x02;         // ��ַ
  g_ucUsart2Buf[1] = 0x11;         // ����
  g_ucUsart2Buf[2] = 0x03;         // ����
  g_ucUsart2Buf[3] = 0x00;         // 00


  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[5] = u16Crc / 0x100;
  g_ucUsart2Buf[6] = u16Crc % 0x100;
  
}


/********************************************************************
* Summary:    ����Զ��״̬
* Parameters:
*             cmd  0�ر�  1��
*       
* Return :    void  
********************************************************************/
static void	CmdRemote(u8 cmd)
{
  u16 u16Crc;
  g_ucUsart2Len = 5;
 
  if(cmd < 2)
    g_ucUsart2Buf[2] = cmd;
  else
    g_ucUsart2Buf[2] = 0;
    
  g_ucUsart2Buf[0] = 0x02;         // ��ַ
  g_ucUsart2Buf[1] = 0x10;         // ����



  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[3] = u16Crc / 0x100;
  g_ucUsart2Buf[4] = u16Crc % 0x100;
  

}



/********************************************************************
* Summary:    ��ѯ����״̬
* Parameters:
*             cmd  
*       
* Return :    void  
********************************************************************/
static void	CmdCheckData(void)
{
  u16 u16Crc;
  g_ucUsart2Len = 5;
 
    
  g_ucUsart2Buf[0] = 0x02;         // ��ַ
  g_ucUsart2Buf[1] = 0x13;         // ����
  g_ucUsart2Buf[2] = 0x01;


  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[3] = u16Crc / 0x100;
  g_ucUsart2Buf[4] = u16Crc % 0x100;
  

}




/********************************************************************
* Summary:    ����2��������
* Parameters:
*             cmd   �����
*       
* Return :    void  
********************************************************************/
void Usart2_SendCmd_Test(u8 cmd)
{
    switch(cmd)
    {
        case 0:
            CmdCheckData();           //�鿴����
            break;
        case 1:
            CmdSourceType(1);         //����S2�������  ��ԴmA
            break;
        case 2:
            CmdSourceType(2);         //����S2�������  ��ԴmA
            break;
        case 3:
            CmdSourceType(3);         //����S2�������  V
            break;
        case 4:
            CmdSourceType(4);         //����S2�������  20mV
            break;
        case 5:
            CmdSourceType(5);         //����S2�������  100mV
            break;
        case 6:
            CmdSourceType(14);         //����S2�������  ŷķ
            break;            
        case 7:
            CmdSourceValue(0);        // ����S2��ѹ 0.00V
            break;
        case 8:
            CmdSourceValue(1500);    // ����S2��ѹ 15.00V
            break;
        case 9:
            CmdSourceValue(200);    // ����S2��ѹ 2.00mV
            break;
        case 10:
            CmdSourceValue(1800);    // ����S2��ѹ 18.00mV
            break;
        case 11:
            CmdSourceValue(100);    // ����S2��ѹ 10.0mV
            break;
        case 12:
            CmdSourceValue(900);    // ����S2��ѹ 90.0mV
            break;
        case 13:
            CmdSourceValue(200);    // ����S2���� 2.000mA
            break;
        case 14:
            CmdSourceValue(2100);    // ����S2���� 21.000mA
            break;
        case 15:
            CmdSourceValue(500);     // ����S2ŷķ 50.0
            break;
        case 16:
            CmdSourceValue(2500);    // ����S2ŷķ 250
            break;
        case 17:
            CmdRemote(0);             //�˳�Զ��
            break;
        case 18:
            CmdRemote(1);             //����Զ��
            break;
        case 19:
            CmdSourceOpen(0);        //�ر����
            break;
        case 20:
            CmdSourceOpen(1);        //�����
            break;  
        case 21:
            CmdMeasureType(1);        //����S2 mA
            break;  
        case 22:
            CmdMeasureType(3);        //����S2 V
            break;   
        case 23:
            CmdMeasureType(4);        //����S2 20mV
            break;   
        case 24:
            CmdMeasureType(5);        //����S2 100mV
            break;   
        case 25:
            CmdMeasureType(14);        //����S2 ŷķ
            break;   
        case 26:
            CmdMeasureOpen(0);        //�رղ�����ʾ
            break;     
        case 27:
            CmdMeasureOpen(1);        //�򿪲�����ʾ
            break;              
        default:;
    }
    Usart2_Send(g_ucUsart2Buf, g_ucUsart2Len);
    msdelay(200);
//    memset(g_ucUsart2Buf, 0, 100 * sizeof(char));
//    g_ucUsart2Len = 0;
}