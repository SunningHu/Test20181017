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
#include <string.h>
#include "DriveEEprom.h"
#include "lcd.h"

#define     MAX_M       20
#define     MIN_M       1

#define     SELF_VIEWID     VIEW_STEP

static  u8      l_u8Init = 0;               //初始化状态
static  u8      l_u8DrawStep = 0;           //TIME  0    N/M 1
static  u8      l_u8SelectNM = 0;           //m 0    n 1
static  u8      l_u8Blink = 0;              //闪烁变量
static  u8      l_u8BlinkFlag = 0;          //是否闪烁标志
static  u16     l_u16Time = 10;             //时间值

static  u16     l_u16n = 1;
static  u16     l_u16m = 1;

static  u16     l_u16Stepn = 1;

static  s32     l_s32SValue = 0;        //Step 输出值
static  u16     l_u16Cnt = 0;       //计时
static  u8      l_u8Status = 0;
extern u8  editNuber[6];     


static void ClearNuber(void)
{
      ClearKeyParam();     
}


//Summary： 显示t1
static  void    Draw_Time(void)
{  
    Write_Value_TIME_COUNT_n(&l_u16Time, 1, 999);  
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
   if(l_u8DrawStep == 1 && l_u8BlinkFlag == 0)
   {
      l_u8Blink = 1 - l_u8Blink;
      if(l_u8SelectNM == 0)
      {
         Write_m(l_u8Blink);
      }
      else
      {
         Write_n(l_u8Blink);         
      }
   }
  
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
      l_s32SValue = (s32)((float)data.s32SourceValue * (float)l_u16Stepn / (float)l_u16m);
      //输出
      if(data.u8SourceEnable == ON)
      {
        DoSource(data.u8SSigType, l_s32SValue);
      }
      else
      {
        DoSource(SIGTYPE_END, l_s32SValue);
      }
    }
  }
}

static  void    OnCreate(void)
{
    l_u8DrawStep = 0;
    l_u16Stepn = l_u16n;
    l_u8Init = 1;
    ClearNuber();     
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
   l_u8SelectNM = 0;        
   l_u8Blink = 0;              
   l_u8BlinkFlag = 0;          
}

static void GetTimes(void)
{
         if(edit.u8KeyPlaces == 0)
        {
          l_u16Time = 1;
        }
       else  if(edit.u8KeyPlaces == 1)
        {
          l_u16Time = editNuber[0];
        }
        else if(edit.u8KeyPlaces == 2)
        {
          l_u16Time = editNuber[0] * 10 + editNuber[1];
        }
        else if(edit.u8KeyPlaces == 3)
        {
          l_u16Time = editNuber[0] * 100 + editNuber[1] * 10 + editNuber[2];
        }    
}

static void GetU16m(void)
{
          if(edit.u8KeyPlaces ==  0)
        {
          l_u16m = 1;
        }
       else if(edit.u8KeyPlaces == 1)
        {
          l_u16m = editNuber[0];
        }
        else if(edit.u8KeyPlaces == 2)
        {
          l_u16m = editNuber[0] * 10 + editNuber[1];
        }  
}

static void GetU16n(void)
{
          if(edit.u8KeyPlaces == 0)
        {
          l_u16m = 1;
        }
       else if(edit.u8KeyPlaces == 1)
        {
          l_u16n = editNuber[0];
        }
        else if(edit.u8KeyPlaces == 2)
        {
          l_u16n = editNuber[0] * 10 + editNuber[1];
        }  
}


static  void    DoNUM0Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {    
    if(l_u8DrawStep == 0)
    {
      if(edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 0;       
          edit.u8KeyPlaces++;
        }
        GetTimes();              
        Draw_Time();
      }
    }
    else 
    {
      if(l_u8SelectNM == 0)
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
              return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 0;       
              edit.u8KeyPlaces++;
            }
            GetU16m();              
            if(l_u16m > MAX_M)
              l_u16m = MAX_M;
            if(l_u16m < MIN_M)
              l_u16m = MIN_M;
            Draw_m();   
        }
      }
      else
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
                return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 0;       
              edit.u8KeyPlaces++;
            }
            GetU16n();              
            if(l_u16n > l_u16m)
              l_u16n = l_u16m;
            if(l_u16n < MIN_M)
              l_u16n = MIN_M;            
            Draw_n();    
        }
      }
      l_u16Stepn = l_u16n;      
    }
  }
}

static  void    DoNUM1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_u8DrawStep == 0)
    {
      if(edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 1;       
          edit.u8KeyPlaces++;
        }
        GetTimes();              
        Draw_Time();
      }
    }
    else 
    {
      if(l_u8SelectNM == 0)
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
              return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 1;       
              edit.u8KeyPlaces++;
            }
            GetU16m();              
            if(l_u16m > MAX_M)
              l_u16m = MAX_M;
            if(l_u16m < MIN_M)
              l_u16m = MIN_M;
            Draw_m();        
        }
      }
      else
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
                return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 1;       
              edit.u8KeyPlaces++;
            }
            GetU16n();              
            if(l_u16n > l_u16m)
              l_u16n = l_u16m;
            if(l_u16n < MIN_M)
              l_u16n = MIN_M;            
            Draw_n();          
        }
      }
      l_u16Stepn = l_u16n;     
    }
  }          
}

static  void    DoNUM2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_u8DrawStep == 0)
    {
      if(edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 2;       
          edit.u8KeyPlaces++;
        }
        GetTimes();              
        Draw_Time();
      }
    }
    else 
    {
      if(l_u8SelectNM == 0)
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
              return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 2;       
              edit.u8KeyPlaces++;
            }
            GetU16m();              
            if(l_u16m > MAX_M)
              l_u16m = MAX_M;
            if(l_u16m < MIN_M)
              l_u16m = MIN_M;
            Draw_m();        
        }
      }
      else
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
                return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 2;       
              edit.u8KeyPlaces++;
            }
            GetU16n();              
            if(l_u16n > l_u16m)
              l_u16n = l_u16m;
            if(l_u16n < MIN_M)
              l_u16n = MIN_M;            
            Draw_n();          
        }
      }
      l_u16Stepn = l_u16n;     
    }
  }
}

static  void    DoNUM3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_u8DrawStep == 0)
    {
      if(edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 3;       
          edit.u8KeyPlaces++;
        }
        GetTimes();              
        Draw_Time();
      }
    }
    else 
    {
      if(l_u8SelectNM == 0)
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
              return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 3;       
              edit.u8KeyPlaces++;
            }
            GetU16m();              
            if(l_u16m > MAX_M)
              l_u16m = MAX_M;
            if(l_u16m < MIN_M)
              l_u16m = MIN_M;
            Draw_m();        
        }
      }
      else
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
                return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 3;       
              edit.u8KeyPlaces++;
            }
            GetU16n();              
            if(l_u16n > l_u16m)
              l_u16n = l_u16m;
            if(l_u16n < MIN_M)
              l_u16n = MIN_M;            
            Draw_n();          
        }
      }
      l_u16Stepn = l_u16n;     
    }
  }
}

static  void    DoNUM4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_u8DrawStep == 0)
    {
      if(edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 4;       
          edit.u8KeyPlaces++;
        }
        GetTimes();              
        Draw_Time();
      }
    }
    else 
    {
      if(l_u8SelectNM == 0)
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
              return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 4;       
              edit.u8KeyPlaces++;
            }
            GetU16m();              
            if(l_u16m > MAX_M)
              l_u16m = MAX_M;
            if(l_u16m < MIN_M)
              l_u16m = MIN_M;
            Draw_m();        
        }
      }
      else
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
                return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 4;       
              edit.u8KeyPlaces++;
            }
            GetU16n();              
            if(l_u16n > l_u16m)
              l_u16n = l_u16m;
            if(l_u16n < MIN_M)
              l_u16n = MIN_M;            
            Draw_n();          
        }
      }
      l_u16Stepn = l_u16n;      
    }
  }
}

static  void    DoNUM5Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_u8DrawStep == 0)
    {
      if(edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 5;       
          edit.u8KeyPlaces++;
        }
        GetTimes();              
        Draw_Time();
      }
    }
    else 
    {
      if(l_u8SelectNM == 0)
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
              return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 5;       
              edit.u8KeyPlaces++;
            }
            GetU16m();              
            if(l_u16m > MAX_M)
              l_u16m = MAX_M;
            if(l_u16m < MIN_M)
              l_u16m = MIN_M;
            Draw_m();        
        }
      }
      else
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
                return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 5;       
              edit.u8KeyPlaces++;
            }
            GetU16n();              
            if(l_u16n > l_u16m)
              l_u16n = l_u16m;
            if(l_u16n < MIN_M)
              l_u16n = MIN_M;            
            Draw_n();          
        }
      }
      l_u16Stepn = l_u16n;      
    }
  }
}

static  void    DoNUM6Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_u8DrawStep == 0)
    {
      if(edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 6;       
          edit.u8KeyPlaces++;
        }
        GetTimes();              
        Draw_Time();
      }
    }
    else 
    {
      if(l_u8SelectNM == 0)
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
              return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 6;       
              edit.u8KeyPlaces++;
            }
            GetU16m();              
            if(l_u16m > MAX_M)
              l_u16m = MAX_M;
            if(l_u16m < MIN_M)
              l_u16m = MIN_M;
            Draw_m();        
        }
      }
      else
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
                return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 6;       
              edit.u8KeyPlaces++;
            }
            GetU16n();              
            if(l_u16n > l_u16m)
              l_u16n = l_u16m;
            if(l_u16n < MIN_M)
              l_u16n = MIN_M;            
            Draw_n();          
        }
      }
      l_u16Stepn = l_u16n;      
    }
  }
}


static  void    DoNUM7Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_u8DrawStep == 0)
    {
      if(edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 7;       
          edit.u8KeyPlaces++;
        }
        GetTimes();              
        Draw_Time();
      }
    }
    else 
    {
      if(l_u8SelectNM == 0)
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
              return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 7;       
              edit.u8KeyPlaces++;
            }
            GetU16m();              
            if(l_u16m > MAX_M)
              l_u16m = MAX_M;
            if(l_u16m < MIN_M)
              l_u16m = MIN_M;
            Draw_m();        
        }
      }
      else
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
                return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 7;       
              edit.u8KeyPlaces++;
            }
            GetU16n();              
            if(l_u16n > l_u16m)
              l_u16n = l_u16m;
            if(l_u16n < MIN_M)
              l_u16n = MIN_M;            
            Draw_n();          
        }
      }
      l_u16Stepn = l_u16n;      
    }
  }
}

static  void    DoNUM8Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_u8DrawStep == 0)
    {
      if(edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 8;       
          edit.u8KeyPlaces++;
        }
        GetTimes();              
        Draw_Time();
      }
    }
    else 
    {
      if(l_u8SelectNM == 0)
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
              return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 8;       
              edit.u8KeyPlaces++;
            }
            GetU16m();              
            if(l_u16m > MAX_M)
              l_u16m = MAX_M;
            if(l_u16m < MIN_M)
              l_u16m = MIN_M;
            Draw_m();        
        }
      }
      else
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
                return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 8;       
              edit.u8KeyPlaces++;
            }
            GetU16n();              
            if(l_u16n > l_u16m)
              l_u16n = l_u16m;
            if(l_u16n < MIN_M)
              l_u16n = MIN_M;            
            Draw_n();          
        }
      }
      l_u16Stepn = l_u16n;      
    }
  }
}

static  void    DoNUM9Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_u8DrawStep == 0)
    {
      if(edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 9;       
          edit.u8KeyPlaces++;
        }
        GetTimes();              
        Draw_Time();
      }
    }
    else 
    {
      if(l_u8SelectNM == 0)
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
              return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 9;       
              edit.u8KeyPlaces++;
            }
            GetU16m();              
            if(l_u16m > MAX_M)
              l_u16m = MAX_M;
            if(l_u16m < MIN_M)
              l_u16m = MIN_M;
            Draw_m();        
        }
      }
      else
      {
        if(edit.u8KeyPlaces <= 1)
        {
            if(editNuber[0] == 0)
                return;
            else
            { 
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 9;       
              edit.u8KeyPlaces++;
            }
            GetU16n();              
            if(l_u16n > l_u16m)
              l_u16n = l_u16m;
            if(l_u16n < MIN_M)
              l_u16n = MIN_M;            
            Draw_n();          
        }
      }
      l_u16Stepn = l_u16n;      
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
        if(l_u8DrawStep >= 1)
        {
          l_u8BlinkFlag = 1;
          Write_n(ON);
          Write_m(ON);
          data.u8SourceEnable =!data.u8SourceEnable;
          DrawSourceEnable();
          DrawStatic();
      //输出
          if(data.u8SourceEnable == ON)
          {
              DoSource(data.u8SSigType, (s32)((float)data.s32SourceValue * (float)l_u16n / l_u16m));
          }
          else
          {
              DoSource(SIGTYPE_END,  (s32)((float)data.s32SourceValue * (float)l_u16n / l_u16m));
          }
          ClearNuber();
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
    ClearNuber();
  }
}

static  void    DoOKKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(data.u8SourceEnable == OFF)
    {
      if(l_u8DrawStep == 1)
      {         
        l_u8BlinkFlag = 0;  
        l_u8SelectNM = 1 - l_u8SelectNM;
        if(l_u8SelectNM == 0)
        {
            Write_n(OFF);
        }
        else
        {
            Write_m(OFF);         
        }
        ClearNuber();
      }
    }
  }     
}
static  void    DoDELKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 1 && edit.u8KeyPlaces != 0)
    {
      if(l_u8DrawStep == 0 )
      {
          edit.u8KeyPlaces  = edit.u8KeyPlaces - 1;
//          if(edit.u8KeyPlaces == 0)
//            editNuber[edit.u8KeyPlaces] = 0;   
          GetTimes();              
          Draw_Time();;
      }      
      else
      {
        if(l_u8SelectNM == 0)
        {
          edit.u8KeyPlaces  = edit.u8KeyPlaces -1;
//          if(edit.u8KeyPlaces == 0)
//            editNuber[edit.u8KeyPlaces] = 0;   
          GetU16m();              
          if(l_u16m > MAX_M)
             l_u16m = MAX_M;
          if(l_u16m < MIN_M)
             l_u16m = MIN_M;
          Draw_m();    
        }
        else
        {
           edit.u8KeyPlaces  = edit.u8KeyPlaces -1;
//          if(edit.u8KeyPlaces == 0)
//            editNuber[edit.u8KeyPlaces] = 0;   
          GetU16n();              
          if(l_u16n > l_u16m)
              l_u16n = l_u16m;
          if(l_u16n < MIN_M)
              l_u16n = MIN_M;            
          Draw_n();                      
        }
      }
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
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    ;
  }
}

static  void    DoXIAKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    ;
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
