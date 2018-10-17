/*=============================================================================
//File:			viewMRSLH.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			MEASYRE RH
===============================================================================*/
#include "viewMRSLH.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewMain.h"
#include "viewFrame.h"

#define     SELF_VIEWID     VIEW_MRSLH

static  u8      l_u8Init = 0;               //初始化状态
static  u8      l_u8Step = RANGE_RL;


//Summary： MEASURE RL
static  void    DrawMRL(void)
{   
    Write_MEASURE_RangeFlg(RANGE_RL);
    Write_Value_MEASURE(&data.s16MRL, data.u8MRLHDot, -9999, 9999);
    Write_MEASURE_Unit(SIGTYPE_END); 
    Write_SigType(data.u8SSigType, data.u8MSigType);  //信号类型
}

//Summary： MEASURE RH
static  void    DrawMRH(void)
{   
    Write_MEASURE_RangeFlg(RANGE_RH);
    Write_Value_MEASURE(&data.s16MRH, data.u8MRLHDot, -9999, 9999);
    Write_MEASURE_Unit(SIGTYPE_END);
    Write_SigType(data.u8SSigType, data.u8MSigType);  //信号类型    
}

//Summary： MEASURE SL
static  void    DrawMSL(void)
{   
    Write_MEASURE_RangeFlg(RANGE_SL);
    Write_Value_MEASURE(&data.s16MSL, g_s16MSigRange[data.u8MSigType][2], 
          g_s16MSigRange[data.u8MSigType][0], g_s16MSigRange[data.u8MSigType][1]);
    Write_MEASURE_Unit(data.u8MSigType); 
    Write_SigType(data.u8SSigType, data.u8MSigType);  //信号类型
}

//Summary： MEASURE SL
static  void    DrawMSH(void)
{   
    Write_MEASURE_RangeFlg(RANGE_SH);
    Write_Value_MEASURE(&data.s16MSH, g_s16MSigRange[data.u8MSigType][2], 
          g_s16MSigRange[data.u8MSigType][0], g_s16MSigRange[data.u8MSigType][1]);
    Write_MEASURE_Unit(data.u8MSigType); 
    Write_SigType(data.u8SSigType, data.u8MSigType);  //信号类型
}

//Summary： 绘制静态刷新
static  void    DrawStatic(void)
{
    if(l_u8Step == RANGE_RL)
    {
      DrawMRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      DrawMRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      DrawMSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      DrawMSH();
    }
    else;
}

//Summary： 绘制动态刷新
static  void    DrawDynamic(void)
{
  
}

static  void    OnCreate(void)
{
    l_u8Step = RANGE_RL;
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
    if(l_u8Step == RANGE_RL)
    {
      data.s16MRL ++;
      DrawMRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16MRH ++;
      DrawMRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16MSL ++;
      DrawMSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16MSH ++;
      DrawMSH();
    }
    else;
  
  }
}

static  void    DoDOWN1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(l_u8Step == RANGE_RL)
    {
      data.s16MRL --;
      DrawMRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16MRH --;
      DrawMRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16MSL --;
      DrawMSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16MSH --;
      DrawMSH();
    }
    else;
  }
}

static  void    DoUP2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(l_u8Step == RANGE_RL)
    {
      data.s16MRL += 10;
      DrawMRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16MRH += 10;
      DrawMRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16MSL += 10;
      DrawMSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16MSH += 10;
      DrawMSH();
    }
    else;
  }
}

static  void    DoDOWN2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(l_u8Step == RANGE_RL)
    {
      data.s16MRL -= 10;
      DrawMRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16MRH -= 10;
      DrawMRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16MSL -= 10;
      DrawMSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16MSH -= 10;
      DrawMSH();
    }
    else;
  }
}

static  void    DoUP3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(l_u8Step == RANGE_RL)
    {
      data.s16MRL += 100;
      DrawMRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16MRH += 100;
      DrawMRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16MSL += 100;
      DrawMSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16MSH += 100;
      DrawMSH();
    }
    else;
  }
}

static  void    DoDOWN3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(l_u8Step == RANGE_RL)
    {
      data.s16MRL -= 100;
      DrawMRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16MRH -= 100;
      DrawMRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16MSL -= 100;
      DrawMSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16MSH -= 100;
      DrawMSH();
    }
    else;
  }
}

static  void    DoUP4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(l_u8Step == RANGE_RL)
    {
      data.s16MRL += 1000;
      DrawMRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16MRH += 1000;
      DrawMRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16MSL += 1000;
      DrawMSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16MSH += 1000;
      DrawMSH();
    }
    else;
  }
}


static  void    DoDOWN4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
      if(l_u8Step == RANGE_RL)
    {
      data.s16MRL -= 1000;
      DrawMRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16MRH -= 1000;
      DrawMRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16MSL -= 1000;
      DrawMSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16MSH -= 1000;
      DrawMSH();
    }
    else;
  }
}

static  void    DoPNKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
      if(l_u8Step == RANGE_RL)
    {
      data.s16MRL = 0 - data.s16MRL;
      DrawMRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16MRH = 0 - data.s16MRH;
      DrawMRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16MSL = 0 - data.s16MSL;
      DrawMSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16MSH = 0 - data.s16MSH;
      DrawMSH();
    }
    else;
  }
}

static  void    DoDotKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    if(l_u8Step == RANGE_RL)
    {
      data.u8MRLHDot ++;
      if(data.u8MRLHDot > 3)
        data.u8MRLHDot = 0;
      DrawMRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.u8MRLHDot ++;
      if(data.u8MRLHDot > 3)
        data.u8MRLHDot = 0;
      DrawMRH();
    }
    else;
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
    data.u8MeasureEnable =!data.u8MeasureEnable;        
    DrawMeasureEnable();
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
    if(data.u8MeasureEnable == ON)
    {
      if(l_u8Step < RANGE_SH)
      {
        l_u8Step ++;
        DrawStatic();
      }
      else
      {
        OnExit();
        SetView(VIEW_MAIN);
      }
    }
  }
}

static  void    DoTEMPKey(PKeyMsg pKeyMsg)
{
  if(data.u8SSigType > SIGTYPE_100MV)  //热电偶
  {
    if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
    {
    }
  }
  else
  {
    if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
    {
    }
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

void    ViewMRSLH(PKeyMsg pKeyMsg)
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
