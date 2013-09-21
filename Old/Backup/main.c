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

// 35 bytes
typedef union _INTPUT_CONTROLS_TYPEDEF
{
    struct
    {
        struct
        {
            BYTE B1:1;
            BYTE B2:1;
            BYTE B3:1;
            BYTE B4:1;
            BYTE B5:1;
            BYTE B6:1;
            BYTE B7:1;
            BYTE B8:1;//
            BYTE B9:1;
            BYTE B10:1;
            BYTE B11:1;
            BYTE B12:1;
            BYTE B13:1;
            BYTE :3;    //filler
        } buttons;
        struct
        {
            BYTE hat_switch:4;
            BYTE :4;//filler
        } hat_switch;
        struct
        {
            BYTE X;
            BYTE Y;
            BYTE Z;
            BYTE Rz;
        } analog_stick;
    } members;
    BYTE val[7];
} INPUT_CONTROLS;

USB_HANDLE lastTransmission_j1;

#define HAT_SWITCH_NORTH            0x0
#define HAT_SWITCH_NORTH_EAST       0x1
#define HAT_SWITCH_EAST             0x2
#define HAT_SWITCH_SOUTH_EAST       0x3
#define HAT_SWITCH_SOUTH            0x4
#define HAT_SWITCH_SOUTH_WEST       0x5
#define HAT_SWITCH_WEST             0x6
#define HAT_SWITCH_NORTH_WEST       0x7
#define HAT_SWITCH_NULL             0x8

#define PI (float)3.14159

// Define the globals for the USB data in the USB RAM of the PIC18F*550
#pragma udata
#if defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50) 
#pragma udata usbram2 
#endif

unsigned char ReceivedDataBuffer[32];
//unsigned char ToSendDataBuffer[32];
INPUT_CONTROLS joystick_input_j1; // 35 bytes
#pragma udata

USB_HANDLE USBOutHandle = 0;
//USB_HANDLE USBInHandle = 0;
BOOL blinkStatusValid = FLAG_TRUE;

volatile unsigned char controller_data[32] = {0};
volatile unsigned char controller_data_error[1];

extern void InitController(void);
extern void PollController(void);

// PIC18F4550/PIC18F2550 configuration for the WFF Generic HID test device
#pragma config CPUDIV   = NOCLKDIV
#pragma config USBDIV   = OFF

#pragma config FOSC     = HS
#pragma config PLLEN	= ON
#pragma config PCLKEN	= OFF

// Private function prototypes
void initialisePic(void);
void processUsbCommands(void);
void applicationInit(void);
void USBCBSendResume(void);
void highPriorityISRCode();
void lowPriorityISRCode();

// High-priority ISR handling function
#pragma interrupt highPriorityISRCode
void highPriorityISRCode()
{
	// Application specific high-priority ISR code goes here
	
	#if defined(USB_INTERRUPT)
		// Perform USB device tasks
		USBDeviceTasks();
	#endif

}

// Low-priority ISR handling function
#pragma interruptlow lowPriorityISRCode
void lowPriorityISRCode()
{
	// Application specific low-priority ISR code goes here
}

// String for creating debug messages
char debugString[64];

// Main program entry point
void main(void)
{   
	UINT16 i;

	// Initialise and configure the PIC ready to go
    initialisePic();

	// If we are running in interrupt mode attempt to attach the USB device
    #if defined(USB_INTERRUPT)
        USBDeviceAttach();
    #endif
	
	// Initialise the debug log functions
    //debugInitialise();
    
    // Show that we are up and running
    //mStatusLED0_on();
	
/*	sprintf(debugString, "USB Generic HID Demonstration 3");
	debugOut(debugString);

	//sprintf(debugString, "(C)2011 Simon Inns - http://www.waitingforfriday.com");
	//debugOut(debugString);
	
	sprintf(debugString, "USB Device Initialised. ");
	debugOut(debugString);

	sprintf(debugString, "Initializing N64 Controller.");
	debugOut(debugString);

	InitController();
	sprintf(debugString, "N64 Controller Initialised.");
	debugOut(debugString);*/
	
	for(i = 0; i < 32000; i++);
	for(i = 0; i < 32000; i++);
	for(i = 0; i < 32000; i++);
	for(i = 0; i < 32000; i++);
	for(i = 0; i < 32000; i++);
	for(i = 0; i < 32000; i++);
	for(i = 0; i < 32000; i++);

	// Main processing loop
    while(1)
    {
        #if defined(USB_POLLING)
			// If we are in polling mode the USB device tasks must be processed here
			// (otherwise the interrupt is performing this task)
	        USBDeviceTasks();
        #endif
    	
    	// Process USB Commands
        processUsbCommands();  
        
        // Note: Other application specific actions can be placed here      
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

	PORTC = 0x00;
	LATC = 0x00;

	TRISC = 0x04;

	// If you have a VBUS sense pin (for self-powered devices when you
	// want to detect if the USB host is connected) you have to specify
	// your input pin in HardwareProfile.h
    #if defined(USE_USB_BUS_SENSE_IO)
    	tris_usb_bus_sense = INPUT_PIN;
    #endif
    
    // In the case of a device which can be both self-powered and bus-powered
    // the device must respond correctly to a GetStatus (device) request and
    // tell the host how it is currently powered.
    //
    // To do this you must device a pin which is high when self powered and low
    // when bus powered and define this in HardwareProfile.h
    #if defined(USE_SELF_POWER_SENSE_IO)
    	tris_self_power = INPUT_PIN;
    #endif

    // Application specific initialisation
    applicationInit();
    
    // Initialise the USB device
    USBDeviceInit();
}

// Application specific device initialisation
void applicationInit(void)
{
	// Initialise the status LEDs
	//mInitStatusLeds();
	
	// Initialise the switch
	//mInitAllSwitches();
	
    // Initialize the variable holding the USB handle for the last transmission
    USBOutHandle = 0;
//    USBInHandle = 0;

	lastTransmission_j1 = 0;
}

// Process USB commands
void processUsbCommands(void)
{   
    // Check if we are in the configured state; otherwise just return
    if((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1))
    {
	    // We are not configured
	    return;
	}

    // If the last transmision is complete send the joystick status
    if(!HIDTxHandleBusy(lastTransmission_j1))
    {
		char XAxis, YAxis;
		int XNew, YNew;
		float cX, cY;
		unsigned char hat;

		//PollController();

		//if(controller_data_error[0])
		//	return;

		// Set everything to the defult
		joystick_input_j1.members.buttons.B1 = 0;
		joystick_input_j1.members.buttons.B2 = 0;
		joystick_input_j1.members.buttons.B3 = 0;
		joystick_input_j1.members.buttons.B4 = 0;
		joystick_input_j1.members.buttons.B5 = 0;
		joystick_input_j1.members.buttons.B6 = 0;
		joystick_input_j1.members.buttons.B7 = 0;
		joystick_input_j1.members.buttons.B8 = 0;
		joystick_input_j1.members.buttons.B9 = 0;
		joystick_input_j1.members.buttons.B10 = 0;
		joystick_input_j1.members.buttons.B11 = 0;
		joystick_input_j1.members.buttons.B12 = 0;
		joystick_input_j1.members.buttons.B13 = 0;
		joystick_input_j1.members.hat_switch.hat_switch = HAT_SWITCH_NULL;

		// 128 is 'middle'
		joystick_input_j1.members.analog_stick.X = 128;
		joystick_input_j1.members.analog_stick.Y = 128;
		joystick_input_j1.members.analog_stick.Z = 128;
		joystick_input_j1.members.analog_stick.Rz = 128;

		// Now adjust according to the joystick switch states
		if (controller_data[0])
			joystick_input_j1.members.buttons.B1 = 1;
		if (controller_data[1])
			joystick_input_j1.members.buttons.B2 = 1;
		if (controller_data[2])
					joystick_input_j1.members.buttons.B3 = 1;
		if (controller_data[3])
					joystick_input_j1.members.buttons.B4 = 1;
		if (controller_data[4])
							joystick_input_j1.members.buttons.B5 = 1;
		if (controller_data[5])
							joystick_input_j1.members.buttons.B6 = 1;
		if (controller_data[6])
							joystick_input_j1.members.buttons.B7 = 1;
		if (controller_data[7])
							joystick_input_j1.members.buttons.B8 = 1;

		if (controller_data[10])
							joystick_input_j1.members.buttons.B9 = 1;
		if (controller_data[11])
							joystick_input_j1.members.buttons.B10 = 1;

		cY = cX = 0;
		if(controller_data[12])
			cY -= 2;
		if(controller_data[13])
			cY += 2;
		if(controller_data[14])
			cX -= 2;
		if(controller_data[15])
			cX += 2;

		if(cY != 0 || cX != 0)
		{
			if(cY < 0)
				hat = round(((2*PI)-acos(cX/sqrt(cX*cX+cY*cY)))*(4/PI))+2;
			else
				hat = round(acos(cX/sqrt(cX*cX+cY*cY))*(4/PI))+2;

			joystick_input_j1.members.hat_switch.hat_switch = hat%8;
		}

		XAxis =	controller_data[16] & 0x80 |
				controller_data[17] & 0x40 |
				controller_data[18] & 0x20 |
				controller_data[19] & 0x10 |
				controller_data[20] & 0x08 |
				controller_data[21] & 0x04 |
				controller_data[22] & 0x02 |
				controller_data[23] & 0x01;
		
		YAxis = controller_data[24] & 0x80 |
				controller_data[25] & 0x40 |
				controller_data[26] & 0x20 |
				controller_data[27] & 0x10 |
				controller_data[28] & 0x08 |
				controller_data[29] & 0x04 |
				controller_data[30] & 0x02 |
				controller_data[31] & 0x01;
		
		XNew = (int)127+(float)XAxis*1.0;
		YNew = (int)127-(float)YAxis*1.0;

		if(XNew > 255)
			XNew = 255;
		else if(XNew < 0)
			XNew = 0;

		if(YNew > 255)
			YNew = 255;
		else if(YNew < 0)
			YNew = 0;

		joystick_input_j1.members.analog_stick.X = (unsigned char)XNew;
		joystick_input_j1.members.analog_stick.Y = (unsigned char)YNew;

		//Send the packet over USB to the host.
		lastTransmission_j1 = HIDTxPacket(HID_EP, (BYTE*)&joystick_input_j1, sizeof(joystick_input_j1));
    }

	// Check if data was received from the host.
/*    if(!HIDRxHandleBusy(USBOutHandle))
    {   
		// Command mode    
        switch(ReceivedDataBuffer[0])
		{
			case 0x10:	// Debug information request from host
				// Copy any waiting debug text to the send data buffer
				copyDebugToSendBuffer((BYTE*)&ToSendDataBuffer[0]);
			        
		        // Transmit the response to the host
                if(!HIDTxHandleBusy(USBInHandle))
				{
					USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
				}
				break;
				
			// Place application specific commands here:
			
            case 0x80:  // Toggle the LED
			{
				int i = 0;
            	//sprintf(debugString, "Received command 0x80 from host - Toggle LED");
				PollController();

				LATBbits.LATB3 = 0;
				for(i = 0; i < 32; i++)
				{
					if(controller_data[i] > 0) {
						LATBbits.LATB3 = 1;	
						sprintf(debugString, "%i   %x",i,controller_data[i]);
						debugOut(debugString);				
					}
				}
				sprintf(debugString, "--------------",controller_data[i],i);
				debugOut(debugString);	

				// Toggle the LED0
				mStatusLED0_Toggle();
            	break;
			}
            	
            case 0x81:  // Read the push switch status
				ToSendDataBuffer[0] = sw0;

				// Transmit the response to the host
                if(!HIDTxHandleBusy(USBInHandle))
				{
					USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
				}
            	break;
            	
            case 0x82:  // Read the LED status
				// Get the LED state and put it in the send buffer
				ToSendDataBuffer[0] = mStatusLED0_Get();

				// Transmit the response to the host
                if(!HIDTxHandleBusy(USBInHandle))
				{
					USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
				}
            	break;

			case 0x83:  // Read the N64 status
			{
				int i;
				PollController();

				ToSendDataBuffer[0] = 0;
				ToSendDataBuffer[1] = 0;
				ToSendDataBuffer[2] = 0;
				ToSendDataBuffer[3] = 0;
				for(i = 0; i < 32; i++)
				{
					if(controller_data[i])
						ToSendDataBuffer[i/8] |= 0x01 << (i%8);
				}
	
				// Transmit the response to the host
                if(!HIDTxHandleBusy(USBInHandle))
				{
					USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
				}
            	break;
			}
	
            default:	// Unknown command received
           		break;
		}
		 
        // Re-arm the OUT endpoint for the next packet
        USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
  	}*/
}

// USB Callback handling routines -----------------------------------------------------------

// Call back that is invoked when a USB suspend is detected
void USBCBSuspend(void)
{
}

// This call back is invoked when a wakeup from USB suspend is detected.
void USBCBWakeFromSuspend(void)
{
}

// The USB host sends out a SOF packet to full-speed devices every 1 ms.
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here. Callback caller is already doing that.
}

// The purpose of this callback is mainly for debugging during development.
// Check UEIR to see which error causes the interrupt.
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.
}

// Check other requests callback
void USBCBCheckOtherReq(void)
{
    USBCheckHIDRequest();
}

// Callback function is called when a SETUP, bRequest: SET_DESCRIPTOR request arrives.
void USBCBStdSetDscHandler(void)
{
    // You must claim session ownership if supporting this request
}

//This function is called when the device becomes initialized
void USBCBInitEP(void)
{
    // Enable the HID endpoint
    USBEnableEndpoint(HID_EP,USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    
    // Re-arm the OUT endpoint for the next packet
    USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
}

// Send resume call-back
void USBCBSendResume(void)
{
    static WORD delay_count;
    
    // Verify that the host has armed us to perform remote wakeup.
    if(USBGetRemoteWakeupStatus() == FLAG_TRUE) 
    {
        // Verify that the USB bus is suspended (before we send remote wakeup signalling).
        if(USBIsBusSuspended() == FLAG_TRUE)
        {
            USBMaskInterrupts();
            
            // Bring the clock speed up to normal running state
            USBCBWakeFromSuspend();
            USBSuspendControl = 0; 
            USBBusIsSuspended = FLAG_FALSE;

            // Section 7.1.7.7 of the USB 2.0 specifications indicates a USB
            // device must continuously see 5ms+ of idle on the bus, before it sends
            // remote wakeup signalling.  One way to be certain that this parameter
            // gets met, is to add a 2ms+ blocking delay here (2ms plus at 
            // least 3ms from bus idle to USBIsBusSuspended() == FLAG_TRUE, yeilds
            // 5ms+ total delay since start of idle).
            delay_count = 3600U;        
            do
            {
                delay_count--;
            } while(delay_count);
            
            // Start RESUME signaling for 1-13 ms
            USBResumeControl = 1;
            delay_count = 1800U;
            do
            {
                delay_count--;
            } while(delay_count);
            USBResumeControl = 0;

            USBUnmaskInterrupts();
        }
    }
}

// USB callback function handler
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_TRANSFER:
            // Application callback tasks and functions go here
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_CONFIGURED: 
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        default:
            break;
    }      
    return FLAG_TRUE; 
}

int round (float x) {
	return (int)(x+0.5);
}

#endif

/*
void main(void)
{
	InitController();
	while(1)
	{
		PollController();
		if(controller_data[0] > 0)
			LATBbits.LATB3 = 0;
		else
			LATBbits.LATB3 = 1;
	}
}*/
