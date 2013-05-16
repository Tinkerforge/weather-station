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
from PyQt4.QtGui import QVBoxLayout, QHBoxLayout, QWidget, QCheckBox, QLineEdit, QSpinBox
from PyQt4.QtGui import QPushButton, QLabel, QFont, QErrorMessage, QSpacerItem
from PyQt4.QtCore import QTimer, Qt


import time
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
    xively_agent = "Tinkerforge Starter Kit Weather Station Demo"
    xively_channel = "Enter Feed ID here"
    xively_api_key = "Enter API Key here"

    xively_items = {}
    xively_headers = None
    xively_params = ""
    xively_update_rate = 5 # in minutes

    text_agent = None
    text_channel = None
    text_api_key = None
    number_update_rate = None

    save_button = None

    xively_timer = None

    error_message = None

    label_upload_active = None

    last_upload = None

    def __init__(self, parent, app):
        super(QWidget, self).__init__()

        self.lcdwidget = LCDWidget(self, app)
        self.lcdwidget.hide()

        self.text_agent = QLineEdit(self)
        self.text_agent.setText(self.xively_agent)
        
        self.text_channel = QLineEdit(self)
        self.text_channel.setText(self.xively_channel)
        
        self.text_api_key = QLineEdit(self)
        self.text_api_key.setText(self.xively_api_key)
        
        self.number_update_rate = QSpinBox(self)
        self.number_update_rate.setRange(1, 1440)
        self.number_update_rate.setSuffix(' min')
        self.number_update_rate.setValue(self.xively_update_rate)

        layout1 = QHBoxLayout()
        layout2 = QVBoxLayout()
        
        layout1.addStretch()
        layout1.addLayout(layout2)
        layout1.addStretch()
        
        layout2.addSpacerItem(QSpacerItem(LCDWidget.FIXED_WIDGTH, 0))

        label = QLabel(self)
        label.setText("Project: <b>Connect to Xively</b>. This project uploads the measured values to Xively. Please find documentation how to configure it and program sources in Python <a href=\"http://www.tinkerforge.com/en/doc/Kits/WeatherStation/WeatherStation.html#connect-to-xively\">here</a>.<br>")
        label.setTextFormat(Qt.RichText)
        label.setTextInteractionFlags(Qt.TextBrowserInteraction)
        label.setOpenExternalLinks(True)
        label.setWordWrap(True)
        label.setAlignment(Qt.AlignJustify)

        layout2.addSpacing(10)
        layout2.addWidget(label)
        layout2.addSpacing(10)

        layout3a = QHBoxLayout()
        label = QLabel("Agent Description:")
        label.setMinimumWidth(150)
        layout3a.addWidget(label)
        layout3a.addWidget(self.text_agent, 1)

        layout2.addLayout(layout3a)
        layout2.addSpacing(10)

        layout3b = QHBoxLayout()
        label = QLabel("Feed:")
        label.setMinimumWidth(150)
        layout3b.addWidget(label)
        layout3b.addWidget(self.text_channel, 1)

        layout2.addLayout(layout3b)
        layout2.addSpacing(10)

        layout3c = QHBoxLayout()
        label = QLabel("API Key:")
        label.setMinimumWidth(150)
        layout3c.addWidget(label)
        layout3c.addWidget(self.text_api_key, 1)

        layout2.addLayout(layout3c)
        layout2.addSpacing(10)

        layout3d = QHBoxLayout()
        label = QLabel("Update Rate:")
        label.setMinimumWidth(150)
        layout3d.addWidget(label)
        layout3d.addWidget(self.number_update_rate, 1)

        layout2.addLayout(layout3d)
        layout2.addSpacing(10)

        self.label_upload_active = QLabel("Not Active", self)
        self.label_upload_active.setMinimumWidth(150)
        font = QFont()
        font.setPixelSize(20)
        self.label_upload_active.setFont(font)
        self.set_active_label(False)

        self.save_button = QPushButton("Save/Activate")

        layout4 = QHBoxLayout()
        layout4.addWidget(self.label_upload_active)
        layout4.addWidget(self.save_button, 1)

        layout2.addLayout(layout4)
        layout2.addStretch()

        self.setLayout(layout1)

        self.qtcb_update_illuminance.connect(self.update_illuminance_data_slot)
        self.qtcb_update_air_pressure.connect(self.update_air_pressure_data_slot)
        self.qtcb_update_temperature.connect(self.update_temperature_data_slot)
        self.qtcb_update_humidity.connect(self.update_humidity_data_slot)
        self.qtcb_button_pressed.connect(self.button_pressed_slot)
        self.save_button.clicked.connect(self.save_configuration)

        self.lcdwidget.clear(self)

        self.error_message = QErrorMessage(self)

    def set_active_label(self, value):
        palette = self.label_upload_active.palette()
        if value:
            palette.setColor(self.foregroundRole(), Qt.darkGreen)
            self.label_upload_active.setText("Active")
        else:
            palette.setColor(self.foregroundRole(), Qt.red)
            self.label_upload_active.setText("Not Active")

        self.label_upload_active.setPalette(palette)

    def save_configuration(self):
        self.xively_agent = str(self.text_agent.text())
        self.xively_channel = str(self.text_channel.text())
        self.xively_api_key = str(self.text_api_key.text())
        self.xively_update_rate = self.number_update_rate.value()

        self.xively_headers = {
            "Content-Type"  : "application/x-www-form-urlencoded",
            "X-ApiKey"      : self.xively_api_key,
            "User-Agent"    : self.xively_agent,
        }
        self.xively_params = "/v2/feeds/" + self.xively_channel

        if self.xively_timer is None:
            self.xively_timer = QTimer(self)
            self.xively_timer.timeout.connect(self.update_xively)
            self.xively_timer.start(self.xively_update_rate*60*1000)

        self.set_active_label(True)
        self.update_xively()

    def write_lcd(self):
        if self.last_upload == None:
            tmp = "Last: Never"
        else:
            tmp = "Last: " + self.last_upload

        self.lcdwidget.write_line(0, 0, "Xively Upload", self)
        self.lcdwidget.write_line(2, 0, tmp, self)

    def update_xively(self):

        if len(self.xively_items) == 0:
            return


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
                return
        except Exception as e:
            self.error_message.showMessage('HTTP error: ' + str(e))
            self.xively_timer.stop()
            self.xively_timer = None
            self.set_active_label(False)
            return

        # set upload time if upload was a success
        self.last_upload = time.strftime("%H:%M:%S")


    def put(self, identifier, value):
        self.write_lcd()
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
        
        



