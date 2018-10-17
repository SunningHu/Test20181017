/*=============================================================================
//File:			viewPrognm.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			n/m画面
===============================================================================*/
#include "viewPrognm.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewMain.h"
#include "viewFrame.h"

#define     MAX_M       20

#define     SELF_VIEWID     VIEW_PROGNM

static  u8      l_u8Init = 0;               //初始化状态
static  u16     l_u16n = 1;
static  u16     l_u16m = 1;
static  s16     l_s16SSValue = 0;       

//Summary： sourceValue *n/m的值
static  void    SourceValue_nm(void)
{ 
  l_s16SSValue = data.s16SourceValue * l_u16n / l_u16m;
}

//Summary： 显示n
static  void    Draw_n(void)
{  
    Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m);  
}

//Summary： 显示m
static  void    Draw_m(void)
{      
    Write_Value_m(&l_u16m, 1, MAX_M);  
}

//Summary： 绘制静态刷新
static  void    DrawStatic(void)
{
    Write_n(ON);
    Write_m(ON);
    Draw_n();
    Draw_m();
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
    l_u16n = 1;
    l_u16m = 1;
    l_u8Init = 1;
    //输出
    SourceValue_nm();
    if(data.u8SourceEnable == ON)
    {
      DoSource(data.u8SSigType, l_s16SSValue);
    }
    else
    {
      DoSource(SIGTYPE_END, l_s16SSValue);
    }
}

static  void    OnExit(void)
{
    Write_n(OFF);
    Write_m(OFF);
    Write_Value_TIME_COUNT_n(&l_u16n, 1000, 1000);//不显示
    Write_Value_m(&l_u16m, 100, 100);//不显示
    l_u8Init = 0;
}

static  void    DoUP1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    l_u16m ++;
    if(l_u16m > MAX_M)
      l_u16m = MAX_M;
    Draw_m();
    //输出
    SourceValue_nm();
    if(data.u8SourceEnable == ON)
    {
      DoSource(data.u8SSigType, l_s16SSValue);
    }
    else
    {
      DoSource(SIGTYPE_END, l_s16SSValue);
    }
  }
}

static  void    DoDOWN1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u16m > l_u16n)
        l_u16m --;
    Draw_m();
    //输出
    SourceValue_nm();
    if(data.u8SourceEnable == ON)
    {
      DoSource(data.u8SSigType, l_s16SSValue);
    }
    else
    {
      DoSource(SIGTYPE_END, l_s16SSValue);
    }
  }
}

static  void    DoUP2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    l_u16n ++;
    if(l_u16n > l_u16m)
      l_u16n = l_u16m;
    Draw_n();
    //输出
    SourceValue_nm();
    if(data.u8SourceEnable == ON)
    {
      DoSource(data.u8SSigType, l_s16SSValue);
    }
    else
    {
      DoSource(SIGTYPE_END, l_s16SSValue);
    }
  }
}

static  void    DoDOWN2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u16n > 0)
      l_u16n --;
    Draw_n();
    //输出
    SourceValue_nm();
    if(data.u8SourceEnable == ON)
    {
      DoSource(data.u8SSigType, l_s16SSValue);
    }
    else
    {
      DoSource(SIGTYPE_END, l_s16SSValue);
    }
  }
}

static  void    DoUP3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
  }
}

static  void    DoDOWN3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
  }
}

static  void    DoUP4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
  }
}


static  void    DoDOWN4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
  }
}

static  void    DoPNKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
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
    data.u8SourceEnable =!data.u8SourceEnable;
    DrawSourceEnable();
    //输出
    SourceValue_nm();
    if(data.u8SourceEnable == ON)
    {
      DoSource(data.u8SSigType, l_s16SSValue);
    }
    else
    {
      DoSource(SIGTYPE_END, l_s16SSValue);
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
    if(pKeyMsg->u8KeyAct == KEY_ACT_RELEA)
    {
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
    OnExit();
    SetView(VIEW_MAIN);
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

void    ViewPrognm(PKeyMsg pKeyMsg)
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
