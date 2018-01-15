namespace sieci
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
            this.Matrix1 = new System.Windows.Forms.TextBox();
            this.Matrix2 = new System.Windows.Forms.TextBox();
            this.ResultMatrix = new System.Windows.Forms.TextBox();
            this.portLabel = new System.Windows.Forms.TextBox();
            this.IP = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // Matrix1
            // 
            this.Matrix1.Location = new System.Drawing.Point(100, 44);
            this.Matrix1.Multiline = true;
            this.Matrix1.Name = "Matrix1";
            this.Matrix1.Size = new System.Drawing.Size(173, 179);
            this.Matrix1.TabIndex = 0;
            this.Matrix1.TextChanged += new System.EventHandler(this.Matrix1_TextChanged);
            // 
            // Matrix2
            // 
            this.Matrix2.Location = new System.Drawing.Point(305, 44);
            this.Matrix2.Multiline = true;
            this.Matrix2.Name = "Matrix2";
            this.Matrix2.Size = new System.Drawing.Size(173, 179);
            this.Matrix2.TabIndex = 1;
            this.Matrix2.TextChanged += new System.EventHandler(this.Matrix2_TextChanged);
            // 
            // ResultMatrix
            // 
            this.ResultMatrix.Location = new System.Drawing.Point(653, 44);
            this.ResultMatrix.Multiline = true;
            this.ResultMatrix.Name = "ResultMatrix";
            this.ResultMatrix.Size = new System.Drawing.Size(173, 179);
            this.ResultMatrix.TabIndex = 2;
            this.ResultMatrix.TextChanged += new System.EventHandler(this.ResultMatrix_TextChanged);
            // 
            // portLabel
            // 
            this.portLabel.Location = new System.Drawing.Point(305, 297);
            this.portLabel.Name = "portLabel";
            this.portLabel.Size = new System.Drawing.Size(100, 20);
            this.portLabel.TabIndex = 3;
            this.portLabel.TextChanged += new System.EventHandler(this.textBox4_TextChanged);
            // 
            // IP
            // 
            this.IP.Location = new System.Drawing.Point(305, 342);
            this.IP.Name = "IP";
            this.IP.ShortcutsEnabled = false;
            this.IP.Size = new System.Drawing.Size(100, 20);
            this.IP.TabIndex = 4;
            this.IP.TextChanged += new System.EventHandler(this.textBox5_TextChanged);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(305, 384);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(100, 23);
            this.button1.TabIndex = 5;
            this.button1.Text = "GO";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(233, 300);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(40, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "PORT:";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(233, 342);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(20, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "IP:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(169, 28);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(44, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Matrix 1";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(369, 27);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(44, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Matrix 2";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(698, 27);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(96, 13);
            this.label5.TabIndex = 10;
            this.label5.Text = "Multiplication result";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(985, 440);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.IP);
            this.Controls.Add(this.portLabel);
            this.Controls.Add(this.ResultMatrix);
            this.Controls.Add(this.Matrix2);
            this.Controls.Add(this.Matrix1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox Matrix1;
        private System.Windows.Forms.TextBox Matrix2;
        private System.Windows.Forms.TextBox ResultMatrix;
        private System.Windows.Forms.TextBox portLabel;
        private System.Windows.Forms.TextBox IP;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
    }
}

