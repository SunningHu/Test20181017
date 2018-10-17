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
									    

#define KEY_NONE    	0xFF
#define	KEY_PN			0				//正负
#define	KEY_DOT			1
#define KEY_NUM0	        2
#define KEY_NUM1	        3
#define KEY_NUM2               	4
#define KEY_NUM3	    	5
#define KEY_NUM4     	        6
#define KEY_NUM5        	7
#define KEY_NUM6        	8
#define KEY_NUM7     	        9
#define KEY_NUM8        	10
#define KEY_NUM9        	11
#define	KEY_MESURE		12
#define	KEY_MFUNC		13
#define	KEY_MRANGE		14
#define	KEY_SOURCE		15
#define	KEY_SFUNC		16
#define	KEY_SRANGE		17
#define	KEY_TEMP		18
#define	KEY_PROG		19
#define	KEY_SWEEP		20
#define KEY_OK                  21
#define KEY_DEL                 22
#define KEY_BLIGHT              23
#define KEY_HOLD                24
#define KEY_SHANG               25
#define KEY_XIA                 26
#define KEY_MODE                27
#define KEY_NONE1               28
#define KEY_NONE2               29
#define KEY_NONE3               30
#define KEY_NONE4               31


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
#define KEY_ACT_LONG    3


//按键描绘结构体
typedef struct _tagKeyMsg
{
    u8 u8KeyName;     //按键名称
    u8 u8KeyAct;      //按键动作
    u8 u8KeyStat;     //按键状态
}KeyMsg, *PKeyMsg;

void ScanKey(void);	   																//浏览按键
u8 GetKeyMsg(void);																	//获取按键信息
void GetKeyMsgEx(PKeyMsg pKeyMsg);													//获取按键描绘信息
void InitKey(void);																	//初始化键盘


#endif
