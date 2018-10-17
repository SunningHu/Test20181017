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

#define     BUFFER_SIZE             MAX_BUF * 5       //采集7个AD通道,采样10个取平均值
#define     BUFFER_ADDRESS         ((u16)(&ADC_Buffer))
#define     ADC1_DR_ADDRESS        ((u16)0x5344)           //ADC1_DRH地址
u16     ADC_Buffer[BUFFER_SIZE];

//Summary:	信号采样，AD初始化
void	Measure_ADC_Init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_2);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);
    ADC_Cmd(ADC1,ENABLE);
    ADC_ChannelCmd(ADC1, ADC_Channel_0, ENABLE);       //采集mA信号
    ADC_ChannelCmd(ADC1, ADC_Channel_1, ENABLE);       //采集V信号    
    ADC_ChannelCmd(ADC1, ADC_Channel_2, ENABLE);       //采集mV信号
    ADC_ChannelCmd(ADC1, ADC_Channel_7, ENABLE);       //采集NTC信号
    ADC_ChannelCmd(ADC1, ADC_Channel_19, ENABLE);        //采集电池电压    
    
  
    //DMA初始化
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
    
    DMA_Cmd(DMA1_Channel0, ENABLE); //在开始转换前开启DMA通道，避免通道顺序混乱
    ADC_SoftwareStartConv(ADC1);
}



