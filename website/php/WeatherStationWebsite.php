<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletHumidity.php');
require_once('Tinkerforge/BrickletHumidityV2.php');
require_once('Tinkerforge/BrickletBarometer.php');
require_once('Tinkerforge/BrickletBarometerV2.php');
require_once('Tinkerforge/BrickletAmbientLight.php');
require_once('Tinkerforge/BrickletAmbientLightV2.php');
require_once('Tinkerforge/BrickletAmbientLightV3.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletHumidity;
use Tinkerforge\BrickletHumidityV2;
use Tinkerforge\BrickletBarometer;
use Tinkerforge\BrickletBarometerV2;
use Tinkerforge\BrickletAmbientLight;
use Tinkerforge\BrickletAmbientLightV2;
use Tinkerforge\BrickletAmbientLightV3;

/*
    IMPORTANT:

    Configure the Bricklet versions and their UIDs according to the setup.
*/
define(
    'CONFIG',
    array(
        'ambient_light' => array(
                            'version' => 3,
                            'uid' => 'HqJ'),
        'humidity' => array(
                            'version' => 1,
                            'uid' => 'zfs'),
        'barometer' => array(
                            'version' => 2,
                            'uid' => 'GvG')));

$ipcon = new IPConnection();

$humidity = NULL;
$temperature = NULL;
$illuminance = NULL;
$air_pressure = NULL;

$bricklet_humidity = NULL;
$bricklet_barometer = NULL;
$bricklet_ambientLight = NULL;

// Get Bricklet objects.

// Humidity Bricklet.
if (CONFIG['humidity']['version'] == 1) {
    $bricklet_humidity =
        new BrickletHumidity(CONFIG['humidity']['uid'], $ipcon);
}
elseif (CONFIG['humidity']['version'] == 2) {
    $bricklet_humidity =
        new BrickletHumidityV2(CONFIG['humidity']['uid'], $ipcon);
}

// Barometer Bricklet.
if (CONFIG['barometer']['version'] == 1) {
    $bricklet_barometer =
        new BrickletBarometer(CONFIG['barometer']['uid'], $ipcon);
}
elseif (CONFIG['barometer']['version'] == 2) {
    $bricklet_barometer =
        new BrickletBarometerV2(CONFIG['barometer']['uid'], $ipcon);
}

// Ambient Light Bricklet.
if (CONFIG['ambient_light']['version'] == 1) {
    $bricklet_ambientLight =
        new BrickletAmbientLight(CONFIG['ambient_light']['uid'], $ipcon);
}
elseif (CONFIG['ambient_light']['version'] == 2) {
    $bricklet_ambientLight =
        new BrickletAmbientLightV2(CONFIG['ambient_light']['uid'], $ipcon);
}
elseif (CONFIG['ambient_light']['version'] == 3) {
    $bricklet_ambientLight =
        new BrickletAmbientLightV3(CONFIG['ambient_light']['uid'], $ipcon);
}

// Connect.
$ipcon->connect('localhost', 4223);

// Read from the Bricklets.
if ($bricklet_humidity) {
    $humidity = $bricklet_humidity->getHumidity() / 100.0;
}

if ($bricklet_barometer) {
    $air_pressure = $bricklet_barometer->getAirPressure() / 1000.0;

    if(CONFIG['barometer']['version'] == 1) {
        $temperature = $bricklet_barometer->getChipTemperature() / 100.0;
    }
    elseif(CONFIG['barometer']['version'] == 2) {
        $temperature = $bricklet_barometer->getTemperature() / 100.0;
    }
}

if ($bricklet_ambientLight) {
    $illuminance = $bricklet_ambientLight->getIlluminance() / 100.0;
}

// Prepare the response.
$response =
    array (
        'illuminance'  => "Illuminance: $illuminance Lux",
        'humidity'     => "Humidity: $humidity %RH",
        'air_pressure' => "Air Pressure: $air_pressure mbar",
        'temperature'  => "Temperature: $temperature &deg;C");

// Send the response.
print_r(json_encode($response));

?>
