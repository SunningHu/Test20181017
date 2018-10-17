#define     _DODATA_C_
#include "dodata.h"
#include "stm8l15x_gpio.h"
#include "viewMain.h"
#include "adcmd.h"
#include "gui.h"
#include "TC.h"
#include "doMsg.h"
#include "viewFrame.h"
#include "battery.h"
#include "dac8552.h"

extern void Write_DAC8552(uint8_t Command, uint16_t Resdata);


#define     OUTHAVESRC_PORT             GPIOF
#define     OUTHAVESRC_PIN              GPIO_Pin_6

#define     OUTSIG_V_PORT               GPIOF
#define     OUTSIG_V_PIN                GPIO_Pin_7

#define     OUTSIG_VIO_PORT               GPIOG
#define     OUTSIG_VIO_PIN                GPIO_Pin_7

#define     OUTSIG_MA_PORT              GPIOF
#define     OUTSIG_MA_PIN               GPIO_Pin_4

#define     OUTSIG_MV_PORT              GPIOF
#define     OUTSIG_MV_PIN               GPIO_Pin_5

#define     OUTSIG_20MV_PORT            GPIOB
#define     OUTSIG_20MV_PIN             GPIO_Pin_7

#define     OUTSIG_VMA_PORT             GPIOF
#define     OUTSIG_VMA_PIN              GPIO_Pin_0

#define     OUTNOTSRC_PORT              GPIOF
#define     OUTNOTSRC_PIN               GPIO_Pin_1

//
#define     AUTOHAND_PORT               GPIOE
#define     AUTOHANG_PIN                GPIO_Pin_6

#define     MASINK_PORT                 GPIOC
#define     MASINK_PIN                  GPIO_Pin_7

//
#define     INSIG_MA_PORT               GPIOC
#define     INSIG_MA_PIN                GPIO_Pin_6

#define     INSIG_20MV_PORT             GPIOC
#define     INSIG_20MV_PIN              GPIO_Pin_4

#define     INSIG_V_PORT                GPIOC
#define     INSIG_V_PIN                 GPIO_Pin_5

//
#define     OUTSIG_R_PORT               GPIOB
#define     OUTSIG_R_PIN                GPIO_Pin_5

#define     OUTSIG_O5_PORT              GPIOB
#define     OUTSIG_O5_PIN               GPIO_Pin_6

#define     OUTSIG_O1_PORT              GPIOE
#define     OUTSIG_O1_PIN               GPIO_Pin_0

#define     OUTSIG_O2_PORT              GPIOG
#define     OUTSIG_O2_PIN               GPIO_Pin_1

#define     OUTSIG_O3_PORT              GPIOG
#define     OUTSIG_O3_PIN               GPIO_Pin_2

#define     OUTSIG_SIG_PORT             GPIOB
#define     OUTSIG_SIG_PIN              GPIO_Pin_4

//
#define     INSIG_O_PORT                GPIOD
#define     INSIG_O_PIN                 GPIO_Pin_4






//MEASURE信号范围
s16  g_s16MSigRange[SIGTYPE_END][3] = 
{
  {   0,  2500, 2},
  {   0,  2500, 2},
  { 400,  2000, 2},
  {   0,  3000, 2},
  {   0 , 2500, 2},
  {   0,  1000, 1},
  {-270,  1372, 0},
  {-270,  1000, 0},
  {-210,  1200, 0},
  {-270,   400, 0},
  { -50,  1768, 0},
  {   0,  1820, 0},
  { -50,  1768, 0},
  {-270,  1300, 0},
  {   0,  4020, 1},
  { -1990,6500, 1},
};

//SOURCE信号范围
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
  { 150, 4000, 1},
  {-1990,6500, 1}, 
};

static  u8  l_u8mAMode = 0; //输出电流模式，有源/无源
extern  CalPwm  calPwm;
s16 FlagI = 0;

//Summary:      上电初始化参数
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

//NTC分度表
//-40~100℃，每1℃一个值
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
//pt100分度表
const static float Tab_BO[] = {
 18.95,  19.38,  19.82,  20.25,  20.68,  21.11,  21.54,  21.97,  22.40,  22.83,
 23.25,  23.68,  24.11,  24.54,  24.97,  25.39,  25.82,  26.24,  26.67,  27.10,
 27.52,  27.95,  28.37,  28.80,  29.22,  29.64,  30.07,  30.49,  30.91,  31.34,
 31.76,  32.18,  32.60,  33.02,  33.44,  33.86,  34.28,  34.70,  35.12,  35.54,
 35.96,  36.38,  36.80,  37.22,  37.64,  38.05,  38.47,  38.89,  39.31,  39.72,
 40.14,  40.56,  40.97,  41.39,  41.80,  42.22,  42.63,  43.05,  43.46,  43.88,
 44.29,  44.70,  45.12,  45.53,  45.94,  46.36,  46.77,  47.18,  47.59,	 48.00,
 48.42,	 48.83,  49.24,  49.65,  50.06,  50.47,  50.88,  51.29,  51.70,	 52.11,
 52.52,  52.93,  53.34,  53.75,  54.15,  54.56,  54.97,  55.38,  55.79,	 56.19,
 56.60,  57.01,  57.41,  57.82,  58.23,  58.63,  59.04,  59.44,  59.85,	 60.26,
 60.66,  61.07,  61.47,  61.88,  62.28,  62.68,  63.09,  63.49,  63.90,	 64.30,
 64.70,	 65.11,  65.51,  65.91,  66.31,  66.72,  67.12,  67.52,  67.92,  68.33,
 68.73,	 69.13,  69.53,  69.93,  70.33,  70.73,  71.13,  71.53,  71.93,	 72.33,
 72.73,	 73.13,  73.53,  73.93,  74.33,  74.73,  75.13,  75.53,  75.93,  76.33,
 76.73,  77.12,  77.52,  77.92,  78.32,  78.72,  79.11,  79.51,  79.91,	 80.31,
 80.70,	 81.10,  81.50,  81.89,  82.29,  82.69,  83.08,  83.48,  83.87,	 84.27,
 84.67,	 85.06,  85.46,  85.85,  86.25,  86.64,  87.04,  87.43,  87.83,	 88.22,
 88.62,	 89.01,  89.40,  89.80,  90.19,  90.59,  90.98,  91.37,  91.77,	 92.16,
 92.55,	 92.95,  93.34,  93.73,  94.12,  94.52,  94.91,  95.30,  95.69,  96.09,
 96.48,	 96.87,  97.26,  97.65,  98.04,  98.44,  98.83,  99.22,  99.61,
 100.00, 100.39, 100.78, 101.17, 101.56, 101.95, 102.34, 102.73, 103.12, 103.51,
 103.90, 104.29, 104.68, 105.07, 105.46, 105.85, 106.24, 106.63, 107.02, 107.40,
 107.79, 108.18, 108.57, 108.96, 109.35, 109.73, 110.12, 110.51, 110.90, 111.29,
 111.67, 112.06, 112.45, 112.83, 113.22, 113.61, 114.00, 114.38, 114.77, 115.15,
 115.54, 115.93, 116.31, 116.70, 117.08, 117.47, 117.86, 118.24, 118.63, 119.01,
 119.40, 119.78, 120.17, 120.55, 120.94, 121.32, 121.71, 122.09, 122.47, 122.86,
 123.24, 123.63, 124.01, 124.39, 124.78, 125.16, 125.54, 125.93, 126.31, 126.69,
 127.08, 127.46, 127.84, 128.22, 128.61, 128.99, 129.37, 129.75, 130.13, 130.52,
 130.90, 131.28, 131.66, 132.04, 132.42, 132.80, 133.18, 133.57, 133.95, 134.33,
 134.71, 135.09, 135.47, 135.85, 136.23, 136.61, 136.99, 137.37, 137.75, 138.13,
 138.51, 138.88, 139.26, 139.64, 140.02, 140.40, 140.78, 141.16, 141.54, 141.91,
 142.29, 142.67, 143.05, 143.43, 143.80, 144.18, 144.56, 144.94, 145.31, 145.69,
 146.07, 146.44, 146.82, 147.20, 147.57, 147.95, 148.33, 148.70, 149.08, 149.46,
 149.83, 150.21, 150.58, 150.96, 151.33, 151.71, 152.08, 152.46, 152.83, 153.21,
 153.58, 153.96, 154.33, 154.71, 155.08, 155.46, 155.83, 156.20, 156.58, 156.95,
 157.33, 157.70, 158.07, 158.45, 158.82, 159.19, 159.56, 159.94, 160.31, 160.68,
 161.05, 161.43, 161.80, 162.17, 162.54, 162.91, 163.29, 163.66, 164.03, 164.40,
 164.77, 165.14, 165.51, 165.89, 166.26, 166.63, 167.00, 167.37, 167.74, 168.11,
 168.48, 168.85, 169.22, 169.59, 169.96, 170.33, 170.70, 171.07, 171.43, 171.80,
 172.17, 172.54, 172.91, 173.28, 173.65, 174.02, 174.38, 174.75, 175.12, 175.49,
 175.86, 176.22, 176.59, 176.96, 177.33, 177.69, 178.06, 178.43, 178.79, 179.16,
179.53,	179.89,	180.26,	180.63,	180.99,	181.36,	181.72,	182.09,	182.46,	182.82,
183.19,	183.55,	183.92,	184.28,	184.65,	185.01,	185.38,	185.74,	186.11,	186.47,
186.84,	187.20,	187.56,	187.93,	188.29,	188.66,	189.02,	189.38,	189.75,	190.11,
190.47,	190.84,	191.20,	191.56,	191.92,	192.29,	192.65,	193.01,	193.37,	193.74,
194.10,	194.46,	194.82,	195.18,	195.55,	195.91,	196.27,	196.63,	196.99,	197.35,
197.71,	198.07,	198.43,	198.79,	199.15,	199.51,	199.87,	200.23,	200.59,	200.95,
201.31,	201.67,	202.03,	202.39,	202.75,	203.11,	203.47,	203.83,	204.19,	204.55,
204.90,	205.26,	205.62,	205.98,	206.34,	206.70,	207.05,	207.41,	207.77,	208.13,
208.48,	208.84,	209.20,	209.56,	209.91,	210.27,	210.63,	210.98,	211.34,	211.70,
212.05,	212.41,	212.76,	213.12,	213.48,	213.83,	214.19,	214.54,	214.90,	215.25,
215.61,	215.96,	216.32,	216.67,	217.03,	217.38,	217.74,	218.09,	218.44,	218.80,
219.15,	219.51,	219.86,	220.21,	220.57,	220.92,	221.27,	221.63,	221.98,	222.33,
222.68,	223.04,	223.39,	223.74,	224.09,	224.45,	224.80,	225.15,	225.50,	225.85,
226.21,	226.56,	226.91,	227.26,	227.61,	227.96,	228.31,	228.66,	229.02,	229.37,
229.72,	230.07,	230.42,	230.77,	231.12,	231.47,	231.82,	232.17,	232.52,	232.87,
233.21,	233.56,	233.91,	234.26,	234.61,	234.96,	235.31,	235.66,	236.00,	236.35,
236.70,	237.05,	237.40,	237.74,	238.09,	238.44,	238.79,	239.13,	239.48,	239.83,
240.18,	240.52,	240.87,	241.22,	241.56,	241.91,	242.26,	242.60,	242.95,	243.29,
243.64,	243.99,	244.33,	244.68,	245.02,	245.37,	245.71,	246.06,	246.40,	246.75,
247.09,	247.44,	247.78,	248.13,	248.47,	248.81,	249.16,	249.50,	245.85,	250.19,
250.53,	250.88,	251.22,	251.56,	251.91,	252.25,	252.59,	252.93,	253.28,	253.62,
253.96,	254.30, 254.65,	254.99,	255.33,	255.67,	256.01,	256.35,	256.70,	257.04,
257.38,	257.72,	258.06,	258.40,	258.74,	259.08,	259.42,	259.76,	260.10,	260.44,
260.78,	261.12,	261.46,	261.80,	262.14,	262.48,	262.82,	263.16,	263.50,	263.84,
264.18,	264.52,	264.86,	265.20,	265.53,	265.87,	266.21,	266.55,	266.89,	267.22,
267.56,	267.90,	268.24,	268.57,	268.91,	269.25,	269.59,	269.92,	270.26,	270.60,
270.93,	271.27,	271.61,	271.94,	272.28,	272.61,	272.95,	273.29,	273.62,	273.96,
274.29,	274.63,	274.96,	275.30,	275.63,	275.97,	276.30,	276.64,	276.97,	277.31,
277.64,	277.98,	278.31,	278.64,	278.98, 279.31,	279.64,	279.98,	280.31,	280.64,
280.98,	281.31,	281.64,	281.98,	282.31,	282.64,	282.97,	283.31,	283.64,	283.97,
284.30,	284.63,	284.97,	285.30,	285.63,	285.96,	286.29,	286.62,	286.85,	287.29,
287.62,	287.95,	288.28,	288.61,	288.94,	289.27,	289.60,	289.93,	290.26,	290.59,
290.92,	291.25,	291.58,	291.91,	292.24,	292.56,	292.89,	293.22,	293.55,	293.88,
294.21,	294.54,	294.86,	295.19,	295.52,	295.85,	296.18,	296.50,	296.83,	297.16,
297.49,	297.81,	298.14,	298.47,	298.80,	299.12,	299.45,	299.78,	300.10,	300.43,
300.75,	301.08,	301.41,	301.73,	302.06,	302.38,	302.71,	303.03,	303.36,	303.69,
304.01,	304.34,	304.66,	304.98,	305.31,	305.63,	305.96,	306.28,	306.61,	306.93,
307.25,	307.58,	307.90,	308.23,	308.55,	308.87,	309.20,	309.52,	309.84,	310.16,
310.49,	310.81,	311.13,	311.45,	311.78,	312.10,	312.42,	312.74,	313.06,	313.39,
313.71,	314.03,	314.35,	314.67,	314.99,	315.31,	315.64,	315.96,	316.28,	316.60,
316.92,	317.24,	317.56,	317.88,	318.20,	318.52,	318.84,	319.16,	319.48,	319.80,
320.12,	320.43,	320.75,	321.07,	321.39,	321.71,	322.03,	322.35,	322.67,	322.98,
323.30,	323.62,	323.94,	324.26,	324.57,	324.89,	325.21,	325.53,	325.84,	326.16,
326.48,	326.79,	327.11,	327.43,	327.74,	328.06,	328.38,	328.69,	329.01,	329.32,
329.64, 329.96};
   
//Summary:      查表	
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
//查表PT100输出
float SeekTable_PT100_S(s16 s16Tempe, const float* fTable, u16 u16TblLen)
{
  s16 Tempe;
  u8  Dec;
  float Res_PT;
    
  if(s16Tempe <= -1990)
    s16Tempe = -1990;
  if(s16Tempe >= 6500)
    s16Tempe = 6500;
  
  if(s16Tempe < 0)
  {
    Tempe = ((-s16Tempe) / 10) * (-1);
    Dec = (-s16Tempe) % 10;
    Res_PT = fTable[Tempe + 199] - ((float)Dec/10.0) * (fTable[Tempe + 200] - fTable[Tempe + 199]);

  }
  else
  {
    Tempe = s16Tempe / 10;
    Dec = s16Tempe % 10;
    Res_PT = fTable[Tempe + 199] + ((float)Dec/10.0) * (fTable[Tempe + 200] - fTable[Tempe + 199]);
  }
  
  return Res_PT;
}

//Summary:      查表	
float SeekTable_PT_Cu_M(float fR, float fMin, float fMax, 
                float fInt, const float* fTable, unsigned short u16TblLen)
{			
	float	fTValue;
	unsigned short  i;
	
    if(fR <= fTable[0])
    {
        return fMin;
    }
    else if(fR >= fTable[u16TblLen - 1])
    {
        return fMax;
    }    
    else
    {
        for(i = 0; i < u16TblLen - 1; i++)
        {
            if(fR >= fTable[i] && fR < fTable[i + 1])
                break;
        }
         fTValue = (fR - fTable[i]) * fInt / (fTable[i + 1] - fTable[i]) + fInt * i + fMin;
    }
	
	return fTValue;
}




//Summary:      计算线性值
static  u16    DoLinear(u16 u16x1, u16 u16x2, float fy1, float fy2, float fData)
{
  u16 u16Value;
  
  if(fy2 == fy1)
    fy2 = fy1 + 1;
  
  u16Value = (u16)((fData - fy1) * (u16x2 - u16x1) / (fy2 - fy1) + u16x1);
    
  return u16Value;
}

//Summary:      计算线性值-电阻输出
static  u16    
DoLinearRes(float fx1, float fx2,float fx3, float fx4, float fx5, float fx6,float fy1, float fy2, float fData)
{
  static u16   u16Value = 0;
  float   fValue;
  float current = 0;
  float mK1 = 0;
  float mB1 = 0;
  float mB2 = 0;
  float mB3 = 0;
  float mK2 = 0;
  
  float nK1 = 0;
  float nB1 = 0;
  float nB2 = 0;
  float nB3 = 0;
  float nK2 = 0;
  
  
  if(fx2 == fx1)
  {return 0; }
    
   mK1 = (fx4-fx3)/(fy2-fy1);
   mB1 =  fx3 - fy1*mK1;
   mB2 =  fx6 - fy1*mK1;
   mK2 =  3*(mB1-mB2)/2.0;
   mB3 =  mB1 - mK2;
   
   nK1 = (fx2-fx1)/(fy2-fy1);
   nB1 =  fx1 - fy1*nK1;
   nB2 =  fx5 - fy1*nK1;
   nK2 =  (nB2-nB1) / 3.0;
   nB3 =  nB1 - nK2*2.0;
   
  if(FlagI == 0)
  {
    current = adCode.u16RRef / 4096.0 * 2500.0 /604.0;
//    data.s16Ri = (s16)(current * 100000) / 10;
    if(current>0.03)
    {
     fValue = (fData - mK2/current - mB3)/mK1;
    u16Value = (u16)((1 - (fValue/604.0)) * 65535);
    if(current < 0.5)
        FlagI = 1;   
    }
  }
  else
  {
    current = adCode.u16RRef / 4096.0 * 2500.0 /3300.0;
//    data.s16Ri = (s16)(current * 100000) / 10; 
    if(current>0.03)
    {
    fValue = (fData - nK2/current - nB3)/nK1;
    u16Value = (u16)((1 - (fValue/3300.0)) * 65535);
    if(current > 0.55)
      FlagI = 0;
    }
  } 
  return u16Value;
}




//Summary:      计算线性值
static  float    DoLinear_Measure(u16 u16x1, u16 u16x2, float fy1, float fy2, u16 u16Data)
{
  float fValue;
  
  if(u16x2 == u16x1)
    u16x2 = u16x1 + 1;
  
  fValue = ((float)u16Data - (float)(u16x1)) * (fy2 - fy1) / (float)(u16x2 - u16x1) + fy1;
    
  return fValue;
}

//Summary:      计算线性值-电阻测量
static  float    DoLinear_MeasureRes(u16 u16x1, u16 u16x2, float fy1, float fy2, u16 u16Data)
{
  float fValue;
  
  if(u16x2 == u16x1)
    u16x2 = u16x1 + 1;
  
  fValue = (float)((((fy2-fy1) / ((float)u16x2-u16x1)) * u16Data) + ((fy1*u16x2-fy2*u16x1) / (u16x2-u16x1)));
    
  return (fValue + 0.5);
}


//Summary:      获取冷端自动值
s16     GetRJAuto(void)
{
    s16  s16RJAuto;
    float fTmp;
     
    //NTC温度计算 
    fTmp = (float)( (4096 - (float)adCode.u16NTCCode)/ adCode.u16NTCCode * 10);   
    fTmp = SeekTable_R_T(fTmp, -40, 100, 1, Tab_NTC, sizeof(Tab_NTC)/ 4);
    
    s16RJAuto = (s16)(fTmp * 10);
    
    return s16RJAuto;
}

//获取冷端模式自动还是手动
u8  GetRJMode(void)
{
  u8  u8AutoHand;
    
  u8AutoHand = GPIO_ReadInputDataBit(AUTOHAND_PORT, AUTOHANG_PIN);
  
  if(u8AutoHand == 0)   //自动 
    return  RJMODE_AUTO;
  else
    return  RJMODE_MAN;
}

//获取mA有源还是无源
u8  GetmASinkMode(void)
{
  u8  u8mASink;
    
  u8mASink = GPIO_ReadInputDataBit(MASINK_PORT, MASINK_PIN);
  
  if(u8mASink == 0)   //有源
    return  MAMODE_MA;
  else
    return  MAMODE_SINK;
}

//Summary:      获取冷端温度值
s16     GetRJTemp(void)
{
    s16     s16RJTemp;
    
    if(data.u8RJMode == RJMODE_AUTO)
      s16RJTemp = data.s16RJAuto;
    else
      s16RJTemp = data.s16RJSet;
    
    return s16RJTemp;
}

//口线初始化
void    Dodata_PortInit(void)
{
    GPIO_Init(OUTHAVESRC_PORT, OUTHAVESRC_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(OUTSIG_V_PORT, OUTSIG_V_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN, GPIO_Mode_Out_PP_High_Slow);    
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
    //R输出
    GPIO_Init(OUTSIG_R_PORT, OUTSIG_R_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(OUTSIG_O1_PORT, OUTSIG_O1_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(OUTSIG_O2_PORT, OUTSIG_O2_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(OUTSIG_O3_PORT, OUTSIG_O3_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(OUTSIG_O5_PORT, OUTSIG_O5_PIN, GPIO_Mode_Out_PP_High_Slow);
      
    GPIO_Init(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN, GPIO_Mode_Out_PP_High_Slow);
   
    
    GPIO_Init(INSIG_O_PORT, INSIG_O_PIN, GPIO_Mode_Out_PP_High_Slow);
          
}

//Summary： 测量信号口线切换
void    SetMSigPort(u8 u8SigType)
{
    switch(u8SigType)
    {
      case SIGTYPE_24VLOOP:
        GPIO_SetBits(INSIG_MA_PORT, INSIG_MA_PIN);
        GPIO_ResetBits(INSIG_20MV_PORT, INSIG_20MV_PIN);
        GPIO_ResetBits(INSIG_V_PORT, INSIG_V_PIN);
        GPIO_SetBits(INSIG_O_PORT, INSIG_O_PIN); 
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN);
        //将输出切换为25mA输出,且输出功能固定不可切换
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
        GPIO_SetBits(INSIG_O_PORT, INSIG_O_PIN);    
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN);
        break;
      case SIGTYPE_20MV:
      case SIGTYPE_T:
      case SIGTYPE_R:
      case SIGTYPE_B:
      case SIGTYPE_S:                            
        GPIO_SetBits(INSIG_MA_PORT, INSIG_MA_PIN);
        GPIO_ResetBits(INSIG_20MV_PORT, INSIG_20MV_PIN);
        GPIO_ResetBits(INSIG_V_PORT, INSIG_V_PIN);
        GPIO_SetBits(INSIG_O_PORT, INSIG_O_PIN);    
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN);
        break;
      case SIGTYPE_100MV:
      case SIGTYPE_K:
      case SIGTYPE_E:
      case SIGTYPE_J:
      case SIGTYPE_N:                            
        GPIO_SetBits(INSIG_MA_PORT, INSIG_MA_PIN);
        GPIO_SetBits(INSIG_20MV_PORT, INSIG_20MV_PIN);
        GPIO_ResetBits(INSIG_V_PORT, INSIG_V_PIN);
        GPIO_SetBits(INSIG_O_PORT, INSIG_O_PIN);
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN);
        break;
      case SIGTYPE_V:                            
        GPIO_ResetBits(INSIG_MA_PORT, INSIG_MA_PIN);
        GPIO_ResetBits(INSIG_20MV_PORT, INSIG_20MV_PIN);
        GPIO_SetBits(INSIG_V_PORT, INSIG_V_PIN);
        GPIO_SetBits(INSIG_O_PORT, INSIG_O_PIN);
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN);
        break;
    case SIGTYPE_PT100:
    case SIGTYPE_PT:
        GPIO_ResetBits(INSIG_MA_PORT, INSIG_MA_PIN);
        GPIO_ResetBits(INSIG_20MV_PORT, INSIG_20MV_PIN);
        GPIO_ResetBits(INSIG_V_PORT, INSIG_V_PIN);      
        GPIO_ResetBits(INSIG_O_PORT, INSIG_O_PIN);
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN); 
        break;
      default:                            
        GPIO_ResetBits(INSIG_MA_PORT, INSIG_MA_PIN);
        GPIO_ResetBits(INSIG_20MV_PORT, INSIG_20MV_PIN);
        GPIO_ResetBits(INSIG_V_PORT, INSIG_V_PIN);
        GPIO_SetBits(INSIG_O_PORT, INSIG_O_PIN);
        break;
    }
}

//Summary:  测量,得到实际信号值
s16    DoMeasure(u8 u8SigType)
{
    s16 s16MValue = 0;
    float fTmp;
  
    //切换信号口线
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
      case SIGTYPE_PT100:
          if((adCode.u16RCode) <= (calPwm.u16MCal_R2[1]))
              fTmp = DoLinear_MeasureRes((calPwm.u16MCal_R2[0]),(calPwm.u16MCal_R2[1]),0.0,200.0,adCode.u16RCode);
          else
              fTmp = DoLinear_MeasureRes((calPwm.u16MCal_R2[1] ),(calPwm.u16MCal_R2[2]),200.0,400.0,adCode.u16RCode);
        s16MValue = (s16)(fTmp * 10);      
        break;
      case SIGTYPE_PT:
          if((adCode.u16RCode) <= (calPwm.u16MCal_R2[1]))
              fTmp = DoLinear_MeasureRes((calPwm.u16MCal_R2[0]),(calPwm.u16MCal_R2[1]),0.0,200.0,adCode.u16RCode);
          else
              fTmp = DoLinear_MeasureRes((calPwm.u16MCal_R2[1]),(calPwm.u16MCal_R2[2]),200.0,400.0,adCode.u16RCode);
         s16MValue = (s16)((SeekTable_PT_Cu_M(fTmp, -199, 650, 1,Tab_BO, sizeof(Tab_BO)/ 4)) * 10);     
        break;     
      default:
          s16MValue = 0;
        break;
    }
    
    
    return s16MValue;
}

//Summary:  输出标定
void     DoSourceCali(u8 u8CaliSig, u16* pu16CalPwm)
{
  if(u8CaliSig < 5)
      TIM2_SetCompare2(*pu16CalPwm);
  switch(u8CaliSig)
    {
      case SCALSIG_MA:   //有源20mA
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
        GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
        GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
        GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
        GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
        GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);      
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN);         
        break;
      case SCALSIG_MASINK: //无源20mA
        GPIO_ResetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
        GPIO_ResetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_SetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
        GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
        GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
        GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
        GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN); 
        break;
      case SCALSIG_20MV:   //20mV
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
        GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_ResetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_SetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_SetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
        GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
        GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
        GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
        GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);        
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN);       
        break;
      case SCALSIG_100MV: //100mV
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
        GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_ResetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_SetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
        GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
        GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
        GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
        GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);        
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN); 
        break;
      case SCALSIG_V:// //V
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_ResetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
        GPIO_SetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
        GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
        GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
        GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
        GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);        
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN); 
        break;
     case SCALSIG_R:
     case SCALSIG_R2: 
     case SCALSIG_R3:
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
        GPIO_SetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);   
        GPIO_SetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN); 
        if(FlagI == 0)
        {
          GPIO_ResetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
          GPIO_SetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
          GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
          GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
          GPIO_ResetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);               
        }
        else
        {
          GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
          GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
          GPIO_SetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
          GPIO_ResetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
          GPIO_ResetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);          
        }
        Write_DAC8552(0,*pu16CalPwm);                
        break;
      default:
        break;
    }
}

//Summary： 输出信号口线切换
void    SetSSigPort(u8 u8SigType)
{
    switch(u8SigType)
    {
      case SIGTYPE_MA:
      case SIGTYPE_4_20MA:
        if(data.u8RemoteFlag == 0)
        {
               //判断有源还是无源
              if(l_u8mAMode == MAMODE_MA) //有源
              {          
                GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
                GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
                GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
                GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
                GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
                GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
                GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
                GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
                GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
                GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
                GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
                GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
                GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);   
                GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN);
              }
              else    //无源
              {                            
                GPIO_ResetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
                GPIO_ResetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
                GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
                GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
                GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
                GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
                GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
                GPIO_SetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
                GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
                GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
                GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
                GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
                GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);   
                GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN);      
              }
        }
        else
        {
          if(data.u8SinkFlag == MAMODE_MA)
          {
                GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
                GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
                GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
                GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
                GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
                GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
                GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
                GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
                GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
                GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
                GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
                GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
                GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);   
                GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN);            
          }
          else
          {
                GPIO_ResetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
                GPIO_ResetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
                GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
                GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
                GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
                GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
                GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
                GPIO_SetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
                GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
                GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
                GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
                GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
                GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);   
                GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN);              
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
        GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_ResetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_SetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_SetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
        GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
        GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
        GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
        GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);   
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN); 
        break;
      case SIGTYPE_100MV:
      case SIGTYPE_K:
      case SIGTYPE_E:
      case SIGTYPE_J:
      case SIGTYPE_N:                            
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_SetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
        GPIO_ResetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_SetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
        GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
        GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
        GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
        GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);   
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN); 
        break;
      case SIGTYPE_V:                            
        GPIO_SetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_ResetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
        GPIO_ResetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_SetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_ResetBits(OUTSIG_20MV_PORT, OUTSIG_20MV_PIN);
        GPIO_ResetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
        GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
        GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
        GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
        GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);   
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN); 
        break;
    case SIGTYPE_PT100:
    case SIGTYPE_PT:
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_VMA_PORT, OUTSIG_MA_PIN);
        GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
        GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        GPIO_SetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN); 
        if(FlagI == 0)
        {
          GPIO_ResetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
          GPIO_SetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
          GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
          GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
          GPIO_ResetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);               
        }
        else
        {
          GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
          GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
          GPIO_SetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
          GPIO_ResetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
          GPIO_ResetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);          
        }        
        break;
      default:                            
        GPIO_ResetBits(OUTHAVESRC_PORT, OUTHAVESRC_PIN);
        GPIO_ResetBits(OUTSIG_MA_PORT, OUTSIG_MA_PIN);
        GPIO_ResetBits(OUTSIG_V_PORT, OUTSIG_V_PIN);
        GPIO_SetBits(OUTSIG_VIO_PORT, OUTSIG_VIO_PIN);
        GPIO_SetBits(OUTSIG_MV_PORT, OUTSIG_MV_PIN);
        
        GPIO_SetBits(OUTSIG_VMA_PORT, OUTSIG_VMA_PIN);
        GPIO_ResetBits(OUTNOTSRC_PORT, OUTNOTSRC_PIN);
        GPIO_SetBits(OUTSIG_R_PORT, OUTSIG_R_PIN);
        GPIO_ResetBits(OUTSIG_O2_PORT, OUTSIG_O2_PIN);
        GPIO_ResetBits(OUTSIG_O3_PORT, OUTSIG_O3_PIN);
        GPIO_SetBits(OUTSIG_O1_PORT, OUTSIG_O1_PIN);
        GPIO_SetBits(OUTSIG_O5_PORT, OUTSIG_O5_PIN);   
        GPIO_ResetBits(OUTSIG_SIG_PORT, OUTSIG_SIG_PIN);
        break;
    }
}

//Summary： 输出信号
void    DoSource(u8 u8SigType, s16 s16SourceValue)
{
    u16     u16PWM;
    u16     u16ResDigValue = 0;
    float   fData;
    
    
    //切换输出信号口线
    SetSSigPort(u8SigType);
    //计算PWM值,并重置PWM占空比
    switch(u8SigType)
    {
      case SIGTYPE_MA:
      case SIGTYPE_4_20MA:
        if(data.u8RemoteFlag == 0)
        {
          //判断有源还是无源
          if(l_u8mAMode == MAMODE_MA) //有源
          {
            u16PWM = DoLinear(calPwm.u16SCalPwm_mA[0], calPwm.u16SCalPwm_mA[1],
                              calPwm.fSCalPwm_mA[0], calPwm.fSCalPwm_mA[1], (float)s16SourceValue /100);
          }
          else    //无源
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
        fData = T_T2E(s16SourceValue) - T_T2E((float)GetRJTemp() / 10); //温度转换为mV值，冷端为0℃
        u16PWM = DoLinear(calPwm.u16SCalPwm_20mV[0], calPwm.u16SCalPwm_20mV[1],
                            calPwm.fSCalPwm_20mV[0], calPwm.fSCalPwm_20mV[1], fData);
        break;
      case SIGTYPE_R:
        fData = R_T2E(s16SourceValue) - R_T2E((float)GetRJTemp() / 10); //温度转换为mV值
        u16PWM = DoLinear(calPwm.u16SCalPwm_20mV[0], calPwm.u16SCalPwm_20mV[1],
                            calPwm.fSCalPwm_20mV[0], calPwm.fSCalPwm_20mV[1], fData);
         break;
      case SIGTYPE_B:
        fData = B_T2E(s16SourceValue); //温度转换为mV值
        u16PWM = DoLinear(calPwm.u16SCalPwm_20mV[0], calPwm.u16SCalPwm_20mV[1],
                            calPwm.fSCalPwm_20mV[0], calPwm.fSCalPwm_20mV[1], fData);
         break;
      case SIGTYPE_S:
        fData = S_T2E(s16SourceValue) - S_T2E((float)GetRJTemp() / 10); //温度转换为mV值
        u16PWM = DoLinear(calPwm.u16SCalPwm_20mV[0], calPwm.u16SCalPwm_20mV[1],
                            calPwm.fSCalPwm_20mV[0], calPwm.fSCalPwm_20mV[1], fData);
        break;      
      case SIGTYPE_K:
        fData = K_T2E(s16SourceValue) - K_T2E((float)GetRJTemp() / 10); //温度转换为mV值
        
        u16PWM = DoLinear(calPwm.u16SCalPwm_100mV[0], calPwm.u16SCalPwm_100mV[1],
                            calPwm.fSCalPwm_100mV[0], calPwm.fSCalPwm_100mV[1], fData);
        break;
      case SIGTYPE_E:
        fData = E_T2E(s16SourceValue) - E_T2E((float)GetRJTemp() / 10); //温度转换为mV值
        u16PWM = DoLinear(calPwm.u16SCalPwm_100mV[0], calPwm.u16SCalPwm_100mV[1],
                            calPwm.fSCalPwm_100mV[0], calPwm.fSCalPwm_100mV[1], fData);
        break;
      case SIGTYPE_J:
        fData = J_T2E(s16SourceValue) - J_T2E((float)GetRJTemp() / 10); //温度转换为mV值
        u16PWM = DoLinear(calPwm.u16SCalPwm_100mV[0], calPwm.u16SCalPwm_100mV[1],
                            calPwm.fSCalPwm_100mV[0], calPwm.fSCalPwm_100mV[1], fData);
        break;
      case SIGTYPE_N:
        fData = N_T2E(s16SourceValue) - N_T2E((float)GetRJTemp() / 10); //温度转换为mV值
        u16PWM = DoLinear(calPwm.u16SCalPwm_100mV[0], calPwm.u16SCalPwm_100mV[1],
                            calPwm.fSCalPwm_100mV[0], calPwm.fSCalPwm_100mV[1], fData);      
        break;
     case SIGTYPE_V:
        u16PWM = DoLinear(calPwm.u16SCalPwm_V[0], calPwm.u16SCalPwm_V[1],
                            calPwm.fSCalPwm_V[0], calPwm.fSCalPwm_V[1], (float)s16SourceValue/100);
        break;
     case SIGTYPE_PT100:
        
      u16ResDigValue = DoLinearRes(calPwm.fSCaLPwm_R[0], calPwm.fSCaLPwm_R[1],calPwm.fSCaLPwm_R2[0], calPwm.fSCaLPwm_R2[1],
                          calPwm.fSCaLPwm_R3[0], calPwm.fSCaLPwm_R3[1],100.0,300.0, ((float)s16SourceValue/10.0) );     
      Write_DAC8552(0,u16ResDigValue); 
        break;
     case SIGTYPE_PT:
     //  adCode.u16RCode = 0;
      u16ResDigValue = DoLinearRes(calPwm.fSCaLPwm_R[0], calPwm.fSCaLPwm_R[1], calPwm.fSCaLPwm_R2[0], calPwm.fSCaLPwm_R2[1],                                    
                          calPwm.fSCaLPwm_R3[0], calPwm.fSCaLPwm_R3[1],100.0,300.0,SeekTable_PT100_S(s16SourceValue,Tab_BO, sizeof(Tab_BO)/ 4));                                             
        Write_DAC8552(0,u16ResDigValue);  
        break;
      default:
        //全关闭
        u16PWM = 0;
        break;
    }
    TIM2_SetCompare2(u16PWM);
}

//Summary:  AD数据采样处理
void    Dodata_ADSample(void)
{
  u8  u8Msg;
  
  u8Msg = GetMsg(MSG_ADOK, 1); 
  if(u8Msg != 0) 
  {
    data.s16RJAuto = GetRJAuto();
    if(data.u8MeasureEnable == ON ) //测量开启 
    {
      data.s16MeasureValue = DoMeasure(data.u8MSigType);
    }
    //显示电池电量
    ViewBatteryLevel(adCode.u16BatteryCode);
    ClearMsg(MSG_ADOK);       
  }
   
}

//Summary:  每秒数据处理
void    Dodata_1S(void)
{  
   data.u8RJMode = GetRJMode();
   l_u8mAMode = GetmASinkMode(); 
   Dodata_ADSample();
}