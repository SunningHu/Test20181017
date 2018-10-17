#ifndef _FUNCTIONDODATA_H__
#define _FUNCTIONDODATA_H__

#include "stm8l15x.h"




#define RELAY_1_PORT    GPIOE
#define RELAY_1_PIN        GPIO_Pin_5     // �Ӱ�1��Դ����
#define RELAY_2_PORT    GPIOE
#define RELAY_2_PIN        GPIO_Pin_4     // �Ӱ�2��Դ����
#define RELAY_3_PORT    GPIOE
#define RELAY_3_PIN        GPIO_Pin_3     // �Ӱ�3��Դ����
#define RELAY_4_PORT    GPIOE
#define RELAY_4_PIN        GPIO_Pin_2     // �Ӱ�4��Դ����
#define RELAY_5_PORT    GPIOE
#define RELAY_5_PIN        GPIO_Pin_1     // �Ӱ�5��Դ����
#define RELAY_6_PORT    GPIOE
#define RELAY_6_PIN        GPIO_Pin_0     // �Ӱ�6��Դ����
#define RELAY_7_PORT    GPIOC
#define RELAY_7_PIN        GPIO_Pin_2     // �Ӱ�7��Դ����
#define RELAY_8_PORT    GPIOC
#define RELAY_8_PIN        GPIO_Pin_3     // �Ӱ�8��Դ����
#define RELAY_9_PORT    GPIOC
#define RELAY_9_PIN        GPIO_Pin_4     // �Ӱ�9��Դ����
#define RELAY_10_PORT   GPIOC
#define RELAY_10_PIN       GPIO_Pin_5     // �Ӱ�10��Դ����
#define RELAY_11_PORT   GPIOC
#define RELAY_11_PIN       GPIO_Pin_6     // �Ӱ�11��Դ����
#define RELAY_12_PORT   GPIOC
#define RELAY_12_PIN       GPIO_Pin_7     // �Ӱ�12��Դ����




#define RELAY_1_OPEN         GPIO_SetBits(RELAY_1_PORT, RELAY_1_PIN)      //���Ӱ�1��Դ����
#define RELAY_1_CLOSE        GPIO_ResetBits(RELAY_1_PORT, RELAY_1_PIN)        //�ر��Ӱ�1��Դ����
#define RELAY_2_OPEN         GPIO_SetBits(RELAY_2_PORT, RELAY_2_PIN)      //���Ӱ�2��Դ����
#define RELAY_2_CLOSE        GPIO_ResetBits(RELAY_2_PORT, RELAY_2_PIN)        //�ر��Ӱ�2��Դ����
#define RELAY_3_OPEN         GPIO_SetBits(RELAY_3_PORT, RELAY_3_PIN)      //���Ӱ�3��Դ����
#define RELAY_3_CLOSE        GPIO_ResetBits(RELAY_3_PORT, RELAY_3_PIN)        //�ر��Ӱ�3��Դ����
#define RELAY_4_OPEN         GPIO_SetBits(RELAY_4_PORT, RELAY_4_PIN)      //���Ӱ�4��Դ����
#define RELAY_4_CLOSE        GPIO_ResetBits(RELAY_4_PORT, RELAY_4_PIN)        //�ر��Ӱ�4��Դ����
#define RELAY_5_OPEN         GPIO_SetBits(RELAY_5_PORT, RELAY_5_PIN)      //���Ӱ�5��Դ����
#define RELAY_5_CLOSE        GPIO_ResetBits(RELAY_5_PORT, RELAY_5_PIN)        //�ر��Ӱ�5��Դ����
#define RELAY_6_OPEN         GPIO_SetBits(RELAY_6_PORT, RELAY_6_PIN)      //���Ӱ�6��Դ����
#define RELAY_6_CLOSE        GPIO_ResetBits(RELAY_6_PORT, RELAY_6_PIN)        //�ر��Ӱ�6��Դ����
#define RELAY_7_OPEN         GPIO_SetBits(RELAY_7_PORT, RELAY_7_PIN)      //���Ӱ�7��Դ����
#define RELAY_7_CLOSE        GPIO_ResetBits(RELAY_7_PORT, RELAY_7_PIN)        //�ر��Ӱ�7��Դ����
#define RELAY_8_OPEN         GPIO_SetBits(RELAY_8_PORT, RELAY_8_PIN)      //���Ӱ�8��Դ����
#define RELAY_8_CLOSE        GPIO_ResetBits(RELAY_8_PORT, RELAY_8_PIN)        //�ر��Ӱ�8��Դ����
#define RELAY_9_OPEN         GPIO_SetBits(RELAY_9_PORT, RELAY_9_PIN)      //���Ӱ�9��Դ����
#define RELAY_9_CLOSE        GPIO_ResetBits(RELAY_9_PORT, RELAY_9_PIN)        //�ر��Ӱ�9��Դ����
#define RELAY_10_OPEN        GPIO_SetBits(RELAY_10_PORT, RELAY_10_PIN)    //���Ӱ�10��Դ����
#define RELAY_10_CLOSE       GPIO_ResetBits(RELAY_10_PORT, RELAY_10_PIN)      //�ر��Ӱ�10��Դ����
#define RELAY_11_OPEN        GPIO_SetBits(RELAY_11_PORT, RELAY_11_PIN)    //���Ӱ�11��Դ����
#define RELAY_11_CLOSE       GPIO_ResetBits(RELAY_11_PORT, RELAY_11_PIN)      //�ر��Ӱ�11��Դ����
#define RELAY_12_OPEN        GPIO_SetBits(RELAY_12_PORT, RELAY_12_PIN)    //���Ӱ�12��Դ����
#define RELAY_12_CLOSE       GPIO_ResetBits(RELAY_12_PORT, RELAY_12_PIN)      //�ر��Ӱ�12��Դ����




typedef struct
{
    u8 u8Type;       //�������ͱ�־λ
    u8 u8count;      //ͨѶ������־λ
      
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
    
    
    
    
    u8 u8address;              //������ַ���ɲ��뿪�ؾ�������Χ0-64
    
//////////////////////////////////////////����Ϊ��MCGSͨѶ���ñ���
    u16 g_bMode;               //��/�Զ���0Ϊ�ֶ���1Ϊ�Զ�
    u16 g_bStart;              //��ʼ��־��0Ϊδ��ʼ��1Ϊ��ʼ                       use
    u16 g_bStop;               //��ͣ��־��0Ϊ����ͣ��1Ϊ��ͣ
    u16 g_bCaliFinish;         //�궨״̬��0Ϊ׼���궨��1Ϊ�궨��ɣ�2Ϊ���ڱ궨
    u16 g_u8MeterNum;          //���ڱ궨��ţ�0-48                                 use
    u16 g_u8MeterMax;          //�����궨��ţ�0-48
    u16 g_bCaliPoint;          //�궨�㣬�궨��������                               use
    u16 g_u16MacData;          //������ֵ                                           use
    float g_fMacData;         //��������ֵ                                         use
    u16 g_u16ReadTimes;       //��ȡ������1-30��
    u16 g_u16ResultMeter;     //�궨�����0Ϊδ��ɣ�1Ϊ�ɹ���2Ϊʧ��               use
/////////////////////////////////////////��չ��־λ
    u16  g_productType;        //��Ʒ����  0 S1  1 S2
    u16  g_functionType;        // 0 ���  1 �궨
  
}Value;


typedef struct
{
    float fPCorrAOut_dw[2];      //��Դ��������궨���жϷ�Χ  ����     [0]Ϊmin��[1]Ϊmax   2-5        6-9
    float fPCorrAOut_up[2];      //��Դ��������궨���жϷ�Χ  ����     [0]Ϊmin��[1]Ϊmax   10-13      14-17
    float fNCorrAOut_dw[2];      //��Դ��������궨���жϷ�Χ  ����     [0]Ϊmin��[1]Ϊmax   18-21      22-25
    float fNCorrAOut_up[2];      //��Դ��������궨���жϷ�Χ  ����     [0]Ϊmin��[1]Ϊmax   26-29      30-33
    float fCorrVOut20_dw[2];     //20mv����궨���жϷ�Χ      ����     [0]Ϊmin��[1]Ϊmax   34-37      38-41
    float fCorrVOut20_up[2];     //20mv����궨���жϷ�Χ      ����     [0]Ϊmin��[1]Ϊmax   42-45      46-49
    float fCorrVOut100_dw[2];    //100mv����궨���жϷ�Χ     ����     [0]Ϊmin��[1]Ϊmax   50-53      54-57
    float fCorrVOut100_up[2];    //100mv����궨���жϷ�Χ     ����     [0]Ϊmin��[1]Ϊmax   58-61      62-65
    float fCorrVOut_dw[2];       //V����궨���жϷ�Χ         ����     [0]Ϊmin��[1]Ϊmax   66-69      70-73
    float fCorrVOut_up[2];       //V����궨���жϷ�Χ         ����     [0]Ϊmin��[1]Ϊmax   74-77      78-81
    u16    u16CorrAIn_dw[2];      //���������궨���жϷ�Χ      ����     [0]Ϊmin��[1]Ϊmax   82-83      84 85
    u16    u16CorrAIn_up[2];      //���������궨���жϷ�Χ      ����     [0]Ϊmin��[1]Ϊmax   86 87      88 89
    u16    u16CorrVIn20_dw[2];    //20mv�����궨���жϷ�Χ      ����     [0]Ϊmin��[1]Ϊmax   90 91      92 93 
    u16    u16CorrVIn20_up[2];    //20mv�����궨���жϷ�Χ      ����     [0]Ϊmin��[1]Ϊmax   94 95      96 97 
    u16    u16CorrVIn100_dw[2];   //100mv�����궨���жϷ�Χ     ����     [0]Ϊmin��[1]Ϊmax   98 99      100 101
    u16    u16CorrVIn100_up[2];   //100mv�����궨���жϷ�Χ     ����     [0]Ϊmin��[1]Ϊmax   102 103    104 105
    u16    u16CorrVIn_dw[2];      //V�����궨���жϷ�Χ         ����     [0]Ϊmin��[1]Ϊmax   106 107    108 109
    u16    u16CorrVIn_up[2];      //V�����궨���жϷ�Χ         ����     [0]Ϊmin��[1]Ϊmax   110 111    112 113

////////////////////////////////////�����ȶ� ǰ�����ݲ�ֵ
    u16    u16MaxOKCount;            //�ж��ź��Ƿ��ȶ�������           EEPROM��ַ  0 1

    
}Limit;




void usdelay(u16 us);
void msdelay(u16 ms);
void sdelay(u16 s);
u16 crc16(volatile u8 *puchMsg, u8 usDataLen);




extern const u16  uCRC16Table[];

extern void RelayInit(void);


#endif