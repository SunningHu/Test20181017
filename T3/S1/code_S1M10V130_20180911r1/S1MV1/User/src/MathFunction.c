/////////////////////////////////////////////////////////////////////////////////////
// Copyright Pangu Inc. All Rights Reserved.
// FILE:        数学函数
// DESC:        
// AUTHOR:      onepiece120116         
// ARCHIVE:     13/11
// CHANGES:                          
/////////////////////////////////////////////////////////////////////////////////////
#include "MathFunction.h"

//Summary:		计算n次幂
//Parameter:   	
//Return:		
double PRDataLib_ZPow(double f, unsigned short u16Index,double a)
{
    double fData;

    switch(u16Index)
    {
    case 0:
        fData = 1*a;
        break;
    case 1:
        fData = f*a;
        break;
    case 2:
        fData = a*f*f;
        break;
    case 3:
        fData = a*f*f*f;
        break;
    case 4:
        fData = a*f*f*f*f;
        break;
    case 5:
        fData = a*f*f*f*f*f;
        break;
    case 6:
        fData = a*f*f*f*f*f*f;
        break;
    case 7:
        fData = a*f*f*f*f*f*f*f;
        break;
    case 8:
        fData = a*f*f*f*f*f*f*f*f;
        break;
    case 9:
        fData = a*f*f*f*f*f*f*f*f*f;
        break;
    case 10:
        fData = a*f*f*f*f*f*f*f*f*f*f;
        break;
    case 11:
        fData = a*f*f*f*f*f*f*f*f*f*f*f;
        break;
    case 12:
        fData = a*f*f*f*f*f*f*f*f*f*f*f*f;
        break;
    case 13:
        fData = a*f*f*f*f*f*f*f*f*f*f*f*f*f;
        break;
    case 14:
        fData = a*f*f*f*f*f*f*f*f*f*f*f*f*f*f;
        break;
    case 15:
        fData = a*f*f*f*f*f*f*f*f*f*f*f*f*f*f*f;
        break;
    case 16:
        fData = a*f*f*f*f*f*f*f*f*f*f*f*f*f*f*f*f;
        break;
    default:
        fData = 0;
        break;
    }
    
    if(fData > 2E+38)
      fData = 2E+38;

    return fData;
}


#define   NORMAL_POS(u16Tmp, u16Len)   (u16Tmp < u16Len) ? u16Tmp : (u16Len - 1)       	//防止数组访问溢出

/****************************************************************
// Summary: 	非线性表格查表函数，做线性插值	
// Parameter: 	[in/float]fDat 	分度值 
//				[in/float]fMin 	分度值最小值
//				[in/float]fMax  分度值最大值
//				[in/float]fInt	表格间隔值
//				[in/const float*]fTable	表格
//				[in/u16]u16TblLen 表格长度
// 	
// return:		1 成功 0 失败
****************************************************************/		
float SeekTable_T_E(float fT, float fMin, float fMax, 
                float fInt, const float* fTable, unsigned short u16TblLen)
{			
	float	fTValue;
	float	fTValue1;
	float	fTValue2;
    float   fTmp;
	unsigned short  	u16Tmp;
	
	if(fT >= fMax)
	{
		fTValue = fTable[u16TblLen - 1];	                                        //最大值
	}
	else if(fT <= fMin)
	{
		fTValue = fTable[0];	                                                    //最小值
    }
	else                                                                            //根据两点线性计算中间值
	{	
        fTmp = (fT - fMin) / fInt;
	    u16Tmp = (unsigned short)fTmp;                                                       	//根据数值获取表格位置
        fTmp -= u16Tmp;

	    u16Tmp = NORMAL_POS(u16Tmp, u16TblLen);                                     //规整表格位置防止溢出
		fTValue1 = fTable[u16Tmp];
		
	    u16Tmp ++;
	    u16Tmp = NORMAL_POS(u16Tmp, u16TblLen);                                     //计算下个表格位置
		fTValue2 = fTable[u16Tmp];                                                  //规整表格位置防止溢出
		
		fTValue = fTValue1 + (fTValue2 - fTValue1) * fTmp;                          //线性计算精确表值	
	}
	
	return fTValue;
}

