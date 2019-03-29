using Tinkerforge;

class WeatherStation
{
	private static string HOST = "localhost";
	private static int PORT = 4223;

	private static IPConnection ipcon = null;
	private static BrickletLCD20x4 brickletLCD = null;
	private static BrickletAmbientLight brickletAmbientLight = null;
	private static BrickletAmbientLightV2 brickletAmbientLightV2 = null;
	private static BrickletAmbientLightV3 brickletAmbientLightV3 = null;
	private static BrickletHumidity brickletHumidity = null;
	private static BrickletHumidityV2 brickletHumidityV2 = null;
	private static BrickletBarometer brickletBarometer = null;
	private static BrickletBarometerV2 brickletBarometerV2 = null;

	static void IlluminanceCB(BrickletAmbientLight sender, int illuminance)
	{
		if(brickletLCD != null)
		{
			string text = string.Format("Illuminanc {0,6:###.00} lx", illuminance / 10.0);

			brickletLCD.WriteLine(0, 0, text);
			System.Console.WriteLine("Write to line 0: " + text);
		}
	}

	static void IlluminanceV2CB(BrickletAmbientLightV2 sender, long illuminance)
	{
		if(brickletLCD != null)
		{
			string text = string.Format("Illumina {0,8:###.00} lx", illuminance / 100.0);

			brickletLCD.WriteLine(0, 0, text);
			System.Console.WriteLine("Write to line 0: " + text);
		}
	}

	static void IlluminanceV3CB(BrickletAmbientLightV3 sender, long illuminance)
	{
		if(brickletLCD != null)
		{
			string text = string.Format("Illumina {0,8:###.00} lx", illuminance / 100.0);

			brickletLCD.WriteLine(0, 0, text);
			System.Console.WriteLine("Write to line 0: " + text);
		}
	}

	static void HumidityCB(BrickletHumidity sender, int humidity)
	{
		if(brickletLCD != null)
		{
			string text = string.Format("Humidity   {0,6:###.00} %", humidity / 10.0);

			brickletLCD.WriteLine(1, 0, text);
			System.Console.WriteLine("Write to line 1: " + text);
		}
	}

	static void HumidityV2CB(BrickletHumidityV2 sender, int humidity)
	{
		if(brickletLCD != null)
		{
			string text = string.Format("Humidity   {0,6:###.00} %", humidity / 100.0);

			brickletLCD.WriteLine(1, 0, text);
			System.Console.WriteLine("Write to line 1: " + text);
		}
	}

	static void AirPressureCB(BrickletBarometer sender, int airPressure)
	{
		if(brickletLCD != null)
		{
			string text = string.Format("Air Press {0,7:####.00} mb", airPressure / 1000.0);

			brickletLCD.WriteLine(2, 0, text);
			System.Console.WriteLine("Write to line 2: " + text);

			int temperature;

			try
			{
				temperature = sender.GetChipTemperature();
			}
			catch(TinkerforgeException e)
			{
				System.Console.WriteLine("Could not get temperature: " + e.Message);

				return;
			}

			// 0xDF == ° on LCD 20x4 charset.
			text = string.Format("Temperature {0,5:##.00} {1}C", temperature / 100.0, (char)0xDF);

			brickletLCD.WriteLine(3, 0, text);
			System.Console.WriteLine("Write to line 3: " + text.Replace((char)0xDF, '°'));
		}
	}

	static void AirPressureV2CB(BrickletBarometerV2 sender, int airPressure)
	{
		if(brickletLCD != null)
		{
			string text = string.Format("Air Press {0,7:####.00} mb", airPressure / 1000.0);

			brickletLCD.WriteLine(2, 0, text);
			System.Console.WriteLine("Write to line 2: " + text);

			int temperature;

			try
			{
				temperature = sender.GetTemperature();
			}
			catch(TinkerforgeException e)
			{
				System.Console.WriteLine("Could not get temperature: " + e.Message);

				return;
			}

			// 0xDF == ° on LCD 20x4 charset.
			text = string.Format("Temperature {0,5:##.00} {1}C", temperature / 100.0, (char)0xDF);

			brickletLCD.WriteLine(3, 0, text);
			System.Console.WriteLine("Write to line 3: " + text.Replace((char)0xDF, '°'));
		}
	}

	static void EnumerateCB(IPConnection sender,
	                        string UID,
	                        string connectedUID,
	                        char position,
	                        short[] hardwareVersion,
	                        short[] firmwareVersion,
	                        int deviceIdentifier,
	                        short enumerationType)
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
					System.Console.WriteLine("LCD 20x4 initialized");
				}
				catch(TinkerforgeException e)
				{
					System.Console.WriteLine("LCD 20x4 init failed: " + e.Message);

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
					System.Console.WriteLine("Ambient Light initialized");
				}
				catch(TinkerforgeException e)
				{
					System.Console.WriteLine("Ambient Light init failed: " + e.Message);

					brickletAmbientLight = null;
				}
			}
			else if(deviceIdentifier == BrickletAmbientLightV2.DEVICE_IDENTIFIER)
			{
				try
				{
					brickletAmbientLightV2 = new BrickletAmbientLightV2(UID, ipcon);

					brickletAmbientLightV2.SetConfiguration(BrickletAmbientLightV2.ILLUMINANCE_RANGE_64000LUX,
					                                        BrickletAmbientLightV2.INTEGRATION_TIME_200MS);
					brickletAmbientLightV2.SetIlluminanceCallbackPeriod(1000);
					brickletAmbientLightV2.Illuminance += IlluminanceV2CB;

					System.Console.WriteLine("Ambient Light 2.0 initialized");
				}
				catch(TinkerforgeException e)
				{
					System.Console.WriteLine("Ambient Light 2.0 init failed: " + e.Message);

					brickletAmbientLightV2 = null;
				}
			}
			else if(deviceIdentifier == BrickletAmbientLightV3.DEVICE_IDENTIFIER)
			{
				try
				{
					brickletAmbientLightV3 = new BrickletAmbientLightV3(UID, ipcon);

					brickletAmbientLightV3.SetConfiguration(BrickletAmbientLightV3.ILLUMINANCE_RANGE_64000LUX,
					                                        BrickletAmbientLightV3.INTEGRATION_TIME_200MS);
					brickletAmbientLightV3.SetIlluminanceCallbackConfiguration(1000, false, 'x', 0, 0);
					brickletAmbientLightV3.IlluminanceCallback += IlluminanceV3CB;

					System.Console.WriteLine("Ambient Light 3.0 initialized");
				}
				catch(TinkerforgeException e)
				{
					System.Console.WriteLine("Ambient Light 3.0 init failed: " + e.Message);

					brickletAmbientLightV3 = null;
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
			else if(deviceIdentifier == BrickletHumidityV2.DEVICE_IDENTIFIER)
			{
				try
				{
					brickletHumidityV2 = new BrickletHumidityV2(UID, ipcon);

					brickletHumidityV2.SetHumidityCallbackConfiguration(1000, true, 'x', 0, 0);
					brickletHumidityV2.HumidityCallback += HumidityV2CB;

					System.Console.WriteLine("Humidity 2.0 initialized");
				}
				catch(TinkerforgeException e)
				{
					System.Console.WriteLine("Humidity 2.0 init failed: " + e.Message);
					brickletHumidityV2 = null;
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
			else if(deviceIdentifier == BrickletBarometerV2.DEVICE_IDENTIFIER)
			{
				try
				{
					brickletBarometerV2 = new BrickletBarometerV2(UID, ipcon);

					brickletBarometerV2.SetAirPressureCallbackConfiguration(1000, false, 'x', 0, 0);
					brickletBarometerV2.AirPressureCallback += AirPressureV2CB;

					System.Console.WriteLine("Barometer 2.0 initialized");
				}
				catch(TinkerforgeException e)
				{
					System.Console.WriteLine("Barometer 2.0 init failed: " + e.Message);

					brickletBarometerV2 = null;
				}
			}
		}
	}

	static void ConnectedCB(IPConnection sender, short connectedReason)
	{
		if(connectedReason == IPConnection.CONNECT_REASON_AUTO_RECONNECT)
		{
			System.Console.WriteLine("Auto Reconnect");

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

		System.Console.WriteLine("Press enter to exit");

		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
