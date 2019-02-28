#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Starter Kit: Weather Station Demo
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

import math

from PyQt5.QtCore import pyqtSignal, Qt
from PyQt5.QtWidgets import QVBoxLayout, QHBoxLayout, QWidget, QLabel, QPushButton

from starter_kit_weather_station_demo.LCDWidget import LCDWidget

class ProjectEnvDisplay(QWidget):
    qtcb_update_illuminance = pyqtSignal(float)
    qtcb_update_air_pressure = pyqtSignal(float)
    qtcb_update_temperature = pyqtSignal(float)
    qtcb_update_humidity = pyqtSignal(float)
    qtcb_button_pressed = pyqtSignal(int)

    lcdwidget = None

    def __init__(self, parent, app):
        super(QWidget, self).__init__()

        layout1 = QHBoxLayout()
        layout2 = QVBoxLayout()

        layout1.addStretch()
        layout1.addLayout(layout2)
        layout1.addStretch()

        label = QLabel(self)
        label.setText("Simple Project: <b>Display Environment Measurements on LCD</b>. Simply displays all measured values on LCD. Sources in all programming languages can be found <a href=\"http://www.tinkerforge.com/en/doc/Kits/WeatherStation/WeatherStation.html#display-environment-measurements-on-lcd\">here</a>.<br>")
        label.setTextFormat(Qt.RichText)
        label.setTextInteractionFlags(Qt.TextBrowserInteraction)
        label.setOpenExternalLinks(True)
        label.setWordWrap(True)
        label.setAlignment(Qt.AlignJustify)

        layout2.addSpacing(10)
        layout2.addWidget(label)
        layout2.addSpacing(10)

        self.lcdwidget = LCDWidget(self, app)

        layout2.addWidget(self.lcdwidget)
        layout2.addStretch()

        self.setLayout(layout1)

        self.qtcb_update_illuminance.connect(self.update_illuminance_data_slot)
        self.qtcb_update_air_pressure.connect(self.update_air_pressure_data_slot)
        self.qtcb_update_temperature.connect(self.update_temperature_data_slot)
        self.qtcb_update_humidity.connect(self.update_humidity_data_slot)
        self.qtcb_button_pressed.connect(self.button_pressed_slot)

    def update_illuminance_data_slot(self, illuminance):
        text = 'Illuminance{}lx'

        if illuminance >= 10000:
            fmt = ' {0:5.0f} '
        else:
            spaces = ' ' * (4 - math.floor(math.log10(illuminance)))
            fmt = spaces + '{0:4.1f}'
        text = text.format(fmt).format(illuminance)
        self.lcdwidget.write_line(0, 0, text, self)

    def update_illuminance(self, illuminance):
        self.qtcb_update_illuminance.emit(illuminance)

    def update_humidity_data_slot(self, humidity):
        text = 'Humidity    %6.1f %%' % humidity
        self.lcdwidget.write_line(1, 0, text, self)

    def update_humidity(self, humidity):
        self.qtcb_update_humidity.emit(humidity)

    def update_air_pressure_data_slot(self, air_pressure):
        text = 'AirPressure%7.1fmb' % air_pressure
        self.lcdwidget.write_line(2, 0, text, self)

    def update_air_pressure(self, air_pressure):
        self.qtcb_update_air_pressure.emit(air_pressure)

    def update_temperature_data_slot(self, temperature):
        # \xDF == ° on LCD 20x4 charset
        text = 'Temperature  %5.1f\xDFC' % temperature
        self.lcdwidget.write_line(3, 0, text, self)

    def update_temperature(self, temperature):
        self.qtcb_update_temperature.emit(temperature)

    def button_pressed_slot(self, button):
        pass

    def button_pressed(self, button):
        self.qtcb_button_pressed.emit(button)
