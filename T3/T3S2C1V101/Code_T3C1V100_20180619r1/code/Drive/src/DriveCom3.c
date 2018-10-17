#include "DriveCom3.h"
#include "FunctionDoData.h"
extern Flag flag;
extern Value value;
extern Limit limit;

u8  g_cUsart3Buf[100];		//串口缓存
u8  g_ucUsart3Len = 0;		//串口接收到的数据长度



//Summary:    串口3初始化，与CA150校验仪通讯
void Usart3_Init(void)
{
    //USART3时钟开启
    CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE);
    
    //PG0,PG1上拉使能
    GPIO_ExternalPullUpConfig(GPIOG, GPIO_Pin_0, ENABLE);
    GPIO_ExternalPullUpConfig(GPIOG, GPIO_Pin_1, ENABLE);
    
    
    //串口USART3波特率9600,8Bit,1 stop, 无校验，使能发送+接收
    USART_Init(USART3, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
    
    //使能接受中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
   
    //使能串口3
    USART_Cmd(USART3, ENABLE);
    
}

//Summary:    串口3接收
void Usart3_Receive(void)
{
    u8 i;
    for(i = 0; i < 100; i++)
    { 
      if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
      {
        
          i = 0;
          if(g_ucUsart3Len < sizeof(g_cUsart3Buf))
          {
              g_cUsart3Buf[g_ucUsart3Len] = USART_ReceiveData8(USART3);
              g_ucUsart3Len ++;
          }
          else
          {
              g_ucUsart3Len = 0;
              break;
          }
      }	
      usdelay(80);
    }
}

//Summary:    串口3发送
//void Usart3_Send(u8 *buf, u8 size)
//{  
//    u8 i = 0;
//    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
//    while(i < g_ucUsart3Len)
//    {		
//        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
//        USART_SendData8(USART3, g_cUsart3Buf[i]);
//        i++;
//    }
//    usdelay(550);
//    usdelay(999);
//    usdelay(999);
//    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
//    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//}



//Summary:    串口3发送
void Usart3_Send(u8 *buf, u8 size)
{  
    u8 i = 0;
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
    while(i < size)
    {		
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        USART_SendData8(USART3, *(buf + i));
        i++;
    }
    usdelay(550);
    usdelay(999);
    usdelay(999);
    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
    g_ucUsart3Len = 0;
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

//Summary:    串口3数据处理
void Analysis_Usart3Buf(void)
{
  
  if(g_cUsart3Buf[0] == 0x02)
  {
    if(g_cUsart3Buf[1] == 0x10)
    {
      
    }
    else if(g_cUsart3Buf[1] == 0x11)
    {
      
    }
    else if(g_cUsart3Buf[1] == 0x12)
    {
      
    }    
    else if(g_cUsart3Buf[1] == 0x13)
    {
        switch(value.g_bCaliPoint)
        {
            case 1:
                value.s32PTestAOut[0] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 2:
                value.s32PTestAOut[1] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 3:
                value.s32NTestAOut[0] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 4:
                value.s32NTestAOut[1] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 5:
                value.s32TestVOut20[0] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 6:
                value.s32TestVOut20[1] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 7:
                value.s32TestVOut100[0] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 8:
                value.s32TestVOut100[1] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 9:
                value.s32TestVOut[0] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 10:
                value.s32TestVOut[1] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 11:
                value.s32TestROut[0] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 12:
                value.s32TestROut[1] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;          
            case 13:
                value.s32TestROut[2] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 14:
                value.s32TestROut[3] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;    
            case 15:
                value.s32TestROut[4] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;
            case 16:
                value.s32TestROut[5] = (s32)((((u32)g_cUsart3Buf[11]) << 24) | (((u32)g_cUsart3Buf[12]) << 16) | (((u32)g_cUsart3Buf[13]) << 8) | (((u32)g_cUsart3Buf[14])));
                break;                    
            default:;
        }     
    }       
  }
  else
  {
      
  }

    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
    g_ucUsart3Len = 0;
}

//Summary:    校验仪mA信号字符串转float
float StringToFloatVmA(char* str)
{
    float fdata = 0;  
    if(str[1] != '0')
    {
        fdata = (str[1] - '0') * 10.0;
    }
    fdata = fdata + (str[2] - '0') * 1.0;
    fdata = fdata + (str[4] - '0') * 0.1;
    fdata = fdata + (str[5] - '0') * 0.01;
    fdata = fdata + (str[6] - '0') * 0.001;
    return fdata;
}

//Summary:    校验仪mV信号字符串转float
float StringToFloatmV(char* str)
{
    float fdata = 0;  
    if(str[1] != '0')
    {
        fdata += (str[1]- '0') * 100.0;
    }
    if(str[2] != '0')
    {
        fdata += (str[2]- '0') * 10.0;
    }
    fdata = fdata + (str[3] - '0') * 1.0;
    fdata = fdata + (str[5] - '0') * 0.1;
    fdata = fdata + (str[6] - '0') * 0.01;
    return fdata;
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
  g_ucUsart3Len = 7;
 
   if(cmd < 5)
    g_cUsart3Buf[4] = cmd;
   else
    g_cUsart3Buf[4] = 16;
    
  
  g_cUsart3Buf[0] = 0x02;         // 地址
  g_cUsart3Buf[1] = 0x12;         // 命令
  g_cUsart3Buf[2] = 0x01;         // 命令
  g_cUsart3Buf[3] = 0x00;         // 00

  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[5] = u16Crc / 0x100;
  g_cUsart3Buf[6] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
//    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
    
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
  g_ucUsart3Len = 7;
 
  if(cmd < 2)
    g_cUsart3Buf[4] = cmd;
  else
    g_cUsart3Buf[4] = 1;

  
  g_cUsart3Buf[0] = 0x02;         // 地址
  g_cUsart3Buf[1] = 0x12;         // 命令
  g_cUsart3Buf[2] = 0x02;         // 命令
  g_cUsart3Buf[3] = 0x00;         // 00

  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[5] = u16Crc / 0x100;
  g_cUsart3Buf[6] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
//    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
}


/********************************************************************
* Summary:    设置S3输出类型
* Parameters:
*             cmd  0/24vloop 1/V 2/mV 3/mA 4/欧姆 
*       
* Return :    void  
********************************************************************/
static void	CmdSourceType(u8 cmd)
{
  u16 u16Crc;
  g_ucUsart3Len = 7;
 
  if(cmd < 5 && cmd > 0)
    g_cUsart3Buf[4] = cmd;
  else
    g_cUsart3Buf[4] = 16;

  
  g_cUsart3Buf[0] = 0x02;         // 地址
  g_cUsart3Buf[1] = 0x11;         // 命令
  g_cUsart3Buf[2] = 0x01;         // 命令
  g_cUsart3Buf[3] = 0x00;         // 00

  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[5] = u16Crc / 0x100;
  g_cUsart3Buf[6] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
//    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
}


/********************************************************************
* Summary:    设置S3输出值
* Parameters:
*             cmd u16
*       
* Return :    void  
********************************************************************/
static void	CmdSourceValue(u16 cmd)
{
  u16 u16Crc;
  g_ucUsart3Len = 9;
 

   g_cUsart3Buf[5] = cmd / 256;
   g_cUsart3Buf[6] = cmd % 256;
  

  
  g_cUsart3Buf[0] = 0x02;         // 地址
  g_cUsart3Buf[1] = 0x11;         // 命令
  g_cUsart3Buf[2] = 0x02;         // 命令
  g_cUsart3Buf[3] = 0x00;         // 00
  g_cUsart3Buf[4] = 0x00;         // 00


  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[7] = u16Crc / 0x100;
  g_cUsart3Buf[8] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
//    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
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
  g_ucUsart3Len = 7;
 
  if(cmd < 2)
    g_cUsart3Buf[4] = cmd;
  else
    g_cUsart3Buf[4] = 0;
    
  g_cUsart3Buf[0] = 0x02;         // 地址
  g_cUsart3Buf[1] = 0x11;         // 命令
  g_cUsart3Buf[2] = 0x03;         // 命令
  g_cUsart3Buf[3] = 0x00;         // 00


  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[5] = u16Crc / 0x100;
  g_cUsart3Buf[6] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
 //   memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
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
  g_ucUsart3Len = 5;
 
  if(cmd < 2)
    g_cUsart3Buf[2] = cmd;
  else
    g_cUsart3Buf[2] = 0;
    
  g_cUsart3Buf[0] = 0x02;         // 地址
  g_cUsart3Buf[1] = 0x10;         // 命令



  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[3] = u16Crc / 0x100;
  g_cUsart3Buf[4] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
//    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
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
  g_ucUsart3Len = 5;
 
    
  g_cUsart3Buf[0] = 0x02;         // 地址
  g_cUsart3Buf[1] = 0x13;         // 命令
  g_cUsart3Buf[2] = 0x01;


  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[3] = u16Crc / 0x100;
  g_cUsart3Buf[4] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
//    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
}



//Summary:    串口3发送命令
void Usart3_SendCmd_Test(u8 cmd)
{
    switch(cmd)
    {
        case 0:
            CmdCheckData();           //查看数据
            msdelay(200);
            break;
        case 1:
            CmdSourceType(1);         //设置S3输出类型  V
            msdelay(200);
            break;
        case 2:
            CmdSourceType(2);         //设置S3输出类型  mV
            msdelay(200);
            break;
        case 3:
            CmdSourceType(3);         //设置S3输出类型  mA
            msdelay(200);
            break;
        case 4:
            CmdSourceType(4);         //设置S3输出类型  欧姆
            msdelay(200);
            break;
        case 5:
            CmdSourceValue(3000);    // 设置S3电压 3.000V
            msdelay(200);
            break;
        case 6:
            CmdSourceValue(10000);    // 设置S3电压 10.000V
            msdelay(200);
            break;
        case 7:
            CmdSourceValue(200);    // 设置S3电压 2.00mV
            msdelay(200);
            break;
        case 8:
            CmdSourceValue(1800);    // 设置S3电压 18.00mV
            msdelay(200);
            break;
        case 9:
            CmdSourceValue(1000);    // 设置S3电压 10.00mV
            msdelay(200);
            break;
        case 10:
            CmdSourceValue(9000);    // 设置S3电压 90.00mV
            msdelay(200);
            break;
        case 11:
            CmdSourceValue(2000);    // 设置S3电流 2.000mA
            msdelay(200);
            break;
        case 12:
            CmdSourceValue(21000);    // 设置S3电流 21.000mA
            msdelay(200);
            break;
        case 13:
            CmdSourceValue(500);     // 设置S3欧姆 50.0
            msdelay(200);
            break;
        case 14:
            CmdSourceValue(2500);    // 设置S3欧姆 250
            msdelay(200);
            break;
        case 15:
            CmdRemote(0);             //退出远程
            msdelay(200);
            break;
        case 16:
            CmdRemote(1);             //进入远程
            msdelay(200);
            break;
        case 17:
            CmdSourceOpen(0);        //关闭输出
            msdelay(200);
            break;
        case 18:
            CmdSourceOpen(1);        //打开输出
            msdelay(200);
            break;  
        case 19:
            CmdMeasureType(0);        //测量S3 24Vloop
            msdelay(200);
            break;  
        case 20:
            CmdMeasureType(1);        //测量S3 V
            msdelay(200);
            break;   
        case 21:
            CmdMeasureType(2);        //测量S3 mV
            msdelay(200);
            break;   
        case 22:
            CmdMeasureType(3);        //测量S3 mA
            msdelay(200);
            break;   
        case 23:
            CmdMeasureType(4);        //测量S3 欧姆
            msdelay(200);
            break;   
        case 24:
            CmdMeasureOpen(0);        //关闭测量显示
            msdelay(200);
            break;     
        case 25:
            CmdMeasureOpen(1);        //打开测量显示
            msdelay(200);
            break;     
        case 26:
            CmdSourceValue(2100);    // 设置S3电压 21.00mV
            msdelay(200);
            break;            
        default:;
    }

}