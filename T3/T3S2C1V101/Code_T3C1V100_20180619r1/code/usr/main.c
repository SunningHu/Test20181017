/***************************************************************
产品名称：全自动标定系统
开发代号：T2
开发人员：钱泽爽(嵌入式软件)，徐冬、胡镇俊(上位机软件)，赵李丹(硬件)，周凯(结构)
开发时间：2016.12.20-2017.05.01
***************************************************************/
 


/*****************************************
所有变量，引用的头文件，全部都在main.h里面
定义
*****************************************/
#include "main.h"
#include "math.h"



//Summary:    主时钟初始化
void clk_init(void)
{
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);  
}


//Summary:    定时器TIM4初始化
void tim4_init(void)
{
    //默认TIM4时钟是关闭的，则需要先开启TIM4的时钟，之后才能对TIM4寄存器进行初始化，切记顺序不可反
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE); 
    //1、TIM4是个8位的定时器，最大计数可以达到255
    //2、系统时钟是16MHZ的，定时器使用的是系统时钟源
    //3、想得到1ms的时基，则需要将时钟分频
    //4、系统时钟128分频，可以得到 16000000 / 128 = 125K的时钟
    //5、时钟设定值公式为 (X * 1000) / 125000 = 1;
    //6、得到X = 125，然后将 X - 1 就得到124，这个值就是我们对定时器的填充值
    TIM4_TimeBaseInit(TIM4_Prescaler_128, 124);
    //清楚TIM4定时器溢出标志位
    TIM4_ClearFlag(TIM4_FLAG_Update);
    //使能TIM4定时器溢出中断
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);
    //开启总中断
    enableInterrupts();
    //启动TIM4定时器
    TIM4_Cmd(ENABLE);     
}

//Summary:    GPIO口初始化
void GPIO_init(void)
{
    GPIO_Init(LED_RUN_PORT, LED_RUN_PIN, GPIO_Mode_Out_PP_High_Slow); //  运行灯
    GPIO_Init(RELAY_M_PORT, RELAY_M_PIN, GPIO_Mode_Out_PP_High_Slow); //  测量继电器
    GPIO_Init(RELAY_S_PORT, RELAY_S_PIN, GPIO_Mode_Out_PP_High_Slow); //  输出继电器
    GPIO_Init(RELAY_MA_PORT, RELAY_MA_PIN, GPIO_Mode_Out_PP_High_Slow) ;//  标定欧姆继电器
    GPIO_Init(RELAY_W_PORT, RELAY_W_PIN, GPIO_Mode_Out_PP_High_Slow); //  预留
    
    //拨码开关
    GPIO_Init(KEY_1_PORT, KEY_1_PIN, GPIO_Mode_In_PU_No_IT); //  key1
    GPIO_Init(KEY_2_PORT, KEY_2_PIN, GPIO_Mode_In_PU_No_IT); //  key2
    GPIO_Init(KEY_3_PORT, KEY_3_PIN, GPIO_Mode_In_PU_No_IT); //  key3
    GPIO_Init(KEY_4_PORT, KEY_4_PIN, GPIO_Mode_In_PU_No_IT); //  key4
    GPIO_Init(KEY_5_PORT, KEY_5_PIN, GPIO_Mode_In_PU_No_IT); //  key5
    GPIO_Init(KEY_6_PORT, KEY_6_PIN, GPIO_Mode_In_PU_No_IT); //  key6

}

/*******************************************************
* Summary:    继电器初始化
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
* Summary:    关闭所有继电器
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

//Summary:    烧录继电器切换
void CodeRelayTab(void)
{

}



//Summary:    得到本机地址
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
* Summary:    输出继电器切换
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
* Summary:    测量继电器切换
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
* Summary:    测量继电器切换
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


//Summary:    标定某一个点
void CalPoint(void)
{
    switch(value.g_bCaliPoint)
    { 
        case 0:
            PCorrComm();              //标定通讯测试
            break;
        case 1:
            SourceRelayTab_Test();    //信号发生器输出继电器切换
            PCorrAOutDown();          //标定有源20mA输出下限值
            break;
        case 2:
            SourceRelayTab_Test();    //信号发生器输出继电器切换
            PCorrAOutUp();            //标定有源20mA输出上限值
            break;
        case 3:
            SourceRelayTab_Test();    //信号发生器输出继电器切换
            NCorrAOutDown();          //标定无源20mA输出下限值
            break;
        case 4 :
            SourceRelayTab_Test();    //信号发生器输出继电器切换
            NCorrAOutUp();            //标定无源20mA输出上限值
            break;
        case 5:
            SourceRelayTab_Test();    //信号发生器输出继电器切换
            CorrVOut20Down();         //标定20mV输出下限值
            break;
        case 6:
            SourceRelayTab_Test();    //信号发生器输出继电器切换
            CorrVOut20Up();           //标定20mV输出上限值
            break;
        case 7:
            SourceRelayTab_Test();    //信号发生器输出继电器切换
            CorrVOut100Down();        //标定100mV输出下限值
            break;
        case 8:
            SourceRelayTab_Test();    //信号发生器输出继电器切换
            CorrVOut100Up();          //标定100mV输出上限值
            break;
        case 9:
            SourceRelayTab_Test();    //信号发生器输出继电器切换
            CorrVOutDown();           //标定V输出下限值
            break;
        case 10:
            SourceRelayTab_Test();    //信号发生器输出继电器切换
            CorrVOutUp();             //标定V输出上限值
            break;
        case 11:
            MeasureRelayTab_Cail();   //信号发生器测量继电器切换
            CorrROut3100();            //标定R 3mA 100
            break;              
        case 12:
            MeasureRelayTab_Cail();   //信号发生器测量继电器切换
            CorrROut1300();            //标定R 1mA 300
            break;  
        case 13:
            MeasureRelayTab_Cail();   //信号发生器测量继电器切换
            CorrROut1100();            //标定R 1mA 100
            break;  
        case 14:
            MeasureRelayTab_Cail();   //信号发生器测量继电器切换
            CorrROut5300();            //标定R 0.5mA 300
            break;  
        case 15:
            MeasureRelayTab_Cail();   //信号发生器测量继电器切换
            CorrROut5100();            //标定R 0.5mA 100
            break;  
        case 16:
            MeasureRelayTab_Cail();   //信号发生器测量继电器切换
            CorrROut2100();            //标定R 0.2mA 100
            break;  
        case 17:
            MeasureRelayTab_Test();   //信号发生器测量继电器切换
            CorrAInDown();            //标定测量电流下限值
            break;
        case 18:
            MeasureRelayTab_Test();   //信号发生器测量继电器切换
            CorrAInUp();              //标定测量电流上限值
            break;
        case 19:
            MeasureRelayTab_Test();   //信号发生器测量继电器切换
            CorrVIn20Down();          //标定测量20mV下限值
            break;
        case 20:
            MeasureRelayTab_Test();   //信号发生器测量继电器切换
            CorrVIn20Up();            //标定测量20mV上限值
            break;
        case 21:
            MeasureRelayTab_Test();   //信号发生器测量继电器切换
            CorrVIn100Down();         //标定测量100mV下限值
            break;
        case 22:
            MeasureRelayTab_Test();   //信号发生器测量继电器切换
            CorrVIn100Up();           //标定测量100mV上限值
            break;
        case 23:
            MeasureRelayTab_Test();   //信号发生器测量继电器切换
            CorrVInDown();            //标定测量V下限值
            break;
        case 24:
            MeasureRelayTab_Test();   //信号发生器测量继电器切换
            CorrVInUp();              //标定测量V上限值  
            break;
            default:;
    }
}






/*******************************************************
* Summary:    检测信号点
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
            CloseAllRelay();       //继电器初始化
            PTestTemp();           // 冷端温度检测
            break;           
        case 1:
            SourceRelayTab_Test(); //信号发生器输出继电器切换
            PTestAOutDown();       // 有源2mA输出检测 
            break;
        case 2:
            SourceRelayTab_Test(); //信号发生器输出继电器切换
            PTestAOutUp();         // 有源21mA输出检测 
            break;
        case 3:
            SourceRelayTab_Test(); //信号发生器输出继电器切换
            NTestAOutDown();       //  无源2mA输出检测
            break;
        case 4 :
            SourceRelayTab_Test(); //信号发生器输出继电器切换
            NTestAOutUp();         //  无源21mA输出检测
            break;
        case 5:
            SourceRelayTab_Test(); //信号发生器输出继电器切换
            TestVOut20Down();      // 2mV输出检测
            break;
        case 6:
            SourceRelayTab_Test(); //信号发生器输出继电器切换
            TestVOut20Up();        // 18mV输出检测
            break;
        case 7:
            SourceRelayTab_Test(); //信号发生器输出继电器切换
            TestVOut100Down();     // 10mV输出检测
            break;
        case 8:
            SourceRelayTab_Test(); //信号发生器输出继电器切换
            TestVOut100Up();       // 90mV输出检测
            break;
        case 9:
            SourceRelayTab_Test(); //信号发生器输出继电器切换
            TestVOutDown();        // 0.000V输出检测
            break;
        case 10:
            SourceRelayTab_Test(); //信号发生器输出继电器切换
            TestVOutUp();          // 15.000V输出检测
            break;
        case 11:
            SourceRelayTab_Test(); //信号发生器输出继电器切换
            TestROutDown();        // 50.0欧姆输出检测
            break;
        case 12:
            SourceRelayTab_Test(); //信号发生器输出继电器切换
            TestROutUp();          // 250.0欧姆输出检测
            break;            
        case 13:
            MeasureRelayTab_Test();     //信号发生器测量继电器切换
            TestAInDown();          //  测量2mA检测
            break;
        case 14:
            MeasureRelayTab_Test();    //信号发生器测量继电器切换
            TestAInUp();          //  测量21mA检测
            break;
        case 15:
            MeasureRelayTab_Test();    //信号发生器测量继电器切换
            TestVIn20Down();      //标定测量20mV下限值
            break;
        case 16:
            MeasureRelayTab_Test();    //信号发生器测量继电器切换
            TestVIn20Up();        //标定测量20mV上限值
            break;
        case 17:
            MeasureRelayTab_Test();   //信号发生器测量继电器切换
            TestVIn100Down();    //标定测量100mV下限值
            break;
        case 18:
            MeasureRelayTab_Test();   //信号发生器测量继电器切换
            TestVIn100Up();      //标定测量100mV上限值
            break;
        case 19:
            MeasureRelayTab_Test();  //信号发生器测量继电器切换
            TestVInDown();          //标定测量V下限值
            break;
        case 20:
            MeasureRelayTab_Test();  //信号发生器测量继电器切换
            TestVInUp();             //标定测量V上限值  
            break;
        case 21:
            MeasureRelayTab_Test();  //信号发生器测量继电器切换
            TestRInDown();           // 50.0欧姆检测
            break;
        case 22:
            MeasureRelayTab_Test();  //信号发生器测量继电器切换
            TestRInUp();             // 250欧姆检测  
            break;            
            default:;
    }
}

void LimitInit(void)
{
//////////////////////////////////////////  连续稳定N次算信号稳定  默认为10
  limit.u16MaxOKCount = eep_read16(0);
  if(limit.u16MaxOKCount == 0)
    limit.u16MaxOKCount = 10;
//////////////////////////////////////////  各信号点 码值范围设置  
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

/////////////////////////////////////////////////// S1 s2 各标定量修正值
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
   //上电初始化
    
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
    value.g_functionType = 0; //检测
    value.g_bCaliPoint = 0;
    value.g_bStart = 0;
    value.g_u16ResultMeter = 0;
    value.g_bCaliFinish = 0;
    sdelay(4);
    while (1)
    {  
      if(value.g_bStart == 1)
      {
          if(value.g_functionType == 0) //检测功能
          {
                TestPoint();
                msdelay(200);        
          }
          else if(value.g_functionType == 1) //标定功能
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
