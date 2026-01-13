/*
 * DCEMMeter.h
 *
 *  Created on: Feb 19, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

#ifndef DCEMETER_H_
#define DCEMETER_H_

#include <stdint.h>
#include "modbus.h"

#define DCEMID1 0x02
#define DCEMID2 0x03
#define DCMETERCOMDELAY 5000
#define MODBUS_ADDRESS_DCVOLTAGE        0x0006  // Address 6 (HEX)
#define MODBUS_ADDRESS_DCCURRENT        0x0008  // Address 8 (HEX)
#define MODBUS_ADDRESS_DCPOWER          0x000A  // Address 10 (HEX)
#define MODBUS_ADDRESS_DCPOS_ENERGY1     0x000C  // Address 12 (HEX)
#define MODBUS_ADDRESS_DCPOS_ENERGY2     0x000E  // Address 12 (HEX)
#define MODBUS_ADDRESS_DCNEG_ENERGY     0x0010  // Address 16 (HEX)


typedef struct {
    float32_t Voltage;        // Stores Circuit 1 voltage (0.1V resolution)
    float32_t Current;        // Stores Circuit 1 current (0.1A resolution)
    float32_t Power;          // Stores Circuit 1 power (0.1kW resolution)
    float32_t NegEnergy;     // Stores Circuit 1 positive energy (10Wh resolution)
    float32_t PosEnergy;     // Stores Circuit 1 negative energy (10Wh resolution
} DC_EnergyMeterDataStruct;

typedef union {
    uint16_t DCEMModBusStatusVal;
    struct {
        ModbusCommStatus DCCurrentStatus : 2;
        ModbusCommStatus DCVoltageStatus : 2;
        ModbusCommStatus DCPowerStatus : 2;
        ModbusCommStatus PosEnergyStatus : 2;
        ModbusCommStatus NegEnergyStatus : 2;
        ModbusCommStatus Reserved : 6;  // Fills up to 16 bits
    } StatusBits;  // Named struct
} DCEMModBusStatusStruct;

/**
 *  @brief Reads a 32-bit floating-point parameter from a Modbus device.
 *
 *  @param DC Energy Meter device ID (slave address of the target device).
 *  @param StartAddress Starting Modbus register address to read the value from.
 *  @param ParaBuffer Pointer to the buffer where the 32-bit float result will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 */
ModbusCommStatus DCReadOneParaValue32(uint8_t ID,uint32_t StartAddress, float32_t *ParaBuffer,uint8_t TimeoutMS);

/**
 *  @brief Reads a 64-bit floating-point parameter from a Modbus device.
 *
 *  @param DC Energy Meter device ID (slave address of the target device).
 *  @param StartAddress Starting Modbus register address to read the value from.
 *  @param ParaBuffer Pointer to the buffer where the 64-bit float result will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 */
ModbusCommStatus DCReadOneParaValue64(uint8_t ID, uint32_t StartAddress, float32_t *ParaBuffer, uint8_t TimeoutMS);
/**
 *  @brief Reads the DC current, voltage, power, positice energy and negative energy data from the DCEMeterData and returns the status.
 *
 *  @param ID Modbus device ID of the IMD (DC Energy meter device).
 *  @param Data Pointer to the DC_EnergyMeterDataStruct structure buffer where the DC energy meter values will be stored.
 *  @param Status Pointer to a variable where the Modbus communication status will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 */
void ReadDCEMeterData(uint8_t ID,DC_EnergyMeterDataStruct *Data,DCEMModBusStatusStruct *Status,uint8_t TimeoutMS);
#endif /* APPLICATIONLAYER_DCEMMETER_H_ */
