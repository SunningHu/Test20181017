/*=============================================================================
//File:			viewFrame.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			画面框架
===============================================================================*/
#include "viewFrame.h"
#include "domsg.h"
#include "viewMain.h"
#include "viewSRange.h"
#include "viewSRSLH.h"
#include "viewMRSLH.h"
#include "viewRJView.h"
#include "viewRJTC.h"
#include "viewPrognm.h"
#include "viewSweep.h"
#include "viewStep.h"
#include "viewRemote.h"
#include "dodata.h"
#include "gui.h"
#include "battery.h"
#include "lcd.h"
#include "measure.h"
#include "stm8l15x_adc.h"
#include "timer.h"

//定义版本号
static   s16     l_s16Serial = 52;  //S2
static   s16     l_s16Version = 120;  //

static  u8  l_u8CurView = 0;

//Summary:  画面框架
void    ViewFrame(PKeyMsg pKeyMsg)
{
  switch(l_u8CurView)
  {
    case VIEW_MAIN:
      ViewMain(pKeyMsg);
      break;
    case VIEW_SRANGE:
      ViewSRange(pKeyMsg);
      break;
    case VIEW_SRSLH:
      ViewSRSLH(pKeyMsg);
      break;
    case VIEW_MRSLH:
      ViewMRSLH(pKeyMsg);
      break;
    case VIEW_RJVIEW:
      ViewRJView(pKeyMsg);
      break;
    case VIEW_RJTC:
      ViewRJTC(pKeyMsg);
      break;
    case VIEW_PROGNM:
      ViewPrognm(pKeyMsg);
      break;
    case VIEW_SWEEP:
      ViewSweep(pKeyMsg);
      break;
    case VIEW_STEP:
      ViewStep(pKeyMsg);
      break;
    case VIEW_REMOTE:
      ViewRemote(pKeyMsg);
      break;      
    default:
      break;
  }
}

//Summary:  画面切换
void    SetView(u8  u8CurView)
{
  l_u8CurView = u8CurView;
  PostMsg(MSG_PAINT, 1);
}

//Summary:  
u8    GetView(void)
{
  return l_u8CurView;
}


//Summary:  初始化画面显示
void    Init_ViewFrame(void)
{
  u8    i;
  //显示开机画面
  LCDClr();
  //显示版本号
  Write_Value_MEASURE(&l_s16Serial, 0, 0, 1000);
  Write_Value_SOURCE(&l_s16Version, 0, 0, 1000);
  
  //进行10次AD采样
  for(i = 0; i < MAX_BUF; i++)
  {
    ADC_SoftwareStartConv(ADC1);  
    delay_ms(20);
  }
  LCDClr();
  
  l_u8CurView = 0;
  SetView(l_u8CurView);
  
  PostMsg(MSG_PAINT, 1);
  PostMsg(MSG_1S, 1);
}