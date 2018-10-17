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

#define     BUFFER_SIZE             MAX_BUF * 5       //�ɼ�7��ADͨ��,����10��ȡƽ��ֵ
#define     BUFFER_ADDRESS         ((u16)(&ADC_Buffer))
#define     ADC1_DR_ADDRESS        ((u16)0x5344)           //ADC1_DRH��ַ
u16     ADC_Buffer[BUFFER_SIZE];

//Summary:	�źŲ�����AD��ʼ��
void	Measure_ADC_Init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_2);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);
    ADC_Cmd(ADC1,ENABLE);
    ADC_ChannelCmd(ADC1, ADC_Channel_0, ENABLE);       //�ɼ�mA�ź�
    ADC_ChannelCmd(ADC1, ADC_Channel_1, ENABLE);       //�ɼ�V�ź�    
    ADC_ChannelCmd(ADC1, ADC_Channel_2, ENABLE);       //�ɼ�mV�ź�
    ADC_ChannelCmd(ADC1, ADC_Channel_7, ENABLE);       //�ɼ�NTC�ź�
    ADC_ChannelCmd(ADC1, ADC_Channel_19, ENABLE);        //�ɼ���ص�ѹ    
    
  
    //DMA��ʼ��
    SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_ADC1ToChannel0);

    DMA_Init(DMA1_Channel0, BUFFER_ADDRESS,
           ADC1_DR_ADDRESS,
           BUFFER_SIZE,
           DMA_DIR_PeripheralToMemory,
           DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,
           DMA_Priority_High,
           DMA_MemoryDataSize_HalfWord);

    /* Enable DMA1 channel0 Transfer complete interrupt */
    DMA_ITConfig(DMA1_Channel0, DMA_ITx_TC, ENABLE);
    DMA_GlobalCmd(ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    
    DMA_Cmd(DMA1_Channel0, ENABLE); //�ڿ�ʼת��ǰ����DMAͨ��������ͨ��˳�����
    ADC_SoftwareStartConv(ADC1);
}



