using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Text;
using System.Windows.Forms;


namespace Pizarra
{	
	public partial class MainForm : Form
	{
		SerialPort sp1 = null;
		
		string cad = "";			
		
		public MainForm()
		{			
			InitializeComponent();		
		}
		
		public void tx_char(SerialPort SBUF, byte[] x)
		{
			if (SBUF == null ) return;			
			if ( SBUF.IsOpen == false )
				SBUF.Open();		
			
			SBUF.Write(x, 0, x.Length);	
		}		
				
		void MainFormLoad(object sender, EventArgs e)
		{
			string[] ports = SerialPort.GetPortNames(); 
			comboBox1.Items.Clear();

						
			foreach(string port in ports) 
			{ 
				comboBox1.Items.Add(port); 
			} 
			
			if (comboBox1.Items.Count > 0)
			{
				comboBox1.Text = comboBox1.Items[0].ToString();
			}
			else
			{
				conButton.Enabled = false;
			}
			
			comboBox2.Items.Add("1200");
			comboBox2.Items.Add("2400"); 
			comboBox2.Items.Add("4800"); 
			comboBox2.Items.Add("9600"); 
			comboBox2.Items.Add("19200"); 
			comboBox2.Text = "9600"; 	
			comboBox3.Items.Add(Parity.Even.ToString()); 
			comboBox3.Items.Add(Parity.Mark.ToString()); 
			comboBox3.Items.Add(Parity.None.ToString()); 
			comboBox3.Items.Add(Parity.Odd.ToString()); 
			comboBox3.Items.Add(Parity.Space.ToString()); 
			comboBox3.Text = "None";
		}							
		
		private void port_DataReceived(object sender, SerialDataReceivedEventArgs e) 
		{ 			 					
			cad = sp1.ReadExisting();			
		}
		
		void MainFormFormClosing(object sender, FormClosingEventArgs e)
		{
			if ( sp1 != null && sp1.IsOpen )
				sp1.Close();			
		}
		
		void ConButtonClick(object sender, EventArgs e)
		{
			if ( sp1 == null || sp1.IsOpen == false )
			{
				Parity Paridad = Parity.None; 
				switch (comboBox3.Text)
				{
					case "Even" : Paridad = Parity.Even; 
								  break;								  
					case "Mark" : Paridad = Parity.Mark; 
								  break;
					case "Odd"  : Paridad = Parity.Odd; 
								  break;
					case "Space": Paridad = Parity.Space; 					
								  break;																	  
					default		: Paridad = Parity.None;
								  break;
				}													
				sp1 = new SerialPort(comboBox1.Text,Convert.ToInt16(comboBox2.Text),Paridad,8,StopBits.One);
				sp1.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived); 
				sp1.Open();
				comboBox1.Enabled = false;
				comboBox2.Enabled = false;
				comboBox3.Enabled = false;
				textBox1.Enabled = true;
				button2.Enabled = true;
				btnHora.Enabled = true;
				conButton.Text = "Desconectar";
			}
			else
			{				
				sp1.Close();
				comboBox1.Enabled = true;
				comboBox2.Enabled = true;
				comboBox3.Enabled = true;
				textBox1.Enabled = false;
				button2.Enabled = false;
				btnHora.Enabled = false;
				conButton.Text = "Conectar";
			}
		}

		void Button2Click(object sender, EventArgs e)
		{
			if ( sp1 != null && sp1.IsOpen == true && textBox1.Text.Length != 0 )
			{			
				
				string f  = textBox1.Text.ToString();
				byte[] frame = null;
				frame = new byte[f.Length + 4];
				
				frame[0] = 0x3A;
				frame[1] = 0x43;
				frame[2] = (byte) f.Length;
				int k = 3;
				for(int i = 0; i < f.Length ; i++)
				{
					switch(f[i])
					{
						case '-' : frame[k++] = (byte)0x10; break;
		                case 'ñ' : frame[k++] = (byte)0x11; break;
                		case 'Ñ' : frame[k++] = (byte)0x11; break;
                		case ' ' : frame[k++] = (byte)0x12; break;
						default: frame[k++] = (byte)f[i]; break;	
					}
				}
				frame[k++] = 0x0D;
				for(int i = 0; i < frame.Length ; i++)
				{
					textBox6.AppendText(frame[i].ToString());
				}
				
				
				this.tx_char(sp1,frame);
			}	 						
		}
		
		void Button5Click(object sender, System.EventArgs e)
		{
			textBox6.Clear();
		}
	
		void btnHoraClick(object sender, EventArgs e)
		{
			DateTime tiempo = DateTime.Now;
			byte[] frame = null;
			
			frame = new byte[11];
			
			frame[0] = 0x3A;
			frame[1] = (byte)'H';
	
			frame[2] = (byte)(tiempo.Second%60);
			frame[3] = (byte)tiempo.Minute;
			frame[4] = (byte)tiempo.Hour;
			if(frame[4]  > 12 || frame[4] == 0 )
			{
				if(frame[4] == 0)
				{
					frame[4] = 12;
					frame[9] = (byte)'P';
				}
				else
				{
					frame[4] -= 12;
					frame[9] = (byte)'P';
				}
			}
			else
			{
				frame[9] = (byte)'A';
			}

			
			frame[5] = (byte)(tiempo.DayOfWeek + 1);
			frame[6] = (byte)(tiempo.Day);
			frame[7] = (byte)(tiempo.Month);
			frame[8] = (byte)(tiempo.Year%100);
			frame[10] = 0x0D;
			

			for(int i = 0; i < frame.Length ; i++)
			{
				textBox6.AppendText(frame[i].ToString());
			}
			
			this.tx_char(sp1,frame);
		}
		
		void AboutClick(object sender, System.EventArgs e)
		{
			MessageBox.Show("Pizarra Electrónica Informativa" + "\nUCLV-FIE" +
			                "\t14/06/2017" + "\nContacto: ragonzalez@uclv.cu ");
		}
		void GroupBox3Enter(object sender, EventArgs e)
		{
	
		}

	}	
}
