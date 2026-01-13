#ifndef MCUSPECIFIC_H
#define MCUSPECIFIC_H

#include "TCA9539.h"
#ifndef DEVICE_H
    #include "device.h"
#endif

#ifndef DRIVERLIB_H
    #include "driverlib.h"
#endif

#define CANFRAMETYPE    CAN_MsgFrameType
#define COUNTFORSECOND      4
#define RADIATORTIME        2
#define RADIATORCOUNT       RADIATORTIME*COUNTFORSECOND

#define GPIO_INPUT          GPIO_DIR_MODE_IN
#define GPIO_OUTPUT         GPIO_DIR_MODE_OUT

#define PUSHPULL       GPIO_PIN_TYPE_STD
#define OPENDRAIN       GPIO_PIN_TYPE_OD

#define FLOATING     GPIO_PIN_TYPE_STD
#define PULLUP    GPIO_PIN_TYPE_PULLUP

#define ANALOG_DISABLED         GPIO_ANALOG_DISABLED
#define ANALOG_ENABLED          GPIO_ANALOG_ENABLED

#define PullUpPin            1
#define PullDownPin          0

#define PulledUpSignal       PullUpPin
#define PulledDownSignal       PullDownPin
//++++++++++++++++++++++++++++ Section For CONTACTOR, FB, FM,FMFB +++++++++++++++++

//++++++++++++++++++++++++++++ Section For CONTACTOR, FB, FM,FMFB +++++++++++++++++

#define DC2_STS_LED_MCU                     28          //Done
#define PLC_ENABLEPIN                       12          //Done
#define EMP_EN_MCU                          PLC_ENABLEPIN      //new
#define DISPLAY2_POWERPIN                   11
#define SPR_EN_MCU                          DISPLAY2_POWERPIN     //new
#define DC2_STP_LED_MCU                     33
#define DISPLAY1_POWERPIN                   16
#define Display_PWR_MCU                     DISPLAY1_POWERPIN
#define DC2_EM_POWERPIN                     17
#define SPR_RLY_MCU                         DC2_EM_POWERPIN       //New
#define AC_CONTECTOR                        25
#define AC_CONTACTOR                        AC_CONTECTOR
//#define COLLING_FANPIN                      changed-STATION_LED             // Temp
#define EXHAUST_FAN                         27
#define PUMP_POWER_ENABLE                   DC2_STS_LED_MCU
#define SPARE2                              57
#define SPARE1                              58
#define RAD_EN_MCU                          29//8        //DC1_EM_POWERPIN-changed
#define LED_BLUE                             23
//#define LED_BLUE                            22
#define DC1_STP_LED_MCU                     13
#define DC1_STS_LED_MCU                     40              //Done
#define GUN1_DCPOSITIVE_CONTACTOR           15
#define GUN1_DCNEGATIVE_CONTACTOR           34
#define GUN1_DCPRECHARGE_CONTACTOR          5
#define GUN2_DCPOSITIVE_CONTACTOR           59
#define GUN2_DCNEGATIVE_CONTACTOR           39
#define GUN2_DCPRECHARGE_CONTACTOR          9
#define CROSS_CONTACTOR                     14
#define STATION_LED                         40
#define ADD_LED1                            4
#define ADD_LED2                            7

#define DC1_STOPCHARGER_SW                  244
#define DC1_STOP_CHARGER_PINMUX             GPIO_244_GPIO244

#define DC2_STOPCHARGER_SW                  245
#define DC2_STOP_CHARGER_PINMUX             GPIO_245_GPIO245

//++++++++++++++++Flow meter pin intialization
#define GUN1_FLOWMETER                      227
#define GUN1_FLOWMETER_PINMUX               GPIO_227_GPIO227
#define GUN1_FLOWMETER_XBARPIN              XBAR_INPUT7
#define GUN2_FLOWMETER                      236
#define GUN2_FLOWMETER_PINMUX               GPIO_236_GPIO236
#define GUN2_FLOWMETER_XBARPIN              XBAR_INPUT5

// Section for  AnalogPins (BRUST OR SCAN MODE)

#define TempGain                     5.0f/3.0f
#define TempPullUpVolt               5000.0f
#define TempPullUpRes                1000.0f
#define TempPullUpResPTC             1000.0f
#define NTCA                        -0.00023f
#define NTCB                         0.233f
#define NTCC                        -246.3f

#define BETA  3950.0f  // Beta value (example, check datasheet)
#define R0    10000.0f  //
#define T0    298.15f  // 25C in Kelvin

#define ADC_TRIGGER_SOURCE          ADC_TRIGGER_CPU1_TINT1
#define ADC_PRIORITY_SEL            ADC_PRI_ALL_ROUND_ROBIN
#define ADC_BURST_SIZE              8U
#define ADC_CYCLE_TIME              20U
#define ADC_INT_LINE                INT_ADCA1
#define ADC_REFERENCE_Volt          ADC_REFERENCE_VREFHI
#define ADC_REF_VOLT                3300
#define ADCA_TOTAL_CH               8
#define CHARGER_ADC_BASE            ADCA_BASE
#define MCUTEMP_ADC_BASE            ADCB_BASE

#define CHARGER_AMB_TEMP_PIN        ADC_CH_ADCIN0
#define BASBAR_1_PIN                ADC_CH_ADCIN1
#define BASBAR_2_PIN                ADC_CH_ADCIN2
#define FLUID_TEMP_PIN              ADC_CH_ADCIN3
#define GUN1_POSITIVE_PIN           ADC_CH_ADCIN4
#define GUN1_NEGATIVE_PIN           ADC_CH_ADCIN5
#define GUN2_NEGATIVE_PIN           ADC_CH_ADCIN6
#define GUN2_POSITIVE_PIN           ADC_CH_ADCIN8

#define CHARGER_AMB_TEMP_SOC        ADC_SOC_NUMBER0
#define BASBAR_1_SOC                ADC_SOC_NUMBER1
#define BASBAR_2_SOC                ADC_SOC_NUMBER2
#define FLUID_TEMP_SOC              ADC_SOC_NUMBER3
#define GUN1_POSITIVE_SOC           ADC_SOC_NUMBER4
#define GUN1_NEGATIVE_SOC           ADC_SOC_NUMBER5
#define GUN2_NEGATIVE_SOC           ADC_SOC_NUMBER6
#define GUN2_POSITIVE_SOC           ADC_SOC_NUMBER7


#define CMS_PCB_ADC_BASE         ADCC_BASE
#define CMS_PCB_TEMP_PIN        ADC_CH_ADCIN4
#define CMS_PCB_TEMP_SOC        ADC_SOC_NUMBER0

// +++++++++++++++++++++++++++ Section for ModBUS Macros  ++++++++++++++++++++++++++++++
#define MODBUS_SCI_BASE         SCIA_BASE  // Change to SCIA_BASE if using SCI-A
#define MODBUS_BAUDRATE         38400
#define MODBUS_DATABITS         SCI_CONFIG_WLEN_8
#define MODBUS_PARITY           SCI_CONFIG_PAR_NONE  // SCI_CONFIG_PAR_EVEN / SCI_CONFIG_PAR_ODD
#define MODBUS_STOPBITS         SCI_CONFIG_STOP_ONE // SCI_CONFIG_STOP_TWO

// GPIO Setting
#define MODBUS_GPIO_Tx          2
#define MODBUS_GPIO_Rx          3
#define MODBUS_GPIO_MUXRx       GPIO_3_SCIA_RX
#define MODBUS_GPIO_MUXTx       GPIO_2_SCIA_TX

/*Routed on I2C Mux */
#define MODBUS_GPIODERE         22
#define MODBUS_GPIO_MUXDERE     GPIO_22_GPIO22
// --------------------------- Section for ModBUS Macros-----------------------------

//+++++++++++++++++++++++++++++++ I2C IO Expander ++++++++++++++++++++++++++++++++++++++
#define I2C_BASE_IOEXPANDER             I2CA_BASE
#define I2C_SCL_GPIO                    1
#define I2C_SDA_GPIO                    0
#define I2C_GPIO_MUXSCL                 GPIO_1_I2CA_SCL
#define I2C_GPIO_MUXSDA                 GPIO_0_I2CA_SDA
#define I2C_SPEED                       100000
#define I2C_EXP1_ADD                    0x74            //  15 Read Signals
#define I2C_EXP2_ADD                    0x75            //  8  Write Signals
#define I2C_EXP3_ADD                    0x76            // 15  Read Signals
//+++++++++++++++++++++++++++++++ I2C IO Expander ++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++ Section for CAN Bus Macros++++++++++++++++++++++++++++++
#ifdef Launchpadx
#define CANATx_GPIO                      32
#define CANA_GPIO_MUXTX                  GPIO_32_CANA_TX

#define CANARx_GPIO                      33
#define CANA_GPIO_MUXRX                  GPIO_33_CANA_RX
#else
#define CANATx_GPIO                      31
#define CANA_GPIO_MUXTX                  GPIO_31_CANA_TX

#define CANARx_GPIO                      30
#define CANA_GPIO_MUXRX                  GPIO_30_CANA_RX
#endif



#define CANBTx_GPIO                      6
#define CANB_GPIO_MUXTX                  GPIO_6_CANB_TX

#define CANBRX_GPIO                      10
#define CANB_GPIO_MUXRX                  GPIO_10_CANB_RX


#define RECTIFIER_GUN1                  CANA_BASE
#define RECTIFIER_GUN2                  CANB_BASE
#define VSECC_CAN_BASE                  RECTIFIER_GUN1

#define CANSPEED                         125000
#define STDFrame                         CAN_MSG_FRAME_STD
#define ExtendedFrame                    CAN_MSG_FRAME_EXT

#define CANOBJECTTYPERX                  2
#define CANOBJECTTYPETX                  0

// For Rx Msgs only we are using mailbox with multi-msg capability
#define CANOBJ_CMS_RX                   5     // CMS receive  msgs
#define CANOBJ_RECTIFIER                1
#define PECC_STATUS1                    2
//#define CANOBJ_VSECC1BASEIO             3
#define PECC_STATUS2                    4

#define CANOBJ_VSECC2BASEIO             31
// Each Tx msg from CMS to VSECC will have its own MailBox
#define CANOBJ_G1PECCSTATUS1              6
#define CANOBJ_G1PECCSTATUS2              7
#define CANOBJ_G1PECCLIMITS1              8
#define CANOBJ_G1PECCLIMITS2              9
#define CANOBJ_G1PECCLIMITS3              10
//#define CANOBJ_G1STOPCHARGING             11

// Each Tx msg from CMS to VSECC will have its own MailBox
#define CANOBJ_G2PECCSTATUS1              12
#define CANOBJ_G2PECCSTATUS2              13
#define CANOBJ_G2PECCLIMITS1              14
#define CANOBJ_G2PECCLIMITS2              15
//#define CANOBJ_G2PECCLIMITS3              16
//#define CANOBJ_G2STOPCHARGING             17

//RNL CAN MESSAGE MAILBOX
#define CANOBJ_RMC_RX    3
#define CANOBJ_CHM_TX    6
#define CANOBJ_CRM_TX    7
#define CANOBJ_CTS_TX    8
#define CANOBJ_CML_TX    9
#define CANOBJ_CRO_TX    10
#define CANOBJ_CCS_TX    11
#define CANOBJ_CST_TX    12
#define CANOBJ_CSD_TX    13
#define CANOBJ_CEM_TX    14
#define CANOBJ_BCPA_TX   15

// Each Tx msg from CMS to Tonhe will have its own MailBox
#define CANOBJ_TONHE_STARTSTOP           18   // Module Start/Stop Command
#define CANOBJ_TONHE_PARAMETERSETTING    19   // Module Parameter Setting
#define CANOBJ_TONHE_TIMINGCOMMAND       20   // Timing Command
#define CANOBJ_TONHE_INPUTMODECONFIG     21   // Input Mode Configuration
#define CANOBJ_TONHE_MODULEADDRSET       22   // Module Address Setting
#define CANOBJ_CMSMODBUS_TX              23    // CMS transmit  msgs
#define CANOBJ_CMSGUNDATA_TX             24
#define CANOBJ_CMSIOT_TX                 27
#define CANOBJ_CMSCOMSTATUS_TX           26
#define CANOBj_CMSGUNSTATUS_TX           28

#define BOOT_RX_MAILBOX                  25    // For Boot Loader
#define RESET_MAILBOX                    29

#define STOPCHARGE                       14
#define CANOBJ_VEHICLE_STATUS            16
#define CANOBJ_PECC2                     17
#define SEND_SCIERR                      0x11110000


// CAN For CMS msg
#define CANOBJ_CMS_ID_BASE                0x100
#define CANOBJ_CMS_FIL_MASK               0xF00

#define MASKFORCMSCAN                     0x600
#define CAN_CMSTX_CHARGERTEMP             0X601
#define CAN_CMSTX_CHARGERDIO              0X602
#define CAN_CMSTX_ACLNVoltage             0x603
#define CAN_CMSTX_ACCurrent               0x604
#define CAN_CMSTX_ACENERGY                0x605
#define CAN_CMSTX_ACPower                 0x606
#define CAN_CMSTX_ACMeter                 0x607
#define CAN_CMSTX_DCRIGHTVoltAmp          0x608                 // RightGun EM V &I
#define CAN_CMSTX_DCPower                 0x609
#define CAN_CMSTX_DCPosEnergy             0x60A                 // Energy Fot BothGUn

#define CAN_CMSTX_IMD                     0x60C
#define CAN_CMS_TXStopCharge              0x60D
//  60E is not used rsvd for future
#define CAN_CMSTX_DCLEFTVoltAmp           0x60F                 // LeftGun EM V &I
#define CAN_CMSTX_FLOW                    0x610
#define CAN_CMS_VERSION                   0x611
#define CAN_CMS_VSECCSTATUS               0X612
#define CAN_CMS_ACENERGYMETER_STATUS      0x613
//  614,5,6 is not used rsvd for future
#define CAN_CMS_FAULTS                    0X617
#define CAN_CMS_TONHESTATUS               0x618
#define CAN_CMS_IOTSTARTSTOP              0x619
#define CAN_CMS_I2CSTATUS                 0x61A
#define CAN_CMS_GUNDATA                   0x61B
#define CAN_CMS_MeasureData               0x61C
#define CAN_CMSTX_DCDiffEnergy            0x61D                 // Energy Fot BothGUn***********************
// 61E is not used rsvd for future
#define CAN_CMS_Gunstate                  0x61F


#define CAN_CMS_RNLVERIFY                 0x301
#define CAN_CMS_RNLVERIFYLeft             0x701
#define CAN_PECCSTATUS1_RIGHT             0x302
#define CAN_PECCSTATUS1_LEFT              0x702
#define CAN_PECCSTATUS2_RIGHT             0x303
#define CAN_PECCSTATUS2_LEFT              0x703

#define CAN_PECCLIMITS1_RIGHT             0x304
#define CAN_PECCLIMITS1_LEFT              0x704
#define CAN_PECCLIMITS2_RIGHT             0x305
#define CAN_PECCLIMITS2_LEFT              0x705
#define CAN_RESET_RIGHT                   0x306
#define CAN_RESET_LEFT                    0x706

#define RIGHT_VEHICLEID                   0x308
#define LEFT_VEHICLEID                    0x708
#define CAN_STOPCHARGE_RIGHT              0x309
#define CAN_STOPCHARGE_LEFT               0x709

#define CAN_CHARGESESSION_INFO1_RIGHT     0x30A
#define CAN_CHARGESESSION_INFO1_LEFT      0x70A

#define CAN_CHARGESESSION_INFO2_RIGHT     0x30B
#define CAN_CHARGESESSION_INFO2_LEFT      0x70B

#define CAN_CHARGESESSION_INFO3_RIGHT     0x30D
#define CAN_CHARGESESSION_INFO3_LEFT      0x70D


// 6120-7 is not used rsvd for future
#define CAN_RIGHT_VEHICLEID               0x628
#define CAN_LEFT_VEHICLEID                0x629

#define CAN_Reset                         0x62A

// RX msg from IOT
#define CAN_CMSTX_IOTGUNCTRL              0x681
#define CAN_CMSRX_FAULTCLEARANCE          0x682
#define CAN_RX_State                      0x683

#define RECTIFIER_BASEID                 0x0000A000
#define RECTIFIER_IDFILTER               0x0000FF00

#define BOOT_APPMSG_ID                   0x12EF34AB

#define RectPerGun                       2

//----------------------------------Analog Charger Struct ----------------------
typedef struct
{
        float BasBar2Volt;
        float ChargerAmbTempVolt;
        float BasBar1Volt;
        float FluidTempVolt;
        float GUN2NegVolt;
        float GUN2PosVolt;
        float GUN1NegVolt;
        float GUN1PosVolt;

}ChargerAnalogReadStruct;


//++++++++++++++++++++++++++++++ Section for ModBUS Enumeration & Structure ++++++++++++++++++++++++++++++
typedef enum
{
    ModBusSuccess =0,
    ModBusTimout = 1,
}ModbusCommstatus;


typedef enum
{
    SCIframing =0,
    SCIparity = 1,
    SCIoverrun = 2,
    SCIbreak = 3,
}SciErrorstatus;
//----------------------------------Section for ModBUS Enumeration & Structure ---------------------

//+++++++++++++++++++++++++++++  Charger Out Pins +++++++++++++++++++++
/**
 *  This function is helpful to configure the analog pins as a digital output by using GPIOSetConfig() function.
 */
void InitialiseChargerOutPins();

/**
 *  This function is helpful to set a analog pin as a digital pin.
 */
void InitialiseChargerInPins();

/**
 *  This function helpful to disable the analog mode of a GPIO and set as output and configure as a pullup.
 *
 *  @param GPIOpin it tell which GPIO pin.
 *  @param Mod tell the output.
 *  @param AnalogMode it tells the Analog mode disabled or enabled.
 *  @param type it tells pullup or pulldown.
 */
void GPIOSetConfig(uint32_t GPIOpin,GPIO_Direction Mod,uint32_t type,GPIO_AnalogMode AnalogMode);

//+++++++++++++++++++++++++++++++  ADC functions ++++++++++++++++
/**
 *  This function is helpful to trigger the ADC SOC by using ADC_TRIGGER_SW_ONLY and enables the interrupt of another function.
 */
void ADC_Init();

/**
 *  This function is helpful to convert sensor voltage to temperature by using tempgain and pullup resistance and voltage.
 *
 *  @param SensorVolatge is a variable stores voltage value.
 */
float GetPT1000TempValue(float32_t SensorVoltage);

/**
 *  This function is helpful to convert sensor voltage to temperature by using tempgain and NTCA and NTCB.
 *
 *  @param SensorVolatge is a variable stores voltage value.
 */
float GetNT1000TempValue(float32_t SensorVoltage);

//++++++++++++++++++++++++++++++++  LiquidColling +++++++++++

//++++++++++++++++++++++++++++++ Generic Function ++++++++++++++++++++++++++++++
/**
 *  This function is helpful to write the value to specified GPIO pin.
 *
 *  @param GPIO Pin number.
 *  @param value data we want to send.
 */
uint8_t WriteDGPIO(uint32_t GPIO,uint32_t  Value);

/**
 *  This function helpful to reads the specified GPIO, and it returns the value.
 *
 *  @param Returns the value in the data register for the specified pin.
 */
uint8_t ReadGPIO(uint32_t GPIO);

/**
 *  This function is helpful to gives the stop switch value for every 250 milli seconds based on current switch state, we use this logic
 *  becaue we reads the switch for every 250 Milli seconds.
 *
 *  @param Signal It is a GPIO pin.
 *  @param InitialState Current position of switch.
 *  @param swstate Current state of pin 0 or 1.
 */
uint8_t DebounceLogic(uint8_t Signal,uint8_t InitialState,uint8_t * swstate);

/**
 *  his function is helpful to reads the input from debounce switch.
 *
 *  @param GPIO It is a GPIO pin.
 *  @param InitialState Current position of switch.
 *  @param swstate Current state of pin 0 or 1.
 */
uint8_t SwitchPinReadDebounce(uint32_t GPIO,uint8_t InitialState,uint8_t *swstate);

/**
 *  This function is helpful to return the time in milli second based on input value.
 *
 *  @param x is a variable is used to set the time.
 */
uint32_t Timeout_MS(uint32_t x);
//static uint64_t SwapEndian64(uint64_t value);

/**
 *  This function is helpful to swap the 8-bits and return value.
 *
 *  @param reg is a variable.
 */
uint16_t swapBytes(uint16_t reg);

/**
 *  This function is helpful to setup the CPU timer.
 */
void CPUTimerInit(uint32_t base, uint16_t TimeMS);
void CPUTimerInitCST(uint32_t base, uint16_t TimeMS);
//---------------------------------- Generic Function--------------------------

//-------------------------------------- Section for I2C Function Declaration ----------+
/**
 *  This function is helpful to configure the GPIO pin as a I2C pin and set the frequency, speed, duty cycle and sets the data byte
 *  bit count and sets the target address, and enable transmit and receive and enable the module.
 */
void I2CGPIOExpanstionInit();


//---------------------------------------Section for ModBUS Function Declaration ---------------------
/**
 *  This function transmits data over the given UART port and waits for the response. It stores the received bytes into RxBuffer.
 *  If the recieve FIFO didnt get any status within a time it returns ModBusTimout.
 *
 *  @param SCIHandle UART peripheral handle (e.g., SCI_A, SCI_B).
 *  @param Data Pointer to the data to be transmitted.
 *  @param Txlength Number of bytes to transmit.
 *  @param RxBuffer Pointer to the buffer to store received data.
 *  @param Rxlength Number of bytes expected to receive.
 *  @param timeout Timeout in milliseconds for receiving data.
 */
ModbusCommstatus MCU_UART_TxRxData(uint32_t SCIHandle,uint8_t *Data,uint8_t Txlength,uint8_t *RxBuffer,uint8_t Rxlength,uint8_t timout);

/**
 *  This function sets up the transmit and receive pins, as well as the direction control pin.
 */
void setModBusGPIO();

/**
 *  This function configures the UART settings such as base address, parity, stop bits, and data bits using `setModBusUART()`.
 */
void setModBusUART();
//---------------------------------------Section for ModBUS Function Declaration ---------------------

/**
 *  This function is useful to configure the alternative function to GPIO and Initializes the CAN and set the clock frequency and
 *  speed.
 */
void InitialiseCAN();

/**
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
void CANMSGObjectSetup(uint32_t CANBASE,uint8_t ObjectNumber,
                       uint32_t BaseID,uint8_t FrameType,
                       uint8_t ObjectType,uint32_t MaskID,
                       uint32_t IntEnable);
//extern __interrupt void CAN_A_ISR(void);
//extern __interrupt void CAN_B_ISR(void);

/**
 *  This function is useful to send the message with the help CANBASE address and Mail bix number.
 *
 *  @param CANBASE is the base address of the CAN controller.
 *  @param MailBoxNo is the object number to configure (1-32).
 *  @param length is the number of bytes of data in the message object (0-8).
 *  @param Data is a pointer to the message object's data.
 */
void CANSendMessage(uint32_t CANBASE,uint32_t MailBoxNo,
                     uint64_t *Data,uint8_t length);

/**
 *  This function is helpful to enable the interrupts for rectifier gun1 and gun2.
 */
void CANInterruptEnable();


void GunFlowMeterPinInit(uint16_t FlowMeterPin,uint32_t MuxPIN, uint8_t XbarPIN,GPIO_ExternalIntNum GpioInt);

void InitialiseCANObject();

void MCU_UART_TxData(uint32_t SCIHandle,uint8_t *Data,uint8_t Txlength);
uint8_t SCI_CheckError(uint32_t SCIHandle);
void SendSCIErrorCan(uint8_t *canData,uint32_t CANBaseAddr);
uint16_t I2CisBusBusy(uint32_t base);
uint32_t I2CgetStopConditionStatus(uint32_t base);
void I2CenableFIFO(uint32_t base);
void I2CsetConfig(uint32_t base, uint16_t config);
void I2CsetSlaveAddress(uint32_t base,uint16_t slaveAddress);
void I2CsetDataCount(uint32_t base,uint16_t count);
void I2CputData(uint32_t base,uint16_t reg);
void I2CsendStartCondition(uint32_t base);
uint32_t I2CgetTxFIFOStatus(uint32_t base);
void I2CsendStopCondition(uint32_t base);
uint16_t I2CgetRxFIFOStatus(uint32_t base);
uint8_t I2C_Error(uint32_t base);
void SCI_BreakDtect();
uint16_t CANgetStatus(uint32_t base);
uint8_t CANreadMessageWithID(uint32_t base, uint32_t objID, CAN_MsgFrameType *frameType, uint32_t *msgID,uint16_t *msgData);
uint32_t CANgetInterruptCause(uint32_t base);
void CANclearInterruptStatus(uint32_t base, uint8_t mailbox);
void CANclearGlobalInterruptStatus(uint32_t base, uint32_t clearbit);
void InterruptclearACKGroup(uint16_t integroup);
uint32_t CPUTimergetTimerCount(uint32_t base);
void InitialiseCANObject();
#endif /* MODBUS_H */
