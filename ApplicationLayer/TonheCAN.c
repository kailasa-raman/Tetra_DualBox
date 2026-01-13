/*
 * TonheCAN.c
 *
 *  Created on: Feb 20, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */
#include "TonheCAN.h"

// ===========================
// Function Implementations
// ===========================

/**
  *  TonheTXCANOBJSET
  *
  *  @brief Configures the CAN transmit objects and 29-bit CAN ID.
  *
  *  This function sets up multiple CAN message objects using a 29-bit CAN ID to send messages to the MCU.
  *  Each message object is configured for a specific CAN ID and uses an extended frame format.
  *  The CAN ID is set using TonheSetCANID().
  *
  *  The function calls `CANMSGObjectSetup()` to initialize three transmit message objects:
  *   -`CANOBJ_TONHE_STARTSTOP`: Transmits messages(Module start/stop command) to MCU.
  *   -`CANOBJ_TONHE_PARAMETERSETTING` : Transmits messages(Module Parameter Setting) to MCU.
  *   -`CANOBJ_TONHE_TIMINGCOMMAND` : Transmits messages(Timing Command) to MCU.
  *
  *   Each message object configured to transmit Extended frame format.
  *
  *   @param uint32_t CANBASE   - Base address of the CAN
  *   @param uint8_t ObjectID   - Message Object ID
  *   @param uint32_t BaseID    - Message Identifier (CAN ID)
  *   @param uint8_t FrameType  - Type of CAN frame, Standard (11-bit ID) or Extended (29-bit ID)
  *   @param uint8_t ObjectType - Type of Message Object, transmission (TX) or reception (RX)
  *   @param uint32_t MaskID    - Mask Identifier for Filtering
  *   @param uint32_t IntName   - Interrupt Configuration
  *
  *   @return None.
  */
void TonheTXCANOBJSet(uint32_t CANBASE)
{
    uint32_t can_id;
    can_id = TonheSetCANID(PRIORITY_STARTSTOP, PGN_START_STOP,
                      TONHE_CHARGING_BROADCAST,TONHE_MASTER_ADDRESS);

    CANMSGObjectSetup(CANBASE,CANOBJ_TONHE_STARTSTOP,
                      can_id,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);

    can_id = TonheSetCANID(PRIORITY_MODULE_PARA_SET, PGN_PARAMETER_SETTING,
                           TONHE_CHARGING_BROADCAST,TONHE_MASTER_ADDRESS);

    CANMSGObjectSetup(CANBASE,CANOBJ_TONHE_PARAMETERSETTING,
                      can_id,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);


    can_id = TonheSetCANID(PRIORITY_TIMING, PGN_TIMING_COMMAND,
                           TONHE_CHARGING_BROADCAST, TONHE_MASTER_ADDRESS);

    CANMSGObjectSetup(CANBASE,CANOBJ_TONHE_TIMINGCOMMAND,
                      can_id,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);
}

/**
 *  TonheRXCANOBJSet
 *
 *  @brief Configures a CAN receive message object.
 *
 *  This function configures a CAN message object to receive messages from Tonhe Module.
 *  Each message object is set up with an extended frame format (29-bit ID) and ID filtering enabled.
 *  It also enables the receive interrupt to handle incoming messages.
 *
 *  @param CANBASE    - Base address of the CAN module.
 *  @param OBJECT     - Message Object ID to be configured.
 *
 *  @return None.
 */
void TonheRXCANOBJSet(uint32_t CANBASE, uint32_t OBJECT)
{
    CANMSGObjectSetup(CANBASE,OBJECT,
                      RECTIFIER_BASEID,ExtendedFrame,
                      CANOBJECTTYPERX,RECTIFIER_IDFILTER,
                      CAN_MSG_OBJ_RX_INT_ENABLE|CAN_MSG_OBJ_USE_ID_FILTER|CAN_MSG_OBJ_USE_EXT_FILTER);
}
// Generate a 29-bit CAN ID using J1939 structure

/**
 *  TonheSetCANID
 *
 *  @brief Generates a 29-bit CAN ID using the J1939 structure.
 *
 *  This function constructs a 29-bit CAN ID following the J1939 protocol by combining
 *  the priority, Parameter Group Number (PGN), destination address (DA),
 *  and source address (SA). The PDU Format (PF) is extracted from the PGN to
 *  determine whether the message is peer-to-peer (PF < 240) or broadcast (PF ≥ 240).
 *
 *  @param priority - 3-bit priority field (higher priority messages have lower values).
 *  @param pgn      - 24-bit Parameter Group Number (PGN), which contains PF, PS, and reserved bits.
 *  @param da       - 8-bit Destination Address.
 *  @param sa       - 8-bit Source Address.
 *
 *  @return 29-bit CAN ID.
 */
uint32_t TonheSetCANID(uint8_t priority, uint32_t pgn, uint8_t da, uint8_t sa)
{
    J1939_Union j1939;
    pgn &= 0x3FFFF;
    // Extract PF from PGN
    uint8_t pf = (pgn >> 8) & 0xFF;

    // Set J1939 fields
    setJ1939ID(&j1939, priority, pf, da, sa);

    return j1939.identifier;
}

// Send a Start/Stop command
/**
 *  TonheStartStopCMD
 *
 *  @brief Sends a Start/Stop command from the MCU to the Tonhe module.
 *
 *  This function is used to send a Start or Stop command from the MCU to the Tonhe
 *  module using CAN communication via the CANSendMessage function.
 *
 *  @param CANBASE - Base address of the CAN module.
 *  @param command - Start/Stop command (0x55 for Stop, 0xAA for Start).
 *
 *  @return None.
 */
void TonheStartStopCMD(uint32_t CANBASE, RectiferStartStopEnum command) {

    uint64_t tempx;
    tempx = 0x000000000FFFFFF;
    tempx =  tempx | ((uint64_t)command <<24);

    // Send CAN Message
    //CANSendMessage()
    CANSendMessage(CANBASE,CANOBJ_TONHE_STARTSTOP,&tempx,8);
}

// Send a parameter setting message
/**
 *  TonheSetParameter
 *
 *  @brief Sends a parameter setting message to Tonhe from the MCU.
 *
 *  This function is used to send voltage and current parameters to the Tonhe module
 *  from the MCU via CAN communication using the CANSendMessage function.
 *
 *  @param CANBASE  - Base address of the CAN module.
 *  @param voltage  - Voltage value to be set (0.1V per bit).
 *  @param current  - Current value to be set (0.01A per bit).
 *
 *  @return None.
 */
void TonheSetParameter(uint32_t CANBASE,float voltage, float current) {
    uint16_t data[8] = {0};
    uint16_t temp;
    // Set module flags
    data[0] = TONHE_CHARGING_BROADCAST;
    data[1] = TONHE_CHARGING_BROADCAST;
    data[2] = TONHE_CHARGING_BROADCAST;

    // Module group number  + address multiple upper lower nibble based
    data[3] = 0x00;

    // Set voltage (0.1V/bit) and current (0.01A/bit)
    temp = (uint16_t) (voltage * 10);
    data[4] = (temp & 0xFF);
    data[5] = (temp >> 8);
    temp =(uint16_t) (current *100);
    data[6] = (temp & 0xFF);
    data[7] = (temp >> 8);
    uint8_t i=0;
    uint64_t temp2=0;
    for (i = 0; i < 8; i++) {
        temp2 |= ((uint64_t)(data[i] & 0x00FF)) << (i * 8);
        }

    // Send CAN Message (Replace with actual CAN send function)
    // can_send(can_id, data, 8);
    CANSendMessage(CANBASE,CANOBJ_TONHE_PARAMETERSETTING,&temp2,8);
}

// Send a timing command
/**
 *  TonheSendTimingCommand
 *
 *  @brief Sends a timing command to Tonhe from the MCU.
 *
 *  This function transmits a **timing command** to the **Tonhe module** over CAN communication.
 *  The timing command is used to ensures that multiple charging modules operate in sync.
 *
 *  @param CANBASE  Base address of the CAN module.
 *
 *  @return None.
 */
void TonheSendTimingCommand(uint32_t CANBASE) {
    //uint8_t data[8] = {0};
    // Send CAN Message (Replace with actual CAN send function)
    // can_send(can_id, data, 8);
    uint64_t temp2=0;
    //memcpy(&temp2,&data,8);
    CANSendMessage(CANBASE,CANOBJ_TONHE_TIMINGCOMMAND,&temp2,8);
}

// Process incoming CAN messages
/**
 *  TonheProcessRxCANMsg
 *
 *  @brief Processes incoming CAN messages from Tonhe modules.
 *
 *  This function extracts data from received CAN messages and stores it in the
 *  CMSReceiveFromTonheStruct structure. It processes information related to:
 *  - Charging module state
 *  - Extension status and faults
 *  - Module start/stop confirmation
 *  - AC information
 *
 *  The function determines where to store the received data based on the
 *  source address (CAN ID).
 *
 *  @param can_id          The CAN message identifier (Source Address).
 *  @param data            Pointer to the received data buffer.
 *  @param length          Length of the received data in bytes.
 *  @param TonheRxCANMsg   Pointer to the structure where extracted data is stored.
 *
 *  @return None.
 */
void TonheProcessRxCANMsg(uint32_t can_id, uint8_t *data, uint8_t length, CMSReceiveFromTonheStruct *TonheRxCANMsg, bool Tonhe_CANDifferentiate)
{
    uint8_t SA;
    uint32_t pgn;
    pgn = ExtractPGN(can_id,&SA);
    SA =  ((uint8_t)(can_id & 0xFF) -1);

    switch (pgn)
    {
        case PGN_MODULE_STATE:
            ProcessModuleState((uint8_t *)data, length, &TonheRxCANMsg[SA].ChargingModuleState);
            break;

        case PGN_MODULE_FAULT_INFO:
            ProcessModuleFaultInfo((uint8_t *)data, length, &TonheRxCANMsg[SA].ExtensionStatusFault);
            break;

        case PGN_START_STOP_CONFIRM:
            ProcessModuleStartStopConfirm((uint8_t *)data, length, &TonheRxCANMsg[SA].ModuleStartStopConfirmation);
            break;

        case PGN_AC_INFO_UPLOAD:
            ProcessACInfo(data, length, &TonheRxCANMsg[SA].AcInformation);
            break;

        default:
            // Unknown PGN
            break;
    }
}

/**
 *  ProcessModuleState
 *
 *  @brief Processes and extracts module state information from the received data buffer.
 *
 *  This function extracts **charging module state information** from the received data buffer
 *  and stores it in the **ChargingModuleStateStruct** structure.
 *
 *  If the received data length is **less than 8 bytes**, the function **exits early** to
 *  prevent invalid processing.
 *
 *  @param data         Pointer to the received data buffer.
 *  @param length       Length of the received data in bytes.
 *  @param moduleState  Pointer to the ChargingModuleStateStruct where the extracted data is stored.
 *
 *  @return None.
 */
void ProcessModuleState(uint8_t *data, uint8_t length, ChargingModuleStateStruct *moduleState)
{
    if (length < 8) return; // Ensure data length is valid

    moduleState->ModuleState   = data[0];                                // Byte 1: Module State
    moduleState->OutputVoltage = (float)((uint16_t)(data[1] | (data[2] << 8)))* 0.1f;  // Bytes 2-3: Voltage (0.1V per bit)
    moduleState->OutputCurrent = (float)((uint16_t)(data[3] | (data[4] << 8))) * 0.01f; // Bytes 4-5: Current (0.01A per bit)
    moduleState->FaultFlags    = (uint16_t)(data[5] | (data[6] << 8));   // Bytes 6-7: Fault Flags
    moduleState->PfcFault      = data[7];                                // Byte 8: PFC Fault Flags
}

/**
 *  ProcessModuleFaultInfo
 *
 *  @brief Processes and extracts module state and fault information from the received data buffer.
 *
 *  This function extracts **module state information** and **fault warnings** from the received
 *  data buffer and stores them in the **ExtensionStatusFaultStruct** structure.
 *
 *  - The **module state information** is stored in the `Modulestate` union.
 *  - The **fault warning information** is stored in the `FaultWarning` union.
 *
 *  Both unions are part of the **ExtensionStatusFaultStruct** structure.
 *
 *  If the received data length is **less than 8 bytes**, the function **exits early** to
 *  prevent invalid processing.
 *
 *  @param data      Pointer to the received data buffer.
 *  @param length    Length of the received data in bytes.
 *  @param faultInfo Pointer to the ExtensionStatusFaultStruct where the extracted data is stored.
 *
 *  @return None.
 */
void ProcessModuleFaultInfo(uint8_t *data, uint8_t length, ExtensionStatusFaultStruct *faultInfo)
{
    if (length < 8) return; // Ensure data length is valid

    // Bytes 1-2: Module State (bitwise)
    faultInfo->ModuleState = (uint16_t)(data[0] | (data[1] << 8));

    // Bytes 3-4: Fault/Warning (bitwise)
    faultInfo->FaultWarning = (uint16_t)(data[2] | (data[3] << 8));

    // Bytes 5-8: Reserved (not used)
    faultInfo->Reserved[0] = data[4];
    faultInfo->Reserved[1] = data[5];
    faultInfo->Reserved[2] = data[6];
    faultInfo->Reserved[3] = data[7];
}

/**
 *  ProcessModuleStartStopConfirm
 *
 *  @brief Processes and stores the module start/stop status in ModuleStartStopConfirmationStruct.
 *
 *  This function extracts the **module start/stop status** from the received data buffer
 *  and stores it in the **ModuleStartStopConfirmationStruct** structure.
 *
 *  If the received data length is **less than 1 byte**, the function **exits early** to
 *  prevent invalid processing.
 *
 *  @param data          Pointer to the received data buffer.
 *  @param length        Length of the received data in bytes.
 *  @param confirmStatus Pointer to the ModuleStartStopConfirmationStruct where the
 *                       extracted data is stored.
 *
 *  @return None.
 */
void ProcessModuleStartStopConfirm(uint8_t *data, uint8_t length, ModuleStartStopConfirmationStruct *confirmStatus)
{
    if (length < 1) return; // Ensure data length is valid

    confirmStatus->MessageStatus = data[0];
    uint8_t i=0;
    for (i = 1; i < 8; i++)
    {
        confirmStatus->Reserved[i - 1] = 0;
    }

    // Reserved bytes are always 0, no need to explicitly set them
}

/**
 *  ProcessACInfo
 *
 *  @brief Processes and stores AC information in the AcInformationStruct structure.
 *
 *  This function extracts AC-related parameters such as **voltage, current, and frequency**
 *  from the received data and stores them in the AcInformationStruct structure.
 *
 *  If the received data length is less than **8 bytes**, the function will **exit early**
 *  to prevent invalid processing.
 *
 *  @param data    Pointer to the received data buffer.
 *  @param length  Length of the received data in bytes.
 *  @param acInfo  Pointer to the AcInformationStruct where extracted data is stored.
 *
 *  @return None.
 */
void ProcessACInfo(uint8_t *data, uint8_t length, AcInformationStruct *acInfo)
{
    if (length < 8) return; // Ensure data length is valid
    acInfo->APhaseVoltage      = (float)(((uint16_t)data[0] | ((uint16_t)data[1] << 8))) * 0.1f;  // Bytes 1-2: Voltage (0.1V per bit)
    acInfo->BPhaseVoltage      = (float)(((uint16_t)data[2] | ((uint16_t)data[3] << 8))) * 0.1f; // Bytes 3-4: Current (0.01A per bit)
    acInfo->CPhaseVoltage      = (float)(((uint16_t)data[4] | ((uint16_t)data[5] << 8))) * 0.1f;  // Bytes 5-6: Frequency (0.1Hz per bit)
    acInfo->AmbientTemperature = (float)(((uint16_t)data[6] | ((uint16_t)data[7] << 8)));  // Bytes 7-8: Temperature (raw value)
}

void TONHECANCyclicCheck(RectifierStatus_t Tonhe_Status)
{
    static uint8_t Rectifier_count[6]={0,0,0,0,0,0};
    static uint8_t Rectifier_IregCheck[6]={0,0,0,0,0,0};
    uint8_t i;
    for(i=0;i<6;i++)
    {
        switch(i)
        {
        case 0:
            CheckTonheOperation(&Rectifier_count[i],&Tonhe_Status.Tonhe_RightRectifier_1,&Rectifier_IregCheck[i]);
            break;
        case 1:
            CheckTonheOperation(&Rectifier_count[i],&Tonhe_Status.Tonhe_RightRectifier_2,&Rectifier_IregCheck[i]);
            break;
        case 2:
            CheckTonheOperation(&Rectifier_count[i],&Tonhe_Status.Tonhe_RightRectifier_3,&Rectifier_IregCheck[i]);
            break;
        case 3:
            CheckTonheOperation(&Rectifier_count[i],&Tonhe_Status.Tonhe_LeftRectifier_1,&Rectifier_IregCheck[i]);
            break;
        case 4:
            CheckTonheOperation(&Rectifier_count[i],&Tonhe_Status.Tonhe_LeftRectifier_2,&Rectifier_IregCheck[i]);
            break;
        case 5:
            CheckTonheOperation(&Rectifier_count[i],&Tonhe_Status.Tonhe_LeftRectifier_3,&Rectifier_IregCheck[i]);
            break;
        }
    }
}

void CheckTonheOperation(uint8_t *count,Tonhe_StatusMode_t *status,uint8_t *Ireg_check)
{
    if((*Ireg_check>=8) && (*count < 4)){
        *status = Communication_lost;
        *Ireg_check=9;
    }
    else if((*Ireg_check>=4) && (*Ireg_check < 8)  && (*count < 4)){
        *status = Not_Updating;
        (*Ireg_check)++;
    }
    else if(*count < 4){
        //*status = TIME_OUT;
        (*Ireg_check)++;
    }
    else if(*count >= 4){
        *status = Data_Coming;
        *count=1;
        *Ireg_check = 1;
    }
}

