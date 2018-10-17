/*=============================================================================
//File:			adcmd.h	
//Author:		tpcon
//Version:	    V0.1
//Date:			2013-8-5
//Dese:			AD通讯协议相关函数头文件
===============================================================================*/
#ifndef __ADCMD_H
#define __ADCMD_H

#define     SCALSIG_MA          0
#define     SCALSIG_MASINK      1
#define     SCALSIG_20MV        2
#define     SCALSIG_100MV       3
#define     SCALSIG_V           4
#define     SCALSIG_R           5
#define     SCALSIG_R2          6
#define     SCALSIG_R3          7

typedef struct
{
    u16       u16SCalPwm_mA[2];
    u16       u16SCalPwm_mASink[2];
    u16       u16SCalPwm_20mV[2];
    u16       u16SCalPwm_100mV[2];
    u16       u16SCalPwm_V[2];
    u16       u16SCalPwm_R[2];
    u16       u16SCalPwm_R2[2];
    u16       u16SCalPwm_R3[2];
    float    fSCalPwm_mA[2];
    float    fSCalPwm_mASink[2];
    float    fSCalPwm_20mV[2];
    float    fSCalPwm_100mV[2];
    float    fSCalPwm_V[2];        
    float    fSCaLPwm_R[2];
    float    fSCaLPwm_R2[2];
    float    fSCaLPwm_R3[2];
    u16       u16MCal_mA[2];        //MEASURE标定码值
    u16       u16MCal_100mV[2];
    u16       u16MCal_V[2];
    u16       u16MCal_R1[4];
}CalPwm;




typedef struct
{
  u8        u8ProductTime[6];           //生产日期
}Product;

u8	Analysis_UsartBuf(void);
void	InitCaliData(void);

#endif

