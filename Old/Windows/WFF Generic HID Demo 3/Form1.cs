//-----------------------------------------------------------------------------
//
//  Form1.cs
//
//  USB Generic HID Demonstration 3_0_0_0
//
//  A reference test application for the usbGenericHidCommunications library
//  Copyright (C) 2011 Simon Inns
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  Web:    http://www.waitingforfriday.com
//  Email:  simon.inns@gmail.com
//
//-----------------------------------------------------------------------------

//
//  Current reference class library version:
//  usbGenericHidCommunications class library version 3.0.0.0
//

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WFF_Generic_HID_Demo_3
    {
    public partial class Form1 : Form
        {
        public Form1()
            {
            InitializeComponent();

            // Create the USB reference device object (passing VID and PID)
            theUsbDemoDevice = new usbDemoDevice(0x04D8, 0x0042);

            // Add a listener for usb events
            theUsbDemoDevice.usbEvent += new usbDemoDevice.usbEventsHandler(usbEvent_receiver);

            // Perform an initial search for the target device
            theUsbDemoDevice.findTargetDevice();
            }

        // Create an instance of the USB reference device
        private usbDemoDevice theUsbDemoDevice;

        // Listener for USB events
        private void usbEvent_receiver(object o, EventArgs e)
            {
            // Check the status of the USB device and update the form accordingly
            if (theUsbDemoDevice.isDeviceAttached)
                {
                // Device is currently attached

                // Update the status label
                this.toolStripStatusLabel1.Text = "USB Device is attached";

                // Update the form
                this.toggleLedStateButton.Enabled = true;
                }
            else
                {
                // Device is currently unattached

                // Update the status label
                this.toolStripStatusLabel1.Text = "USB Device is detached";

                // Update the form
                this.toggleLedStateButton.Enabled = false;
                }
            }

        // Link label to website clicked
        private void linkLabel1_LinkClicked_1(object sender, LinkLabelLinkClickedEventArgs e)
            {
            // Specify that the link was visited.
            this.linkLabel1.LinkVisited = true;

            // Navigate to a URL.
            System.Diagnostics.Process.Start("http://www.waitingforfriday.com");
            }

        // Toggle LED state button pressed
        private void toggleLedStateButton_Click(object sender, EventArgs e)
            {
            if (theUsbDemoDevice.isDeviceAttached)
                {
                theUsbDemoDevice.toggleLedState();
                }
            }

        // Timer 1 has ticked, poll the USB device for status
        private void timer1_Tick(object sender, EventArgs e)
            {
            if (theUsbDemoDevice.isDeviceAttached)
                {
                // Read the push button state
                bool buttonState = theUsbDemoDevice.readPushButtonState();

                if (buttonState == true) this.pushButtonStateLabel.Text = "Button pressed";
                else this.pushButtonStateLabel.Text = "Button not pressed";

                // Read the LED state
                bool ledState = theUsbDemoDevice.readLedState();

                if (ledState == true) this.ledStateLabel.Text = "LED is on";
                else this.ledStateLabel.Text = "LED is off";
                }

                UInt32 N64State = theUsbDemoDevice.readN64State();
                this.n64label.Text = Convert.ToString(N64State, 2).PadLeft(8, '0'); ;
            }

        // Collect debug timer has ticked
        private void debugCollectionTimer_Tick(object sender, EventArgs e)
            {
            String debugString;

            // Only collect debug if the device is attached
            if (theUsbDemoDevice.isDeviceAttached)
                {
                // Collect the debug information from the device
                debugString = theUsbDemoDevice.collectDebug();

                // Display the debug information
                if (debugString != String.Empty)
                    {
                    this.debugTextBox.AppendText(debugString);
                    }
                }
            else
                {
                // Clear the debug window
                this.debugTextBox.Clear();
                }
            }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
        }
    }
