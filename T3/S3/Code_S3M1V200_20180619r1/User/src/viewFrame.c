/*=============================================================================
//File:			viewFrame.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04
//Dese:			������
===============================================================================*/
#include "viewFrame.h"
#include "domsg.h"
#include "viewMain.h"
#include "viewSRange.h"
#include "viewSRSLH.h"
#include "viewMRSLH.h"
#include "viewRJTC.h"
#include "viewPrognm.h"
#include "viewSweep.h"
#include "viewStep.h"
#include "viewRemote.h"
#include "dodata.h"
#include "gui.h"
#include "battery.h"
#include "lcd.h"
#include "measure.h"
#include "stm8l15x_adc.h"
#include "timer.h"

//����汾��
static   s32     l_s32Serial = 53;  //S3
static   s32     l_s32Version = 200;  //

static  u8  l_u8CurView = 0;




/********************************************************************
* Summary:    ������
* Parameters:
*             l_u8CurView   ��������
*       
* Return :    void
********************************************************************/
void    ViewFrame(PKeyMsg pKeyMsg)
{
  switch(l_u8CurView)
  {
    case VIEW_MAIN:
      ViewMain(pKeyMsg);
      break;
    case VIEW_SRANGE:
      ViewSRange(pKeyMsg);
      break;
    case VIEW_SRSLH:
      ViewSRSLH(pKeyMsg);
      break;
    case VIEW_MRSLH:
      ViewMRSLH(pKeyMsg);
      break;
    case VIEW_RJTC:
      ViewRJTC(pKeyMsg);
      break;
    case VIEW_PROGNM:
      ViewPrognm(pKeyMsg);
      break;
    case VIEW_SWEEP:
      ViewSweep(pKeyMsg);
      break;
    case VIEW_STEP:
      ViewStep(pKeyMsg);
      break;
    case VIEW_REMOTE:
      ViewRemote(pKeyMsg);
      break;      
    default:
      break;
  }
}


/********************************************************************
* Summary:    �����л�
* Parameters:
*             u8CurView  ҳ��
*       
* Return :    void
********************************************************************/
void    SetView(u8  u8CurView)
{
  l_u8CurView = u8CurView;
  PostMsg(MSG_PAINT, 1);
}

/*******************************************************/
//
//Summary:	 �õ���������
//Parameter:     void
//Return:        void
//
/*******************************************************/ 
u8    GetView(void)
{
  return l_u8CurView;
}


/********************************************************************
* Summary:    ��������
* Parameters:
*             void
*       
* Return :    void
********************************************************************/
void    Init_ViewFrame(void)
{
  //��ʾ��������
  LCDClr();

  //��ʾ�汾��  S3 V1.00
  Write_Value_MEASURE(&l_s32Serial, 0, 0, 1000);
  Write_Value_SOURCE(&l_s32Version, 0, 0, 1000); 
  
  delay_ms(1500); 
  LCDClr();


  l_u8CurView = 0;
  
  //�����л�
  SetView(l_u8CurView);

  Write_Line(ON);
  
  PostMsg(MSG_PAINT, 1);
  PostMsg(MSG_1S, 1);
}