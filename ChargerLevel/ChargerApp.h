/*
 * File: ChargerApp.h
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

#ifndef ChargerApp_h_
#define ChargerApp_h_
#ifndef ChargerApp_COMMON_INCLUDES_
#define ChargerApp_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#endif                                 /* ChargerApp_COMMON_INCLUDES_ */

#include "Charger.h"
#include "ChargerApp_FCN01ACFaultFlag.h"

/* Definition for custom storage class: Define */
#define ChargerMaxCurrent              200.0F                    /* Referenced by: '<S5>/Constant3' */
#define ChargerMaxVoltage              1000.0F                   /* Referenced by: '<S5>/Constant1' */
#define ChargerMinCurrent              0.0F                      /* Referenced by: '<S5>/Constant2' */
#define ChargerMinPower                0.0F                      /* Referenced by: '<S5>/Constant4' */
#define ChargerMinVoltage              50.0F                     /* Referenced by: '<S5>/Constant' */
#define CurrentImbalanceLimit          10.0F                     /* Referenced by: '<S12>/Constant' */
#define CurrentLowerLimit              -5.0F                     /* Referenced by: '<S12>/Constant2' */
#define CurrentUpperLimit              500.0F                    /* Referenced by: '<S12>/Constant1' */
#define DigitalInputDefault            false                     /* Referenced by: '<S1>/DefaultInput' */

#define IMDLowerLimit                 500000.0f                 /* Referenced by: '<S30>/Constant' */
#define IMDUpperLimit                 2500000.0f                 /* Referenced by: '<S30>/Constant' */
#define LineFreLowerLimit              47.5F                     /* Referenced by: '<S9>/Constant4' */
#define LineFreUpperLimit              52.5F                     /* Referenced by: '<S9>/Constant3' */
#define LinePFLowerLimit               0.0F                      /* Referenced by: '<S15>/Constant4' */
#define LinePFUpperLimit               2.0F                      /* Referenced by: '<S15>/Constant3' */
#define MaxPower                       HalfPower*2.0f                 /* Referenced by:*/


#define StopChargerCurrentLimit                1.0F                      /* Referenced by: '<S69>/Constant' */
#define VoltageImbalanceLimit                  30.0F                     /* Referenced by: '<S14>/VoltageImbalanceLimit' */
#define VoltageFaultClearLowerLimit           175.0F                    /* Referenced by: '<S14>/Constant2' */
#define VoltageFaultTriggerLowerLimit         165.0F
#define VoltageFaultClearUpperLimit           265.0F                    /* Referenced by: '<S14>/Constant1' */
#define VoltageFaultTriggerUpperLimit         275.0F

/* Model entry point functions */
extern void ChargerApp_initialize(void);

//++++++++++++FUNCTIONS RELATED TO DECISION MAKING BASED ON CHARGER
void ModbusFaultcheck(ModbusCommStatus ModbusCommStatusValue, bool *FaultFlag ,  uint8_t *FaultCounter);
void Gunroutin(GunStruct *PrimaryGun, GunStruct *SecGun,DIOStatus_t DIOStatus,ReceiveFromRMLStruct *RNLGun);
void IOT_ProcessCANMsg(uint32_t can_id, uint8_t *data, uint8_t length, GunStruct *Right, GunStruct * Left,ChargerStruct *Charger);
void GunOperativeStatus(uint8_t *EVConnectionState, operationalStatus_t *PECCOperationState);
void ChargerFault(ChargerStruct *ChargerData, GunStruct RightGun, GunStruct LeftGun,uint8_t *ELRFaultCounter, float AC_EnergymeterData, uint8_t ACLNModbusstatus);
void ChargerApp_VoltageLimitFault(float *PhaseVolt,bool *limitfault);
void CANPerfaultcheck(uint8_t *CANAFaultCounter, bool *CANAPerFault,bool Gun);
void GunTempFault(bool DCEMfault, bool IMDfault, GunStruct *gun, uint8_t *IsolationState);
void GunPerFault(GunStruct *gun);
void ChargerTempFault(bool ACFault, ChargerStruct *ChargerData);
void ChargerPerFault(bool RightGunCANfault, bool LeftGunCANfault, ChargerStruct *ChargerData);
void ChargerApp_FCN00StopChrage00(GunStruct *uy_PrimaryGun);
void ChargerApp_FCN02DCGUNFaultCheck(GunStruct *uy_GunData);
void ChargerApp_FCN06CrossContactorVSECCOut(GunStruct *uy_RightGun, GunStruct *uy_LeftGun, ChargerStruct *uy_ChargerGun);
#endif                                 /* ChargerApp_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
