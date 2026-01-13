/*
 * RNL.h
 *
 *  Created on: 14-Aug-2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

/* RNL.h */
#ifndef RNL_H
#define RNL_H
#ifndef MCUSPECIFIC_H
#include "MCUSpecific.h"
#endif

#ifndef DRIVERLIB_H
    #include "driverlib.h"
#endif


#ifndef DEVICE_H
    #include "device.h"
#endif
#include <stdint.h>




#define CAN_MSG_ID_FILTER_29BIT    0x1FFFFFFF

#define RNL_RX_ID_MASK   0x0000FFFF
#define RNL_MSG_OFFSET   0xFFFF0000
#define RNL_RX_ID        0x56F4
#define RNL_RX_FILTER_ID 0x000056F4
#define RNL_BASE_ID      RNL_RX_FILTER_ID

#define CHM_ID 0x1826F456
#define CRM_ID 0x1801F456
#define CTS_ID 0x1807F456
#define CML_ID 0x1808F456
#define CRO_ID 0x100AF456
#define CCS_ID 0x1812F456
#define CST_ID 0x101AF456
#define CSD_ID 0x181DF456
#define CEM_ID 0x181FF456
#define SENDBCP_ID 0x1CECF456

#define BHM_ID 0x18270000
#define BRM_ID 0x1C020000

#define RTS_ID 0x1CEC0000
#define MPM_ID 0x1CEB0000//CHANGED ID AS PER RNL LOG BASED ON TESTING

#define BRO_ID 0x10090000
#define BCL_ID 0x18100000
#define BCS_ID 0x18110000
#define BSM_ID 0x18130000
#define BST_ID 0x10190000
#define BSD_ID 0x181C0000
#define BEM_ID 0x081E0000

#define DLC_CHM    8
#define DLC_CRM    8
#define DLC_CTS    7
#define DLC_CML    8
#define DLC_CRO    2
#define DLC_CCS    8
#define DLC_CST    4
#define DLC_CSD    5
#define DLC_CEM    4




#define FACTOR_POWERMAXLIMIT 1000
#define CHM_VERSIONMINOR     0X1
#define CHM_VERSIONMAJORL    0X1
#define CHM_VERSIONMAJORH    0X0


#define FACTOR_MAXVOLT 0.1f
#define FACTOR_MINVOLT 0.1f
#define FACTOR_MAXCURR 0.1f
#define FACTOR_MINCURR 0.1f

#define EV_YEAR        1
#define EV_MONTH       2
#define EV_DAY         3
#define EV_HOUR        4
#define EV_MIN         6
#define EV_SEC         5

#define CANOBJECTTYPERX 2
//#define CANOBJECTTYPETX 0

 typedef enum RNL
 {
     SendCHM=0,
     SendCRM,
     SendCTS1,
     SendCTS2,
     SendCTSCML,
     SendCRO,
     SendCCS,
     SendCSD,
     SendCST,
     NoSend
 }RNLMSGCTRLEnum;

 typedef enum RNL_Check
 {
     BHMFlag=0,
     BRMFlag,
     RTS1Flag,
     RTS2Flag,
     BCPFlag,
     BROFlag,
     BCLFlag,
     BSMFlag,
     BCSFlag,
     BSTFlag,
     BSDFlag,
     BEMFlag,
     Flag1,
     Flag2,
     Flag3,
     NoFlag
 }RNLFlagEnum;


/* Frame / mailbox types kept as macros for readability; driver types used in MCU layer */

typedef union
{
    uint64_t BasicHSData;
    struct
    {
        uint16_t CR_Plc_EvPermissibleMaxVolt;
        uint8_t  PLC_AAG_Detection_Value:8;
        uint8_t  CR_Plc_CpStatus:2;
        uint8_t  rsvd1:2;
        uint8_t  CR_Plc_CommStart:2;
        uint8_t  rsvd2:2;
        uint16_t SECC_FW_Year;
        uint8_t  SECC_FW_Month:8;
        uint8_t  SECC_FW_Day:8;
    } BasicHSDataSignal;
} BasicHSDataStruct;

typedef union
{
    uint64_t BasicResponseData;
    struct
    {
        uint8_t  CR_Plc_VersionMinor:8;
        uint64_t  CR_Plc_EVCC_MAC:56;
    } BasicResponseSignal;
} BasicResponseStruct;

typedef union
{
    uint64_t Battery_Charging_Parameter1;
    struct
    {
        uint64_t MSGnumber:8;
        uint64_t CR_Plc_MaxVoltSingleBattery:16;
        uint64_t CR_Plc_EvmaxCurrent:15;
        uint64_t CR_Plc_EvmaxCurrentFalg:1;
        uint64_t CR_Plc_EvNormialEnergy:16;
        uint64_t CR_Plc_EvmaxVoltLB:8;
    }Battery_Charging_Parameter1Signal;
}Battery_Charging_Parameter1Struct;

typedef union
{
    uint64_t Battery_Charging_Parameter2;
    struct
    {
        uint64_t MSGnumber:8;
        uint64_t CR_Plc_EvmaxVoltHB:8;
        uint64_t CR_Plc_EvAvailableTemp:8;
        uint64_t CR_Plc_EvRessSOC:16;
        uint64_t CR_Plc_EvCurrentVolt:16;
        uint64_t CR_Plc_EvTransferType:4;
        uint64_t rsvd:4;
    }Battery_Charging_Parameter2Signal;
}Battery_Charging_Parameter2Struct;

typedef struct
{
    uint16_t SignalRCV;
    uint16_t CR_Plc_MaxVoltSingleBattery;
    uint16_t CR_Plc_EvmaxCurrent:15;
    uint8_t CR_Plc_EvmaxCurrentFalg:1;
    uint16_t CR_Plc_EvNormialEnergy;
    uint16_t CR_Plc_EvmaxVolt;
    uint8_t CR_Plc_EvAvailableTemp;
    uint16_t CR_Plc_EvRessSOC;
    uint16_t CR_Plc_EvCurrentVolt;
    uint8_t CR_Plc_EvTransferType:4;
}Battery_Charging_ParameterStruct;

typedef union
{
    uint64_t BatteryReadinessOKData;
    struct
    {
        uint16_t CF_Plc_PowerDeliveryStart;
        uint16_t rsvd1;
        uint32_t rsvd2;
    } BatteryReadinessOKSignal;
} BatteryReadinessOKStruct;

typedef union
{
    uint64_t BatteryChargingLimitData;
    struct
    {
        uint64_t CR_Plc_EvTargetVolt : 16;                // Byte 1-2
        uint64_t CR_Plc_EvTargetCurrent : 15;        // Byte 3-4 bits 1–7, Byte 4 bits 1–7
    uint64_t  CR_Plc_EvTargetCurrent_400A : 1;    // Byte 5, bit 7 (MSB)

        uint64_t  CR_Plc_ChargingMode:8;                // Byte 6 (full 8 bits)
        uint64_t CR_Plc_EvMaxVoltLimit:16;              // Byte 7–8
        uint64_t  CR_Plc_PreCharge_Stauts:8;            // Byte 9 (optional 9th byte if present)
    } BatteryChargingLimitSignal;
} BatteryChargingLimitStruct;

typedef union
{
    uint64_t BatteryChargingStatus1Data;
    struct
    {
        uint64_t MSGnumber:8;
        uint64_t CR_Plc_SensingVolt:16;               // 1.1–2.8 (16 bits)
        uint64_t CR_Plc_SensingCurrent:16;            // 3.1–4.8 (16 bits)

        uint64_t CR_Plc_HighestVoltSingle : 12;    // 5.1–6.4 (14 bits)
        uint64_t  HighestSinglePackNo      : 4;     // 6.5–6.8 (2 bits)

        uint64_t  CR_Plc_EvRessSOC         : 8;     // 7.1–7.8 (8 bits)
    } BatteryChargingStatus1Signal;
} BatteryChargingStatus1Struct;

typedef union
{
    uint64_t BatteryChargingStatus2Data;
    struct
    {
        uint64_t MSGnumber:8;
        uint64_t CR_Plc_FullSOCRemainedTime:16;       // 8.1–9.8 (16 bits)
        uint64_t rsvd1:32;
        uint64_t rsvd2:8;
    } BatteryChargingStatus2Signal;
} BatteryChargingStatus2Struct;

typedef struct
{
    uint16_t CR_Plc_SensingVolt;               // 1.1–2.8 (16 bits)
    uint16_t CR_Plc_SensingCurrent;            // 3.1–4.8 (16 bits)

    uint16_t CR_Plc_HighestVoltSingle : 12;    // 5.1–6.4 (14 bits)
    uint8_t  HighestSinglePackNo      : 4;     // 6.5–6.8 (2 bits)

    uint8_t  CR_Plc_EvRessSOC         : 8;     // 7.1–7.8 (8 bits)
    uint16_t CR_Plc_FullSOCRemainedTime;
} BatteryChargingStatusStruct;

typedef union
{
    uint64_t BatteryStatusData;

    struct
    {
        uint8_t  CR_Plc_SernumSinglePower   : 8;   // 1.1–1.8
        uint8_t  CR_Plc_HighestTemp         : 8;   // 2.1–2.8
        uint8_t  CR_Plc_SernumHighTemp      : 8;   // 3.1–3.8
        uint8_t  CR_Plc_LowestTemp          : 8;   // 4.1–4.8
        uint8_t  CR_Plc_SernumLowTemp       : 8;   // 5.1–5.8

        uint8_t  CF_Plc_OverlowVolSinglePowe : 2;  // 6.1–6.2
        uint8_t  CF_Plc_OverlowSocBat        : 2;  // 6.3–6.4
        uint8_t  CF_Plc_OverCurr             : 2;  // 6.5–6.6
        uint8_t  CF_Plc_ExcexxTemp           : 2;  // 6.7–6.8

        uint8_t  CF_Plc_InsulState           : 2;  // 7.1–7.2
        uint8_t  CF_Plc_ConnectionState      : 2;  // 7.3–7.4
        uint8_t  CF_Plc_EvReady              : 2;  // 7.5–7.6

        uint8_t  rsvd1                       : 2;  // 7.7–7.8 (Reserved)
        uint8_t  rsvd2                       : 8;  // 8.1–8.8 (Reserved)
    } BatteryStatusSignal;

}BatteryStatusStruct;

typedef union
{
    uint64_t BatteryStopTriggerData;

    struct
    {
        uint8_t CF_Plc_ReachSoC           : 2;  // 1.1–1.2
        uint8_t CF_Plc_ReachTotalVolt     : 2;  // 1.3–1.4
        uint8_t CF_Plc_ReachSingleVolt    : 2;  // 1.5–1.6
        uint8_t ChargerSuspendFlag        : 2;  // 1.7

        uint8_t CF_Plc_InstationFault     : 2;  // 2.1–2.2
        uint8_t CF_Plc_ConnectorHighTemp  : 2;  // 2.3–2.4
        uint8_t CF_Plc_BmsOverTemp        : 2;  // 2.5–2.6
        uint8_t CF_Plc_ConnectorFault     : 2;  // 2.7–2.8

        uint8_t CF_Plc_BatOverTemp        : 2;  // 3.1–3.2
        uint8_t CF_Plc_HighVoltRelay      : 2;  // 3.3–3.4
        uint8_t CF_Plc_ErrCheckPoint2     : 2;  // 3.5–3.6
        uint8_t CF_Plc_OtherFault         : 2;  // 3.7–3.8

        uint8_t CF_Plc_CurrOver           : 2;  // 4.1–4.2
        uint8_t CF_Plc_AbnormalVolt       : 2;  // 4.3–4.4
        uint8_t reserved2                 : 2;  // 4.5–4.6 (reserved/padding)
        uint8_t FullChargeCompleted       : 1;  // 4.7
        uint8_t CF_Plc_ChgFinished        : 1;  // 4.8

        uint8_t Message_Sequence:8;              // 5.1–5.8 (1 byte)
        uint8_t CR_Plc_ErrorCodeII:8;            // 6.1–6.8 (1 byte)
        uint8_t CR_Plc_ErrorCode:8;              // 7.1–7.8 (1 byte)

        uint8_t ErrorCode_CP_PP_State     : 3;  // 8.1–8.3
        uint8_t Error_EV          : 4;
        uint8_t rsvd              : 1;
        }BatteryStopTriggerSignal;
} BatteryStopTriggerStruct;

typedef union
{
    uint64_t BatteryShutdownData;

    struct
    {
        uint64_t CR_Plc_SuspendSoc:8;  // 1.1–1.8 (8 bits)

        uint64_t CR_MinVoltSinglePower:16; // 2.1–3.8 (2 bytes)

        uint64_t CR_MaxVoltSinglePower:16; // 4.1–5.8 (2 bytes)

        uint64_t CR_MinTempPower         : 8;  // 6.1–6.8 (1 byte)

        uint64_t CR_ManTempPower         : 8;  // 7.1–7.8 (1 byte)

        uint64_t WeldDetectionRequest    : 1;  // 8.1 (1 bit)
        uint64_t reserved                : 7;  // 8.2–8.8 (padding to make 64-bit)
    }BatteryShutdownSignal;
} BatteryShutdownStruct;

typedef union
{
    uint64_t BEM_Data;  // 64 bits = 8 bytes
    struct
    {
        uint8_t CF_Plc_CRM00Timeout       : 2;
        uint8_t CF_Plc_CRMAATimeout       : 2;
        uint8_t reserved0                 : 4;

        uint8_t CF_Plc_CTS_CMLTimeout     : 2;
        uint8_t CF_Plc_CROAALTimeout      : 2;
        uint8_t reserved1                 : 4;

        uint8_t CF_Plc_CCSTimeout         : 2;
        uint8_t CF_Plc_CSTTimeout         : 2;
        uint8_t reserved2                 : 4;

        uint8_t CF_Plc_CSDTimeout         : 2;
        uint8_t other                     : 2;
        uint8_t reserved3                 : 4;

        uint8_t Message_Sequence          : 8;

        uint8_t CR_Plc_ErrorCodeII        : 8;           // 6.1–6.8 (8 bits)

        uint8_t CR_Plc_ErrorCode          : 8;             // 7.1–7.8 (8 bits)

        uint8_t CP_PP_State_ErrorCode    : 3; // 8.1–8.3
        uint8_t CR_PLc_EvErrCode         : 4; // 8.4–8.7
        uint8_t reserved4                : 1; // 8.8 (padding to complete byte)
    }BEMSignal;
}BEMStruct;
typedef struct
{
    uint8_t BRMTimeout;
    uint8_t BCPTimeout;
    uint8_t BROTimeout;
    uint8_t BCSTimeout;
    uint8_t BSDTimeout;
    uint8_t CRM_Send_Msg;
    uint8_t BMRMsg;
    uint8_t BCP_CTS;
    uint8_t BCS_CTS;
}RNLGeneralStruct;
typedef struct
{
    BasicHSDataStruct BasicHSData;
    BasicResponseStruct BasicResponse;
    Battery_Charging_Parameter1Struct Battery_Charging_Parameter1;
    Battery_Charging_Parameter2Struct Battery_Charging_Parameter2;
    Battery_Charging_ParameterStruct Battery_Charging_Parameter;
    BatteryReadinessOKStruct BatteryReadinessOK;
    BatteryChargingLimitStruct  BatteryChargingLimit;
    BatteryChargingStatus1Struct BatteryChargingStatus1;
    BatteryChargingStatus2Struct BatteryChargingStatus2;
    BatteryChargingStatusStruct BatteryChargingStatus;
    BatteryStatusStruct BatteryStatus;
    BatteryStopTriggerStruct BatteryStopTrigger;
    BatteryShutdownStruct BatteryShutdown;
    BEMStruct BEMValue;
} ReceiveFromRMLStruct;

typedef enum Send
{
 SendRTS1 = 0,
SendRTS2,
SendRTS3
} RNLDualStruct;



typedef union
{
    uint64_t ChargeHandshakeMessageData;
    struct
    {
        uint8_t VerMinor:8;
        uint8_t VerMajorL:8;
        uint8_t VerMajorH:8;
        uint8_t EVSETransferType:4;
        uint8_t StartPLC:1;
        uint8_t rsvd1:3;
        uint16_t MaxPowerLimit;
        uint8_t InitCPState:1;
        uint8_t StartCPState:1;
        uint8_t SLACTime:6;
        uint8_t rsvd2:1;
        uint8_t ChgType:1;
        uint8_t ChgMode:1;
        uint8_t rsvd3:1;
        uint8_t SLACRestartTime:4;
    } ChargeHandshakeMessageSignal;
} ChargeHandshakeStruct;

typedef union
{
    uint64_t ChargeRecognitionData;
    struct
    {
        uint8_t  SECC_Recognition_Result:8;   // Charger identification results
        uint32_t CR_Secc_ChargerNum;       // Charger number
        uint32_t CR_Secc_LocationCode:24;     // Charger position coding
    } ChargeRecognitionSignal;
} ChargeRecognitionStruct;

typedef union
{
    uint64_t ChargingTimeStartData;
    struct
    {
        uint64_t CR_Secc_EvseSecond:8;  // Seconds (1.1–1.8)
        uint64_t CR_Secc_EvseMin:8;     // Minutes (2.1–2.8)
        uint64_t CR_Secc_EvseHour:8;    // Hours (3.1–3.8)
        uint64_t CR_Secc_EvseDay:8;     // Day (4.1–4.8)
        uint64_t CR_Secc_EvseMonth:8;   // Month (5.1–5.8)
        uint64_t CR_Secc_EvseYear:16;      // Year LSB (6.1–6.8)
        uint64_t rsvd:8;
    } ChargingTimeStartSignal;
} ChargingTimeStartStruct;


typedef union
{
    uint64_t ChargingMaxLimitData;
    struct
    {
        uint16_t CR_Secc_EvseMaxVolt_V;
        uint16_t CR_Secc_EvseMinVolt_V;
        uint16_t CR_Secc_EvseMaxCurr_A : 15;
        uint8_t  CR_Secc_EvseMaxCurr_A400A : 1;
        uint16_t CR_Secc_EvseMinCurr_A:15;
        uint8_t  CR_Secc_EvseMinCurr_A400A : 1;
    }ChargingMaxLimitSignal;
} ChargingMaxLimitStruct;

typedef union
{
    uint64_t ChargerReadinessOKData;
    struct
    {
        uint8_t  CR_Secc_ContactorOn:8;
        uint8_t  CR_Secc_EvseIsolationStatus : 3;
        uint8_t  rsvd1 : 5;
        uint32_t rsvd2;
        uint16_t rsvd3;
    } ChargerReadinessOKSignal;
} ChargerReadinessOKStruct;

typedef union
{
    uint64_t ChargerChargingStatusData;

    struct
    {
        uint16_t CR_Secc_EvseOutVolt;            // 1.1–2.8  (16 bits)

        uint16_t CR_Secc_EvseOutCurrent          : 15;      // 3.1–4.7  (15 bits)
        uint16_t CR_Secc_EvseOutCurrentOver400A  : 1;       // 4.8      (1 bit)

        uint16_t CR_Secc_MaxPowerLimit;          // 5.1–6.8  (16 bits)

        uint8_t  CF_Secc_EvseSuspended           : 2;       // 7.1–7.2  (2 bits)
        uint8_t  CR_Secc_EvsseIsolationStatus    : 3;       // 7.3–7.5  (3 bits)
        uint8_t  CR_Secc_EvseMaxCurr_IsUse       : 1;       // 7.6      (1 bit)
        uint16_t  CR_Secc_EvseMaxCurr_A           : 10;      // 7.7–8.8  (10 bit)
    } ChargerChargingStatusSignal;
} ChargerChargingStatusStruct;

typedef union
{
    uint64_t ChargingStopTimeData;

    struct
    {
        uint8_t CF_Secc_Suspended         : 2;  // 1.1–1.2
        uint8_t CF_Secc_ArtificialSuspend : 2;  // 1.3–1.4
        uint8_t CF_Secc_FaultSpended      : 2;  // 1.5–1.6
        uint8_t CF_Secc_BmsSuspended      : 2;  // 1.7–1.8

        uint8_t CF_Secc_OverTemp          : 2;  // 2.1–2.2
        uint8_t CF_Secc_ConnectorFail     : 2;  // 2.3–2.4
        uint8_t CF_Secc_InternelOverTemp  : 2;  // 2.5–2.6
        uint8_t CF_Secc_RequestReject     : 2;  // 2.7–2.8

        uint8_t CF_Secc_SuddenStop        : 2;  // 3.1–3.2
        uint8_t CF_Secc_OtherFault        : 2;  // 3.3–3.4
        uint8_t reserved1                 : 4;  // 3.5–3.8 (not used or reserved)

        uint8_t CF_Secc_CurrentMismatch   : 2;  // 4.1–4.2
        uint8_t CF_Secc_VoltAbnormal      : 2;  // 4.3–4.4
        uint8_t reserved2                 : 4;  // 4.5–4.8 (padding to make 64-bit)

    uint32_t rsvd;
    }ChargingStopTimeSignal;
} ChargingStopTimeStruct;

typedef union
{
    uint64_t ChargerShutdownData;

    struct
    {
        uint16_t CR_Secc_CumulativeChTime     : 16;

        uint16_t CR_Secc_EvseOutVolt          : 16;

        uint8_t  CR_Secc_EvseIsolationStatus  : 3;

        uint8_t  reserved                     : 5;
        uint8_t rsvd1                         : 8;
        uint16_t reserved2                    : 16;
    }ChargerShutdownSignal;
} ChargerShutdownStruct;

typedef union
{
    uint64_t CEM_Data;  // 64 bits = 8 bytes

    struct
    {
        uint8_t CF_Secc_BRMTimeout       : 2;
        uint8_t reserved0                : 6;

        uint8_t CF_Secc_BCPTimeout       : 2;
        uint8_t CF_Secc_BROTimeout       : 2;
        uint8_t reserved2                : 4;

        uint8_t CF_Secc_BCSTimeout       : 2;
        uint8_t CF_Secc_BCLTimeout       : 2;
        uint8_t CF_Secc_BSTTimeout       : 2;
        uint8_t reserved3                : 2;

        uint8_t CF_Secc_BSDTimeout       : 2;
        uint8_t other                    : 2;
        uint8_t reserved4                : 4;

        uint16_t reserved_bytes[2];
    }CEMSignal;
} CEMStruct;


typedef struct
{
    ChargeHandshakeStruct ChargeHandshake;
    ChargeRecognitionStruct  ChargeRecognition;
    ChargingTimeStartStruct ChargingTimeStart;
    ChargingMaxLimitStruct ChargingMaxLimit;
    ChargerReadinessOKStruct ChargerReadinessOK;
    ChargerChargingStatusStruct ChargerChargingStatus;
    ChargingStopTimeStruct ChargingStopTime;
    ChargerShutdownStruct ChargerShutdown;
    CEMStruct CEM;
}SendToRNLStruct;




void RNLRXCANOBJSet(uint32_t CANBASE);
void RNLTXCANOBJSet(uint32_t CANBASE);

void SendtoRML_CHM(uint32_t CANBASE,ChargeHandshakeStruct *ChargeHandshakeTemp);
void SendtoRML_CRM(uint32_t CANBASE,ChargeRecognitionStruct *  ChargeRecognitionTemp);
void SendtoRML_CTS(uint32_t CANBASE,ChargingTimeStartStruct *  ChargingTimeStartTemp);
void SendtoRML_CML(uint32_t CANBASE,ChargingMaxLimitStruct *  ChargingMaxLimitTemp);
void SendtoRML_CRO(uint32_t CANBASE,ChargerReadinessOKStruct *  Temp);
void SendtoRML_CCS(uint32_t CANBASE,ChargerChargingStatusStruct *  Temp);
void SendtoRML_CST(uint32_t CANBASE,ChargingStopTimeStruct *  Temp);
void SendtoRML_CSD(uint32_t CANBASE,ChargerShutdownStruct *  Temp);
void SendtoRML_CEM(uint32_t CANBASE,CEMStruct *  Temp);


void RNL_ProcessCANMsg(uint32_t can_id, uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData,RNLFlagEnum *FLAG,uint8_t GunX,RNLDualStruct *Send);
void ProcessBatteryHandshakeMessage(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData);
void ProcessBatteryResponseMessage(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData);
void ProcessBatteryChargingParameter1(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData);
void ProcessBatteryChargingParameter2(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData);
void ProcessBatteryReadinessOk(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData);
void ProcessBatteryChargingLimit(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData);
void ProcessBatteryChargingStatus1(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData);
void ProcessBatteryChargingStatus2(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData);
void ProcessBatteryStatusMessage(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData);
void ProcessBatteryStopTrigger(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData);
void ProcessBatteryShutdown(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData);
void ProcessBEM(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData);
void ProcessBCP(ReceiveFromRMLStruct *receivedData);
void ProcessBCS(ReceiveFromRMLStruct *receivedData);

void setupChargeHandshake(ChargeHandshakeStruct *chs);
void setupChargeTimeStart(ChargingTimeStartStruct *cts);
#endif /* RNL_H */

