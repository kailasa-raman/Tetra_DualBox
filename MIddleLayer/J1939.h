/*
 * J1939.H
 *
 *  Created on: Feb 20, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

#ifndef J1939_H_
#define J1939_H_
#ifndef DRIVERLIB_H
    #include "driverlib.h"
#endif

#ifndef DEVICE_H
    #include "device.h"
#endif
#ifndef MCUSPECIFIC_H
#include "MCUspecific.h"
#endif
#include <stdint.h>

// Structure to store the extracted 29-bit CAN ID.
typedef struct {
    uint8_t sa;         // Source Address (8 bits)
    uint8_t ps;         // PDU Specific (8 bits) / Destination Address
    uint8_t pf;         // PDU Format (8 bits)
    uint8_t dp : 1;     // Data Page (1 bit)
    uint8_t reserved : 1; // Reserved bit (Always 0)
    uint8_t priority : 3; // Priority (3 bits)
    uint8_t unused : 3;  // Padding to align with 29-bit ID
} J1939_ID_t;

// Union to allow access as full 32-bit identifier or structured fields
typedef union {
    uint32_t identifier; // 29-bit Identifier stored in a 32-bit variable
    J1939_ID_t fields;   // Structured breakdown with bit fields
} J1939_Union;

/**
 *  This function is helpful to set the J1939 CAN ID frame format and store the 29-structure bit in J1939_Union.
 *
 *  @param j1939 is a pointer to the J1939_Union.
 *  @param priority is a variable to store the priority of a CAN message.
 *  @param pf PDU Format it tells message is a broadcast or peer-to-peer.
 *  @param ps PDU Specific has a target address if pf is less than 240.
 *  @param sa Source Address of a slave.
 */
void setJ1939ID(J1939_Union *j1939, uint8_t priority, uint8_t pf, uint8_t ps, uint8_t sa);

/**
 *  This function is helpful to extract the pgn and store the pf, ps, dp separately.
 *
 *  @param identifier It is a variable of pgn.
 *  @param sa It a pointer variable store the source address.
 */
uint32_t ExtractPGN(uint32_t identifier,uint8_t *sourceAdd);
#endif /* MIDDLELAYER_J1939_H_ */
