/*
 * TonheCAN.h
 *
 *  Created on: Feb 20, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

#ifndef TONHECAN_H_
#define TONHECAN_H_

#include <stdint.h>
#include "J1939.h"  // Use your existing J1939 structure

#include "MCUSpecific.h"
// ===========================6
// Tonhe-Specific J1939 Macros
// ===========================

// Master and Charging Module Addresses (from Tonhe PDF)
#define TONHE_MASTER_ADDRESS     0xA0  // Master Control Default Address
#define TONHE_CHARGING_BROADCAST 0xFF  // Broadcast to all Charging Modules


/*
#define RECTIFIERSET_PROCESS_FLAG 7
#define MODULE1_PROCESS_FLAG 1
#define MODULE2_PROCESS_FLAG 2
#define MODULE3_PROCESS_FLAG 4*/


// ===========================
// PGN Definitions for uplink and downlink Data(from Tonhe PDF)
// ===========================
//From Tonhe to CMS
#define PGN_MODULE_STATE         0x0001A0  // Charging Module State
#define PGN_MODULE_FAULT_INFO    0x0091A0  // Extended State/Fault Info
#define PGN_START_STOP_CONFIRM   0x0002A0  // Module Start/Stop Confirmation
#define PGN_AC_INFO_UPLOAD       0x000BA0  // AC Information Upload


#define PGN_START_STOP           0x000300  // Module Start/Stop Command
#define PGN_PARAMETER_SETTING    0x000400  // Module Parameter Setting
#define PGN_TIMING_COMMAND       0x000500  // Timing Command
#define PGN_INPUT_MODE_CONFIG    0x00AA00  // Input Mode Configuration
#define PGN_MODULE_ADDR_SET      0x000900  // Module Address Setting


// ===========================
// Message Priorities (from Tonhe PDF)
// ===========================

#define PRIORITY_CHARGINGSTATE                  6  // Charging module state
#define PRIORITY_STARTSTOP_OVUUSET_CONFIRM      2 // Specific Module start stop
#define PRIORITY_ACINFO                         6  // Charging module state
#define PRIORITY_FAULT                          7  // Fault messages


#define PRIORITY_STARTSTOP                      2  // Start/Stop
#define PRIORITY_MODULE_PARA_SET                4  // Parameter settings
#define PRIORITY_TIMING                         6  // Timing command
#define PRIORITY_INPUT_MOD_CONFIG               6  //INPUT MODE CONFIG
#define PRIORITY_ADDRESS_SET_SEL                7  //ADDRESS SET SELECTION
#define PRIORITY_MODULE_ADDRESS_SET             6  //MODULE_ADDRESS_SETTING
#define PRIORITY_STARTSTOP_OVUUSET_SET          2  //INPUT MODE CONFIG

/**
 *  This definition is used to determine the rectifier is ON or OFF that will
 *  set up via a call to the TonheStartStopCMD().
 */
typedef enum
{
    RectifierOn = 0xAA,
    RectifierOff = 0x55,
}RectiferStartStopEnum;


// Charging Module State Message Structure (PGN: 0x000100)
/**
 *  This definition is used to determine the Module state that will be
 *  set up by via call to ProcessModuleState() API.
 */
typedef struct {
    uint8_t  ModuleState;    // Byte 1: Module State (0x00 = OFF, 0x01 = ON, 0x11 = Fault OFF)
    float OutputVoltage;     // Bytes 2-3: Output Voltage (0.1V per bit)
    float OutputCurrent;     // Bytes 4-5: Output Current (0.01A per bit)

    union { // Fault/Alarm Flags
        uint16_t FaultFlags;
        struct {
            uint16_t InputUndervoltage   : 1;  // Bit 0
            uint16_t InputPhaseLoss      : 1;  // Bit 1
            uint16_t InputOvervoltage    : 1;  // Bit 2
            uint16_t OutputOvervoltage   : 1;  // Bit 3
            uint16_t OutputOvercurrent   : 1;  // Bit 4
            uint16_t HighTemperature     : 1;  // Bit 5
            uint16_t FanFault            : 1;  // Bit 6
            uint16_t HardwareFault       : 1;  // Bit 7
            uint16_t BusException        : 1;  // Bit 8
            uint16_t SCICommException    : 1;  // Bit 9
            uint16_t DischargeFault      : 1;  // Bit 10
            uint16_t PFCShutdown         : 1;  // Bit 11
            uint16_t OutputUndervoltage  : 1;  // Bit 12
            uint16_t OutputOvervoltageW  : 1;  // Bit 13
            uint16_t PowerLimitTemp      : 1;  // Bit 14
            uint16_t ShortCircuit        : 1;  // Bit 15
        };
    };

    union { // PFC Fault Flags
        uint8_t PfcFault;
        struct {
            uint8_t InputOvercurrent  : 1;  // Bit 0
            uint8_t MainsFreqFault    : 1;  // Bit 1
            uint8_t MainsImbalance    : 1;  // Bit 2
            uint8_t DCTzFault         : 1;  // Bit 3
            uint8_t AddressConflict   : 1;  // Bit 4
            uint8_t BusBias           : 1;  // Bit 5
            uint8_t PhaseException    : 1;  // Bit 6
            uint8_t BusOvervoltage    : 1;  // Bit 7
            uint8_t RSV               : 8;  // All other
        };
    };
}ChargingModuleStateStruct;

/**
 *  This definition is used to determine the Module status stop/start that will be
 *  set up by via call to ProcessModuleStartStopConfirm() API.
 */
typedef struct {
    uint8_t MessageStatus;   // Byte 1: Message Receive Status (Confirmation)
    uint8_t Reserved[7];     // Bytes 2-8: Reserved (Always 0)
} ModuleStartStopConfirmationStruct;

/**
 *  This definition is used to determine the AC information that will be
 *  set up by via call to ProcessACInfo() API.
 */
typedef struct {
    float APhaseVoltage;      // Bytes 1-2: AC Input Voltage (0.1V per bit)
    float BPhaseVoltage;      // Bytes 3-4: AC Input Current (0.01A per bit)
    float CPhaseVoltage;    // Bytes 5-6: AC Input Frequency (0.1Hz per bit)
    float AmbientTemperature;    // Bytes 7-8: Reserved (Always 0)
}AcInformationStruct;

/**
 *  This definition is used to determine the Module state and Fault warning that will be
 *  set up by via call to ProcessModuleFaultInfo() API.
 */
typedef struct {
    union {
        uint16_t ModuleState;  // Bytes 1-2: Raw module state
        struct {
            uint16_t CurrentEqualization : 1; // Bit 0
            uint16_t Mute : 1;                // Bit 1
            uint16_t E2FaultOverflow : 1;     // Bit 2
            uint16_t InputSource : 1;         // Bit 3 (0 = AC, 1 = DC)
            uint16_t E2FaultDisabled : 1;     // Bit 4 (0 = Enabled, 1 = Disabled)
            uint16_t HotPlugEnabled : 1;      // Bit 5 (0 = Disabled, 1 = Enabled)
            uint16_t Reserved : 10;           // Bits 6-15 (Unused)
        } ModuleStateBits;
    };

    union {
        uint16_t FaultWarning;  // Bytes 3-4: Raw fault/warning status
        struct {
            uint16_t PreLevelWaveStop : 1;        // Bit 0
            uint16_t HotPlugFault : 1;           // Bit 1
            uint16_t CANTimeout : 1;             // Bit 2
            uint16_t Reserved1 : 1;              // Bit 3 (Reserved)
            uint16_t RelayOperationFault : 1;    // Bit 4
            uint16_t Reserved2 : 1;              // Bit 5 (Reserved)
            uint16_t InternalOvertemperature : 1;// Bit 6
            uint16_t AirInletOvertemperature : 1;// Bit 7
            uint16_t InputPowerLimit : 1;        // Bit 8
            uint16_t PowerLimitOvertemperature : 1;// Bit 9
            uint16_t DischargeChangeoverAbnormal : 1; // Bit 10
            uint16_t Reserved3 : 5;              // Bits 11-15 (Unused)
        } FaultWarningBits;
    };

    uint8_t Reserved[4];  // Bytes 5-8: Reserved (Always 0)
} ExtensionStatusFaultStruct;

/**
 *  This definition is used to determine the where to store the data that will be
 *  set up by via call to TonheProcessRxCANMsg() API.
 */
typedef struct
{
    ChargingModuleStateStruct ChargingModuleState;
    ModuleStartStopConfirmationStruct ModuleStartStopConfirmation;
    AcInformationStruct AcInformation;
    ExtensionStatusFaultStruct ExtensionStatusFault;
}CMSReceiveFromTonheStruct;

typedef enum{
    Data_Coming = 0X0,
    Not_Updating = 0x01,
    Communication_lost = 0x02,
}Tonhe_StatusMode_t;

typedef struct{
    Tonhe_StatusMode_t Tonhe_RightRectifier_1;
    Tonhe_StatusMode_t Tonhe_RightRectifier_2;
    Tonhe_StatusMode_t Tonhe_RightRectifier_3;
    Tonhe_StatusMode_t Tonhe_LeftRectifier_1;
    Tonhe_StatusMode_t Tonhe_LeftRectifier_2;
    Tonhe_StatusMode_t Tonhe_LeftRectifier_3;
}RectifierStatus_t;


// ===========================
// Function Declarations
// ===========================
// It set up the Transmit CAN object
void TonheTXCANOBJSet(uint32_t BaseAddress);

// It set up the receive CAN object
void TonheRXCANOBJSet(uint32_t CANBASE, uint32_t OBJECT);

// Generate a 29-bit CAN ID for Tonhe messages
uint32_t TonheSetCANID(uint8_t priority, uint32_t pgn, uint8_t da, uint8_t sa);

// Send a Start/Stop command
void TonheStartStopCMD(uint32_t CANBASE,RectiferStartStopEnum command);
// Send a parameter setting message
void TonheSetParameter(uint32_t CANBASE,float voltage, float current);

// Send a timing command
void TonheSendTimingCommand(uint32_t CANBASE);
// Process incoming CAN messages
void TonheProcessRxCANMsg(uint32_t can_id, uint8_t *data, uint8_t length, CMSReceiveFromTonheStruct *TonheRxCANMsg, bool Tonhe_CANDifferentiate);

// Process module state information from the received data buffer
void ProcessModuleState(uint8_t *data, uint8_t length,
                        ChargingModuleStateStruct *moduleState);

// Process module state and fault information from the received data buffer
void ProcessModuleFaultInfo(uint8_t *data, uint8_t length,
                            ExtensionStatusFaultStruct *faultInfo);

// Process module start/stop status
void ProcessModuleStartStopConfirm(uint8_t *data, uint8_t length,
                                   ModuleStartStopConfirmationStruct *confirmStatus);

// Process AC information
void ProcessACInfo(uint8_t *data, uint8_t length,
                   AcInformationStruct *acInfo);

void TONHECANCyclicCheck(RectifierStatus_t Tonhe_Status);

void CheckTonheOperation(uint8_t *count,Tonhe_StatusMode_t *status,uint8_t *Ireg_check);
#endif /* APPLICATIONLAYER_TONHECAN_H_ */
