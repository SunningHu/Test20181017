/*=============================================================================
//File:			viewSweep.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			sweep����
===============================================================================*/
#include "viewSweep.h"
#include "gui.h"
#include "key.h"
#include "stm8l15x_tim3.h"
#include "dodata.h"
#include "viewMain.h"
#include "viewFrame.h"

#define     SELF_VIEWID     VIEW_SWEEP

static  u8      l_u8Init = 0;               //��ʼ��״̬
static  u16     l_u16Timetn[4] = {10, 10, 10, 10};
static  u16     l_u16Count = 0;

static  u8      l_u8DrawStep = TIME_T1;
static  u8      l_u8Sweepstep = TIME_T1;  //SWEEP����
static  u16     l_u16Cnt = 1;       //��ʱ
static  u8      l_u8Blink = 0;      //��˸
static  u8      l_u8CntStatus = 0;      //countΪ0������ѭ��
static  s16     l_s16SSweepValue = 0;       //SOURCE SWEEPֵ
static  float   l_fSweepData = 0; //sweepÿ�α仯ֵ


//Summary�� ��ʾtn
static  void    Draw_Timetn(void)
{  
    Write_Value_TIME_COUNT_n(&l_u16Timetn[l_u8DrawStep], 0, 999);  
}

//Summary�� ��ʾCount
static  void    Draw_Count(void)
{  
    Write_Value_TIME_COUNT_n(&l_u16Count, 0, 999);  
}

//Summary�� ��ʾSweep
static  void    DrawSweep(void)
{  
    Write_TIME(ON);
    Write_Time_t(TIME_SWEEP);    //ȫ��
    Write_COUNT(ON);
    Draw_Count();  
}

//Summary�� ��ʾtn��˸
static  void    DrawTnBlink(void)
{  
  l_u8Blink = !l_u8Blink;
  if(l_u8Blink == 0)
  { 
    Write_Time_t(TIME_SWEEP);        //ȫ��ʾ
  }
  else
  {
    Write_Time_t(l_u8Sweepstep + TIME_SWEEPSTEP1);    //SWEEP STEP
  }
}

//Summary�� SOURCE SWEEPֵ
static  void    DrawSSweepValue(void)
{
    Write_Value_SOURCE(&l_s16SSweepValue, g_s16SSigRange[data.u8SSigType][2], 
            g_s16SSigRange[data.u8SSigType][0], g_s16SSigRange[data.u8SSigType][1]);
}

//Summary�� ���ƾ�̬ˢ��
static  void    DrawStatic(void)
{
    if(l_u8DrawStep < TIME_TEND)        //��ʾTn
    {
      Write_COUNT(OFF);
      Write_Value_TIME_COUNT_n(&l_u16Count, 1000, 1000);//����ʾ
   
      Write_TIME(ON);
      Write_s(ON);
      Write_Time_t(l_u8DrawStep);
      Draw_Timetn();
    }
    else            //��ʾcount
    {
      Write_TIME(OFF);
      Write_s(OFF);
      Write_Time_t(TIME_TEND);
      Write_Value_TIME_COUNT_n(&l_u16Timetn[l_u8DrawStep], 1000, 1000);//����ʾ
   
      Write_COUNT(ON);
      Draw_Count();
      if(l_u16Count > 0)
        l_u8CntStatus = 1;  //����ѭ��
      else
        l_u8CntStatus = 0;
    }
}

//Summary�� ���ƶ�̬ˢ��
static  void    DrawDynamic(void)
{
  

  //����
  if(data.u8MRangeEnable == 1)
    DrawMRangeValueRealTime();
  else
    DrawMeasureValueRealTime();
  //���
  if(data.u8SourceEnable == ON)    //��ʼ���
  {
    l_u16Cnt ++;        //ÿ100ms++   
    if(l_u8CntStatus == 0 || (l_u8CntStatus == 1 && l_u16Count > 0)) //����ѭ����������ѭ��CNT����0
    {
      if(!(l_u16Cnt % 10))
        DrawTnBlink();      //ÿ����˸
      
      
      
      if(l_u8Sweepstep == TIME_T1)  //T1���
      {
        l_fSweepData += (float)(data.s16SourceValue - g_s16SSigRange[data.u8SSigType][0]) / (l_u16Timetn[l_u8Sweepstep]) / 10.0; //ÿ100ms++ 
        l_s16SSweepValue = (s16)l_fSweepData;
        if(l_s16SSweepValue > data.s16SourceValue)
          l_s16SSweepValue = data.s16SourceValue;
        
        DrawSSweepValue();
        //���
        DoSource(data.u8SSigType, l_s16SSweepValue);
      }
      else if(l_u8Sweepstep == TIME_T2)  //T2���
      {
        l_s16SSweepValue = data.s16SourceValue;
        l_fSweepData = l_s16SSweepValue;
        DrawSSweepValue();
        
        //���
        DoSource(data.u8SSigType, l_s16SSweepValue);
      }
      else if(l_u8Sweepstep == TIME_T3)  //T3���
      {        
        l_fSweepData -= (float)(data.s16SourceValue - g_s16SSigRange[data.u8SSigType][0]) / (l_u16Timetn[l_u8Sweepstep]) / 10;
        l_s16SSweepValue = (s16)l_fSweepData;
        DrawSSweepValue();      //�˴����޷�
        //���
        DoSource(data.u8SSigType, l_s16SSweepValue);
      }
      else if(l_u8Sweepstep == TIME_T4)  //T4���
      {
        l_s16SSweepValue  = g_s16SSigRange[data.u8SSigType][0];
        l_fSweepData = l_s16SSweepValue;
        DrawSSweepValue();
        //���
        DoSource(data.u8SSigType, l_s16SSweepValue);
      }
      else    //һ��ѭ������
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
    else        //����
    {
      data.u8SourceEnable = OFF;
      l_u16Cnt = 1;
      DrawSourceEnable();
      DrawStatic();
      //,�ر�,���
      DoSource(SIGTYPE_END, l_s16SSweepValue);
    }
  }
}

static  void    OnCreate(void)
{
    l_u8DrawStep = TIME_T1;
    data.u8SweepStatus = 0;
    l_u8Sweepstep = TIME_T1;
    l_s16SSweepValue = g_s16SSigRange[data.u8SSigType][0];
    l_fSweepData = l_s16SSweepValue;
    l_u16Cnt = 1;
    l_u8Init = 1;
}

static  void    OnExit(void)
{
   Write_TIME(OFF);
   Write_s(OFF);
   Write_Time_t(TIME_TEND);
   Write_Value_TIME_COUNT_n(&l_u16Timetn[l_u8DrawStep], 1000, 1000);//����ʾ
   
   Write_COUNT(OFF);
   Write_Value_TIME_COUNT_n(&l_u16Count, 1000, 1000);//����ʾ
   
   data.u8SweepMode = SWEEP_NONE;
   
    data.u8SweepStatus = 0;   //�˳�SWEEPģʽ
   l_u8Init = 0;
}

static  void    DoUP1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u8DrawStep < TIME_TEND)
    {
      l_u16Timetn[l_u8DrawStep] ++;
      Draw_Timetn();
    }
    else
    {
      l_u16Count ++;
      Draw_Count();
      if(l_u16Count > 0)
        l_u8CntStatus = 1;  //����ѭ��
      else
        l_u8CntStatus = 0;
    }
  }
}

static  void    DoDOWN1Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u8DrawStep < TIME_TEND)
    {
      l_u16Timetn[l_u8DrawStep] --;
      Draw_Timetn();
    }
    else
    {
      l_u16Count --;
      Draw_Count();
      if(l_u16Count > 0)
        l_u8CntStatus = 1;  //����ѭ��
      else
        l_u8CntStatus = 0;
    }
  }
}

static  void    DoUP2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u8DrawStep < TIME_TEND)
    {
      l_u16Timetn[l_u8DrawStep] += 10;
      Draw_Timetn();
    }
    else
    {
      l_u16Count += 10;
      Draw_Count();
      if(l_u16Count > 0)
        l_u8CntStatus = 1;  //����ѭ��
      else
        l_u8CntStatus = 0;
    }
  }
}

static  void    DoDOWN2Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u8DrawStep < TIME_TEND)
    {
      l_u16Timetn[l_u8DrawStep] -= 10;
      Draw_Timetn();
    }
    else
    {
      l_u16Count -= 10;
      Draw_Count();
      if(l_u16Count > 0)
        l_u8CntStatus = 1;  //����ѭ��
      else
        l_u8CntStatus = 0;
    }
  }
}

static  void    DoUP3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u8DrawStep < TIME_TEND)
    {
      l_u16Timetn[l_u8DrawStep] += 100;
      Draw_Timetn();
    }
    else
    {
      l_u16Count += 100;
      Draw_Count();
      if(l_u16Count > 0)
        l_u8CntStatus = 1;  //����ѭ��
      else
        l_u8CntStatus = 0;
    }
  }
}

static  void    DoDOWN3Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
    if(l_u8DrawStep < TIME_TEND)
    {
      l_u16Timetn[l_u8DrawStep] -= 100;
      Draw_Timetn();
    }
    else
    {
      l_u16Count -= 100;
      Draw_Count();
      if(l_u16Count > 0)
        l_u8CntStatus = 1;  //����ѭ��
      else
        l_u8CntStatus = 0;
    }
  }
}

static  void    DoUP4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
  }
}


static  void    DoDOWN4Key(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS || pKeyMsg->u8KeyAct == KEY_ACT_HOLD)
  {
  }
}

static  void    DoPNKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
  }
}

static  void    DoDotKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
  }
}

static  void    DoSOURCEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    if(l_u8DrawStep >= TIME_TEND)
    {
      data.u8SourceEnable =!data.u8SourceEnable;
      DrawSourceEnable();
      if(l_u16Count > 0)
        l_u8CntStatus = 1;  //����ѭ��
      else
        l_u8CntStatus = 0;
      
      //���
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
        //,�ر�,���
        DoSource(SIGTYPE_END, l_s16SSweepValue);
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
      //,�ر�,���
      DoSource(SIGTYPE_END, l_s16SSweepValue);
    }
  }
}

static  void    DoSFUNCKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {  

  }
}

static  void    DoSRANGEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
  }
}

static  void    DoMEASUREKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    data.u8MeasureEnable =!data.u8MeasureEnable;        
    DrawMeasureEnable();
  }
}

static  void    DoMFUNCKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
    if(data.u8MeasureEnable == ON)
    {
      data.u8MSigType ++;
      if(data.u8MSigType > SIGTYPE_END - 1)
        data.u8MSigType = SIGTYPE_24VLOOP;
      DrawMeasureData();
    }
  }
}

static  void    DoMRANGEKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
  }
}

static  void    DoTEMPKey(PKeyMsg pKeyMsg)
{
    if(pKeyMsg->u8KeyAct == KEY_ACT_RELEA)
    {
    }
}

static  void    DoSWEEPKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
  {
  }
}

static  void    DoPROGKey(PKeyMsg pKeyMsg)
{
  if(pKeyMsg->u8KeyAct == KEY_ACT_PRESS)
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
  }
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
