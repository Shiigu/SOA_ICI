using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SOA_ArduinoMock
{
    public static class ThreadHelperClass
    {
        delegate void SetTextCallback(Form f, Control ctrl, string text);
        delegate void AppendTextCallback(Form f, Control ctrl, string text);

        public static void SetText(Form form, Control ctrl, string text)
        {
            // InvokeRequired required compares the thread ID of the 
            // calling thread to the thread ID of the creating thread. 
            // If these threads are different, it returns true. 
            if (ctrl.InvokeRequired)
            {
                var d = new SetTextCallback(SetText);
                form.Invoke(d, form, ctrl, text);
            }
            else
            {
                ctrl.Text = text;
            }
        }
        
        public static void AppendText(Form form, Control ctrl, string text)
        {
            // InvokeRequired required compares the thread ID of the 
            // calling thread to the thread ID of the creating thread. 
            // If these threads are different, it returns true. 
            if (ctrl.InvokeRequired)
            {
                var d = new AppendTextCallback(AppendText);
                form.Invoke(d, form, ctrl, text);
            }
            else
            {
                ((TextBox) ctrl).AppendText(text);
            }
        }
    }
}
