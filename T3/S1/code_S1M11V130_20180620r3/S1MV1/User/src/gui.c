/*=============================================================================
//File:			gui.c	 
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			液晶屏显示
===============================================================================*/
#include "gui.h"
#include "lcd.h"
#include "dodata.h"
#include <string.h>

extern  u8  LCD_Buf[];

const   u8  TAB_DIGTIAL_MEASURE[10] = 
{
   // 0    1     2     3     4     5     6     7      8     9 
    0xFA, 0x0A, 0xBC, 0x9E, 0x4E, 0xD6, 0xF6, 0x8A, 0xFE, 0xDE
};

const   u8  TAB_DIGTIAL_SOURCE[10] = 
{
   // 0    1     2     3     4     5     6     7      8     9 
    0xAF, 0xA0, 0xCB, 0xE9, 0xE4, 0x6D, 0x6F, 0xA8, 0xEF, 0xED
};

const   u8  TAB_DIGTIAL_SMALL[10] = 
{
   // 0    1     2     3     4     5     6     7      8     9 
    0xAF, 0xA0, 0x6D, 0xE9, 0xE2, 0xCB, 0xCF, 0xA1, 0xEF, 0xEB
};

//Summary： 数字转换为显示数组,u8Dot为小数位数
static void    ValueToDigital(u8* u8Digtial, u16 u16Value, u8 u8Dot)
{
  switch(u8Dot)
  {
    case 0:
      if(u16Value < 10)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;
        u8Digtial[2] = 10;
        u8Digtial[3] = u16Value;
      }
      else if(u16Value < 100)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;
        u8Digtial[2] = u16Value / 10;
        u8Digtial[3] = u16Value - u8Digtial[2] * 10;
      }
      else if(u16Value < 1000)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = u16Value / 100;
        u8Digtial[2] = (u16Value - u8Digtial[1] * 100) / 10;
        u8Digtial[3] = u16Value - u8Digtial[1] * 100 - u8Digtial[2] * 10;
      }
      else
      {
        u8Digtial[0] = u16Value / 1000;
        u8Digtial[1] = (u16Value - u8Digtial[0] * 1000) / 100;
        u8Digtial[2] = (u16Value - u8Digtial[0] * 1000 - u8Digtial[1] * 100) / 10;
        u8Digtial[3] = u16Value - u8Digtial[0] * 1000 - u8Digtial[1] * 100 - u8Digtial[2] * 10;
      }
      break;
    case 1:
      if(u16Value < 100)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;
        u8Digtial[2] = u16Value / 10;
        u8Digtial[3] = u16Value - u8Digtial[2] * 10;
      }
      else if(u16Value < 1000)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = u16Value / 100;
        u8Digtial[2] = (u16Value - u8Digtial[1] * 100) / 10;
        u8Digtial[3] = u16Value - u8Digtial[1] * 100 - u8Digtial[2] * 10;
      }
      else
      {
        u8Digtial[0] = u16Value / 1000;
        u8Digtial[1] = (u16Value - u8Digtial[0] * 1000) / 100;
        u8Digtial[2] = (u16Value - u8Digtial[0] * 1000 - u8Digtial[1] * 100) / 10;
        u8Digtial[3] = u16Value - u8Digtial[0] * 1000 - u8Digtial[1] * 100 - u8Digtial[2] * 10;
      }
      break;
    case 2:
      if(u16Value < 1000)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = u16Value / 100;
        u8Digtial[2] = (u16Value - u8Digtial[1] * 100) / 10;
        u8Digtial[3] = u16Value - u8Digtial[1] * 100 - u8Digtial[2] * 10;
      }
      else
      {
        u8Digtial[0] = u16Value / 1000;
        u8Digtial[1] = (u16Value - u8Digtial[0] * 1000) / 100;
        u8Digtial[2] = (u16Value - u8Digtial[0] * 1000 - u8Digtial[1] * 100) / 10;
        u8Digtial[3] = u16Value - u8Digtial[0] * 1000 - u8Digtial[1] * 100 - u8Digtial[2] * 10;
      }
      break;
    case 3:
      u8Digtial[0] = u16Value / 1000;
      u8Digtial[1] = (u16Value - u8Digtial[0] * 1000) / 100;
      u8Digtial[2] = (u16Value - u8Digtial[0] * 1000 - u8Digtial[1] * 100) / 10;
      u8Digtial[3] = u16Value - u8Digtial[0] * 1000 - u8Digtial[1] * 100 - u8Digtial[2] * 10;
      break;
    default:    //都不显示
      u8Digtial[0] = 10;
      u8Digtial[1] = 10;
      u8Digtial[2] = 10;
      u8Digtial[3] = 10;
      break;
  }
}

//Summary： Time,count,n数字转换为数组
static void    Time_Count_nToDigital(u8* u8Digtial, u16 u16Value)
{
  if(u16Value < 10) //1位
  {
    u8Digtial[0] = 10;
    u8Digtial[1] = 10;
    u8Digtial[2] = u16Value;
  }
  else if(u16Value < 100)//2位
  {
    u8Digtial[0] = 10;
    u8Digtial[1] = u16Value / 10;
    u8Digtial[2] = u16Value - u8Digtial[1] * 10;
  }
  else if(u16Value < 1000)
  {
    u8Digtial[0] = u16Value / 100;
    u8Digtial[1] = (u16Value - u8Digtial[0] * 100)/10;
    u8Digtial[2] = u16Value - u8Digtial[0] * 100 - u8Digtial[1] * 10;
  }
  else
  {
    u8Digtial[0] = 10;
    u8Digtial[1] = 10;
    u8Digtial[2] = 10;
  }
}

//Summary： m数字转换为数组
static void    mToDigital(u8* u8Digtial, u16 u16Value)
{
  if(u16Value < 10) //1位
  {
    u8Digtial[0] = 10;
    u8Digtial[1] = u16Value;
  }
  else if(u16Value < 100)//2位
  {
    u8Digtial[0] = u16Value / 10;
    u8Digtial[1] = u16Value - u8Digtial[0] * 10;
  }
  else
  {
    u8Digtial[0] = 10;
    u8Digtial[1] = 10;
  }
}

//Summary:	显示MEASURE
void	Write_MEASURE(u8 u8Stat)
{     
    if(u8Stat == ON)
      LCD_Buf[0] |= MEASURE;
    else
      LCD_Buf[0] &= ~MEASURE;
      
    HT16C23_WriteRAM_Ones(0, LCD_Buf[0]);
}

//Summary:	显示MEASURE数字-号
void	Write_MEASURE_MINUS(u8 u8Stat)
{     
    if(u8Stat == ON)
      LCD_Buf[0] |= MEASURE_MINUS;
    else
      LCD_Buf[0] &= ~MEASURE_MINUS;
      
    HT16C23_WriteRAM_Ones(0, LCD_Buf[0]);
}

//Summary:	显示MEASURE RL,RH,SL,SH
void	Write_MEASURE_RangeFlg(u8 u8RangeFlg)
{ 
  switch(u8RangeFlg)
  {
    case RANGE_NONE:
      LCD_Buf[6] &= ~MEASURE_RL;
      LCD_Buf[6] &= ~MEASURE_RH;
      LCD_Buf[6] &= ~MEASURE_SL;
      LCD_Buf[6] &= ~MEASURE_SH;
      HT16C23_WriteRAM_Ones(6, LCD_Buf[6]);
      break;
    case RANGE_RL:
      LCD_Buf[6] |= MEASURE_RL;
      LCD_Buf[6] &= ~MEASURE_RH;
      LCD_Buf[6] &= ~MEASURE_SL;
      LCD_Buf[6] &= ~MEASURE_SH;
      HT16C23_WriteRAM_Ones(6, LCD_Buf[6]);
      break;
    case RANGE_RH:
      LCD_Buf[6] &= ~MEASURE_RL;
      LCD_Buf[6] |= MEASURE_RH;
      LCD_Buf[6] &= ~MEASURE_SL;
      LCD_Buf[6] &= ~MEASURE_SH;
      HT16C23_WriteRAM_Ones(6, LCD_Buf[6]);
      break;
    case RANGE_SL:
      LCD_Buf[6] &= ~MEASURE_RL;
      LCD_Buf[6] &= ~MEASURE_RH;
      LCD_Buf[6] |= MEASURE_SL;
      LCD_Buf[6] &= ~MEASURE_SH;
      HT16C23_WriteRAM_Ones(6, LCD_Buf[6]);
      break;
    case RANGE_SH:
      LCD_Buf[6] &= ~MEASURE_RL;
      LCD_Buf[6] &= ~MEASURE_RH;
      LCD_Buf[6] &= ~MEASURE_SL;
      LCD_Buf[6] |= MEASURE_SH;
      HT16C23_WriteRAM_Ones(6, LCD_Buf[6]);
      break;
    default:
      LCD_Buf[6] &= ~MEASURE_RL;
      LCD_Buf[6] &= ~MEASURE_RH;
      LCD_Buf[6] &= ~MEASURE_SL;
      LCD_Buf[6] &= ~MEASURE_SH;
      HT16C23_WriteRAM_Ones(6, LCD_Buf[6]);
      break;
  }
}

//Summary:  显示MEASURE单位
void    Write_MEASURE_Unit(u8 u8SigType)
{
  switch(u8SigType)
  {
    case SIGTYPE_24VLOOP:
    case SIGTYPE_MA:
    case SIGTYPE_4_20MA:
      LCD_Buf[7] &= ~MEASURE_MV;
      LCD_Buf[7] &= ~MEASURE_V;
      LCD_Buf[7] &= ~MEASURE_HZ;
      LCD_Buf[7] &= ~MEASURE_KHZ;
      LCD_Buf[7] &= ~MEASURE_SSD;
      LCD_Buf[7] &= ~MEASURE_R;
      LCD_Buf[7] |= MEASURE_MA;
      HT16C23_WriteRAM_Ones(7, LCD_Buf[7]);
      break;
    case SIGTYPE_V:
      LCD_Buf[7] &= ~MEASURE_MV;
      LCD_Buf[7] &= ~MEASURE_HZ;
      LCD_Buf[7] &= ~MEASURE_KHZ;
      LCD_Buf[7] &= ~MEASURE_SSD;
      LCD_Buf[7] &= ~MEASURE_R;
      LCD_Buf[7] &= ~MEASURE_MA;
      LCD_Buf[7] |= MEASURE_V;
      HT16C23_WriteRAM_Ones(7, LCD_Buf[7]);
      break;
    case SIGTYPE_20MV:
    case SIGTYPE_100MV:      
      LCD_Buf[7] &= ~MEASURE_HZ;
      LCD_Buf[7] &= ~MEASURE_KHZ;
      LCD_Buf[7] &= ~MEASURE_SSD;
      LCD_Buf[7] &= ~MEASURE_R;
      LCD_Buf[7] &= ~MEASURE_MA;
      LCD_Buf[7] &= ~MEASURE_V;
      LCD_Buf[7] |= MEASURE_MV;
      HT16C23_WriteRAM_Ones(7, LCD_Buf[7]);
      break;
    case SIGTYPE_K:
    case SIGTYPE_E:
    case SIGTYPE_J:
    case SIGTYPE_T:
    case SIGTYPE_R:
    case SIGTYPE_B:
    case SIGTYPE_S:
    case SIGTYPE_N:
      LCD_Buf[7] &= ~MEASURE_HZ;
      LCD_Buf[7] &= ~MEASURE_KHZ;      
      LCD_Buf[7] &= ~MEASURE_R;
      LCD_Buf[7] &= ~MEASURE_MA;
      LCD_Buf[7] &= ~MEASURE_V;
      LCD_Buf[7] &= ~MEASURE_MV;
      LCD_Buf[7] |= MEASURE_SSD;
      HT16C23_WriteRAM_Ones(7, LCD_Buf[7]);
      break;
    default:
      LCD_Buf[7] &= ~MEASURE_HZ;
      LCD_Buf[7] &= ~MEASURE_KHZ;      
      LCD_Buf[7] &= ~MEASURE_R;
      LCD_Buf[7] &= ~MEASURE_MA;
      LCD_Buf[7] &= ~MEASURE_V;
      LCD_Buf[7] &= ~MEASURE_MV;
      LCD_Buf[7] &= ~MEASURE_SSD;
      HT16C23_WriteRAM_Ones(7, LCD_Buf[7]);
      break;
  }
}

//Summary:  显示信号类型,分别为SOURCE信号类型，MEASURE信号类型
void    Write_SigType(u8 u8SSigType, u8 u8MSigType)
{
  //先全部关闭
  LCD_Buf[0] &= ~LOOP24V;
  LCD_Buf[0] &= ~MV20;
  LCD_Buf[21] &= ~MV100;
  LCD_Buf[21] &= ~MA4_20;
  LCD_Buf[21] &= ~K;
  LCD_Buf[21] &= ~E;
  LCD_Buf[9] &= ~J;
  LCD_Buf[9] &= ~T;
  LCD_Buf[9] &= ~R;
  LCD_Buf[9] &= ~B;
  LCD_Buf[8] &= ~S;
  LCD_Buf[8] &= ~N;
  LCD_Buf[8] &= ~PT100;
  
  //SOURCE信号类型
  switch(u8SSigType)
  {
    case SIGTYPE_24VLOOP:
      LCD_Buf[0] |= LOOP24V;
      break;
    case SIGTYPE_MA:
      break;
    case SIGTYPE_4_20MA:
      LCD_Buf[21] |= MA4_20;
      break;
    case SIGTYPE_V:
      break;
    case SIGTYPE_20MV:
      LCD_Buf[0] |= MV20;
      break;
    case SIGTYPE_100MV:
      LCD_Buf[21] |= MV100;
      break;
    case SIGTYPE_K:
      LCD_Buf[21] |= K;
      break;
    case SIGTYPE_E:
      LCD_Buf[21] |= E;
      break;
    case SIGTYPE_J:
      LCD_Buf[9] |= J;
      break;
    case SIGTYPE_T:
      LCD_Buf[9] |= T;
      break;
    case SIGTYPE_R:
      LCD_Buf[9] |= R;
      break;
    case SIGTYPE_B:
      LCD_Buf[9] |= B;
      break;
    case SIGTYPE_S:
      LCD_Buf[8] |= S;
      break;
    case SIGTYPE_N:
      LCD_Buf[8] |= N;
      break;
    default:
      break;
  }
  
  //MEASURE信号类型
  switch(u8MSigType)
  {
    case SIGTYPE_24VLOOP:
      LCD_Buf[0] |= LOOP24V;
      break;
    case SIGTYPE_MA:
      break;
    case SIGTYPE_4_20MA:
      LCD_Buf[21] |= MA4_20;
      break;
    case SIGTYPE_V:
      break;
    case SIGTYPE_20MV:
      LCD_Buf[0] |= MV20;
      break;
    case SIGTYPE_100MV:
      LCD_Buf[21] |= MV100;
      break;
    case SIGTYPE_K:
      LCD_Buf[21] |= K;
      break;
    case SIGTYPE_E:
      LCD_Buf[21] |= E;
      break;
    case SIGTYPE_J:
      LCD_Buf[9] |= J;
      break;
    case SIGTYPE_T:
      LCD_Buf[9] |= T;
      break;
    case SIGTYPE_R:
      LCD_Buf[9] |= R;
      break;
    case SIGTYPE_B:
      LCD_Buf[9] |= B;
      break;
    case SIGTYPE_S:
      LCD_Buf[8] |= S;
      break;
    case SIGTYPE_N:
      LCD_Buf[8] |= N;
      break;
    default:
      break;
  }
  HT16C23_WriteRAM_Ones(0, LCD_Buf[0]);
  HT16C23_WriteRAM_Ones(21, LCD_Buf[21]);
  HT16C23_WriteRAM_Ones(9, LCD_Buf[9]);
  HT16C23_WriteRAM_Ones(8, LCD_Buf[8]);
}

//Summary:	显示RJA
void	Write_RJA(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[8] |= RJA;
    else
      LCD_Buf[8] &= ~RJA;
      
    HT16C23_WriteRAM_Ones(8, LCD_Buf[8]);
}

//Summary:	显示RJM
void	Write_RJM(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[6] |= RJM;
    else
      LCD_Buf[6] &= ~RJM;
      
    HT16C23_WriteRAM_Ones(6, LCD_Buf[6]);
}

//Summary:	显示RJMode
void	Write_RJMode(u8 u8RJMode)
{
  switch(u8RJMode)
  {
    case RJMODE_NONE:
      Write_RJA(OFF);
      Write_RJM(OFF);
      break;
    case RJMODE_AUTO:
      Write_RJA(ON);
      Write_RJM(OFF);
      break;
    case RJMODE_MAN:
      Write_RJA(OFF);
      Write_RJM(ON);
      break;
    default:
      Write_RJA(OFF);
      Write_RJM(OFF);
      break;
  }
}

//Summary:	显示电池电量
void	Write_BatteryLevel(u8 u8Level)
{  
    LCD_Buf[7] |= L18;
    
    if(u8Level == 0)
    {
      LCD_Buf[8] &= ~L17;
      LCD_Buf[8] &= ~L16;
      LCD_Buf[8] &= ~L15;
    }
    else if(u8Level == 1)
    {
      LCD_Buf[8] |= L17;
      LCD_Buf[8] &= ~L16;
      LCD_Buf[8] &= ~L15;
    }
    else if(u8Level == 2)
    {
      LCD_Buf[8] |= L17;
      LCD_Buf[8] |= L16;
      LCD_Buf[8] &= ~L15;
    }
    else if(u8Level == 3)
    {
      LCD_Buf[8] |= L17;
      LCD_Buf[8] |= L16;
      LCD_Buf[8] |= L15;
    }
      
    HT16C23_WriteRAM_Ones(7, LCD_Buf[7]);
    HT16C23_WriteRAM_Ones(8, LCD_Buf[8]);
}

//Summary:	显示充电标志
void	Write_PowerFlg(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[8] |= POWERFLG;
    else
      LCD_Buf[8] &= ~POWERFLG;
      
    HT16C23_WriteRAM_Ones(8, LCD_Buf[8]);
}

//Summary:	显示SOURCE ON/OFF
void	Write_SOURCE_ON_OFF(u8 u8Stat)
{  
    LCD_Buf[20] |= SOURCE;
    
    if(u8Stat == ON)
    {
      LCD_Buf[20] |= SOURCE_ON;
      LCD_Buf[20] &= ~SOURCE_OFF;
    }
    else
    {
      LCD_Buf[20] &= ~SOURCE_ON;
      LCD_Buf[20] |= SOURCE_OFF;
    }
      
    HT16C23_WriteRAM_Ones(20, LCD_Buf[20]);
}

//Summary:	显示SOURCE数字-号
void	Write_SOURCE_MINUS(u8 u8Stat)
{     
    if(u8Stat == ON)
      LCD_Buf[20] |= SOURCE_MINUS;
    else
      LCD_Buf[20] &= ~SOURCE_MINUS;
      
    HT16C23_WriteRAM_Ones(20, LCD_Buf[20]);
}

//Summary:	显示SOURCE RL,RH,SL,SH
void	Write_SOURCE_RangeFlg(u8 u8RangeFlg)
{ 
  switch(u8RangeFlg)
  {
    case RANGE_NONE:
      LCD_Buf[9] &= ~SOURCE_RL;
      LCD_Buf[9] &= ~SOURCE_RH;
      LCD_Buf[9] &= ~SOURCE_SL;
      LCD_Buf[9] &= ~SOURCE_SH;
      HT16C23_WriteRAM_Ones(9, LCD_Buf[9]);
      break;
    case RANGE_RL:
      LCD_Buf[9] |= SOURCE_RL;
      LCD_Buf[9] &= ~SOURCE_RH;
      LCD_Buf[9] &= ~SOURCE_SL;
      LCD_Buf[9] &= ~SOURCE_SH;
      HT16C23_WriteRAM_Ones(9, LCD_Buf[9]);
      break;
    case RANGE_RH:
      LCD_Buf[9] &= ~SOURCE_RL;
      LCD_Buf[9] |= SOURCE_RH;
      LCD_Buf[9] &= ~SOURCE_SL;
      LCD_Buf[9] &= ~SOURCE_SH;
      HT16C23_WriteRAM_Ones(9, LCD_Buf[9]);
      break;
    case RANGE_SL:
      LCD_Buf[9] &= ~SOURCE_RL;
      LCD_Buf[9] &= ~SOURCE_RH;
      LCD_Buf[9] |= SOURCE_SL;
      LCD_Buf[9] &= ~SOURCE_SH;
      HT16C23_WriteRAM_Ones(9, LCD_Buf[9]);
      break;
    case RANGE_SH:
      LCD_Buf[9] &= ~SOURCE_RL;
      LCD_Buf[9] &= ~SOURCE_RH;
      LCD_Buf[9] &= ~SOURCE_SL;
      LCD_Buf[9] |= SOURCE_SH;
      HT16C23_WriteRAM_Ones(9, LCD_Buf[9]);
      break;
    default:
      LCD_Buf[9] &= ~SOURCE_RL;
      LCD_Buf[9] &= ~SOURCE_RH;
      LCD_Buf[9] &= ~SOURCE_SL;
      LCD_Buf[9] &= ~SOURCE_SH;
      HT16C23_WriteRAM_Ones(9, LCD_Buf[9]);
      break;
  }
}

//Summary:  显示SOURCE单位
void    Write_SOURCE_Unit(u8 u8SigType)
{
  LCD_Buf[11] &= ~SOURCE_HZ;
  LCD_Buf[10] &= ~SOURCE_K;
  LCD_Buf[13] &= ~SOURCE_R;
  LCD_Buf[10] &= ~SOURCE_MA;
  LCD_Buf[10] &= ~SOURCE_V;
  LCD_Buf[10] &= ~SOURCE_MV;
  LCD_Buf[10] &= ~SOURCE_SSD;  
  switch(u8SigType)
  {
    case SIGTYPE_24VLOOP:
    case SIGTYPE_MA:
    case SIGTYPE_4_20MA:
      LCD_Buf[10] |= SOURCE_MA;
      break;
    case SIGTYPE_V:
      LCD_Buf[10] |= SOURCE_V;
      break;
    case SIGTYPE_20MV:
    case SIGTYPE_100MV:                 
      LCD_Buf[10] |= SOURCE_MV; 
      break;
    case SIGTYPE_K:
    case SIGTYPE_E:
    case SIGTYPE_J:
    case SIGTYPE_T:
    case SIGTYPE_R:
    case SIGTYPE_B:
    case SIGTYPE_S:
    case SIGTYPE_N:
      LCD_Buf[10] |= SOURCE_SSD;      
      break;
    default:
      break;
  }
  
  HT16C23_WriteRAM_Ones(13, LCD_Buf[13]);
  HT16C23_WriteRAM_Ones(11, LCD_Buf[11]);
  HT16C23_WriteRAM_Ones(10, LCD_Buf[10]);
}


//Summary:	显示SWEEP 模式
void	Write_SWEEPMode(u8 u8Sweep)
{
  switch(u8Sweep)
  {
    case SWEEP_NONE:
      LCD_Buf[16] &= ~STEP;
      LCD_Buf[16] &= ~SWEEP;
      HT16C23_WriteRAM_Ones(16, LCD_Buf[16]);
      break;
    case SWEEP_SWEEP:
      LCD_Buf[16] &= ~STEP;
      LCD_Buf[16] |= SWEEP;
      HT16C23_WriteRAM_Ones(16, LCD_Buf[16]);
      break;
    case SWEEP_STEP:
      LCD_Buf[16] |= STEP;
      LCD_Buf[16] &= ~SWEEP;
      HT16C23_WriteRAM_Ones(16, LCD_Buf[16]);
      break;
    default:
      LCD_Buf[16] &= ~STEP;
      LCD_Buf[16] &= ~SWEEP;
      HT16C23_WriteRAM_Ones(16, LCD_Buf[16]);
      break;
  }
}

//Summary:	显示TIME
void	Write_TIME(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[16] |= TIME;
    else
      LCD_Buf[16] &= ~TIME;
      
    HT16C23_WriteRAM_Ones(16, LCD_Buf[16]);
}

//Summary:	显示time t
void	Write_Time_t(u8 u8t)
{ 
  switch(u8t)
  {
    case TIME_T1:
      LCD_Buf[15] |= TIME_L1;
      LCD_Buf[15] &= ~TIME_L2;
      LCD_Buf[15] &= ~TIME_L3;
      LCD_Buf[15] &= ~TIME_L4;
      HT16C23_WriteRAM_Ones(15, LCD_Buf[15]);
      break;
    case TIME_T2:
      LCD_Buf[15] &= ~TIME_L1;
      LCD_Buf[15] |= TIME_L2;
      LCD_Buf[15] &= ~TIME_L3;
      LCD_Buf[15] &= ~TIME_L4;
      HT16C23_WriteRAM_Ones(15, LCD_Buf[15]);
      break;
    case TIME_T3:
      LCD_Buf[15] &= ~TIME_L1;
      LCD_Buf[15] &= ~TIME_L2;
      LCD_Buf[15] |= TIME_L3;
      LCD_Buf[15] &= ~TIME_L4;
      HT16C23_WriteRAM_Ones(15, LCD_Buf[15]);
      break;
    case TIME_T4:
      LCD_Buf[15] &= ~TIME_L1;
      LCD_Buf[15] &= ~TIME_L2;
      LCD_Buf[15] &= ~TIME_L3;
      LCD_Buf[15] |= TIME_L4;
      HT16C23_WriteRAM_Ones(15, LCD_Buf[15]);
      break;
    case TIME_SWEEP:
      LCD_Buf[15] |= TIME_L1;
      LCD_Buf[15] |= TIME_L2;
      LCD_Buf[15] |= TIME_L3;
      LCD_Buf[15] |= TIME_L4;
      HT16C23_WriteRAM_Ones(15, LCD_Buf[15]);
      break;
    case TIME_SWEEPSTEP1:
      LCD_Buf[15] &= ~TIME_L1;
      LCD_Buf[15] |= TIME_L2;
      LCD_Buf[15] |= TIME_L3;
      LCD_Buf[15] |= TIME_L4;
      HT16C23_WriteRAM_Ones(15, LCD_Buf[15]);
      break;
    case TIME_SWEEPSTEP2:
      LCD_Buf[15] |= TIME_L1;
      LCD_Buf[15] &= ~TIME_L2;
      LCD_Buf[15] |= TIME_L3;
      LCD_Buf[15] |= TIME_L4;
      HT16C23_WriteRAM_Ones(15, LCD_Buf[15]);
      break;
    case TIME_SWEEPSTEP3:
      LCD_Buf[15] |= TIME_L1;
      LCD_Buf[15] |= TIME_L2;
      LCD_Buf[15] &= ~TIME_L3;
      LCD_Buf[15] |= TIME_L4;
      HT16C23_WriteRAM_Ones(15, LCD_Buf[15]);
      break;
    case TIME_SWEEPSTEP4:
      LCD_Buf[15] |= TIME_L1;
      LCD_Buf[15] |= TIME_L2;
      LCD_Buf[15] |= TIME_L3;
      LCD_Buf[15] &= ~TIME_L4;
      HT16C23_WriteRAM_Ones(15, LCD_Buf[15]);
      break;
    default:
      LCD_Buf[15] &= ~TIME_L1;
      LCD_Buf[15] &= ~TIME_L2;
      LCD_Buf[15] &= ~TIME_L3;
      LCD_Buf[15] &= ~TIME_L4;
      HT16C23_WriteRAM_Ones(15, LCD_Buf[15]);
      break;
  }
}


//Summary:	显示COUNT
void	Write_COUNT(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[16] |= COUNT;
    else
      LCD_Buf[16] &= ~COUNT;
      
    HT16C23_WriteRAM_Ones(16, LCD_Buf[16]);
}


//Summary:	显示n
void	Write_n(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[14] |= STEP_N;
    else
      LCD_Buf[14] &= ~STEP_N;
      
    HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);
}

//Summary:	显示m
void	Write_m(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[16] |= STEP_M;
    else
      LCD_Buf[16] &= ~STEP_M;
      
    HT16C23_WriteRAM_Ones(16, LCD_Buf[16]);
}

//Summary:	显示s
void	Write_s(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[12] |= SEC;
    else
      LCD_Buf[12] &= ~SEC;
      
    HT16C23_WriteRAM_Ones(12, LCD_Buf[12]);
}

//Summary:	显示MEASURE数字
//          s16MValue       数字
//          u8Dot           小数点个数
void	Write_Value_MEASURE(s16* ps16MValue, u8 u8Dot, s16 s16L, s16 s16H)
{
  u8    u8Digbuf[4], i;
  u16   u16DigValue;
  
  if(*ps16MValue > s16H)
    *ps16MValue = s16H;
  if(*ps16MValue < s16L)
    *ps16MValue = s16L;
          
  if(*ps16MValue < 0)
  {
      Write_MEASURE_MINUS(ON);//正负号
      u16DigValue = 0 - *ps16MValue;
  }
  else
  {
      Write_MEASURE_MINUS(OFF);
      u16DigValue = *ps16MValue;
  }
      
  ValueToDigital(u8Digbuf, u16DigValue, u8Dot);
  
  for(i = 0; i < 4; i++)
  {
      Write_Digtial_MEASURE(i, u8Digbuf[i], i == (3 - u8Dot));
  }
  HT16C23_WriteRAM_Ones(0, LCD_Buf[0]);
  HT16C23_WriteRAM_Ones(1, LCD_Buf[1]);
  HT16C23_WriteRAM_Ones(4, LCD_Buf[4]);
  HT16C23_WriteRAM_Ones(5, LCD_Buf[5]);
  HT16C23_WriteRAM_Ones(6, LCD_Buf[6]);
}

//Summary:	显示MEASURE数字
//          u8Pos           位置
//          u8Digtial       数字
//          u8DotPos        小数点位置
void	Write_Digtial_MEASURE(u8 u8Pos, u8 u8Digtial, u8 Dot)
{ 
    switch(u8Pos)
    {
      case 0:       //MEASURE 数字第1位
        if(u8Digtial == 10)     //不显示
        {
            LCD_Buf[0] &= 0x0F;
            LCD_Buf[1] &= 0xF0;
        }
        else
        {
          LCD_Buf[0] &= 0x0F;
          LCD_Buf[0] |= (TAB_DIGTIAL_MEASURE[u8Digtial] & 0xF0);
          LCD_Buf[1] &= 0xF0;
          LCD_Buf[1] |= (TAB_DIGTIAL_MEASURE[u8Digtial] & 0x0F);
          if(Dot == 1)     //有小数点
            LCD_Buf[1] |= 0x01;
        }  
        break;
      case 1:       //MEASURE 数字第2位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[1] &= 0x0F;
          LCD_Buf[4] &= 0xF0;
        }
        else
        {
          LCD_Buf[1] &= 0x0F;
          LCD_Buf[1] |= (TAB_DIGTIAL_MEASURE[u8Digtial] & 0xF0);
          LCD_Buf[4] &= 0xF0;
          LCD_Buf[4] |= (TAB_DIGTIAL_MEASURE[u8Digtial] & 0x0F);
          if(Dot == 1)     //有小数点
            LCD_Buf[4] |= 0x01;
        }    
        break;
      case 2:       //MEASURE 数字第3位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[4] &= 0x0F;
          LCD_Buf[5] &= 0xF0;
        }
        else
        {
          LCD_Buf[4] &= 0x0F;
          LCD_Buf[4] |= (TAB_DIGTIAL_MEASURE[u8Digtial] & 0xF0);
          LCD_Buf[5] &= 0xF0;
          LCD_Buf[5] |= (TAB_DIGTIAL_MEASURE[u8Digtial] & 0x0F);
          if(Dot == 1)     //有小数点
            LCD_Buf[5] |= 0x01;
        }
        break;
      case 3:       //MEASURE 数字第4位
        if(u8Digtial == 10)     //不显示
        {
           LCD_Buf[5] &= 0x0F;
           LCD_Buf[6] &= 0xF1;
        }
        else
        {
          LCD_Buf[5] &= 0x0F;
          LCD_Buf[5] |= (TAB_DIGTIAL_MEASURE[u8Digtial] & 0xF0);
          LCD_Buf[6] &= 0xF1;
          LCD_Buf[6] |= (TAB_DIGTIAL_MEASURE[u8Digtial] & 0x0F);
        }
        
        break;
      default:
        break;
    }
}

//Summary:	显示SOURCE数字
//          s16MValue       数字
//          u8Dot           小数点个数
void	Write_Value_SOURCE(s16* ps16SValue, u8 u8Dot, s16 s16L, s16 s16H)
{
  u8    u8Digbuf[4], i;
  u16   u16DigValue;
  
  if(*ps16SValue > s16H)
    *ps16SValue = s16H;
  if(*ps16SValue < s16L)
    *ps16SValue = s16L;
          
  if(*ps16SValue < 0)
  {
      Write_SOURCE_MINUS(ON);//正负号
      u16DigValue = 0 - *ps16SValue;
  }
  else
  {
      Write_SOURCE_MINUS(OFF);
      u16DigValue = *ps16SValue;
  }
      
  ValueToDigital(u8Digbuf, u16DigValue, u8Dot);
  
  for(i = 0; i < 4; i++)
  {
      Write_Digtial_SOURCE(i, u8Digbuf[i], i == (3 - u8Dot));
  }
  HT16C23_WriteRAM_Ones(19, LCD_Buf[19]);
  HT16C23_WriteRAM_Ones(20, LCD_Buf[20]);
  HT16C23_WriteRAM_Ones(18, LCD_Buf[18]);
  HT16C23_WriteRAM_Ones(17, LCD_Buf[17]);
  HT16C23_WriteRAM_Ones(16, LCD_Buf[16]);
}

//Summary:	显示SOURCE数字
//          u8Pos           位置
//          u8Digtial       数字
//          u8DotPos        小数点位置
void	Write_Digtial_SOURCE(u8 u8Pos, u8 u8Digtial, u8 Dot)
{ 
    switch(u8Pos)
    {
      case 0:       //SOURCE 数字第1位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[19] &= 0x0F;
          LCD_Buf[20] &= 0xF0;          
        }
        else
        {
          LCD_Buf[19] &= 0x0F;
          LCD_Buf[19] |= (TAB_DIGTIAL_SOURCE[u8Digtial] & 0xF0);
          LCD_Buf[20] &= 0xF0;
          LCD_Buf[20] |= (TAB_DIGTIAL_SOURCE[u8Digtial] & 0x0F);
          if(Dot == 1)     //有小数点
            LCD_Buf[19] |= 0x10;
        }
        break;
      case 1:       //SOURCE 数字第2位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[18] &= 0x0F;
          LCD_Buf[19] &= 0xF0;          
        }
        else
        {
          LCD_Buf[18] &= 0x0F;
          LCD_Buf[18] |= (TAB_DIGTIAL_SOURCE[u8Digtial] & 0xF0);
          LCD_Buf[19] &= 0xF0;
          LCD_Buf[19] |= (TAB_DIGTIAL_SOURCE[u8Digtial] & 0x0F);
          if(Dot == 1)     //有小数点
            LCD_Buf[18] |= 0x10;
        }
        break;
      case 2:       //SOURCE 数字第3位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[17] &= 0x0F;
          LCD_Buf[18] &= 0xF0;          
        }
        else
        {
          LCD_Buf[17] &= 0x0F;
          LCD_Buf[17] |= (TAB_DIGTIAL_SOURCE[u8Digtial] & 0xF0);
          LCD_Buf[18] &= 0xF0;
          LCD_Buf[18] |= (TAB_DIGTIAL_SOURCE[u8Digtial] & 0x0F);
          if(Dot == 1)     //有小数点
            LCD_Buf[17] |= 0x10;
        }
        break;
      case 3:       //SOURCE 数字第4位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[16] &= 0x1F;
          LCD_Buf[17] &= 0xF0;          
        }
        else
        {
          LCD_Buf[16] &= 0x1F;
          LCD_Buf[16] |= (TAB_DIGTIAL_SOURCE[u8Digtial] & 0xF0);
          LCD_Buf[17] &= 0xF0;
          LCD_Buf[17] |= (TAB_DIGTIAL_SOURCE[u8Digtial] & 0x0F);
        }
        break;
      default:
        break;
    }
}

//Summary:	显示n数字
//          pu8n            n值
void	Write_Value_TIME_COUNT_n(u16* pu16Value, u16 u16L, u16 u16H)
{
  u8    u8Digbuf[3], i;
  
  if(u16L >= 1000 || u16H >= 1000)
  {
    //不显示
    u8Digbuf[0] = 10;
    u8Digbuf[1] = 10;
    u8Digbuf[2] = 10;
  }
  else
  {
    if(*pu16Value > u16H)
      *pu16Value = u16H;
    if(*pu16Value < u16L)
      *pu16Value = u16L;
        
    Time_Count_nToDigital(u8Digbuf, *pu16Value);
  }
  
  for(i = 0; i < 3; i++)
  {
      Write_Digtial_TIME_COUNT_n(i, u8Digbuf[i]);
  }
  HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);
  HT16C23_WriteRAM_Ones(15, LCD_Buf[15]);
  HT16C23_WriteRAM_Ones(13, LCD_Buf[13]);
  HT16C23_WriteRAM_Ones(12, LCD_Buf[12]);
}

//Summary:	显示TIME COUNT n数字
//          u8Pos           位置
//          u8Digtial       数字
//          u8DotPos        小数点位置
void	Write_Digtial_TIME_COUNT_n(u8 u8Pos, u8 u8Digtial)
{ 
    switch(u8Pos)
    {
      case 0:       //数字第1位
        if(u8Digtial == 10)
        {
          LCD_Buf[14] &= 0x1F;
          LCD_Buf[15] &= 0xF0;
        }
        else
        {
          LCD_Buf[14] &= 0x1F;
          LCD_Buf[14] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0xF0);
          LCD_Buf[15] &= 0xF0;
          LCD_Buf[15] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0x0F);
        }
        break;
      case 1:       //数字第2位
        if(u8Digtial == 10)
        {
          LCD_Buf[13] &= 0x1F;
          LCD_Buf[14] &= 0xF0;
        }
        else
        {
          LCD_Buf[13] &= 0x1F;
          LCD_Buf[13] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0xF0);
          LCD_Buf[14] &= 0xF0;
          LCD_Buf[14] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0x0F);
        }
        break;
      case 2:       //数字第3位
        if(u8Digtial == 10)
        {
          LCD_Buf[12] &= 0x1F;
          LCD_Buf[13] &= 0xF0;
        }
        else
        {
          LCD_Buf[12] &= 0x1F;
          LCD_Buf[12] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0xF0);
          LCD_Buf[13] &= 0xF0;
          LCD_Buf[13] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0x0F);
        }
        break;
      default:
        break;
    }
}


//Summary:	显示m数字
//          pu8n            n值
void	Write_Value_m(u16* pu16Value, u16 u16L, u16 u16H)
{
  u8    u8Digbuf[2], i;
  
  if(u16L >= 100 || u16H >= 100)
  {
    //不显示
    u8Digbuf[0] = 10;
    u8Digbuf[1] = 10;
  }
  else
  {
    if(*pu16Value > u16H)
      *pu16Value = u16H;
    if(*pu16Value < u16L)
      *pu16Value = u16L;
        
    mToDigital(u8Digbuf, *pu16Value);
  }
  
  for(i = 0; i < 2; i++)
  {
      Write_Digtial_m(i, u8Digbuf[i]);
  }
  HT16C23_WriteRAM_Ones(11, LCD_Buf[11]);
  HT16C23_WriteRAM_Ones(12, LCD_Buf[12]);
  HT16C23_WriteRAM_Ones(10, LCD_Buf[10]);
}

//Summary:	显示m数字
//          u8Pos           位置
//          u8Digtial       数字
//          u8DotPos        小数点位置
void	Write_Digtial_m(u8 u8Pos, u8 u8Digtial)
{ 
    switch(u8Pos)
    {
      case 0:       //数字第1位
        if(u8Digtial == 10)
        {
          LCD_Buf[11] &= 0x1F;
          LCD_Buf[12] &= 0xF0;
        }
        else
        {
          LCD_Buf[11] &= 0x1F;
          LCD_Buf[11] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0xF0);
          LCD_Buf[12] &= 0xF0;
          LCD_Buf[12] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0x0F);
        }
        break;
      case 1:       //数字第2位
        if(u8Digtial == 10)
        {
          LCD_Buf[10] &= 0x1F;
          LCD_Buf[11] &= 0xF0;
        }
        else
        {
          LCD_Buf[10] &= 0x1F;
          LCD_Buf[10] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0xF0);
          LCD_Buf[11] &= 0xF0;
          LCD_Buf[11] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0x0F);
        }
        break;
      default:
        break;
    }
}