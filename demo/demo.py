#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Starter Kit: Weather Station Demo Application
Copyright (C) 2013 Bastian Nordmeyer <bastian@tinkerforge.com>

demo.py: Entry file for Demo

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License 
as published by the Free Software Foundation; either version 2 
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.
"""

import socket
import sys
import time
import math

from tinkerforge.ip_connection import IPConnection
from tinkerforge.ip_connection import Error
from tinkerforge.bricklet_lcd_20x4 import LCD20x4
from tinkerforge.bricklet_ambient_light import AmbientLight
from tinkerforge.bricklet_humidity import Humidity
from tinkerforge.bricklet_barometer import Barometer

import sys
from PyQt4.QtGui import QApplication
from PyQt4.QtGui import QWidget
from PyQt4.QtGui import QGridLayout
from PyQt4.QtGui import QPalette
from PyQt4.QtGui import QTextFormat
from PyQt4.QtGui import QTabWidget
from PyQt4.QtCore import pyqtSignal, SIGNAL, SLOT



from Project_Env_Display import ProjectEnvDisplay
from Project_Statistics import ProjectStatistics
from Project_Xively import ProjectXively



class WeatherStation (QApplication):
    HOST = "localhost"
    PORT = 4223

    ipcon = None
    lcd = None
    al = None
    hum = None
    baro = None

    widget = None

    projects = []
    active_project = None



    def __init__(self, args):
        super(QApplication, self).__init__(args)

        self.tabs = QTabWidget()
        self.tabs.setFixedSize(700, 400)
        self.projects.append(ProjectEnvDisplay(self.tabs, self))
        self.projects.append(ProjectStatistics(self.tabs, self))
        self.projects.append(ProjectXively(self.tabs, self))

        self.tabs.addTab(self.projects[0], "Display Environment Measurements")
        self.tabs.addTab(self.projects[1], "Show Statistics with Button Control")
        self.tabs.addTab(self.projects[2], "Connect to Xively")

        self.active_project = self.projects[0]

        self.tabs.currentChanged.connect(self.tabChangedSlot)

        self.tabs.setWindowTitle("Starter Kit: Weather Station Demo")
        self.tabs.show()

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

        sys.exit(self.exec_())

    def tabChangedSlot(self, tabIndex):
        self.active_project = self.projects[tabIndex]

    def cb_illuminance(self, illuminance):
        for p in self.projects:
            p.update_illuminance(illuminance)

    def cb_humidity(self, humidity):
        for p in self.projects:
            p.update_humidity(humidity)

    def cb_air_pressure(self, air_pressure):
        for p in self.projects:
            p.update_air_pressure(air_pressure)

        try:
            temperature = self.baro.get_chip_temperature()
        except Error as e:
            log.error('Could not get temperature: ' + str(e.description))
            return

        for p in self.projects:
            p.update_temperature(temperature)

    def configure_custom_chars(self):
        c = [[0x00 for x in range(8)] for y in range(8)]
	
        c[0] = [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff]
        c[1] = [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff]
        c[2] = [0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff]
        c[3] = [0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff]
        c[4] = [0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff]
        c[5] = [0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
        c[6] = [0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
        c[7] = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]

        for i in range(len(c)):
            self.lcd.set_custom_character(i, c[i]);

    def cb_button_pressed(self, button):
        for p in self.projects:
            p.button_pressed(button)

    def cb_enumerate(self, uid, connected_uid, position, hardware_version,
                     firmware_version, device_identifier, enumeration_type):
        if enumeration_type == IPConnection.ENUMERATION_TYPE_CONNECTED or \
           enumeration_type == IPConnection.ENUMERATION_TYPE_AVAILABLE:
            if device_identifier == LCD20x4.DEVICE_IDENTIFIER:
                try:
                    self.lcd = LCD20x4(uid, self.ipcon)
                    self.lcd.clear_display()
                    self.lcd.backlight_on()
                    self.lcd.register_callback(self.lcd.CALLBACK_BUTTON_PRESSED, self.cb_button_pressed)
                    self.configure_custom_chars()

                except Error as e:
                    print('LCD 20x4 init failed: ' + str(e.description))
                    self.lcd = None
            elif device_identifier == AmbientLight.DEVICE_IDENTIFIER:
                try:
                    self.al = AmbientLight(uid, self.ipcon)
                    self.al.set_illuminance_callback_period(1000)
                    self.al.register_callback(self.al.CALLBACK_ILLUMINANCE,
                                              self.cb_illuminance)
                except Error as e:
                    print('Ambient Light init failed: ' + str(e.description))
                    self.al = None
            elif device_identifier == Humidity.DEVICE_IDENTIFIER:
                try:
                    self.hum = Humidity(uid, self.ipcon)
                    self.hum.set_humidity_callback_period(1000)
                    self.hum.register_callback(self.hum.CALLBACK_HUMIDITY,
                                               self.cb_humidity)
                except Error as e:
                    print('Humidity init failed: ' + str(e.description))
                    self.hum = None
            elif device_identifier == Barometer.DEVICE_IDENTIFIER:
                try:
                    self.baro = Barometer(uid, self.ipcon)
                    self.baro.set_air_pressure_callback_period(1000)
                    self.baro.register_callback(self.baro.CALLBACK_AIR_PRESSURE,
                                                self.cb_air_pressure)
                except Error as e:
                    print('Barometer init failed: ' + str(e.description))
                    self.baro = None

    def cb_connected(self, connected_reason):
        if connected_reason == IPConnection.CONNECT_REASON_AUTO_RECONNECT:
            log.info('Auto Reconnect')

            while True:
                try:
                    self.ipcon.enumerate()
                    break
                except Error as e:
                    print('Enumerate Error: ' + str(e.description))
                    time.sleep(1)

if __name__ == "__main__":
    weather_station = WeatherStation(sys.argv)

    if weather_station.ipcon != None:
        weather_station.ipcon.disconnect()
