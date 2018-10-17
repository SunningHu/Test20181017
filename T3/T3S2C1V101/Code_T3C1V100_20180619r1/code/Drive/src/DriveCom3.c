#include "DriveCom3.h"
#include "FunctionDoData.h"
extern Flag flag;
extern Value value;
extern Limit limit;

u8  g_cUsart3Buf[100];		//���ڻ���
u8  g_ucUsart3Len = 0;		//���ڽ��յ������ݳ���



//Summary:    ����3��ʼ������CA150У����ͨѶ
void Usart3_Init(void)
{
    //USART3ʱ�ӿ���
    CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE);
    
    //PG0,PG1����ʹ��
    GPIO_ExternalPullUpConfig(GPIOG, GPIO_Pin_0, ENABLE);
    GPIO_ExternalPullUpConfig(GPIOG, GPIO_Pin_1, ENABLE);
    
    
    //����USART3������9600,8Bit,1 stop, ��У�飬ʹ�ܷ���+����
    USART_Init(USART3, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
    
    //ʹ�ܽ����ж�
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
   
    //ʹ�ܴ���3
    USART_Cmd(USART3, ENABLE);
    
}

//Summary:    ����3����
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

//Summary:    ����3����
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



//Summary:    ����3����
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

//Summary:    ����3���ݴ���
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

//Summary:    У����mA�ź��ַ���תfloat
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

//Summary:    У����mV�ź��ַ���תfloat
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
* Summary:    ���ò�������
* Parameters:
*             cmd   ���ʹ���    0/24vloop 1/V 2/mV 3/mA 4/ŷķ
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
    
  
  g_cUsart3Buf[0] = 0x02;         // ��ַ
  g_cUsart3Buf[1] = 0x12;         // ����
  g_cUsart3Buf[2] = 0x01;         // ����
  g_cUsart3Buf[3] = 0x00;         // 00

  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[5] = u16Crc / 0x100;
  g_cUsart3Buf[6] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
//    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
    
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
  g_ucUsart3Len = 7;
 
  if(cmd < 2)
    g_cUsart3Buf[4] = cmd;
  else
    g_cUsart3Buf[4] = 1;

  
  g_cUsart3Buf[0] = 0x02;         // ��ַ
  g_cUsart3Buf[1] = 0x12;         // ����
  g_cUsart3Buf[2] = 0x02;         // ����
  g_cUsart3Buf[3] = 0x00;         // 00

  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[5] = u16Crc / 0x100;
  g_cUsart3Buf[6] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
//    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
}


/********************************************************************
* Summary:    ����S3�������
* Parameters:
*             cmd  0/24vloop 1/V 2/mV 3/mA 4/ŷķ 
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

  
  g_cUsart3Buf[0] = 0x02;         // ��ַ
  g_cUsart3Buf[1] = 0x11;         // ����
  g_cUsart3Buf[2] = 0x01;         // ����
  g_cUsart3Buf[3] = 0x00;         // 00

  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[5] = u16Crc / 0x100;
  g_cUsart3Buf[6] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
//    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
}


/********************************************************************
* Summary:    ����S3���ֵ
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
  

  
  g_cUsart3Buf[0] = 0x02;         // ��ַ
  g_cUsart3Buf[1] = 0x11;         // ����
  g_cUsart3Buf[2] = 0x02;         // ����
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
* Summary:    �����
* Parameters:
*             cmd  0�ر�  1��
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
    
  g_cUsart3Buf[0] = 0x02;         // ��ַ
  g_cUsart3Buf[1] = 0x11;         // ����
  g_cUsart3Buf[2] = 0x03;         // ����
  g_cUsart3Buf[3] = 0x00;         // 00


  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[5] = u16Crc / 0x100;
  g_cUsart3Buf[6] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
 //   memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
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
  g_ucUsart3Len = 5;
 
  if(cmd < 2)
    g_cUsart3Buf[2] = cmd;
  else
    g_cUsart3Buf[2] = 0;
    
  g_cUsart3Buf[0] = 0x02;         // ��ַ
  g_cUsart3Buf[1] = 0x10;         // ����



  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[3] = u16Crc / 0x100;
  g_cUsart3Buf[4] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
//    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
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
  g_ucUsart3Len = 5;
 
    
  g_cUsart3Buf[0] = 0x02;         // ��ַ
  g_cUsart3Buf[1] = 0x13;         // ����
  g_cUsart3Buf[2] = 0x01;


  u16Crc = crc16(g_cUsart3Buf, g_ucUsart3Len - 2);
  g_cUsart3Buf[3] = u16Crc / 0x100;
  g_cUsart3Buf[4] = u16Crc % 0x100;
  
    Usart3_Send(g_cUsart3Buf, g_ucUsart3Len);
//    memset(g_cUsart3Buf, 0, 100 * sizeof(char));
//    g_ucUsart3Len = 0;
}



//Summary:    ����3��������
void Usart3_SendCmd_Test(u8 cmd)
{
    switch(cmd)
    {
        case 0:
            CmdCheckData();           //�鿴����
            msdelay(200);
            break;
        case 1:
            CmdSourceType(1);         //����S3�������  V
            msdelay(200);
            break;
        case 2:
            CmdSourceType(2);         //����S3�������  mV
            msdelay(200);
            break;
        case 3:
            CmdSourceType(3);         //����S3�������  mA
            msdelay(200);
            break;
        case 4:
            CmdSourceType(4);         //����S3�������  ŷķ
            msdelay(200);
            break;
        case 5:
            CmdSourceValue(3000);    // ����S3��ѹ 3.000V
            msdelay(200);
            break;
        case 6:
            CmdSourceValue(10000);    // ����S3��ѹ 10.000V
            msdelay(200);
            break;
        case 7:
            CmdSourceValue(200);    // ����S3��ѹ 2.00mV
            msdelay(200);
            break;
        case 8:
            CmdSourceValue(1800);    // ����S3��ѹ 18.00mV
            msdelay(200);
            break;
        case 9:
            CmdSourceValue(1000);    // ����S3��ѹ 10.00mV
            msdelay(200);
            break;
        case 10:
            CmdSourceValue(9000);    // ����S3��ѹ 90.00mV
            msdelay(200);
            break;
        case 11:
            CmdSourceValue(2000);    // ����S3���� 2.000mA
            msdelay(200);
            break;
        case 12:
            CmdSourceValue(21000);    // ����S3���� 21.000mA
            msdelay(200);
            break;
        case 13:
            CmdSourceValue(500);     // ����S3ŷķ 50.0
            msdelay(200);
            break;
        case 14:
            CmdSourceValue(2500);    // ����S3ŷķ 250
            msdelay(200);
            break;
        case 15:
            CmdRemote(0);             //�˳�Զ��
            msdelay(200);
            break;
        case 16:
            CmdRemote(1);             //����Զ��
            msdelay(200);
            break;
        case 17:
            CmdSourceOpen(0);        //�ر����
            msdelay(200);
            break;
        case 18:
            CmdSourceOpen(1);        //�����
            msdelay(200);
            break;  
        case 19:
            CmdMeasureType(0);        //����S3 24Vloop
            msdelay(200);
            break;  
        case 20:
            CmdMeasureType(1);        //����S3 V
            msdelay(200);
            break;   
        case 21:
            CmdMeasureType(2);        //����S3 mV
            msdelay(200);
            break;   
        case 22:
            CmdMeasureType(3);        //����S3 mA
            msdelay(200);
            break;   
        case 23:
            CmdMeasureType(4);        //����S3 ŷķ
            msdelay(200);
            break;   
        case 24:
            CmdMeasureOpen(0);        //�رղ�����ʾ
            msdelay(200);
            break;     
        case 25:
            CmdMeasureOpen(1);        //�򿪲�����ʾ
            msdelay(200);
            break;     
        case 26:
            CmdSourceValue(2100);    // ����S3��ѹ 21.00mV
            msdelay(200);
            break;            
        default:;
    }

}