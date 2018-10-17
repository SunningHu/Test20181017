/***************************************************************
��Ʒ���ƣ�ȫ�Զ��궨ϵͳ
�������ţ�T2
������Ա��Ǯ��ˬ(Ƕ��ʽ���)���춬������(��λ�����)�����(Ӳ��)���ܿ�(�ṹ)
����ʱ�䣺2016.12.20-2017.05.01
***************************************************************/
 


/*****************************************
���б��������õ�ͷ�ļ���ȫ������main.h����
����
*****************************************/
#include "main.h"
#include "math.h"



//Summary:    ��ʱ�ӳ�ʼ��
void clk_init(void)
{
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);  
}


//Summary:    ��ʱ��TIM4��ʼ��
void tim4_init(void)
{
    //Ĭ��TIM4ʱ���ǹرյģ�����Ҫ�ȿ���TIM4��ʱ�ӣ�֮����ܶ�TIM4�Ĵ������г�ʼ�����м�˳�򲻿ɷ�
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE); 
    //1��TIM4�Ǹ�8λ�Ķ�ʱ�������������Դﵽ255
    //2��ϵͳʱ����16MHZ�ģ���ʱ��ʹ�õ���ϵͳʱ��Դ
    //3����õ�1ms��ʱ��������Ҫ��ʱ�ӷ�Ƶ
    //4��ϵͳʱ��128��Ƶ�����Եõ� 16000000 / 128 = 125K��ʱ��
    //5��ʱ���趨ֵ��ʽΪ (X * 1000) / 125000 = 1;
    //6���õ�X = 125��Ȼ�� X - 1 �͵õ�124�����ֵ�������ǶԶ�ʱ�������ֵ
    TIM4_TimeBaseInit(TIM4_Prescaler_128, 124);
    //���TIM4��ʱ�������־λ
    TIM4_ClearFlag(TIM4_FLAG_Update);
    //ʹ��TIM4��ʱ������ж�
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);
    //�������ж�
    enableInterrupts();
    //����TIM4��ʱ��
    TIM4_Cmd(ENABLE);     
}

//Summary:    GPIO�ڳ�ʼ��
void GPIO_init(void)
{
    GPIO_Init(LED_RUN_PORT, LED_RUN_PIN, GPIO_Mode_Out_PP_High_Slow); //  ���е�
    GPIO_Init(RELAY_M_PORT, RELAY_M_PIN, GPIO_Mode_Out_PP_High_Slow); //  �����̵���
    GPIO_Init(RELAY_S_PORT, RELAY_S_PIN, GPIO_Mode_Out_PP_High_Slow); //  ����̵���
    GPIO_Init(RELAY_MA_PORT, RELAY_MA_PIN, GPIO_Mode_Out_PP_High_Slow) ;//  �궨ŷķ�̵���
    GPIO_Init(RELAY_W_PORT, RELAY_W_PIN, GPIO_Mode_Out_PP_High_Slow); //  Ԥ��
    
    //���뿪��
    GPIO_Init(KEY_1_PORT, KEY_1_PIN, GPIO_Mode_In_PU_No_IT); //  key1
    GPIO_Init(KEY_2_PORT, KEY_2_PIN, GPIO_Mode_In_PU_No_IT); //  key2
    GPIO_Init(KEY_3_PORT, KEY_3_PIN, GPIO_Mode_In_PU_No_IT); //  key3
    GPIO_Init(KEY_4_PORT, KEY_4_PIN, GPIO_Mode_In_PU_No_IT); //  key4
    GPIO_Init(KEY_5_PORT, KEY_5_PIN, GPIO_Mode_In_PU_No_IT); //  key5
    GPIO_Init(KEY_6_PORT, KEY_6_PIN, GPIO_Mode_In_PU_No_IT); //  key6

}

/*******************************************************
* Summary:    �̵�����ʼ��
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
void RelayInit(void)
{
    RELAY_M_CLOSE;
    RELAY_S_CLOSE;
    RELAY_MA_CLOSE;
    RELAY_W_CLOSE;
    RELAY_W_CLOSE;  
    RELAY_W_CLOSE;
    
//    RELAY_S_OPEN;
//    RELAY_MA_OPEN;
//    RELAY_W_CLOSE;    

}

/*******************************************************
* Summary:    �ر����м̵���
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
static void CloseAllRelay(void)
{
    RELAY_M_CLOSE;
    RELAY_S_CLOSE;
    RELAY_MA_CLOSE;
    RELAY_W_OPEN;
   
}

//Summary:    ��¼�̵����л�
void CodeRelayTab(void)
{

}



//Summary:    �õ�������ַ
void GetAddress(void)
{
    u8 i = 0;
    if(GPIO_ReadInputDataBit(KEY_1_PORT, KEY_1_PIN) == RESET) 
    {
        i = i + 1;
    }
    if(GPIO_ReadInputDataBit(KEY_2_PORT, KEY_2_PIN) == RESET) 
    {
        i = i + 2;
    }
    if(GPIO_ReadInputDataBit(KEY_3_PORT, KEY_3_PIN) == RESET) 
    {
        i = i + 4;
    }
    if(GPIO_ReadInputDataBit(KEY_4_PORT, KEY_4_PIN) == RESET) 
    {
        i = i + 8;
    }
    if(GPIO_ReadInputDataBit(KEY_5_PORT, KEY_5_PIN) == RESET) 
    {
        i = i + 16;
    }
    if(GPIO_ReadInputDataBit(KEY_6_PORT, KEY_6_PIN) == RESET) 
    {
        i = i + 32;
    }
    value.u8address = i;
} 


/*******************************************************
* Summary:    ����̵����л�
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
static void SourceRelayTab_Test(void)
{
    RELAY_M_OPEN;
    RELAY_W_OPEN;
    RELAY_S_CLOSE;
    RELAY_MA_CLOSE;
//    RELAY_W_CLOSE;
//    RELAY_M_CLOSE;
}

/*******************************************************
* Summary:    �����̵����л�
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
static void MeasureRelayTab_Test(void)
{
    RELAY_S_OPEN;
//    RELAY_MA_OPEN;
    RELAY_M_CLOSE;
    RELAY_W_OPEN;
//    RELAY_S_CLOSE;
    RELAY_MA_CLOSE;    
}


/*******************************************************
* Summary:    �����̵����л�
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
static void MeasureRelayTab_Cail(void)
{
    RELAY_S_OPEN;
//    RELAY_MA_OPEN;
    RELAY_M_OPEN;
    RELAY_W_CLOSE;
//    RELAY_S_CLOSE;
    RELAY_MA_OPEN;    
}


//Summary:    �궨ĳһ����
void CalPoint(void)
{
    switch(value.g_bCaliPoint)
    { 
        case 0:
            PCorrComm();              //�궨ͨѶ����
            break;
        case 1:
            SourceRelayTab_Test();    //�źŷ���������̵����л�
            PCorrAOutDown();          //�궨��Դ20mA�������ֵ
            break;
        case 2:
            SourceRelayTab_Test();    //�źŷ���������̵����л�
            PCorrAOutUp();            //�궨��Դ20mA�������ֵ
            break;
        case 3:
            SourceRelayTab_Test();    //�źŷ���������̵����л�
            NCorrAOutDown();          //�궨��Դ20mA�������ֵ
            break;
        case 4 :
            SourceRelayTab_Test();    //�źŷ���������̵����л�
            NCorrAOutUp();            //�궨��Դ20mA�������ֵ
            break;
        case 5:
            SourceRelayTab_Test();    //�źŷ���������̵����л�
            CorrVOut20Down();         //�궨20mV�������ֵ
            break;
        case 6:
            SourceRelayTab_Test();    //�źŷ���������̵����л�
            CorrVOut20Up();           //�궨20mV�������ֵ
            break;
        case 7:
            SourceRelayTab_Test();    //�źŷ���������̵����л�
            CorrVOut100Down();        //�궨100mV�������ֵ
            break;
        case 8:
            SourceRelayTab_Test();    //�źŷ���������̵����л�
            CorrVOut100Up();          //�궨100mV�������ֵ
            break;
        case 9:
            SourceRelayTab_Test();    //�źŷ���������̵����л�
            CorrVOutDown();           //�궨V�������ֵ
            break;
        case 10:
            SourceRelayTab_Test();    //�źŷ���������̵����л�
            CorrVOutUp();             //�궨V�������ֵ
            break;
        case 11:
            MeasureRelayTab_Cail();   //�źŷ����������̵����л�
            CorrROut3100();            //�궨R 3mA 100
            break;              
        case 12:
            MeasureRelayTab_Cail();   //�źŷ����������̵����л�
            CorrROut1300();            //�궨R 1mA 300
            break;  
        case 13:
            MeasureRelayTab_Cail();   //�źŷ����������̵����л�
            CorrROut1100();            //�궨R 1mA 100
            break;  
        case 14:
            MeasureRelayTab_Cail();   //�źŷ����������̵����л�
            CorrROut5300();            //�궨R 0.5mA 300
            break;  
        case 15:
            MeasureRelayTab_Cail();   //�źŷ����������̵����л�
            CorrROut5100();            //�궨R 0.5mA 100
            break;  
        case 16:
            MeasureRelayTab_Cail();   //�źŷ����������̵����л�
            CorrROut2100();            //�궨R 0.2mA 100
            break;  
        case 17:
            MeasureRelayTab_Test();   //�źŷ����������̵����л�
            CorrAInDown();            //�궨������������ֵ
            break;
        case 18:
            MeasureRelayTab_Test();   //�źŷ����������̵����л�
            CorrAInUp();              //�궨������������ֵ
            break;
        case 19:
            MeasureRelayTab_Test();   //�źŷ����������̵����л�
            CorrVIn20Down();          //�궨����20mV����ֵ
            break;
        case 20:
            MeasureRelayTab_Test();   //�źŷ����������̵����л�
            CorrVIn20Up();            //�궨����20mV����ֵ
            break;
        case 21:
            MeasureRelayTab_Test();   //�źŷ����������̵����л�
            CorrVIn100Down();         //�궨����100mV����ֵ
            break;
        case 22:
            MeasureRelayTab_Test();   //�źŷ����������̵����л�
            CorrVIn100Up();           //�궨����100mV����ֵ
            break;
        case 23:
            MeasureRelayTab_Test();   //�źŷ����������̵����л�
            CorrVInDown();            //�궨����V����ֵ
            break;
        case 24:
            MeasureRelayTab_Test();   //�źŷ����������̵����л�
            CorrVInUp();              //�궨����V����ֵ  
            break;
            default:;
    }
}






/*******************************************************
* Summary:    ����źŵ�
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
static void TestPoint(void)
{
    switch(value.g_bCaliPoint)
   // switch(13)
    { 
        case 0:
            CloseAllRelay();       //�̵�����ʼ��
            PTestTemp();           // ����¶ȼ��
            break;           
        case 1:
            SourceRelayTab_Test(); //�źŷ���������̵����л�
            PTestAOutDown();       // ��Դ2mA������ 
            break;
        case 2:
            SourceRelayTab_Test(); //�źŷ���������̵����л�
            PTestAOutUp();         // ��Դ21mA������ 
            break;
        case 3:
            SourceRelayTab_Test(); //�źŷ���������̵����л�
            NTestAOutDown();       //  ��Դ2mA������
            break;
        case 4 :
            SourceRelayTab_Test(); //�źŷ���������̵����л�
            NTestAOutUp();         //  ��Դ21mA������
            break;
        case 5:
            SourceRelayTab_Test(); //�źŷ���������̵����л�
            TestVOut20Down();      // 2mV������
            break;
        case 6:
            SourceRelayTab_Test(); //�źŷ���������̵����л�
            TestVOut20Up();        // 18mV������
            break;
        case 7:
            SourceRelayTab_Test(); //�źŷ���������̵����л�
            TestVOut100Down();     // 10mV������
            break;
        case 8:
            SourceRelayTab_Test(); //�źŷ���������̵����л�
            TestVOut100Up();       // 90mV������
            break;
        case 9:
            SourceRelayTab_Test(); //�źŷ���������̵����л�
            TestVOutDown();        // 0.000V������
            break;
        case 10:
            SourceRelayTab_Test(); //�źŷ���������̵����л�
            TestVOutUp();          // 15.000V������
            break;
        case 11:
            SourceRelayTab_Test(); //�źŷ���������̵����л�
            TestROutDown();        // 50.0ŷķ������
            break;
        case 12:
            SourceRelayTab_Test(); //�źŷ���������̵����л�
            TestROutUp();          // 250.0ŷķ������
            break;            
        case 13:
            MeasureRelayTab_Test();     //�źŷ����������̵����л�
            TestAInDown();          //  ����2mA���
            break;
        case 14:
            MeasureRelayTab_Test();    //�źŷ����������̵����л�
            TestAInUp();          //  ����21mA���
            break;
        case 15:
            MeasureRelayTab_Test();    //�źŷ����������̵����л�
            TestVIn20Down();      //�궨����20mV����ֵ
            break;
        case 16:
            MeasureRelayTab_Test();    //�źŷ����������̵����л�
            TestVIn20Up();        //�궨����20mV����ֵ
            break;
        case 17:
            MeasureRelayTab_Test();   //�źŷ����������̵����л�
            TestVIn100Down();    //�궨����100mV����ֵ
            break;
        case 18:
            MeasureRelayTab_Test();   //�źŷ����������̵����л�
            TestVIn100Up();      //�궨����100mV����ֵ
            break;
        case 19:
            MeasureRelayTab_Test();  //�źŷ����������̵����л�
            TestVInDown();          //�궨����V����ֵ
            break;
        case 20:
            MeasureRelayTab_Test();  //�źŷ����������̵����л�
            TestVInUp();             //�궨����V����ֵ  
            break;
        case 21:
            MeasureRelayTab_Test();  //�źŷ����������̵����л�
            TestRInDown();           // 50.0ŷķ���
            break;
        case 22:
            MeasureRelayTab_Test();  //�źŷ����������̵����л�
            TestRInUp();             // 250ŷķ���  
            break;            
            default:;
    }
}

void LimitInit(void)
{
//////////////////////////////////////////  �����ȶ�N�����ź��ȶ�  Ĭ��Ϊ10
  limit.u16MaxOKCount = eep_read16(0);
  if(limit.u16MaxOKCount == 0)
    limit.u16MaxOKCount = 10;
//////////////////////////////////////////  ���źŵ� ��ֵ��Χ����  
//  limit.fPCorrAOut_dw[0] = eep_readf(2);
//  if(limit.fPCorrAOut_dw[0] == 0)
//    limit.fPCorrAOut_dw[0] = 4.6;
//  
//  limit.fPCorrAOut_dw[1] = eep_readf(6);
//  if(limit.fPCorrAOut_dw[1] == 0)
//    limit.fPCorrAOut_dw[1] = 5.0;
//  
//  limit.fPCorrAOut_up[0] = eep_readf(10);
//  if(limit.fPCorrAOut_up[0] == 0)
//    limit.fPCorrAOut_up[0] = 19.6;  
//  
//  limit.fPCorrAOut_up[1] = eep_readf(14);
//  if(limit.fPCorrAOut_up[1] == 0)
//    limit.fPCorrAOut_up[1] = 20.0;
//  
//  limit.fNCorrAOut_dw[0] = eep_readf(18);
//  if(limit.fNCorrAOut_dw[0] == 0)
//    limit.fNCorrAOut_dw[0] = 4.6;
//  
//  limit.fNCorrAOut_dw[1] = eep_readf(22);
//  if(limit.fNCorrAOut_dw[1] == 0)
//    limit.fNCorrAOut_dw[1] = 5.0;  
//  
//  limit.fNCorrAOut_up[0] = eep_readf(26);
//  if(limit.fNCorrAOut_up[0] == 0)
//    limit.fNCorrAOut_up[0] = 19.6;  
//  
//  limit.fNCorrAOut_up[1] = eep_readf(30);
//  if(limit.fNCorrAOut_up[1] == 0)
//    limit.fNCorrAOut_up[1] = 20.0;  
//  
//  limit.fCorrVOut20_dw[0] = eep_readf(34);
//  if(limit.fCorrVOut20_dw[0] == 0)
//    limit.fCorrVOut20_dw[0] = 4.6;  
//  
//  limit.fCorrVOut20_dw[1] = eep_readf(38);
//  if(limit.fCorrVOut20_dw[1] == 0)
//    limit.fCorrVOut20_dw[1] = 5.2;    
//  
//  limit.fCorrVOut20_up[0] = eep_readf(42);
//  if(limit.fCorrVOut20_up[0] == 0)
//    limit.fCorrVOut20_up[0] = 20.0;  
//  
//  limit.fCorrVOut20_up[1] = eep_readf(46);
//  if(limit.fCorrVOut20_up[1] == 0)
//    limit.fCorrVOut20_up[1] = 21.0;   
//  
//  limit.fCorrVOut100_dw[0] = eep_readf(50);
//  if(limit.fCorrVOut100_dw[0] == 0)
//    limit.fCorrVOut100_dw[0] = 23.0;  
//  
//  limit.fCorrVOut100_dw[1] = eep_readf(54);
//  if(limit.fCorrVOut100_dw[1] == 0)
//    limit.fCorrVOut100_dw[1] = 25.0;    
//  
//  limit.fCorrVOut100_up[0] = eep_readf(58);
//  if(limit.fCorrVOut100_up[0] == 0)
//    limit.fCorrVOut100_up[0] = 98.0;  
//  
//  limit.fCorrVOut100_up[1] = eep_readf(62);
//  if(limit.fCorrVOut100_up[1] == 0)
//    limit.fCorrVOut100_up[1] = 100.0;    
//  
//  limit.fCorrVOut_dw[0] = eep_readf(66);
//  if(limit.fCorrVOut_dw[0] == 0)
//    limit.fCorrVOut_dw[0] = 2.0;  
//  
//  limit.fCorrVOut_dw[1] = eep_readf(70);
//  if(limit.fCorrVOut_dw[1] == 0)
//    limit.fCorrVOut_dw[1] = 2.2;    
//  
//  limit.fCorrVOut_up[0] = eep_readf(74);
//  if(limit.fCorrVOut_up[0] == 0)
//    limit.fCorrVOut_up[0] = 9.5;  
//  
//  limit.fCorrVOut_up[1] = eep_readf(78);
//  if(limit.fCorrVOut_up[1] == 0)
//    limit.fCorrVOut_up[1] = 10.0;    
//  
//  limit.u16CorrAIn_dw[0] = eep_read16(82);
//  if(limit.u16CorrAIn_dw[0] == 0)
//    limit.u16CorrAIn_dw[0] = 300;  
//  
//  limit.u16CorrAIn_dw[1] = eep_read16(84);
//  if(limit.u16CorrAIn_dw[1] == 0)
//    limit.u16CorrAIn_dw[1] = 350;    
//  
//  limit.u16CorrAIn_up[0] = eep_read16(86);
//  if(limit.u16CorrAIn_up[0] == 0)
//    limit.u16CorrAIn_up[0] = 3400;  
//  
//  limit.u16CorrAIn_up[1] = eep_read16(88);
//  if(limit.u16CorrAIn_up[1] == 0)
//    limit.u16CorrAIn_up[1] = 3520;      
//  
//  limit.u16CorrVIn20_dw[0] = eep_read16(90);
//  if(limit.u16CorrVIn20_dw[0] == 0)
//    limit.u16CorrVIn20_dw[0] = 300;  
//  
//  limit.u16CorrVIn20_dw[1] = eep_read16(92);
//  if(limit.u16CorrVIn20_dw[1] == 0)
//    limit.u16CorrVIn20_dw[1] = 400;    
//  
//  limit.u16CorrVIn20_up[0] = eep_read16(94);
//  if(limit.u16CorrVIn20_up[0] == 0)
//    limit.u16CorrVIn20_up[0] = 3400;  
//  
//  limit.u16CorrVIn20_up[1] = eep_read16(96);
//  if(limit.u16CorrVIn20_up[1] == 0)
//    limit.u16CorrVIn20_up[1] = 3520;    
//  
//  limit.u16CorrVIn100_dw[0] = eep_read16(98);
//  if(limit.u16CorrVIn100_dw[0] == 0)
//    limit.u16CorrVIn100_dw[0] = 55;  
//  
//  limit.u16CorrVIn100_dw[1] = eep_read16(100);
//  if(limit.u16CorrVIn100_dw[1] == 0)
//    limit.u16CorrVIn100_dw[1] = 80;    
//  
//  limit.u16CorrVIn100_up[0] = eep_read16(102);
//  if(limit.u16CorrVIn100_up[0] == 0)
//    limit.u16CorrVIn100_up[0] = 3400;  
//  
//  limit.u16CorrVIn100_up[1] = eep_read16(104);
//  if(limit.u16CorrVIn100_up[1] == 0)
//    limit.u16CorrVIn100_up[1] = 3510;  
//
//  limit.u16CorrVIn_dw[0] = eep_read16(106);
//  if(limit.u16CorrVIn_dw[0] == 0)
//    limit.u16CorrVIn_dw[0] = 215;  
//  
//  limit.u16CorrVIn_dw[1] = eep_read16(108);
//  if(limit.u16CorrVIn_dw[1] == 0)
//    limit.u16CorrVIn_dw[1] = 300;    
//  
//  limit.u16CorrVIn_up[0] = eep_read16(110);
//  if(limit.u16CorrVIn_up[0] == 0)
//    limit.u16CorrVIn_up[0] = 1100;  
//  
//  limit.u16CorrVIn_up[1] = eep_read16(112);
//  if(limit.u16CorrVIn_up[1] == 0)
//    limit.u16CorrVIn_up[1] = 1400;    

/////////////////////////////////////////////////// S1 s2 ���궨������ֵ
//  limit.fs1PCorrectAOut_dw = eep_readf(114); 
//  limit.fs1PCorrectAOut_up = eep_readf(118);
//  limit.fs1NCorrectAOut_dw = eep_readf(122); 
//  limit.fs1NCorrectAOut_up = eep_readf(126); 
//  limit.fs1CorrectVOut20_dw = eep_readf(130); 
//  limit.fs1CorrectVOut20_up = eep_readf(134);
//  limit.fs1CorrectVOut100_dw = eep_readf(138); 
//  limit.fs1CorrectVOut100_up = eep_readf(142);   
//  limit.fs1CorrectVOut_dw = eep_readf(146); 
//  limit.fs1CorrectVOut_up = eep_readf(150);      
//  
//  limit.s16s1CorrectAIn_dw = eep_read16(154);
//  limit.s16s1CorrectAIn_up = eep_read16(156);
//  limit.s16s1CorrectVIn20_dw = eep_read16(158);
//  limit.s16s1CorrectVIn20_up = eep_read16(160);
//  limit.s16s1CorrectVIn100_dw = eep_read16(162);
//  limit.s16s1CorrectVIn100_up = eep_read16(164);
//  limit.s16s1CorrectVIn_dw = eep_read16(166);
//  limit.s16s1CorrectVIn_up = eep_read16(168);
//
//  limit.fs2PCorrectAOut_dw = eep_readf(170); 
//  limit.fs2PCorrectAOut_up = eep_readf(174);
//  limit.fs2NCorrectAOut_dw = eep_readf(178); 
//  limit.fs2NCorrectAOut_up = eep_readf(182); 
//  limit.fs2CorrectVOut20_dw = eep_readf(186); 
//  limit.fs2CorrectVOut20_up = eep_readf(190);
//  limit.fs2CorrectVOut100_dw = eep_readf(194); 
//  limit.fs2CorrectVOut100_up = eep_readf(198);   
//  limit.fs2CorrectVOut_dw = eep_readf(202); 
//  limit.fs2CorrectVOut_up = eep_readf(206);      
//  
//  limit.s16s2CorrectAIn_dw = eep_read16(210);
//  limit.s16s2CorrectAIn_up = eep_read16(212);
//  limit.s16s2CorrectVIn20_dw = eep_read16(214);
//  limit.s16s2CorrectVIn20_up = eep_read16(216);
//  limit.s16s2CorrectVIn100_dw = eep_read16(218);
//  limit.s16s2CorrectVIn100_up = eep_read16(220);
//  limit.s16s2CorrectVIn_dw = eep_read16(222);
//  limit.s16s2CorrectVIn_up = eep_read16(224);  
}

/*****************************************************************************************************************/
void main(void)                                                          
{
   //�ϵ��ʼ��
    
    clk_init();
    GPIO_init();    
    tim4_init();
    RelayInit();
    Usart1_Init();
    Usart2_Init();
    Usart3_Init();
    GetAddress();
    limit.u16MaxOKCount = 10;
    value.g_productType = 0;  //s1
    value.g_functionType = 0; //���
    value.g_bCaliPoint = 0;
    value.g_bStart = 0;
    value.g_u16ResultMeter = 0;
    value.g_bCaliFinish = 0;
    sdelay(4);
    while (1)
    {  
      if(value.g_bStart == 1)
      {
          if(value.g_functionType == 0) //��⹦��
          {
                TestPoint();
                msdelay(200);        
          }
          else if(value.g_functionType == 1) //�궨����
          {
                CalPoint();
                msdelay(200);           
          }    
          else
          {
             CloseAllRelay();
          }                   

      }
      else
      {
        CloseAllRelay();
      }
    
           
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
