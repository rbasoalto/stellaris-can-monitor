//*****************************************************************************
//
// simple_rx.c - Example demonstrating simple CAN message reception.
//
// Copyright (c) 2010-2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 9453 of the Stellaris Firmware Development Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "driverlib/pin_map.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "uartstdio.h"

#include "spilcd.h"

void InitConsole(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  UARTStdioInit(0);
}

void CANIntHandler(void) {
  unsigned long status_flags;
  status_flags = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

  if (status_flags == CAN_INT_INTID_STATUS) {
    status_flags = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
    // TODO: process the error?
  } else if (status_flags == 1) {
    // TODO: read message and process it
    CANIntClear(CAN0_BASE, 1);

  } else if (status_flags == 2) {
    // TODO: read message and process it
    CANIntClear(CAN0_BASE, 2);

  } else if (status_flags == 3) {
    // TODO: read message and process it
    CANIntClear(CAN0_BASE, 3);

  } else if (status_flags == 4) {
    // TODO: read message and process it
    CANIntClear(CAN0_BASE, 4);

  } else {
    //
    // Spurious interrupt handling can go here.
    //
  }
}

//*****************************************************************************
//
// Configure the CAN and enter a loop to receive CAN messages.
//
//*****************************************************************************
int main(void) {
  tCANMsgObject sCANMessage;
  unsigned char ucMsgData[8];

  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_16MHZ);

  InitConsole();

  SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

  GPIOPinConfigure(GPIO_PE4_CAN0RX);
  GPIOPinConfigure(GPIO_PE5_CAN0TX);

  GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

  CANInit(CAN0_BASE);

  CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);

  CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

  IntEnable(INT_CAN0);

  CANEnable(CAN0_BASE);

  //
  // Initialize a message object to be used for receiving CAN messages with
  // any CAN ID.  In order to receive any CAN ID, the ID and mask must both
  // be set to 0, and the ID filter enabled.
  //
  sCANMessage.ui32MsgID = 0x398;     // CAN msg ID - 0 for any
  sCANMessage.ui32MsgIDMask = 0x7ff; // mask is 0 for any ID
  sCANMessage.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
  sCANMessage.ui32MsgLen = 16; // allow up to 8 bytes

  //
  // Now load the message object into the CAN peripheral.  Once loaded the
  // CAN will receive any message on the bus, and an interrupt will occur.
  // Use message object 1 for receiving messages (this is not the same as
  // the CAN ID which can be any value in this example).
  //
  CANMessageSet(CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_RX);

  UARTprintf("Ready to start receiving CAN stuff...\n");

  lcd_port_setup();
  lcd_init();

  //
  // Enter loop to process received messages.  This loop just checks a flag
  // that is set by the interrupt handler, and if set it reads out the
  // message and displays the contents.  This is not a robust method for
  // processing incoming CAN data and can only handle one messages at a time.
  // If many messages are being received close together, then some messages
  // may be dropped.  In a real application, some other method should be used
  // for queuing received messages in a way to ensure they are not lost.  You
  // can also make use of CAN FIFO mode which will allow messages to be
  // buffered before they are processed.
  //
  for (;;) {

    if (true) {
      uint8_t uIdx;
      //
      // Reuse the same message object that was used earlier to configure
      // the CAN for receiving messages.  A buffer for storing the
      // received data must also be provided, so set the buffer pointer
      // within the message object.
      //
      sCANMessage.pui8MsgData = ucMsgData;

      //
      // Read the message from the CAN.  Message object number 1 is used
      // (which is not the same thing as CAN ID).  The interrupt clearing
      // flag is not set because this interrupt was already cleared in
      // the interrupt handler.
      //
      CANMessageGet(CAN0_BASE, 1, &sCANMessage, 0);

      //
      // Check to see if there is an indication that some messages were
      // lost.
      //
      if (sCANMessage.ui32Flags & MSG_OBJ_DATA_LOST) {
        UARTprintf("CAN message loss detected\n");
      }

      //
      // Print out the contents of the message that was received.
      //
      UARTprintf("Msg ID=0x%08X len=%u data=0x", sCANMessage.ui32MsgID,
                 sCANMessage.ui32MsgLen);
      for (uIdx = 0; uIdx < sCANMessage.ui32MsgLen; uIdx++) {
        UARTprintf("%02X ", ucMsgData[uIdx]);
      }
    }
  }

  //
  // Return no errors
  //
  return (0);
}
