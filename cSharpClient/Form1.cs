using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;

namespace cSharpClient
{
    public partial class Form1 : Form
    {
        Socket reciever = null;
        Socket senderr = null;
        public Form1()
        {
            InitializeComponent();
        }

        private void btnSendMessage_Click(object sender, EventArgs e)
        {
            senderr = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            senderr.Connect("127.0.0.1",54000);
            senderr.Send(Encoding.ASCII.GetBytes("MessageTo:-:"+txtUserName.Text+":-:"+lbOnlineUsers.SelectedItem+":-:" + txtMessage.Text));
            senderr.Close();
        }

        void WaitForMessages()
        {
            while (true)
            {
                byte[] res = new byte[4096];
                int size = reciever.Receive(res);
                object[] args = new Object[1];
                Invoke(new Action(() => this.ResponseServer(Encoding.ASCII.GetString(res,0,size-1))));
            }
        }
        void ResponseServer(string message)
        {
            string[] res = message.Split(new[] { ":-:" }, StringSplitOptions.None);
            if(res[0] == "ONLINE_USERS")
            {
                string[] users = res[3].Split(':');
                for (int i = 0; i < users.Length; i++)
                {
                    if(res[i] != txtUserName.Text)
                    {
                        lbOnlineUsers.Items.Add(users[i]);
                    }
                }
            }
            else if(res[0] == "NEW_ONLINE_USER")
            {
                lbOnlineUsers.Items.Add(res[3]);
            }
            else
            {
                txtMessages.Text += message + Environment.NewLine;
            }
        }

        private void btnLogIn_Click(object sender, EventArgs e)
        {
            reciever = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            reciever.Connect("127.0.0.1", 54000);
            reciever.Send(Encoding.ASCII.GetBytes("LogIn:-:"+txtUserName.Text+":-::-:"));
            byte[] bytes = new byte[4069];
            int size = reciever.Receive(bytes);
            string[] res = Encoding.ASCII.GetString(bytes,0,size-1).Split(new[] { ":-:" }, StringSplitOptions.None);
            MessageBox.Show(res[0]);
            if (res[0] == "LOGIN_OK")
            {
                Task t = new Task(WaitForMessages);
                t.Start();
                panel1.Hide();
            }
            else
            {
                reciever.Close();
            }
        }
    }
}
