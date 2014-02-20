namespace SWServerDLG
{
    partial class ServMainDialog
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
            this.startButton = new System.Windows.Forms.Button();
            this.stopServiceButton = new System.Windows.Forms.Button();
            this.servNameTxtBx = new System.Windows.Forms.TextBox();
            this.StatusBarLbl = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // startButton
            // 
            this.startButton.Location = new System.Drawing.Point(39, 59);
            this.startButton.Name = "startButton";
            this.startButton.Size = new System.Drawing.Size(75, 23);
            this.startButton.TabIndex = 0;
            this.startButton.Text = "start";
            this.startButton.UseVisualStyleBackColor = true;
            this.startButton.Click += new System.EventHandler(this.startButton_Click);
            // 
            // stopServiceButton
            // 
            this.stopServiceButton.Location = new System.Drawing.Point(138, 59);
            this.stopServiceButton.Name = "stopServiceButton";
            this.stopServiceButton.Size = new System.Drawing.Size(75, 23);
            this.stopServiceButton.TabIndex = 1;
            this.stopServiceButton.Text = "stop";
            this.stopServiceButton.UseVisualStyleBackColor = true;
            this.stopServiceButton.Click += new System.EventHandler(this.stopServiceButton_Click);
            // 
            // servNameTxtBx
            // 
            this.servNameTxtBx.Location = new System.Drawing.Point(39, 33);
            this.servNameTxtBx.Name = "servNameTxtBx";
            this.servNameTxtBx.Size = new System.Drawing.Size(174, 20);
            this.servNameTxtBx.TabIndex = 2;
            // 
            // StatusBarLbl
            // 
            this.StatusBarLbl.AutoSize = true;
            this.StatusBarLbl.Location = new System.Drawing.Point(36, 85);
            this.StatusBarLbl.Name = "StatusBarLbl";
            this.StatusBarLbl.Size = new System.Drawing.Size(0, 13);
            this.StatusBarLbl.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(36, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(121, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Type service name here";
            // 
            // ServMainDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(262, 106);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.StatusBarLbl);
            this.Controls.Add(this.servNameTxtBx);
            this.Controls.Add(this.stopServiceButton);
            this.Controls.Add(this.startButton);
            this.Name = "ServMainDialog";
            this.Text = "ServiceManager";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button startButton;
        private System.Windows.Forms.Button stopServiceButton;
        private System.Windows.Forms.TextBox servNameTxtBx;
        private System.Windows.Forms.Label StatusBarLbl;
        private System.Windows.Forms.Label label1;
    }
}

