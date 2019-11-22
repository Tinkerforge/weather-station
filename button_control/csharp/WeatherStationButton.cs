using Tinkerforge;
using System;
using System.Collections;
using System.Threading;

class WeatherStation
{
	private static string HOST = "localhost";
	private static int PORT = 4223;

	private const byte UPDATE_TYPE_STANDARD = 0;
	private const byte UPDATE_TYPE_GRAPH = 1;
	private const byte UPDATE_TYPE_MIN_MAX_AVG = 2;
	private const byte UPDATE_TYPE_TIME = 3;

	private const int LINE_LENGTH = 20;
	private const int CUSTOM_CHAR_START = 8;
	private const int CUSTOM_CHAR_END = 15;
	private const int BAR_HEIGHT = 24;

	private const int MODE_ILLUMINANCE = 0;
	private const int MODE_HUMIDITY = 1;
	private const int MODE_AIR_PRESSURE = 2;
	private const int MODE_TEMPERATURE = 3;

	private static IPConnection ipcon = null;
	private static BrickletLCD20x4 brickletLCD = null;
	private static BrickletAmbientLight brickletAmbientLight = null;
	private static BrickletAmbientLightV2 brickletAmbientLightV2 = null;
	private static BrickletAmbientLightV3 brickletAmbientLightV3 = null;
	private static BrickletHumidity brickletHumidity = null;
	private static BrickletHumidityV2 brickletHumidityV2 = null;
	private static BrickletBarometer brickletBarometer = null;
	private static BrickletBarometerV2 brickletBarometerV2 = null;

	private static double latestIlluminance = Double.NaN;
	private static double latestHumidity = Double.NaN;
	private static double latestAirPressure = Double.NaN;
	private static double latestTemperature = Double.NaN;

	private static Queue illuminanceQueue = new Queue();
	private static Queue humidityQueue = new Queue();
	private static Queue airPressureQueue = new Queue();
	private static Queue temperatureQueue = new Queue();

	private static byte buttonPressed = 0;
	private static int[] buttonPressedCounter = {0, 0, 0, 0};

	private static Timer timer;

	static void IlluminanceCB(BrickletAmbientLight sender, int illuminance)
	{
		latestIlluminance = illuminance/10.0;
	}

	static void IlluminanceV2CB(BrickletAmbientLightV2 sender, long illuminance)
	{
		latestIlluminance = illuminance/100.0;
	}

	static void IlluminanceV3CB(BrickletAmbientLightV3 sender, long illuminance)
	{
		latestIlluminance = illuminance/100.0;
	}

	static void HumidityCB(BrickletHumidity sender, int humidity)
	{
		latestHumidity = humidity/10.0;
	}

	static void HumidityV2CB(BrickletHumidityV2 sender, int humidity)
	{
		latestHumidity = humidity/100.0;
	}

	static void AirPressureCB(BrickletBarometer sender, int airPressure)
	{
		latestAirPressure = airPressure/1000.0;

		int temperature = sender.GetChipTemperature();
		latestTemperature = temperature/100.0;
	}

	static void AirPressureV2CB(BrickletBarometerV2 sender, int airPressure)
	{
		latestAirPressure = airPressure/1000.0;

		int temperature = sender.GetTemperature();
		latestTemperature = temperature/100.0;
	}

	static void PressedCB(BrickletLCD20x4 sender, byte button)
	{
		if(button == buttonPressed)
		{
			buttonPressedCounter[button]++;
		}
		else
		{
			buttonPressed = button;
		}

		brickletLCD.ClearDisplay();
		UpdateSwitch();
	}

	static void EnumerateCB(object sender, string UID, string connectedUID, char position,
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
					ConfigureCustomChars(brickletLCD);
					brickletLCD.ButtonPressed += PressedCB;
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
					brickletBarometer = null;
				}
			}
		}
	}

	static void ConnectedCB(object sender, short connectedReason)
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

	static private void ConfigureCustomChars(BrickletLCD20x4 lcd)
	{
		byte[][] c = new byte[8][];
		c[0] = new byte[8] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff};
		c[1] = new byte[8] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff};
		c[2] = new byte[8] {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff};
		c[3] = new byte[8] {0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff};
		c[4] = new byte[8] {0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff};
		c[5] = new byte[8] {0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
		c[6] = new byte[8] {0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
		c[7] = new byte[8] {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

		for(byte i = 0; i < c.Length; i++)
		{
			lcd.SetCustomCharacter(i, c[i]);
		}
	}

	static private string TimeFromSeconds(int s)
	{
		string str = "(";
		int m = s/60;
		int h = m/60;

		if(h > 0)
		{
			str += h + "h)";
		}
		else
		{
			if(m == 0)
			{
				m = 1;
			}
			str += m + "m)";
		}

		if(str.Length == 4)
		{
			return " " + str;
		}

		return str;
	}

	// From http://www.tinkerforge.com/en/doc/Software/Bricklets/LCD20x4_Bricklet_CSharp.html#unicode
	// Maps a normal UTF-16 encoded string to the LCD charset
	static string UTF16ToKS0066U(string utf16)
	{
		string ks0066u = "";
		char c;

		for (int i = 0; i < utf16.Length; i++)
		{
			int codePoint = Char.ConvertToUtf32(utf16, i);

			if (Char.IsSurrogate(utf16, i))
			{
				// Skip low surrogate
				i++;
			}

			// ASCII subset from JIS X 0201
			if (codePoint >= 0x0020 && codePoint <= 0x007e)
			{
				// The LCD charset doesn't include '\' and '~', use similar characters instead
				switch (codePoint)
				{
					case 0x005c: c = (char)0xa4; break; // REVERSE SOLIDUS maps to IDEOGRAPHIC COMMA
					case 0x007e: c = (char)0x2d; break; // TILDE maps to HYPHEN-MINUS
					default: c = (char)codePoint; break;
				}
			}
			// Katakana subset from JIS X 0201
			else if (codePoint >= 0xff61 && codePoint <= 0xff9f)
			{
				c = (char)(codePoint - 0xfec0);
			}
			// Special characters
			else
			{
				switch (codePoint)
				{
					case 0x00a5: c = (char)0x5c; break; // YEN SIGN
					case 0x2192: c = (char)0x7e; break; // RIGHTWARDS ARROW
					case 0x2190: c = (char)0x7f; break; // LEFTWARDS ARROW
					case 0x00b0: c = (char)0xdf; break; // DEGREE SIGN maps to KATAKANA SEMI-VOICED SOUND MARK
					case 0x03b1: c = (char)0xe0; break; // GREEK SMALL LETTER ALPHA
					case 0x00c4: c = (char)0xe1; break; // LATIN CAPITAL LETTER A WITH DIAERESIS
					case 0x00e4: c = (char)0xe1; break; // LATIN SMALL LETTER A WITH DIAERESIS
					case 0x00df: c = (char)0xe2; break; // LATIN SMALL LETTER SHARP S
					case 0x03b5: c = (char)0xe3; break; // GREEK SMALL LETTER EPSILON
					case 0x00b5: c = (char)0xe4; break; // MICRO SIGN
					case 0x03bc: c = (char)0xe4; break; // GREEK SMALL LETTER MU
					case 0x03c2: c = (char)0xe5; break; // GREEK SMALL LETTER FINAL SIGMA
					case 0x03c1: c = (char)0xe6; break; // GREEK SMALL LETTER RHO
					case 0x221a: c = (char)0xe8; break; // SQUARE ROOT
					case 0x00b9: c = (char)0xe9; break; // SUPERSCRIPT ONE maps to SUPERSCRIPT (minus) ONE
					case 0x00a4: c = (char)0xeb; break; // CURRENCY SIGN
					case 0x00a2: c = (char)0xec; break; // CENT SIGN
					case 0x2c60: c = (char)0xed; break; // LATIN CAPITAL LETTER L WITH DOUBLE BAR
					case 0x00f1: c = (char)0xee; break; // LATIN SMALL LETTER N WITH TILDE
					case 0x00d6: c = (char)0xef; break; // LATIN CAPITAL LETTER O WITH DIAERESIS
					case 0x00f6: c = (char)0xef; break; // LATIN SMALL LETTER O WITH DIAERESIS
					case 0x03f4: c = (char)0xf2; break; // GREEK CAPITAL THETA SYMBOL
					case 0x221e: c = (char)0xf3; break; // INFINITY
					case 0x03a9: c = (char)0xf4; break; // GREEK CAPITAL LETTER OMEGA
					case 0x00dc: c = (char)0xf5; break; // LATIN CAPITAL LETTER U WITH DIAERESIS
					case 0x00fc: c = (char)0xf5; break; // LATIN SMALL LETTER U WITH DIAERESIS
					case 0x03a3: c = (char)0xf6; break; // GREEK CAPITAL LETTER SIGMA
					case 0x03c0: c = (char)0xf7; break; // GREEK SMALL LETTER PI
					case 0x0304: c = (char)0xf8; break; // COMBINING MACRON
					case 0x00f7: c = (char)0xfd; break; // DIVISION SIGN

					default:
					case 0x25a0: c = (char)0xff; break; // BLACK SQUARE
				}
			}

			// Special handling for 'x' followed by COMBINING MACRON
			if (c == (char)0xf8)
			{
				if (!ks0066u.EndsWith("x"))
				{
					c = (char)0xff; // BLACK SQUARE
				}

				if (ks0066u.Length > 0)
				{
					ks0066u = ks0066u.Remove(ks0066u.Length - 1, 1);
				}
			}

			ks0066u += c;
		}

		return ks0066u;
	}

	static private double[] GetMinMaxAvg(Queue q)
	{
		if(q.Count == 0)
		{
			return new double[] {0.0, 0.0, 0.0};
		}

		double min = 10000.0;
		double max = -10000.0;
		double avg = 0.0;

		foreach(double v in q)
		{
			if(v < min)
			{
				min = v;
			}

			if(v > max)
			{
				max = v;
			}

			avg += v;
		}

		avg /= q.Count;

		return new double[] {min, max, avg};
	}

	static private void UpdateStandard()
	{
		string text = string.Format("Illumina {0,8:###.00} lx", latestIlluminance);
		brickletLCD.WriteLine(0, 0, text);

		text = string.Format("Humidity   {0,6:###.00} %", latestHumidity);
		brickletLCD.WriteLine(1, 0, text);

		text = string.Format("Air Press {0,7:####.00} mb", latestAirPressure);
		brickletLCD.WriteLine(2, 0, text);

		text = string.Format("Temperature {0,5:##.00} {1}C", latestTemperature, (char)0xDF);
		brickletLCD.WriteLine(3, 0, text);
	}

	static private void UpdateGraph()
	{
		double barSumMin;
		double barSumMax;

		switch(buttonPressedCounter[1] % 4)
		{
			case MODE_ILLUMINANCE:
				UpdateGraphWriteBars(illuminanceQueue, out barSumMin, out barSumMax);
				UpdateGraphWriteTitle("I: ", barSumMin, barSumMax, illuminanceQueue.Count);
				break;

			case MODE_HUMIDITY:
				UpdateGraphWriteBars(humidityQueue, out barSumMin, out barSumMax);
				UpdateGraphWriteTitle("H: ", barSumMin, barSumMax, humidityQueue.Count);
				break;

			case MODE_AIR_PRESSURE:
				UpdateGraphWriteBars(airPressureQueue, out barSumMin, out barSumMax);
				UpdateGraphWriteTitle("A: ", barSumMin, barSumMax, airPressureQueue.Count);
				break;

			case MODE_TEMPERATURE:
				UpdateGraphWriteBars(temperatureQueue, out barSumMin, out barSumMax);
				UpdateGraphWriteTitle("T: ", barSumMin, barSumMax, temperatureQueue.Count);
				break;
		}
	}

	static private void UpdateGraphWriteTitle(string s, double barSumMin, double barSumMax, int count)
	{
		string line0 = s + (int)barSumMin + " - " + (int)(barSumMax+1.0);
		string time = TimeFromSeconds(count);

		int numSpaces = LINE_LENGTH - line0.Length - time.Length;
		for(int i = 0; i < numSpaces; i++)
		{
			line0 += " ";
		}

		brickletLCD.WriteLine(0, 0, line0 + time);
	}

	static private void UpdateGraphWriteBars(Queue q, out double barSumMin, out double barSumMax)
	{
		barSumMin = 10000.0;
		barSumMax = -10000.0;

		int count = q.Count;

		int countBars = count/LINE_LENGTH;
		if(countBars == 0)
		{
			countBars = 1;
		}

		double[] barSum = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
		                   0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
		int i = 0;
		foreach(double v in q)
		{
			barSum[i/countBars] += v;
			i++;
			if(i == LINE_LENGTH*countBars)
			{
				break;
			}
		}

		for(i = 0; i < barSum.Length; i++)
		{
			barSum[i] /= countBars;
			if(barSum[i] < barSumMin)
			{
				barSumMin = barSum[i];
			}
			if(barSum[i] > barSumMax)
			{
				barSumMax = barSum[i];
			}
		}

		double scale = (BAR_HEIGHT-1)/(barSumMax - barSumMin);
		double offset = barSumMin * scale - 1;

		int[] barHeight = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		for(i = 0; i < barSum.Length; i++)
		{
			barHeight[i] = (int)Math.Round(barSum[i] * scale - offset, 0);
		}

		char[][] lines = new char[4][];
		lines[1] = new char[LINE_LENGTH] {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		                                  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
		lines[2] = new char[LINE_LENGTH] {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		                                  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
		lines[3] = new char[LINE_LENGTH] {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		                                  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

		for(i = 0; i < barHeight.Length; i++)
		{
			int barLength = barHeight[i]/8;
			int j = 0;
			for(j = 0; j < barLength; j++)
			{
				lines[j+1][i] = (char)CUSTOM_CHAR_END;
			}
			if(j < 3)
			{
				lines[j+1][i] = (char)((barHeight[i]-1)/3 + CUSTOM_CHAR_START);
			}
		}

		for(byte line = 1; line < 4; line++)
		{
			brickletLCD.WriteLine((byte)(4-line), 0, new string(lines[line]));
		}
	}

	static private void UpdateMinMaxAvg()
	{
		switch(buttonPressedCounter[2] % 4)
		{
			case MODE_ILLUMINANCE:
				UpdateMinMaxAvgWrite("Illuminance    " + TimeFromSeconds(illuminanceQueue.Count),
				                     "lx",
				                     GetMinMaxAvg(illuminanceQueue));
				break;

			case MODE_HUMIDITY:
				UpdateMinMaxAvgWrite("Humidity       " + TimeFromSeconds(humidityQueue.Count),
				                     "%RH",
				                     GetMinMaxAvg(humidityQueue));
				break;

			case MODE_AIR_PRESSURE:
				UpdateMinMaxAvgWrite("Air Pressure   " + TimeFromSeconds(airPressureQueue.Count),
				                     "hPa",
				                     GetMinMaxAvg(airPressureQueue));
				break;

			case MODE_TEMPERATURE:
				UpdateMinMaxAvgWrite("Temperature    " + TimeFromSeconds(temperatureQueue.Count),
				                     ((char)0xDF) + "C",
				                     GetMinMaxAvg(temperatureQueue));
				break;
		}
	}

	static private void UpdateMinMaxAvgWrite(string title, string unit, double[] values)
	{
		String min = string.Format("Min: {0,2:##.00} {1}", values[0], unit);
		String max = string.Format("Max: {0,2:##.00} {1}", values[1], unit);
		String avg = string.Format("Avg: {0,2:##.00} {1}", values[2], unit);
		brickletLCD.WriteLine(0, 0, title);
		brickletLCD.WriteLine(1, 0, min);
		brickletLCD.WriteLine(2, 0, avg);
		brickletLCD.WriteLine(3, 0, max);
	}

	static private void UpdateTime()
	{
		string line0 = DateTime.Now.ToString("HH:mm:ss");
		string line1 = DateTime.Now.ToString("dddd");
		string line2 = DateTime.Now.ToString("D");
		brickletLCD.WriteLine((byte)0, (byte)((LINE_LENGTH-line0.Length)/2), line0);
		brickletLCD.WriteLine((byte)1, (byte)((LINE_LENGTH-line1.Length)/2), UTF16ToKS0066U(line1));
		brickletLCD.WriteLine((byte)2, (byte)((LINE_LENGTH-line2.Length)/2), UTF16ToKS0066U(line2));
	}

	static private void UpdateSwitch()
	{
		switch(buttonPressed)
		{
			case UPDATE_TYPE_STANDARD: UpdateStandard(); break;
			case UPDATE_TYPE_GRAPH: UpdateGraph(); break;
			case UPDATE_TYPE_MIN_MAX_AVG: UpdateMinMaxAvg(); break;
			case UPDATE_TYPE_TIME: UpdateTime(); break;
		}
	}

	static private void Update(object state)
	{
		if(Double.IsNaN(latestIlluminance) ||
		   Double.IsNaN(latestHumidity) ||
		   Double.IsNaN(latestAirPressure) ||
		   Double.IsNaN(latestTemperature))
		{
			return;
		}

		illuminanceQueue.Enqueue(latestIlluminance);
		if(illuminanceQueue.Count > 60*60*24)
		{
			illuminanceQueue.Dequeue();
		}

		humidityQueue.Enqueue(latestHumidity);
		if(humidityQueue.Count > 60*60*24)
		{
			humidityQueue.Dequeue();
		}

		airPressureQueue.Enqueue(latestAirPressure);
		if(airPressureQueue.Count > 60*60*24)
		{
			airPressureQueue.Dequeue();
		}

		temperatureQueue.Enqueue(latestTemperature);
		if(temperatureQueue.Count > 60*60*24)
		{
			temperatureQueue.Dequeue();
		}

		UpdateSwitch();
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

		timer = new Timer(Update, null, TimeSpan.Zero, TimeSpan.FromSeconds(1));

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
