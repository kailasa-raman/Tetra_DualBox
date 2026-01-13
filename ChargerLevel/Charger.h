
/*
 * Charger.h
 *
 *  Created on: Feb 28, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

#ifndef CHARGER_H_
#define CHARGER_H_


#ifndef VSECC_H

#endif

#ifndef TONHECAN_H
#include "TonheCAN.h"
#endif

#ifndef MUXIO_H_
#include "MuxIO.h"
#endif

#ifndef ACEMTER_H_
#include "ACEmeter.h"
#endif

#ifndef DCEMTER_H_
#include "DCEmeter.h"
#endif

#ifndef IMD_H_
#include "IMD.h"
#endif

#include "F021_F28004x_C28x.h"
#include "flash.h"
#include"RNL.h"
#include "IMD.h"
#include "VSECC.h"

#define ChargerTetraAlgoTime             10// in MS
#define ChargerSECCTask                  250
#define ChargerSECCTaskCount             (ChargerSECCTask/ChargerTetraAlgoTime)

#define ModbusAlgoTime                   1000
#define ModbusAlgoTimeCount              (ModbusAlgoTime/ChargerTetraAlgoTime)



#define WritePin    WriteDGPIO
#define WriteContactor  WritePin
#define PowerEnable     ContactorWrite
#define DIOTIMOUT       10000               // in uS


#define  ACEM_COMM_FAULT_COUNT          10
#define  DCEM_COMM_FAULT_COUNT          10
#define  IMD_COMM_FAULT_COUNT           10

#define Charger120
#ifdef Charger60
#define RectifierPerGun 1
#define HalfPower 30000
#endif

#ifdef Charger120
#define RectifierPerGun 1
#define HalfPower 30000
#endif

#ifdef Charger180
#define RectifierPerGun 3
#define HalfPower 90000
#endif




/**
 * EV Connection State Types (evConnectionStateType)
 */
typedef enum {
    EV_DISCONNECTED = 0x0,
    EV_CONNECTED = 0x1,
    EV_ENERGY_TRANSFER_ALLOWED = 0x2,
    EV_ERROR = 0x3
}evConnectionState_t;

/**
 * Operational Status Types (operationalStatusType)
 */
typedef enum {
    OPERATIONAL_STATUS_OPERATIVE = 0x0,
    OPERATIONAL_STATUS_INOPERATIVE = 0x1
} operationalStatus_t;

/**
 * Isolation Status Types (isolationStatusType)
 */
typedef enum {
    ISOLATION_INVALID = 0x0,
    ISOLATION_VALID = 0x1,
    ISOLATION_WARNING = 0x2,
    ISOLATION_FAULT = 0x3
} isolationStatus_t;

/**
 * Contactor Status Types (contactorsStatusType)
 */
typedef enum {
    CONTACTOR_OPEN = 0x0,
    CONTACTOR_CLOSED = 0x1
} contactorsStatus_t;

/**
 * Charging State Types (chargingStateType)
 */
typedef enum {
    CHARGING_STANDBY = 0x0,
    CHARGING_CABLE_CHECK = 0x1,
    CHARGING_PRECHARGE = 0x2,
    CHARGING_CHARGE = 0x3,
    CHARGING_POSTCHARGE = 0x4
} chargingState_t;

/**
 * Charge Mode Types (chargeModeType)
 */
typedef enum {
    CHARGE_MODE_UNKNOWN = 0x0,
    CHARGE_MODE_SCHEDULED = 0x1,
    CHARGE_MODE_DYNAMIC = 0x2,
    CHARGE_MODE_DYNAMIC_BPT = 0x3
} chargeMode_t;

/**
 *  Vehicle Status Types (VehicleStatusType)
 */
typedef union
{
uint64_t VehicleStatus;
struct {
    uint64_t targetContactorsStatus:1;
    uint64_t evConnectionState:2;
    uint64_t chargingState:3;
    uint64_t reserved1:2;
    uint64_t targetVoltage:16;
    uint64_t targetCurrent:16;
    uint64_t batteryStateOfCharge:7;
    uint64_t reserved2:1;
    uint64_t cableCheckVoltage:16;
} VehicleStatus_t;
}status;

typedef union
{
    uint64_t Pecc1;
struct {
        uint64_t contactorsStatus:1;
        uint64_t operationalStatus:1;
        uint64_t isolationStatus:3;
        uint64_t reserved2:3;
        uint64_t drivenVoltage:16;
        uint64_t drivenCurrent:16;
        uint64_t temperature:16;
        uint64_t reserved1:8;

} PECCStatus1_t;
}PeccStatus1;

typedef union
{
    uint64_t Pecc2;
struct {
        uint64_t measuredVoltage:16;
        uint64_t measuredCurrent:16;
        uint64_t status:16;
        uint64_t reserve:16;
} PECCStatus2_t;
}PeccStatus2;
typedef union
{
    uint64_t Stopcharge;
struct {
        uint8_t measuredVoltage:16;

} StopchargeStatus;
}StopChargeRNL;

typedef union
{ uint64_t pecclimit2;
struct {
    uint64_t limitCurrentMin:16;
    uint64_t limitCurrentMax:16;
} PECCLimits2_t;
}PeccLimits2;

typedef union
{  uint64_t pecclimit1;
struct {
    uint64_t limitVoltageMin:16;
    uint64_t limitVoltageMax:16;
    uint64_t limitPowerMax:16;
    uint64_t limitPowerMin:16;
} PECCLimits1_t;
}PeccLimits1;


typedef union
{ uint64_t chargesessioninfo1;
struct {
    uint64_t chargingProfileMaxPowerLimit:16;
    uint64_t timeToFullSoc:16;
} ChargingSessionInfo1_t;
}ChargeSessionInfo1;



typedef union
{ uint64_t chargingsessioninfo2;
        struct {
    uint64_t evMaxVoltage:16;
    uint64_t evMaxCurrent:16;
    uint64_t evMaxPower:16;
} ChargingSessionInfo2_t;
}ChargeSessionInfo2;

typedef union
{ uint64_t chargingsessioninfo3;
struct {
    uint64_t evMinVoltage:16;
    uint64_t evMinCurrent:16;
    uint64_t evMinPower:16;
    uint64_t chargeMode:8;
} ChargingSessionInfo3_t;
}ChargeSessionInfo3;

typedef enum
{
    DCEM1,
    DCEM2,
}DCEMNumber;

typedef enum
{
    IMD1,
    IMD2,
}IMDNumber;

typedef enum
{
    Display1,
    Display2,
}DisplayNumber;
typedef struct
{
    bool ACVotlageLimitFlag;
    bool ACCurrentLimitFlag;
    bool ACFrequencyFlag;
    bool ACVoltageImbalanceFlag;
    bool ACCurrentImbalanceFlag;
}ChargerVarStatusStruct;

typedef enum
{
    NormalOff=0,
    NormalOn,
    FaultOFF,
}ChargeModuleStateEnum;
typedef enum
{
    PLCOff=0,
    PLCOn,
}PLCCommand;

typedef enum
{
    Emergency=0,
    NoEmergency,
}EmergencyButtonStatus;

typedef enum
{
    IMDFault=0,
    IMDNoFault,
}IMDFBStatus;

typedef enum
{
   Off=0,
   On,
}ContactorWrite;

typedef enum
{
    DoorOpen=0,
    DoorClosed,
}DoorStatus;

typedef enum
{
    PumpOff=0,
    PumpOn,
}PumpWrite;

typedef enum
{
    FBNotOk=0,
    FBOk,
}ContactorFeedbackStatus;

typedef enum
{
    BothGunFree=0,
    Gun1Free,
    Gun2Free,
    BothGunUsed,
}GunStatus;

typedef enum
{
    LEDRed=0,
    LEDGreen,
    LEDGreenBlink,
}AddLEDStatus;

typedef enum
{
    Positive=0,
    Negetive,
    PreCharge,
    Cross,
}GunContactortype;
typedef enum
{
    Gun1=1,
    Gun2=2,
}GunNumberType;

typedef struct
{
    uint16_t RightGunState;
    uint16_t LeftGunState;
    uint16_t ChargerState;
}IOTStruct;

typedef enum
{
    InOperative=0,
    Operative,
}ContactorOperationStateEnum;
typedef enum
{
    NotCharging=0,
    SlowChargeing,
    FastCharging,
}ChargingTypeEnum;
typedef enum
{
    IMD_Disable=0,
    IMD_Enable,
}IMDstateTypeEnum;
typedef struct
{
    //inputs for CMS
    //++++++++++++++++++++++ VSECC,RNL SECC SECTION  +++++++++++++++++++
    float ChargingProfileMaxPowerLimit;
    float TimeToFullSoC;

    float EVMinVoltage;
    float EVMinCurrent;
    float EVMinPower;
    float EVMaxVoltage;
    float EVMaxCurrent;
    float EVMaxPower;

    uint16_t PowerReallocCounter;
    uint8_t powerflag:1;
    float EVMAXPowerReallocate;
    uint8_t PowerReallocFlag;
    float CurrentChangeFactor;
    uint8_t CurrentChangeCounter;
    chargeMode_t ChargeMode;

    bool TargetContactorState;
    evConnectionState_t EVConnectionState;
    chargingState_t EVChargingState;
    float TargetVoltage;
    float TargetCurrent;
    uint8_t VehicleSoC;
    float CableCheckVoltage;
    bool ResetRquest;
    uint64_t VehicleID;
    bool CableCheckFalg;
    float EVTargetPower;

    //++++++++++++++++++++++Tonhe SECTION  +++++++++++++++++++
    ChargeModuleStateEnum ChargeModuleState[3];
    float MeasuredVoltage;   /* input and output as well*/
    float MeasuredCurrent[3];   /* input and output as well*/
    float TotalMeasuredCurrent;
    uint16_t FaultAlarm;
    uint8_t PFCFault;
    uint8_t StartStopFB;
    float RectPhaseAVoltage;
    float RectPhaseBVoltage;
    float RectPhaseCVoltage;
    float TotalMeasuredVoltage;
    float RectAmbiantTemp[3];
    uint8_t ModuleState[3];
    uint16_t ExtensionFaultWarning;

    // DC EMMeter
    float DCEMVoltage;
    float DCEMCurrent;
    float DCEMStartTime;
    float DCEMStartEnergy;
    float DCEMEndTime;
    float DCEMEndEnergy;
    float DiffEnergy1;
    float DiffEnergy2;
    float DiffEnergy;
    float DCEMDeltaValue;
    float DCEMEnergyReading;
    //IMD
    float InsulationResistance;

    //++++++++++++++++++++++CMS Board SECTION  +++++++++++++++++++

    float BusBarTemperature;            // Done
    float GunPosTemperature;
    float GunNegTemperature;

    ContactorWrite PositiveContactorFB;  // Done
    ContactorWrite NegativeContactorFB;  // Done
    bool GunFlowMeterFB;            // Done
    bool GunPresecnce;              // Done -Emulated
    bool GunVoltagePresence;        // Done
    bool GunSolenoidFB;             // Done -Emulated -Will be removed in future, Not Clear
    bool StopChargeUserButton;     //Done
    uint8_t StopChargeUserCount;
    bool IMDFault;                 // Done   0 means Fault 1means no fault
    bool MotorBallValveFB;          // Done -Emulated with switch

    // output
    uint16_t FlowPWMEdgeCount;      //Flowmeter count using gpio interrupt
    float FlowRate;
    bool IMDEnable;
    uint8_t CodeGunStatus;

    //++++++++++++++++++++++OutPut to SECC SECTION  +++++++++++++++++++
    contactorsStatus_t ContactorState;
    operationalStatus_t PECCOperationState;
    isolationStatus_t IsolationState;
    float DrivenVoltage;
    float DrivenCurrent;
    float TempratureToVsecc;
    float VseccMeasuredVoltage;
    float VseccMeasuredCurrent;


    float VseccMinVoltage;
    float VseccMinCurrent;
    float VseccMinPower;

    float VseccMaxVoltage;
    float VseccMaxCurrent;
    float VseccMaxPower;

    float AAGValue;
    uint8_t PLC_Start;
    uint8_t Max_Curr_Flag;
    uint16_t Battery_State_Of_Charge;
    uint8_t Targ_Curr_Flag;
    uint8_t VehicleReset;

    //++++++++++++++++++++++Output to Tonhe SECTION  +++++++++++++++++++
    ContactorWrite RectState;
    float RectDriveVoltage;
    float RectDriveCurrent;
    uint8_t ParallelRect;
    uint32_t IOTState;
    uint8_t IMDCount;

    //++++++++++++++++++++++Output to CMS SECTION  +++++++++++++++++++++
    ContactorOperationStateEnum ContactorOperationState;
    ContactorWrite PositiveContactorDrive;            // Done
    ContactorWrite NegativeContactorDrive;            // Done
    ContactorWrite PrechargeContactorDrive;            // Done
    ChargingTypeEnum ChargingSpeed;
    IMDstateTypeEnum IMDState;

    uint32_t DisplayMsg;    /* not clear yet*/

    uint16_t CANAFaultCounter;
    uint16_t CANBFaultCounter;

    bool DisplayPowerEnable;                 // Done
    bool EnergyMeterPowerEnable;            // Done

    bool PowerLED;
    bool StatusLED;                         // Done //it is addressable LED
    bool StopChargeLED;                     // Done inbuilt with Stop Charge Button
    bool StopCharge;                        //CMS stop charge
    bool StartCharge;                       //CMS start Charge
    uint8_t GunStateFault;

    bool MotBallValveIN1;                      // Done -Emulated using LED
    bool MotBallValveIN2;                      // Done -Emulated using LED
    bool GunSolenoideDriveIN2;                // Done  not using
    bool GunSolenoideDriveIN1;                // Done  not using

    uint8_t GunNum;
    //++++++++++++++++++++++IOT SECTION  +++++++++++++++++++++
    bool IOTstartCharge;
    bool IOTstopCharge;
    bool IoTGunActivation;


    //++++++++++++++++++++++FAULT SECTION  +++++++++++++++++++++
    bool GunFault;
    bool PositiveContactorFault;
    bool NegativeContactorFault;

    bool GunIMDFault;
    bool IMDResFault;

    bool CANAPerFault;
    bool CANBPerFault;
    bool CANTempFault;

    bool TempFault;
    bool ParmanantFault;
}GunStruct;

typedef struct
{
    // INPUT
    // ACEM
    float PhaseVoltage[3];
    float PhaseCurrent[3];
    float PhasePower[3];
    float PhaseEnergy[3];
    float frequency;
    float PowerFactor;
    float TotalActiveEnergy;

    float StartEnergy;
    float EndEnergy;
    float StartTime;
    float EndTime;

    //CMS
    float  EnvAmbTemp;            // Done
    float PCBTemperature;               // Done
    float FluidTemperature;            // Done

    bool SmokeSensor;                   // Done  -Emulated same emergency switch
    bool ELRFB;                         // Done
    EmergencyButtonStatus EmergencyButton;  // Done HW Connection to AC Contactor
    bool SPDFB;                             // Done
    ContactorWrite ACContactorFB;  // Done
    ContactorWrite ACContactorCoilFB;  // Done
    ContactorWrite CrossContactorPositiveFB;                // Done //DC_CONT_FB_PL_HI_MCU
    ContactorWrite CrossContactorNegativeFB;                // Done //DC_CONT_FB_PL_HI_MCU
    DoorStatus FrontDoor;  // Done ->  like stop button->AC Contactor
    DoorStatus RearDoor;  // Done ->  like stop button->AC Contactor
    ContactorOperationStateEnum CrossConteactorOperationState;

    // CMS Output  lb
    bool CoolingFanControl;                 // Done  -> Radiator Fan LED emulation
    bool ExhuastFanControl;                 // Done  -> Fans near to Rect.
    bool CoolingPumpPowerControl;           // Done  -> POWER Enable Pin
    uint8_t CANFaultClear;
    ContactorWrite ACContactorDrive;        // Done
    ContactorWrite CrossContactorDrive;            // Done
    uint16_t DIOFaliureCount;
    PLCCommand VSECCPowerEnable;                // Done

    bool StationLED;                            // Done
    bool DebugLEDRed;                          // Done
    bool DebugLEDBlue;                         // Done
    GunStatus GunInUse;

    bool ChargerFault;

    bool VoltageBalanceFault;
    bool CurrentBalanceFault;

    bool VoltageLimitFault;
    bool CurrentLimitFault;

    bool FrequncyLimitFault;
    bool PowerFactorLimit;
    bool EMButtonFault;
    bool SPDFault;
    bool ELRFBFault;
    bool ACContactorFault;
    uint8_t TempFaultCounter;
    bool TempFault;
    bool ParmanantFault;
    bool IoTResetRequest;
    bool ACEMCommFault;
    bool CMSDIOInFault;
    bool CrossContactorFault;
    bool CrossContactorWeldFault;
    bool BatteryFailure;
    bool DCOk;
    bool waterlevelsensor;
    uint8_t powerallocfault;
}ChargerStruct;

typedef struct
{
    uint8_t ACEMFaultCount;
    uint8_t DCEM1FaultCount;
    uint8_t DCEM2FaultCount;
    uint8_t IMD1FaultCount;
    uint8_t IMD2FaultCount;
    uint8_t ELRfaultCounter;

    bool ACEMFault;
    bool DCEM1Fault;
    bool DCEM2Fault;
    bool IMDM1Fault;
    bool IMDM2Fault;
}FaultAndCounterStruct;


typedef union
{
    uint64_t AllTemprature;
    struct
    {
        uint16_t RightGunPositive:8;
        uint16_t RightGunNegetive:8;
        uint16_t LeftGunPositive:8;
        uint16_t LeftGunNegetive:8;
        uint16_t BusBar1:8;
        uint16_t BusBar2:8;
        uint16_t FluidTemp:8;
        uint16_t EmbTemp:8;
    }TempValues;
}CANTEMPStruct;


typedef enum
{
    DIOSuccess=0,
    DIOBouncingError=1,

}DIOStatus_t;
// Function  to get input
int8_t GetEVDataRNL(GunStruct *GunNumber,ReceiveFromRMLStruct *rnl,RNLMSGCTRLEnum *RNLCTRL,SendToRNLStruct *rnlSend,RNLFlagEnum *FLAG);
int8_t GetEVDataBCL(GunStruct *GunNumber,ReceiveFromRMLStruct *rnl,RNLMSGCTRLEnum *RNLCTRL,SendToRNLStruct *rnlSend,RNLFlagEnum *FLAG);
int8_t GetEVDataBST(GunStruct *GunNumber,ReceiveFromRMLStruct *rnl,RNLMSGCTRLEnum *RNLCTRL,SendToRNLStruct *rnlSend,RNLFlagEnum *FLAG);
//void RML_ProcessCANMsg(uint32_t can_id, uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData,RNLFlagEnum *FLAG,GunNumberType GunX);
void GetRectStatusInfo(GunStruct *GunNumber,CMSReceiveFromTonheStruct *Temp);
void GetDCEMValue(GunStruct * GunNUmber, DC_EnergyMeterDataStruct *Temp);
void GetDCEMStartEnergy(GunStruct * GunNumber, DC_EnergyMeterDataStruct *Temp);
void GetDCEMEndEnergy(GunStruct * GunNumber, DC_EnergyMeterDataStruct *Temp);
void GetDCEMStartTime(GunStruct * GunNumber,uint32_t time);
void GetDCEMStopTime(GunStruct * GunNumber,uint32_t time);
void GetIMDData(GunStruct * GunNumber, float Resistance);
void GetDigitalInputData(uint8_t *I2Cstatus,uint16_t *Data1, uint16_t *Data2);
DIOStatus_t DigitalInputData(GunStruct * GunNumber1,GunStruct * GunNumber2,ChargerStruct *ChargerData,uint16_t tempio1,uint16_t tempio2);
void GetACEMData(ChargerStruct *ChargerData,AC_EnergyMeterDataStruct *ACEMData);
void GetAnalogInputData(GunStruct *GunData1,GunStruct *GunData2,ChargerStruct *ChargerAnalog,ChargerAnalogReadStruct *ChargerAnalogRead);
void GetChargerADCData();

// Function to write output

void SendtoVsecc(GunNumberType GunX,GunStruct *GunNumber);
void SendToRNL(GunNumberType GunX,GunStruct *GunNumber,RNLMSGCTRLEnum *RNLCTRL,SendToRNLStruct *rnl);
void SendCMSCANMsg();
void SetRectVoltageCurrent(GunNumberType  GunNumber,GunStruct *GunData);
void RectAliveCommand(GunNumberType GunNumber);
void SetChargerParameter(ChargerStruct *ChargerData);
void SendToIOT(GunStruct *GunNumber,uint32_t msg);    // Need Info
void SendToDisplay(GunStruct *GunNumber,uint32_t msg);      // Need Info
void AddresableLEDValue(GunStruct *GunNumber,uint32_t AddressableLED);  //NeedInfo
//void SetChargeLED(GunStruct *GunNumber, bool LEDValue);         //NeedInfo
void SetStatusLED(GunStruct *GunNumber, bool LEDValue);
//void SetAddLED(GunStruct *GunNumber, bool LEDValue);     will do later
void SetStopChargeLED(GunStruct *GunNumber, bool LEDValue);
void SetMotBallValveIN12(GunStruct *GunNumber, bool Value1, bool Value2);
void SetGunSolenoideDriveIN12(GunStruct *GunNumber, bool Value1, bool Value2);
void StopCharging(GunNumberType Gun,SendToRNLStruct *rnl,RNLMSGCTRLEnum *RNLCTRL);

// Function for contactors
void SetACContactor(ContactorWrite Pos);
void SetGunContactor(GunNumberType GunX, ContactorWrite Pos, ContactorWrite Neg, ContactorWrite Precharge);
void SetCrossContactor(ContactorWrite Value);
void ResetGunContactor(GunNumberType Gun,GunContactortype x);

// Function for initializing values for CAN, other init values
void ChargerSetInitValues();
void GunVarInitValues(GunStruct *PrimaryGun);
void ChargerContectorInitValues();
void DCEMPower(DCEMNumber number, PowerEnable value);
void DisplayPower(GunNumberType Gunx,PowerEnable value);
void PLCPower(PowerEnable value);
void RectifierOnOffCtrl(GunNumberType Gunx,PowerEnable value);
void StatusLED(GunNumberType Gunx,PowerEnable value);
void CoolingFan(PowerEnable value);
void GeneralGPIO(uint32_t GPIOPindef1,uint32_t GPIOPindef2,GunNumberType Gunx,PowerEnable value);
void StationLED(PowerEnable value);
void ExhaustFan(PowerEnable value);
void PumpPower(PowerEnable value);
void GunFault(GunStruct *Gun);
void StopChargingGun(bool RGSC,bool LGSC,RNLMSGCTRLEnum *RNLRightMSGCTRL,SendToRNLStruct *RNLRightGunSend,RNLMSGCTRLEnum *RNLLeftMSGCTRL,SendToRNLStruct *RNLLeftGunSend);
void StopLED(GunNumberType Gunx,PowerEnable value);
void Charger_FastChargeMSG(GunStruct *uy_PriGun, GunStruct *uy_SecGun);
void Charger_SlowChargeMsg(GunStruct *uy_PriGun);
//++++++++++++++++++++++ BOOTLOADER FIRMWARE UPDATE FUNCTION FOR OTA +++++++++++++++++++++
void BootISR(void);

void SendRNLMessageVehicleStatus(GunStruct *Gun,status Vehicle,uint8_t GunNumber);
void SendRNLMessagePECCStatus1(GunStruct *Gun,PeccStatus1 PS1,uint8_t GunNumber);
void SendRNLMessageStopCharge(GunStruct *Gun,uint8_t GunNumber);
void SendRNLMessageVehicleId(GunStruct *Gun,uint8_t GunNumber);
void SendRNLMessagePECCStatus2(GunStruct *Gun1,GunStruct *Gun2,PeccStatus2 PS2,uint8_t GunNumber);
void SendRNLMessagePECCLimits1(GunStruct *Gun,PeccLimits1 PL1,uint8_t GunNumber);
void SendRNLMessagePECCLimits2(GunStruct *Gun,PeccLimits2 PL2,uint8_t GunNumber);
void SendRNLMessageChargeSessionInfo1(GunStruct *Gun,ChargeSessionInfo1 CSI1,uint8_t GunNumber);
void SendRNLMessageChargeSessionInfo2(GunStruct *Gun,ChargeSessionInfo2 CSI2,uint8_t GunNumber);
void SendRNLMessageChargeSessionInfo3(GunStruct *Gun,ChargeSessionInfo3 CSI3,uint8_t GunNumber);
void SendRNLMessageReset(GunStruct *Gun,uint8_t GunNumber);
#endif /* CHARGERLEVEL_CHARGER_H_ */
