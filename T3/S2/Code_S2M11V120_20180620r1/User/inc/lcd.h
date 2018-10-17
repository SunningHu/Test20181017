/*=============================================================================
//File:			lcd.h	
//Author:		tpcon 
//Version:	    V0.1
//Date:			2014/04
//Dese:			Òº¾§ÆÁÇý¶¯
===============================================================================*/

#ifndef __LCD_H
#define __LCD_H

#include	"stm8l15x.h"

//LCD±³¹â
#define     LCDBRIGHT_ON            0
#define     LCDBRIGHT_OFF           1

void	TPCON_LCD_Init(void);
void	SetLCDBright(u16 u16KeyTime);
void	HT16C23_ReadRAM(u8 u8RegAddr, u8* pu8Data, u8 u8Len);
void	HT16C23_WriteRAM(u8 u8RegAddr, u8* pu8Data, u8 u8Len);
void	HT16C23_WriteRAM_Ones(u8 u8RegAddr, u8 u8Data);
void	HT16C23_WriteConfig(u8 u8Cmdbyte, u8 u8CmdSet);
void	LCDBlink(u8 u8Blink);
void	LCDClr(void);

#endif
