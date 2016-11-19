namespace SOA_ArduinoMock
{
    partial class frmMain
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
            this.grpMode = new System.Windows.Forms.GroupBox();
            this.rdbPost = new System.Windows.Forms.RadioButton();
            this.rdbGet = new System.Windows.Forms.RadioButton();
            this.grpSends = new System.Windows.Forms.GroupBox();
            this.rdbHumidity = new System.Windows.Forms.RadioButton();
            this.rdbTemperature = new System.Windows.Forms.RadioButton();
            this.btnStart = new System.Windows.Forms.Button();
            this.txtMockStatus = new System.Windows.Forms.TextBox();
            this.btnExit = new System.Windows.Forms.Button();
            this.grpRead = new System.Windows.Forms.GroupBox();
            this.txtTimeout = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.btnStop = new System.Windows.Forms.Button();
            this.txtWaitTime = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.grpMode.SuspendLayout();
            this.grpSends.SuspendLayout();
            this.grpRead.SuspendLayout();
            this.SuspendLayout();
            // 
            // grpMode
            // 
            this.grpMode.Controls.Add(this.rdbPost);
            this.grpMode.Controls.Add(this.rdbGet);
            this.grpMode.Location = new System.Drawing.Point(13, 15);
            this.grpMode.Name = "grpMode";
            this.grpMode.Size = new System.Drawing.Size(181, 57);
            this.grpMode.TabIndex = 0;
            this.grpMode.TabStop = false;
            this.grpMode.Text = "Arduino role (HTTP Request type)";
            // 
            // rdbPost
            // 
            this.rdbPost.AutoSize = true;
            this.rdbPost.Location = new System.Drawing.Point(83, 25);
            this.rdbPost.Name = "rdbPost";
            this.rdbPost.Size = new System.Drawing.Size(54, 17);
            this.rdbPost.TabIndex = 1;
            this.rdbPost.Text = "POST";
            this.rdbPost.UseVisualStyleBackColor = true;
            // 
            // rdbGet
            // 
            this.rdbGet.AutoSize = true;
            this.rdbGet.Checked = true;
            this.rdbGet.Location = new System.Drawing.Point(6, 25);
            this.rdbGet.Name = "rdbGet";
            this.rdbGet.Size = new System.Drawing.Size(47, 17);
            this.rdbGet.TabIndex = 0;
            this.rdbGet.TabStop = true;
            this.rdbGet.Text = "GET";
            this.rdbGet.UseVisualStyleBackColor = true;
            this.rdbGet.CheckedChanged += new System.EventHandler(this.rdbGet_CheckedChanged);
            // 
            // grpSends
            // 
            this.grpSends.Controls.Add(this.rdbHumidity);
            this.grpSends.Controls.Add(this.rdbTemperature);
            this.grpSends.Location = new System.Drawing.Point(207, 15);
            this.grpSends.Name = "grpSends";
            this.grpSends.Size = new System.Drawing.Size(203, 56);
            this.grpSends.TabIndex = 1;
            this.grpSends.TabStop = false;
            this.grpSends.Text = "Arduino POST parameters";
            this.grpSends.Visible = false;
            // 
            // rdbHumidity
            // 
            this.rdbHumidity.AutoSize = true;
            this.rdbHumidity.Location = new System.Drawing.Point(110, 25);
            this.rdbHumidity.Name = "rdbHumidity";
            this.rdbHumidity.Size = new System.Drawing.Size(65, 17);
            this.rdbHumidity.TabIndex = 1;
            this.rdbHumidity.Text = "Humidity";
            this.rdbHumidity.UseVisualStyleBackColor = true;
            // 
            // rdbTemperature
            // 
            this.rdbTemperature.AutoSize = true;
            this.rdbTemperature.Checked = true;
            this.rdbTemperature.Location = new System.Drawing.Point(11, 25);
            this.rdbTemperature.Name = "rdbTemperature";
            this.rdbTemperature.Size = new System.Drawing.Size(85, 17);
            this.rdbTemperature.TabIndex = 0;
            this.rdbTemperature.TabStop = true;
            this.rdbTemperature.Text = "Temperature";
            this.rdbTemperature.UseVisualStyleBackColor = true;
            // 
            // btnStart
            // 
            this.btnStart.Location = new System.Drawing.Point(13, 78);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(104, 36);
            this.btnStart.TabIndex = 2;
            this.btnStart.Text = "Start!";
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // txtMockStatus
            // 
            this.txtMockStatus.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.txtMockStatus.Location = new System.Drawing.Point(12, 129);
            this.txtMockStatus.Multiline = true;
            this.txtMockStatus.Name = "txtMockStatus";
            this.txtMockStatus.ReadOnly = true;
            this.txtMockStatus.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtMockStatus.Size = new System.Drawing.Size(397, 170);
            this.txtMockStatus.TabIndex = 3;
            // 
            // btnExit
            // 
            this.btnExit.Location = new System.Drawing.Point(156, 305);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(99, 48);
            this.btnExit.TabIndex = 4;
            this.btnExit.Text = "Exit";
            this.btnExit.UseVisualStyleBackColor = true;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // grpRead
            // 
            this.grpRead.Controls.Add(this.txtTimeout);
            this.grpRead.Controls.Add(this.label2);
            this.grpRead.Controls.Add(this.label1);
            this.grpRead.Location = new System.Drawing.Point(206, 16);
            this.grpRead.Name = "grpRead";
            this.grpRead.Size = new System.Drawing.Size(203, 56);
            this.grpRead.TabIndex = 2;
            this.grpRead.TabStop = false;
            this.grpRead.Text = "Arduino GET parameters";
            // 
            // txtTimeout
            // 
            this.txtTimeout.Location = new System.Drawing.Point(130, 24);
            this.txtTimeout.Name = "txtTimeout";
            this.txtTimeout.Size = new System.Drawing.Size(40, 20);
            this.txtTimeout.TabIndex = 1;
            this.txtTimeout.Text = "60";
            this.txtTimeout.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.txtTimeout.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtTimeout_KeyPress);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(173, 27);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(24, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "sec";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 27);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(123, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Assume disconnect after";
            // 
            // btnStop
            // 
            this.btnStop.Enabled = false;
            this.btnStop.Location = new System.Drawing.Point(305, 77);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(104, 36);
            this.btnStop.TabIndex = 5;
            this.btnStop.Text = "Stop";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // txtWaitTime
            // 
            this.txtWaitTime.Location = new System.Drawing.Point(221, 87);
            this.txtWaitTime.Name = "txtWaitTime";
            this.txtWaitTime.Size = new System.Drawing.Size(40, 20);
            this.txtWaitTime.TabIndex = 4;
            this.txtWaitTime.Text = "5";
            this.txtWaitTime.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.txtWaitTime.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtWaitTime_KeyPress);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(267, 89);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(24, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "sec";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(123, 90);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(92, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "POST/GET every";
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(422, 358);
            this.Controls.Add(this.txtWaitTime);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.grpRead);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.txtMockStatus);
            this.Controls.Add(this.btnStart);
            this.Controls.Add(this.grpSends);
            this.Controls.Add(this.grpMode);
            this.Name = "frmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "SOA - Arduino Mock";
            this.grpMode.ResumeLayout(false);
            this.grpMode.PerformLayout();
            this.grpSends.ResumeLayout(false);
            this.grpSends.PerformLayout();
            this.grpRead.ResumeLayout(false);
            this.grpRead.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox grpMode;
        private System.Windows.Forms.RadioButton rdbPost;
        private System.Windows.Forms.RadioButton rdbGet;
        private System.Windows.Forms.GroupBox grpSends;
        private System.Windows.Forms.RadioButton rdbHumidity;
        private System.Windows.Forms.RadioButton rdbTemperature;
        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.TextBox txtMockStatus;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.GroupBox grpRead;
        private System.Windows.Forms.TextBox txtTimeout;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.TextBox txtWaitTime;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
    }
}

