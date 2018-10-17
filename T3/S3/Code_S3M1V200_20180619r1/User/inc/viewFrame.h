/*=============================================================================
//File:			viewFrame.h	
//Author:		tpcon 
//Version:	    V0.1
//Date:			2014/04
//Dese:			画面框架头文件
===============================================================================*/

#ifndef __VIEWFRAME_H
#define __VIEWFRAME_H

#include	"stm8l15x.h"
#include    "key.h"

#define     VIEW_MAIN           0
#define     VIEW_SRANGE         1
#define     VIEW_RJTC           2
#define     VIEW_RJVIEW         3
#define     VIEW_PROGNM         4
#define     VIEW_SRSLH          5
#define     VIEW_MRSLH          6
#define     VIEW_STEP           7
#define     VIEW_SWEEP          8
#define     VIEW_REMOTE         9

void    ViewFrame(PKeyMsg pKeyMsg);
void    SetView(u8  u8CurView);
void    Init_ViewFrame(void);
u8      GetView(void);


#endif
