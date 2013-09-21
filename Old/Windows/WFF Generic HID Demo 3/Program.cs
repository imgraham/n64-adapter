using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace WFF_Generic_HID_Demo_3
    {
    static class Program
        {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
            {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
            }
        }
    }
