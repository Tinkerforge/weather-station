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
		if(brickletLCD != null)
		{
			string text = string.Format("Illuminanc {0,6:###.00} lx", illuminance/10.0);
			brickletLCD.WriteLine(0, 0, text);
			System.Console.WriteLine("Write to line 0: " + text);
		}
	}

	static void HumidityCB(BrickletHumidity sender, int humidity)
	{
		if(brickletLCD != null)
		{
			string text = string.Format("Humidity   {0,6:###.00} %", humidity/10.0);
			brickletLCD.WriteLine(1, 0, text);
			System.Console.WriteLine("Write to line 1: " + text);
		}
	}

	static void AirPressureCB(BrickletBarometer sender, int airPressure)
	{
		if(brickletLCD != null)
		{
			string text = string.Format("Air Press {0,7:####.00} mb", airPressure/1000.0);
			brickletLCD.WriteLine(2, 0, text);
			System.Console.WriteLine("Write to line 2: " + text);

			int temperature = sender.GetChipTemperature();
			text = string.Format("Temperature {0,5:##.00} {1}C", temperature/100.0, (char)0xDF);
			brickletLCD.WriteLine(3, 0, text);
			System.Console.WriteLine("Write to line 3: " + text);
		}
	}

	static void EnumerateCB(IPConnection sender, string UID, string connectedUID, char position,
	                        short[] hardwareVersion, short[] firmwareVersion,
	                        int deviceIdentifier, short enumerationType)
	{
		if(enumerationType == IPConnection.ENUMERATION_TYPE_CONNECTED ||
		   enumerationType == IPConnection.ENUMERATION_TYPE_AVAILABLE)
		{
			if(deviceIdentifier == BrickletLCD20x4.DEVICE_IDENTIFIER)
			{
				try
				{
					brickletLCD = new BrickletLCD20x4(UID, ipcon);
					brickletLCD.ClearDisplay();
					brickletLCD.BacklightOn();
					System.Console.WriteLine("LCD20x4 initialized");
				}
				catch(TinkerforgeException e)
				{
					System.Console.WriteLine("LCD20x4 init failed: " + e.Message);
					brickletLCD = null;
				}
			}
			else if(deviceIdentifier == BrickletAmbientLight.DEVICE_IDENTIFIER)
			{
				try
				{
					brickletAmbientLight = new BrickletAmbientLight(UID, ipcon);
					brickletAmbientLight.SetIlluminanceCallbackPeriod(1000);
					brickletAmbientLight.Illuminance += IlluminanceCB;
					System.Console.WriteLine("AmbientLight initialized");
				}
				catch(TinkerforgeException e)
				{
					System.Console.WriteLine("AmbientLight init failed: " + e.Message);
					brickletAmbientLight = null;
				}
			}
			else if(deviceIdentifier == BrickletHumidity.DEVICE_IDENTIFIER)
			{
				try
				{
					brickletHumidity = new BrickletHumidity(UID, ipcon);
					brickletHumidity.SetHumidityCallbackPeriod(1000);
					brickletHumidity.Humidity += HumidityCB;
					System.Console.WriteLine("Humidity initialized");
				}
				catch(TinkerforgeException e)
				{
					System.Console.WriteLine("Humidity init failed: " + e.Message);
					brickletHumidity = null;
				}
			}
			else if(deviceIdentifier == BrickletBarometer.DEVICE_IDENTIFIER)
			{
				try
				{
					brickletBarometer = new BrickletBarometer(UID, ipcon);
					brickletBarometer.SetAirPressureCallbackPeriod(1000);
					brickletBarometer.AirPressure += AirPressureCB;
					System.Console.WriteLine("Barometer initialized");
				}
				catch(TinkerforgeException e)
				{
					System.Console.WriteLine("Barometer init failed: " + e.Message);
					brickletBarometer = null;
				}
			}
		}
	}

	static void ConnectedCB(IPConnection sender, short connectedReason)
	{
		if(connectedReason == IPConnection.CONNECT_REASON_AUTO_RECONNECT)
		{
			while(true)
			{
				try
				{
					ipcon.Enumerate();
					break;
				}
				catch(NotConnectedException e)
				{
					System.Console.WriteLine("Enumeration Error: " + e.Message);
					System.Threading.Thread.Sleep(1000);
				}
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
				break;
			}
			catch(System.Net.Sockets.SocketException e)
			{
				System.Console.WriteLine("Connection Error: " + e.Message);
				System.Threading.Thread.Sleep(1000);
			}
		}

		ipcon.EnumerateCallback += EnumerateCB;
		ipcon.Connected += ConnectedCB;

		while(true)
		{
			try
			{
				ipcon.Enumerate();
				break;
			}
			catch(NotConnectedException e)
			{
				System.Console.WriteLine("Enumeration Error: " + e.Message);
				System.Threading.Thread.Sleep(1000);
			}
		}

		System.Console.WriteLine("Press key to exit");
		System.Console.ReadKey();
		ipcon.Disconnect();
	}
}
