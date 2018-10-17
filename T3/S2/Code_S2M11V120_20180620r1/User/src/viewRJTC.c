/*=============================================================================
//File:			viewRJTC.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			热电偶冷端显示画面
===============================================================================*/
#include "viewRJTC.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewMain.h"
#include "viewFrame.h"

#define     SELF_VIEWID     VIEW_RJTC

static  u8      l_u8Init = 0;               //初始化状态
static  u8      l_u8Blink = 0;

//Summary： 显示冷端值
static  void    DrawRJTC(void)
{     
    s16     s16RJ;
    s16RJ = GetRJTemp();
      
    Write_Value_SOURCE(&s16RJ, 1, -9999, 9999);
    Write_RJMode(data.u8RJMode);
}

//Summary： 绘制静态刷新
static  void    DrawStatic(void)
{
    DrawRJTC();
    Write_SOURCE_Unit(SIGTYPE_K);   //℃ 
}

//Summary： 绘制动态刷新
static  void    DrawDynamic(void)
{
  //℃每秒闪烁一次
  DrawRJTC();
  l_u8Blink = !l_u8Blink;
  if(l_u8Blink == 0)
    Write_SOURCE_Unit(SIGTYPE_K);   //℃
  else
    Write_SOURCE_Unit(SIGTYPE_END);  
  
  if(data.u8MRangeEnable == 1)
    DrawMRangeValueRealTime();
  else
    DrawMeasureValueRealTime();
  
  //输出
  if(data.u8SourceEnable == ON && data.u8SSigType > SIGTYPE_100MV)
  {
    DoSource(data.u8SSigType, data.s16SourceValue);
  }
}

static  void    OnCreate(void)
{
    l_u8Blink = 0;
    l_u8Init = 1;
}

static  void    OnExit(void)
{
    Write_RJMode(RJMODE_NONE);
    l_u8Init = 0;
}

static  void    DoUP1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(data.u8RJMode == RJMODE_MAN)
    {
      data.s16RJSet++;
      DrawRJTC();
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
    if(data.u8RJMode == RJMODE_MAN)
    {
      data.s16RJSet--;
      DrawRJTC();
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
    if(data.u8RJMode == RJMODE_MAN)
    {
      data.s16RJSet += 10;
      DrawRJTC();
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
    if(data.u8RJMode == RJMODE_MAN)
    {
      data.s16RJSet -= 10;
      DrawRJTC();
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
    if(data.u8RJMode == RJMODE_MAN)
    {
      data.s16RJSet += 100;
      DrawRJTC();
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
    if(data.u8RJMode == RJMODE_MAN)
    {
      data.s16RJSet -= 100;
      DrawRJTC();
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
    if(data.u8RJMode == RJMODE_MAN)
    {
      data.s16RJSet += 1000;
      DrawRJTC();
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
    if(data.u8RJMode == RJMODE_MAN)
    {
      data.s16RJSet -= 1000;
      DrawRJTC();
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
    if(data.u8RJMode == RJMODE_MAN)
    {
      data.s16RJSet = 0 - data.s16RJSet;
      DrawRJTC();
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

static  void    DoDotKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
  }
}

static  void    DoSOURCEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
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
  }
}

static  void    DoMFUNCKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
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
    if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
    {
      OnExit();
      SetView(VIEW_MAIN);
    }
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

void    ViewRJTC(PKeyMsg pKeyMsg)
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
