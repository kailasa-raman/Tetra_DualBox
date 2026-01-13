/*
 * TCA9539.c
 *
 *  Created on: Mar 4, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

#include "TCA9539.h" // Defines I2C_BASE_IOEXPANDER

/**
 *  TCAL9539_WriteRegister
 *
 *  @brief It is used to write in I2C slave.
 *
 *  This function is helpful to write in the I2C slave port and pins.
 *
 *  @param base is the base address of the I2C instance used.
 *  @param slaveAddress Address of a slave.
 *  @param reg where we give port number.
 *  @param data data we sent.
 *
 *  @return None.
 */
I2CCommStatus TCAL9539_WriteRegister(uint32_t base, uint8_t slaveAddress, uint8_t reg, uint8_t data)
{
    uint8_t Attempt= 0;
    while(I2CisBusBusy(base) == 1)
    {
        Attempt++;
        DEVICE_DELAY_US(100);
        if(Attempt >= 5)
        {
            return I2CBusyTimout;
        }
    }
    Attempt= 0;
    while(I2CgetStopConditionStatus(base) == 1)
    {
        Attempt++;
        DEVICE_DELAY_US(100);
        if(Attempt >= 5)
        {
            return I2CStopTimout;
        }
    }
    Attempt = 0;
    I2CenableFIFO(base);
    I2CsetConfig(base,I2C_CONTROLLER_SEND_MODE);
    I2CsetSlaveAddress(base,slaveAddress);
    I2CsetDataCount(base,3);
    I2CputData(base,reg);
    I2CputData(base,data);
    I2CputData(base,data);
    I2CsendStartCondition(base);
    while(I2CgetTxFIFOStatus(base))
    {
        Attempt++;
        DEVICE_DELAY_US(100);
        if(Attempt >= 5)
        {
            return I2CTxFifoTimout;
        }
    }
    I2CsendStopCondition(base);
    Attempt= 0;
    while(I2CgetStopConditionStatus(base) == 0)
    {
        Attempt++;
        DEVICE_DELAY_US(100);
        if(Attempt >= 5)
        {
            return I2CStopTimout;
        }
    }
    return (I2CCommStatus)0;
}

/**
 *  TCAL9539_ReadRegister
 *
 *  @brief It is used to read from slave.
 *
 *  This function is helpful to read from I2C slave, from a specific port and pin.
 *
 *  @param base is the base address of the I2C instance used.
 *  @param slaveAddress Address of a slave.
 *  @param reg where we give port number.
 *
 *  @return data.
 */
uint8_t TCAL9539_ReadRegister(uint32_t base, uint8_t slaveAddress, uint8_t reg, uint8_t *I2Cstatus)
{
    //whether to give as status error aor as can message
    if(I2C_Error(base))
       {
           *I2Cstatus = I2CMCUError;
           return 0;
       }

    uint8_t data,i;
    uint8_t tempI2Cdata[2]={0};
    uint8_t Attempt= 0;
  /*  while(I2CisBusBusy(base) == 1)
    {
            Attempt++;
            DEVICE_DELAY_US(100);
            if(Attempt >= 5)
            {
                *I2Cstatus=I2CBusyTimout;
                return 0;
            }
    }*/
    Attempt= 0;
    while(I2CgetStopConditionStatus(base) == 1)
    {
            Attempt++;
            DEVICE_DELAY_US(100);
            if(Attempt >= 5)
            {
               *I2Cstatus=I2CStopTimout;

               return 0;
            }
    }
    I2CenableFIFO(base);
    I2CsetConfig(base,I2C_CONTROLLER_SEND_MODE);
    I2CsetSlaveAddress(base,slaveAddress);
    I2CsetDataCount(base,1);
    I2CputData(base,reg);
    I2CsendStartCondition(base);
    Attempt= 0;
    while(I2CgetTxFIFOStatus(base))
    {
        Attempt++;
        DEVICE_DELAY_US(100);
        if(Attempt >= 5)
           {
            *I2Cstatus=I2CTxFifoTimout;
             return 0;
           }
    }
    I2CsetConfig(base, I2C_CONTROLLER_RECEIVE_MODE);
    I2CsetDataCount(base,2);
    I2CsendStartCondition(base);
    for(i=0;i<2;i++)
    {
        tempI2Cdata[i] = I2C_getData(base);
    }
    Attempt= 0;
    while(I2CgetRxFIFOStatus(base) != I2C_FIFO_RX2)
    {
        Attempt++;
        DEVICE_DELAY_US(100);
        if(Attempt >= 5)
           {
            *I2Cstatus =I2CRxFifoTimout;
             return 0;
           }
    }
    I2CsendStopCondition(base);
    Attempt= 0;
    while(I2CgetStopConditionStatus(base) == 0)
    {
        Attempt++;
        DEVICE_DELAY_US(100);
        if(Attempt >= 5)
        {
            *I2Cstatus =I2CStopTimout;
            return 0;
        }
    }
    data = (tempI2Cdata[0] & 0xFF) + ((tempI2Cdata[1] & 0xFF) << 8);

    *I2Cstatus =I2CSuccess;

    return data;// Send stop condition to end the write phase

}


// Function Implementations
/**
 *  ReadInputPort0
 *
 *  @brief It reads from specfic slave, from port0.
 *
 *  By using this function we can read the data from specific slave, from port0.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress is the slave address.
 *
 *  @return data.
 */
uint8_t ReadInputPort0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t *status) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, INPUT_PORT0, status);
}

/**
 *  ReadInputPort1
 *
 *  @brief It reads from specfic slave, from port1.
 *
 *  By using this function we can read the data from specific slave, from port1.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress is the slave address.
 *
 *  @return data.
 */
uint8_t ReadInputPort1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, INPUT_PORT1, status);
}

/**
 *  WriteOutputPort0
 *
 *  @brief It write in a specific slave, port0.
 *
 *  By using this function we write in a specific slave, in port0.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data data what we write.
 *
 *  @return None.
 */
void WriteOutputPort0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, OUTPUT_PORT0, data);
}

/**
 *  WriteOutputPort1
 *
 *  @brief It write in a specific slave, port1.
 *
 *  By using this function we write in a specific slave, in port1.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data data what we write.
 *
 *  @return None.
 */
void WriteOutputPort1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, OUTPUT_PORT1, data);
}

/**
 *  ReadOutputPort0
 *
 *  @brief It is used to read ouput port0.
 *
 *  By using this function we can read the output port0 from a specific slave based on slave address.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @pram slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadOutputPort0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, OUTPUT_PORT0, status);
}

/**
 *  ReadOutputPort1
 *
 *  @brief It is used to read ouput port1.
 *
 *  By using this function we can read the output port0 from a specific slave based on slave address.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadOutputPort1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, OUTPUT_PORT1, status);
}

/**
 *  WritePolarityInversion0
 *
 *  @brief It is used write a polarity inversion in port0.
 *
 *  By using this function we can write a polarity inversion in a port0 to a specific slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we want to write in slave.
 *
 *  @return None.
 */
void WritePolarityInversion0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data)
{
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, POLARITY_INVERSION0,data);
}

/**
 *  WritePolarityInversion1
 *
 *  @brief It is used write a polarity inversion in port1.
 *
 *  By using this function we can write a polarity inversion in a port1 to a specific slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we want to write in slave.
 *
 *  @return None.
 */
void WritePolarityInversion1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, POLARITY_INVERSION1, data);
}

/**
 *  ReadPolarityInversion0
 *
 *  @brief It is used to read the inversion data.
 *
 *  By using this function we read polarity inversion data from port0 from a specific slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadPolarityInversion0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, POLARITY_INVERSION0, status);
}

/**
 *  ReadPolarityInversion1
 *
 *  @brief It is used to read the inversion data.
 *
 *  By using this function we read polarity inversion data from port1 from a specific slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadPolarityInversion1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, POLARITY_INVERSION1, status);
}

/**
 *  WriteConfiguration0
 *
 *  @brief Configures the port0 as a input or output.
 *
 *  By using this function we can configure the port0 as a input or output.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WriteConfiguration0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, CONFIGURATION0, data);
}

/**
 *  ReadConfiguration0
 *
 *  @brief It reads the port0 is input or output.
 *
 *  By using this function we read that specified pin is a output or input in a port0.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadConfiguration0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, CONFIGURATION0, status);
}

/**
 *  WriteConfiguration1
 *
 *  @brief Configures the port1 as a input or output.
 *
 *  By using this function we can configure the port1 as a input or output.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WriteConfiguration1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, CONFIGURATION1, data);
}

/**
 *  ReadConfiguration1
 *
 *  @brief It reads the port1 is input or output.
 *
 *  By using this function we read that specified pin is a output or input in a port1.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadConfiguration1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, CONFIGURATION1, status);
}

/**
 *  WriteOutputDriveStrength0
 *
 *  @brief sets a driven strength of a specific pin in a port0.
 *
 *  By using this function we can a set drive strength0, 00b = 0.25x drive strength, 01b = 0.5x drive
 *  strength, 10b = 0.75x drive strength, or 11b = 1x for full drive strength capability for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WriteOutputDriveStrength0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, OUTPUT_DRIVE_STRENGTH0, data);
}

/**
 *  ReadOutputDriveStrength0
 *
 *  @brief Reads the driven strength of a port0.
 *
 *  By using this function we can read the driven strength0 for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadOutputDriveStrength0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, OUTPUT_DRIVE_STRENGTH0, status);
}

/**
 *  WriteOutputDriveStrength1
 *
 *  @brief sets a drive strength1.
 *
 *  By using this function we can a set driven strength1, 00b = 0.25x drive strength, 01b = 0.5x drive
 *  strength, 10b = 0.75x drive strength, or 11b = 1x for full drive strength capability for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WriteOutputDriveStrength1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, OUTPUT_DRIVE_STRENGTH1, data);
}

/**
 *  ReadOutputDriveStrength1
 *
 *  @brief Reads the drive strength1.
 *
 *  By using this function we can read the driven strength1 for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadOutputDriveStrength1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, OUTPUT_DRIVE_STRENGTH1, status);
}

/**
 *  WriteOutputDriveStrength2
 *
 *  @brief sets a drive strength2.
 *
 *  By using this function we can a set driven strength2, 00b = 0.25x drive strength, 01b = 0.5x drive
 *  strength, 10b = 0.75x drive strength, or 11b = 1x for full drive strength capability for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WriteOutputDriveStrength2(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, OUTPUT_DRIVE_STRENGTH2, data);
}

/**
 *  ReadOutputDriveStrength2
 *
 *  @brief Reads the drive strength2.
 *
 *  By using this function we can read the driven strength2 for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadOutputDriveStrength2(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, OUTPUT_DRIVE_STRENGTH2, status);
}

/**
 *  WriteOutputDriveStrength3
 *
 *  @brief sets a drive strength3.
 *
 *  By using this function we can a set driven strength3, 00b = 0.25x drive strength, 01b = 0.5x drive
 *  strength, 10b = 0.75x drive strength, or 11b = 1x for full drive strength capability for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WriteOutputDriveStrength3(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, OUTPUT_DRIVE_STRENGTH3, data);
}

/**
 *  ReadOutputDriveStrength3

 *  @brief Reads the drive strength3.
 *
 *  By using this function we can read the driven strength3 for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadOutputDriveStrength3(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, OUTPUT_DRIVE_STRENGTH3, status);
}

/**
 *  WriteInputLatch0
 *
 *  @brief Writes the input latch enable register for port0
 *
 *  This function is helpful to enable the latch for port0, A change of state of the input generates an interrupt and the input
 *  logic value is loaded into the corresponding bit of the input port1 register,bit 1 for enable latch and bit 0 for disable latch.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WriteInputLatch0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, INPUT_LATCH0, data);
}

/**
 *  ReadInputLatch0
 *
 *  @brief Reads the input latch enable register for port0
 *
 *  This function helpful to read input latch for port0, A read of the input port0 register clears the interrupt.
 *  bit 1 for enable latch and bit 0 for disable latch.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadInputLatch0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, INPUT_LATCH0, status);
}

/**
 *  WriteInputLatch1
 *
 *  @brief Writes the input latch enable register for port1
 *
 *  This function is helpful to enable the latch for port1, A change of state of the input generates an interrupt and the input
 *  logic value is loaded into the corresponding bit of the input port1 register, bit 1 for enable latch and bit 0 for disable latch.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WriteInputLatch1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, INPUT_LATCH1, data);
}

/**
 *  ReadInputLatch1
 *
 *  @brief Reads the input latch enable register for port1
 *
 *  This function helpful to read input latch for port1, A read of the input port1 register clears the interrupt.
 *  bit 1 for enable latch and bit 0 for disable latch.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadInputLatch1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, INPUT_LATCH1, status);
}

/**
 *  WritePullUpPullDownEnable0
 *
 *  @brief Write the pullup or pulldown register for the port0.
 *
 *  This function is helpful to write the pull-up/pull-down enable registers allow the user to enable or disable
 *  pull-up/pull-down resistors on the GPIO pins in port0.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WritePullUpPullDownEnable0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, PULL_UP_PULL_DOWN_ENABLE0, data);
}

/**
 *  ReadPullUpPullDownEnable0
 *
 *  @brief Reads the pullup or pulldown register for the port0.
 *
 *  This function helpful to read the port0 is pullup or pulldown.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadPullUpPullDownEnable0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, PULL_UP_PULL_DOWN_ENABLE0, status);
}

/**
 *  WritePullUpPullDownEnable1
 *
 *  @brief Write the pullup or pulldown register for the port1.
 *
 *  This function is helpful to write the pull-up/pull-down enable registers allow the user to enable or disable
 *  pull-up/pull-down resistors on the GPIO pins in port1.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WritePullUpPullDownEnable1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, PULL_UP_PULL_DOWN_ENABLE1, data);
}

/**
 *  ReadPullUpPullDownEnable1
 *
 *  @brief Reads the pullup or pulldown register for the port1.
 *
 *  This function helpful to read the port0 is pullup or pulldown.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadPullUpPullDownEnable1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, PULL_UP_PULL_DOWN_ENABLE1, status);
}

/**
 *  WritePullUpPullDownSelect0
 *
 *  @brief Select the pullup or pulldown selected register for the port0.
 *
 *  This function is helpful to the pull-up/pull-down selection registers allow the user to configure each GPIO to have a
 *  pull-up or pull-down resistor by programming the respective register bit.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WritePullUpPullDownSelect0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, PULL_UP_PULL_DOWN_SELECT0, data);
}

/**
 *  ReadPullUpPullDownSelect0
 *
 *  @brief Reads the pullup or pulldown the selected register for the port0.
 *
 *  This function is helpul to select the pullup or pulldown register for the port0.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadPullUpPullDownSelect0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, PULL_UP_PULL_DOWN_SELECT0, status);
}

/**
 *  WritePullUpPullDownSelect1
 *
 *  @brief Select the pullup or pulldown selected register for the port1.
 *
 *  This function is helpful to the pull-up/pull-down selection registers allow the user to configure each GPIO to have a
 *  pull-up or pull-down resistor by programming the respective register bit.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WritePullUpPullDownSelect1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, PULL_UP_PULL_DOWN_SELECT1, data);
}

/**
 *  ReadPullUpPullDownSelect1
 *
 *  @brief Reads the pullup or pulldown the selected register for the port1.
 *
 *  This function is helpul to select the pullup or pulldown register for the port1.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadPullUpPullDownSelect1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, PULL_UP_PULL_DOWN_SELECT1, status);
}

/**
 *  WriteInterruptMask0
 *
 *  @brief Write the Interrupt mask register for port0.
 *
 *  This function is helful to enabel or disable the interrupt generation for port0 in a specified pin.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WriteInterruptMask0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, INTERRUPT_MASK0, data);
}

/**
 *  ReadInterruptMask0
 *
 *  @brief Reads the interrupt mask register for port0.
 *
 *  This function is helpful to reads the interrupt mask register for port0.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadInterruptMask0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, INTERRUPT_MASK0, status);
}

/**
 *  WriteInterruptMask1
 *
 *  @brief Write the Interrupt mask register for port1.
 *
 *  This function is helful to enabel or disable the interrupt generation for port1 in a specified pin.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WriteInterruptMask1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data) {
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, INTERRUPT_MASK1, data);
}

/**
 *  ReadInterruptMask1
 *
 *  @brief Reads the interrupt mask register for port1.
 *
 *  This function is helpful to reads the interrupt mask register for port1.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadInterruptMask1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, INTERRUPT_MASK1, status);
}

/**
 *  ReadInterruptStatus0
 *
 *  @brief Reads the interrupt status register for port0.
 *
 *  This function helpful to read the Interrupt status registers are read only registers used to identify the source of an interrupt.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadInterruptStatus0(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, INTERRUPT_STATUS0, status);
}

/**
 *  ReadInterruptStatus1
 *
 *  @brief Reads the interrupt status register for port1.
 *
 *  This function helpful to read the Interrupt status registers are read only registers used to identify the source of an interrupt.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadInterruptStatus1(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, INTERRUPT_STATUS1, status);
}

/**
 *  WriteOutputPortConfig
 *
 *  @brief Configures the push-pull or open drain output port.
 *
 *  This function is helpful to the output port configuration register selects port-wise push-pull or open-drain I/O stage.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 *
 *  @return None.
 */
void WriteOutputPortConfig(uint32_t I2C_Base,uint8_t slaveAddress, uint8_t data)
{
    TCAL9539_WriteRegister(I2C_Base,slaveAddress, OUTPUT_PORT_CONFIG, data);
}

/**
 *  ReadOutputPortConfig
 *
 *  @brief Reads the output port pushpull or open drain.
 *
 *  This function is helpful to read the pin is push pull or opendrain.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *
 *  @return data.
 */
uint8_t ReadOutputPortConfig(uint32_t I2C_Base,uint8_t slaveAddress,uint8_t status[2]) {
    return TCAL9539_ReadRegister(I2C_Base,slaveAddress, OUTPUT_PORT_CONFIG, status);
}
