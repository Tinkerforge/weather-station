#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import sys
import time
import math
import logging as log
log.basicConfig(level=log.INFO)

from tinkerforge.ip_connection import IPConnection
from tinkerforge.ip_connection import Error
from tinkerforge.bricklet_lcd_20x4 import BrickletLCD20x4
from tinkerforge.bricklet_ambient_light import BrickletAmbientLight
from tinkerforge.bricklet_ambient_light_v2 import BrickletAmbientLightV2
from tinkerforge.bricklet_ambient_light_v3 import BrickletAmbientLightV3
from tinkerforge.bricklet_humidity import BrickletHumidity
from tinkerforge.bricklet_humidity_v2 import BrickletHumidityV2
from tinkerforge.bricklet_barometer import BrickletBarometer
from tinkerforge.bricklet_barometer_v2 import BrickletBarometerV2

class WeatherStation:
    HOST = "localhost"
    PORT = 4223

    ipcon = None
    lcd = None
    al = None
    al_v2 = None
    al_v3 = None
    hum = None
    hum_v2 = None
    baro = None
    baro_v2 = None

    def __init__(self):
        self.ipcon = IPConnection()
        while True:
            try:
                self.ipcon.connect(WeatherStation.HOST, WeatherStation.PORT)
                break
            except Error as e:
                log.error('Connection Error: ' + str(e.description))
                time.sleep(1)
            except socket.error as e:
                log.error('Socket error: ' + str(e))
                time.sleep(1)

        self.ipcon.register_callback(IPConnection.CALLBACK_ENUMERATE,
                                     self.cb_enumerate)
        self.ipcon.register_callback(IPConnection.CALLBACK_CONNECTED,
                                     self.cb_connected)

        while True:
            try:
                self.ipcon.enumerate()
                break
            except Error as e:
                log.error('Enumerate Error: ' + str(e.description))
                time.sleep(1)

    def cb_illuminance(self, illuminance):
        if self.lcd is not None:
            text = 'Illuminanc %6.2f lx' % (illuminance/10.0)
            self.lcd.write_line(0, 0, text)
            log.info('Write to line 0: ' + text)

    def cb_illuminance_v2(self, illuminance):
        if self.lcd is not None:
            text = 'Illumina %8.2f lx' % (illuminance/100.0)
            self.lcd.write_line(0, 0, text)
            log.info('Write to line 0: ' + text)

    def cb_illuminance_v3(self, illuminance):
        if self.lcd is not None:
            text = 'Illumina %8.2f lx' % (illuminance/100.0)
            self.lcd.write_line(0, 0, text)
            log.info('Write to line 0: ' + text)

    def cb_humidity(self, humidity):
        if self.lcd is not None:
            text = 'Humidity   %6.2f %%' % (humidity/10.0)
            self.lcd.write_line(1, 0, text)
            log.info('Write to line 1: ' + text)

    def cb_humidity_v2(self, humidity):
        if self.lcd is not None:
            text = 'Humidity   %6.2f %%' % (humidity/100.0)
            self.lcd.write_line(1, 0, text)
            log.info('Write to line 1: ' + text)

    def cb_air_pressure(self, air_pressure):
        if self.lcd is not None:
            text = 'Air Press %7.2f mb' % (air_pressure/1000.0)
            self.lcd.write_line(2, 0, text)
            log.info('Write to line 2: ' + text)

            try:
                temperature = self.baro.get_chip_temperature()
            except Error as e:
                log.error('Could not get temperature: ' + str(e.description))
                return

            # \xDF == ° on LCD 20x4 charset
            text = 'Temperature %5.2f \xDFC' % (temperature/100.0)
            self.lcd.write_line(3, 0, text)
            log.info('Write to line 3: ' + text.replace('\xDF', '°'))

    def cb_air_pressure_v2(self, air_pressure):
        if self.lcd is not None:
            text = 'Air Press %7.2f mb' % (air_pressure/1000.0)
            self.lcd.write_line(2, 0, text)
            log.info('Write to line 2: ' + text)

            try:
                temperature = self.baro_v2.get_temperature()
            except Error as e:
                log.error('Could not get temperature: ' + str(e.description))
                return

            # \xDF == ° on LCD 20x4 charset
            text = 'Temperature %5.2f \xDFC' % (temperature/100.0)
            self.lcd.write_line(3, 0, text)
            log.info('Write to line 3: ' + text.replace('\xDF', '°'))

    def cb_enumerate(self, uid, connected_uid, position, hardware_version,
                     firmware_version, device_identifier, enumeration_type):
        if enumeration_type == IPConnection.ENUMERATION_TYPE_CONNECTED or \
           enumeration_type == IPConnection.ENUMERATION_TYPE_AVAILABLE:
            if device_identifier == BrickletLCD20x4.DEVICE_IDENTIFIER:
                try:
                    self.lcd = BrickletLCD20x4(uid, self.ipcon)
                    self.lcd.clear_display()
                    self.lcd.backlight_on()
                    log.info('LCD 20x4 initialized')
                except Error as e:
                    log.error('LCD 20x4 init failed: ' + str(e.description))
                    self.lcd = None
            elif device_identifier == BrickletAmbientLight.DEVICE_IDENTIFIER:
                try:
                    self.al = BrickletAmbientLight(uid, self.ipcon)
                    self.al.set_illuminance_callback_period(1000)
                    self.al.register_callback(self.al.CALLBACK_ILLUMINANCE,
                                              self.cb_illuminance)
                    log.info('Ambient Light initialized')
                except Error as e:
                    log.error('Ambient Light init failed: ' + str(e.description))
                    self.al = None
            elif device_identifier == BrickletAmbientLightV2.DEVICE_IDENTIFIER:
                try:
                    self.al_v2 = BrickletAmbientLightV2(uid, self.ipcon)
                    self.al_v2.set_configuration(self.al_v2.ILLUMINANCE_RANGE_64000LUX,
                                                 self.al_v2.INTEGRATION_TIME_200MS)
                    self.al_v2.set_illuminance_callback_period(1000)
                    self.al_v2.register_callback(self.al_v2.CALLBACK_ILLUMINANCE,
                                                 self.cb_illuminance_v2)
                    log.info('Ambient Light 2.0 initialized')
                except Error as e:
                    log.error('Ambient Light 2.0 init failed: ' + str(e.description))
                    self.al_v2 = None
            elif device_identifier == BrickletAmbientLightV3.DEVICE_IDENTIFIER:
                try:
                    self.al_v3 = BrickletAmbientLightV3(uid, self.ipcon)
                    self.al_v3.set_configuration(self.al_v3.ILLUMINANCE_RANGE_64000LUX,
                                                 self.al_v3.INTEGRATION_TIME_200MS)
                    self.al_v3.set_illuminance_callback_configuration(1000, False, 'x', 0, 0)
                    self.al_v3.register_callback(self.al_v3.CALLBACK_ILLUMINANCE,
                                                 self.cb_illuminance_v3)
                    log.info('Ambient Light 3.0 initialized')
                except Error as e:
                    log.error('Ambient Light 3.0 init failed: ' + str(e.description))
                    self.al_v3 = None
            elif device_identifier == BrickletHumidity.DEVICE_IDENTIFIER:
                try:
                    self.hum = BrickletHumidity(uid, self.ipcon)
                    self.hum.set_humidity_callback_period(1000)
                    self.hum.register_callback(self.hum.CALLBACK_HUMIDITY,
                                               self.cb_humidity)
                    log.info('Humidity initialized')
                except Error as e:
                    log.error('Humidity init failed: ' + str(e.description))
                    self.hum = None
            elif device_identifier == BrickletHumidityV2.DEVICE_IDENTIFIER:
                try:
                    self.hum_v2 = BrickletHumidityV2(uid, self.ipcon)
                    self.hum_v2.set_humidity_callback_configuration(1000, True, 'x', 0, 0)
                    self.hum_v2.register_callback(self.hum_v2.CALLBACK_HUMIDITY,
                                                  self.cb_humidity_v2)
                    log.info('Humidity 2.0 initialized')
                except Error as e:
                    log.error('Humidity 2.0 init failed: ' + str(e.description))
                    self.hum_v2 = None
            elif device_identifier == BrickletBarometer.DEVICE_IDENTIFIER:
                try:
                    self.baro = BrickletBarometer(uid, self.ipcon)
                    self.baro.set_air_pressure_callback_period(1000)
                    self.baro.register_callback(self.baro.CALLBACK_AIR_PRESSURE,
                                                self.cb_air_pressure)
                    log.info('Barometer initialized')
                except Error as e:
                    log.error('Barometer init failed: ' + str(e.description))
                    self.baro = None
            elif device_identifier == BrickletBarometerV2.DEVICE_IDENTIFIER:
                try:
                    self.baro_v2 = BrickletBarometerV2(uid, self.ipcon)
                    self.baro_v2.set_air_pressure_callback_configuration(1000, False, 'x', 0, 0)
                    self.baro_v2.register_callback(self.baro_v2.CALLBACK_AIR_PRESSURE,
                                                   self.cb_air_pressure_v2)
                    log.info('Barometer initialized')
                except Error as e:
                    log.error('Barometer init failed: ' + str(e.description))
                    self.baro_v2 = None

    def cb_connected(self, connected_reason):
        if connected_reason == IPConnection.CONNECT_REASON_AUTO_RECONNECT:
            log.info('Auto Reconnect')

            while True:
                try:
                    self.ipcon.enumerate()
                    break
                except Error as e:
                    log.error('Enumerate Error: ' + str(e.description))
                    time.sleep(1)

if __name__ == "__main__":
    log.info('Weather Station: Start')

    weather_station = WeatherStation()

    if sys.version_info < (3, 0):
        input = raw_input # Compatibility for Python 2.x
    input('Press key to exit\n')

    if weather_station.ipcon != None:
        weather_station.ipcon.disconnect()

    log.info('Weather Station: End')
