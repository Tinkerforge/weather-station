<?php
require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletAmbientLight.php');
require_once('Tinkerforge/BrickletHumidity.php');
require_once('Tinkerforge/BrickletBarometer.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletAmbientLight;
use Tinkerforge\BrickletHumidity;
use Tinkerforge\BrickletBarometer;

$ipcon = new IPConnection();
$brickletAmbientLight = new BrickletAmbientLight("apy", $ipcon);
$brickletHumidity = new BrickletHumidity("7bA", $ipcon);
$brickletBarometer = new BrickletBarometer("d99", $ipcon);

$ipcon->connect("localhost", 4223);

$illuminance = $brickletAmbientLight->getIlluminance()/10.0;
$humidity = $brickletHumidity->getHumidity()/10.0;
$air_pressure = $brickletBarometer->getAirPressure()/1000.0;
$temperature = $brickletBarometer->getChipTemperature()/100.0;

$response = array (
    "illuminance"  => "Illuminance: $illuminance Lux",
    "humidity"     => "Humidity: $humidity %RH",
    "air_pressure" => "Air Pressure: $air_pressure mbar",
    "temperature"  => "Temperature: $temperature °C",
);

print_r(json_encode($response));
?>
