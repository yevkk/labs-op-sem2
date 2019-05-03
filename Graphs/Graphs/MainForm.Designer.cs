namespace Graphs
{
    partial class MainForm
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.clearBtn = new System.Windows.Forms.Button();
            this.nodeDestroyBtn = new System.Windows.Forms.Button();
            this.edgeAddBtn = new System.Windows.Forms.Button();
            this.nodeAddBtn = new System.Windows.Forms.Button();
            this.canvas = new System.Windows.Forms.PictureBox();
            this.nodeMoveBtn = new System.Windows.Forms.Button();
            this.colorBtn = new System.Windows.Forms.Button();
            this.colorDialog = new System.Windows.Forms.ColorDialog();
            this.matrixTB = new System.Windows.Forms.TextBox();
            this.BfsBtn = new System.Windows.Forms.Button();
            this.matrixBtn = new System.Windows.Forms.Button();
            this.edgeDestroyBtn = new System.Windows.Forms.Button();
            this.bfsTimer = new System.Windows.Forms.Timer(this.components);
            this.dfsBtn = new System.Windows.Forms.Button();
            this.dfsTimer = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.canvas)).BeginInit();
            this.SuspendLayout();
            // 
            // clearBtn
            // 
            this.clearBtn.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.clearBtn.FlatAppearance.BorderSize = 0;
            this.clearBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.clearBtn.Image = ((System.Drawing.Image)(resources.GetObject("clearBtn.Image")));
            this.clearBtn.Location = new System.Drawing.Point(254, 4);
            this.clearBtn.Name = "clearBtn";
            this.clearBtn.Size = new System.Drawing.Size(43, 43);
            this.clearBtn.TabIndex = 4;
            this.clearBtn.UseVisualStyleBackColor = false;
            this.clearBtn.Click += new System.EventHandler(this.Btn_Click);
            // 
            // nodeDestroyBtn
            // 
            this.nodeDestroyBtn.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.nodeDestroyBtn.FlatAppearance.BorderSize = 0;
            this.nodeDestroyBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.nodeDestroyBtn.Image = ((System.Drawing.Image)(resources.GetObject("nodeDestroyBtn.Image")));
            this.nodeDestroyBtn.Location = new System.Drawing.Point(110, 4);
            this.nodeDestroyBtn.Name = "nodeDestroyBtn";
            this.nodeDestroyBtn.Size = new System.Drawing.Size(43, 43);
            this.nodeDestroyBtn.TabIndex = 3;
            this.nodeDestroyBtn.UseVisualStyleBackColor = false;
            this.nodeDestroyBtn.Click += new System.EventHandler(this.Btn_Click);
            // 
            // edgeAddBtn
            // 
            this.edgeAddBtn.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.edgeAddBtn.FlatAppearance.BorderSize = 0;
            this.edgeAddBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.edgeAddBtn.Image = ((System.Drawing.Image)(resources.GetObject("edgeAddBtn.Image")));
            this.edgeAddBtn.Location = new System.Drawing.Point(61, 4);
            this.edgeAddBtn.Name = "edgeAddBtn";
            this.edgeAddBtn.Size = new System.Drawing.Size(43, 43);
            this.edgeAddBtn.TabIndex = 2;
            this.edgeAddBtn.UseVisualStyleBackColor = false;
            this.edgeAddBtn.Click += new System.EventHandler(this.Btn_Click);
            // 
            // nodeAddBtn
            // 
            this.nodeAddBtn.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.nodeAddBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.nodeAddBtn.Image = ((System.Drawing.Image)(resources.GetObject("nodeAddBtn.Image")));
            this.nodeAddBtn.Location = new System.Drawing.Point(12, 4);
            this.nodeAddBtn.Name = "nodeAddBtn";
            this.nodeAddBtn.Size = new System.Drawing.Size(43, 43);
            this.nodeAddBtn.TabIndex = 1;
            this.nodeAddBtn.Tag = "";
            this.nodeAddBtn.UseVisualStyleBackColor = false;
            this.nodeAddBtn.Click += new System.EventHandler(this.Btn_Click);
            // 
            // canvas
            // 
            this.canvas.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.canvas.Location = new System.Drawing.Point(12, 53);
            this.canvas.Name = "canvas";
            this.canvas.Size = new System.Drawing.Size(935, 542);
            this.canvas.TabIndex = 0;
            this.canvas.TabStop = false;
            this.canvas.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseClick);
            this.canvas.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseMove);
            // 
            // nodeMoveBtn
            // 
            this.nodeMoveBtn.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.nodeMoveBtn.FlatAppearance.BorderSize = 0;
            this.nodeMoveBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.nodeMoveBtn.Image = ((System.Drawing.Image)(resources.GetObject("nodeMoveBtn.Image")));
            this.nodeMoveBtn.Location = new System.Drawing.Point(205, 4);
            this.nodeMoveBtn.Name = "nodeMoveBtn";
            this.nodeMoveBtn.Size = new System.Drawing.Size(43, 43);
            this.nodeMoveBtn.TabIndex = 5;
            this.nodeMoveBtn.UseVisualStyleBackColor = false;
            this.nodeMoveBtn.Click += new System.EventHandler(this.Btn_Click);
            // 
            // colorBtn
            // 
            this.colorBtn.BackColor = System.Drawing.Color.Black;
            this.colorBtn.Location = new System.Drawing.Point(303, 4);
            this.colorBtn.Name = "colorBtn";
            this.colorBtn.Size = new System.Drawing.Size(43, 43);
            this.colorBtn.TabIndex = 6;
            this.colorBtn.UseVisualStyleBackColor = false;
            this.colorBtn.MouseClick += new System.Windows.Forms.MouseEventHandler(this.ColorBtn_MouseClick);
            // 
            // matrixTB
            // 
            this.matrixTB.Font = new System.Drawing.Font("Rockwell", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.matrixTB.Location = new System.Drawing.Point(953, 53);
            this.matrixTB.MaximumSize = new System.Drawing.Size(350, 350);
            this.matrixTB.MinimumSize = new System.Drawing.Size(350, 350);
            this.matrixTB.Multiline = true;
            this.matrixTB.Name = "matrixTB";
            this.matrixTB.Size = new System.Drawing.Size(350, 350);
            this.matrixTB.TabIndex = 7;
            // 
            // BfsBtn
            // 
            this.BfsBtn.FlatAppearance.BorderSize = 0;
            this.BfsBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.BfsBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.BfsBtn.Location = new System.Drawing.Point(383, 5);
            this.BfsBtn.Name = "BfsBtn";
            this.BfsBtn.Size = new System.Drawing.Size(47, 42);
            this.BfsBtn.TabIndex = 8;
            this.BfsBtn.Text = "BFS";
            this.BfsBtn.UseVisualStyleBackColor = true;
            this.BfsBtn.Click += new System.EventHandler(this.BfsBtn_Click);
            // 
            // matrixBtn
            // 
            this.matrixBtn.FlatAppearance.BorderSize = 0;
            this.matrixBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.matrixBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.matrixBtn.Location = new System.Drawing.Point(486, 4);
            this.matrixBtn.Name = "matrixBtn";
            this.matrixBtn.Size = new System.Drawing.Size(47, 42);
            this.matrixBtn.TabIndex = 9;
            this.matrixBtn.Text = "MTRX";
            this.matrixBtn.UseVisualStyleBackColor = true;
            this.matrixBtn.Click += new System.EventHandler(this.MatrixBtn_Click);
            // 
            // edgeDestroyBtn
            // 
            this.edgeDestroyBtn.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.edgeDestroyBtn.FlatAppearance.BorderSize = 0;
            this.edgeDestroyBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.edgeDestroyBtn.Image = ((System.Drawing.Image)(resources.GetObject("edgeDestroyBtn.Image")));
            this.edgeDestroyBtn.Location = new System.Drawing.Point(156, 5);
            this.edgeDestroyBtn.Name = "edgeDestroyBtn";
            this.edgeDestroyBtn.Size = new System.Drawing.Size(43, 43);
            this.edgeDestroyBtn.TabIndex = 10;
            this.edgeDestroyBtn.UseVisualStyleBackColor = false;
            this.edgeDestroyBtn.Click += new System.EventHandler(this.Btn_Click);
            // 
            // bfsTimer
            // 
            this.bfsTimer.Interval = 500;
            this.bfsTimer.Tick += new System.EventHandler(this.BfsTimer_Tick);
            // 
            // dfsBtn
            // 
            this.dfsBtn.FlatAppearance.BorderSize = 0;
            this.dfsBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.dfsBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.dfsBtn.Location = new System.Drawing.Point(436, 6);
            this.dfsBtn.Name = "dfsBtn";
            this.dfsBtn.Size = new System.Drawing.Size(47, 42);
            this.dfsBtn.TabIndex = 11;
            this.dfsBtn.Text = "DFS";
            this.dfsBtn.UseVisualStyleBackColor = true;
            this.dfsBtn.Click += new System.EventHandler(this.DfsBtn_Click);
            // 
            // dfsTimer
            // 
            this.dfsTimer.Interval = 500;
            this.dfsTimer.Tick += new System.EventHandler(this.DfsTimer_Tick);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1351, 607);
            this.Controls.Add(this.dfsBtn);
            this.Controls.Add(this.edgeDestroyBtn);
            this.Controls.Add(this.matrixBtn);
            this.Controls.Add(this.BfsBtn);
            this.Controls.Add(this.matrixTB);
            this.Controls.Add(this.colorBtn);
            this.Controls.Add(this.nodeMoveBtn);
            this.Controls.Add(this.clearBtn);
            this.Controls.Add(this.nodeDestroyBtn);
            this.Controls.Add(this.edgeAddBtn);
            this.Controls.Add(this.nodeAddBtn);
            this.Controls.Add(this.canvas);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainForm";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.canvas)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox canvas;
        private System.Windows.Forms.Button nodeAddBtn;
        private System.Windows.Forms.Button edgeAddBtn;
        private System.Windows.Forms.Button nodeDestroyBtn;
        private System.Windows.Forms.Button clearBtn;
        private System.Windows.Forms.Button nodeMoveBtn;
        private System.Windows.Forms.Button colorBtn;
        private System.Windows.Forms.ColorDialog colorDialog;
        private System.Windows.Forms.TextBox matrixTB;
        private System.Windows.Forms.Button BfsBtn;
        private System.Windows.Forms.Button matrixBtn;
        private System.Windows.Forms.Button edgeDestroyBtn;
        private System.Windows.Forms.Timer bfsTimer;
        private System.Windows.Forms.Button dfsBtn;
        private System.Windows.Forms.Timer dfsTimer;
    }
}

