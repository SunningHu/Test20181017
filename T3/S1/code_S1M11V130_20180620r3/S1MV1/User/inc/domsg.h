/*=============================================================================
//File:			domsg.h
//Author:		tpcon 
//Version:	    V0.1
//Date:			2014/04
//Dese:			��Ϣ����ͷ�ļ�
===============================================================================*/

#ifndef _DOMSG_H_
#define _DOMSG_H_

#include	"stm8l15x.h"

#define         MSG_PAINT           0           //ˢ��
#define         MSG_1S              1           //1S
#define         MSG_100MS           2           //100ms
#define         MSG_ADOK            3           //ADת����ɱ�־ 

#define         MAX_MSGS            4       //�����Ϣ��

u8   PostMsg(u8 u8MsgID, u8 u8Param);
u8   GetMsg(u8 u8MsgID, u8 u8Clear);
void    ClearMsg(u8 u8MsgID);
                                                                                                 

#endif