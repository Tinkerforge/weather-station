using Tinkerforge;

class WeatherStation 
{
	private static string HOST = "localhost";
	private static int PORT = 4223;

	private static IPConnection ipcon = null;
	private static BrickletLCD20x4 brickletLCD = null;
	private static BrickletHumidity brickletHumidity = null;
	private static BrickletBarometer brickletBarometer = null;
	private static BrickletAmbientLight brickletAmbientLight = null;

	static void IlluminanceCB(BrickletAmbientLight sender, int illuminance)
	{
		string text = string.Format("Illuminanc {0,6:###.00} lx", illuminance/10.0);
		brickletLCD.WriteLine(0, 0, text);
		System.Console.WriteLine("Write to line 0: " + text);
	}

	static void HumidityCB(BrickletHumidity sender, int humidity)
	{
		string text = string.Format("Humidity   {0,6:###.00} %", humidity/10.0);
		brickletLCD.WriteLine(1, 0, text);
		System.Console.WriteLine("Write to line 1: " + text);
	}

	static void AirPressureCB(BrickletBarometer sender, int airPressure)
	{
		string text = string.Format("Air Press {0,7:####.00} mb", airPressure/1000.0);
		brickletLCD.WriteLine(2, 0, text);
		System.Console.WriteLine("Write to line 2: " + text);

		int temperature = sender.GetChipTemperature();
		text = string.Format("Temperature {0,2:##.00} {1}C", temperature/100.0, (char)0xDF);
		brickletLCD.WriteLine(3, 0, text);
		System.Console.WriteLine("Write to line 3: " + text);
	}


	static void EnumerateCB(object sender, string UID, string connectedUID, char position, short[] hardwareVersion, short[] firmwareVersion, int deviceIdentifier, short enumerationType)
	{
		if(enumerationType == IPConnection.ENUMERATION_TYPE_CONNECTED ||
		   enumerationType == IPConnection.ENUMERATION_TYPE_AVAILABLE)
		{
			if(deviceIdentifier == BrickletLCD20x4.DEVICE_IDENTIFIER)
			{
				brickletLCD = new BrickletLCD20x4(UID, ipcon);
				brickletLCD.ClearDisplay();
				brickletLCD.BacklightOn();
			}
			else if(deviceIdentifier == BrickletAmbientLight.DEVICE_IDENTIFIER) 
			{
				brickletAmbientLight = new BrickletAmbientLight(UID, ipcon);
				brickletAmbientLight.SetIlluminanceCallbackPeriod(1000);
				brickletAmbientLight.Illuminance += IlluminanceCB;
			}
			else if(deviceIdentifier == BrickletHumidity.DEVICE_IDENTIFIER) 
			{
				brickletHumidity = new BrickletHumidity(UID, ipcon);
				brickletHumidity.SetHumidityCallbackPeriod(1000);
				brickletHumidity.Humidity += HumidityCB;
			}
			else if(deviceIdentifier == BrickletBarometer.DEVICE_IDENTIFIER) 
			{
				brickletBarometer = new BrickletBarometer(UID, ipcon);
				brickletBarometer.SetAirPressureCallbackPeriod(1000);
				brickletBarometer.AirPressure += AirPressureCB;
			}
		}
	}

	static void ConnectedCB(object sender, short connectedReason) 
	{
		if(connectedReason == IPConnection.CONNECT_REASON_AUTO_RECONNECT)
		{
			while(true)
			{
				try
				{
					ipcon.Enumerate();
				}
				catch(NotConnectedException e)
				{
					System.Console.WriteLine("Enumeration Error: " + e.Message);
					System.Threading.Thread.Sleep(1000);
					continue;
				}
				break;
			}
		}
	}

	static void Main() 
	{
		ipcon = new IPConnection();
		while(true)
		{
			try 
			{
				ipcon.Connect(HOST, PORT);
			}
			catch(System.Net.Sockets.SocketException e)
			{
				System.Console.WriteLine("Connection Error: " + e.Message);
				System.Threading.Thread.Sleep(1000);
				continue;
			}
			break;
		}

		ipcon.EnumerateCallback += EnumerateCB;
		ipcon.Connected += ConnectedCB;

		while(true)
		{
			try
			{
				ipcon.Enumerate();
			}
			catch(NotConnectedException e)
			{
				System.Console.WriteLine("Enumeration Error: " + e.Message);
				System.Threading.Thread.Sleep(1000);
				continue;
			}
			break;
		}

		System.Console.WriteLine("Press key to exit");
		System.Console.ReadKey();
	}
}
