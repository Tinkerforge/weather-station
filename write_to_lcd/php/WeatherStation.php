<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLCD20x4.php');
require_once('Tinkerforge/BrickletAmbientLight.php');
require_once('Tinkerforge/BrickletHumidity.php');
require_once('Tinkerforge/BrickletBarometer.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLCD20x4;
use Tinkerforge\BrickletAmbientLight;
use Tinkerforge\BrickletHumidity;
use Tinkerforge\BrickletBarometer;

class WeatherStation
{
	const HOST = 'localhost';
	const PORT = 4223;

	public function __construct()
    {
		$this->brickletLCD = null;
		$this->brickletAmbientLight = null;
		$this->brickletHumidity = null;
		$this->brickletBarometer = null;
		$this->ipcon = new IPConnection();

		while(true)
		{
			try 
			{
				$this->ipcon->connect(WeatherStation::HOST, WeatherStation::PORT);
			} 
			catch(Exception $e) 
			{
				sleep(1);
				continue;
			}
			break;
		}

		$this->ipcon->registerCallback(IPConnection::CALLBACK_ENUMERATE, array($this, 'enumerateCB'));
		$this->ipcon->registerCallback(IPConnection::CALLBACK_CONNECTED, array($this, 'connectedCB'));

		while(true)
		{
			try 
			{
				$this->ipcon->enumerate();
			} 
			catch(Exception $e) 
			{
				sleep(1);
				continue;
			}
			break;
		}
	}


	function illuminanceCB($illuminance)
	{
		$text = sprintf("Illuminanc %6.2f lx", $illuminance/10.0);

		if($this->brickletLCD != null)
		{
			$this->brickletLCD->writeLine(0, 0, $text);
			echo "Write to line 0: $text\n";
		}
	}

	function humidityCB($humidity)
	{
		$text = sprintf("Humidity   %6.2f %%", $humidity/10.0);
		if($this->brickletLCD != null)
		{
			$this->brickletLCD->writeLine(1, 0, $text);
			echo "Write to line 1: $text\n";
		}
	}

	function airPressureCB($airPressure)
	{
		$text = sprintf("Air Press %7.2f mb", $airPressure/1000.0);
		if($this->brickletLCD != null)
		{
			$this->brickletLCD->writeLine(2, 0, $text);
			echo "Write to line 2: $text\n";
		}

		$temperature = $this->brickletBarometer->getChipTemperature();
		$text = sprintf("Temperature %2.2f %cC", $temperature/100.0, 0xDF);
		if($this->brickletLCD != null)
		{
			$this->brickletLCD->writeLine(3, 0, $text);
			echo "Write to line 3: $text\n";
		}
	}

	function enumerateCB($uid, $connectedUid, $position, $hardwareVersion, $firmwareVersion, $deviceIdentifier, $enumerationType)
	{
		if($enumerationType == IPConnection::ENUMERATION_TYPE_CONNECTED ||
		   $enumerationType == IPConnection::ENUMERATION_TYPE_AVAILABLE) 
		{
			if($deviceIdentifier == BrickletLCD20x4::DEVICE_IDENTIFIER) 
			{
				$this->brickletLCD = new BrickletLCD20x4($uid, $this->ipcon);
				$this->brickletLCD->clearDisplay();
				$this->brickletLCD->backlightOn();
			}

			else if($deviceIdentifier == BrickletAmbientLight::DEVICE_IDENTIFIER) 
			{
				$this->brickletAmbientLight = new BrickletAmbientLight($uid, $this->ipcon);
				$this->brickletAmbientLight->setIlluminanceCallbackPeriod(1000);
				$this->brickletAmbientLight->registerCallback(BrickletAmbientLight::CALLBACK_ILLUMINANCE, array($this, 'illuminanceCB'));
			}

			else if($deviceIdentifier == BrickletHumidity::DEVICE_IDENTIFIER) 
			{
				$this->brickletHumidity = new BrickletHumidity($uid, $this->ipcon);
				$this->brickletHumidity->setHumidityCallbackPeriod(1000);
				$this->brickletHumidity->registerCallback(BrickletHumidity::CALLBACK_HUMIDITY, array($this, 'humidityCB'));
			}

			else if($deviceIdentifier == BrickletBarometer::DEVICE_IDENTIFIER) 
			{
				$this->brickletBarometer = new BrickletBarometer($uid, $this->ipcon);
				$this->brickletBarometer->setAirPressureCallbackPeriod(1000);
				$this->brickletBarometer->registerCallback(BrickletBarometer::CALLBACK_AIR_PRESSURE, array($this, 'airPressureCB'));
			}
		}
	}

	function connectedCB($connectedReason)
	{
		if($connectedReason == IPConnection::CONNECT_REASON_AUTO_RECONNECT)
		{
			while(true)
			{
				try 
				{
					$this->ipcon->enumerate();
				} 
				catch(Exception $e) 
				{
					sleep(1);
					continue;
				}
				break;
			}
		}
	}
}

$weatherStation = new WeatherStation();
echo "Press ctrl+c to exit\n";
$weatherStation->ipcon->dispatchCallbacks(-1);

?>
