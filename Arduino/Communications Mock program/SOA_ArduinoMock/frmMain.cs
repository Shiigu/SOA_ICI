using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SOA_ArduinoMock
{
    public partial class frmMain : Form
    {
        private Thread _httpThread;

        private const string DweetPostHttp = "https://dweet.io/dweet/for/soa-ici-sensor";
        private const string DweetGetHttp = "https://dweet.io/get/latest/dweet/for/soa-ici-android";

        public frmMain()
        {
            InitializeComponent();
        }

        #region Methods

        private void SetGroupBox()
        {
            grpSends.Visible = rdbPost.Checked;
            grpRead.Visible = rdbGet.Checked;
            rdbTemperature.Checked = !grpSends.Visible || rdbTemperature.Checked;
            txtTimeout.Text = !grpRead.Visible ? "60" : txtTimeout.Text;
        }

        private void BeginRequestThread(Action<int> requestType)
        {
            if(IsThreadActive) btnStop_Click(null, null);
            var waitTime = int.Parse(txtWaitTime.Text);
            _httpThread = new Thread(() => requestType(waitTime));
            _httpThread.Start();
            txtMockStatus.AppendText($"Initiating mock Arduino client at {(requestType == GET_Thread ? "GET" : "POST")} mode...\r\n");
        }

        private static double GetRandomNumber(double minimum, double maximum)
        {
            var random = new Random();
            return random.NextDouble() * (maximum - minimum) + minimum;
        }

        private void SwitchEnabledStates()
        {
            grpRead.Enabled = !grpRead.Enabled;
            grpMode.Enabled = !grpMode.Enabled;
            grpSends.Enabled = !grpSends.Enabled;
            btnStart.Enabled = !btnStart.Enabled;
            btnStop.Enabled = !btnStop.Enabled;
            txtWaitTime.Enabled = !txtWaitTime.Enabled;
        }

        private bool IsThreadActive => _httpThread != null && _httpThread.ThreadState != ThreadState.AbortRequested && _httpThread.ThreadState != ThreadState.Aborted;

        private bool MustSendTemperature => rdbPost.Checked && rdbTemperature.Checked;

        private bool MustSendHumidity => rdbPost.Checked && rdbHumidity.Checked;

        #endregion

        #region Events

        private void rdbGet_CheckedChanged(object sender, EventArgs e)
        {
            SetGroupBox();
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            _httpThread?.Abort();
            Application.Exit();
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            var requestToBegin = (rdbGet.Checked) ? GET_Thread : (Action<int>) POST_Thread;
            SwitchEnabledStates();
            BeginRequestThread(requestToBegin);
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            txtMockStatus.AppendText("\r\nStopping current mock Arduino mode...\r\n");
            SwitchEnabledStates();
            _httpThread.Abort();
            _httpThread = null;
        }

        private void txtTimeout_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar))
            {
                e.Handled = true;
            }
        }

        private void txtWaitTime_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar))
            {
                e.Handled = true;
            }
        }
        #endregion

        #region Http Request Threads

        private void GET_Thread(int waitTime)
        {
            var noTimeout = false;
            var lastGet = DateTime.MinValue;
            var timeoutLimit = int.Parse(txtTimeout.Text);
            while (IsThreadActive)
            {
                var response = Http.Get(DweetGetHttp);
                if (response != null)
                {
                    var lastNoTimeout = noTimeout;
                    noTimeout = (DateTime.Now - response.SendingDateTime).TotalSeconds > timeoutLimit;
                    if (!noTimeout)
                    {
                        if (lastNoTimeout)
                            ThreadHelperClass.AppendText(this, txtMockStatus, "\r\nAndroid device regained connection. Resuming...");
                        if (lastGet != response.SendingDateTime)
                        {
                            ThreadHelperClass.AppendText(this, txtMockStatus, $"\r\nRecieving {response.Type} from DWEET.IO - Value: {response.Value}");
                            lastGet = response.SendingDateTime;
                        }
                    }
                    else
                    {
                        if (lastNoTimeout)
                            ThreadHelperClass.AppendText(this, txtMockStatus, $"\r\nAndroid device has not sent information during the last {timeoutLimit} seconds. Waiting...");
                    }
                }
                Thread.Sleep(waitTime * 1000);
            }
        }

        private void POST_Thread(int waitTime)
        {
            var higherLimit = (MustSendTemperature) ? 40 : 100;
            var type = (MustSendTemperature) ? "temperature" : "humidity";
            while (IsThreadActive)
            {
                var value = GetRandomNumber(0, higherLimit);
                var response = Http.Post(DweetPostHttp, new NameValueCollection
                {
                    { "type", type },
                    { "value", value.ToString(CultureInfo.InvariantCulture) }
                });
                ThreadHelperClass.AppendText(this, txtMockStatus, $"\r\nSending {type} to DWEET.IO - Value: {value}");
                Thread.Sleep(waitTime * 1000);
            }
        }

        #endregion

    }
}
