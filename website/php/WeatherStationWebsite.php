<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletAmbientLight.php');
require_once('Tinkerforge/BrickletAmbientLightV2.php');
require_once('Tinkerforge/BrickletHumidity.php');
require_once('Tinkerforge/BrickletHumidityV2.php');
require_once('Tinkerforge/BrickletBarometer.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletAmbientLight;
use Tinkerforge\BrickletAmbientLightV2;
use Tinkerforge\BrickletHumidity;
use Tinkerforge\BrickletHumidityV2;
use Tinkerforge\BrickletBarometer;

$ipcon = new IPConnection();
// NOTE: if you have a first generation Ambient Light Bricklet,
// then uncomment the next line and comment the line after that
//$brickletAmbientLight = new BrickletAmbientLight("apy", $ipcon);
$brickletAmbientLightV2 = new BrickletAmbientLightV2("bhL", $ipcon);
$brickletHumidity = new BrickletHumidity("7bA", $ipcon);
$brickletBarometer = new BrickletBarometer("d99", $ipcon);

$ipcon->connect("localhost", 4223);

// NOTE: if you have a first generation Ambient Light Bricklet,
// then uncomment the next line and comment the line after that
//$illuminance = $brickletAmbientLight->getIlluminance()/10.0;
$illuminance = $brickletAmbientLightV2->getIlluminance()/100.0;
// NOTE: if you have a first generation Humidity Bricklet,
// then uncomment the next line and comment the line after that
//$humidity = $brickletHumidity->getHumidity()/10.0;
$humidity = $brickletHumidityV2->getHumidity()/100.0;
$air_pressure = $brickletBarometer->getAirPressure()/1000.0;
$temperature = $brickletBarometer->getChipTemperature()/100.0;

$response = array (
    "illuminance"  => "Illuminance: $illuminance Lux",
    "humidity"     => "Humidity: $humidity %RH",
    "air_pressure" => "Air Pressure: $air_pressure mbar",
    "temperature"  => "Temperature: $temperature &deg;C",
);

print_r(json_encode($response));

?>
