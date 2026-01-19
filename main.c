#include "device.h"
#include "driverlib.h"
/* main File */
#include "MCUSpecific.h"
#include "modbus.h"
#include "ACEMeter.h"
#include "DCEMeter.h"
#include "IMD.h"
#include "VSECC.h"
#include "TonheCAN.h"
#include "Charger.h"
#include <string.h>
#include "RNL.h"
#include "MuxIO.h"
#include "CMSCAN.h"
#include "ChargerApp.h"
#include "F021_F28004x_C28x.h"
#include "flash.h"
#include "driverlib.h"

#define CAN_TX_TIMEOUT_MS   5

//++++++++++++++++++++++Energy Meters and IMD +++++++++++++++++++++
AC_EnergyMeterDataStruct AC_EnergyMeterData;
DC_EnergyMeterDataStruct DC_EnergyMeterData1;
DC_EnergyMeterDataStruct DC_EnergyMeterData2;
float32_t IMDData1;
float32_t IMDData2;
uint8_t SCIbreakDTflag=0;
uint8_t RCAN, LCAN;

ACEMModBusStatusStruct ACEMModBusStatus;
DCEMModBusStatusStruct DCEMModBusStatus1;
DCEMModBusStatusStruct DCEMModBusStatus2;
ModbusCommStatus IMDStatus1;
ModbusCommStatus IMDStatus2;
FaultAndCounterStruct FaultAndCounter ={0,0,0,0,0,0,0,0,0,0,0};

//+++++++++++++++++++ Variable to read a data from I2C and send it on CAN (Raw Value)
uint16_t MuxIO1,MuxIO2;
DIOStatus_t DIOStatus;
uint16_t Data1[2], Data2[2];
I2CCommStatus I2Cstatus[2];

//++++++++++++++++++++++CHARGER ANALOG STRUCT  +++++++++++++++++++++
ChargerAnalogReadStruct ChargerAnalogReadVolt;

//+++++++++++++Timer ISR
uint32_t startTime,endTime,elapsedTime;
uint8_t ModBusStartTaskFlag = 0;
uint8_t ModbusStartTask = 0;
uint8_t ACDisconnect = 0;

//++++++++++++Global Structure for Gun releted and charger relerted data
GunStruct RightGun,LeftGun;
ChargerStruct ChargerData;
status Right,Left;
PeccStatus1 RightPS1,LeftPS1;
PeccStatus2 RightPS2,LeftPS2;
PeccLimits1 RightPL1,LeftPL1;
PeccLimits2 RightPL2,LeftPL2;
ChargeSessionInfo1 RightCSI1,LeftCSI1;
ChargeSessionInfo2 RightCSI2,LeftCSI2;
ChargeSessionInfo3 RightCSI3,LeftCSI3;

//+++++++++++++++++++ Rectfier Data++++++++++++++++
CMSReceiveFromTonheStruct TonheRightGun[3];
CMSReceiveFromTonheStruct TonheLeftGun[3];
RectifierStatus_t Tonhe_Status;

//+++++++++++++++++++ SECC Data++++++++++++++++
ReceiveFromRMLStruct RNLRightGun;
ReceiveFromRMLStruct RNLLeftGun;
SendToRNLStruct RNLRightGunSend;
SendToRNLStruct RNLLeftGunSend;
RNLMSGCTRLEnum RNLRightMSGCTRL;
RNLMSGCTRLEnum RNLLeftMSGCTRL;
RNLFlagEnum RNLRightFlag;
RNLFlagEnum RNLLeftFlag;
RNLDualStruct SendRight;
RNLDualStruct SendLeft;

extern uint16_t TonheRCount;
extern uint16_t TonheLCount;

extern uint8_t BCL_Received_Flag_G1;
extern uint8_t BCL_Received_Flag_G2;
extern uint8_t BST_Received_Flag_G1;
extern uint8_t BST_Received_Flag_G2;
//++++++++++++++++Function Declaration ++++++++++++
 __interrupt void GUN1_CANISR(void);
__interrupt void GUN2_CANISR(void);
__interrupt void Timer1_ISR(void);
__interrupt void Flow_Meter1(void);
__interrupt void Flow_Meter2(void);
__interrupt void wakeupISR(void);

void BenchTestCode();
void SendCMSRNLMSGTOVSECC();
void IMDChecks();
uint8_t CAN_WaitTxComplete(uint32_t base);


void main(void)
{
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);

    Device_init();

    Modbus_Init();

    GPIO_setPadConfig(23, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(23, GPIO_DIR_MODE_OUT);

    memset(&RightGun, 0, sizeof(RightGun));
    memset(&LeftGun, 0, sizeof(LeftGun));
    memset(&ChargerData, 0, sizeof(ChargerData));
    memset(&RNLRightGunSend,0,sizeof(RNLRightGunSend));
    memset(&RNLLeftGunSend,0,sizeof(RNLLeftGunSend));

    //TIMER INITIALIZE FOR THE TIMER_ISR FUNCTION


    // Pin Init section
    I2CGPIOExpanstionInit();
    ConfigMuxInputPins();
    GunFlowMeterPinInit(GUN1_FLOWMETER,GUN1_FLOWMETER_PINMUX,GUN1_FLOWMETER_XBARPIN,GPIO_INT_XINT1);
    GunFlowMeterPinInit(GUN2_FLOWMETER,GUN2_FLOWMETER_PINMUX,GUN2_FLOWMETER_XBARPIN,GPIO_INT_XINT2);

    InitialiseCAN();
    InitialiseCANObject();

    CANInterruptEnable();
    // Start CAN
    CAN_startModule(RECTIFIER_GUN1);
    CAN_startModule(RECTIFIER_GUN2);


    // Interuupt
    Interrupt_initModule();
    Interrupt_initVectorTable();

    Interrupt_enableMaster();
    EINT;
    ERTM;

    //CPU Timer 1 for 250msec
    CPUTimerInit(CPUTIMER1_BASE, ChargerTetraAlgoTime);
    ADC_Init();
    //Interrupt function declaration
    Interrupt_register(INT_CANA0,&GUN1_CANISR);
    Interrupt_register(INT_CANB0,&GUN2_CANISR);
    Interrupt_register(INT_TIMER1,&Timer1_ISR);
    Interrupt_register(INT_XINT1, &Flow_Meter1);
    Interrupt_register(INT_XINT2, &Flow_Meter2);
    Interrupt_register(INT_WAKE, &wakeupISR);
    //Interrupt_register(ADC_INT_LINE, &ChargerADC);
    Interrupt_enable(INT_CANA0);
    DEVICE_DELAY_US(20000);
    Interrupt_enable(INT_CANB0);
    DEVICE_DELAY_US(20000);
    Interrupt_enable(INT_TIMER1);
    DEVICE_DELAY_US(20000);
    Interrupt_enable(INT_XINT1);
    DEVICE_DELAY_US(20000);
    Interrupt_enable(INT_XINT2);
    DEVICE_DELAY_US(20000);
    GPIO_enableInterrupt(GPIO_INT_XINT1);
    DEVICE_DELAY_US(20000);
    GPIO_enableInterrupt(GPIO_INT_XINT2);
    DEVICE_DELAY_US(20000);
    Interrupt_enable(INT_WAKE);
    DEVICE_DELAY_US(20000);
    // Pin Configuration (Init and intial Values at start)
    InitialiseChargerInPins();
    Interrupt_enableMaster();
    ChargerSetInitValues();


    //SETUP THE CHM,CTS MESSAGES THAT ARE CONSTANT FOR RNL
    setupChargeHandshake(&RNLRightGunSend.ChargeHandshake);
    setupChargeTimeStart(&RNLRightGunSend.ChargingTimeStart);
    setupChargeHandshake(&RNLLeftGunSend.ChargeHandshake);
    setupChargeTimeStart(&RNLLeftGunSend.ChargingTimeStart);

    // CHarger Initial Value assignment
    GunVarInitValues(&RightGun);
    GunVarInitValues(&LeftGun);

    ChargerData.ACContactorCoilFB = Off;
    ChargerData.ACContactorFB = Off;
    ChargerData.ELRFBFault =0;
    ChargerData.EMButtonFault =0;
    ChargerData.ACContactorFault = 0;
    ChargerData.SPDFault =0;
    ChargerData.ParmanantFault = 0;
    ChargerData.IoTResetRequest =0;
    ChargerData.TempFault = 0;
    ChargerData.DIOFaliureCount=0;
    ChargerData.CrossContactorFault = 0;
    ChargerData.CANFaultClear = 0;

    SetGunContactor(Gun1, Off,Off,Off);
    SetGunContactor(Gun2, Off,Off,Off);

    AC_EnergyMeterData.LNVoltage[0] = 0;
    AC_EnergyMeterData.LNVoltage[1] = 0;
    AC_EnergyMeterData.LNVoltage[2] = 0;

    GPIO_writePin(RAD_EN_MCU,0);
    ACEMModBusStatus.ACEMModBusStatusVal = 0xFFFF;
    IMDWriteRegisters(IMDID1,MODBUS_ADDRESS_STOP,IMDSTOPSIZE,IMDStartWrite);
    DEVICE_DELAY_US(10000);
    IMDWriteRegisters(IMDID2,MODBUS_ADDRESS_STOP,IMDSTOPSIZE,IMDStartWrite);
    DEVICE_DELAY_US(10000);

    //FLASH ENABLE PREFETCH
    Flash_enablePrefetch(FLASH0CTRL_BASE);
    FLASH_DELAY_CONFIG;

    //START THE TIMER 1
    CPUTimer_startTimer(CPUTIMER1_BASE);

    SysCtl_setWatchdogMode(SYSCTL_WD_MODE_INTERRUPT);
    SysCtl_setWatchdogPredivider(SYSCTL_WD_PREDIV_256);
    SysCtl_setWatchdogPrescaler(SYSCTL_WD_PRESCALE_64);
    SysCtl_serviceWatchdog();
    while(1)
      {

          if(ModBusStartTaskFlag == 1 && ModbusStartTask == 1)
           {
              AC_EnergyMeterData.LNVoltage[0] = 0;
              AC_EnergyMeterData.LNVoltage[1] = 0;
              AC_EnergyMeterData.LNVoltage[2] = 0;

              ReadACEMeterData(&AC_EnergyMeterData,&ACEMModBusStatus,MODBUS_TIMOUT_MS);
              ModbusFaultcheck(ACEMModBusStatus.StatusBits.LNVoltageStatus,&(FaultAndCounter.ACEMFault),&(FaultAndCounter.ACEMFaultCount));
              DEVICE_DELAY_US(10000);

              ReadDCEMeterData(DCEMID1,&DC_EnergyMeterData1,&DCEMModBusStatus1,MODBUS_TIMOUT_MS);
              //ModbusFaultcheck(DCEMModBusStatus1.StatusBits.PosEnergyStatus,&(FaultCounter.DCEM1Fault),&(FaultCounter.DCEM1FaultCount));
              if(DCEMModBusStatus1.StatusBits.PosEnergyStatus != MODBUS_STATUS_OK)
              {
                  FaultAndCounter.DCEM1FaultCount++;
                  if(FaultAndCounter.DCEM1FaultCount >=DCEM_COMM_FAULT_COUNT)
                  {
                      FaultAndCounter.DCEM1FaultCount = DCEM_COMM_FAULT_COUNT +1;
                      FaultAndCounter.DCEM1Fault =0;//1;
                  }
              }
              else
              {
                  RightGun.DCEMDeltaValue = DC_EnergyMeterData1.PosEnergy - RightGun.DCEMEnergyReading;
                  FaultAndCounter.DCEM1Fault =0;
                  FaultAndCounter.DCEM1FaultCount=0;
              }
              DEVICE_DELAY_US(10000);

              ReadDCEMeterData(DCEMID2,&DC_EnergyMeterData2,&DCEMModBusStatus2,MODBUS_TIMOUT_MS);
              //ModbusFaultcheck(DCEMModBusStatus2.StatusBits.PosEnergyStatus,&(FaultCounter.DCEM2Fault),&(FaultCounter.DCEM2FaultCount));
              if(DCEMModBusStatus2.StatusBits.PosEnergyStatus != MODBUS_STATUS_OK)
              {
                  FaultAndCounter.DCEM2FaultCount++;
                  if(FaultAndCounter.DCEM2FaultCount >=DCEM_COMM_FAULT_COUNT)
                  {
                      FaultAndCounter.DCEM2FaultCount = DCEM_COMM_FAULT_COUNT +1;
                      FaultAndCounter.DCEM2Fault =0;//1;
                  }
              }
              else
              {
                  LeftGun.DCEMDeltaValue = DC_EnergyMeterData2.PosEnergy - LeftGun.DCEMEnergyReading;
                  FaultAndCounter.DCEM2Fault =0;
                  FaultAndCounter.DCEM2FaultCount=0;
              }
              DEVICE_DELAY_US(10000);

              if(LeftGun.ChargingSpeed != FastCharging)
              {
                  ReadIMDData(IMDID1,&IMDData1,&IMDStatus1,MODBUS_TIMOUT_MS);
                  ModbusFaultcheck(IMDStatus1,&(FaultAndCounter.IMDM1Fault),&(FaultAndCounter.IMD1FaultCount));
                  DEVICE_DELAY_US(10000);
              }
              if(RightGun.ChargingSpeed != FastCharging)
              {
                  ReadIMDData(IMDID2,&IMDData2,&IMDStatus2,MODBUS_TIMOUT_MS);
                  ModbusFaultcheck(IMDStatus2,&(FaultAndCounter.IMDM2Fault),&(FaultAndCounter.IMD2FaultCount));
                  DEVICE_DELAY_US(10000);
              }


               if(RightGun.CANAPerFault == 0 &&  LeftGun.CANBPerFault == 0)
                   SendCMSCANMsg();

               ModBusStartTaskFlag = 0;

           }
      }
}
__interrupt void Timer1_ISR(void)
{
    EINT;
    static uint16_t TimerTick10ms = 0;
    TimerTick10ms++;
    static uint16_t I2CDataCount =  0;

    //++++++++++++++++++++ 10ms Task start ++++++++++++++++++++++
    if(RNLRightMSGCTRL == SendCST )//10ms
    {
        RNLRightGunSend.ChargingStopTime.ChargingStopTimeSignal.CF_Secc_Suspended = 1;
        SendtoRML_CST(RECTIFIER_GUN1,&RNLRightGunSend.ChargingStopTime);
    }
    if(RNLLeftMSGCTRL == SendCST )//10ms
    {
        RNLLeftGunSend.ChargingStopTime.ChargingStopTimeSignal.CF_Secc_Suspended = 1;
        SendtoRML_CST(RECTIFIER_GUN2,&RNLLeftGunSend.ChargingStopTime);
    }
    GetDigitalInputData(&I2Cstatus[0],&Data1[I2CDataCount],&Data2[I2CDataCount]);
    I2CDataCount++;
   if(I2CDataCount == 2)
   {
      if((Data1[0] == Data1[1])  && (Data2[0] == Data2[1]))
      {
          MuxIO1 = Data2[0];
          MuxIO2 = Data1[0];
         DIOStatus = DigitalInputData(&RightGun,&LeftGun,&ChargerData,MuxIO2,MuxIO1);
      }
      else
      {
          DIOStatus = DIOBouncingError;
      }
      I2CDataCount= 0;
   }
   if(BST_Received_Flag_G1 == 1)
   {
       GetEVDataBST(&RightGun,&RNLRightGun,&RNLRightMSGCTRL,&RNLRightGunSend,&RNLRightFlag);
       BST_Received_Flag_G1 = 0;
   }
   else if(BST_Received_Flag_G2 == 1)
   {
       GetEVDataBST(&LeftGun,&RNLLeftGun,&RNLLeftMSGCTRL,&RNLLeftGunSend,&RNLLeftFlag);
       BST_Received_Flag_G2 =0;
   }

    //++++++++++++++++++++ 10ms Task  End++++++++++++++++++++++

   //++++++++++++++++++++ 50ms Task  Start++++++++++++++++++++++
if(TimerTick10ms % 5 == 0)
{
   if(RNLRightMSGCTRL == SendCCS )//10ms
   {
       RNLRightGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutVolt = RightGun.VseccMeasuredVoltage/FACTOR_MAXVOLT;
        if(RightGun.VseccMeasuredCurrent < 400)
        {
            RNLRightGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutCurrentOver400A = 0;
            RNLRightGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutCurrent = (400 - RightGun.VseccMeasuredCurrent)/FACTOR_MAXCURR;
        }
        else
        {
            RNLRightGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutCurrentOver400A = 1;
            RNLRightGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutCurrent = ( RightGun.VseccMeasuredCurrent +  400)/FACTOR_MAXCURR;
        }
        RNLRightGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_MaxPowerLimit = (uint16_t) (RightGun.VseccMaxPower / FACTOR_POWERMAXLIMIT) ; // Check Factor
        RNLRightGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CF_Secc_EvseSuspended = 0x1;// might need this parameter to retry
        RNLRightGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvsseIsolationStatus = RightGun.IsolationState;
        RNLRightGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseMaxCurr_IsUse = 0x1; // might need to change basaed on testing , might require processing
        RNLRightGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseMaxCurr_A = ChargerMaxCurrent;// 0 to 1000 range and factor multiple is 1
        SendtoRML_CCS(RECTIFIER_GUN1, &RNLRightGunSend.ChargerChargingStatus);
   }
   if(RNLLeftMSGCTRL == SendCCS )//10ms
      {
          RNLLeftGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutVolt = LeftGun.VseccMeasuredVoltage/FACTOR_MAXVOLT;
           if(LeftGun.VseccMeasuredCurrent < 400)
           {
               RNLLeftGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutCurrentOver400A = 0;
               RNLLeftGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutCurrent = (400 - LeftGun.VseccMeasuredCurrent)/FACTOR_MAXCURR;
           }
           else
           {
               RNLLeftGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutCurrentOver400A = 1;
               RNLLeftGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseOutCurrent = ( LeftGun.VseccMeasuredCurrent +  400)/FACTOR_MAXCURR;
           }
           RNLLeftGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_MaxPowerLimit = (uint16_t) (LeftGun.VseccMaxPower / FACTOR_POWERMAXLIMIT) ; // Check Factor
           RNLLeftGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CF_Secc_EvseSuspended = 0x1;// might need this parameter to retry
           RNLLeftGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvsseIsolationStatus = LeftGun.IsolationState;
           RNLLeftGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseMaxCurr_IsUse = 0x1; // might need to change basaed on testing , might require processing
           RNLLeftGunSend.ChargerChargingStatus.ChargerChargingStatusSignal.CR_Secc_EvseMaxCurr_A = ChargerMaxCurrent;// 0 to 1000 range and factor multiple is 1
           SendtoRML_CCS(RECTIFIER_GUN2, &RNLLeftGunSend.ChargerChargingStatus);
      }
   if(BCL_Received_Flag_G1 == 1)
   {
       GetEVDataBCL(&RightGun,&RNLRightGun,&RNLRightMSGCTRL,&RNLRightGunSend,&RNLRightFlag);
       BCL_Received_Flag_G1 = 0;
   }
   else if(BCL_Received_Flag_G2 == 1)
   {
       GetEVDataBCL(&LeftGun,&RNLLeftGun,&RNLLeftMSGCTRL,&RNLLeftGunSend,&RNLLeftFlag);
       BCL_Received_Flag_G2 = 0;
   }

}
//++++++++++++++++++++ 50ms Task  End++++++++++++++++++++++

    //++++++++++++++++++++ 250ms Task Start  ++++++++++++++++++++++
    if(TimerTick10ms % ChargerSECCTaskCount == 0)//250MS
       {
            SysCtl_enableWatchdog();
            startTime = CPUTimer_getTimerCount(CPUTIMER0_BASE);
            GPIO_togglePin(23);
            SCI_BreakDtect();
            if(FaultAndCounter.ACEMFault == 1 && FaultAndCounter.DCEM1Fault ==1 && FaultAndCounter.DCEM2Fault == 1)
            {
                setModBusUART();
            }
            RectAliveCommand(Gun1);
            RectAliveCommand(Gun2);

            GetACEMData(&ChargerData, &AC_EnergyMeterData);

            GetDCEMValue(&RightGun,&DC_EnergyMeterData1);
            GetDCEMValue(&LeftGun,&DC_EnergyMeterData2);
            GetIMDData(&RightGun, IMDData1);
            GetIMDData(&LeftGun, IMDData2);
            GetChargerADCData();
            GetRectStatusInfo(&RightGun,TonheRightGun);
            GetRectStatusInfo(&LeftGun,TonheLeftGun);
            //GET DATA FROM RNL SECC
            int8_t RNLStatusRight = GetEVDataRNL(&RightGun,&RNLRightGun,&RNLRightMSGCTRL,&RNLRightGunSend,&RNLRightFlag);

            int8_t RNLStatusLeft = GetEVDataRNL(&LeftGun,&RNLLeftGun,&RNLLeftMSGCTRL,&RNLLeftGunSend,&RNLLeftFlag);

            /*Test Code Start Here */
           //BenchTestCode();

            if((uint16_t)RightGun.EVConnectionState  >= 4  || (uint16_t)RightGun.EVChargingState >= 5)
            {
                RightGun.EVConnectionState = EV_DISCONNECTED;
            }
            if((uint16_t)LeftGun.EVConnectionState  >= 4  || (uint16_t)LeftGun.EVChargingState >= 5)
            {
                LeftGun.EVConnectionState = EV_DISCONNECTED;
            }

            ChargerApp_FCN01ACFaultFlag(&ChargerData);

            if(DIOStatus == DIOSuccess)
            {
                ChargerData.DIOFaliureCount=0;
                ChargerFault(&ChargerData,RightGun, LeftGun, &(FaultAndCounter.ELRfaultCounter), AC_EnergyMeterData.LNVoltage[0],
                             ACEMModBusStatus.StatusBits.LNVoltageStatus);
            }
            else
            {
                ChargerData.DIOFaliureCount++;//**************If failt mulitple time in single shot raise a charger Temp Fault;
            }

            // ChargerApp_FCN01ACFaultGlobalFlag(&ChargerData); delete this declaration and definfation
            if(ChargerData.DIOFaliureCount > 100)
            {
                ChargerData.DIOFaliureCount= 0;
                ChargerData.CMSDIOInFault = 1;
            }

            CANPerfaultcheck(&(RightGun.CANAFaultCounter), &(RightGun.CANAPerFault),Gun1);
            CANPerfaultcheck(&(LeftGun.CANBFaultCounter), &(LeftGun.CANBPerFault),Gun2);

            ChargerPerFault(RightGun.CANAPerFault, LeftGun.CANBPerFault, &ChargerData);
            ChargerTempFault(FaultAndCounter.ACEMFault, &ChargerData);
            ChargerData.ChargerFault =  ChargerData.ParmanantFault | ChargerData.TempFault;

         /*  ChargerData.ChargerFault = 0;
           ChargerData.ParmanantFault = 0;
           ChargerData.TempFault =0;*/

            if(ChargerData.ChargerFault == 0 && ChargerData.ParmanantFault == 0 && ChargerData.TempFault ==0)
            {
                if(DIOStatus == DIOSuccess)
                {
                    ChargerApp_FCN02DCGUNFaultCheck(&RightGun);
                    ChargerApp_FCN02DCGUNFaultCheck(&LeftGun);
                    ChargerData.DIOFaliureCount =0;

                    GunPerFault(&RightGun);
                    GunTempFault(FaultAndCounter.DCEM1Fault, FaultAndCounter.IMDM1Fault, &RightGun, &(RightGun.IsolationState));
                    RightGun.GunFault = RightGun.TempFault | RightGun.ParmanantFault;

                    GunPerFault(&LeftGun);
                    GunTempFault(FaultAndCounter.DCEM2Fault, FaultAndCounter.IMDM2Fault, &LeftGun, &(LeftGun.IsolationState));
                    LeftGun.GunFault = LeftGun.TempFault | LeftGun.ParmanantFault;
                }

                if(RightGun.EVConnectionState == EV_DISCONNECTED && LeftGun.EVConnectionState == EV_DISCONNECTED)
                {
                    ACDisconnect++;
                    if(ACDisconnect == 12)
                    {
                        ACDisconnect =0;
                        ChargerData.ACContactorDrive= Off;
                    }
                }
                else
                {
                      ChargerData.ACContactorDrive= On;
                }
                //uncomment for testing
              //  PrimaryGun->GunFault ==0 && PrimaryGun->ParmanantFault == 0 && PrimaryGun->TempFault == 0)
                Gunroutin(&RightGun,&LeftGun, DIOStatus,&RNLRightGun);
                Gunroutin(&LeftGun,&RightGun, DIOStatus,&RNLLeftGun);

            }
            else
            {
                ChargerData.ACContactorDrive =  Off;                        // CHANGE added
                RightGun.StopCharge=1;
                LeftGun.StopCharge=1;
                ChargerApp_FCN00StopChrage00(&RightGun);
                ChargerApp_FCN00StopChrage00(&LeftGun);
            }

            if(ChargerData.IoTResetRequest ==1)
            {
                GunOperativeStatus(&(RightGun.EVConnectionState), &(RightGun.PECCOperationState));
                GunOperativeStatus(&(LeftGun.EVConnectionState), &(LeftGun.PECCOperationState));
                if( RightGun.PECCOperationState ==  OPERATIONAL_STATUS_INOPERATIVE && LeftGun.PECCOperationState ==  OPERATIONAL_STATUS_INOPERATIVE)
                {
                    SetACContactor(Off);
                    DEVICE_DELAY_US(5000000);
                    ChargerData.IoTResetRequest =0;
                    RightGun.PECCOperationState =  OPERATIONAL_STATUS_OPERATIVE;
                    LeftGun.PECCOperationState =  OPERATIONAL_STATUS_OPERATIVE;
                }
            }

            ChargerApp_FCN06CrossContactorVSECCOut(&RightGun,&LeftGun,&ChargerData);

            if(RightGun.ChargingSpeed ==  FastCharging)
            {
                Charger_FastChargeMSG(&RightGun, &LeftGun);
            }
            else if(LeftGun.ChargingSpeed ==  FastCharging)
            {
                Charger_FastChargeMSG(&LeftGun, &RightGun);
            }
            else
            {
                Charger_SlowChargeMsg(&RightGun);
                Charger_SlowChargeMsg(&LeftGun);
            }

            if(RightGun.EVChargingState ==  CHARGING_CHARGE || LeftGun.EVChargingState ==  CHARGING_CHARGE)
            {
                ExhaustFan((ContactorWrite)On);
                CoolingFan((ContactorWrite)On);
                PumpPower((ContactorWrite)On);
            }
            else
            {
                ExhaustFan((ContactorWrite)Off);
                CoolingFan((ContactorWrite)Off);
                PumpPower((ContactorWrite)Off);
            }
            if(RightGun.EVConnectionState >= (evConnectionState_t)1 && LeftGun.EVConnectionState >= (evConnectionState_t)1 && ChargerData.CrossContactorDrive == On)
            {
                if(RightGun.ChargingSpeed ==  FastCharging)
                {
                    if(LeftGun.PositiveContactorDrive == On | LeftGun.NegativeContactorDrive == On | LeftGun.PrechargeContactorDrive == On )
                    {
                        ChargerData.powerallocfault = 1;
                    }
                    LeftGun.PositiveContactorDrive = Off;
                    LeftGun.NegativeContactorDrive = Off;
                    LeftGun.PrechargeContactorDrive =  Off;
                }
                if(LeftGun.ChargingSpeed ==  FastCharging)
                {
                    if(RightGun.PositiveContactorDrive == On | RightGun.NegativeContactorDrive == On | RightGun.PrechargeContactorDrive == On )
                    {
                        ChargerData.powerallocfault = 1;
                    }
                    RightGun.PositiveContactorDrive = Off;
                    RightGun.NegativeContactorDrive = Off;
                    RightGun.PrechargeContactorDrive =  Off;
                }
            }
            if(RightGun.EVConnectionState > (evConnectionState_t)1 && LeftGun.EVConnectionState >(evConnectionState_t)1)
            {
                ChargerData.CrossContactorDrive = Off;
                ChargerData.powerallocfault = 2;
            }

            if(ChargerData.VoltageLimitFault == 0)
                StationLED(On);
            else
                StationLED(Off);

            SendACMeterVoltageInputCANData(ChargerData);
            SetACContactor(ChargerData.ACContactorDrive);
            SetCrossContactor(ChargerData.CrossContactorDrive);
            StopLED(Gun1,(PowerEnable)RightGun.StopChargeLED);
            StopLED(Gun2,(PowerEnable)LeftGun.StopChargeLED);
            SetGunContactor(Gun1, RightGun.PositiveContactorDrive,
                            RightGun.NegativeContactorDrive,
                            RightGun.PrechargeContactorDrive);

            SetGunContactor(Gun2, LeftGun.PositiveContactorDrive,
                         LeftGun.NegativeContactorDrive,
                         LeftGun.PrechargeContactorDrive);

            // Change made to turn of Cross contactor in case of fault as it goes to no charging in stop charge function

            SendFaultCANData(RightGun,LeftGun,ChargerData,FaultAndCounter);

            SetRectVoltageCurrent(Gun1,&RightGun);  // Get Ack from Rectifier
            RectifierOnOffCtrl(Gun1,RightGun.RectState);    // Get Ack from Rectifier
            SendToRNL(Gun1,&RightGun,&RNLRightMSGCTRL,&RNLRightGunSend);

            SetRectVoltageCurrent(Gun2,&LeftGun);
            RectifierOnOffCtrl(Gun2,LeftGun.RectState);
            //will uncomment after testing completes
            SendToRNL(Gun2,&LeftGun,&RNLLeftMSGCTRL,&RNLLeftGunSend);

            StopChargingGun(RightGun.StopCharge, LeftGun.StopCharge,&RNLRightMSGCTRL,&RNLRightGunSend,&RNLLeftMSGCTRL,&RNLLeftGunSend);

            SendGUNStatus(RightGun.CodeGunStatus,LeftGun.CodeGunStatus,TonheRCount,TonheLCount,ChargerData.DIOFaliureCount);
           // RightGun.StopCharge =1;
            //SendCMSRNLMSGTOVSECC();

            SysCtl_serviceWatchdog();
        }

    //++++++++++++++++++++ 250ms Task End ++++++++++++++++++++++

    // ++++++++++++++++++1000ms task start
    if(TimerTick10ms % ModbusAlgoTimeCount == 0)
    {
        RightGun.FlowRate =  RightGun.FlowPWMEdgeCount/7.5;
        RightGun.FlowPWMEdgeCount= 0;
        LeftGun.FlowRate =  LeftGun.FlowPWMEdgeCount/7.5;
        LeftGun.FlowPWMEdgeCount = 0;
        ModBusStartTaskFlag =1;
    }
    if(TimerTick10ms == 1000)
    {
        ModbusStartTask = 1;
    }
    DINT;
}
void SendCMSCANMsg()
{
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendACMeterVoltageInputCANData(ChargerData);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendACMeterCurrentInputCANData(ChargerData);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendACMeterEnergyInputCANData(ChargerData);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendACMeterInputCANData(AC_EnergyMeterData, ChargerData);

    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendDCMeterVoltAmpCANData(DC_EnergyMeterData1, 0);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendDCMeterVoltAmpCANData(DC_EnergyMeterData2, 1);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendDCMeterPowerCANData(DC_EnergyMeterData1, DC_EnergyMeterData2);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendDCMeterPosEnergyCANData(RightGun, LeftGun);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendIMDMeterCANData(IMDData1,IMDData2);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SENDDiffDCEMeter(RightGun,LeftGun);

    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendCMSTempCANData(&RightGun,&LeftGun,&ChargerData);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendCMSFlowrate(RightGun.FlowRate, LeftGun.FlowRate);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendCMSDigitalInputCANData(MuxIO1,MuxIO2);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendGunData(RightGun,LeftGun,ChargerData);

    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendCANStopChargeLED(RightGun.StopChargeUserButton, LeftGun.StopChargeUserButton);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendCMSIOTStartStop(RightGun, LeftGun);

    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendCMSVersionCANData();

    // Communication
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendCMSModbusMeterStatus(&ACEMModBusStatus,&DCEMModBusStatus1,&DCEMModBusStatus2,&IMDStatus1,&IMDStatus2);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendCMS_TonheStatus(Tonhe_Status);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendCMSI2CStatus(DIOStatus, I2Cstatus);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendCMS_TotalMeasuredAmpVol(RightGun, LeftGun);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendVehicleIDRight(RightGun.VehicleID);
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
    SendVehicleIDLeft(LeftGun.VehicleID);
}


void SendCMSRNLMSGTOVSECC()
{
    if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageVehicleStatus(&RightGun,Right,1);
   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageVehicleStatus(&LeftGun,Left,2);

   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessagePECCStatus1(&RightGun,RightPS1,1);
   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessagePECCStatus1(&LeftGun,LeftPS1,2);

   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessagePECCLimits1(&RightGun,RightPL1,1);
   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessagePECCLimits1(&LeftGun,LeftPL1,2);

   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessagePECCLimits2(&RightGun,RightPL2,1);
   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessagePECCLimits2(&LeftGun,LeftPL2,2);

   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageChargeSessionInfo1(&RightGun,RightCSI1,1);
   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageChargeSessionInfo1(&LeftGun,LeftCSI1,2);


   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageChargeSessionInfo2(&RightGun,RightCSI2,1);
   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageChargeSessionInfo2(&LeftGun,LeftCSI2,2);

   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageChargeSessionInfo3(&RightGun,RightCSI3,1);
   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageChargeSessionInfo3(&LeftGun,LeftCSI3,2);


   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessagePECCStatus2(&RightGun,&LeftGun,RightPS2,1);
   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessagePECCStatus2(&LeftGun,&RightGun,LeftPS2,2);

   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageStopCharge(&RightGun,1);
   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageStopCharge(&LeftGun,2);

   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageReset(&RightGun,1);
   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageReset(&LeftGun,2);


   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageVehicleId(&RightGun,1);
   if(!CAN_WaitTxComplete(RECTIFIER_GUN1))return;
   SendRNLMessageVehicleId(&LeftGun,2);
}

void GetChargerADCData()
{
    ADC_forceMultipleSOC(CHARGER_ADC_BASE, 0);
    DEVICE_DELAY_US(1000);
    float *AnalogPointer;
    uint8_t i=0;
    AnalogPointer = (float *)&(ChargerAnalogReadVolt);
       for (i = 0; i < ADCA_TOTAL_CH; i++)
       {
           *AnalogPointer = (ADC_readResult(ADCARESULT_BASE, (ADC_SOCNumber)i))*3300.0f/4095.0f;
           AnalogPointer++;
       }
     GetAnalogInputData(&RightGun,&LeftGun, &ChargerData,&ChargerAnalogReadVolt);
}
void IMDChecks() // Changes made to know imd resistance failure in ideal case
{
    static uint8_t RightStartTempIMD = 1;
    static uint8_t LeftStartTempIMD = 1;
    if(LeftGun.ChargingSpeed !=  FastCharging)
    {
        if(RightStartTempIMD == 1)
        {
            IMDWriteRegisters(IMDID1, MODBUS_ADDRESS_STOP, 1,1);
            //RightStartTempIMD = 2;
            DEVICE_DELAY_US(10000);
        }


        ReadIMDData(IMDID1,&IMDData1,&IMDStatus1,MODBUS_TIMOUT_MS);
        if(IMDStatus1 != MODBUS_STATUS_OK)
        {
            FaultAndCounter.IMD1FaultCount++;
            if(FaultAndCounter.IMD1FaultCount >=IMD_COMM_FAULT_COUNT)
            {
                 FaultAndCounter.IMD1FaultCount = IMD_COMM_FAULT_COUNT+1;
                 FaultAndCounter.IMDM1Fault =1;

            }
         }
        else
        {
            FaultAndCounter.IMD1FaultCount =0;
            FaultAndCounter.IMDM1Fault =0;
            RightGun.InsulationResistance = 2000000;
        }
    }
    else
    {
        RightStartTempIMD = 1;
        FaultAndCounter.IMDM1Fault =0;
        IMDWriteRegisters(IMDID1, MODBUS_ADDRESS_STOP,1,0);
        DEVICE_DELAY_US(10000);

    }

     DEVICE_DELAY_US(10000);
     if(RightGun.ChargingSpeed != FastCharging)
     {
         if(LeftStartTempIMD == 1)
         {
             IMDWriteRegisters(IMDID2, MODBUS_ADDRESS_STOP, 1,1);
             //LeftStartTempIMD = 2;
             DEVICE_DELAY_US(10000);
         }
         // turn on right side IMD just for one time
        ReadIMDData(IMDID2,&IMDData2,&IMDStatus2,MODBUS_TIMOUT_MS);
        if(IMDStatus2 != MODBUS_STATUS_OK)
        {
          FaultAndCounter.IMD2FaultCount++;
          if(FaultAndCounter.IMD2FaultCount >=IMD_COMM_FAULT_COUNT)
          {
              FaultAndCounter.IMD2FaultCount  = IMD_COMM_FAULT_COUNT+1;
              FaultAndCounter.IMDM2Fault =1;
          }
        }
        else
        {
         FaultAndCounter.IMD2FaultCount =0;
         FaultAndCounter.IMDM2Fault =0;
         LeftGun.InsulationResistance = 2000000;
        }
     }
     else
     {
         LeftStartTempIMD = 1;
         FaultAndCounter.IMDM2Fault =0;
         IMDWriteRegisters(IMDID2, MODBUS_ADDRESS_STOP, 1,0);
     }
}
void BenchTestCode()
{
    ChargerData.PhaseVoltage[0]= 230;
    ChargerData.PhaseVoltage[1]= 230;
    ChargerData.PhaseVoltage[2]= 230;
    ChargerData.PhaseCurrent[0] = 0;
    ChargerData.PhaseCurrent[1] = 0;
    ChargerData.PhaseCurrent[2] = 0;
    ChargerData.PowerFactor = 1;
    ChargerData.PhasePower[0] = 0;
    ChargerData.PhasePower[1] = 0;
    ChargerData.PhasePower[2] = 0;
    ChargerData.frequency = 50;
    //ChargerData.EmergencyButton =  TestEmergency;
    ChargerData.SPDFault = 0;
    ChargerData.ACContactorDrive = Off;

    ChargerData.SmokeSensor = 1;
    ChargerData.ELRFB = 1;
    ChargerData.SPDFB=0;
    ChargerData.ACContactorCoilFB = (ContactorWrite)(ChargerData.ACContactorFB);
    FaultAndCounter.ACEMFault = 0;
    ChargerData.CrossContactorPositiveFB =  ChargerData.CrossContactorDrive;
    ChargerData.CrossContactorNegativeFB =  ChargerData.CrossContactorDrive;
    RightGun.PositiveContactorFB =  (ContactorWrite)RightGun.PositiveContactorDrive;
    RightGun.NegativeContactorFB = (ContactorWrite)RightGun.NegativeContactorDrive;
    RightGun.InsulationResistance =  2000000;

    LeftGun.PositiveContactorFB = (ContactorWrite) LeftGun.PositiveContactorDrive;
    LeftGun.NegativeContactorFB = (ContactorWrite)LeftGun.NegativeContactorDrive;
    LeftGun.InsulationResistance =  2000000;

    LeftGun.IMDFault = (IMDFBStatus)1;
    RightGun.IMDFault = (IMDFBStatus)1;

}
__interrupt void wakeupISR(void)
{
    uint64_t Data = 0;
    CANMSGObjectSetup(RECTIFIER_GUN1, RESET_MAILBOX, CAN_Reset, STDFrame,
                      CANOBJECTTYPETX, 0, CAN_MSG_OBJ_NO_FLAGS);
    CANSendMessage(RECTIFIER_GUN1, RESET_MAILBOX, (uint64_t *)&Data,8);
    SysCtl_setWatchdogMode(SYSCTL_WD_MODE_RESET);
    SysCtl_resetDevice();
}
uint8_t CAN_WaitTxComplete(uint32_t base)
{
    uint32_t timeout = CAN_TX_TIMEOUT_MS;

    while (CAN_getTxRequests(base))
    {
        if (timeout-- == 0)
        {
            return 0;   // timeout
        }
        DEVICE_DELAY_US(1000); // 1 ms
    }
    return 1;            // TX completed
}


