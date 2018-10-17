/*******************************************************************************
* COPYRIGHT 2014 TPCon.com
* File Name          : key.h
* Author             : tpcon 
* Version            : V0.1
* Date               : 2014/04
* Description        : 按键头文件
*******************************************************************************/

#ifndef _KEY_H_
#define _KEY_H_

#include "stm8l15x.h"
									    
#define BTN0            0
#define BTN1            1
#define BTN2            2
#define BTN3            3
#define BTN4            4
#define BTN5            5
#define BTN6            6
#define BTN7            7
#define BTN8            8
#define BTN9            9
#define BTN10           10
#define BTN11           11
#define BTN12           12
#define BTN13           13
#define BTN14           14
#define BTN15           15
#define BTN16           16
#define BTN17           17
#define BTN18           18
#define BTN19           19

#define KEY_NONE    	0xFF
#define	KEY_PN			0				//正负
#define	KEY_DOT			1
#define KEY_UP1	        2
#define KEY_DOWN1      	3
#define KEY_UP2	    	4
#define KEY_DOWN2      	5
#define KEY_UP3	    	6
#define KEY_DOWN3      	7
#define KEY_UP4	    	8
#define KEY_DOWN4      	9
#define	KEY_MESURE		10
#define	KEY_MFUNC		11
#define	KEY_MRANGE		12
#define	KEY_SOURCE		13
#define	KEY_SFUNC		14
#define	KEY_SRANGE		15
#define	KEY_TEMP		16
#define	KEY_PROG		17
#define	KEY_SWEEP		18


//BTN STATUS
#define BTN_STAT_OFF    0
#define BTN_STAT_ON     1

//KEY STATUS
#define KEY_STAT_ON     1
#define KEY_STAT_OFF    0

//KEY ACTION
#define KEY_ACT_NONE    0
#define KEY_ACT_DOWN    1
#define KEY_ACT_UP      2
#define KEY_ACT_HOLD    3

#define KEY_ACT_NONE		0
#define	KEY_ACT_PRESS		1
#define	KEY_ACT_RELEA		2

//按键描绘结构体
typedef struct _tagKeyMsg
{
    u8 u8KeyName;
    u8 u8KeyAct;
    u8 u8KeyStat;
}KeyMsg, *PKeyMsg;

void ScanKey(void);	   																//浏览按键
u8 GetKeyMsg(void);																	//获取按键信息
void GetKeyMsgEx(PKeyMsg pKeyMsg);													//获取按键描绘信息
void InitKey(void);																	//初始化键盘


#endif
