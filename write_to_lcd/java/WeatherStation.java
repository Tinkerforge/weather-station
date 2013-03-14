import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletLCD20x4;
import com.tinkerforge.BrickletAmbientLight;
import com.tinkerforge.BrickletHumidity;
import com.tinkerforge.BrickletBarometer;

class WeatherListener implements IPConnection.EnumerateListener, 
                                 IPConnection.ConnectedListener, 
								 BrickletAmbientLight.IlluminanceListener, 
								 BrickletHumidity.HumidityListener, 
								 BrickletBarometer.AirPressureListener {
	BrickletLCD20x4 brickletLCD = null;
	BrickletAmbientLight brickletAmbientLight = null;
	BrickletHumidity brickletHumidity = null;
	BrickletBarometer brickletBarometer = null;
	IPConnection ipcon = null;

	WeatherListener(IPConnection ipcon) {
		this.ipcon = ipcon;
	}

	public void illuminance(int illuminance) {
		String text = String.format("Illuminanc %6.2f lx", illuminance/10.0);
		try {
			brickletLCD.writeLine((short)0, (short)0, text);
		} catch(com.tinkerforge.TimeoutException e) {
		} catch(com.tinkerforge.NotConnectedException e) {
		}

		System.out.println("Write to line 0: " + text);
    }

	public void humidity(int humidity) {
		String text = String.format("Humidity   %6.2f %%", humidity/10.0);
		try {
			brickletLCD.writeLine((short)1, (short)0, text);
		} catch(com.tinkerforge.TimeoutException e) {
		} catch(com.tinkerforge.NotConnectedException e) {
		}

		System.out.println("Write to line 1: " + text);
    }

	public void airPressure(int airPressure) {
		String text = String.format("Air Press %7.2f mb", airPressure/1000.0);
		try {
			brickletLCD.writeLine((short)2, (short)0, text);
		} catch(com.tinkerforge.TimeoutException e) {
		} catch(com.tinkerforge.NotConnectedException e) {
		}

		System.out.println("Write to line 2: " + text);

		int temperature = 0;
		try {
			temperature = brickletBarometer.getChipTemperature();
		} catch(com.tinkerforge.TimeoutException e) {
		} catch(com.tinkerforge.NotConnectedException e) {
		}
		text = String.format("Temperature %2.2f %cC", temperature/100.0, 0xDF);
		try {
			brickletLCD.writeLine((short)3, (short)0, text);
		} catch(com.tinkerforge.TimeoutException e) {
		} catch(com.tinkerforge.NotConnectedException e) {
		}
		System.out.println("Write to line 3: " + text);
    }

	public void enumerate(String uid, String connectedUid, char position,
                          short[] hardwareVersion, short[] firmwareVersion,
                          int deviceIdentifier, short enumerationType) {
		if(enumerationType == IPConnection.ENUMERATION_TYPE_CONNECTED ||
		   enumerationType == IPConnection.ENUMERATION_TYPE_AVAILABLE) {
			if(deviceIdentifier == BrickletLCD20x4.DEVICE_IDENTIFIER) {
				brickletLCD = new BrickletLCD20x4(uid, ipcon); 
				try {
					brickletLCD.clearDisplay();
					brickletLCD.backlightOn();
				} catch(com.tinkerforge.TimeoutException e) {
				} catch(com.tinkerforge.NotConnectedException e) {
				}

			} else if(deviceIdentifier == BrickletAmbientLight.DEVICE_IDENTIFIER) {
				brickletAmbientLight = new BrickletAmbientLight(uid, ipcon);
				try {
					brickletAmbientLight.setIlluminanceCallbackPeriod(1000);
				} catch(com.tinkerforge.TimeoutException e) {
				} catch(com.tinkerforge.NotConnectedException e) {
				}
				brickletAmbientLight.addIlluminanceListener(this);

			} else if(deviceIdentifier == BrickletHumidity.DEVICE_IDENTIFIER) {
				brickletHumidity = new BrickletHumidity(uid, ipcon);
				try {
					brickletHumidity.setHumidityCallbackPeriod(1000);
				} catch(com.tinkerforge.TimeoutException e) {
				} catch(com.tinkerforge.NotConnectedException e) {
				}
				brickletHumidity.addHumidityListener(this);
			} else if(deviceIdentifier == BrickletBarometer.DEVICE_IDENTIFIER) {
				brickletBarometer = new BrickletBarometer(uid, ipcon);
				try {
					brickletBarometer.setAirPressureCallbackPeriod(1000);
				} catch(com.tinkerforge.TimeoutException e) {
				} catch(com.tinkerforge.NotConnectedException e) {
				}
				brickletBarometer.addAirPressureListener(this);
			}
		}
	}

	public void connected(short connectedReason) {
		if(connectedReason == IPConnection.CONNECT_REASON_AUTO_RECONNECT) {
			while(true) {
				try {
					ipcon.enumerate();
				} catch(com.tinkerforge.NotConnectedException e) {
					try {
						Thread.sleep(1000);
					} catch(java.lang.InterruptedException ei) {
					}
					continue;
				}
				break;
			}
		}
	}
}

public class WeatherStation {
    private static final String host = "localhost";
    private static final int port = 4223;
	private static IPConnection ipcon = null;

    // Note: To make the example code cleaner we do not handle exceptions. Exceptions you
    //       might normally want to catch are described in the documentation
    public static void main(String args[]) {
        // Create connection and connect to brickd
        ipcon = new IPConnection();

		while(true) {
			try {
				ipcon.connect(host, port);
			} catch(java.net.UnknownHostException e) {
				try {
					Thread.sleep(1000);
				} catch(java.lang.InterruptedException ei) {
				}
				continue;
			} catch(java.io.IOException e) {
				try {
					Thread.sleep(1000);
				} catch(java.lang.InterruptedException ei) {
				}
				continue;
			} catch(com.tinkerforge.AlreadyConnectedException e) {
				try {
					Thread.sleep(1000);
				} catch(java.lang.InterruptedException ei) {
				}
				continue;
			}
			break;
		}

        // Register enumerate listener and print incoming information
		WeatherListener weatherListener = new WeatherListener(ipcon);
        ipcon.addEnumerateListener(weatherListener);
        ipcon.addConnectedListener(weatherListener);

		while(true) {
			try {
        		ipcon.enumerate();
			} catch(com.tinkerforge.NotConnectedException e) {
				try {
					Thread.sleep(1000);
				} catch(java.lang.InterruptedException ei) {
				}
				continue;
			}
			break;
		}

        System.console().readLine("Press key to exit\n");
		try {
			ipcon.disconnect();
		} catch(com.tinkerforge.NotConnectedException e) {
		}
    }
}
