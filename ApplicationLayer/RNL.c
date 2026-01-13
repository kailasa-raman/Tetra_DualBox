/*
 * RNL.c
 *
 *  Created on: 14-Aug-2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

/* RNL.c */
#include "RNL.h"
#include "Charger.h"

 uint8_t BCL_Received_Flag_G1 = 0;
 uint8_t BCL_Received_Flag_G2 = 0;
 uint8_t BST_Received_Flag_G1 = 0;
 uint8_t BST_Received_Flag_G2 = 0;
/**
  *  RMLRXCANOBJSet
  *
  *  @brief Configures the CAN receive objects.
  *
  *  This function sets up multiple CAN message objects using a 29-bit CAN ID to send messages to the MCU.
  *  Each message object is configured for a specific CAN ID and uses an extended frame format.
  *
  *
  *  The function calls `CANMSGObjectSetup()` to initialize receive message objects for RNL:
  *  BHM,BRM,BCP,BRO,BCL,BCS,BCM,BST,BSD,BEM message id's.
  *
  *   Each message object configured to receive Extended frame format.
  *
  *   @param uint32_t CANBASE   - Base address of the CAN

  *   @return None.
  */
void RNLRXCANOBJSet(uint32_t CANBASE)
{
    CANMSGObjectSetup(CANBASE,
                      CANOBJ_RMC_RX,         // Choose RX object
                      RNL_BASE_ID,        // Extended ID to match
                      ExtendedFrame,                // Extended Frame flag
                      CANOBJECTTYPERX,         // RX message object
                      RNL_RX_ID_MASK,          // Mask to apply
                      CAN_MSG_OBJ_RX_INT_ENABLE|CAN_MSG_OBJ_USE_ID_FILTER|CAN_MSG_OBJ_USE_EXT_FILTER);

}
/**
  *  RMLTXCANOBJSet
  *
  *  @brief Configures the CAN transmit objects.
  *
  *  This function sets up multiple CAN message objects using a 29-bit CAN ID to send messages to the MCU.
  *  Each message object is configured for a specific CAN ID and uses an extended frame format.
  *
  *
  *  The function calls `CANMSGObjectSetup()` to initialize receive message objects for RNL:
  *  CHM,CRM,CTS,CML,CRO,CCS,BCM,CST,CSD,CEM and SENDBCP message id's.
  *
  *   Each message object configured to transmit Extended frame format.
  *
  *   @param uint32_t CANBASE   - Base address of the CAN

  *   @return None.
  */
void RNLTXCANOBJSet(uint32_t CANBASE)
{
    CANMSGObjectSetup(CANBASE,CANOBJ_CHM_TX,
                      CHM_ID,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);

    CANMSGObjectSetup(CANBASE,CANOBJ_CRM_TX,
                      CRM_ID,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);

    CANMSGObjectSetup(CANBASE,CANOBJ_CTS_TX,
                      CTS_ID,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);

    CANMSGObjectSetup(CANBASE,CANOBJ_CML_TX,
                      CML_ID,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);


    CANMSGObjectSetup(CANBASE,CANOBJ_CRO_TX,
                      CRO_ID,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);

    CANMSGObjectSetup(CANBASE,CANOBJ_CCS_TX,
                      CCS_ID,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);

    CANMSGObjectSetup(CANBASE,CANOBJ_CST_TX,
                      CST_ID,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);


    CANMSGObjectSetup(CANBASE,CANOBJ_CSD_TX,
                      CSD_ID,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);

    CANMSGObjectSetup(CANBASE,CANOBJ_CEM_TX,
                      CEM_ID,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);

    CANMSGObjectSetup(CANBASE,CANOBJ_BCPA_TX,
                      SENDBCP_ID,ExtendedFrame,
                             CANOBJECTTYPETX,0,
                             CAN_MSG_OBJ_NO_FLAGS);
}
//*****************************************************************************
//
//! @brief SendtoRML_CHM message to RNL
//!
//! @param CANBASE is the base address for can messages.
//! @param ChargeHandshakeTemp is the pointer to the chargehandshke structure in the RNL.
//!
//! This function will send the data of the CHM to the RNL.
//!
//!
//! @return None.
//
//*****************************************************************************
void SendtoRML_CHM(uint32_t CANBASE,ChargeHandshakeStruct *ChargeHandshakeTemp)
{

    CANSendMessage(CANBASE,
                   CANOBJ_CHM_TX,
                   &(ChargeHandshakeTemp->ChargeHandshakeMessageData),
                   DLC_CHM);
}
//*****************************************************************************
//
//! @brief SendtoRML_CRM message to RNL
//!
//! @param CANBASE is the base address for can messages.
//! @param ChargeRecognitionTemp is the pointer to the ChargeRecognition structure in the RNL.
//!
//! This function will send the data of the CRM to the RNL.
//!
//!
//! @return None.
///*****************************************************************************
void SendtoRML_CRM(uint32_t CANBASE,ChargeRecognitionStruct *ChargeRecognitionTemp)
{


    CANSendMessage(CANBASE,
                   CANOBJ_CRM_TX,
                   &(ChargeRecognitionTemp->ChargeRecognitionData),
                   DLC_CRM);
}
//*****************************************************************************
//
//! @brief SendtoRML_CTS message to RNL
//!
//! @param CANBASE is the base address for can messages.
//! @param ChargingTimeStartTemp is the pointer to the ChargingTimeStart structure in the RNL.
//!
//! This function will send the data of the CTS to the RNL.
//!
//!
//! @return None.
///*****************************************************************************
void SendtoRML_CTS(uint32_t CANBASE,ChargingTimeStartStruct *ChargingTimeStartTemp)
{

    CANSendMessage(CANBASE,
                   CANOBJ_CTS_TX,
                   &(ChargingTimeStartTemp->ChargingTimeStartData),
                   DLC_CTS);
}
//*****************************************************************************
//
//! @brief SendtoRML_CML message to RNL
//!
//! @param CANBASE is the base address for can messages.
//! @param ChargingMaxLimitTemp is the pointer to the ChargingMaxLimit structure in the RNL.
//!
//! This function will send the data of the CML to the RNL.
//!
//!
//! @return None.
///*****************************************************************************
void SendtoRML_CML(uint32_t CANBASE,ChargingMaxLimitStruct *  ChargingMaxLimitTemp)
{
    CANSendMessage(CANBASE,
                   CANOBJ_CML_TX,
                   &(ChargingMaxLimitTemp->ChargingMaxLimitData),
                   DLC_CML);
}
//*****************************************************************************
//
//! @brief SendtoRML_CRO message to RNL
//!
//! @param CANBASE is the base address for can messages.
//! @param ChargerReadinessOKTemp is the pointer to the ChargerReadinessOK structure in the RNL.
//!
//! This function will send the data of the CRO to the RNL.
//!
//!
//! @return None.
///*****************************************************************************
void SendtoRML_CRO(uint32_t CANBASE,ChargerReadinessOKStruct *ChargerReadinessOKTemp)
{
    CANSendMessage(CANBASE,
                   CANOBJ_CRO_TX,
                   &(ChargerReadinessOKTemp->ChargerReadinessOKData),
                   DLC_CRO);
}
//*****************************************************************************
//
//! @brief SendtoRML_CCS message to RNL
//!
//! @param CANBASE is the base address for can messages.
//! @param Temp is the pointer to the ChargerChargingStatus structure in the RNL.
//!
//! This function will send the data of the CCS to the RNL.
//!
//!
//! @return None.
///*****************************************************************************
void SendtoRML_CCS(uint32_t CANBASE,ChargerChargingStatusStruct *Temp)
{
    CANSendMessage(CANBASE,
                   CANOBJ_CCS_TX,
                   &(Temp->ChargerChargingStatusData),
                   DLC_CCS);
}
//*****************************************************************************
//
//! @brief SendtoRML_CST message to RNL
//!
//! @param CANBASE is the base address for can messages.
//! @param Temp is the pointer to the ChargingStopTime structure in the RNL.
//!
//! This function will send the data of the CST to the RNL.
//!
//!
//! @return None.
///*****************************************************************************
void SendtoRML_CST(uint32_t CANBASE,ChargingStopTimeStruct *Temp)
{
    CANSendMessage(CANBASE,
                   CANOBJ_CST_TX,
                   &(Temp->ChargingStopTimeData),
                   DLC_CST);
}
//*****************************************************************************
//
//! @brief SendtoRML_CSD message to RNL
//!
//! @param CANBASE is the base address for can messages.
//! @param Temp is the pointer to the ChargerShutdown structure in the RNL.
//!
//! This function will send the data of the CSD to the RNL.
//!
//!
//! @return None.
///*****************************************************************************
void SendtoRML_CSD(uint32_t CANBASE,ChargerShutdownStruct *Temp)
{
    CANSendMessage(CANBASE,
                   CANOBJ_CSD_TX,
                   &(Temp->ChargerShutdownData),
                   DLC_CSD);
}
//*****************************************************************************
//
//! @brief SendtoRML_CEM message to RNL
//!
//! @param CANBASE is the base address for can messages.
//! @param Temp is the pointer to the CEM structure in the RNL.
//!
//! This function will send the data of the CEM to the RNL.
//!
//!
//! @return None.
///*****************************************************************************
void SendtoRML_CEM(uint32_t CANBASE,CEMStruct *  Temp)
{
    CANSendMessage(CANBASE,
                   CANOBJ_CEM_TX,
                   &(Temp->CEM_Data),
                   DLC_CEM);
}
/**
 *  @brief This function process the can id received from RNL, based on can id it gives the data to a specific
 *  process function.
 *
 *  @brief 'can_id' It is a combination of base address and offset id of specific process function.
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *  @param 'FLAG' An enum flag set here will be used to send the proper RNL message only at the particular time .
 *
 *  This function process the can id received from RNL, based on can id it gives the data and length to a specific
 *  process function to extract the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void RNL_ProcessCANMsg(uint32_t can_id, uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData,RNLFlagEnum *FLAG,uint8_t GunX,RNLDualStruct *Send)
{
    uint32_t CANBaseAddr;
    if(GunX == Gun1)
    {
        CANBaseAddr =RECTIFIER_GUN1;
    }
    else
    {
        CANBaseAddr =RECTIFIER_GUN2;
    }
    // static uint8_t BSM_Received_Flag = 0;
     static uint8_t BCS_RSV = 0;
     static uint8_t BCP_RSV = 0;
     //static uint8_t MPM_RXFlag;
     if (receivedData == NULL || data == NULL) return;

     uint32_t MSGBase =  can_id & RNL_RX_ID_MASK;

     uint32_t MsgOffset = can_id & RNL_MSG_OFFSET;


     if( MSGBase == RNL_RX_ID)
     {
         switch (MsgOffset)
         {
             case BHM_ID:

                     ProcessBatteryHandshakeMessage(data, length, receivedData);
                     *FLAG = BHMFlag;
                     break;

             case BRM_ID:

                     ProcessBatteryResponseMessage(data, length, receivedData);
                     *FLAG = BRMFlag;
                     break;
             case RTS_ID:  // Change this to RTS
                 // if 7th  11 then send CTS 7th byte 11 and you will make flag 1
                 if(data[6] == 0x06)
                 {
                    // MPM_RXFlag =1;
                     *Send  = SendRTS1;
                     *FLAG = RTS1Flag;
                 }
                 else
                 {
                     *Send = SendRTS2;
                    // MPM_RXFlag=2;
                     uint8_t CTS2_Msg1[8] = {0x11, 0x02, 0x01, 0xFF, 0xFF, 0x00, 0x11, 0x00};
                     CAN_sendMessage(CANBaseAddr, CANOBJ_BCPA_TX, 8, (uint16_t *)CTS2_Msg1);
                 }
                 break;
             case MPM_ID:
                 if(*Send == SendRTS1)
                 {
                     if(data[0]==1)
                     {
                         BCP_RSV |= data[0];
                         ProcessBatteryChargingParameter1(data, length, receivedData);
                     }
                     else if(data[0] == 2)
                     {
                         BCP_RSV |= data[0];
                         ProcessBatteryChargingParameter2(data, length, receivedData);
                         BCP_RSV = 0;
                         ProcessBCP(receivedData);
                         *FLAG = BCPFlag;
                     }

                 }
                 else
                 {
                     if(data[0] == 1)
                     {
                         ProcessBatteryChargingStatus1(data, length, receivedData);
                         BCS_RSV |= data[0];
                     }
                     else if(data[0] == 2)
                     {
                         ProcessBatteryChargingStatus2(data, length, receivedData);
                         BCS_RSV |= data[0];
                         uint8_t CTS2_Msg2[8] = {0x13, 0x09, 0x00, 0x02, 0xFF, 0x00, 0x11, 0x00};
                         CAN_sendMessage(CANBaseAddr, CANOBJ_BCPA_TX, 8, (uint16_t *)CTS2_Msg2);
                         BCS_RSV = 0;
                         ProcessBCS(receivedData);
                         *FLAG = BCSFlag;
                     }
                 }
                 break;
             case BRO_ID:

                     ProcessBatteryReadinessOk(data, length, receivedData);
                    *FLAG = BROFlag;
                 break;
             case BCL_ID:

                     ProcessBatteryChargingLimit(data, length, receivedData);
                     if(GunX == Gun1)
                     {
                         BCL_Received_Flag_G1 = 1;
                     }
                     else if(GunX == Gun2)
                     {
                         BCL_Received_Flag_G2 =1;
                     }
                    // BCL_Received_Flag = 1;
                 break;
             case BSM_ID:
                     ProcessBatteryStatusMessage(data, length, receivedData);
                 break;
             case BST_ID:

                     ProcessBatteryStopTrigger(data, length, receivedData);
                     if(GunX == Gun1)
                     {
                         BST_Received_Flag_G1 = 1;
                     }
                     else if(GunX == Gun2)
                     {
                         BST_Received_Flag_G2 = 1;
                     }
                      //*FLAG = BSTFlag;
                 break;
             case BSD_ID:

                     ProcessBatteryShutdown(data, length, receivedData);
                     *FLAG = BSDFlag;

                 break;
             case BEM_ID:
                     ProcessBEM(data, length, receivedData);
                     *FLAG = BEMFlag;
                 break;
             default:
                  // Unknown CAN ID, ignore or log error
                  break;
         }
     }

}
/**
 *  @brief This function process the data received from RNL, and stores it in the BasicHSData structure of RNL.
 *
 *
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process  the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBatteryHandshakeMessage(uint8_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData)
{
    uint64_t temp = 0;
       uint8_t *byteData = (uint8_t *)data;
       uint8_t i = 0;

       // Convert raw bytes into a 64-bit integer
       for (i = 0; i < length; i++)
       {
           temp |= ((uint64_t)byteData[i] << (8 * i));
       }
       receivedData->BasicHSData.BasicHSData = temp;
}
/**
 *  @brief This function process the data received from RNL, and stores it in the BasicResponseData structure of RNL.
 *
 *
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process  the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBatteryResponseMessage(uint16_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData)
{
    uint64_t temp = 0;
       uint8_t *byteData = (uint8_t *)data;
       uint8_t i = 0;

       // Convert raw bytes into a 64-bit integer
       for (i = 0; i < length; i++)
       {
           temp |= ((uint64_t)byteData[i] << (8 * i));
       }

      receivedData->BasicResponse.BasicResponseData = temp;

}
/**
 *  @brief This function process the data received from RNL, and stores it in the Battery_Charging_Parameter1 structure of RNL.
 *
 *
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBatteryChargingParameter1(uint16_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData)
{
    uint64_t temp = 0;
       uint8_t *byteData = (uint8_t *)data;
       uint8_t i = 0;

       // Convert raw bytes into a 64-bit integer
       for (i = 0; i < length; i++)
       {
           temp |= ((uint64_t)byteData[i] << (8 * i));
       }
       receivedData->Battery_Charging_Parameter1.Battery_Charging_Parameter1 = temp;
}
/**
 *  @brief This function process the data received from RNL, and stores it in the Battery_Charging_Parameter2 structure of RNL.
 *
 *
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process  the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBatteryChargingParameter2(uint16_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData)
{
    uint64_t temp = 0;
       uint8_t *byteData = (uint8_t *)data;
       uint8_t i = 0;

       // Convert raw bytes into a 64-bit integer
       for (i = 0; i < length; i++)
       {
           temp |= ((uint64_t)byteData[i] << (8 * i));
       }
       receivedData->Battery_Charging_Parameter2.Battery_Charging_Parameter2 = temp;
}
/**
 *  @brief This function process the data received from RNL, and stores it in the BatteryReadinessOKData structure of RNL.
 *
 *
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process  the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBatteryReadinessOk(uint16_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData)
{
    uint64_t temp = 0;
       uint8_t *byteData = (uint8_t *)data;
       uint8_t i = 0;

       // Convert raw bytes into a 64-bit integer
       for (i = 0; i < length; i++)
       {
           temp |= ((uint64_t)byteData[i] << (8 * i));
       }

       receivedData->BatteryReadinessOK.BatteryReadinessOKData =temp;

}
/**
 *  @brief This function process the data received from RNL, and stores it in the BatteryChargingLimitData structure of RNL.
 *
 *
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process  the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBatteryChargingLimit(uint16_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData)
{
    uint64_t temp = 0;
       uint8_t *byteData = (uint8_t *)data;
       uint8_t i = 0;

       // Convert raw bytes into a 64-bit integer
       for (i = 0; i < length; i++)
       {
           temp |= ((uint64_t)byteData[i] << (8 * i));
       }
       receivedData->BatteryChargingLimit.BatteryChargingLimitData = temp;

}
/**
 *  @brief This function process the data received from RNL, and stores it in the BatteryChargingStatus1Data structure of RNL.
 *
 *
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process  the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBatteryChargingStatus1(uint16_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData)
{
    uint64_t temp = 0;
       uint8_t *byteData = (uint8_t *)data;
       uint8_t i = 0;

       // Convert raw bytes into a 64-bit integer
       for (i = 0; i < length; i++)
       {
           temp |= ((uint64_t)byteData[i] << (8 * i));
       }

       receivedData->BatteryChargingStatus1.BatteryChargingStatus1Data = temp;
}
/**
 *  @brief This function process the data received from RNL, and stores it in the BatteryChargingStatus2Data structure of RNL.
 *
 *
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process  the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBatteryChargingStatus2(uint16_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData)
{
    uint64_t temp = 0;
       uint8_t *byteData = (uint8_t *)data;
       uint8_t i = 0;

       // Convert raw bytes into a 64-bit integer
       for (i = 0; i < length; i++)
       {
           temp |= ((uint64_t)byteData[i] << (8 * i));
       }

       receivedData->BatteryChargingStatus2.BatteryChargingStatus2Data = temp;

}
/**
 *  @brief This function process the data received from RNL, and stores it in the BatteryStatusData structure of RNL.
 *
 *
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process  the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBatteryStatusMessage(uint16_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData)
{
    uint64_t temp = 0;
       uint8_t *byteData = (uint8_t *)data;
       uint8_t i = 0;

       // Convert raw bytes into a 64-bit integer
       for (i = 0; i < length; i++)
       {
           temp |= ((uint64_t)byteData[i] << (8 * i));
       }

       receivedData->BatteryStatus.BatteryStatusData = temp;
}
/**
 *  @brief This function process the data received from RNL, and stores it in the BatteryStopTriggerData structure of RNL.
 *
 *
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process  the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBatteryStopTrigger(uint16_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData)
{
    uint64_t temp = 0;
       uint8_t *byteData = (uint8_t *)data;
       uint8_t i = 0;

       // Convert raw bytes into a 64-bit integer
       for (i = 0; i < length; i++)
       {
           temp |= ((uint64_t)byteData[i] << (8 * i));
       }

       receivedData->BatteryStopTrigger.BatteryStopTriggerData = temp;

}
/**
 *  @brief This function process the data received from RNL, and stores it in the BatteryShutdownData structure of RNL.
 *
 *
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process  the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBatteryShutdown(uint16_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData)
{
    uint64_t temp = 0;
       uint8_t *byteData = (uint8_t *)data;
       uint8_t i = 0;

       // Convert raw bytes into a 64-bit integer
       for (i = 0; i < length; i++)
       {
           temp |= ((uint64_t)byteData[i] << (8 * i));
       }

       receivedData->BatteryShutdown.BatteryShutdownData = temp;
}
/**
 *  @brief This function process the data received from RNL, and stores it in the BEM_Data structure of RNL.
 *
 *
 *  @param 'data' Pointer to the received vehicle status data.
 *  @param 'length' Length of the received data in bytes.
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process  the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBEM(uint16_t *data, uint8_t length, ReceiveFromRMLStruct *receivedData)
{
    uint64_t temp = 0;
       uint8_t *byteData = (uint8_t *)data;
       uint8_t i = 0;

       // Convert raw bytes into a 64-bit integer
       for (i = 0; i < length; i++)
       {
           temp |= ((uint64_t)byteData[i] << (8 * i));
       }
       receivedData->BEMValue.BEM_Data = temp;
}
/**
 *  @brief This function process the data received from RNL, and stores it in the respective structure of RNL.
 *
 *
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBCP(ReceiveFromRMLStruct *receivedData)
{
    receivedData->Battery_Charging_Parameter.CR_Plc_MaxVoltSingleBattery = receivedData->Battery_Charging_Parameter1.Battery_Charging_Parameter1Signal.CR_Plc_MaxVoltSingleBattery;

    receivedData->Battery_Charging_Parameter.CR_Plc_EvmaxCurrent = receivedData->Battery_Charging_Parameter1.Battery_Charging_Parameter1Signal.CR_Plc_EvmaxCurrent;
    receivedData->Battery_Charging_Parameter.CR_Plc_EvmaxCurrentFalg = receivedData->Battery_Charging_Parameter1.Battery_Charging_Parameter1Signal.CR_Plc_EvmaxCurrentFalg;
    receivedData->Battery_Charging_Parameter.CR_Plc_EvNormialEnergy = receivedData->Battery_Charging_Parameter1.Battery_Charging_Parameter1Signal.CR_Plc_EvNormialEnergy;
    receivedData->Battery_Charging_Parameter.CR_Plc_EvmaxVolt = (receivedData->Battery_Charging_Parameter2.Battery_Charging_Parameter2Signal.CR_Plc_EvmaxVoltHB << 8)|receivedData->Battery_Charging_Parameter1.Battery_Charging_Parameter1Signal.CR_Plc_EvmaxVoltLB;
    receivedData->Battery_Charging_Parameter.CR_Plc_EvAvailableTemp = receivedData->Battery_Charging_Parameter2.Battery_Charging_Parameter2Signal.CR_Plc_EvAvailableTemp;
    receivedData->Battery_Charging_Parameter.CR_Plc_EvRessSOC = receivedData->Battery_Charging_Parameter2.Battery_Charging_Parameter2Signal.CR_Plc_EvRessSOC;
    receivedData->Battery_Charging_Parameter.CR_Plc_EvCurrentVolt = receivedData->Battery_Charging_Parameter2.Battery_Charging_Parameter2Signal.CR_Plc_EvCurrentVolt;
    receivedData->Battery_Charging_Parameter.CR_Plc_EvTransferType = receivedData->Battery_Charging_Parameter2.Battery_Charging_Parameter2Signal.CR_Plc_EvTransferType;
}
/**
 *  @brief This function process the data received from RNL, and stores it in the respective structure of RNL.
 *
 *
 *  @param 'receivedData' Pointer to a structure where the extracted vehicle status information will be stored.
 *
 *  This function process the data and extracted data stores it in the provided `receivedData` structure.
 *
 *  @return None.
 */
void ProcessBCS(ReceiveFromRMLStruct *receivedData)
{
    receivedData->BatteryChargingStatus.CR_Plc_SensingVolt = receivedData->BatteryChargingStatus1.BatteryChargingStatus1Signal.CR_Plc_SensingVolt;
    receivedData->BatteryChargingStatus.CR_Plc_SensingCurrent = receivedData->BatteryChargingStatus1.BatteryChargingStatus1Signal.CR_Plc_SensingCurrent;
    receivedData->BatteryChargingStatus.CR_Plc_HighestVoltSingle = receivedData->BatteryChargingStatus1.BatteryChargingStatus1Signal.CR_Plc_HighestVoltSingle;
    receivedData->BatteryChargingStatus.HighestSinglePackNo = receivedData->BatteryChargingStatus1.BatteryChargingStatus1Signal.HighestSinglePackNo;
    receivedData->BatteryChargingStatus.CR_Plc_EvRessSOC = receivedData->BatteryChargingStatus1.BatteryChargingStatus1Signal.CR_Plc_EvRessSOC;
    receivedData->BatteryChargingStatus.CR_Plc_FullSOCRemainedTime = receivedData->BatteryChargingStatus2.BatteryChargingStatus2Signal.CR_Plc_FullSOCRemainedTime;
}
/**
 *  @brief This function process the data received from RNL, and stores it in the respective structure of RNL.
 *
 *
 *  @param 'chs' Pointer to a structure where the default information will be stored.
 *
 *  This function process and stores the default values in the structure
 *
 *  @return None.
 */
void setupChargeHandshake(ChargeHandshakeStruct *chs)
{
    chs->ChargeHandshakeMessageSignal.VerMinor = CHM_VERSIONMINOR;
    chs->ChargeHandshakeMessageSignal.VerMajorL = CHM_VERSIONMAJORL;
    chs->ChargeHandshakeMessageSignal.VerMajorH = CHM_VERSIONMAJORH;
    chs->ChargeHandshakeMessageSignal.EVSETransferType = 0x3;//DEFAULT
    chs->ChargeHandshakeMessageSignal.SLACTime = 0;  //SLAC Timout default is 7sec for 0
    chs->ChargeHandshakeMessageSignal.ChgType = 0;       // CCS2 (1  for CCS1)
    chs->ChargeHandshakeMessageSignal.ChgMode = 0;       // Charger Paramenter Discovery
    chs->ChargeHandshakeMessageSignal.SLACRestartTime = 8;  // For ISO standard it is 8  for DIN it is 3
}
/**
 *  @brief This function process the data received from RNL, and stores it in the respective structure of RNL.
 *
 *
 *  @param 'cts' Pointer to a structure where the default information will be stored.
 *
 *  This function process and stores the default values in the structure
 *
 *  @return None.
 */
void setupChargeTimeStart(ChargingTimeStartStruct *cts)
{
    //BCD VALUES
    cts->ChargingTimeStartSignal.CR_Secc_EvseSecond = 0x01;
    cts->ChargingTimeStartSignal.CR_Secc_EvseMin = 0x04;
    cts->ChargingTimeStartSignal.CR_Secc_EvseHour = 0x10;
    cts->ChargingTimeStartSignal.CR_Secc_EvseDay = 0x09;
    cts->ChargingTimeStartSignal.CR_Secc_EvseMonth = 0x10;
    cts->ChargingTimeStartSignal.CR_Secc_EvseYear = 0x2025;
}

