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
#include <stdio.h>

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

#include "main.h"

// network order 16bit int to host 16bit

uint16_t ntohs(uint16_t x) {
  return ((x&0x00FF)<<8)+((x&0xFF00)>>8);
}

// Data buffers

wheel_t wheel_a_data;
wheel_t wheel_b_data;
engine_t engine_data;
fuel_t fuel_data;

tCANMsgObject sCANMessage;

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
  } else if (status_flags == 1) { // Wheel A
                                  // TODO: read message and process it
    // CANIntClear(CAN0_BASE, 1);
    sCANMessage.pui8MsgData = (uint8_t *)&wheel_a_data;
    CANMessageGet(CAN0_BASE, 1, &sCANMessage, 1);
  } else if (status_flags == 2) { // Wheel B
                                  // TODO: read message and process it
    // CANIntClear(CAN0_BASE, 2);
    sCANMessage.pui8MsgData = (uint8_t *)&wheel_b_data;
    CANMessageGet(CAN0_BASE, 2, &sCANMessage, 1);
  } else if (status_flags == 3) { // Engine
                                  // TODO: read message and process it
    // CANIntClear(CAN0_BASE, 3);
    sCANMessage.pui8MsgData = (uint8_t *)&engine_data;
    CANMessageGet(CAN0_BASE, 3, &sCANMessage, 1);
  } else if (status_flags == 4) { // Fuel
                                  // TODO: read message and process it
    // CANIntClear(CAN0_BASE, 4);
    sCANMessage.pui8MsgData = (uint8_t *)&fuel_data;
    CANMessageGet(CAN0_BASE, 4, &sCANMessage, 1);
  } else {
    //
    // Spurious interrupt handling can go here.
    //
  }
}

int main(void) {
  char txtBuffer[32];

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
  sCANMessage.ui32MsgIDMask = 0x7ff; // mask is 0 for any ID
  sCANMessage.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
  sCANMessage.ui32MsgLen = 16; // allow up to 8 bytes

  //
  // Now load the message object into the CAN peripheral.  Once loaded the
  // CAN will receive any message on the bus, and an interrupt will occur.
  // Use message object 1 for receiving messages (this is not the same as
  // the CAN ID which can be any value in this example).
  //
  sCANMessage.ui32MsgID = 0x0b0; // wheels A
  sCANMessage.pui8MsgData = (uint8_t *)&wheel_a_data;
  CANMessageSet(CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_RX);

  sCANMessage.ui32MsgID = 0x0b2; // wheels B
  sCANMessage.pui8MsgData = (uint8_t *)&wheel_b_data;
  CANMessageSet(CAN0_BASE, 2, &sCANMessage, MSG_OBJ_TYPE_RX);

  sCANMessage.ui32MsgID = 0x2c4; // engine
  sCANMessage.pui8MsgData = (uint8_t *)&engine_data;
  CANMessageSet(CAN0_BASE, 3, &sCANMessage, MSG_OBJ_TYPE_RX);

  sCANMessage.ui32MsgID = 0x398; // fuel
  sCANMessage.pui8MsgData = (uint8_t *)&fuel_data;
  CANMessageSet(CAN0_BASE, 4, &sCANMessage, MSG_OBJ_TYPE_RX);

  UARTprintf("Ready to start receiving CAN stuff...\n");

  lcd_port_setup();
  lcd_init();

  for (;;) {
    //lcd_clear_and_home();
    snprintf(txtBuffer, 20, "A: %4hd B: %4hd", ntohs(wheel_a_data.wheel1), ntohs(wheel_a_data.wheel2));
    lcd_goto(0,0);
    lcd_puts(txtBuffer);
    
    snprintf(txtBuffer, 20, "A: %4hd B: %4hd", ntohs(wheel_b_data.wheel1), ntohs(wheel_b_data.wheel2));
    lcd_goto(1,0);
    lcd_puts(txtBuffer);
    
    snprintf(txtBuffer, 20, "RPM: %4hd", ntohs(engine_data.rpm));
    lcd_goto(2,0);
    lcd_puts(txtBuffer);
    
    snprintf(txtBuffer, 20, "L/H: %4hd", ntohs(fuel_data.fuel));
    lcd_goto(3,0);
    lcd_puts(txtBuffer);
  }

  //
  // Return no errors
  //
  return (0);
}
