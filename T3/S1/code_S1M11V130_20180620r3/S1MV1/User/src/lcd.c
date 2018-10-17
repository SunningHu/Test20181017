/*=============================================================================
//File:			lcd.c	
//Author:		tpcon 
//Version:	    V0.1
//Date:			2014/04
//Dese:			Һ��������
===============================================================================*/
#include "lcd.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_i2c.h"
#include "timer.h"
#include "gui.h"
#include <string.h>


//�������
#define		LCD_BRIGHT_PORT			    GPIOB
#define		LCD_BRIGHT_PIN				GPIO_Pin_2

//SLAVE ADDRESS
#define     HT16C23_READ                0x7D
#define     HT16C23_WRITE               0x7C

//���RAM��ַ
#define     MAXRAM_4DUTY                0x1B
#define     MAXRAM_8DUTY                0x33

//��ʾ����
u8  LCD_Buf[MAXRAM_4DUTY + 1];

//HT16C23����
#define     CMD_WRITERAM                0x80                //��ʾ��������/�������
#define     CMD_DRIVEMODE               0x82                //����ģʽ����,Duty��Bias����
#define     CMD_SYSMODE                 0x84                //ϵͳģʽ����,ϵͳ��������ʾ����/�ر�����
#define     CMD_FRSET                   0x86                //֡Ƶ������
#define     CMD_BLINK                   0x88                //��˸Ƶ������
#define     CMD_IVA                     0x8A                //�ڲ���ѹ����(IVA)��������

//���ò���
#define     DUTY4BIAS3                  0x00                //1/4 duty 1/3 bias
#define     DUTY4BIAS4                  0x01                //1/4 duty 1/4 bias
#define     DUTY8BIAS3                  0x02                //1/8 duty 1/3 bias
#define     DUTY8BIAS4                  0x03                //1/8 duty 1/4 bias

#define     OSCOFF_LCDOFF               0x00                //ϵͳ����OFF,LCD OFF
#define     OSCON_LCDOFF                0x02                //ϵͳ����ON,LCD OFF
#define     OSCON_LCDON                 0x03                //ϵͳ����ON,LCD ON

#define     FR_80HZ                     0x00                //80Hz
#define     FR_160HZ                    0x01                //160Hz

#define     BLINK_OFF                   0x00                //��˸�ر�
#define     BLINK_2HZ                   0x01                //��˸Ƶ��2Hz
#define     BLINK_1HZ                   0x02                //��˸Ƶ��1Hz
#define     BLINK_05HZ                  0x03                //��˸Ƶ��0.5Hz

#define     IVA_VLCDOFF                 0x00                //�ڲ���ѹ��������OFF
#define     IVA_VLCDON                  0x10                //�ڲ���ѹ��������ON
#define     IVA_SEG55OFF                0x20                //
#define     IVA_SEG55ON                 0x30                //

//16��VLCD��ѹ
#define     VLCD_V1                     0x00
#define     VLCD_V2                     0x01
#define     VLCD_V3                     0x02
#define     VLCD_V4                     0x03
#define     VLCD_V5                     0x04
#define     VLCD_V6                     0x05
#define     VLCD_V7                     0x06
#define     VLCD_V8                     0x07
#define     VLCD_V9                     0x08
#define     VLCD_V10                    0x09
#define     VLCD_V11                    0x0A
#define     VLCD_V12                    0x0B
#define     VLCD_V13                    0x0C
#define     VLCD_V14                    0x0D
#define     VLCD_V15                    0x0E
#define     VLCD_V16                    0x0F

//Summary:	д����
void	HT16C23_WriteConfig(u8 u8Cmdbyte, u8 u8CmdSet)
{
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, HT16C23_WRITE, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, u8Cmdbyte);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData(I2C1, u8CmdSet);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);
}

//Summary:	д����RAM
void	HT16C23_WriteRAM_Ones(u8 u8RegAddr, u8 u8Data)
{
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, HT16C23_WRITE, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, CMD_WRITERAM);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData(I2C1, u8RegAddr);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData(I2C1, u8Data);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);
}

//Summary:	д���RAM
void	HT16C23_WriteRAM(u8 u8RegAddr, u8* pu8Data, u8 u8Len)
{
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, HT16C23_WRITE, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, CMD_WRITERAM);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData(I2C1, u8RegAddr);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    while(u8Len)
    {
      I2C_SendData(I2C1, *pu8Data);
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
      pu8Data ++;
      u8Len --;
    }
    I2C_GenerateSTOP(I2C1, ENABLE);
}

//Summary:	��RAM
void	HT16C23_ReadRAM(u8 u8RegAddr, u8* pu8Data, u8 u8Len)
{    
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, HT16C23_WRITE, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, CMD_WRITERAM);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData(I2C1, u8RegAddr);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);
    
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, HT16C23_READ, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR);
        
    while(u8Len)
    {
        if(u8Len == 1)
            I2C_AcknowledgeConfig(I2C1, DISABLE);
        if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            *pu8Data = I2C_ReceiveData(I2C1);
            pu8Data ++;
            u8Len--;
        }            
    }
    
    I2C_GenerateSTOP(I2C1, ENABLE);    
}

//Summary:	Һ������ʼ��
void	TPCON_LCD_Init(void)
{    
    //I2C��ʼ��
    GPIO_Init(GPIOC, GPIO_Pin_1 | GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast);
    delay_ms(10);
    CLK_PeripheralClockConfig(CLK_Peripheral_I2C1,ENABLE);
    I2C_Init(I2C1, 100000, 0xA0, I2C_Mode_I2C, I2C_DutyCycle_2, I2C_Ack_Enable,I2C_AcknowledgedAddress_7bit);
    I2C_Cmd(I2C1,ENABLE);
    delay_ms(100);    
    //HT16C23��ʼ��
    GPIO_Init(LCD_BRIGHT_PORT, LCD_BRIGHT_PIN, GPIO_Mode_Out_PP_High_Slow); //��������
    memset(LCD_Buf, 0xFF, sizeof(LCD_Buf));
    HT16C23_WriteConfig(CMD_DRIVEMODE, DUTY4BIAS3);         //1/4 duty, 1/3 bias
    HT16C23_WriteConfig(CMD_FRSET, FR_80HZ);                //80Hz
    HT16C23_WriteConfig(CMD_IVA, IVA_VLCDOFF);               //�ڲ���ѹ����OFF
    HT16C23_WriteConfig(CMD_BLINK, BLINK_OFF);              //��˸�ر�
    HT16C23_WriteRAM(0x00, LCD_Buf, sizeof(LCD_Buf));     //дRAM����,����
    HT16C23_WriteConfig(CMD_SYSMODE, OSCON_LCDON);          //��ʾ���� 
    delay_ms(1000);        
}



//Summary:	�л�����,60���ް����������ر���
void	SetLCDBright(u16 u16KeyTime)
{
    if(u16KeyTime >= 120)        //2�����ް����������ر���
    {
        GPIO_ResetBits(LCD_BRIGHT_PORT, LCD_BRIGHT_PIN);  
    }
    else
    {
        GPIO_SetBits(LCD_BRIGHT_PORT, LCD_BRIGHT_PIN);
    }       
}

//Summary:	��Ļ��˸
void	LCDBlink(u8 u8Blink)
{
  if(u8Blink == ON)
    HT16C23_WriteConfig(CMD_BLINK, BLINK_1HZ);
  else
    HT16C23_WriteConfig(CMD_BLINK, BLINK_OFF);
}

//Summary:	ȫ����ʾ
void	LCDClr(void)
{
  memset(LCD_Buf, 0x00, sizeof(LCD_Buf));
  HT16C23_WriteRAM(0x00, LCD_Buf, sizeof(LCD_Buf));     //дRAM����,����
}