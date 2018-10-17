/*=============================================================================
//File:			measure.c	
//Author:		tpcon  
//Version:	    V0.1
//Date:			2014/04
//Dese:			AD�������ɶ�ʱ��4������ÿ�����һ��,DMAģʽ
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
//Summary:	 AD��ʼ��
//Parameter:     void
//Return:        void
/*******************************************************/
void	Measure_ADC_Init(void)
{
    //ADCʱ��Ĭ�Ϲرգ�ʹ��ADC֮ǰ�����Ⱦ�Ҫ����ʱ��
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
  
  
    //��ʼ��ADCΪ����ת��ģʽ���ֱ���12bit��ADCʱ��1��Ƶ
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);
    //���ò���ͨ��Ϊ0-23��ͨADC����������ʱ��Ϊ384��ADCʱ������
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);
    //ʹ��ADC
    ADC_Cmd(ADC1, ENABLE);

    //ʹ��PF0��Ӧ��ADC_IN3ͨ���Ĳ���
    ADC_ChannelCmd(ADC1, ADC_Channel_0, ENABLE);

    //����ʹ���жϵķ�ʽ����ADC����
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    enableInterrupts();
    //��������һ��ת��
    ADC_SoftwareStartConv(ADC1);
}

/*************************************************
1���ɼ�һ��ADC����ֵ
2��12bit�ֱ��������Եõ�4096����ֵ
3�����ǰ�4096�ֳ�1000�ݣ����õ�LED��˸ʱ���ռ�ձ�
4��LEDÿһ������˸��ռ�ձ���ADCת���������
*************************************************/
void adc_handle(void)
{  
    //�ж�ADCת�����
    if(adc_int_flag == 1)
    {  
        //��ȡ��ʱ���־λ�Զ����
        adc_int_flag = 0;
        //��ȡADת����ֵ
        data.u16ADdata=ADC_GetConversionValue(ADC1);

        //������һ��ADCת��
        ADC_SoftwareStartConv(ADC1);
    }      
}

//Summary:    mA�ɼ�
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

//Summary:    MV�ɼ�
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

//Summary:    V�ɼ�
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

//Summary:    NTC�ɼ�
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

//Summary:    R�ɼ�
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

//Summary:    Battery�ɼ�
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

//Summary:    Battery�ɼ�
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