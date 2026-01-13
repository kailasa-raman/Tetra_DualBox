/*
 * CMACAN.c
 *
 *  Created on: Apr 15, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

#include <CMSCAN.h>


/**
 *  CMSRXCANOBJSet
 *
 *  @brief It is used to set the CN receive message object.
 *
 *  This function is used to set the CAN message object for reception.
 *
 *  @param CAN_BASE is the CAN base address
 *
 *  @return None.
 */
void  CMSRXCANOBJSet(uint32_t CAN_BASE)
{
    CANMSGObjectSetup(CAN_BASE, CANOBJ_CMS_RX, CANOBJ_CMS_ID_BASE,
                      STDFrame, CANOBJECTTYPERX,MASKFORCMSCAN,
                      CAN_MSG_OBJ_RX_INT_ENABLE|CAN_MSG_OBJ_USE_ID_FILTER);
}

/**
 *  CMS_ProcessCANMsg
 *
 *  @brief It is used to process the values based on CMS can messages.
 *
 *  This function is used to process the CMS can messages from the CAN and set values.
 *
 *  @param can_id is the can id from the IOT
 *  @param data is the pointer to the data from the received can id
 *  @param length is the length of the data
 *  @param receivedData is the pointer to the struct for CMSExternalReceiveStruct structure that stores the data in it
 *
 *  @return None.
 */

/**
 *  ProcessCMSPECCLimit1
 *
 *  @brief It is used to process the values based on CMS can messages.
 *
 *  This function is used to process the CMS can messages from the CAN and set values for PECC limit 1.
 *
 *  @param data is the pointer to the data from the received can id
 *  @param length is the length of the data
 *  @param receivedData is the pointer to the struct for CMSExternalReceiveStruct structure that stores the data in it
 *
 *  @return None.
 */

/**
 *  ProcessCMSPECCLimit2
 *
 *  @brief It is used to process the values based on CMS can messages.
 *
 *  This function is used to process the CMS can messages from the CAN and set values for PECC limit 2.
 *
 *  @param data is the pointer to the data from the received can id
 *  @param length is the length of the data
 *  @param receivedData is the pointer to the struct for CMSExternalReceiveStruct structure that stores the data in it
 *
 *  @return None.
 */

/**
 *  SendCMSTempCANData
 *
 *  @brief It is used to set the charge struct values.
 *
 *  This function is used to store the charge data vlaues form the gun struct data values.
 *
 *  @param ChargerData is the pointer to the ChargerStruct sturct that stores the values form the gun struct
 *  @param GunNumber1, GunNumber2 are the pointers to the struct for GunStruct structure
 *
 *  @return None.
 */
void SendCMSTempCANData(GunStruct *GunNumber1,GunStruct *GunNumber2,ChargerStruct *ChargerData)
{
    CANTEMPStruct CANTEMPS;
    CANTEMPS.TempValues.RightGunPositive = (uint16_t)GunNumber1->GunPosTemperature;   // 0th byter
    CANTEMPS.TempValues.RightGunNegetive = (uint16_t)GunNumber1->GunNegTemperature;
    CANTEMPS.TempValues.LeftGunPositive = (uint16_t)GunNumber2->GunPosTemperature;
    CANTEMPS.TempValues.LeftGunNegetive = (uint16_t)GunNumber2->GunNegTemperature;
    CANTEMPS.TempValues.BusBar1= (uint16_t)GunNumber1->BusBarTemperature;
    CANTEMPS.TempValues.BusBar2=(uint16_t)GunNumber2->BusBarTemperature;
    CANTEMPS.TempValues.EmbTemp= (uint16_t)ChargerData->EnvAmbTemp;
    CANTEMPS.TempValues.FluidTemp=(uint16_t)ChargerData->FluidTemperature;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, CAN_CMSTX_CHARGERTEMP,
                      STDFrame, CANOBJECTTYPETX, 0
                      , CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, &(CANTEMPS.AllTemprature), 8);
}
/**
 *  SendCMSDigitalInputCANData
 *
 *  @brief It is used to send digital input data via can.
 *
 *  This function is used to set the can message object and then send the digital input data via can.
 *
 *  @param MuxIO1,MuxIO2 are the values for the digital input
 *
 *  @return None.
 */
void SendCMSDigitalInputCANData(uint16_t MuxIO1, uint16_t MuxIO2)
{
    uint64_t tempData = 0;
    tempData =(((uint32_t)MuxIO2)<<16) +  MuxIO1;


    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, CAN_CMSTX_CHARGERDIO,
                          STDFrame, CANOBJECTTYPETX, 0
                          , CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, &tempData, 8);
}
/**
 *  SendACMeterVoltageInputCANData
 *
 *  @brief It is used to send AC meter voltage input data via can.
 *
 *  This function is used to set the can message object and then send the AC meter voltage input data via can.
 *
 *  @param ChargerData is the pointer to the ChargerStruct sturct that stores the values from the phase voltage
 *
 *  @return None.
 */
void SendACMeterVoltageInputCANData(ChargerStruct ChargerData)
{
    CANVoltageStruct CANVoltage;
    CANVoltage.IndividualVol.PhaseAVoltage = (ChargerData.PhaseVoltage[0]*10);
    CANVoltage.IndividualVol.PhaseBVoltage = (ChargerData.PhaseVoltage[1]*10);
    CANVoltage.IndividualVol.PhaseCVoltage = (ChargerData.PhaseVoltage[2]*10);
    CANVoltage.IndividualVol.rsvd =0;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSMODBUS_TX, CAN_CMSTX_ACLNVoltage,
                          STDFrame, CANOBJECTTYPETX, 0
                          , CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSMODBUS_TX, &(CANVoltage.ALLVoltageData), 8);
}
/**
 *  SendACMeterCurrentInputCANData
 *
 *  @brief It is used to send AC meter current input data via can.
 *
 *  This function is used to set the can message object and then send the AC meter current input data via can.
 *
 *  @param ChargerData is the pointer to the ChargerStruct struct that stores the values from the phase current
 *
 *  @return None.
 */
void SendACMeterCurrentInputCANData(ChargerStruct ChargerData)
{
    CANCurrentStruct CANCurrent;
    CANCurrent.IndividualCurrent.PhaseACurrent = (ChargerData.PhaseCurrent[0]*10);
    CANCurrent.IndividualCurrent.PhaseBCurrent = (ChargerData.PhaseCurrent[1]*10);
    CANCurrent.IndividualCurrent.PhaseCCurrent = (ChargerData.PhaseCurrent[2]*10);
    CANCurrent.IndividualCurrent.rsvd = 0;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSMODBUS_TX, CAN_CMSTX_ACCurrent,
                          STDFrame, CANOBJECTTYPETX, 0
                          , CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSMODBUS_TX, &(CANCurrent.ALLCurrentData), 8);
}
/**
 *  SendACMeterEnergyInputCANData
 *
 *  @brief It is used to send AC meter energy input data via can.
 *
 *  This function is used to set the can message object and then send the AC meter energy input data via can.
 *
 *  @param ChargerData is the pointer to the ChargerStruct struct that stores the values from the phase energy
 *
 *  @return None.
 */
void SendACMeterEnergyInputCANData(ChargerStruct ChargerData)
{
    CANLLVoltageStruct CANVoltage;
    CANVoltage.IndividualEng.PhaseAEnergy = (ChargerData.PhaseEnergy[0]*10);
    CANVoltage.IndividualEng.PhaseBEnergy = (ChargerData.PhaseEnergy[1]*10);
    CANVoltage.IndividualEng.PhaseCEnergy = (ChargerData.PhaseEnergy[2]*10);
    CANVoltage.IndividualEng.rsvd =0;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSMODBUS_TX, CAN_CMSTX_ACENERGY,
                          STDFrame, CANOBJECTTYPETX, 0
                          , CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSMODBUS_TX, &(CANVoltage.ALL_Energy), 8);
}
/**
 *  SendACMeterPowerInputCANData
 *
 *  @brief It is used to send AC meter power input data via can.
 *
 *  This function is used to set the can message object and then send the AC meter power input data via can.
 *
 *  @param ChargerData is the pointer to the ChargerStruct struct that stores the values from the phase power
 *
 *  @return None.
 */
void SendACMeterPowerInputCANData(ChargerStruct ChargerData)
{
    CANPowerStruct CANPower;
    CANPower.IndividualPow.PhaseAActivePower = (ChargerData.PhasePower[0]*10);
    CANPower.IndividualPow.PhaseBActivePower = (ChargerData.PhasePower[1]*10);
    CANPower.IndividualPow.PhaseCActivePower = (ChargerData.PhasePower[2]*10);
    CANPower.IndividualPow.rsvd =0;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSMODBUS_TX, CAN_CMSTX_ACPower,
                          STDFrame, CANOBJECTTYPETX, 0
                          , CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSMODBUS_TX, &(CANPower.ALLActivePower), 8);
}
/**
 *  SendACMeterInputCANData
 *
 *  @brief It is used to send AC meter power factor and frequency input data via can.
 *
 *  This function is used to set the can message object and then send the AC meter power factor and frequency input data via can.
 *
 *  @param AC_EnergyMeterDataStruct is the AC_EnergyMeterDataStruct struct that tells us the total active power
 *  @param ChargerData is the pointer to the ChargerStruct struct that has power factor frequency and total active energy
 *
 *  @return None.
 */
void SendACMeterInputCANData(AC_EnergyMeterDataStruct ACValues, ChargerStruct ChargerData)
{
    CANTPHzPfStruct CANdata;
    CANdata.WHzPfData.TotalActivePower = (ACValues.TotalActivePower*10);
    CANdata.WHzPfData.Frequency = (ChargerData.frequency*10);
    CANdata.WHzPfData.PowerFactor = (ChargerData.PowerFactor*10);
    CANdata.WHzPfData.TotalActiveEnergy = (ChargerData.TotalActiveEnergy*10);
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSMODBUS_TX, CAN_CMSTX_ACMeter,
                          STDFrame, CANOBJECTTYPETX, 0
                          , CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSMODBUS_TX, &(CANdata.ALLTP_Hz_PFData), 8);
}
/**
 *  SendDCMeterVoltAmpCANData
 *
 *  @brief It is used to send DC meter voltage and current data via can.
 *
 *  This function is used to set the can message object and then send the DC meter voltage and current data via can.
 *
 *  @param DCValues is the DC_EnergyMeterDataStruct struct that tells us the DC voltage and current
 *  @param value tells us the which object id we should select based on left or right gun
 *
 *  @return None.
 */
void SendDCMeterVoltAmpCANData(DC_EnergyMeterDataStruct DCValues, bool value)
{
    uint16_t objid;
    CANVoltAmpStruct CANdata;
    CANdata.MeterData.voltage = (DCValues.Voltage*10);
    CANdata.MeterData.current = (DCValues.Current*10);
    if(value == 0)
        objid = CAN_CMSTX_DCRIGHTVoltAmp;
    else
        objid = CAN_CMSTX_DCLEFTVoltAmp;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, objid, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, (uint64_t *)&CANdata.ALL_VoltAmpData,8);
}
/**
 *  SendDCMeterPowerCANData
 *
 *  @brief It is used to send DC meter power data via can.
 *
 *  This function is used to set the can message object and then send the DC meter power data via can.
 *
 *  @param DCValues,DCValues2 are the DC_EnergyMeterDataStruct structs that tells us the DC power
 *
 *  @return None.
 */
void SendDCMeterPowerCANData(DC_EnergyMeterDataStruct DCValues, DC_EnergyMeterDataStruct DCValues2)
{
    CANDCPowerStruct CANdata;
    CANdata.power.right_power = (DCValues.Power*1000);
    CANdata.power.left_power = (DCValues2.Power*1000);;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, CAN_CMSTX_DCPower, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, (uint64_t *)&CANdata.ALL_power,8);
}
/**
 *  SendDCMeterPosEnergyCANData
 *
 *  @brief It is used to send DC meter energy data via can.
 *
 *  This function is used to set the can message object and then send the DC meter energy data via can.
 *
 *  @param RightGun,leftGun are the GunStruct structs that tells us the DC energy meter reading
 *
 *  @return None.
 */
void SendDCMeterPosEnergyCANData(GunStruct RightGun,GunStruct leftGun)
{
    CANDCEnergyStruct pos_energy;
    pos_energy.energy.right_pos = (RightGun.DCEMEnergyReading*1000);
    pos_energy.energy.left_pos = (leftGun.DCEMEnergyReading*1000);
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, CAN_CMSTX_DCPosEnergy, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, (uint64_t *)&pos_energy.ALL_Energy,8);
}
/**
 *  SENDDiffDCEMeter
 *
 *  @brief It is used to send DC meter Differential energy data via can.
 *
 *  This function is used to set the can message object and then send the DC meter Differential energy data via can.
 *
 *  @param RightGun,leftGun are the GunStruct structs that tells us the DC Differential energy meter reading
 *
 *  @return None.
 */
void SENDDiffDCEMeter(GunStruct RightGun,GunStruct leftGun)
{
    CANDCEnergyDiffStruct EnergyDiff;
    EnergyDiff.energy.right_pos = (RightGun.DiffEnergy*1000);
    EnergyDiff.energy.left_pos = (leftGun.DiffEnergy*1000);
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, CAN_CMSTX_DCDiffEnergy, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, (uint64_t *)&EnergyDiff.ALL_Energy,8);
}
/**
 *  SendIMDMeterCANData
 *
 *  @brief It is used to send IMD data via can.
 *
 *  This function is used to set the can message object and then send the IMD data via can.
 *
 *  @param IMDdata,IMDdata2 are the variables that has the IMD value to be sent via CAN
 *
 *  @return None.
 */
void SendIMDMeterCANData(float IMDdata,float IMDdata2)
{
    CANIMDStruct CANdata;
    CANdata.IMD.Right_IMD = IMDdata;
    CANdata.IMD.Left_IMD = IMDdata2;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, CAN_CMSTX_IMD, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, (uint64_t *)&CANdata.ALL_IMD ,8);
}
/**
 *  CMSTempSensor
 *
 *  @brief It is used to read the temperature sensor values.
 *
 *  This function is used to read the temperature sensor values using ADC.
 *
 *  @return temperature value in Integer.
 */
int CMSTempSensor()
{
    uint16_t sensorSample;
    float sensorTemp;
    // asysctl initialization
    // Enables the temperature sensor output to the ADC.
    ASysCtl_enableTemperatureSensor();
    DEVICE_DELAY_US(500);

    sensorSample = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER9);
    sensorTemp = ADC_getTemperatureC(sensorSample, 3.3f);

    ADC_setVREF(MCUTEMP_ADC_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setPrescaler(MCUTEMP_ADC_BASE, ADC_CLK_DIV_2_0);
    ADC_enableConverter(MCUTEMP_ADC_BASE);
    DEVICE_DELAY_US(5000);

    ADC_setupSOC(MCUTEMP_ADC_BASE, ADC_SOC_NUMBER9, ADC_TRIGGER_SW_ONLY,
                 ADC_CH_ADCIN14, 20U);
    ADC_forceSOC(MCUTEMP_ADC_BASE, ADC_SOC_NUMBER9);
    DEVICE_DELAY_US(50000);

    return sensorTemp;
}
/**
 *  SendCANStopChargeLED
 *
 *  @brief It is used to stop the stop charge led.
 *
 *  This function is used to stop the led for both the guns.
 *
 *  @param gun1,gun2 are the variables that has the stop charge value for both the guns to be sent via CAN
 *
 *  @return None.
 */
void SendCANStopChargeLED(bool gun1, bool gun2)         //  Need correction
{
    uint8_t data[2];
    data[0]=gun1;
    data[1]=gun2;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSIOT_TX, CAN_CMS_TXStopCharge, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSIOT_TX, (uint64_t *)&data ,8);
}
/**
 *  CMSRXCANOBJState
 *
 *  @brief It is used to set the can object for reception.
 *
 *  This function is used to set the can object for reception.
 *
 *  @param CAN_BASE is the base of the can module that has to used for object setup.
 *
 *  @return None.
 */
void  CMSRXCANOBJState(uint32_t CAN_BASE)
{
    CANMSGObjectSetup(CAN_BASE, CANOBJ_CMS_RX, CAN_RX_State,
                      STDFrame, CANOBJECTTYPERX,MASKFORCMSCAN,
                      CAN_MSG_OBJ_RX_INT_ENABLE|CAN_MSG_OBJ_USE_ID_FILTER);
}
/**
 *  CMS_ProcessState
 *
 *  @brief It is used to process the values and put the data in can state struct .
 *
 *  This function is used to process the CMS can message from the CAN and put the data in can state struct .
 *
 *  @param can_id is the can id from the IOT
 *  @param data is the pointer to the data from the received can id
 *  @param length is the length of the data
 *  @param states is the pointer to the struct for CANStateStruct structure that stores the state data in it
 *
 *  @return None.
 */
void  CMS_ProcessState(uint32_t can_id, uint8_t *data, uint8_t length, CANStateStruct *states)
{
    uint64_t temp = 0;
    uint16_t *byteData = (uint16_t *)data;
    uint8_t i = 0;
    if (states == NULL || data == NULL) return;  // Null check
        uint32_t MSGBase =  can_id & CAN_RX_State;

        if (MSGBase == CAN_RX_State)
        {
                // Convert raw bytes into a 64-bit integer
                for (i = 0; i < length; i++)
                {
                     temp |= ((uint64_t)(byteData[i] & 0x00FF) << (8 * i));
                }

                states->all_states.state =
                        (temp >> 0) & 0xFFFF;
                states->all_states.leftstate =
                        (temp >> 16) & 0xFFFF;
                states->all_states.rightState =
                        (temp >> 32) & 0xFFFF;

        }
}
/**
 *  SendCMSFlowrate
 *
 *  @brief It is used to set the flow rate for the guns.
 *
 *  This function is used to set the flow rate received from the flow meter gpio using gpio interrupt .
 *
 *  @param right_flow,left_flow are the flow rate given by the rising edge of the gpio for both the guns.
 *
 *  @return None.
 */
void SendCMSFlowrate(float right_flow, float left_flow)
{
    CANFlowStruct CANdata;
    CANdata.Flow.right_flow = right_flow*100;
    CANdata.Flow.left_flow = left_flow*100;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, CAN_CMSTX_FLOW, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, (uint64_t *)&CANdata.BothFlow,8);
}
/**
 *  SendCMSVersionCANData
 *
 *  @brief It is used to send the version via CAN.
 *
 *  This function is used to send the version of the firmware via CAN .
 *
 *  @return None.
 */
void SendCMSVersionCANData()
{
    CANVersionStruct CANdata;
    CANdata.Individualversion.ChargerRating = 12;
    CANdata.Individualversion.BaseVersion = 1;
    CANdata.Individualversion.MatlabVersion = 1;
    CANdata.Individualversion.ChangeNumber = 7;

    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSIOT_TX, CAN_CMS_VERSION, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSIOT_TX,
                   (uint64_t*) &CANdata.versions,8);
}
/**
 *  SendFaultCANData
 *
 *  @brief It is used to send the fault data message via CAN.
 *
 *  This function is used to send the fault data message via can received form the gun structures,charger structure and modbus fault.
 *
 *  @param RightGun,LeftGun are the pointers to the struct for GunStruct structure
 *  @param ChargerData is the pointer to the struct for ChargerStruct structure
 *  @param ModBusFaultDdata is the pointers to the struct for ModbusFaultStruct structure
 *
 *  @return None.
 */
void SendFaultCANData(GunStruct RightGun,GunStruct LeftGun,ChargerStruct ChargerData,FaultAndCounterStruct ModBusFaultDdata)
{
    Charger_Left_Right_fault CANdata;
    CANdata.bitfaults.RightGunFault = RightGun.GunFault;
    CANdata.bitfaults.RightPositiveContactorFault = RightGun.PositiveContactorFault;
    CANdata.bitfaults.RightNegativeContactorFault = RightGun.NegativeContactorFault;
    CANdata.bitfaults.RightGunIMDFault = RightGun.GunIMDFault;
    CANdata.bitfaults.RightIMDResFault = RightGun.IMDResFault;
    CANdata.bitfaults.RightTempFault = RightGun.TempFault;
    CANdata.bitfaults.RightParmanantFault = RightGun.ParmanantFault;
    CANdata.bitfaults.RightGunErrorState =  RightGun.EVConnectionState ==  EV_ERROR;

    CANdata.bitfaults.LeftGunFault = LeftGun.GunFault;
    CANdata.bitfaults.LeftPositiveContactorFault = LeftGun.PositiveContactorFault;
    CANdata.bitfaults.LeftNegativeContactorFault = LeftGun.NegativeContactorFault;
    CANdata.bitfaults.LeftGunIMDFault = LeftGun.GunIMDFault;
    CANdata.bitfaults.LeftIMDResFault = LeftGun.IMDResFault;
    CANdata.bitfaults.LeftTempFault = LeftGun.TempFault;
    CANdata.bitfaults.LeftParmanantFault = LeftGun.ParmanantFault;
    CANdata.bitfaults.LeftGunErrorState =  LeftGun.EVConnectionState ==  EV_ERROR;


    CANdata.bitfaults.ChargerFault = ChargerData.ChargerFault;
    CANdata.bitfaults.VoltageBalanceFault = ChargerData.VoltageBalanceFault;
    CANdata.bitfaults.CurrentBalanceFault = ChargerData.CurrentBalanceFault;
    CANdata.bitfaults.VoltageLimitFault = ChargerData.VoltageLimitFault;
    CANdata.bitfaults.CurrentLimitFault = ChargerData.CurrentLimitFault;
    CANdata.bitfaults.FrequncyLimitFault = ChargerData.FrequncyLimitFault;
    CANdata.bitfaults.EMButtonFault = ChargerData.EMButtonFault;
    CANdata.bitfaults.SPDFault = ChargerData.SPDFault;
    CANdata.bitfaults.ELRFBFault = ChargerData.ELRFBFault;
    CANdata.bitfaults.ACContactorFault = ChargerData.ACContactorFault;
    CANdata.bitfaults.ChargerPermantFault = ChargerData.ParmanantFault;
    CANdata.bitfaults.ChargerTempFault = ChargerData.TempFault;
    CANdata.bitfaults.ChargerDIOFault =ChargerData.CMSDIOInFault;
    CANdata.bitfaults.CrossContactorFault = ChargerData.CrossContactorFault;
    CANdata.bitfaults.CrossContactorWeldFault =  ChargerData.CrossContactorWeldFault;


    CANdata.bitfaults.ACEMFault = ModBusFaultDdata.ACEMFault;
    CANdata.bitfaults.DCEM1Fault = ModBusFaultDdata.DCEM1Fault;
    CANdata.bitfaults.DCEM2Fault= ModBusFaultDdata.DCEM2Fault;
    CANdata.bitfaults.IMDM1Fault= ModBusFaultDdata.IMDM1Fault;
    CANdata.bitfaults.IMDM2Fault=ModBusFaultDdata.IMDM2Fault;

    CANdata.bitfaults.VSECCCyclic =0;
    CANdata.bitfaults.TonheLeftCyclic=0;
    CANdata.bitfaults.TonheRightCyclic=0;

    CANdata.bitfaults.TonheRight1Fault=0;
    CANdata.bitfaults.TonheRight2Fault=0;
    CANdata.bitfaults.TonheRight3Fault=0;

    CANdata.bitfaults.TonheLeftt1Fault=0;
    CANdata.bitfaults.TonheLeft2Fault=0;
    CANdata.bitfaults.TonheLeft3Fault=0;

    CANdata.bitfaults.Rsvd=0;
    CANdata.bitfaults.rsvd1=0;

    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, CAN_CMS_FAULTS, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, (uint64_t *)&CANdata.fault,8);
}
/**
 *  SendCMSModbusMeterStatus
 *
 *  @brief It is used to send the modbus meter status via CAN.
 *
 *  This function is used to send the modbus meter data message via can received from the ACEMModBusStatus structures,DCEMModBusStatusStruct structure and ModbusFaultStruct structures.
 *
 *  @param ACEMModBusStatus is the pointer to the struct for ACEMModBusStatusStruct structure
 *  @param DCEMModBusStatus1,DCEMModBusStatus2 are the pointers to the struct for DCEMModBusStatusStruct structure
 *  @param IMDStatus1,IMDStatus2 are the pointers to the struct for ModbusFaultStruct structure
 *
 *  @return None.
 */
void SendCMSModbusMeterStatus(ACEMModBusStatusStruct *ACEMModBusStatus,DCEMModBusStatusStruct *DCEMModBusStatus1,DCEMModBusStatusStruct *DCEMModBusStatus2,ModbusCommStatus *IMDStatus1,ModbusCommStatus *IMDStatus2)
{
    uint64_t tempModStatus = 0;
    tempModStatus = ( ((uint64_t)(ACEMModBusStatus->ACEMModBusStatusVal))|(((uint64_t)(DCEMModBusStatus1->DCEMModBusStatusVal)) << 16) |(((uint64_t)(DCEMModBusStatus2->DCEMModBusStatusVal )<< 32)) | (((uint64_t)(*IMDStatus1) << 48)) | ((uint64_t)(*IMDStatus1) << 50));
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSCOMSTATUS_TX, CAN_CMS_ACENERGYMETER_STATUS, STDFrame,
                          CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
        CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSCOMSTATUS_TX,&tempModStatus,8);

}
/**
 *  SendCMS_TonheStatus
 *
 *  @brief It is used to send the VSECC status via CAN.
 *
 *  This function is used to send the Tonhe status that is received from the Tonhe status structure.
 *
 *  @param Tonhe_Status is the pointer to the struct for RectifierStatus_t structure
 *
 *  @return None.
 */
void SendCMS_TonheStatus(RectifierStatus_t Tonhe_Status)
{
    uint8_t RectWorkingStatus[8] = {0};
    uint8_t CANARectifiers = RectWorkingStatus[1]+RectWorkingStatus[2]+RectWorkingStatus[3];
    uint8_t CANBRectifiers = RectWorkingStatus[5]+RectWorkingStatus[6]+RectWorkingStatus[7];
    CANTonheStatus CANdata;
    CANdata.bitsStatus.Tonhe_RightRectifier_1 = Tonhe_Status.Tonhe_RightRectifier_1;
    CANdata.bitsStatus.Tonhe_RightRectifier_2 = Tonhe_Status.Tonhe_RightRectifier_2;
    CANdata.bitsStatus.Tonhe_RightRectifier_3 = Tonhe_Status.Tonhe_RightRectifier_3;
    CANdata.bitsStatus.Tonhe_LeftRectifier_1 = Tonhe_Status.Tonhe_LeftRectifier_1;
    CANdata.bitsStatus.Tonhe_LeftRectifier_2 = Tonhe_Status.Tonhe_LeftRectifier_2;
    CANdata.bitsStatus.Tonhe_LeftRectifier_3 = Tonhe_Status.Tonhe_LeftRectifier_3;
    CANdata.bitsStatus.Gun1_Rectifiers = CANARectifiers;
    CANdata.bitsStatus.Gun2_Rectifiers = CANBRectifiers;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSCOMSTATUS_TX, CAN_CMS_TONHESTATUS, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSCOMSTATUS_TX, (uint64_t *)&CANdata.TonheStatus,8);
}
/**
 *  SendCMSIOTStartStop
 *
 *  @brief It is used to send the IOT start or stop message via CAN.
 *
 *  This function is used to send the IOT start or stop message from the gun structure data .
 *
 *  @param RightGun,LeftGun are the pointers to the struct for GunStruct structure
 *
 *  @return None.
 */
void SendCMSIOTStartStop(GunStruct RightGun, GunStruct LeftGun)
{
    CANIOTStatus_t CANdata;
    CANdata.IOTStatus.Right_IOTStop = RightGun.IOTstopCharge;
    CANdata.IOTStatus.Right_IOTStart = RightGun.IOTstartCharge;
    CANdata.IOTStatus.Left_IOTStop = LeftGun.IOTstopCharge;
    CANdata.IOTStatus.Left_IOTStart = LeftGun.IOTstartCharge;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSIOT_TX, CAN_CMS_IOTSTARTSTOP, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSCOMSTATUS_TX, (uint64_t *)&CANdata.BothIOTStatus,8);
}
/**
 *  SendCMSI2CStatus
 *
 *  @brief It is used to send the I2C status message via CAN.
 *
 *  This function is used to send the I2C status message from the I2C status and DIO status via CAN .
 *
 *  @param DIOStatus is the status of the DIO as parameter
 *  @param I2Cstatus is the pointer to the I2C status
 *
 *  @return None.
 */
void SendCMSI2CStatus(uint8_t DIOStatus, uint8_t *I2Cstatus)
{
    DIOI2CCheckStatus_t CANData;
    CANData.DIOI2Cstatus.DIOStatus = DIOStatus;
    CANData.DIOI2Cstatus.I2C1Status = I2Cstatus[0];
    CANData.DIOI2Cstatus.I2C2Status = I2Cstatus[1];
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSIOT_TX, CAN_CMS_I2CSTATUS, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSIOT_TX, (uint64_t *)&CANData.I2Cstatus,8);
}
/**
 *  SendGunData
 *
 *  @brief It is used to send all the gun data via CAN.
 *
 *  This function is used to send the gun data from the gun structures and the charger structure via CAN .
 *
 *  @param RightGun, LeftGun are the GunStruct structures
 *  @param ChargerDataX is the charger structure data
 *
 *  @return None.
 */
void SendGunData(GunStruct RightGun,GunStruct LeftGun,ChargerStruct ChargerDataX)
{
    GunDataStatus_t CANData;
    CANData.GunData.RightChargingSpeed = RightGun.ChargingSpeed;
    CANData.GunData.LeftChargingSpeed = LeftGun.ChargingSpeed;
    CANData.GunData.RightPositiveContactorDrive = RightGun.PositiveContactorDrive;
    CANData.GunData.RightNegativeContactorDrive = RightGun.NegativeContactorDrive;
    CANData.GunData.RightPrechargeContactorDrive = RightGun.PrechargeContactorDrive;

    CANData.GunData.RightPowerLED = RightGun.PowerLED;
    CANData.GunData.RightStopChargeLED = RightGun.StopChargeLED;
    CANData.GunData.RightStopCharge = RightGun.StopCharge;
    CANData.GunData.RightStartCharge = RightGun.StartCharge;
    CANData.GunData.RightStatusLED = RightGun.StatusLED;
    CANData.GunData.RightGunIOTStartCharge=RightGun.IOTstartCharge;
    CANData.GunData.RightGunIOTStopCharge=RightGun.IOTstopCharge;
    CANData.GunData.RightGunIMDEnable =  RightGun.IMDEnable;
    CANData.GunData.RightGunVSECCReset =  RightGun.ResetRquest;


    CANData.GunData.LeftPositiveContactorDrive = LeftGun.PositiveContactorDrive;
    CANData.GunData.LeftNegativeContactorDrive = LeftGun.NegativeContactorDrive;
    CANData.GunData.LeftPrechargeContactorDrive = LeftGun.PrechargeContactorDrive;

    CANData.GunData.LeftPowerLED = LeftGun.PowerLED;
    CANData.GunData.LeftStopChargeLED = LeftGun.StopChargeLED;
    CANData.GunData.LeftStopCharge = LeftGun.StopCharge;
    CANData.GunData.LeftStartCharge = LeftGun.StartCharge;
    CANData.GunData.LeftStatusLED = LeftGun.StatusLED;

    CANData.GunData.LeftIOTStartCharge=LeftGun.IOTstartCharge;
    CANData.GunData.LeftIOTStopCharge=LeftGun.IOTstopCharge;
    CANData.GunData.LeftGunIMDEnable =  LeftGun.IMDEnable;
    CANData.GunData.LeftGunVSECCReset =  LeftGun.ResetRquest;


    CANData.GunData.ACContactorDrive =  ChargerDataX.ACContactorDrive;
    CANData.GunData.CrossContactorDrive = ChargerDataX.CrossContactorDrive;
    CANData.GunData.ExhuastFanControl =  ChargerDataX.ExhuastFanControl;
    CANData.GunData.CoolingFanControl =  ChargerDataX.CoolingFanControl;
    CANData.GunData.CoolingPumpPowerControl = ChargerDataX.CoolingPumpPowerControl;


    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, CAN_CMS_GUNDATA, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, (uint64_t *)&CANData.CANData,8);
}
/**
 *  SendCMS_TotalMeasuredAmpVol
 *
 *  @brief It is used to send the measured voltage and current via CAN.
 *
 *  This function is used to send the measured voltage and current data from the gun structures via CAN .
 *
 *  @param RightGun, LeftGun are the GunStruct structures
 *
 *  @return None.
 */
void SendCMS_TotalMeasuredAmpVol(GunStruct RightGun,GunStruct LeftGun)
{
    TotalMeasureVOLAMP_t CANDATA ;
    CANDATA.TotalAMPVol = 0;
    CANDATA.VOLAMP.TotalMeasuredVoltageR = (uint16_t)RightGun.TotalMeasuredVoltage;
    CANDATA.VOLAMP.TotalMeasuredCurrentR = (uint16_t)RightGun.TotalMeasuredCurrent;
    CANDATA.VOLAMP.TotalMeasuredVoltageL = (uint16_t)LeftGun.TotalMeasuredVoltage;
    CANDATA.VOLAMP.TotalMeasuredCurrentL = (uint16_t)LeftGun.TotalMeasuredCurrent;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, CAN_CMS_MeasureData, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX,&CANDATA.TotalAMPVol,8);
}
/**
 *  SendGUNStatus
 *
 *  @brief It is used to send the current gun status via CAN.
 *
 *  This function is used to send the gun status, DIO count and Tonhe left and right gun values via CAN .
 *
 *  @param RGGS, LGGS are the variables for the gun status for both the left and right gun's
 *  @param TR, TL are the variables for the Tonhe count for both the left and right gun's
 *  @param DioCOunt is the variable for the DIO count
 *
 *  @return None.
 */
void SendGUNStatus(uint8_t RGGS,uint8_t LGGS,uint8_t TR,uint8_t TL,uint16_t DioCOunt)
{
    BothGunState Value;
    Value.Gunstatus.RGState =  RGGS & 0xFF;
    Value.Gunstatus.LGState =  LGGS & 0xFF;
    Value.Gunstatus.TonheLCount = TL & 0xFF;
    Value.Gunstatus.TonheRCount = TR & 0xFF;
    Value.Gunstatus.DIOCOunt = DioCOunt;
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBj_CMSGUNSTATUS_TX, CAN_CMS_Gunstate, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBj_CMSGUNSTATUS_TX, (uint64_t *)&Value.RawValue,8);
}
/**
 *  SendVehicleIDRight
 *
 *  @brief It is used to send the vehicle ID via CAN.
 *
 *  This function is used to send the vehicle ID from Right Gun via CAN .
 *
 *  @param VehicleID is the Right gun connected vehicle's vehicle ID
 *
 *  @return None.
 */
void SendVehicleIDRight(uint64_t VehicleID)
{
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSIOT_TX, CAN_RIGHT_VEHICLEID, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSIOT_TX, (uint64_t *)&VehicleID,6);
}
/**
 *  SendVehicleIDLeft
 *
 *  @brief It is used to send the vehicle ID via CAN.
 *
 *  This function is used to send the vehicle ID from Left Gun via CAN .
 *
 *  @param VehicleID is the Left gun connected vehicle's vehicle ID
 *
 *  @return None.
 */
void SendVehicleIDLeft(uint64_t VehicleID)
{
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSIOT_TX, CAN_LEFT_VEHICLEID, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, CANOBJ_CMSIOT_TX, (uint64_t *)&VehicleID,6);
}
