/*
 * Charger.c
 *
 *  Created on: Mar 3, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */
#include "Charger.h"
#include "ChargerApp.h"
#include "RNL.h"
#include"string.h"
#include "CMSCAN.h"

//VARIABLE STRUCT RNL
RNLGeneralStruct RNL_Var;
extern uint8_t BCL_Received_Flag;



/**
*  @brief Stores EV data received from  RNL SECC.
*
*  This function stores the EV-related data into the `GunStruct` structure, which is present inside the
*  `ReceiveFromRNLStruct` structure received from the RNL SECC.
*
*  @param GunNumber Pointer to the `GunStruct` structure containing all measured values like
*         TargetContactorState, EVConnectionState, and EVChargingState, etc.
*  @param rnl Pointer to the `ReceiveFromRMLStruct` structure which is received from RNL SECC
*  @param RNLCTRL Pointer to the enum which decides which msg should we send
*  @param rnlSend POinter to the SendToRNLStruct Structure where which parameter form the structure should we send in CAN form the structure
*  @param FLAG Pointer to the flag enum which condition should we send the msg to the RNL via CAN
*
*
*  @return None
*/
int8_t GetEVDataRNL(GunStruct *GunNumber,ReceiveFromRMLStruct *rnl,RNLMSGCTRLEnum *RNLCTRL,SendToRNLStruct *rnlSend,RNLFlagEnum *FLAG) // sending all msg based on flow chart
{

    static  uint64_t startTimeBRM,startTimeBCP;
    static uint8_t BRMStartTimeFlag, BCPStartTimeFlag;

    switch (*FLAG)
    {
        case BHMFlag:
        {
            if(rnl->BasicHSData.BasicHSDataSignal.CR_Plc_CpStatus == 0)
            {
                *RNLCTRL=SendCHM;
                rnlSend->ChargeHandshake.ChargeHandshakeMessageSignal.StartPLC = 0x0;
                GunNumber->EVConnectionState =  EV_DISCONNECTED;
                BRMStartTimeFlag = 0;
                BCPStartTimeFlag = 0;
                memset(rnl,0,sizeof(*rnl));
                memset(&RNL_Var, 0, sizeof(RNL_Var));
                GunNumber->EVChargingState =  CHARGING_STANDBY;
                GunNumber->AAGValue = 0;
                GunNumber->PLC_Start = 0;
            }
            else if((rnl->BasicHSData.BasicHSDataSignal.CR_Plc_CpStatus == 1)&&(rnl->BasicHSData.BasicHSDataSignal.CR_Plc_CommStart != 1))
            {
                rnlSend->ChargeHandshake.ChargeHandshakeMessageSignal.StartPLC = 0x1;
                GunNumber->EVConnectionState =  EV_CONNECTED;
                GunNumber->EVChargingState =  CHARGING_STANDBY;
            }
            else if((rnl->BasicHSData.BasicHSDataSignal.PLC_AAG_Detection_Value != 0) && ((rnl->BasicHSData.BasicHSDataSignal.CR_Plc_CommStart == 1)))
            {
                    *RNLCTRL = SendCRM;
                    if(rnl->BasicResponse.BasicResponseSignal.CR_Plc_EVCC_MAC == 0 && BRMStartTimeFlag == 0)
                    {
                        rnlSend->ChargeRecognition.ChargeRecognitionSignal.SECC_Recognition_Result  = 0;
                        startTimeBRM = CPUTimergetTimerCount(CPUTIMER0_BASE);
                        BRMStartTimeFlag = 1;
                    }
                    uint64_t currentTimeBRM = CPUTimergetTimerCount(CPUTIMER0_BASE);
                    uint64_t elapsedTimeBRM = (startTimeBRM >= currentTimeBRM) ? (startTimeBRM - currentTimeBRM) : (startTimeBRM + (0xFFFFFFFF - currentTimeBRM));
                    if(BRMStartTimeFlag == 1 && elapsedTimeBRM > Timeout_MS(100000))
                    {
                        GunNumber->EVConnectionState =  EV_ERROR;
                        rnlSend->CEM.CEMSignal.CF_Secc_BRMTimeout = 0x1;
                        return -1;
                    }
            }
            break;
        }
        case BRMFlag:
        {
            rnlSend->ChargeRecognition.ChargeRecognitionSignal.SECC_Recognition_Result = 0xAA;
            GunNumber->VehicleID = rnl->BasicResponse.BasicResponseSignal.CR_Plc_EVCC_MAC;
            if(BCPStartTimeFlag == 0)
            {
                startTimeBRM = CPUTimergetTimerCount(CPUTIMER0_BASE);
                BCPStartTimeFlag = 1;
            }
            uint64_t currentTimeBCP = CPUTimergetTimerCount(CPUTIMER0_BASE);
            uint64_t elapsedTimeBCP = (startTimeBRM >= currentTimeBCP) ? (startTimeBRM - currentTimeBCP) : (startTimeBCP + (0xFFFFFFFF - currentTimeBCP));
            if(BCPStartTimeFlag == 1 &&  (elapsedTimeBCP >Timeout_MS(60000)))
            {
                GunNumber->EVConnectionState =  EV_ERROR;
                rnlSend->CEM.CEMSignal.CF_Secc_BCPTimeout = 0x1;
                return -1;
            }
            break;
        }
        case RTS1Flag:
        {
            *RNLCTRL =  SendCTS1;
            RNL_Var.BMRMsg = 0;
            RNL_Var.BCS_CTS = 0;
            break;
        }
        case BCPFlag:
        {
                *RNLCTRL =  SendCTSCML;
                GunNumber->EVMaxVoltage = rnl->Battery_Charging_Parameter.CR_Plc_EvmaxVolt*FACTOR_MAXVOLT;
                 if(rnl->Battery_Charging_Parameter1.Battery_Charging_Parameter1Signal.CR_Plc_EvmaxCurrentFalg == 1)
                 {
                     GunNumber->EVMaxCurrent =  ((rnl->Battery_Charging_Parameter.CR_Plc_EvmaxCurrent + 4000) *FACTOR_MAXCURR);
                 }
                 else if (rnl->Battery_Charging_Parameter1.Battery_Charging_Parameter1Signal.CR_Plc_EvmaxCurrentFalg == 0)
                 {
                     GunNumber->EVMaxCurrent =  ((4000 - rnl->Battery_Charging_Parameter.CR_Plc_EvmaxCurrent) *FACTOR_MAXCURR);
                 }
                GunNumber->CableCheckVoltage = GunNumber->EVMaxVoltage;
                GunNumber->Battery_State_Of_Charge = rnl->Battery_Charging_Parameter.CR_Plc_EvRessSOC;
                GunNumber->EVConnectionState =  EV_CONNECTED;
                GunNumber->EVChargingState =  CHARGING_STANDBY;
                break;
        }
        case BROFlag:
        {
            if(rnl->BatteryReadinessOK.BatteryReadinessOKSignal.CF_Plc_PowerDeliveryStart != 0)
            {
                    *RNLCTRL = SendCRO;
                    GunNumber->EVConnectionState =  EV_ENERGY_TRANSFER_ALLOWED;
                    GunNumber->EVChargingState =  CHARGING_CABLE_CHECK;
                    GunNumber->TargetContactorState = 1;
            }
            break;
        }
        case RTS2Flag:
        {
            *RNLCTRL =  SendCTS2;
            RNL_Var.BCS_CTS = 0;
            break;
        }

        case BCSFlag:
        {
             *RNLCTRL = SendCCS;
             RNL_Var.BCS_CTS = 0;
             GunNumber->TimeToFullSoC = rnl->BatteryChargingStatus.CR_Plc_FullSOCRemainedTime;
             GunNumber->Battery_State_Of_Charge =  rnl->BatteryChargingStatus.CR_Plc_EvRessSOC;
             break;
        }
        case BSTFlag:
        {
            *RNLCTRL = SendCST;
            GunNumber->RectDriveCurrent = 0;
            GunNumber->EVConnectionState =  EV_ENERGY_TRANSFER_ALLOWED;
            GunNumber->EVChargingState =  CHARGING_POSTCHARGE;
            if((rnl->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_InstationFault == 1)||
            (rnl->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_ConnectorHighTemp ==1 )||
            (rnl->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_BmsOverTemp == 1)||
            (rnl->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_ConnectorFault == 1)||
            (rnl->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_BatOverTemp == 1)||
            (rnl->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_HighVoltRelay == 1)||
            (rnl->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_OtherFault == 1)||
            (rnl->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_ErrCheckPoint2 == 1)||
            (rnl->BatteryStopTrigger.BatteryStopTriggerSignal.CF_Plc_CurrOver == 1))
            {
                GunNumber->VehicleReset = 1;
            }
         break;
        }
        case BSDFlag:
        {
            if(rnl->BatteryShutdown.BatteryShutdownSignal.WeldDetectionRequest != 0)
            {
                *RNLCTRL = SendCSD;
                GunNumber->IOTstopCharge = 0;
                GunNumber->RectDriveCurrent = 0;
                GunNumber->RectDriveVoltage = 0;
                GunNumber->EVConnectionState =  EV_ENERGY_TRANSFER_ALLOWED;
                GunNumber->EVChargingState =  CHARGING_POSTCHARGE;
            }
            else
            {
                *RNLCTRL = NoSend;
            }
            break;
        }

        case BEMFlag:
        {

           if(( rnl->BEMValue.BEMSignal.CF_Plc_CRM00Timeout == 1) ||( rnl->BEMValue.BEMSignal.CF_Plc_CRMAATimeout == 1 )||
              ( rnl->BEMValue.BEMSignal.CF_Plc_CTS_CMLTimeout == 1 )||(rnl->BEMValue.BEMSignal.CF_Plc_CROAALTimeout == 1)||
              ( rnl->BEMValue.BEMSignal.CF_Plc_CCSTimeout == 1 )||( rnl->BEMValue.BEMSignal.CF_Plc_CSTTimeout == 1 )||
              ( rnl->BEMValue.BEMSignal.CF_Plc_CSDTimeout == 1 )||(rnl->BEMValue.BEMSignal.CR_Plc_ErrorCode != 0) ||
              (rnl->BEMValue.BEMSignal.CR_Plc_ErrorCodeII != 0)||(rnl->BEMValue.BEMSignal.CR_PLc_EvErrCode != 0)||
              (rnl->BEMValue.BEMSignal.CP_PP_State_ErrorCode == 0x3)||(rnl->BEMValue.BEMSignal.CP_PP_State_ErrorCode == 0x4) ||
              (rnl->BEMValue.BEMSignal.CP_PP_State_ErrorCode == 0x5))
           {
               GunNumber->EVConnectionState =  EV_ERROR;
               rnl->BEMValue.BEM_Data = 0;
               return -1;
           }
           break;
        }
    }

    return 0;

}
int8_t GetEVDataBCL(GunStruct *GunNumber,ReceiveFromRMLStruct *rnl,RNLMSGCTRLEnum *RNLCTRL,SendToRNLStruct *rnlSend,RNLFlagEnum *FLAG)
{

            // Current Caluacltion starts here
            if(rnl->BatteryChargingLimit.BatteryChargingLimitSignal.CR_Plc_EvTargetCurrent_400A == 0)
            {
                //GunNumber->Targ_Curr_Flag = 0;
                GunNumber->TargetCurrent = ( 4000 - (rnl->BatteryChargingLimit.BatteryChargingLimitSignal.CR_Plc_EvTargetCurrent) )*FACTOR_MAXCURR;
            }
            else if(rnl->BatteryChargingLimit.BatteryChargingLimitSignal.CR_Plc_EvTargetCurrent_400A == 1)
            {
               // GunNumber->Targ_Curr_Flag = 1;
                GunNumber->TargetCurrent = ((rnl->BatteryChargingLimit.BatteryChargingLimitSignal.CR_Plc_EvTargetCurrent) + 4000)*FACTOR_MAXCURR;
            }
            // Current Caluacltion ends here

            // BCL Msg from RNL to GUn start nhere
            if((rnl->BatteryChargingLimit.BatteryChargingLimitSignal.CR_Plc_PreCharge_Stauts == 1) && (GunNumber->EVChargingState ==  CHARGING_CABLE_CHECK) )
            {
                GunNumber->TargetVoltage = (float)((rnl->BatteryChargingLimit.BatteryChargingLimitSignal.CR_Plc_EvTargetVolt)*FACTOR_MAXVOLT);
                GunNumber->EVChargingState = CHARGING_PRECHARGE;
                GunNumber->TargetContactorState =1;
            }
            else if((GunNumber->EVChargingState == CHARGING_PRECHARGE) &&( rnl->BatteryChargingLimit.BatteryChargingLimitSignal.CR_Plc_PreCharge_Stauts == 0))
            {
                GunNumber->EVMaxVoltage = rnl->Battery_Charging_Parameter.CR_Plc_EvmaxVolt*FACTOR_MAXVOLT;
                GunNumber->TargetVoltage = (float)((rnl->BatteryChargingLimit.BatteryChargingLimitSignal.CR_Plc_EvTargetVolt)*FACTOR_MAXVOLT);
                GunNumber->EVChargingState = CHARGING_CHARGE;
            }
            else if(GunNumber->EVChargingState == CHARGING_CHARGE)
            {
                GunNumber->EVConnectionState = EV_ENERGY_TRANSFER_ALLOWED;
                GunNumber->TargetVoltage = (float)((rnl->BatteryChargingLimit.BatteryChargingLimitSignal.CR_Plc_EvTargetVolt)*FACTOR_MAXVOLT);
                GunNumber->TargetContactorState =1;
            }


    return 0;

}
int8_t GetEVDataBST(GunStruct *GunNumber,ReceiveFromRMLStruct *rnl,RNLMSGCTRLEnum *RNLCTRL,SendToRNLStruct *rnlSend,RNLFlagEnum *FLAG)
{

            *RNLCTRL = SendCST;
            *FLAG = NoFlag;
            GunNumber->RectDriveCurrent = 0;
            GunNumber->RectDriveVoltage = 0;
            GunNumber->TargetVoltage = 0;
            GunNumber->TargetCurrent = 0;
            GunNumber->TotalMeasuredCurrent = 0;
            GunNumber->EVConnectionState =  EV_ENERGY_TRANSFER_ALLOWED;
            GunNumber->EVChargingState =  CHARGING_POSTCHARGE;

            return 0;

}

/**
 *  SendtoRNL
 *
 *  @brief Send the data to the gun1 or gun2.
 *
 *  This function is used to send the voltage, current, power min and max limits and charge status and measured voltage and current
 *  to a specific gun with help of  RNL SECC.
 *
 *  @param GunX is a enum variable, it tell which gun
 *  @param GunNumber is a pointer to the GunStruct structure.
 *  @param RNLCTRL is a pointer to enum, it tell which msg should we send
 *  @param rnl is a pointer to the RNL Struct, it tell which parameter from the receive struct should we send
 *
 *  @return None.
 */
void SendToRNL(GunNumberType GunX,GunStruct *GunNumber,RNLMSGCTRLEnum *RNLCTRL,SendToRNLStruct *rnl)
{
    //CTS MESSAGES FOR MULTIPLE PACKETS
    uint8_t CTS_Msg1[8] = {0x11, 0x02, 0x01, 0xFF, 0xFF, 0x00, 0x06, 0x00};
    uint8_t CTS_Msg2[8] = {0x13, 0x0E, 0x00, 0x02, 0xFF, 0x00, 0x06, 0x00};
    uint32_t CANBaseAddr;
    if(GunX == Gun1)
    {
        CANBaseAddr =RECTIFIER_GUN1;
    }
    else
    {
        CANBaseAddr =RECTIFIER_GUN2;
    }
    //CANBaseAddr = CMSCAN;

   // SendtoRML_CEM(CANBaseAddr,&rnl->CEM);
    if(GunNumber->StopCharge == 1)
    {
        *RNLCTRL = SendCST;
       // GunNumber->StopCharge = 0;
       // GunNumber->IOTstopCharge = 0;
    }
        switch (*RNLCTRL)
            {
            case SendCHM:
               rnl->ChargeHandshake.ChargeHandshakeMessageSignal.StartCPState = 0;  // based on Testing
               rnl->ChargeHandshake.ChargeHandshakeMessageSignal.MaxPowerLimit = (uint16_t)(MaxPower /FACTOR_POWERMAXLIMIT);  // Will Changed based on tetra algo of power sharing
               rnl->ChargeHandshake.ChargeHandshakeMessageSignal.InitCPState = 0;  // Need to chagne in case or in case of SECC Fault ambiu
               SendtoRML_CHM(CANBaseAddr,&rnl->ChargeHandshake);
               break;
            case SendCRM:
                SendtoRML_CRM( CANBaseAddr,&rnl->ChargeRecognition);
                break;
            case SendCTS1:
                if(RNL_Var.BMRMsg == 0)
                {
                    CAN_sendMessage(CANBaseAddr, CANOBJ_BCPA_TX, 8, (uint16_t *)CTS_Msg1);
                    RNL_Var.BMRMsg = 1;
                }

                break;
            case SendCTSCML:
                if(RNL_Var.BCP_CTS == 0)
                {
                    CAN_sendMessage(CANBaseAddr, CANOBJ_BCPA_TX, 8, (uint16_t *)CTS_Msg2);
                    RNL_Var.BCP_CTS = 1;
                }
                SendtoRML_CTS( CANBaseAddr,&rnl->ChargingTimeStart);

                rnl->ChargingMaxLimit.ChargingMaxLimitSignal.CR_Secc_EvseMaxVolt_V = (ChargerMaxVoltage / FACTOR_MAXVOLT);
                rnl->ChargingMaxLimit.ChargingMaxLimitSignal.CR_Secc_EvseMinVolt_V = (ChargerMinVoltage / FACTOR_MINVOLT);
                if(ChargerMaxCurrent < 400)
                {
                    rnl->ChargingMaxLimit.ChargingMaxLimitSignal.CR_Secc_EvseMaxCurr_A400A = 0;
                    rnl->ChargingMaxLimit.ChargingMaxLimitSignal.CR_Secc_EvseMaxCurr_A =(uint16_t) ((400 - ChargerMaxCurrent)/FACTOR_MAXCURR);
                    }
                else
                {
                    rnl->ChargingMaxLimit.ChargingMaxLimitSignal.CR_Secc_EvseMaxCurr_A400A = 1;
                    rnl->ChargingMaxLimit.ChargingMaxLimitSignal.CR_Secc_EvseMaxCurr_A = (uint16_t)((ChargerMaxCurrent + 400)/FACTOR_MAXCURR);
                }
                if(ChargerMinCurrent < 400)
                {
                    rnl->ChargingMaxLimit.ChargingMaxLimitSignal.CR_Secc_EvseMinCurr_A400A = 0;
                    rnl->ChargingMaxLimit.ChargingMaxLimitSignal.CR_Secc_EvseMinCurr_A =(uint16_t) ((400 - ChargerMinCurrent)/FACTOR_MINCURR);
                    }
                else
                {
                    rnl->ChargingMaxLimit.ChargingMaxLimitSignal.CR_Secc_EvseMinCurr_A400A = 1;
                    rnl->ChargingMaxLimit.ChargingMaxLimitSignal.CR_Secc_EvseMinCurr_A = (uint16_t)((ChargerMinCurrent + 400)/FACTOR_MINCURR);
                }
                SendtoRML_CML(CANBaseAddr,&rnl->ChargingMaxLimit);
                break;

            case SendCRO:
                if(GunNumber->IMDEnable == 0)
                {
                  rnl->ChargerReadinessOK.ChargerReadinessOKSignal.CR_Secc_ContactorOn =0;
                }
                else if(GunNumber->IMDEnable == 1)
                {
                    rnl->ChargerReadinessOK.ChargerReadinessOKSignal.CR_Secc_ContactorOn =0xAA;
                }
                rnl->ChargerReadinessOK.ChargerReadinessOKSignal.CR_Secc_EvseIsolationStatus =  GunNumber->IsolationState;
                SendtoRML_CRO(CANBaseAddr,&rnl->ChargerReadinessOK);
                break;
            case SendCTS2:
                if(RNL_Var.BCS_CTS == 0)
                {
                    //uint8_t CTS2_Msg1[8] = {0x11, 0x02, 0x01, 0xFF, 0xFF, 0x00, 0x11, 0x00};
                   // CAN_sendMessage(CANBaseAddr, CANOBJ_BCPA_TX, 8, (uint16_t *)CTS2_Msg1);
                    RNL_Var.BCS_CTS=1;
                }
                break;
            case SendCCS:
                if(RNL_Var.BCS_CTS == 0)
                {

                    RNL_Var.BCS_CTS = 1;
                }
               rnl->ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutVolt = GunNumber->VseccMeasuredVoltage/FACTOR_MAXVOLT;
                if(GunNumber->VseccMeasuredCurrent < 400)
                {
                    rnl->ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutCurrentOver400A = 0;
                    rnl->ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutCurrent = (400 - GunNumber->VseccMeasuredCurrent)/FACTOR_MAXCURR;
                }
                else
                {
                    rnl->ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutCurrentOver400A = 1;
                    rnl->ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutCurrent = ( GunNumber->VseccMeasuredCurrent -  400)/FACTOR_MAXCURR;
                }
                rnl->ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_MaxPowerLimit = (uint16_t) (GunNumber->VseccMaxPower / FACTOR_POWERMAXLIMIT) ; // Check Factor
                rnl->ChargerChargingStatus.ChargerChargingStatusSignal.CF_Secc_EvseSuspended = 0x1;// might need this parameter to retry
                rnl->ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvsseIsolationStatus = GunNumber->IsolationState;
                rnl->ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseMaxCurr_IsUse = 0x1; // might need to change basaed on testing , might require processing
                rnl->ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseMaxCurr_A = ChargerMaxCurrent;// 0 to 1000 range and factor multiple is 1
               // SendtoRML_CCS(CANBaseAddr, &rnl->ChargerChargingStatus);
                break;
            case SendCSD:
                rnl->ChargerShutdown.ChargerShutdownSignal.CR_Secc_EvseOutVolt = (GunNumber->VseccMeasuredVoltage/FACTOR_MAXVOLT);
                rnl->ChargerShutdown.ChargerShutdownSignal.CR_Secc_EvseIsolationStatus = GunNumber->IsolationState;
                SendtoRML_CSD(CANBaseAddr,&rnl->ChargerShutdown);
                break;
            case SendCST:
                rnl->ChargingStopTime.ChargingStopTimeSignal.CF_Secc_Suspended = 1;
                SendtoRML_CST(CANBaseAddr,&rnl->ChargingStopTime);
                break;
             }
}
/**
 *  GetRectStatusInfo
 *
 *  @brief Stores the data received from tonhe struct to the respective gun structure.
 *
 *  This function stores the Tonhe rectifier-related data into the `GunStruct` structure, which is present inside the
 *  `CMSReceiveFromTonheStruct` structure received from the rectifier.
 *
 *  @param GunNumber Pointer to the `GunStruct` structure containing all measured values like Current, Voltage, etc.
 *  @param Temp pointer to the `CMSReceiveFromTonheStruct` which stores the data received from Tonhe rectifier r.
 *
 *  @return None.
 */
void GetRectStatusInfo(GunStruct  *GunNumber,CMSReceiveFromTonheStruct *temp)
{
    uint8_t i=0;
    GunNumber->TotalMeasuredCurrent = 0;
    for (i =0;i<=RectPerGun;i++)
    {
        GunNumber->ChargeModuleState[i] =  (ChargeModuleStateEnum)temp[i].ChargingModuleState.ModuleState;
        GunNumber->FaultAlarm |=  temp[i].ChargingModuleState.FaultFlags;
        GunNumber->PFCFault |= temp[i].ChargingModuleState.PfcFault;

        GunNumber->StartStopFB &= temp[i].ModuleStartStopConfirmation.MessageStatus;
        GunNumber->ExtensionFaultWarning |= temp[i].ExtensionStatusFault.FaultWarning;

        GunNumber->RectAmbiantTemp[i] = temp[i].AcInformation.AmbientTemperature;
        GunNumber->MeasuredCurrent[i] = temp[i].ChargingModuleState.OutputCurrent;

        GunNumber->ModuleState[i] = temp[i].ExtensionStatusFault.ModuleState;
        GunNumber->ExtensionFaultWarning |= temp[i].ExtensionStatusFault.FaultWarning;
        GunNumber->TotalMeasuredCurrent += temp[i].ChargingModuleState.OutputCurrent;
    }
    // As Voltage in parallel will be same so taking rectifier 1 as referance as of now
    GunNumber->MeasuredVoltage =   temp[0].ChargingModuleState.OutputVoltage;
    GunNumber->RectPhaseAVoltage = temp[0].AcInformation.APhaseVoltage;
    GunNumber->RectPhaseBVoltage = temp[0].AcInformation.BPhaseVoltage;
    GunNumber->RectPhaseCVoltage = temp[0].AcInformation.CPhaseVoltage;
}

/**
 *  GetDCEMValue
 *
 *  @brief Stores DC Energy meter data received from DC_EnergyMeterDataStruct.
 *
 *  This function stores the DC Energy meter-related data into the `GunStruct` structure, which is present inside the
 *  `DC_EnergyMeterDataStruct` structure.
 *
 *  @param GunNumber Pointer to the `GunStruct` structure containing all measured values like Current, Voltage, etc.
 *  @param Temp pointer to the `DC_EnergyMeterDataStruct` which stores the data received from DC Energy meter.
 *
 *  @return None.
 */
void GetDCEMValue(GunStruct * GunNumber, DC_EnergyMeterDataStruct *Temp)
{
    GunNumber->DCEMCurrent = Temp->Current;
    GunNumber->DCEMVoltage =  Temp->Voltage;
    GunNumber->DCEMEnergyReading =  Temp->PosEnergy;
}

/**
 *  GetDCEMStartEnergy
 *
 *  @brief Stores DC Energy meter data received from DC_EnergyMeterDataStruct.
 *
 *  This function stores the DC Energy meter-related data into the `GunStruct` structure, which is present inside the
 *  `DC_EnergyMeterDataStruct` structure.
 *
 *  @param GunNumber Pointer to the `GunStruct` structure containing all measured values like Current, Voltage, etc.
 *  @param Temp pointer to the `DC_EnergyMeterDataStruct` which stores the data received from DC Energy meter.
 *
 *  @return None.
 */
void GetDCEMStartEnergy(GunStruct * GunNumber, DC_EnergyMeterDataStruct *Temp)
{
    GunNumber->DCEMStartEnergy =  Temp->PosEnergy;
}

/**
 *  GetDCEMEndEnergy
 *
 *  @brief Stores DC Energy meter data received from DC_EnergyMeterDataStruct.
 *
 *  This function stores the DC Energy meter-related data into the `GunStruct` structure, which is present inside the
 *  `DC_EnergyMeterDataStruct` structure.
 *
 *  @param GunNumber Pointer to the `GunStruct` structure containing all measured values like Current, Voltage, etc.
 *  @param Temp pointer to the `DC_EnergyMeterDataStruct` which stores the data received from DC Energy meter.
 *
 *  @return None.
 */
void GetDCEMEndEnergy(GunStruct * GunNumber, DC_EnergyMeterDataStruct *Temp)
{
    GunNumber->DCEMEndEnergy =  Temp->PosEnergy;
}

/**
 *  GetDCEMStartTime
 *
 *  @brief Stores the start time of DC Energy meter.
 *
 *  This function stores the DC Energy meter start time into the `GunStruct` structure.
 *
 *  @param GunNumber pointer to the `GunStruct` structure containing the start time and different data.
 *  @param `time` it is a temperary variable which stores the time.
 *
 *  @return None.
 */
void GetDCEMStartTime(GunStruct * GunNumber,uint32_t time)
{
    GunNumber->DCEMStartTime =  time;
}

/**
 *  GetDCEMStopTime
 *
 *  @brief Stores the stop time of DC Energy meter.
 *
 *  This function stores the DC Energy meter stop time into the `GunStruct` structure.
 *
 *  @param GunNumber pointer to the `GunStruct` structure containing the stop time and different data.
 *  @param `time` it is a temparary variable which stores the time.
 *
 *  @return None.
 */
void GetDCEMStopTime(GunStruct * GunNumber,uint32_t time)
{
    GunNumber->DCEMEndTime =  time;
}

/**
 *  GetIMDData
 *
 *  @brief Stores the resistance of IMD.
 *
 *  This function stores the resistance of IMD into the `GunStruct` structure.
 *
 *  @param GunNumber pointer to the `GunStruct` structure containing the resistance and different data.
 *  @param `Resistance` it is a temparary variable which stores the resistance given by IMD.
 *
 *  @return None.
 */
void GetIMDData(GunStruct * GunNumber, float Resistance)
{
    GunNumber->InsulationResistance =  Resistance;
}

/**
 *  GetDigitalInputData
 *
 *  @brief Stores the digital input values given by I2C IO expanders.
 *
 *  This function stores the digital input values coming from I2C IO Expanders and CMS board.
 *  Stored in GunStruct and ChargerStruct structures.
 *
 *  @param GunNumber1 and GunNumber2 both are pointer to the `GunStruct` structure which are used to store the data coming from
 *  different devices.
 *  @param ChargerData is pointer to the ChargerStruct, which stores the data coming from I2C IO expander.
 *  @param tempio1,tempio2 are temporary variable pointers used for storing temporarily
 *  @param I2Cstatus defines the status of the I2C that we get form the 'GetMuxInput ' function
 *
 *  @return None.
 */
void GetDigitalInputData(uint8_t *I2Cstatus,uint16_t *Data1, uint16_t *Data2)
{

    GetMuxInput(Data1, Data2, I2Cstatus);                // Reading it 1st Time
    *Data1 =  *Data1  & 0xFBFF;
    *Data2 =  *Data2  & 0xFBFF;
}
DIOStatus_t DigitalInputData(GunStruct * GunNumber1,GunStruct * GunNumber2,ChargerStruct *ChargerData,uint16_t tempio1,uint16_t tempio2)
{
    InputExpander1 Input1 ={0};
    InputExpander2 Input2 ={0};
    Input1.RawData = tempio2;
    Input2.RawData = tempio1;

    ChargerData->ACContactorCoilFB = (ContactorWrite)(Input1.bits.ACCoilFB);
    ChargerData->ACContactorFB =(ContactorWrite) Input1.bits.ACContactorFB;
    ChargerData->CrossContactorNegativeFB = (ContactorWrite)Input2.bits.CrossDCContactorFBLo;
    ChargerData->CrossContactorPositiveFB =(ContactorWrite) Input2.bits.CrossDCContactorFBHi;
    ChargerData->SmokeSensor = Input1.bits.SmokeSensor;
    ChargerData->ELRFB =  (1-Input1.bits.ELRFB);
    ChargerData->EmergencyButton = (EmergencyButtonStatus)Input2.bits.EmergencySW;
    ChargerData->SPDFB =  Input1.bits.SPDFB;
   // ChargerData->FrontDoor =  (DoorStatus)Input2.bits.FrontDoor;
   // ChargerData->RearDoor =  (DoorStatus)Input2.bits.RareDoor;
    ChargerData->BatteryFailure = Input1.bits.BatteryFaliure;
    ChargerData->DCOk = Input1.bits.DCOk;
    ChargerData->waterlevelsensor = Input1.bits.WaterLevelSensor;

    GunNumber1->PositiveContactorFB =  (ContactorWrite)Input2.bits.DC1PositiveContactorFB;
    GunNumber1->NegativeContactorFB =  (ContactorWrite)Input2.bits.DC1NegativeContactorFB;
    GunNumber1->GunPresecnce = Input2.bits.DC1GunPresence;

    GunNumber1->GunVoltagePresence = Input1.bits.DC1VoltageDetection;
    GunNumber1->StopChargeUserButton =  Input2.bits.DC1StopCharge;
    GunNumber1->IMDFault =(IMDFBStatus) Input1.bits.IMD1FB;
    //GunNumber1->MotorBallValveFB = Input2.bits.DC1MotorBallValveFB;


    GunNumber2->PositiveContactorFB =  (ContactorWrite)Input2.bits.DC2PositiveContactorFB;
    GunNumber2->NegativeContactorFB = (ContactorWrite) Input2.bits.DC2NegativeContactorFB;
    GunNumber2->GunVoltagePresence = Input1.bits.DC2VoltageDetection;
    GunNumber2->StopChargeUserButton =  Input2.bits.DC2StopCharge;
    GunNumber2->IMDFault = (IMDFBStatus)Input1.bits.IMD2FB;
   // GunNumber2->MotorBallValveFB = Input2.bits.DC2MotorBallValveFB;
    return DIOSuccess;
}
/**
 * GetACEMData
 *
 * @brief Stores AC Energy Meter data into the charger structure.
 *
 * This function copies real-time voltage, current, power, frequency, and power factor data
 * from the `AC_EnergyMeterDataStruct` to the corresponding fields in the `ChargerStruct`.
 *
 * @param ChargerData Pointer to the `ChargerStruct` structure where AC energy meter readings will be stored.
 * It includes arrays for PhaseVoltage, PhaseCurrent, PhasePower, and other parameters like frequency and power factor.
 *
 * @param ACEMData Pointer to the `AC_EnergyMeterDataStruct` structure containing the latest energy meter readings
 * including line-to-neutral voltages, line currents, active power per phase, frequency, and power factor.
 *
 * @return None
 */
void GetACEMData(ChargerStruct *ChargerData,AC_EnergyMeterDataStruct *ACEMData)
{
    ChargerData->PhaseVoltage[0]=ACEMData->LNVoltage[0];
    ChargerData->PhaseVoltage[1]=ACEMData->LNVoltage[1];
    ChargerData->PhaseVoltage[2]=ACEMData->LNVoltage[2];
    ChargerData->PhaseCurrent[0]=ACEMData->Linecurrent[0];
    ChargerData->PhaseCurrent[1]=ACEMData->Linecurrent[1];
    ChargerData->PhaseCurrent[2]=ACEMData->Linecurrent[2];
    ChargerData->PhasePower[0]=ACEMData->ActivePower[0];
    ChargerData->PhasePower[1]=ACEMData->ActivePower[1];
    ChargerData->PhasePower[2]=ACEMData->ActivePower[2];
    ChargerData->frequency=ACEMData->Frequency;
    ChargerData->PowerFactor=ACEMData->PowerFactor;
    ChargerData->PhaseEnergy[0]=ACEMData->ActiveEnergy[0];
    ChargerData->PhaseEnergy[1]=ACEMData->ActiveEnergy[1];
    ChargerData->PhaseEnergy[2]=ACEMData->ActiveEnergy[2];
}

/**
 *  SetChargerParameter
 *
 *  @brief Setting the devices by using ChargerStruct data.
 *
 *  This function set the Colling fan, Exhaust fan, etc by using the ChargerStruct data and GPIO pins.
 *
 *  @param ChargerData is a pointer to the ChargerStruct where the charger parameters are stored.
 *
 *  @return None.
 */
void SetChargerParameter(ChargerStruct *ChargerData)
{
    WritePin(RAD_EN_MCU,ChargerData->CoolingFanControl);
    WritePin(EXHAUST_FAN,ChargerData->ExhuastFanControl);
    WritePin(PUMP_POWER_ENABLE,ChargerData->CoolingPumpPowerControl);
    WritePin(AC_CONTECTOR,ChargerData->ACContactorDrive);
    WritePin(CROSS_CONTACTOR,ChargerData->CrossContactorDrive);
    WritePin(PLC_ENABLEPIN,ChargerData->VSECCPowerEnable);
   // WritePin(STATION_LED,ChargerData->StationLED);
}

/**
 *  SetGunContactor
 *
 *  @brief Setting the gun-1 or gun-2 contactor and precharge
 *
 *  This function is sets the gun contactor and precharge turn ON or OFF.
 *
 *  @param GunX is use GunNumberType structure to choose the gun1 or gun2.
 *  @param Pos is use ContactorWrite to turn ON or OFF the positive contactor.
 *  @param Neg is use ContactorWrite to turn ON or OFF the negative contactor.
 *  @param Precharge is use ContactorWrite to turn ON or OFF the Precharge.
 *
 *  @return None
 */
void SetGunContactor(GunNumberType GunX, ContactorWrite Pos, ContactorWrite Neg, ContactorWrite Precharge)
{
    switch (GunX)
    {
    case Gun1:
        WriteDGPIO(GUN1_DCPOSITIVE_CONTACTOR, (uint32_t)Pos);
        WriteDGPIO(GUN1_DCNEGATIVE_CONTACTOR, (uint32_t)Neg);
        WriteDGPIO(GUN1_DCPRECHARGE_CONTACTOR, (uint32_t)Precharge);
        break;
    case Gun2:
        WriteDGPIO(GUN2_DCPOSITIVE_CONTACTOR, (uint32_t)Pos);
        WriteDGPIO(GUN2_DCNEGATIVE_CONTACTOR, (uint32_t)Neg);
        WriteDGPIO(GUN2_DCPRECHARGE_CONTACTOR,(uint32_t)Precharge);
        break;
    }
}

/**
 *  SetStatusLED
 *
 *  @brief Sets the status led of specific gun based on which gun is in use.
 *
 *  This function is sets the status led of specific gun based on which gun is in use.
 *
 *  @param GunNumber is a pointer to the structure it specified the gun1 or gun2.
 *  @param LEDValue is a boolean parameter to set the led.
 *
 *  @return None
 */
void SetStatusLED(GunStruct *GunNumber, bool LEDValue) {
    if (GunNumber->GunNum == 1)
    {
        WritePin(DC1_STS_LED_MCU, LEDValue);
    }
    else if (GunNumber->GunNum == 2)
    {
        WritePin(DC2_STS_LED_MCU, LEDValue);
    }
}

/**
 *  SetStopChargeLED
 *
 *  @brief Sets the stop charge led.
 *
 *  This function is used to sets the stop charge led based on stop button.
 *
 *  @param GunNumber is a pointer to structure it specified the gun1 or gun2.
 */
void SetStopChargeLED(GunStruct *GunNumber, bool LEDValue) {
    if (GunNumber->GunNum == 1)
    {
        WritePin(DC1_STP_LED_MCU, LEDValue);
    }
    else if (GunNumber->GunNum == 2)
    {
        WritePin(DC2_STP_LED_MCU, LEDValue);
    }
}

/**
 *  SetMotBallValveIN12
 *
 *  @brief Setting the Motball valve, based on which gun is turned on.
 *
 *  This function is helpful to turn on the gun1 or gun2 and turn off motball valve based on which gun is turned on.
 *
 *  @param GunNumber is a pointer to the GunStruct structure.
 *  @param value1 and value2 is a boolean variables
 *
 *  @return None.
 */
void SetMotBallValveIN12(GunStruct *GunNumber, bool value1, bool value2)
{

    if (GunNumber->GunNum == 2)
    {
        SetDC2MOTBVIN2(I2C_BASE_IOEXPANDER,value2);
        SetDC2MOTBVIN1(I2C_BASE_IOEXPANDER,value1);
    }
    else if (GunNumber->GunNum == 1)
    {
        SetDC1MOTBVIN2(I2C_BASE_IOEXPANDER,value2);
        SetDC1MOTBVIN1(I2C_BASE_IOEXPANDER,value1);
    }
}

/**
 *  SetGunSolenoideDriveIN12
 *
 *  @brief Setting the Solenoide, based on which gun is turned on.
 *
 *  This function is helpful to turn on the gun1 or gun2 and turn off solenoide based on which gun is turned on.
 *
 *  @param GunNumber is a pointer to the GunStruct structure.
 *  @param value1 and value2 is a boolean variables
 *
 *  @return None.
 */
void SetGunSolenoideDriveIN12(GunStruct *GunNumber, bool value1, bool value2)
{
    if (GunNumber->GunNum == 2)
    {
        SetDC2GUNSolenoid1(I2C_BASE_IOEXPANDER,value2);
        SetDC2GUNSolenoid1(I2C_BASE_IOEXPANDER,value1);
    }
    else if (GunNumber->GunNum == 1)
    {
        SetDC1GUNSolenoid1(I2C_BASE_IOEXPANDER,value2);
        SetDC1GUNSolenoid1(I2C_BASE_IOEXPANDER,value1);
    }
}

/**
 * SendSCIErrorCan
 *
 *  @brief Send can message for the current error status of the SCI.
 *
 *  @param canData is the pointer to the data we have to send.
 *  @param CANBaseAddr is the base address of the CAN Module we are using.
 *
 *  @return None.
 */
void SendSCIErrorCan(uint8_t *canData,uint32_t CANBaseAddr)
{
   // CAN_sendMessage(CANBaseAddr, CANOBJ_SCIERR_TX, 8,(uint16_t *)canData);
}

/**
 * SetRectVoltageCurrent
 *
 *  @brief Set the rectifier voltage and current.
 *
 *  This function is helpful to set rectifier volatge and current, for gun1 or gun2 by using the TonheSetParameter function
 *  to send the values.
 *
 *  @param GunNumber is a GunNumberType enum, it tell which gun.
 *  @param GunData is a pointer to a GunStruct structure, where voltage and current information is stored.
 *  @param ParallalRect is a variable, by using this we divide the current, based on vehicle demand.
 *
 *  @return None.
 */
void SetRectVoltageCurrent(GunNumberType  GunNumber,GunStruct *GunData)
{
    uint32_t CANBASE;
    float Current=0;
    Current=(GunData->RectDriveCurrent);
    if (GunNumber == Gun1)
    {
        CANBASE =RECTIFIER_GUN1;
    }
    else if (GunNumber == Gun2)
    {
        CANBASE =RECTIFIER_GUN2;
    }

    TonheSetParameter(CANBASE,GunData->RectDriveVoltage,Current);
}

/**
 *  RectAliveCommand
 *
 *  @brief Send alive command to rectifier
 *
 *  This function is helpful to send a rectifier alive command for every 5 second to rectifier to ensure the charging continuosly,
 *  when alive command did'nt receive for every 5 second the system stop the charging.
 *
 *  @param GunNumber is a GunNumberType structure variable, it tells which gun.
 *
 *  @return None.
 */
void RectAliveCommand(GunNumberType GunNumber) // Every 5 Second
{
    uint32_t CANBASE;
    if (GunNumber == Gun1)
    {
        CANBASE =RECTIFIER_GUN1;
    }
    else if (GunNumber == Gun2)
    {
        CANBASE =RECTIFIER_GUN2;
    }
    TonheSendTimingCommand(CANBASE);
}

/**
 *  GetAnalogInputData
 *
 *  @brief Stores the analog data of a specific gun.
 *
 *  This function is helpful to store the analog temperature values which are in the ChargerAnalogReadStruct into
 *  the GunStruct and ChargerStruct.
 *
 *  @param GunNumber is a pointer to GunStruct structure, where we store Busbar, Gun positive and Gun Negative temperatures.
 *  @param ChargerAnalog is a pointer to ChargerStruct structure, where we store the Ambient and Fluid temperature.
 *  @param ChargerAnalogRead is a pointer to ChargerAnalogReadStruct, from here we get all data.
 *
 *  @return None.
 */
void GetAnalogInputData(GunStruct *GunData1,GunStruct *GunData2,ChargerStruct *ChargerAnalog,ChargerAnalogReadStruct *ChargerAnalogRead)
{
    ChargerAnalog->EnvAmbTemp =GetNT1000TempValue(ChargerAnalogRead->ChargerAmbTempVolt);
    ChargerAnalog->FluidTemperature =GetNT1000TempValue(ChargerAnalogRead->FluidTempVolt);

    GunData1->BusBarTemperature=GetNT1000TempValue(ChargerAnalogRead->BasBar1Volt);
    GunData1->GunPosTemperature=GetPT1000TempValue(ChargerAnalogRead->GUN1PosVolt);
    GunData1->GunNegTemperature=GetPT1000TempValue(ChargerAnalogRead->GUN1NegVolt);

    GunData2->BusBarTemperature=GetNT1000TempValue(ChargerAnalogRead->BasBar2Volt);
    GunData2->GunPosTemperature=GetPT1000TempValue(ChargerAnalogRead->GUN2PosVolt);
    GunData2->GunNegTemperature=GetPT1000TempValue(ChargerAnalogRead->GUN2NegVolt);
}

/**
 *  SetACContactor///
 *
 *  @brief It is used to set the AC contactor.
 *
 *  This function is helpful to turn on the AC contactor, by using the GPIO_writePin.
 *
 *  @return None.
 */
void SetACContactor(ContactorWrite Pos)
{
    WriteDGPIO(AC_CONTACTOR,(uint32_t)Pos);
}

/**
  *  ResetGunContactor
  *
  *  @brief IT reset the gun contactor.
  *
  *  This function is helpful to reset the gun1 or gun2 DC positive contactor or negative contactor or precharger or cross contactor
  *  based on gun, and x parameters.
  *
  *  @param Gun is a GunNumberType enum variable it tell which gun.
  *  @param x is GunContactortype enum which tells which contactor.
  *
  *  @return None.
  */
void ResetGunContactor(GunNumberType Gun,GunContactortype x)
{
    if(Gun ==Gun1)
    {
        switch(x)
        {
            case Positive:
                WritePin(GUN1_DCPOSITIVE_CONTACTOR,0);
                break;
            case Negetive:
                WritePin(GUN1_DCNEGATIVE_CONTACTOR,0);
                break;
            case PreCharge:
                WritePin(GUN1_DCPRECHARGE_CONTACTOR,0);
                break;
            case Cross:
                WritePin(CROSS_CONTACTOR,0);
                break;
        }
    }else if (Gun ==Gun2)
    {
        switch(x)
        {
            case Positive:
                  WritePin(GUN1_DCPOSITIVE_CONTACTOR,0);
                break;
            case Negetive:
                WritePin(GUN1_DCNEGATIVE_CONTACTOR,0);
                break;
            case PreCharge:
                WritePin(GUN1_DCPRECHARGE_CONTACTOR,0);
                break;
            case Cross:
                WritePin(CROSS_CONTACTOR,0);
                break;
        }
    }
}

/**
 *  StopCharging
 *
 *  @brief It is used to send stop charging to VSECC.
 *
 *  This function is helpful to send a stop charging signal to VSECC and VSECC to specific gun, based on which gun.
 *
 *  @param Gun is a GunNumberType enum, it tells which gun.
 *
 *  @return None.
 */
void StopCharging(GunNumberType Gun,SendToRNLStruct *rnl,RNLMSGCTRLEnum *RNLCTRL)
{
    if(Gun ==Gun1)
    {
        rnl->ChargingStopTime.ChargingStopTimeSignal.CF_Secc_ArtificialSuspend = 1;
        *RNLCTRL = SendCST;
    }
    else if(Gun ==Gun2)
    {
        rnl->ChargingStopTime.ChargingStopTimeSignal.CF_Secc_ArtificialSuspend = 1;
        *RNLCTRL = SendCST;
    }
}

/**
 *  SetCrossContactor
 *
 *  @brief It is used to set the Cross Contactor.
 *
 *  This function is helpful to turn on the AC Cross contactor, by using the GPIO_writePin.
 *
 *  @return None.
 */
void SetCrossContactor(ContactorWrite Value)
{
    WritePin(CROSS_CONTACTOR,Value);
}
/**
 *  ChargerSetInitValues
 *
 *  @brief It is used to set the initial values for the very first time.
 *
 *  This function is used to initialize the output pins and contactor initial values of the charger.

 *  @return None.
 */
void ChargerSetInitValues()
{
    InitialiseChargerOutPins();
    ChargerContectorInitValues();  // All Contactor are off

    DCEMPower(DCEM1,Off);           // DCEM Fff
    DCEMPower(DCEM2,Off);
    DisplayPower(Gun1,Off);        // Display Off
    DisplayPower(Gun2,Off);
    PLCPower(On);                       // PLC On
    RectifierOnOffCtrl(Gun1,Off);       // Rect 1  off
    RectifierOnOffCtrl(Gun2,Off);       // Rect 2  off

    StatusLED(Gun1,Off);
    StopLED(Gun1,Off);
    CoolingFan(Off);
    StationLED(Off);
    ExhaustFan(Off);
    PumpPower(Off);

    StatusLED(Gun2,Off);
    StopLED(Gun2,Off);
}
/**
 *  GunVarInitValues
 *
 *  @brief It is used to set the initial values of the Gun for the very first time.
 *
 *  This function is used to initialize the initial values of the gun structure.

 *  @return None.
 */
void GunVarInitValues(GunStruct *PrimaryGun)
{
    PrimaryGun->StartCharge = 0;
    PrimaryGun->StopChargeLED = 0;
    PrimaryGun->StopChargeUserButton = 1;
    PrimaryGun->IoTGunActivation =(bool)0;
    PrimaryGun->IOTstartCharge = (bool)0;
    PrimaryGun->IOTstartCharge = (bool)0;
    PrimaryGun->IsolationState= ISOLATION_INVALID;
    PrimaryGun->TempFault = 0;
    PrimaryGun->ParmanantFault = 0;
    PrimaryGun->IMDEnable = 0;
    PrimaryGun->ResetRquest =0;
    PrimaryGun->ChargingSpeed =  NotCharging;
    PrimaryGun->EVConnectionState = EV_DISCONNECTED;
    PrimaryGun->PrechargeContactorDrive = Off;
    PrimaryGun->PositiveContactorDrive =  Off;
    PrimaryGun->NegativeContactorDrive =  Off;
    PrimaryGun->IMDState = IMD_Enable;
}
/**
 *  ChargerContectorInitValues
 *
 *  @brief It is used to set the initial values of the Gun contactors.
 *
 *  This function is used to initialize the values of the gun contactors such precharge,cross, positve and negative contactors.

 *  @return None.
 */
void ChargerContectorInitValues()
{
    ResetGunContactor(Gun1, Positive);
    ResetGunContactor(Gun1, Negetive);
    ResetGunContactor(Gun1, PreCharge);
    ResetGunContactor(Gun2, Positive);
    ResetGunContactor(Gun2, Negetive);
    ResetGunContactor(Gun2, PreCharge);
    ResetGunContactor(Gun2, Cross);
}
/**
 *  DCEMPower
 *
 *  @brief It is used to set the DCEM pin values.
 *
 *  This function is used to set the power values for the DCEM energy meter.
 *
 *  @param number is used to define the DCEM number
 *  @param value is the value that we set for the DCEM meter
 *
 *  @return None.
 */
void DCEMPower(DCEMNumber number, PowerEnable value)
{
    switch(number)
    {
    case DCEM1:
        WriteDGPIO(DC2_EM_POWERPIN, value);
        break;
    case DCEM2:
        WriteDGPIO(DC2_EM_POWERPIN, value);
        break;
    }
}
/**
 *  DisplayPower
 *
 *  @brief It is used to set the Gun power value.
 *
 *  This function is used to set the power values for the respective Gun.
 *
 *  @param Gunx is used to define the Gun number
 *  @param value is the value that we set for the DCEM meter
 *
 *  @return None.
 */
void DisplayPower(GunNumberType Gunx,PowerEnable value)
{
    switch(Gunx)
    {
    case Gun1:
        WriteDGPIO(DISPLAY1_POWERPIN, value);
        break;
    case Gun2:
        WriteDGPIO(DISPLAY2_POWERPIN, value);
        break;
    }
}
/**
 *  PLCPower
 *
 *  @brief It is used to set the PLC value.
 *
 *  This function is used to set the power values for the PLC.
 *
 *  @param value is the value that we set for the PLC
 *
 *  @return None.
 */
void PLCPower(PowerEnable value)
{
    WriteDGPIO(PLC_ENABLEPIN,value);
}
/**
 *  RectifierOnOffCtrl
 *
 *  @brief It is used to set the Rectifier power value.
 *
 *  This function is used to set the power values for the PLC.
 *
 *  @param Gunx is used to define the Gun number
 *  @param value is the value that we set for the rectifier
 *
 *  @return None.
 */
void RectifierOnOffCtrl(GunNumberType Gunx,PowerEnable value)
{
    RectiferStartStopEnum  RectiferStartStopVar;
    if (Gunx == Gun1)
    {
        if (value == On)
        {
            RectiferStartStopVar =  RectifierOn;
        }
        else if (value == Off)
        {
            RectiferStartStopVar =  RectifierOff;
        }

           TonheStartStopCMD(RECTIFIER_GUN1,RectiferStartStopVar);
    }
    if (Gunx == Gun2)
    {
        if (value == On)
        {
            RectiferStartStopVar =  RectifierOn;
        }
        else
        {
            RectiferStartStopVar =  RectifierOff;
        }

        TonheStartStopCMD(RECTIFIER_GUN2,RectiferStartStopVar);
    }
}
/**
 *  GeneralGPIO
 *
 *  @brief It is used to set the Rectifier power value.
 *
 *  This function is used to set the power values for the PLC.
 *
 *  @param GPIOPindef1,GPIOPindef2 are the gpio pins for the gun1 and gun 2 respectively
 *  @param Gunx is used to define the Gun number
 *  @param value is the value that we set for the gun
 *
 *  @return None.
 */
void GeneralGPIO(uint32_t GPIOPindef1,uint32_t GPIOPindef2,GunNumberType Gunx,PowerEnable value)
{
    switch(Gunx)
    {
    case Gun1:
            WriteDGPIO(GPIOPindef1,(uint32_t)value);
            break;
    case Gun2:
            WriteDGPIO(GPIOPindef2,(uint32_t)value);
            break;
    }
}
/**
 *  StatusLED
 *
 *  @brief It is used to set the status led value.
 *
 *  This function is used to set the power values for the status led.
 *
 *  @param Gunx is used to define the Gun number
 *  @param value is the value that we set for the status led
 *
 *  @return None.
 */
void StatusLED(GunNumberType Gunx,PowerEnable value)
{
    GeneralGPIO(DC1_STS_LED_MCU,DC2_STS_LED_MCU,Gunx,value);
}
/**
 *  StopLED
 *
 *  @brief It is used to set the StopLED value.
 *
 *  This function is used to set the power values for the StopLED.
 *
 *  @param Gunx is used to define the Gun number
 *  @param value is the value that we set for the StopLED
 *
 *  @return None.
 */
void StopLED(GunNumberType Gunx,PowerEnable value)
{
    GeneralGPIO(DC1_STP_LED_MCU,DC2_STP_LED_MCU,Gunx,value);
}
/**
 *  CoolingFan
 *
 *  @brief It is used to set the CoolingFan value.
 *
 *  This function is used to set the power values for the CoolingFan.
 *
 *  @param value is the value that we set for the CoolingFan
 *
 *  @return None.
 */
void CoolingFan(PowerEnable value)  // 1 colling Fan
{
    WriteDGPIO(RAD_EN_MCU,value);
}
/**
 *  StationLED
 *
 *  @brief It is used to set the StationLED value.
 *
 *  This function is used to set the power values for the StationLED.
 *
 *  @param Gunx is used to define the Gun number
 *  @param value is the value that we set for the StationLED
 *
 *  @return None.
 */
void StationLED(PowerEnable value)
{
    WriteDGPIO(STATION_LED,value);
}
/**
 *  ExhaustFan
 *
 *  @brief It is used to set the ExhaustFan value.
 *
 *  This function is used to set the power values for the ExhaustFan.
 *
 *  @param value is the value that we set for the ExhaustFan
 *
 *  @return None.
 */
void ExhaustFan(PowerEnable value)
{
    WriteDGPIO(EXHAUST_FAN,value);
}
/**
 *  PumpPower
 *
 *  @brief It is used to set the PumpPower value.
 *
 *  This function is used to set the power values for the PumpPower.
 *
 *  @param value is the value that we set for the PumpPower
 *
 *  @return None.
 */
void PumpPower(PowerEnable value)
{
    WriteDGPIO(PUMP_POWER_ENABLE,value);
}
/**
 *  StopChargingGun
 *
 *  @brief It is used to set the stop charge value of the respective gun.
 *
 *  This function is used to set the stop charge values for the gun using 'StopCharging' function
 *
 *  @param RGSC,LGSC are the values of the right and left gun stop charge values respectively.
 *  @param  RNLRightMSGCTRL,RNLLeftMSGCTRL are the enum that points to the message flag triggered for stop charge
 *  @param RNLRightGunSend,RNLLeftGunSend are the pointers to the SendToRNLStruct structure that sends the messages to RNL
 *
 *  @return None.
 */
void StopChargingGun(bool RGSC,bool LGSC,RNLMSGCTRLEnum *RNLRightMSGCTRL,SendToRNLStruct *RNLRightGunSend,RNLMSGCTRLEnum *RNLLeftMSGCTRL,SendToRNLStruct *RNLLeftGunSend)
{
    if (RGSC == 1)
    {
        StopCharging(Gun1,RNLRightGunSend,RNLRightMSGCTRL);
    }
    if (LGSC ==1)
    {
        StopCharging(Gun2,RNLLeftGunSend,RNLLeftMSGCTRL);
    }
}
/**
 *  GunFault
 *
 *  @brief It is used to set the gun fault value.
 *
 *  This function is used to set the fault values of the gun for permanent and isolation fault .
 *
 *  @param Gun is the value of the pointer to the gun struct
 *
 *  @return None.
 */
void GunFault(GunStruct *Gun)
{
    // PERMANENT FAULT
    if (Gun->NegativeContactorFault == 1 || Gun->PositiveContactorFault == 1)
    {
        Gun->ParmanantFault =  1;
    }
    // ISOLATION FAULT
    if(Gun->IMDResFault == 1)// Changes made to tell vehical that IMD Fault without Gunstate
    {
        Gun->IsolationState =  ISOLATION_FAULT;
    }
    else
    {
        Gun->IsolationState =  ISOLATION_INVALID;

     }
}
/**
 *  Charger_FastChargeMSG
 *
 *  @brief It is used to set the gun struct values based on fast charging.
 *
 *  This function is used to set the gun struct values for the primary gun in fast charging .
 *
 *  @param PriGun is the value of the pointer to the gun struct
 *  @param SecGun is the value of the pointer to the gun struct
 *
 *  @return None.
 */
void Charger_FastChargeMSG(GunStruct *PriGun, GunStruct *SecGun)
{
    PriGun->DrivenVoltage = PriGun->RectDriveVoltage * (uint16_t)(PriGun->NegativeContactorDrive);
PriGun->DrivenCurrent = ((PriGun->RectDriveCurrent +   SecGun->RectDriveCurrent) * RectifierPerGun) * (uint16_t)(PriGun->NegativeContactorDrive);
SecGun->RectState = PriGun->RectState;
SecGun->RectDriveVoltage = PriGun->RectDriveVoltage;
if(PriGun->PowerReallocFlag ==  0)
    SecGun->RectDriveCurrent = PriGun->RectDriveCurrent;
else
    SecGun->RectDriveCurrent = 0;

PriGun->VseccMeasuredVoltage = PriGun->MeasuredVoltage * (uint16_t)(PriGun->NegativeContactorDrive);
PriGun->VseccMeasuredCurrent = (PriGun->TotalMeasuredCurrent +  SecGun->TotalMeasuredCurrent)* (uint16_t)(PriGun->NegativeContactorDrive);

  PriGun->VseccMinVoltage = ChargerMinVoltage;
  PriGun->VseccMinCurrent = ChargerMinCurrent;
  PriGun->VseccMinPower = ChargerMinPower;
  PriGun->VseccMaxVoltage = ChargerMaxVoltage;
  PriGun->VseccMaxCurrent = ChargerMaxCurrent;


  SecGun->PositiveContactorDrive = Off;
  SecGun->NegativeContactorDrive = Off;
  SecGun->PrechargeContactorDrive = Off;

  SecGun->VseccMeasuredCurrent = 0.0;
  SecGun->VseccMeasuredVoltage = 0.0;
  SecGun->DrivenVoltage = 0;
  SecGun->DrivenCurrent = 0;


  SecGun->VseccMinVoltage = 0;
  SecGun->VseccMinCurrent = 0;
  SecGun->VseccMinPower = 0;
  SecGun->VseccMaxVoltage = 0;
  SecGun->VseccMaxCurrent = 0;}
/**
 *  Charger_SlowChargeMsg
 *
 *  @brief It is used to set the gun struct values based on slow charging.
 *
 *  This function is used to set the gun struct values for the primary gun in slow charging .
 *
 *  @param PriGun is the value of the pointer to the gun struct
 *
 *  @return None.
 */
void Charger_SlowChargeMsg(GunStruct *PriGun)
{
  PriGun->DrivenVoltage =( PriGun->RectDriveVoltage* (uint16_t)(PriGun->NegativeContactorDrive));
  PriGun->DrivenCurrent = (RectifierPerGun * PriGun->RectDriveCurrent* (uint16_t)(PriGun->NegativeContactorDrive));
  PriGun->VseccMeasuredVoltage = (PriGun->MeasuredVoltage* (uint16_t)(PriGun->NegativeContactorDrive));
  PriGun->VseccMeasuredCurrent = (PriGun->TotalMeasuredCurrent* (uint16_t)(PriGun->NegativeContactorDrive));

  PriGun->VseccMinVoltage = ChargerMinVoltage;
  PriGun->VseccMinCurrent = ChargerMinCurrent;
  PriGun->VseccMinPower = ChargerMinPower;
  PriGun->VseccMaxVoltage = ChargerMaxVoltage;
  PriGun->VseccMaxCurrent = ChargerMaxCurrent;
}
/**
 *  BootISR
 *
 *  @brief It is used to update firmware hex file via OTA.
 *
 *  This function acknowledges the CAN message then jumps to the application via soft reset for firmware update .
 *
 *  @return None.
 */

void BootISR(void)
{
    EALLOW;//ENABLE PROTECTED REGISTERS

    Fapi_FlashStatusType oFlashStatus;
    Fapi_StatusType oReturnCheck;

    Flash_disablePrefetch(FLASH0CTRL_BASE);
    FLASH_DELAY_CONFIG;
    oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, DEVICE_SYSCLK_FREQ/1000000);
    while(Fapi_checkFsmForReady() != Fapi_Status_FsmReady);
    if(oReturnCheck != Fapi_Status_Success){}
    oReturnCheck =  Fapi_setActiveFlashBank(Fapi_FlashBank0);
    if(oReturnCheck != Fapi_Status_Success)
    {
        //send can message it failed and returned
    }

     uint16_t data2[1] = {0x1111};
    uint32_t firmwareUpdateAddress = 0x00084FEC;
     Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32_t *)firmwareUpdateAddress);
     while(Fapi_checkFsmForReady() != Fapi_Status_FsmReady);
     if(oReturnCheck != Fapi_Status_Success){}
     oFlashStatus = Fapi_getFsmStatus();
     if(oFlashStatus != Fapi_Status_Success){}
     Fapi_issueProgrammingCommand((uint32_t *)firmwareUpdateAddress,(uint16_t *)data2,1,0,0,Fapi_AutoEccGeneration);
     while(Fapi_checkFsmForReady() != Fapi_Status_FsmReady);
     if(oReturnCheck != Fapi_Status_Success)
     {
         //send can message it failed and returned
     }

     oFlashStatus = Fapi_getFsmStatus();
     if(oFlashStatus != Fapi_Status_Success)
     {
         //send can message it failed and returned
     }
     EDIS;
     DEVICE_DELAY_US(2000000);//OTA DELAY
     CAN_setupMessageObject(CANA_BASE, 30, 0x12EF34AA,
                          CAN_MSG_FRAME_EXT, CAN_MSG_OBJ_TYPE_TX, 0, 0, 8);
     uint8_t BL_IN_APP[8]    =      {0, 0, 0, 0, 0, 0, 0, 0};// MCU IS in APPLICATION
     CAN_sendMessage(CANA_BASE, 30, 8,(uint16_t *)BL_IN_APP);
     DEVICE_DELAY_US(40000);//OTA DELAY FOR MCU IS IN BL
     SysCtl_setWatchdogMode(SYSCTL_WD_MODE_RESET);
     SysCtl_resetDevice();
}
void SendRNLMessagePECCStatus1(GunStruct *Gun,PeccStatus1 PS1,uint8_t GunNumber)
{
    PS1.PECCStatus1_t.contactorsStatus = Gun->ContactorState;
    PS1.PECCStatus1_t.drivenCurrent = (uint16_t)Gun->DrivenCurrent;
    PS1.PECCStatus1_t.drivenVoltage = (uint16_t)Gun->DrivenVoltage;
    PS1.PECCStatus1_t.isolationStatus = Gun->IsolationState;
    PS1.PECCStatus1_t.operationalStatus = Gun->PECCOperationState;
    PS1.PECCStatus1_t.temperature = (uint16_t)Gun->TempratureToVsecc;
    PS1.PECCStatus1_t.reserved1 = 0;
    PS1.PECCStatus1_t.reserved2 = 0;

    if(GunNumber == 1)
    {
        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_VEHICLE_STATUS, CAN_PECCSTATUS1_RIGHT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_VEHICLE_STATUS, (uint64_t *)&PS1.Pecc1 ,8);

    }
    else if (GunNumber == 2)
    {

        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_VEHICLE_STATUS, CAN_PECCSTATUS1_LEFT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_VEHICLE_STATUS, (uint64_t *)&PS1.Pecc1 ,8);
    }
}
void SendRNLMessageStopCharge(GunStruct *Gun,uint8_t GunNumber)
{
    if(GunNumber == 1)
    {
        if(Gun->StopCharge == 1)
        {
            uint64_t send_null = 0;
        CANMSGObjectSetup(RECTIFIER_GUN1, STOPCHARGE, CAN_STOPCHARGE_RIGHT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, STOPCHARGE, (uint64_t*)&send_null ,8);
        }

    }
    else if (GunNumber == 2)
    {
        if(Gun->StopCharge == 1)
        {
            uint64_t send_null = 0;
        CANMSGObjectSetup(RECTIFIER_GUN1, STOPCHARGE, CAN_STOPCHARGE_LEFT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, STOPCHARGE, (uint64_t*)&send_null,8);
        }
    }
}
void SendRNLMessageVehicleId(GunStruct *Gun,uint8_t GunNumber)
{
    if(GunNumber == 1)
    {

        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_VEHICLE_STATUS, RIGHT_VEHICLEID, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_VEHICLE_STATUS,(uint64_t *) &Gun->VehicleID ,8);


    }
    else if (GunNumber == 2)
    {

        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_VEHICLE_STATUS, LEFT_VEHICLEID, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_VEHICLE_STATUS,(uint64_t *) &Gun->VehicleID,8);

    }
}
void SendRNLMessagePECCStatus2(GunStruct *Gun1,GunStruct *Gun2,PeccStatus2 PS2,uint8_t GunNumber)
{
    if(Gun1->ChargingSpeed == FastCharging )
    {
    PS2.PECCStatus2_t.measuredCurrent = (uint16_t)((Gun1->MeasuredCurrent[0] + Gun1->MeasuredCurrent[1] + Gun1->MeasuredCurrent[2] + Gun2->MeasuredCurrent[0] + Gun2->MeasuredCurrent[1] + Gun2->MeasuredCurrent[2])*10);
    }
    else if(Gun1->ChargingSpeed == SlowChargeing)
    {
        PS2.PECCStatus2_t.measuredCurrent = (uint16_t)((Gun1->MeasuredCurrent[0] + Gun1->MeasuredCurrent[1] + Gun1->MeasuredCurrent[2])*10);
    }
    else if(Gun1->ChargingSpeed == NotCharging)
    {
        PS2.PECCStatus2_t.measuredCurrent = 0;
    }
    PS2.PECCStatus2_t.measuredVoltage = (uint16_t)((Gun1->MeasuredVoltage)*10);
    PS2.PECCStatus2_t.status = 0;
    PS2.PECCStatus2_t.reserve = 0;

    if(GunNumber == 1)
    {
        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_PECC2, CAN_PECCSTATUS2_RIGHT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_PECC2, (uint64_t *)&PS2.Pecc2 ,8);

    }
    else if (GunNumber == 2)
    {

        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_PECC2, CAN_PECCSTATUS2_LEFT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_PECC2, (uint64_t *)&PS2.Pecc2 ,8);
    }
}
void SendRNLMessagePECCLimits1(GunStruct *Gun,PeccLimits1 PL1,uint8_t GunNumber)
{
PL1.PECCLimits1_t.limitVoltageMax = (uint16_t)(Gun->VseccMaxVoltage / 0.1);
PL1.PECCLimits1_t.limitVoltageMin =(uint16_t) (Gun->VseccMinVoltage / 0.1);
PL1.PECCLimits1_t.limitPowerMax = (uint16_t) (Gun->VseccMaxPower / 10);
PL1.PECCLimits1_t.limitPowerMin = (uint16_t)(Gun->VseccMinPower /10);
    if(GunNumber == 1)
    {
        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_PECC2, CAN_PECCLIMITS1_RIGHT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_PECC2, (uint64_t *)&PL1.pecclimit1 ,8);

    }
    else if (GunNumber == 2)
    {

        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_PECC2, CAN_PECCLIMITS1_LEFT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_PECC2, (uint64_t *)&PL1.pecclimit1 ,8);
    }
}
void SendRNLMessagePECCLimits2(GunStruct *Gun,PeccLimits2 PL2,uint8_t GunNumber)
{
PL2.PECCLimits2_t.limitCurrentMax = (uint16_t)(Gun->VseccMaxCurrent * 10);
PL2.PECCLimits2_t.limitCurrentMin = (uint16_t)Gun->VseccMinCurrent;
    if(GunNumber == 1)
    {
        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_PECC2, CAN_PECCLIMITS2_RIGHT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_PECC2, (uint64_t *)&PL2.pecclimit2 ,8);

    }
    else if (GunNumber == 2)
    {

        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_PECC2, CAN_PECCLIMITS2_LEFT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_PECC2, (uint64_t *)&PL2.pecclimit2,8);
    }
}

void SendRNLMessageChargeSessionInfo1(GunStruct *Gun,ChargeSessionInfo1 CSI1,uint8_t GunNumber)
{
CSI1.ChargingSessionInfo1_t.chargingProfileMaxPowerLimit = (uint16_t)Gun->ChargingProfileMaxPowerLimit;
CSI1.ChargingSessionInfo1_t.timeToFullSoc =(uint16_t) Gun->TimeToFullSoC;
    if(GunNumber == 1)
    {
        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_PECC2, CAN_CHARGESESSION_INFO1_RIGHT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_PECC2, (uint64_t *)&CSI1.chargesessioninfo1 ,8);

    }
    else if (GunNumber == 2)
    {

        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_PECC2, CAN_CHARGESESSION_INFO1_LEFT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_PECC2, (uint64_t *)&CSI1.chargesessioninfo1,8);
    }
}

void SendRNLMessageChargeSessionInfo2(GunStruct *Gun,ChargeSessionInfo2 CSI2,uint8_t GunNumber)
{
CSI2.ChargingSessionInfo2_t.evMaxCurrent = (uint16_t)Gun->EVMaxCurrent;
CSI2.ChargingSessionInfo2_t.evMaxPower = (uint16_t)Gun->EVMaxPower;
CSI2.ChargingSessionInfo2_t.evMaxVoltage = (uint16_t)Gun->EVMaxVoltage;
    if(GunNumber == 1)
    {
        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_PECC2, CAN_CHARGESESSION_INFO2_RIGHT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_PECC2, (uint64_t *)&CSI2.chargingsessioninfo2 ,8);

    }
    else if (GunNumber == 2)
    {

        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_PECC2, CAN_CHARGESESSION_INFO2_LEFT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_PECC2, (uint64_t *)&CSI2.chargingsessioninfo2,8);
    }
}

void SendRNLMessageChargeSessionInfo3(GunStruct *Gun,ChargeSessionInfo3 CSI3,uint8_t GunNumber)
{
CSI3.ChargingSessionInfo3_t.chargeMode = (uint8_t)Gun->ChargeMode;
CSI3.ChargingSessionInfo3_t.evMinCurrent = (uint16_t)Gun->EVMinCurrent;
CSI3.ChargingSessionInfo3_t.evMinPower = (uint16_t)Gun->EVMinPower;
CSI3.ChargingSessionInfo3_t.evMinVoltage = (uint16_t)Gun->EVMinVoltage;
    if(GunNumber == 1)
    {
        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_PECC2, CAN_CHARGESESSION_INFO3_RIGHT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_PECC2, (uint64_t *)&CSI3.chargingsessioninfo3 ,8);

    }
    else if (GunNumber == 2)
    {

        CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_PECC2, CAN_CHARGESESSION_INFO3_LEFT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_PECC2, (uint64_t *)&CSI3.chargingsessioninfo3,8);
    }
}

void SendRNLMessageVehicleStatus(GunStruct *Gun,status Vehicle,uint8_t GunNumber)
{
    Vehicle.VehicleStatus_t.batteryStateOfCharge = Gun->Battery_State_Of_Charge;
    Vehicle.VehicleStatus_t.cableCheckVoltage = (uint16_t)Gun->CableCheckVoltage;
    Vehicle.VehicleStatus_t.chargingState = Gun->EVChargingState;
    Vehicle.VehicleStatus_t.evConnectionState = Gun->EVConnectionState;
    Vehicle.VehicleStatus_t.targetContactorsStatus = Gun->TargetContactorState;
    Vehicle.VehicleStatus_t.targetCurrent = Gun->TargetCurrent;
    Vehicle.VehicleStatus_t.targetVoltage = Gun->TargetVoltage;
    Vehicle.VehicleStatus_t.reserved1 = 0;
    Vehicle.VehicleStatus_t.reserved2 = 0;

    if(GunNumber == 1)
    {

        CANMSGObjectSetup(RECTIFIER_GUN1, PECC_STATUS1, CAN_CMS_RNLVERIFY, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, PECC_STATUS1, (uint64_t *)&Vehicle.VehicleStatus ,8);

    }
    else if (GunNumber == 2)
    {

        CANMSGObjectSetup(RECTIFIER_GUN1, PECC_STATUS1, CAN_CMS_RNLVERIFYLeft, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, PECC_STATUS1, (uint64_t *)&Vehicle.VehicleStatus ,8);
    }

}
void SendRNLMessageReset(GunStruct *Gun,uint8_t GunNumber)
{
    if(GunNumber == 1)
    {
        if(Gun->VehicleReset == 1)
        {
            uint64_t send_null = 0;
        CANMSGObjectSetup(RECTIFIER_GUN1, STOPCHARGE, CAN_RESET_RIGHT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, STOPCHARGE, (uint64_t*)&send_null ,8);
        }

    }
    else if (GunNumber == 2)
    {
        if(Gun->VehicleReset == 1)
        {
            uint64_t send_null = 0;
        CANMSGObjectSetup(RECTIFIER_GUN1, STOPCHARGE, CAN_RESET_LEFT, STDFrame, CANOBJECTTYPETX, 0,CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, STOPCHARGE, (uint64_t*)&send_null,8);
        }
    }
}
