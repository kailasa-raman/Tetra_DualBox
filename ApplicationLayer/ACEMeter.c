#include "ACEMeter.h"

//-------------------Check This  -------------------
/**
 *  ACEMRead3PhaseValue
 *
 *  @brief Reads a 64-bit floating-point parameter from a Modbus device.
 *
 *  This function initiates a read command over Modbus to get a 64-bit floating-point value from the specified device.
 *  Internally, it uses the ReadRegisters() function with the MODBUS_TYPE_FLOAT32 data type, requesting 4 registers (8 bytes).
 *  Modbus register length needed to read 3 parameters.
 *
 *  @param AC Energy Meter device ID(ACEMID) (slave address of the target device).
 *  @param StartAddress Starting Modbus register address to read the value from.
 *  @param ParaBuffer Pointer to the buffer where the 64-bit float result will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 *
 *  @return ModbusCommStatus Returns communication status such as success, timeout, or CRC error.
 */
ModbusCommStatus ACEMRead3PhaseValue(uint32_t StartAddress, float *phaseBuffer,uint8_t TimeoutMS)
{
    ModbusCommStatus XYZ;
    XYZ =  ReadRegisters(ACEMID, StartAddress, MODBUS_LENGTH_3PHASE,(float32_t *)phaseBuffer, MODBUS_TYPE_FLOAT32, TimeoutMS);

    return XYZ;
}

/**
 *  ACReadOneParaValue
 *
 *  @brief Reads a 64-bit floating-point parameter from a Modbus device.
 *
 *  This function initiates a read command over Modbus to get a 64-bit floating-point value from the specified device.
 *  Internally, it uses the ReadRegisters() function with the MODBUS_TYPE_FLOAT32 data type, requesting 4 registers (8 bytes).
 *  Modbus register length needed to read 1 parameter.
 *
 *  @param AC Energy Meter device ID(ACEMID) (slave address of the target device).
 *  @param StartAddress Starting Modbus register address to read the value from.
 *  @param ParaBuffer Pointer to the buffer where the 64-bit float result will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 *
 *  @return ModbusCommStatus Returns communication status such as success, timeout, or CRC error.
 */
ModbusCommStatus ACReadOneParaValue(uint32_t StartAddress, float *ParaBuffer,uint8_t TimeoutMS)
{
    ModbusCommStatus XYZ;
    XYZ = ReadRegisters(ACEMID, StartAddress,MODBUS_LENGTH_1PARA,(float32_t *)ParaBuffer, MODBUS_TYPE_FLOAT32, TimeoutMS);
    return XYZ;
}

/**
 *  ReadDCEMeterData
 *
 *  @brief Reads the AC Energy meter data values from the ACEMeterData.
 *
 *  This function acts as a wrapper for reading a more 32-bit and 64-bit float value from the AC energy meter.
 *  It uses ACReadOneParaValue32() and ACReadOneParaValue64 with a different address for different data which
 *  corresponds to the DC Energy meter readings.
 *
 *  @param Data Pointer to the AC_EnergyMeterDataStruct structure buffer where the AC energy meter values will be stored.
 *  @param Status Pointer to a variable where the Modbus communication status will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 *
 *  @return None.
 */
void ReadACEMeterData(AC_EnergyMeterDataStruct *Data,ACEMModBusStatusStruct *Status,uint8_t TimeoutMS)
{
    Status->StatusBits.ACCurrentStatus = ACEMRead3PhaseValue(MODBUS_ADDRESS_CURRENT,Data->Linecurrent,TimeoutMS);
    DEVICE_DELAY_US(ACMETERCOMDELAY);
    Status->StatusBits.LNVoltageStatus = ACEMRead3PhaseValue(MODBUS_ADDRESS_VOLTAGE_LN,Data->LNVoltage,TimeoutMS);
    DEVICE_DELAY_US(ACMETERCOMDELAY);
    //Status->StatusBits.LLVoltageStatus = ACEMRead3PhaseValue(MODBUS_ADDRESS_VOLTAGE_LL,Data->LLVoltage,TimeoutMS);
    //DEVICE_DELAY_US(ACMETERCOMDELAY);
    Status->StatusBits.ACPowerStatus   = ACEMRead3PhaseValue(MODBUS_ADDRESS_ACTIVE_POWER,Data->ActivePower,TimeoutMS);
    DEVICE_DELAY_US(ACMETERCOMDELAY);
    Status->StatusBits.TotalActivePowerStatus   =  ACReadOneParaValue(MODBUS_ADDRESS_TOTAL_ACTIVE_POWER,&(Data->TotalActivePower),TimeoutMS);
    DEVICE_DELAY_US(ACMETERCOMDELAY);
    Status->StatusBits.FrequencyStatus   =  ACReadOneParaValue(MODBUS_ADDRESS_FREQUENCY,&(Data->Frequency),TimeoutMS);
    DEVICE_DELAY_US(ACMETERCOMDELAY);
    Status->StatusBits.PowerfactorStatus   =  ACReadOneParaValue(MODBUS_ADDRESS_POWER_FACTOR,&(Data->PowerFactor),TimeoutMS);
    DEVICE_DELAY_US(ACMETERCOMDELAY);
    //Status->StatusBits.ActiveEnergy   =  ACEMRead3PhaseValue(MODBUS_ADDRESS_ENERGY,Data->ActiveEnergy,TimeoutMS);
    //DEVICE_DELAY_US(ACMETERCOMDELAY);
    Status->StatusBits.ActiveEnergy   =  ACReadOneParaValue(MODBUS_ADDRESS_TOTALACTIVE_ENERGY,&(Data->ActiveEnergy[0]),TimeoutMS);
    DEVICE_DELAY_US(ACMETERCOMDELAY);
    Status->StatusBits.TotalActiveEnergy   =  ACReadOneParaValue(MODBUS_ADDRESS_TOTALACTIVE_ENERGY,&(Data->TotalACtiveEnergy),TimeoutMS);
    DEVICE_DELAY_US(ACMETERCOMDELAY);
}

