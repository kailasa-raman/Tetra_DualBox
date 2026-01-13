/*
 * InterruptHandling.c
 *
 *  Created on: Feb 24, 2025
 *  Owner  : Tetra Automotives Pvt LTD.
 */
#ifndef DRIVERLIB_H
    #include "driverlib.h"
#endif

#ifndef MCUSPECIFIC_H
#include "MCUSpecific.h"
#endif

#ifndef DEVICE_H
    #include "device.h"
#endif

#include "Charger.h"
#include "ChargerApp.h"


extern uint8_t count;

#define CANATONHEMESG     0
#define CANBTONHEMESG     1

//++++++++++++EXTERN STRUCT, ENUM AND INTERRUPT FUNCITONS FROM MAIN.C
extern CMSReceiveFromTonheStruct TonheRightGun[2];
extern CMSReceiveFromTonheStruct TonheLeftGun[2];
extern ReceiveFromRMLStruct RNLRightGun;
extern ReceiveFromRMLStruct RNLLeftGun;
extern RNLFlagEnum RNLRightFlag;
extern RNLFlagEnum RNLLeftFlag;
extern RNLDualStruct SendRight;
extern RNLDualStruct SendLeft;



extern GunStruct RightGun;
extern GunStruct LeftGun;
extern ChargerStruct ChargerData;

bool CANstatus;
extern __interrupt void GUN1_CANISR(void);
extern __interrupt void GUN2_CANISR(void);
extern __interrupt void Flow_Meter1(void);
extern __interrupt void Flow_Meter2(void);

extern uint16_t TonheRCount =0;
extern uint16_t TonheLCount =0;
uint8_t GunNumber1=  1;
uint8_t GunNumber2 = 2;

/**
 * This CAN interrupt Function is to process the message based on each mailbox number when CAN interrupt occurs on Right Gun
 *
 * @return none
 */
__interrupt void GUN1_CANISR(void)
{

    uint32_t mailbox = 0;
     uint32_t can_id = 0;
    uint16_t Rxdata[8]; // Max 8 bytes = 4 words (16-bit each)
    uint8_t rxData[8];//rnl receive
    CANFRAMETYPE frameType;


    mailbox = CAN_getInterruptCause(CANA_BASE);
    CAN_getStatus(CANA_BASE) & CAN_STATUS_LEC_MSK;

    if(mailbox != 0)
    {
        RightGun.CANTempFault =0;
        RightGun.CANAFaultCounter=0;
        switch (mailbox)
        {
        case CANOBJ_RECTIFIER:

            CANstatus= CAN_readMessageWithID(CANA_BASE, CANOBJ_RECTIFIER, &frameType, &can_id,(uint16_t *)Rxdata);
            if (CANstatus == 1)
            {
                TonheProcessRxCANMsg(can_id,Rxdata,8,TonheRightGun,CANATONHEMESG);
            }
            TonheRCount++;
            break;
        case CANOBJ_RMC_RX://RNL MESSAGES
            CANstatus= CAN_readMessageWithID(CANA_BASE, CANOBJ_RMC_RX, &frameType, &can_id,(uint16_t *)rxData);
            if (CANstatus == 1)
            {
                RNL_ProcessCANMsg(can_id,rxData,8,&RNLRightGun,&RNLRightFlag,GunNumber1,&SendRight);
            }
            break;
        case BOOT_RX_MAILBOX:
            CANstatus= CAN_readMessageWithID(CANA_BASE, BOOT_RX_MAILBOX, &frameType, &can_id,(uint16_t *) Rxdata);
            if(CANstatus == 1)
            {
                if(can_id == BOOT_APPMSG_ID && Rxdata[0]  == 0x5A && Rxdata[2]  == 0x5A && Rxdata[4]  == 0x5A && Rxdata[6]  == 0x5A)
                {
                    BootISR();
                }
            }
            break;
        case CANOBJ_CMS_RX:
            CANstatus= CAN_readMessageWithID(CANA_BASE, CANOBJ_CMS_RX, &frameType, &can_id, (uint16_t *)Rxdata);
            if(CANstatus == 1)
            {
                IOT_ProcessCANMsg(can_id,Rxdata,8,&RightGun,&LeftGun,&ChargerData);
            }
            break;

        }
    }
    CAN_clearInterruptStatus(CANA_BASE, mailbox);
    CAN_clearGlobalInterruptStatus(CANA_BASE, CAN_GLOBAL_INT_CANINT0);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}
/**
 * This CAN interrupt Function is to process the message when CAN interrupt occurs on Left Gun.
 *
 * @return none
 */
__interrupt void GUN2_CANISR(void)
{
    uint32_t mailbox = 0;
     uint32_t can_id = 0;
    uint16_t Rxdata[8]; // Max 8 bytes = 4 words (16-bit each)
    uint8_t rxData[8];//rnl receive
    CANFRAMETYPE frameType;

    mailbox = CAN_getInterruptCause(CANB_BASE);
    CAN_getStatus(CANB_BASE) & CAN_STATUS_LEC_MSK;
    if(mailbox != 0)
    {
        LeftGun.CANTempFault =0;
        LeftGun.CANBFaultCounter=0;
        switch (mailbox)
         {
            case CANOBJ_RECTIFIER:
                CANstatus= CANreadMessageWithID(CANB_BASE,CANOBJ_RECTIFIER, &frameType, &can_id, (uint16_t *)Rxdata);
                if (CANstatus == 1)
                {
                    TonheProcessRxCANMsg(can_id,Rxdata,8,TonheLeftGun,CANBTONHEMESG);
                }
                TonheLCount++;
                break;

            case CANOBJ_RMC_RX://RNL MESSAGES
                CANstatus= CANreadMessageWithID(CANB_BASE, CANOBJ_RMC_RX, &frameType, &can_id,(uint16_t *)rxData);
                if (CANstatus == 1)
                {
                    RNL_ProcessCANMsg(can_id,rxData,8,&RNLLeftGun,&RNLLeftFlag,GunNumber2,&SendLeft);
                }
                break;
          }
    }
    CANclearInterruptStatus(CANB_BASE, mailbox);
    CANclearGlobalInterruptStatus(CANB_BASE, CAN_GLOBAL_INT_CANINT0);
    InterruptclearACKGroup(INTERRUPT_ACK_GROUP9);
}
/**
 * This GPIO interrupt Function is to increment the right gun flow meter count
 *
 * @return none
 */
__interrupt void Flow_Meter1(void)
{
    RightGun.FlowPWMEdgeCount++;
    InterruptclearACKGroup(INTERRUPT_ACK_GROUP1);
}
/**
 * This GPIO interrupt Function is to increment the left gun flow meter count
 *
 * @return none
 */
__interrupt void Flow_Meter2(void)
{
    LeftGun.FlowPWMEdgeCount++;
    InterruptclearACKGroup(INTERRUPT_ACK_GROUP1);
}

