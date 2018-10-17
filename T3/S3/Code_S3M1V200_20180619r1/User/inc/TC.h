/////////////////////////////////////////////////////////////////////////////////////
// Copyright Pangu Inc. All Rights Reserved.
// FILE:        TC.h
// DESC:        热电偶计算头文件
// AUTHOR:      onepiece120116          
// ARCHIVE:     13/11
// CHANGES:                          
/////////////////////////////////////////////////////////////////////////////////////

#ifndef _TC_H_
#define _TC_H_

float S_E2T(float fE);																//S型热电偶
float S_T2E(float fT);

float R_E2T(float fE);															   	//R型热电偶
float R_T2E(float fT);																

float B_E2T(float fE);																//B型热电偶
float B_T2E(float fT);

float K_E2T(float fE); 																//K型热电偶
float K_T2E(float fT);

float N_E2T(float fE);																//N型热电偶
float N_T2E(float fT);

float E_E2T(float fE);																//E型热电偶
float E_T2E(float fT);

float J_E2T(float fE);																//J型热电偶
float J_T2E(float fT);

float T_E2T(float fE);																//T型热电偶
float T_T2E(float fT);

float WRE526_E2T(float fE);
float WRE526_T2E(float fT);

float WRE325_E2T(float fE);
float WRE325_T2E(float fT);

float F1_E2T(float fE);
float F1_T2E(float fT);																//没有使用
										   
float F2_E2T(float fE);
float F2_T2E(float fT);	 															//没有使用


#endif
