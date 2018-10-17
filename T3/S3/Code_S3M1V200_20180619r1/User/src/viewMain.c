/*=============================================================================
//File:			viewMain.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			主画面及操作
===============================================================================*/
#include "viewMain.h"
#include "stm8l15x_tim1.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim5.h"
#include "dodata.h"
#include "viewFrame.h"
#include <string.h>
#include "DriveEEprom.h"
#include "lcd.h"

#define     SELF_VIEWID         VIEW_MAIN
extern  s16  FlagI;

static  u8      l_u8Init = 0;               //初始化状态
u8  editNuber[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

//Summary： SOURCE ON/OFF
void    DrawSourceEnable(void)
{
    Write_SOURCE_ON_OFF(data.u8SourceEnable);
}

//Summary： SOURCE ON/OFF
void    DrawHoldEnable(void)
{
    Write_HOLD_ON_OFF(data.u8MHoldEnable);
}

//Summary： SOURCE值
void    DrawSourceData(void)
{
    Write_SOURCE_RangeFlg(RANGE_NONE);      //不显示RL,RH,SL,SH
    
    if(edit.u8WriteStutus == 1)
    {
     Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
    }
    else
    {   
        Write_Value_SOURCE(&data.s32SourceValue, g_s32SSigRange[data.u8SSigType][2], 
                g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);          
    }


    Write_SOURCE_Unit(data.u8SSigType);        //单位
    if(data.u8MeasureEnable == ON)
      Write_SigType(data.u8SSigType, data.u8MSigType);  //信号类型
    else
      Write_SigType(data.u8SSigType, SIGTYPE_END);  //信号类型    
    
// Write_Value_SOURCE(&adCode.u16RRef, 0, 
//                0, 70000);     
    
}




//Summary： MEASURE值
void    DrawMeasureData(void)
{
    if(data.u8MeasureEnable == ON)
    {
      Write_MEASURE_RangeFlg(RANGE_NONE);
      
      Write_Value_MEASURE(&data.s32MeasureValue, g_s32MSigRange[data.u8MSigType][2],
            g_s32MSigRange[data.u8MSigType][0], g_s32MSigRange[data.u8MSigType][1]); 
     
      
      Write_MEASURE_Unit(data.u8MSigType);      
      Write_SigType(data.u8SSigType, data.u8MSigType);
    }
}

//Summary： MEASURE实时采样值
void    DrawMeasureValueRealTime(void)
{
    if(data.u8MeasureEnable == ON)
    {
        Write_Value_MEASURE(&data.s32MeasureValue, g_s32MSigRange[data.u8MSigType][2],
                g_s32MSigRange[data.u8MSigType][0], g_s32MSigRange[data.u8MSigType][1]);
        Write_MEASURE_Unit(data.u8MSigType);                        
    }
//    if(data.u8MeasureEnable == ON)
//    {
//        Write_Value_MEASURE(&data.s32MeasureValue, 3,
//                -99999, 99999);
//        Write_MEASURE_Unit(data.u8MSigType);  
//    }  
  
}

//Summary： Range值换算成Source值
static  void    MValuetoMRange(void)
{ 
  s32  s32Tmp;
  
  s32Tmp = data.s32MSH - data.s32MSL;
  if(s32Tmp == 0)
    s32Tmp = 1;
   data.s32MRangeValue = (s32)((s32)(data.s32MRH - data.s32MRL) * (s32)(data.s32MeasureValue - data.s32MSL)) / s32Tmp + data.s32MRL; 
}

//Summary： MEASURE RANGE实时值
void    DrawMRangeValueRealTime(void)
{
    if(data.u8MeasureEnable == ON)
    {
        MValuetoMRange();
        Write_Value_MEASURE(&data.s32MRangeValue, data.u8MRLHDot, data.s32MRL, data.s32MRH);
        Write_MEASURE_Unit(SIGTYPE_END);  
    }
}

//Summary： MEASURE ON
void    DrawMeasureEnable(void)
{
    Write_MEASURE(ON);
    if(data.u8MeasureEnable == OFF)
    {
        Write_Value_MEASURE(&data.s32MeasureValue, 5, 0, 10000);    //4位小数时，都不显示        
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

//Summary： 清理按键参数
void    ClearKeyParam(void)
{
      edit.u8WriteStutus = 0;
      memset(editNuber, 0xFF, sizeof(editNuber));
      edit.u8KeyPlaces = 0;
      edit.u8DotPlace = 0;
      edit.u8KeyPN = 0;  
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
  Write_AUTOPOWER(data.u8AutoPowerEnable);
}

//Summary： 绘制动态刷新
static  void    DrawDynamic(void)
{
  if(data.u8MRangeEnable == 1)
    DrawMRangeValueRealTime();
  else
    DrawMeasureValueRealTime();
  
     if(data.u8SourceEnable == ON && data.u8SSigType > SIGTYPE_PT100 && data.u8SSigType < SIGTYPE_Hz)    
        DoSource(data.u8SSigType, data.s32SourceValue);    

}

static  void    OnCreate(void)
{
    l_u8Init = 1;
    //输出
           
       if(data.u8SourceEnable == ON )
      {
         DoSource(data.u8SSigType, data.s32SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s32SourceValue);
      }    
      ClearKeyParam();      
}

static  void    OnExit(void)
{
    l_u8Init = 0;
      ClearKeyParam();       
}

static  void    DoNUM0Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 0)
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          edit.u8KeyPN = 0;
          Write_SOURCE_MINUS(edit.u8KeyPN); 
          editNuber[edit.u8KeyPlaces] = 0;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }
    }
    else
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 0;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }      
    }
  }
}

static  void    DoNUM1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 0)
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          edit.u8KeyPN = 0;
          Write_SOURCE_MINUS(edit.u8KeyPN); 
          editNuber[edit.u8KeyPlaces] = 1;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }
    }
    else
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 1;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }      
    }
  }
  
  
}

static  void    DoNUM2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 0)
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          edit.u8KeyPN = 0;
          Write_SOURCE_MINUS(edit.u8KeyPN); 
          editNuber[edit.u8KeyPlaces] = 2;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }
    }
    else
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 2;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }      
    }
  
    
  }
}

static  void    DoNUM3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 0)
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          edit.u8KeyPN = 0;
          Write_SOURCE_MINUS(edit.u8KeyPN); 
          editNuber[edit.u8KeyPlaces] = 3;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }
    }
    else
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 3;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }      
    }
  }
}

static  void    DoNUM4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 0)
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          edit.u8KeyPN = 0;
          Write_SOURCE_MINUS(edit.u8KeyPN); 
          editNuber[edit.u8KeyPlaces] = 4;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }
    }
    else
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 4;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }      
    }
       
  }
}

static  void    DoNUM5Key(PKeyMsg pKeyMsg)
{
   
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 0)
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          edit.u8KeyPN = 0;
          Write_SOURCE_MINUS(edit.u8KeyPN); 
          editNuber[edit.u8KeyPlaces] = 5;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }
    }
    else
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 5;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }      
    } 
  }
}

static  void    DoNUM6Key(PKeyMsg pKeyMsg)
{
   
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 0)
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          edit.u8KeyPN = 0;
          Write_SOURCE_MINUS(edit.u8KeyPN); 
          editNuber[edit.u8KeyPlaces] = 6;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }
    }
    else
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 6;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }      
    }   
  }
}


static  void    DoNUM7Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    
    if(edit.u8WriteStutus == 0)
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          edit.u8KeyPN = 0;
          Write_SOURCE_MINUS(edit.u8KeyPN); 
          editNuber[edit.u8KeyPlaces] = 7;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }
    }
    else
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 7;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }      
    }
  }
}

static  void    DoNUM8Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 0)
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          edit.u8KeyPN = 0;
          Write_SOURCE_MINUS(edit.u8KeyPN); 
          editNuber[edit.u8KeyPlaces] = 8;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }
    }
    else
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 8;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }      
    }
  }
}

static  void    DoNUM9Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 0)
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          edit.u8KeyPN = 0;
          Write_SOURCE_MINUS(edit.u8KeyPN); 
          editNuber[edit.u8KeyPlaces] = 9;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }
    }
    else
    {
      if(edit.u8KeyPlaces <= 4)
      {
        if(editNuber[0] == 0 && edit.u8DotPlace != 1)
            return;
        else
        {           
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 9;      
          edit.u8KeyPlaces++;
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
        }
      }      
    }
  }
}

static  void    DoOKKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {

      if(edit.u8WriteStutus == 1)
      {
         data.fSourceValue = Dodata_s32(editNuber,edit.u8KeyPlaces,edit.u8KeyPN,edit.u8DotPlace);
         data.u8DotNum = edit.u8KeyPlaces - edit.u8DotPlace;
         if(data.u8DotNum == edit.u8KeyPlaces)
           data.u8DotNum = 0;
         data.s32SourceValue =   Write_Value_fSOURCE(&data.fSourceValue,data.u8DotNum, g_s32SSigRange[data.u8SSigType][2], 
                g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);           
      }  
      
      ClearKeyParam();
      DrawSourceData();     
            //输出
      if(data.u8SourceEnable == ON)
      {         
        DoSource(data.u8SSigType, data.s32SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s32SourceValue);
      }
        
    
  }
}
static  void    DoDELKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if( edit.u8WriteStutus == 1 )
      {
        u8 objFlag = 0;
        if(edit.u8DotPlace >= edit.u8KeyPlaces )
        {
          edit.u8DotPlace = 0;
        }
        else
        {
          edit.u8KeyPlaces  = edit.u8KeyPlaces -1;
          if(edit.u8KeyPlaces == 0)
          {
            editNuber[edit.u8KeyPlaces] = 0;   
            edit.u8KeyPlaces = 1;
             DrawSourceData();
             objFlag = 1;
             ClearKeyParam();  
          }
        }
        if(objFlag == 0)
           DrawSourceData();
      }              
  
  }
}

static  void    DoPNKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
     
     if(edit.u8WriteStutus == 0)
     {
      
          if(data.s32MRL == 0 && data.s32MRH == 0)
          {
          }
          else
          {
              if(data.u8MSigType <= SIGTYPE_V) //线性信号可切换
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
     else
     {
       edit.u8KeyPN = 1 - edit.u8KeyPN;
       Write_SOURCE_MINUS(edit.u8KeyPN);
     }
  }
}

static  void    DoDotKey(PKeyMsg pKeyMsg)
{
 
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
   
      if(edit.u8WriteStutus == 0)
      {     
          if(data.s32SRL == 0 && data.s32SRH == 0)
          {
          }
          else
          {
            OnExit();
            SetView(VIEW_SRANGE);
          }
      }
      else
      {
        if(edit.u8DotPlace == 0)
          edit.u8DotPlace = edit.u8KeyPlaces;    
        DrawSourceData();
      }
    
  }
}

static  void    DoSOURCEKey(PKeyMsg pKeyMsg)
{ 
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
  
      data.u8SourceEnable =!data.u8SourceEnable;
      DrawSourceEnable();
    
      //输出
      if(data.u8SourceEnable == ON)
      {        
        DoSource(data.u8SSigType, data.s32SourceValue);
        if(data.u8SSigType == SIGTYPE_Hz)
           TIM1_Cmd(ENABLE);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s32SourceValue);
        if(data.u8SSigType == SIGTYPE_Hz)
           TIM1_Cmd(DISABLE);
      }
    
  }
}

static  void    DoSFUNCKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
  
      data.u8SSigType ++;
      if(data.u8SSigType > SIGTYPE_END - 1)
        data.u8SSigType = SIGTYPE_V;
      ClearKeyParam();  
      data.s32SourceValue = 0;
      DrawSourceData();    
      //信号类型切换过，自动变成OFF
      data.u8SourceEnable = OFF;
      DrawSourceEnable();
      DoSource(SIGTYPE_END, data.s32SourceValue);
    
  }
}

static  void    DoSRANGEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {

      if(data.u8SSigType <= SIGTYPE_MA && data.u8SweepMode == SWEEP_NONE)   //线性信号，可设置RL,RH,SL,SH
      {
        //信号类型切换过，自动变成OFF
          data.u8SourceEnable = OFF;
          DrawSourceEnable();
          DoSource(SIGTYPE_END, data.s32SourceValue);
          OnExit();
          SetView(VIEW_SRSLH); 
      }  
  }
}





static  void    DoMEASUREKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
       data.u8MeasureEnable =!data.u8MeasureEnable;        
       DrawMeasureEnable();
  }
}

static  void    DoMFUNCKey(PKeyMsg pKeyMsg)
{
   
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(data.u8MeasureEnable == ON && data.u8MHoldEnable == OFF)
    {
      data.u8MRangeEnable = 0;
      data.u8MSigType ++;
      if(data.u8MSigType == SIGTYPE_V)
      {
        data.u8VLoopFflag = 0;
        TIM5_Cmd(DISABLE); 
      }
      else if(data.u8MSigType == SIGTYPE_Hz)
         TIM5_Cmd(ENABLE); 
      else
         TIM5_Cmd(DISABLE); 
      if(data.u8MSigType > SIGTYPE_END - 1)
        data.u8MSigType = SIGTYPE_24VLOOP;
      SetMSigPort(data.u8MSigType);
      DrawMeasureData();
    }
  }
}

static  void    DoMRANGEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {

    if(data.u8MSigType <= SIGTYPE_MA && data.u8MeasureEnable == ON && data.u8MHoldEnable == OFF)   //线性信号，可设置RL,RH,SL,SH
    {
      OnExit();
      SetView(VIEW_MRSLH);
    }
  }
}

static  void    DoTEMPKey(PKeyMsg pKeyMsg)
{
//  if(data.u8MSigType != SIGTYPE_24VLOOP)
//  {
    if((data.u8MSigType > SIGTYPE_PT100 &&  data.u8MSigType <  SIGTYPE_Hz) || (data.u8SSigType > SIGTYPE_PT100 && data.u8SSigType < SIGTYPE_Hz))  //热电偶
    {
      if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
      {
        OnExit();
        SetView(VIEW_RJTC);
      }
    }
    else
    {
     ;
    }

}

static  void    DoSWEEPKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if( data.u8SSigType != SIGTYPE_Hz)
    {
      data.u8SweepMode ++;
      if(data.u8SweepMode > SWEEP_STEP)
        data.u8SweepMode = SWEEP_NONE;
      DrawSweepMode();
    }
  }
}

static  void    DoBLIGHTKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
     data.u8LCDBlight = 1 - data.u8LCDBlight;
     SetBright(data.u8LCDBlight);  
  }
}

static  void    DoHOLDKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {

       data.u8MHoldEnable = !data.u8MHoldEnable;
       DrawHoldEnable();
       
    
  }
}

static  void    DoSHANGKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN || pKeyMsg->u8KeyAct == KEY_ACT_LONG)
  {

    if(edit.u8WriteStutus == 1)
    {
      data.s32SourceValue = 0;
      ClearKeyParam();  
      DrawSourceData();
    }
    else
    {
      data.s32SourceValue = Write_UP_SOURCE(&data.s32SourceValue,data.u8DotNum,g_s32SSigRange[data.u8SSigType][2], 
                g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);
      DrawSourceData();
      if(data.u8SourceEnable == ON)
      {         
        DoSource(data.u8SSigType, data.s32SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s32SourceValue);
      }
    }
     
  }
}

static  void    DoXIAKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN || pKeyMsg->u8KeyAct == KEY_ACT_LONG )
  {

    if(edit.u8WriteStutus == 1)
    {
      ClearKeyParam();  
      data.s32SourceValue = 0;
      DrawSourceData();      
   }
   else
   {
      data.s32SourceValue = Write_DOWN_SOURCE(&data.s32SourceValue,data.u8DotNum,g_s32SSigRange[data.u8SSigType][2], 
                g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);
      DrawSourceData();
            if(data.u8SourceEnable == ON)
      {         
        DoSource(data.u8SSigType, data.s32SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s32SourceValue);
      }
   }        
   
  }
}


static  void  DoMODEKey(PKeyMsg pKeyMsg)
{
      if(pKeyMsg->u8KeyAct == KEY_ACT_LONG)
      {
        if(data.u8KeyLongUpF == 0)
        {
          data.u8AutoPowerEnable = 1 - data.u8AutoPowerEnable;
          eep_write8(200,data.u8AutoPowerEnable);
          Write_AUTOPOWER(data.u8AutoPowerEnable);
          data.u8KeyLongUpF = 1;
        }
      }
  
}




static  void    DoPROGKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if( data.u8SSigType != SIGTYPE_Hz)
    {
      
      if(data.u8SweepMode == SWEEP_NONE)
      {
        //信号类型切换过，自动变成OFF
          data.u8SourceEnable = OFF;
          DrawSourceEnable();
          DoSource(SIGTYPE_END, data.s32SourceValue);
          data.u8ProgMode++;
          OnExit();
          SetView(VIEW_PROGNM);
      }
      else if(data.u8SweepMode == SWEEP_SWEEP)
      {
        //信号类型切换过，自动变成OFF
          data.u8SourceEnable = OFF;
          DrawSourceEnable();
          DoSource(SIGTYPE_END, data.s32SourceValue);
          OnExit();
          SetView(VIEW_SWEEP);
      }
      else if(data.u8SweepMode == SWEEP_STEP)
      {
        //信号类型切换过，自动变成OFF
          data.u8SourceEnable = OFF;
          DrawSourceEnable();
          DoSource(SIGTYPE_END, data.s32SourceValue);
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
      case KEY_NUM0:    
        DoNUM0Key(pKeyMsg);
        break;
      case KEY_NUM1:    
        DoNUM1Key(pKeyMsg);
        break;
      case KEY_NUM2:    
        DoNUM2Key(pKeyMsg);
        break;
      case KEY_NUM3:    
        DoNUM3Key(pKeyMsg);
        break;
      case KEY_NUM4:    
        DoNUM4Key(pKeyMsg);
        break;
      case KEY_NUM5:    
        DoNUM5Key(pKeyMsg);
        break;
      case KEY_NUM6:    
        DoNUM6Key(pKeyMsg);
        break;
      case KEY_NUM7:    
        DoNUM7Key(pKeyMsg);
        break;
      case KEY_NUM8:    
        DoNUM8Key(pKeyMsg);
        break;
       case KEY_NUM9:    
        DoNUM9Key(pKeyMsg);
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
      case KEY_OK:
        DoOKKey(pKeyMsg);
        break;  
      case KEY_DEL:
        DoDELKey(pKeyMsg);
        break;   
      case KEY_BLIGHT:
        DoBLIGHTKey(pKeyMsg);
        break;           
      case KEY_HOLD:
        DoHOLDKey(pKeyMsg);
        break;   
       case KEY_SHANG:
        DoSHANGKey(pKeyMsg);
        break; 
      case KEY_XIA:
        DoXIAKey(pKeyMsg);
        break;   
      case KEY_MODE:
        DoMODEKey(pKeyMsg);
        break;         
      default:
        break;     
    }
}

void  ViewMain(PKeyMsg pKeyMsg)
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

