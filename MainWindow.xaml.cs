using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.IO;
using System.Threading; 
using System.Speech.Synthesis;


namespace Calendar_wpf
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        string m_sReadData;
        Thread ReadTheHoliday;
        string m_MonthName;
        public MainWindow()
        {
            InitializeComponent();
            for (int nIndex = 2019; nIndex < 2050; ++nIndex)
            {
                Year.Items.Add(nIndex);
            }
            for (int nIndex = 1; nIndex < 13; ++nIndex)
            {
                Month.Items.Add(nIndex);
            }
            HolidayType.Items.Add("OPT");
            HolidayType.Items.Add("HOL");
            HolidayType.Items.Add("BOTH");
        }

        //private void DisplayInListBox()
        //{
        //    DisplayBox.Items.Add(m_sReadData);
        //}
        private void DisplayInListBox(string strReadData)
        {
            if (Application.Current.Dispatcher.CheckAccess())
            {
                DisplayBox.Items.Add(strReadData);
            }
            else
            {
                Application.Current.Dispatcher.BeginInvoke(
                  System.Windows.Threading.DispatcherPriority.Background,
                  new Action(() =>
                  {
                      this.DisplayBox.Items.Add(strReadData);
                  }));
            }
        }
        private void VoiceOutput(string strReadData)
        {
            SpeechSynthesizer synthesizer = new SpeechSynthesizer();
            synthesizer.Volume = 100;  // 0...100
            synthesizer.Rate = -3;     // -10...10
            synthesizer.SelectVoiceByHints(VoiceGender.Female);
            // Synchronous
            //synthesizer.Speak("Hello World");
            synthesizer.Speak(strReadData);
            //// Asynchronous
            //synthesizer.SpeakAsync("Hello World");
        }
        private void ReadFile(string strfilepath, string strHolidayType)
        {
            // the path of the file
            // strfilepath = "D:\\friendinfo.txt";
            FileStream inFile = new FileStream(strfilepath, FileMode.Open, FileAccess.Read);
            StreamReader reader = new StreamReader(inFile);
            //Thread thread = new Thread(DisplayInListBox);
            //Thread thread2 = new Thread(VoiceOutput);
            string sReadData;
            // string sReadDataMonthWise;

            try
            {
                //the program reads the record and displays it on the screen
                sReadData = reader.ReadLine();
                while ( sReadData != null)
                {
                    if ( "BOTH" == strHolidayType)
                    {
                        sReadData = sReadData.Substring(sReadData.IndexOf(':') + 1);
                        DisplayInListBox(sReadData);
                        VoiceOutput(sReadData);
                    }
                    else if( sReadData.Contains(strHolidayType))
                    {
                        sReadData = sReadData.Substring(sReadData.IndexOf(':') + 1);
                        //sReadDataMonthWise = sReadData.Substring(sReadData.IndexOf('-') + 1);
                        //sReadDataMonthWise = sReadDataMonthWise.Substring(0,sReadDataMonthWise.IndexOf('-'));
                        if (sReadData.Contains(m_MonthName))
                        {
                            //MessageBox.Show(sReadDataMonthWise);
                            //MessageBox.Show(sReadData);
                            DisplayInListBox(sReadData);
                            VoiceOutput(sReadData);
                        }
                    }
                    sReadData = reader.ReadLine();
                }
            }
            finally
            {
                //after the record is done being read, the progam closes
                reader.Close();
                inFile.Close();
                if( ReadTheHoliday.IsAlive )
                {

                    ReadTheHoliday.Abort();
                }
            }
        }

        private void ReadTheTextFile(string strPath, string strHolidayType)
        {
            switch (strHolidayType)
            {
                case "OPT":
                    {
                        DisplayBox.Items.Add("Displaying the Optional Holidays");
                        break;
                    }
                case "HOL":
                    {
                        //ReadFile(strPath, "H");
                        DisplayBox.Items.Add("Displaying the Holidays");
                        break;
                    }
                case "BOTH":
                    {
                        //ReadFile(strPath, "H");
                        DisplayBox.Items.Add("Displaying the Holidays");
                        break;
                    }
                default:
                    break;
            }
            ReadTheHoliday = new Thread(() => ReadFile(strPath, strHolidayType));
            ReadTheHoliday.Start();
        }


        private void OnClickSay(object sender, RoutedEventArgs e)
        {
            DisplayBox.Items.Clear();
            if( null == Year.SelectedValue | null == Month.SelectedValue | null == HolidayType.SelectedValue )
            {
                Window myMessageBox = new Window();
                myMessageBox.WindowStartupLocation = WindowStartupLocation.CenterScreen;
                myMessageBox.Show();
                //MessageBox.Show(this, "Please enetr valid inputs");
                return;
            }
            string sYear  = Year.SelectedItem.ToString();
            sYear += ".txt";
            string sMonth = Month.SelectedItem.ToString();
            string sHolidayType = HolidayType.SelectedItem.ToString();
            var path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory,sYear);
            //string path = "D:\\temp\\" + sYear + ".txt";
            int nMonth = Int32.Parse(sMonth);
            switch( nMonth )
            {
                case 1:
                    {
                        m_MonthName = "Jan";
                        break;
                    }
                case 2:
                    {
                        m_MonthName = "Feb";
                        break;
                    }
                case 3:
                    {
                        m_MonthName = "Mar";
                        break;
                    }
                case 4:
                    {
                        m_MonthName = "Apr";
                        break;
                    }
                case 5:
                    {
                        m_MonthName = "May";
                        break;
                    }
                case 6:
                    {
                        m_MonthName = "Jun";
                        break;
                    }
                case 7:
                    {
                        m_MonthName = "Jul";
                        break;
                    }
                case 8:
                    {
                        m_MonthName = "Aug";
                        break;
                    }
                case 9:
                    {
                        m_MonthName = "Sep";
                        break;
                    }
                case 10:
                    {
                        m_MonthName = "Oct";
                        break;
                    }
                case 11:
                    {
                        m_MonthName = "Nov";
                        break;
                    }
               case 12:
                    {
                        m_MonthName = "Dec";
                        break;
                    }

                default:
                    break;
            }
            if (File.Exists( path))
            {
                ReadTheTextFile(path, sHolidayType);

            }
            
        }
    }
}
