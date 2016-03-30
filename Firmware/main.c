/************************************************************************
 *      main.c
 *
 *      N64 Adapter main. This file handles the USB communication and
 *      retreiving data from the controller
 *
 *      ian.grhm@gmail.com
 *      2013
************************************************************************/

#ifndef MAIN_C
#define MAIN_C

// Global includes
#include <Compiler.h>

// Local includes
#include "HardwareProfile.h"
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

// PIC18F14K50 Hardware Configuration
#pragma config CPUDIV   = NOCLKDIV
#pragma config USBDIV   = OFF

#pragma config FOSC     = HS
#pragma config PLLEN	= ON
#pragma config PCLKEN	= OFF

#pragma config HFOFST	= OFF

#pragma config MCLRE	= ON

#pragma config WDTEN 	= OFF

// 32 bytes
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
            BYTE B8:1;
            BYTE B9:1;
            BYTE B10:1;
            BYTE :6;    //filler to byte-align
        } buttons;
        struct
        {
            BYTE hat_switch:4;
            BYTE :4;	//filler to byte-align
        } hat_switch;
        struct
        {
            BYTE X;
            BYTE Y;
        } analog_stick;
    } members;
} INPUT_CONTROLS;

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
#define round(X) (int)(X+0.5)

extern void PollController(void);

// Private function prototypes
void initialisePic(void);
void processUsbCommands(void);
void applicationInit(void);
void USBCBSendResume(void);
void highPriorityISRCode();
void lowPriorityISRCode();

// Define the globals for the USB data in the USB RAM of the PIC18F*550
#pragma udata
USB_HANDLE lastTransmission = 0;

char buffer[64]; //not sure why it doesn't work without this...

volatile unsigned char controller_data[32] = {0};
volatile unsigned char controller_data_error[1];

#if defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50) 
#pragma udata usbram2 
#endif
INPUT_CONTROLS joystickUSBBuffer; // 32 bytes
BYTE hid_report[8];
#pragma udata

#pragma code
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

// Main program entry point
void main(void)
{   
    // Initialise and configure the PIC ready to go
    initialisePic();

    // If we are running in interrupt mode attempt to attach the USB device
    #if defined(USB_INTERRUPT)
        USBDeviceAttach();
    #endif

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
    }
}

// Initialise the PIC
void initialisePic(void)
{
    // PIC port set up --------------------------------------------------------
    // Default all pins to digital
    ANSEL = 0x00;

    //Initialize ports and set unused pins to GND
    LATC = 0x00;
    PORTC = 0x00;
    TRISC = 0x04;

    LATB = 0x00;
    PORTB = 0x00;
    TRISB = 0x00;

    //disable watchdog
    WDTCON = 0;

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
    // Initialize the variable holding the USB handle for the last transmission
    lastTransmission = 0;
}

// Process USB commands
void processUsbCommands(void)
{
    static unsigned char read_error_count = 0;

    // Check if we are in the configured state; otherwise just return
    if((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1))
    {
        // We are not configured
        return;
    }

    // If the last transmision is complete send the joystick status
    if(!HIDTxHandleBusy(lastTransmission))
    {
        char XAxis, YAxis;
        int XNew, YNew;
        float cX = 0, cY = 0;
        unsigned char hat;

        //get updated data from controller
        PollController();

        //don't send anything if there was an issue getting controller data
        if(controller_data_error[0]) {
            read_error_count++;
            if(read_error_count > 10)
                memset((char*)controller_data,0,32);
            else
                return;
        }
        read_error_count = 0;

        // Set the button values
        joystickUSBBuffer.members.buttons.B1 = controller_data[0]  & 0x01;
        joystickUSBBuffer.members.buttons.B2 = controller_data[1]  & 0x01;
        joystickUSBBuffer.members.buttons.B3 = controller_data[2]  & 0x01;
        joystickUSBBuffer.members.buttons.B4 = controller_data[3]  & 0x01;
        joystickUSBBuffer.members.buttons.B5 = controller_data[4]  & 0x01;
        joystickUSBBuffer.members.buttons.B6 = controller_data[5]  & 0x01;
        joystickUSBBuffer.members.buttons.B7 = controller_data[6]  & 0x01;
        joystickUSBBuffer.members.buttons.B8 = controller_data[7]  & 0x01;
        joystickUSBBuffer.members.buttons.B9 = controller_data[10] & 0x01;
        joystickUSBBuffer.members.buttons.B10= controller_data[11] & 0x01;
        joystickUSBBuffer.members.hat_switch.hat_switch = HAT_SWITCH_NULL;

        //determine the hat position
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

            joystickUSBBuffer.members.hat_switch.hat_switch = hat%8;
        }

        //convert array to single byte
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

        //change from unsigned to signed
        XNew = (int)127+(int)XAxis;
        YNew = (int)127-(int)YAxis;

        //make sure there is no overflow
        if(XNew > 255)
            XNew = 255;
        else if(XNew < 0)
            XNew = 0;

        if(YNew > 255)
            YNew = 255;
        else if(YNew < 0)
            YNew = 0;

        joystickUSBBuffer.members.analog_stick.X = (unsigned char)XNew;
        joystickUSBBuffer.members.analog_stick.Y = (unsigned char)YNew;

        //Send the packet over USB to the host.
        lastTransmission = HIDTxPacket(HID_EP, (BYTE*)&joystickUSBBuffer, sizeof(joystickUSBBuffer));
    }
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
#pragma code

#endif
