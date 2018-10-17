#include "DriveCom2.h"
#include "FunctionDoData.h"
extern Flag flag;
extern Value value;
extern Limit limit;

u8  g_ucUsart2Buf[100];		//串口缓存
u8  g_ucUsart2Len = 0;		//串口接收到的数据长度


//Summary:    将浮点数转换成16进制数的联合体
union 
{
    float f;
    u8 d[4];
}fd;

//Summary:    串口2初始化，与信号发生器通讯
void Usart2_Init(void)
{
    //USART2时钟开启
    CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE);
    
    //PE3,PE4上拉使能
    GPIO_ExternalPullUpConfig(GPIOE, GPIO_Pin_3, ENABLE);
    GPIO_ExternalPullUpConfig(GPIOE, GPIO_Pin_4, ENABLE);
    
    //串口USART2波特率57600,8Bit,1 stop, 无校验，使能发送+接收
    if(value.g_productType == 0)
       USART_Init(USART2, 57600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
    else
       USART_Init(USART2, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
    
    //使能接受中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
   
    //使能串口2
    USART_Cmd(USART2, ENABLE);
    
}

//Summary:    串口2接收
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

//Summary:    串口2发送
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

//Summary:    串口2数据处理
void Analysis_Usart2Buf(void)
{ 
  if(g_ucUsart2Buf[0] == 0x01)
  {
    if(g_ucUsart2Buf[1] == 0x10)
    {
        if(g_ucUsart2Buf[3] == 0x08)
        {
                flag.u8count = 0;//计数清零
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;
                value.g_bCaliPoint = 1;          
        }
        else
        {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //标定失败            
        }
    }
    else if(g_ucUsart2Buf[1] == 0x12)
    {
        if(g_ucUsart2Buf[2] == 0x00)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                flag.u8count = 0;//计数清零
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;
                value.g_bCaliPoint = 2;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //标定失败  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x01)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//计数清零
                value.g_bCaliPoint = 3;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //标定失败  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x02)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;               
                flag.u8count = 0;//计数清零
                value.g_bCaliPoint = 4;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //标定失败  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x03)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//计数清零
                value.g_bCaliPoint = 5;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //标定失败  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x04)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//计数清零
                value.g_bCaliPoint = 6;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //标定失败  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x05)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {             
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//计数清零
                value.g_bCaliPoint = 7;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //标定失败  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x06)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//计数清零
                value.g_bCaliPoint = 8;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //标定失败  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x07)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//计数清零
                 value.g_bCaliPoint = 9;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //标定失败  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x08)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//计数清零
                 value.g_bCaliPoint = 10;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //标定失败  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x09)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                   
                flag.u8count = 0;//计数清零
                value.g_bCaliPoint = 11;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;                 
                value.g_u16ResultMeter = 2;  //标定失败  
            }
        }
        else 
        {
            value.g_u16ResultMeter = 2;  //标定失败  
        }
    }
    else if(g_ucUsart2Buf[1] == 0x13)
    {
        if(g_ucUsart2Buf[2] == 0x00)
        {
            switch(value.g_bCaliPoint)
            {
                case 11: //20mA下限标定值
                    value.u16CorrAIn[0] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                case 12: //20mA上限标定值
                    value.u16CorrAIn[1] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                default:;
            }  
        }
        else if(g_ucUsart2Buf[2] == 0x01)
        {
            switch(value.g_bCaliPoint)
            {
                case 13: //20mV下限标定值
                    value.u16CorrVIn20[0] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                case 14: //20mV上限标定值
                    value.u16CorrVIn20[1] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                default:;
            }
        }
        else if(g_ucUsart2Buf[2] == 0x02)
        {
            switch(value.g_bCaliPoint)
            {
                case 15: //100mV下限标定值
                    value.u16CorrVIn100[0] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                case 16: //100mV上限标定值
                    value.u16CorrVIn100[1] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                default:;
            }
        }
        else if(g_ucUsart2Buf[2] == 0x03)
        {
            switch(value.g_bCaliPoint)
            {
                case 17: //V下限标定值
                    value.u16CorrVIn[0] = g_ucUsart2Buf[3] * 256 + g_ucUsart2Buf[4];
                    break;
                case 18: //V上限标定值
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
                flag.u8count = 0;//计数清零
                value.g_bCaliPoint = 12;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;              
                value.g_u16ResultMeter = 2;  //标定失败  
            }
        }
        else if(g_ucUsart2Buf[2] == 0x01)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;
                flag.u8count = 0;//计数清零                
                value.g_bCaliPoint = 13;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;                 
                value.g_u16ResultMeter = 2;  //标定失败               
            }
        }
        else if(g_ucUsart2Buf[2] == 0x02)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;
                flag.u8count = 0;//计数清零
                value.g_bCaliPoint = 14;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;               
                value.g_u16ResultMeter = 2;  //标定失败                 
            }          
        }
        else if(g_ucUsart2Buf[2] == 0x03)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1; 
                flag.u8count = 0;//计数清零              
                value.g_bCaliPoint = 15;               
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;                
                value.g_u16ResultMeter = 2;  //标定失败                
            }
        }
        else if(g_ucUsart2Buf[2] == 0x04)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;    
                flag.u8count = 0;//计数清零           
                value.g_bCaliPoint = 16;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;               
                value.g_u16ResultMeter = 2;  //标定失败                 
            }
        }
        else if(g_ucUsart2Buf[2] == 0x05)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1; 
                flag.u8count = 0;//计数清零               
                value.g_bCaliPoint = 17;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;                 
                value.g_u16ResultMeter = 2;  //标定失败               
            }
        }
        else if(g_ucUsart2Buf[2] == 0x06)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;  
                flag.u8count = 0;//计数清零              
                value.g_bCaliPoint = 18;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;                
                value.g_u16ResultMeter = 2;  //标定失败                
            }
        }
        else if(g_ucUsart2Buf[2] == 0x07)
        {
            if(g_ucUsart2Buf[3] == 0x01)
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 1;                
                flag.u8count = 0;//计数清零
                if(value.g_u16ResultMeter == 0)
                     value.g_u16ResultMeter = 1;  //标定成功
                value.g_bStart = 0;
                value.g_bCaliFinish = 1;
            }
            else
            {
                value.u8CailPointFlag[value.g_bCaliPoint] = 2;               
                value.g_u16ResultMeter = 2;  //标定失败                
            }
        }
    }
    else
    {
//        value.g_u16ResultMeter = 2;  //标定失败
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

//Summary:    串口2发送命令
void Usart2_SendCmd(u8 cmd)
{
    switch(cmd)
    {
        case 0:
            Cmd1100();//有源20mA下限输出读
            break;
        case 1:
            Cmd1101();//有源20mA上限输出读
            break;
        case 2:
            Cmd1102();//无源20mA下限输出读
            break;
        case 3:
            Cmd1103();//无源20mA上限输出读
            break;
        case 4:
            Cmd1104();//20mV下限输出读
            break;
        case 5:
            Cmd1105();//20mV上限输出读
            break;
        case 6:
            Cmd1106();//100mV下限输出读
            break;
        case 7:
            Cmd1107();//100mV上限输出读
            break;
        case 8:
            Cmd1108();//V下限输出读
            break;
        case 9:
            Cmd1109();//V上限输出读
            break;
        case 10:
            Cmd1300();//20mA测量读
            break;
        case 11:
            Cmd1301();//20mV测量读
            break;
        case 12:
            Cmd1302();//100mV测量读
            break;
        case 13:
            Cmd1303();//V测量读
            break;
        case 14:
            Cmd1000();//通讯测试 读版本号
            break;            
        default:;
    }
    Usart2_Send(g_ucUsart2Buf, g_ucUsart2Len);
//    memset(g_ucUsart2Buf, 0, 100 * sizeof(char));
//    g_ucUsart2Len = 0;
}



//Summary:    读版本号 通讯测试
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
          

//Summary:    有源20mA下限输出读
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

//Summary:    有源20mA上限输出读
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

//Summary:    无源20mA下限输出读
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

//Summary:    无源20mA上限输出读
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

//Summary:    20mV下限输出读
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

//Summary:    20mV上限输出读
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

//Summary:    100mV下限输出读
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

//Summary:    100mV上限输出读
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

//Summary:    V下限输出读
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

//Summary:    V上限输出读
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


//Summary:    20mA测量读
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

//Summary:    20mV测量读
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

//Summary:    100mV测量读
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

//Summary:    V测量读
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

//Summary:    输出标定写并发送命令
void Cmd12Send(float fdata)
{
    u16 crc;  
    g_ucUsart2Len = 9;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x12;
    switch(value.g_bCaliPoint)
    {
        case 1: //有源20mA下限输出写
            g_ucUsart2Buf[2] = 0x00;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 2: //有源20mA上限输出写
            g_ucUsart2Buf[2] = 0x01;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 3: //无源20mA下限输出写
            g_ucUsart2Buf[2] = 0x02;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 4: //无源20mA上限输出写
            g_ucUsart2Buf[2] = 0x03;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 5: //20mV下限输出写
            g_ucUsart2Buf[2] = 0x04;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 6: //20mV上限输出写
            g_ucUsart2Buf[2] = 0x05;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 7: //100mV下限输出写
            g_ucUsart2Buf[2] = 0x06;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 8: //100mV上限输出写
            g_ucUsart2Buf[2] = 0x07;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 9: //V下限输出写
            g_ucUsart2Buf[2] = 0x08;
            fd.f = fdata;
            g_ucUsart2Buf[3] = fd.d[0];
            g_ucUsart2Buf[4] = fd.d[1];
            g_ucUsart2Buf[5] = fd.d[2];
            g_ucUsart2Buf[6] = fd.d[3];
            break;
        case 10: //V上限输出写
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

//Summary:    测量标定写并发送命令
void Cmd14Send(s16 s16data)
{
    u16 crc;  
    g_ucUsart2Len = 7;
    g_ucUsart2Buf[0] = 0x01;
    g_ucUsart2Buf[1] = 0x14;
    switch(value.g_bCaliPoint)
    {
        case 11: //20mA下限输出写
            g_ucUsart2Buf[2] = 0x00;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 12: //20mA上限输出写
            g_ucUsart2Buf[2] = 0x01;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 13: //20mV下限输出写
            g_ucUsart2Buf[2] = 0x02;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 14: //20mV上限输出写
            g_ucUsart2Buf[2] = 0x03;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 15: //100mV下限输出写
            g_ucUsart2Buf[2] = 0x04;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 16: //100mV上限输出写
            g_ucUsart2Buf[2] = 0x05;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 17: //V下限输出写
            g_ucUsart2Buf[2] = 0x06;
            g_ucUsart2Buf[3] = s16data >> 8;
            g_ucUsart2Buf[4] = s16data & 0x00ff;
            break;
        case 18: //V上限输出写
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
* Summary:    设置测量类型
* Parameters:
*             cmd   类型代号    0/24vloop 1/V 2/mV 3/mA 4/欧姆
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
    case 14:g_ucUsart2Buf[4] = 14;break; //欧姆
    default:g_ucUsart2Buf[4] = 16;break; //end
  }  
    
  
  g_ucUsart2Buf[0] = 0x02;         // 地址
  g_ucUsart2Buf[1] = 0x12;         // 命令
  g_ucUsart2Buf[2] = 0x01;         // 命令
  g_ucUsart2Buf[3] = 0x00;         // 00

  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[5] = u16Crc / 0x100;
  g_ucUsart2Buf[6] = u16Crc % 0x100;
  
    
}


/********************************************************************
* Summary:    打开测量
* Parameters:
*             cmd   0 关 1 开  
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

  
  g_ucUsart2Buf[0] = 0x02;         // 地址
  g_ucUsart2Buf[1] = 0x12;         // 命令
  g_ucUsart2Buf[2] = 0x02;         // 命令
  g_ucUsart2Buf[3] = 0x00;         // 00

  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[5] = u16Crc / 0x100;
  g_ucUsart2Buf[6] = u16Crc % 0x100;
  

}


/********************************************************************
* Summary:    设置S2输出类型
* Parameters:
*             cmd   1/mA 2/无源mA  3/V  4/20mV  5/100mV   14/欧姆
*       
* Return :    void  
********************************************************************/
static void	CmdSourceType(u8 cmd)
{
  u16 u16Crc;
  g_ucUsart2Len = 7;
 
  switch(cmd)
  {
    case 1:g_ucUsart2Buf[3] = 0x00;g_ucUsart2Buf[4] = 1;break;     //有源mA   
    case 2:g_ucUsart2Buf[3] = 0x01;g_ucUsart2Buf[4] = 1;break;     //无源mA
    case 3:g_ucUsart2Buf[3] = 0x00;g_ucUsart2Buf[4] = 3;break;     //V
    case 4:g_ucUsart2Buf[3] = 0x00;g_ucUsart2Buf[4] = 4;break;     //20mV
    case 5:g_ucUsart2Buf[3] = 0x00;g_ucUsart2Buf[4] = 5;break;     //100mV
    case 14:g_ucUsart2Buf[3] = 0x00;g_ucUsart2Buf[4] = 14;break;     //欧姆
    default:g_ucUsart2Buf[3] = 0x00;g_ucUsart2Buf[4] = 16;break;  //end
  }  

  
  g_ucUsart2Buf[0] = 0x02;         // 地址
  g_ucUsart2Buf[1] = 0x11;         // 命令
  g_ucUsart2Buf[2] = 0x01;         // 命令


  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[5] = u16Crc / 0x100;
  g_ucUsart2Buf[6] = u16Crc % 0x100;

}


/********************************************************************
* Summary:    设置S2输出值
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
  

  
  g_ucUsart2Buf[0] = 0x02;         // 地址
  g_ucUsart2Buf[1] = 0x11;         // 命令
  g_ucUsart2Buf[2] = 0x02;         // 命令
  g_ucUsart2Buf[3] = 0x00;         // 00
  g_ucUsart2Buf[4] = 0x00;         // 00


  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[7] = u16Crc / 0x100;
  g_ucUsart2Buf[8] = u16Crc % 0x100;
  

}

/********************************************************************
* Summary:    打开输出
* Parameters:
*             cmd  0关闭  1打开
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
    
  g_ucUsart2Buf[0] = 0x02;         // 地址
  g_ucUsart2Buf[1] = 0x11;         // 命令
  g_ucUsart2Buf[2] = 0x03;         // 命令
  g_ucUsart2Buf[3] = 0x00;         // 00


  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[5] = u16Crc / 0x100;
  g_ucUsart2Buf[6] = u16Crc % 0x100;
  
}


/********************************************************************
* Summary:    进度远程状态
* Parameters:
*             cmd  0关闭  1打开
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
    
  g_ucUsart2Buf[0] = 0x02;         // 地址
  g_ucUsart2Buf[1] = 0x10;         // 命令



  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[3] = u16Crc / 0x100;
  g_ucUsart2Buf[4] = u16Crc % 0x100;
  

}



/********************************************************************
* Summary:    查询数据状态
* Parameters:
*             cmd  
*       
* Return :    void  
********************************************************************/
static void	CmdCheckData(void)
{
  u16 u16Crc;
  g_ucUsart2Len = 5;
 
    
  g_ucUsart2Buf[0] = 0x02;         // 地址
  g_ucUsart2Buf[1] = 0x13;         // 命令
  g_ucUsart2Buf[2] = 0x01;


  u16Crc = crc16(g_ucUsart2Buf, g_ucUsart2Len - 2);
  g_ucUsart2Buf[3] = u16Crc / 0x100;
  g_ucUsart2Buf[4] = u16Crc % 0x100;
  

}




/********************************************************************
* Summary:    串口2发送命令
* Parameters:
*             cmd   命令号
*       
* Return :    void  
********************************************************************/
void Usart2_SendCmd_Test(u8 cmd)
{
    switch(cmd)
    {
        case 0:
            CmdCheckData();           //查看数据
            break;
        case 1:
            CmdSourceType(1);         //设置S2输出类型  有源mA
            break;
        case 2:
            CmdSourceType(2);         //设置S2输出类型  无源mA
            break;
        case 3:
            CmdSourceType(3);         //设置S2输出类型  V
            break;
        case 4:
            CmdSourceType(4);         //设置S2输出类型  20mV
            break;
        case 5:
            CmdSourceType(5);         //设置S2输出类型  100mV
            break;
        case 6:
            CmdSourceType(14);         //设置S2输出类型  欧姆
            break;            
        case 7:
            CmdSourceValue(0);        // 设置S2电压 0.00V
            break;
        case 8:
            CmdSourceValue(1500);    // 设置S2电压 15.00V
            break;
        case 9:
            CmdSourceValue(200);    // 设置S2电压 2.00mV
            break;
        case 10:
            CmdSourceValue(1800);    // 设置S2电压 18.00mV
            break;
        case 11:
            CmdSourceValue(100);    // 设置S2电压 10.0mV
            break;
        case 12:
            CmdSourceValue(900);    // 设置S2电压 90.0mV
            break;
        case 13:
            CmdSourceValue(200);    // 设置S2电流 2.000mA
            break;
        case 14:
            CmdSourceValue(2100);    // 设置S2电流 21.000mA
            break;
        case 15:
            CmdSourceValue(500);     // 设置S2欧姆 50.0
            break;
        case 16:
            CmdSourceValue(2500);    // 设置S2欧姆 250
            break;
        case 17:
            CmdRemote(0);             //退出远程
            break;
        case 18:
            CmdRemote(1);             //进入远程
            break;
        case 19:
            CmdSourceOpen(0);        //关闭输出
            break;
        case 20:
            CmdSourceOpen(1);        //打开输出
            break;  
        case 21:
            CmdMeasureType(1);        //测量S2 mA
            break;  
        case 22:
            CmdMeasureType(3);        //测量S2 V
            break;   
        case 23:
            CmdMeasureType(4);        //测量S2 20mV
            break;   
        case 24:
            CmdMeasureType(5);        //测量S2 100mV
            break;   
        case 25:
            CmdMeasureType(14);        //测量S2 欧姆
            break;   
        case 26:
            CmdMeasureOpen(0);        //关闭测量显示
            break;     
        case 27:
            CmdMeasureOpen(1);        //打开测量显示
            break;              
        default:;
    }
    Usart2_Send(g_ucUsart2Buf, g_ucUsart2Len);
    msdelay(200);
//    memset(g_ucUsart2Buf, 0, 100 * sizeof(char));
//    g_ucUsart2Len = 0;
}