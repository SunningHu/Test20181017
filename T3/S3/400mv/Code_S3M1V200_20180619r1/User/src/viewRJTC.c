/*=============================================================================
//File:			viewRJTC.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			热电偶冷端显示画面
===============================================================================*/
#include "viewRJTC.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewMain.h"
#include "viewFrame.h"
#include <string.h>
#include "DriveEEprom.h"
#include "lcd.h"

#define     SELF_VIEWID     VIEW_RJTC

static  u8      l_u8Init = 0;               //初始化状态
static  u8      l_u8Blink = 0;
extern u8 editNuber[6];

//Summary： 显示冷端值
static  void    DrawRJTC(void)
{     
    s32     s32RJ;
    
    if(data.u8RJMode == RJMODE_AUTO)
    {
       if(data.u8TempType == 0)
          s32RJ = data.s32RJAuto;
       else
         s32RJ = (s32)((((float)data.s32RJAuto/10.0) * 1.8 + 32) * 10);
    } 
    else if(data.u8RJMode == RJMODE_MAN)
    {
        s32RJ = data.s32RJSet;
    }
    
    
    if(data.u8SSigType > SIGTYPE_PT100 || data.u8MSigType > SIGTYPE_PT100)
    {
        Write_Value_SOURCE(&s32RJ, 1, -1999, 1999);
        Write_RJModeS(data.u8RJMode);
        Write_SOURCE_Unit(SIGTYPE_K);   //℃ 
    }
//    if(data.u8MSigType > SIGTYPE_PT100)
//    {
//        Write_Value_MEASURE(&s32RJ, 1, -1999, 1999);
//        Write_RJModeM(data.u8RJMode);
//        Write_MEASURE_Unit(SIGTYPE_K);   //℃ 
//    }
}

//Summary： 绘制静态刷新
static  void    DrawStatic(void)
{
    DrawRJTC();
}

//Summary： 绘制动态刷新
static  void    DrawDynamic(void)
{
  //℃每秒闪烁一次
  if(edit.u8WriteStutus == 0)
    DrawRJTC();
  l_u8Blink = !l_u8Blink;

    if(l_u8Blink == 0)
      Write_SOURCE_Unit(SIGTYPE_K);   //℃
    else
      Write_SOURCE_Unit(SIGTYPE_END);  

//  if(data.u8MSigType > SIGTYPE_PT100)
//  {
//    if(l_u8Blink == 0)
//      Write_MEASURE_Unit(SIGTYPE_K);   //℃
//    else
//      Write_MEASURE_Unit(SIGTYPE_END);  
//  }  
  
  if(data.u8MRangeEnable == 1)
    DrawMRangeValueRealTime();
  else
    DrawMeasureValueRealTime();
  
  //输出
  if(data.u8SourceEnable == ON && data.u8SSigType > SIGTYPE_PT100)
  {
    DoSource(data.u8SSigType, data.s32SourceValue);
  }
}

static  void    OnCreate(void)
{
    l_u8Blink = 0;
    l_u8Init = 1;
    
    ClearKeyParam();   
}

static  void    OnExit(void)
{
    Write_RJModeM(RJMODE_NONE);
    Write_RJModeS(RJMODE_NONE);
    l_u8Init = 0;
}

static  void    DoNUM0Key(PKeyMsg pKeyMsg)
{  if(data.u8RJMode == RJMODE_MAN) {
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
}

static  void    DoNUM1Key(PKeyMsg pKeyMsg)
{
   if(data.u8RJMode == RJMODE_MAN) {
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
}

static  void    DoNUM2Key(PKeyMsg pKeyMsg)
{  if(data.u8RJMode == RJMODE_MAN) {
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
}

static  void    DoNUM3Key(PKeyMsg pKeyMsg)
{
    if(data.u8RJMode == RJMODE_MAN) {
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
}

static  void    DoNUM4Key(PKeyMsg pKeyMsg)
{
    if(data.u8RJMode == RJMODE_MAN) {
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
}

static  void    DoNUM5Key(PKeyMsg pKeyMsg)
{
    if(data.u8RJMode == RJMODE_MAN) {
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
}

static  void    DoNUM6Key(PKeyMsg pKeyMsg)
{
    if(data.u8RJMode == RJMODE_MAN) {
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
}


static  void    DoNUM7Key(PKeyMsg pKeyMsg)
{
    if(data.u8RJMode == RJMODE_MAN) {
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
}

static  void    DoNUM8Key(PKeyMsg pKeyMsg)
{
    if(data.u8RJMode == RJMODE_MAN) {
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
}

static  void    DoNUM9Key(PKeyMsg pKeyMsg)
{
    if(data.u8RJMode == RJMODE_MAN) {
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
}

static  void    DoOKKey(PKeyMsg pKeyMsg)
{
    if(data.u8RJMode == RJMODE_MAN) {
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {

      if(edit.u8WriteStutus == 1)
      {
         data.fSourceValue = Dodata_s32(editNuber,edit.u8KeyPlaces,edit.u8KeyPN,edit.u8DotPlace);
         data.u8DotNum = edit.u8KeyPlaces - edit.u8DotPlace;
         if(data.u8DotNum == edit.u8KeyPlaces)
           data.u8DotNum = 0;     
         data.s32RJSet =   Write_Value_fSOURCE(&data.fSourceValue,data.u8DotNum, 1, 
                -2000, 2000);
      }
      ClearKeyParam(); 
      DrawRJTC();    
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
static  void    DoDELKey(PKeyMsg pKeyMsg)
{
    if(data.u8RJMode == RJMODE_MAN) 
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

static  void    DoPNKey(PKeyMsg pKeyMsg)
{
    if(data.u8RJMode == RJMODE_MAN)
    {
       if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
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
  if(data.u8RJMode == RJMODE_MAN)                                               
  {
      if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
      {
  
        if(edit.u8DotPlace == 0 && edit.u8KeyPlaces != 0)
          edit.u8DotPlace = edit.u8KeyPlaces;    
        Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace);
         
      }
  }
}

static  void    DoSOURCEKey(PKeyMsg pKeyMsg)
{ 
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    ;
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
    ;
  }
}

static  void    DoMFUNCKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    ;
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
  if(data.u8TempType == 0)
  {
    memcpy(g_s32MSigRange,g_s32MSigRange1,sizeof(g_s32MSigRange));
    memcpy(g_s32SSigRange,g_s32SSigRange1,sizeof(g_s32SSigRange));
  }
  else
  {
    memcpy(g_s32MSigRange,g_s32MSigRange2,sizeof(g_s32MSigRange));
    memcpy(g_s32SSigRange,g_s32SSigRange2,sizeof(g_s32SSigRange));
  }    
        OnExit();
        SetView(VIEW_MAIN);
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
    ;
  }
}

static  void    DoSHANGKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
//    if(data.u8RJMode == RJMODE_MAN )
//    {        
//      if(edit.u8WriteStutus == 1)
//      {
//          ClearKeyParam(); 
//          data.s32RJSet = 0;
//          Write_Value_SOURCE(&data.s32RJSet,1,  -2000, 2000);    
//       }
//       else
//       {
//          data.s32RJSet = Write_UP_SOURCE(&data.s32RJSet,data.u8DotNum,1, -2000, 2000);
//          Write_Value_SOURCE(&data.s32RJSet, 1,  -2000, 2000);
//       }
//    }
    if(edit.u8WriteStutus == 0)
    {
      data.u8TempType = 1 - data.u8TempType;
      if(data.u8SSigType > SIGTYPE_PT100) 
         Write_SOURCE_TempType(data.u8TempType);
      if(data.u8MSigType > SIGTYPE_PT100)  
         Write_MEASURE_TempType(data.u8TempType);
      eep_write8(202,data.u8TempType);
    }
  }
}

static  void    DoXIAKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
//    if(data.u8RJMode == RJMODE_MAN )
//    {        
//      if(edit.u8WriteStutus == 1)
//      {
//        ClearKeyParam(); 
//        data.s32RJSet = 0;
//        Write_Value_SOURCE(&data.s32RJSet, 1,  -2000, 2000);   
//      }
//      else
//      {
//          data.s32RJSet = Write_DOWN_SOURCE(&data.s32RJSet,data.u8DotNum,1, -2000, 2000);
//          Write_Value_SOURCE(&data.s32RJSet, 1,  -2000, 2000);
//      }
//    }
    
    if(edit.u8WriteStutus == 0)
    {
      data.u8TempType = 1 - data.u8TempType;
      if(data.u8SSigType > SIGTYPE_PT100) 
         Write_SOURCE_TempType(data.u8TempType);
      if(data.u8MSigType > SIGTYPE_PT100)  
         Write_MEASURE_TempType(data.u8TempType);
      eep_write8(202,data.u8TempType);
    }
  }
}


static  void  DoMODEKey(PKeyMsg pKeyMsg)
{

      if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
      {
          data.u8RJMode = 1 - data.u8RJMode;
    //      eep_write8(201,data.u8RJMode);
          if(data.u8SSigType > SIGTYPE_PT100 || data.u8MSigType > SIGTYPE_PT100)
          {
            Write_RJModeS(data.u8RJMode);
          }
//          if(data.u8MSigType > SIGTYPE_PT100)
//          {
//            Write_RJModeM(data.u8RJMode);
//          }
      }
}




static  void    DoPROGKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    ;
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

void    ViewRJTC(PKeyMsg pKeyMsg)
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
