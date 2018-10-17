/*=============================================================================
//File:			viewSweep.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			sweep画面
===============================================================================*/
#include "viewSweep.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewMain.h"
#include "viewFrame.h"
#include <string.h>
#include "DriveEEprom.h"
#include "lcd.h"

#define     SELF_VIEWID     VIEW_SWEEP

static  u8      l_u8Init = 0;               //初始化状态
static  u16     l_u16Timetn[4] = {10, 10, 10, 10};
static  u16     l_u16Count = 0;

static  u8      l_u8DrawStep = TIME_T1;
static  u8      l_u8Sweepstep = TIME_T1;  //SWEEP步骤
static  u16     l_u16Cnt = 1;       //计时
static  u8      l_u8Blink = 0;      //闪烁
static  u8      l_u8CntStatus = 0;      //count为0，无限循环
static  s32     l_s32SSweepValue = 0;       //SOURCE SWEEP值
static  float   l_fSweepData = 0; //sweep每次变化值
extern u8 editNuber[6];

/*******************************************************/
//
//Summary:	 显示n
//Parameter:     void
//Return:        void
//
/*******************************************************/
static  void    Draw_Timetn(void)
{  
    Write_Value_TIME_COUNT_n(&l_u16Timetn[l_u8DrawStep], 0, 999);  
}

/*******************************************************/
//
//Summary:	 显示count
//Parameter:     void
//Return:        void
//
/*******************************************************/
static  void    Draw_Count(void)
{  
    Write_Value_TIME_COUNT_n(&l_u16Count, 0, 999);  
}

//Summary： 显示Sweep
static  void    DrawSweep(void)
{  
    Write_TIME(ON);
    Write_Time_t(TIME_SWEEP);    //全亮
    Write_COUNT(ON);
    Draw_Count();  
}


/*******************************************************/
//
//Summary:	 显示tn闪烁
//Parameter:     void
//Return:        void
//
/*******************************************************/
static  void    DrawTnBlink(void)
{  
  l_u8Blink = !l_u8Blink;
  if(l_u8Blink == 0)
  { 
    Write_Time_t(TIME_SWEEP);        //全显示
  }
  else
  {
    Write_Time_t(l_u8Sweepstep + TIME_SWEEPSTEP1);    //SWEEP STEP
  }
}


/*******************************************************/
//
//Summary:	 SOURCE SWEEP值
//Parameter:     void
//Return:        void
//
/*******************************************************/
static  void    DrawSSweepValue(void)
{
    Write_Value_SOURCE(&l_s32SSweepValue, g_s32SSigRange[data.u8SSigType][2], 
            g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);
}

/*******************************************************/
//
//Summary:	 绘制静态刷新
//Parameter:     void
//Return:        void
//
/*******************************************************/
static  void    DrawStatic(void)
{
    if(l_u8DrawStep < TIME_TEND)        //显示Tn
    {
      Write_COUNT(OFF);
      Write_Value_TIME_COUNT_n(&l_u16Count, 1000, 1000);//不显示
   
      Write_TIME(ON);
      Write_s(ON);
      Write_Time_t(l_u8DrawStep);
      Draw_Timetn();
    }
    else            //显示count
    {
      Write_TIME(OFF);
      Write_s(OFF);
      Write_Time_t(TIME_TEND);
      Write_Value_TIME_COUNT_n(&l_u16Timetn[l_u8DrawStep], 1000, 1000);//不显示
   
      Write_COUNT(ON);
      Draw_Count();
      if(l_u16Count > 0)
        l_u8CntStatus = 1;  //有限循环
      else
        l_u8CntStatus = 0;
    }
}


/*******************************************************/
//
//Summary:	 绘制动态刷新
//Parameter:     void
//Return:        void
//
/*******************************************************/
static  void    DrawDynamic(void)
{
  

  //测量
  if(data.u8MRangeEnable == 1)
    DrawMRangeValueRealTime();
  else
    DrawMeasureValueRealTime();
  //输出
  if(data.u8SourceEnable == ON)    //开始输出
  {
    l_u16Cnt ++;        //每100ms++   
    if(l_u8CntStatus == 0 || (l_u8CntStatus == 1 && l_u16Count > 0)) //无限循环或者有限循环CNT大于0
    {
      if(!(l_u16Cnt % 10))
        DrawTnBlink();      //每秒闪烁
      
      
      
      if(l_u8Sweepstep == TIME_T1)  //T1输出
      {
        l_fSweepData += (float)(data.s32SourceValue - g_s32SSigRange[data.u8SSigType][0]) / (l_u16Timetn[l_u8Sweepstep]) / 10.0; //每100ms++ 
        l_s32SSweepValue = (s32)l_fSweepData;
        if(l_s32SSweepValue > data.s32SourceValue)
          l_s32SSweepValue = data.s32SourceValue;
        
        DrawSSweepValue();
        //输出
        DoSource(data.u8SSigType, l_s32SSweepValue);
      }
      else if(l_u8Sweepstep == TIME_T2)  //T2输出
      {
        l_s32SSweepValue = data.s32SourceValue;
        l_fSweepData = l_s32SSweepValue;
        DrawSSweepValue();
        
        //输出
        DoSource(data.u8SSigType, l_s32SSweepValue);
      }
      else if(l_u8Sweepstep == TIME_T3)  //T3输出
      {        
        l_fSweepData -= (float)(data.s32SourceValue - g_s32SSigRange[data.u8SSigType][0]) / (l_u16Timetn[l_u8Sweepstep]) / 10;
        l_s32SSweepValue = (s32)l_fSweepData;
        DrawSSweepValue();      //此处会限幅
        //输出
        DoSource(data.u8SSigType, l_s32SSweepValue);
      }
      else if(l_u8Sweepstep == TIME_T4)  //T4输出
      {
        l_s32SSweepValue  = g_s32SSigRange[data.u8SSigType][0];
        l_fSweepData = l_s32SSweepValue;
        DrawSSweepValue();
        //输出
        DoSource(data.u8SSigType, l_s32SSweepValue);
      }
      else    //一次循环结束
      {
        l_u16Cnt = 0;
        l_u8Sweepstep = TIME_T1;
        if(l_u16Count > 0)
          l_u16Count --;
        Draw_Count();
      }
      
      if(l_u16Cnt >= l_u16Timetn[l_u8Sweepstep] * 10)   //l_u8Sweepstep ++
      { 
        l_u16Cnt = 0;
        l_u8Sweepstep ++;           
      }
 
    }
    else        //结束
    {
      data.u8SourceEnable = OFF;
      l_u16Cnt = 1;
      DrawSourceEnable();
      DrawStatic();
      //,关闭,输出
      DoSource(SIGTYPE_END, l_s32SSweepValue);
    }
  }
}

static  void    OnCreate(void)
{
    l_u8DrawStep = TIME_T1;
    data.u8SweepStatus = 0;
    l_u8Sweepstep = TIME_T1;
    l_s32SSweepValue = g_s32SSigRange[data.u8SSigType][0];
    l_fSweepData = l_s32SSweepValue;
    l_u16Cnt = 1;
    l_u8Init = 1;
}

static  void    OnExit(void)
{
   Write_TIME(OFF);
   Write_s(OFF);
   Write_Time_t(TIME_TEND);
   Write_Value_TIME_COUNT_n(&l_u16Timetn[l_u8DrawStep], 1000, 1000);//不显示
   
   Write_COUNT(OFF);
   Write_Value_TIME_COUNT_n(&l_u16Count, 1000, 1000);//不显示
   
   data.u8SweepMode = SWEEP_NONE;
   
    data.u8SweepStatus = 0;   //退出SWEEP模式
   l_u8Init = 0;
}

static void GetTimetn(void)
{
      if(edit.u8KeyPlaces == 0)
        l_u16Timetn[l_u8DrawStep] = 0;
      else if(edit.u8KeyPlaces == 1)
      {
        l_u16Timetn[l_u8DrawStep] = editNuber[0];
      }
      else if(edit.u8KeyPlaces == 2)
      {
        l_u16Timetn[l_u8DrawStep] = editNuber[0] * 10 + editNuber[1];
      }
      else if(edit.u8KeyPlaces == 3)
      {
        l_u16Timetn[l_u8DrawStep] = editNuber[0] * 100 + editNuber[1] * 10  + editNuber[2];
      } 
}

static void GetCount(void)
{
       if(edit.u8KeyPlaces == 1)
      {
        l_u16Count = editNuber[0];
      }
      else if(edit.u8KeyPlaces == 2)
      {
        l_u16Count = editNuber[0] * 10 + editNuber[1];
      }
      else if(edit.u8KeyPlaces == 3)
      {
        l_u16Count = editNuber[0] * 100 + editNuber[1] * 10  + editNuber[2];
      } 
}


static  void    DoNUM0Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if( edit.u8KeyPlaces <= 2)
      {
        if(l_u8DrawStep < TIME_TEND)
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 0;       
              edit.u8KeyPlaces++;
              GetTimetn();     
              Draw_Timetn();
            }
        }
        else
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 0;       
              edit.u8KeyPlaces++;
              GetCount();     
              Draw_Count();
              if(l_u16Count > 0)
                l_u8CntStatus = 1;  //有限循环
              else
                l_u8CntStatus = 0;
            }          
        }
      
     }
  }
}

static  void    DoNUM1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if( edit.u8KeyPlaces <= 2)
      {
        if(l_u8DrawStep < TIME_TEND)
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 1;       
              edit.u8KeyPlaces++;
              GetTimetn();     
              Draw_Timetn();
            }
        }
        else
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 1;       
              edit.u8KeyPlaces++;
              GetCount();     
              Draw_Count();
              if(l_u16Count > 0)
                l_u8CntStatus = 1;  //有限循环
              else
                l_u8CntStatus = 0;
            }          
        }
      
     }
  }
}

static  void    DoNUM2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if( edit.u8KeyPlaces <= 2)
      {
        if(l_u8DrawStep < TIME_TEND)
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 2;       
              edit.u8KeyPlaces++;
              GetTimetn();     
              Draw_Timetn();
            }
        }
        else
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 2;       
              edit.u8KeyPlaces++;
              GetCount();     
              Draw_Count();
              if(l_u16Count > 0)
                l_u8CntStatus = 1;  //有限循环
              else
                l_u8CntStatus = 0;
            }          
        }
      
     }
  }
}

static  void    DoNUM3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if( edit.u8KeyPlaces <= 2)
      {
        if(l_u8DrawStep < TIME_TEND)
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 3;       
              edit.u8KeyPlaces++;
              GetTimetn();     
              Draw_Timetn();
            }
        }
        else
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 3;       
              edit.u8KeyPlaces++;
              GetCount();     
              Draw_Count();
              if(l_u16Count > 0)
                l_u8CntStatus = 1;  //有限循环
              else
                l_u8CntStatus = 0;
            }          
        }
      
     }
  }
}

static  void    DoNUM4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if( edit.u8KeyPlaces <= 2)
      {
        if(l_u8DrawStep < TIME_TEND)
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 4;       
              edit.u8KeyPlaces++;
              GetTimetn();     
              Draw_Timetn();
            }
        }
        else
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 4;       
              edit.u8KeyPlaces++;
              GetCount();     
              Draw_Count();
              if(l_u16Count > 0)
                l_u8CntStatus = 1;  //有限循环
              else
                l_u8CntStatus = 0;
            }          
        }
      
     }
  }
}

static  void    DoNUM5Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if( edit.u8KeyPlaces <= 2)
      {
        if(l_u8DrawStep < TIME_TEND)
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 5;       
              edit.u8KeyPlaces++;
              GetTimetn();     
              Draw_Timetn();
            }
        }
        else
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 5;       
              edit.u8KeyPlaces++;
              GetCount();     
              Draw_Count();
              if(l_u16Count > 0)
                l_u8CntStatus = 1;  //有限循环
              else
                l_u8CntStatus = 0;
            }          
        }
      
     }  }
}

static  void    DoNUM6Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if( edit.u8KeyPlaces <= 2)
      {
        if(l_u8DrawStep < TIME_TEND)
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 6;       
              edit.u8KeyPlaces++;
              GetTimetn();     
              Draw_Timetn();
            }
        }
        else
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 6;       
              edit.u8KeyPlaces++;
              GetCount();     
              Draw_Count();
              if(l_u16Count > 0)
                l_u8CntStatus = 1;  //有限循环
              else
                l_u8CntStatus = 0;
            }          
        }
      
     }
  }
}


static  void    DoNUM7Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if( edit.u8KeyPlaces <= 2)
      {
        if(l_u8DrawStep < TIME_TEND)
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 7;       
              edit.u8KeyPlaces++;
              GetTimetn();     
              Draw_Timetn();
            }
        }
        else
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 7;       
              edit.u8KeyPlaces++;
              GetCount();     
              Draw_Count();
              if(l_u16Count > 0)
                l_u8CntStatus = 1;  //有限循环
              else
                l_u8CntStatus = 0;
            }          
        }
      
     }
  }
}

static  void    DoNUM8Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if( edit.u8KeyPlaces <= 2)
      {
        if(l_u8DrawStep < TIME_TEND)
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 8;       
              edit.u8KeyPlaces++;
              GetTimetn();     
              Draw_Timetn();
            }
        }
        else
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 8;       
              edit.u8KeyPlaces++;
              GetCount();     
              Draw_Count();
              if(l_u16Count > 0)
                l_u8CntStatus = 1;  //有限循环
              else
                l_u8CntStatus = 0;
            }          
        }
      
     }
  }
}

static  void    DoNUM9Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if( edit.u8KeyPlaces <= 2)
      {
        if(l_u8DrawStep < TIME_TEND)
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 9;       
              edit.u8KeyPlaces++;
              GetTimetn();     
              Draw_Timetn();
            }
        }
        else
        {
            if(editNuber[0] == 0)
                return;
            else
            {  
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 9;       
              edit.u8KeyPlaces++;
              GetCount();     
              Draw_Count();
              if(l_u16Count > 0)
                l_u8CntStatus = 1;  //有限循环
              else
                l_u8CntStatus = 0;
            }          
        }
      
     }
  }
}

static  void    DoOKKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_u8DrawStep < TIME_TEND)
    {
       GetTimetn();     
       Draw_Timetn();
    }
    else
    {
       GetCount();     
       Draw_Count();
       if(l_u16Count > 0)
          l_u8CntStatus = 1;  //有限循环
       else
          l_u8CntStatus = 0;
    }          
      edit.u8WriteStutus = 0;
      memset(editNuber, 0xFF, sizeof(editNuber));
      edit.u8KeyPlaces = 0;
      edit.u8DotPlace = 0;
   }
}
static  void    DoDELKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if( edit.u8WriteStutus == 1 && edit.u8KeyPlaces != 0)
      {
          edit.u8KeyPlaces  = edit.u8KeyPlaces -1;
//          if(edit.u8KeyPlaces == 0)
//          {
//            editNuber[edit.u8KeyPlaces] = 0;            
//          }
          
          if(l_u8DrawStep < TIME_TEND)
          {
              GetTimetn();     
              Draw_Timetn();
          }
          else
          {
              GetCount();     
              Draw_Count();
          }                       
     }              
  
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
    if(l_u8DrawStep >= TIME_TEND)
    {
      data.u8SourceEnable =!data.u8SourceEnable;
      DrawSourceEnable();
      if(l_u16Count > 0)
        l_u8CntStatus = 1;  //有限循环
      else
        l_u8CntStatus = 0;
      
      //输出
      if(data.u8SourceEnable == ON)
      {
        data.u8SweepStatus = 1;
        l_u16Cnt = 1;
        l_u8Sweepstep = TIME_T1;
        DrawSweep();
      }
      else
      {
        data.u8SweepStatus = 0;
        l_u16Cnt = 1;
        l_u8Sweepstep = TIME_T1;
        DrawStatic();
        //,关闭,输出
        DoSource(SIGTYPE_END, l_s32SSweepValue);
      }
    }
    else
    {
      data.u8SourceEnable = OFF;
      l_u16Cnt = 1;
      data.u8SweepStatus = 0;
      l_u8Sweepstep = TIME_T1;
      DrawStatic();
      DrawSourceEnable();
      //,关闭,输出
      DoSource(SIGTYPE_END, l_s32SSweepValue);
    }
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
    data.u8MeasureEnable =!data.u8MeasureEnable;        
    DrawMeasureEnable();
  }
}

static  void    DoMFUNCKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(data.u8MeasureEnable == ON)
    {
      data.u8MSigType ++;
      if(data.u8MSigType == SIGTYPE_V)
      {
        data.u8VLoopFflag = 0;
      }
      if(data.u8MSigType > SIGTYPE_END - 1)
        data.u8MSigType = SIGTYPE_24VLOOP;
      DrawMeasureData();
    }
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
    if(l_u8DrawStep < TIME_TEND)
    {
      l_u8DrawStep ++;
      DrawStatic();
    }
    else
    {
      OnExit();
      SetView(VIEW_MAIN);
    }
      edit.u8WriteStutus = 0;
      memset(editNuber, 0xFF, sizeof(editNuber));
      edit.u8KeyPlaces = 0;
      edit.u8DotPlace = 0;
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
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 0)
    {
      if(l_u8DrawStep < TIME_TEND)
      {   
        l_u16Timetn[l_u8DrawStep] += 1;
        if(l_u16Timetn[l_u8DrawStep] > 999)
          l_u16Count = 0;
        Draw_Timetn();
      }
      else
      {
        l_u16Count += 1; 
        if(l_u16Count > 999)
          l_u16Count = 0;
        Draw_Count();
        if(l_u16Count > 0)
            l_u8CntStatus = 1;  //有限循环
        else
            l_u8CntStatus = 0;
      }          
      edit.u8WriteStutus = 0;
      memset(editNuber, 0xFF, sizeof(editNuber));
      edit.u8KeyPlaces = 0;
      edit.u8DotPlace = 0;
   }
  }
}

static  void    DoXIAKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 0)
    {
      if(l_u8DrawStep < TIME_TEND)
      {   
        l_u16Timetn[l_u8DrawStep] -= 1;       
        Draw_Timetn();
      }
      else
      {
        l_u16Count -= 1;     
        Draw_Count();
        if(l_u16Count > 0)
            l_u8CntStatus = 1;  //有限循环
        else
            l_u8CntStatus = 0;
      }          
      edit.u8WriteStutus = 0;
      memset(editNuber, 0xFF, sizeof(editNuber));
      edit.u8KeyPlaces = 0;
      edit.u8DotPlace = 0;
   }
  }
}


static  void  DoMODEKey(PKeyMsg pKeyMsg)
{

      if(pKeyMsg->u8KeyAct == KEY_ACT_LONG)
      {
          data.u8AutoPowerEnable = 1 - data.u8AutoPowerEnable;
          eep_write8(200,data.u8AutoPowerEnable);
          Write_AUTOPOWER(data.u8AutoPowerEnable);
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

void    ViewSweep(PKeyMsg pKeyMsg)
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
