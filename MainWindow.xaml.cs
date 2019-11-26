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
using System.Windows.Shapes;
using System.Windows.Threading;

namespace VideoPlayer_sample_wpf
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Uri m_Filepath;
        private TimeSpan TotalTime;
        private DispatcherTimer timerVideoTime;
        private bool bValue = false;
        private bool bVoiceCtrlClicked = false;
        private double MedaiaVolume = 0;
        public MainWindow()
        {
            InitializeComponent();
            VideoPlayer.Volume = (double)volumeSlider.Value;
            timelineSlider.AddHandler(MouseLeftButtonUpEvent,
                      new MouseButtonEventHandler(timeSlider_MouseLeftButtonUp),
                      true);
        }
        private void timeSlider_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            timerVideoTime.Stop();
            if (TotalTime.TotalSeconds > 0)
            {
                VideoPlayer.Position = TimeSpan.FromSeconds(TotalTime.TotalSeconds / (100/timelineSlider.Value));
            }
            timerVideoTime.Start();
        }


        private void Element_MediaOpened(object sender, EventArgs e)
        {
            TotalTime = VideoPlayer.NaturalDuration.TimeSpan;

            // Create a timer that will update the counters and the time slider
            timerVideoTime = new DispatcherTimer();
            timerVideoTime.Interval = TimeSpan.FromMilliseconds(1500);
            timerVideoTime.Tick += new EventHandler(timer_Tick);
            timerVideoTime.Start();
        }
        private void OnMediaEnd(object sender, EventArgs e)
        {
            VideoPlayer.Stop();
            timelineSlider.Value = 0;
        }
        void timer_Tick(object sender, EventArgs e)
        {
            // Check if the movie finished calculate it's total time
            if (VideoPlayer.NaturalDuration.TimeSpan.TotalSeconds > 0)
            {
                if (TotalTime.TotalSeconds > 0)
                {
                    // Updating time slider
                    timelineSlider.Value = VideoPlayer.Position.TotalSeconds / TotalTime.TotalSeconds*100;
                }
            }
        }
        private void OnclickPlay(object sender, RoutedEventArgs e)
        {
            // Create a timer that will update the counters and the time slider
            bValue = true;
            if( null != m_Filepath)
            {
                VideoPlayer.Source = m_Filepath;
            }
            VideoPlayer.Volume = 0.0;
            volumeSlider.Value = 0.0;
            m_Filepath = VideoPlayer.Source;
            VideoPlayer.Play();
            //VideoPlayer.Close();
        }
        private void OnclickPause(object sender, RoutedEventArgs e)
        {
            VideoPlayer.Pause();
        }
        private void OnclickStop(object sender, RoutedEventArgs e)
        {
            timerVideoTime.Stop();
            VideoPlayer.Stop();
            VideoPlayer.Source = null;
            VideoPlayer.Volume = 0.0;
            volumeSlider.Value = 0.0;
            VideoPlayer.Close();
            timelineSlider.Value = 0;
        }
        private void OnclickBrowse(object sender, RoutedEventArgs e)
        {
            if (bValue)
            {
                timerVideoTime.Stop();
                int SliderValue = (int)timelineSlider.Value;

                // Overloaded constructor takes the arguments days, hours, minutes, seconds, milliseconds.
                // Create a TimeSpan with miliseconds equal to the slider value.
                TimeSpan ts = new TimeSpan(0, 0, 0, 0, SliderValue);
                VideoPlayer.Position = ts;

            }
            VideoPlayer.Volume = 0.0; 
            Microsoft.Win32.OpenFileDialog openFileDlg = new Microsoft.Win32.OpenFileDialog();

            // Launch OpenFileDialog by calling ShowDialog method
            Nullable<bool> result = openFileDlg.ShowDialog();
            // Get the selected file name and display in a TextBox.
            // Load content of file in a TextBlock
            if (result == true)
            {
                openFileDlg.DefaultExt = ".mp4";
                openFileDlg.Multiselect = false;   
                openFileDlg.Filter = "mp4files (*.mp4)|*.mp4|3gpfiles (*.3gp)|*.3gp";
                string filename = openFileDlg.FileName;
                m_Filepath = new Uri(filename);
                VideoPlayer.Source = m_Filepath;
                VideoPlayer.Play();

            }
        }
        private void ChangeMediaVolume(object sender, RoutedPropertyChangedEventArgs<double> args)
        {
            VideoPlayer.Volume = (double)volumeSlider.Value;
        }
        private void onclickVoiceControl(object sender, RoutedEventArgs e)
        {
            if (false == bVoiceCtrlClicked)
            {
                MedaiaVolume = VideoPlayer.Volume;
                VideoPlayer.Volume = 0;
                Image img = new Image();
                img.Source = new BitmapImage(new Uri(@"D:\voice diabled.png"));
                button4.Content = img;
                bVoiceCtrlClicked = true;
            }
            else
            {
                VideoPlayer.Volume = MedaiaVolume;
                Image img = new Image();
                img.Source = new BitmapImage(new Uri(@"D:\voice.png"));
                button4.Content = img;
                bVoiceCtrlClicked = false;
            }
        }
    }
}
