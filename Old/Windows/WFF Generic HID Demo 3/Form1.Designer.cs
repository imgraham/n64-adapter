namespace WFF_Generic_HID_Demo_3
    {
    partial class Form1
        {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
            {
            if (disposing && (components != null))
                {
                components.Dispose();
                }
            base.Dispose(disposing);
            }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
            {
            this.components = new System.ComponentModel.Container();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.toggleLedStateButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.ledStateLabel = new System.Windows.Forms.Label();
            this.pushButtonStateLabel = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.debugTextBox = new System.Windows.Forms.TextBox();
            this.debugCollectionTimer = new System.Windows.Forms.Timer(this.components);
            this.label3 = new System.Windows.Forms.Label();
            this.n64label = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 307);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(373, 22);
            this.statusStrip1.SizingGrip = false;
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(116, 17);
            this.toolStripStatusLabel1.Text = "USB Status unknown";
            // 
            // linkLabel1
            // 
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.Location = new System.Drawing.Point(201, 12);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(158, 13);
            this.linkLabel1.TabIndex = 1;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "http://www.waitingforfriday.com";
            this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked_1);
            // 
            // toggleLedStateButton
            // 
            this.toggleLedStateButton.Location = new System.Drawing.Point(12, 12);
            this.toggleLedStateButton.Name = "toggleLedStateButton";
            this.toggleLedStateButton.Size = new System.Drawing.Size(115, 23);
            this.toggleLedStateButton.TabIndex = 3;
            this.toggleLedStateButton.Text = "Toggle LED State";
            this.toggleLedStateButton.UseVisualStyleBackColor = true;
            this.toggleLedStateButton.Click += new System.EventHandler(this.toggleLedStateButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(50, 46);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "LED state:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(16, 59);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(93, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Push-button state:";
            // 
            // ledStateLabel
            // 
            this.ledStateLabel.AutoSize = true;
            this.ledStateLabel.Location = new System.Drawing.Point(115, 46);
            this.ledStateLabel.Name = "ledStateLabel";
            this.ledStateLabel.Size = new System.Drawing.Size(53, 13);
            this.ledStateLabel.TabIndex = 6;
            this.ledStateLabel.Text = "Unknown";
            // 
            // pushButtonStateLabel
            // 
            this.pushButtonStateLabel.AutoSize = true;
            this.pushButtonStateLabel.Location = new System.Drawing.Point(115, 59);
            this.pushButtonStateLabel.Name = "pushButtonStateLabel";
            this.pushButtonStateLabel.Size = new System.Drawing.Size(53, 13);
            this.pushButtonStateLabel.TabIndex = 7;
            this.pushButtonStateLabel.Text = "Unknown";
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 10;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // debugTextBox
            // 
            this.debugTextBox.Location = new System.Drawing.Point(12, 209);
            this.debugTextBox.Multiline = true;
            this.debugTextBox.Name = "debugTextBox";
            this.debugTextBox.ReadOnly = true;
            this.debugTextBox.Size = new System.Drawing.Size(346, 95);
            this.debugTextBox.TabIndex = 8;
            // 
            // debugCollectionTimer
            // 
            this.debugCollectionTimer.Enabled = true;
            this.debugCollectionTimer.Interval = 250;
            this.debugCollectionTimer.Tick += new System.EventHandler(this.debugCollectionTimer_Tick);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 193);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(75, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Debug output:";
            // 
            // n64label
            // 
            this.n64label.AutoSize = true;
            this.n64label.Location = new System.Drawing.Point(115, 100);
            this.n64label.Name = "n64label";
            this.n64label.Size = new System.Drawing.Size(53, 13);
            this.n64label.TabIndex = 11;
            this.n64label.Text = "Unknown";
            this.n64label.Click += new System.EventHandler(this.label4_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(53, 100);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(56, 13);
            this.label5.TabIndex = 10;
            this.label5.Text = "N64 Data:";
            this.label5.Click += new System.EventHandler(this.label5_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(373, 329);
            this.Controls.Add(this.n64label);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.debugTextBox);
            this.Controls.Add(this.pushButtonStateLabel);
            this.Controls.Add(this.ledStateLabel);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.toggleLedStateButton);
            this.Controls.Add(this.linkLabel1);
            this.Controls.Add(this.statusStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Name = "Form1";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "WFF Generic HID Demo 3";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

            }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.LinkLabel linkLabel1;
        private System.Windows.Forms.Button toggleLedStateButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label ledStateLabel;
        private System.Windows.Forms.Label pushButtonStateLabel;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TextBox debugTextBox;
        private System.Windows.Forms.Timer debugCollectionTimer;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label n64label;
        private System.Windows.Forms.Label label5;
        }
    }

