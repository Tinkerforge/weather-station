<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLCD20x4.php');
require_once('Tinkerforge/BrickletAmbientLight.php');
require_once('Tinkerforge/BrickletAmbientLightV2.php');
require_once('Tinkerforge/BrickletAmbientLightV3.php');
require_once('Tinkerforge/BrickletHumidity.php');
require_once('Tinkerforge/BrickletHumidityV2.php');
require_once('Tinkerforge/BrickletBarometer.php');
require_once('Tinkerforge/BrickletBarometerV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLCD20x4;
use Tinkerforge\BrickletAmbientLight;
use Tinkerforge\BrickletAmbientLightV2;
use Tinkerforge\BrickletAmbientLightV3;
use Tinkerforge\BrickletHumidity;
use Tinkerforge\BrickletHumidityV2;
use Tinkerforge\BrickletBarometer;
use Tinkerforge\BrickletBarometerV2;

class WeatherStation
{
	const HOST = 'localhost';
	const PORT = 4223;

	public function __construct()
    {
		$this->brickletLCD = null;
		$this->brickletAmbientLight = null;
		$this->brickletAmbientLightV2 = null;
		$this->brickletAmbientLightV3 = null;
		$this->brickletHumidity = null;
		$this->brickletHumidityV2 = null;
		$this->brickletBarometer = null;
		$this->brickletBarometerV2 = null;
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

	function cb_illuminanceV2($illuminance)
	{
		if($this->brickletLCD != null) {
			$text = sprintf("Illumina %8.2f lx", $illuminance/100.0);
			$this->brickletLCD->writeLine(0, 0, $text);
			echo "Write to line 0: $text\n";
		}
	}

	function cb_illuminanceV3($illuminance)
	{
		if($this->brickletLCD != null) {
			$text = sprintf("Illumina %8.2f lx", $illuminance/100.0);
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

	function cb_humidityV2($humidity)
	{
		if($this->brickletLCD != null) {
			$text = sprintf("Humidity   %6.2f %%", $humidity/100.0);
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

	function cb_airPressureV2($airPressure)
	{
		if($this->brickletLCD != null) {
			$text = sprintf("Air Press %7.2f mb", $airPressure/1000.0);
			$this->brickletLCD->writeLine(2, 0, $text);
			echo "Write to line 2: $text\n";

			try {
				$temperature = $this->brickletBarometerV2->getTemperature();
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
			} else if($deviceIdentifier == BrickletAmbientLightV2::DEVICE_IDENTIFIER) {
				try {
					$this->brickletAmbientLightV2 = new BrickletAmbientLightV2($uid, $this->ipcon);
					$this->brickletAmbientLightV2->setConfiguration(BrickletAmbientLightV2::ILLUMINANCE_RANGE_64000LUX,
					                                                BrickletAmbientLightV2::INTEGRATION_TIME_200MS);
					$this->brickletAmbientLightV2->setIlluminanceCallbackPeriod(1000);
					$this->brickletAmbientLightV2->registerCallback(BrickletAmbientLightV2::CALLBACK_ILLUMINANCE,
					                                                array($this, 'cb_illuminanceV2'));
					echo "Ambient Light 2.0 initialized\n";
				} catch(Exception $e) {
					$this->brickletAmbientLight = null;
					echo "Ambient Light 2.0 init failed: $e\n";
				}
			} else if($deviceIdentifier == BrickletAmbientLightV3::DEVICE_IDENTIFIER) {
				try {
					$this->brickletAmbientLightV3 = new BrickletAmbientLightV3($uid, $this->ipcon);
					$this->brickletAmbientLightV3->setConfiguration(BrickletAmbientLightV3::ILLUMINANCE_RANGE_64000LUX,
					                                                BrickletAmbientLightV3::INTEGRATION_TIME_200MS);
					$this->brickletAmbientLightV3->setIlluminanceCallbackConfiguration(1000, false, 'x', 0, 0);
					$this->brickletAmbientLightV3->registerCallback(BrickletAmbientLightV3::CALLBACK_ILLUMINANCE,
					                                                array($this, 'cb_illuminanceV3'));
					echo "Ambient Light 3.0 initialized\n";
				} catch(Exception $e) {
					$this->brickletAmbientLight = null;
					echo "Ambient Light 3.0 init failed: $e\n";
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
			} else if($deviceIdentifier == BrickletHumidity::DEVICE_IDENTIFIER) {
				try {
					$this->brickletHumidityV2 = new BrickletHumidityV2($uid, $this->ipcon);
					$this->brickletHumidityV2->setHumidityCallbackPeriod(1000, true, 'x', 0, 0);
					$this->brickletHumidityV2->registerCallback(BrickletHumidityV2::CALLBACK_HUMIDITY,
					                                            array($this, 'cb_humidityV2'));
					echo "Humidity 2.0 initialized\n";
				} catch(Exception $e) {
					$this->brickletHumidity = null;
					echo "Humidity 2.0 init failed: $e\n";
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
			} else if($deviceIdentifier == BrickletBarometerV2::DEVICE_IDENTIFIER) {
				try {
					$this->brickletBarometerV2 = new BrickletBarometerV2($uid, $this->ipcon);
					$this->brickletBarometerV2->setAirPressureCallbackConfiguration(1000, false, 'x', 0, 0);
					$this->brickletBarometerV2->registerCallback(BrickletBarometerV2::CALLBACK_AIR_PRESSURE,
					                                             array($this, 'cb_airPressureV2'));
					echo "Barometer 2.0 initialized\n";
				} catch(Exception $e) {
					$this->brickletBarometer = null;
					echo "Barometer 2.0 init failed: $e\n";
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
