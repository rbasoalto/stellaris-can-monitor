/******************************************************************************
 *
 * Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
 * Software License Agreement
 * 
 * Texas Instruments (TI) is supplying this software for use solely and
 * exclusively on TI's microcontroller products. The software is owned by
 * TI and/or its suppliers, and is protected under applicable copyright
 * laws. You may not combine this software with "viral" open-source
 * software in order to form a larger program.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
 * NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
 * NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
 * DAMAGES, FOR ANY REASON WHATSOEVER.
 * 
 * This is part of revision 9453 of the EK-LM4F120XL Firmware Package.
 *
 *****************************************************************************/

ENTRY(ResetISR)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00040000
    RAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 0x00008000
	STACK (rwx) : ORIGIN = 0x20007FFF, LENGTH = 0x00000000
}

SECTIONS
{
    .text :
    {
        _start_text = .;
        KEEP(*(.nvic_table))
        *(.text*)
        *(.rodata*)
        _end_text = .;
    } > FLASH

    .data :
    {
        _start_data = .;
        *(vtable)
        *(.data*)
        _end_data = .;
    } > RAM AT > FLASH

    .bss :
    {
        _start_bss = .;
        *(.bss*)
        *(COMMON)
        _end_bss = .;
    } > RAM

    .stack :
    {
        _stack_top = .;
    } > STACK
}

PROVIDE(end = _end_bss);
