/*******************************************************************************
* COPYRIGHT 2014 TPCon.com
* File Name          : key.c
* Author             : 
* Version            : V0.1
* Date               : 2014/04
* Description        : 4*5������̣���20������ 
*******************************************************************************/

#include "key.h"
#include "timer.h"
#include <string.h>

#define		GPIO_KEYLINE1				GPIOG
#define		PIN_KEYLINE1				GPIO_Pin_3

#define		GPIO_KEYLINE2				GPIOE
#define		PIN_KEYLINE2				GPIO_Pin_1

#define		GPIO_KEYLINE3				GPIOE
#define		PIN_KEYLINE3				GPIO_Pin_2

#define		GPIO_KEYLINE4				GPIOE
#define		PIN_KEYLINE4				GPIO_Pin_3

#define		GPIO_KEYLINE5				GPIOE
#define		PIN_KEYLINE5				GPIO_Pin_4

#define		GPIO_KEYROW1				GPIOE
#define		PIN_KEYROW1					GPIO_Pin_5

#define		GPIO_KEYROW2				GPIOD
#define		PIN_KEYROW2					GPIO_Pin_0

#define		GPIO_KEYROW3				GPIOD
#define		PIN_KEYROW3					GPIO_Pin_1

#define		GPIO_KEYROW4				GPIOD
#define		PIN_KEYROW4					GPIO_Pin_2

#define		BTN_NUM         	        20            //��ť����
#define 	KEY_NUM         	        20            //��������������ϼ�
#define 	KEYACC_TIMES    	        2
#define 	HOLD_DELAY      	        1
#define 	KEYHOLD_TIMES			    9
		
static  u8	l_u8BtnStat[BTN_NUM];      
static  u8  l_u8KeyStat[KEY_NUM];
					
static  u8  l_u8KeyName;
static  u8  l_u8KeyAct;
static  u8  l_u8KeyMsg;
static  u8  l_u8KeyHold;
static  u8  l_u8HoldDly;

static KeyMsg  l_keyMsg;

const static u8 constKeyMap[] = 
{   
    KEY_UP1,
	KEY_DOWN1,
    KEY_SOURCE,
    KEY_NONE,
    KEY_UP2,
	KEY_DOWN2,
    KEY_SFUNC,
	KEY_SRANGE,
    KEY_UP3,
	KEY_DOWN3,
    KEY_MFUNC,
    KEY_SWEEP,
    KEY_UP4,
	KEY_DOWN4,
    KEY_MRANGE,
    KEY_PROG,
	KEY_PN,
	KEY_DOT,
	KEY_MESURE,
	KEY_TEMP,
};

/****************************************************************
// Summary: 	ROW��Ѳ
// Parameter: 	
//
// return:		�޷���
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
			break;
		case 1:
			GPIO_SetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_ResetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_SetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_SetBits(GPIO_KEYROW4, PIN_KEYROW4);
			break;
		case 2:
			GPIO_SetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_SetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_ResetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_SetBits(GPIO_KEYROW4, PIN_KEYROW4);
			break;
		case 3:
			GPIO_SetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_SetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_SetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_ResetBits(GPIO_KEYROW4, PIN_KEYROW4);
			break;
		default:
			GPIO_SetBits(GPIO_KEYROW1, PIN_KEYROW1);
			GPIO_SetBits(GPIO_KEYROW2, PIN_KEYROW2);
			GPIO_SetBits(GPIO_KEYROW3, PIN_KEYROW3);
			GPIO_SetBits(GPIO_KEYROW4, PIN_KEYROW4);
			break;
	}
}

/****************************************************************
// Summary: 	��ȡ��ť״̬
// Parameter: 	
//
// return:		�޷���
****************************************************************/
static	void GetBtnStat(void)
{
    u32 i;
    
    for(i = 0; i < 4; i++)
    {
        //KEY ROW���״̬
        KEYROW_OutPut(i);
        //delay_us(5);
    
        //��ѯKEYLine1״̬
        if(GPIO_ReadInputDataBit(GPIO_KEYLINE1, PIN_KEYLINE1) == 0)
            l_u8BtnStat[i] = BTN_STAT_ON;
        else
            l_u8BtnStat[i] = BTN_STAT_OFF;
        
        //��ѯKEYLine2״̬
        if(GPIO_ReadInputDataBit(GPIO_KEYLINE2, PIN_KEYLINE2) == 0)
            l_u8BtnStat[i + 4] = BTN_STAT_ON;
        else
            l_u8BtnStat[i + 4] = BTN_STAT_OFF;
        
        //��ѯKEYLine3״̬
        if(GPIO_ReadInputDataBit(GPIO_KEYLINE3, PIN_KEYLINE3) == 0)
            l_u8BtnStat[i + 8] = BTN_STAT_ON;
        else
            l_u8BtnStat[i + 8] = BTN_STAT_OFF;
                
        //��ѯKEYLine4״̬
        if(GPIO_ReadInputDataBit(GPIO_KEYLINE4, PIN_KEYLINE4) == 0)
            l_u8BtnStat[i + 12] = BTN_STAT_ON;
        else
            l_u8BtnStat[i + 12] = BTN_STAT_OFF;
                
        //��ѯKEYLine5״̬
        if(GPIO_ReadInputDataBit(GPIO_KEYLINE5, PIN_KEYLINE5) == 0)
            l_u8BtnStat[i + 16] = BTN_STAT_ON;
        else
            l_u8BtnStat[i + 16] = BTN_STAT_OFF;
    }
}

/****************************************************************
// Summary: 	��ѯ����״̬, ������Ϣ
// Parameter:   [in/u8]u8Key		����
//				[in/u8]u8BtnStat	��ť״̬
//
// return:		0����Ϣ 1 ����Ϣ 2HOLD���
****************************************************************/
static u8 QryKeyStat(u8 u8Key, u8 u8Btn)
{
    u8 u8Msg = 0;
    
    if(l_u8BtnStat[u8Btn] == BTN_STAT_ON)                   //��ť�պ�
    {
        if(l_u8KeyStat[u8Key] == KEY_STAT_OFF)              //OFF->ON:DOWN
        {
            l_u8KeyStat[u8Key] = KEY_STAT_ON;	  									//KEY����
            l_u8KeyAct = KEY_ACT_DOWN;
            u8Msg = 1;
        }
        else                                                //ON->n*ON:HOLD
        {
            l_u8KeyStat[u8Key] = KEY_STAT_ON;
            l_u8KeyAct = KEY_ACT_NONE;		 										//���ְ���״̬ 
            if(l_u8KeyHold >= KEYACC_TIMES)											//�����������									
            {
                l_u8KeyHold = KEYACC_TIMES;
                if(l_u8HoldDly >= HOLD_DELAY)										//����ʱ�� ���ΰ���
                {
		            l_u8KeyAct = KEY_ACT_HOLD;
		            u8Msg = 1;
		            l_u8HoldDly = 0;
                }
                else
                {
                    l_u8HoldDly ++;
                    u8Msg = 2;                                                      //��������HOLD��ѯ̬
                } 
            }
            else
            {
                l_u8KeyHold ++;
                u8Msg = 2;                                                         	//��������HOLD��ѯ̬
            }
        }
    }
    else                                                                            //��ťOFF
    {
        if(l_u8KeyStat[u8Key] == KEY_STAT_ON)                                       //ON->OFF:UP
        {
            l_u8KeyStat[u8Key] = KEY_STAT_OFF;
            l_u8KeyAct = KEY_ACT_UP;
            u8Msg = 1;
            l_u8KeyHold = 0;
            l_u8HoldDly = 0;
        }
        else                                                                        //OFF->OFF
        {
            l_u8KeyAct = KEY_ACT_NONE;
        }
    }

    return u8Msg;    
}

/****************************************************************
// Summary: 	���ü���״̬
// Parameter:  
//
// return:		[void]
****************************************************************/
void SetKeyStat(void)
{
    u32  i;
    u8   u8Key;
    
    l_u8KeyName = KEY_NONE;

    //����״̬��ѯ 
    for(i = 0; i < BTN_NUM; i++)
    {
        u8Key = constKeyMap[i];
        l_u8KeyMsg = QryKeyStat(u8Key, i);
        
        if(l_u8KeyMsg >= 1)                                                         //����Ϣ��������������
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

/////////////////////////////////////////////////ȫ�ֺ���///////////////////////////////////////////////////
/****************************************************************
// Summary: 	������� ���ø���״̬
// Parameter:  	[void]
//
// return:		[void]
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

/****************************************************************
// Summary: 	��ȡ������Ϣ
// Parameter:  	[void]
//
// return:		[out/u8]
****************************************************************/
u8 GetKeyMsg(void)
{
    return l_u8KeyMsg;
}

/****************************************************************
// Summary: 	�õ�����Ϣ
// Parameter:  	[out/PKeyMsg]pKeyMsg  	������Ϣ�����ṹ��
//
// return:		[out]l_u8KeyMsg			ȫ�ֵİ�����Ϣ
****************************************************************/
void GetKeyMsgEx(PKeyMsg pKeyMsg)                                            
{
    pKeyMsg->u8KeyName = l_keyMsg.u8KeyName;
    pKeyMsg->u8KeyAct = l_keyMsg.u8KeyAct;
    pKeyMsg->u8KeyStat = l_keyMsg.u8KeyStat;
	l_u8KeyMsg = 0;
}

/****************************************************************
// Summary: 	��ʼ������
// Parameter:  	[void]
//
// return:		[void]
****************************************************************/
void InitKey(void)
{
    //��ʼ������
    GPIO_Init(GPIO_KEYLINE1, PIN_KEYLINE1, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIO_KEYLINE2, PIN_KEYLINE2, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIO_KEYLINE3, PIN_KEYLINE3, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIO_KEYLINE4, PIN_KEYLINE4, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIO_KEYLINE5, PIN_KEYLINE5, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIO_KEYROW1, PIN_KEYROW1, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIO_KEYROW2, PIN_KEYROW2, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIO_KEYROW3, PIN_KEYROW3, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIO_KEYROW4, PIN_KEYROW4, GPIO_Mode_Out_PP_High_Slow);  
  
    l_u8KeyMsg = 0;
    l_u8KeyHold = 0;
    l_u8HoldDly = 0;
    l_u8KeyName = KEY_NONE;
    l_u8KeyAct = KEY_ACT_NONE;
    
    memset(l_u8KeyStat, 0, KEY_NUM);
    memset(l_u8BtnStat, 0, BTN_NUM);
}

