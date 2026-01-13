/*
 * File: ChargerApp_FCN01ACFaultFlag.h
 *
 * Code generated for Simulink model 'ChargerApp'.
 *
 * Model version                  : 1.226
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Wed Jun 11 13:30:06 2025
 */

#ifndef ChargerApp_FCN01ACFaultFlag_h_
#define ChargerApp_FCN01ACFaultFlag_h_
#include "Charger.h"

extern void ChargerApp_FCN01ACFaultFlag(ChargerStruct *uy_ChargerData);
void ChargerApp_LimitCheck(double u_x, float u_UL, float u_LL, bool *y_Status);
void ChargerApp_PhaseBalanceCheck(float u_R, float u_Y, float u_B, float u_Limit,bool *y_Status);
void ChargerApp_PhaseLimitCheck(float u_R, float u_Y, float u_B, float u_UL,float u_LL, bool *y_Status);

#endif                                 /* ChargerApp_FCN01ACFaultFlag_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
