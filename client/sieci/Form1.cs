using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;


namespace sieci
{

    public partial class Form1 : Form
    {
        private const int MAX = 2;
        private string index;
        private Form obj;
        delegate void setThreadedTextBoxCallback(String text);
        delegate void setThreadedStatusLabelCallback(String text);
        delegate void setThreadedButtonCallback(bool status);
        public Form1()
        {
            InitializeComponent();
            this.obj = this;
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void Matrix1_TextChanged(object sender, EventArgs e)
        {

        }

        private void Matrix2_TextChanged(object sender, EventArgs e)
        {

        }

        private void ResultMatrix_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {
        }

        private void setThreadedTextBox(String text)
        {
            if (this.ResultMatrix.InvokeRequired)
            {
                setThreadedTextBoxCallback textBoxCallback = new
                                           setThreadedTextBoxCallback(setThreadedTextBox);
                this.obj.Invoke(textBoxCallback, text);
            }
            else
            {
                this.ResultMatrix.Text = text;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string matrix1 = Matrix1.Text;
            string myArrayString1 = matrix1.Replace(" ", ",");

            string matrix2 = Matrix2.Text;
            string myArrayString2 = matrix2.Replace(" ", ",");

            index = myArrayString1 + myArrayString2;
            index = index.Replace("\r", "");

            index = stringDecorator(index);

            Socket socketFd = null;
            string ipAddress = IP.Text ; //"192.168.1.109"
            IPEndPoint endPoint = null;
            int port = Int32.Parse(portLabel.Text);


            IPAddress address = IPAddress.Parse(ipAddress);

            socketFd = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            endPoint = new IPEndPoint(address, port);
            socketFd.BeginConnect(endPoint, new AsyncCallback(ConnectCallback), socketFd);


            int a = 6;
        }

        private void ConnectCallback(IAsyncResult ar)
        {
            /* retrieve the socket from the state object */
            Socket socketFd = (Socket)ar.AsyncState;

            /* complete the connection */
            socketFd.EndConnect(ar);

            /* send data */
            SendData(socketFd); ////////////////aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

            /* create the SocketStateObject */
            SocketStateObject state = new SocketStateObject();
            state.m_SocketFd = socketFd;


            /* begin receiving the data */
            socketFd.BeginReceive(state.m_DataBuf, 0, SocketStateObject.BUF_SIZE, 0,
                                    new AsyncCallback(ReceiveCallback), state);

            /////////aaaaaaaaaaaaaaaaaaaaaaaaaaa
        }

        private void SendData(Socket socketFd)
        {
            byte[] dataBuf = Encoding.ASCII.GetBytes(index);

            /* begin sending the date */
            socketFd.BeginSend(dataBuf, 0, dataBuf.Length, 0,
                                new AsyncCallback(SendCallback), socketFd);
        }

        private static void SendCallback(IAsyncResult ar)
        {
            /* retrieve the socket from the ar object */
            Socket socketFd = (Socket)ar.AsyncState;

            /* end pending asynchronous send */
            int bytesSent = socketFd.EndSend(ar);

            Console.WriteLine("\t\tSent {0} bytes to the client\n\tEND connection", bytesSent);
        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            /* retrieve the SocketStateObject */
            SocketStateObject state = (SocketStateObject)ar.AsyncState;
            Socket socketFd = state.m_SocketFd;

            /* read data */
            int size = socketFd.EndReceive(ar);

            if (size > 0)
            {
                state.m_StringBuilder.Append(Encoding.ASCII.GetString(state.m_DataBuf, 0, size));

                /* get the rest of the data */
                socketFd.BeginReceive(state.m_DataBuf, 0, SocketStateObject.BUF_SIZE, 0,
                                        new AsyncCallback(ReceiveCallback), state);
            }
            else
            {
                /* all the data has arrived */
                if (state.m_StringBuilder.Length > 1)
                {
                    string result = state.m_StringBuilder.ToString();
                    result = result.Replace(",", " ");
                    result = result.Replace("\n", "\r\n");
                    setThreadedTextBox(result);
                    /* shutdown and close socket */
                    socketFd.Shutdown(SocketShutdown.Both);
                    socketFd.Close();
                }
            }
        }

        private string stringDecorator(string s)
        {
            int size = s.Length;
            string decorated = size.ToString();
            decorated += "\n" + s;
            return decorated;
        }

        public class SocketStateObject
        {
            public const int BUF_SIZE = 2;
            public byte[] m_DataBuf = new byte[BUF_SIZE];
            public StringBuilder m_StringBuilder = new StringBuilder();
            public Socket m_SocketFd = null;
        }
    }
}

