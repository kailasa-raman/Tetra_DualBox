/*
 * IMD.h
 *
 *  Created on: Feb 19, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

#ifndef IMD_H_
#define IMD_H_

#include <stdint.h>
#include "modbus.h"
#define IMDID1 0x04
#define IMDID2 0x05
#define IMDStopWrite  0
#define IMDStartWrite 1
#define IMDStopWrite  0
#define IMDSTOPSIZE   1
#define MODBUS_ADDRESS_IR    1000
#define MODBUS_ADDRESS_STOP  3026

/**
 *  @brief Reads a 32-bit floating-point parameter from a Modbus device.
 *
 *  @param ID Modbus device ID (slave address of the target device).
 *  @param StartAddress Starting Modbus register address to read the value from.
 *  @param ParaBuffer Pointer to the buffer where the 32-bit float result will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 */
ModbusCommStatus IMDReadOneParaValue32(uint8_t ID,uint32_t StartAddress, float32_t *ParaBuffer,uint8_t TimeoutMS);

/**
 *  @brief Reads insulation resistance data from the IMD and returns the status.
 *
 *  @param ID Modbus device ID of the IMD (isolation monitoring device).
 *  @param Data Pointer to the float buffer where the insulation resistance value will be stored.
 *  @param Status Pointer to a variable where the Modbus communication status will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 */
void ReadIMDData(uint8_t ID,float32_t *Data,ModbusCommStatus *Status,uint8_t TimeoutMS);

void IMDWriteRegisters(uint8_t DeviceID,uint32_t StartAddress,uint8_t size,uint8_t Data);
#endif /* APPLICATIONLAYER_IMD_H_ */
