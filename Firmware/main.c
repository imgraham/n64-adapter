/************************************************************************
 *      main.c
 *
 *      N64 Adapter main. This file handles the USB communication and
 *      retrieving data from the controller
 *
 *      ian.grhm@gmail.com
 *      2013
************************************************************************/

#ifndef MAIN_C
#define MAIN_C
#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER
// Global includes
#include <Compiler.h>

// Local includes
#include "HardwareProfile.h"

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

// 32 bytes - joystick layout (USB driver)
typedef union _INTPUT_CONTROLS_TYPEDEF
{
    struct
    {
        struct
        {
            BYTE A:1;
            BYTE B:1;
            BYTE Z:1;
            BYTE Start:1;
            BYTE Up:1;
            BYTE Down:1;
            BYTE Left:1;
            BYTE Right:1;
            BYTE L:1;
            BYTE R:1;
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

typedef struct N64ControllerBitstream {
    BYTE A;
    BYTE B;
    BYTE Z;
    BYTE Start;
    BYTE Up;
    BYTE Down;
    BYTE Left;
    BYTE Right;

    BYTE Reserved[2];
    BYTE L;
    BYTE R;
    BYTE C_Up;
    BYTE C_Down;
    BYTE C_Left;
    BYTE C_Right;

    BYTE X_Axis[8];
    BYTE Y_Axis[8];
} N64ControllerBitstream;

//hat positions
#define HAT_SWITCH_NORTH            0x0
#define HAT_SWITCH_NORTH_EAST       0x1
#define HAT_SWITCH_EAST             0x2
#define HAT_SWITCH_SOUTH_EAST       0x3
#define HAT_SWITCH_SOUTH            0x4
#define HAT_SWITCH_SOUTH_WEST       0x5
#define HAT_SWITCH_WEST             0x6
#define HAT_SWITCH_NORTH_WEST       0x7
#define HAT_SWITCH_NULL             0x8

// C button mappings
#define C_UP 0x01
#define C_DOWN 0x02
#define C_LEFT 0x04
#define C_RIGHT 0x08

//extern BYTE PollController(void);
//extern BYTE IdentifyController(void);

extern void N64CommSendCommand(BYTE cmd);
extern BYTE N64CommReadData(void *data, BYTE size);

// Private function prototypes
void initialisePic(void);
void processUsbCommands(void);
void applicationInit(void);
void USBCBSendResume(void);
void highPriorityISRCode();
void lowPriorityISRCode();

// Define the globals for the USB data in the USB RAM
#if defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50)
#pragma udata usbram2
#endif
USB_HANDLE lastTransmission = 0;
INPUT_CONTROLS joystickUSBBuffer; // 32 bytes
BYTE hid_report[8];

#pragma udata
//char buffer[64]; //not sure why it doesn't work without this...

volatile N64ControllerBitstream controller_data = {0};

/** VECTOR REMAPPING ***********************************************/
#if defined(__18CXX)
	//On PIC18 devices, addresses 0x00, 0x08, and 0x18 are used for
	//the reset, high priority interrupt, and low priority interrupt
	//vectors.  However, the current Microchip USB bootloader
	//examples are intended to occupy addresses 0x00-0x7FF or
	//0x00-0xFFF depending on which bootloader is used.  Therefore,
	//the bootloader code remaps these vectors to new locations
	//as indicated below.  This remapping is only necessary if you
	//wish to program the hex file generated from this project with
	//the USB bootloader.  If no bootloader is used, edit the
	//usb_config.h file and comment out the following defines:
	//#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER
	//#define PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER

	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x1000
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x1008
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x1018
	#elif defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x800
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x808
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x818
	#else
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x00
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x08
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x18
	#endif

	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
	extern void _startup (void);        // See c018i.c in your C18 compiler dir
	#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
	void _reset (void)
	{
	    _asm goto _startup _endasm
	}
	#endif
	#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
	void Remapped_High_ISR (void)
	{
	     //_asm goto YourHighPriorityISRCode _endasm
	}
	#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
	void Remapped_Low_ISR (void)
	{
	     //_asm goto YourLowPriorityISRCode _endasm
        }

	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
	//Note: If this project is built while one of the bootloaders has
	//been defined, but then the output hex file is not programmed with
	//the bootloader, addresses 0x08 and 0x18 would end up programmed with 0xFFFF.
	//As a result, if an actual interrupt was enabled and occured, the PC would jump
	//to 0x08 (or 0x18) and would begin executing "0xFFFF" (unprogrammed space).  This
	//executes as nop instructions, but the PC would eventually reach the REMAPPED_RESET_VECTOR_ADDRESS
	//(0x1000 or 0x800, depending upon bootloader), and would execute the "goto _startup".  This
	//would effective reset the application.

	//To fix this situation, we should always deliberately place a
	//"goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS" at address 0x08, and a
	//"goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS" at address 0x18.  When the output
	//hex file of this project is programmed with the bootloader, these sections do not
	//get bootloaded (as they overlap the bootloader space).  If the output hex file is not
	//programmed using the bootloader, then the below goto instructions do get programmed,
	//and the hex file still works like normal.  The below section is only required to fix this
	//scenario.
	#pragma code HIGH_INTERRUPT_VECTOR = 0x08
	void High_ISR (void)
	{
	     _asm goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#pragma code LOW_INTERRUPT_VECTOR = 0x18
	void Low_ISR (void)
	{
	     _asm goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#endif	//end of "#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER)"

	#pragma code


	//These are your actual interrupt handling routines.
	#pragma interrupt YourHighPriorityISRCode
	void YourHighPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
        #if defined(USB_INTERRUPT)
	        USBDeviceTasks();
        #endif

	}	//This return will be a "retfie fast", since this is in a #pragma interrupt section
	#pragma interruptlow YourLowPriorityISRCode
	void YourLowPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.

	}	//This return will be a "retfie", since this is in a #pragma interruptlow section

#elif defined(__C30__)
    #if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)
        /*
         *	ISR JUMP TABLE
         *
         *	It is necessary to define jump table as a function because C30 will
         *	not store 24-bit wide values in program memory as variables.
         *
         *	This function should be stored at an address where the goto instructions
         *	line up with the remapped vectors from the bootloader's linker script.
         *
         *  For more information about how to remap the interrupt vectors,
         *  please refer to AN1157.  An example is provided below for the T2
         *  interrupt with a bootloader ending at address 0x1400
         */
//        void __attribute__ ((address(0x1404))) ISRTable(){
//
//        	//asm("reset"); //reset instruction to prevent runaway code
//        	//asm("goto %0"::"i"(&_T2Interrupt));  //T2Interrupt's address
//        }
    #endif
#endif

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

//void convertControllerData (unsigned char num_bytes, void* ret)
//{
//    for(; num_bytes > 0; num_bytes--)
//    {
//        *(unsigned long*)ret |= (unsigned long)(controller_data[num_bytes-1] & 0x01) << 24-num_bytes;
//    }
//}

// Main program entry point
void main(void)
{
    // Commented out because I guess this didn't work
    //unsigned long id = 0;
    //unsigned int i;

    // Initialise and configure the PIC ready to go
    initialisePic();

    // If we are running in interrupt mode attempt to attach the USB device
    #if defined(USB_INTERRUPT)
        USBDeviceAttach();
    #endif

    /*while(id&0xFFFFF8 != 0x050000) {
        IdentifyController();
        id = 0;
        if(!controller_data_error) {
            convertControllerData(24, &id);
        }

        //delay
        for(i = 0; i < 32000; i++);
    } */

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

static BYTE IdentifyController(void) {
    N64CommSendCommand(0x00); // TODO: magic number
    return N64CommReadData(&controller_data, 24);
}

static BYTE PollController(void) {
    N64CommSendCommand(0x01); // TODO: magic number
    return N64CommReadData(&controller_data, sizeof(controller_data));
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

    // If the last transmission is complete send the joystick status
    if(!HIDTxHandleBusy(lastTransmission))
    {
        char XAxis, YAxis;
        int XNew, YNew;
        int cY = 0;
        unsigned char controller_reads = 0;

        //get updated data from controller
        for (;;) {
            if (!PollController()) {
                break;
            }

            //don't send anything if there was an issue getting controller data
            if (controller_reads++ > 5) {
                memset((char*)&controller_data, 0, sizeof(controller_data));
                break;
            }
        }

        //convert controller array to USB joystick data
        // Set the button values
        joystickUSBBuffer.members.buttons.A = controller_data.A;
        joystickUSBBuffer.members.buttons.B = controller_data.B;
        joystickUSBBuffer.members.buttons.Z = controller_data.Z;
        joystickUSBBuffer.members.buttons.Start = controller_data.Start;
        joystickUSBBuffer.members.buttons.Up = controller_data.Up;
        joystickUSBBuffer.members.buttons.Down = controller_data.Down;
        joystickUSBBuffer.members.buttons.Left = controller_data.Left;
        joystickUSBBuffer.members.buttons.Right = controller_data.Right;
        joystickUSBBuffer.members.buttons.L = controller_data.L;
        joystickUSBBuffer.members.buttons.R= controller_data.R;
        joystickUSBBuffer.members.hat_switch.hat_switch = HAT_SWITCH_NULL;

        //determine the hat position
        // Note: North is less than south in the hat mapping
        if(controller_data.C_Up)
            cY += 1;
        if(controller_data.C_Down)
            cY -= 1;

        if (controller_data.C_Right) {
            joystickUSBBuffer.members.hat_switch.hat_switch = HAT_SWITCH_EAST - cY;
        } else if (controller_data.C_Left) {
            joystickUSBBuffer.members.hat_switch.hat_switch = HAT_SWITCH_WEST + cY;
        } else if (controller_data.C_Up) {
            joystickUSBBuffer.members.hat_switch.hat_switch = HAT_SWITCH_NORTH;
        } else if (controller_data.C_Down) {
            joystickUSBBuffer.members.hat_switch.hat_switch = HAT_SWITCH_SOUTH;
        }

        //convert array to single byte
        XAxis = controller_data.X_Axis[0] & 0x80 |
                controller_data.X_Axis[1] & 0x40 |
                controller_data.X_Axis[2] & 0x20 |
                controller_data.X_Axis[3] & 0x10 |
                controller_data.X_Axis[4] & 0x08 |
                controller_data.X_Axis[5] & 0x04 |
                controller_data.X_Axis[6] & 0x02 |
                controller_data.X_Axis[7] & 0x01;

        YAxis = controller_data.Y_Axis[0] & 0x80 |
                controller_data.Y_Axis[1] & 0x40 |
                controller_data.Y_Axis[2] & 0x20 |
                controller_data.Y_Axis[3] & 0x10 |
                controller_data.Y_Axis[4] & 0x08 |
                controller_data.Y_Axis[5] & 0x04 |
                controller_data.Y_Axis[6] & 0x02 |
                controller_data.Y_Axis[7] & 0x01;

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

#endif
