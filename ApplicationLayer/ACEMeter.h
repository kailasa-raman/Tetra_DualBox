/*
 * ACEMeter.h
 *
 *  Created on: Feb 18, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */
#ifndef ACEMETER_H
#define ACEMETER_H

#include <stdint.h>
#include "modbus.h"
// Modbus addresses and data length for IMA3250
//New-Energy meter EM6400NG+
/*
 * name of Excel sheet 7.2.2
 * Base address 400000(Neglet other base address such as 42,44,43).
 * Take offset substract 1, and convert into hexadecimal.
 * Eg:403028 becomes 3027 and convert into hex-0x0BD3.
 */
#define ACEMID  0x01
#define ACMETERCOMDELAY                  2000
#define MODBUS_ADDRESS_CURRENT           0x0BB7  // Start address for 3-phase current
#define MODBUS_ADDRESS_VOLTAGE_LN        0x0BD3  //0x0BD3  // Start address for L-N voltage
#define MODBUS_ADDRESS_VOLTAGE_LL        0x0BCB  // Start address for L-L voltage
#define MODBUS_ADDRESS_ACTIVE_POWER      0x0BED  // Start address for active power
#define MODBUS_ADDRESS_ENERGY            0x0C83  //0xB039  // Start address for 3-phase active energy

#define MODBUS_LENGTH_3PHASE             6  // 6 registers (12 bytes) for current, voltage, etc.
#define MODBUS_LENGTH_1PARA              2  // 2 REGISTER (4 BYTE) FOR TOTAL ACTIVE POWER, POWER FACTOR & FREQUNCY
#define MODBUS_ADDRESS_TOTAL_ACTIVE_POWER 0x0BF3  //0x0BF3  // Address for Total Active Power
#define MODBUS_ADDRESS_FREQUENCY 0x0C25 // Address for Frequency
#define MODBUS_ADDRESS_POWER_FACTOR 0x0C0B  // Address for Power Factor
#define MODBUS_ADDRESS_TOTALACTIVE_ENERGY  0x0C83  //0xB02B

#define MODBUS_MAX_RESPONSE_SIZE 256  // Maximum response size
//Old AC Energy meter address
/*#define ACEMID  0x01
#define ACMETERCOMDELAY                  2000
#define MODBUS_ADDRESS_CURRENT           0x0BB7  // Start address for 3-phase current
#define MODBUS_ADDRESS_VOLTAGE_LN        0x0BD3  // Start address for L-N voltage
#define MODBUS_ADDRESS_VOLTAGE_LL        0x0BCB  // Start address for L-L voltage
#define MODBUS_ADDRESS_ACTIVE_POWER      0x0BED  // Start address for active power
#define MODBUS_ADDRESS_ENERGY            0xB039  // Start address for 3-phase active energy

#define MODBUS_LENGTH_3PHASE             6  // 6 registers (12 bytes) for current, voltage, etc.
#define MODBUS_LENGTH_1PARA              2  // 2 REGISTER (4 BYTE) FOR TOTAL ACTIVE POWER, POWER FACTOR & FREQUNCY

#define MODBUS_ADDRESS_TOTAL_ACTIVE_POWER 0x0BF3  // Address for Total Active Power
#define MODBUS_ADDRESS_FREQUENCY 0x0C25 // Address for Frequency
#define MODBUS_ADDRESS_POWER_FACTOR 0x0C0B  // Address for Power Factor
#define MODBUS_ADDRESS_TOTALACTIVE_ENERGY  0xB02B

#define MODBUS_MAX_RESPONSE_SIZE 256  // Maximum response size*/

typedef struct {
    float Linecurrent[3];    // 3-phase current (A)
    float LNVoltage[3]; // L-N voltage (V)
    float LLVoltage[3];    // L-L voltage (V)
    float ActivePower[3];     // Active power (W)
    float TotalActivePower;  // Total active power (W)
    float Frequency;           // Frequency (Hz)
    float PowerFactor;        // Power factor
    float ActiveEnergy[3];       //3-phase actice enrgy (KWH)
    float TotalACtiveEnergy;   //Total active energy (KWH)
}AC_EnergyMeterDataStruct;

typedef union {
    uint16_t ACEMModBusStatusVal;
    struct {
        ModbusCommStatus ACCurrentStatus : 2;
        ModbusCommStatus LNVoltageStatus : 2;
        ModbusCommStatus ACPowerStatus : 2;
        ModbusCommStatus TotalActivePowerStatus : 2;
        ModbusCommStatus PowerfactorStatus : 2;
        ModbusCommStatus FrequencyStatus : 2;
        ModbusCommStatus ActiveEnergy : 2;  // Fills up to 16 bits
        ModbusCommStatus TotalActiveEnergy : 2;
    } StatusBits;  // Named struct
} ACEMModBusStatusStruct;




// Function to read 3-phase current from the energy meter
/**
 *  @brief Reads a 64-bit floating-point parameter from a Modbus device.
 *
 *  @param AC Energy Meter device ID(ACEMID) (slave address of the target device).
 *  @param StartAddress Starting Modbus register address to read the value from.
 *  @param ParaBuffer Pointer to the buffer where the 64-bit float result will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 */
ModbusCommStatus ACEMRead3PhaseValue(uint32_t StartAddress, float *phaseBuffer,uint8_t TimeoutMS);

/**
 *  @brief Reads a 64-bit floating-point parameter from a Modbus device.
 *
 *  @param AC Energy Meter device ID(ACEMID) (slave address of the target device).
 *  @param StartAddress Starting Modbus register address to read the value from.
 *  @param ParaBuffer Pointer to the buffer where the 64-bit float result will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 */
ModbusCommStatus ACReadOneParaValue(uint32_t StartAddress, float *ParaBuffer,uint8_t TimeoutMS);

/**
 *  @brief Reads the AC Energy meter data values from the DCEMeterData.
 *
 *  @param Data Pointer to the AC_EnergyMeterDataStruct structure buffer where the AC energy meter values will be stored.
 *  @param Status Pointer to a variable where the Modbus communication status will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 */
void ReadACEMeterData(AC_EnergyMeterDataStruct *Data,ACEMModBusStatusStruct *Status,uint8_t TimeoutMS);

#endif // AC_ENERGY_METER_H

