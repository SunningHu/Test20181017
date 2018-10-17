/*=============================================================================
//File:			measure.c	
//Author:		tpcon  
//Version:	    V0.1
//Date:			2014/04
//Dese:			AD采样，由定时器4触发，每秒采样一次,DMA模式
===============================================================================*/
#include "measure.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_adc.h"
#include "stm8l15x_dma.h"
#include "stm8l15x_syscfg.h"
#include "dodata.h"

#define   N   30
extern  void delay_us(u16 us);

extern s32  fPeriodArray[30];

u8 adc_int_flag = 0;


/********************************************************************
* Summary:    AD初始化  电池电压  _CH1   NTC_CH24
* Parameters:
*             void
*       
* Return :    void
********************************************************************/
void	MADC_Init(void)
{
    //开启ADC外设时钟
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
  
    //初始化ADC为单次转换模式，分辨率12bit，ADC时钟1分频
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);
    
    //设置采样通道为0-23普通ADC采样，采样时间为384个ADC时钟周期
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);
    
    //使能ADC
    ADC_Cmd(ADC1, ENABLE);

    //使能PF0对应的ADC_IN3通道的采样
    ADC_ChannelCmd(ADC1, ADC_Channel_1, ENABLE);

    //我们使用中断的方式进行ADC采样
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    
    //立即启动一次转换
    ADC_SoftwareStartConv(ADC1);
}

/*************************************************
1、采集一次ADC的数值
2、12bit分辨率最大可以得到4096的数值
3、我们把4096分成1000份，来得到LED闪烁时间的占空比
4、LED每一秒钟闪烁的占空比由ADC转换结果而定
*************************************************/
void adc_handle(void)
{  
    //判断ADC转换完成
    if(adc_int_flag == 1)
    {  
        //读取的时候标志位自动清空
        adc_int_flag = 0;
        //读取AD转换的值
        data.u16ADdata=ADC_GetConversionValue(ADC1);
        //开启下一次ADC转换
        ADC_SoftwareStartConv(ADC1);
    }      
}


/********************************************************************
* Summary:    电池电量采集滤波算法
* Parameters:
*             void
*       
* Return :    void
********************************************************************/
void ADC_BatteryADdata(void)
{   
    u8 cf = 0,ck = 0,cj = 0;
    u16 ad_buf[30] = {0};
    u16 tk = 0;
    u8 count = 0;
    u32 sum = 0;
    static u8 ci = 0;
    static u16 ADdataBattery;
    for(count = 0;count < N;count ++)
    { 
      adc_handle();
      delay_us(125);
      ad_buf[count] = data.u16ADdata;
    }
    for(ck = 0;ck < (N - 1);ck ++)
    {
      for(cj = 0;cj < (N - ck);cj ++)
      {
        if(ad_buf[cj] > ad_buf[cj+1])
        {
          tk = ad_buf[cj];
          ad_buf[cj] = ad_buf[cj+1];
          ad_buf[cj + 1] = tk;
        }
      }
    }
    for(cf = 6;cf < (N - 6);cf ++)
    {sum += ad_buf[cf];}
    
    ADdataBattery = (u16)(sum / (N - 12));
    
    if(ADdataBattery != adCode.u16BatteryCode)
    {ci ++;}
    if(ADdataBattery == adCode.u16BatteryCode)
    {ci = 0;}
    if(ci >= 5)
    {adCode.u16BatteryCode = ADdataBattery;ci = 0;} 
}




/********************************************************************
* Summary:    电池电量采集NTC采集
*             void
*       
* Return :    void
********************************************************************/
void ADC_NTCdata(void)
{   
    u8 cf = 0,ck = 0,cj = 0;
    u16 ad_buf[30] = {0};
    u16 tk = 0;
    u8 count = 0;
    u32 sum = 0;
    static u8 ci = 0;
    static u16 ADdata;
    for(count = 0;count < N;count ++)
    { 
      adc_handle();
      delay_us(125);
      ad_buf[count] = data.u16ADdata;
    }
    for(ck = 0;ck < (N - 1);ck ++)
    {
      for(cj = 0;cj < (N - ck);cj ++)
      {
        if(ad_buf[cj] > ad_buf[cj+1])
        {
          tk = ad_buf[cj];
          ad_buf[cj] = ad_buf[cj+1];
          ad_buf[cj + 1] = tk;
        }
      }
    }
    for(cf = 6;cf < (N - 6);cf ++)
    {sum += ad_buf[cf];}
    
    ADdata = (u16)(sum / (N - 12));
    
    if(ADdata != adCode.u16NTCCode)
    {ci ++;}
    if(ADdata == adCode.u16NTCCode)
    {ci = 0;}
    if(ci >= 5)
    {adCode.u16NTCCode = ADdata;ci = 0;} 
}

      
/********************************************************************
* Summary:    HZ滤波算法
* Parameters:
*             void
*       
* Return :    void
********************************************************************/    
void ADC_Hzdata(void)
{   
    u8 cf = 0,ck = 0,cj = 0;
    s32 tk = 0;
    s32 sum = 0;
    static u8 ci = 0;
    static s32 ADdata;

    for(ck = 0;ck < (N - 1);ck ++)
    {
      for(cj = 0;cj < (N - ck - 1);cj ++)
      {
        if(fPeriodArray[cj] > fPeriodArray[cj+1])
        {
          tk = fPeriodArray[cj];
          fPeriodArray[cj] = fPeriodArray[cj+1];
          fPeriodArray[cj + 1] = tk;
        }
      }
    }
    for(cf = 5;cf < (N - 5);cf ++)
    {sum += fPeriodArray[cf];}
    
    ADdata = (s32)((sum / (N - 10)) + 0.5);
    
        if(ADdata != adCode.s32HzCode)
        {ci ++;}
        if(ADdata == adCode.s32HzCode)
        {ci = 0;}
        if(ci >= 8)
        {adCode.s32HzCode = ADdata;ci = 0;} 
    
}      
    