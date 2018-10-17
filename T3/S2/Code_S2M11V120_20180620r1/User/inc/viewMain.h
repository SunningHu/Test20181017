/*=============================================================================
//File:			viewMain.h	
//Author:		tpcon  
//Version:	    V0.1
//Date:			2014/04
//Dese:			主画面及操作
===============================================================================*/

#ifndef __VIEWMAIN_H
#define __VIEWMAIN_H

#include	"stm8l15x.h"
#include    "key.h"



void    ViewMain(PKeyMsg pKeyMsg);
void    DrawMeasureDataRealTime(void);
void    DrawSourceEnable(void);
void    DrawMeasureEnable(void);
void    DrawMeasureData(void);
s16     GetSourceValue(void);
void    DrawSweepMode(void);
void    DrawSourceData(void);
void    DrawMeasureValueRealTime(void);
void    DrawMRangeValueRealTime(void);

#endif
