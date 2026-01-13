/*
 * MuxIO.c
 *
 *  Created on: Mar 5, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */

#include "MuxIO.h"

/**
 *  ConfigMuxInputPins
 *
 *  @brief Configures IO I2C Expander modules as a Input.
 *
 *  This function is helpful to configure the IO I2C Expander-1 (I2C_INPUT_EXP1-0x74) and IO I2C Expander-2 (I2C_INPUT_EXP2-0x76)
 *  port0 as a 'input' and enable the 'pullup' and set the all pins as a 'pullup' mode.
 *
 *  @return None.
 */
void ConfigMuxInputPins()
{
    // This is Left Gun Digital Input == DC2
    WriteConfiguration0(I2C_BASE_IOEXPANDER, I2C_INPUT_EXP1, 0xFF);
    WritePullUpPullDownEnable0(I2C_BASE_IOEXPANDER,I2C_INPUT_EXP1,0xFF);
    WritePullUpPullDownEnable1(I2C_BASE_IOEXPANDER,I2C_INPUT_EXP1,0xFE);
    WritePullUpPullDownSelect0(I2C_BASE_IOEXPANDER,I2C_INPUT_EXP1,0xFF);
    WritePullUpPullDownSelect1(I2C_BASE_IOEXPANDER,I2C_INPUT_EXP1,0xFE);


    // This is Right Gun Digital Input == DC1
    WriteConfiguration0(I2C_BASE_IOEXPANDER, I2C_INPUT_EXP2, 0xFF);
    WritePullUpPullDownEnable0(I2C_BASE_IOEXPANDER,I2C_INPUT_EXP2,0xFF);
    WritePullUpPullDownEnable1(I2C_BASE_IOEXPANDER,I2C_INPUT_EXP2,0xFF);
    WritePullUpPullDownSelect0(I2C_BASE_IOEXPANDER,I2C_INPUT_EXP2,0xFF);
    WritePullUpPullDownSelect1(I2C_BASE_IOEXPANDER,I2C_INPUT_EXP2,0xFF);  // Here make it FE

}

/**
 *  ConfigMuxOutputPins
 *
 *  @brief Configures IO I2C Expander Module as a output.
 *
 *  This function is helpful to configures the IO I2C Expander-3 (I2C_OUTPUT_EXP1-0x75) as a 'output'. And 8-Pins are in use,
 *  4 pins in port0 and 4 pins in port1.
 *
 *  @return None.
 */
void ConfigMuxOutputPins()
{
    WriteConfiguration1(I2C_BASE_IOEXPANDER, I2C_OUTPUT_EXP1, 0x00);

}

/**
 *  GetMuxInput
 *
 *  @brief Reads the IO I2C Expander Modules.
 *
 *  This function is helpful to read I2C_INPUT_EXP1 and I2C_INPUT_EXP2 modules. and store the data in temp1, and temp2.
 *
 *  @param data1  -  To store the I2C_INPUT_EXP1 data.
 *  @param data2  -  To store the I2C_INPUT_EXP2 data.
 *
 *  @return None.
 */
void GetMuxInput(uint16_t *data1, uint16_t *data2, uint8_t *I2Cstatus)
{
    uint16_t temp1,temp2;
    temp1 = ReadInputPort0(I2C_BASE_IOEXPANDER, I2C_INPUT_EXP1, I2Cstatus);
    *data1 = temp1;
    DEVICE_DELAY_US(50);
    temp2 = ReadInputPort0(I2C_BASE_IOEXPANDER, I2C_INPUT_EXP2, I2Cstatus);
    *data2 = temp2;
}

/**
 *  SetMuxOutput
 *
 *  @brief SET port-0 4-pins and port-1 4-pins as a output in I2C_OUTPUT_EXP1.
 *
 *  This function is helpful to set the port-0 4 pins and port-1 4 pins as output.
 *
 *  @param data1  -  Data to set the pins.
 *
 *  @return None.
 */
void SetMuxOutput(uint16_t data1)
{
    uint16_t temp3;
    uint8_t tempLo,tempHi;
    temp3 =  data1;
    tempLo =  (uint8_t)(temp3 & 0x00FF);
    tempHi = (uint8_t)((temp3 & 0xFF00) >>8);

     WriteOutputPort0(I2C_BASE_IOEXPANDER,I2C_OUTPUT_EXP1, tempLo);
     WriteOutputPort0(I2C_BASE_IOEXPANDER,I2C_OUTPUT_EXP1, tempHi);
}

/**
 *  SetDC1MOTBVIN2
 *
 *  @brief Set the Motbvalve-1 off.
 *
 *  This function is helpful to set the Motobvalve-1 OFF based on value and read the current output state, pins are setting in
 *  port0 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @return None.
 */
void SetDC1MOTBVIN2(uint32_t I2C_Base, bool value)
{
    uint8_t status[2];
    uint8_t port0Data = ReadOutputPort0(I2C_Base, I2C_OUTPUT_EXP1, status); // Read current output state
    if (value)
        port0Data |= (1 << 0); // Set bit 0 (DC1MOTBVIN2)
    else
        port0Data &= ~(1 << 0); // Clear bit 0

    WriteOutputPort0(I2C_Base, I2C_OUTPUT_EXP1, port0Data); // Write updated state
}

/**
 *  SetDC1MOTBVIN1
 *
 *  @brief Set the Motbvalve-1 on.
 *
 *  This function is helpful to set the Motobvalve-1 ON based on value and read the current output state, pins are setting in
 *  port0 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @return None.
 */
void SetDC1MOTBVIN1(uint32_t I2C_Base, bool value)
{
    uint8_t status[2];
    uint8_t port0Data = ReadOutputPort0(I2C_Base, I2C_OUTPUT_EXP1, status);
    if (value)
        port0Data |= (1 << 1); // Set bit 1 (DC1MOTBVIN1)
    else
        port0Data &= ~(1 << 1); // Clear bit 1

    WriteOutputPort0(I2C_Base, I2C_OUTPUT_EXP1, port0Data);
}

/**
 *  SetDC1GUNSolenoid1
 *
 *  @brief Set the Solenoid-1 on.
 *
 *  This function is helpful to set the solenoid-1 ON based on value and read the current output state, pins are setting in
 *  port0 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @Return None.
 */
void SetDC1GUNSolenoid1(uint32_t I2C_Base, bool value)
{
    uint8_t status[2];
    uint8_t port0Data = ReadOutputPort0(I2C_Base, I2C_OUTPUT_EXP1, status);
    if (value)
        port0Data |= (1 << 2); // Set bit 2 (DC1GUNSolenoid1)
    else
        port0Data &= ~(1 << 2); // Clear bit 2

    WriteOutputPort0(I2C_Base, I2C_OUTPUT_EXP1, port0Data);
}

/**
 *  SetDC1GUNSolenoid2
 *
 *  @brief Set the Solenoid-2 off.
 *
 *  This function is helpful to set the solenoid-2 OFF based on value and read the current output state, pins are setting in
 *  port0 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @Return None.
 */
void SetDC1GUNSolenoid2(uint32_t I2C_Base, bool value)
{
    uint8_t status[2];
    uint8_t port0Data = ReadOutputPort0(I2C_Base, I2C_OUTPUT_EXP1, status);
    if (value)
        port0Data |= (1 << 3); // Set bit 3 (DC1GUNSolenoid2)
    else
        port0Data &= ~(1 << 3); // Clear bit 3

    WriteOutputPort0(I2C_Base, I2C_OUTPUT_EXP1, port0Data);
}

/**
 *  SetDC2MOTBVIN2
 *
 *  @brief Set the Motbvalve based on value.
 *
 *  This function is helpful to set the motobvalve-2 off based on value and read the current output state, pins are setting in
 *  port1 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @return None.
 */
void SetDC2MOTBVIN2(uint32_t I2C_Base, bool value)
{
    uint8_t status[2];
    uint8_t port1Data = ReadOutputPort1(I2C_Base, I2C_OUTPUT_EXP1, status);
    if (value)
        port1Data |= (1 << 0); // Set bit 8 (DC2MOTBVIN2, 0th bit of Port1)
    else
        port1Data &= ~(1 << 0); // Clear bit 8

    WriteOutputPort1(I2C_Base, I2C_OUTPUT_EXP1, port1Data);
}

/**
 *  SetDC2MOTBVIN1
 *
 *  @brief Set the Motbvalve based on value.
 *
 *  This function is helpful to set the motobvalve-2 on based on value and read the current output state, pins are setting in
 *  port1 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @return None.
 */
void SetDC2MOTBVIN1(uint32_t I2C_Base, bool value)
{
    uint8_t status[2];
    uint8_t port1Data = ReadOutputPort1(I2C_Base, I2C_OUTPUT_EXP1, status);
    if (value)
        port1Data |= (1 << 1); // Set bit 9 (DC2MOTBVIN1, 1st bit of Port1)
    else
        port1Data &= ~(1 << 1); // Clear bit 9

    WriteOutputPort1(I2C_Base, I2C_OUTPUT_EXP1, port1Data);
}

/**
 *  SetDC1GUNSolenoid1
 *
 *  @brief Set the Solenoid-1 on.
 *
 *  This function is helpful to set the solenoid-1 ON based on value and read the current output state, pins are setting in
 *  port1 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @return None.
 */
void SetDC2GUNSolenoid1(uint32_t I2C_Base, bool value)
{
    uint8_t status[2];
    uint8_t port1Data = ReadOutputPort1(I2C_Base, I2C_OUTPUT_EXP1, status);
    if (value)
        port1Data |= (1 << 2); // Set bit 10 (DC2GUNSolenoid1, 2nd bit of Port1)
    else
        port1Data &= ~(1 << 2); // Clear bit 10

    WriteOutputPort1(I2C_Base, I2C_OUTPUT_EXP1, port1Data);
}

/**
 *  SetDC1GUNSolenoid2
 *
 *  @brief Set the Solenoid-2 on.
 *
 *  This function is helpful to set the solenoid-2 OFF based on value and read the current output state, pins are setting in
 *  port1 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @return None.
 */
void SetDC2GUNSolenoid2(uint32_t I2C_Base, bool value)
{
    uint8_t status[2];
    uint8_t port1Data = ReadOutputPort1(I2C_Base, I2C_OUTPUT_EXP1, status);
    if (value)
        port1Data |= (1 << 3); // Set bit 11 (DC2GUNSolenoid2, 3rd bit of Port1)
    else
        port1Data &= ~(1 << 3); // Clear bit 11

    WriteOutputPort1(I2C_Base, I2C_OUTPUT_EXP1, port1Data);
}
