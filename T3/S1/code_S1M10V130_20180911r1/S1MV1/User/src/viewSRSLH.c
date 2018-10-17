/*=============================================================================
//File:			viewSRSLH.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			SOURCE RL/SH/DL/SH
===============================================================================*/
#include "viewSRSLH.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewMain.h"
#include "viewFrame.h"

#define     SELF_VIEWID     VIEW_SRSLH

static  u8      l_u8Init = 0;               //初始化状态
static  u8      l_u8Step = RANGE_RL;
   

//Summary： SOURCE RL
static  void    DrawSRL(void)
{   
    Write_SOURCE_RangeFlg(RANGE_RL);
    Write_Value_SOURCE(&data.s16SRL, data.u8SRLHDot, -9999, 9999);
    Write_SOURCE_Unit(SIGTYPE_END);
    if(data.u8MeasureEnable == ON)
      Write_SigType(data.u8SSigType, data.u8MSigType);  //信号类型
    else
      Write_SigType(data.u8SSigType, SIGTYPE_END);  //信号类型  
}

//Summary： SOURCE RH
static  void    DrawSRH(void)
{   
    Write_SOURCE_RangeFlg(RANGE_RH);
    Write_Value_SOURCE(&data.s16SRH, data.u8SRLHDot, -9999, 9999);
    Write_SOURCE_Unit(SIGTYPE_END);
    if(data.u8MeasureEnable == ON)
      Write_SigType(data.u8SSigType, data.u8MSigType);  //信号类型
    else
      Write_SigType(data.u8SSigType, SIGTYPE_END);  //信号类型  
}

//Summary： SOURCE SL
static  void    DrawSSL(void)
{   
    Write_SOURCE_RangeFlg(RANGE_SL);
    Write_Value_SOURCE(&data.s16SSL, g_s16SSigRange[data.u8SSigType][2], 
          g_s16SSigRange[data.u8SSigType][0], g_s16SSigRange[data.u8SSigType][1]);
    Write_SOURCE_Unit(data.u8SSigType); 
    if(data.u8MeasureEnable == ON)
      Write_SigType(data.u8SSigType, data.u8MSigType);  //信号类型
    else
      Write_SigType(data.u8SSigType, SIGTYPE_END);  //信号类型  
}

//Summary： SOURCE SL
static  void    DrawSSH(void)
{   
    Write_SOURCE_RangeFlg(RANGE_SH);
    Write_Value_SOURCE(&data.s16SSH, g_s16SSigRange[data.u8SSigType][2], 
          g_s16SSigRange[data.u8SSigType][0], g_s16SSigRange[data.u8SSigType][1]);
    Write_SOURCE_Unit(data.u8SSigType); 
    if(data.u8MeasureEnable == ON)
      Write_SigType(data.u8SSigType, data.u8MSigType);  //信号类型
    else
      Write_SigType(data.u8SSigType, SIGTYPE_END);  //信号类型  
}

//Summary： 绘制静态刷新
static  void    DrawStatic(void)
{
    if(l_u8Step == RANGE_RL)
      DrawSRL();
    else if(l_u8Step == RANGE_RH)
      DrawSRH();
    else if(l_u8Step == RANGE_SL)
      DrawSSL();
    else if(l_u8Step == RANGE_SH)
      DrawSSH();
    else;
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
      data.s16SRL ++;
      DrawSRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16SRH ++;
      DrawSRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16SSL ++;
      DrawSSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16SSH ++;
      DrawSSH();
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
      data.s16SRL --;
      DrawSRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16SRH --;
      DrawSRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16SSL --;
      DrawSSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16SSH --;
      DrawSSH();
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
      data.s16SRL += 10;
      DrawSRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16SRH += 10;
      DrawSRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16SSL += 10;
      DrawSSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16SSH += 10;
      DrawSSH();
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
      data.s16SRL -= 10;
      DrawSRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16SRH -= 10;
      DrawSRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16SSL -= 10;
      DrawSSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16SSH -= 10;
      DrawSSH();
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
      data.s16SRL += 100;
      DrawSRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16SRH += 100;
      DrawSRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16SSL += 100;
      DrawSSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16SSH += 100;
      DrawSSH();
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
      data.s16SRL -= 100;
      DrawSRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16SRH -= 100;
      DrawSRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16SSL -= 100;
      DrawSSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16SSH -= 100;
      DrawSSH();
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
      data.s16SRL += 1000;
      DrawSRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16SRH += 1000;
      DrawSRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16SSL += 1000;
      DrawSSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16SSH += 1000;
      DrawSSH();
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
      data.s16SRL -= 1000;
      DrawSRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16SRH -= 1000;
      DrawSRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16SSL -= 1000;
      DrawSSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16SSH -= 1000;
      DrawSSH();
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
      data.s16SRL = 0 - data.s16SRL;
      DrawSRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.s16SRH = 0 - data.s16SRH;
      DrawSRH();
    }
    else if(l_u8Step == RANGE_SL)
    {
      data.s16SSL = 0 - data.s16SSL;
      DrawSSL();
    }
    else if(l_u8Step == RANGE_SH)
    {
      data.s16SSH = 0 - data.s16SSH;
      DrawSSH();
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
      data.u8SRLHDot ++;
      if(data.u8SRLHDot > 3)
        data.u8SRLHDot = 0;
      DrawSRL();
    }
    else if(l_u8Step == RANGE_RH)
    {
      data.u8SRLHDot ++;
      if(data.u8SRLHDot > 3)
        data.u8SRLHDot = 0;
      DrawSRH();
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

void    ViewSRSLH(PKeyMsg pKeyMsg)
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
