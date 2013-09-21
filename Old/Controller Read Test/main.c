/************************************************************************
	main.c

	WFF USB Generic HID Demonstration 3
    usbGenericHidCommunication reference firmware 3_0_0_0
    Copyright (C) 2011 Simon Inns

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Email: simon.inns@gmail.com

************************************************************************/

#ifndef MAIN_C
#define MAIN_C

// Global includes
// Note: string.h is required for sprintf commands for debug
#include <Compiler.h>

#include <string.h>

// Local includes
#include "HardwareProfile.h"
#include "debug.h"
#include <Math.h>

// Microchip Application Library includes
// (expects V2.9a of the USB library from "Microchip Solutions v2011-07-14")
//
// The library location must be set in:
// Project -> Build Options Project -> Directories -> Include search path
// in order for the project to compile.
#include "./USB/usb.h"
#include "./USB/usb_function_hid.h"

// Ensure we have the correct target PIC device family
#if !defined(__18F14K50)
	#error "This firmware only supports the PIC18F14K50 microcontroller."
#endif

// PIC18F4550/PIC18F2550 configuration for the WFF Generic HID test device
#pragma config CPUDIV   = NOCLKDIV
#pragma config USBDIV   = OFF

#pragma config FOSC     = HS
#pragma config PLLEN	= ON
#pragma config PCLKEN	= OFF

USB_HANDLE USBOutHandle = 0;
//USB_HANDLE USBInHandle = 0;
BOOL blinkStatusValid = FLAG_TRUE;

volatile unsigned char controller_data[32] = {0};
volatile unsigned char controller_data_error[1];

extern void InitController(void);
extern void PollController(void);

// Private function prototypes
void initialisePic(void);
void processUsbCommands(void);
void applicationInit(void);

// Main program entry point
void main(void)
{
	// Initialise and configure the PIC ready to go
    initialisePic();

	// Main processing loop
    while(1)
    {
		PollController();
		if(!controller_data_error[0])
		{
			if(controller_data[15] > 0)
				LATBbits.LATB7 = 0;
			else
				LATBbits.LATB7 = 1;
		}
    }
}

// Initialise the PIC
void initialisePic(void)
{
    // PIC port set up --------------------------------------------------------

	// Default all pins to digital
    ANSEL = 0x00;

	// Configure ports as inputs (1) or outputs(0)
/*	TRISA = 0b11111111;
	TRISB = 0b11111111;
	TRISC = 0b11111111;
#if defined(__18F4550)
	TRISD = 0b00000000;
	TRISE = 0b00000000;
#endif

	// Clear all ports
	PORTA = 0b00000000;
	PORTB = 0b00000000;
	PORTC = 0b00000000;
#if defined(__18F4550)
	PORTD = 0b00000000;
	PORTE = 0b00000000;
#endif*/

	LATC = 0x00;
	PORTC = 0x00;
	TRISC = 0x04;

	LATB = 0x00;
	PORTB = 0x00;
	TRISB = 0x7F;
	

    // Application specific initialisation
    applicationInit();
}

// Application specific device initialisation
void applicationInit(void)
{

}

#endif
