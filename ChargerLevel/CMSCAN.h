/*
 * CMSCAN.h
 *
 *  Created on: Apr 15, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

#ifndef CMSCAN_H_
#define CMSCAN_H_

#ifndef MCUSPECIFIC_H
#include "MCUSpecific.h"
#endif

#ifndef CHARGER_H_
#include "Charger.h"
#endif

#ifndef MUXIO_H_
#include "Muxio.h"
#endif

#ifndef ACEMETER_H
#include "ACEMeter.h"
#endif

#ifndef DCEMETER_H
#include "DCEMeter.h"
#endif

#ifndef VSECC_H
#include "VSECC.h"
#endif

#include "stdint.h"

#define CMS_EXTERNAL_PECCLIMIT1_OFFSET      0X94
#define CMS_EXTERNAL_PECCLIMIT2_OFFSET      0X95


typedef struct {
    float limitVoltageMin;
    float limitVoltageMax;
    float limitPowerMax;
    float limitPowerMin;
} CMSPECCLimits1_t;

typedef struct {
    float limitCurrentMin;
    float limitCurrentMax;
} CMSPECCLimits2_t;

 typedef union
 {
     uint64_t ALLVoltageData;
     struct
     {
         uint16_t PhaseAVoltage;
         uint16_t PhaseBVoltage;
         uint16_t PhaseCVoltage;
         uint16_t rsvd;
     }IndividualVol;
 }CANVoltageStruct;

 typedef union
  {
      uint64_t ALLCurrentData;
      struct
      {
          uint16_t PhaseACurrent;
          uint16_t PhaseBCurrent;
          uint16_t PhaseCCurrent;
          uint16_t rsvd;
      }IndividualCurrent;
  }CANCurrentStruct;

  typedef union
   {
       uint64_t ALL_Energy;
       struct
       {
           uint16_t PhaseAEnergy;
           uint16_t PhaseBEnergy;
           uint16_t PhaseCEnergy;
           uint16_t rsvd;
       }IndividualEng;
   }CANLLVoltageStruct;

   typedef union
   {
       uint64_t ALLActivePower;
       struct
       {
           uint16_t PhaseAActivePower;
           uint16_t PhaseBActivePower;
           uint16_t PhaseCActivePower;
           uint16_t rsvd;
       }IndividualPow;
   }CANPowerStruct;

   typedef union
   {
       uint64_t ALLTP_Hz_PFData;
       struct
       {
           uint16_t TotalActivePower;
           uint16_t Frequency;
           uint16_t PowerFactor;
           uint16_t TotalActiveEnergy;
       }WHzPfData;
   }CANTPHzPfStruct;

typedef union
{
    uint64_t ALL_VoltAmpData;
    struct
    {
        uint32_t voltage;
        uint32_t current;
    }MeterData;
}CANVoltAmpStruct;

typedef union{
    uint64_t ALL_power;
    struct{
        uint32_t right_power;
        uint32_t left_power;
    }power;
}CANDCPowerStruct;

typedef union{
    uint64_t versions;
    struct{
        uint16_t ChargerRating;
        uint16_t BaseVersion;
        uint16_t MatlabVersion;
        uint16_t ChangeNumber;
    }Individualversion;
}CANVersionStruct;

typedef union{
    uint64_t ALL_Energy;
    struct{
        uint32_t right_pos;
        uint32_t left_pos;
    }energy;
}CANDCEnergyStruct;

typedef union{
    uint64_t ALL_Energy;
    struct{
        uint32_t right_pos;
        uint32_t left_pos;
    }energy;
}CANDCEnergyDiffStruct;
typedef union{
    uint64_t ALL_IMD;
    struct{
        uint32_t Right_IMD;
        uint32_t Left_IMD;
    }IMD;
}CANIMDStruct;

typedef struct{
    float64_t negEnergy;
}CANNegEnergyStruct;

typedef struct
{
    CMSPECCLimits1_t   CMSPECCLimits1;
    CMSPECCLimits2_t   CMSPECCLimits2;
}CMSExternalReceiveStruct;

typedef struct
{
    uint8_t G1StartCharge:2;
    uint8_t G1StopCharge:2;
    uint8_t G2connected:2;
    uint8_t G2stopCharge:2;
}IOTChargestruct;

typedef enum
{
    Gun1_stop = 0x1,
    Gun1_start = 0x2,
    Gun1_Ack = 0x3
}ChargerGun1_Stop_t;

typedef enum
{
    Gun2_stop = 0x1,
    Gun2_start = 0x2,
    Gun2_Ack = 0x3
}ChargerGun2_Stop_t;

typedef union{
    uint32_t I2Cstatus;
    struct{
        uint8_t DIOStatus;
        uint8_t I2C1Status;
        uint8_t I2C2Status;
    }DIOI2Cstatus;
}DIOI2CCheckStatus_t;

typedef union{
    uint64_t CANData;
    struct{
         uint8_t RightPositiveContactorDrive:1;
         uint8_t RightNegativeContactorDrive:1;
         uint8_t RightPrechargeContactorDrive:1;
         uint8_t RightChargingSpeed:2;
         uint8_t RightPowerLED:1;
         uint8_t RightStopChargeLED:1;
         uint8_t RightStopCharge:1;

         uint8_t RightStartCharge:1;
         uint8_t RightStatusLED:1;
         uint8_t RightGunVSECCReset:1;
         uint8_t RightGunIOTStartCharge:1;
         uint8_t RightGunIOTStopCharge:1;
         uint8_t RightGunIMDEnable:1;
         uint8_t LeftPositiveContactorDrive:1;
         uint8_t LeftNegativeContactorDrive:1;

         uint8_t LeftPrechargeContactorDrive:1;
         uint8_t LeftChargingSpeed:2;
         uint8_t LeftPowerLED:1;
         uint8_t LeftStopChargeLED:1;
         uint8_t LeftStopCharge:1;
         uint8_t LeftStartCharge:1;
         uint8_t LeftStatusLED:1;

         uint8_t LeftGunVSECCReset:1;
         uint8_t LeftIOTStartCharge:1;
         uint8_t LeftIOTStopCharge:1;
         uint8_t LeftGunIMDEnable:1;
         uint8_t CoolingFanControl:1;                 // Done  -> Radiator Fan LED emulation
         uint8_t ExhuastFanControl:1;                 // Done  -> Fans near to Rect.
         uint8_t CoolingPumpPowerControl:1;           // Done  -> POWER Enable Pin
         uint8_t ACContactorDrive:1;        // Done


         uint8_t CrossContactorDrive:1;

         uint16_t Rsvd:7;

         uint16_t Rsvd1[3];
    }GunData;
}GunDataStatus_t;

typedef union{
    uint64_t TotalAMPVol;
    struct{
        uint16_t TotalMeasuredVoltageR;
        uint16_t TotalMeasuredCurrentR;
        uint16_t TotalMeasuredVoltageL;
        uint16_t TotalMeasuredCurrentL;
    }VOLAMP;
}TotalMeasureVOLAMP_t;

typedef union{
    uint64_t  RawValue;
    struct{
        uint8_t RGState:8;
        uint8_t LGState:8;
        uint8_t TonheRCount:8;
        uint8_t TonheLCount:8;
        uint8_t VSECCRCount:8;
        uint8_t VSECCLCount:8;
        uint16_t DIOCOunt;
    }Gunstatus;
}BothGunState;


typedef union
{
    uint64_t gunstate;
    struct{
        uint16_t state;
        uint16_t rightState;
        uint16_t leftstate;
        uint16_t reserv;
    }all_states;
}CANStateStruct;

typedef union
{
    bool StopChargeLED1;
    bool StopChargeLED2;
}CANStopChargeStruct;

typedef union{
    uint64_t BothFlow;
    struct{
        uint32_t right_flow;
        uint32_t left_flow;
    }Flow;
}CANFlowStruct;

typedef union{
    uint64_t fault;
    struct{

        // 7 Fault bits for rightGun
        uint8_t RightGunFault:1;
        uint8_t RightPositiveContactorFault:1;
        uint8_t RightNegativeContactorFault:1;
        uint8_t RightGunIMDFault:1;
        uint8_t RightIMDResFault:1;
        uint8_t RightTempFault:1;
        uint8_t RightParmanantFault:1;


        // 7 Fault bits for rightGun
        uint8_t LeftGunFault:1;
        uint8_t LeftPositiveContactorFault:1;
        uint8_t LeftNegativeContactorFault:1;
        uint8_t LeftGunIMDFault:1;
        uint8_t LeftIMDResFault:1;
        uint8_t LeftTempFault:1;
        uint8_t LeftParmanantFault:1;

        // 12 Fault For ChargerData
        uint8_t ChargerFault:1;
        uint8_t ChargerPermantFault:1;
        uint8_t ChargerTempFault:1;
        uint8_t VoltageBalanceFault:1;
        uint8_t CurrentBalanceFault:1;
        uint8_t VoltageLimitFault:1;
        uint8_t CurrentLimitFault:1;
        uint8_t FrequncyLimitFault:1;
        uint8_t EMButtonFault:1;
        uint8_t SPDFault:1;
        uint8_t ELRFBFault:1;
        uint8_t ACContactorFault:1;

        // 5 Fault For ModBus
        uint8_t ACEMFault:1;
        uint8_t DCEM1Fault:1;
        uint8_t DCEM2Fault:1;
        uint8_t IMDM1Fault:1;
        uint8_t IMDM2Fault:1;

        // 3 Faults for CAN Cyclic VSECC, TOnhe Left and Tonhe Right
        uint8_t VSECCCyclic:1;
        uint8_t TonheRightCyclic:1;
        uint8_t TonheLeftCyclic:1;

        // 6 For TOnhe Specific
       uint8_t TonheRight1Fault:1;
       uint8_t TonheRight2Fault:1;
       uint8_t TonheRight3Fault:1;

       uint8_t TonheLeftt1Fault:1;
       uint8_t TonheLeft2Fault:1;
       uint8_t TonheLeft3Fault:1;

       // 3 New Fault
       uint8_t ChargerDIOFault:1;
       uint8_t CrossContactorFault:1;
       uint8_t CrossContactorWeldFault:1;
       uint8_t RightGunErrorState:1;
       uint8_t LeftGunErrorState:1;
       uint8_t Rsvd:3;

       uint16_t rsvd1;
    }bitfaults;
}Charger_Left_Right_fault;


typedef union{
    uint16_t TonheStatus;
    struct{
        uint8_t Tonhe_RightRectifier_1:2;
        uint8_t Tonhe_RightRectifier_2:2;
        uint8_t Tonhe_RightRectifier_3:2;
        uint8_t Tonhe_LeftRectifier_1:2;
        uint8_t Tonhe_LeftRectifier_2:2;
        uint8_t Tonhe_LeftRectifier_3:2;
        uint8_t Gun1_Rectifiers:2;
        uint8_t Gun2_Rectifiers:2;
    }bitsStatus;
}CANTonheStatus;

typedef union{
    uint32_t BothIOTStatus;
    struct{
        uint8_t Right_IOTStop;
        uint8_t Right_IOTStart;
        uint8_t Left_IOTStop;
        uint8_t Left_IOTStart;
    }IOTStatus;
}CANIOTStatus_t;

void  CMSTXCANOBJSet(uint32_t CAN_BASE);
void  CMSRXCANOBJSet(uint32_t CAN_BASE);
void  CMS_ProcessCANMsg(uint32_t can_id, uint8_t *data, uint8_t length, CMSExternalReceiveStruct *receivedData);
void ProcessCMSPECCLimit2(uint16_t *data, uint8_t length, CMSExternalReceiveStruct *receivedData);
void ProcessCMSPECCLimit1(uint16_t *data, uint8_t length, CMSExternalReceiveStruct *receivedData);
void SendCMSTempCANData(GunStruct *GunNumber1,GunStruct *GunNumber2,ChargerStruct *ChargerData);
void SendCMSDigitalInputCANData(uint16_t MuxIO1, uint16_t MuxIO2);
void SendACMeterVoltageInputCANData(ChargerStruct ChargerData);
void SendACMeterCurrentInputCANData(ChargerStruct ChargerData);
void SendACMeterEnergyInputCANData(ChargerStruct ChargerData);
void SendACMeterPowerInputCANData(ChargerStruct ChargerData);
void SendACMeterInputCANData(AC_EnergyMeterDataStruct ACValues, ChargerStruct ChargerData);
void SendDCMeterVoltAmpCANData(DC_EnergyMeterDataStruct DCValues, bool value);
void SendDCMeterPowerCANData(DC_EnergyMeterDataStruct DCValues, DC_EnergyMeterDataStruct DCValues2);
void SendDCMeterPosEnergyCANData(GunStruct RightGun,GunStruct leftGun);
void SendDCMeterNegEnergyCANData(DC_EnergyMeterDataStruct DCValues);
void SendIMDMeterCANData(float IMDdata,float IMDdata2);
int CMSTempSensor();
void SendCANState(uint16_t flowState, uint16_t flowRight, uint16_t flowLeft);
void SendCANStopChargeLED(bool gun1, bool gun2);
void SendCMSVersionCANData();
void SendCMSFlowrate(float right_flow, float left_flow);
void CMSStopMessage(GunStruct *GunData1, GunStruct *GunData2);
void SendFaultCANData(GunStruct RightGun,GunStruct LeftGun,ChargerStruct ChargerData,FaultAndCounterStruct ModBusFaultDdata);
void SendCMSModbusMeterStatus(ACEMModBusStatusStruct *ACEMModBusStatus,DCEMModBusStatusStruct *DCEMModBusStatus1,DCEMModBusStatusStruct *DCEMModBusStatus2,ModbusCommStatus *IMDStatus1,ModbusCommStatus *IMDStatus2);
void SendCMSDC1EnergyMeterModbusStatus();
void SendCMSDC2EnergyMeterModbusStatus();
void SendCMS_BothGuns_IMDModbusStatus();
void SendCMS_TonheStatus(RectifierStatus_t Tonhe_Status);
void SendCMSIOTStartStop(GunStruct RightGun, GunStruct LeftGun);
void SendCMSI2CStatus(uint8_t DIOStatus, uint8_t *I2Cstatus);
void SendGunData(GunStruct RightGun,GunStruct LeftGun,ChargerStruct ChargerDataX);
void SendCMS_TotalMeasuredAmpVol(GunStruct RightGun,GunStruct LeftGun);
void SENDDiffDCEMeter(GunStruct RightGun,GunStruct leftGun);
void SendGUNStatus(uint8_t RGGS,uint8_t LGGS,uint8_t TR,uint8_t TL,uint16_t DioCOunt);
void SendVehicleIDRight(uint64_t VehicleID);
void SendVehicleIDLeft(uint64_t VehicleID);
#endif
