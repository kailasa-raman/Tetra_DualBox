/*
 * J1939.c
 *
 *  Created on: Feb 20, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */
#include <J1939.h>

/**
 *  setJ1939ID
 *
 *  @brief Setup the J1939 CAN ID.
 *
 *  This function is helpful to set the J1939 CAN ID frame format and store the 29-structure bit in J1939_Union.
 *
 *  @param j1939 is a pointer to the J1939_Union.
 *  @param priority is a variable to store the priority of a CAN message.
 *  @param pf PDU Format it tells message is a broadcast or peer-to-peer.
 *  @param ps PDU Specific has a target address if pf is less than 240.
 *  @param sa Source Address of a slave.
 *
 *  This function is helpful to set the J1939 29-bit CAN ID frame format. If pf is less than 240 it is a peer-to-peer message,
 *  else if pf is greater or equal to than 240 it is a broadcast message.
 *
 *  @return None.
 */
void setJ1939ID(J1939_Union *j1939, uint8_t priority, uint8_t pf, uint8_t ps, uint8_t sa)
{
    // Set fields
    j1939->fields.priority = priority & 0x07;  // 3-bit field
    j1939->fields.reserved = 0;                // Always 0
    j1939->fields.dp = 0;                      // Default Data Page (0)
    j1939->fields.pf = pf;                     // 8-bit PF
    j1939->fields.ps = ps;                     // 8-bit PS (Destination Address if PDU1)
    j1939->fields.sa = sa;                     // 8-bit Source Address

    // PGN Calculation
    /*
    uint16_t pgn;
    if (pf >= 240)
    { // PDU2 (Broadcast PGN)
        pgn = (pf << 8) | ps;
        if (pf >= 240) {  // PDU2: PS is part of PGNuint16_t pgn = (pf << 8);
            if (pf >= 240) {  // PDU2: Include DP in PGN
                pgn |= (j1939->fields.dp << 8);
            }
            pgn |= (j1939->fields.dp << 16);
        }
    } else
    { // PDU1 (Peer-to-Peer PGN)
        pgn = (pf << 8);
        if (pf >= 240) {  // PDU2: Include DP in PGN
            pgn |= (j1939->fields.dp << 8);
        }
    }
    */

    // Assemble the 29-bit identifier into a 32-bit value
    j1939->identifier = ((uint32_t) j1939->fields.priority << 26) |
                        ((uint32_t) j1939->fields.reserved << 25) |
                        ((uint32_t) j1939->fields.dp << 24) |
                        ((uint32_t) j1939->fields.pf << 16) |
                        ((uint32_t) j1939->fields.ps << 8) |
                        ((uint32_t) j1939->fields.sa);

}

/**
 *  ExtractPGN
 *
 *  @brief This function is helpful to extract the pgn.
 *
 *  This function is helpful to extract the pgn and store the pf, ps, dp separately.
 *
 *  @param identifier It is a variable of pgn.
 *  @param sa It a pointer variable store the source address.
 *
 *  @return pgn.
 */
uint32_t ExtractPGN(uint32_t identifier,uint8_t *sa)
{
    uint8_t pf = (identifier >> 16) & 0xFF;  // Extract PF (Bits 16-23)
    uint8_t ps = (identifier >> 8) & 0xFF;   // Extract PS (Bits 8-15)
    uint8_t dp = (identifier >> 24) & 0x01;  // Extract DP (Bit 24)
    *sa =  (identifier) * 0xFF;

    uint32_t pgn;
    if (pf >= 240) {
        // PDU2 (Broadcast Message) - PGN only includes DP and PF
        pgn = ((uint32_t)dp << 16) | (pf << 8);
    } else {
        // PDU1 (Peer-to-Peer Message) - PGN includes DP, PF, and PS
        pgn = ((uint32_t)dp << 16) | (pf << 8) | ps;
    }
    return pgn;
}


