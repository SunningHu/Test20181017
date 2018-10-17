/*=============================================================================
//File:			adcmd.c	
//Author:		tpcon 
//Version:	    V0.1
//Date:			2013-8-5
//Dese:			AD通讯协议相关函数
===============================================================================*/
#include "stm8l15x.h"
#include "dodata.h"
#include "adcmd.h"
#include "stm8l15x_flash.h"
#include "viewMain.h"
#include <string.h>

#define     EEPROM_ADDR_PRODUCT     512

//extern void    DrawSourceData(void);

extern	u8  g_ucUsartBuf[50];		//串口缓存
extern	u8  g_ucUsartLen;				//串口接收到的数据长度

u16     g_u16SourceCalPWM = 0;
u8      g_u8SourceCalSig = 0;

u8      g_u8SourceCalFlg = 0;       //输出标定标志
CalPwm  calPwm;
Product product;

static	const u8 ucCRCHi[] = 
{
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;

static	const u8 ucCRCLo[] =
{
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
} ;

//Summary:		CRC校验
//Parameter:
//Retern:
static	u16	CalCrc(u8 *pucData , u16 usDataLen)
{
	u8 ucCrcLo = 0xFF ; 
	u8 ucCrcHi = 0xFF ; 
	u8 ucIndex ;
	
	while(usDataLen--) 
	{
		ucIndex = ucCrcLo ^ *pucData++ ;
		ucCrcLo = ucCrcHi ^ ucCRCHi[ucIndex] ;
		ucCrcHi = ucCRCLo[ucIndex] ;
	};
	
	return (ucCrcHi * 0x100 + ucCrcLo) ;
}


//Summary:		存储数据到EEPROM
//Parameter:
//Return:
static void	SaveCaliData(u16 u16Addr, u8* u8Buf, u16 u16Len)
{
  u16 i;
  FLASH_Unlock(FLASH_MemType_Data);
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
  {}
  
  for(i = 0; i < u16Len; i++)
  {
    FLASH_ProgramByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + u16Addr, u8Buf[i]);
    u16Addr ++;
  }
  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET)
  {}
  FLASH_Lock(FLASH_MemType_Data);
}

//Summary:		读取数据到内存
//Parameter:
//Return:
static void	LoadCaliData(u16 u16Addr, u8* u8Buf, u16 u16Len)
{
  u16 i;
  
  FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
  
  FLASH_Unlock(FLASH_MemType_Data);
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
  {}
  
  for(i = 0; i < u16Len; i++)
  {
    u8Buf[i] = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + u16Addr);
    u16Addr ++;
  }
  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET)
  {}
  FLASH_Lock(FLASH_MemType_Data);
}

//Summary:		初始化标定数据
//Parameter:
//Return:
void	InitCaliData(void)
{
  LoadCaliData(0, (u8*)&calPwm.u16SCalPwm_mA[0], sizeof(CalPwm));
}

//Summary:		错误返回命令0x80
//Parameter:
//Return:
static void	DoCmd80(void)
{   
    u16 u16Crc;
    g_ucUsartLen = 5;
    
    g_ucUsartBuf[0] = 0x01;
    g_ucUsartBuf[1] = 0x80;
    g_ucUsartBuf[2] = 0x01;    
    u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
    g_ucUsartBuf[3] = u16Crc % 0x100;
    g_ucUsartBuf[4] = u16Crc / 0x100;
}

#define		S1_VERSION		"S1M1V130"

//Summary:			读取AD版本号		
//Parameter:
//Return:
static	void	DoCmd10(void)
{
  u16 u16Crc;
  g_ucUsartLen = 14;
  
  g_ucUsartBuf[0] = g_ucUsartBuf[0];         // 地址
  g_ucUsartBuf[1] = g_ucUsartBuf[1];         // 命令
  g_ucUsartBuf[2] = g_ucUsartBuf[2];         // 保留
  g_ucUsartBuf[3] = 0x08;                   // 长度
  memcpy(g_ucUsartBuf + 4, S1_VERSION, 8);
  
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[12] = u16Crc % 0x100;
  g_ucUsartBuf[13] = u16Crc / 0x100;
}

//Summary:			
//Parameter:
//Return:
static void	DoCmd11(void)
{
   u16 u16Crc;
   
   SetMSigPort(SIGTYPE_END); //关闭测量
   
   //切换信号类型 
  if(g_ucUsartBuf[2] < 0x04)//20mA
    data.u8SSigType = SIGTYPE_MA;
  else if(g_ucUsartBuf[2] < 0x06)//20mV
    data.u8SSigType = SIGTYPE_20MV;
  else if(g_ucUsartBuf[2] < 0x08)//100mV
    data.u8SSigType = SIGTYPE_100MV;
  else//V
    data.u8SSigType = SIGTYPE_V;
  
  //DrawSourceData();
  
  //处理PWM输出    
  memcpy(&calPwm.u16SCalPwm_mA[0] + g_ucUsartBuf[2], &g_ucUsartBuf[3], 2);
  DoSourceCali(g_ucUsartBuf[2]/2, &calPwm.u16SCalPwm_mA[0] + g_ucUsartBuf[2]);   //输出
  SaveCaliData(g_ucUsartBuf[2] * 2, &g_ucUsartBuf[3], 2);
  
  g_ucUsartLen = 6;   
  g_ucUsartBuf[0] = g_ucUsartBuf[0];
  g_ucUsartBuf[1] = g_ucUsartBuf[1];
  g_ucUsartBuf[2] = g_ucUsartBuf[2];
  g_ucUsartBuf[3] = 0x01;
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[4] = u16Crc % 0x100;
  g_ucUsartBuf[5] = u16Crc / 0x100;
  
  g_u8SourceCalFlg = 1;
}

//Summary:			
//Parameter:
//Return:
static void	DoCmd12(void)
{
  u16 u16Crc;
  
  //处理PWM输出  
  memcpy(&calPwm.fSCalPwm_mA[0] + g_ucUsartBuf[2], &g_ucUsartBuf[3], 4);
  SaveCaliData(20 + g_ucUsartBuf[2] * 4, &g_ucUsartBuf[3], 4);
  
  g_ucUsartLen = 6;   
  g_ucUsartBuf[0] = g_ucUsartBuf[0];
  g_ucUsartBuf[1] = g_ucUsartBuf[1];
  g_ucUsartBuf[2] = g_ucUsartBuf[2];
  g_ucUsartBuf[3] = 0x01;
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[4] = u16Crc % 0x100;
  g_ucUsartBuf[5] = u16Crc / 0x100;
  
  g_u8SourceCalFlg = 0;
}

//Summary:			
//Parameter:
//Return:
static void	DoCmd13(void)
{
   u16 u16Crc;
   
   SetSSigPort(SIGTYPE_END);
   
  //切换信号类型 
  if(g_ucUsartBuf[2] == 0x00)//20mA
    data.u8MSigType = SIGTYPE_MA;
  else if(g_ucUsartBuf[2] == 0x01)//20mV
    data.u8MSigType = SIGTYPE_20MV;
  else if(g_ucUsartBuf[2] == 0x02)//100mV
    data.u8MSigType = SIGTYPE_100MV;
  else//V
    data.u8MSigType = SIGTYPE_V;
  
  //DrawMeasureData();
  
  g_ucUsartLen = 7;   
  g_ucUsartBuf[0] = g_ucUsartBuf[0];
  g_ucUsartBuf[1] = g_ucUsartBuf[1];
  g_ucUsartBuf[2] = g_ucUsartBuf[2];
  if(g_ucUsartBuf[2] == 0x00)//20mA
  {
    g_ucUsartBuf[3] = adCode.u16mACode / 0x100;
    g_ucUsartBuf[4] = adCode.u16mACode % 0x100;
  }
  else if(g_ucUsartBuf[2] == 0x01 || g_ucUsartBuf[2] == 0x02)//20mV/100mV
  {
    g_ucUsartBuf[3] = adCode.u16mVCode / 0x100;
    g_ucUsartBuf[4] = adCode.u16mVCode % 0x100;
  }
  else
  {
    g_ucUsartBuf[3] = adCode.u16VCode / 0x100;
    g_ucUsartBuf[4] = adCode.u16VCode % 0x100;
  }  
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[5] = u16Crc % 0x100;
  g_ucUsartBuf[6] = u16Crc / 0x100;
}

//Summary:			
//Parameter:
//Return:
static void	DoCmd14(void)
{
  u16 u16Crc;
  
  //处理PWM输出  
  memcpy(&calPwm.u16MCal_mA[0] + g_ucUsartBuf[2], &g_ucUsartBuf[3], 2);
  SaveCaliData(60 + g_ucUsartBuf[2] * 2, &g_ucUsartBuf[3], 2);
  
  g_ucUsartLen = 6;   
  g_ucUsartBuf[0] = g_ucUsartBuf[0];
  g_ucUsartBuf[1] = g_ucUsartBuf[1];
  g_ucUsartBuf[2] = g_ucUsartBuf[2];
  g_ucUsartBuf[3] = 0x01;
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[4] = u16Crc % 0x100;
  g_ucUsartBuf[5] = u16Crc / 0x100;
}

//Summary:			 读生产日期	
//Parameter:
//Return:
static	void	DoCmd22(void)
{
  u16 u16Crc;
  
  if(g_ucUsartBuf[2] == 0x01)
  {
    g_ucUsartLen = sizeof(Product) + 4;
  
  //返回参数
    g_ucUsartBuf[0] = g_ucUsartBuf[0];         // 地址
    g_ucUsartBuf[1] = g_ucUsartBuf[1];         // 命令
  
    LoadCaliData(EEPROM_ADDR_PRODUCT, (u8*)&g_ucUsartBuf[2], sizeof(Product));
    
    u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
    g_ucUsartBuf[g_ucUsartLen - 2] = u16Crc % 0x100;
    g_ucUsartBuf[g_ucUsartLen - 1] = u16Crc / 0x100;
  }
  else
  {
    g_ucUsartLen = 0;
  }
  
}

//Summary:			 写生产日期	
//Parameter:
//Return:
static	void	DoCmd23(void)
{
  u16 u16Crc;
  g_ucUsartLen = 5;
  
  //返回参数
  g_ucUsartBuf[0] = g_ucUsartBuf[0];         // 地址
  g_ucUsartBuf[1] = g_ucUsartBuf[1];         // 命令
  
  SaveCaliData(EEPROM_ADDR_PRODUCT, (u8*)&g_ucUsartBuf[2], sizeof(Product));
  g_ucUsartBuf[2] = 1;
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[g_ucUsartLen - 2] = u16Crc % 0x100;
  g_ucUsartBuf[g_ucUsartLen - 1] = u16Crc / 0x100;
  
}

/********************************************************************
* Summary:    设置是否进入远程模式  
* Parameters:
*             void     0否 1是
*       
* Return :    void  
********************************************************************/
static	void	DoCmd0210(void)
{
  u16 u16Crc;
  g_ucUsartLen = 5;

  data.u8RemoteFlag = g_ucUsartBuf[2];         // 远程模式 0否 1是
  
  g_ucUsartBuf[0] = g_ucUsartBuf[0];         // 地址
  g_ucUsartBuf[1] = g_ucUsartBuf[1];         // 命令
  if(data.u8RemoteFlag == 1)
    g_ucUsartBuf[2] = 1;
  else
    g_ucUsartBuf[2] = 0;
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[3] = u16Crc % 0x100;
  g_ucUsartBuf[4] = u16Crc / 0x100;
}


/********************************************************************
* Summary:    设置输出类型 
* Parameters:
*             void      1 mA  4 20mV  3 V  5 100mV
*       
* Return :    void  
********************************************************************/
static	void	DoCmd021101()
{
  u16 u16Crc;
  g_ucUsartLen = 7;
 
  switch(g_ucUsartBuf[4])
  {
    case 1:
      data.u8SSigType = 1;
      data.u8SinkFlag = g_ucUsartBuf[3];
      break; //mA
    case 3:data.u8SSigType = 3;break; //V
    case 4:data.u8SSigType = 4;break; //20mV
    case 5:data.u8SSigType = 5;break; //100mV
    default:data.u8SSigType = 14;break; //end
  }  
    data.u8SourceEnable = 0;
  g_ucUsartBuf[0] = g_ucUsartBuf[0];         // 地址
  g_ucUsartBuf[1] = g_ucUsartBuf[1];         // 命令
  g_ucUsartBuf[2] = g_ucUsartBuf[2];         // 命令

  g_ucUsartBuf[3] = g_ucUsartBuf[3];         // 00
  g_ucUsartBuf[4] = data.u8SSigType;         // 命令 
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[5] = u16Crc % 0x100;
  g_ucUsartBuf[6] = u16Crc / 0x100;
}

/********************************************************************
* Summary:    设置输出值
* Parameters:
*             void     
*       
* Return :    void  
********************************************************************/
static	void	DoCmd021102(void)
{
  u16 u16Crc;
  g_ucUsartLen = 9;
 
  data.s16SourceValue =  g_ucUsartBuf[5] * 256 + g_ucUsartBuf[6];
  
  g_ucUsartBuf[0] = g_ucUsartBuf[0];         // 地址
  g_ucUsartBuf[1] = g_ucUsartBuf[1];         // 命令
  g_ucUsartBuf[2] = g_ucUsartBuf[2];         // 命令
  g_ucUsartBuf[3] = 0;         
  g_ucUsartBuf[4] = 0;  
  g_ucUsartBuf[5] = data.s16SourceValue / 256; 
  g_ucUsartBuf[6] = data.s16SourceValue % 256; 
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[7] = u16Crc % 0x100;
  g_ucUsartBuf[8] = u16Crc / 0x100;
}

/********************************************************************
* Summary:    打开/关闭输出  0关闭 1打开
* Parameters:
*             void     
*       
* Return :    void  
********************************************************************/
static	void	DoCmd021103(void)
{
  u16 u16Crc;
  g_ucUsartLen = 7;
 
  data.u8SourceEnable =  g_ucUsartBuf[4];
  
  g_ucUsartBuf[0] = g_ucUsartBuf[0];         // 地址
  g_ucUsartBuf[1] = g_ucUsartBuf[1];         // 命令
  g_ucUsartBuf[2] = g_ucUsartBuf[2];         // 命令
  g_ucUsartBuf[3] = g_ucUsartBuf[3];         
  g_ucUsartBuf[4] = data.u8SourceEnable;  
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[5] = u16Crc % 0x100;
  g_ucUsartBuf[6] = u16Crc / 0x100;
}


/********************************************************************
* Summary:    设置测量类型 
* Parameters:
*             void     0 24VLOOP  3 mA  2mV  1 V  4 欧姆  
*       
* Return :    void  
********************************************************************/
static	void	DoCmd021201()
{
  u16 u16Crc;
  g_ucUsartLen = 7;
 
  switch(g_ucUsartBuf[4])
  {
    case 1:data.u8MSigType = 1;break; //mA
    case 3:data.u8MSigType = 3;break; //V
    case 4:data.u8MSigType = 4;break; //20mV
    case 5:data.u8MSigType = 5;break; //100mV
    default:data.u8MSigType = 14;break; //end
  }  
  
  g_ucUsartBuf[0] = g_ucUsartBuf[0];         // 地址
  g_ucUsartBuf[1] = g_ucUsartBuf[1];         // 命令
  g_ucUsartBuf[2] = g_ucUsartBuf[2];         // 命令
  g_ucUsartBuf[3] = g_ucUsartBuf[3];         // 00
  g_ucUsartBuf[4] = data.u8MSigType;         // 命令 
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[5] = u16Crc % 0x100;
  g_ucUsartBuf[6] = u16Crc / 0x100;
}


/********************************************************************
* Summary:    打开/关闭测量  0关闭 1打开
* Parameters:
*             void     
*       
* Return :    void  
********************************************************************/
static	void	DoCmd021202(void)
{
  u16 u16Crc;
  g_ucUsartLen = 7;
 
  data.u8MeasureEnable =  g_ucUsartBuf[4];
  
  g_ucUsartBuf[0] = g_ucUsartBuf[0];         // 地址
  g_ucUsartBuf[1] = g_ucUsartBuf[1];         // 命令
  g_ucUsartBuf[2] = g_ucUsartBuf[2];         // 命令
  g_ucUsartBuf[3] = g_ucUsartBuf[3];         
  g_ucUsartBuf[4] = data.u8MeasureEnable;  
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[5] = u16Crc % 0x100;
  g_ucUsartBuf[6] = u16Crc / 0x100;
}

/********************************************************************
* Summary:    发送远程相关状态值
* Parameters:
*             void     
*       
* Return :    void  
********************************************************************/
static	void	DoCmd021301(void)
{
  u16 u16Crc;
  g_ucUsartLen = 21;
  
  g_ucUsartBuf[0] = g_ucUsartBuf[0];         // 地址
  g_ucUsartBuf[1] = g_ucUsartBuf[1];         // 命令
  g_ucUsartBuf[2] = 0x0c;                    // 数据字节个数
  g_ucUsartBuf[3] = data.u8RemoteFlag; 
  g_ucUsartBuf[4] = data.u8SSigType; 
  g_ucUsartBuf[5] = 0;         
  g_ucUsartBuf[6] = 0;  
  g_ucUsartBuf[7] = data.s16SourceValue / 256; 
  g_ucUsartBuf[8] = data.s16SourceValue % 256; 
  g_ucUsartBuf[9] = data.u8SourceEnable; 
  g_ucUsartBuf[10] = data.u8MSigType; 
  g_ucUsartBuf[11] = 0;         
  g_ucUsartBuf[12] = 0;  
  g_ucUsartBuf[13] = data.s16MeasureValue / 256; 
  g_ucUsartBuf[14] = data.s16MeasureValue % 256;   
  g_ucUsartBuf[15] = 0;         
  g_ucUsartBuf[16] = 0;  
  g_ucUsartBuf[17] = data.s16RJAuto / 256; 
  g_ucUsartBuf[18] = data.s16RJAuto % 256;   
  u16Crc = CalCrc(g_ucUsartBuf, g_ucUsartLen - 2);
  g_ucUsartBuf[19] = u16Crc % 0x100;
  g_ucUsartBuf[20] = u16Crc / 0x100;
}


//Summary:		根据命令解析串口接收数据
//Parameter:	None
//Return:			1	成功，0 失败
u8	Analysis_UsartBuf(void)
{
    u16	u16Crc;
    data.u16KeyTime = 0;  //背光常亮
    
    if(g_ucUsartBuf[0] == 0x01)
    {  
          u16Crc = g_ucUsartBuf[g_ucUsartLen - 1] * 0x100 + g_ucUsartBuf[g_ucUsartLen - 2];
        if(u16Crc != CalCrc(g_ucUsartBuf, g_ucUsartLen - 2))		//校验不对
        {
            DoCmd80();
            return 1;
        }		
        else if(g_ucUsartBuf[1] == 0x10)		
        {
            DoCmd10();
            return 1;
        }
        else if(g_ucUsartBuf[1] == 0x11)		
        {
            DoCmd11();
            return 1;
        }
        else if(g_ucUsartBuf[1] == 0x12)		
        {
            DoCmd12();
            return 1;
        }
        else if(g_ucUsartBuf[1] == 0x13)		
        {
            DoCmd13();
            return 1;
        }
        else if(g_ucUsartBuf[1] == 0x14)		
        {
            DoCmd14();
            return 1;
        }
        else if(g_ucUsartBuf[1] == 0x22)		
        {
            DoCmd22();
            return 1;
        }
        else if(g_ucUsartBuf[1] == 0x23)		
        {
            DoCmd23();
            return 1;
        }
        else
        {
            return 0;
        }
    }    
    else if(g_ucUsartBuf[0] == 0x02)
    {
          u16Crc = g_ucUsartBuf[g_ucUsartLen - 1] * 0x100 + g_ucUsartBuf[g_ucUsartLen - 2];
        if(u16Crc != CalCrc(g_ucUsartBuf, g_ucUsartLen - 2))		//校验不对
        {
            DoCmd80();
            return 1;
        }		
        else if(g_ucUsartBuf[1] == 0x10)		
        {
            DoCmd0210();
            return 1;
        }  
        else if(g_ucUsartBuf[1] == 0x11)		
        {
          if(g_ucUsartBuf[2] == 0x01)
          {
             DoCmd021101();
             return 1;
          }             
          else if(g_ucUsartBuf[2] == 0x02)
          {
             DoCmd021102();
             return 1;
          }
          else if(g_ucUsartBuf[2] == 0x03)
          {
             DoCmd021103();
             return 1;
          }    
          else
          {
            DoCmd80();
            return 1;            
          }
        }
        else if(g_ucUsartBuf[1] == 0x12)		
        {
          if(g_ucUsartBuf[2] == 0x01)
          {
             DoCmd021201();
             return 1;
          } 
          else if(g_ucUsartBuf[2] == 0x02)
          {
             DoCmd021202();
             return 1;
          }              
          else
          {
            DoCmd80();
            return 1;            
          }          
        }
        else if(g_ucUsartBuf[1] == 0x13)		
        {
          if(g_ucUsartBuf[2] == 0x01)
          {
             DoCmd021301();
      //       data.s16SourceValue ++;
       //     DrawSourceData();
             return 1;
          }               
          else
          {
            DoCmd80();
            return 1;            
          }          
        }
        else
        {
            DoCmd80();
            return 1;            
        }        
    }
    else
    {
            DoCmd80();
            return 1;      
    }
    
    

}
