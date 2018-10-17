/*=============================================================================
//File:			domsg.c	
//Author:		tpcon 
//Version:	    V0.1
//Date:			2014/04
//Dese:			��Ϣ����
===============================================================================*/

#include "domsg.h"

/*��Ϣ���е���Ϣ״̬*/
static  u8   l_u8MsgBox[MAX_MSGS] = {0, 0, 0, 0};                   

u8   PostMsg(u8 u8MsgID, u8 u8Param)
{
    if (u8MsgID >= MAX_MSGS)
    {
        return 0;
    }

    /*��ѯ�Ƿ���ͬ���͵���Ϣδ������*/
    if(l_u8MsgBox[u8MsgID] == 0)                                                    
    {
        l_u8MsgBox[u8MsgID] = u8Param;
        return 1;
    }
    else
    {
        return 0;
    }
}

/*��ȡ�Ƿ�����Ϣ�����Ƿ����ٸ���Ϣ
 *
 *parameter
  *ucMsgID:��Ϣ����
  *ucClear:�Ƿ����ٸ�������Ϣ
 *
 *return
  *uchar:����Ϣ���͵���Ϣ״̬
 */
u8   GetMsg(u8 u8MsgID, u8 u8Clear)
{
    u8   u8Msg;
    
    if (u8MsgID >= MAX_MSGS)
    {
        return 0;
    }

    u8Msg = l_u8MsgBox[u8MsgID];
    
    /*������Ϣ������������ѡ��*/
    if((u8Clear == 1) && (u8Msg != 0))                                            
    {
        l_u8MsgBox[u8MsgID] = 0;
    }    
    
    return  u8Msg;
}

/*����ĳ����Ϣ���͵���Ϣ
 *
 *parameter
  *@ucMsgID:��Ϣ����
 *
 *return:none
 */
void    ClearMsg(u8 u8MsgID)
{
    if (u8MsgID >= MAX_MSGS)
    {
        return;
    }

    l_u8MsgBox[u8MsgID] = 0;
}
