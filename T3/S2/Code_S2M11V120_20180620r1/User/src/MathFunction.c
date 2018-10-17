/////////////////////////////////////////////////////////////////////////////////////
// Copyright Pangu Inc. All Rights Reserved.
// FILE:        ��ѧ����
// DESC:        
// AUTHOR:      onepiece120116         
// ARCHIVE:     13/11
// CHANGES:                          
/////////////////////////////////////////////////////////////////////////////////////
#include "MathFunction.h"

//Summary:		����n����
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


#define   NORMAL_POS(u16Tmp, u16Len)   (u16Tmp < u16Len) ? u16Tmp : (u16Len - 1)       	//��ֹ����������

/****************************************************************
// Summary: 	�����Ա�������������Բ�ֵ	
// Parameter: 	[in/float]fDat 	�ֶ�ֵ 
//				[in/float]fMin 	�ֶ�ֵ��Сֵ
//				[in/float]fMax  �ֶ�ֵ���ֵ
//				[in/float]fInt	�����ֵ
//				[in/const float*]fTable	���
//				[in/u16]u16TblLen ��񳤶�
// 	
// return:		1 �ɹ� 0 ʧ��
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
		fTValue = fTable[u16TblLen - 1];	                                        //���ֵ
	}
	else if(fT <= fMin)
	{
		fTValue = fTable[0];	                                                    //��Сֵ
    }
	else                                                                            //�����������Լ����м�ֵ
	{	
        fTmp = (fT - fMin) / fInt;
	    u16Tmp = (unsigned short)fTmp;                                                       	//������ֵ��ȡ���λ��
        fTmp -= u16Tmp;

	    u16Tmp = NORMAL_POS(u16Tmp, u16TblLen);                                     //�������λ�÷�ֹ���
		fTValue1 = fTable[u16Tmp];
		
	    u16Tmp ++;
	    u16Tmp = NORMAL_POS(u16Tmp, u16TblLen);                                     //�����¸����λ��
		fTValue2 = fTable[u16Tmp];                                                  //�������λ�÷�ֹ���
		
		fTValue = fTValue1 + (fTValue2 - fTValue1) * fTmp;                          //���Լ��㾫ȷ��ֵ	
	}
	
	return fTValue;
}

