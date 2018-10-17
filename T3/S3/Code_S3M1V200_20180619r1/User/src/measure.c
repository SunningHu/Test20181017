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

#define   N   30
extern  void delay_us(u16 us);

extern s32  fPeriodArray[30];

u8 adc_int_flag = 0;


/********************************************************************
* Summary:    AD��ʼ��  ��ص�ѹ  _CH1   NTC_CH24
* Parameters:
*             void
*       
* Return :    void
********************************************************************/
void	MADC_Init(void)
{
    //����ADC����ʱ��
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
  
    //��ʼ��ADCΪ����ת��ģʽ���ֱ���12bit��ADCʱ��1��Ƶ
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);
    
    //���ò���ͨ��Ϊ0-23��ͨADC����������ʱ��Ϊ384��ADCʱ������
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);
    
    //ʹ��ADC
    ADC_Cmd(ADC1, ENABLE);

    //ʹ��PF0��Ӧ��ADC_IN3ͨ���Ĳ���
    ADC_ChannelCmd(ADC1, ADC_Channel_1, ENABLE);

    //����ʹ���жϵķ�ʽ����ADC����
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    
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


/********************************************************************
* Summary:    ��ص����ɼ��˲��㷨
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
* Summary:    ��ص����ɼ�NTC�ɼ�
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
* Summary:    HZ�˲��㷨
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
    