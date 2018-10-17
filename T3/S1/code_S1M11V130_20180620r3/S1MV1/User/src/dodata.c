/*=============================================================================
//File:			dodata.c	
//Author:		tpcon 
//Version:	    V0.1
//Date:			2014/04
//Dese:			���ݴ���
===============================================================================*/

#define     _DODATA_C_

#include "dodata.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_tim1.h"
#include "viewMain.h"
#include "adcmd.h"
#include "gui.h"
#include "TC.h"
#include "doMsg.h"
#include "viewFrame.h"
#include "battery.h"

#define     OUT_V_PORT                  GPIOD
#define     OUT_V_PIN                   GPIO_Pin_6

#define     OUTHAVESRC_PORT             GPIOB
#define     OUTHAVESRC_PIN              GPIO_Pin_7

#define     OUTSIG_V_PORT               GPIOG
#define     OUTSIG_V_PIN                GPIO_Pin_5

#define     OUTSIG_MA_PORT              GPIOB
#define     OUTSIG_MA_PIN               GPIO_Pin_5

#define     OUTSIG_MV_PORT              GPIOB
#define     OUTSIG_MV_PIN               GPIO_Pin_6

#define     OUTSIG_20MV_PORT            GPIOD
#define     OUTSIG_20MV_PIN             GPIO_Pin_4

#define     OUTSIG_VMA_PORT             GPIOB
#define     OUTSIG_VMA_PIN              GPIO_Pin_3

#define     OUTNOTSRC_PORT              GPIOB
#define     OUTNOTSRC_PIN               GPIO_Pin_4

//
#define     AUTOHAND_PORT               GPIOE
#define     AUTOHANG_PIN                GPIO_Pin_6

#define     MASINK_PORT                 GPIOC
#define     MASINK_PIN                  GPIO_Pin_7

//
#define     INSIG_MA_PORT               GPIOC
#define     INSIG_MA_PIN                GPIO_Pin_4

#define     INSIG_20MV_PORT             GPIOC
#define     INSIG_20MV_PIN              GPIO_Pin_6

#define     INSIG_V_PORT                GPIOC
#define     INSIG_V_PIN                 GPIO_Pin_5


//MEASURE�źŷ�Χ
s16  g_s16MSigRange[SIGTYPE_END][3] = 
{
  {   0, 2500, 2},
  {   0, 2500, 2},
  { 400, 2000, 2},
  {   0, 3000, 2},
  {   0, 2500, 2},
  {   0, 1000, 1},
  {-270, 1372, 0},
  {-270, 1000, 0},
  {-210, 1200, 0},
  {-270,  400, 0},
  { -50, 1768, 0},
  {   0, 1820, 0},
  { -50, 1768, 0},
  {-270, 1300, 0},
};

//SOURCE�źŷ�Χ
s16  g_s16SSigRange[SIGTYPE_END][3] = 
{
  {   0, 2500, 2},
  {   0, 2500, 2},
  { 400, 2000, 2},
  {   0, 1500, 2},
  {   0, 2500, 2},
  {   0, 1000, 1},
  {   0, 1372, 0},
  {   0, 1000, 0},
  {   0, 1200, 0},
  {   0,  400, 0},
  {   0, 1768, 0},
  {   0, 1820, 0},
  {   0, 1768, 0},
  {   0, 1300, 0},
};

static  u8  l_u8mAMode = 0; //�������ģʽ����Դ/��Դ
extern  CalPwm  calPwm;

//Summary:      �ϵ��ʼ������
void ResetData(void)
{
  data.s16MeasureValue = 0;
  data.s16MRangeValue = 0;
  data.s16MRH = 0;
  data.s16MRL = 0;
  data.s16MSH = 0;
  data.s16MSL = 0;
  data.s16SourceValue = 0;
  data.s16SRangeValue = 0;
  data.s16SRH = 0;
  data.s16SRL = 0;
  data.s16SSH = 0;
  data.s16SSL = 0;
  data.u8MeasureEnable = ON;
  data.u8MRLHDot = 2;
  data.u8MSigType = SIGTYPE_MA;
  data.u8RJMode = RJMODE_AUTO;
  data.u8SourceEnable = OFF;
  data.u8SRLHDot = 2;
  data.u8SSigType = SIGTYPE_MA;
  data.u8SweepMode = SWEEP_NONE;
  data.u8SweepStatus = 0;
  data.s16RJAuto = 0;
  data.s16RJSet = 0;
  data.u8MRangeEnable = 0;
}

//NTC�ֶȱ�
//-40~100�棬ÿ1��һ��ֵ
const static float Tab_NTC[] = 
{
  339.096, 317.135, 296.739, 277.788, 260.171, 243.785, 228.537, 214.341, 201.118, 188.796,
  177.307, 166.592, 156.592, 147.255, 138.535, 130.386, 122.768, 115.643, 108.976, 102.735,
  96.8904, 91.4148, 86.2826, 81.4703, 76.9562, 72.7200, 68.7430, 65.0079, 61.4985, 58.2000,
  55.0985, 52.1811, 49.4359, 46.8516, 44.4180, 42.1254, 39.9649, 37.9281, 36.0073, 34.1951,
  32.4849, 30.8704, 29.3456, 27.9341, 26.5796, 25.2982, 24.0857, 22.9379, 21.8512, 20.8220,
  19.8469, 18.9227, 18.0467, 17.2160, 16.4280, 15.6804, 14.9708, 14.2972, 13.6575, 13.0499,
  12.4726, 11.9239, 11.4023, 10.9062, 10.4344, 10.0000, 9.5583,  9.1517,  8.7644,  8.3956,  
  8.0443,  7.7095,  7.3904,  7.0861,  6.7960,  6.5192,  6.2552,  6.0032,  5.7626,  5.5330,  
  5.3137,  5.1042,  4.9040,  4.7127,  4.5298,  4.3550,  4.1867,  4.0270,  3.8742,  3.7281,
  3.5882,  3.4543,  3.3261,  3.2033,  3.0858,  2.9731,  2.8651,  2.7617,  2.6624,  2.5673,
  2.4761,  2.3886,  2.3046,  2.2240,  2.1466,  2.0723,  2.0010,  1.9325,  1.8666,  1.8034,
  1.7426,  1.6842,  1.6280,  1.5739,  1.5220,  1.4720,  1.4239,  1.3776,  1.3330,  1.2902,
  1.2489,  1.2091,  1.1708,  1.1338,  1.0992,  1.0640,  1.0309,  0.9991,  0.9684,  0.9351,
  0.9070,  0.8798,  0.8536,  0.8283,  0.8038,  0.7803,  0.7575,  0.7355,  0.7143,  0.6938,
  0.6740,
  
};

//Summary:      ���	
float SeekTable_R_T(float fR, float fMin, float fMax, 
                float fInt, const float* fTable, unsigned short u16TblLen)
{			
	float	fTValue;
	unsigned short  i;
	
    if(fR <= fTable[u16TblLen - 1])
    {
        return fMax;
    }
    else if(fR >= fTable[0])
    {
        return fMin;
    }
    else
    {
        for(i = 0; i < u16TblLen - 1; i++)
        {
            if(fR <= fTable[i] && fR > fTable[i + 1])
                break;
        }
       
        fTValue = (fR - fTable[i]) * fInt / (fTable[i + 1] - fTable[i]) + fInt * i + fMin;
    }
	
	return fTValue;
}

//Summary:      ��������ֵ
static  u16    DoLinear(u16 u16x1, u16 u16x2, float fy1, float fy2, float fData)
{
  u16 u16Value;
  
  if(fy2 == fy1)
    fy2 = fy1 + 1;
  
  u16Value = (u16)((fData - fy1) * (u16x2 - u16x1) / (fy2 - fy1) + u16x1);
    
  return u16Value;
}

//Summary:      ��������ֵ
static  float    DoLinear_Measure(u16 u16x1, u16 u16x2, float fy1, float fy2, u16 u16Data)
{
  float fValue;
  
  if(u16x2 == u16x1)
    u16x2 = u16x1 + 1;
  
  fValue = ((float)u16Data - (float)(u16x1)) * (fy2 - fy1) / (float)(u16x2 - u16x1) + fy1;
    
  return fValue;
}

//Summary:      ��ȡ����Զ�ֵ
s16     GetRJAuto(void)
{
    s16  s16RJAuto;
    float fTmp;
     
    //NTC�¶ȼ��� 
    fTmp = (float)( (4096 - (float)adCode.u16NTCCode)/ adCode.u16NTCCode * 10);   
    fTmp = SeekTable_R_T(fTmp, -40, 100, 1, Tab_NTC, sizeof(Tab_NTC)/ 4);
    
    s16RJAuto = (s16)(fTmp * 10);
    
    return s16RJAuto;
}

//��ȡ���ģʽ�Զ������ֶ�
u8  GetRJMode(void)
{
  u8  u8AutoHand;
    
  u8AutoHand = GPIO_ReadInputDataBit(AUTOHAND_PORT, AUTOHANG_PIN);
  
  if(u8AutoHand == 0)   //�Զ� 
    return  RJMODE_AUTO;
  else
    return  RJMODE_MAN;
}

//��ȡmA��Դ������Դ
u8  GetmASinkMode(void)
{
  u8  u8mASink;
    
  u8mASink = GPIO_ReadInputDataBit(MASINK_PORT, MASINK_PIN);
  
  if(u8mASink == 0)   //��Դ
    return  MAMODE_MA;
  else
    return  MAMODE_SINK;
}

//Summary:      ��ȡ����¶�ֵ
s16     GetRJTemp(void)
{
    s16     s16RJTemp;
    
    if(data.u8RJMode == RJMODE_AUTO)
      s16RJTemp = data.s16RJAuto;
    else
      s16RJTemp = data.s16RJSet;
    
    return s16RJTemp;
}

//���߳�ʼ��
void    Dodata_PortInit(void)
{
    GPIO_Init(OUT_V_PORT, OUT_V_PIN, GPIO_Mode_Out_PP_High_Slow);
  
    GPIO_Init(OUTHAVESRC_PORT, OUTHAVESRC_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(OUTSIG_V_PORT, OUTSIG_V_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(OUTSIG_MA_PORT, OUTSIG_MA_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(OUTSIG_MV_PORT, OUTSIG_MV_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(OUTNOTSRC_PORT, OUTNOTSRC_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(MASINK_PORT, MASINK_PIN, GPIO_Mode_In_PU_No_IT);
    
    GPIO_Init(INSIG_MA_PORT, INSIG_MA_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(INSIG_20MV_PORT, INSIG_20MV_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(INSIG_V_PORT, INSIG_V_PIN, GPIO_Mode_Out_PP_Low_Slow);
    
    GPIO_Init(AUTOHAND_PORT, AUTOHANG_PIN, GPIO_Mode_In_PU_No_IT);
}

//Summary�� �����źſ����л�
void    SetMSigPort(u8 u8SigType)
{
    switch(u8SigType)
    {
      case SIGTYPE_24VLOOP:
        GPIO_SetBits(INSIG_MA_PORT, INSIG_MA_PIN);
        GPIO_ResetBits(INSIG_20MV_PORT, INSIG_20MV_PIN);
        GPIO_ResetBits(INSIG_V_PORT, INSIG_V_PIN);
        
        //������л�Ϊ25mA���,��������̶ܹ������л�
        data.u8SourceEnable = ON;
        data.u8SSigType = SIGTYPE_MA;
        data.s16SourceValue = 2500;
        DoSource(data.u8SSigType, data.s16SourceValue);
        DrawSourceData();
        DrawSourceEnable();
        break;
      case SIGTYPE_MA:
      case SIGTYPE_4_20MA:
        GPIO_SetBits(INSIG_MA_PORT, INSIG_MA_PIN);
        GPIO_ResetBits(INSIG_20MV_PORT, INSIG_20MV_PIN);
        GPIO_ResetBits(INSIG_V_PORT, INSIG_V_PIN);
        break;
      case SIGTYPE_20MV:
      case SIGTYPE_T:
      case SIGTYPE_R:
      case SIGTYPE_B:
      case SIGTYPE_S:                            
        GPIO_ResetBits(INSIG_MA_PORT, INSIG_MA_PIN);
        GPIO_ResetBits(INSIG_20MV_PORT, INSIG_20MV_PIN);
        GPIO_ResetBits(INSIG_V_PORT, INSIG_V_PIN);
        break;
      case SIGTYPE_100MV:
      case SIGTYPE_K:
      case SIGTYPE_E:
      case SIGTYPE_J:
      case SIGTYPE_N:                            
        GPIO_ResetBits(INSIG_MA_PORT, INSIG_MA_PIN);
        GPIO_SetBits(INSIG_20MV_PORT, INSIG_20MV_PIN);
        GPIO_ResetBits(INSIG_V_PORT, INSIG_V_PIN);
        break;
      case SIGTYPE_V:                            
        GPIO_ResetBits(INSIG_MA_PORT, INSIG_MA_PIN);
        GPIO_ResetBits(INSIG_20MV_PORT, INSIG_20MV_PIN);
        GPIO_SetBits(INSIG_V_PORT, INSIG_V_PIN);
        break;
      default:                            
        GPIO_ResetBits(INSIG_MA_PORT, INSIG_MA_PIN);
        GPIO_ResetBits(INSIG_20MV_PORT, INSIG_20MV_PIN);
        GPIO_ResetBits(INSIG_V_PORT, INSIG_V_PIN);
        break;
    }
}

//Summary:  ����,�õ�ʵ���ź�ֵ
s16    DoMeasure(u8 u8SigType)
{
    s16 s16MValue = 0;
    float fTmp;
  
    //�л��źſ���
    SetMSigPort(u8SigType);
    
    switch(u8SigType)
    {
      case SIGTYPE_24VLOOP:
        fTmp = DoLinear_Measure(calPwm.u16MCal_mA[0], calPwm.u16MCal_mA[1], 2, 21, adCode.u16mACode);
        s16MValue = (s16)(fTmp * 100);
        break;
      case SIGTYPE_MA:
        fTmp = DoLinear_Measure(calPwm.u16MCal_mA[0], calPwm.u16MCal_mA[1], 2, 21, adCode.u16mACode);
        s16MValue = (s16)(fTmp * 100);
        break;
      case SIGTYPE_4_20MA:
        fTmp = DoLinear_Measure(calPwm.u16MCal_mA[0], calPwm.u16MCal_mA[1], 2, 21, adCode.u16mACode);
        s16MValue = (s16)(fTmp * 100);
        break;
      case SIGTYPE_V:
        fTmp = DoLinear_Measure(calPwm.u16MCal_V[0], calPwm.u16MCal_V[1], 2, 10, adCode.u16VCode);
        s16MValue = (s16)(fTmp * 100);
        break;
      case SIGTYPE_20MV:
        fTmp = DoLinear_Measure(calPwm.u16MCal_20mV[0], calPwm.u16MCal_20mV[1], 2, 21, adCode.u16mVCode);
        s16MValue = (s16)(fTmp * 100);
        break;
      case SIGTYPE_100MV:
        fTmp = DoLinear_Measure(calPwm.u16MCal_100mV[0], calPwm.u16MCal_100mV[1], 2, 100, adCode.u16mVCode);
        s16MValue = (s16)(fTmp * 10);
        break;
      case SIGTYPE_K:
        fTmp = DoLinear_Measure(calPwm.u16MCal_100mV[0], calPwm.u16MCal_100mV[1], 2, 100, adCode.u16mVCode);
        fTmp += K_T2E(GetRJTemp() / 10);
		fTmp = K_E2T(fTmp) ;
        s16MValue = (s16)fTmp;
        break;
      case SIGTYPE_E:
        fTmp = DoLinear_Measure(calPwm.u16MCal_100mV[0], calPwm.u16MCal_100mV[1], 2, 100, adCode.u16mVCode);
        fTmp += E_T2E(GetRJTemp() / 10);
		fTmp = E_E2T(fTmp) ;
        s16MValue = (s16)fTmp;
        break;
      case SIGTYPE_J:
        fTmp = DoLinear_Measure(calPwm.u16MCal_100mV[0], calPwm.u16MCal_100mV[1], 2, 100, adCode.u16mVCode);
        fTmp += J_T2E(GetRJTemp() / 10);
		fTmp = J_E2T(fTmp) ;
        s16MValue = (s16)fTmp;
        break;
      case SIGTYPE_T:
        fTmp = DoLinear_Measure(calPwm.u16MCal_20mV[0], calPwm.u16MCal_20mV[1], 2, 21, adCode.u16mVCode);
        fTmp += T_T2E(GetRJTemp() / 10);
		fTmp = T_E2T(fTmp) ;
        s16MValue = (s16)fTmp;
        break;
      case SIGTYPE_R:
        fTmp = DoLinear_Measure(calPwm.u16MCal_20mV[0], calPwm.u16MCal_20mV[1], 2, 21, adCode.u16mVCode);
        fTmp += R_T2E(GetRJTemp() / 10);
		fTmp = R_E2T(fTmp) ;
        s16MValue = (s16)fTmp;
        break;
      case SIGTYPE_B:
        fTmp = DoLinear_Measure(calPwm.u16MCal_20mV[0], calPwm.u16MCal_20mV[1], 2, 21, adCode.u16mVCode);
		fTmp = B_E2T(fTmp) ;
        s16MValue = (s16)fTmp;
        break;
      case SIGTYPE_S:
        fTmp = DoLinear_Measure(calPwm.u16MCal_20mV[0], calPwm.u16MCal_20mV[1], 2, 21, adCode.u16mVCode);
        fTmp += S_T2E(GetRJTemp() / 10);
		fTmp = S_E2T(fTmp) ;
        s16MValue = (s16)fTmp;
        break;
      case SIGTYPE_N:
        fTmp = DoLinear_Measure(calPwm.u16MCal_100mV[0], calPwm.u16MCal_100mV[1], 2, 100, adCode.u16mVCode);
        fTmp += N_T2E(GetRJTemp() / 10);
		fTmp = N_E2T(fTmp) ;
        s16MValue = (s16)fTmp;
        break;
      default:
          s16MValue = 0;
        break;
    }
    
    
    return s16MValue;
}

//Summary:  ����궨
void     DoSourceCali(u8 u8CaliSig, u16* pu16CalPwm)
{
  TIM1_SetCompare3(*pu16CalPwm);
  switch(u8CaliSig)
    {
      case SCALSIG_MA:   //��Դ20mA
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUT_V_PORT, OUT_V_PIN);
        break;
      case SCALSIG_MASINK: //��Դ20mA
        GPIO_ResetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_ResetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_SetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUT_V_PORT, OUT_V_PIN);
        break;
      case SCALSIG_20MV:   //20mV
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_ResetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_SetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_SetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUT_V_PORT, OUT_V_PIN);
        break;
      case SCALSIG_100MV: //100mV
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_ResetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_SetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUT_V_PORT, OUT_V_PIN);
        break;
      case SCALSIG_V:// //V
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_ResetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_SetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_ResetBits(OUT_V_PORT, OUT_V_PIN);
        break;
      default:
        break;
    }
}

//Summary�� ����źſ����л�
void    SetSSigPort(u8 u8SigType)
{
    switch(u8SigType)
    {
      case SIGTYPE_MA:
      case SIGTYPE_4_20MA:
        if(data.u8RemoteFlag == 0)
        {
            //�ж���Դ������Դ
            if(l_u8mAMode == MAMODE_MA) //��Դ
            {          
              GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
              GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
              GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
              GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
              GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
              GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
              GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
              GPIO_SetBits(OUT_V_PORT, OUT_V_PIN);
            }
            else    //��Դ
            {                            
              GPIO_ResetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
              GPIO_ResetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
              GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
              GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
              GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
              GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
              GPIO_SetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
              GPIO_SetBits(OUT_V_PORT, OUT_V_PIN);
            }          
        }
        else
        {
            //�ж���Դ������Դ
            if(data.u8SinkFlag == MAMODE_MA) //��Դ
            {          
              GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
              GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
              GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
              GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
              GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
              GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
              GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
              GPIO_SetBits(OUT_V_PORT, OUT_V_PIN);
            }
            else    //��Դ
            {                            
              GPIO_ResetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
              GPIO_ResetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
              GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
              GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
              GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
              GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
              GPIO_SetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
              GPIO_SetBits(OUT_V_PORT, OUT_V_PIN);
            }          
        }

        break;
      case SIGTYPE_20MV:
      case SIGTYPE_T:
      case SIGTYPE_R:
      case SIGTYPE_B:
      case SIGTYPE_S:                            
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_ResetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_SetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_SetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUT_V_PORT, OUT_V_PIN);
        break;
      case SIGTYPE_100MV:
      case SIGTYPE_K:
      case SIGTYPE_E:
      case SIGTYPE_J:
      case SIGTYPE_N:                            
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_ResetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_SetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUT_V_PORT, OUT_V_PIN);
        break;
      case SIGTYPE_V:                            
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_ResetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_SetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_ResetBits(OUT_V_PORT, OUT_V_PIN);
        break;
      default:                            
        GPIO_ResetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_ResetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        
        GPIO_SetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUT_V_PORT, OUT_V_PIN);
        break;
    }
}

//Summary�� ����ź�
void    DoSource(u8 u8SigType, s16 s16SourceValue)
{
    u16     u16PWM;
    float   fData;
    
    
    //�л�����źſ���
    SetSSigPort(u8SigType);
    //����PWMֵ,������PWMռ�ձ�
    switch(u8SigType)
    {
      case SIGTYPE_MA:
      case SIGTYPE_4_20MA:
        if(data.u8RemoteFlag == 0)
        {        
          //�ж���Դ������Դ
          if(l_u8mAMode == MAMODE_MA) //��Դ
          {
            u16PWM = DoLinear(calPwm.u16SCalPwm_mA[0], calPwm.u16SCalPwm_mA[1],
                              calPwm.fSCalPwm_mA[0], calPwm.fSCalPwm_mA[1], (float)s16SourceValue /100);
          }
          else    //��Դ
          {
            u16PWM = DoLinear(calPwm.u16SCalPwm_mASink[0], calPwm.u16SCalPwm_mASink[1],
                              calPwm.fSCalPwm_mASink[0], calPwm.fSCalPwm_mASink[1], (float)s16SourceValue/100);
          }
        }
        else
        {
          if(data.u8SinkFlag == MAMODE_MA)
          {     
            u16PWM = DoLinear(calPwm.u16SCalPwm_mA[0], calPwm.u16SCalPwm_mA[1],
                              calPwm.fSCalPwm_mA[0], calPwm.fSCalPwm_mA[1], (float)s16SourceValue /100);
          }
          else
          {
             u16PWM = DoLinear(calPwm.u16SCalPwm_mASink[0], calPwm.u16SCalPwm_mASink[1],
                              calPwm.fSCalPwm_mASink[0], calPwm.fSCalPwm_mASink[1], (float)s16SourceValue/100);            
          }
        }
        break;
      case SIGTYPE_20MV:
        u16PWM = DoLinear(calPwm.u16SCalPwm_20mV[0], calPwm.u16SCalPwm_20mV[1],
                            calPwm.fSCalPwm_20mV[0], calPwm.fSCalPwm_20mV[1], (float)s16SourceValue/100);
         break;
      case SIGTYPE_100MV:
        u16PWM = DoLinear(calPwm.u16SCalPwm_100mV[0], calPwm.u16SCalPwm_100mV[1],
                            calPwm.fSCalPwm_100mV[0], calPwm.fSCalPwm_100mV[1], (float)s16SourceValue/10);
        break;
      case SIGTYPE_T:
        fData = T_T2E(s16SourceValue) - T_T2E((float)GetRJTemp() / 10); //�¶�ת��ΪmVֵ�����Ϊ0��
        u16PWM = DoLinear(calPwm.u16SCalPwm_20mV[0], calPwm.u16SCalPwm_20mV[1],
                            calPwm.fSCalPwm_20mV[0], calPwm.fSCalPwm_20mV[1], fData);
        break;
      case SIGTYPE_R:
        fData = R_T2E(s16SourceValue) - R_T2E((float)GetRJTemp() / 10); //�¶�ת��ΪmVֵ
        u16PWM = DoLinear(calPwm.u16SCalPwm_20mV[0], calPwm.u16SCalPwm_20mV[1],
                            calPwm.fSCalPwm_20mV[0], calPwm.fSCalPwm_20mV[1], fData);
         break;
      case SIGTYPE_B:
        fData = B_T2E(s16SourceValue); //�¶�ת��ΪmVֵ
        u16PWM = DoLinear(calPwm.u16SCalPwm_20mV[0], calPwm.u16SCalPwm_20mV[1],
                            calPwm.fSCalPwm_20mV[0], calPwm.fSCalPwm_20mV[1], fData);
         break;
      case SIGTYPE_S:
        fData = S_T2E(s16SourceValue) - S_T2E((float)GetRJTemp() / 10); //�¶�ת��ΪmVֵ
        u16PWM = DoLinear(calPwm.u16SCalPwm_20mV[0], calPwm.u16SCalPwm_20mV[1],
                            calPwm.fSCalPwm_20mV[0], calPwm.fSCalPwm_20mV[1], fData);
        break;      
      case SIGTYPE_K:
        fData = K_T2E(s16SourceValue) - K_T2E((float)GetRJTemp() / 10); //�¶�ת��ΪmVֵ
        
        u16PWM = DoLinear(calPwm.u16SCalPwm_100mV[0], calPwm.u16SCalPwm_100mV[1],
                            calPwm.fSCalPwm_100mV[0], calPwm.fSCalPwm_100mV[1], fData);
        break;
      case SIGTYPE_E:
        fData = E_T2E(s16SourceValue) - E_T2E((float)GetRJTemp() / 10); //�¶�ת��ΪmVֵ
        u16PWM = DoLinear(calPwm.u16SCalPwm_100mV[0], calPwm.u16SCalPwm_100mV[1],
                            calPwm.fSCalPwm_100mV[0], calPwm.fSCalPwm_100mV[1], fData);
        break;
      case SIGTYPE_J:
        fData = J_T2E(s16SourceValue) - J_T2E((float)GetRJTemp() / 10); //�¶�ת��ΪmVֵ
        u16PWM = DoLinear(calPwm.u16SCalPwm_100mV[0], calPwm.u16SCalPwm_100mV[1],
                            calPwm.fSCalPwm_100mV[0], calPwm.fSCalPwm_100mV[1], fData);
        break;
      case SIGTYPE_N:
        fData = N_T2E(s16SourceValue) - N_T2E((float)GetRJTemp() / 10); //�¶�ת��ΪmVֵ
        u16PWM = DoLinear(calPwm.u16SCalPwm_100mV[0], calPwm.u16SCalPwm_100mV[1],
                            calPwm.fSCalPwm_100mV[0], calPwm.fSCalPwm_100mV[1], fData);
        break;
     case SIGTYPE_V:
        u16PWM = DoLinear(calPwm.u16SCalPwm_V[0], calPwm.u16SCalPwm_V[1],
                            calPwm.fSCalPwm_V[0], calPwm.fSCalPwm_V[1], (float)s16SourceValue/100);
        break;
      default:
        //ȫ�ر�
        u16PWM = 0;
        break;
    }
    TIM1_SetCompare3(u16PWM);
}

//Summary:  AD���ݲ�������
void    Dodata_ADSample(void)
{
  u8  u8Msg;
  
  u8Msg = GetMsg(MSG_ADOK, 1); 
  if(u8Msg != 0) 
  {
    data.s16RJAuto = GetRJAuto();
    if(data.u8MeasureEnable == ON) //�������� 
    {
      data.s16MeasureValue = DoMeasure(data.u8MSigType);
    }
    //��ʾ��ص���
    ViewBatteryLevel(adCode.u16BatteryCode);
    ClearMsg(MSG_ADOK);       
  }
   
}

//Summary:  ÿ�����ݴ���
void    Dodata_1S(void)
{  
   data.u8RJMode = GetRJMode();
   l_u8mAMode = GetmASinkMode(); 
   Dodata_ADSample();
}