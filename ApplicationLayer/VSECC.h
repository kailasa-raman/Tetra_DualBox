/*
 * VSECC.h
 *
 *  Created on: Feb 21, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

#ifndef VSECC_H_
#define VSECC_H_

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

#define TIME1   4
#define TIME2   8
#define COUNTCHECK    4
// Base address for CAN IDs 0x00000301
#define CTRLGUN1                0x300
#define CTRLGUN2                0x700

// Base address for gun-1 0x00000300 and input/outputs of gun1 base address 0x500
#define BASE_ADDRESS_G1           CTRLGUN1
#define BASE_ADDRESS_IO_G1        0x500

// Base address for gun-1 0x00000700 and input/outputs of gun2 base address 0x900
#define BASE_ADDRESS_G2             CTRLGUN2
#define BASE_ADDRESS_IO_G2          0x900

// Mask for CAN IDs 0xFFFF7F0 and Mask for CAN messages 0X0FF
#define MASKFORBASE     0xFFFF7F0
#define MASKFORMSG       0X0FF
// CAN Message IDs (Offsets from Base Address)
//  From VSECC
#define VEHICLE_STATUS_OFFSET          0x1
#define VEHICLE_ID_OFFSET              0x8
#define CHARGING_SESSION_INFO1_OFFSET  0xA
#define CHARGING_SESSION_INFO2_OFFSET  0xB
#define CHARGING_SESSION_INFO3_OFFSET  0xD
#define CHARGING_SESSION_INFO4_OFFSET  0xE
#define ANALOG_INS1_OFFSET             0x3
#define ANALOG_INS2_OFFSET             0x4
#define ANALOG_INS3_OFFSET             0x5
#define RESET_OFFSET                   0x6
// From PECC
#define PECC_STATUS1_OFFSET            0x2
#define PECC_STATUS2_OFFSET            0x3
#define PECC_LIMITS1_OFFSET            0x4
#define PECC_LIMITS2_OFFSET            0x5
#define PECC_LIMITS3_OFFSET            0xC
#define STOP_CHARGING_OFFSET           0x9          // Gracefull shutdown by PECCC


#define DLC_ANALOGINS3               6
#define DLC_ANALOGINS2               8
#define DLC_ANALOGINS1               8
#define DLC_DIGITALINS                1
#define DLC_DIGITALOUTS2              4
#define DLC_DIGITALOUTS1              4


#define DLC_CHARGINGSESSIONINFO4      8
#define DLC_CHARGINGSESSIONINFO3      8
#define DLC_CHARGINGSESSIONINFO2      8
#define DLC_CHARGINGSESSIONINFO1      8
#define DLC_STOPCHARGING              0
#define DLC_PECCLIMITS1              8
#define DLC_PECCLIMITS2              4
#define DLC_PECCLIMITS3              8
#define DLC_PECCSTATUS1              7
#define DLC_PECCSTATUS2              6
#define DLC_VEHICLEID                6
#define DLC_RESET                    0
#define DLC_SECCSENSORS              4
#define DLC_VEHICLESTATUS            8

#define FACTOR_DISCHARGE_CURRENT -10.0f
#define FACTOR_DISCHARGE_POWER -10.0f
#define FACTOR_VOLTAGE      10.0f
#define FACTOR_CURRENT  10.0f
#define FACTOR_POWER 0.1f
#define FACTOR_TEMPERATURE 10.0f

#define cyclic_time    300
#endif /* APPLICATIONLAYER_VSECC_H_ */
