#include "DCEMeter.h"
int32_t VoltageX;

/**
 *  DCReadOneParaValue32
 *
 *  @brief Reads a 32-bit floating-point parameter from a Modbus device.
 *
 *  This function initiates a read command over Modbus to get a 32-bit floating-point value from the specified device.
 *  Internally, it uses the ReadRegisters() function with the MODBUS_TYPE_FLOAT32 data type, requesting 2 registers (4 bytes).
 *
 *  @param DC Energy Meter device ID (slave address of the target device).
 *  @param StartAddress Starting Modbus register address to read the value from.
 *  @param ParaBuffer Pointer to the buffer where the 32-bit float result will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 *
 *  @return ModbusCommStatus Returns communication status such as success, timeout, or CRC error.
 */
ModbusCommStatus DCReadOneParaValue32(uint8_t ID, uint32_t StartAddress, float32_t *ParaBuffer, uint8_t TimeoutMS)
{
    ModbusCommStatus XYZ;
    float32_t tempDC=0;
    // Change from float64_t to int32_t
    XYZ = ReadRegisters(ID, StartAddress, 2, (float32_t * )&tempDC, MODBUS_TYPE_FLOAT32, TimeoutMS);
    *ParaBuffer = (float32_t)(tempDC);  // Assign value properly
    return XYZ;
}

/**
 *  DCReadOneParaValue64
 *
 *  @brief Reads a 64-bit floating-point parameter from a Modbus device.
 *
 *  This function initiates a read command over Modbus to get a 64-bit floating-point value from the specified device.
 *  Internally, it uses the ReadRegisters() function with the MODBUS_TYPE_FLOAT32 data type, requesting 4 registers (8 bytes).
 *
 *  @param DC Energy Meter device ID (slave address of the target device).
 *  @param StartAddress Starting Modbus register address to read the value from.
 *  @param ParaBuffer Pointer to the buffer where the 64-bit float result will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 *
 *  @return ModbusCommStatus Returns communication status such as success, timeout, or CRC error.
 */
ModbusCommStatus DCReadOneParaValue64(uint8_t ID, uint32_t StartAddress, float32_t *ParaBuffer, uint8_t TimeoutMS)
{
    ModbusCommStatus XYZ;
    float32_t tempDC1=0;
    XYZ = ReadRegisters(ID, StartAddress, 4, (float32_t *)&tempDC1, MODBUS_TYPE_INT64, TimeoutMS);
    //DEVICE_DELAY_US(10000);
    if(tempDC1 != 0)
        *ParaBuffer = (float32_t)tempDC1;  // Assign value properly
    return XYZ;
}

/**
 *  ReadDCEMeterData
 *
 *  @brief Reads the DC current, voltage, power, positive energy and negative energy data from the DCEMeterData and returns the status.
 *
 *  This function acts as a wrapper for reading a more 32-bit and 64-bit float value from the DC energy meter. It uses
 *  DCReadOneParaValue32() and DCReadOneParaValue64 with a different address for different data which corresponds to the
 *  DC Energy meter readings.
 *
 *  @param ID Modbus device ID of the IMD (DC Energy meter device).
 *  @param Data Pointer to the DC_EnergyMeterDataStruct structure buffer where the DC energy meter values will be stored.
 *  @param Status Pointer to a variable where the Modbus communication status will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 *
 *  @return None.
 */
void ReadDCEMeterData(uint8_t ID,DC_EnergyMeterDataStruct *Data,DCEMModBusStatusStruct *Status,uint8_t TimeoutMS)
{
    Status->StatusBits.DCCurrentStatus = DCReadOneParaValue32(ID,MODBUS_ADDRESS_DCCURRENT,&(Data->Current),TimeoutMS);
    DEVICE_DELAY_US(DCMETERCOMDELAY);
    Status->StatusBits.DCVoltageStatus = DCReadOneParaValue32(ID,MODBUS_ADDRESS_DCVOLTAGE,&(Data->Voltage),TimeoutMS);
    DEVICE_DELAY_US(DCMETERCOMDELAY);
    Status->StatusBits.DCPowerStatus =   DCReadOneParaValue32(ID,MODBUS_ADDRESS_DCPOWER,&(Data->Power),TimeoutMS);
    DEVICE_DELAY_US(DCMETERCOMDELAY);
    Status->StatusBits.PosEnergyStatus = DCReadOneParaValue64(ID,MODBUS_ADDRESS_DCPOS_ENERGY1,&(Data->PosEnergy),TimeoutMS);
    DEVICE_DELAY_US(DCMETERCOMDELAY);

    /*Status->StatusBits.PosEnergyStatus = DCReadOneParaValue32(ID,MODBUS_ADDRESS_DCPOS_ENERGY2,&Temp2,TimeoutMS);
    DEVICE_DELAY_US(DCMETERCOMDELAY);
    if(Temp1 ==0  && Temp2 ==0)
    {
        Status->StatusBits.PosEnergyStatus= MODBUS_STATUS_FUNC_CODE_ERR;
    }
    else if(Temp1 == Temp2)
    {
        Data->PosEnergy= Temp1;
    }
    else if(Temp1 == 0 && Temp2 != 0)
    {
        Data->PosEnergy= Temp2;
    }
    else if(Temp1 != 0 && Temp2 == 0)
    {
        Data->PosEnergy= Temp1;
    }*/
    //Status->StatusBits.NegEnergyStatus = DCReadOneParaValue64(ID,MODBUS_ADDRESS_DCNEG_ENERGY,&(Data->NegEnergy),TimeoutMS);
    //DEVICE_DELAY_US(DCMETERCOMDELAY);
}
