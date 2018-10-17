#include "FunctionDoData.h"
#include "DriveCom1.h"
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

//Summary:    秒级延迟计算
void sdelay(u16 s)
{
    while(s > 0)
    {
        s --;
        msdelay(999);
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
static float S32tofloat(s32 s32Data)
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





