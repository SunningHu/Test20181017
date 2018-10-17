/*=============================================================================
//File:			AD7705.c	
//Author:		胡镇俊
//Version:	        V0.1
//Date:			2016/07
//Dese:			TM7705 驱动模块(2通道带PGA的16位ADC)软件SPI
===============================================================================*/
#include "AD7705.h"

extern void 	delay_us(u16 us);

/* 通道1和通道2的增益,输入缓冲，极性 */
#define __CH1_GAIN_BIPOLAR_BUF	(GAIN_1 | BIPOLAR | BUF_EN)  
#define __CH2_GAIN_BIPOLAR_BUF	(GAIN_1 | UNIPOLAR | BUF_EN)

/*
      TM7705模块   STM8L美女开发板(STM8L152C6)
      VCC   ------  3.3V
      GND   ------  GND
      SCK   ------  PB3   <---时钟信号
      DOUT  ------  PC5   <---数据输出引脚
      DIN   ------  PB2   <--数据输入引脚
      CS    ------  PB1   <--- 片选信号，低电平有效，可以一直与地相连
      DRDY  ------  PB6   <--- 判断是否有新数据可读取
      RST   ------  PC6    <----复位引脚，低电平有效
*/

/* 软件SPI 定义GPIO端口*/
 
      #define PORT_SCK	        GPIOE
      #define PIN_SCK		GPIO_Pin_5

      #define PORT_DIN	        GPIOD
      #define PIN_DIN		GPIO_Pin_0

      #define PORT_DOUT	        GPIOD
      #define PIN_DOUT	        GPIO_Pin_2

      #define PORT_DRDY 	GPIOD
      #define PIN_DRDY	        GPIO_Pin_1

/* 定义口线置0和置1的宏 */


      #define SCK_1()		(GPIO_ResetBits(PORT_SCK, PIN_SCK)) 
      #define SCK_0()		(GPIO_SetBits(PORT_SCK, PIN_SCK))

      #define DIN_1()		(GPIO_ResetBits(PORT_DIN, PIN_DIN))
      #define DIN_0()		(GPIO_SetBits(PORT_DIN, PIN_DIN))

      #define DOUT_IS_HIGH()	((GPIO_ReadInputDataBit(PORT_DOUT, PIN_DOUT)) == 0)

      #define DRDY_IS_LOW()	((GPIO_ReadInputDataBit(PORT_DRDY, PIN_DRDY)) != 0)


/* 通信寄存器bit定义 */
enum
{
	/* 寄存器选择  RS2 RS1 RS0  */
	REG_COMM	= 0x00,	/* 通信寄存器 */
	REG_SETUP	= 0x10,	/* 设置寄存器 */
	REG_CLOCK	= 0x20,	/* 时钟寄存器 */
	REG_DATA	= 0x30,	/* 数据寄存器 */
	REG_ZERO_CH1	= 0x60,	/* CH1 偏移寄存器 */
	REG_FULL_CH1	= 0x70,	/* CH1 满量程寄存器 */
	REG_ZERO_CH2	= 0x61,	/* CH2 偏移寄存器 */
	REG_FULL_CH2	= 0x71,	/* CH2 满量程寄存器 */

	/* 读写操作 */
	WRITE 		= 0x00,	/* 写操作 */
	READ 		= 0x08,	/* 读操作 */

	/* 通道 */
	CH_1		= 0,	/* AIN1+  AIN1- */
	CH_2		= 1,	/* AIN2+  AIN2- */
	CH_3		= 2,	/* AIN1-  AIN1- */
	CH_4		= 3	/* AIN1-  AIN2- */
};

/* 设置寄存器bit定义 */
enum
{
	MD_NORMAL		= (0 << 6),	/* 正常模式 */
	MD_CAL_SELF		= (1 << 6),	/* 自校准模式 */
	MD_CAL_ZERO		= (2 << 6),	/* 校准0刻度模式 */
	MD_CAL_FULL		= (3 << 6),	/* 校准满刻度模式 */

	GAIN_1			= (0 << 3),	/* 增益 */
	GAIN_2			= (1 << 3),	/* 增益 */
	GAIN_4			= (2 << 3),	/* 增益 */
	GAIN_8			= (3 << 3),	/* 增益 */
	GAIN_16			= (4 << 3),	/* 增益 */
	GAIN_32			= (5 << 3),	/* 增益 */
	GAIN_64			= (6 << 3),	/* 增益 */
	GAIN_128		= (7 << 3),	/* 增益 */

        /* 无论双极性还是单极性都不改变任何输入信号的状态，它只改变输出数据的代码和转换函数上的校准点 */
	BIPOLAR			= (0 << 2),	/* 双极性输入 */
	UNIPOLAR		= (1 << 2),	/* 单极性输入 */

	BUF_NO			= (0 << 1),	/* 输入无缓冲（内部缓冲器不启用) */
	BUF_EN			= (1 << 1),	/* 输入有缓冲 (启用内部缓冲器) */

	FSYNC_0			= 0,
	FSYNC_1			= 1		/* 不启用 */
};

/* 时钟寄存器bit定义 */
enum
{
	CLKDIS_0	= 0x00,		/* 时钟输出使能 （当外接晶振时，必须使能才能振荡） */
	CLKDIS_1	= 0x10,		/* 时钟禁止 （当外部提供时钟时，设置该位可以禁止MCK_OUT引脚输出时钟以省电 */

	/*2.4576MHz（CLKDIV=0 ）或为 4.9152MHz （CLKDIV=1 ），CLK 应置 “1”。
	1MHz （CLKDIV=0 ）或 2MHz   （CLKDIV=1 ），CLK 该位应置  “0”*/	
	CLK_4_9152M =    0x0C,
	CLK_2_4576M =    0x04,
	CLK_1M 		= 0x00,
	CLK_2M 		= 0x80,
        
        /* 2M或者1M下*/
        FS_20HZ         = 0x00,
        FS_25HZ         = 0x01,
        FS_100HZ         = 0x02,
        FS_200HZ         = 0x03,
        
        /*4.9152MHz 或者 2.4576MHz下 */
	FS_50HZ		= 0x00,
	FS_60HZ		= 0x01,
	FS_250HZ	= 0x02,
	FS_500HZ	= 0x03,

	ZERO_0		= 0x00,
	ZERO_1		= 0x80
};

static void TM7705_SyncSPI(void);
static void TM7705_Send8Bit(uint8_t _data);
static uint8_t TM7705_Recive8Bit(void);
static void TM7705_WriteByte(uint8_t _data);
static void TM7705_Write3Byte(uint32_t _data);
static uint8_t TM7705_ReadByte(void);
static uint16_t TM7705_Read2Byte(void);
static uint32_t TM7705_Read3Byte(void);
static void TM7705_WaitDRDY(void);
static void TM7705_ResetHard(void);
static void TM7705_Delay(void);

/*
*********************************************************************************************************
*	函 数 名: bsp_DelayMS
*	功能说明: 延时函数，STM8L152  不分频16M  毫秒级
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_DelayMS(unsigned int n)
{	
  
    delay_ms(n);
//	unsigned int k;
//
////	while(n--)
////	{
////		for (k = 0; k < 75; k++);
////	}
}



/********************************************************************
* Summary:    配置STM8L的GPIO和SPI接口，用于连接 AD7705
* Parameters:
*             void
*       
* Return :    void
********************************************************************/
void bsp_InitTM7705(void)
{		


	/* 配置SCK口线为推挽输出，缺省输出高电平 */
	  GPIO_Init(PORT_SCK,PIN_SCK, GPIO_Mode_Out_PP_Low_Fast);

	/* 配置DIN口线为推挽输出，缺省输出高电平 */
	 GPIO_Init(PORT_DIN, PIN_DIN , GPIO_Mode_Out_PP_Low_Fast);

	/* 配置DOUT口线为输入浮空模式 */
	 GPIO_Init(PORT_DOUT, PIN_DOUT, GPIO_Mode_In_PU_No_IT);

	/* 配置DRDY口线为输入上拉模式 */
	 GPIO_Init(PORT_DRDY, PIN_DRDY, GPIO_Mode_In_FL_No_IT);

 
	 bsp_DelayMS(10);
       /* 硬件复位 */
	 TM7705_ResetHard();		
	
       /*在接口序列丢失的情况下，如果在DIN 高电平的写操作持续了足够长的时间（至少 32个串行时钟周期），
	TM7705 将会回到默认状态。*/	
	bsp_DelayMS(5);
       /* 同步SPI接口时序 */
	TM7705_SyncSPI();		
	bsp_DelayMS(5);

      /* 配置时钟寄存器 */
	TM7705_WriteByte(REG_CLOCK | WRITE | CH_1);	        /* 先写通信寄存器，下一步是写时钟寄存器 */
	TM7705_WriteByte(CLKDIS_0 | CLK_2_4576M | FS_50HZ);	/* 刷新速率50Hz */

      /* 每次上电进行一次自校准 */
	TM7705_CalibSelf(1);	/* 内部自校准 CH1 */
	bsp_DelayMS(50);
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Delay
*	功能说明: CLK之间的延迟，时序延迟. 用于STM8L  16M主频
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TM7705_Delay(void)
{
	uint16_t i;

	for (i = 0; i < 3; i++);
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_ResetHard
*	功能说明: 硬件复位 TM7705芯片
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TM7705_ResetHard(void)
{

	bsp_DelayMS(1);
	bsp_DelayMS(3);
	bsp_DelayMS(1);
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_SyncSPI
*	功能说明: 同步TM7705芯片SPI接口时序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TM7705_SyncSPI(void)
{
	/* AD7705串行接口失步后将其复位。复位后要延时500us再访问 */
	
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);

}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Send8Bit
*	功能说明: 向SPI总线发送8个bit数据。 不带CS控制。
*	形    参: _data : 数据
*	返 回 值: 无
*********************************************************************************************************
*/
static void TM7705_Send8Bit(uint8_t _data)
{
	uint8_t i;

	for(i = 0; i < 8; i++)
	{
		if (_data & 0x80)
		{
			DIN_1();
		}
		else
		{
			DIN_0();
		}
		SCK_0();
		_data <<= 1;
    	       TM7705_Delay();
              delay_us(50);
           //    bsp_DelayMS(1);
		SCK_1();
		TM7705_Delay();
               delay_us(50);
           //     bsp_DelayMS(1);
	}
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Recive8Bit
*	功能说明: 从SPI总线接收8个bit数据。 不带CS控制。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static uint8_t TM7705_Recive8Bit(void)
{
	uint8_t i;
	uint8_t read = 0;

	for (i = 0; i < 8; i++)
	{
		SCK_0();
            //    bsp_DelayMS(1);
                delay_us(50);
		TM7705_Delay();
		read = read<<1;
		if (DOUT_IS_HIGH())
		{
			read++;
		}
		SCK_1();
            //    bsp_DelayMS(1);
                delay_us(50);
		TM7705_Delay();
	}
	return read;
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_WriteByte
*	功能说明: 写入1个字节。带CS控制
*	形    参: _data ：将要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
static void TM7705_WriteByte(uint8_t _data)
{
	
	TM7705_Send8Bit(_data);
	
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Write3Byte
*	功能说明: 写入3个字节。带CS控制
*	形    参: _data ：将要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
static void TM7705_Write3Byte(uint32_t _data)
{

	TM7705_Send8Bit((_data >> 16) & 0xFF);
	TM7705_Send8Bit((_data >> 8) & 0xFF);
	TM7705_Send8Bit(_data);
	
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_ReadByte
*	功能说明: 从AD芯片读取一个字（16位）
*	形    参: 无
*	返 回 值: 读取的字（16位）
*********************************************************************************************************
*/
static uint8_t TM7705_ReadByte(void)
{
	uint8_t read;

	
	read = TM7705_Recive8Bit();
	

	return read;
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Read2Byte
*	功能说明: 读2字节数据
*	形    参: 无
*	返 回 值: 读取的数据（16位）
*********************************************************************************************************
*/
static uint16_t TM7705_Read2Byte(void)
{
	uint16_t read;

	
	read = TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	

	return read;
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Read3Byte
*	功能说明: 读3字节数据
*	形    参: 无
*	返 回 值: 读取到的数据（24bit) 高8位固定为0.
*********************************************************************************************************
*/
static uint32_t TM7705_Read3Byte(void)
{
	uint32_t read;

	
	read = TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	
	return read;
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_WaitDRDY
*	功能说明: 等待内部操作完成。 自校准时间较长，需要等待。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TM7705_WaitDRDY(void)
{
	uint32_t i;

	for (i = 0; i < 400000; i++)
	{
		if (DRDY_IS_LOW())
		{
			break;
		}
	}
        if(i >= 400000)
        {
         	TM7705_SyncSPI();  

		bsp_DelayMS(5);
        }
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_WriteReg
*	功能说明: 写指定的寄存器
*	形    参:  _RegID : 寄存器ID
*			  _RegValue : 寄存器值。 对于8位的寄存器，取32位形参的低8bit
*	返 回 值: 无
*********************************************************************************************************
*/
void TM7705_WriteReg(uint8_t _RegID, uint32_t _RegValue)
{
	uint8_t bits;

	switch (_RegID)
	{
		case REG_COMM:		/* 通信寄存器 */
		case REG_SETUP:		/* 设置寄存器 8bit */
		case REG_CLOCK:		/* 时钟寄存器 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 偏移寄存器 24bit */
		case REG_FULL_CH1:	/* CH1 满量程寄存器 24bit */
		case REG_ZERO_CH2:	/* CH2 偏移寄存器 24bit */
		case REG_FULL_CH2:	/* CH2 满量程寄存器 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* 数据寄存器 16bit */
		default:
			return;
	}

	TM7705_WriteByte(_RegID | WRITE);/* 写通信寄存器, 指定下一步是写操作，并指定写哪个寄存器 */

	if (bits == 8)
	{
		TM7705_WriteByte((uint8_t)_RegValue);
	}
	else	/* 24bit */
	{
		TM7705_Write3Byte(_RegValue);
	}
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_ReadReg
*	功能说明: 写指定的寄存器
*	形    参:  _RegID : 寄存器ID
*			  _RegValue : 寄存器值。 对于8位的寄存器，取32位形参的低8bit
*	返 回 值: 读到的寄存器值。 对于8位的寄存器，取32位形参的低8bit
*********************************************************************************************************
*/
uint32_t TM7705_ReadReg(uint8_t _RegID)
{
	uint8_t bits;
	uint32_t read;

	switch (_RegID)
	{
		case REG_COMM:		/* 通信寄存器 */
		case REG_SETUP:		/* 设置寄存器 8bit */
		case REG_CLOCK:		/* 时钟寄存器 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 偏移寄存器 24bit */
		case REG_FULL_CH1:	/* CH1 满量程寄存器 24bit */
		case REG_ZERO_CH2:	/* CH2 偏移寄存器 24bit */
		case REG_FULL_CH2:	/* CH2 满量程寄存器 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* 数据寄存器 16bit */
		default:
			return 0xFFFFFFFF;
	}

	TM7705_WriteByte(_RegID | READ);	/* 写通信寄存器, 指定下一步是写操作，并指定写哪个寄存器 */

	if (bits == 16)
	{
		read = TM7705_Read2Byte();
	}
	else if (bits == 8)
	{
		read = TM7705_ReadByte();
	}
	else	/* 24bit */
	{
		read = TM7705_Read3Byte();
	}
	return read;
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_CalibSelf
*	功能说明: 启动自校准. 内部自动短接AIN+ AIN-校准0位，内部短接到Vref 校准满位。此函数执行过程较长，
*			  实测约 180ms
*	形    参:  _ch : ADC通道，1或2
*	返 回 值: 无
*********************************************************************************************************
*/
void TM7705_CalibSelf(uint8_t _ch)
{
	if (_ch == 1)
	{
		/* 自校准CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* 写通信寄存器，下一步是写设置寄存器，通道1 */
		TM7705_WriteByte(MD_CAL_SELF | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* 启动自校准 */
		TM7705_WaitDRDY();	/* 等待内部操作完成 --- 时间较长，约180ms */
	}
	else if (_ch == 2)
	{
		/* 自校准CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* 写通信寄存器，下一步是写设置寄存器，通道2 */
		TM7705_WriteByte(MD_CAL_SELF | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* 启动自校准 */
		TM7705_WaitDRDY();	/* 等待内部操作完成  --- 时间较长，约180ms */
	}
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_SytemCalibZero
*	功能说明: 启动系统校准零位. 请将AIN+ AIN-短接后，执行该函数。校准应该由主程序控制并保存校准参数。
*			 执行完毕后。可以通过 TM7705_ReadReg(REG_ZERO_CH1) 和  TM7705_ReadReg(REG_ZERO_CH2) 读取校准参数。
*	形    参: _ch : ADC通道，1或2
*	返 回 值: 无
*********************************************************************************************************
*/
void TM7705_SytemCalibZero(uint8_t _ch)
{
	if (_ch == 1)
	{
		/* 校准CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* 写通信寄存器，下一步是写设置寄存器，通道1 */
		TM7705_WriteByte(MD_CAL_ZERO | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* 启动自校准 */
		TM7705_WaitDRDY();	/* 等待内部操作完成 */
	}
	else if (_ch == 2)
	{
		/* 校准CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* 写通信寄存器，下一步是写设置寄存器，通道1 */
		TM7705_WriteByte(MD_CAL_ZERO | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* 启动自校准 */
		TM7705_WaitDRDY();	/* 等待内部操作完成 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_SytemCalibFull
*	功能说明: 启动系统校准满位. 请将AIN+ AIN-接最大输入电压源，执行该函数。校准应该由主程序控制并保存校准参数。
*			 执行完毕后。可以通过 TM7705_ReadReg(REG_FULL_CH1) 和  TM7705_ReadReg(REG_FULL_CH2) 读取校准参数。
*	形    参:  _ch : ADC通道，1或2
*	返 回 值: 无
*********************************************************************************************************
*/
void TM7705_SytemCalibFull(uint8_t _ch)
{
	if (_ch == 1)
	{
		/* 校准CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* 写通信寄存器，下一步是写设置寄存器，通道1 */
		TM7705_WriteByte(MD_CAL_FULL | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* 启动自校准 */
		TM7705_WaitDRDY();	/* 等待内部操作完成 */
	}
	else if (_ch == 2)
	{
		/* 校准CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* 写通信寄存器，下一步是写设置寄存器，通道1 */
		TM7705_WriteByte(MD_CAL_FULL | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* 启动自校准 */
		TM7705_WaitDRDY();	/* 等待内部操作完成 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_ReadAdc1
*	功能说明: 读通道1或2的ADC数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t TM7705_ReadAdc(uint8_t _ch)
{
	uint8_t i;
	uint16_t read = 0;

	/* 为了避免通道切换造成读数失效，读2次 */
	for (i = 0; i < 1; i++)
	{
		TM7705_WaitDRDY();		/* 等待DRDY口线为0 */

		if (_ch == 1)
		{
			TM7705_WriteByte(0x38);
		}
		else if (_ch == 2)
		{
			TM7705_WriteByte(0x39);
		}

		read = TM7705_Read2Byte();
        }
	
	return read;
}

/*******************************************************************************************************/