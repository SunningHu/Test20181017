/*=============================================================================
//File:			viewRJView.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			冷端显示画面
===============================================================================*/
#include "viewRJView.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewMain.h"
#include "viewFrame.h"
#include "doMsg.h"

#define     SELF_VIEWID     VIEW_RJVIEW

static  u8      l_u8Init = 0;               //初始化状态
static  u8      l_u8Blink = 0;

static  u8      l_u8RJViewFlg = 0;      //不在RJView画面
//Summary： 显示冷端值
static  void    DrawRJView(void)
{     
    s16     s16RJAuto;
    
    s16RJAuto = GetRJAuto();
    Write_Value_SOURCE(&s16RJAuto, 1, -9999, 9999);  
}

//Summary： 绘制静态刷新
static  void    DrawStatic(void)
{
    DrawRJView();
    Write_SOURCE_Unit(SIGTYPE_K);   //℃
    Write_RJMode(RJMODE_AUTO);
}

//Summary： 绘制动态刷新
static  void    DrawDynamic(void)
{
  //℃每秒闪烁一次
  DrawRJView();
  l_u8Blink = !l_u8Blink;
  if(l_u8Blink == 0)
    Write_SOURCE_Unit(SIGTYPE_K);   //℃
  else
    Write_SOURCE_Unit(SIGTYPE_END);  
  
  if(data.u8MRangeEnable == 1)
    DrawMRangeValueRealTime();
  else
    DrawMeasureValueRealTime();
}

static  void    OnCreate(void)
{
    l_u8Blink = 0;
    l_u8Init = 1;
    l_u8RJViewFlg = 1;
    
    edit.u8WriteStutus = 0;
    memset(editNuber, 0xFF, sizeof(editNuber));
    edit.u8KeyPlaces = 0;
    edit.u8DotPlace = 0;
    edit.u8KeyPN = 0;     
}

static  void    OnExit(void)
{
    Write_RJMode(RJMODE_NONE);
    l_u8Init = 0;
    l_u8RJViewFlg = 0;
}

static  void    DoUP1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN || pKeyMsg->u8KeyAct == KEY_ACT_LONG)
  {
  }
}

static  void    DoDOWN1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN || pKeyMsg->u8KeyAct == KEY_ACT_LONG)
  {
  }
}

static  void    DoUP2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN || pKeyMsg->u8KeyAct == KEY_ACT_LONG)
  {
  }
}

static  void    DoDOWN2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN || pKeyMsg->u8KeyAct == KEY_ACT_LONG)
  {
  }
}

static  void    DoUP3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN || pKeyMsg->u8KeyAct == KEY_ACT_LONG)
  {
  }
}

static  void    DoDOWN3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN || pKeyMsg->u8KeyAct == KEY_ACT_LONG)
  {
  }
}

static  void    DoUP4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN || pKeyMsg->u8KeyAct == KEY_ACT_LONG)
  {
  }
}


static  void    DoDOWN4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN || pKeyMsg->u8KeyAct == KEY_ACT_LONG)
  {
  }
}

static  void    DoPNKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
  }
}

static  void    DoDotKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
  }
}

static  void    DoSOURCEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
  }
}

static  void    DoSFUNCKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {  
  }
}

static  void    DoSRANGEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
  }
}

static  void    DoMEASUREKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
  }
}

static  void    DoMFUNCKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
  }
}

static  void    DoMRANGEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
  }
}

static  void    DoTEMPKey(PKeyMsg pKeyMsg)
{
    if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
    {
      OnExit();
      SetView(VIEW_MAIN);
    }
}

static  void    DoSWEEPKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
  }
}

static  void    DoPROGKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
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

void    ViewRJView(PKeyMsg pKeyMsg)
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
      
      if(l_u8RJViewFlg == 1 && GetMsg(MSG_PAINT, 1) != 0)
          DrawDynamic();
    }
}
