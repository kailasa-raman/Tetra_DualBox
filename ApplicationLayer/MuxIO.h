/*
 * MuxIO.h
 *
 *  Created on: Mar 5, 2025
 *   Owner  : Tetra Automotives Pvt LTD.
 */

#ifndef MUXIO_H_
#define MUXIO_H_

#ifndef MCUSPECIFIC_H
#include "MCUSpecific.h"
#endif

#ifndef DRIVERLIB_H
    #include "driverlib.h"
#endif

#ifndef DEVICE_H
    #include "device.h"
#endif

#ifndef TCA9539_H
#include "TCA9539.h"
#endif

#define I2C_INPUT_EXP1              0x74            //  15 Read Signals
#define I2C_INPUT_EXP2              0x76            // 15 Read Signals
#define I2C_OUTPUT_EXP1             0x75            // 8  Write Signals

/**
 *  NOT IN USE, FUTURE PURPOSE.
 */
typedef union
{
    uint16_t RawData;
    struct
    {
        unsigned char GND:1;
       // unsigned char DC2StopCharge:1;
        unsigned char IMD1FB:1;
        unsigned char WaterLevelSensor:1;                //DC2GunPresence:1;
        unsigned char BatteryFaliure:1;// unsigned char DC1StopCharge:1;         //DC2NegativeContactorFB:1;
        unsigned char IMD2FB:1;
        unsigned char DCOk:1;
        unsigned char SmokeSensor:1;     //DC2PositiveContactorFB:1;

        unsigned char SPDFB :1;
        unsigned char DC2VoltageDetection:1;
        unsigned char DC1VoltageDetection:1;
        unsigned char ACCoilFB:1;
        unsigned char ELRFB:1;
        unsigned char ACContactorFB:1;
        unsigned char Spare1:1;
        unsigned char Spare2:1;
    }bits;
}InputExpander1;

/**
 *  NOT IN USE, FUTURE PURPOSE.
 */
typedef union
{
    uint16_t RawData;
    struct
    {
        unsigned char DC1StopCharge:1;//unsigned char FrontDoor:1;
        unsigned char EmergencySW:1;
        unsigned char CrossDCContactorFBHi:1;
        unsigned char CrossDCContactorFBLo:1;
        unsigned char DC2PositiveContactorFB:1;   //SmokeSensor:1;  //---
        unsigned char DC2StopCharge:1;// unsigned char RareDoor:1;
        //unsigned char DC1GunSolenoidFB:1;
        //unsigned char DC1MotorBallValveFB:1;

        //unsigned char DC2GunSolenoidFB:1;
        //unsigned char DC2MotorBallValveFB:1;
        unsigned char NC:5;
        unsigned char DC1GunPresence:1;
        unsigned char DC1PositiveContactorFB:1;
        unsigned char DC1NegativeContactorFB:1;
        unsigned char DC2NegativeContactorFB:1;      //DC1StopCharge:1; //---
        //unsigned char BatteryFaliure:1;
    }bits;
}InputExpander2;

/**
 *  Values that can be passed to set the MOTBVIN and GUNSolenoid functions to set pins.
 */
typedef union
{
    uint16_t rawdata;
    struct
    {
        bool DC1MOTBVIN2;
        bool DC1MOTBVIN1;
        bool DC1GUNSolenoid1;
        bool DC1GUNSolenoid2;
        bool rsv1;
        bool rsv2;
        bool rsv3;
        bool rsv4;

        bool DC2MOTBVIN2;
        bool DC2MOTBVIN1;
        bool DC2GUNSolenoid1;
        bool DC2GUNSolenoid2;
        bool rsv5;
        bool rsv6;
        bool rsv7;
        bool rsv8;
    }bits;
}OutputExpander2;

/**
 *  Values can be passed to ConfigMuxInputPins() and ConfigMuxOutputPins() to set pins as output or input.
 */
typedef union
{
    uint64_t MuxData;
    struct
    {
    InputExpander1 Input1;
    InputExpander2 Input2;
    uint16_t revdl[4];
    }IOIC;
}AllMuXIO;


/**
 *  ConfigMuxInputPins
 *
 *  This function is helpful to configure the IO I2C Expander-1 (I2C_INPUT_EXP1-0x74) and IO I2C Expander-2 (I2C_INPUT_EXP2-0x76)
 *  port0 as a 'input' and enable the 'pullup' and set the all pins as a 'pullup' mode.
 *
 *  @return None.
 */
void ConfigMuxInputPins();

/**
 *  ConfigMuxOutputPins
 *
 *  This function is helpful to configures the IO I2C Expander-3 (I2C_OUTPUT_EXP1-0x75) as a 'output'. And 8-Pins are in use,
 *  4 pins in port0 and 4 pins in port1.
 *
 *  @return None.
 */
void ConfigMuxOutputPins();

/**
 *  GetMuxInput
 *
 *  This function is helpful to read I2C_INPUT_EXP1 and I2C_INPUT_EXP2 modules. and store the data in temp1, and temp2.
 *
 *  @param data1  -  To store the I2C_INPUT_EXP1 data.
 *  @param data2  -  To store the I2C_INPUT_EXP2 data.
 *
 *  @return None.
 */
void GetMuxInput(uint16_t *data1, uint16_t *data2, uint8_t *I2Cstatus);

/**
 *  SetMuxOutput
 *
 *  This function is helpful to set the port-0 4 pins and port-1 4 pins as output.
 *
 *  @param data1  -  Data to set the pins.
 *
 *  @return None.
 */
void SetMuxOutput(uint16_t data1);

/**
 *  This function is helpful to set the Motobvalve-1 OFF based on value and read the current output state, pins are setting in
 *  port0 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @return None.
 */
void SetDC1MOTBVIN2(uint32_t I2CBase, bool value);

/**
 *  This function is helpful to set the Motobvalve-1 ON based on value and read the current output state, pins are setting in
 *  port0 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @return None.
 */
void SetDC1MOTBVIN1(uint32_t I2CBase, bool value);

/**
 *  This function is helpful to set the solenoid-1 ON based on value and read the current output state, pins are setting in
 *  port0 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @Return None.
 */
void SetDC1GUNSolenoid1(uint32_t I2CBase, bool value);

/**
 *  This function is helpful to set the solenoid-1 OFF based on value and read the current output state, pins are setting in
 *  port0 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @Return None.
 */
void SetDC1GUNSolenoid2(uint32_t I2CBase, bool value);

/**
 *  This function is helpful to set the motobvalve-2 off based on value and read the current output state, pins are setting in
 *  port1 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @return None.
 */
void SetDC2MOTBVIN2(uint32_t I2CBase, bool value);

/**
 *  This function is helpful to set the motobvalve-2 on based on value and read the current output state, pins are setting in
 *  port1 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @return None.
 */
void SetDC2MOTBVIN1(uint32_t I2CBase, bool value);

/**
 *  This function is helpful to set the solenoid-2 ON based on value and read the current output state, pins are setting in
 *  port1 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @return None.
 */
void SetDC2GUNSolenoid1(uint32_t I2CBase, bool value);

/**
 *  This function is helpful to set the solenoid-2 OFF based on value and read the current output state, pins are setting in
 *  port1 I2C_OUTPUT_EXP1.
 *
 *  @param I2C_Base  -   Master Address
 *  @param value     -   value to set the pins
 *
 *  @return None.
 */
void SetDC2GUNSolenoid2(uint32_t I2CBase, bool value);

#endif /* APPLICATIONLAYER_MUXIO_H_ */
