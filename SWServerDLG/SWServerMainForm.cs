using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data.OleDb;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;

namespace SWServerDLG
{
    public delegate void _IServiceServerObjectImplEvents_ReportErrorEventHandler(String Text);
    public partial class ServMainDialog : Form
    {
        static private InteropServiceServerLib.ServiceServerObjectImpl mIServer = null;

        private static bool isValidString(string uString){
            var reg = new Regex("^[a-zA-Z0-9]*$");
            if (!reg.IsMatch(uString))
            {

                MessageBox.Show("An error has occured: invalid simbols");
                return false;
            }

            if (String.IsNullOrEmpty(uString))
            {
                MessageBox.Show("An error has occured:  please enter service name");
                return false;
            }
   
            return true;
        }

        public ServMainDialog()
        {
            InitializeComponent();
            try
            {
                mIServer = new InteropServiceServerLib.ServiceServerObjectImpl();
            }
            catch (AccessViolationException) {

                MessageBox.Show("An error has occured: unable to link ServiceServer library");
            }
            if (mIServer != null)
            {
                mIServer.ReportError += new InteropServiceServerLib.
                    _IServiceServerObjectImplEvents_ReportErrorEventHandler(ReportWasEmitted);
            }


        }

        private void startButton_Click(object sender, EventArgs e)
        {
            string serviceName = servNameTxtBx.Text;

            if (isValidString(serviceName))
            {
                mIServer.startService(serviceName);
            }
            

        }

        static void ReportWasEmitted(String Text)
        {
            MessageBox.Show(Text);
        }

        private void stopServiceButton_Click(object sender, EventArgs e)
        {
            string serviceName = servNameTxtBx.Text;

            if (isValidString(serviceName))
            {
                mIServer.stopService(serviceName);
            }
        }


    }
}
