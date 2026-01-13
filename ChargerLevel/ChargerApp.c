/*
 * File: ChargerApp.c
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

#include "ChargerApp.h"
#include "Charger.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "ChargerApp_FCN01ACFaultFlag.h"

/**
 *  ChargerApp_FCN01ACFaultFlag
 *
 *  @brief It is used to check the AC fault with charger data values.
 *
 *  This function is used to check the AC fault uisng 'ChargerApp_PhaseBalanceCheck', 'ChargerApp_VoltageLimitFault','ChargerApp_PhaseLimitCheck','ChargerApp_LimitCheck' functions.
 *
 *  @param ChargerData is the value of the pointer to the chargerstruct structure
 *
 *  @return None.
 */
void ChargerApp_FCN01ACFaultFlag(ChargerStruct *ChargerData)
{
  bool Current_Balance_Fault;
  bool Current_Limit_Fault;
  bool Fre_Limit;
  bool Voltage_Balance_Fault;
 // bool Voltage_Limit_Fault;

  ChargerApp_PhaseBalanceCheck(ChargerData->PhaseVoltage[0],
    ChargerData->PhaseVoltage[1], ChargerData->PhaseVoltage[2],
    VoltageImbalanceLimit, &Voltage_Balance_Fault);

  ChargerApp_VoltageLimitFault(ChargerData->PhaseVoltage, &(ChargerData->VoltageLimitFault));

  ChargerApp_PhaseBalanceCheck(ChargerData->PhaseCurrent[0],
    ChargerData->PhaseCurrent[1], ChargerData->PhaseCurrent[2],
    CurrentImbalanceLimit, &Current_Balance_Fault);

  ChargerApp_PhaseLimitCheck(ChargerData->PhaseCurrent[0],
    ChargerData->PhaseCurrent[1], ChargerData->PhaseCurrent[2],
    CurrentUpperLimit, CurrentLowerLimit, &Current_Limit_Fault);

  ChargerApp_LimitCheck((double)ChargerData->frequency, LineFreUpperLimit,
                        LineFreLowerLimit, &Fre_Limit);

  ChargerData->VoltageBalanceFault = !Voltage_Balance_Fault;
  ChargerData->CurrentBalanceFault = !Current_Balance_Fault;
  ChargerData->CurrentLimitFault = !Current_Limit_Fault;
  ChargerData->FrequncyLimitFault = !Fre_Limit;
  ChargerData->PowerFactorLimit = 0;
}
/**
 *  ChargerApp_FCN01ACFaultGlobalFlag
 *
 *  @brief It is used to check the AC fault and set global fault value.
 *
 *  This function is used to check the AC fault and make the global chargerfault value in the chargerData struct.
 *
 *  @param ChargerData is the value of the pointer to the chargerstruct structure
 *
 *  @return None.
 */
void ChargerApp_FCN01ACFaultGlobalFlag(ChargerStruct *ChargerData)
{
  ChargerData->ChargerFault = ChargerData->VoltageBalanceFault ||
    ChargerData->CurrentBalanceFault || ChargerData->VoltageLimitFault ||
    ChargerData->CurrentLimitFault || ChargerData->FrequncyLimitFault ||
    ChargerData->PowerFactorLimit || ChargerData->EMButtonFault ||
    ChargerData->SPDFault || ChargerData->ELRFBFault ||
    ChargerData->ACContactorFault;
}
/**
 *  ChargerApp_FCN06CrossContactorVSECCOut
 *
 *  @brief It is used to check the cross contactor values based on the gun struct values .
 *
 *  This function is used to set the cross contactor values based on the right gun and left gun values based on the positive and negative contactors.
 *
 *  @param ChargerGun is the value of the pointer to the chargerstruct structure
 *  @param RightGun, LeftGun are the pointer to the GunStruct structure form which we get the positive, negative contactors values
 *
 *  @return None.
 */
void ChargerApp_FCN06CrossContactorVSECCOut(GunStruct *RightGun, GunStruct *LeftGun, ChargerStruct *ChargerGun)
{
  if (RightGun->ChargingSpeed == 2) {
      if(LeftGun->PositiveContactorDrive == Off
              && LeftGun->NegativeContactorDrive == Off
              && LeftGun->PositiveContactorFB == Off
              && LeftGun->NegativeContactorFB == Off)
      {
          ChargerGun->CrossContactorDrive = On;
      }
      else
      {
          if(RightGun ->EVConnectionState ==  EV_DISCONNECTED)
              ChargerGun->CrossContactorDrive = Off;
      }

  } else if (LeftGun->ChargingSpeed == 2) {
      if(RightGun->PositiveContactorDrive == Off
              && RightGun->NegativeContactorDrive == Off
              && RightGun->PositiveContactorFB == Off
              && RightGun->NegativeContactorFB == Off)
      {
          ChargerGun->CrossContactorDrive = On;
      }
      else
      {
          ChargerGun->CrossContactorDrive = Off;
      }
  } else {
      ChargerGun->CrossContactorDrive = Off;
    RightGun->PECCOperationState = OPERATIONAL_STATUS_OPERATIVE;
    LeftGun->PECCOperationState = OPERATIONAL_STATUS_OPERATIVE;
  }
}
/**
 *  ChargerApp_FCN02DCGUNFaultCheck
 *
 *  @brief It is used to check the gun fault based on the contactors values .
 *
 *  This function is used to set the gun fault values based on the right gun and left gun values based on the positive and negative contactors.
 *
 *  @param GunData is the pointer to the GunStruct structure form which we get the positive, negative contactors values
 *
 *  @return None.
 */
void ChargerApp_FCN02DCGUNFaultCheck(GunStruct *GunData)
{
  bool IMD_Fault_Inv;
  bool Negtive_Contactor_OP;
  bool Postive_Contactor_OP;

  Postive_Contactor_OP = (int32_t)GunData->PositiveContactorDrive != 0 ^
    (int32_t)GunData->PositiveContactorFB != 0;

  Negtive_Contactor_OP = (int32_t)GunData->NegativeContactorFB != 0 ^
    (int32_t)GunData->NegativeContactorDrive != 0;

  IMD_Fault_Inv = (int32_t)GunData->IMDFault == 0;

  GunData->IMDResFault =0;// GunData->InsulationResistance <= IMDLowerLimit;

  GunData->GunFault = Postive_Contactor_OP || Negtive_Contactor_OP ||
    IMD_Fault_Inv || GunData->IMDResFault;
 // GunData->GunFault = 0;
  if(Postive_Contactor_OP == 1)
      GunData->PositiveContactorFault = 0;//1;

  if(Negtive_Contactor_OP == 1)
  GunData->NegativeContactorFault =0;// 1;

  GunData->GunIMDFault =0;// IMD_Fault_Inv;
}
/**
 *  ChargerApp_FCN04PowerAllocation
 *
 *  @brief It is used to set the power based on the charging speed .
 *
 *  This function is used to set the power values based on the primary gun and secondary gun charging speed values.
 *
 *  @param PrimaryGun, SecondaryGun are the pointer to the GunStruct structure form which we get the charging speed values
 *
 *  @return None.
 */
void ChargerApp_FCN04PowerAllocation(GunStruct *PrimaryGun, GunStruct *SecondaryGun)
{
    if(PrimaryGun->EVMaxPower == 0)
    {
        PrimaryGun->EVMaxPower = PrimaryGun->EVMaxVoltage*PrimaryGun->EVMaxCurrent*0.75f;
    }
        switch(SecondaryGun->ChargingSpeed)
        {
        case NotCharging:
            if(PrimaryGun->EVMaxPower >  HalfPower )
            {
                if(SecondaryGun->EVConnectionState == EV_DISCONNECTED)
                {
                    PrimaryGun->VseccMaxPower= MaxPower;
                }
                else
                {
                    PrimaryGun->VseccMaxPower= HalfPower;
                    PrimaryGun->PowerReallocFlag =  1;
                }
                if(SecondaryGun->TotalMeasuredCurrent < 2  && PrimaryGun->PowerReallocFlag == 1)
                    PrimaryGun->ChargingSpeed =  SlowChargeing;
                else
                    PrimaryGun->ChargingSpeed =  FastCharging;
            }
            else
            {
                PrimaryGun->VseccMaxPower = HalfPower;
                PrimaryGun->ChargingSpeed =  SlowChargeing;
            }
            break;
        case SlowChargeing:
            PrimaryGun->VseccMaxPower= HalfPower;
            PrimaryGun->ChargingSpeed =  SlowChargeing;
            SecondaryGun->PECCOperationState = OPERATIONAL_STATUS_OPERATIVE;
            break;
        case FastCharging:
            PrimaryGun->VseccMaxPower = 0;
            PrimaryGun->ChargingSpeed =  NotCharging;
           break;
        }

}
/**
 *  ChargerApp_FCN05GunState05
 *
 *  @brief It is used to set the values of the gun struct based on the gun connection state .
 *
 *  This function is used to set gun struct power values and other things related to the gun struct based on the gun connection state.
 *
 *  @param PrimaryGun, SecondaryGun are the pointer to the GunStruct structure form which we get the charging speed values and gun connection state
 *
 *  @return None.
 */
void ChargerApp_FCN05GunState05(GunStruct *PrimaryGun,GunStruct *SecondaryGun)// IMD Resistance invalid removed as we are checking it all the time
{
  int32_t tmp;
  switch ((int32_t)PrimaryGun->EVConnectionState) {
   case EV_DISCONNECTED:
       PrimaryGun-> CodeGunStatus = 00;
       PrimaryGun->IMDEnable = 0;
       PrimaryGun->IMDCount = 0;
       PrimaryGun->IsolationState = ISOLATION_INVALID;
       PrimaryGun->CableCheckFalg = 0;
       PrimaryGun->IOTstopCharge = 0;
       PrimaryGun->TargetContactorState = 0;
       PrimaryGun->CableCheckVoltage = 0;
       PrimaryGun->Battery_State_Of_Charge = 0;

        PrimaryGun->RectState = Off;
        PrimaryGun->ContactorState = CONTACTOR_OPEN;
        PrimaryGun->RectDriveVoltage = 0.0F;
        PrimaryGun->RectDriveCurrent = 0.0F;
        PrimaryGun->PositiveContactorDrive = Off;
        PrimaryGun->NegativeContactorDrive = Off;
        PrimaryGun->PrechargeContactorDrive = Off;
        PrimaryGun->StopChargeLED = false;
        PrimaryGun->StopCharge = false;
        PrimaryGun->DiffEnergy = 0;
    break;

   case EV_CONNECTED:
        PrimaryGun-> CodeGunStatus = 10;
        PrimaryGun->IsolationState = ISOLATION_INVALID;
        PrimaryGun->RectState = Off;
        PrimaryGun->ContactorState = CONTACTOR_OPEN;
        PrimaryGun->RectDriveVoltage = 0.0F;
        PrimaryGun->RectDriveCurrent = 0.0F;
        PrimaryGun->PositiveContactorDrive = Off;
        PrimaryGun->NegativeContactorDrive = Off;
        PrimaryGun->PrechargeContactorDrive = Off;
    break;

   case EV_ENERGY_TRANSFER_ALLOWED:   // Energy Transfer Case
       PrimaryGun-> CodeGunStatus = 20;
       //PrimaryGun->DiffEnergy = 0;
        if( PrimaryGun->IMDResFault == 0  && PrimaryGun->InsulationResistance <= IMDUpperLimit &&  PrimaryGun->InsulationResistance > IMDLowerLimit)
        {
           PrimaryGun->IsolationState = ISOLATION_VALID;
        }
        else
        {
           PrimaryGun->IsolationState = ISOLATION_INVALID;
        }
        if (SecondaryGun->GunFault == 0)
        {
            ChargerApp_FCN04PowerAllocation(PrimaryGun,SecondaryGun);
        }
        else
        {
            PrimaryGun->VseccMaxPower = (float)HalfPower;
            PrimaryGun->ChargingSpeed = (ChargingTypeEnum)SlowChargeing;
        }
        if(SecondaryGun->EVConnectionState == EV_CONNECTED && PrimaryGun->ChargingSpeed == FastCharging)
        {
            PrimaryGun->PowerReallocFlag = 1;    //make 0 in disconnected
        }
    switch ((int32_t)PrimaryGun->EVChargingState) {
     case CHARGING_STANDBY:
      PrimaryGun-> CodeGunStatus = 20;
      PrimaryGun->IsolationState = ISOLATION_INVALID;
      PrimaryGun->RectState = Off;
      PrimaryGun->ContactorState = CONTACTOR_OPEN;
      PrimaryGun->RectDriveVoltage = 0.0F;
      PrimaryGun->RectDriveCurrent = 0.0F;
      PrimaryGun->PositiveContactorDrive = Off;
      PrimaryGun->NegativeContactorDrive = Off;
      PrimaryGun->PrechargeContactorDrive = Off;
      PrimaryGun->IMDEnable = 0;
      break;

     case CHARGING_CABLE_CHECK:
          PrimaryGun->DiffEnergy = 0;//Cable Check
          if(PrimaryGun->IMDEnable ==0)
          {
              PrimaryGun->IMDCount++;
              PrimaryGun->IsolationState = ISOLATION_INVALID;
          }
          if( PrimaryGun->IMDCount > 60)  // Based on testing
          {
              PrimaryGun->IMDCount=61;
              PrimaryGun->IMDEnable =1;
              if(PrimaryGun->InsulationResistance <= IMDUpperLimit &&  PrimaryGun->InsulationResistance > IMDLowerLimit)
                  {
                      PrimaryGun->IsolationState = ISOLATION_VALID;
                  }
              else if(PrimaryGun->InsulationResistance < IMDLowerLimit && PrimaryGun->InsulationResistance > 400000)
                  {
                      PrimaryGun->IsolationState= ISOLATION_WARNING;
                  }
              else if(PrimaryGun->InsulationResistance < 400000)
                  {
                      PrimaryGun->IsolationState= ISOLATION_FAULT;
                  }
              else
                  {
                      PrimaryGun->IsolationState= ISOLATION_INVALID;
                  }
          }
          PrimaryGun->ContactorState = CONTACTOR_OPEN;
          if(PrimaryGun->IsolationState == ISOLATION_VALID)
          {
              PrimaryGun-> CodeGunStatus = 212;
              PrimaryGun->RectState = Off;
              PrimaryGun->RectDriveVoltage = 0;
              PrimaryGun->NegativeContactorDrive = Off;
              PrimaryGun->PrechargeContactorDrive = Off;
          }
          else if( PrimaryGun->IMDEnable == 0)
          {
              PrimaryGun-> CodeGunStatus = 211;
              PrimaryGun->RectState = On;
              PrimaryGun->RectDriveVoltage = PrimaryGun->CableCheckVoltage;
              PrimaryGun->NegativeContactorDrive = On;
              PrimaryGun->PrechargeContactorDrive = On;
          }
          PrimaryGun->RectDriveCurrent = 0.0F;
          PrimaryGun->PositiveContactorDrive = Off;
       break;
      case CHARGING_PRECHARGE:
          if (PrimaryGun->TargetContactorState)
          {
              PrimaryGun-> CodeGunStatus = 221;
              PrimaryGun->DiffEnergy = 0;
              PrimaryGun->RectState = On;
              PrimaryGun->ContactorState = CONTACTOR_OPEN;
              PrimaryGun->RectDriveVoltage = PrimaryGun->TargetVoltage;
              if (PrimaryGun->ChargingSpeed == FastCharging)
              {
                  tmp = 2;
              }
              else
              {
                  tmp = 1;
              }
            PrimaryGun->DCEMStartEnergy = PrimaryGun->DCEMEnergyReading;
            PrimaryGun->RectDriveCurrent = PrimaryGun->TargetCurrent /(float)((double)tmp * RectifierPerGun);
            PrimaryGun->PositiveContactorDrive = Off;
            PrimaryGun->NegativeContactorDrive = On;
            PrimaryGun->PrechargeContactorDrive = On;
          }
          else
          {
              PrimaryGun-> CodeGunStatus = 222;
              PrimaryGun->RectState = Off;
              PrimaryGun->ContactorState = CONTACTOR_OPEN;
              PrimaryGun->RectDriveVoltage = 0.0F;
              PrimaryGun->RectDriveCurrent = 0.0F;
              PrimaryGun->PositiveContactorDrive = Off;
              PrimaryGun->NegativeContactorDrive = Off;
              PrimaryGun->PrechargeContactorDrive = Off;
          }
          break;
      case CHARGING_CHARGE:
          PrimaryGun-> CodeGunStatus = 23;
          PrimaryGun->CableCheckFalg = 0;
          PrimaryGun->IOTstartCharge = 0;
          PrimaryGun->RectState = On;
          PrimaryGun->ContactorState = CONTACTOR_CLOSED;
          PrimaryGun->RectDriveVoltage = PrimaryGun->TargetVoltage;
          PrimaryGun->EVTargetPower =   PrimaryGun->TargetVoltage * PrimaryGun->TargetCurrent;
          if (PrimaryGun->ChargingSpeed == FastCharging)
           {
               //PrimaryGun->DiffEnergy = (PrimaryGun->DCEMEnergyReading + SecondaryGun->DCEMEnergyReading ) - (PrimaryGun->DCEMStartEnergy + SecondaryGun->DCEMStartEnergy);
               PrimaryGun->DiffEnergy = PrimaryGun->DiffEnergy + PrimaryGun->DCEMDeltaValue + SecondaryGun->DCEMDeltaValue;
               PrimaryGun->DCEMDeltaValue  = 0;
               SecondaryGun->DCEMDeltaValue = 0;
               tmp = 2;
           }
           else
           {
                 //PrimaryGun->DiffEnergy = (PrimaryGun->DCEMEnergyReading) - PrimaryGun->DCEMStartEnergy;
                 PrimaryGun->DiffEnergy = PrimaryGun->DiffEnergy + PrimaryGun->DCEMDeltaValue;
                 PrimaryGun->DCEMDeltaValue = 0;
                 tmp = 1;
           }
          PrimaryGun->RectDriveCurrent = PrimaryGun->TargetCurrent /(float)((double)tmp * RectifierPerGun);
          PrimaryGun->PositiveContactorDrive = On;
          PrimaryGun->NegativeContactorDrive = On;
          PrimaryGun->PrechargeContactorDrive = Off;
          break;
      case CHARGING_POSTCHARGE:
          PrimaryGun-> CodeGunStatus = 24;
          ChargerApp_FCN00StopChrage00(PrimaryGun);
          if(PrimaryGun->EVConnectionState ==  EV_DISCONNECTED)
          {
              PrimaryGun->StopCharge = 0;
              PrimaryGun->StartCharge = 0;
              PrimaryGun->CableCheckFalg = 0;
          }
          PrimaryGun->IMDEnable =0;
          break;
    }
    break;

   default:  //EV_ERROR
       PrimaryGun-> CodeGunStatus = 30;
       PrimaryGun->DiffEnergy= 0;
    ChargerApp_FCN00StopChrage00(PrimaryGun);
    if(PrimaryGun->EVConnectionState ==  EV_DISCONNECTED)
     {
         PrimaryGun->StopCharge = 0;
         PrimaryGun->StartCharge = 0;
     }
     PrimaryGun->IMDEnable =0;
     PrimaryGun->CableCheckFalg = 0;
    break;
  }
}
/**
 *  ChargerApp_FCN00StopChrage00
 *
 *  @brief It is used to set the stop charge values of the gun struct.
 *
 *  This function is used to set gun stop charge value based on the gun struct value.
 *
 *  @param PrimaryGun is the pointer to the GunStruct structure from which we get the gun struct parameters
 *
 *  @return None.
 */
void ChargerApp_FCN00StopChrage00(GunStruct *PrimaryGun)// IMD Resistance invalid removed as we are checking it all the time
{
  int32_t tmp;
  if (PrimaryGun->ChargingSpeed == FastCharging) {
      tmp = 2;
    } else {
      tmp = 1;
    }
  if (PrimaryGun->TotalMeasuredCurrent >= StopChargerCurrentLimit || PrimaryGun->TargetCurrent >= StopChargerCurrentLimit)
  {
      PrimaryGun->CodeGunStatus = 40;
      PrimaryGun->ContactorState = CONTACTOR_CLOSED;
      PrimaryGun->RectState = On;
      PrimaryGun->StopChargeLED = On;
      PrimaryGun->RectDriveVoltage = PrimaryGun->TargetVoltage;
      PrimaryGun->RectDriveCurrent = PrimaryGun->TargetCurrent / (float)((double)tmp * RectifierPerGun);
      PrimaryGun->PositiveContactorDrive = On;
      PrimaryGun->NegativeContactorDrive = On;
  }
  else
  {
      PrimaryGun->CodeGunStatus = 41;
      PrimaryGun->StopChargeLED = On;
      PrimaryGun->ContactorState = CONTACTOR_OPEN;
      PrimaryGun->RectState = Off;
      PrimaryGun->RectDriveVoltage =0;
      if(PrimaryGun->EVChargingState == CHARGING_POSTCHARGE)
      {
       PrimaryGun->StopCharge = 0;
      }
      PrimaryGun->IMDEnable = 0;
      PrimaryGun->IMDCount = 0;
      PrimaryGun->RectDriveCurrent = 0;
      PrimaryGun->PositiveContactorDrive = Off;
      PrimaryGun->NegativeContactorDrive = Off;
  }
  PrimaryGun->PrechargeContactorDrive = Off;
  if(PrimaryGun->EVConnectionState  == EV_DISCONNECTED)
  {
      PrimaryGun->CodeGunStatus = 00;
      PrimaryGun->ContactorState = CONTACTOR_OPEN;
      PrimaryGun->StopChargeLED = Off;
      PrimaryGun->IMDEnable = 0;
      PrimaryGun->StopCharge = 0;
      PrimaryGun->IOTstopCharge = 0;
      PrimaryGun->DiffEnergy = 0;
      PrimaryGun->DCEMStartEnergy =  PrimaryGun->DCEMEnergyReading;
      PrimaryGun->ChargingSpeed = NotCharging;
  }
  else
  {
      PrimaryGun->CodeGunStatus = 255;
  }
}
/**
 *  IOT_ProcessCANMsg
 *
 *  @brief It is used to process the values form the IOT messages.
 *
 *  This function is used to process the IOT messages form the CAN and set values for the GunStruct and ChargerStruct parameters.
 *
 *  @param can_id is the can id from the IOT
 *  @param data is the pointer to the data from the received can id
 *  @param length is the length of the data
 *  @param Charger is the pointer to the struct for ChargerStruct structure
 *  @param Right,Left are the pointers to the GunStruct structure from which we get the gun struct parameters
 *
 *  @return None.
 */
void IOT_ProcessCANMsg(uint32_t can_id, uint8_t *data, uint8_t length, GunStruct *Right, GunStruct * Left,ChargerStruct *Charger)
{
    bool CanLeftGunFaultMSG,CanRightGunFaultMSG,CanChargerFaultMSG;
    if(can_id == CAN_CMSTX_IOTGUNCTRL)
    {
        Right->IOTstopCharge = (bool)(data[0] & 0x01);
        Right->IOTstartCharge =  (bool) ((data[0] & 0x02) >>1);
        Left->IOTstopCharge = (bool)((data[0] & 0x04)>>2);
        Left->IOTstartCharge = (bool)((data[0] & 0x08)>>3);
       Charger->IoTResetRequest =  ((data[0] & 0x30) == 0x30);
    }

    if(can_id == CAN_CMSRX_FAULTCLEARANCE)
    {
        CanRightGunFaultMSG = data[0] & 0x01;
        if(CanRightGunFaultMSG == 1)
            Right->ParmanantFault =0;

        CanLeftGunFaultMSG = data[0] & 0x02;
        if(CanLeftGunFaultMSG == 1)
            Left->ParmanantFault =0;

        CanChargerFaultMSG = data[0] & 0x04;
        if(CanChargerFaultMSG == 1)
            Charger->ParmanantFault =0;
            Charger->ELRFBFault =0;
            Charger->EMButtonFault =0;
            Charger->ACContactorFault = 0;
            Charger->SPDFault =0;
            Charger->CrossContactorFault = 0;
            Charger ->CrossContactorWeldFault = 0;
            Charger->CMSDIOInFault =0;
            Right->CANAPerFault  = 0 ;
            Left->CANAPerFault = 0;
       // SOFT RESET FROM IOT
      // CanChargerSoftResetIOT = (data[0] & 0x07);
       if(data[0] == 0x07)
        {
           SysCtl_setWatchdogMode(SYSCTL_WD_MODE_RESET);
           SysCtl_resetDevice();
        }

    }
}
/**
 *  ModbusFaultcheck
 *
 *  @brief It is used to check the modbus fault using counter.
 *
 *  This function is used to chekc the fault in modbus using polling method by checking the ACEM_COMM_FAULT_COUNT with FaultCounter.
 *
 *  @param ModbusCommStatusValue is the status of the modbus
 *  @param FaultFlag is the pointer to the fault flag raised
 *  @param FaultCounter is the pointer to the fault counter that increments when modbus fault occurs
 *
 *  @return None.
 */
void ModbusFaultcheck(ModbusCommStatus ModbusCommStatusValue, bool *FaultFlag ,  uint8_t *FaultCounter)
{
    if(ModbusCommStatusValue != MODBUS_STATUS_OK)
    {
        *FaultCounter++;
        if(*FaultCounter >= ACEM_COMM_FAULT_COUNT)
        {
            *FaultCounter = *FaultCounter + 1;
            *FaultFlag =1;
        }
    }
    else
    {
        *FaultFlag =0;
        *FaultCounter =0;
    }
}
/**
 *  Gunroutin
 *
 *  @brief It is used to set the gun struct values based on the connection,IOT and charging state .
 *
 *  This function is used to process the EVConnectionState,IOT messages and ChargingSpeed and set the values on the GunStruct  .
 *
 *  @param DIOStatus is the status of the DIO whether it is success or error for GunStruct parameters
 *  @param PrimaryGun,SecGun are the pointers to the GunStruct structure from which we get the gun struct parameters
 *
 *  @return None.
 */
void Gunroutin(GunStruct *PrimaryGun, GunStruct *SecGun,DIOStatus_t DIOStatus,ReceiveFromRMLStruct *RNLGun)
{
    /*PrimaryGun->GunFault =0 ;
    PrimaryGun->ParmanantFault = 0;
    PrimaryGun->TempFault = 0;*/
    if(SecGun->ChargingSpeed !=  FastCharging)
    {
        if(PrimaryGun->GunFault ==0 && PrimaryGun->ParmanantFault == 0 && PrimaryGun->TempFault == 0)
        {
            PrimaryGun->GunStateFault = 0;
            if((uint16_t)PrimaryGun->EVConnectionState  >= 4  || (uint16_t)PrimaryGun->EVChargingState >= 5)
            {
                PrimaryGun->EVConnectionState = EV_DISCONNECTED;
            }
            if(SecGun->GunFault == 1)
            {
                PrimaryGun->ChargingSpeed = SlowChargeing;
                PrimaryGun->VseccMaxPower = HalfPower;
            }
            else if(SecGun->ChargingSpeed ==  NotCharging)
            {
                PrimaryGun->VseccMaxPower = MaxPower;
            }
            else if(SecGun->ChargingSpeed ==  SlowChargeing)
            {
                PrimaryGun->ChargingSpeed = SlowChargeing;
                PrimaryGun->VseccMaxPower = HalfPower;
            }
            else if(SecGun->ChargingSpeed == FastCharging)
            {
                PrimaryGun->ChargingSpeed = NotCharging;
                PrimaryGun->VseccMaxPower = 0;
             }
            if(PrimaryGun->EVConnectionState ==  EV_DISCONNECTED)
            {
                PrimaryGun->CodeGunStatus = 0;
                PrimaryGun->ChargingSpeed = NotCharging;
                PrimaryGun->RectState = Off;
                PrimaryGun->IsolationState =  ISOLATION_INVALID;
                PrimaryGun->PrechargeContactorDrive = Off;
                PrimaryGun->PositiveContactorDrive = Off;
                PrimaryGun->NegativeContactorDrive = Off;
                PrimaryGun->ContactorState = CONTACTOR_OPEN;
                PrimaryGun->DCEMStartEnergy = PrimaryGun->DCEMEnergyReading;
                PrimaryGun->GunStateFault = 0;
                PrimaryGun->IMDEnable = 0;
                PrimaryGun->IMDCount = 0;
                PrimaryGun->VehicleID = 0;
                RNLGun->BasicResponse.BasicResponseSignal.CR_Plc_EVCC_MAC = 0;
                PrimaryGun->DiffEnergy = 0;
                PrimaryGun->ResetRquest = 0;
                PrimaryGun->StartCharge = 0;
                PrimaryGun->StopCharge = 0;
                PrimaryGun->IOTstartCharge = 0;
                PrimaryGun->IOTstopCharge = 0;
                PrimaryGun->StopChargeLED = 0;
                PrimaryGun->StopChargeUserButton = 1;
                PrimaryGun->StopChargeUserCount = 0;
                PrimaryGun->CurrentChangeCounter = 0;
                PrimaryGun->PowerReallocFlag = 0;
                PrimaryGun->EVMaxVoltage = 0;
                PrimaryGun->EVMaxPower = 0;
                PrimaryGun->EVMaxCurrent = 0;
                SecGun->PowerReallocFlag = 0;
                PrimaryGun->TargetContactorState = 0;
                PrimaryGun->VehicleReset = 0;
                PrimaryGun->CableCheckVoltage = 0;
                PrimaryGun->Battery_State_Of_Charge = 0;
                PrimaryGun->TimeToFullSoC = 0;

            }
            else
            {
          //IOT base Changess to Start and StopCharge
              if(PrimaryGun->IOTstartCharge  == 0  && PrimaryGun->IOTstopCharge == 0 && PrimaryGun->StartCharge == 0 )
                {
                  //  PrimaryGun->StartCharge = 0;
                   // PrimaryGun->RectState =  Off;
                }
                else if(PrimaryGun->IOTstartCharge == 1 && PrimaryGun->IOTstopCharge == 0)
                {
                    PrimaryGun->StartCharge = 1;
                    PrimaryGun->StopCharge = 0;   // Reduandant
                }

                if(PrimaryGun->IOTstopCharge == 1 || PrimaryGun->ResetRquest == 1  ||
                  ( PrimaryGun->StopChargeUserButton == 0 && DIOStatus == DIOSuccess)||
                  ( RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.ChargerSuspendFlag == 1)||
                  (RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_InstationFault == 1)||
                  (RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_ConnectorHighTemp ==1 )||
                  (RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_BmsOverTemp == 1)||
                  (RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_ConnectorFault == 1)||
                  (RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_BatOverTemp == 1)||
                  (RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_HighVoltRelay == 1)||
                  (RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_OtherFault == 1)||
                  (RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_ErrCheckPoint2 == 1)||
                  (RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_CurrOver == 1))
                {
                    PrimaryGun->StopCharge = 1;
                    PrimaryGun->IOTstartCharge = 0;
                    PrimaryGun->StartCharge = 0;
                    PrimaryGun->ResetRquest = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.ChargerSuspendFlag = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_InstationFault = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_ConnectorHighTemp = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_BmsOverTemp = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_ConnectorFault = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_BatOverTemp = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_HighVoltRelay = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_ErrCheckPoint2 = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_OtherFault = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_CurrOver = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_AbnormalVolt = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CR_Plc_ErrorCode = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.CR_Plc_ErrorCodeII = 0;
                    RNLGun->BatteryStopTrigger.BatteryStopTriggerSignal.Error_EV = 0;
                    PrimaryGun->StopChargeUserCount = 0;
                }

               if(PrimaryGun->StartCharge == 0 && PrimaryGun->StopCharge == 0)
                {
                    PrimaryGun->StopChargeLED = Off;
                    ChargerApp_FCN05GunState05(PrimaryGun,SecGun);
                }
                else if(PrimaryGun->StopCharge ==  1 )
                {
                    ChargerApp_FCN00StopChrage00(PrimaryGun);
                }
            }
        }
        else
        {
            PrimaryGun->VseccMaxPower =  0;
            if(PrimaryGun->EVConnectionState != EV_ERROR)
                ChargerApp_FCN00StopChrage00(PrimaryGun);
        }
    }
}
/**
 *  GunOperativeStatus
 *
 *  @brief It is used to check the operational status.
 *
 *  This function is used to check the operational status based on ev connection state.
 *
 *  @param EVConnectionState is the pointer to the status of the ev connection in GunStruct
 *  @param PECCOperationState is the pointer to tnhe operational status of the GunStruct
 *
 *  @return None.
 */
void GunOperativeStatus(uint8_t *EVConnectionState, operationalStatus_t *PECCOperationState)
{
    if(*EVConnectionState == EV_DISCONNECTED)
    {
        *PECCOperationState =  OPERATIONAL_STATUS_INOPERATIVE;
    }
}
/**
 *  ChargerFault
 *
 *  @brief It is used to check the faults on the charger.
 *
 *  This function is used to check the faults on the charger using GunStruct, ELR, and AC meter data.
 *
 *  @param ChargerData is the pointer to the status of the ChargerStruct
 *  @param RightGun,LeftGun are the pointers to the GunStruct structure from which we get the gun struct parameters
 *  @param ELRFaultCounter is the pointer to the ELR counter for fault
 *  @param AC_EnergymeterData tells us the current value of the ac energy meter for voltage limit check
 *  @param ACLNModbusstatus tells us the status of the ac meter modbus status
 *
 *  @return None.
 */
void ChargerFault(ChargerStruct *ChargerData, GunStruct RightGun, GunStruct LeftGun,
                  uint8_t *ELRFaultCounter, float AC_EnergymeterData, uint8_t ACLNModbusstatus)
{
    //Emergency fAULT
    switch(ChargerData->EmergencyButton)
    {
    case Emergency:
        ChargerData->EMButtonFault = 1;
        break;
    case NoEmergency:
        ChargerData->EMButtonFault = 0;
    }

    //AC contactor fault
    if((ChargerData->ACContactorFB ^ ChargerData->ACContactorCoilFB) == 1)  // 0 changed to 1
    {

        ChargerData->ACContactorFault = 0;//1;
    }

    //SPDF fault
   if(ChargerData->SPDFB ==1)
   {
       ChargerData->SPDFault =0;//1;
   }

   if(ChargerData->ELRFB == 0 && AC_EnergymeterData > VoltageFaultClearLowerLimit  && ACLNModbusstatus == MODBUS_STATUS_OK)
   {
       (*ELRFaultCounter)++;
       if(*ELRFaultCounter >= 20)
       {
       ChargerData->ELRFBFault =0;//1;
       *ELRFaultCounter = 0;
       }
   }
   else
   {
       *ELRFaultCounter=0;
   }

   if((RightGun.PositiveContactorFB ==1  || RightGun.NegativeContactorFB ==1) &&
           (LeftGun.PositiveContactorFB ==1 || LeftGun.NegativeContactorFB ==1))
   {
       if(ChargerData->CrossContactorDrive == 1 || ChargerData->CrossContactorNegativeFB ==1 || ChargerData->CrossContactorPositiveFB ==1)
       {
           ChargerData->CrossContactorFault = 0;//1;
           ChargerData->CrossContactorDrive = (ContactorWrite)0;
       }
   }

   //crosscontactor weld fault
   if(ChargerData->CrossContactorDrive != ChargerData->CrossContactorPositiveFB || ChargerData->CrossContactorDrive != ChargerData->CrossContactorNegativeFB)
   {
       ChargerData->CrossContactorWeldFault = 0;//1;
       ChargerData->CrossContactorDrive = (ContactorWrite)0;
   }
}
/**
 *  ChargerApp_VoltageLimitFault
 *
 *  @brief It is used to check the voltage limit check faults on the charger.
 *
 *  This function is used to check fault in the charger using phase voltage and limit check .
 *
 *  @param PhaseVolt is the pointer that tells us phasevolt
 *  @param limitfault is the pointer to the fault limit caused by the voltage limit.
 *
 *  @return None.
 */
void ChargerApp_VoltageLimitFault(float *PhaseVolt,bool *limitfault)
{
    uint8_t i = 0;
    for (i = 0;i<=2;i++)
    {
        if((PhaseVolt[i] < VoltageFaultTriggerLowerLimit || PhaseVolt[i] > VoltageFaultTriggerUpperLimit))
        {
            *limitfault = 1;
        }
    }
    i=0;

        if((PhaseVolt[0] > VoltageFaultClearLowerLimit  && PhaseVolt[0] < VoltageFaultClearUpperLimit) &&
                (PhaseVolt[1] > VoltageFaultClearLowerLimit  && PhaseVolt[1] < VoltageFaultClearUpperLimit) &&
                (PhaseVolt[2] > VoltageFaultClearLowerLimit  && PhaseVolt[2] < VoltageFaultClearUpperLimit))
        {
            *limitfault = 0;
        }
}
/**
 *  CANPerfaultcheck
 *
 *  @brief It is used to check the can fault.
 *
 *  This function is used to check can fault based on can fault counter .
 *
 *  @param CANFaultCounter is the pointer to the counter for fault
 *  @param CANPerFault is the pointer to the can permanent fault.
 *
 *  @return None.
 */

void CANPerfaultcheck(uint8_t *CANFaultCounter, bool *CANPerFault,bool Gun)
{
    uint32_t base;
    if(Gun == Gun1)
    {
        base = RECTIFIER_GUN1;
    }
    else if(Gun == Gun2)
    {
        base = RECTIFIER_GUN2;
    }
    if(CAN_getStatus(base) & 0x80 == 0x80)
    {
        *CANFaultCounter++;
        if(*CANFaultCounter >= 4)
        {
            *CANFaultCounter = 0;
            *CANPerFault = 1;
        }
    }
    else
    {
        *CANFaultCounter = 0;
    }
}
/**
 *  GunPerFault
 *
 *  @brief It is used to check permanent fault for gun.
 *
 *  This function is used to check permannent fault for gun based on positive and negative contactors  .
 *
 *  @param gun is the pointer to the gun struct.
 *
 *  @return None.
 */
void GunPerFault(GunStruct *gun)
{
    if (gun->NegativeContactorFault == 1 || gun->PositiveContactorFault == 1)
   {
        gun->ParmanantFault =  1;
   }
}
/**
 *  GunTempFault
 *
 *  @brief It is used to check temporary fault for gun.
 *
 *  This function is used to check temporary fault based on IMD,DCEM and isolation state  .
 *
 *  @param gun is the pointer to the gun struct.
 *  @param DCEMfault is the bool that tells us the DC energy meter fault
 *  @param IMDfault  is the bool that tells us the IMD fault
 *  @param IsolationState is the pointer to the isolation state from the gun struct
 *
 *  @return None.
 */
void GunTempFault(bool DCEMfault, bool IMDfault, GunStruct *gun, uint8_t *IsolationState)
{
    if( gun->IMDResFault ==1 || DCEMfault==1  ||  IMDfault==1)
    {
        gun->TempFault = 1;
    }
    else
    {
        gun->TempFault = 0;
    }

    switch(gun->IMDResFault)
    {
        case 0:
            *IsolationState = ISOLATION_VALID;
            break;
        case 1:
            *IsolationState = ISOLATION_INVALID;
            break;
     }
}
/**
 *  ChargerPerFault
 *
 *  @brief It is used to check permanent fault for charger.
 *
 *  This function is used to check permanent fault based on ChargerData  .
 *
 *  @param LeftGunCANfault, RightGunCANfault  are the bool that tells us the gun can fault
 *  @param ChargerData is the pointer to the ChargerStruct
 *
 *  @return None.
 */
void ChargerPerFault(bool RightGunCANfault, bool LeftGunCANfault, ChargerStruct *ChargerData)
{
    if(((ChargerData->ELRFBFault ==1 || ChargerData->SPDFault ==1 || ChargerData->ACContactorFault ==1 || ChargerData->CMSDIOInFault ==1) && ChargerData->VoltageLimitFault == 0)
            || ChargerData->CrossContactorFault ==1 || ChargerData->CrossContactorWeldFault ==1 ||RightGunCANfault  == 1 ||  LeftGunCANfault == 1)
    {
        ChargerData->ParmanantFault =1;
    }
}
/**
 *  ChargerTempFault
 *
 *  @brief It is used to check temporary fault for charger.
 *
 *  This function is used to check temporary fault based on ChargerData  .
 *
 *  @param ACFault  is the bool that tells us the AC fault
 *  @param ChargerData is the pointer to the ChargerStruct
 *
 *  @return None.
 */
void ChargerTempFault(bool ACFault, ChargerStruct *ChargerData)
{
    if(ChargerData->VoltageBalanceFault==1 || ChargerData->VoltageLimitFault==1
            || ChargerData->CurrentBalanceFault==1 || ChargerData->CurrentLimitFault==1
             || ChargerData->FrequncyLimitFault==1 || ACFault==1 || ChargerData->EMButtonFault == 1)
    {
        ChargerData->TempFaultCounter++;
        if(ChargerData->TempFaultCounter > 5)
        {
            ChargerData->TempFaultCounter = 6;
            ChargerData->TempFault =1;
        }
    }
    else
    {
        ChargerData->TempFaultCounter = 0;
        ChargerData->TempFault =0;
    }
}
/*
 * File trailer for generated code.
 *
 * [EOF]
 */
