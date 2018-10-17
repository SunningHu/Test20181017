/*=============================================================================
//File:			battery.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04 
//Dese:			��ع���
===============================================================================*/
#include "battery.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_adc.h"
#include "gui.h"
#include "lcd.h"
#include "dodata.h"

//��س����ɱ�־
#define		BATTERY_CHARGEOK_PORT			GPIOA
#define		BATTERY_CHARGEOK_PIN		    GPIO_Pin_7

//�ⲿ��Դ�߽�ͨ��־
#define		BATTERY_CHARGE_PORT			    GPIOA
#define		BATTERY_CHARGE_PIN				GPIO_Pin_3

//POWER����
#define		POWER_PORT			            GPIOB
#define		POWER_PIN				        GPIO_Pin_0

//Auto power off��־
#define		AUTOPOWEROFF_PORT			    GPIOE
#define		AUTOPOWEROFF_PIN				GPIO_Pin_7

//Summary:	�����ؿ��߳�ʼ��
void	Battery_Init(void)
{
    GPIO_Init(BATTERY_CHARGE_PORT, BATTERY_CHARGE_PIN, GPIO_Mode_In_FL_No_IT);
    GPIO_Init(BATTERY_CHARGEOK_PORT, BATTERY_CHARGEOK_PIN, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(POWER_PORT, POWER_PIN, GPIO_Mode_In_FL_No_IT);
    GPIO_Init(AUTOPOWEROFF_PORT, AUTOPOWEROFF_PIN, GPIO_Mode_In_PU_No_IT);
}

//Summary:	��ȡ���״̬��־
static u8	GetBatteryChargeFlg(void)
{
  u8  u8BatteryChargeFlg;
    
  u8BatteryChargeFlg = GPIO_ReadInputDataBit(BATTERY_CHARGE_PORT, BATTERY_CHARGE_PIN);    //���״̬��־
  
  return u8BatteryChargeFlg;
}

//Summary:	��ȡ������״̬��־
static u8	GetBatteryChargeOKFlg(void)
{
  u8  u8BatteryChargeOKFlg;
    
  u8BatteryChargeOKFlg = GPIO_ReadInputDataBit(BATTERY_CHARGEOK_PORT, BATTERY_CHARGEOK_PIN);    //�����ɱ�־
  
  return u8BatteryChargeOKFlg;
}


//Summary:	��Դ���أ�10�����޲������Զ��ػ�
void	PowerOff(void)
{
    static u8 u8Flg = 0;
    static u8  u8AutoPowerOff;
    static u8  u8AutoPowerOffPre = 0;
    
    if(GetBatteryChargeFlg() != 0)  //���״̬
    {
      if(adCode.u16BatteryCode >= 3290) //������
      {
        Write_PowerFlg(ON);
      }
      else
      {
        u8Flg = !u8Flg;
        Write_PowerFlg(u8Flg);
      }
      LCDBlink(OFF);
    }
    else
    {
      Write_PowerFlg(OFF);
      u8AutoPowerOff = GPIO_ReadInputDataBit(AUTOPOWEROFF_PORT, AUTOPOWEROFF_PIN);  //�Զ��ػ���־
    
      if(u8AutoPowerOff != u8AutoPowerOffPre) //���뿪��״̬�ı�
      {
        data.u16KeyTime = 0;
      }
      u8AutoPowerOffPre = u8AutoPowerOff;
      
      if(u8AutoPowerOff != 0)     //�����뿪����Auto power offλ����Ч
      {
        if(data.u16KeyTime >= 570) //9��30�����޶�������ʼ��˸
        {
          LCDBlink(ON);              //��˸
          if(data.u16KeyTime >= 600)   //10���ӣ��ر�
          {
            GPIO_Init(POWER_PORT, POWER_PIN, GPIO_Mode_Out_PP_High_Slow);
          }
        }
          
      }
      else        //�ر���˸
      {
        LCDBlink(OFF);
      }
    }
}


//Summary:	�������ͣ��Զ��ػ�
void	LowBatteryPowerOff(void)
{
    GPIO_Init(POWER_PORT, POWER_PIN, GPIO_Mode_Out_PP_High_Slow);
}

void    ViewBatteryLevel(u16 u16BatteryVoltage)
{
  if(u16BatteryVoltage > BATTERY_LEVEL3)
    Write_BatteryLevel(3);
  else if(u16BatteryVoltage > BATTERY_LEVEL2)
    Write_BatteryLevel(2);
  else if(u16BatteryVoltage > BATTERY_LEVEL1)
    Write_BatteryLevel(1);
  else if(u16BatteryVoltage > BATTERY_LEVEL0)
    Write_BatteryLevel(0);
  else  //�������ͣ��ػ�
  {
    if(GetBatteryChargeFlg() == 0)  //���״̬
    {
      Write_BatteryLevel(0);
    }
    else
    {
      Write_BatteryLevel(0);
      LowBatteryPowerOff();     //���ڳ��״̬����ػ�
    }
  }
    
}



