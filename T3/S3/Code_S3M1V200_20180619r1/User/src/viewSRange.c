/*=============================================================================
//File:			viewSRange.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			SOURCE换算后画面
===============================================================================*/
#include "viewSRange.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewMain.h"
#include "viewFrame.h"
#include <string.h>
#include "DriveEEprom.h"
#include "lcd.h"

#define     SELF_VIEWID     VIEW_SRANGE

static  u8      l_u8Init = 0;               //初始化状态
extern  u8      editNuber[6];

//Summary： Range值换算成Source值
static  void    SRangetoSValue(void)
{ 
  s32   s32Tmp;
  
  s32Tmp = data.s32SRH - data.s32SRL;
  if(s32Tmp == 0)
    s32Tmp = 1;
   data.s32SourceValue = (s32)(((float)(data.s32SSH - data.s32SSL) * (float)(data.s32SRangeValue - data.s32SRL)) / (float)s32Tmp + (float)data.s32SSL); 
}

//Summary： SOURCE 换算后值
static  void    DrawSRangeValue(void)
{      
    Write_SOURCE_RangeFlg(RANGE_NONE);
    Write_Value_SOURCE(&data.s32SRangeValue, data.u8SRLHDot, data.s32SRL, data.s32SRH);
    Write_SOURCE_Unit(SIGTYPE_END); 
}

//Summary： 绘制静态刷新
static  void    DrawStatic(void)
{
  DrawSRangeValue();
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
    data.s32SRangeValue = 0;
    l_u8Init = 1;
    
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
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
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
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
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
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
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
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
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
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
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
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
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
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
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
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
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
          Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace); 
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

static  void    DoPNKey(PKeyMsg pKeyMsg)
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

static  void    DoDotKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if(edit.u8WriteStutus == 0)
      {     
         //信号类型切换过，自动变成OFF
          data.u8SourceEnable = OFF;
          DrawSourceEnable();
          DoSource(SIGTYPE_END, data.s32SourceValue);
          OnExit();
          SetView(VIEW_MAIN);
      }
      else
      {
        if(edit.u8DotPlace == 0)
          edit.u8DotPlace = edit.u8KeyPlaces;    
        Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace);
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
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s32SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s32SourceValue);
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
  if(data.u8MSigType > SIGTYPE_PT100)  //热电偶
  {
    if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
    {
      OnExit();
      SetView(VIEW_RJTC);
    }
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
         data.s32SRangeValue =   Write_Value_fSOURCE(&data.fSourceValue,data.u8DotNum, data.u8SRLHDot, data.s32SRL, data.s32SRH);
      //    data.s32SRangeValue =   Write_Value_fSOURCE(&data.fSourceValue,0, 3, 5000, 10000);
      }
      DrawSRangeValue();
       if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s32SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s32SourceValue);
      }      
      
      ClearKeyParam();  
    
  }
}
static  void    DoDELKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if(edit.u8WriteStutus == 1 && edit.u8KeyPlaces != 0)
      {
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
             Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces -  edit.u8DotPlace);
             ClearKeyParam();  
          }      
        }
        
        Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces - edit.u8DotPlace);
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
    if(edit.u8WriteStutus == 1)
    {
      data.s32SRangeValue = data.s32SRL;
      Write_Value_SOURCE(&data.s32SRangeValue, data.u8SRLHDot, data.s32SRL, data.s32SRH);
      ClearKeyParam();  
    }
    else
    {
      data.s32SRangeValue = Write_UP_SOURCE(&data.s32SRangeValue,data.u8DotNum,data.u8SRLHDot, data.s32SRL, data.s32SRH);
      Write_Value_SOURCE(&data.s32SRangeValue, data.u8SRLHDot, data.s32SRL, data.s32SRH);
    }
      if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s32SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s32SourceValue);
      }    
  }
}

static  void    DoXIAKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 1)
    {
      data.s32SRangeValue = data.s32SRL;
      Write_Value_SOURCE(&data.s32SRangeValue, data.u8SRLHDot, data.s32SRL, data.s32SRH);
      ClearKeyParam();
   }
   else
   {
      data.s32SRangeValue = Write_DOWN_SOURCE(&data.s32SRangeValue,data.u8DotNum,data.u8SRLHDot, data.s32SRL, data.s32SRH);
       Write_Value_SOURCE(&data.s32SRangeValue, data.u8SRLHDot, data.s32SRL, data.s32SRH);
   }
      if(data.u8SourceEnable == ON)
      {
        SRangetoSValue();
        DoSource(data.u8SSigType, data.s32SourceValue);
      }
      else
      {
        DoSource(SIGTYPE_END, data.s32SourceValue);
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

void    ViewSRange(PKeyMsg pKeyMsg)
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
