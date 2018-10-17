/*=============================================================================
//File:			DAC8811.c	
//Author:		huzhenjun
//Version:	        V0.1
//Date:			2017/06
//Dese:			DAC8811驱动
===============================================================================*/

#include "DAC8811.h"

extern   void delay_us(u16 us);
#define     DACSDI_PORT             GPIOG
#define     DACSDI_PIN                   GPIO_Pin_0

#define     DACCLK_PORT             GPIOA
#define     DACCLK_PIN                   GPIO_Pin_7

#define     DACCS_PORT              GPIOA
#define     DACCS_PIN                    GPIO_Pin_6


#define     SDI_H                  (GPIO_SetBits(DACSDI_PORT, DACSDI_PIN))      
#define     SDI_L                  (GPIO_ResetBits(DACSDI_PORT, DACSDI_PIN))

#define     CLK_H                  (GPIO_SetBits(DACCLK_PORT, DACCLK_PIN))      
#define     CLK_L                  (GPIO_ResetBits(DACCLK_PORT, DACCLK_PIN))

#define     CS_H                   (GPIO_SetBits(DACCS_PORT, DACCS_PIN))      
#define     CS_L                   (GPIO_ResetBits(DACCS_PORT, DACCS_PIN))

#define     VREF                    3.0


/********************************************************************
* Summary:    DAC8811初始化
* Parameters:
*             void
*       
* Return :    void
********************************************************************/
void DAC8811_Init(void)
{
    GPIO_Init(DACSDI_PORT, DACSDI_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(DACCLK_PORT, DACCLK_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(DACCS_PORT, DACCS_PIN, GPIO_Mode_Out_PP_High_Slow);
}


//Summary:      DAC8811数字输入
//Parameter:    [u16Data]  16位码值
//Return:       [void]
void DAC8811_Write(u16 u16Data)
{
	uint8_t i;

	CS_H;
	CS_L;
        CLK_L;
	for (i=0; i<16; i++)          
	{
		CLK_L;
		if (u16Data & 0x8000)
		{
			SDI_H;
		}
		else
		{
			SDI_L;
		}
		CLK_H;
		u16Data <<= 1;
	}
	CS_H;    
}

//Summary:      DAC8811模拟输入
//Parameter:    [fData]  输出电压值
//Return:       [void]
void DAC8811_OUTV(float fData)
{
  u16 u16Data = 0;
  
  u16Data = (u16)((fData / VREF) * 65535);
  DAC8811_Write(u16Data);
}

