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
#include <math.h>
extern  u8  LCD_Buf[];

const   u8  TAB_DIGTIAL_MEASURE[10] = 
{
   // 0    1     2     3     4     5     6     7      8     9 
    0xFA, 0x60, 0xD6, 0xF4, 0x6C, 0xBC, 0xBE, 0xE0, 0xFE, 0xFC
};

const   u8  TAB_DIGTIAL_SOURCE[10] = 
{
   // 0    1     2     3     4     5     6     7      8     9 
    0xAF, 0x06, 0x6D, 0x4F, 0xC6, 0xCB, 0xEB, 0x0E, 0xEF, 0xCF
};

const   u8  TAB_DIGTIAL_SMALL[10] = 
{
   // 0    1     2     3     4     5     6     7      8     9 
    0xF5, 0x60, 0xB6, 0xF2, 0x63, 0xD3, 0xD7, 0x70, 0xF7, 0xF3
};

//Summary： 数字转换为显示数组,u8Dot为小数位数
static void  ValueToDigital(u8* u8Digtial, u32 u32Value, u8 dot)
{
  switch(dot)
  {
    case 0:
      if(u32Value < 10)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;
        u8Digtial[2] = 10;
        u8Digtial[3] = 10;
        u8Digtial[4] = 10;        
        u8Digtial[5] = u32Value;
      }
      else if(u32Value < 100)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;
        u8Digtial[2] = 10;
        u8Digtial[3] = 10;        
        u8Digtial[4] = u32Value / 10;
        u8Digtial[5] = u32Value % 10;
      }
      else if(u32Value < 1000)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;
        u8Digtial[2] = 10;        
        u8Digtial[3] = u32Value / 100;
        u8Digtial[4] = (u32Value % 100) / 10;
        u8Digtial[5] = u32Value % 10;
      }
      else if(u32Value < 10000)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;      
        u8Digtial[2] = u32Value / 1000;
        u8Digtial[3] = (u32Value % 1000) / 100;
        u8Digtial[4] = (u32Value % 100) / 10;
        u8Digtial[5] = u32Value % 10;
      }
      else 
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = u32Value / 10000;  
        u8Digtial[2] = (u32Value % 10000) / 1000;
        u8Digtial[3] = (u32Value % 1000) / 100;
        u8Digtial[4] = (u32Value % 100) / 10;
        u8Digtial[5] = u32Value % 10;
      }      
      break;
    case 1:
      if(u32Value < 100)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;
        u8Digtial[2] = 10;
        u8Digtial[3] = 10;        
        u8Digtial[4] = u32Value / 10;
        u8Digtial[5] = u32Value % 10;
      }
      else if(u32Value < 1000)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;
        u8Digtial[2] = 10;        
        u8Digtial[3] = u32Value / 100;
        u8Digtial[4] = (u32Value % 100) / 10;
        u8Digtial[5] = u32Value % 10;
      }
      else if(u32Value < 10000)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;      
        u8Digtial[2] = u32Value / 1000;
        u8Digtial[3] = (u32Value % 1000) / 100;
        u8Digtial[4] = (u32Value % 100) / 10;
        u8Digtial[5] = u32Value % 10;
      }
      else 
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = u32Value / 10000;  
        u8Digtial[2] = (u32Value % 10000) / 1000;
        u8Digtial[3] = (u32Value % 1000) / 100;
        u8Digtial[4] = (u32Value % 100) / 10;
        u8Digtial[5] = u32Value % 10;
      }  
      break;
    case 2:
      if(u32Value < 1000)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;
        u8Digtial[2] = 10;        
        u8Digtial[3] = u32Value / 100;
        u8Digtial[4] = (u32Value % 100) / 10;
        u8Digtial[5] = u32Value % 10;
      }
      else if(u32Value < 10000)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;      
        u8Digtial[2] = u32Value / 1000;
        u8Digtial[3] = (u32Value % 1000) / 100;
        u8Digtial[4] = (u32Value % 100) / 10;
        u8Digtial[5] = u32Value % 10;
      }
      else 
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = u32Value / 10000;  
        u8Digtial[2] = (u32Value % 10000) / 1000;
        u8Digtial[3] = (u32Value % 1000) / 100;
        u8Digtial[4] = (u32Value % 100) / 10;
        u8Digtial[5] = u32Value % 10;
      }  
      break;
    case 3:
      if(u32Value < 10000)
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = 10;      
        u8Digtial[2] = u32Value / 1000;
        u8Digtial[3] = (u32Value % 1000) / 100;
        u8Digtial[4] = (u32Value % 100) / 10;
        u8Digtial[5] = u32Value % 10;
      }
      else 
      {
        u8Digtial[0] = 10;
        u8Digtial[1] = u32Value / 10000;  
        u8Digtial[2] = (u32Value % 10000) / 1000;
        u8Digtial[3] = (u32Value % 1000) / 100;
        u8Digtial[4] = (u32Value % 100) / 10;
        u8Digtial[5] = u32Value % 10;
      }        
      break;
     case 4:
        u8Digtial[0] = 10;
        u8Digtial[1] = u32Value / 10000;  
        u8Digtial[2] = (u32Value % 10000) / 1000;
        u8Digtial[3] = (u32Value % 1000) / 100;
        u8Digtial[4] = (u32Value % 100) / 10;
        u8Digtial[5] = u32Value % 10;
      break;     
    default:    //都不显示
      u8Digtial[0] = 10;
      u8Digtial[1] = 10;
      u8Digtial[2] = 10;
      u8Digtial[3] = 10;
      u8Digtial[4] = 10;
      u8Digtial[5] = 10;      
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
      LCD_Buf[0] &= ~MEASURE_RL;
      LCD_Buf[0] &= ~MEASURE_RH;
      LCD_Buf[0] &= ~MEASURE_SL;
      LCD_Buf[0] &= ~MEASURE_SH;
      HT16C23_WriteRAM_Ones(0, LCD_Buf[0]);
      break;
    case RANGE_RL:
      LCD_Buf[0] |= MEASURE_RL;
      LCD_Buf[0] &= ~MEASURE_RH;
      LCD_Buf[0] &= ~MEASURE_SL;
      LCD_Buf[0] &= ~MEASURE_SH;
      HT16C23_WriteRAM_Ones(0, LCD_Buf[0]);
      break;
    case RANGE_RH:
      LCD_Buf[0] &= ~MEASURE_RL;
      LCD_Buf[0] |= MEASURE_RH;
      LCD_Buf[0] &= ~MEASURE_SL;
      LCD_Buf[0] &= ~MEASURE_SH;
      HT16C23_WriteRAM_Ones(0, LCD_Buf[0]);
      break;
    case RANGE_SL:
      LCD_Buf[0] &= ~MEASURE_RL;
      LCD_Buf[0] &= ~MEASURE_RH;
      LCD_Buf[0] |= MEASURE_SL;
      LCD_Buf[0] &= ~MEASURE_SH;
      HT16C23_WriteRAM_Ones(0, LCD_Buf[0]);
      break;
    case RANGE_SH:
      LCD_Buf[0] &= ~MEASURE_RL;
      LCD_Buf[0] &= ~MEASURE_RH;
      LCD_Buf[0] &= ~MEASURE_SL;
      LCD_Buf[0] |= MEASURE_SH;
      HT16C23_WriteRAM_Ones(0, LCD_Buf[0]);
      break;
    default:
      LCD_Buf[0] &= ~MEASURE_RL;
      LCD_Buf[0] &= ~MEASURE_RH;
      LCD_Buf[0] &= ~MEASURE_SL;
      LCD_Buf[0] &= ~MEASURE_SH;
      HT16C23_WriteRAM_Ones(0, LCD_Buf[0]);
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
//    case SIGTYPE_SMA:
      LCD_Buf[13] |= MEASURE_m;
      LCD_Buf[13] &= ~MEASURE_V;
      LCD_Buf[13] |= MEASURE_A;
      LCD_Buf[14] &= ~MEASURE_Hz;
      LCD_Buf[14] &= ~MEASURE_K;
      LCD_Buf[7] &= ~MEASURE_SSD;
      LCD_Buf[7] &= ~MEASURE_R;
      LCD_Buf[7] &= ~MEASURE_F;
      break;
    case SIGTYPE_MV:
      LCD_Buf[13] |= MEASURE_m;
      LCD_Buf[13] |= MEASURE_V;
      LCD_Buf[13] &= ~MEASURE_A;
      LCD_Buf[14] &= ~MEASURE_Hz;
      LCD_Buf[14] &= ~MEASURE_K;
      LCD_Buf[7] &= ~MEASURE_SSD;
      LCD_Buf[7] &= ~MEASURE_R;
      LCD_Buf[7] &= ~MEASURE_F;
      break;      
    case SIGTYPE_V:
      LCD_Buf[13] &= ~MEASURE_m;
      LCD_Buf[13] |= MEASURE_V;
      LCD_Buf[13] &= ~MEASURE_A;
      LCD_Buf[14] &= ~MEASURE_Hz;
      LCD_Buf[14] &= ~MEASURE_K;
      LCD_Buf[7] &= ~MEASURE_SSD;
      LCD_Buf[7] &= ~MEASURE_R;
      LCD_Buf[7] &= ~MEASURE_F;
      break;
    case SIGTYPE_Res:
      LCD_Buf[13] &= ~MEASURE_m;
      LCD_Buf[13] &= ~MEASURE_V;
      LCD_Buf[13] &= ~MEASURE_A;
      LCD_Buf[14] &= ~MEASURE_Hz;
      LCD_Buf[14] &= ~MEASURE_K;
      LCD_Buf[7] &= ~MEASURE_SSD;
      LCD_Buf[7] |= MEASURE_R;
      LCD_Buf[7] &= ~MEASURE_F;
      break;    
    case SIGTYPE_Cu50:       
    case SIGTYPE_PT100: 
    case SIGTYPE_K:
    case SIGTYPE_E:
    case SIGTYPE_J:
    case SIGTYPE_T:
    case SIGTYPE_R:
    case SIGTYPE_B:
    case SIGTYPE_S:
    case SIGTYPE_N:
      LCD_Buf[13] &= ~MEASURE_m;
      LCD_Buf[13] &= ~MEASURE_V;
      LCD_Buf[13] &= ~MEASURE_A;
      LCD_Buf[14] &= ~MEASURE_Hz;
      LCD_Buf[14] &= ~MEASURE_K;
      LCD_Buf[7] &= ~MEASURE_R;
      if(data.u8TempType == 0)
      {
        LCD_Buf[7] |= MEASURE_SSD;
        LCD_Buf[7] &= ~MEASURE_F;
      }
      else
      {
        LCD_Buf[7] |= MEASURE_F;
        LCD_Buf[7] &= ~MEASURE_SSD;        
      }
      break;
    case SIGTYPE_Hz:
      LCD_Buf[13] &= ~MEASURE_m;
      LCD_Buf[13] &= ~MEASURE_V;
      LCD_Buf[13] &= ~MEASURE_A;
      LCD_Buf[14] |= MEASURE_Hz;
      LCD_Buf[14] &= ~MEASURE_K;
      LCD_Buf[7] &= ~MEASURE_SSD;
      LCD_Buf[7] &= ~MEASURE_R;
      LCD_Buf[7] &= ~MEASURE_F;         
      break;
    default:
      LCD_Buf[13] &= ~MEASURE_m;
      LCD_Buf[13] &= ~MEASURE_V;
      LCD_Buf[13] &= ~MEASURE_A;
      LCD_Buf[14] &= ~MEASURE_Hz;
      LCD_Buf[14] &= ~MEASURE_K;
      LCD_Buf[7] &= ~MEASURE_SSD;
      LCD_Buf[7] &= ~MEASURE_R;
      LCD_Buf[7] &= ~MEASURE_F;
      break;
  }
      HT16C23_WriteRAM_Ones(7, LCD_Buf[7]);
      HT16C23_WriteRAM_Ones(13, LCD_Buf[13]); 
      HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);       
}

//Summary:  显示信号类型,分别为SOURCE信号类型，MEASURE信号类型
void    Write_SigType(u8 u8SSigType, u8 u8MSigType)
{
  //先全部关闭

  LCD_Buf[14] &= ~SOURCE_SINK;
  LCD_Buf[22] &= ~SOURCE_Cu50; 
  LCD_Buf[22] &= ~SOURCE_Pt100;  
/*************关闭S点阵 *************************/ 
  LCD_Buf[15] &= ~0x0F;
  LCD_Buf[18] &= ~0xFF;
  LCD_Buf[19] &= ~0xFF;
  LCD_Buf[20] &= ~0xEE;
  LCD_Buf[21] &= ~0xEE;
  LCD_Buf[22] &= ~0x0E;
/*************关闭M点阵 *************************/   
  LCD_Buf[7] &= ~LOOP24V;   
  LCD_Buf[13] &= ~MEASURE_Cu50;     
  LCD_Buf[13] &= ~MEASURE_Pt100;
/*************关闭M点阵 *************************/ 
  LCD_Buf[10]  &= ~0xEF;
  LCD_Buf[8]  &= ~0xFF;
  LCD_Buf[9]  &= ~0xFF;
  LCD_Buf[11] &= ~0xEE;
  LCD_Buf[12] &= ~0xEE;
/*************关闭M点阵 *************************/
  
  //SOURCE信号类型
  switch(u8SSigType)
  {
    case SIGTYPE_24VLOOP:
      LCD_Buf[7] |= LOOP24V;
      break;
    case SIGTYPE_MA:
    case SIGTYPE_MV:
    case SIGTYPE_V:   
    case SIGTYPE_Res:
    case SIGTYPE_Hz:
      break;
//    case SIGTYPE_SMA:
//      LCD_Buf[14] |= SOURCE_SINK;  
//      break;
    case SIGTYPE_Cu50:
      LCD_Buf[22] |= SOURCE_Cu50;
      break;      
    case SIGTYPE_PT100:
      LCD_Buf[22] |= SOURCE_Pt100;
      break;
    case SIGTYPE_K:    //1.5.6.9.11.13.16.17.21.23.26.29.31.35
      LCD_Buf[15] |= 0x0F;
      LCD_Buf[18] |= 0x21;
      LCD_Buf[19] |= 0x84;
      LCD_Buf[20] |= 0x48;
      LCD_Buf[21] |= 0x02;
      LCD_Buf[22] |= 0x0E;
      break;
    case SIGTYPE_E:   //1.2.3.4.5.6.11.16.17.18.19.21.26.31.32.33.34.35          
      LCD_Buf[15] |= 0x0F;
      LCD_Buf[18] |= 0x99;
      LCD_Buf[19] |= 0x89;
      LCD_Buf[20] |= 0x88;
      LCD_Buf[21] |= 0x88;
      LCD_Buf[22] |= 0x0E;
      break;
    case SIGTYPE_J:  //5.10.15.20.25.26.30.32.33.34
      LCD_Buf[19] |= 0xF0;
      LCD_Buf[20] |= 0x86;
      LCD_Buf[21] |= 0x88;
      LCD_Buf[22] |= 0x04;
      break;
    case SIGTYPE_T: //1.2.3.4.5.8.13.18.23.28.33
      LCD_Buf[15] |= 0x08;
      LCD_Buf[19] |= 0x88;
      LCD_Buf[18] |= 0xF8;
      LCD_Buf[21] |= 0x0E;
      break;
    case SIGTYPE_R: //1.2.3.4.6.10.11.15.16.17.18.19.21.23.26.29.31.35
      LCD_Buf[15] |= 0x0F;
      LCD_Buf[18] |= 0x99;
      LCD_Buf[19] |= 0x69;
      LCD_Buf[20] |= 0x48;
      LCD_Buf[21] |= 0x02;
      LCD_Buf[22] |= 0x0E;
      break;
    case SIGTYPE_B: //1.2.3.4.6.10.11.15.16.17.18.19.21.25.26.30.31.32.33.34
      LCD_Buf[15] |= 0x0F;
      LCD_Buf[18] |= 0x99;
      LCD_Buf[19] |= 0x69;
      LCD_Buf[20] |= 0x86;
      LCD_Buf[21] |= 0x88;
      LCD_Buf[22] |= 0x0E;
      break;
    case SIGTYPE_S: //2.3.4.6.10.11.17.18.19.25.26.30.32.33.34
      LCD_Buf[15] |= 0x06;
      LCD_Buf[18] |= 0x99;
      LCD_Buf[19] |= 0x49;
      LCD_Buf[20] |= 0x86;
      LCD_Buf[21] |= 0x88;
      LCD_Buf[22] |= 0x04;
      break;
    case SIGTYPE_N: //1.5.6.7.10.11.13.15.16.19.20.21.25.26.30.31.35
      LCD_Buf[15] |= 0x0F;
      LCD_Buf[18] |= 0x24;
      LCD_Buf[19] |= 0xF1;
      LCD_Buf[20] |= 0x0E;
      LCD_Buf[22] |= 0x0E;
      break;
    default:
      LCD_Buf[14] &= ~SOURCE_SINK;
      LCD_Buf[22] &= ~SOURCE_Cu50; 
      LCD_Buf[22] &= ~SOURCE_Pt100;        
      LCD_Buf[15] &= ~0x0F;
      LCD_Buf[18] &= ~0xFF;
      LCD_Buf[19] &= ~0xFF;
      LCD_Buf[20] &= ~0xEE;
      LCD_Buf[21] &= ~0xEE;
      LCD_Buf[22] &= ~0x0E;
      break;
  }
  
  //MEASURE信号类型
  switch(u8MSigType)
  {
    case SIGTYPE_24VLOOP:
      LCD_Buf[7] |= LOOP24V;
      break;
    case SIGTYPE_MA:
    case SIGTYPE_MV:
    case SIGTYPE_V:
    case SIGTYPE_Res:
      break;
    case SIGTYPE_Cu50:
      LCD_Buf[13] |= MEASURE_Cu50;
       break;      
    case SIGTYPE_PT100:
      LCD_Buf[13] |= MEASURE_Pt100;
       break;
    case SIGTYPE_K: //1.5.6.9.11.13.16.17.21.23.26.29.31.35
      LCD_Buf[8] |= 0x1F;
      LCD_Buf[9] |= 0x42;
      LCD_Buf[10] |= 0x88;
      LCD_Buf[11] |= 0x24;
      LCD_Buf[12] |= 0xE0;
     break;
    case SIGTYPE_E: //1.2.3.4.5.6.11.16.17.18.19.21.26.31.32.33.34.35  
      LCD_Buf[8] |= 0x9F;
      LCD_Buf[9] |= 0x99;
      LCD_Buf[10] |= 0x88;
      LCD_Buf[11] |= 0x88;
      LCD_Buf[12] |= 0xE8;
      break;
    case SIGTYPE_J://5.10.15.20.25.26.30.32.33.34
      LCD_Buf[10] |= 0x6F;
      LCD_Buf[11] |= 0x88;
      LCD_Buf[12] |= 0x48;
      break;
    case SIGTYPE_T://1.2.3.4.5.8.13.18.23.28.33
      LCD_Buf[8] |= 0x88;
      LCD_Buf[9] |= 0x8F;
      LCD_Buf[10] |= 0x08;
      LCD_Buf[11] |= 0xE0;
      break;
    case SIGTYPE_R://1.2.3.4.6.10.11.15.16.17.18.19.21.23.26.29.31.35
      LCD_Buf[8] |= 0x9F;
      LCD_Buf[9] |= 0x99;
      LCD_Buf[10] |= 0x86;
      LCD_Buf[11] |= 0x24;
      LCD_Buf[12] |= 0xE0;
      break;
    case SIGTYPE_B: //1.2.3.4.6.10.11.15.16.17.18.19.21.25.26.30.31.32.33.34
      LCD_Buf[8] |= 0x9F;
      LCD_Buf[9] |= 0x99;
      LCD_Buf[10] |= 0x66;
      LCD_Buf[11] |= 0x88;
      LCD_Buf[12] |= 0xE8;
      break;
    case SIGTYPE_S: //2.3.4.6.10.11.17.18.19.25.26.30.32.33.34
      LCD_Buf[8] |= 0x96;
      LCD_Buf[9] |= 0x99;
      LCD_Buf[10] |= 0x64;
      LCD_Buf[11] |= 0x88;
      LCD_Buf[12] |= 0x48;
      break;
    case SIGTYPE_N: //1.5.6.7.10.11.13.15.16.19.20.21.25.26.30.31.35
      LCD_Buf[8] |= 0x4F;
      LCD_Buf[9] |= 0x12;
      LCD_Buf[10] |= 0xEF;
      LCD_Buf[12] |= 0xE0;
      break;
    default:
      LCD_Buf[7] &= ~LOOP24V;   
      LCD_Buf[13] &= ~MEASURE_Cu50;     
      LCD_Buf[13] &= ~MEASURE_Pt100;      
      LCD_Buf[10]  &= ~0xEF;
      LCD_Buf[8]  &= ~0xFF;
      LCD_Buf[9]  &= ~0xFF;
      LCD_Buf[11] &= ~0xEE;
      LCD_Buf[12] &= ~0xEE;
      break;
  }
  HT16C23_WriteRAM_Ones(7, LCD_Buf[7]);
  HT16C23_WriteRAM_Ones(8, LCD_Buf[8]);
  HT16C23_WriteRAM_Ones(9, LCD_Buf[9]);
  HT16C23_WriteRAM_Ones(10, LCD_Buf[10]);
  HT16C23_WriteRAM_Ones(11, LCD_Buf[11]);
  HT16C23_WriteRAM_Ones(12, LCD_Buf[12]);
  HT16C23_WriteRAM_Ones(13, LCD_Buf[13]);  
  HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);
  HT16C23_WriteRAM_Ones(15, LCD_Buf[15]);
  HT16C23_WriteRAM_Ones(20, LCD_Buf[20]);
  HT16C23_WriteRAM_Ones(21, LCD_Buf[21]);
  HT16C23_WriteRAM_Ones(18, LCD_Buf[18]);
  HT16C23_WriteRAM_Ones(19, LCD_Buf[19]);
  HT16C23_WriteRAM_Ones(22, LCD_Buf[22]);
}

//Summary:	显示测量RJA
void	Write_MEASURE_RJA(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[13] |= MEASURE_RJA;
    else
      LCD_Buf[13] &= ~MEASURE_RJA;
      
    HT16C23_WriteRAM_Ones(13, LCD_Buf[13]);
}

//Summary:	显示测量RJM
void	Write_MEASURE_RJM(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[13] |= MEASURE_RJM;
    else
      LCD_Buf[13] &= ~MEASURE_RJM;
      
    HT16C23_WriteRAM_Ones(13, LCD_Buf[13]);
}

//Summary:	显示输出JA
void	Write_SOURSE_RJA(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[22] |= SOURCE_RJA;
    else
      LCD_Buf[22] &= ~SOURCE_RJA;
      
    HT16C23_WriteRAM_Ones(22, LCD_Buf[22]);
}

//Summary:	显示输出RJM
void	Write_SOURSE_RJM(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[22] |= SOURCE_RJM;
    else
      LCD_Buf[22] &= ~SOURCE_RJM;
      
    HT16C23_WriteRAM_Ones(22, LCD_Buf[22]);
}

//Summary:	显示自动关机标志
void	Write_AUTOPOWER(u8 u8Stat)
{  
    if(u8Stat == OFF)
      LCD_Buf[0] |= AUTOPOWER;
    else
      LCD_Buf[0] &= ~AUTOPOWER;
      
    HT16C23_WriteRAM_Ones(0, LCD_Buf[0]);
}
	
/********************************************************************
* Summary:    显示横线
* Parameters:
*             u8Stat ： ON点亮  OFF关闭
*       
* Return :    void
********************************************************************/
void	Write_Line(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[14] |= HG;
    else
      LCD_Buf[14] &= ~HG;
      
    HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);
}

//Summary:	显示输出端的RJMode
void	Write_RJModeS(u8 u8RJMode)
{
  switch(u8RJMode)
  {
    case RJMODE_NONE:
      Write_SOURSE_RJA(OFF);
      Write_SOURSE_RJM(OFF);  
      break;
    case RJMODE_AUTO:
      Write_SOURSE_RJA(ON);
      Write_SOURSE_RJM(OFF);
      break;
    case RJMODE_MAN:
      Write_SOURSE_RJA(OFF);
      Write_SOURSE_RJM(ON);
      break;
    default:
      Write_SOURSE_RJA(OFF);
      Write_SOURSE_RJM(OFF);
      break;
  }
}

//Summary:	显示测量端的RJMode
void	Write_RJModeM(u8 u8RJMode)
{
  switch(u8RJMode)
  {
    case RJMODE_NONE:
      Write_MEASURE_RJA(OFF);
      Write_MEASURE_RJM(OFF);  
      break;
    case RJMODE_AUTO:
      Write_MEASURE_RJA(ON);
      Write_MEASURE_RJM(OFF);
      break;
    case RJMODE_MAN:
      Write_MEASURE_RJA(OFF);
      Write_MEASURE_RJM(ON);
      break;
    default:
      Write_MEASURE_RJA(OFF);
      Write_MEASURE_RJM(OFF);
      break;
  }
}

/********************************************************************
* Summary:    显示电池电量框
* Parameters:
*             u8Stat ： ON点亮  OFF关闭
*       
* Return :    void
********************************************************************/
void	Write_BatteryLine(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[6] |= CELL_5;
    else
      LCD_Buf[6] &= ~CELL_5;
      
    HT16C23_WriteRAM_Ones(6, LCD_Buf[6]);
}




//Summary:	显示电池电量
void	Write_BatteryLevel(u8 u8Level)
{  
    LCD_Buf[6] |= CELL_5;
    
    switch(u8Level)
    {
     case 0:
       LCD_Buf[6] &= ~CELL_1;
       LCD_Buf[6] &= ~CELL_2;
       LCD_Buf[7] &= ~CELL_3;
       LCD_Buf[7] &= ~CELL_4;
       break;
     case 1:
        LCD_Buf[6] &= ~CELL_1;
        LCD_Buf[6] &= ~CELL_2;
        LCD_Buf[7] &= ~CELL_3;
        LCD_Buf[7] |= CELL_4;
       break;
     case 2:
      LCD_Buf[6] &= ~CELL_1;
      LCD_Buf[6] &= ~CELL_2;
      LCD_Buf[7] |= CELL_3;
      LCD_Buf[7] |= CELL_4;
       break;
     case 3:
      LCD_Buf[6] &= ~CELL_1;
      LCD_Buf[6] |= CELL_2;
      LCD_Buf[7] |= CELL_3;
      LCD_Buf[7] |= CELL_4;
       break;
     case 4:
      LCD_Buf[6] |= CELL_1;
      LCD_Buf[6] |= CELL_2;
      LCD_Buf[7] |= CELL_3;
      LCD_Buf[7] |= CELL_4; 
       break;  
    default:
       LCD_Buf[6] &= ~CELL_1;
       LCD_Buf[6] &= ~CELL_2;
       LCD_Buf[7] &= ~CELL_3;
       LCD_Buf[7] &= ~CELL_4;
       break;     
    }
         
    HT16C23_WriteRAM_Ones(6, LCD_Buf[6]);
    HT16C23_WriteRAM_Ones(7, LCD_Buf[7]);
}

//Summary:	显示充电标志
void	Write_PowerFlg(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[7] |= CELL_N;
    else
      LCD_Buf[7] &= ~CELL_N;
      
    HT16C23_WriteRAM_Ones(7, LCD_Buf[7]);
}

//Summary:	显示SOURCE ON/OFF
void	Write_SOURCE_ON_OFF(u8 u8Stat)
{  
    LCD_Buf[35] |= SOURCE;
    
    if(u8Stat == ON)
    {
      LCD_Buf[34] |= SOURCE_ON;
      LCD_Buf[34] &= ~SOURCE_OFF;
    }
    else
    {
      LCD_Buf[34] &= ~SOURCE_ON;
      LCD_Buf[34] |= SOURCE_OFF;
    }
      
    HT16C23_WriteRAM_Ones(34, LCD_Buf[34]);
    HT16C23_WriteRAM_Ones(35, LCD_Buf[35]);
}

//Summary:	显示SOURCE ON/OFF
void	Write_HOLD_ON_OFF(u8 u8Stat)
{  
    LCD_Buf[0] &= ~HOLD;
    
    if(u8Stat == ON)
    {
      LCD_Buf[0] |= HOLD;
    }
    else
    {
      LCD_Buf[0] &= ~HOLD;
    }
      
    HT16C23_WriteRAM_Ones(0, LCD_Buf[0]);
}


//Summary:	显示SOURCE数字-号
void	Write_SOURCE_MINUS(u8 u8Stat)
{     
    if(u8Stat == ON)
      LCD_Buf[34] |= SOURCE_MINUS;
    else
      LCD_Buf[34] &= ~SOURCE_MINUS;
      
    HT16C23_WriteRAM_Ones(34, LCD_Buf[34]);
}

//Summary:	显示SOURCE摄氏和华氏切换
void	Write_SOURCE_TempType(u8 u8Stat)
{     
    if(u8Stat == ON)
    {
      LCD_Buf[14] |= SOURCE_F;
      LCD_Buf[14] &= ~SOURCE_SSD;
    }
    else
    {
      LCD_Buf[14] |= SOURCE_SSD;
      LCD_Buf[14] &= ~SOURCE_F;
    }
      
    HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);
}


//Summary:	显示MEASURE摄氏和华氏切换
void	Write_MEASURE_TempType(u8 u8Stat)
{     
    if(u8Stat == ON)
    {
      LCD_Buf[7] |= MEASURE_F;
      LCD_Buf[7] &= ~MEASURE_SSD;
    }
    else
    {
      LCD_Buf[7] |= MEASURE_SSD;
      LCD_Buf[7] &= ~MEASURE_F;
    }
      
    HT16C23_WriteRAM_Ones(7, LCD_Buf[7]);
}




//Summary:	显示SOURCE RL,RH,SL,SH
void	Write_SOURCE_RangeFlg(u8 u8RangeFlg)
{ 
  switch(u8RangeFlg)
  {
    case RANGE_NONE:
      LCD_Buf[35] &= ~SOURCE_RL;
      LCD_Buf[35] &= ~SOURCE_RH;
      LCD_Buf[35] &= ~SOURCE_SL;
      LCD_Buf[14] &= ~SOURCE_SH;
      HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);
      HT16C23_WriteRAM_Ones(35, LCD_Buf[35]);
      break;
    case RANGE_RL:
      LCD_Buf[35] |= SOURCE_RL;
      LCD_Buf[35] &= ~SOURCE_RH;
      LCD_Buf[35] &= ~SOURCE_SL;
      LCD_Buf[14] &= ~SOURCE_SH;
      HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);
      HT16C23_WriteRAM_Ones(35, LCD_Buf[35]);
      break;
    case RANGE_RH:
      LCD_Buf[35] &= ~SOURCE_RL;
      LCD_Buf[35] |= SOURCE_RH;
      LCD_Buf[35] &= ~SOURCE_SL;
      LCD_Buf[14] &= ~SOURCE_SH;
      HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);
      HT16C23_WriteRAM_Ones(35, LCD_Buf[35]);
      break;
    case RANGE_SL:
      LCD_Buf[35] &= ~SOURCE_RL;
      LCD_Buf[35] &= ~SOURCE_RH;
      LCD_Buf[35] |= SOURCE_SL;
      LCD_Buf[14] &= ~SOURCE_SH;
      HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);
      HT16C23_WriteRAM_Ones(35, LCD_Buf[35]);
      break;
    case RANGE_SH:
      LCD_Buf[35] &= ~SOURCE_RL;
      LCD_Buf[35] &= ~SOURCE_RH;
      LCD_Buf[35] &= ~SOURCE_SL;
      LCD_Buf[14] |= SOURCE_SH;
      HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);
      HT16C23_WriteRAM_Ones(35, LCD_Buf[35]);
      break;
    default:
      LCD_Buf[35] &= ~SOURCE_RL;
      LCD_Buf[35] &= ~SOURCE_RH;
      LCD_Buf[35] &= ~SOURCE_SL;
      LCD_Buf[14] &= ~SOURCE_SH;
      HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);
      HT16C23_WriteRAM_Ones(35, LCD_Buf[35]);
      break;
  }
}

//Summary:  显示SOURCE单位
void    Write_SOURCE_Unit(u8 u8SigType)
{
  LCD_Buf[24] &= ~SOURCE_HZ;
  LCD_Buf[25] &= ~SOURCE_K;
  LCD_Buf[14] &= ~SOURCE_R;
  LCD_Buf[14] &= ~SOURCE_F;
  LCD_Buf[23] &= ~SOURCE_m;
  LCD_Buf[23] &= ~SOURCE_A;
  LCD_Buf[23] &= ~SOURCE_V;
  LCD_Buf[14] &= ~SOURCE_SSD;  
  switch(u8SigType)
  {
    case SIGTYPE_24VLOOP:
    case SIGTYPE_MA:
//    case SIGTYPE_SMA:
      LCD_Buf[23] |= SOURCE_m;
      LCD_Buf[23] |= SOURCE_A;
      LCD_Buf[23] &= ~SOURCE_V;
      LCD_Buf[24] &= ~SOURCE_HZ;
      LCD_Buf[25] &= ~SOURCE_K;
      LCD_Buf[14] &= ~SOURCE_R;
      LCD_Buf[14] &= ~SOURCE_F;
      LCD_Buf[14] &= ~SOURCE_SSD; 
      break;
    case SIGTYPE_MV:
      LCD_Buf[23] |= SOURCE_m;
      LCD_Buf[23] |= SOURCE_V;
      LCD_Buf[23] &= ~SOURCE_A;
      LCD_Buf[24] &= ~SOURCE_HZ;
      LCD_Buf[25] &= ~SOURCE_K;
      LCD_Buf[14] &= ~SOURCE_R;
      LCD_Buf[14] &= ~SOURCE_F;
      LCD_Buf[14] &= ~SOURCE_SSD;       
      break;
    case SIGTYPE_V:              
      LCD_Buf[23] |= SOURCE_V; 
      LCD_Buf[23] &= ~SOURCE_m;
      LCD_Buf[23] &= ~SOURCE_A;
      LCD_Buf[24] &= ~SOURCE_HZ;
      LCD_Buf[25] &= ~SOURCE_K;
      LCD_Buf[14] &= ~SOURCE_R;
      LCD_Buf[14] &= ~SOURCE_F;
      LCD_Buf[14] &= ~SOURCE_SSD;        
      break;
    case SIGTYPE_Res:  
      LCD_Buf[14] |= SOURCE_R; 
      LCD_Buf[23] &= ~SOURCE_m;
      LCD_Buf[23] &= ~SOURCE_A;
      LCD_Buf[24] &= ~SOURCE_HZ;
      LCD_Buf[25] &= ~SOURCE_K;
      LCD_Buf[14] &= ~SOURCE_V;
      LCD_Buf[14] &= ~SOURCE_F;
      LCD_Buf[14] &= ~SOURCE_SSD;
      break;   
    case SIGTYPE_Cu50:  
    case SIGTYPE_PT100:                 
    case SIGTYPE_K:
    case SIGTYPE_E:
    case SIGTYPE_J:
    case SIGTYPE_T:
    case SIGTYPE_R:
    case SIGTYPE_B:
    case SIGTYPE_S:
    case SIGTYPE_N:
       if(data.u8TempType == 0)
      {
        LCD_Buf[24] &= ~SOURCE_HZ;
        LCD_Buf[25] &= ~SOURCE_K;
        LCD_Buf[14] &= ~SOURCE_R;
        LCD_Buf[23] &= ~SOURCE_m;
        LCD_Buf[23] &= ~SOURCE_A;
        LCD_Buf[23] &= ~SOURCE_V;         
        LCD_Buf[14] |= SOURCE_SSD; 
        LCD_Buf[14] &= ~SOURCE_F;
      }
      else
      {
        LCD_Buf[24] &= ~SOURCE_HZ;
        LCD_Buf[25] &= ~SOURCE_K;
        LCD_Buf[14] &= ~SOURCE_R;
        LCD_Buf[23] &= ~SOURCE_m;
        LCD_Buf[23] &= ~SOURCE_A;
        LCD_Buf[23] &= ~SOURCE_V;        
        LCD_Buf[14] &= ~SOURCE_SSD;
        LCD_Buf[14] |= SOURCE_F; 
      }    
      break;
    case SIGTYPE_Hz:
      LCD_Buf[14] &= ~SOURCE_R; 
      LCD_Buf[23] &= ~SOURCE_m;
      LCD_Buf[23] &= ~SOURCE_A;
      LCD_Buf[24] |= SOURCE_HZ;
      LCD_Buf[25] &= ~SOURCE_K;
      LCD_Buf[14] &= ~SOURCE_V;
      LCD_Buf[14] &= ~SOURCE_F;
      LCD_Buf[14] &= ~SOURCE_SSD;
      break;         
    default:
      LCD_Buf[24] &= ~SOURCE_HZ;
      LCD_Buf[25] &= ~SOURCE_K;
      LCD_Buf[14] &= ~SOURCE_R;
      LCD_Buf[14] &= ~SOURCE_F;
      LCD_Buf[23] &= ~SOURCE_m;
      LCD_Buf[23] &= ~SOURCE_A;
      LCD_Buf[23] &= ~SOURCE_V;
      LCD_Buf[14] &= ~SOURCE_SSD;         
      break;
  }
  HT16C23_WriteRAM_Ones(25, LCD_Buf[25]);
  HT16C23_WriteRAM_Ones(23, LCD_Buf[23]);
  HT16C23_WriteRAM_Ones(24, LCD_Buf[24]);
  HT16C23_WriteRAM_Ones(14, LCD_Buf[14]);
}


//Summary:	显示SWEEP 模式
void	Write_SWEEPMode(u8 u8Sweep)
{
  switch(u8Sweep)
  {
    case SWEEP_NONE:
      LCD_Buf[29] &= ~STEP;
      LCD_Buf[29] &= ~SWEEP;
      HT16C23_WriteRAM_Ones(29, LCD_Buf[29]);
      break;
    case SWEEP_SWEEP:
      LCD_Buf[29] &= ~STEP;
      LCD_Buf[29] |= SWEEP;
      HT16C23_WriteRAM_Ones(29, LCD_Buf[29]);
      break;
    case SWEEP_STEP:
      LCD_Buf[29] |= STEP;
      LCD_Buf[29] &= ~SWEEP;
      HT16C23_WriteRAM_Ones(29, LCD_Buf[29]);
      break;
    default:
      LCD_Buf[29] &= ~STEP;
      LCD_Buf[29] &= ~SWEEP;
      HT16C23_WriteRAM_Ones(29, LCD_Buf[29]);
      break;
  }
}

//Summary:	显示TIME
void	Write_TIME(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[29] |= TIME;
    else
      LCD_Buf[29] &= ~TIME;
      
    HT16C23_WriteRAM_Ones(29, LCD_Buf[29]);
}

//Summary:	显示time t
void	Write_Time_t(u8 u8t)
{ 
  switch(u8t)
  {
    case TIME_T1:
      LCD_Buf[28] |= TIME_L1;
      LCD_Buf[28] &= ~TIME_L2;
      LCD_Buf[28] &= ~TIME_L3;
      LCD_Buf[28] &= ~TIME_L4;
      HT16C23_WriteRAM_Ones(28, LCD_Buf[28]);
      break;
    case TIME_T2:
      LCD_Buf[28] &= ~TIME_L1;
      LCD_Buf[28] |= TIME_L2;
      LCD_Buf[28] &= ~TIME_L3;
      LCD_Buf[28] &= ~TIME_L4;
      HT16C23_WriteRAM_Ones(28, LCD_Buf[28]);
      break;
    case TIME_T3:
      LCD_Buf[28] &= ~TIME_L1;
      LCD_Buf[28] &= ~TIME_L2;
      LCD_Buf[28] |= TIME_L3;
      LCD_Buf[28] &= ~TIME_L4;
      HT16C23_WriteRAM_Ones(28, LCD_Buf[28]);
      break;
    case TIME_T4:
      LCD_Buf[28] &= ~TIME_L1;
      LCD_Buf[28] &= ~TIME_L2;
      LCD_Buf[28] &= ~TIME_L3;
      LCD_Buf[28] |= TIME_L4;
      HT16C23_WriteRAM_Ones(28, LCD_Buf[28]);
      break;
    case TIME_SWEEP:
      LCD_Buf[28] |= TIME_L1;
      LCD_Buf[28] |= TIME_L2;
      LCD_Buf[28] |= TIME_L3;
      LCD_Buf[28] |= TIME_L4;
      HT16C23_WriteRAM_Ones(28, LCD_Buf[28]);
      break;
    case TIME_SWEEPSTEP1:
      LCD_Buf[28] &= ~TIME_L1;
      LCD_Buf[28] |= TIME_L2;
      LCD_Buf[28] |= TIME_L3;
      LCD_Buf[28] |= TIME_L4;
      HT16C23_WriteRAM_Ones(28, LCD_Buf[28]);
      break;
    case TIME_SWEEPSTEP2:
      LCD_Buf[28] |= TIME_L1;
      LCD_Buf[28] &= ~TIME_L2;
      LCD_Buf[28] |= TIME_L3;
      LCD_Buf[28] |= TIME_L4;
      HT16C23_WriteRAM_Ones(28, LCD_Buf[28]);
      break;
    case TIME_SWEEPSTEP3:
      LCD_Buf[28] |= TIME_L1;
      LCD_Buf[28] |= TIME_L2;
      LCD_Buf[28] &= ~TIME_L3;
      LCD_Buf[28] |= TIME_L4;
      HT16C23_WriteRAM_Ones(28, LCD_Buf[28]);
      break;
    case TIME_SWEEPSTEP4:
      LCD_Buf[28] |= TIME_L1;
      LCD_Buf[28] |= TIME_L2;
      LCD_Buf[28] |= TIME_L3;
      LCD_Buf[28] &= ~TIME_L4;
      HT16C23_WriteRAM_Ones(28, LCD_Buf[28]);
      break;
    default:
      LCD_Buf[28] &= ~TIME_L1;
      LCD_Buf[28] &= ~TIME_L2;
      LCD_Buf[28] &= ~TIME_L3;
      LCD_Buf[28] &= ~TIME_L4;
      HT16C23_WriteRAM_Ones(28, LCD_Buf[28]);
      break;
  }
}


//Summary:	显示COUNT
void	Write_COUNT(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[29] |= COUNT;
    else
      LCD_Buf[29] &= ~COUNT;
      
    HT16C23_WriteRAM_Ones(29, LCD_Buf[29]);
}


//Summary:	显示n
void	Write_n(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[28] |= STEP_N;
    else
      LCD_Buf[28] &= ~STEP_N;
      
    HT16C23_WriteRAM_Ones(28, LCD_Buf[28]);
}

//Summary:	显示m
void	Write_m(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[26] |= STEP_M;
    else
      LCD_Buf[26] &= ~STEP_M;
      
    HT16C23_WriteRAM_Ones(26, LCD_Buf[26]);
}

//Summary:	显示s
void	Write_s(u8 u8Stat)
{  
    if(u8Stat == ON)
      LCD_Buf[27] |= SEC;
    else
      LCD_Buf[27] &= ~SEC;
      
    HT16C23_WriteRAM_Ones(27, LCD_Buf[27]);
}

//Summary:	显示MEASURE数字
//          s16MValue       数字
//          u8Dot           小数点个数
void	Write_Value_MEASURE(s32* ps32MValue, u8 u8Dot, s32 s32L, s32 s32H)
{
  u8    u8Digbuf[6], i;
  u32   u32DigValue;
  
  if(*ps32MValue > s32H)
    *ps32MValue = s32H;
  if(*ps32MValue < s32L)
    *ps32MValue = s32L;
          
  if(*ps32MValue < 0)
  {
      Write_MEASURE_MINUS(ON);//正负号
      u32DigValue = 0 - *ps32MValue;
  }
  else
  {
      Write_MEASURE_MINUS(OFF);
      u32DigValue = *ps32MValue;
  }
      
  ValueToDigital(u8Digbuf, u32DigValue, u8Dot);
  
  for(i = 0; i < 6; i++)
  {
      Write_Digtial_MEASURE(i, u8Digbuf[i], i == (5 - u8Dot));
  }
  HT16C23_WriteRAM_Ones(2, LCD_Buf[2]);
  HT16C23_WriteRAM_Ones(3, LCD_Buf[3]);
  HT16C23_WriteRAM_Ones(1, LCD_Buf[1]);
  HT16C23_WriteRAM_Ones(4, LCD_Buf[4]);
  HT16C23_WriteRAM_Ones(5, LCD_Buf[5]);
  HT16C23_WriteRAM_Ones(6, LCD_Buf[6]);
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
            LCD_Buf[7] &= ~MEASURE_1;
            LCD_Buf[1] &= 0xFE;
        }
        else
        {
          LCD_Buf[7] |= MEASURE_1;
          if(Dot == 1)     //有小数点
          { 
            LCD_Buf[1] |= 0x01;
            LCD_Buf[2] &= 0xFE;
            LCD_Buf[3] &= 0xFE;
            LCD_Buf[4] &= 0xFE;
            LCD_Buf[5] &= 0xFE;
          }
        }  
        break;
      case 1:       //MEASURE 数字第2位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[1] &= 0x01;
          LCD_Buf[2] &= 0xFE;
        }
        else
        {
          LCD_Buf[1] &= 0x01;
          LCD_Buf[1] |= TAB_DIGTIAL_MEASURE[u8Digtial] ;
          if(Dot == 1)     //有小数点
          { 
            LCD_Buf[2] |= 0x01;
            LCD_Buf[1] &= 0xFE;
            LCD_Buf[3] &= 0xFE;
            LCD_Buf[4] &= 0xFE;
            LCD_Buf[5] &= 0xFE;
          }
        }    
        break;
      case 2:       //MEASURE 数字第3位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[2] &= 0x01;
          LCD_Buf[3] &= 0xFE;
        }
        else
        {
          LCD_Buf[2] &= 0x01;
          LCD_Buf[2] |= TAB_DIGTIAL_MEASURE[u8Digtial] ;
          if(Dot == 1)     //有小数点
          { 
            LCD_Buf[3] |= 0x01;
            LCD_Buf[2] &= 0xFE;
            LCD_Buf[1] &= 0xFE;
            LCD_Buf[4] &= 0xFE;
            LCD_Buf[5] &= 0xFE;
          }
        }
        break;
      case 3:       //MEASURE 数字第3位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[3] &= 0x01;
          LCD_Buf[4] &= 0xFE;
        }
        else
        {
          LCD_Buf[3] &= 0x01;
          LCD_Buf[3] |= TAB_DIGTIAL_MEASURE[u8Digtial] ;
          if(Dot == 1)     //有小数点
          { 
            LCD_Buf[4] |= 0x01;
            LCD_Buf[2] &= 0xFE;
            LCD_Buf[3] &= 0xFE;
            LCD_Buf[1] &= 0xFE;
            LCD_Buf[5] &= 0xFE;
          }
        }
        break;    
      case 4:       //MEASURE 数字第3位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[4] &= 0x01;
          LCD_Buf[5] &= 0xFE;
        }
        else
        {
          LCD_Buf[4] &= 0x01;
          LCD_Buf[4] |= TAB_DIGTIAL_MEASURE[u8Digtial] ;
          if(Dot == 1)     //有小数点
          { 
            LCD_Buf[5] |= 0x01;
            LCD_Buf[2] &= 0xFE;
            LCD_Buf[3] &= 0xFE;
            LCD_Buf[4] &= 0xFE;
            LCD_Buf[1] &= 0xFE;
          }
        }
        break;        
      case 5:       //MEASURE 数字第4位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[5] &= 0xF1;
          LCD_Buf[6] &= 0xF0;
        }
        else
        {
          LCD_Buf[5] &= 0xf1;
          LCD_Buf[5] |= (TAB_DIGTIAL_MEASURE[u8Digtial] & 0x0F);
          LCD_Buf[6] &= 0xf0;
          LCD_Buf[6] |= ((TAB_DIGTIAL_MEASURE[u8Digtial] & 0xF0) >> 4);          
        }
        break;
      default:
        break;
    }
}

//Summary:	显示SOURCE数字
//          s16MValue       数字
//          u8Dot           小数点个数
void	Write_Value_SOURCE(s32* ps32SValue, u8 u8Dot, s32 s32L, s32 s32H)
{
  u8    u8Digbuf[6], i;
  u32   u32DigValue;
  
  if(*ps32SValue > s32H)
    *ps32SValue = s32H;
  if(*ps32SValue < s32L)
    *ps32SValue = s32L;
          
  if(*ps32SValue < 0)
  {
      Write_SOURCE_MINUS(ON);//正负号
      u32DigValue = 0 - *ps32SValue;
  }
  else
  {
      Write_SOURCE_MINUS(OFF);
      u32DigValue = *ps32SValue;
  }
      
  ValueToDigital(u8Digbuf, u32DigValue, u8Dot);
  
  for(i = 0; i < 6; i++)
  {
      Write_Digtial_SOURCE(i, u8Digbuf[i], i == (5 - u8Dot));
  }
  HT16C23_WriteRAM_Ones(29, LCD_Buf[23]);
  HT16C23_WriteRAM_Ones(29, LCD_Buf[29]);
  HT16C23_WriteRAM_Ones(30, LCD_Buf[30]);
  HT16C23_WriteRAM_Ones(31, LCD_Buf[31]);
  HT16C23_WriteRAM_Ones(32, LCD_Buf[32]);
  HT16C23_WriteRAM_Ones(33, LCD_Buf[33]);
  HT16C23_WriteRAM_Ones(34, LCD_Buf[34]);
}

//Summary:	编辑SOURCE数字
//          s16MValue       数字
//          ot           小数点位子 0-P10 ， 1-P9， 2-P8， 3-P7
void	Edit_Value_SOURCE(u8 editNum[], u8 u8KeyPlace, u8 dot)
{
 
  LCD_Buf[34] &= ~SOURCE_1;  //1
  LCD_Buf[34] &= 0xFE;       //P6
  switch(u8KeyPlace)
  {
     case 1:
       //第1个数
          LCD_Buf[29] &= 0x0F;
          LCD_Buf[29] |= (TAB_DIGTIAL_SOURCE[editNum[0]] << 4);
          LCD_Buf[30] &= 0xF0;
          LCD_Buf[30] |= (TAB_DIGTIAL_SOURCE[editNum[0]] >> 4);    
       //其他不显示
          LCD_Buf[34] &= 0xF0;
          LCD_Buf[33] &= 0x0F;   
          LCD_Buf[31] &= 0xF0;
          LCD_Buf[30] &= 0x0F;   
          LCD_Buf[32] &= 0xF0;
          LCD_Buf[31] &= 0x0F;   
          LCD_Buf[33] &= 0xF0;
          LCD_Buf[32] &= 0x0F;  
          if(dot == 0)
            LCD_Buf[23] |= SOURCE_P12;
          else
            LCD_Buf[23] &= ~SOURCE_P12;
        break;
     case 2:
       //第2个数
          LCD_Buf[30] &= 0x0F;
          LCD_Buf[30] |= (TAB_DIGTIAL_SOURCE[editNum[0]] << 4);
          LCD_Buf[31] &= 0xF0;
          LCD_Buf[31] |= (TAB_DIGTIAL_SOURCE[editNum[0]] >> 4);          
          LCD_Buf[29] &= 0x0F;
          LCD_Buf[29] |= (TAB_DIGTIAL_SOURCE[editNum[1]] << 4);
          LCD_Buf[30] &= 0xF0;
          LCD_Buf[30] |= (TAB_DIGTIAL_SOURCE[editNum[1]] >> 4);         
       //其他不显示
          LCD_Buf[34] &= 0xF0;
          LCD_Buf[33] &= 0x0F;   
          LCD_Buf[33] &= 0xF0;
          LCD_Buf[32] &= 0x0F;   
          LCD_Buf[32] &= 0xF0;
          LCD_Buf[31] &= 0x0F;  
          if(dot == 0)
          {
             LCD_Buf[23] |= SOURCE_P12;
             LCD_Buf[30] &= 0xFE;
          }
          else if(dot == 1)
          {
            LCD_Buf[30] |= 0x01;
            LCD_Buf[23] &= ~SOURCE_P12;
          }        
          else
          {
             LCD_Buf[23] &= ~SOURCE_P12;
             LCD_Buf[30] &= 0xFE;
          }
        break;
     case 3:
       //第3个数
          LCD_Buf[31] &= 0x0F;
          LCD_Buf[31] |= (TAB_DIGTIAL_SOURCE[editNum[0]] << 4);
          LCD_Buf[32] &= 0xF0;
          LCD_Buf[32] |= (TAB_DIGTIAL_SOURCE[editNum[0]] >> 4);          
          LCD_Buf[30] &= 0x0F;
          LCD_Buf[30] |= (TAB_DIGTIAL_SOURCE[editNum[1]] << 4);
          LCD_Buf[31] &= 0xF0;
          LCD_Buf[31] |= (TAB_DIGTIAL_SOURCE[editNum[1]] >> 4);                
          LCD_Buf[29] &= 0x0F;
          LCD_Buf[29] |= (TAB_DIGTIAL_SOURCE[editNum[2]] << 4);
          LCD_Buf[30] &= 0xF0;
          LCD_Buf[30] |= (TAB_DIGTIAL_SOURCE[editNum[2]] >> 4);   
       //其他不显示 
          LCD_Buf[34] &= 0xF0;
          LCD_Buf[33] &= 0x0F;   
          LCD_Buf[33] &= 0xF0;
          LCD_Buf[32] &= 0x0F;
          if(dot == 0)
          {
             LCD_Buf[23] |= SOURCE_P12;
             LCD_Buf[30] &= 0xFE;
             LCD_Buf[31] &= 0xFE;
          }
          else if(dot == 1)
          {
            LCD_Buf[23] &= ~SOURCE_P12;
            LCD_Buf[30] |= 0x01;  //P10
            LCD_Buf[31] &= 0xFE;
          }
          else if(dot == 2)
          {
            LCD_Buf[23] &= ~SOURCE_P12;
            LCD_Buf[30] &= 0xFE;
            LCD_Buf[31] |= 0x01; //P9           
          }  
          else
          {
            LCD_Buf[23] &= ~SOURCE_P12;
            LCD_Buf[30] &= 0xFE;
            LCD_Buf[31] &= 0xFE; //P9             
          }
        break;
     case 4:
       //第4个数
          LCD_Buf[32] &= 0x0F;
          LCD_Buf[32] |= (TAB_DIGTIAL_SOURCE[editNum[0]] << 4);
          LCD_Buf[33] &= 0xF0;
          LCD_Buf[33] |= (TAB_DIGTIAL_SOURCE[editNum[0]] >> 4);          
          LCD_Buf[31] &= 0x0F;
          LCD_Buf[31] |= (TAB_DIGTIAL_SOURCE[editNum[1]] << 4);
          LCD_Buf[32] &= 0xF0;
          LCD_Buf[32] |= (TAB_DIGTIAL_SOURCE[editNum[1]] >> 4);                
          LCD_Buf[30] &= 0x0F;
          LCD_Buf[30] |= (TAB_DIGTIAL_SOURCE[editNum[2]] << 4);
          LCD_Buf[31] &= 0xF0;
          LCD_Buf[31] |= (TAB_DIGTIAL_SOURCE[editNum[2]] >> 4);            
          LCD_Buf[29] &= 0x0F;
          LCD_Buf[29] |= (TAB_DIGTIAL_SOURCE[editNum[3]] << 4);
          LCD_Buf[30] &= 0xF0;
          LCD_Buf[30] |= (TAB_DIGTIAL_SOURCE[editNum[3]] >> 4);   
       //其他不显示 
          LCD_Buf[34] &= 0xF0;
          LCD_Buf[33] &= 0x0F; 
          if(dot == 0)
          {
             LCD_Buf[23] |= SOURCE_P12;
             LCD_Buf[30] &= 0xFE;
             LCD_Buf[31] &= 0xFE;
             LCD_Buf[32] &= 0xFE;
          }          
          else if(dot == 1)
          {
            LCD_Buf[23] &= ~SOURCE_P12;
            LCD_Buf[30] |= 0x01;  //P10
            LCD_Buf[31] &= 0xFE;
            LCD_Buf[32] &= 0xFE;
          }
          else if(dot == 2)
          {
            LCD_Buf[23] &= ~SOURCE_P12;
            LCD_Buf[30] &= 0xFE;
            LCD_Buf[31] |= 0x01;  //P9
            LCD_Buf[32] &= 0xFE;
          }
          else if(dot == 3)
          {
            LCD_Buf[23] &= ~SOURCE_P12;
            LCD_Buf[30] &= 0xFE;
            LCD_Buf[31] &= 0xFE;
            LCD_Buf[32] |= 0x01;       
          }   
          else
          {
            LCD_Buf[23] &= ~SOURCE_P12;
            LCD_Buf[30] &= 0xFE;
            LCD_Buf[31] &= 0xFE;
            LCD_Buf[32] &= 0xFE;             
          }
        break;
     case 5:
       //第一个数
          LCD_Buf[33] &= 0x0F;
          LCD_Buf[33] |= (TAB_DIGTIAL_SOURCE[editNum[0]] << 4);
          LCD_Buf[34] &= 0xF0;
          LCD_Buf[34] |= (TAB_DIGTIAL_SOURCE[editNum[0]] >> 4);          
          LCD_Buf[32] &= 0x0F;
          LCD_Buf[32] |= (TAB_DIGTIAL_SOURCE[editNum[1]] << 4);
          LCD_Buf[33] &= 0xF0;
          LCD_Buf[33] |= (TAB_DIGTIAL_SOURCE[editNum[1]] >> 4);                
          LCD_Buf[31] &= 0x0F;
          LCD_Buf[31] |= (TAB_DIGTIAL_SOURCE[editNum[2]] << 4);
          LCD_Buf[32] &= 0xF0;
          LCD_Buf[32] |= (TAB_DIGTIAL_SOURCE[editNum[2]] >> 4);            
          LCD_Buf[30] &= 0x0F;
          LCD_Buf[30] |= (TAB_DIGTIAL_SOURCE[editNum[3]] << 4);
          LCD_Buf[31] &= 0xF0;
          LCD_Buf[31] |= (TAB_DIGTIAL_SOURCE[editNum[3]] >> 4);     
          LCD_Buf[29] &= 0x0F;
          LCD_Buf[29] |= (TAB_DIGTIAL_SOURCE[editNum[4]] << 4);
          LCD_Buf[30] &= 0xF0;
          LCD_Buf[30] |= (TAB_DIGTIAL_SOURCE[editNum[4]] >> 4);            
          
       //其他不显示  
          if(dot == 0)
          {
             LCD_Buf[23] |= SOURCE_P12;
             LCD_Buf[30] &= 0xFE;
             LCD_Buf[31] &= 0xFE;
             LCD_Buf[32] &= 0xFE;
             LCD_Buf[33] &= 0xFE;
          }             
          else if(dot == 1)
          {
            LCD_Buf[23] &= ~SOURCE_P12;
            LCD_Buf[30] |= 0x01;  //P10
            LCD_Buf[31] &= 0xFE;
            LCD_Buf[32] &= 0xFE;
            LCD_Buf[33] &= 0xFE;
          }
          else if(dot == 2)
          {
            LCD_Buf[23] &= ~SOURCE_P12;
            LCD_Buf[31] |= 0x01;  //P9
            LCD_Buf[30] &= 0xFE;
             LCD_Buf[32] &= 0xFE;
             LCD_Buf[33] &= 0xFE;
          }
          else if(dot == 3)
          {
            LCD_Buf[23] &= ~SOURCE_P12;
            LCD_Buf[32] |= 0x01;  //P8 
            LCD_Buf[30] &= 0xFE;
             LCD_Buf[31] &= 0xFE;
             LCD_Buf[33] &= 0xFE;
          }
          else if(dot == 4)
          {
            LCD_Buf[23] &= ~SOURCE_P12;
            LCD_Buf[33] |= 0x01;  //P7  
            LCD_Buf[30] &= 0xFE;
            LCD_Buf[32] &= 0xFE;
            LCD_Buf[31] &= 0xFE;
          }    
          else
          {
            LCD_Buf[23] &= ~SOURCE_P12;
            LCD_Buf[33] &= 0xFE;  //P7  
            LCD_Buf[30] &= 0xFE;
            LCD_Buf[32] &= 0xFE;
            LCD_Buf[31] &= 0xFE;            
          }
        break;
     default:   
        break;    
   }
  
  HT16C23_WriteRAM_Ones(30, LCD_Buf[30]);
  HT16C23_WriteRAM_Ones(31, LCD_Buf[31]);
  HT16C23_WriteRAM_Ones(32, LCD_Buf[32]);
  HT16C23_WriteRAM_Ones(33, LCD_Buf[33]);
  HT16C23_WriteRAM_Ones(34, LCD_Buf[34]);
  HT16C23_WriteRAM_Ones(29, LCD_Buf[29]);
  HT16C23_WriteRAM_Ones(23, LCD_Buf[23]);
}

//Summary:	编辑MESSURE数字
//          s16MValue       数字
//          ot           小数点位子 0-P10 ， 1-P9， 2-P8， 3-P7
void	Edit_Value_MEASURE(u8 editNum[], u8 u8KeyPlace, u8 dot)
{ 
  LCD_Buf[7] &= ~MEASURE_1;  //1
  LCD_Buf[1] &= 0xFE;       //P6
  switch(u8KeyPlace)
  {
     case 1:
       //第1个数
          LCD_Buf[5] &= 0xF0;
          LCD_Buf[5] |= (TAB_DIGTIAL_MEASURE[editNum[0]]);  
          LCD_Buf[6] &= 0xF0;
          LCD_Buf[6] |= (TAB_DIGTIAL_MEASURE[editNum[0]] >> 4);           
       //其他不显示
          LCD_Buf[4] &= 0x00;
          LCD_Buf[3] &= 0x00;   
          LCD_Buf[2] &= 0x00;
          LCD_Buf[1] &= 0x00;     
          if(dot == 0)
          {
            LCD_Buf[13] |=  MEASURE_P11;
          }
          else
          {
             LCD_Buf[13] &= ~MEASURE_P11;
          }
        break;
     case 2:
       //第2个数
          LCD_Buf[5] &= 0xF0;
          LCD_Buf[5] |= (TAB_DIGTIAL_MEASURE[editNum[1]]);  
          LCD_Buf[6] &= 0xF0;
          LCD_Buf[6] |= (TAB_DIGTIAL_MEASURE[editNum[1]] >> 4);    
          LCD_Buf[4] &= 0x00;
          LCD_Buf[4] |= TAB_DIGTIAL_MEASURE[editNum[0]];                  
       //其他不显示
          LCD_Buf[3] &= 0x00;
          LCD_Buf[2] &= 0x00;   
          LCD_Buf[1] &= 0x00;   
           if(dot == 0)
          {
            LCD_Buf[13] |=  MEASURE_P11;
            LCD_Buf[5] &= 0xFE;
          }
          else if(dot == 1)
          {
            LCD_Buf[5] |= 0x01;
            LCD_Buf[13] &=  ~MEASURE_P11;
          }      
          else
          {
            LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[5] &= 0xFE;           
          }
        break;
     case 3:
       //第3个数
          LCD_Buf[5] &= 0xF0;
          LCD_Buf[5] |= (TAB_DIGTIAL_MEASURE[editNum[2]]);  
          LCD_Buf[6] &= 0xF0;
          LCD_Buf[6] |= (TAB_DIGTIAL_MEASURE[editNum[2]] >> 4);  
          LCD_Buf[4] &= 0x00;
          LCD_Buf[4] |= TAB_DIGTIAL_MEASURE[editNum[1]];  
          LCD_Buf[3] &= 0x00;
          LCD_Buf[3] |= TAB_DIGTIAL_MEASURE[editNum[0]];              
       //其他不显示 
          LCD_Buf[2] &= 0x00;
          LCD_Buf[1] &= 0x00;   
           if(dot == 0)
          {
            LCD_Buf[13] |=  MEASURE_P11;
            LCD_Buf[5] &= 0xFE;
            LCD_Buf[4] &= 0xFE;  //P10
          }
          else if(dot == 1)
          {
           LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[4] &= 0xFE;
            LCD_Buf[5] |= 0x01;  //P10
          }
          else if(dot == 2)
          {
            LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[5] &= 0xFE;
            LCD_Buf[4] |= 0x01; //P9
          }  
          else
          {
            LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[5] &= 0xFE;    
            LCD_Buf[4] &= 0xFE;
          }
        break;
     case 4:
       //第4个数
          LCD_Buf[5] &= 0xF0;
          LCD_Buf[5] |= (TAB_DIGTIAL_MEASURE[editNum[3]]);  
          LCD_Buf[6] &= 0xF0;
          LCD_Buf[6] |= (TAB_DIGTIAL_MEASURE[editNum[3]] >> 4);   
          LCD_Buf[4] &= 0x00;
          LCD_Buf[4] |= TAB_DIGTIAL_MEASURE[editNum[2]];  
          LCD_Buf[3] &= 0x00;
          LCD_Buf[3] |= TAB_DIGTIAL_MEASURE[editNum[1]];                   
          LCD_Buf[2] &= 0x00;
          LCD_Buf[2] |= TAB_DIGTIAL_MEASURE[editNum[0]]; 
       //其他不显示 
          LCD_Buf[1] &= 0x00;
           if(dot == 0)
          {
            LCD_Buf[13] |=  MEASURE_P11;
            LCD_Buf[5] &= 0xFE;
            LCD_Buf[3] &= 0xFE;
            LCD_Buf[4] &= 0xFE;  //P10
          }         
          else if(dot == 1)
          {
            LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[3] &= 0xFE;
            LCD_Buf[4] &= 0xFE;
            LCD_Buf[5] |= 0x01;  //P10
          }
          else if(dot == 2)
          {
            LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[5] &= 0xFE;
            LCD_Buf[3] &= 0xFE;            
            LCD_Buf[4] |= 0x01;  //P9
          }
          else if(dot == 3)
          {
            LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[5] &= 0xFE;
            LCD_Buf[4] &= 0xFE;            
            LCD_Buf[3] |= 0x01;       
          }   
          else
          {
            LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[5] &= 0xFE;    
            LCD_Buf[4] &= 0xFE;
            LCD_Buf[3] &= 0xFE;  
          }          
        break;
     case 5:
       //第一个数
          LCD_Buf[5] &= 0xF0;
          LCD_Buf[5] |= (TAB_DIGTIAL_MEASURE[editNum[4]]);  
          LCD_Buf[6] &= 0xF0;
          LCD_Buf[6] |= (TAB_DIGTIAL_MEASURE[editNum[4]] >> 4);  
          LCD_Buf[4] &= 0x00;
          LCD_Buf[4] |= TAB_DIGTIAL_MEASURE[editNum[3]];  
          LCD_Buf[3] &= 0x00;
          LCD_Buf[3] |= TAB_DIGTIAL_MEASURE[editNum[2]];                   
          LCD_Buf[2] &= 0x00;
          LCD_Buf[2] |= TAB_DIGTIAL_MEASURE[editNum[1]]; 
          LCD_Buf[1] &= 0x00;
          LCD_Buf[1] |= TAB_DIGTIAL_MEASURE[editNum[0]];    
       //其他不显示    
           if(dot == 0)
          {
            LCD_Buf[13] |=  MEASURE_P11;
            LCD_Buf[5] &= 0xFE;
            LCD_Buf[3] &= 0xFE;
            LCD_Buf[2] &= 0xFE;
            LCD_Buf[4] &= 0xFE;  //P10
          }             
          else if(dot == 1)
          {
            LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[2] &= 0xFE;
            LCD_Buf[3] &= 0xFE;
            LCD_Buf[4] &= 0xFE;            
            LCD_Buf[5] |= 0x01;  //P10
          }
          else if(dot == 2)
          {
            LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[5] &= 0xFE;
            LCD_Buf[2] &= 0xFE;
            LCD_Buf[3] &= 0xFE;                 
            LCD_Buf[4] |= 0x01;  //P9
          }
          else if(dot == 3)
          {
            LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[5] &= 0xFE;
            LCD_Buf[4] &= 0xFE;
            LCD_Buf[2] &= 0xFE;                 
            LCD_Buf[3] |= 0x01;  //P8 
          }
          else if(dot == 4)
          {
            LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[5] &= 0xFE;
            LCD_Buf[3] &= 0xFE;
            LCD_Buf[4] &= 0xFE;                 
            LCD_Buf[2] |= 0x01;  //P7                  
          }     
          else
          {
            LCD_Buf[13] &= ~MEASURE_P11;
            LCD_Buf[5] &= 0xFE;    
            LCD_Buf[4] &= 0xFE;
            LCD_Buf[3] &= 0xFE;
            LCD_Buf[2] &= 0xFE; 
          }                
        break;
     default:   
        break;    
   }
  
  HT16C23_WriteRAM_Ones(1, LCD_Buf[1]);
  HT16C23_WriteRAM_Ones(2, LCD_Buf[2]);
  HT16C23_WriteRAM_Ones(3, LCD_Buf[3]);
  HT16C23_WriteRAM_Ones(4, LCD_Buf[4]);
  HT16C23_WriteRAM_Ones(5, LCD_Buf[5]);
  HT16C23_WriteRAM_Ones(6, LCD_Buf[6]);
  HT16C23_WriteRAM_Ones(7, LCD_Buf[7]);  
  HT16C23_WriteRAM_Ones(13, LCD_Buf[13]); 
}


//Summary:	显示SOURCE数字
//          u8Pos           位置
//          u8Digtial       数字
//          u8DotPos        小数点位置
void	Write_Digtial_SOURCE(u8 u8Pos, u8 u8Digtial, u8 Dot)
{ 
    LCD_Buf[23] &= ~SOURCE_P12;
    switch(u8Pos)
    {
      case 0:       //SOURCE 数字第1位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[34] &= ~SOURCE_1;         
        }
        else
        {  
          LCD_Buf[34] |= SOURCE_MINUS;
          if(Dot == 1)     //有小数点
          {
            LCD_Buf[34] |= 0x01;
            LCD_Buf[33] &= ~0x01;
            LCD_Buf[32] &= ~0x01;
            LCD_Buf[31] &= ~0x01;
            LCD_Buf[30] &= ~0x01;
          }
        }
        break;
      case 1:       //SOURCE 数字第2位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[33] &= 0x0F;
          LCD_Buf[34] &= 0xF1;          
        }
        else
        {
          LCD_Buf[33] &= 0x0F;
          LCD_Buf[33] |= (TAB_DIGTIAL_SOURCE[u8Digtial] << 4);
          LCD_Buf[34] &= 0xF1;
          LCD_Buf[34] |= (TAB_DIGTIAL_SOURCE[u8Digtial] >> 4);
          if(Dot == 1)     //有小数点
          {
            LCD_Buf[34] &= ~0x01;
            LCD_Buf[33] |= 0x01;
            LCD_Buf[32] &= ~0x01;
            LCD_Buf[31] &= ~0x01;
            LCD_Buf[30] &= ~0x01;
          }
        }
        break;
      case 2:       //SOURCE 数字第3位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[32] &= 0x0F;
          LCD_Buf[33] &= 0xF1;          
        }
        else
        {
          LCD_Buf[32] &= 0x0F;
          LCD_Buf[32] |= (TAB_DIGTIAL_SOURCE[u8Digtial] << 4);
          LCD_Buf[33] &= 0xF1;
          LCD_Buf[33] |= (TAB_DIGTIAL_SOURCE[u8Digtial] >> 4);
          if(Dot == 1)     //有小数点
          {
            LCD_Buf[34] &= ~0x01;
            LCD_Buf[33] &= ~0x01;
            LCD_Buf[32] |= 0x01;
            LCD_Buf[31] &= ~0x01;
            LCD_Buf[30] &= ~0x01;
          }
        }
        break;
      case 3:       //SOURCE 数字第4位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[31] &= 0x0F;
          LCD_Buf[32] &= 0xF1;          
        }
        else
        {
          LCD_Buf[31] &= 0x0F;
          LCD_Buf[31] |= (TAB_DIGTIAL_SOURCE[u8Digtial] << 4);
          LCD_Buf[32] &= 0xF1;
          LCD_Buf[32] |= (TAB_DIGTIAL_SOURCE[u8Digtial] >> 4);
          if(Dot == 1)     //有小数点
          {
            LCD_Buf[34] &= ~0x01;
            LCD_Buf[33] &= ~0x01;
            LCD_Buf[32] &= ~0x01;
            LCD_Buf[31] |= 0x01;
            LCD_Buf[30] &= ~0x01;
          }
        }
        break;
         case 4:       //SOURCE 数字第5位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[30] &= 0x0F;
          LCD_Buf[31] &= 0xF1;          
        }
        else
        {
          LCD_Buf[30] &= 0x0F;
          LCD_Buf[30] |= (TAB_DIGTIAL_SOURCE[u8Digtial] << 4);
          LCD_Buf[31] &= 0xF1;
          LCD_Buf[31] |= (TAB_DIGTIAL_SOURCE[u8Digtial] >> 4);
          if(Dot == 1)     //有小数点
          {
            LCD_Buf[34] &= ~0x01;
            LCD_Buf[33] &= ~0x01;
            LCD_Buf[32] &= ~0x01;
            LCD_Buf[31] &= ~0x01;
            LCD_Buf[30] |=  0x01;
          }
        }
        break;
      case 5:       //SOURCE 数字第6位
        if(u8Digtial == 10)     //不显示
        {
          LCD_Buf[29] &= 0x0F;
          LCD_Buf[30] &= 0xF1;          
        }
        else
        {
          LCD_Buf[29] &= 0x0F;
          LCD_Buf[29] |= (TAB_DIGTIAL_SOURCE[u8Digtial] << 4);
          LCD_Buf[30] &= 0xF1;
          LCD_Buf[30] |= (TAB_DIGTIAL_SOURCE[u8Digtial] >> 4);
          if(Dot == 1)     //有小数点
          {
            LCD_Buf[34] &= ~0x01;
            LCD_Buf[33] &= ~0x01;
            LCD_Buf[32] &= ~0x01;
            LCD_Buf[31] &= ~0x01;
            LCD_Buf[30] &= ~0x01;
          }          
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
  HT16C23_WriteRAM_Ones(25, LCD_Buf[25]);
  HT16C23_WriteRAM_Ones(26, LCD_Buf[26]);
  HT16C23_WriteRAM_Ones(27, LCD_Buf[27]);
  HT16C23_WriteRAM_Ones(28, LCD_Buf[28]);
}

//Summary:	显示TIME COUNT n数字
//          u8Pos           位置
//          u8Digtial       数字
void	Write_Digtial_TIME_COUNT_n(u8 u8Pos, u8 u8Digtial)
{ 
    switch(u8Pos)
    {
      case 0:       //数字第1位
        if(u8Digtial == 10)
        {
          LCD_Buf[28] &= 0xF8;
          LCD_Buf[27] &= 0x0F;
        }
        else
        {
          LCD_Buf[28] &= 0xF8;
          LCD_Buf[28] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0x07);
          LCD_Buf[27] &= 0x0F;
          LCD_Buf[27] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0xF0);
        }
        break;
      case 1:       //数字第2位
        if(u8Digtial == 10)
        {
          LCD_Buf[27] &= 0xF8;
          LCD_Buf[26] &= 0x0F;
        }
        else
        {
          LCD_Buf[27] &= 0xF8;
          LCD_Buf[27] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0x07);
          LCD_Buf[26] &= 0x0F;
          LCD_Buf[26] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0xF0);
        }
        break;
      case 2:       //数字第3位
        if(u8Digtial == 10)
        {
          LCD_Buf[26] &= 0xF8;
          LCD_Buf[25] &= 0x0F;
        }
        else
        {
          LCD_Buf[26] &= 0xF8;
          LCD_Buf[26] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0x07);
          LCD_Buf[25] &= 0x0F;
          LCD_Buf[25] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0xF0);

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
  HT16C23_WriteRAM_Ones(23, LCD_Buf[23]);  
  HT16C23_WriteRAM_Ones(24, LCD_Buf[24]);
  HT16C23_WriteRAM_Ones(25, LCD_Buf[25]);
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
          LCD_Buf[25] &= 0xF8;
          LCD_Buf[24] &= 0x0F;
        }
        else
        {
          LCD_Buf[25] &= 0xF8;
          LCD_Buf[25] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0x07);
          LCD_Buf[24] &= 0x0F;
          LCD_Buf[24] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0xF0);          
        }
        break;
      case 1:       //数字第2位
        if(u8Digtial == 10)
        {
          LCD_Buf[24] &= 0xF8;
          LCD_Buf[23] &= 0x0F;
        }
        else
        {
          LCD_Buf[24] &= 0xF8;
          LCD_Buf[24] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0x07);
          LCD_Buf[23] &= 0x0F;
          LCD_Buf[23] |= (TAB_DIGTIAL_SMALL[u8Digtial] & 0xF0);      
        }
        break;
      default:
        break;
    }
         
}

//Summary:	显示SOURCE数字
//          s16MValue       数字
//          u8Dot           小数点个数
 s32  Write_Value_fSOURCE(float* psfSValue, u8 fDot,u8 u8Dot, s32 s32L, s32 s32H)
{
  float floatL;
  float floatH;
  float fdivisor = 0;
  s32   s32SourceValue = 0;
  
 if(u8Dot == 0)
 {
   fdivisor = 10.0;
 }
 else if(u8Dot == 1)
 {
   fdivisor = 10.0;
 }
 else if(u8Dot == 2)
 {
   fdivisor = 100.0;
 }  
 else if(u8Dot == 3)
 {
   fdivisor = 1000.0;
 }  
 else if(u8Dot == 4)
 {
   fdivisor = 10000.0;
 } 
 else if(u8Dot == 5)
 {
   fdivisor = 100000.0;
 }  
 
 
  floatL = (float)s32L/(fdivisor);
  floatH = (float)s32H/(fdivisor);

  
   if(*psfSValue > floatH)
     *psfSValue = floatH;
  else if(*psfSValue < floatL)
     *psfSValue = floatL;   
 
  if(fDot <= u8Dot)
  {   
    switch(u8Dot)
    {
      case 0:
        s32SourceValue = (s32)(*psfSValue * 1);
        break;
      case 1:
        s32SourceValue = (s32)(*psfSValue * 10);
         break;
      case 2:
        s32SourceValue = (s32)(*psfSValue * 100);
        break;
      case 3:
        s32SourceValue = (s32)(*psfSValue * 1000);
        break;
      case 4:
        s32SourceValue = (s32)(*psfSValue * 10000);        
        break;
      default:
        break;      
    }
  }
  else
  {
    switch(u8Dot)
    {
      case 0:
        *psfSValue += 0.5;
        s32SourceValue = (s32)(*psfSValue);        
        break;
      case 1:
        *psfSValue += 0.05;
        s32SourceValue = (s32)(*psfSValue * 10);
         break;
      case 2:
        *psfSValue += 0.005;
        s32SourceValue = (s32)(*psfSValue * 100);
        break;
      case 3:
        *psfSValue += 0.0005;
        s32SourceValue = (s32)(*psfSValue * 1000);
        break;
      default:
        break;      
    }
  }

  return s32SourceValue;
}
  
  
//Summary:	显示SOURCE数字
//          s16MValue       数字
//          u8Dot           小数点个数
 s32  Write_UP_SOURCE(s32* psSValue, u8 fDot,u8 u8Dot, s32 s32L, s32 s32H)
{

  
     
  if(fDot <= u8Dot)
  {   
    switch(u8Dot - fDot)
    {
      case 0:
        * psSValue += 1;
        break;
      case 1:
        * psSValue += 10;
         break;
      case 2:
       * psSValue += 100;
        break;
      case 3:
        * psSValue += 1000;
        break;
      default:
        break;      
    }
  }
  else
  {
      * psSValue += 1;
  }

   if(*psSValue > s32H)
     *psSValue = s32H;
   if(*psSValue < s32L)
     *psSValue = s32L;

  
  return *psSValue;
}
  
 s32  Write_DOWN_SOURCE(s32* psSValue, u8 fDot,u8 u8Dot, s32 s32L, s32 s32H)
{

     
  if(fDot <= u8Dot)
  {   
    switch(u8Dot - fDot)
    {
      case 0:
        * psSValue -= 1;
        break;
      case 1:
        * psSValue -= 10;
         break;
      case 2:
       * psSValue -= 100;
        break;
      case 3:
        * psSValue -= 1000;
        break;
      default:
        break;      
    }
  }
  else
  {
      * psSValue -= 1;
  }

   if(*psSValue > s32H)
     *psSValue = s32H;
  else if(*psSValue < s32L)
     *psSValue = s32L;

  
  return *psSValue;
  
}  




