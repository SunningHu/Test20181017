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
#define		BATTERY_CHARGEOK_PORT		GPIOD
#define		BATTERY_CHARGEOK_PIN		    GPIO_Pin_6

//外部电源线接通标志
#define		BATTERY_CHARGE_PORT	        GPIOC
#define		BATTERY_CHARGE_PIN	            GPIO_Pin_6

//POWER软开关
#define		POWER_PORT			 GPIOA
#define		POWER_PIN		            GPIO_Pin_4




/*******************************************************
* Summary:    电池相关IO口初始化
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
void	BatteryGPIO_Init(void)
{
    GPIO_Init(BATTERY_CHARGE_PORT, BATTERY_CHARGE_PIN, GPIO_Mode_In_FL_No_IT);
    GPIO_Init(BATTERY_CHARGEOK_PORT, BATTERY_CHARGEOK_PIN, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(POWER_PORT, POWER_PIN, GPIO_Mode_In_FL_No_IT);
}

////Summary:	获取充电状态标志    1正在充电    0没充电
static u8 GetBatteryChargeFlg(void)
{
  u8  u8BatteryChargeFlg;
    
  u8BatteryChargeFlg = GPIO_ReadInputDataBit(BATTERY_CHARGE_PORT, BATTERY_CHARGE_PIN);    //充电状态标志
  
  return u8BatteryChargeFlg;
}
//
////Summary:	获取充电完成状态标志   1完成   0正在充电
//static u8 GetBatteryChargeOKFlg(void)
//{
//  u8  u8BatteryChargeOKFlg;
//    
//  u8BatteryChargeOKFlg = GPIO_ReadInputDataBit(BATTERY_CHARGEOK_PORT, BATTERY_CHARGEOK_PIN);    //充电完成标志
//  
//  return u8BatteryChargeOKFlg;
//}

//Summary:	电量过低，自动关机
void	LowBatteryPowerOff(void)
{
    GPIO_Init(POWER_PORT, POWER_PIN, GPIO_Mode_Out_PP_High_Slow);
}



/********************************************************************
* Summary:    电源软开关，10分钟无操作，自动关机
* Parameters:
*             u8AutoPowerOff   自动关机   0自动  1 手动
*       
* Return :    void
********************************************************************/
void	PowerOff(u8 u8AutoPowerOff)
{
    
    
    if(GetBatteryChargeFlg() == 0)  //充电状态
    {

      Write_PowerFlg(OFF);  
   
      if(u8AutoPowerOff == 0)     //自动关机
      {
        if(data.u16KeyTime >= 570) //9分30秒钟无动作，开始闪烁
        {
          LCDBlink(ON);              //闪烁
          if(data.u16KeyTime >= 600)   //10分钟，关闭
          {
            LowBatteryPowerOff();  //软开关
          }
        }
          
      }
      else        //关闭闪烁
      {
        LCDBlink(OFF); 
      }
    }
}


/********************************************************************
* Summary:     电池电量显示
* Parameters:
*             u16BatteryVoltage     电池电压码值
*       
* Return :      Void
********************************************************************/
void    ViewBatteryLevel(u16 u16BatteryVoltage)
{
  static u8  u8Flg = 0;
  
  if(u16BatteryVoltage > BATTERY_LEVEL4)
    Write_BatteryLevel(4);
  else if(u16BatteryVoltage > BATTERY_LEVEL3)
    Write_BatteryLevel(3);
  else if(u16BatteryVoltage > BATTERY_LEVEL2)
    Write_BatteryLevel(2);
  else if(u16BatteryVoltage > BATTERY_LEVEL1)
    Write_BatteryLevel(1);
  else if(u16BatteryVoltage > BATTERY_LEVEL0)
    Write_BatteryLevel(0);  
  else  //电量过低，关机
  {
        Write_BatteryLevel(0); 
        u8Flg = !u8Flg;
        Write_BatteryLine(u8Flg);
//    if(GetBatteryChargeFlg() != 0)  //充电状态
//    {
//      Write_BatteryLevel(0);
//    }
//    else
//    {
//      Write_BatteryLevel(0);
//      LowBatteryPowerOff();     //不在充电状态，则关机
//    }
  }
    
}



