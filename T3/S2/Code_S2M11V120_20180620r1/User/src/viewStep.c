/*=============================================================================
//File:			viewStep.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			time 画面
===============================================================================*/
#include "viewStep.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewMain.h"
#include "viewFrame.h"

#define     MAX_M       20

#define     SELF_VIEWID     VIEW_STEP

static  u8      l_u8Init = 0;               //初始化状态
static  u8      l_u8DrawStep = 0;
static  u16     l_u16Time = 10;

static  u16     l_u16n = 1;
static  u16     l_u16m = 1;

static  u16     l_u16Stepn = 1;

static  s16     l_s16SValue = 0;        //Step 输出值
static  u16     l_u16Cnt = 0;       //计时
static  u8      l_u8Status = 0;
     

//Summary： 显示t1
static  void    Draw_Time(void)
{  
    Write_Value_TIME_COUNT_n(&l_u16Time, 0, 999);  
}

//Summary： 显示n
static  void    Draw_n(void)
{  
    Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m);  
}

//Summary： 显示step n
static  void    Draw_Stepn(void)
{  
    Write_Value_TIME_COUNT_n(&l_u16Stepn, 0, l_u16m);  
}

//Summary： 显示m
static  void    Draw_m(void)
{      
    Write_Value_m(&l_u16m, 1, MAX_M);  
}

//Summary： 绘制静态刷新
static  void    DrawStatic(void)
{
   if(l_u8DrawStep == 0)
   {
      Write_n(OFF);
      Write_m(OFF);
      Write_Value_TIME_COUNT_n(&l_u16n, 1000, 1000);//不显示
      Write_Value_m(&l_u16m, 100, 100);//不显示
      
      Write_TIME(ON);
      Write_s(ON);
      Write_Time_t(TIME_TEND);
      Draw_Time();
  
   }
   else
   {
      Write_TIME(OFF);
      Write_s(OFF);
      Write_Time_t(TIME_TEND);
      Write_Value_TIME_COUNT_n(&l_u16Time, 1000, 1000);//不显示
   
      Write_n(ON);
      Write_m(ON);
      Draw_n();
      Draw_m();
   }
}

//Summary： 绘制动态刷新
static  void    DrawDynamic(void)
{
  //测量
  if(data.u8MRangeEnable == 1)
    DrawMRangeValueRealTime();
  else
    DrawMeasureValueRealTime();
  //
  if(data.u8SourceEnable == ON)    //开始输出
  {
    l_u16Cnt ++;        //每秒++
    if(l_u16Cnt >= l_u16Time)   //n++
    { 
      l_u16Cnt = 0;
      if(l_u16Stepn < l_u16m && l_u8Status == 0)
      {
        l_u16Stepn ++;
      }
      else
      {
        l_u8Status = 1;
      }
      
      if(l_u16Stepn > l_u16n && l_u8Status == 1)
      {
        l_u16Stepn --;
      }
      else
      {
        l_u8Status = 0;
      }
      
      Draw_Stepn();
      
      //计算输出值
      l_s16SValue = data.s16SourceValue * l_u16Stepn / l_u16m;
      //输出
      if(data.u8SourceEnable == ON)
      {
        DoSource(data.u8SSigType, l_s16SValue);
      }
      else
      {
        DoSource(SIGTYPE_END, l_s16SValue);
      }
    }
  }
}

static  void    OnCreate(void)
{
    l_u8DrawStep = 0;
    l_u16Stepn = l_u16n;
    l_u8Init = 1;
}

static  void    OnExit(void)
{
   Write_TIME(OFF);
   Write_s(OFF);
   Write_Time_t(TIME_TEND);
   Write_Value_TIME_COUNT_n(&l_u16Time, 1000, 1000);//不显示
   
   Write_n(OFF);
   Write_m(OFF);
   Write_Value_TIME_COUNT_n(&l_u16n, 1000, 1000);//不显示
   Write_Value_m(&l_u16m, 100, 100);//不显示
   
   data.u8SweepMode = SWEEP_NONE;
      
   l_u8Init = 0;
}

static  void    DoUP1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u8DrawStep == 0)
    {
      if(data.u8SourceEnable == OFF)
      {
        l_u16Time ++;
        Draw_Time();
      }
    }
    else
    {
      if(data.u8SourceEnable == OFF)
      {
        l_u16m ++;
        if(l_u16m > MAX_M)
          l_u16m = MAX_M;
        Draw_m();
        l_u16Stepn = l_u16n;
      }
    }
  }
}

static  void    DoDOWN1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u8DrawStep == 0)
    {
      if(data.u8SourceEnable == OFF)
      {
        l_u16Time --;
        Draw_Time();
      }
    }
    else
    {
      if(data.u8SourceEnable == OFF)
      {
        if(l_u16m > l_u16n)
          l_u16m --;
        Draw_m();
        l_u16Stepn = l_u16n;
      }
    }
  }
}

static  void    DoUP2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u8DrawStep == 0)
    {
      if(data.u8SourceEnable == OFF)
      {
        l_u16Time += 10;
        Draw_Time();
      }
    }
    else
    {
      if(data.u8SourceEnable == OFF)
      {
        l_u16n ++;
        if(l_u16n > l_u16m)
          l_u16n = l_u16m;
        Draw_n();
        l_u16Stepn = l_u16n;
      }
    }
  }
}

static  void    DoDOWN2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u8DrawStep == 0)
    {
      if(data.u8SourceEnable == OFF)
      {
        l_u16Time -= 10;
        Draw_Time();
      }
    }
    else
    {
      if(data.u8SourceEnable == OFF)
      {
        if(l_u16n > 0)
          l_u16n --;
        Draw_n();
        l_u16Stepn = l_u16n;
      }
    }
  }
}

static  void    DoUP3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u8DrawStep == 0)
    {
      if(data.u8SourceEnable == OFF)
      {
        l_u16Time += 100;
        Draw_Time();
      }
    }
  }
}

static  void    DoDOWN3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u8DrawStep == 0)
    {
      if(data.u8SourceEnable == OFF)
      {
        l_u16Time -= 100;
        Draw_Time();
      }
    }
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
    if(l_u8DrawStep == 1)
    {
      data.u8SourceEnable =!data.u8SourceEnable;
      DrawSourceEnable();
      DrawStatic();
      //输出
      if(data.u8SourceEnable == ON)
      {
        DoSource(data.u8SSigType, data.s16SourceValue * l_u16n / l_u16m);
      }
      else
      {
        DoSource(SIGTYPE_END,  data.s16SourceValue * l_u16n / l_u16m);
      }
    }
    else
    {
      data.u8SourceEnable = OFF;
      DrawSourceEnable();
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
    data.u8MSigType ++;
    if(data.u8MSigType > SIGTYPE_END - 1)
      data.u8MSigType = SIGTYPE_24VLOOP;
    DrawMeasureData();
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
    if(l_u8DrawStep < 1)
    {
      l_u8DrawStep ++;
      DrawStatic();
    }
    else
    {
      OnExit();
      SetView(VIEW_MAIN);
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

void    ViewStep(PKeyMsg pKeyMsg)
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
