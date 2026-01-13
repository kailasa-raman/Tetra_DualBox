#include "modbus.h"

/**
 *  The table stores the CRC values for each possible byte (0-255) based on the chosen CRC polynomial.
 */
const uint16_t CRCTable_8005[] = {
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
    0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
    0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
    0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
    0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
    0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
    0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
    0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
    0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
    0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
    0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
    0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
    0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
    0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
    0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
    0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
    0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
    0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
    0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
    0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
    0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
    0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
    0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
    0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
};

const uint16_t CRCTable[] = {
        0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
        0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
        0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
        0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
        0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
        0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
        0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
        0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
        0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
        0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
        0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
        0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
        0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
        0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
        0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
        0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
        0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
        0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
        0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
        0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
        0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
        0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
        0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
        0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
        0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
        0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
        0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
        0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
        0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
        0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
        0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
        0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
    };

/**
 *  Modbus_Init
 *
 *  @brief Initializes the Modbus communication settings.
 *
 *  This function sets up the transmit and receive pins, as well as the direction control pin using `setModBusGPIO()`.
 *  It also configures the UART settings such as base address, parity, stop bits, and data bits using `setModBusUART()`.
 *
 *  @return None.
 */
void Modbus_Init(void) {
    // GPIO Configuration
    setModBusGPIO();
    setModBusUART();


    // SCI Configuration
}

/**
 *  ModbusFrameRegisterRead
 *
 *  @brief Prepares a Modbus RTU frame to read holding registers.
 *
 *  This function builds a Modbus RTU request frame with Function Code, used to read holding registers from a Modbus slave device.
 *  The generated frame includes the device ID, function code, start address, register count, and CRC.
 *
 *  @param MBusFrame Pointer to the Modbus frame structure to fill.
 *  @param ID Modbus device/slave ID to communicate with.
 *  @param Address Starting register address to read from.
 *  @param Size Number of registers to read.
 *
 *  @return None.
 */
void ModbusFrameRegisterRead(ModbusBusFrameStruct *MBusFrame, uint8_t ID, uint16_t Address, uint16_t Size)
{
    uint8_t CRCLow, CRCHigh;
    MBusFrame->DeviceID = ID;
    MBusFrame->FunctionCode = MODBUS_FUNC_READ_HOLDING_REGS;  // Read Input Registers

    // Store Address in Big-Endian
    MBusFrame->AddressHi = (Address >> 8) & 0xFF;
    MBusFrame->AddressLo = Address & 0xFF;

    // Store Size in Big-Endian
    MBusFrame->SizeHi = (Size >> 8) & 0xFF;
    MBusFrame->SizeLo = Size & 0xFF;
    CRC16(MBusFrame->msg,6,&CRCLow,&CRCHigh);
    // Store CRC in Little-Endian
    MBusFrame->CRCLo =CRCLow;
    MBusFrame->CRCHi = CRCHigh;

}

/**
 *  Modbus_TxRx
 *
 *  @brief Sends a Modbus request and receives the response.
 *
 *  This function sends a Modbus RTU frame over the communication interface(usually UART) and waits for the slave's response.
 *  The received data is stored in the provided RxData buffer.
 *
 *  @param TxData Pointer to the Modbus transmit buffer (data to send).
 *  @param Txlength Length of the transmit data in bytes.
 *  @param RxData Pointer to the buffer where the received Modbus data will be stored.
 *  @param Rxlength Expected length of the received data in bytes.
 *  @param timeout Timeout duration in milliseconds for receiving the response.
 *
 *  @return 1 if communication was successful, 0 if timeout or error occurred.
 */
uint8_t Modbus_TxRx(uint8_t *TxData, uint16_t Txlength,uint8_t *RxData,uint8_t Rxlength,uint8_t timout)
{
    uint8_t status;
    status = MCU_UART_TxRxData(MODBUS_SCI_BASE,TxData,Txlength,RxData,Rxlength,timout);
    return status;
}

/*
uint16_t Modbus_Receive(uint8_t *buffer, uint16_t bufferSize) {
    uint16_t count = 0;

    while (SCI_getRxFIFOStatus(MODBUS_SCI_BASE) != SCI_FIFO_RX0 && count < bufferSize) {
        buffer[count++] = SCI_readCharBlockingFIFO(MODBUS_SCI_BASE);
    }

    return count;
}
*/

/**
 * @brief Calculates CRC16 for a Modbus frame.
 *
 * @param data Pointer to the data buffer for which the CRC is to be calculated.
 * @param length Number of bytes in the data buffer.
 * @param CRCLo Pointer to store the calculated CRC low byte.
 * @param CRCHi Pointer to store the calculated CRC high byte.
 *
 * This function computes the Modbus CRC16 checksum and stores the result
 * as two separate bytes: a low byte and a high byte.
 *
 * @return None
 */
void CRC16(uint8_t *Data, uint16_t Length, uint8_t *CRCLo, uint8_t *CRCHi)
{
    uint16_t CRCWord = 0xFFFF;

    while (Length--) {
        uint8_t Temp = (*Data++) ^ (CRCWord & 0xFF);
        CRCWord = (CRCWord >> 8) ^ CRCTable[Temp];  // Correct usage of 16-bit table
    }

    // Store CRC in Little Endian format (Low byte first)
    *CRCLo = CRCWord & 0xFF;        // Low byte
    *CRCHi = (CRCWord >> 8) & 0xFF; // High byte
}

/**
 * @brief Reads multiple Modbus holding registers and stores the values in a buffer.
 *
 * @param DeviceID The Modbus device ID of the slave device to communicate with.
 * @param StartAddress The starting register address in the slave device to begin reading from.
 * @param ModbusRegQuantity Number of Modbus registers to read from the StartAddress.
 * @param dataBuffer Pointer to a buffer where the decoded data (float32, int16, int32, int64) will be stored.
 * @param dataType The data type of the values to be interpreted (MODBUS_TYPE_FLOAT32, MODBUS_TYPE_INT16, etc.).
 * @param TimeoutMS Timeout in milliseconds for waiting for the Modbus response.
 *
 * This function builds a Modbus RTU frame to request reading of specified registers, transmits the request,
 * receives the response, checks for CRC and function code correctness, and finally decodes the received register
 * data into the appropriate data type format based on the `dataType` provided. Supported types include 16-bit,
 * 32-bit, and 64-bit values.
 *
 * Internally, the function handles CRC checking and error validation.
 * The decoded data is saved into the provided `dataBuffer` pointer.
 *
 * @return Returns a ModbusCommStatus enum value indicating the result:
 *         - MODBUS_STATUS_OK: Data read and validated successfully.
 *         - MODBUS_STATUS_TIMOUT: No response received within timeout.
 *         - MODBUS_STATUS_CRC_ERROR: CRC check failed.
 *         - MODBUS_STATUS_FUNC_CODE_ERR: Function code mismatch.
 */
ModbusCommStatus ReadRegisters(uint8_t DeviceID,uint32_t StartAddress,uint8_t ModbusRegQuantity,float32_t *dataBuffer,ModbusDataType dataType, uint8_t TimeoutMS)
{
    float32_t rawValueTemp2;
    uint8_t i = 0;
    uint8_t RxBufferSize =  ModbusRegQuantity*2 + MODBUSHEADER;
    uint32_t FinalValue=0;
    uint8_t RxBuffer[MODBUS_FUNC_RESPONCE_SIZE];
    uint8_t ResponceCRCLo,ResponceCRCHo;
    ModbusBusFrameStruct TxFrame;

    // Fill Modbus frame for 3-phase current reading
    ModbusFrameRegisterRead(&TxFrame, DeviceID, StartAddress, ModbusRegQuantity);  // 0x01 = Device ID, 0x0BB7 = Address, 0x06 = Size (6 registers)
    // Transmit and receive Modbus data using your Modbus TxRx function
    uint8_t status = Modbus_TxRx(TxFrame.msg, ReadRegisterByteSize, RxBuffer, RxBufferSize, TimeoutMS);  // RxBuffer will store the received data
    if (status == 1)
    {
        return MODBUS_STATUS_TIMOUT;
    }
    // Check CRC validity
    uint8_t RxSize = RxBuffer[2];
    uint8_t CRCLo = RxBuffer[3+RxSize];
    uint8_t CRCHi = RxBuffer[4+RxSize];
    uint16_t receivedCRC = ((uint16_t)CRCHi << 8) | CRCLo;

    CRC16(RxBuffer, 3+RxSize, &ResponceCRCLo, &ResponceCRCHo);

    uint16_t calculatedCRC = ((uint16_t)ResponceCRCHo << 8) | ResponceCRCLo;

    // Validate CRC and function code
    if (receivedCRC != calculatedCRC) {
        return MODBUS_STATUS_CRC_ERROR; // CRC mismatch
    }

    if (RxBuffer[1] != TxFrame.FunctionCode) {
        return MODBUS_STATUS_FUNC_CODE_ERR; // Function code mismatch
    }

    // If no errors, convert the raw data to engineering values
    uint32_t raw_value;
    switch (dataType)
       {
           case MODBUS_TYPE_FLOAT32:
                   for (i = 0; i < (ModbusRegQuantity / 2); i++)
                   {
                     raw_value = ((uint32_t)RxBuffer[3 + i * 4] << 24) |
                               ((uint32_t)RxBuffer[4 + i * 4] << 16) |
                               ((uint32_t)RxBuffer[5 + i * 4] << 8)  |
                               (uint32_t)RxBuffer[6 + i * 4];
                     memcpy(&rawValueTemp2, &raw_value, sizeof(float));
                    ((float32_t *)dataBuffer)[i] = (float32_t)rawValueTemp2;
                   }

                   //rawValueTemp2 = (float)rawValueTemp1;
                   break;

           case MODBUS_TYPE_INT32:
               for (i = 0; i < (ModbusRegQuantity / 2); i++) {
                   uint32_t raw_value = ((uint32_t)RxBuffer[3 + i * 4] << 24) |
                                        ((uint32_t)RxBuffer[4 + i * 4] << 16) |
                                        ((uint32_t)RxBuffer[5 + i * 4] << 8)  |
                                        (uint32_t)RxBuffer[6 + i * 4];
                   ((int32_t*)dataBuffer)[i] = (int32_t)raw_value;
               }
               break;

           case MODBUS_TYPE_INT16:
               for (i = 0; i < ModbusRegQuantity; i++) {
                   uint16_t raw_value = ((uint16_t)RxBuffer[3 + i * 2] << 8) | (uint16_t)RxBuffer[4 + i * 2];
                   ((int16_t*)dataBuffer)[i] = (int16_t)raw_value;
               }
               break;

           case MODBUS_TYPE_INT64:
               if(dataType ==MODBUS_TYPE_INT64 )
               {
                   uint32_t raw_value = ((uint64_t)RxBuffer[3] << 24) |
                                        ((uint64_t)RxBuffer[4] << 16) |
                                        ((uint64_t)RxBuffer[5] << 8) |
                                        ((uint64_t)RxBuffer[6]);

                   uint32_t raw_value1 =((uint64_t)RxBuffer[7] << 24) |
                                        ((uint64_t)RxBuffer[8] << 16) |
                                        ((uint64_t)RxBuffer[9] << 8)  |
                                        ((uint64_t)RxBuffer[10]);
                   if(raw_value == raw_value1)
                   {
                       raw_value1 =0;
                       FinalValue =  raw_value;
                   }
                   else if(raw_value == 0  && raw_value1 != 0)
                   {
                       FinalValue = raw_value1;
                   }
                   else if(raw_value1 == 0  && raw_value != 0)
                   {
                       FinalValue = raw_value;
                   }
               }

                   memcpy(&rawValueTemp2, &FinalValue, sizeof(float32_t));
                   ((float32_t *)dataBuffer)[i] = (float32_t)rawValueTemp2;
               break;

       }

    return MODBUS_STATUS_OK; // Successfully read and validated
}
/**
 * @brief writes the values in the ModbusBusWriteFrameStruct.
 *
 * @param MBusFrame Pointer to the ModbusBusWriteFrameStruct structure that stores the values in it .
 * @param ID  is the Device ID.
 * @param Address is the register address.
 * @param Size is the number of registers.
 * @param Data is the data value to be stored.
 * @param numberofbytes is the number of data bytes to be stored.
 *
 * This function gets all the data to be written in the modbus frame register and writes them properly.
 *
 * @return None
 */
void ModbusFrameRegisterWrite(ModbusBusWriteFrameStruct *MBusFrame, uint8_t ID, uint16_t Address, uint16_t Size,uint16_t Data,uint8_t numberofbytes)
{
    uint8_t CRCLow, CRCHigh;
    MBusFrame->DeviceID = ID;
    MBusFrame->FunctionCode = MODBUS_FUNC_WRITE_MULTIPLE_REGS;  // Write in the Registers

    // Store Start Register in Big-Endian
    MBusFrame->StartRegisterHi = (Address >> 8) & 0xFF;
    MBusFrame->StartRegisterLo = Address & 0xFF;

    // Store Number of Register in Big-Endian
    MBusFrame->NumberOfRegisterHi = (Size >> 8) & 0xFF;
    MBusFrame->NumberOfRegisterLo = Size & 0xFF;

    MBusFrame->NumOfDataBytes = numberofbytes;

    MBusFrame->DataHi = (Data >> 8) & 0xFF;
    MBusFrame->DataLo = Data & 0xFF;
    CRC16(MBusFrame->msg,9,&CRCLow,&CRCHigh);

    // Store CRC in Little-Endian
    MBusFrame->CRCLo =CRCLow;
    MBusFrame->CRCHi = CRCHigh;

}
