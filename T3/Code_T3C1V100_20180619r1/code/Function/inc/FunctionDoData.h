#ifndef _FUNCTIONDODATA_H__
#define _FUNCTIONDODATA_H__

#include "stm8l15x.h"


#define READ_MAXTIME   50      //����ȡ����
#define READ_VALIDTIME 10      //��Ч��ȡ����

typedef struct
{
    u8 u8Type;       //�������ͱ�־λ
    u8 u8count;      //ͨѶ������־λ
    u8 u8ReStart;
      
}Flag;

typedef struct
{
    float fPCorrAOut[2];       //��Դ��������궨ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    float fNCorrAOut[2];       //��Դ��������궨ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    float fCorrVOut20[2];      //20mV��ѹ����궨ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    float fCorrVOut100[2];     //100mV��ѹ����궨ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    float fCorrVOut[2];        //V��ѹ����궨ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    float fCorrROut[2];        //ŷķ����궨ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    u16 u16CorrAIn[2];         //���������궨ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    u16 u16CorrVIn20[2];       //20mV��ѹ�����궨ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    u16 u16CorrVIn100[2];      //100mV��ѹ�����궨ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    u16 u16CorrVIn[2];         //V��ѹ�����궨ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    u16 u16CorrRIn[2];         //V��ѹ�����궨ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    
    s32 s32TestTemp;           //����¶ȡ�
    s32 s32PTestAOut[2];       //��Դ����������ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    s32 s32NTestAOut[2];       //��Դ����������ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    s32 s32TestVOut20[2];      //20mV��ѹ������ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    s32 s32TestVOut100[2];     //100mV��ѹ������ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    s32 s32TestVOut[2];        //V��ѹ������ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    s32 s32TestROut[2];        //ŷķ������ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    s32 s32TestAIn[2];         //�����������ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    s32 s32TestVIn20[2];       //20mV��ѹ�������ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    s32 s32TestVIn100[2];      //100mV��ѹ�������ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    s32 s32TestVIn[2];         //V��ѹ�������ֵ��[0]Ϊ���ޣ�[1]Ϊ����
    s32 s32TestRIn[2];         //V��ѹ�������ֵ��[0]Ϊ���ޣ�[1]Ϊ����    
    
    u8  u8CailPointFlag[25];   //���궨��״̬  0���궨  1 �ɹ�  2ʧ��  3����
    
    
    u8 u8address;              //������ַ���ɲ��뿪�ؾ�������Χ0-64
    
//////////////////////////////////////////����Ϊ��MCGSͨѶ���ñ���
    u16 g_bStart;              //��ʼ��־��0Ϊδ��ʼ��1Ϊ��ʼ                       use
    u16 g_bCaliFinish;         //�궨״̬��0Ϊ׼���궨��1Ϊ�궨��ɣ�2Ϊ���ڱ궨
    u16 g_u8MeterNum;          //���ڱ궨��ţ�0-48                                 use
    u16 g_u8MeterMax;          //�����궨��ţ�0-48
    u16 g_bCaliPoint;          //�궨�㣬�궨��������                               use
    u16 g_u16MacData;          //������ֵ                                           use
    float g_fMacData;         //��������ֵ                                         use
    u16 g_u16ResultMeter;     //�궨�����0Ϊδ��ɣ�1Ϊ�ɹ���2Ϊʧ��               use
/////////////////////////////////////////��չ��־λ
    u16  g_productType;        //��Ʒ����  0 S1  1 S2
    u16  g_functionType;        // 0 ���  1 �궨
  
}Value;


typedef struct
{
//    float fPCorrAOut_dw[2];      //��Դ��������궨���жϷ�Χ  ����     [0]Ϊmin��[1]Ϊmax   2-5        6-9
//    float fPCorrAOut_up[2];      //��Դ��������궨���жϷ�Χ  ����     [0]Ϊmin��[1]Ϊmax   10-13      14-17
//    float fNCorrAOut_dw[2];      //��Դ��������궨���жϷ�Χ  ����     [0]Ϊmin��[1]Ϊmax   18-21      22-25
//    float fNCorrAOut_up[2];      //��Դ��������궨���жϷ�Χ  ����     [0]Ϊmin��[1]Ϊmax   26-29      30-33
//    float fCorrVOut20_dw[2];     //20mv����궨���жϷ�Χ      ����     [0]Ϊmin��[1]Ϊmax   34-37      38-41
//    float fCorrVOut20_up[2];     //20mv����궨���жϷ�Χ      ����     [0]Ϊmin��[1]Ϊmax   42-45      46-49
//    float fCorrVOut100_dw[2];    //100mv����궨���жϷ�Χ     ����     [0]Ϊmin��[1]Ϊmax   50-53      54-57
//    float fCorrVOut100_up[2];    //100mv����궨���жϷ�Χ     ����     [0]Ϊmin��[1]Ϊmax   58-61      62-65
//    float fCorrVOut_dw[2];       //V����궨���жϷ�Χ         ����     [0]Ϊmin��[1]Ϊmax   66-69      70-73
//    float fCorrVOut_up[2];       //V����궨���жϷ�Χ         ����     [0]Ϊmin��[1]Ϊmax   74-77      78-81
//    u16    u16CorrAIn_dw[2];      //���������궨���жϷ�Χ      ����     [0]Ϊmin��[1]Ϊmax   82-83      84 85
//    u16    u16CorrAIn_up[2];      //���������궨���жϷ�Χ      ����     [0]Ϊmin��[1]Ϊmax   86 87      88 89
//    u16    u16CorrVIn20_dw[2];    //20mv�����궨���жϷ�Χ      ����     [0]Ϊmin��[1]Ϊmax   90 91      92 93 
//    u16    u16CorrVIn20_up[2];    //20mv�����궨���жϷ�Χ      ����     [0]Ϊmin��[1]Ϊmax   94 95      96 97 
//    u16    u16CorrVIn100_dw[2];   //100mv�����궨���жϷ�Χ     ����     [0]Ϊmin��[1]Ϊmax   98 99      100 101
//    u16    u16CorrVIn100_up[2];   //100mv�����궨���жϷ�Χ     ����     [0]Ϊmin��[1]Ϊmax   102 103    104 105
//    u16    u16CorrVIn_dw[2];      //V�����궨���жϷ�Χ         ����     [0]Ϊmin��[1]Ϊmax   106 107    108 109
//    u16    u16CorrVIn_up[2];      //V�����궨���жϷ�Χ         ����     [0]Ϊmin��[1]Ϊmax   110 111    112 113

////////////////////////////////////�����ȶ� ǰ�����ݲ�ֵ
    u16    u16MaxOKCount;            //�ж��ź��Ƿ��ȶ�������           EEPROM��ַ  0 1

    
}Limit;




void usdelay(u16 us);
void msdelay(u16 ms);
void sdelay(u16 s);
u16 crc16(volatile u8 *puchMsg, u8 usDataLen);
void PCorrAOutDown(void);
void PCorrAOutUp(void);
void NCorrAOutDown(void);
void NCorrAOutUp(void);
void CorrVOut20Down(void);
void CorrVOut20Up(void);
void CorrVOut100Down(void);
void CorrVOut100Up(void);
void CorrVOutDown(void);
void CorrVOutUp(void);
void CorrAInDown(void);
void CorrAInUp(void);
void CorrVIn20Down(void);
void CorrVIn20Up(void);
void CorrVIn100Down(void);
void CorrVIn100Up(void);
void CorrVInDown(void);
void CorrVInUp(void);
void CorrROutDown(void);
void CorrROutUp(void);
void PCorrComm(void);

void PTestTemp(void);
void PTestAOutDown(void);
void PTestAOutUp(void);
void NTestAOutDown(void);
void NTestAOutUp(void);
void TestVOut20Down(void);
void TestVOut20Up(void);
void TestVOut100Down(void);
void TestVOut100Up(void);
void TestVOutDown(void);
void TestVOutUp(void);
void TestROutDown(void);
void TestROutUp(void);
void TestAInDown(void);
void TestAInUp(void);
void TestVIn20Down(void);
void TestVIn20Up(void);
void TestVIn100Down(void);
void TestVIn100Up(void);
void TestVInDown(void);
void TestVInUp(void);
void TestRInDown(void);
void TestRInUp(void);

float S32tofloat(s32 s32Data);
float S32tofloatCheck(s32 s32Data,u8 dotNUm);

extern const u16  uCRC16Table[];

extern void RelayInit(void);


#endif