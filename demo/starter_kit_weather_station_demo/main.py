#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Starter Kit: Weather Station Demo
Copyright (C) 2013 Bastian Nordmeyer <bastian@tinkerforge.com>
Copyright (C) 2015 Matthias Bolte <matthias@tinkerforge.com>

main.py: Entry file for Starter Kit: Weather Station Demo

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

import sys
if (sys.hexversion & 0xFF000000) != 0x03000000:
    print('Python 3.x required')
    sys.exit(1)

import os

def prepare_package(package_name):
    # from http://www.py2exe.org/index.cgi/WhereAmI
    if hasattr(sys, 'frozen'):
        program_path = os.path.dirname(os.path.realpath(sys.executable))
    else:
        program_path = os.path.dirname(os.path.realpath(__file__))

    # add program_path so OpenGL is properly imported
    sys.path.insert(0, program_path)

    # allow the program to be directly started by calling 'main.py'
    # without '<package_name>' being in the path already
    if package_name not in sys.modules:
        head, tail = os.path.split(program_path)

        if head not in sys.path:
            sys.path.insert(0, head)

        if not hasattr(sys, 'frozen'):
            # load and inject in modules list, this allows to have the source in a
            # directory named differently than '<package_name>'
            sys.modules[package_name] = __import__(tail, globals(), locals())

prepare_package('starter_kit_weather_station_demo')

import socket
import time
import math
import signal

from PyQt5.QtWidgets import QApplication, QWidget, QErrorMessage, QGridLayout, QTabWidget, QMainWindow, QVBoxLayout
from PyQt5.QtGui import QPalette, QTextFormat, QFont, QIcon
from PyQt5.QtCore import QTimer, pyqtSignal

from starter_kit_weather_station_demo.tinkerforge.ip_connection import IPConnection, Error
from starter_kit_weather_station_demo.tinkerforge.bricklet_lcd_20x4 import BrickletLCD20x4
from starter_kit_weather_station_demo.tinkerforge.bricklet_ambient_light import BrickletAmbientLight
from starter_kit_weather_station_demo.tinkerforge.bricklet_ambient_light_v2 import BrickletAmbientLightV2
from starter_kit_weather_station_demo.tinkerforge.bricklet_ambient_light_v3 import BrickletAmbientLightV3
from starter_kit_weather_station_demo.tinkerforge.bricklet_humidity import BrickletHumidity
from starter_kit_weather_station_demo.tinkerforge.bricklet_humidity_v2 import BrickletHumidityV2
from starter_kit_weather_station_demo.tinkerforge.bricklet_barometer import BrickletBarometer
from starter_kit_weather_station_demo.tinkerforge.bricklet_barometer_v2 import BrickletBarometerV2
from starter_kit_weather_station_demo.Project_Env_Display import ProjectEnvDisplay
from starter_kit_weather_station_demo.Project_Statistics import ProjectStatistics
from starter_kit_weather_station_demo.config import DEMO_VERSION
from starter_kit_weather_station_demo.load_pixmap import load_pixmap, get_resources_path

def load_commit_id(name):
    try:
        # Don't warn if the file is missing, as it is expected when run from source.
        path = get_resources_path(name, warn_on_missing_file=False)

        if path is not None:
            with open(path, 'r') as f:
                return f.read().strip()
    except FileNotFoundError:
        pass

    return None

INTERNAL = load_commit_id('internal')

SNAPSHOT = load_commit_id('snapshot')

DEMO_FULL_VERSION = DEMO_VERSION

if INTERNAL != None:
    DEMO_FULL_VERSION += '+internal~{}'.format(INTERNAL)
elif SNAPSHOT != None:
    DEMO_FULL_VERSION += '+snapshot~{}'.format(SNAPSHOT)

class MainWindow(QMainWindow):
    def __init__(self, app, parent=None):
        super().__init__(parent)

        self.app = app

    def closeEvent(self, event):
        self.app.exit_demo()

class WeatherStation(QApplication):
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

    projects = []
    active_project = None

    error_msg = None

    def __init__(self, args):
        super().__init__(args)

        self.error_msg = QErrorMessage()
        self.ipcon = IPConnection()

        signal.signal(signal.SIGINT, self.exit_demo)
        signal.signal(signal.SIGTERM, self.exit_demo)

        timer = QTimer(self)
        timer.setSingleShot(True)
        timer.timeout.connect(self.connect)
        timer.start(1)

    def exit_demo(self, signl=None, frme=None):
        try:
            self.ipcon.disconnect()
            self.timer.stop()
            self.tabs.destroy()
        except:
            pass

        sys.exit()

    def open_gui(self):
        self.main = MainWindow(self)
        self.main.setFixedSize(730, 430)
        self.main.setWindowIcon(QIcon(load_pixmap('starter_kit_weather_station_demo-icon.png')))

        self.tabs = QTabWidget()

        widget = QWidget()
        layout = QVBoxLayout()
        layout.addWidget(self.tabs)
        widget.setLayout(layout)

        self.main.setCentralWidget(widget)

        self.projects.append(ProjectEnvDisplay(self.tabs, self))
        self.projects.append(ProjectStatistics(self.tabs, self))

        self.tabs.addTab(self.projects[0], "Display Environment Measurements")
        self.tabs.addTab(self.projects[1], "Show Statistics with Button Control")

        self.active_project = self.projects[0]

        self.tabs.currentChanged.connect(self.tabChangedSlot)

        self.main.setWindowTitle("Starter Kit: Weather Station Demo " + DEMO_FULL_VERSION)
        self.main.show()

    def connect(self):
        try:
            self.ipcon.connect(WeatherStation.HOST, WeatherStation.PORT)
        except Error as e:
            self.error_msg.showMessage('Connection Error: ' + str(e.description) + "\nBrickd installed and running?")
            return
        except socket.error as e:
            self.error_msg.showMessage('Socket error: ' + str(e) + "\nBrickd installed and running?")
            return

        self.ipcon.register_callback(IPConnection.CALLBACK_ENUMERATE,
                                     self.cb_enumerate)
        self.ipcon.register_callback(IPConnection.CALLBACK_CONNECTED,
                                     self.cb_connected)

        try:
            self.ipcon.enumerate()
        except Error as e:
            self.error_msg.showMessage('Enumerate Error: ' + str(e.description))
            return

        self.open_gui()

    def tabChangedSlot(self, tabIndex):

        if self.lcd is not None:
            self.lcd.clear_display()

        self.active_project = self.projects[tabIndex]

    def cb_illuminance(self, illuminance):
        for p in self.projects:
            p.update_illuminance(illuminance/10.0)

    def cb_illuminance_v2(self, illuminance):
        for p in self.projects:
            p.update_illuminance(illuminance/100.0)

    def cb_illuminance_v3(self, illuminance):
        for p in self.projects:
            p.update_illuminance(illuminance/100.0)

    def cb_humidity(self, humidity):
        for p in self.projects:
            p.update_humidity(humidity/10.0)

    def cb_humidity_v2(self, humidity):
        for p in self.projects:
            p.update_humidity(humidity/100.0)

    def cb_air_pressure(self, air_pressure):
        for p in self.projects:
            p.update_air_pressure(air_pressure/1000.0)

        try:
            temperature = self.baro.get_chip_temperature()
        except Error as e:
            print('Could not get temperature: ' + str(e.description))
            return

        for p in self.projects:
            p.update_temperature(temperature/100.0)

    def cb_air_pressure_v2(self, air_pressure):
        for p in self.projects:
            p.update_air_pressure(air_pressure/1000.0)

        try:
            temperature = self.baro_v2.get_temperature()
        except Error as e:
            print('Could not get temperature: ' + str(e.description))
            return

        for p in self.projects:
            p.update_temperature(temperature/100.0)

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
            if device_identifier == BrickletLCD20x4.DEVICE_IDENTIFIER:
                try:
                    self.lcd = BrickletLCD20x4(uid, self.ipcon)
                    self.lcd.clear_display()
                    self.lcd.backlight_on()
                    self.lcd.register_callback(self.lcd.CALLBACK_BUTTON_PRESSED, self.cb_button_pressed)
                    self.configure_custom_chars()

                except Error as e:
                    self.error_msg.showMessage('LCD 20x4 init failed: ' + str(e.description))
                    self.lcd = None
            elif device_identifier == BrickletAmbientLight.DEVICE_IDENTIFIER:
                try:
                    self.al = BrickletAmbientLight(uid, self.ipcon)
                    self.al.set_illuminance_callback_period(1000)
                    self.al.register_callback(self.al.CALLBACK_ILLUMINANCE,
                                              self.cb_illuminance)
                except Error as e:
                    self.error_msg.showMessage('Ambient Light init failed: ' + str(e.description))
                    self.al = None
            elif device_identifier == BrickletAmbientLightV2.DEVICE_IDENTIFIER:
                try:
                    self.al_v2 = BrickletAmbientLightV2(uid, self.ipcon)
                    self.al_v2.set_configuration(self.al_v2.ILLUMINANCE_RANGE_64000LUX,
                                                 self.al_v2.INTEGRATION_TIME_200MS)
                    self.al_v2.set_illuminance_callback_period(1000)
                    self.al_v2.register_callback(self.al_v2.CALLBACK_ILLUMINANCE,
                                                 self.cb_illuminance_v2)
                except Error as e:
                    self.error_msg.showMessage('Ambient Light 2.0 init failed: ' + str(e.description))
                    self.al_v2 = None
            elif device_identifier == BrickletAmbientLightV3.DEVICE_IDENTIFIER:
                try:
                    self.al_v3 = BrickletAmbientLightV3(uid, self.ipcon)
                    self.al_v3.set_illuminance_callback_configuration(1000, False, 'x', 0, 0)
                    self.al_v3.register_callback(self.al_v3.CALLBACK_ILLUMINANCE,
                                                 self.cb_illuminance_v3)
                except Error as e:
                    self.error_msg.showMessage('Ambient Light 3.0 init failed: ' + str(e.description))
                    self.al_v3 = None
            elif device_identifier == BrickletHumidity.DEVICE_IDENTIFIER:
                try:
                    self.hum = BrickletHumidity(uid, self.ipcon)
                    self.hum.set_humidity_callback_period(1000)
                    self.hum.register_callback(self.hum.CALLBACK_HUMIDITY,
                                               self.cb_humidity)
                except Error as e:
                    self.error_msg.showMessage('Humidity init failed: ' + str(e.description))
                    self.hum = None
            elif device_identifier == BrickletHumidityV2.DEVICE_IDENTIFIER:
                try:
                    self.hum_v2 = BrickletHumidityV2(uid, self.ipcon)
                    self.hum_v2.set_humidity_callback_configuration(1000, True, 'x', 0, 0)
                    self.hum_v2.register_callback(self.hum_v2.CALLBACK_HUMIDITY,
                                                  self.cb_humidity_v2)
                except Error as e:
                    self.error_msg.showMessage('Humidity 2.0 init failed: ' + str(e.description))
                    self.hum_v2 = None
            elif device_identifier == BrickletBarometer.DEVICE_IDENTIFIER:
                try:
                    self.baro = BrickletBarometer(uid, self.ipcon)
                    self.baro.set_air_pressure_callback_period(1000)
                    self.baro.register_callback(self.baro.CALLBACK_AIR_PRESSURE,
                                                self.cb_air_pressure)
                except Error as e:
                    self.error_msg.showMessage('Barometer init failed: ' + str(e.description))
                    self.baro = None
            elif device_identifier == BrickletBarometerV2.DEVICE_IDENTIFIER:
                try:
                    self.baro_v2 = BrickletBarometerV2(uid, self.ipcon)
                    self.baro_v2.set_air_pressure_callback_configuration(1000, False, 'x', 0, 0)
                    self.baro_v2.register_callback(self.baro_v2.CALLBACK_AIR_PRESSURE,
                                                   self.cb_air_pressure_v2)
                except Error as e:
                    self.error_msg.showMessage('Barometer 2.0 init failed: ' + str(e.description))
                    self.baro_v2 = None

    def cb_connected(self, connected_reason):
        if connected_reason == IPConnection.CONNECT_REASON_AUTO_RECONNECT:

            while True:
                try:
                    self.ipcon.enumerate()
                    break
                except Error as e:
                    self.error_msg.showMessage('Enumerate Error: ' + str(e.description))
                    time.sleep(1)

if __name__ == "__main__":
    argv = sys.argv

    sys.exit(WeatherStation(argv).exec_())
