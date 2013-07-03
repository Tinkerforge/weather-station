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

		while(true) {
			try {
				$this->ipcon->connect(self::HOST, self::PORT);
				break;
			} catch(Exception $e) {
				sleep(1);
			}
		}

		$this->ipcon->registerCallback(IPConnection::CALLBACK_ENUMERATE,
		                               array($this, 'cb_enumerate'));
		$this->ipcon->registerCallback(IPConnection::CALLBACK_CONNECTED,
		                               array($this, 'cb_connected'));

		while(true) {
			try {
				$this->ipcon->enumerate();
				break;
			} catch(Exception $e) {
				sleep(1);
			}
		}
	}

	function cb_illuminance($illuminance)
	{
		if($this->brickletLCD != null) {
			$text = sprintf("Illuminanc %6.2f lx", $illuminance/10.0);
			$this->brickletLCD->writeLine(0, 0, $text);
			echo "Write to line 0: $text\n";
		}
	}

	function cb_humidity($humidity)
	{
		if($this->brickletLCD != null) {
			$text = sprintf("Humidity   %6.2f %%", $humidity/10.0);
			$this->brickletLCD->writeLine(1, 0, $text);
			echo "Write to line 1: $text\n";
		}
	}

	function cb_airPressure($airPressure)
	{
		if($this->brickletLCD != null) {
			$text = sprintf("Air Press %7.2f mb", $airPressure/1000.0);
			$this->brickletLCD->writeLine(2, 0, $text);
			echo "Write to line 2: $text\n";

			try {
				$temperature = $this->brickletBarometer->getChipTemperature();
			} catch(Exception $e) {
				echo "Could not get temperature: $e\n";
				return;
			}

			// 0xDF == ° on LCD 20x4 charset
			$text = sprintf("Temperature %5.2f %cC", $temperature/100.0, 0xDF);
			$this->brickletLCD->writeLine(3, 0, $text);
			$text = str_replace(sprintf("%c", 0xDF), '°', $text);
			echo "Write to line 3: $text\n";
		}
	}

	function cb_enumerate($uid, $connectedUid, $position, $hardwareVersion,
	                      $firmwareVersion, $deviceIdentifier, $enumerationType)
	{
		if($enumerationType == IPConnection::ENUMERATION_TYPE_CONNECTED ||
		   $enumerationType == IPConnection::ENUMERATION_TYPE_AVAILABLE) {
			if($deviceIdentifier == BrickletLCD20x4::DEVICE_IDENTIFIER) {
				try {
					$this->brickletLCD = new BrickletLCD20x4($uid, $this->ipcon);
					$this->brickletLCD->clearDisplay();
					$this->brickletLCD->backlightOn();
					echo "LCD 20x4 initialized\n";
				} catch(Exception $e) {
					$this->brickletLCD = null;
					echo "LCD 20x4 init failed: $e\n";
				}
			} else if($deviceIdentifier == BrickletAmbientLight::DEVICE_IDENTIFIER) {
				try {
					$this->brickletAmbientLight = new BrickletAmbientLight($uid, $this->ipcon);
					$this->brickletAmbientLight->setIlluminanceCallbackPeriod(1000);
					$this->brickletAmbientLight->registerCallback(BrickletAmbientLight::CALLBACK_ILLUMINANCE,
					                                              array($this, 'cb_illuminance'));
					echo "Ambient Light initialized\n";
				} catch(Exception $e) {
					$this->brickletAmbientLight = null;
					echo "Ambient Light init failed: $e\n";
				}
			} else if($deviceIdentifier == BrickletHumidity::DEVICE_IDENTIFIER) {
				try {
					$this->brickletHumidity = new BrickletHumidity($uid, $this->ipcon);
					$this->brickletHumidity->setHumidityCallbackPeriod(1000);
					$this->brickletHumidity->registerCallback(BrickletHumidity::CALLBACK_HUMIDITY,
					                                          array($this, 'cb_humidity'));
					echo "Humidity initialized\n";
				} catch(Exception $e) {
					$this->brickletHumidity = null;
					echo "Humidity init failed: $e\n";
				}
			} else if($deviceIdentifier == BrickletBarometer::DEVICE_IDENTIFIER) {
				try {
					$this->brickletBarometer = new BrickletBarometer($uid, $this->ipcon);
					$this->brickletBarometer->setAirPressureCallbackPeriod(1000);
					$this->brickletBarometer->registerCallback(BrickletBarometer::CALLBACK_AIR_PRESSURE,
					                                           array($this, 'cb_airPressure'));
					echo "Barometer initialized\n";
				} catch(Exception $e) {
					$this->brickletBarometer = null;
					echo "Barometer init failed: $e\n";
				}
			}
		}
	}

	function cb_connected($connectedReason)
	{
		if($connectedReason == IPConnection::CONNECT_REASON_AUTO_RECONNECT) {
			echo "Auto Reconnect\n";

			while(true) {
				try {
					$this->ipcon->enumerate();
					break;
				} catch(Exception $e) {
					sleep(1);
				}
			}
		}
	}
}

$weatherStation = new WeatherStation();
echo "Press ctrl+c to exit\n";
$weatherStation->ipcon->dispatchCallbacks(-1);

?>
