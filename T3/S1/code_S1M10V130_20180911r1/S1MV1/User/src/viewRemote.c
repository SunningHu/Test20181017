/*=============================================================================
//File:			viewMain.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			主画面及操作
===============================================================================*/
#include "viewRemote.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewFrame.h"

#define     SELF_VIEWID         VIEW_REMOTE

static  u8      l_u8Init = 0;               //初始化状态
static  u16     l_u16n = 485;               //初始化状态

//Summary： SOURCE ON/OFF
static void  DrawSourceEnable(void)
{
    Write_SOURCE_ON_OFF(data.u8SourceEnable);
}

//Summary： SOURCE值
static void    DrawSourceData(void)
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
static void    DrawMeasureData(void)
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






//Summary： MEASURE ON
static void    DrawMeasureEnable(void)
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



//Summary： 绘制静态刷新
static  void    DrawStatic(void)
{
  Write_Value_TIME_COUNT_n(&l_u16n, 0, 500); 
  DrawSourceEnable();
  DrawSourceData();
  DrawMeasureEnable();
}

//Summary： 绘制动态刷新
static  void    DrawDynamic(void)
{
  DrawSourceEnable();
  DrawSourceData();
  DrawMeasureEnable();
  //输出
    if(data.u8SourceEnable == ON )
    {
      DoSource(data.u8SSigType, data.s16SourceValue);
    }
    else
    {
      DoSource(SIGTYPE_END, data.s16SourceValue);
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



static  void    DoUP1Key(PKeyMsg pKeyMsg)
{

}

static  void    DoDOWN1Key(PKeyMsg pKeyMsg)
{

}

static  void    DoUP2Key(PKeyMsg pKeyMsg)
{

}

static  void    DoDOWN2Key(PKeyMsg pKeyMsg)
{

}

static  void    DoUP3Key(PKeyMsg pKeyMsg)
{

}

static  void    DoDOWN3Key(PKeyMsg pKeyMsg)
{

}

static  void    DoUP4Key(PKeyMsg pKeyMsg)
{

}


static  void    DoDOWN4Key(PKeyMsg pKeyMsg)
{

}

static  void    DoPNKey(PKeyMsg pKeyMsg)
{

}

static  void    DoDotKey(PKeyMsg pKeyMsg)
{

}

static  void    DoSOURCEKey(PKeyMsg pKeyMsg)
{

}

static  void    DoSFUNCKey(PKeyMsg pKeyMsg)
{

}

static  void    DoSRANGEKey(PKeyMsg pKeyMsg)
{

}

static  void    DoMEASUREKey(PKeyMsg pKeyMsg)
{

}

static  void    DoMFUNCKey(PKeyMsg pKeyMsg)
{

}

static  void    DoMRANGEKey(PKeyMsg pKeyMsg)
{

}

static  void    DoTEMPKey(PKeyMsg pKeyMsg)
{

}

static  void    DoSWEEPKey(PKeyMsg pKeyMsg)
{

}

static  void    DoPROGKey(PKeyMsg pKeyMsg)
{

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

void    ViewRemote(PKeyMsg pKeyMsg)
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

