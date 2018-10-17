/*=============================================================================
//File:			dac8552.c	
//Author:		HuZhenjun 
//Version:	        V0.1
//Date:			2016-6-27
//Dese:			8552相关函数
===============================================================================*/

#include "dac8552.h"
 
extern  void 	delay_us(u16 us);

#define       GPIO_DIN_PORT        GPIOG
#define       GPIO_DIN_PIN         GPIO_Pin_4

#define       GPIO_SCLK_PORT        GPIOG
#define       GPIO_SCLK_PIN         GPIO_Pin_5

#define       GPIO_SYNC_PORT        GPIOG
#define       GPIO_SYNC_PIN         GPIO_Pin_6


#define SYNC_H()     (GPIO_SetBits(GPIO_SYNC_PORT,GPIO_SYNC_PIN))
#define SYNC_L()     (GPIO_ResetBits(GPIO_SYNC_PORT,GPIO_SYNC_PIN))

#define SCLK_H()     (GPIO_SetBits(GPIO_SCLK_PORT,GPIO_SCLK_PIN))
#define SCLK_L()     (GPIO_ResetBits(GPIO_SCLK_PORT,GPIO_SCLK_PIN))

#define DIN_H()     (GPIO_SetBits(GPIO_DIN_PORT,GPIO_DIN_PIN))
#define DIN_L()     (GPIO_ResetBits(GPIO_DIN_PORT,GPIO_DIN_PIN))


/****************************************************************
// Summary: 	        初始化8552接口
// Parameter: 	        [void]
//
// return:		[void]
****************************************************************/


void dac8552_Init(void)
{
    GPIO_Init(GPIO_DIN_PORT, GPIO_DIN_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIO_SCLK_PORT, GPIO_SCLK_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIO_SYNC_PORT, GPIO_SYNC_PIN, GPIO_Mode_Out_PP_High_Slow);
}



/****************************************************************
// Summary: 	        通道命令，数据
// Parameter: 	        [[in/u8]Command  A/B通道，A =0X30  B =0X34
//                          [[in/u16]Resdata  电阻值
// return:		[void]
****************************************************************/
void Write_DAC8552(uint8_t Command,u16 Resdata)
{
	uint8_t i;

	SYNC_H();
	SYNC_L();
        delay_us(1);
	for (i=0; i<8; i++)              //Write 8 bit command 
	{
		SCLK_H();
		if (Command & 0x80)
		{
                    DIN_H();
		}
		else
		{
                    DIN_L();
		}
		SCLK_L();
		Command <<= 1;
	}

	for (i=0; i<16; i++)             //write 16 bit data
	{
		SCLK_H();
		if (Resdata & 0x8000)
		{
			DIN_H();
		}
		else
		{
			DIN_L();
		}
		SCLK_L();
		Resdata <<= 1;
	}
	SYNC_H();
}


