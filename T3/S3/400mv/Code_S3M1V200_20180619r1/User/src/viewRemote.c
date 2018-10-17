/*=============================================================================
//File:			viewMain.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			�����漰����
===============================================================================*/
#include "viewRemote.h"
#include "stm8l15x_tim1.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim5.h"
#include "dodata.h"
#include "viewFrame.h"
#include <string.h>
#include "DriveEEprom.h"
#include "lcd.h"

#define     SELF_VIEWID         VIEW_REMOTE
extern  void delay_ms(u16 ms);

static  u8      l_u8Init = 0;               //��ʼ��״̬
static  u16     l_u16n = 485;               //��ʼ��״̬
static  u8      l_u8MSigType = 1;


//Summary�� SOURCE ON/OFF
static void DrawSourceEnable(void)
{
    Write_SOURCE_ON_OFF(data.u8SourceEnable);
}

//Summary�� SOURCEֵ
static void DrawSourceData(void)
{
    Write_SOURCE_RangeFlg(RANGE_NONE);      //����ʾRL,RH,SL,SH
    
        Write_Value_SOURCE(&data.s32SourceValue, g_s32SSigRange[data.u8SSigType][2], 
                g_s32SSigRange[data.u8SSigType][0], g_s32SSigRange[data.u8SSigType][1]);          



    Write_SOURCE_Unit(data.u8SSigType);        //��λ
    if(data.u8MeasureEnable == ON)
      Write_SigType(data.u8SSigType, data.u8MSigType);  //�ź�����
    else
      Write_SigType(data.u8SSigType, SIGTYPE_END);  //�ź�����    
    
// Write_Value_SOURCE(&adCode.u16RRef, 0, 
//                0, 70000);     
    
}

//Summary�� MEASUREֵ
static void DrawMeasureData(void)
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
//Summary�� MEASURE ON
static void DrawMeasureEnable(void)
{
    Write_MEASURE(ON);
    if(data.u8MeasureEnable == OFF)
    {
        Write_Value_MEASURE(&data.s32MeasureValue, 5, 0, 10000);    //4λС��ʱ��������ʾ        
        Write_SigType(data.u8SSigType, SIGTYPE_END); 
        Write_MEASURE_Unit(SIGTYPE_END);
        Write_MEASURE_RangeFlg(RANGE_NONE);
    }
    else
    {
        DrawMeasureData();
    }
}

//Summary�� ���ƾ�̬ˢ��
static void DrawStatic(void)
{
  Write_Value_TIME_COUNT_n(&l_u16n, 0, 500);  
   DrawSourceEnable();
   DrawSourceData();
   DrawMeasureEnable();
}


//Summary�� ���ƶ�̬ˢ��
static  void    DrawDynamic(void)
{
  if(data.u8MSigType != l_u8MSigType)
  {
   l_u8MSigType =  data.u8MSigType;
   if(data.u8MSigType != SIGTYPE_24VLOOP)
   {
         GPIO_ResetBits(GPIOB, GPIO_Pin_3);
         delay_ms(10);
         GPIO_SetBits(GPIOB, GPIO_Pin_3);
         data.u8VLoopFflag = 1;
   }
   else
   {
          GPIO_ResetBits(GPIOB, GPIO_Pin_4);
          delay_ms(10);
          GPIO_SetBits(GPIOB, GPIO_Pin_4);
          data.u8VLoopFflag = 1;     
   }
   data.u8VLoopFflag = 0;
  }
   DrawSourceEnable();
   DrawSourceData();
   DrawMeasureEnable();
     if(data.u8SourceEnable == ON )    
        DoSource(data.u8SSigType, data.s32SourceValue);    
     else
        DoSource(SIGTYPE_END, data.s32SourceValue);    
     

}

static  void    OnCreate(void)
{
    l_u8Init = 1;
    //���
           
      
}



static  void    DoNUM0Key(PKeyMsg pKeyMsg)
{

}

static  void    DoNUM1Key(PKeyMsg pKeyMsg)
{

  
  
}

static  void    DoNUM2Key(PKeyMsg pKeyMsg)
{

}

static  void    DoNUM3Key(PKeyMsg pKeyMsg)
{

}

static  void    DoNUM4Key(PKeyMsg pKeyMsg)
{

}

static  void    DoNUM5Key(PKeyMsg pKeyMsg)
{
   
}

static  void    DoNUM6Key(PKeyMsg pKeyMsg)
{
   
}


static  void    DoNUM7Key(PKeyMsg pKeyMsg)
{

}

static  void    DoNUM8Key(PKeyMsg pKeyMsg)
{

}

static  void    DoNUM9Key(PKeyMsg pKeyMsg)
{

}

static  void    DoOKKey(PKeyMsg pKeyMsg)
{

}
static  void    DoDELKey(PKeyMsg pKeyMsg)
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

static  void    DoBLIGHTKey(PKeyMsg pKeyMsg)
{

}

static  void    DoHOLDKey(PKeyMsg pKeyMsg)
{

}

static  void    DoSHANGKey(PKeyMsg pKeyMsg)
{

}

static  void    DoXIAKey(PKeyMsg pKeyMsg)
{

}


static  void  DoMODEKey(PKeyMsg pKeyMsg)
{

  
}




static  void    DoPROGKey(PKeyMsg pKeyMsg)
{

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

void  ViewRemote(PKeyMsg pKeyMsg)
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

