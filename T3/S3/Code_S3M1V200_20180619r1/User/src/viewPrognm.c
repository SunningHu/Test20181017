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
#include <string.h>
#include "DriveEEprom.h"
#include "lcd.h"

#define     MAX_M       20

#define     SELF_VIEWID     VIEW_PROGNM
extern u8 editNuber[6];


static  u8      l_u8Init = 0;               //初始化状态
static  u8      l_NOBlink = 0;
static  u16     l_u16n = 1;
static  u16     l_u16m = 1;
static  u8      l_u8BlinkN = 0;
static  s32     l_s32SSValue = 0;       

//Summary： sourceValue *n/m的值
static  void    SourceValue_nm(void)
{ 
  l_s32SSValue = (s32)((float)data.s32SourceValue * (float)l_u16n / (float)l_u16m);
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
  
  l_u8BlinkN = !l_u8BlinkN;
  if(data.u8ProgMode == PROGNM_M)
  {
    if(l_u8BlinkN == 0 && l_NOBlink == 0)
      Write_m(OFF);
    else
      Write_m(ON);  
  }
              
  if(data.u8ProgMode == PROGNM_N)
  {
    if(l_u8BlinkN == 0  && l_NOBlink == 0)
      Write_n(OFF);
    else
      Write_n(ON);  
  }              
  
  if(data.u8MRangeEnable == 1)
    DrawMRangeValueRealTime();
  else
    DrawMeasureValueRealTime();
}

static  void    OnCreate(void)
{
    l_u16n = 1;   //n值
    l_u16m = 1;   //m值
    l_u8Init = 1;
    l_u8BlinkN = 0;    //闪烁
    l_NOBlink = 0;     //不闪烁
    //输出 
    SourceValue_nm();  //计算输出值
    if(data.u8SourceEnable == ON)
    {
      DoSource(data.u8SSigType, l_s32SSValue);
    }
    else
    {
      DoSource(SIGTYPE_END, l_s32SSValue);
    }
    
      ClearKeyParam();     
}

static  void    OnExit(void)
{
    Write_n(OFF);
    Write_m(OFF);
    Write_Value_TIME_COUNT_n(&l_u16n, 1000, 1000);//不显示
    Write_Value_m(&l_u16m, 100, 100);//不显示
    l_u8Init = 0;
    l_u8BlinkN = 0;
    l_NOBlink = 0;
      ClearKeyParam(); 
}

static void Get_u16m(void)
{
     if(edit.u8KeyPlaces == 0)
     {  l_u16m = 1;}
     else  if(edit.u8KeyPlaces == 1)
     {
        l_u16m = editNuber[0];
      }
      else if(edit.u8KeyPlaces == 2)
      {
        l_u16m = editNuber[0] * 10 + editNuber[1];       
      }  
}

static void Get_u16n(void)
{
      if(edit.u8KeyPlaces == 0)
      {  l_u16n = 1;}
      else if(edit.u8KeyPlaces == 1)
      {
        l_u16n = editNuber[0];
      }
      else if(edit.u8KeyPlaces == 2)
      {
        l_u16n = editNuber[0] * 10 + editNuber[1];       
      }  
      else if(edit.u8KeyPlaces == 3)
      {
        l_u16n = editNuber[0] * 100 + editNuber[1] * 10 + editNuber[0]; 
      }
}

static  void    DoNUM0Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_NOBlink == 0)
    {    
      if(data.u8ProgMode == PROGNM_M && edit.u8KeyPlaces <= 1)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 0;       
          edit.u8KeyPlaces++;
        }
          Get_u16m();
          Write_Value_m(&l_u16m, 0, MAX_M);            
      }
      if(data.u8ProgMode == PROGNM_N && edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        {  
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 0;       
          edit.u8KeyPlaces++;
        }
        Get_u16n();
        Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m);
      }
    }
    else
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
}

static  void    DoNUM1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_NOBlink == 0)
    {
      if(data.u8ProgMode == PROGNM_M && edit.u8KeyPlaces <= 1)
      {
         if(editNuber[0] == 0)
             return;
         else
         { 
           edit.u8WriteStutus = 1;
           editNuber[edit.u8KeyPlaces] = 1;       
           edit.u8KeyPlaces++;
         }
           Get_u16m();
           Write_Value_m(&l_u16m, 0, MAX_M);            
      }
      if(data.u8ProgMode == PROGNM_N && edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        {  
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 1;       
          edit.u8KeyPlaces++;
        }
        Get_u16n();
        Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m);
      }
      
    }
    else
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
}

static  void    DoNUM2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if(l_NOBlink == 0)
    {    
      if(data.u8ProgMode == PROGNM_M && edit.u8KeyPlaces <= 1)
      {
      
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 2;       
          edit.u8KeyPlaces++;
        }
          Get_u16m();
          Write_Value_m(&l_u16m, 0, MAX_M);            
      }
      if(data.u8ProgMode == PROGNM_N && edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        {  
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 2;       
          edit.u8KeyPlaces++;
        }
        Get_u16n();
        Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m);
      }
    }
    else
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
}

static  void    DoNUM3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if(l_NOBlink == 0)
    {     
      if(data.u8ProgMode == PROGNM_M && edit.u8KeyPlaces <= 1)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 3;       
          edit.u8KeyPlaces++;
        }
          Get_u16m();
          Write_Value_m(&l_u16m, 0, MAX_M);            
      }
      if(data.u8ProgMode == PROGNM_N && edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        {  
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 3;       
          edit.u8KeyPlaces++;
        }
        Get_u16n();
        Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m);
      }
    }
    else
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
}

static  void    DoNUM4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_NOBlink == 0)
    {      
      if(data.u8ProgMode == PROGNM_M && edit.u8KeyPlaces <= 1)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 4;       
          edit.u8KeyPlaces++;
        }
          Get_u16m();
          Write_Value_m(&l_u16m, 0, MAX_M);            
      }
      if(data.u8ProgMode == PROGNM_N && edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        {  
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 4;       
          edit.u8KeyPlaces++;
        }
        Get_u16n();
        Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m);
      }
    }
    else
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
}

static  void    DoNUM5Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_NOBlink == 0)
    {  
      if(data.u8ProgMode == PROGNM_M && edit.u8KeyPlaces <= 1)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 5;       
          edit.u8KeyPlaces++;
        }
          Get_u16m();
          Write_Value_m(&l_u16m, 0, MAX_M);            
      }
      if(data.u8ProgMode == PROGNM_N && edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        {  
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 5;       
          edit.u8KeyPlaces++;
        }
        Get_u16n();
        Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m);
      }
    }
    else
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
}

static  void    DoNUM6Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_NOBlink == 0)
    {      
      if(data.u8ProgMode == PROGNM_M && edit.u8KeyPlaces <= 1)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 6;       
          edit.u8KeyPlaces++;
        }
          Get_u16m();
          Write_Value_m(&l_u16m, 0, MAX_M);            
      }
      if(data.u8ProgMode == PROGNM_N && edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        {  
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 6;       
          edit.u8KeyPlaces++;
        }
        Get_u16n();
        Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m);
      }
    }
    else
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
}


static  void    DoNUM7Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_NOBlink == 0)
    {          
      if(data.u8ProgMode == PROGNM_M && edit.u8KeyPlaces <= 1)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 7;       
          edit.u8KeyPlaces++;
        }
          Get_u16m();
          Write_Value_m(&l_u16m, 0, MAX_M);            
      }
      if(data.u8ProgMode == PROGNM_N && edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        {  
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 7;       
          edit.u8KeyPlaces++;
        }
        Get_u16n();
        Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m);
      }
    }
    else
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
}

static  void    DoNUM8Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_NOBlink == 0)
    {       
      if(data.u8ProgMode == PROGNM_M && edit.u8KeyPlaces <= 1)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 8;       
          edit.u8KeyPlaces++;
        }
          Get_u16m();
          Write_Value_m(&l_u16m, 0, MAX_M);            
      }
      if(data.u8ProgMode == PROGNM_N && edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        {  
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 8;       
          edit.u8KeyPlaces++;
        }
        Get_u16n();
        Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m);
      }
    }
    else
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
}

static  void    DoNUM9Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_NOBlink == 0)
    {       
      if(data.u8ProgMode == PROGNM_M && edit.u8KeyPlaces <= 1)
      {
        if(editNuber[0] == 0)
            return;
        else
        { 
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 9;       
          edit.u8KeyPlaces++;
        }
          Get_u16m();
          Write_Value_m(&l_u16m, 0, MAX_M);            
      }
      if(data.u8ProgMode == PROGNM_N && edit.u8KeyPlaces <= 2)
      {
        if(editNuber[0] == 0)
            return;
        else
        {  
          edit.u8WriteStutus = 1;
          editNuber[edit.u8KeyPlaces] = 9;       
          edit.u8KeyPlaces++;
        }
        Get_u16n();
        Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m);
      }
    }
    else
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
}

static  void    DoOKKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    
     if( l_NOBlink == 1)
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
      
      DrawSourceData();      
     }
     ClearKeyParam();     
     
      if(data.u8ProgMode == PROGNM_M )
      {
          Draw_m();  
     //     l_NOBlink = 1;
          l_u8BlinkN = 0;
          Write_m(ON);
          data.u8ProgMode = PROGNM_N;
      }
      else  if(data.u8ProgMode == PROGNM_N)
      {
          Draw_n();
          l_NOBlink = 1;
          l_u8BlinkN = 0;
          Write_n(ON);
      }
      SourceValue_nm();
      if(data.u8SourceEnable == ON)
      {
        DoSource(data.u8SSigType, l_s32SSValue);
      }
      else
      {
        DoSource(SIGTYPE_END, l_s32SSValue);
      }
      

  }     
}
static  void    DoDELKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if( l_NOBlink == 0)
   {
      if(data.u8ProgMode == PROGNM_M && edit.u8WriteStutus == 1 && edit.u8KeyPlaces != 0)
      {
          edit.u8KeyPlaces  = edit.u8KeyPlaces -1;
//          if(edit.u8KeyPlaces == 0)
//            editNuber[edit.u8KeyPlaces] = 0;   
          Get_u16m();
          Write_Value_m(&l_u16m, 0, MAX_M); 
      }
        
      if(data.u8ProgMode == PROGNM_N && edit.u8WriteStutus == 1 && edit.u8KeyPlaces != 0)
      {
          edit.u8KeyPlaces  = edit.u8KeyPlaces -1;
//          if(edit.u8KeyPlaces == 0)
//            editNuber[edit.u8KeyPlaces] = 0;   
          Get_u16n();
          Write_Value_TIME_COUNT_n(&l_u16n, 0, l_u16m); 
      }              
   }
   else
   {
      if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
      {
         if(edit.u8WriteStutus == 1 && edit.u8KeyPlaces != 0)
        {
          if(edit.u8DotPlace >= edit.u8KeyPlaces)
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
               Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces -  edit.u8DotPlace);
               ClearKeyParam();        
            }                
         }
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
       }
  
     }
   }
      
  }
}

static  void    DoPNKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
       if(l_NOBlink == 1)
       {
          if(edit.u8KeyPlaces == 0)
          {
            s32 s32PNnum = 0;
             Write_Value_SOURCE(&s32PNnum, 0, -99999, 99999);
            edit.u8KeyPN = 1 - edit.u8KeyPN;
            Write_SOURCE_MINUS(edit.u8KeyPN);
            edit.u8WriteStutus = 1;
          }
          else
          {
             edit.u8KeyPN = 1 - edit.u8KeyPN;
             Write_SOURCE_MINUS(edit.u8KeyPN);
              edit.u8WriteStutus = 1;
          }
       }
  }
}

static  void    DoDotKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(l_NOBlink == 1)
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
    l_NOBlink = 1;
    Write_n(ON);
    Write_m(ON);
    //输出
    SourceValue_nm();
    if(data.u8SourceEnable == ON)
    {
      DoSource(data.u8SSigType, l_s32SSValue);
    }
    else
    {
      DoSource(SIGTYPE_END, l_s32SSValue);
    }
  }
}

static  void    DoSFUNCKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    ;
  }
}

static  void    DoSRANGEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    ;
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
     ;
  }
}

static  void    DoTEMPKey(PKeyMsg pKeyMsg)
{

    if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)  //热电偶
    {
      ;
    }
}

static  void    DoSWEEPKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    ;
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
      data.u8ProgMode = PROGNM_M;
      l_NOBlink = 0;
      l_u8BlinkN = 0;
      ClearKeyParam(); 
      Write_n(ON);
  }
}

static  void    DoXIAKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      data.u8ProgMode = PROGNM_N;
      l_NOBlink = 0;
      l_u8BlinkN = 0;
      ClearKeyParam(); 
      Write_m(ON);
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
    l_NOBlink = 0;
    l_u8BlinkN = 0;
    data.u8ProgMode++;
    if(data.u8ProgMode > PROGNM_N)
      data.u8ProgMode = PROGNM_NONE;
    if(data.u8ProgMode == PROGNM_NONE)
    {
       OnExit();
       SetView(VIEW_MAIN);
       return;
    }
    else if(data.u8ProgMode == PROGNM_M)
    {
        Write_n(ON);
    }
    else if(data.u8ProgMode == PROGNM_N)
    {      
        Write_m(ON);
    }
     Draw_n();
     Draw_m();
    //输出
    SourceValue_nm();
    if(data.u8SourceEnable == ON)
    {
      DoSource(data.u8SSigType, l_s32SSValue);
    }
    else
    {
      DoSource(SIGTYPE_END, l_s32SSValue);
    }

      ClearKeyParam(); 
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
