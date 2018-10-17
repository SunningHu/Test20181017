/**
  ******************************************************************************
  * @file    main.h
  * @author  Microcontroller Division
  * @version V1.2.0
  * @date    06/2017
  * @brief   Main program body
  ******************************************************************************
**/
#ifndef _MAIN_H__
#define _MAIN_H__
#include "stm8l15x.h"
#include "stm8l15x_clk.h"
#include "timer.h"
#include "key.h"
#include "usart1.h"
#include "stm8l15x_usart.h"
#include "battery.h"
#include "measure.h"
#include "lcd.h"
#include "stm8l15x_i2c.h"
#include "gui.h"
#include "viewMain.h"
#include "stm8l15x_tim3.h"
#include "pwm.h"
#include "dodata.h"
#include "viewFrame.h"
#include "domsg.h"
#include "adcmd.h"
#include "stm8l15x_iwdg.h"
#include "dac8552.h"
#include "dac8811.h"
#include "DriveEEprom.h"
#include "AD7705.h"






void clk_init(void);

#endif