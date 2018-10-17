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
#include <string.h>
#include "DriveEEprom.h"
#include "lcd.h"

#define     SELF_VIEWID     VIEW_SRSLH

extern  u8      editNuber[6];
static  u8      l_u8Init = 0;               //初始化状态
static  u8      l_u8Step = RANGE_RL;
   

//Summary： SOURCE RL
static  void    DrawSRL(void)
{   
    Write_SOURCE_RangeFlg(RANGE_RL);
    Write_Value_SOURCE(&data.s32SRL, data.u8SRLHDot, -99999, 99999);
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
    Write_Value_SOURCE(&data.s32SRH, data.u8SRLHDot, -99999, 99999);
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
    Write_Value_SOURCE(&data.s32SSL, g_s32SSigRange[data.u8SSigType][2], 
          g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);
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
    Write_Value_SOURCE(&data.s32SSH, g_s32SSigRange[data.u8SSigType][2], 
          g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);
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
      if(edit.u8KeyPlaces <= 4)
      {
        if(edit.u8WriteStutus == 0)
        {
             if(editNuber[0] == 0 && edit.u8DotPlace != 1)
                return;
             else
            {  
              edit.u8KeyPN = 0;
              Write_SOURCE_MINUS(edit.u8KeyPN);
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 0;       
              edit.u8KeyPlaces++;
              Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
            }          
        }
        else
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
      if( edit.u8KeyPlaces <= 4)
      {
        if(edit.u8WriteStutus == 0)
        {
             if(editNuber[0] == 0 && edit.u8DotPlace != 1)
                return;
             else
            {  
              edit.u8KeyPN = 0;
              Write_SOURCE_MINUS(edit.u8KeyPN);
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 1;       
              edit.u8KeyPlaces++;
              Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
            }          
        }
        else
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
      if(edit.u8KeyPlaces <= 4)
      {
        if(edit.u8WriteStutus == 0)
        {
             if(editNuber[0] == 0 && edit.u8DotPlace != 1)
                return;
             else
            {  
              edit.u8KeyPN = 0;
              Write_SOURCE_MINUS(edit.u8KeyPN);
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 2;       
              edit.u8KeyPlaces++;
              Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
            }          
        }
        else
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
      if(edit.u8KeyPlaces <= 4)
      {
        if(edit.u8WriteStutus == 0)
        {
             if(editNuber[0] == 0 && edit.u8DotPlace != 1)
                return;
             else
            {  
              edit.u8KeyPN = 0;
              Write_SOURCE_MINUS(edit.u8KeyPN);
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 3;       
              edit.u8KeyPlaces++;
              Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
            }          
        }
        else
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
      if(edit.u8KeyPlaces <= 4)
      {
        if(edit.u8WriteStutus == 0)
        {
             if(editNuber[0] == 0 && edit.u8DotPlace != 1)
                return;
             else
            {  
              edit.u8KeyPN = 0;
              Write_SOURCE_MINUS(edit.u8KeyPN);
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 4;       
              edit.u8KeyPlaces++;
              Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
            }          
        }
        else
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
      if(edit.u8KeyPlaces <= 4)
      {
        if(edit.u8WriteStutus == 0)
        {
             if(editNuber[0] == 0 && edit.u8DotPlace != 1)
                return;
             else
            {  
              edit.u8KeyPN = 0;
              Write_SOURCE_MINUS(edit.u8KeyPN);
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 5;       
              edit.u8KeyPlaces++;
              Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
            }          
        }
        else
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
      if(edit.u8KeyPlaces <= 4)
      {
        if(edit.u8WriteStutus == 0)
        {
             if(editNuber[0] == 0 && edit.u8DotPlace != 1)
                return;
             else
            {  
              edit.u8KeyPN = 0;
              Write_SOURCE_MINUS(edit.u8KeyPN);
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 6;       
              edit.u8KeyPlaces++;
              Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
            }          
        }
        else
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
      if(edit.u8KeyPlaces <= 4)
      {
        if(edit.u8WriteStutus == 0)
        {
             if(editNuber[0] == 0 && edit.u8DotPlace != 1)
                return;
             else
            {  
              edit.u8KeyPN = 0;
              Write_SOURCE_MINUS(edit.u8KeyPN);
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 7;       
              edit.u8KeyPlaces++;
              Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
            }          
        }
        else
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
      if(edit.u8KeyPlaces <= 4)
      {
        if(edit.u8WriteStutus == 0)
        {
             if(editNuber[0] == 0 && edit.u8DotPlace != 1)
                return;
             else
            {  
              edit.u8KeyPN = 0;
              Write_SOURCE_MINUS(edit.u8KeyPN);
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 8;       
              edit.u8KeyPlaces++;
              Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
            }          
        }
        else
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
      if(edit.u8KeyPlaces <= 4)
      {
        if(edit.u8WriteStutus == 0)
        {
             if(editNuber[0] == 0 && edit.u8DotPlace != 1)
                return;
             else
            {  
              edit.u8KeyPN = 0;
              Write_SOURCE_MINUS(edit.u8KeyPN);
              edit.u8WriteStutus = 1;
              editNuber[edit.u8KeyPlaces] = 9;       
              edit.u8KeyPlaces++;
              Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
            }          
        }
        else
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
      if(l_u8Step == RANGE_RL)
      {
        data.u8SRLHDot ++;
        if(data.u8SRLHDot > 4)
          data.u8SRLHDot = 0;
        DrawSRL();
      }
      else if(l_u8Step == RANGE_RH)
      {
        data.u8SRLHDot ++;
        if(data.u8SRLHDot > 4)
          data.u8SRLHDot = 0;
        DrawSRH();
      }
      else;
    }
    else
    {
       if(edit.u8DotPlace == 0)
          edit.u8DotPlace = edit.u8KeyPlaces;    
       Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces, edit.u8KeyPlaces - edit.u8DotPlace);
    }
    
  }
}

static  void    DoSOURCEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
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
    if(edit.u8WriteStutus == 0)
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
  }
}

static  void  DoOKKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
      if(edit.u8WriteStutus == 1)
      {
         data.fSourceValue = Dodata_s32(editNuber,edit.u8KeyPlaces,edit.u8KeyPN,edit.u8DotPlace);
         data.u8DotNum = edit.u8KeyPlaces - edit.u8DotPlace;
         if(data.u8DotNum == edit.u8KeyPlaces)
           data.u8DotNum = 0;
         if(l_u8Step == RANGE_RL)
         {
            data.s32SRL =  Write_Value_fSOURCE(&data.fSourceValue,data.u8DotNum, data.u8SRLHDot, -99999, 99999);
            DrawSRL();
         }
         else if(l_u8Step == RANGE_RH)
         {
            data.s32SRH =  Write_Value_fSOURCE(&data.fSourceValue,data.u8DotNum, data.u8SRLHDot, -99999, 99999);
            DrawSRH();
         }
         else if(l_u8Step == RANGE_SL)
         {
            data.s32SSL =  Write_Value_fSOURCE(&data.fSourceValue,data.u8DotNum, data.u8SRLHDot,g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);
            DrawSSL();
         }
         else if(l_u8Step == RANGE_SH)
         {
            data.s32SSH =  Write_Value_fSOURCE(&data.fSourceValue,data.u8DotNum, data.u8SRLHDot,g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);
            DrawSSH();
         }
         else;

      
      ClearKeyParam(); 
    }
    else
    {
         if(l_u8Step == RANGE_RL)
         {
            DrawSRL();
         }
         else if(l_u8Step == RANGE_RH)
         {
            DrawSRH();
         }
         else if(l_u8Step == RANGE_SL)
         {
            DrawSSL();
         }
         else if(l_u8Step == RANGE_SH)
         {
            DrawSSH();
         }
         else;      
    }
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
         Edit_Value_SOURCE(editNuber, edit.u8KeyPlaces,edit.u8KeyPlaces -  edit.u8DotPlace);
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
    if(data.u8MSigType != SIGTYPE_24VLOOP)
    {
       data.u8MHoldEnable = !data.u8MHoldEnable;
       DrawHoldEnable();
       
    }
  }
}

static  void    DoSHANGKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 1)
    {
           ClearKeyParam(); 
         if(l_u8Step == RANGE_RL)
         {
            data.s32SRL =  0;
            DrawSRL();
         }
         else if(l_u8Step == RANGE_RH)
         {
            data.s32SRH =  0;
            DrawSRH();
         }
         else if(l_u8Step == RANGE_SL)
         {
            data.s32SSL =  0;
            DrawSSL();
         }
         else if(l_u8Step == RANGE_SH)
         {
            data.s32SSH =  0;
            DrawSSH();
         }
         else;   
    }
    else
    {
         if(l_u8Step == RANGE_RL)
         {
            data.s32SRL =  Write_UP_SOURCE(&data.s32SRL,data.u8DotNum, data.u8SRLHDot, -99999, 99999);
            DrawSRL();
         }
         else if(l_u8Step == RANGE_RH)
         {
            data.s32SRH =  Write_UP_SOURCE(&data.s32SRH,data.u8DotNum, data.u8SRLHDot, -99999, 99999);
            DrawSRH();
         }
         else if(l_u8Step == RANGE_SL)
         {
            data.s32SSL =  Write_UP_SOURCE(&data.s32SSL,data.u8DotNum, data.u8SRLHDot, g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);
            DrawSSL();
         }
         else if(l_u8Step == RANGE_SH)
         {
            data.s32SSH =  Write_UP_SOURCE(&data.s32SSH,data.u8DotNum, data.u8SRLHDot, g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);
            DrawSSH();
         }
         else;
    }
  }
}

static  void    DoXIAKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_DOWN)
  {
    if(edit.u8WriteStutus == 1)
    {
         if(l_u8Step == RANGE_RL)
         {
            data.s32SRL =  0;
            DrawSRL();
         }
         else if(l_u8Step == RANGE_RH)
         {
            data.s32SRH =  0;
            DrawSRH();
         }
         else if(l_u8Step == RANGE_SL)
         {
            data.s32SSL =  0;
            DrawSSL();
         }
         else if(l_u8Step == RANGE_SH)
         {
            data.s32SSH =  0;
            DrawSSH();
         }
         else;
      ClearKeyParam();   
   }
   else
   {
         if(l_u8Step == RANGE_RL)
         {
            data.s32SRL =  Write_DOWN_SOURCE(&data.s32SRL,data.u8DotNum, data.u8SRLHDot, -99999, 99999);
            DrawSRL();
         }
         else if(l_u8Step == RANGE_RH)
         {
            data.s32SRH =  Write_DOWN_SOURCE(&data.s32SRH,data.u8DotNum, data.u8SRLHDot, -99999, 99999);
            DrawSRH();
         }
         else if(l_u8Step == RANGE_SL)
         {
            data.s32SSL =  Write_DOWN_SOURCE(&data.s32SSL,data.u8DotNum, data.u8SRLHDot, g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);
            DrawSSL();
         }
         else if(l_u8Step == RANGE_SH)
         {
            data.s32SSH =  Write_DOWN_SOURCE(&data.s32SSH,data.u8DotNum, data.u8SRLHDot, g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);
            DrawSSH();
         }
         else;     
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
