using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CompilerUI
{
    public partial class Form1 : Form
    {
        CompilerCore.Manager compiler;

        public Form1()
        {
            InitializeComponent();
            compiler = new CompilerCore.Manager();
        }

        private void compileToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            textBox2.Clear();
            textBox3.Clear();
            textBox3.Text = "Iniciando...\r\n";
            string holu = textBox1.Text;
            String[] compDetails = compiler.compile(holu);
            textBox3.Text += "Terminando...\r\n";
            for (int j = 0; j < compiler.getErrorCount(); j++)
            {
                textBox2.Text += compDetails[j] + "\r\n";
            }
            for (int i = compiler.getErrorCount(); i < compDetails.Length; i++)
            {
                textBox3.Text += compDetails[i] + "\r\n";
            }
            if (compiler.getErrorCount() == 0)
            {
                textBox2.Text = "No errors found.\r\n";
            }
            else
            {
                textBox3.Text = "Fix code first.\r\n";
            }
        }
    }
}