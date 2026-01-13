/*
 * TCA9539.h
 *
 *  Created on: Mar 4, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

#ifndef TCA9539_H_
#define TCA9539_H_

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

#ifndef MUXIO_H
    #include "MuxIO.h"
#endif

#define INPUT_PORT0                 0x00
#define INPUT_PORT1                 0x01
#define OUTPUT_PORT0                0x02
#define OUTPUT_PORT1                0x03
#define POLARITY_INVERSION0         0x04
#define POLARITY_INVERSION1         0x05
#define CONFIGURATION0              0x06
#define CONFIGURATION1              0x07
#define OUTPUT_DRIVE_STRENGTH0      0x40
#define OUTPUT_DRIVE_STRENGTH1      0x41
#define OUTPUT_DRIVE_STRENGTH2      0x42
#define OUTPUT_DRIVE_STRENGTH3      0x43
#define INPUT_LATCH0                0x44
#define INPUT_LATCH1                0x45
#define PULL_UP_PULL_DOWN_ENABLE0   0x46
#define PULL_UP_PULL_DOWN_ENABLE1   0x47
#define PULL_UP_PULL_DOWN_SELECT0   0x48
#define PULL_UP_PULL_DOWN_SELECT1   0x49
#define INTERRUPT_MASK0             0x4A
#define INTERRUPT_MASK1             0x4B
#define INTERRUPT_STATUS0           0x4C
#define INTERRUPT_STATUS1           0x4D
#define OUTPUT_PORT_CONFIG          0x4F

typedef enum
{
    I2CSuccess = 0,
    I2CBusyTimout,
    I2CStopTimout,
    I2CTxFifoTimout,
    I2CRxFifoTimout,
    I2CMCUError,
}I2CCommStatus;

typedef union
{
    uint8_t I2CStatus;
    struct{
        I2CCommStatus I2C1Status:4;
        I2CCommStatus I2C2Status:4;
    }Statusbits;
}I2CStatusStruct_t;
// Read Input Port 0 register
/**
 *  By using this function we can read the data from specific slave, from port0.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress is the slave address.
 */
uint8_t ReadInputPort0(uint32_t base,uint8_t slaveAddress, uint8_t *status);

// Read Input Port 1 register
/**
 *  By using this function we can read the data from specific slave, from port1.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress is the slave address.
 */
uint8_t ReadInputPort1(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Output Port 0 register
/**
 *  By using this function we write in a specific slave, in port0.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data data what we write.
 */
void WriteOutputPort0(uint32_t base,uint8_t slaveAddress, uint8_t data);
/**
 *  By using this function we can read the output port0 from a specific slave based on slave address.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @pram slaveAddress address of the slave.
 */
uint8_t ReadOutputPort0(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Output Port 1 register
/**
 *  By using this function we write in a specific slave, in port1.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data data what we write.
 */
void WriteOutputPort1(uint32_t base,uint8_t slaveAddress, uint8_t data);
/**
 *  By using this function we can read the output port0 from a specific slave based on slave address.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadOutputPort1(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Polarity Inversion 0 register
/**
 *  By using this function we can write a polarity inversion in a port0 to a specific slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we want to write in slave.
 */
void WritePolarityInversion0(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  By using this function we read polarity inversion data from port0 from a specific slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadPolarityInversion0(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Polarity Inversion 1 register
/**
 *  By using this function we can write a polarity inversion in a port1 to a specific slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we want to write in slave.
 */
void WritePolarityInversion1(uint32_t base,uint8_t slaveAddress, uint8_t data);
/**
 *  By using this function we read polarity inversion data from port1 from a specific slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadPolarityInversion1(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Configuration 0 register
/**
 *  By using this function we can configure the port0 as a input or output.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WriteConfiguration0(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  By using this function we read that specified pin is a output or input in a port0.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadConfiguration0(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Configuration 1 register
/**
 *  By using this function we can configure the port1 as a input or output.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WriteConfiguration1(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  By using this function we read that specified pin is a output or input in a port1.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadConfiguration1(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Output Drive Strength 0 register
/**
 *  By using this function we can a set drive strength0, 00b = 0.25x drive strength, 01b = 0.5x drive
 *  strength, 10b = 0.75x drive strength, or 11b = 1x for full drive strength capability for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WriteOutputDriveStrength0(uint32_t base,uint8_t slaveAddress, uint8_t data);
/**
 *  By using this function we can read the driven strength0 for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadOutputDriveStrength0(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Output Drive Strength 1 register
/**
 *  By using this function we can a set driven strength1, 00b = 0.25x drive strength, 01b = 0.5x drive
 *  strength, 10b = 0.75x drive strength, or 11b = 1x for full drive strength capability for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
void WriteOutputDriveStrength1(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  By using this function we can read the driven strength1 for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadOutputDriveStrength1(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Output Drive Strength 2 register
/**
 *  By using this function we can a set driven strength2, 00b = 0.25x drive strength, 01b = 0.5x drive
 *  strength, 10b = 0.75x drive strength, or 11b = 1x for full drive strength capability for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WriteOutputDriveStrength2(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  By using this function we can read the driven strength2 for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadOutputDriveStrength2(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Output Drive Strength 3 register
/**
 *  By using this function we can a set driven strength3, 00b = 0.25x drive strength, 01b = 0.5x drive
 *  strength, 10b = 0.75x drive strength, or 11b = 1x for full drive strength capability for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WriteOutputDriveStrength3(uint32_t base,uint8_t slaveAddress, uint8_t data);
/**
 *  By using this function we can read the driven strength3 for a specified slave.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadOutputDriveStrength3(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Input Latch 0 register
/**
 *  This function is helpful to enable the latch for port0, A change of state of the input generates an interrupt and the input
 *  logic value is loaded into the corresponding bit of the input port1 register,bit 1 for enable latch and bit 0 for disable latch.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WriteInputLatch0(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  This function helpful to read input latch for port0, A read of the input port0 register clears the interrupt.
 *  bit 1 for enable latch and bit 0 for disable latch.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadInputLatch0(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Input Latch 1 register
/**
 *  This function is helpful to enable the latch for port1, A change of state of the input generates an interrupt and the input
 *  logic value is loaded into the corresponding bit of the input port1 register, bit 1 for enable latch and bit 0 for disable latch.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WriteInputLatch1(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  This function helpful to read input latch for port1, A read of the input port1 register clears the interrupt.
 *  bit 1 for enable latch and bit 0 for disable latch.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadInputLatch1(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Pull-up/Pull-down Enable 0 register
/**
 *  This function is helpful to write the pull-up/pull-down enable registers allow the user to enable or disable
 *  pull-up/pull-down resistors on the GPIO pins in port0.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WritePullUpPullDownEnable0(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  This function helpful to read the port0 is pullup or pulldown.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadPullUpPullDownEnable0(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Pull-up/Pull-down Enable 1 register
/**
 *  This function is helpful to write the pull-up/pull-down enable registers allow the user to enable or disable
 *  pull-up/pull-down resistors on the GPIO pins in port1.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WritePullUpPullDownEnable1(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  This function helpful to read the port0 is pullup or pulldown.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadPullUpPullDownEnable1(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Pull-up/Pull-down Selection 0 register
/**
 *  This function is helpful to the pull-up/pull-down selection registers allow the user to configure each GPIO to have a
 *  pull-up or pull-down resistor by programming the respective register bit.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
void WritePullUpPullDownSelect0(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  This function is helpul to select the pullup or pulldown register for the port0.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadPullUpPullDownSelect0(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Pull-up/Pull-down Selection 1 register
/**
 *  This function is helpful to the pull-up/pull-down selection registers allow the user to configure each GPIO to have a
 *  pull-up or pull-down resistor by programming the respective register bit.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WritePullUpPullDownSelect1(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  This function is helpul to select the pullup or pulldown register for the port1.
 *
 *  @param I2C_Base is the base address of the
 */
uint8_t ReadPullUpPullDownSelect1(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Interrupt Mask 0 register
/**
 *  This function is helful to enabel or disable the interrupt generation for port0 in a specified pin.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WriteInterruptMask0(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  This function is helpful to reads the interrupt mask register for port0.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadInterruptMask0(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read or Write Interrupt Mask 1 register
/**
 *  This function is helful to enabel or disable the interrupt generation for port1 in a specified pin.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WriteInterruptMask1(uint32_t base,uint8_t slaveAddress, uint8_t data);

/**
 *  This function is helpful to reads the interrupt mask register for port1.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadInterruptMask1(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read Interrupt Status 0 register
/**
 *  This function helpful to read the Interrupt status registers are read only registers used to identify the source of an interrupt.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadInterruptStatus0(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Read Interrupt Status 1 register
/**
 *  This function helpful to read the Interrupt status registers are read only registers used to identify the source of an interrupt.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadInterruptStatus1(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

// Write Output Port Configuration register
/**
 *  This function is helpful to the output port configuration register selects port-wise push-pull or open-drain I/O stage.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 *  @param data we write.
 */
void WriteOutputPortConfig(uint32_t base,uint8_t slaveAddress, uint8_t data);

// Reads Output port configuration register.
/**
 *  This function is helpful to read the pin is push pull or opendrain.
 *
 *  @param I2C_Base is the base address of the I2C instance used.
 *  @param slaveAddress address of the slave.
 */
uint8_t ReadOutputPortConfig(uint32_t base,uint8_t slaveAddress, uint8_t status[2]);

/**This function is helpful to write in the I2C slave port and pins.
 *
 *  @param base is the base address of the I2C instance used.
 *  @param slaveAddress Address of a slave.
 *  @param reg where we give port number.
 *  @param data data we sent.
*/
I2CCommStatus TCAL9539_WriteRegister(uint32_t base, uint8_t slaveAddress, uint8_t reg, uint8_t data);
uint8_t TCAL9539_ReadRegister(uint32_t base, uint8_t slaveAddress, uint8_t reg, I2CCommStatus *I2Cstatus);

#endif /* MIDDLELAYER_TCA9539_H_ */
