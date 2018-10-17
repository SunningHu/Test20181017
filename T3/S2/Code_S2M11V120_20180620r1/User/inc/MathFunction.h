/////////////////////////////////////////////////////////////////////////////////////
// Copyright Pangu Inc. All Rights Reserved.
// FILE:        Math.h
// DESC:        数学函数头文件
// AUTHOR:      onepiece120116          
// ARCHIVE:     13/11
// CHANGES:                          
/////////////////////////////////////////////////////////////////////////////////////

#ifndef _MATHFUNCTION_H_
#define _MATHFUNCTION_H_

#define INT2FLOAT(nData,ucDot)  ((double)nData / PRDataLib_IPow(ucDot))           // 整形转浮点数
double  PRDataLib_IPow(short sDot) ; 
double PRDataLib_ZPow(double f, unsigned short u16Index,double a);
/*
float SeekTable_E_T(float fE, float fMin, float fMax, 
                float fInt, const float* fTable, unsigned short u16TblLen);
float SeekTable_T_E(float fT, float fMin, float fMax, 
                float fInt, const float* fTable, unsigned short u16TblLen);
float   CToF(float fC);
float   FToC(float fF);
float	DoAverage(unsigned char N , unsigned long* ulBuf, unsigned long ulInData);*/
#endif
