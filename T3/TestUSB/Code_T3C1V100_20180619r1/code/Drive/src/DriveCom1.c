#include "DriveCom1.h"
#include "FunctionDoData.h"
#include "DriveEEprom.h"
extern Flag flag;
extern Value value;
extern Limit limit;

extern void Usart2_Init(void);

u8  g_ucUsart1Buf[100];		//串口缓存
u8  g_ucUsart1Len = 0;		//串口接收到的数据长度

//Summary:    将浮点数转换成16进制数的联合体
union 
{
    float f;
    u8 d[4];
}fd1;
//Summary:    串口1初始化，与MCGS通讯
void 
Usart1_Init(void)
{
    //USART1时钟开启
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
    
    //PC2,PC3上拉使能
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_2, ENABLE);
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_3, ENABLE);
     
    //PC4控制串口1发送（485）
    GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow); 
    
    //串口USART1波特率9600,8Bit,1 stop, 无校验，使能发送+接收
    USART_Init(USART1, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
    
    //使能接受中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
   
    //使能串口1
    USART_Cmd(USART1, ENABLE);
}

//Summary:    串口1接收
u8 Usart1_Receive(void)
{
    u8 i;
    for(i = 0; i < 100; i++)
    { 
      if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
      {
          i = 0;
          if(g_ucUsart1Len < sizeof(g_ucUsart1Buf))
          {
              g_ucUsart1Buf[g_ucUsart1Len] = USART_ReceiveData8(USART1);
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

//Summary:    串口1发送
void Usart1_Send(void)
{  
    u8 i = 0;
    USART1_RS485_H;
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    while(g_ucUsart1Len != 0)
    {		
        USART_SendData8(USART1, g_ucUsart1Buf[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        i++;
        g_ucUsart1Len--;
    }
    usdelay(499);
    usdelay(999);
    usdelay(999);

    memset(g_ucUsart1Buf, 0, 100 * sizeof(char));
    g_ucUsart1Len = 0;
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART1_RS485_L; 
}

//Summary:    串口1数据处理
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
  else if(g_ucUsart1Buf[1] == 0x03)
  {
      SendUsart1Data();          //将数据发送给MCGS
      return 1;
  }
  else if(g_ucUsart1Buf[1] == 0x06)
  {
      ReceiveUsart1Data();       //接收从MCGS传输的数据
      return 1;
  }  
  else
  {
      return 0;
  }
}


//Summary:	重置串口计数
void ResetUsart1(void)
{
    memset(g_ucUsart1Buf, 0, 100 * sizeof(char));
    g_ucUsart1Len = 0;
}

static void ResetCheckParam(void)
{
   memset(value.u8CailPointFlag, 0, 25 * sizeof(char));
   value.g_u16ResultMeter = 0;
   value.g_bCaliPoint = 0;
   value.g_bCaliFinish = 2;
}

//Summary:  接收从MCGS传输的数据
void ReceiveUsart1Data(void)
{
    switch(g_ucUsart1Buf[3])
    {
        case 0: 
          value.g_bStart = g_ucUsart1Buf[4] * 256 + g_ucUsart1Buf[5];
          if(value.g_bStart == 1)
          {
            ResetCheckParam();
          }
          else
          {
            ResetCheckParam();
            value.g_bCaliFinish = 0;
          }
          break;
//        case 1: value.g_bCaliFinish = g_ucUsart1Buf[4] * 256 + g_ucUsart1Buf[5];break;
        case 2: value.g_bCaliPoint = g_ucUsart1Buf[4] * 256 + g_ucUsart1Buf[5];break;
//        case 3: value.g_u16MacData = g_ucUsart1Buf[4] * 256 + g_ucUsart1Buf[5];break;
//        case 4: 
//            fd1.d[0] = g_ucUsart1Buf[4];
//            fd1.d[1] = g_ucUsart1Buf[5];
//            fd1.d[2] = g_ucUsart1Buf[6];
//            fd1.d[3] = g_ucUsart1Buf[7];
//            value.g_fMacData = fd1.f;
//            break;
//        case 5: value.g_u16ResultMeter = g_ucUsart1Buf[4] * 256 + g_ucUsart1Buf[5];break;
        case 6: value.g_productType  = g_ucUsart1Buf[4] * 256 + g_ucUsart1Buf[5]; Usart2_Init();break;
        case 7: value.g_functionType  = g_ucUsart1Buf[4] * 256 + g_ucUsart1Buf[5];break;
        case 8:SendCheckData();break;
        case 9:SendCailData();break;
        default:
        break;
    }
}


//Summary:  将数据发送给MCGS
void SendUsart1Data(void)
{
    u16 crc = 0;  
    g_ucUsart1Len = 48;  
    g_ucUsart1Buf[0] = value.u8address;
    g_ucUsart1Buf[1] = 0x03;
    g_ucUsart1Buf[2] = 0x30;
    g_ucUsart1Buf[3] = value.g_bStart >> 8;     
    g_ucUsart1Buf[4] = value.g_bStart & 0x00ff;          //开始标志，0为未开始，1为开始
    g_ucUsart1Buf[5] = value.g_bCaliFinish >> 8;       
    g_ucUsart1Buf[6] = value.g_bCaliFinish & 0x00ff;    //标定状态，0为准备标定，1为标定完成，2为正在标定
    g_ucUsart1Buf[7] = value.g_bCaliPoint >> 8;       
    g_ucUsart1Buf[8] = value.g_bCaliPoint & 0x00ff;     //标定点，标定数据类型
    g_ucUsart1Buf[9] = value.g_u16MacData >> 8;       
    g_ucUsart1Buf[10] = value.g_u16MacData & 0x00ff;     //整数码值
    fd1.f = value.g_fMacData;
    g_ucUsart1Buf[11] = fd1.d[3];     
    g_ucUsart1Buf[12] = fd1.d[2];     
    g_ucUsart1Buf[13] = fd1.d[1];     
    g_ucUsart1Buf[14] = fd1.d[0];                        //浮点数码值
    g_ucUsart1Buf[15] = value.g_u16ResultMeter >> 8;       
    g_ucUsart1Buf[16] = value.g_u16ResultMeter & 0x00ff; //标定结果，0为未完成，1为成功，2为失败        
    g_ucUsart1Buf[17] = value.g_productType >> 8;       
    g_ucUsart1Buf[18] = value.g_productType & 0x00ff;   // 0 S1  1 S2
    g_ucUsart1Buf[19] = value.g_functionType >> 8;       
    g_ucUsart1Buf[20] = value.g_functionType & 0x00ff;   // 0 S1  1 S2    

    g_ucUsart1Buf[21] = value.u8CailPointFlag[0];    
    g_ucUsart1Buf[22] = value.u8CailPointFlag[1];     
    g_ucUsart1Buf[23] = value.u8CailPointFlag[2];     
    g_ucUsart1Buf[24] = value.u8CailPointFlag[3];                      
    g_ucUsart1Buf[25] = value.u8CailPointFlag[4];       
    g_ucUsart1Buf[26] = value.u8CailPointFlag[5];        
    g_ucUsart1Buf[27] = value.u8CailPointFlag[6];      
    g_ucUsart1Buf[28] = value.u8CailPointFlag[7];
    g_ucUsart1Buf[29] = value.u8CailPointFlag[8];       
    g_ucUsart1Buf[30] = value.u8CailPointFlag[9];  
    g_ucUsart1Buf[31] = value.u8CailPointFlag[10];    
    g_ucUsart1Buf[32] = value.u8CailPointFlag[11];     
    g_ucUsart1Buf[33] = value.u8CailPointFlag[12];     
    g_ucUsart1Buf[34] = value.u8CailPointFlag[13];                      
    g_ucUsart1Buf[35] = value.u8CailPointFlag[14];       
    g_ucUsart1Buf[36] = value.u8CailPointFlag[15];        
    g_ucUsart1Buf[37] = value.u8CailPointFlag[16];      
    g_ucUsart1Buf[38] = value.u8CailPointFlag[17];
    g_ucUsart1Buf[39] = value.u8CailPointFlag[18];       
    g_ucUsart1Buf[40] = value.u8CailPointFlag[19];     
    g_ucUsart1Buf[41] = value.u8CailPointFlag[20];    
    g_ucUsart1Buf[42] = value.u8CailPointFlag[21];     
    g_ucUsart1Buf[43] = value.u8CailPointFlag[22];     
    g_ucUsart1Buf[44] = value.u8CailPointFlag[23];                      
    g_ucUsart1Buf[45] = value.u8CailPointFlag[24]; 
    
    crc = crc16(g_ucUsart1Buf,46);
    g_ucUsart1Buf[46]= crc >> 8;
    g_ucUsart1Buf[47]= crc & 0x00ff;   
}


//Summary:  将检测数据发送给MCGS
void SendCheckData(void)
{
    u16 crc = 0;  
    g_ucUsart1Len = 97;  
    g_ucUsart1Buf[0] = value.u8address;
    g_ucUsart1Buf[1] = 0x03;
    g_ucUsart1Buf[2] = 97;
    fd1.f = S32tofloatCheck(value.s32TestTemp,1);
    g_ucUsart1Buf[3] = fd1.d[3];     
    g_ucUsart1Buf[4] = fd1.d[2];     
    g_ucUsart1Buf[5] = fd1.d[1];     
    g_ucUsart1Buf[6] = fd1.d[0];                        //冷端温度
    fd1.f = S32tofloatCheck(value.s32PTestAOut[0],3); 
    g_ucUsart1Buf[7] = fd1.d[3];     
    g_ucUsart1Buf[8] = fd1.d[2];     
    g_ucUsart1Buf[9] = fd1.d[1];     
    g_ucUsart1Buf[10] = fd1.d[0];                      //有源mA下限
    fd1.f = S32tofloatCheck(value.s32PTestAOut[1],3); 
    g_ucUsart1Buf[11] = fd1.d[3];     
    g_ucUsart1Buf[12] = fd1.d[2];     
    g_ucUsart1Buf[13] = fd1.d[1];     
    g_ucUsart1Buf[14] = fd1.d[0];                      //有源mA上限        
    fd1.f = S32tofloatCheck(value.s32NTestAOut[0],3); 
    g_ucUsart1Buf[15] = fd1.d[3];     
    g_ucUsart1Buf[16] = fd1.d[2];     
    g_ucUsart1Buf[17] = fd1.d[1];     
    g_ucUsart1Buf[18] = fd1.d[0];                      //无源mA下限
    fd1.f = S32tofloatCheck(value.s32NTestAOut[1],3); 
    g_ucUsart1Buf[19] = fd1.d[3];     
    g_ucUsart1Buf[20] = fd1.d[2];     
    g_ucUsart1Buf[21] = fd1.d[1];     
    g_ucUsart1Buf[22] = fd1.d[0];                      //无源mA上限     
    fd1.f = S32tofloatCheck(value.s32TestVOut20[0],2); 
    g_ucUsart1Buf[23] = fd1.d[3];     
    g_ucUsart1Buf[24] = fd1.d[2];     
    g_ucUsart1Buf[25] = fd1.d[1];     
    g_ucUsart1Buf[26] = fd1.d[0];                      //20mV下限
    fd1.f = S32tofloatCheck(value.s32TestVOut20[1],2); 
    g_ucUsart1Buf[27] = fd1.d[3];     
    g_ucUsart1Buf[28] = fd1.d[2];     
    g_ucUsart1Buf[29] = fd1.d[1];     
    g_ucUsart1Buf[30] = fd1.d[0];                      //20mV上限         
    fd1.f = S32tofloatCheck(value.s32TestVOut100[0],2); 
    g_ucUsart1Buf[31] = fd1.d[3];     
    g_ucUsart1Buf[32] = fd1.d[2];     
    g_ucUsart1Buf[33] = fd1.d[1];     
    g_ucUsart1Buf[34] = fd1.d[0];                      //100mV下限
    fd1.f = S32tofloatCheck(value.s32TestVOut100[1],2); 
    g_ucUsart1Buf[35] = fd1.d[3];     
    g_ucUsart1Buf[36] = fd1.d[2];     
    g_ucUsart1Buf[37] = fd1.d[1];     
    g_ucUsart1Buf[38] = fd1.d[0];                      //100mV上限       
    fd1.f = S32tofloatCheck(value.s32TestVOut[0],3); 
    g_ucUsart1Buf[39] = fd1.d[3];     
    g_ucUsart1Buf[40] = fd1.d[2];     
    g_ucUsart1Buf[41] = fd1.d[1];     
    g_ucUsart1Buf[42] = fd1.d[0];                      //V下限
    fd1.f = S32tofloatCheck(value.s32TestVOut[1],3); 
    g_ucUsart1Buf[43] = fd1.d[3];     
    g_ucUsart1Buf[44] = fd1.d[2];     
    g_ucUsart1Buf[45] = fd1.d[1];     
    g_ucUsart1Buf[46] = fd1.d[0];                      //V上限        
    fd1.f = S32tofloatCheck(value.s32TestROut[0],1); 
    g_ucUsart1Buf[47] = fd1.d[3];     
    g_ucUsart1Buf[48] = fd1.d[2];     
    g_ucUsart1Buf[49] = fd1.d[1];     
    g_ucUsart1Buf[50] = fd1.d[0];                      //R下限
    fd1.f = S32tofloatCheck(value.s32TestROut[1],1); 
    g_ucUsart1Buf[51] = fd1.d[3];     
    g_ucUsart1Buf[52] = fd1.d[2];     
    g_ucUsart1Buf[53] = fd1.d[1];     
    g_ucUsart1Buf[54] = fd1.d[0];                      //R上限   
    fd1.f = S32tofloatCheck(value.s32TestAIn[0],2); 
    g_ucUsart1Buf[55] = fd1.d[3];     
    g_ucUsart1Buf[56] = fd1.d[2];     
    g_ucUsart1Buf[57] = fd1.d[1];     
    g_ucUsart1Buf[58] = fd1.d[0];                      //测量mA下限
    fd1.f = S32tofloatCheck(value.s32TestAIn[1],2); 
    g_ucUsart1Buf[59] = fd1.d[3];     
    g_ucUsart1Buf[60] = fd1.d[2];     
    g_ucUsart1Buf[61] = fd1.d[1];     
    g_ucUsart1Buf[62] = fd1.d[0];                      //测量mA上限       
    fd1.f = S32tofloatCheck(value.s32TestVIn20[0],2); 
    g_ucUsart1Buf[63] = fd1.d[3];     
    g_ucUsart1Buf[64] = fd1.d[2];     
    g_ucUsart1Buf[65] = fd1.d[1];     
    g_ucUsart1Buf[66] = fd1.d[0];                      //测量20mV下限
    fd1.f = S32tofloatCheck(value.s32TestVIn20[1],2); 
    g_ucUsart1Buf[67] = fd1.d[3];     
    g_ucUsart1Buf[68] = fd1.d[2];     
    g_ucUsart1Buf[69] = fd1.d[1];     
    g_ucUsart1Buf[70] = fd1.d[0];                      //测量20mV上限       
    fd1.f = S32tofloatCheck(value.s32TestVIn100[0],1); 
    g_ucUsart1Buf[71] = fd1.d[3];     
    g_ucUsart1Buf[72] = fd1.d[2];     
    g_ucUsart1Buf[73] = fd1.d[1];     
    g_ucUsart1Buf[74] = fd1.d[0];                      //测量100mV下限
    fd1.f = S32tofloatCheck(value.s32TestVIn100[1],1); 
    g_ucUsart1Buf[75] = fd1.d[3];     
    g_ucUsart1Buf[76] = fd1.d[2];     
    g_ucUsart1Buf[77] = fd1.d[1];     
    g_ucUsart1Buf[78] = fd1.d[0];                      //测量100mV上限       
    fd1.f = S32tofloatCheck(value.s32TestVIn[0],2); 
    g_ucUsart1Buf[79] = fd1.d[3];     
    g_ucUsart1Buf[80] = fd1.d[2];     
    g_ucUsart1Buf[81] = fd1.d[1];     
    g_ucUsart1Buf[82] = fd1.d[0];                      //测量V下限
    fd1.f = S32tofloatCheck(value.s32TestVIn[1],2); 
    g_ucUsart1Buf[83] = fd1.d[3];     
    g_ucUsart1Buf[84] = fd1.d[2];     
    g_ucUsart1Buf[85] = fd1.d[1];     
    g_ucUsart1Buf[86] = fd1.d[0];                      //测量V上限       
    fd1.f = S32tofloatCheck(value.s32TestRIn[0],1); 
    g_ucUsart1Buf[87] = fd1.d[3];     
    g_ucUsart1Buf[88] = fd1.d[2];     
    g_ucUsart1Buf[89] = fd1.d[1];     
    g_ucUsart1Buf[90] = fd1.d[0];                      //测量R下限
    fd1.f = S32tofloatCheck(value.s32TestRIn[1],1); 
    g_ucUsart1Buf[91] = fd1.d[3];     
    g_ucUsart1Buf[92] = fd1.d[2];     
    g_ucUsart1Buf[93] = fd1.d[1];     
    g_ucUsart1Buf[94] = fd1.d[0];                      //测量r上限              
    crc = crc16(g_ucUsart1Buf,95);
    g_ucUsart1Buf[95]= crc >> 8;
    g_ucUsart1Buf[96]= crc & 0x00ff;   
}


//Summary:  将标定数据发送给MCGS
void SendCailData(void)
{
    u16 crc = 0;  
    g_ucUsart1Len = 73;  
    g_ucUsart1Buf[0] = value.u8address;
    g_ucUsart1Buf[1] = 0x03;
    g_ucUsart1Buf[2] = 73;
    fd1.f = value.fPCorrAOut[0];
    g_ucUsart1Buf[3] = fd1.d[3];     
    g_ucUsart1Buf[4] = fd1.d[2];     
    g_ucUsart1Buf[5] = fd1.d[1];     
    g_ucUsart1Buf[6] = fd1.d[0];                        //冷端温度
    fd1.f = value.fPCorrAOut[1]; 
    g_ucUsart1Buf[7] = fd1.d[3];     
    g_ucUsart1Buf[8] = fd1.d[2];     
    g_ucUsart1Buf[9] = fd1.d[1];     
    g_ucUsart1Buf[10] = fd1.d[0];                      //有源mA下限
    fd1.f = value.fNCorrAOut[0]; 
    g_ucUsart1Buf[11] = fd1.d[3];     
    g_ucUsart1Buf[12] = fd1.d[2];     
    g_ucUsart1Buf[13] = fd1.d[1];     
    g_ucUsart1Buf[14] = fd1.d[0];                      //有源mA上限        
    fd1.f = value.fNCorrAOut[1]; 
    g_ucUsart1Buf[15] = fd1.d[3];     
    g_ucUsart1Buf[16] = fd1.d[2];     
    g_ucUsart1Buf[17] = fd1.d[1];     
    g_ucUsart1Buf[18] = fd1.d[0];                      //无源mA下限
    fd1.f = value.fCorrVOut20[0]; 
    g_ucUsart1Buf[19] = fd1.d[3];     
    g_ucUsart1Buf[20] = fd1.d[2];     
    g_ucUsart1Buf[21] = fd1.d[1];     
    g_ucUsart1Buf[22] = fd1.d[0];                      //无源mA上限     
    fd1.f = value.fCorrVOut20[1]; 
    g_ucUsart1Buf[23] = fd1.d[3];     
    g_ucUsart1Buf[24] = fd1.d[2];     
    g_ucUsart1Buf[25] = fd1.d[1];     
    g_ucUsart1Buf[26] = fd1.d[0];                      //20mV下限
    fd1.f = value.fCorrVOut100[0]; 
    g_ucUsart1Buf[27] = fd1.d[3];     
    g_ucUsart1Buf[28] = fd1.d[2];     
    g_ucUsart1Buf[29] = fd1.d[1];     
    g_ucUsart1Buf[30] = fd1.d[0];                      //20mV上限     
    fd1.f = value.fCorrVOut100[1]; 
    g_ucUsart1Buf[31] = fd1.d[3];     
    g_ucUsart1Buf[32] = fd1.d[2];     
    g_ucUsart1Buf[33] = fd1.d[1];     
    g_ucUsart1Buf[34] = fd1.d[0];                      //20mV上限        
    fd1.f = value.fCorrVOut[0]; 
    g_ucUsart1Buf[35] = fd1.d[3];     
    g_ucUsart1Buf[36] = fd1.d[2];     
    g_ucUsart1Buf[37] = fd1.d[1];     
    g_ucUsart1Buf[38] = fd1.d[0];                      //100mV下限
    fd1.f = value.fCorrVOut[1];  
    g_ucUsart1Buf[39] = fd1.d[3];     
    g_ucUsart1Buf[40] = fd1.d[2];     
    g_ucUsart1Buf[41] = fd1.d[1];     
    g_ucUsart1Buf[42] = fd1.d[0];                      //100mV上限       
    fd1.f = value.fCorrROut[1]; 
    g_ucUsart1Buf[43] = fd1.d[3];     
    g_ucUsart1Buf[44] = fd1.d[2];     
    g_ucUsart1Buf[45] = fd1.d[1];     
    g_ucUsart1Buf[46] = fd1.d[0];                      //V下限
    fd1.f = value.fCorrROut[1]; 
    g_ucUsart1Buf[47] = fd1.d[3];     
    g_ucUsart1Buf[48] = fd1.d[2];     
    g_ucUsart1Buf[49] = fd1.d[1];     
    g_ucUsart1Buf[50] = fd1.d[0];                      //V上限        

    g_ucUsart1Buf[51] = value.u16CorrAIn[0] >> 8;   
    g_ucUsart1Buf[52] = value.u16CorrAIn[0] & 0x00ff;    
    g_ucUsart1Buf[53] = value.u16CorrAIn[1] >> 8;     
    g_ucUsart1Buf[54] = value.u16CorrAIn[1] & 0x00ff;                     //R下限
    g_ucUsart1Buf[55] = value.u16CorrVIn20[0] >> 8;     
    g_ucUsart1Buf[56] = value.u16CorrVIn20[0] & 0x00ff; 
    g_ucUsart1Buf[57] = value.u16CorrVIn20[1] >> 8;     
    g_ucUsart1Buf[58] = value.u16CorrVIn20[1] & 0x00ff;                     //R上限   
    g_ucUsart1Buf[59] = value.u16CorrVIn100[0] >> 8;     
    g_ucUsart1Buf[60] = value.u16CorrVIn100[0] & 0x00ff;    
    g_ucUsart1Buf[61] = value.u16CorrVIn100[1] >> 8;     
    g_ucUsart1Buf[62] = value.u16CorrVIn100[1] & 0x00ff;                       //测量mA下限
    g_ucUsart1Buf[63] = value.u16CorrVIn[0] >> 8;     
    g_ucUsart1Buf[64] = value.u16CorrVIn[0] & 0x00ff;      
    g_ucUsart1Buf[65] = value.u16CorrVIn[1] >> 8;     
    g_ucUsart1Buf[66] = value.u16CorrVIn[1] & 0x00ff;                       //测量mA上限       
    g_ucUsart1Buf[67] = value.u16CorrRIn[0] >> 8;   
    g_ucUsart1Buf[68] = value.u16CorrRIn[0] & 0x00ff;    
    g_ucUsart1Buf[69] = value.u16CorrRIn[1] >> 8;     
    g_ucUsart1Buf[70] = value.u16CorrRIn[1] & 0x00ff;                     //R下限                
    crc = crc16(g_ucUsart1Buf,71);
    g_ucUsart1Buf[71]= crc >> 8;
    g_ucUsart1Buf[72]= crc & 0x00ff;   
}




//Summary:  将数据发送给MCGS
void SendManageData(void)
{
    u16 crc = 0;  
    g_ucUsart1Len = 8;  
    g_ucUsart1Buf[0] = value.u8address;
    g_ucUsart1Buf[1] = 0x11;
    g_ucUsart1Buf[2] = 0x04;
    g_ucUsart1Buf[3] = 0x02;
    g_ucUsart1Buf[4] = limit.u16MaxOKCount >> 8;        
    g_ucUsart1Buf[5] = limit.u16MaxOKCount & 0x00ff;           //手/自动，0为手动，1为自动
    
    crc = crc16(g_ucUsart1Buf,6);
    g_ucUsart1Buf[6]= crc >> 8;
    g_ucUsart1Buf[7]= crc & 0x00ff;   
}

//static void WriteFloat(u8 reg, u8 val[])
//{
//  for(int i=0;i<4;i++)
//  {
//    eep_write8(reg + i,val[i]);
//  }
//}

//Summary:  将数据发送给MCGS
void ReceiveManageData(void)
{    
    u16 crc = 0;  
    
    limit.u16MaxOKCount = g_ucUsart1Buf[4] * 256 + g_ucUsart1Buf[5];
    eep_write16(0,limit.u16MaxOKCount);    
    
    memset(g_ucUsart1Buf, 0, 100 * sizeof(char));
    g_ucUsart1Len = 6;
    g_ucUsart1Buf[0] = value.u8address;
    g_ucUsart1Buf[1] = 0x12;
    g_ucUsart1Buf[2] = 0x04;
    g_ucUsart1Buf[3] = 0x01;    
    crc = crc16(g_ucUsart1Buf,5);
    g_ucUsart1Buf[4]= crc >> 8;
    g_ucUsart1Buf[5]= crc & 0x00ff;   
}


