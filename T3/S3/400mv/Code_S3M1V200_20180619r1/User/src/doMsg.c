/*=============================================================================
//File:			domsg.c	
//Author:		tpcon 
//Version:	    V0.1
//Date:			2014/04
//Dese:			消息处理
===============================================================================*/

#include "domsg.h"

/*消息盒中的消息状态*/
static  u8   l_u8MsgBox[MAX_MSGS] = {0, 0, 0, 0};   

/********************************************************************
* Summary:    设置某类型的消息存在
* Parameters:
*             u8MsgID   消息类型
*             u8Param   消息状态  ENABLE 存在  DIENABLE
* Return :    该消息类型的消息状态  1存在  0不存在
********************************************************************/
u8   PostMsg(u8 u8MsgID, u8 u8Param)
{
    if (u8MsgID >= MAX_MSGS)
    {
        return 0;
    }

    /*查询是否有同类型的消息未被处理*/
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



/********************************************************************
* Summary:    获取是否有消息，且是否销毁该消息
* Parameters:
*             ucMsgID   消息类型
              ucClear   是否销毁该类型消息
*       
* Return :    该消息类型的消息状态  1存在  0不存在
********************************************************************/
u8   GetMsg(u8 u8MsgID, u8 u8Clear)
{
    u8   u8Msg;
    
    if (u8MsgID >= MAX_MSGS)
    {
        return 0;
    }

    u8Msg = l_u8MsgBox[u8MsgID];
    
    /*存在消息且配置了销毁选项*/
    if((u8Clear == 1) && (u8Msg != 0))                                            
    {
        l_u8MsgBox[u8MsgID] = 0;
    }    
    
    return  u8Msg;
}





/********************************************************************
* Summary:      销毁某种类型的消息
* Parameters:   
*               u8MsgID   消息类型
*         
* Return :      void
********************************************************************/
void    ClearMsg(u8 u8MsgID)
{
    if (u8MsgID >= MAX_MSGS)
    {
        return;
    }

    l_u8MsgBox[u8MsgID] = 0;
}
