#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Starter Kit: Weather Station Demo Application(Brick Viewer) 
Copyright (C) 2013 Bastian Nordmeyer <bastian@tinkerforge.com>

Project_Env_Display.py: Environment Display Project Implementation

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


from PyQt4.QtCore import pyqtSignal, SIGNAL, SLOT
from PyQt4.QtGui import QGridLayout
from PyQt4.QtGui import QWidget
from PyQt4.QtCore import QTimer

import httplib
import json

from LCDWidget import LCDWidget

import math


class ProjectXively(QWidget):

    qtcb_update_illuminance = pyqtSignal(float)
    qtcb_update_air_pressure = pyqtSignal(float)
    qtcb_update_temperature = pyqtSignal(float)
    qtcb_update_humidity = pyqtSignal(float)
    qtcb_button_pressed = pyqtSignal(int)

    lcdwidget = None

    xively_host = "api.xively.com"
    xively_agent = "Tinkerforge Weather Kit: Starter Kit Demo"
    xively_feed = ""
    xively_api_key = ""

    xively_items = {}
    xively_headers = None
    xively_params = ""
    xively_update_rate = 0.1 # in minutes

    xively_timer = None

    def __init__(self, parent, app):
        super(QWidget, self).__init__()

        self.lcdwidget = LCDWidget(self, app)
        self.lcdwidget.hide()
        
        self.grid = QGridLayout()
        #self.grid.addWidget(self.lcdwidget)
        self.setLayout(self.grid)

        self.qtcb_update_illuminance.connect(self.update_illuminance_data_slot)
        self.qtcb_update_air_pressure.connect(self.update_air_pressure_data_slot)
        self.qtcb_update_temperature.connect(self.update_temperature_data_slot)
        self.qtcb_update_humidity.connect(self.update_humidity_data_slot)
        self.qtcb_button_pressed.connect(self.button_pressed_slot)

        self.save_configuration()

    def save_configuration(self):
        self.xively_headers = {
            "Content-Type"  : "application/x-www-form-urlencoded",
            "X-ApiKey"      : self.xively_api_key,
            "User-Agent"    : self.xively_agent,
        }
        self.xively_params = "/v2/feeds/" + str(self.xively_feed)

        if self.xively_timer == None:
            self.xively_timer = QTimer(self)
            self.xively_timer.timeout.connect(self.update_xively)
            self.xively_timer.start(self.xively_update_rate*60*1000)

    def update_xively(self):

        self.lcdwidget.clear(self)
        self.lcdwidget.write_line(0,0,"Uploading...", self)

        stream_items = []
        for identifier, value in self.xively_items.items():
            stream_items.append({'id': identifier,
                                 'current_value': value[0],
                                 'min_value': value[1],
                                 'max_value': value[2]})

        data = {'version': '1.0.0',
                    'datastreams': stream_items}
        self.xively_items = {}
        body = json.dumps(data)

        try:
            http = httplib.HTTPSConnection(self.xively_host)
            http.request('PUT', self.xively_params, body, self.xively_headers)
            response = http.getresponse()
            http.close()

            if response.status != 200:
                print('Could not upload to xively -> ' +
                          str(response.status) + ': ' + response.reason)
        except Exception as e:
            print('HTTP error: ' + str(e))


    def put(self, identifier, value):
        self.lcdwidget.clear(self)
        try:
            _, min_value, max_value = self.xively_items[identifier]
            if value < min_value:
                min_value = value
            if value > max_value:
                max_value = value
            self.xively_items[identifier] = (value, min_value, max_value)
        except:
            self.xively_items[identifier] = (value, value, value)

    def update_illuminance_data_slot(self, illuminance):
        self.put('AmbientLight', illuminance/10.0)

    def update_illuminance(self, illuminance):
        self.qtcb_update_illuminance.emit(illuminance)

    def update_humidity_data_slot(self, humidity):
        self.put('Humidity', humidity/10.0)
    
    def update_humidity(self, humidity):
        self.qtcb_update_humidity.emit(humidity)

    def update_air_pressure_data_slot(self, air_pressure):
        self.put('AirPressure', air_pressure/1000.0)
    
    def update_air_pressure(self, air_pressure):
        self.qtcb_update_air_pressure.emit(air_pressure)

    def update_temperature_data_slot(self, temperature):
        self.put('Temperature', temperature/100.0)

    def update_temperature(self, temperature):
        self.qtcb_update_temperature.emit(temperature)

    def button_pressed_slot(self, button):
        pass

    def button_pressed(self, button):
        self.qtcb_button_pressed.emit(button)
        
        



