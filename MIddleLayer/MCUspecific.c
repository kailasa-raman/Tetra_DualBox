#include "MCUspecific.h"
#include "RNL.h"
#include "TonheCAN.h"


typedef struct {
    float temperature; // Temperature in °C
    float resistance;  // Resistance in Ohms
} PTC_Lookup;

PTC_Lookup ptcTable[50] = {
    {0, 1000.0}, {5, 1019.5}, {10, 1039.0}, {15, 1058.4}, {20, 1077.9}, {25, 1097.3}, {30, 1116.7}, {35, 1136.1}, {40, 1155.4}, {45, 1174.7},
    {50, 1194.0}, {53, 1205.7}, {56, 1217.4}, {59, 1229.1}, {62, 1240.9}, {65, 1252.6}, {68, 1264.3}, {71, 1276.0}, {74, 1287.7}, {77, 1299.4},
    {80, 1311.1}, {83, 1322.8}, {86, 1334.5}, {89, 1346.2}, {92, 1357.9}, {95, 1369.6}, {98, 1381.3}, {101, 1393.0}, {104, 1404.7}, {107, 1416.4},
    {110, 1428.1}, {113, 1439.8}, {116, 1451.5}, {119, 1463.2}, {122, 1474.9}, {125, 1486.6}, {128, 1498.3}, {132, 1511.6}, {136, 1524.9}, {140, 1538.2},
    {144, 1551.5}, {148, 1564.8}, {152, 1578.1}, {156, 1591.4}, {160, 1604.7}, {165, 1621.1}, {170, 1637.5}, {175, 1653.8}, {180, 1670.0}
};

#define TABLE_SIZE (sizeof(ptc_resistance) / sizeof(ptc_resistance[0]))
//----------------------------------------------- Modbus
/**
 *  MCU_UART_TxRxData
 *
 *  @brief Sends and receives data over UART using the specified SCI handle.
 *
 *  This function transmits data over the given UART port and waits for the response. It stores the received bytes into RxBuffer.
 *  If the recieve FIFO didnt get any status within a time it returns ModBusTimout.
 *
 *  @param SCIHandle UART peripheral handle (e.g., SCI_A, SCI_B).
 *  @param Data Pointer to the data to be transmitted.
 *  @param Txlength Number of bytes to transmit.
 *  @param RxBuffer Pointer to the buffer to store received data.
 *  @param Rxlength Number of bytes expected to receive.
 *  @param timeout Timeout in milliseconds for receiving data.
 *
 *  @return MODBUS_STATUS_OK on success, or an appropriate ModbusCommstatus error code if a timeout or error occurs.
 */

ModbusCommstatus MCU_UART_TxRxData(uint32_t SCIHandle,uint8_t *Data,uint8_t Txlength,uint8_t *RxBuffer,uint8_t Rxlength,uint8_t timout)
{
    // whether tot send via CAN or return as status and modify this funciton to get the status info alone?
    uint8_t status_SCI = SCI_CheckError(SCIHandle);

    uint16_t i,count,RxAttempt=0,TxAttempt = 0;
    uint32_t startTime= 0;
    ModbusCommstatus status;
    SCI_resetTxFIFO(SCIHandle);
    DEVICE_DELAY_US(20);
    WriteDGPIO(MODBUS_GPIODERE,1);

    for ( i = 0; i < Txlength; i++)
    {
        while(SCI_getTxFIFOStatus(SCIHandle) > SCI_FIFO_TX16 && TxAttempt < 200) // code stuck 1
        {
            TxAttempt++;
            DEVICE_DELAY_US(500);
        }
        if(TxAttempt > 199)
        {
            TxAttempt = 0;
            return ModBusTimout;
        }
        TxAttempt = 0;

        while(SCI_getTxFIFOStatus(SCIHandle) == SCI_FIFO_TX16  && TxAttempt < 200)
        {
            TxAttempt++;
            DEVICE_DELAY_US(500);
        }
        if(TxAttempt > 199)
        {
            TxAttempt = 0;
            return ModBusTimout;
        }
        TxAttempt = 0;
        HWREGH(SCIHandle + SCI_O_TXBUF) = Data[i];
    }
    // Wait until TX FIFO is completely empty
    while(SCI_getTxFIFOStatus(SCIHandle) != SCI_FIFO_TX0 && TxAttempt < 200)    // code stuck 3 while loop inside
    {
        TxAttempt++;
        DEVICE_DELAY_US(500);
    }
    if(TxAttempt > 199)
    {
        TxAttempt = 0;
        return ModBusTimout;
    }
    TxAttempt = 0;

    while ((HWREG(SCIHandle + SCI_O_CTL2) & SCI_CTL2_TXEMPTY) == 0 && TxAttempt < 200)   // code stuck 4 while loop it is
    {
        TxAttempt++;
        DEVICE_DELAY_US(500);
    }
    if(TxAttempt > 199)
    {
        TxAttempt = 0;
        return ModBusTimout;
    }
    TxAttempt = 0;

    SCI_resetRxFIFO(SCIHandle);
    DEVICE_DELAY_US(20);
    WriteDGPIO(MODBUS_GPIODERE,0);
    // Get the current CPUTimer0  count
    startTime = CPUTimer_getTimerCount(CPUTIMER0_BASE);
    // wait for

    while (SCI_getRxFIFOStatus(SCIHandle) == 0)
    {
        uint32_t currentTime = CPUTimer_getTimerCount(CPUTIMER0_BASE);
        uint32_t elapsedTime = (startTime >= currentTime) ? (startTime - currentTime) : (startTime + (0xFFFFFFFF - currentTime));
        if (elapsedTime >= Timeout_MS(timout))  // Timeout exceeded
        {
            status =ModBusTimout;
            return status ;
        }
    }
    count = 0;
    while (count < Rxlength)
    {
        while(SCI_getRxFIFOStatus(SCIHandle) == SCI_FIFO_RX0 && RxAttempt < 200)
        {
            RxAttempt++;
            DEVICE_DELAY_US(500);
        }
        if (RxAttempt > 199)   // Problemetic as it was 500
        {
            RxAttempt = 0;
            return ModBusTimout;
        }

        RxBuffer[count]= ((uint16_t)(HWREGH(SCIHandle + SCI_O_RXBUF) & SCI_RXBUF_SAR_M));
        count++;
    }
    status =ModBusSuccess;
    return status;

}
/**
 *  SCI_CheckError
 *
 *  @brief checks SCI status register and sends the CAN message.
 *
 *  This function checks the error in SCI status register and sends the CAN message via the 'SendSCIErrorCan' function.
 *  @param SCIHandle is the base address handle of the SCI
 *
 *  @return None.
 */
uint8_t SCI_CheckError(uint32_t SCIHandle)
 {
   // uint32_t CANBaseAddr = RECTIFIER_GUN1;
    uint16_t status = SCI_getRxStatus(SCIHandle);
   // uint8_t canData[8] = {0,0,0,0,0,0,0,0};
   // uint8_t index = 0;

    if(status & SCI_RXSTATUS_FRAMING)
    {
        return SCIframing;//frameing error
    }

    if(status & SCI_RXSTATUS_OVERRUN)
    {
        return SCIoverrun;//overrun error
    }

    if(status & SCI_RXSTATUS_PARITY)
    {
        return SCIparity;//pariety error
    }

    if(status & SCI_RXSTATUS_BREAK)
    {
        return SCIbreak;//sci break error
    }
    return 0;
    //SendSCIErrorCan(canData,CANBaseAddr);
}


/**
 *  setModBusGPIO
 *
 *  @brief Sets the GPIO pins for MODBUS communication.
 *
 *  This function sets up the transmit and receive pins, as well as the direction control pin.
 *
 *  @return None.
 */
 void setModBusGPIO()
{
    // Configuration of GPIO for A & B line
    GPIO_setPinConfig(MODBUS_GPIO_MUXRx);
    GPIO_setControllerCore(MODBUS_GPIO_Rx, GPIO_CORE_CPU1);
    GPIO_setPadConfig(MODBUS_GPIO_Rx, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(MODBUS_GPIO_Rx, GPIO_DIR_MODE_IN);
    GPIO_setQualificationMode(MODBUS_GPIO_Rx, GPIO_QUAL_ASYNC);

    GPIO_setPinConfig(MODBUS_GPIO_MUXTx);
    GPIO_setPadConfig(MODBUS_GPIO_Tx, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(MODBUS_GPIO_Tx, GPIO_DIR_MODE_OUT);
    GPIO_setQualificationMode(MODBUS_GPIO_Tx, GPIO_QUAL_ASYNC);

    // Configuration of GPIO for DE/RE line
    GPIO_setPadConfig(MODBUS_GPIODERE, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(MODBUS_GPIO_MUXDERE);
    GPIO_setDirectionMode(MODBUS_GPIODERE, GPIO_DIR_MODE_OUT);
    GPIO_writePin(MODBUS_GPIODERE, 0);

}

/**
 *  setModBusUART
 *
 *  @brief Configuration of UART parameters for MODBUS communication.
 *
 *  This function configures the UART settings such as base address, parity, stop bits, and data bits using `setModBusUART()`.
 *
 *  @return None.
 */
void setModBusUART()
{
    SCI_disableModule(MODBUS_SCI_BASE);
    // Configure SCI-B for  baud rate
    SCI_setConfig(MODBUS_SCI_BASE, DEVICE_LSPCLK_FREQ, MODBUS_BAUDRATE,
                      (SCI_CONFIG_WLEN_8 | MODBUS_STOPBITS | MODBUS_PARITY));
    // Enable TX & RX FIFOs and reset them
    SCI_enableFIFO(MODBUS_SCI_BASE);
    SCI_resetChannels(MODBUS_SCI_BASE);
    // Enable SCI-B module
    SCI_enableModule(MODBUS_SCI_BASE);
}

//-------------------------------Generic
/**
 *  ReadGPIO
 *
 *  @brief Reads the specific GPIO pin.
 *
 *  This function helpful to reads the specified GPIO, and it returns the value.
 *
 *  @param Returns the value in the data register for the specified pin.
 *
 *  @return None.
 */
uint8_t ReadGPIO(uint32_t GPIO)
{
    return (uint8_t)GPIO_readPin(GPIO);
}

/**
 *  SwitchPinReadDebounce
 *
 *  @brief Reads the debounce switch.
 *
 *  This function is helpful to reads the input from debounce switch.
 *
 *  @param GPIO It is a GPIO pin.
 *  @param InitialState Current position of switch.
 *  @param swstate Current state of pin 0 or 1.
 *
 *  @return 0(low) or 1(high).
 */
uint8_t SwitchPinReadDebounce(uint32_t GPIO,uint8_t InitialState,uint8_t *swstate)
{
    uint8_t PinValue;
    PinValue =  (uint8_t)ReadGPIO(GPIO);
    return DebounceLogic(PinValue,InitialState,swstate);

}

/**
 *  DebounceLogic
 *
 *  @brief It gives the switch output based on cuurent position.
 *
 *  This function is helpful to gives the stop switch value for every 250 milli seconds based on current switch state, we use this logic
 *  becaue we reads the switch for every 250 Milli seconds.
 *
 *  @param Signal It is a GPIO pin.
 *  @param InitialState Current position of switch.
 *  @param swstate Current state of pin 0 or 1.
 *
 *  @return 0(low) or 1(high).
 */
uint8_t DebounceLogic(uint8_t Signal,uint8_t InitialState,uint8_t *swstate)
{
    if(Signal != InitialState && *swstate == 0)
        {
        *swstate = 1;
        }
        else if(Signal != InitialState && *swstate == 1)
        {
            *swstate = 2;
        }
        else if (Signal == InitialState && *swstate == 2)
        {
            *swstate = 3;
        }
        else if (Signal == InitialState && *swstate == 3)
        {
            *swstate = 0;
            return 1;
        }
    return 0;

}

/**
 *  WriteDGPIO
 *
 *  @brief Writes a value to GPIO pin.
 *
 *  This function is helpful to write the value to specified GPIO pin.
 *
 *  @param GPIO Pin number.
 *  @param value data we want to send.
 *
 *  @return 1.
 */
uint8_t WriteDGPIO(uint32_t GPIO,uint32_t  Value)
{
    GPIO_writePin(GPIO, Value);
    return 1;
}

/**
 *  Timeout_MS
 *
 *  @brief Sets the time in milli second.
 *
 *  This function is helpful to return the time in milli second based on input value.
 *
 *  @param x is a variable is used to set the time.
 *
 *  @return Time in MS.
 */
uint32_t Timeout_MS(uint32_t x)
{
    uint32_t y= 0;
    y=(uint32_t)((uint32_t)DEVICE_SYSCLK_FREQ/1000) * x;  // 20ms in CPU cycles
    return y;
}
/*
static uint64_t SwapEndian64(uint64_t value)
{
    return ((value >> 56) & 0x00000000000000FF) |
           ((value >> 40) & 0x000000000000FF00) |
           ((value >> 24) & 0x0000000000FF0000) |
           ((value >>  8) & 0x00000000FF000000) |
           ((value <<  8) & 0x000000FF00000000) |
           ((value << 24) & 0x0000FF0000000000) |
           ((value << 40) & 0x00FF000000000000) |
           ((value << 56) & 0xFF00000000000000);
}
*/

/**
 *  swapBytes
 *
 *  @brief It swap the bits.
 *
 *  This function is helpful to swap the 8-bits and return value.
 *
 *  @param reg is a variable.
 *
 *  @return Swaped value.
 */
uint16_t swapBytes(uint16_t reg) {
    return ((reg << 8) | (reg >> 8));
}

/**
 *  GetPT1000TempValue
 *
 *  @brief Convert the sensor volatge to temperature.
 *
 *  This function is helpful to convert sensor voltage to temperature by using tempgain and pullup resistance and voltage.
 *
 *  @param SensorVolatge is a variable stores voltage value.
 *
 *  @return Temperature
 */
float GetPT1000TempValue(float32_t SensorVoltage)
{
    float Resistance=0;
    float Temperature = 0;
    float CMSAnalogVoltage   = SensorVoltage *TempGain;
    Resistance = ((TempPullUpResPTC * CMSAnalogVoltage)/(TempPullUpVolt - CMSAnalogVoltage)) -1000.0f;
    int i;
    for (i = 0; i < (sizeof(ptcTable)/sizeof(ptcTable[0])) - 1; i++)
    {
        if (Resistance >= ptcTable[i].resistance && Resistance <= ptcTable[i + 1].resistance)
        {
                // Linear interpolation
                float t1 = ptcTable[i].temperature;
                float t2 = ptcTable[i + 1].temperature;
                float r1 = ptcTable[i].resistance;
                float r2 = ptcTable[i + 1].resistance;
                Temperature =  t1 + ((Resistance - r1) * (t2 - t1) / (r2 - r1));
                return Temperature;
        }
    }
    return 255;
}

/**
 *  GetPT1000TempValue
 *
 *  @brief Convert the sensor volatge to temperature.
 *
 *  This function is helpful to convert sensor voltage to temperature by using tempgain and NTCA and NTCB.
 *
 *  @param SensorVolatge is a variable stores voltage value.
 *
 *  @return Temperature
 */
float GetNT1000TempValue(float32_t SensorVoltage)
{
  float Resistance=0;
  float Temperature = 0;
    float CMSAnalogVoltage   = SensorVoltage *TempGain;
    if (CMSAnalogVoltage > 4950)
        return 255;
    Resistance = ((TempPullUpRes * CMSAnalogVoltage)/(TempPullUpVolt - CMSAnalogVoltage))-1000.0f;
    float ratio = Resistance / R0;  // R/R0
    float approx_ln = (ratio - 1) - ((ratio - 1) * (ratio - 1)) / 2 + ((ratio - 1) * (ratio - 1) * (ratio - 1)) / 3;
    //temperature = (A * r_ntc * r_ntc) + (B * r_ntc) + C
    Temperature =  BETA / (approx_ln + (BETA / T0));
    Temperature = Temperature - 273.15;
    return Temperature;
}
// --------------------------CAN
/**
 *  InitialiseCAN
 *
 *  @brief Configure the GPIO and initializes the CAN.
 *
 *  This function is useful to configure the alternative function to GPIO and Initializes the CAN and set the clock frequency and
 *  speed.
 *
 *  @return None.
 */
void InitialiseCAN()
{
    GPIO_setPinConfig(CANA_GPIO_MUXTX);
    GPIO_setPinConfig(CANA_GPIO_MUXRX);

    GPIO_setPinConfig(CANB_GPIO_MUXTX);
    GPIO_setPinConfig(CANB_GPIO_MUXRX);

    CAN_initModule(RECTIFIER_GUN1);
    CAN_initModule(RECTIFIER_GUN2);

    CAN_setBitRate(RECTIFIER_GUN1, DEVICE_SYSCLK_FREQ, CANSPEED, 20);
    CAN_setBitRate(RECTIFIER_GUN2, DEVICE_SYSCLK_FREQ, CANSPEED, 20);

    CAN_enableAutoBusOn(RECTIFIER_GUN1);
    CAN_enableAutoBusOn(RECTIFIER_GUN2);
    CAN_setAutoBusOnTime(RECTIFIER_GUN1, 50000);
    CAN_setAutoBusOnTime(RECTIFIER_GUN2, 50000);

    CANInterruptEnable();

}
/**
 *  CANMSGObjectSetup
 *
 *  @brief Setup the CAN object
 *
 *  This function is useful to setup the message objects based on objectid.
 *
 *  @param CANBASE is the base address of the CAN controller.
 *  @param objectID is the message object number to configure (1-32).
 *  @param BaseID is the CAN message identifier used for the 11 or 29 bit
 *         identifiers
 *  @param FrameType is the CAN ID frame type
 *  @param ObjectType is the message object type
 *  @param msgIDMask is the CAN message identifier mask used when identifier
 *         filtering is enabled
 *  @param IntName is the various flags and settings to be set for the message
 *         object
 */

void CANMSGObjectSetup(uint32_t CANBASE,uint8_t ObjectID,
                       uint32_t BaseID,uint8_t FrameType,
                       uint8_t ObjectType,uint32_t MaskID,
                       uint32_t IntName)
{
    CAN_MsgFrameType CAN_MsgFrame =(CAN_MsgFrameType)FrameType;
    CAN_MsgObjType CAN_MsgObjReRxTX = (CAN_MsgObjType)ObjectType;
    //CAN_setupMessageObject(CANBASE,ObjectID,BaseID,CAN_MsgFrame,CAN_MsgObjReRxTX, MaskID,IntName,8);
        CAN_setupMessageObject(CANBASE,ObjectID,BaseID,CAN_MsgFrame,CAN_MsgObjReRxTX, MaskID,IntName,8);


}

/**
 *  CANSendMessage
 *
 *  @brief Send the message by using address and mail box number.
 *
 *  This function is useful to send the message with the help CANBASE address and Mail box number.
 *
 *  @param CANBASE is the base address of the CAN controller.
 *  @param MailBoxNo is the object number to configure (1-32).
 *  @param length is the number of bytes of data in the message object (0-8).
 *  @param Data is a pointer to the message object's data.
 *
 *  @return None.
 */
void CANSendMessage(uint32_t CANBASE,uint32_t MailBoxNo,
                     uint64_t *Data,uint8_t length)
{
    uint8_t temp[8],i;
    //memcpy(temp,Data,8);

     for (i = 0; i< 8;  i++)
     {
         temp[i] = (*Data >> (8*i))& 0xFF;
      }
    CAN_sendMessage(CANBASE,MailBoxNo,(uint16_t)length, (uint16_t *)temp);
}

/**
 *  CANInterruptEnable
 *
 *  @brief Enable the Interrupts.
 *
 *  This function is helpful to enable the interrupts for rectifier gun1 and gun2.
 *
 *  @return None.
 */
void CANInterruptEnable()
{
    CAN_enableInterrupt(RECTIFIER_GUN1, CAN_INT_IE0|CAN_INT_ERROR|CAN_INT_STATUS);
    CAN_enableInterrupt(RECTIFIER_GUN2, CAN_INT_IE0|CAN_INT_ERROR|CAN_INT_STATUS);
    CAN_enableGlobalInterrupt(CANA_BASE, CAN_GLOBAL_INT_CANINT0);
    CAN_enableGlobalInterrupt(CANB_BASE, CAN_GLOBAL_INT_CANINT0);
}

/**
 *  InitialiseChargerInPins
 *
 *  @brief Set the analog pin as digital pin.
 *
 *  This function is helpful to set a analog pin as a digital pin.
 *
 *  @return None.
 */
void InitialiseChargerInPins()
{
    GPIO_setPinConfig(DC1_STOP_CHARGER_PINMUX);  // Set as GPIO
    GPIOSetConfig(DC1_STOPCHARGER_SW,GPIO_INPUT,PULLUP,ANALOG_DISABLED);

    GPIO_setPinConfig(DC2_STOP_CHARGER_PINMUX);  // Set as GPIO
    GPIOSetConfig(DC2_STOPCHARGER_SW,GPIO_INPUT,PULLUP,ANALOG_DISABLED);


}
/**
 *  InitialiseCANObject
 *
 *  @brief Sets the can objects that has to be set initially .
 *
 *  This function is helpful to set can objects for CAN transmission.
 *
 *  @return None.
 */

/**
 *  InitialiseChargerOutPins
 *
 *  @brief Set the analog pins as a digital inputs.
 *
 *  This function is helpful to configure the analog pins as a digital output by using GPIOSetConfig() function.
 *
 *  @return None.
 */
void InitialiseChargerOutPins()
{
    GPIOSetConfig(DC2_STS_LED_MCU,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(DC1_STP_LED_MCU,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(PLC_ENABLEPIN,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(DISPLAY2_POWERPIN,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(DC2_STP_LED_MCU,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(DISPLAY1_POWERPIN,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(DC2_EM_POWERPIN,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(AC_CONTECTOR,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(RAD_EN_MCU,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(EXHAUST_FAN,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(PUMP_POWER_ENABLE,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(SPARE2,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(SPARE1,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    //GPIOSetConfig(DC1_EM_POWERPIN,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    //GPIOSetConfig(LED_RED,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(LED_BLUE,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(DC1_STS_LED_MCU,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(GUN1_DCPOSITIVE_CONTACTOR,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(GUN1_DCNEGATIVE_CONTACTOR,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(GUN1_DCPRECHARGE_CONTACTOR,GPIO_OUTPUT,PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(GUN2_DCPOSITIVE_CONTACTOR, GPIO_OUTPUT, PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(GUN2_DCNEGATIVE_CONTACTOR, GPIO_OUTPUT, PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(GUN2_DCPRECHARGE_CONTACTOR, GPIO_OUTPUT, PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(CROSS_CONTACTOR, GPIO_OUTPUT, PUSHPULL,ANALOG_DISABLED);
    //GPIOSetConfig(STATION_LED, GPIO_OUTPUT, PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(ADD_LED1, GPIO_OUTPUT, PUSHPULL,ANALOG_DISABLED);
    GPIOSetConfig(ADD_LED2, GPIO_OUTPUT, PUSHPULL,ANALOG_DISABLED);
}

/**
 *  GPIOSetConfig
 *
 *  @brief It disable the analog mode, set the direction and configure it pullup.
 *
 *  This function helpful to disable the analog mode of a GPIO and set as output and configure as a pullup.
 *
 *  @param GPIOpin it tell which GPIO pin.
 *  @param Mod tell the output.
 *  @param AnalogMode it tells the Analog mode disabled or enabled.
 *  @param type it tells pullup or pulldown.
 *
 *  @return None.
 */

void GPIOSetConfig(uint32_t GPIOpin,GPIO_Direction Mod,uint32_t type,GPIO_AnalogMode AnalogMode)
{
    GPIO_setAnalogMode(GPIOpin, AnalogMode);
    GPIO_setDirectionMode(GPIOpin, Mod);    // GPIO6 = output
    GPIO_setPadConfig(GPIOpin, type);     // Enable pullup on GPIO6
}

/**
 *  I2CGPIOExpanstionInit
 *
 *  @brief It configure the GPIO pin for I2c.
 *
 *  This function is helpful to configure the GPIO pin as a I2C pin and set the frequency, speed, duty cycle and sets the data byte
 *  bit count and sets the target address, and enable transmit and receive and enable the module.
 *
 *  @return None.
 */
void I2CGPIOExpanstionInit()
{
// Configure SCL pin
GPIO_setPinConfig(I2C_GPIO_MUXSCL);
GPIO_setPadConfig(I2C_SCL_GPIO, GPIO_PIN_TYPE_PULLUP);
GPIO_setDirectionMode(I2C_SCL_GPIO, GPIO_DIR_MODE_OUT);
GPIO_setQualificationMode(I2C_SCL_GPIO, GPIO_QUAL_ASYNC);

// Configure SDA pin
GPIO_setPinConfig(I2C_GPIO_MUXSDA);
GPIO_setPadConfig(I2C_SDA_GPIO, GPIO_PIN_TYPE_PULLUP);
GPIO_setDirectionMode(I2C_SDA_GPIO, GPIO_DIR_MODE_OUT);
GPIO_setQualificationMode(I2C_SDA_GPIO, GPIO_QUAL_ASYNC);

// Disable the I2C module before configuration
I2C_disableModule(I2C_BASE_IOEXPANDER);

// Initialize I2C controller with desired parameters
I2C_initController(I2C_BASE_IOEXPANDER, DEVICE_SYSCLK_FREQ, I2C_SPEED, I2C_DUTYCYCLE_50);
I2C_setBitCount(I2C_BASE_IOEXPANDER, I2C_BITCOUNT_8);

// Set the target slave address
I2C_setTargetAddress(I2C_BASE_IOEXPANDER, 0x74);

// Set I2C to controller transmit mode (make sure your header defines the proper macro)
I2C_setConfig(I2C_BASE_IOEXPANDER, I2C_CONTROLLER_SEND_MODE);

// Set addressing mode to 7-bit
I2C_setAddressMode(I2C_BASE_IOEXPANDER, I2C_ADDR_MODE_7BITS);

// Enable FIFO (use the same base address)
I2C_enableFIFO(I2C_BASE_IOEXPANDER);

// Clear any pending interrupts
I2C_clearInterruptStatus(I2C_BASE_IOEXPANDER, I2C_INT_ARB_LOST | I2C_INT_NO_ACK);

// Set FIFO interrupt levels (again, use the same base address)
I2C_setFIFOInterruptLevel(I2C_BASE_IOEXPANDER, I2C_FIFO_TXEMPTY, I2C_FIFO_RX2);

// Enable required interrupts
I2C_enableInterrupt(I2C_BASE_IOEXPANDER, I2C_INT_ADDR_SLAVE | I2C_INT_ARB_LOST | I2C_INT_NO_ACK | I2C_INT_STOP_CONDITION);

// Set emulation mode
//I2C_setEmulationMode(I2C_BASE_IOEXPANDER, I2C_EMULATION_FREE_RUN);

// Finally, enable the I2C module
I2C_enableModule(I2C_BASE_IOEXPANDER);
}

/**
 *  ADC_Init
 *
 *  @brief Sets the ADC SOC and enable the interrupt.
 *
 *  This function is helpful to trigger the ADC SOC by using ADC_TRIGGER_SW_ONLY and enables the interrupt of another function.
 *
 *  @return None.
 */
void ADC_Init()
{
    uint8_t i=0;
    ADC_setVREF(CHARGER_ADC_BASE, ADC_REFERENCE_EXTERNAL, ADC_REFERENCE_Volt);
    ADC_setPrescaler(CHARGER_ADC_BASE, ADC_CLK_DIV_2_0);            // -> 50MHZ
    ADC_setInterruptPulseMode(CHARGER_ADC_BASE, ADC_PULSE_END_OF_CONV);
    ADC_enableConverter(CHARGER_ADC_BASE);
    DEVICE_DELAY_US(5000);

    ADC_enableBurstMode(CHARGER_ADC_BASE);
    ADC_setBurstModeConfig(CHARGER_ADC_BASE, ADC_TRIGGER_SOURCE, (ADC_BURST_SIZE-1));
    ADC_setSOCPriority(CHARGER_ADC_BASE, ADC_PRIORITY_SEL);

    for(i = 0;i<=ADC_BURST_SIZE;i++)
    {
        if(i == 7)
            ADC_setupSOC(CHARGER_ADC_BASE, (ADC_SOCNumber)i, ADC_TRIGGER_SOURCE,(ADC_Channel) 8, ADC_CYCLE_TIME);   // ADC_TRIGGER_SW_ONLY IS IGNORED IN BRUST MODE
        else
            ADC_setupSOC(CHARGER_ADC_BASE, (ADC_SOCNumber)i, ADC_TRIGGER_SOURCE,(ADC_Channel) i, ADC_CYCLE_TIME);   // ADC_TRIGGER_SW_ONLY IS IGNORED IN BRUST MODE
        ADC_setInterruptSOCTrigger(CHARGER_ADC_BASE,(ADC_SOCNumber) i, ADC_INT_SOC_TRIGGER_NONE);
    }
    ADC_setInterruptSource(CHARGER_ADC_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER7);
    ADC_clearInterruptStatus(CHARGER_ADC_BASE, ADC_INT_NUMBER1);
    ADC_disableContinuousMode(CHARGER_ADC_BASE, ADC_INT_NUMBER1);
    ADC_enableInterrupt(CHARGER_ADC_BASE, ADC_INT_NUMBER1);
}

/**
 *  CPUTimerInit
 *
 *  @brief Intializing the CPU timer.
 *
 *  This function is helpful to setup the CPU timer.
 *
 *  @reuturn None.
 */
void CPUTimerInit(uint32_t base, uint16_t TimeMS)
{
    uint32_t PRDValue;
    PRDValue = (uint64_t)((DEVICE_SYSCLK_FREQ * TimeMS)/1000) -1;
    CPUTimer_setPeriod(base, PRDValue);
    CPUTimer_setPreScaler(base, 0);
    CPUTimer_stopTimer(base);
    CPUTimer_reloadTimerCounter(base);
    CPUTimer_enableInterrupt(base);
}
/**
 * This Function is to initialize the Pins For  for Flow meter frequency measurement
 *
 * @param FlowMeterPin is the Analog Pin that gets data form the Flow meter
 * @param MuxPIN is the digital  gpio pin for the analog input we are getting
 * @param XbarPIN the crossbar parameter to get the PWM from flow meter
 * @param GpioInt the gpio_external_interrupt  parameter to set the interrupt parameter forn gpio
 */
void GunFlowMeterPinInit(uint16_t FlowMeterPin,uint32_t MuxPIN, uint8_t XbarPIN,GPIO_ExternalIntNum GpioInt)
{
    GPIO_setAnalogMode(FlowMeterPin, GPIO_ANALOG_DISABLED);
    GPIO_setPadConfig(FlowMeterPin, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(MuxPIN);
    GPIO_setDirectionMode(FlowMeterPin, GPIO_DIR_MODE_IN);
    GPIO_setQualificationMode(FlowMeterPin, GPIO_QUAL_3SAMPLE);
    XBAR_setInputPin((XBAR_InputNum)XbarPIN, FlowMeterPin);

    GPIO_setInterruptPin(FlowMeterPin,GpioInt);
    GPIO_setInterruptType(GpioInt, GPIO_INT_TYPE_RISING_EDGE);
    GPIO_disableInterrupt(GpioInt);
}
/**
 * This Function is to send the data via UART
 *
 * @param SCIHandle is the base address of the SCI
 * @param Data is the pointer to the data to be transmitted
 * @param Txlength is the length of the  transmitting data
 */
void MCU_UART_TxData(uint32_t SCIHandle,uint8_t *Data,uint8_t Txlength)
{
    uint8_t i;
    WriteDGPIO(MODBUS_GPIODERE,1);
    SCI_resetTxFIFO(SCIHandle);
    //SCI_resetRxFIFO(SCIHandle);
    for ( i = 0; i < Txlength; i++)
    {
        while(SCI_getTxFIFOStatus(SCIHandle) > SCI_FIFO_TX16);
        SCI_writeCharBlockingFIFO(SCIHandle, Data[i]);
    }
    // Wait until TX FIFO is completely empty
    while(SCI_getTxFIFOStatus(SCIHandle) != SCI_FIFO_TX0);
}
/**
 * This Function is to check whether I2C is busy or not
 *
 * @param base is the base address of the I2c
 *
 *
 * @return uint16_t value for busy I2C
 */
uint16_t I2CisBusBusy(uint32_t base)
{
    return I2C_isBusBusy(base);
}
/**
 * This Function is to check stop condition for I2C
 *
 * @param base is the base address of the I2c
 *
 * @return uint32_t value for busy I2C
 */
uint32_t I2CgetStopConditionStatus(uint32_t base)
{
    return I2C_getStopConditionStatus(base);
}
/**
 * This Function is to enable FIFO for I2C
 *
 * @param base is the base address of the I2c
 *
 * @return none
 */
void I2CenableFIFO(uint32_t base)
{
    I2C_enableFIFO(base);
}
/**
 * This Function is to enable base address configuration for I2C
 *
 * @param base is the base address of the I2c
 * @param config is the configuration mode of the I2C
 *
 * @return none
 */
void I2CsetConfig(uint32_t base, uint16_t config)
{
    I2C_setConfig(base, config);
}
/**
 * This Function is to set the slave address for I2C
 *
 * @param base is the base address of the I2c
 * @param slaveAddress is the slave address of the I2C
 *
 * @return none
 */
void I2CsetSlaveAddress(uint32_t base,uint16_t slaveAddress)
{
    I2C_setSlaveAddress(base,slaveAddress);
}
/**
 * This Function is to set the data count register value for I2C
 *
 * @param base is the base address of the I2c
 * @param count is the value to be put in the I2C data count register.
 *
 * @return none
 */
void I2CsetDataCount(uint32_t base,uint16_t count)
{
    I2C_setDataCount(base,count);
}
/**
 * This Function is to transmit the data  for I2C
 *
 * @param base is the base address of the I2c
 * @param reg is the data to be transmitted
 *
 * @return none
 */
void I2CputData(uint32_t base,uint16_t reg)
{
    I2C_putData(base,reg);
}
/**
 * This Function is to transmit the start condition  for I2C
 *
 * @param base is the base address of the I2c
 *
 * @return none
 */
void I2CsendStartCondition(uint32_t base)
{
    I2C_sendStartCondition(base);
}
/**
 * This Function is to get the transmit FIFO status for I2C
 *
 * @param base is the base address of the I2c
 *
 * @return uint32_t type value of the status
 */
uint32_t I2CgetTxFIFOStatus(uint32_t base)
{
    return I2C_getTxFIFOStatus(base);
}
/**
 * This Function is to transmit the stop condition  for I2C
 *
 * @param base is the base address of the I2c
 *
 * @return none
 */
void I2CsendStopCondition(uint32_t base)
{
    I2C_sendStopCondition(base);
}
/**
 * This Function is to get the receive FIFO status for I2C
 *
 * @param base is the base address of the I2c
 *
 * @return uint16_t type value of the status
 */
uint16_t I2CgetRxFIFOStatus(uint32_t base)
{
    return I2C_getRxFIFOStatus(base);
}
/**
 * This Function is to send the I2C error status defined by the status register of the I2C
 *
 * @param base is the base address of the I2C
 * @param I2Cstatus is the pointer to the I2C status
 *
 * @return none
 */
uint8_t I2C_Error(uint32_t base)
{
    uint16_t status = HWREGH(base + I2C_O_STR);

    if( ((status & I2C_STR_NACK)) || (status & I2C_STR_ARBL))
    {
        return 1;
    }
    return 0;
}

void SCI_BreakDtect()
{
    if((HWREGH(MODBUS_SCI_BASE + SCI_O_RXST) & SCI_RXST_BRKDT) ==  SCI_RXST_BRKDT)
    {
        SCI_performSoftwareReset(MODBUS_SCI_BASE);
    }
}
uint16_t CANgetStatus(uint32_t base)
{
    return CAN_getStatus(base);
}
uint8_t CANreadMessageWithID(uint32_t base,
                           uint32_t objID,
                           CAN_MsgFrameType *frameType,
                           uint32_t *msgID,
                           uint16_t *msgData)
{
    return CAN_readMessageWithID(base, objID, frameType, msgID, msgData);
}
uint32_t CANgetInterruptCause(uint32_t base)
{
    return CAN_getInterruptCause(base);
}
void CANclearInterruptStatus(uint32_t base, uint8_t mailbox)
{
    CAN_clearInterruptStatus(base, mailbox);
}
void CANclearGlobalInterruptStatus(uint32_t base, uint32_t clearbit)
{
    CAN_clearGlobalInterruptStatus(base, clearbit);
}
void InterruptclearACKGroup(uint16_t integroup)
{
    Interrupt_clearACKGroup(integroup);
}
uint32_t CPUTimergetTimerCount(uint32_t base)
{
    return CPUTimer_getTimerCount(base);
}
void InitialiseCANObject()
{
    CANMSGObjectSetup(RECTIFIER_GUN1, BOOT_RX_MAILBOX, BOOT_APPMSG_ID, ExtendedFrame, CANOBJECTTYPERX, 0, CAN_MSG_OBJ_RX_INT_ENABLE);

    RNLRXCANOBJSet(RECTIFIER_GUN1);//added a acomment for git push
    RNLTXCANOBJSet(RECTIFIER_GUN1);
    RNLRXCANOBJSet(RECTIFIER_GUN2);
    RNLTXCANOBJSet(RECTIFIER_GUN2);
    TonheTXCANOBJSet(RECTIFIER_GUN1);  // Rectifier CAN bus as
    TonheTXCANOBJSet(RECTIFIER_GUN2);  // Rectifier CAN bus as
    TonheRXCANOBJSet(RECTIFIER_GUN1,CANOBJ_RECTIFIER);
    TonheRXCANOBJSet(RECTIFIER_GUN2,CANOBJ_RECTIFIER);
    // CAN Object For CMS
   // CAN_setupMessageObject(CANA_BASE, BOOT_RX_MAILBOX, BOOT_APPMSG_ID , CAN_MSG_FRAME_EXT,CAN_MSG_OBJ_TYPE_RX, 0, CAN_MSG_OBJ_RX_INT_ENABLE,8);
    CANMSGObjectSetup(RECTIFIER_GUN1, BOOT_RX_MAILBOX, BOOT_APPMSG_ID, ExtendedFrame, CANOBJECTTYPERX, 0, CAN_MSG_OBJ_RX_INT_ENABLE);
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSMODBUS_TX, CANOBJ_CMS_ID_BASE, STDFrame, CANOBJECTTYPETX, CANOBJ_CMS_FIL_MASK, CAN_MSG_OBJ_USE_ID_FILTER);
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSGUNDATA_TX, CANOBJ_CMS_ID_BASE, STDFrame, CANOBJECTTYPETX, CANOBJ_CMS_FIL_MASK, CAN_MSG_OBJ_USE_ID_FILTER);
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSIOT_TX, CANOBJ_CMS_ID_BASE, STDFrame, CANOBJECTTYPETX, CANOBJ_CMS_FIL_MASK, CAN_MSG_OBJ_USE_ID_FILTER);
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMSCOMSTATUS_TX, CANOBJ_CMS_ID_BASE, STDFrame, CANOBJECTTYPETX, CANOBJ_CMS_FIL_MASK, CAN_MSG_OBJ_USE_ID_FILTER);


    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBJ_CMS_RX, CAN_CMSTX_IOTGUNCTRL,
                          STDFrame, CANOBJECTTYPERX,MASKFORCMSCAN,
                          CAN_MSG_OBJ_RX_INT_ENABLE|CAN_MSG_OBJ_USE_ID_FILTER);
    CANMSGObjectSetup(RECTIFIER_GUN1, CANOBj_CMSGUNSTATUS_TX, CANOBJ_CMS_ID_BASE, STDFrame, CANOBJECTTYPETX, CANOBJ_CMS_FIL_MASK, CAN_MSG_OBJ_USE_ID_FILTER);
}
