/*******************************************************************************
* COPYRIGHT 2014 TPCon.com
* File Name          : key.c
* Author             : 
* Version            : V0.1
* Date               : 2014/04
* Description        : 4*5矩阵键盘，共20个按键 
*******************************************************************************/

#include "key.h"
#include "timer.h"
#include <string.h>
#include "dodata.h"

#define		GPIO_KEYLINE1				GPIOG
#define		PIN_KEYLINE1				GPIO_Pin_5

#define		GPIO_KEYLINE2				GPIOG
#define		PIN_KEYLINE2				GPIO_Pin_4

#define		GPIO_KEYLINE3				GPIOE
#define		PIN_KEYLINE3				GPIO_Pin_6

#define		GPIO_KEYLINE4				GPIOE
#define		PIN_KEYLINE4				GPIO_Pin_7


#define		GPIO_KEYROW1				GPIOF
#define		PIN_KEYROW1					GPIO_Pin_1

#define		GPIO_KEYROW2				GPIOF
#define		PIN_KEYROW2					GPIO_Pin_4

#define		GPIO_KEYROW3				GPIOF
#define		PIN_KEYROW3					GPIO_Pin_5

#define		GPIO_KEYROW4				GPIOF
#define		PIN_KEYROW4					GPIO_Pin_6

#define		GPIO_KEYROW5				GPIOF
#define		PIN_KEYROW5				        GPIO_Pin_7

#define		GPIO_KEYROW6				GPIOG
#define		PIN_KEYROW6				        GPIO_Pin_7

#define		GPIO_KEYROW7				GPIOD
#define		PIN_KEYROW7				        GPIO_Pin_5

#define		GPIO_KEYROW8				GPIOG
#define		PIN_KEYROW8				        GPIO_Pin_6



#define		BTN_NUM         	        32            //按钮数量
#define 	KEY_NUM         	        32            //键数量，包括组合键
#define 	KEYSHORT_TIMES    	        1
#define         KEYLONG_TIMES                   20


		
static  u8	l_u8BtnStat[BTN_NUM];      
static  u8  l_u8KeyStat[KEY_NUM];
					
static  u8  l_u8KeyName;
static  u8  l_u8KeyAct;     //按键状态
static  u8  l_u8KeyTime;
static  u8  l_u8KeyMsg;

static KeyMsg  l_keyMsg;

const static u8 constKeyMap[] = 
{   
    KEY_DEL,
	KEY_MESURE,
        KEY_NONE1,
    KEY_NUM1,
    KEY_NUM4,
	KEY_NUM7,
    KEY_PN,
	KEY_PROG,
    KEY_BLIGHT,
	KEY_MFUNC,
    KEY_MRANGE,
    KEY_NUM2,
    KEY_NUM5,
	KEY_NUM8,
        KEY_NUM0,
        KEY_TEMP,
    KEY_HOLD,
    KEY_SFUNC,
    KEY_SRANGE,
	KEY_NUM3,
        KEY_NUM6,
	KEY_NUM9,
	KEY_DOT,
	KEY_SWEEP,
        KEY_NONE2,   
        KEY_SOURCE,
        KEY_NONE3,
        KEY_SHANG,
        KEY_XIA,
        KEY_MODE,
        KEY_OK,
        KEY_NONE4, 
};


/****************************************************************
// Summary: 	ROW轮巡
// Parameter: 	
//
// return:		无返回
****************************************************************/
static	void KEYROW_OutPut(u8 u8Row)
{
	switch(u8Row)
	{
		case 0:
			GPIO_ResetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_SetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_SetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_SetBits(GPIO_KEYROW4, PIN_KEYROW4);
                        GPIO_SetBits(GPIO_KEYROW5, PIN_KEYROW5);
                        GPIO_SetBits(GPIO_KEYROW6, PIN_KEYROW6);
                        GPIO_SetBits(GPIO_KEYROW7, PIN_KEYROW7);
                        GPIO_SetBits(GPIO_KEYROW8, PIN_KEYROW8);
			break;
		case 1:
			GPIO_SetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_ResetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_SetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_SetBits(GPIO_KEYROW4, PIN_KEYROW4);
                        GPIO_SetBits(GPIO_KEYROW5, PIN_KEYROW5);
                        GPIO_SetBits(GPIO_KEYROW6, PIN_KEYROW6);
                        GPIO_SetBits(GPIO_KEYROW7, PIN_KEYROW7); 
                        GPIO_SetBits(GPIO_KEYROW8, PIN_KEYROW8);
			break;
		case 2:
			GPIO_SetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_SetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_ResetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_SetBits(GPIO_KEYROW4, PIN_KEYROW4);
                        GPIO_SetBits(GPIO_KEYROW5, PIN_KEYROW5);
                        GPIO_SetBits(GPIO_KEYROW6, PIN_KEYROW6);
                        GPIO_SetBits(GPIO_KEYROW7, PIN_KEYROW7);  
                        GPIO_SetBits(GPIO_KEYROW8, PIN_KEYROW8);
			break;
		case 3:
			GPIO_SetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_SetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_SetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_ResetBits(GPIO_KEYROW4, PIN_KEYROW4);
                        GPIO_SetBits(GPIO_KEYROW5, PIN_KEYROW5);
                        GPIO_SetBits(GPIO_KEYROW6, PIN_KEYROW6);
                        GPIO_SetBits(GPIO_KEYROW7, PIN_KEYROW7);  
                        GPIO_SetBits(GPIO_KEYROW8, PIN_KEYROW8);
			break;
		case 4:
			GPIO_SetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_SetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_SetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_SetBits(GPIO_KEYROW4, PIN_KEYROW4);
                        GPIO_ResetBits(GPIO_KEYROW5, PIN_KEYROW5);
                        GPIO_SetBits(GPIO_KEYROW6, PIN_KEYROW6);
                        GPIO_SetBits(GPIO_KEYROW7, PIN_KEYROW7);   
                        GPIO_SetBits(GPIO_KEYROW8, PIN_KEYROW8);  
			break;
		case 5:
			GPIO_SetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_SetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_SetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_SetBits(GPIO_KEYROW4, PIN_KEYROW4);
                        GPIO_SetBits(GPIO_KEYROW5, PIN_KEYROW5);
                        GPIO_ResetBits(GPIO_KEYROW6, PIN_KEYROW6);
                        GPIO_SetBits(GPIO_KEYROW7, PIN_KEYROW7);  
                        GPIO_SetBits(GPIO_KEYROW8, PIN_KEYROW8);
			break;
		case 6:
			GPIO_SetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_SetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_SetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_SetBits(GPIO_KEYROW4, PIN_KEYROW4);
                        GPIO_SetBits(GPIO_KEYROW5, PIN_KEYROW5);
                        GPIO_SetBits(GPIO_KEYROW6, PIN_KEYROW6);
                        GPIO_ResetBits(GPIO_KEYROW7, PIN_KEYROW7);
                        GPIO_SetBits(GPIO_KEYROW8, PIN_KEYROW8);
			break;    
		case 7:
			GPIO_SetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_SetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_SetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_SetBits(GPIO_KEYROW4, PIN_KEYROW4);
                        GPIO_SetBits(GPIO_KEYROW5, PIN_KEYROW5);
                        GPIO_SetBits(GPIO_KEYROW6, PIN_KEYROW6);
                        GPIO_SetBits(GPIO_KEYROW7, PIN_KEYROW7);   
                        GPIO_ResetBits(GPIO_KEYROW8, PIN_KEYROW8);
			break;                        
		default:
			GPIO_SetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_SetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_SetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_SetBits(GPIO_KEYROW4, PIN_KEYROW4);
                        GPIO_SetBits(GPIO_KEYROW5, PIN_KEYROW5);
                        GPIO_SetBits(GPIO_KEYROW6, PIN_KEYROW6);
                        GPIO_SetBits(GPIO_KEYROW7, PIN_KEYROW7); 
                        GPIO_SetBits(GPIO_KEYROW8, PIN_KEYROW8);
			break;
	}
}

/****************************************************************
// Summary: 	获取按钮状态
// Parameter: 	void
//
// return:      无返回
****************************************************************/
static	void GetBtnStat(void)
{
    u32 i;
    
    for(i = 0; i < 8; i++)
    {
        //KEY ROW输出状态
        KEYROW_OutPut(i);
    
        //查询KEYLine1状态
        if(GPIO_ReadInputDataBit(GPIO_KEYLINE1, PIN_KEYLINE1) == 0)
            l_u8BtnStat[i] = BTN_STAT_ON;     
        else
            l_u8BtnStat[i] = BTN_STAT_OFF;
        
        //查询KEYLine2状态
        if(GPIO_ReadInputDataBit(GPIO_KEYLINE2, PIN_KEYLINE2) == 0)
            l_u8BtnStat[i + 8] = BTN_STAT_ON;             
        else
            l_u8BtnStat[i + 8] = BTN_STAT_OFF;
        
        //查询KEYLine3状态
        if(GPIO_ReadInputDataBit(GPIO_KEYLINE3, PIN_KEYLINE3) == 0)
            l_u8BtnStat[i + 16] = BTN_STAT_ON;          
        else
            l_u8BtnStat[i + 16] = BTN_STAT_OFF;
                
        //查询KEYLine4状态
        if(GPIO_ReadInputDataBit(GPIO_KEYLINE4, PIN_KEYLINE4) == 0)
          l_u8BtnStat[i + 24] = BTN_STAT_ON;                               
        else
            l_u8BtnStat[i + 24] = BTN_STAT_OFF;
                
    }
}

/****************************************************************
// Summary: 	查询单键状态, 返回消息
// Parameter:   [in/u8]u8Key		键名
//				[in/u8]u8BtnStat	按钮状态
//
// return:		0无消息 1 有消息 2LONG检测
****************************************************************/
static u8 QryKeyStat(u8 u8Key, u8 u8Btn)
{
    u8 u8Msg = 0;
    
    if(l_u8BtnStat[u8Btn] == BTN_STAT_ON)                   //按钮闭合
    {
        l_u8KeyTime++;
        if(l_u8KeyTime >= KEYSHORT_TIMES)
        {

            if(l_u8KeyStat[u8Key] == KEY_STAT_OFF)              //OFF->ON:DOWN
            {
                l_u8KeyStat[u8Key] = KEY_STAT_ON;	  	//按键按下
                l_u8KeyAct = KEY_ACT_DOWN;
                u8Msg = 1;     
            }
            else                                                //ON->n*ON:LONG
            {
                l_u8KeyStat[u8Key] = KEY_STAT_ON;
                l_u8KeyAct = KEY_ACT_NONE;		 	//保持按下状态 
                if(l_u8KeyTime >= KEYLONG_TIMES)	        //超过1.5s									
                {
                    l_u8KeyTime = KEYLONG_TIMES;
                    l_u8KeyAct = KEY_ACT_LONG;                
                    u8Msg = 1;                        
                }
                else
                {             
                    u8Msg = 2;                                                        
                }
            }
        }
    }
    else                                                                          
    {
        
        if(l_u8KeyStat[u8Key] == KEY_STAT_ON)                    //按键抬起                   
        {
            l_u8KeyStat[u8Key] = KEY_STAT_OFF;
            l_u8KeyAct = KEY_ACT_UP;
            data.u8KeyLongUpF = 0;
            u8Msg = 1;
            l_u8KeyTime = 0;
        }
        else                                                         
        {
            l_u8KeyAct = KEY_ACT_NONE;
        }
    }

    return u8Msg;    
}

/****************************************************************
// Summary: 	设置键盘状态
// Parameter:  
//
// return:		[void]
****************************************************************/
void SetKeyStat(void)
{
    u32  i;
   static   u8   u8Key = 0;
    
    l_u8KeyName = KEY_NONE;

    //单键状态查询 
    for(i = 0; i < BTN_NUM; i++)
    {
        u8Key = constKeyMap[i];
        l_u8KeyMsg = QryKeyStat(u8Key, i);
        
        if(l_u8KeyMsg >= 1)                                                         //有消息，忽略其他按键
        {
            if(l_u8KeyMsg == 1)
            {
                l_u8KeyName = u8Key;
                
            }
                
            if(l_u8KeyMsg == 2)
                l_u8KeyMsg = 0;
                
            break;
        }
    }
}


/****************************************************************
// Summary: 	  按键扫描
// Parameter:  	  [void]
//
// return:        [void]
****************************************************************/
void ScanKey(void)
{
    GetBtnStat();
    SetKeyStat();
    
    if(l_u8KeyMsg)
    {
        l_keyMsg.u8KeyName = l_u8KeyName;
        l_keyMsg.u8KeyAct = l_u8KeyAct;
        l_keyMsg.u8KeyStat = 0;
    }
}


/********************************************************************
* Summary:    获取按键消息
* Parameters:
*             void
*       
* Return :    void
********************************************************************/
u8 GetKeyMsg(void)
{
    return l_u8KeyMsg;
}


/********************************************************************
* Summary:    得到键消息
* Parameters:
*             pKeyMsg     按键消息描述结构体
*       
* Return :    l_u8KeyMsg  全局的按键消息
********************************************************************/
void GetKeyMsgEx(PKeyMsg pKeyMsg)                                            
{
    pKeyMsg->u8KeyName = l_keyMsg.u8KeyName;
    pKeyMsg->u8KeyAct = l_keyMsg.u8KeyAct;
    pKeyMsg->u8KeyStat = l_keyMsg.u8KeyStat;
	l_u8KeyMsg = 0;
}


/********************************************************************
* Summary:    初始化按键
* Parameters:
*             void
*       
* Return :    void
********************************************************************/
void InitKey(void)
{
    //初始化口线
    GPIO_Init(GPIO_KEYLINE1, PIN_KEYLINE1, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIO_KEYLINE2, PIN_KEYLINE2, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIO_KEYLINE3, PIN_KEYLINE3, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIO_KEYLINE4, PIN_KEYLINE4, GPIO_Mode_In_PU_No_IT);
    
    GPIO_Init(GPIO_KEYROW1, PIN_KEYROW1, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIO_KEYROW2, PIN_KEYROW2, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIO_KEYROW3, PIN_KEYROW3, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIO_KEYROW4, PIN_KEYROW4, GPIO_Mode_Out_PP_High_Slow);  
    GPIO_Init(GPIO_KEYROW5, PIN_KEYROW5, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIO_KEYROW6, PIN_KEYROW6, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIO_KEYROW7, PIN_KEYROW7, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIO_KEYROW8, PIN_KEYROW8, GPIO_Mode_Out_PP_High_Slow);
  
    l_u8KeyMsg = 0;
    l_u8KeyTime = 0;
    l_u8KeyName = KEY_NONE;
    l_u8KeyAct = KEY_ACT_NONE;
    
    memset(l_u8KeyStat, 0, KEY_NUM);
    memset(l_u8BtnStat, 0, BTN_NUM);
}

