/*
 * File: ChargerApp_FCN01ACFaultFlag.c
 *
 * Code generated for Simulink model 'ChargerApp'.
 *
 * Model version                  : 1.226
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Wed Jun 11 13:30:06 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include <stdbool.h>
#include "ChargerApp_FCN01ACFaultFlag.h"

/* Includes for objects with custom storage classes */
#include "ChargerApp.h"

/* Output and update for Simulink Function: '<S1>/LimitCheck' */
/**
 *  ChargerApp_LimitCheck
 *
 *  @brief It is used to check charger limit check.
 *
 *  This function is used to check charger limit check based on limits   .
 *
 *  @param u_x  tells us charger data frequency
 *  @param u_UL tells us charger data frequency upper limit
 *  @param u_LL tells us charger data frequency lower limit
 *  @param y_Status is the pointer to the status of the limit check
 *
 *  @return None.
 */
void ChargerApp_LimitCheck(double u_x, float u_UL, float u_LL, bool *y_Status)
{
  *y_Status = u_x >= u_LL && u_x <= u_UL;
}

/* Output and update for Simulink Function: '<S1>/PhaseBalanceCheck' */
/**
 *  ChargerApp_PhaseBalanceCheck
 *
 *  @brief It is used to check charger phase balance check.
 *
 *  This function is used to check charger phase balance check based on limits   .
 *
 *  @param u_R  tells us phase value 0
 *  @param u_Y tells us phase value 1
 *  @param u_B tells us phase value 2
 *  @param u_Limit is the phase balance limit
 *  @param y_Status is the pointer to the status of the phase balance check
 *
 *  @return None.
 */
void ChargerApp_PhaseBalanceCheck(float u_R, float u_Y, float u_B, float u_Limit,
  bool *y_Status)
{
  *y_Status = u_R - u_Y < u_Limit && u_Y - u_B < u_Limit && u_R - u_B < u_Limit;
}

/* Output and update for Simulink Function: '<S1>/PhaseLimitCheck' */
/* Output and update for Simulink Function: '<S1>/PhaseBalanceCheck' */
/**
 *  ChargerApp_PhaseBalanceCheck
 *
 *  @brief It is used to check charger phase balance check.
 *
 *  This function is used to check charger phase balance check based on limits   .
 *
 *  @param u_R  tells us phase value 0
 *  @param u_Y tells us phase value 1
 *  @param u_B tells us phase value 2
 *  @param u_UL is the pahse upper limit
 *  @param u_LL is the pahse lower limit
 *  @param y_Status is the pointer to the status of the phase limit check
 *
 *  @return None.
 */
void ChargerApp_PhaseLimitCheck(float u_R, float u_Y, float u_B, float u_UL,
  float u_LL, bool *y_Status)
{
  *y_Status = u_R > u_LL && u_R < u_UL && (u_Y > u_LL && u_Y < u_UL) && (u_B >
    u_LL && u_B < u_UL);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
