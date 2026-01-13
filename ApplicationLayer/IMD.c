/*
 * IMD.c
 *
 *  Created on: Feb 19, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

#include "IMD.h"

/**
 *  IMDReadOneParaValue32
 *
 *  @brief Reads a 32-bit floating-point parameter from a Modbus device.
 *
 *  This function initiates a read command over Modbus to get a 32-bit floating-point value from the specified device.
 *  Internally, it uses the ReadRegisters() function with the MODBUS_TYPE_FLOAT32 data type, requesting 2 registers (4 bytes).
 *
 *  @param ID Modbus device ID (slave address of the target device).
 *  @param StartAddress Starting Modbus register address to read the value from.
 *  @param ParaBuffer Pointer to the buffer where the 32-bit float result will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 *
 *  @return ModbusCommStatus Returns communication status such as success, timeout, or CRC error.
 */
ModbusCommStatus IMDReadOneParaValue32(uint8_t ID,uint32_t StartAddress, float32_t *ParaBuffer,uint8_t TimeoutMS)
{
    ModbusCommStatus XYZ;
    XYZ = ReadRegisters(ID, StartAddress,2,(float32_t *)ParaBuffer, MODBUS_TYPE_FLOAT32, TimeoutMS);
    return XYZ;
}

/**
 *  @brief Reads insulation resistance data from the IMD and returns the status.
 *
 *  This function acts as a wrapper for reading a single 32-bit float value from the IMD. It uses IMDReadOneParaValue32()
 *  with a fixed address (MODBUS_ADDRESS_IR) which corresponds to the insulation resistance parameter.
 *
 *  @param ID Modbus device ID of the IMD (isolation monitoring device).
 *  @param Data Pointer to the float buffer where the insulation resistance value will be stored.
 *  @param Status Pointer to a variable where the Modbus communication status will be stored.
 *  @param TimeoutMS Timeout in milliseconds for Modbus communication.
 *
 *  @return None.
 */
void ReadIMDData(uint8_t ID,float32_t *Data,uint8_t *Status,uint8_t TimeoutMS)
{
    *Status= IMDReadOneParaValue32(ID,MODBUS_ADDRESS_IR,Data,TimeoutMS);
}

void IMDWriteRegisters(uint8_t DeviceID,uint32_t StartAddress,uint8_t size,uint8_t Data)
{
    ModbusBusWriteFrameStruct TxFrame;
    uint8_t numberofbytes = 0x0001;
    uint8_t RxBuff[8];

    // Fill Modbus frame for 3-phase current reading
    ModbusFrameRegisterWrite(&TxFrame, DeviceID, StartAddress,size, Data,numberofbytes);
    //MCU_UART_TxData(MODBUS_SCI_BASE,TxFrame.msg,WriteRegisterByteSize);
    MCU_UART_TxRxData(MODBUS_SCI_BASE,(uint8_t *)&TxFrame,11,RxBuff,8,20);
}
