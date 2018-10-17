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
#define		BATTERY_CHARGEOK_PORT		GPIOD
#define		BATTERY_CHARGEOK_PIN		    GPIO_Pin_6

//�ⲿ��Դ�߽�ͨ��־
#define		BATTERY_CHARGE_PORT	        GPIOC
#define		BATTERY_CHARGE_PIN	            GPIO_Pin_6

//POWER����
#define		POWER_PORT			 GPIOA
#define		POWER_PIN		            GPIO_Pin_4




/*******************************************************
* Summary:    ������IO�ڳ�ʼ��
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

////Summary:	��ȡ���״̬��־    1���ڳ��    0û���
static u8 GetBatteryChargeFlg(void)
{
  u8  u8BatteryChargeFlg;
    
  u8BatteryChargeFlg = GPIO_ReadInputDataBit(BATTERY_CHARGE_PORT, BATTERY_CHARGE_PIN);    //���״̬��־
  
  return u8BatteryChargeFlg;
}
//
////Summary:	��ȡ������״̬��־   1���   0���ڳ��
//static u8 GetBatteryChargeOKFlg(void)
//{
//  u8  u8BatteryChargeOKFlg;
//    
//  u8BatteryChargeOKFlg = GPIO_ReadInputDataBit(BATTERY_CHARGEOK_PORT, BATTERY_CHARGEOK_PIN);    //�����ɱ�־
//  
//  return u8BatteryChargeOKFlg;
//}

//Summary:	�������ͣ��Զ��ػ�
void	LowBatteryPowerOff(void)
{
    GPIO_Init(POWER_PORT, POWER_PIN, GPIO_Mode_Out_PP_High_Slow);
}



/********************************************************************
* Summary:    ��Դ���أ�10�����޲������Զ��ػ�
* Parameters:
*             u8AutoPowerOff   �Զ��ػ�   0�Զ�  1 �ֶ�
*       
* Return :    void
********************************************************************/
void	PowerOff(u8 u8AutoPowerOff)
{
    
    
    if(GetBatteryChargeFlg() == 0)  //���״̬
    {

      Write_PowerFlg(OFF);  
   
      if(u8AutoPowerOff == 0)     //�Զ��ػ�
      {
        if(data.u16KeyTime >= 570) //9��30�����޶�������ʼ��˸
        {
          LCDBlink(ON);              //��˸
          if(data.u16KeyTime >= 600)   //10���ӣ��ر�
          {
            LowBatteryPowerOff();  //����
          }
        }
          
      }
      else        //�ر���˸
      {
        LCDBlink(OFF); 
      }
    }
}


/********************************************************************
* Summary:     ��ص�����ʾ
* Parameters:
*             u16BatteryVoltage     ��ص�ѹ��ֵ
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
  else  //�������ͣ��ػ�
  {
        Write_BatteryLevel(0); 
        u8Flg = !u8Flg;
        Write_BatteryLine(u8Flg);
//    if(GetBatteryChargeFlg() != 0)  //���״̬
//    {
//      Write_BatteryLevel(0);
//    }
//    else
//    {
//      Write_BatteryLevel(0);
//      LowBatteryPowerOff();     //���ڳ��״̬����ػ�
//    }
  }
    
}



