/*=============================================================================
//File:			battery.c	
//Author:		tpcon
//Version:	    V0.1
//Date:			2014/04 
//Dese:			电池管理
===============================================================================*/
#include "battery.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_adc.h"
#include "gui.h"
#include "lcd.h"
#include "dodata.h"

//电池充电完成标志
#define		BATTERY_CHARGEOK_PORT			GPIOA
#define		BATTERY_CHARGEOK_PIN		    GPIO_Pin_7

//外部电源线接通标志
#define		BATTERY_CHARGE_PORT			    GPIOA
#define		BATTERY_CHARGE_PIN				GPIO_Pin_3

//POWER软开关
#define		POWER_PORT			            GPIOB
#define		POWER_PIN				        GPIO_Pin_0

//Auto power off标志
#define		AUTOPOWEROFF_PORT			    GPIOE
#define		AUTOPOWEROFF_PIN				GPIO_Pin_7

//Summary:	电池相关口线初始化
void	Battery_Init(void)
{
    GPIO_Init(BATTERY_CHARGE_PORT, BATTERY_CHARGE_PIN, GPIO_Mode_In_FL_No_IT);
    GPIO_Init(BATTERY_CHARGEOK_PORT, BATTERY_CHARGEOK_PIN, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(POWER_PORT, POWER_PIN, GPIO_Mode_In_FL_No_IT);
    GPIO_Init(AUTOPOWEROFF_PORT, AUTOPOWEROFF_PIN, GPIO_Mode_In_PU_No_IT);
}

//Summary:	获取充电状态标志
static u8	GetBatteryChargeFlg(void)
{
  u8  u8BatteryChargeFlg;
    
  u8BatteryChargeFlg = GPIO_ReadInputDataBit(BATTERY_CHARGE_PORT, BATTERY_CHARGE_PIN);    //充电状态标志
  
  return u8BatteryChargeFlg;
}

//Summary:	获取充电完成状态标志
static u8	GetBatteryChargeOKFlg(void)
{
  u8  u8BatteryChargeOKFlg;
    
  u8BatteryChargeOKFlg = GPIO_ReadInputDataBit(BATTERY_CHARGEOK_PORT, BATTERY_CHARGEOK_PIN);    //充电完成标志
  
  return u8BatteryChargeOKFlg;
}


//Summary:	电源软开关，10分钟无操作，自动关机
void	PowerOff(void)
{
    static u8 u8Flg = 0;
    static u8  u8AutoPowerOff;
    static u8  u8AutoPowerOffPre = 0;
    
    if(GetBatteryChargeFlg() != 0)  //充电状态
    {
      if(adCode.u16BatteryCode >= 3290) //充电完成
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
      u8AutoPowerOff = GPIO_ReadInputDataBit(AUTOPOWEROFF_PORT, AUTOPOWEROFF_PIN);  //自动关机标志
    
      if(u8AutoPowerOff != u8AutoPowerOffPre) //拨码开关状态改变
      {
        data.u16KeyTime = 0;
      }
      u8AutoPowerOffPre = u8AutoPowerOff;
      
      if(u8AutoPowerOff != 0)     //仅拨码开关在Auto power off位置有效
      {
        if(data.u16KeyTime >= 570) //9分30秒钟无动作，开始闪烁
        {
          LCDBlink(ON);              //闪烁
          if(data.u16KeyTime >= 600)   //10分钟，关闭
          {
            GPIO_Init(POWER_PORT, POWER_PIN, GPIO_Mode_Out_PP_High_Slow);
          }
        }
          
      }
      else        //关闭闪烁
      {
        LCDBlink(OFF);
      }
    }
}


//Summary:	电量过低，自动关机
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
  else  //电量过低，关机
  {
    if(GetBatteryChargeFlg() == 0)  //充电状态
    {
      Write_BatteryLevel(0);
    }
    else
    {
      Write_BatteryLevel(0);
      LowBatteryPowerOff();     //不在充电状态，则关机
    }
  }
    
}



