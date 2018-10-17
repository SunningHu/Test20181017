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
#include "gui.h"

#define   NUM   30
extern  void delay_us(u16 us);

u8 adc_int_flag = 0;

/*******************************************************/
//Summary:	 AD初始化
//Parameter:     void
//Return:        void
/*******************************************************/
void	Measure_ADC_Init(void)
{
    //ADC时钟默认关闭，使用ADC之前，最先就要开启时钟
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
  
  
    //初始化ADC为单次转换模式，分辨率12bit，ADC时钟1分频
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);
    //设置采样通道为0-23普通ADC采样，采样时间为384个ADC时钟周期
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);
    //使能ADC
    ADC_Cmd(ADC1, ENABLE);

    //使能PF0对应的ADC_IN3通道的采样
    ADC_ChannelCmd(ADC1, ADC_Channel_0, ENABLE);

    //我们使用中断的方式进行ADC采样
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    enableInterrupts();
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

//Summary:    mA采集
void ADC_MAdata(void)
{   
    u8 cf = 0,ck = 0,cj = 0;
    u16 ad_buf[30] = {0};
    u16 tk = 0;
    u8 count = 0;
    u32 sum = 0;
    static u8 ci = 0;
    static u16 ADdata;
    for(count = 0;count < NUM;count ++)
    { 
      adc_handle();
      delay_us(125);
      ad_buf[count] = data.u16ADdata;
    }
    for(ck = 0;ck < (NUM - 1);ck ++)
    {
      for(cj = 0;cj < (NUM - ck);cj ++)
      {
        if(ad_buf[cj] > ad_buf[cj+1])
        {
          tk = ad_buf[cj];
          ad_buf[cj] = ad_buf[cj+1];
          ad_buf[cj + 1] = tk;
        }
      }
    }
    for(cf = 6;cf < (NUM - 6);cf ++)
    {sum += ad_buf[cf];}
    
    ADdata = (u16)(sum / (NUM - 12));
    
    if(ADdata != adCode.u16mACode)
    {ci ++;}
    if(ADdata == adCode.u16mACode)
    {ci = 0;}
    if(ci >= 5)
    {adCode.u16mACode = ADdata;ci = 0;} 
}

//Summary:    MV采集
void ADC_MVdata(void)
{   
    u8 cf = 0,ck = 0,cj = 0;
    u16 ad_buf[30] = {0};
    u16 tk = 0;
    u8 count = 0;
    u32 sum = 0;
    static u8 ci = 0;
    static u16 ADdata;
    for(count = 0;count < NUM;count ++)
    { 
      adc_handle();
      delay_us(125);
      ad_buf[count] = data.u16ADdata;
    }
    for(ck = 0;ck < (NUM - 1);ck ++)
    {
      for(cj = 0;cj < (NUM - ck);cj ++)
      {
        if(ad_buf[cj] > ad_buf[cj+1])
        {
          tk = ad_buf[cj];
          ad_buf[cj] = ad_buf[cj+1];
          ad_buf[cj + 1] = tk;
        }
      }
    }
    for(cf = 6;cf < (NUM - 6);cf ++)
    {sum += ad_buf[cf];}
    
    ADdata = (u16)(sum / (NUM - 12));
    
    if(ADdata != adCode.u16mVCode)
    {ci ++;}
    if(ADdata == adCode.u16mVCode)
    {ci = 0;}
    if(ci >= 5)
    {adCode.u16mVCode = ADdata;ci = 0;} 
}

//Summary:    V采集
void ADC_Vdata(void)
{   
    u8 cf = 0,ck = 0,cj = 0;
    u16 ad_buf[30] = {0};
    u16 tk = 0;
    u8 count = 0;
    u32 sum = 0;
    static u8 ci = 0;
    static u16 ADdata;
    for(count = 0;count < NUM;count ++)
    { 
      adc_handle();
      delay_us(125);
      ad_buf[count] = data.u16ADdata;
    }
    for(ck = 0;ck < (NUM - 1);ck ++)
    {
      for(cj = 0;cj < (NUM - ck);cj ++)
      {
        if(ad_buf[cj] > ad_buf[cj+1])
        {
          tk = ad_buf[cj];
          ad_buf[cj] = ad_buf[cj+1];
          ad_buf[cj + 1] = tk;
        }
      }
    }
    for(cf = 6;cf < (NUM - 6);cf ++)
    {sum += ad_buf[cf];}
    
    ADdata = (u16)(sum / (NUM - 12));
    
    if(ADdata != adCode.u16VCode)
    {ci ++;}
    if(ADdata == adCode.u16VCode)
    {ci = 0;}
    if(ci >= 5)
    {adCode.u16VCode = ADdata;ci = 0;} 
}

//Summary:    NTC采集
void ADC_NTCdata(void)
{   
    u8 cf = 0,ck = 0,cj = 0;
    u16 ad_buf[30] = {0};
    u16 tk = 0;
    u8 count = 0;
    u32 sum = 0;
    static u8 ci = 0;
    static u16 ADdata;
    for(count = 0;count < NUM;count ++)
    { 
      adc_handle();
      delay_us(125);
      ad_buf[count] = data.u16ADdata;
    }
    for(ck = 0;ck < (NUM - 1);ck ++)
    {
      for(cj = 0;cj < (NUM - ck);cj ++)
      {
        if(ad_buf[cj] > ad_buf[cj+1])
        {
          tk = ad_buf[cj];
          ad_buf[cj] = ad_buf[cj+1];
          ad_buf[cj + 1] = tk;
        }
      }
    }
    for(cf = 6;cf < (NUM - 6);cf ++)
    {sum += ad_buf[cf];}
    
    ADdata = (u16)(sum / (NUM - 12));
    
    if(ADdata != adCode.u16NTCCode)
    {ci ++;}
    if(ADdata == adCode.u16NTCCode)
    {ci = 0;}
    if(ci >= 5)
    {adCode.u16NTCCode = ADdata;ci = 0;} 
}

//Summary:    R采集
void ADC_Rdata(void)
{   
    u8 cf = 0,ck = 0,cj = 0;
    u16 ad_buf[30] = {0};
    u16 tk = 0;
    u8 count = 0;
    u32 sum = 0;
    static u8 ci = 0;
    static u16 ADdata;
    for(count = 0;count < NUM;count ++)
    { 
      adc_handle();
      delay_us(125);
      ad_buf[count] = data.u16ADdata;
    }
    for(ck = 0;ck < (NUM - 1);ck ++)
    {
      for(cj = 0;cj < (NUM - ck);cj ++)
      {
        if(ad_buf[cj] > ad_buf[cj+1])
        {
          tk = ad_buf[cj];
          ad_buf[cj] = ad_buf[cj+1];
          ad_buf[cj + 1] = tk;
        }
      }
    }
    for(cf = 6;cf < (NUM - 6);cf ++)
    {sum += ad_buf[cf];}
    
    ADdata = (u16)(sum / (NUM - 12));
    
    if(ADdata != adCode.u16RCode)
    {ci ++;}
    if(ADdata == adCode.u16RCode)
    {ci = 0;}
    if(ci >= 5)
    {adCode.u16RCode = ADdata;ci = 0;} 
}

//Summary:    Battery采集
void ADC_Batterydata(void)
{   
    u8 cf = 0,ck = 0,cj = 0;
    u16 ad_buf[30] = {0};
    u16 tk = 0;
    u8 count = 0;
    u32 sum = 0;
    static u8 ci = 0;
    static u16 ADdata;
    for(count = 0;count < NUM;count ++)
    { 
      adc_handle();
      delay_us(125);
      ad_buf[count] = data.u16ADdata;
    }
    for(ck = 0;ck < (NUM - 1);ck ++)
    {
      for(cj = 0;cj < (NUM - ck);cj ++)
      {
        if(ad_buf[cj] > ad_buf[cj+1])
        {
          tk = ad_buf[cj];
          ad_buf[cj] = ad_buf[cj+1];
          ad_buf[cj + 1] = tk;
        }
      }
    }
    for(cf = 6;cf < (NUM - 6);cf ++)
    {sum += ad_buf[cf];}
    
    ADdata = (u16)(sum / (NUM - 12));
    
    if(ADdata != adCode.u16BatteryCode)
    {ci ++;}
    if(ADdata == adCode.u16BatteryCode)
    {ci = 0;}
    if(ci >= 5)
    {adCode.u16BatteryCode = ADdata;ci = 0;} 
}

//Summary:    Battery采集
void ADC_RIdata(void)
{   
    u8 cf = 0,ck = 0,cj = 0;
    u16 ad_buf[30] = {0};
    u16 tk = 0;
    u8 count = 0;
    u32 sum = 0;
    static u8 ci = 0;
    static u16 ADdata;
    for(count = 0;count < NUM;count ++)
    { 
      adc_handle();
      delay_us(125);
      ad_buf[count] = data.u16ADdata;
    }
    for(ck = 0;ck < (NUM - 1);ck ++)
    {
      for(cj = 0;cj < (NUM - ck);cj ++)
      {
        if(ad_buf[cj] > ad_buf[cj+1])
        {
          tk = ad_buf[cj];
          ad_buf[cj] = ad_buf[cj+1];
          ad_buf[cj + 1] = tk;
        }
      }
    }
    for(cf = 6;cf < (NUM - 6);cf ++)
    {sum += ad_buf[cf];}
    
    adCode.u16RRef = (u16)(sum / (NUM - 12));
    
    
     if(data.u8SourceEnable == ON && data.u8SSigType > SIGTYPE_N )    
        DoSource(data.u8SSigType, data.s16SourceValue);    
    
//    if(ADdata != adCode.u16RRef)
//    {ci ++;}
//    if(ADdata == adCode.u16RRef)
//    {ci = 0;}
//    if(ci >= 5)
//    {adCode.u16RRef = ADdata;ci = 0;} 
}