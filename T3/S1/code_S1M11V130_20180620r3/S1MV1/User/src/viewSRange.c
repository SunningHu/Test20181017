/*=============================================================================
//File:			viewSRange.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			SOURCE换算后画面
===============================================================================*/
#include "viewSRange.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewMain.h"
#include "viewFrame.h"

#define     SELF_VIEWID     VIEW_SRANGE

static  u8      l_u8Init = 0;               //初始化状态


//Summary： Range值换算成Source值
static  void    SRangetoSValue(void)
{ 
  s16   s16Tmp;
  
  s16Tmp = data.s16SRH - data.s16SRL;
  if(s16Tmp == 0)
    s16Tmp = 1;
   data.s16SourceValue = (s32)((s32)(data.s16SSH - data.s16SSL) * (s32)(data.s16SRangeValue - data.s16SRL)) / s16Tmp + data.s16SSL; 
}

//Summary： SOURCE 换算后值
static  void    DrawSRangeValue(void)
{      
    Write_SOURCE_RangeFlg(RANGE_NONE);
    Write_Value_SOURCE(&data.s16SRangeValue, data.u8SRLHDot, data.s16SRL, data.s16SRH);
    Write_SOURCE_Unit(SIGTYPE_END); 
}

//Summary： 绘制静态刷新
static  void    DrawStatic(void)
{
  DrawSRangeValue();
}

//Summary： 绘制动态刷新
static  void    DrawDynamic(void)
{  
  if(data.u8MRangeEnable == 1)
    DrawMRangeValueRealTime();
  else
    DrawMeasureValueRealTime();
}

static  void    OnCreate(void)
{
    data.s16SRangeValue = 0;
    l_u8Init = 1;
}

static  void    OnExit(void)
{
    l_u8Init = 0;
}

static  void    DoUP1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      data.s16SRangeValue ++;
      DrawSRangeValue();
      
      //输出
      
      if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s16SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s16SourceValue);
      }
  }
}

static  void    DoDOWN1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      data.s16SRangeValue --;
      DrawSRangeValue();
      //输出
      
      if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s16SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s16SourceValue);
      }
  }
}

static  void    DoUP2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      data.s16SRangeValue += 10;
      DrawSRangeValue();
      //输出
      
      if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s16SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s16SourceValue);
      }
  }
}

static  void    DoDOWN2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      data.s16SRangeValue -= 10;
      DrawSRangeValue();
      //输出
      
      if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s16SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s16SourceValue);
      }
  }
}

static  void    DoUP3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      data.s16SRangeValue += 100;
      DrawSRangeValue();
      //输出
      
      if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s16SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s16SourceValue);
      }
  }
}

static  void    DoDOWN3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      data.s16SRangeValue -= 100;
      DrawSRangeValue();
      //输出
      
      if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s16SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s16SourceValue);
      }
  }
}

static  void    DoUP4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      data.s16SRangeValue += 1000;
      DrawSRangeValue();
      //输出
      
      if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s16SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s16SourceValue);
      }
  }
}


static  void    DoDOWN4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      data.s16SRangeValue -= 1000;
      DrawSRangeValue();
      //输出
      
      if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s16SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s16SourceValue);
      }
  }
}

static  void    DoPNKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
      data.s16SRangeValue = 0 - data.s16SRangeValue;
      DrawSRangeValue();
      //输出
      
      if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s16SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s16SourceValue);
      }
  }
}

static  void    DoDotKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    //信号类型切换过，自动变成OFF
    data.u8SourceEnable = OFF;
    DrawSourceEnable();
    DoSource(SIGTYPE_END, data.s16SourceValue);
    OnExit();
    SetView(VIEW_MAIN);
    
  }
}

static  void    DoSOURCEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    data.u8SourceEnable =!data.u8SourceEnable;
    DrawSourceEnable();
    //输出
      
      if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s16SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s16SourceValue);
      }
  }
}

static  void    DoSFUNCKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {  
  }
}

static  void    DoSRANGEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
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
  }
}

static  void    DoTEMPKey(PKeyMsg pKeyMsg)
{
  if(data.u8MSigType > SIGTYPE_100MV)  //热电偶
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
}

static  void    DoSWEEPKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
  }
}

static  void    DoPROGKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
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

void    ViewSRange(PKeyMsg pKeyMsg)
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
