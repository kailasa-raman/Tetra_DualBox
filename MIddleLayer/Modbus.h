#ifndef MODBUS_H
#define MODBUS_H

#include <stdint.h>

#ifndef MCUSPECIFIC_H
#include "MCUSpecific.h"
#endif

#ifndef DRIVERLIB_H
    #include "driverlib.h"
#endif

#ifndef DEVICE_H
    #include "device.h"
#endif

// Configuration macros for SCI
#define ReadRegisterByteSize 8
#define WriteRegisterByteSize 11
#define MODBUSHEADER 5

#define MODBUS_FUNC_READ_COILS          0x01
#define MODBUS_FUNC_READ_DISCRETE_INPUT 0x02
#define MODBUS_FUNC_READ_HOLDING_REGS   0x03
#define MODBUS_FUNC_READ_INPUT_REGS     0x04
#define MODBUS_FUNC_WRITE_SINGLE_COIL   0x05
#define MODBUS_FUNC_WRITE_SINGLE_REG    0x06
#define MODBUS_FUNC_WRITE_MULTIPLE_COILS 0x0F
#define MODBUS_FUNC_WRITE_MULTIPLE_REGS 0x10

#define MODBUS_FUNC_RESPONCE_SIZE 20

#define MODBUS_TIMOUT_MS        20


typedef union
{
    uint8_t msg[8];  // Complete 8-byte Modbus frame
    struct
    {
        uint8_t DeviceID;      // 1 byte
        uint8_t FunctionCode;  // 1 byte
        uint8_t AddressHi;     // 1 byte (High byte of Address)
        uint8_t AddressLo;     // 1 byte (Low byte of Address)
        uint8_t SizeHi;        // 1 byte (High byte of Size)
        uint8_t SizeLo;        // 1 byte (Low byte of Size)
        uint8_t CRCLo;         // 1 byte (Low byte of CRC)
        uint8_t CRCHi;         // 1 byte (High byte of CRC)
    };
} ModbusBusFrameStruct;

typedef union
{
    uint8_t msg[11];  // Complete 8-byte Modbus frame
    struct
    {
        uint8_t DeviceID;      // 1 byte
        uint8_t FunctionCode;  // 1 byte
        uint8_t StartRegisterHi;     // 1 byte (High byte of Address)
        uint8_t StartRegisterLo;     // 1 byte (Low byte of Address)
        uint8_t NumberOfRegisterHi;        // 1 byte (High byte of Size)
        uint8_t NumberOfRegisterLo;        // 1 byte (Low byte of Size)
        uint8_t NumOfDataBytes;
        uint8_t DataHi;        // 1 byte (High byte of Data)
        uint8_t DataLo;        // 1 byte (Low byte of Data)
        uint8_t CRCLo;         // 1 byte (High byte of CRC)
        uint8_t CRCHi;         // 1 byte (Low byte of CRC)
    };
} ModbusBusWriteFrameStruct;

typedef enum {
    MODBUS_STATUS_OK = 0,        // Valid response and same function code
    MODBUS_STATUS_TIMOUT = 1,
    MODBUS_STATUS_CRC_ERROR = 2, // CRC error
    MODBUS_STATUS_FUNC_CODE_ERR = 3 // Invalid function code
}ModbusCommStatus;


typedef enum {
    MODBUS_TYPE_FLOAT32,
    MODBUS_TYPE_INT32,
    MODBUS_TYPE_INT16,
    MODBUS_TYPE_INT64,
}ModbusDataType;

/**
 *  @brief Prepares a Modbus RTU frame to read holding registers.
 *
 *  @param MBusFrame Pointer to the Modbus frame structure to fill.
 *  @param ID Modbus device/slave ID to communicate with.
 *  @param Address Starting register address to read from.
 *  @param Size Number of registers to read.
 */
void ModbusFrameRegisterRead(ModbusBusFrameStruct *MBusFrame, uint8_t ID, uint16_t Address, uint16_t Size);

// Function declarations
/**
 *  This function sets up the transmit and receive pins, as well as the direction control pin using `setModBusGPIO()`.
 *  It also configures the UART settings such as base address, parity, stop bits, and data bits using `setModBusUART()`.
 */
void Modbus_Init(void);

/**
 *  @brief Sends a Modbus request and receives the response.
 *
 *  @param TxData Pointer to the Modbus transmit buffer (data to send).
 *  @param Txlength Length of the transmit data in bytes.
 *  @param RxData Pointer to the buffer where the received Modbus data will be stored.
 *  @param Rxlength Expected length of the received data in bytes.
 *  @param timeout Timeout duration in milliseconds for receiving the response.
 *
 *  @return 1 if communication was successful, 0 if timeout or error occurred.
 */
uint8_t Modbus_TxRx(uint8_t *TxData, uint16_t Txlength,uint8_t *RxData,uint8_t Rxlength,uint8_t TimoutMS);
//uint16_t Modbus_Receive(uint8_t *buffer, uint16_t bufferSize);

/**
 *  @brief Calculates CRC16 for a Modbus frame.
 *
 *  @param data Pointer to the data buffer for which the CRC is to be calculated.
 *  @param length Number of bytes in the data buffer.
 *  @param CRCLo Pointer to store the calculated CRC low byte.
 *  @param CRCHi Pointer to store the calculated CRC high byte.
 */
void CRC16(uint8_t *Data, uint16_t Length, uint8_t *CRCLo, uint8_t *CRCHi) ;

/**
 *  @brief Reads multiple Modbus holding registers and stores the values in a buffer.
 *
 * @param DeviceID The Modbus device ID of the slave device to communicate with.
 * @param StartAddress The starting register address in the slave device to begin reading from.
 * @param ModbusRegQuantity Number of Modbus registers to read from the StartAddress.
 * @param dataBuffer Pointer to a buffer where the decoded data (float32, int16, int32, int64) will be stored.
 * @param dataType The data type of the values to be interpreted (MODBUS_TYPE_FLOAT32, MODBUS_TYPE_INT16, etc.).
 * @param TimeoutMS Timeout in milliseconds for waiting for the Modbus response.
 *
 * @return Returns a ModbusCommStatus enum value indicating the result:
 *         - MODBUS_STATUS_OK: Data read and validated successfully.
 *         - MODBUS_STATUS_TIMOUT: No response received within timeout.
 *         - MODBUS_STATUS_CRC_ERROR: CRC check failed.
 *         - MODBUS_STATUS_FUNC_CODE_ERR: Function code mismatch.
 */
ModbusCommStatus ReadRegisters(uint8_t DeviceID,uint32_t StartAddress,uint8_t ModbusRegQuantity,float32_t *dataBuffer,ModbusDataType dataType, uint8_t TimeoutMS);

void ModbusFrameRegisterWrite(ModbusBusWriteFrameStruct *MBusFrame, uint8_t ID, uint16_t Address, uint16_t Size,uint16_t Data,uint8_t number_of_registers);
#endif /* MODBUS_H */
