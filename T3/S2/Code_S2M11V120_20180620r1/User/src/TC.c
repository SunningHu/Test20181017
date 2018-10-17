/////////////////////////////////////////////////////////////////////////////////////
// Copyright Pangu Inc. All Rights Reserved.
// FILE:        �ȵ�ż����
// DESC:        
// AUTHOR:      onepiece120116         
// ARCHIVE:     13/11
// CHANGES:                          
/////////////////////////////////////////////////////////////////////////////////////
#include "MathFunction.h"
#include <math.h>


//�����ȵ�ż�����ޣ���λ��
#define	 SIG_T_L            -270
#define	 SIG_T_H            400 
#define	 SIG_E_L            -270
#define	 SIG_E_H            1000
#define	 SIG_K_L            -270
#define	 SIG_K_H            1372
#define  SIG_S_L            -50
#define  SIG_S_H            1768.1
#define	 SIG_B_L            0
#define	 SIG_B_H            1820
#define  SIG_J_L            -210
#define  SIG_J_H            1200
#define	 SIG_R_L            -50
#define	 SIG_R_H            1768.1
#define	 SIG_N_L            -270
#define	 SIG_N_H            1300

//S���ȵ�żmvֵת��Ϊ�¶�
/****************************************************************
* S���ȵ�ż		 -50 ~ 250, -235uV ~ 1874uV
****************************************************************/
const double s_d0[10] = 
{
    0,
	1.84949460E-1,
	-8.00504062E-5,
	1.02237430E-7,
	-1.52248592E-10,
	1.88821343E-13,
	-1.59085941E-16,
	8.23027880E-20,
	-2.34181944E-23,
	2.79786260E-27,
};

/****************************************************************
* S���ȵ�ż		 250 ~ 1064, 1874uV ~ 10332uV
****************************************************************/
const double s_d1[10] = 
{
    1.291507177E1,
    1.466298863E-1,
    -1.534713402E-5,
    3.145945973E-9,
    -4.163257839E-13,
    3.187963771E-17,
    -1.291637500E-21,
    2.183475087E-26,
    -1.447379511E-31,
    8.211272125E-36,
};

/****************************************************************
* S���ȵ�ż		 1064 ~ 1664.5, 10332uV ~ 17536uV
****************************************************************/
const double s_d2[6] = 
{
    -8.087801117E1,
    1.621573104E-1,
    -8.536869453E-6,
    4.719686976E-10,
    -1.441693666E-14,
    2.081618890E-19,
};

/****************************************************************
* S���ȵ�ż		 1664.5 ~ 1768.1, 17536uV ~ 18694uV
****************************************************************/
const double s_d3[5] = 
{
    5.333875126E4,
    -1.235892298E1,
    1.092657613E-3,
    -4.265693686E-8,
    6.247205420E-13,
};

/****************************************************************
// Summary: 	S���ȵ�ż���� mvֵת��Ϊ�¶�	
// Parameter: 	[in/float]fE 	mvֵ 
// 	
// return:		[out/float]�¶�ֵ
****************************************************************/
float S_E2T(float fE)
{
    unsigned long i;
    double fT = 0;
    const double* pd;

    fE *= 1000;
    if(fE < -235)
    {
        i = 0;
        fT = SIG_S_L;
    }
    else if(fE >= -235 && fE <= 1874)
    {
        i = 10;
        pd = s_d0;
    }
    else if(fE > 1874 && fE <= 10332)
    {
        i = 10;
        pd = s_d1;
    }
    else if(fE > 10332 && fE <= 17536)
    {
        i = 6;
        pd = s_d2;
    }
    else if(fE > 17536 && fE <= 18694)
    {
        i = 5;
        pd = s_d3;
    }
    else
    {
        i = 0;
        fT = SIG_S_H;
    }

    for(; i > 0; i--)
        fT += PRDataLib_ZPow(fE, i - 1, pd[i - 1]);

    //�޷�
    if(fT < SIG_S_L)
        fT = SIG_S_L;
    if(fT > SIG_S_H)
        fT = SIG_S_H;

    return (float)fT;
}

//�¶�ת��Ϊmvֵ
/****************************************************************
* S���ȵ�ż		  -50 ~ 1064.18
****************************************************************/
const double s_c0[9] = 
{
    0,
    5.40313308631E-3,
    1.25934289740E-5,
    -2.32477968689E-8, 
    3.22028823036E-11,
    -3.31465196389E-14,
    2.55744251786E-17,
    -1.25068871393E-20,
    2.71443176145E-24,
};

/****************************************************************
* S���ȵ�ż		 1064.18 ~ 1664.5
****************************************************************/
const double s_c1[5] = 
{
    1.32900444085E-0,
    3.34509311344E-3, 
    6.54805192818E-6, 
    -1.64856259209E-9, 
    1.29989605174E-14,
};

/****************************************************************
* S���ȵ�ż	 	1664.5 ~ 1768.1
****************************************************************/
const double s_c2[5] = 
{
    1.46628232636E2, 
    -2.58430516752E-1, 
    1.63693574641E-4, 
    -3.30439046987E-8, 
    -9.43223690612E-15,
};

/****************************************************************
// Summary: 	S���ȵ�ż���� �¶�ת��Ϊmvֵ	
// Parameter: 	[in/float]fT 	�¶� 
// 	
// return:		[out/float]mvֵ
****************************************************************/
float S_T2E(float fT)
{
    unsigned long i;
    double fE = 0;
    const double* pc;

    if(fT >= -50 && fT <= 1064.18)
    {
        i = 9;
        pc = s_c0;
    }
    else if(fT > 1064.18 && fT <= 1664.5)
    {
        i = 5;
        pc = s_c1;
    }
    else if(fT > 1664.5 && fT <= 1768.1)
    {
        i = 5;
        pc = s_c2;
    }
    else
    {
        i = 0;
        fE = 0;
    }

    for(; i > 0; i--)
        fE += PRDataLib_ZPow(fT, i - 1, pc[i - 1]);

    return (float)fE;
}

//R���ȵ�żmvֵת��Ϊ�¶�
/****************************************************************
* R���ȵ�ż		 -50 ~ 250, -226 ~ 1923mv
****************************************************************/
const double r_d0[11] = 
{
    0,
    1.8891380E-1,
    -9.3835290E-5,
    1.3068619E-7,
    -2.2703580E-10,
    3.5145659E-13,
    -3.8953900E-16,
    2.8239471E-19,
    -1.2607281E-22,
    3.1353611E-26,
    -3.3187769E-30,
};

/****************************************************************
* R���ȵ�ż		 250 ~ 1064.18, 1923 ~ 11361mv
****************************************************************/
const double r_d1[10] = 
{
    1.334584505E1,
    1.472644573E-1,
    -1.844024844E-5,
    4.031129726E-9,
    -6.249428360E-13,
    6.468412046E-17,
    -4.458750426E-21,
    1.994710149E-25,
    -5.313401790E-30,
    6.481976217E-35,
};

/****************************************************************
* R���ȵ�ż		 1064.18 ~ 1664.5, 11361 ~ 19739mv
****************************************************************/
const double r_d2[6] = 
{
    -8.199599416E1,
    1.553962042E-1,
    -8.342197663E-6,
    4.279433549E-10,
    -1.191577910E-14,
    1.492290091E-19,
};

/****************************************************************
* R���ȵ�ż		 1664.5 ~ 1768.1, 19739 ~ 21103mv
****************************************************************/
const double r_d3[5] = 
{
    3.406177836E4,
    -7.023729171,
    5.582903813E-4,
    -1.952394635E-8,
    2.560740231E-13,
};

/****************************************************************
// Summary: 	R���ȵ�ż���� mvֵת��Ϊ�¶�	
// Parameter: 	[in/float]fE mvֵ	 
// 	
// return:		[out/float]�¶�
****************************************************************/
float R_E2T(float fE)
{
    unsigned long i;
    double fT = 0;
    const double* pd;

    fE *= 1000;
    if(fE < -226)
    {
        i = 0;
        fT = SIG_R_L;
    }
    else if(fE >= -226 && fE <= 1923)
    {
        i = 11;
        pd = r_d0;
    }
    else if(fE > 1923 && fE <= 11361)
    {
        i = 10;
        pd = r_d1;
    }
    else if(fE > 11361 && fE <= 19739)
    {
        i = 6;
        pd = r_d2;
    }
    else if(fE > 19739 && fE <= 21103)
    {
        i = 5;
        pd = r_d3;
    }
    else
    {
        i = 0;
        fT = SIG_R_H;
    }

    for(; i > 0; i--)
        fT += PRDataLib_ZPow(fE, i - 1, pd[i - 1]);

    //�޷�
    if(fT < SIG_R_L)
        fT = SIG_R_L;
    if(fT > SIG_R_H)
        fT = SIG_R_H;

    return (float)fT;
}

//R���ȵ�ż �¶�ת��Ϊmvֵ
/****************************************************************
* R���ȵ�ż		 -50 ~ 1064.18
****************************************************************/
const double r_c0[10] = 
{
    0,
    5.28961729765E-3,
    1.39166589782E-5,
    -2.38855693017E-8, 
    3.56916001063E-11,
    -4.62347666298E-14,
    5.00777441034E-17,
    -3.73105886191E-20,
    1.57716482367E-23,
	-2.81038625251E-27,
};

/****************************************************************
* R���ȵ�ż		1064.18 ~ 1664.5
****************************************************************/
const double r_c1[6] =
{
	2.95157925316,
	-2.52061251332E-3,
	1.59564501865E-5,
	-7.64085947576E-9,
	2.05305291024E-12,
	-2.93359668173E-16,
};

/****************************************************************
* R���ȵ�ż		1664.5 ~ 1768.1
****************************************************************/
const double r_c2[5] =
{
	1.52232118209E2,
	-2.68819888545E-1,
	1.71280280471E-4,
	-3.45895706453E-8,
	-9.34633971046E-15,
};

/****************************************************************
// Summary: 	R���ȵ�ż���� �¶�ת��Ϊmvֵ	
// Parameter: 	[in/float]fT 	�¶� 
// 	
// return:		[out/float]mvֵ
****************************************************************/
float R_T2E(float fT)
{
    unsigned long i;
    double fE = 0;
    const double* pc;

    if(fT >= -50 && fT <= 1064.18)
    {
        i = 10;
        pc = r_c0;
    }
    else if(fT > 1064.18 && fT <= 1664.5)
    {
        i = 6;
        pc = r_c1;
    }
    else if(fT > 1664.5 && fT <= 1768.1)
    {
        i = 5;
        pc = r_c2;
    }
    else
    {
        i = 0;
        fE = 0;
    }

    for(; i > 0; i--)
        fE += PRDataLib_ZPow(fT, i - 1, pc[i - 1]);

    return (float)fE;	
}

//B���ȵ�ż  mvֵת��Ϊ�¶� 
/****************************************************************
* B���ȵ�ż		0 ~ 630.615, 0.291 ~ 2.431
****************************************************************/
const double b_d0[9] = 
{
    9.8423321E1,
    6.9971500E-1,
    -8.4765304E-4,
    1.0052644E-6,
    -8.3345952E-10,
    4.5508542E-13,
    -1.5523037E-16,
    2.9886750E-20,
    -2.4742860E-24,
};

/****************************************************************
* B���ȵ�ż		630.615 ~ 1820, 2.431 ~ 13.82
****************************************************************/
const double b_d1[9] = 
{
    2.1315071E2,
    2.8510504E-1,
    -5.2742887E-5,
    9.9160804E-9,
    -1.2965303E-12,
    1.1195870E-16,
    -6.0625199E-21,
    1.8661696E-25,
    -2.4878585E-30,
};

/****************************************************************
// Summary: 	R���ȵ�ż���� mvֵת��Ϊ�¶�	
// Parameter: 	[in/float]fE 	mvֵ
// 	
// return:		[out/float]�¶�
****************************************************************/
float B_E2T(float fE)
{
    unsigned long i;
    double fT = 0;
    const double* pd;

    fE *= 1000;
    if(fE < 291)
    {
        i = 0;
        fT = SIG_B_L;
    }
    else if(fE >= 291 && fE <= 2431)
    {
        i = 9;
        pd = b_d0;
    }
    else if(fE > 2431 && fE <= 13820)
    {
        i = 9;
        pd = b_d1;
    }
    else
    {
        i = 0;
        fT = SIG_B_H;
    }

    for(; i > 0; i--)
        fT += PRDataLib_ZPow(fE, i - 1, pd[i - 1]);

    //�޷�
    if(fT < SIG_B_L)
        fT = SIG_B_L;
    if(fT > SIG_B_H)
        fT = SIG_B_H;

    return (float)fT;
}

//B���ȵ�ż   �¶�ת��Ϊmvֵ 
/****************************************************************
* B���ȵ�ż		0 ~ 630.615, 0.291 ~ 2.431
****************************************************************/
const double b_c0[7] =
{
	0,
	-2.4650848346E-4,
	5.9040421171E-6,
	-1.3257931636E-9,
	1.5668291901E-12,
	-1.6944529240E-15,
	6.2990347094E-19,
};

/****************************************************************
* B���ȵ�ż		630.615 ~ 1820, 2.431 ~ 13.82
****************************************************************/
const double b_c1[9] =
{
	-3.8938168621,
	2.8571747470E-2,
	-8.4885104785E-5,
	1.5785280164E-7,
	-1.6835344864E-10,
	1.1109794013E-13,
	-4.4515431033E-17,
	9.8975640821E-21,
	-9.3791330289E-25,
};

/****************************************************************
// Summary: 	B���ȵ�ż����  �¶�ת��Ϊmvֵ	
// Parameter: 	[in/float]fT 	�¶�
// 	
// return:		[out/float]mvֵ
****************************************************************/
float B_T2E(float fT)
{    
	unsigned long i;
    double fE = 0;
    const double* pc;

    if(fT >= 0 && fT <= 630.615)
    {
        i = 7;
        pc = b_c0;
    }
    else if(fT > 630.15 && fT <= 1820)
    {
        i = 9;
        pc = b_c1;
    }
    else
    {
        i = 0;
        fE = 0;
    }

    for(; i > 0; i--)
        fE += PRDataLib_ZPow(fT, i - 1, pc[i - 1]);

    return (float)fE;	
}

//K���ȵ�ż   �¶�ת��Ϊmvֵ 
/****************************************************************
* K���ȵ�ż		-200 ~ 0, -5.891 ~ 0
****************************************************************/
const double k_d0[9] = 
{
    0,
    2.5173462E-2,
    -1.1662878E-6,
    -1.0833638E-9,
    -8.9773540E-13,
    -3.7342377E-16,
    -8.6632643E-20,
    -1.0450598E-23,
    -5.1920577E-28,
};

/****************************************************************
* K���ȵ�ż		0 ~ 500, 0 ~ 20.644
****************************************************************/
const double k_d1[10] =
{
    0,
    2.508355E-2,
    7.860106E-8,
    -2.503131E-10,
    8.315270E-14,
    -1.228034E-17,
    9.804036E-22,
    -4.413030E-26,
    1.057734E-30,
    -1.052755E-35,
};

/****************************************************************
* K���ȵ�ż		500 ~ 1372, 20.644 ~ 54.886
****************************************************************/
const double k_d2[7] =
{
    -1.318058E2,
    4.830222E-2,
    -1.646031E-6,
    5.464731E-11,
    -9.650715E-16,
    8.802193E-21,
    -3.110810E-26,
};

/****************************************************************
// Summary: 	K���ȵ�ż����  mvֵת��Ϊ�¶�	
// Parameter: 	[in/float]fE	mvֵ
// 	
// return:		[out/float]�¶�
****************************************************************/
float K_E2T(float fE)
{
    unsigned long i;
    double fT = 0;
    const double* pd;

    fE *= 1000;
    if(fE < -5891)
    {
        i = 0;
        fT = SIG_K_L;
    }
    else if(fE >= -5891 && fE <= 0)
    {
        i = 9;
        pd = k_d0;
    }
    else if(fE > 0 && fE <= 20644)
    {
        i = 10;
        pd = k_d1;
    }
    else if(fE > 20644 && fE <= 54886)
    {
        i = 7;
        pd = k_d2;
    }
    else
    {
        i = 0;
        fT = SIG_K_H;
    }

    for(; i > 0; i--)
        fT += PRDataLib_ZPow(fE, i - 1, pd[i - 1]);

    //�޷�
    if(fT < SIG_K_L)
        fT = SIG_K_L;
    if(fT > SIG_K_H)
        fT = SIG_K_H;

    return (float)fT;
}
/*
//K���ȵ�ż   �¶�ת��Ϊmvֵ 
const static float Tab_K_E[] = 
{
    0.000f, 0.397f, 0.798f, 1.203f, 1.612f, 
    2.203f, 2.436f, 2.851f, 3.267f, 3.682f,
    4.096f  
};

****************************************************************
// Summary: 	K���ȵ�ż����  �¶�ת��Ϊmvֵ	
// Parameter: 	[in/float]fT 	�¶�
// 	
// return:		[out/float]mvֵ
****************************************************************
float K_T2E(float fT)
{
    return SeekTable_T_E(fT, 0, 100, 10, Tab_K_E, sizeof(Tab_K_E) / 4);
}
*/
// ���ڹ�ʽ��Ƶ�exp������ѡ�ò����ʽ��ѯ���Ի�ȡ����Ч��
// �����ò��ִ���
//-270~0
const double k_a[11] =
{	
	0,
	3.9450128025E-2,
	2.3622373598E-5,
	-3.2858906784E-7,
	-4.9904828777E-9,
	-6.7509059173E-11,
	-5.7410327428E-13,
	-3.1088872894E-15,
	-1.0451609365E-17,
	-1.9889266878E-20,
	-1.6322697486E-23,
};
//0~1372
const double k_b[10] =
{
	0,
	3.8921204975E-2,
	1.8558770032E-5,
	-9.9457592874E-8,
	3.1840945719E-10,
	-5.6072844889E-13,
	5.6075059059E-16,
	-3.2020720003E-19,
	9.7151147152E-23,
	-1.2104721275E-26,
};
const double k_c[2] =
{
	1.185976E-1,
	-1.183432E-4,
};
const double k_d = -1.7600413686E-2;

float K_T2E(float fT)
{
    unsigned long i;
    double fE = 0;
    const double* pc;
	double fLinshi1,fLinshi2;

    if(fT >= -270 && fT <= 0)
    {
        i = 11;
        pc = k_a;
    }
    else if(fT > 0 && fT <= 1372)
    {
        i = 10;
        pc = k_b;
    }
    else
    {
        i = 0;
        fE = 0;
    }

    for(; i > 0; i--)
        fE += PRDataLib_ZPow(fT, i - 1, pc[i - 1]);
	if(fT > 0 && fT <= 1372)
	{
		fLinshi1 = (fT-126.9686)*(fT-126.9686);
		fLinshi1 *= k_c[1];
		//fLinshi2 = PRDataLib_ZPow(2.718281828, (unsigned short)(fLinshi1), k_c[0]); //exp(fLinshi1);
                fLinshi2 = k_c[0] * exp(fLinshi1);
		fE = fE + fLinshi2 + k_d ;

	}
    return (float)fE;
}


//N���ȵ�ż   mvֵת��Ϊ�¶� 
/****************************************************************
* N���ȵ�ż		-200 ~ 0, -3.99 ~ 0
****************************************************************/
const double n_d0[10] = 
{
    0,
    3.8436847E-2,
    1.1010485E-6,
    5.2229312E-9,
    7.2060525E-12,
    5.8488586E-15,
    2.7754916E-18,
    7.7075166E-22,
    1.1582665E-25,
    7.3138868E-30,
};

/****************************************************************
* N���ȵ�ż		0 ~ 600, 0 ~ 20.613
****************************************************************/
const double n_d1[8] =
{
    0,
    3.86896E-2,
    -1.08267E-6,
    4.70205E-11,
    -2.12169E-18,
    -1.17272E-19,
    5.39280E-24,
    -7.98156E-29,
};

/****************************************************************
* N���ȵ�ż		600 ~ 1300, 20.613 ~ 47.513
****************************************************************/
const double n_d2[6] =
{
    1.972485E1,
    3.300943E-2,
    -3.915159E-7,
    9.855391E-12,
    -1.274371E-16,
    7.767022E-22,
};

/****************************************************************
// Summary: 	N���ȵ�ż����  mvֵת��Ϊ�¶�	
// Parameter: 	[in/float]fE 	mvֵ
// 							  
// return:		[out/float]�¶�
****************************************************************/
float N_E2T(float fE)
{
    unsigned long i;
    double fT = 0;
    const double* pd;

    fE *= 1000;
    if(fE < -3990)
    {
        i = 0;
        fT = SIG_N_L;
    }
    else if(fE >= -3990 && fE <= 0)
    {
        i = 10;
        pd = n_d0;
    }
    else if(fE > 0 && fE <= 20613)
    {
        i = 8;
        pd = n_d1;
    }
    else if(fE > 20613 && fE <= 47513)
    {
        i = 6;
        pd = n_d2;
    }
    else
    {
        i = 0;
        fT = SIG_N_H;
    }

    for(; i > 0; i--)
        fT += PRDataLib_ZPow(fE, i - 1, pd[i - 1]);

    //�޷�
    if(fT < SIG_N_L)
        fT = SIG_N_L;
    if(fT > SIG_N_H)
        fT = SIG_N_H;

    return (float)fT;
}

//N���ȵ�ż  �¶�ת��Ϊmvֵ 
/****************************************************************
* N���ȵ�ż		-270 ~ 0
****************************************************************/
const double n_c0[9] =
{
	0,
	2.6159105962E-2,
	1.0957484228E-5,
	-9.3841111554E-8,
	-4.6412039759E-11,
	-2.6303357716E-12,
	-2.2653438003E-14,
	-7.6089300791E-17,
	-9.3419667835E-20,
};

/****************************************************************
* N���ȵ�ż		0 ~ 1300
****************************************************************/
const double n_c1[11] =
{
	0,
	2.5929394601E-2,
	1.5710141880E-5,
	4.3825627237E-8,
	-2.5261169794E-10,
	6.4311819339E-13,
	-1.0063471519E-15,
	9.9745338992E-19,
	-6.0863245607E-22,
	2.0849229339E-25,
	-3.0682196151E-29,
};

/****************************************************************
// Summary: 	N���ȵ�ż����  �¶�ת��Ϊmvֵ	
// Parameter: 	[in/float]fT 	�¶�
// 	
// return:		[out/float]mvֵ
****************************************************************/
float N_T2E(float fT)
{
    unsigned long i;
    double fE = 0;
    const double* pc;

    if(fT >= -270 && fT <= 0)
    {
        i = 9;
        pc = n_c0;
    }
    else if(fT > 0 && fT <= 1300)
    {
        i = 11;
        pc = n_c1;
    }
    else
    {
        i = 0;
        fE = 0;
    }

    for(; i > 0; i--)
        fE += PRDataLib_ZPow(fT, i - 1, pc[i - 1]);

    return (float)fE;	
}

//E���ȵ�ż  �¶�ת��Ϊmvֵ
/****************************************************************
* E���ȵ�ż		-200 ~ 0, -8.825 ~ 0
****************************************************************/
const double e_d0[9] = 
{
    0,
    1.6977288E-2,
    -4.3514970E-7,
    -1.5859697E-10,
    -9.2502871E-14,
    -2.6084314E-17,
    -4.1360199E-21,
    -3.4034030E-25,
    -1.1564890E-29,
};

/****************************************************************
* E���ȵ�ż		0 ~ 1000, 0 ~ 76.373
****************************************************************/
const double e_d1[10] = 
{
    0,
    1.7057035E-2,
    -2.3301759E-7,
    6.5435585E-12,
    -7.3562749E-17,
    -1.7896001E-21,
    8.4036165E-26,
    -1.3735879E-30,
    1.0629823E-35,
    -3.2447087E-41,
};

/****************************************************************
// Summary: 	E���ȵ�ż����  mvֵת��Ϊ�¶�	
// Parameter: 	[in/float]fE 	mvֵ
// 	
// return:		[out/float]�¶�
****************************************************************/
float E_E2T(float fE)
{
    unsigned long i;
    double fT = 0;
    const double* pd;

    fE *= 1000;
    if(fE < -8825)
    {
        i = 0;
        fT = SIG_E_L;
    }
    else if(fE >= -8825 && fE <= 0)
    {
        i = 9;
        pd = e_d0;
    }
    else if(fE > 0 && fE <= 76373)
    {
        i = 10;
        pd = e_d1;
    }
    else
    {
        i = 0;
        fT = SIG_E_H;
    }

    for(; i > 0; i--)
    {
        fT += (PRDataLib_ZPow(fE, i - 1,pd[i - 1]));
    }
    //�޷�
    if(fT < SIG_E_L)
        fT = SIG_E_L;
    if(fT > SIG_E_H)
        fT = SIG_E_H;

    return (float)fT;
}

//E���ȵ�ż �¶�ת��Ϊmvֵ
/****************************************************************
* E���ȵ�ż		-270 ~ 0
****************************************************************/
const double e_c0[14] =
{
	0,
	5.8665508708E-2,
	4.5410977124E-5,
	-7.7998048686E-7,
	-2.5800160843E-8,
	-5.9452583057E-10,
	-9.3214058667E-12,
	-1.0287605534E-13,
	-8.0370123621E-16,
	-4.3979497391E-18,
	-1.6414776355E-20,
	-3.9673619516E-23,
	-5.5827328721E-26,
	-3.4657842013E-29,
};

/****************************************************************
* E���ȵ�ż		0 ~ 1000
****************************************************************/
const double e_c1[11] =
{
	0,
	5.8665508710E-2,
	4.5032275582E-5,
	2.8908407212E-8,
	-3.3056896652E-10,
	6.5024403270E-13,
	-1.9197495504E-16,
	-1.2536600497E-18,
	2.1489217569E-21,
	-1.4388041782E-24,
	3.5960899481E-28,
};

/****************************************************************
// Summary: 	E���ȵ�ż����  �¶�ת��Ϊmvֵ	
// Parameter: 	[in/float]fT 	�¶�
// 	
// return:		[out/float]mvֵ
****************************************************************/
float E_T2E(float fT)
{
    unsigned long i;
    double fE = 0;
    const double* pc;

    if(fT >= -270 && fT <= 0)
    {
        i = 14;
        pc = e_c0;
    }
    else if(fT > 0 && fT <= 1000)
    {
        i = 11;
        pc = e_c1;
    }
    else
    {
        i = 0;
        fE = 0;
    }

    for(; i > 0; i--)
        fE += PRDataLib_ZPow(fT, i - 1 , pc[i - 1]);

    return (float)fE;
}

//J���ȵ�ż �¶�ת��Ϊmvֵ
/****************************************************************
* J���ȵ�ż		-174 ~ 0
****************************************************************/
const double j_d0[9] =
{
    0,
    1.9528268E-2,
    -1.2286185E-6,
    -1.0752178E-9,
    -5.9086933E-13,
    -1.7256713E-16,
    -2.8131513E-20,
    -2.3963370E-24,
    -8.3823321E-29,
};

/****************************************************************
* J���ȵ�ż		0 ~ 573
****************************************************************/
const double j_d1[8] =
{
    0,
    1.978425E-2,
    -2.001204E-7,
    1.036969E-11,
    -2.549687E-16,
    3.585153E-21,
    -5.344285E-26,
    5.099890E-31,
};

/****************************************************************
* J���ȵ�ż		573 ~ 910
****************************************************************/
const double j_d2[6] =
{
    -3.11358187E3,
    3.00543684E-1,
    -9.94773230E-6,
    1.70276630E-10,
    -1.43033468E-15,
    4.73886084E-21,
};

/****************************************************************
// Summary: 	J���ȵ�ż����  mvֵת��Ϊ�¶�	
// Parameter: 	[in/float]fE 	mvֵ
// 	
// return:		[out/float]�¶�
****************************************************************/
float J_E2T(float fE)
{
    unsigned long i;
    double fT = 0;
    const double* pd;

    fE *= 1000;
    if(fE < -8095)
    {
        i = 0;
        fT = SIG_J_L;
    }
    else if(fE >= -8095 && fE <= 0)
    {
        i = 9;
        pd = j_d0;
    }
    else if(fE > 0 && fE <= 42919)
    {
        i = 8;
        pd = j_d1;
    }
    else if(fE > 42919 && fE <= 69553)
    {
        i = 6;
        pd = j_d2;
    }
    else
    {
        i = 0;
        fT = SIG_J_H;
    }

    for(; i > 0; i--)
        fT += PRDataLib_ZPow(fE, i - 1, pd[i - 1]);

    //�޷�
    if(fT < SIG_J_L)
        fT = SIG_J_L;
    if(fT > SIG_J_H)
        fT = SIG_J_H;

    return (float)fT;
}

//J���ȵ�ż mvֵת��Ϊ�¶�
/****************************************************************
* J���ȵ�ż		-210 ~ 760
****************************************************************/
const double j_c0[9] =
{
	0,
	5.0381187815E-2,
	3.0475836930E-5,
	-8.5681065720E-8,
	1.3228195295E-10,
	-1.7052958337E-13,
	2.0948090697E-16,
	-1.2538395336E-19,
	1.5631725697E-23,
};

/****************************************************************
* J���ȵ�ż		760 ~ 1200
****************************************************************/
const double j_c1[6] =
{
	2.9645625681E2,
	-1.4976127786,
	3.1787103924E-3,
	-3.1847686701E-6,
	1.5720819004E-9,
	-3.0691369056E-13,
};

/****************************************************************
// Summary: 	J���ȵ�ż����  �¶�ת��Ϊmvֵ	
// Parameter: 	[in/float]fT 	�¶�
// 	
// return:		[out/float]mvֵ
****************************************************************/
float J_T2E(float fT)
{
    unsigned long i;
    double fE = 0;
    const double* pc;

    if(fT >= -210 && fT <= 760)
    {
        i = 9;
        pc = j_c0;
    }
    else if(fT > 760 && fT <= 1200)
    {
        i = 6;
        pc = j_c1;
    }
    else
    {
        i = 0;
        fE = 0;
    }

    for(; i > 0; i--)
        fE += PRDataLib_ZPow(fT, i - 1, pc[i - 1]);

    return (float)fE;
}

//T���ȵ�ż mvֵת��Ϊ�¶�
/****************************************************************
* T���ȵ�ż		-200 ~ 0
****************************************************************/
const double t_d0[8] = 
{
    0,
    2.5949192E-2,
    -2.1316967E-7,
    7.9018692E-10,
    4.2527777E-13,
    1.3304473E-16,
    2.0241446E-20,
    1.2668171E-24,
};

/****************************************************************
* T���ȵ�ż		0 ~ 400
****************************************************************/
const double t_d1[7] = 
{
    0,
    2.592800E-2,
    -7.602961E-7,
    4.637791E-11,
    -2.165394E-15,
    6.048144E-20,
    -7.293422E-25,
};

/****************************************************************
// Summary: 	T���ȵ�ż����  mvֵת��Ϊ�¶�	
// Parameter: 	[in/float]fE 	mvֵ
// 	
// return:		[out/float]�¶�ֵ
****************************************************************/
float T_E2T(float fE)
{
    unsigned long i;
    double fT = 0;
    const double* pd;

    fE *= 1000;
    if(fE < -5603)
    {
        i = 0;
        fT = SIG_T_L;
    }
    else if(fE >= -5603 && fE <= 0)
    {
        i = 8;
        pd = t_d0;
    }
    else if(fE > 0 && fE <= 20872)
    {
        i = 7;
        pd = t_d1;
    }
    else
    {
        i = 0;
        fT = SIG_T_H;
    }

    for(; i > 0; i--)
        fT += PRDataLib_ZPow(fE, i - 1, pd[i - 1]);

    //�޷�
    if(fT < SIG_T_L)
        fT = SIG_T_L;
    if(fT > SIG_T_H)
        fT = SIG_T_H;

    return (float)fT;
}

//T���ȵ�ż mvֵת��Ϊ�¶�
/****************************************************************
* T���ȵ�ż		-270 ~ 0
****************************************************************/
const double t_c0[15] =
{
	0,
	3.8748106364E-2,
	4.4194434347E-5,
	1.1844323105E-7,
	2.0032973554E-8,
	9.0138019559E-10,
	2.2651156593E-11,
	3.6071154205E-13,
	3.8493939883E-15,
	2.8213521925E-17,
	1.4251594779E-19,
	4.8768662286E-22,
	1.0795539270E-24,
	1.3945027062E-27,
	7.9795153927E-31,
};

/****************************************************************
* T���ȵ�ż		0 ~ 400
****************************************************************/
const double t_c1[9] =
{
	0,
	3.8748106364E-2,
	3.3292227880E-5,
	2.0618243404E-7,
	-2.1882256846E-9,
	1.0996880928E-11,
	-3.0815758772E-14,
	4.5479135290E-17,
	-2.7512901673E-20,
};

/****************************************************************
// Summary: 	T���ȵ�ż����  �¶�ת��Ϊmvֵ	
// Parameter: 	[in/float]fT 	�¶�
// 	
// return:		[out/float]mvֵ
****************************************************************/
float T_T2E(float fT)
{
    unsigned long i;
    double fE = 0;
    const double* pc;

    if(fT >= -270 && fT <= 0)
    {
        i = 15;
        pc = t_c0;
    }
    else if(fT > 0 && fT <= 400)
    {
        i = 9;
        pc = t_c1;
    }
    else
    {
        i = 0;
        fE = 0;
    }

    for(; i > 0; i--)
        fE += PRDataLib_ZPow(fT, i - 1, pc[i - 1]);

    return (float)fE;	
}

