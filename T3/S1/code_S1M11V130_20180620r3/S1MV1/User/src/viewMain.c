/*=============================================================================
//File:			viewMain.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			主画面及操作
===============================================================================*/
#include "viewMain.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewFrame.h"

#define     SELF_VIEWID         VIEW_MAIN

static  u8      l_u8Init = 0;               //初始化状态

//Summary： SOURCE ON/OFF
void    DrawSourceEnable(void)
{
    Write_SOURCE_ON_OFF(data.u8SourceEnable);
}

//Summary： SOURCE值
void    DrawSourceData(void)
{
    Write_SOURCE_RangeFlg(RANGE_NONE);      //不显示RL,RH,SL,SH
    Write_Value_SOURCE(&data.s16SourceValue, g_s16SSigRange[data.u8SSigType][2], 
            g_s16SSigRange[data.u8SSigType][0], g_s16SSigRange[data.u8SSigType][1]);

    Write_SOURCE_Unit(data.u8SSigType);        //单位
    if(data.u8MeasureEnable == ON)
      Write_SigType(data.u8SSigType, data.u8MSigType);  //信号类型
    else
      Write_SigType(data.u8SSigType, SIGTYPE_END);  //信号类型      
}

//Summary： MEASURE值
void    DrawMeasureData(void)
{
    if(data.u8MeasureEnable == ON)
    {
      Write_MEASURE_RangeFlg(RANGE_NONE);

      Write_Value_MEASURE(&data.s16MeasureValue, g_s16MSigRange[data.u8MSigType][2],
            g_s16MSigRange[data.u8MSigType][0], g_s16MSigRange[data.u8MSigType][1]); 
      
      Write_MEASURE_Unit(data.u8MSigType);      
      Write_SigType(data.u8SSigType, data.u8MSigType);
    }
}

//Summary： MEASURE实时采样值
void    DrawMeasureValueRealTime(void)
{
    if(data.u8MeasureEnable == ON)
    {
        Write_Value_MEASURE(&data.s16MeasureValue, g_s16MSigRange[data.u8MSigType][2],
                g_s16MSigRange[data.u8MSigType][0], g_s16MSigRange[data.u8MSigType][1]);
        Write_MEASURE_Unit(data.u8MSigType);  
    }
}

//Summary： Range值换算成Source值
static  void    MValuetoMRange(void)
{ 
  s16   s16Tmp;
  
  s16Tmp = data.s16MSH - data.s16MSL;
  if(s16Tmp == 0)
    s16Tmp = 1;
   data.s16MRangeValue = (s32)((s32)(data.s16MRH - data.s16MRL) * (s32)(data.s16MeasureValue - data.s16MSL)) / s16Tmp + data.s16MRL; 
}

//Summary： MEASURE RANGE实时值
void    DrawMRangeValueRealTime(void)
{
    if(data.u8MeasureEnable == ON)
    {
        MValuetoMRange();
        Write_Value_MEASURE(&data.s16MRangeValue, data.u8MRLHDot, data.s16MRL, data.s16MRH);
        Write_MEASURE_Unit(SIGTYPE_END);  
    }
}

//Summary： MEASURE ON
void    DrawMeasureEnable(void)
{
    Write_MEASURE(ON);
    if(data.u8MeasureEnable == OFF)
    {
        Write_Value_MEASURE(0, 4, 0, 0);    //4位小数时，都不显示        
        Write_SigType(data.u8SSigType, SIGTYPE_END); 
        Write_MEASURE_Unit(SIGTYPE_END);
        Write_MEASURE_RangeFlg(RANGE_NONE);
    }
    else
    {
        DrawMeasureData();
    }
}

//Summary： SWEEP 模式
void    DrawSweepMode(void)
{
  Write_SWEEPMode(data.u8SweepMode);
}

//Summary： 绘制静态刷新
static  void    DrawStatic(void)
{
  u16 l_u16n = 50;
  Write_Value_TIME_COUNT_n(&l_u16n, 1000, 1000);
  DrawSourceEnable();
  DrawSourceData();
  DrawMeasureEnable();
  DrawSweepMode();
}

//Summary： 绘制动态刷新
static  void    DrawDynamic(void)
{
  if(data.u8MRangeEnable == 1)
    DrawMRangeValueRealTime();
  else
    DrawMeasureValueRealTime();
  
  //输出
    if(data.u8SourceEnable == ON &&  data.u8SSigType > SIGTYPE_100MV)
    {
      DoSource(data.u8SSigType, data.s16SourceValue);
    }
}


static  void    OnCreate(void)
{
    l_u8Init = 1;
    //输出
    if(data.u8SourceEnable == ON && data.u8MSigType != SIGTYPE_24VLOOP)
    {
      DoSource(data.u8SSigType, data.s16SourceValue);
    }
    else
    {
      DoSource(SIGTYPE_END, data.s16SourceValue);
    }
}

static  void    OnExit(void)
{
    l_u8Init = 0;
}

static  void    DoUP1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(data.u8MSigType != SIGTYPE_24VLOOP)
      {
        data.s16SourceValue ++;
        DrawSourceData();
        //输出
        if(data.u8SourceEnable == ON)
        {
          DoSource(data.u8SSigType, data.s16SourceValue);
        }
        else
        {
          DoSource(SIGTYPE_END, data.s16SourceValue);
        }
      }
  }
}

static  void    DoDOWN1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(data.u8MSigType != SIGTYPE_24VLOOP)
      {
        data.s16SourceValue --;
        DrawSourceData();
        //输出
        if(data.u8SourceEnable == ON)
        {
          DoSource(data.u8SSigType, data.s16SourceValue);
        }
        else
        {
          DoSource(SIGTYPE_END, data.s16SourceValue);
        }
      }
  }
}

static  void    DoUP2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(data.u8MSigType != SIGTYPE_24VLOOP)
      {
        data.s16SourceValue += 10;
        DrawSourceData();
        //输出
        if(data.u8SourceEnable == ON)
        {
          DoSource(data.u8SSigType, data.s16SourceValue);
        }
        else
        {
          DoSource(SIGTYPE_END, data.s16SourceValue);
        }
      }
  }
}

static  void    DoDOWN2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(data.u8MSigType != SIGTYPE_24VLOOP)
      {
        data.s16SourceValue -= 10;
        DrawSourceData();
        //输出
        if(data.u8SourceEnable == ON)
        {
          DoSource(data.u8SSigType, data.s16SourceValue);
        }
        else
        {
          DoSource(SIGTYPE_END, data.s16SourceValue);
        }
      }
  }
}

static  void    DoUP3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(data.u8MSigType != SIGTYPE_24VLOOP)
      {
        if(data.u8SSigType == SIGTYPE_4_20MA)
          data.s16SourceValue += 400;
        else
          data.s16SourceValue += 100;
        DrawSourceData();
        //输出
        if(data.u8SourceEnable == ON)
        {
          DoSource(data.u8SSigType, data.s16SourceValue);
        }
        else
        {
          DoSource(SIGTYPE_END, data.s16SourceValue);
        }
      }
  }
}

static  void    DoDOWN3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(data.u8MSigType != SIGTYPE_24VLOOP)
      {
        if(data.u8SSigType == SIGTYPE_4_20MA)
          data.s16SourceValue -= 400;
        else
          data.s16SourceValue -= 100;
        DrawSourceData();
        //输出
        if(data.u8SourceEnable == ON)
        {
          DoSource(data.u8SSigType, data.s16SourceValue);
        }
        else
        {
          DoSource(SIGTYPE_END, data.s16SourceValue);
        }
      }
  }
}

static  void    DoUP4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(data.u8MSigType != SIGTYPE_24VLOOP)
      {
        data.s16SourceValue += 1000;
        DrawSourceData();
        //输出
        if(data.u8SourceEnable == ON)
        {
          DoSource(data.u8SSigType, data.s16SourceValue);
        }
        else
        {
          DoSource(SIGTYPE_END, data.s16SourceValue);
        }
      }
  }
}


static  void    DoDOWN4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(data.u8MSigType != SIGTYPE_24VLOOP)
      {
        data.s16SourceValue -= 1000;
        DrawSourceData();
        //输出
        if(data.u8SourceEnable == ON)
        {
          DoSource(data.u8SSigType, data.s16SourceValue);
        }
        else
        {
          DoSource(SIGTYPE_END, data.s16SourceValue);
        }
      }
  }
}

static  void    DoPNKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    if(data.s16MRL == 0 && data.s16MRH == 0)
    {
    }
    else
    {
      if(data.u8MSigType <= SIGTYPE_100MV) //线性信号可切换
      {
        data.u8MRangeEnable = !data.u8MRangeEnable;
      }
      else
      {
        data.u8MRangeEnable = 0;
      }
      DrawDynamic();
    }
  }
}

static  void    DoDotKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    if(data.u8MSigType != SIGTYPE_24VLOOP)
    {
      if(data.s16SRL == 0 && data.s16SRH == 0)
      {
      }
      else
      {
        OnExit();
        SetView(VIEW_SRANGE);
      }
    }
  }
}

static  void    DoSOURCEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    if(data.u8MSigType != SIGTYPE_24VLOOP)
    {
      data.u8SourceEnable =!data.u8SourceEnable;
      DrawSourceEnable();
      //输出
      if(data.u8SourceEnable == ON)
      {
        DoSource(data.u8SSigType, data.s16SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s16SourceValue);
      }
    }
  }
}

static  void    DoSFUNCKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    if(data.u8MSigType != SIGTYPE_24VLOOP)
    {
      data.u8SSigType ++;
      if(data.u8SSigType > SIGTYPE_END - 1)
        data.u8SSigType = SIGTYPE_MA;
      DrawSourceData();    
      //信号类型切换过，自动变成OFF
      data.u8SourceEnable = OFF;
      DrawSourceEnable();
      DoSource(SIGTYPE_END, data.s16SourceValue);
    }
  }
}

static  void    DoSRANGEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    if(data.u8MSigType != SIGTYPE_24VLOOP)
    {
      if(data.u8SSigType <= SIGTYPE_100MV && data.u8SweepMode == SWEEP_NONE)   //线性信号，可设置RL,RH,SL,SH
      {
        //信号类型切换过，自动变成OFF
          data.u8SourceEnable = OFF;
          DrawSourceEnable();
          DoSource(SIGTYPE_END, data.s16SourceValue);
          OnExit();
          SetView(VIEW_SRSLH);
      }
    }
  }
}

static  void    DoMEASUREKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    data.u8MeasureEnable =!data.u8MeasureEnable;        
    DrawMeasureEnable();
  }
}

static  void    DoMFUNCKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    if(data.u8MeasureEnable == ON)
    {
      data.u8MRangeEnable = 0;
      data.u8MSigType ++;
      if(data.u8MSigType > SIGTYPE_END - 1)
        data.u8MSigType = SIGTYPE_24VLOOP;
      DrawMeasureData();
    }
  }
}

static  void    DoMRANGEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    if(data.u8MSigType <= SIGTYPE_100MV)   //线性信号，可设置RL,RH,SL,SH
    {
      OnExit();
      SetView(VIEW_MRSLH);
    }
  }
}

static  void    DoTEMPKey(PKeyMsg pKeyMsg)
{
  //if(data.u8MSigType != SIGTYPE_24VLOOP)
  //{
    if(data.u8MSigType > SIGTYPE_100MV || data.u8SSigType > SIGTYPE_100MV)  //热电偶
    {
      if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
      {
        OnExit();
        SetView(VIEW_RJTC);
      }
    }
    //else
    //{
    //  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
    //  {
    //    OnExit();
    //    SetView(VIEW_RJVIEW);
    //  }
    //}
  //}
}

static  void    DoSWEEPKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    if(data.u8MSigType != SIGTYPE_24VLOOP)
    {
      data.u8SweepMode ++;
      if(data.u8SweepMode > SWEEP_STEP)
        data.u8SweepMode = SWEEP_NONE;
      DrawSweepMode();
    }
  }
}

static  void    DoPROGKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    if(data.u8MSigType != SIGTYPE_24VLOOP)
    {
      if(data.u8SweepMode == SWEEP_NONE)
      {
        //信号类型切换过，自动变成OFF
          data.u8SourceEnable = OFF;
          DrawSourceEnable();
          DoSource(SIGTYPE_END, data.s16SourceValue);
        OnExit();
        SetView(VIEW_PROGNM);
      }
      else if(data.u8SweepMode == SWEEP_SWEEP)
      {
        //信号类型切换过，自动变成OFF
          data.u8SourceEnable = OFF;
          DrawSourceEnable();
          DoSource(SIGTYPE_END, data.s16SourceValue);
          OnExit();
          SetView(VIEW_SWEEP);
      }
      else if(data.u8SweepMode == SWEEP_STEP)
      {
        //信号类型切换过，自动变成OFF
          data.u8SourceEnable = OFF;
          DrawSourceEnable();
          DoSource(SIGTYPE_END, data.s16SourceValue);
        OnExit();
        SetView(VIEW_STEP);
      }
      else;
    }
  }
}

static    void    DoWithKey(PKeyMsg pKeyMsg)
{
    switch(pKeyMsg->u8KeyName)
    {
      case KEY_UP1:    
        DoUP1Key(pKeyMsg);
        break;
      case KEY_DOWN1:
        DoDOWN1Key(pKeyMsg);
        break;
      case KEY_UP2:
        DoUP2Key(pKeyMsg);
        break;
      case KEY_DOWN2:
        DoDOWN2Key(pKeyMsg);
        break;
      case KEY_UP3:   
        DoUP3Key(pKeyMsg);
        break;
      case KEY_DOWN3:
        DoDOWN3Key(pKeyMsg);
        break;
      case KEY_UP4:
        DoUP4Key(pKeyMsg);
        break;
      case KEY_DOWN4:
        DoDOWN4Key(pKeyMsg);
        break;
      case KEY_PN:
        DoPNKey(pKeyMsg);
        break;
      case KEY_DOT:
        DoDotKey(pKeyMsg);
        break;
      case KEY_SOURCE:
        DoSOURCEKey(pKeyMsg);
        break;
      case KEY_SFUNC:
        DoSFUNCKey(pKeyMsg);
        break;
      case KEY_SRANGE:
        DoSRANGEKey(pKeyMsg);
        break;
      case KEY_MESURE: 
        DoMEASUREKey(pKeyMsg);
        break;
      case KEY_MFUNC:
        DoMFUNCKey(pKeyMsg);
        break;
      case KEY_MRANGE:
        DoMRANGEKey(pKeyMsg);
        break;
      case KEY_TEMP:
        DoTEMPKey(pKeyMsg);
        break;
      case KEY_SWEEP:
        DoSWEEPKey(pKeyMsg);
        break;
      case KEY_PROG:
        DoPROGKey(pKeyMsg);
        break;
      default:
        break;
      
    }
}

void    ViewMain(PKeyMsg pKeyMsg)
{
    if(pKeyMsg)
        DoWithKey(pKeyMsg);
    
    if(GetView() == SELF_VIEWID)
    {
      if(l_u8Init == 0)
      {
          OnCreate();
          DrawStatic();
      }
      
      DrawDynamic();
    }
}

