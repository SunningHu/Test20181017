#include "FunctionDoData.h"
#include "DriveCom1.h"
#include "DriveCom2.h"
#include "DriveCom3.h"
#include <stdlib.h>

Flag flag;
Value value;
Limit limit;

//Summary:    微秒级延迟计算
void usdelay(u16 us)
{    
    while(us > 0)
    {
        us --;
        __no_operation();
        __no_operation();
        __no_operation();
        __no_operation();
        __no_operation();
        __no_operation();
        __no_operation();
        __no_operation();
        __no_operation();
        __no_operation();
    }
}

//Summary:    毫秒级延迟计算
void msdelay(u16 ms)
{
    while(ms > 0)
    {
        ms --;
        usdelay(999);
        if(value.g_bStart == 0)
        {
            RelayInit();
            break;
        }
    }
}

//Summary:    毫秒级延迟计算
void sdelay(u16 s)
{
    while(s > 0)
    {
        s --;
        msdelay(999);
        if(value.g_bStart == 0)
        {
            RelayInit();
            break;
        }
    }
}
//Summary:    CRC校验码计算
u16 crc16(volatile u8 *puchMsg, u8 usDataLen) 
{
    u16 uCRC16  = 0xffff;
    u16 uIndex = 0;
    u16 i =0;

    for (i=0; i < usDataLen; i++)
    {
        uIndex = ((uCRC16 & 0xff) ^ (puchMsg[i] & 0xff));
        uCRC16 = ((uCRC16 >> 8) & 0xff) ^ uCRC16Table[uIndex];
    }
    return (uCRC16 >> 8) | (uCRC16 << 8);
}

/********************************************************************
* Summary:    s32装float
* Parameters:
*             s32Data  s32输入
*       
* Return :    float 
********************************************************************/
float S32tofloat(s32 s32Data)
{
   float fData = 0.0;
   switch(value.g_bCaliPoint)
   {
     case 1:
     case 2:
     case 3:
     case 4:
     case 9:
     case 10:
       fData = ((float)s32Data) / 1000.0;break;    
     case 5:
     case 6:
     case 7:
     case 8:       
     case 13:
     case 14:       
     case 15:
     case 16:       
     case 19:
     case 20:      
       fData = ((float)s32Data) / 100.0;break;
     case 11:
     case 12:
     case 17:
     case 18:
     case 21:
     case 22:
       fData = ((float)s32Data) / 10.0;break;
   default:
       fData = 0.0;break;      
   }
   
   return fData;
}


/********************************************************************
* Summary:    s32转float
* Parameters:
*             s32Data  s32输入   dotNUm 小数位数
*       
* Return :    float 
********************************************************************/
float S32tofloatCheck(s32 s32Data,u8 dotNUm)
{
   float fData = 0.0;
   switch(dotNUm)
   {
     case 3:
       fData = ((float)s32Data) / 1000.0;break;    
     case 2:   
       fData = ((float)s32Data) / 100.0;break;
     case 1:
       fData = ((float)s32Data) / 10.0;break;
   default:
       fData = 0.0;break;      
   }
   
   return fData;
}


/********************************************************************
* Summary:    冷端检测
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void PTestTemp(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart2_SendCmd_Test(18);//进入S2远程
    Usart2_SendCmd_Test(18);//进入S2远程    
    Usart2_SendCmd_Test(18);//进入S2远程       
    msdelay(400);
    Usart2_SendCmd_Test(0);//读取s1/s2 冷端温度
    msdelay(100);
    s32data[0] = value.s32TestTemp;
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart2_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestTemp;
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 50 && (s32data[i] - s32data[i-1]) >= -50)
        {
            if(s32data[i] >= -100 && s32data[i] <= 500)
            {
                j++; 

            }   
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
       value.u8CailPointFlag[value.g_bCaliPoint] = 1;
        
    }
    else
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_u16ResultMeter = 2;
    }
     value.g_bCaliPoint = 1;
}





/********************************************************************
* Summary:    有源2mA输出
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void PTestAOutDown(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程    
    Usart3_SendCmd_Test(22);//设置S3测量mA
    Usart2_SendCmd_Test(18);//进入S2远程
    Usart2_SendCmd_Test(1);//设置S2为mA输出
    Usart2_SendCmd_Test(13);//设置S2为输出 2.00mA
    Usart3_SendCmd_Test(16);//进入S3远程    
    Usart3_SendCmd_Test(22);//设置S3测量mA
    Usart2_SendCmd_Test(18);//进入S2远程
    Usart2_SendCmd_Test(1);//设置S2为mA输出
    Usart2_SendCmd_Test(13);//设置S2为输出 2.00mA    
    Usart2_SendCmd_Test(20);//打开S2输出
    Usart2_SendCmd_Test(20);//打开S2输出    
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32PTestAOut[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32PTestAOut[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 20 && (s32data[i] - s32data[i-1]) >= -20)
        {
            if(s32data[i] >= 1980 && s32data[i] <= 2020)
            {
                j++; 

            }   
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 1;
    }
    else
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_u16ResultMeter = 2;
    }
     value.g_bCaliPoint = 2;
}


/********************************************************************
* Summary:    有源21mA输出
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void PTestAOutUp(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(22);//设置S3测量mA
    Usart2_SendCmd_Test(18);//进入S2远程
    Usart2_SendCmd_Test(1);//设置S2为mA输出
    Usart2_SendCmd_Test(14);//设置S2为输出 21.00mA
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(22);//设置S3测量mA
    Usart2_SendCmd_Test(18);//进入S2远程
    Usart2_SendCmd_Test(1);//设置S2为mA输出
    Usart2_SendCmd_Test(14);//设置S2为输出 21.00mA
    Usart2_SendCmd_Test(20);//打开S2输出    
    Usart2_SendCmd_Test(20);//打开S2输出
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32PTestAOut[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32PTestAOut[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 20 && (s32data[i] - s32data[i-1]) >= -20)
        {
            if(s32data[i] >= 20980 && s32data[i] <= 21020)
            {
                j++; 
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
       value.u8CailPointFlag[value.g_bCaliPoint] = 1;
    }
    else
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
        value.g_u16ResultMeter = 2;
    }
    value.g_bCaliPoint = 3;
}



/********************************************************************
* Summary:    无源2mA输出
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void NTestAOutDown(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(19);//设置S3测量24VLOOP
    Usart2_SendCmd_Test(18);//进入S2远程
    Usart2_SendCmd_Test(2);//设置S2为无源mA输出
    Usart2_SendCmd_Test(13);//设置S2为输出 2.00mA
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(19);//设置S3测量24VLOOP
    Usart2_SendCmd_Test(18);//进入S2远程
    Usart2_SendCmd_Test(2);//设置S2为无源mA输出
    Usart2_SendCmd_Test(13);//设置S2为输出 2.00mA    
    Usart2_SendCmd_Test(20);//打开S2输出
    Usart2_SendCmd_Test(20);//打开S2输出    
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32NTestAOut[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32NTestAOut[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 20 && (s32data[i] - s32data[i-1]) >= -20)
        {
            if(s32data[i] >= 1980 && s32data[i] <= 2020)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 1;      

    }
    else
    {
       value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
       value.g_u16ResultMeter = 2;
    }
    
    value.g_bCaliPoint = 4;   
}



/********************************************************************
* Summary:    无源21mA输出
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void NTestAOutUp(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(19);//设置S3测量mA
    Usart2_SendCmd_Test(18);//进入S2远程
    Usart2_SendCmd_Test(2);//设置S2为无源mA输出
    Usart2_SendCmd_Test(14);//设置S2为输出 2.00mA
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(19);//设置S3测量mA
    Usart2_SendCmd_Test(18);//进入S2远程
    Usart2_SendCmd_Test(2);//设置S2为无源mA输出
    Usart2_SendCmd_Test(14);//设置S2为输出 2.00mA    
    Usart2_SendCmd_Test(20);//打开S2输出
    Usart2_SendCmd_Test(20);//打开S2输出    
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32NTestAOut[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32NTestAOut[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 20 && (s32data[i] - s32data[i-1]) >= -20)
        {
            if(s32data[i] >= 20980 && s32data[i] <= 21020)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
       value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
       value.g_u16ResultMeter = 2;
    }
    value.g_bCaliPoint = 5;
}


/********************************************************************
* Summary:    2mV输出
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestVOut20Down(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(21); //设置S3测量mV
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(4);  //设置S2为20mV输出
    Usart2_SendCmd_Test(9);  //设置S2为输出 2.00mV
    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(21); //设置S3测量mV
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(4);  //设置S2为20mV输出
    Usart2_SendCmd_Test(9);  //设置S2为输出 2.00mV
    Usart2_SendCmd_Test(20); //打开S2输出    
    Usart2_SendCmd_Test(20); //打开S2输出
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    
    s32data[0] = value.s32TestVOut20[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut20[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 3 && (s32data[i] - s32data[i-1]) >= -3)
        {
            if(s32data[i] >= 197 && s32data[i] <= 203)
            {
                j++; 
            }
            else
            { 
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
         value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
       value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
       value.g_u16ResultMeter = 2;
    }
    
    value.g_bCaliPoint = 6;    
}


/********************************************************************
* Summary:    18mV输出
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestVOut20Up(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(21); //设置S3测量mV
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(4);  //设置S2为20mV输出
    Usart2_SendCmd_Test(10);  //设置S2为输出 18.00mV
    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(21); //设置S3测量mV
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(4);  //设置S2为20mV输出
    Usart2_SendCmd_Test(10);  //设置S2为输出 18.00mV
    Usart2_SendCmd_Test(20); //打开S2输出    
    Usart2_SendCmd_Test(20); //打开S2输出
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    
    s32data[0] = value.s32TestVOut20[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut20[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 3 && (s32data[i] - s32data[i-1]) >= -3)
        {
            if(s32data[i] >= 1797 && s32data[i] <= 1803)
            {
                j++; 
            }
            else
            { 
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
         value.u8CailPointFlag[value.g_bCaliPoint] = 1;

    }
    else
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
        value.g_u16ResultMeter = 2;
    }
    value.g_bCaliPoint = 7;    
}


/********************************************************************
* Summary:    10mV输出
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestVOut100Down(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(21); //设置S3测量mV
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(5);  //设置S2为100mV输出
    Usart2_SendCmd_Test(11);  //设置S2为输出 10.0mV
    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(21); //设置S3测量mV
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(5);  //设置S2为100mV输出
    Usart2_SendCmd_Test(11);  //设置S2为输出 10.0mV
    Usart2_SendCmd_Test(20); //打开S2输出    
    Usart2_SendCmd_Test(20); //打开S2输出
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    
    s32data[0] = value.s32TestVOut100[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut100[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 10 && (s32data[i] - s32data[i-1]) >= -10)
        {
            if(s32data[i] >= 990 && s32data[i] <= 1010)
            {
                j++; 
            }
            else
            { 
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
         value.u8CailPointFlag[value.g_bCaliPoint] = 1;

    }
    else
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
        value.g_u16ResultMeter = 2;
    }
     value.g_bCaliPoint = 8;    
}


/********************************************************************
* Summary:    90mV输出
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestVOut100Up(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(21); //设置S3测量mV
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(5);  //设置S2为100mV输出
    Usart2_SendCmd_Test(12);  //设置S2为输出 90.0mV
    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(21); //设置S3测量mV
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(5);  //设置S2为100mV输出
    Usart2_SendCmd_Test(12);  //设置S2为输出 90.0mV
    Usart2_SendCmd_Test(20); //打开S2输出    
    Usart2_SendCmd_Test(20); //打开S2输出
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    
    s32data[0] = value.s32TestVOut100[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut100[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 10 && (s32data[i] - s32data[i-1]) >= -10)
        {
            if(s32data[i] >= 8990 && s32data[i] <= 9010)
            {
                j++; 
            }
            else
            { 
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    { 
          value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
         value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
         value.g_u16ResultMeter = 2;
    }
    
    value.g_bCaliPoint = 9;    
}


/********************************************************************
* Summary:    0.00V输出
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestVOutDown(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(20); //设置S3测量V
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(3);  //设置S2为V输出
    Usart2_SendCmd_Test(7);  //设置S2为输出 0.00V
    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(20); //设置S3测量V
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(3);  //设置S2为V输出
    Usart2_SendCmd_Test(7);  //设置S2为输出 0.00V
    Usart2_SendCmd_Test(20); //打开S2输出    
    Usart2_SendCmd_Test(20); //打开S2输出
    sdelay(10);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    
    s32data[0] = value.s32TestVOut[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 20 && (s32data[i] - s32data[i-1]) >= -20)
        {
            if(s32data[i] >= -20 && s32data[i] <= 20)
            {
                j++; 
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
          value.u8CailPointFlag[value.g_bCaliPoint] = 1;      

    }
    else
    {
          value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
          value.g_u16ResultMeter = 2;
    }
     value.g_bCaliPoint = 10;    
}

/********************************************************************
* Summary:    15.00V输出
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestVOutUp(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(20); //设置S3测量V
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(3);  //设置S2为V输出
    Usart2_SendCmd_Test(8);  //设置S2为输出 15.00V
    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(20); //设置S3测量V
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(3);  //设置S2为V输出
    Usart2_SendCmd_Test(8);  //设置S2为输出 15.00V
    Usart2_SendCmd_Test(20); //打开S2输出    
    Usart2_SendCmd_Test(20); //打开S2输出
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    
    s32data[0] = value.s32TestVOut[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 20 && (s32data[i] - s32data[i-1]) >= -20)
        {
            if(s32data[i] >= 14980 && s32data[i] <= 15020)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
       value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
      value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
      value.g_u16ResultMeter = 2;
    }
      if(value.g_productType == 0)        
         value.g_bCaliPoint = 13;
      else
        value.g_bCaliPoint = 11;    
}

/********************************************************************
* Summary:    50.0欧姆输出
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestROutDown(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(23); //设置S3测量欧姆
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(26); //关闭S2测量
    Usart2_SendCmd_Test(6);  //设置S2为欧姆输出
    Usart2_SendCmd_Test(15);  //设置S2为输出 50.0欧姆
    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(23); //设置S3测量欧姆
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(26); //关闭S2测量
    Usart2_SendCmd_Test(6);  //设置S2为欧姆输出
    Usart2_SendCmd_Test(15);  //设置S2为输出 50.0欧姆
    Usart2_SendCmd_Test(20); //打开S2输出    
    Usart2_SendCmd_Test(20); //打开S2输出
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    
    s32data[0] = value.s32TestROut[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestROut[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
            if(s32data[i] >= 496 && s32data[i] <= 504)
            {
                j++; 
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
        value.g_u16ResultMeter = 2;
    }
    value.g_bCaliPoint = 12;

}

/********************************************************************
* Summary:    250.0欧姆输出
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestROutUp(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(23); //设置S3测量欧姆
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(26); //关闭S2测量
    Usart2_SendCmd_Test(6);  //设置S2为欧姆输出
    Usart2_SendCmd_Test(16);  //设置S2为输出 250欧姆
    Usart3_SendCmd_Test(16); //进入S3远程
    Usart3_SendCmd_Test(23); //设置S3测量欧姆
    Usart2_SendCmd_Test(18); //进入S2远程
    Usart2_SendCmd_Test(26); //关闭S2测量
    Usart2_SendCmd_Test(6);  //设置S2为欧姆输出
    Usart2_SendCmd_Test(16);  //设置S2为输出 250欧姆
    Usart2_SendCmd_Test(20); //打开S2输出    
    Usart2_SendCmd_Test(20); //打开S2输出
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    
    s32data[0] = value.s32TestROut[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestROut[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
            if(s32data[i] >= 2496 && s32data[i] <= 2504)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
         value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
       value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
       value.g_u16ResultMeter = 2;
    }
     value.g_bCaliPoint = 13;
}







/********************************************************************
* Summary:    2mA测试
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestAInDown(void)
{
//  for(int i=0;i<10;i++)
//  {
//          Usart2_SendCmd_Test(0);//读取校验仪测量值
//       msdelay(1000);
//      value.g_fMacData = S32tofloat(value.s32TestAIn[0]);
//  }
//  value.g_bStart = 0;
       
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(21);  //设置S2为测量mA
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(3);   //设置S3输出mA
    Usart3_SendCmd_Test(11);  //设置S3输出 2.000mA  
    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(21);  //设置S2为测量mA
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(3);   //设置S3输出mA
    Usart3_SendCmd_Test(11);  //设置S3输出 2.000mA  
    Usart3_SendCmd_Test(18);  //打开输出     
    Usart3_SendCmd_Test(18);  //打开输出 
    
    sdelay(3);
    Usart2_SendCmd_Test(0);//读取校验仪测量值
    
    s32data[0] = value.s32TestAIn[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart2_SendCmd_Test(0);//mA测量读
        s32data[i] = value.s32TestAIn[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 2 && (s32data[i] - s32data[i-1]) >= -2)
        {
            if(s32data[i] >= 198 && s32data[i] <= 202)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {    
         value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
         value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
         value.g_u16ResultMeter = 2;
    }
    value.g_bCaliPoint = 14; 
}

/********************************************************************
* Summary:    21mA测试
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestAInUp(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    
    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(21);  //设置S2为测量mA
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(3);   //设置S3输出mA
    Usart3_SendCmd_Test(12);  //设置S3输出 21.000mA  
    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(21);  //设置S2为测量mA
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(3);   //设置S3输出mA
    Usart3_SendCmd_Test(12);  //设置S3输出 21.000mA  
    Usart3_SendCmd_Test(18);  //打开输出     
    Usart3_SendCmd_Test(18);  //打开输出 
    
    sdelay(3);
    Usart2_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32TestAIn[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart2_SendCmd_Test(0);//读取S2测量值
        s32data[i] = value.s32TestAIn[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 2 && (s32data[i] - s32data[i-1]) >= -2)
        {
            if(s32data[i] >= 2098 && s32data[i] <= 2102)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
          value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
         value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
         value.g_u16ResultMeter = 2;
    }
     value.g_bCaliPoint = 15; 
}


/********************************************************************
* Summary:    2mV测试
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestVIn20Down(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(23);  //设置S2为测量mV
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(2);   //设置S3输出mV
    Usart3_SendCmd_Test(7);  //设置S3输出 2.00mV
     Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(23);  //设置S2为测量mV
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(2);   //设置S3输出mV
    Usart3_SendCmd_Test(7);  //设置S3输出 2.00mV
    Usart3_SendCmd_Test(18);  //打开输出    
    Usart3_SendCmd_Test(18);  //打开输出 
    
    sdelay(3);
    Usart2_SendCmd_Test(0);//读取S2测量值
    
    s32data[0] = value.s32TestVIn20[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart2_SendCmd_Test(0);//读取S2测量值
        s32data[i] = value.s32TestVIn20[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 3 && (s32data[i] - s32data[i-1]) >= -3)
        {
            if(s32data[i] >= 197 && s32data[i] <= 203)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
        value.g_u16ResultMeter = 2;
    }
    value.g_bCaliPoint = 16;     
}

/********************************************************************
* Summary:    18mV测试
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestVIn20Up(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(23);  //设置S2为测量mV
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(2);   //设置S3输出mV
    Usart3_SendCmd_Test(8);  //设置S3输出 18.00mV
    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(23);  //设置S2为测量mV
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(2);   //设置S3输出mV
    Usart3_SendCmd_Test(8);  //设置S3输出 18.00mV
    Usart3_SendCmd_Test(18);  //打开输出     
    Usart3_SendCmd_Test(18);  //打开输出     
    sdelay(3);
    Usart2_SendCmd_Test(0);//读取S2测量值
    
    s32data[0] = value.s32TestVIn20[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart2_SendCmd_Test(0);//读取S2测量值
        s32data[i] = value.s32TestVIn20[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 3 && (s32data[i] - s32data[i-1]) >= -3)
        {
            if(s32data[i] >= 1797 && s32data[i] <= 1803)
            {
                j++;
            }
            else
            {
                j = 0;
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
       value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
       value.g_u16ResultMeter = 2;
    }
    value.g_bCaliPoint = 17;    
}

/********************************************************************
* Summary:    10mV测试
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestVIn100Down(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(24);  //设置S2为测量mV
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(2);   //设置S3输出mV
    Usart3_SendCmd_Test(9);  //设置S3输出 10.00mV
    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(24);  //设置S2为测量mV
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(2);   //设置S3输出mV
    Usart3_SendCmd_Test(9);  //设置S3输出 10.00mV
    Usart3_SendCmd_Test(18);  //打开输出     
    Usart3_SendCmd_Test(18);  //打开输出 
    sdelay(3);
    Usart2_SendCmd_Test(0);//读取S2测量值
    
    s32data[0] = value.s32TestVIn100[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100); 
        Usart2_SendCmd_Test(0);//读取S2测量值
        s32data[i] = value.s32TestVIn100[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 1 && (s32data[i] - s32data[i-1]) >= -1)
        {
            if(s32data[i] >= 99 && s32data[i] <= 101)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
         value.u8CailPointFlag[value.g_bCaliPoint] = 1;
    }
    else
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
        value.g_u16ResultMeter = 2;
    }
    value.g_bCaliPoint = 18;   
}

/********************************************************************
* Summary:    90mV测试
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestVIn100Up(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(24);  //设置S2为测量100mV
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(2);   //设置S3输出mV
    Usart3_SendCmd_Test(10);  //设置S3输出 90.00mV
    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(24);  //设置S2为测量100mV
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(2);   //设置S3输出mV
    Usart3_SendCmd_Test(10);  //设置S3输出 90.00mV
    Usart3_SendCmd_Test(18);  //打开输出     
    Usart3_SendCmd_Test(18);  //打开输出 
    sdelay(3);
    Usart2_SendCmd_Test(0);//读取S2测量值
    
    s32data[0] = value.s32TestVIn100[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart2_SendCmd_Test(0);//读取S2测量值
        s32data[i] = value.s32TestVIn100[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 1 && (s32data[i] - s32data[i-1]) >= -1)
        {
            if(s32data[i] >= 899 && s32data[i] <= 901)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
         value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
         value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
         value.g_u16ResultMeter = 2;
    }
    value.g_bCaliPoint = 19;    
}

/********************************************************************
* Summary:    3V测试
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestVInDown(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(22);  //设置S2为测量V
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(1);   //设置S3输出mV
    Usart3_SendCmd_Test(5);  //设置S3输出 3.00V
    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(22);  //设置S2为测量V
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(1);   //设置S3输出mV
    Usart3_SendCmd_Test(5);  //设置S3输出 3.00V
    Usart3_SendCmd_Test(18);  //打开输出     
    Usart3_SendCmd_Test(18);  //打开输出 
    sdelay(3);
    Usart2_SendCmd_Test(0);//读取S2测量值
    
    s32data[0] = value.s32TestVIn[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart2_SendCmd_Test(0);//读取S2测量值
        s32data[i] = value.s32TestVIn[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 3 && (s32data[i] - s32data[i-1]) >= -3)
        {
             if(s32data[i] >= 297 && s32data[i] <= 303)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
        value.g_u16ResultMeter = 2;
    }
    value.g_bCaliPoint = 20;    
}

/********************************************************************
* Summary:    10V测试
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestVInUp(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(22);  //设置S2为测量V
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(1);   //设置S3输出mV
    Usart3_SendCmd_Test(6);  //设置S3输出 10.00V
    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(22);  //设置S2为测量V
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(1);   //设置S3输出mV
    Usart3_SendCmd_Test(6);  //设置S3输出 10.00V
    Usart3_SendCmd_Test(18);  //打开输出     
    Usart3_SendCmd_Test(18);  //打开输出 
    sdelay(3);
    Usart2_SendCmd_Test(0);//读取S2测量值
    
    s32data[0] = value.s32TestVIn[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart2_SendCmd_Test(0);//读取S2测量值
        s32data[i] = value.s32TestVIn[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 3 && (s32data[i] - s32data[i-1]) >= -3)
        {
             if(s32data[i] >= 997 && s32data[i] <= 1003)
            {
                j++;
            }
            else
            { 
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
       value.u8CailPointFlag[value.g_bCaliPoint] = 1;  
      if(value.g_productType == 0)
      {
        if(value.g_u16ResultMeter == 0)
          value.g_u16ResultMeter = 1;
  
      }
       else 
         value.g_bCaliPoint = 21;
    }
    else
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;     
      if(value.g_productType == 0)
      {
        value.g_u16ResultMeter = 2;
      }
       else 
         value.g_bCaliPoint = 21;        
               
    }
    
    if(value.g_productType == 0)
    {
        value.g_bStart = 0;
        value.g_bCaliFinish = 1;
    }
    Usart3_SendCmd_Test(17);// 关闭输出 
}


/********************************************************************
* Summary:    50欧姆测试
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestRInDown(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(25);  //设置S2为测量欧姆
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(4);   //设置S3输出欧姆
    Usart3_SendCmd_Test(13);  //设置S3输出 50.0欧姆
    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(25);  //设置S2为测量欧姆
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(4);   //设置S3输出欧姆
    Usart3_SendCmd_Test(13);  //设置S3输出 50.0欧姆
    Usart3_SendCmd_Test(18);  //打开输出     
    Usart3_SendCmd_Test(18);  //打开输出 
    sdelay(3);
    Usart2_SendCmd_Test(0);//读取S2测量值
    
    s32data[0] = value.s32TestRIn[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart2_SendCmd_Test(0);//读取S2测量值
        s32data[i] = value.s32TestRIn[0];
         value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
             if(s32data[i] >= 496 && s32data[i] <= 504)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 1;      
    }
    else
    {
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
        value.g_u16ResultMeter = 2;
    }
    value.g_bCaliPoint = 22;   
}

/********************************************************************
* Summary:    250欧姆测试
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void TestRInUp(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];

    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(25);  //设置S2为测量欧姆
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(4);   //设置S3输出欧姆
    Usart3_SendCmd_Test(14);  //设置S3输出 250.0欧姆
    Usart2_SendCmd_Test(18);  //进入S2远程
    Usart2_SendCmd_Test(19);  //关闭S2输出
    Usart2_SendCmd_Test(27);  //打开S2测量
    Usart2_SendCmd_Test(25);  //设置S2为测量欧姆
    
    Usart3_SendCmd_Test(16);  //进入S3远程
    Usart3_SendCmd_Test(4);   //设置S3输出欧姆
    Usart3_SendCmd_Test(14);  //设置S3输出 250.0欧姆
    Usart3_SendCmd_Test(18);  //打开输出     
    Usart3_SendCmd_Test(18);  //打开输出 
    sdelay(3);
    Usart2_SendCmd_Test(0);//读取S2测量值
    
    s32data[0] = value.s32TestRIn[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        msdelay(100);
        Usart2_SendCmd_Test(0);//读取S2测量值
        s32data[i] = value.s32TestRIn[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
             if(s32data[i] >= 2496 && s32data[i] <= 2504)
            {
                j++;
            }
            else
            { 
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {
       value.u8CailPointFlag[value.g_bCaliPoint] = 1;   
       if(value.g_u16ResultMeter == 0)
          value.g_u16ResultMeter = 1;
      //   value.g_bCaliPoint = 23;
    }
    else
    {
         value.u8CailPointFlag[value.g_bCaliPoint] = 2;      
         value.g_u16ResultMeter = 2;
    }    
       Usart3_SendCmd_Test(17);// 关闭输出 
        value.g_bStart = 0;
        value.g_bCaliFinish = 1;

}


/********************************************************************
* Summary:    标定测试通讯
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void PCorrComm(void)
{
    Usart2_SendCmd_Test(17);
    Usart2_SendCmd(14);// 通讯测试 读版本型号
    msdelay(250);
    Usart2_SendCmd(14);// 通讯测试 读版本型号
    msdelay(250);
    Usart2_SendCmd(14);// 通讯测试 读版本型号
    msdelay(250);   
    Usart2_SendCmd(14);// 通讯测试 读版本型号
    msdelay(250);      
    flag.u8count++;
    if(flag.u8count >= 5)
    {
            value.u8CailPointFlag[value.g_bCaliPoint] = 2;  
            value.g_u16ResultMeter = 2;//标定失败
            flag.u8count = 0;//计数清零
            value.g_bStart = 0;
            value.g_bCaliFinish = 1;
    }
    
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(17);
    Usart3_SendCmd_Test(15);//进入S3远程
}


/**************)******************************************************
* Summary:    标定输出有源20mA下限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void PCorrAOutDown(void)
{
    u8 i;
    u8 j = 0;    
    s32 s32data[READ_MAXTIME];
    Usart2_SendCmd_Test(17); //S2退出远程
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(22);//设置S3测量mA
    Usart2_SendCmd(0);//有源20mA下限输出读
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(22);//设置S3测量mA
    Usart2_SendCmd(0);//有源20mA下限输出读    
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32PTestAOut[0];
    value.g_fMacData = S32tofloat(s32data[0]);     
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(0);//有源20mA下限输出读
        Usart2_SendCmd(0);//有源20mA下限输出读      
        Usart2_SendCmd(0);//有源20mA下限输出读             
        msdelay(200);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32PTestAOut[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
            if(s32data[i] >= 4000 && s32data[i] <= 5500)
            {
                j++; 
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    value.fPCorrAOut[0] =  S32tofloat(s32data[i]); 
    if(j >= limit.u16MaxOKCount)
    {          
        Cmd12Send(value.fPCorrAOut[0]);
        msdelay(400);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 2;             
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 2;        
    }
}


/********************************************************************
* Summary:    标定输出有源20mA上限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void PCorrAOutUp(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(22);//设置S3测量mA
    Usart2_SendCmd(1);//有源20mA上限输出读
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(22);//设置S3测量mA
    Usart2_SendCmd(1);//有源20mA上限输出读    
    sdelay(3);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32PTestAOut[1];
    value.g_fMacData = S32tofloat(s32data[0]);  
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(1);//有源20mA上限输出读
        Usart2_SendCmd(1);//有源20mA上限输出读        
        Usart2_SendCmd(1);//有源20mA上限输出读           
        msdelay(200);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32PTestAOut[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {  
            if(s32data[i] >= 18000 && s32data[i] <= 21000)
            {
                j++; 
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    value.fPCorrAOut[1] =  S32tofloat(s32data[i]); 
    if(j >= limit.u16MaxOKCount)
    {
        Cmd12Send(value.fPCorrAOut[1]);//S2输出标定写并发送命令 
        msdelay(400);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 3;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 3;            
    }
}


/********************************************************************
* Summary:    标定输出无源20mA下限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void NCorrAOutDown(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(19);//设置S3测量24VLOOP   
    Usart2_SendCmd(2);//无源20mA下限输出读
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(19);//设置S3测量24VLOOP   
    Usart2_SendCmd(2);//无源20mA下限输出读    
    msdelay(2000);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32NTestAOut[0];
    value.g_fMacData = S32tofloat(s32data[0]); 
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(2);//无源20mA下限输出读
        Usart2_SendCmd(2);//无源20mA下限输出读
        Usart2_SendCmd(2);//无源20mA下限输出读        
        msdelay(200);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32NTestAOut[0];
        value.g_fMacData = S32tofloat(s32data[i]); 
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
            if(s32data[i] >= 4000 && s32data[i] <= 5500)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    value.fNCorrAOut[0] =  S32tofloat(s32data[i]); 
    if(j >= limit.u16MaxOKCount)
    {

        Cmd12Send(value.fNCorrAOut[0]); //S2输出标定写并发送命令 
        msdelay(400);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 4;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 4;          
    }
}


/********************************************************************
* Summary:    标定输出无源20mA上限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void NCorrAOutUp(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(19);//设置S3测量24VLOOP   
    Usart2_SendCmd(3);//无源20mA上限输出读
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(19);//设置S3测量24VLOOP   
    Usart2_SendCmd(3);//无源20mA上限输出读    
    msdelay(2000);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32NTestAOut[1];
    value.g_fMacData = S32tofloat(s32data[0]); 
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(3);//无源20mA上限输出读
        Usart2_SendCmd(3);//无源20mA上限输出读
        Usart2_SendCmd(3);//无源20mA上限输出读        
        msdelay(200);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32NTestAOut[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
            if(s32data[i] >= 18000 && s32data[i] <= 21000)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    value.fNCorrAOut[1] =  S32tofloat(s32data[i]); 
    if(j >= limit.u16MaxOKCount)
    {

        Cmd12Send(value.fNCorrAOut[1]); //S2输出标定写并发送命令 + 修正
        msdelay(400);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 5;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 5;          
    }
}


/********************************************************************
* Summary:    标定输出20mV下限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrVOut20Down(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(21);//设置S3测量mv      
    Usart2_SendCmd(4);//20mV下限输出读
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(21);//设置S3测量mv      
    Usart2_SendCmd(4);//20mV下限输出读    
    msdelay(400);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32TestVOut20[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(4);//20mV下限输出读
        Usart2_SendCmd(4);//20mV下限输出读
        Usart2_SendCmd(4);//20mV下限输出读
        msdelay(200);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut20[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
            if(s32data[i] >= 420 && s32data[i] <= 570)
            {
                j++; 
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    value.fCorrVOut20[0] =  S32tofloat(s32data[i]); 
    if(j >= limit.u16MaxOKCount)
    {
        Cmd12Send(value.fCorrVOut20[0]);//S2输出标定写并发送命令 
        msdelay(400);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 6;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 6;          
    }
}


/********************************************************************
* Summary:    标定输出20mV上限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrVOut20Up(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(21);//设置S3测量mv   
    Usart2_SendCmd(5);//20mV上限输出读
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(21);//设置S3测量mv   
    Usart2_SendCmd(5);//20mV上限输出读    
    msdelay(400);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32TestVOut20[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(5);//20mV上限输出读
        Usart2_SendCmd(5);//20mV上限输出读
        Usart2_SendCmd(5);//20mV上限输出读        
        msdelay(200);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut20[1];
        value.g_fMacData =  S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
            if(s32data[i] >= 1900 && s32data[i] <= 2200)
            {
                j++; 
            }
            else
            { 
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    value.fCorrVOut20[1] =  S32tofloat(s32data[i]); 
    if(j >= limit.u16MaxOKCount)
    {
        Cmd12Send(value.fCorrVOut20[1]);//S2输出标定写并发送命令 
        msdelay(400);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 7;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 7;          
    }
}


/********************************************************************
* Summary:    标定输出100mV下限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrVOut100Down(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(21);//设置S3测量mv  
    Usart2_SendCmd(6);//100mV下限输出读
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(21);//设置S3测量mv  
    Usart2_SendCmd(6);//100mV下限输出读    
    msdelay(400);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32TestVOut100[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(6);//100mV下限输出读
        Usart2_SendCmd(6);//100mV下限输出读
        Usart2_SendCmd(6);//100mV下限输出读        
        msdelay(200);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut100[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
            if(s32data[i] >= 2250 && s32data[i] <= 2750)
            {
                j++;  
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    value.fCorrVOut100[0] =  S32tofloat(s32data[i]); 
    if(j >= limit.u16MaxOKCount)
    {
        Cmd12Send(value.fCorrVOut100[0]);//S2输出标定写并发送命令 + 修正
        msdelay(400);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 8;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 8;          
    }
}


/********************************************************************
* Summary:    标定输出100mV上限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrVOut100Up(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(21);//设置S3测量mv  
    Usart2_SendCmd(7);//100mV上限输出读
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(21);//设置S3测量mv  
    Usart2_SendCmd(7);//100mV上限输出读    
    msdelay(400);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32TestVOut100[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(7);//100mV上限输出读
        Usart2_SendCmd(7);//100mV上限输出读
        Usart2_SendCmd(7);//100mV上限输出读        
        msdelay(200);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut100[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
            if(s32data[i] >= 9350 && s32data[i] <= 10550)
            {
                j++;  
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    value.fCorrVOut100[1] =  S32tofloat(s32data[i]);
    if(j >= limit.u16MaxOKCount)
    {
        Cmd12Send(value.fCorrVOut100[1]);//S2输出标定写并发送命令 
        msdelay(400);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 9;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 9;          
    }
}

/********************************************************************
* Summary:    标定输出V下限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrVOutDown(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(20);//设置S3测量v  
    Usart2_SendCmd(8);//V下限输出读
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(20);//设置S3测量v  
    Usart2_SendCmd(8);//V下限输出读    
    msdelay(400);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32TestVOut[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(8);//V下限输出读
        Usart2_SendCmd(8);//V下限输出读
        Usart2_SendCmd(8);//V下限输出读        
        msdelay(200);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
            if(s32data[i] >= 1900 && s32data[i] <= 2350)
            {
                j++; 
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    value.fCorrVOut[0] =  S32tofloat(s32data[i]);
    if(j >= limit.u16MaxOKCount)
    {
        Cmd12Send(value.fCorrVOut[0]);     //S2输出标定写并发送命令 
        msdelay(400);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 10;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 10;          
    }
}



/********************************************************************
* Summary:    标定输出V上限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrVOutUp(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(20);//设置S3测量v  
    Usart2_SendCmd(9);//V上限输出读
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(20);//设置S3测量v  
    Usart2_SendCmd(9);//V上限输出读    
    msdelay(400);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32TestVOut[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(9);//V上限输出读
        Usart2_SendCmd(9);//V上限输出读
        Usart2_SendCmd(9);//V上限输出读        
        msdelay(200);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 4 && (s32data[i] - s32data[i-1]) >= -4)
        {
            if(s32data[i] >= 9550 && s32data[i] <= 10500)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    value.fCorrVOut[1] =  S32tofloat(s32data[i]);
    if(j >= limit.u16MaxOKCount)
    {
        Cmd12Send(value.fCorrVOut[1] + 0.006);           //S2输出标定写并发送命令 
        msdelay(400);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 11;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 11;          
    }
}


/********************************************************************
* Summary:    欧姆标定上限
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrROutDown(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(20);//设置S3测量mV 
    Usart2_SendCmd(8);//V下限输出读
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(20);//设置S3测量mV 
    Usart2_SendCmd(8);//V下限输出读    
    msdelay(400);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32TestVOut[0];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(8);//V下限输出读
        Usart2_SendCmd(8);//V下限输出读
        Usart2_SendCmd(8);//V下限输出读        
        msdelay(200);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut[0];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 2 && (s32data[i] - s32data[i-1]) >= -2)
        {
            if(s32data[i] >= 1950 && s32data[i] <= 2250)
            {
                j++; 
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    value.fCorrROut[0] =  S32tofloat(s32data[i]);
    if(j >= limit.u16MaxOKCount)
    {
        Cmd12Send(value.fCorrROut[0]);     //S2输出标定写并发送命令 
        msdelay(400);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
    }
}



/********************************************************************
* Summary:    欧姆标定上限
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrROutUp(void)
{
    u8 i;
    u8 j = 0;
    s32 s32data[READ_MAXTIME];
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(20);//设置S3测量v  
    Usart2_SendCmd(9);//V上限输出读
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(20);//设置S3测量v  
    Usart2_SendCmd(9);//V上限输出读    
    msdelay(400);
    Usart3_SendCmd_Test(0);//读取校验仪测量值
    s32data[0] = value.s32TestVOut[1];
    value.g_fMacData = S32tofloat(s32data[0]);
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(9);//V上限输出读
        Usart2_SendCmd(9);//V上限输出读
        Usart2_SendCmd(9);//V上限输出读        
        msdelay(200);
        Usart3_SendCmd_Test(0);//读取校验仪测量值
        s32data[i] = value.s32TestVOut[1];
        value.g_fMacData = S32tofloat(s32data[i]);
        if((s32data[i] - s32data[i-1]) <= 2 && (s32data[i] - s32data[i-1]) >= -2)
        {
            if(s32data[i] >= 9550 && s32data[i] <= 10500)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    value.fCorrROut[1] =  S32tofloat(s32data[i]);
    if(j >= limit.u16MaxOKCount)
    {
        Cmd12Send(value.fCorrROut[1]);           //S2输出标定写并发送命令 
        msdelay(400);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
    }
}



/********************************************************************
* Summary:    标定测量电流下限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrAInDown(void)
{
    u8 i;
    u8 j = 0;
    s16 s16data[READ_MAXTIME];
       
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(3);//设置S3输出mA
    Usart3_SendCmd_Test(11);//设置S3输出2.000mA    
   
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(3);//设置S3输出mA
    Usart3_SendCmd_Test(11);//设置S3输出2.000mA    
    Usart3_SendCmd_Test(18);// 打开输出        
    Usart3_SendCmd_Test(18);// 打开输出     
    msdelay(3000);    //等待信号发生器接收完命令后返回
    Usart2_SendCmd(10);//mA测量读
    msdelay(50);
    s16data[0] = (s16)(value.u16CorrAIn[0]);    
    value.g_u16MacData = (u16)s16data[0];
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(10);//mA测量读
        msdelay(200);
        s16data[i] = (s16)(value.u16CorrAIn[0]);
        value.g_u16MacData = (u16)s16data[i];
        if((s16data[i] - s16data[i-1]) <= 4 && (s16data[i] - s16data[i-1]) >= -4)
        {
            if(s16data[i] >= 290 && s16data[i] <= 360)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {       
        Cmd14Send(s16data[i]);//S2输出标定写并发送命令          
        msdelay(200);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 12;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 12;          
    }
}


/********************************************************************
* Summary:    标定测量电流上限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrAInUp(void)
{
    u8 i;
    u8 j = 0;
    s16 s16data[READ_MAXTIME];
    
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(3);//设置S3输出mA
    Usart3_SendCmd_Test(12);//设置S3输出21.000mA    
  
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(3);//设置S3输出mA
    Usart3_SendCmd_Test(12);//设置S3输出21.000mA    
    Usart3_SendCmd_Test(18);// 打开输出        
    Usart3_SendCmd_Test(18);// 打开输出      
    msdelay(3000);          //等待信号发生器接收完命令后返回
    Usart2_SendCmd(10);//mA测量读
    msdelay(50);
    s16data[0] = (s16)value.u16CorrAIn[1];
    value.g_u16MacData = (u16)s16data[0];
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(10);//mA测量读
        msdelay(200);
        s16data[i] = (s16)value.u16CorrAIn[1];
        value.g_u16MacData = (u16)s16data[i];
        if((s16data[i] - s16data[i-1]) <= 4 && (s16data[i] - s16data[i-1]) >= -4)
        {
            if(s16data[i] >= 3300 && s16data[i] <= 3600)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {   
        Cmd14Send(s16data[i]);   //S2输出标定写并发送命令            
        msdelay(200);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 13;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 13;          
    }
}


/********************************************************************
* Summary:    标定测量20mV下限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrVIn20Down(void)
{
    u8 i;
    u8 j = 0;
    s16 s16data[READ_MAXTIME];

    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(2);//设置S3输出mV
    Usart3_SendCmd_Test(7);// 设置S3输出2mV   
  
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(2);//设置S3输出mV
    Usart3_SendCmd_Test(7);// 设置S3输出2mV   
    Usart3_SendCmd_Test(18);// 打开输出     
    Usart3_SendCmd_Test(18);// 打开输出      
    msdelay(3000);  
    Usart2_SendCmd(11);//20mV测量读
    msdelay(50);
    s16data[0] = (s16)value.u16CorrVIn20[0];
    value.g_u16MacData = (u16)s16data[0];
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(11);//20mV测量读
        msdelay(200);
        s16data[i] = (s16)value.u16CorrVIn20[0];
        value.g_u16MacData = (u16)s16data[i];
        if((s16data[i] - s16data[i-1]) <= 4 && (s16data[i] - s16data[i-1]) >= -4)
        {
            if(s16data[i] >= 290 && s16data[i] <= 410)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {    
        Cmd14Send(s16data[i]);   //S2输出标定写并发送命令        
        msdelay(200);      
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 14;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 14;    
    }
}


/********************************************************************
* Summary:    标定测量20mV上限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrVIn20Up(void)
{
    u8 i;
    u8 j = 0;
    s16 s16data[READ_MAXTIME];

    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(2);//设置S3输出mV
    Usart3_SendCmd_Test(26);// 设置S3输出21mV   

    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(2);//设置S3输出mV
    Usart3_SendCmd_Test(26);// 设置S3输出21mV   
    Usart3_SendCmd_Test(18);// 打开输出    
    Usart3_SendCmd_Test(18);// 打开输出        
    msdelay(3000);  
    Usart2_SendCmd(11);//20mV测量读
    s16data[0] = (s16)value.u16CorrVIn20[1];
    msdelay(50);
    value.g_u16MacData = (u16)s16data[0];
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(11);//20mV测量读
        msdelay(200);
        s16data[i] = (s16)value.u16CorrVIn20[1];
        value.g_u16MacData = (u16)s16data[i];
        if((s16data[i] - s16data[i-1]) <= 4 && (s16data[i] - s16data[i-1]) >= -4)
        {
            if(s16data[i] >= 3300 && s16data[i] <= 3600)
            {
                j++;
            }
            else
            {
                j = 0;
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {  
        Cmd14Send(s16data[i]);    //S2输出标定写并发送命令      
        msdelay(200);        
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 15;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 15;          
    }
}


/********************************************************************
* Summary:    标定测量100mV下限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrVIn100Down(void)
{
    u8 i;
    u8 j = 0;
    s16 s16data[READ_MAXTIME];

    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(2);//设置S3输出mV
    Usart3_SendCmd_Test(7);// 设置S3输出2mV   
  
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(2);//设置S3输出mV
    Usart3_SendCmd_Test(7);// 设置S3输出2mV   
    Usart3_SendCmd_Test(18);// 打开输出      
    Usart3_SendCmd_Test(18);// 打开输出      
    msdelay(3000); 
    Usart2_SendCmd(12);//100mV测量读
    msdelay(50);
    s16data[0] = (s16)value.u16CorrVIn100[0];
    value.g_u16MacData = (u16)s16data[0];
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(12);//100mV测量读
        msdelay(200);
        s16data[i] = (s16)value.u16CorrVIn100[0];
        value.g_u16MacData = (u16)s16data[i];
        if((s16data[i] - s16data[i-1]) <= 4 && (s16data[i] - s16data[i-1]) >= -4)
        {
            if(s16data[i] >= 40 && s16data[i] <= 90)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {     
        Cmd14Send(s16data[i]);    //S2输出标定写并发送命令      
        msdelay(200);  
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 16;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 16;          
    }
}


/********************************************************************
* Summary:    标定测量100mV上限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrVIn100Up(void)
{
    u8 i;
    u8 j = 0;
    s16 s16data[READ_MAXTIME];

    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(2);//设置S3输出mV
    Usart3_SendCmd_Test(6);// 设置S3输出100mV 
  
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(2);//设置S3输出mV
    Usart3_SendCmd_Test(6);// 设置S3输出100mV 
    Usart3_SendCmd_Test(18);// 打开输出       
    Usart3_SendCmd_Test(18);// 打开输出     
    msdelay(3000); 
    Usart2_SendCmd(12);//100mV测量读
    msdelay(50);
    s16data[0] = (s16)value.u16CorrVIn100[1];
    value.g_u16MacData = (u16)s16data[0];
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(12);//100mV测量读
        msdelay(200);
        s16data[i] = (s16)value.u16CorrVIn100[1];
        value.g_u16MacData = (u16)s16data[i];
        if((s16data[i] - s16data[i-1]) <= 4 && (s16data[i] - s16data[i-1]) >= -4)
        {
            if(s16data[i] >= 3300 && s16data[i] <= 3600)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {     
        Cmd14Send(s16data[i]);    //S2输出标定写并发送命令      
        msdelay(200);     
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 17;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 17;          
    }
}


/********************************************************************
* Summary:    标定测量V下限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrVInDown(void)
{
    u8 i;
    u8 j = 0;
    s16 s16data[READ_MAXTIME];

    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(1);//设置S3输出V
    Usart3_SendCmd_Test(11);// 设置S3输出2.000V
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(1);//设置S3输出V
    Usart3_SendCmd_Test(11);// 设置S3输出2.000V
    Usart3_SendCmd_Test(18);// 打开输出       
    Usart3_SendCmd_Test(18);// 打开输出       
    msdelay(3000); 
    Usart2_SendCmd(13);//V测量读
    msdelay(50);
    s16data[0] = (s16)value.u16CorrVIn[0];
    value.g_u16MacData = (u16)s16data[0];
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(13);//V测量读
        msdelay(200);
        s16data[i] = (s16)value.u16CorrVIn[0];
        value.g_u16MacData = (u16)s16data[i];
        if((s16data[i] - s16data[i-1]) <= 4 && (s16data[i] - s16data[i-1]) >= -4)
        {
             if(s16data[i] >= 215 && s16data[i] <= 400)
            {
                j++;
            }
            else
            {
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {   
        Cmd14Send(s16data[i]);    //S2输出标定写并发送命令      
        msdelay(200);
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 18;            
            flag.u8count = 0;//计数清零
        }
    }
    else
    {
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2;
        value.g_bCaliPoint = 18;          
    }
}


/********************************************************************
* Summary:    标定测量V上限值
* Parameters:
*             void
*       
* Return :    void  
********************************************************************/
void CorrVInUp(void)
{
    u8 i;
    u8 j = 0;
    s16 s16data[READ_MAXTIME];

    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(1);//设置S3输出V
    Usart3_SendCmd_Test(6);// 设置S3输出10.000V
    Usart3_SendCmd_Test(16);//进入S3远程
    Usart3_SendCmd_Test(1);//设置S3输出V
    Usart3_SendCmd_Test(6);// 设置S3输出10.000V    
    Usart3_SendCmd_Test(18);// 打开输出 
    Usart3_SendCmd_Test(18);// 打开输出       
    msdelay(3000); 
    Usart2_SendCmd(13);//V测量读
    msdelay(50);
    s16data[0] = (s16)value.u16CorrVIn[1];
    value.g_u16MacData = (u16)s16data[0];
    for(i = 1; i < READ_MAXTIME; i++)
    {
        Usart2_SendCmd(13);//V测量读
        msdelay(200);
        s16data[i] = (s16)value.u16CorrVIn[1];
        value.g_u16MacData = (u16)s16data[i];
        if((s16data[i] - s16data[i-1]) <= 4 && (s16data[i] - s16data[i-1]) >= -4)
        {
             if(s16data[i] >= 1050 && s16data[i] <= 1450)
            {
                j++;
            }
            else
            { 
                if(j == 0)
                {
                   j = 0;
                }
                else
                {
                   j = j - 1; 
                }
            }
        }
        else
        {
            if(j == 0)
            {
               j = 0;
            }
            else
            {
               j = j - 1; 
            }
        }
        if(j >= limit.u16MaxOKCount)
        {
            break;
        }
    }
    if(j >= limit.u16MaxOKCount)
    {   
        Cmd14Send(s16data[i]);    //S2输出标定写并发送命令      
        msdelay(200);
        Usart3_SendCmd_Test(17);// 关闭输出           
        flag.u8count++;
        if(flag.u8count >= 5)
        {
            value.g_u16ResultMeter = 2;//标定失败
            value.u8CailPointFlag[value.g_bCaliPoint] = 3;
            value.g_bCaliPoint = 2;            
            flag.u8count = 0;//计数清零
            value.g_bStart = 0;
            value.g_bCaliFinish = 1;            
        }
    }
    else
    {
        Usart3_SendCmd_Test(17);// 关闭输出 
        value.g_u16ResultMeter = 2;//标定失败
        value.u8CailPointFlag[value.g_bCaliPoint] = 2; 
        value.g_bStart = 0;
        value.g_bCaliFinish = 1;
    }
    
}