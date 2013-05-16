#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Starter Kit: Weather Station Demo Application
Copyright (C) 2013 Bastian Nordmeyer <bastian@tinkerforge.com>

Project_Xively.py: Xively Data Upload Project Implementation

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
from PyQt4.QtGui import QCheckBox
from PyQt4.QtGui import QTextEdit
from PyQt4.QtGui import QPushButton
from PyQt4.QtGui import QLabel
from PyQt4.QtGui import QFont
from PyQt4.QtGui import QErrorMessage
from PyQt4.QtCore import QTimer
from PyQt4.QtCore import Qt


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
    xively_feed = "Enter Feed ID here"
    xively_api_key = "Enter API Key here"

    xively_items = {}
    xively_headers = None
    xively_params = ""
    xively_update_rate = 5 # in minutes

    text_agent = None
    text_feed = None
    text_key = None
    text_rate = None

    save_button = None

    xively_timer = None

    error_message = None

    label_upload_active = None

    def __init__(self, parent, app):
        super(QWidget, self).__init__()

        self.lcdwidget = LCDWidget(self, app)
        self.lcdwidget.hide()

        self.text_agent = QTextEdit(self)
        self.text_agent.setPlainText(str(self.xively_agent))
        self.text_agent.setFixedSize(400,30)
        self.text_feed = QTextEdit(self)
        self.text_feed.setPlainText(str(self.xively_feed))
        self.text_feed.setFixedSize(400,30)
        self.text_key = QTextEdit(self)
        self.text_key.setPlainText(str(self.xively_api_key))
        self.text_key.setFixedSize(400,30)
        self.text_rate = QTextEdit(self)
        self.text_rate.setPlainText(str(self.xively_update_rate))
        self.text_rate.setFixedSize(100,30)

        self.save_button = QPushButton("Save/Activate")
        self.save_button.setFixedSize(120,30)
        
        self.grid = QGridLayout()

        label = QLabel(self)
        label.setText("Project: <b>Connect to Xively</b>. This project uploads the measured values to Xively. Please find documentation how to configure it and program sources in Python <a href=\"http://www.tinkerforge.com/en/doc/Kits/WeatherStation/WeatherStation.html#connect-to-xively\">here</a>.")
        label.setTextFormat(Qt.RichText)
        label.setTextInteractionFlags(Qt.TextBrowserInteraction)
        label.setOpenExternalLinks(True)
        label.setWordWrap(True)
        self.grid.addWidget(label, 0, 0, 1, 2)

        self.grid.addWidget(QLabel("Agent Description:"),1,0)
        self.grid.addWidget(self.text_agent,1,1)
        self.grid.addWidget(QLabel("Feed:"),2,0)
        self.grid.addWidget(self.text_feed,2,1)
        self.grid.addWidget(QLabel("Key:"),3,0)
        self.grid.addWidget(self.text_key,3,1)
        self.grid.addWidget(QLabel("Update Rate (min):"),4,0)
        self.grid.addWidget(self.text_rate,4,1)
        self.grid.addWidget(self.save_button,5,1)

        self.label_upload_active = QLabel("Not Active", self)
        font = QFont()
        font.setPixelSize(20)
        self.label_upload_active.setFont(font)
        self.label_upload_active.setFixedSize(120, 50)
        self.set_active_label(False)
        self.grid.addWidget(self.label_upload_active,5,0)

        self.setLayout(self.grid)

        self.qtcb_update_illuminance.connect(self.update_illuminance_data_slot)
        self.qtcb_update_air_pressure.connect(self.update_air_pressure_data_slot)
        self.qtcb_update_temperature.connect(self.update_temperature_data_slot)
        self.qtcb_update_humidity.connect(self.update_humidity_data_slot)
        self.qtcb_button_pressed.connect(self.button_pressed_slot)
        self.save_button.clicked.connect(self.save_configuration)

        self.error_message = QErrorMessage(self)

    def set_active_label(self, value):
        palette = self.label_upload_active.palette()
        if value:
            palette.setColor(self.foregroundRole(), Qt.green)
            self.label_upload_active.setText("Active")
        else:
            palette.setColor(self.foregroundRole(), Qt.red)
            self.label_upload_active.setText("Not Active")

        self.label_upload_active.setPalette(palette)

    def save_configuration(self):

        self.xively_agent = self.text_agent.toPlainText()
        self.xively_feed = self.text_feed.toPlainText()
        self.xively_api_key = self.text_key.toPlainText()
        self.xively_update_rate = float(self.text_rate.toPlainText())

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

        self.set_active_label(True)
        self.update_xively()

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
                self.error_message.showMessage('Could not upload to xively -> Response:' +
                          str(response.status) + ': ' + response.reason + '. Check your configuration.')
                self.xively_timer.stop()
                self.xively_timer = None
                self.set_active_label(False)
        except Exception as e:
            self.error_message.showMessage('HTTP error: ' + str(e))
            self.xively_timer.stop()
            self.xively_timer = None
            self.set_active_label(False)


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
        
        



