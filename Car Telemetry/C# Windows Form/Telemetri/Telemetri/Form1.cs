using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;

namespace Telemetri
{
    public partial class Form1 : Form
    {        public SerialPort serialPort1;
        public  String line;
        public String[] ports;
        public List<String> telemetry;
        public Random r;

        public Form1()
        {
            telemetry = new List<String>();
            InitializeComponent();
            ports = SerialPort.GetPortNames();
            foreach(String s in ports)
            {
                comboBox1.Items.Add(s);
            }
            comboBox1.SelectedIndex = 0;
            r = new Random();
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            if(serialPort1.IsOpen)
            {
                string line = serialPort1.ReadLine();
                this.BeginInvoke(new LineReceivedEvent(LineReceived), line);
            }
        }

        private delegate void LineReceivedEvent(string line);
        private void LineReceived(string line)
        {
            if (serialPort1.IsOpen)
            {
                textBox1.Text = line;
                telemetry.Add(line);
            }
            //progressBar1.Value = int.Parse(line);
        }

        public void readSerial()
        {
            try
            {
                serialPort1 = new SerialPort();
                serialPort1.BaudRate = 9600;
                serialPort1.PortName = comboBox1.SelectedItem.ToString();
                serialPort1.Open();
                serialPort1.DataReceived += serialPort1_DataReceived;
            }
            catch (ArgumentException e){} 
        }

        private void button3_Click(object sender, EventArgs e)
        {
            readSerial();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            serialPort1.Close();
            textBox1.Text = "";
            int random = 1000000 + (int)(r.NextDouble() * 8999999);
            String path = "..\\..\\..\\" + random.ToString() + ".txt";
            StreamWriter sw = new StreamWriter(path);
            foreach(String s in telemetry)
            {
                sw.WriteLine(s);
            }
            sw.Close();
        }
    }
}
