#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Starter Kit: Weather Station Demo Application(Brick Viewer) 
Copyright (C) 2013 Bastian Nordmeyer <bastian@tinkerforge.com>

Project_Statistics.py: Statistics Display Project Implementation

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

import math


from LCDWidget import LCDWidget

class ProjectStatistics(QWidget):

    qtcb_update_illuminance = pyqtSignal(float)
    qtcb_update_air_pressure = pyqtSignal(float)
    qtcb_update_temperature = pyqtSignal(float)
    qtcb_update_humidity = pyqtSignal(float)

    lcdwidget = None

    def __init__(self, parent, app):
        super(QWidget, self).__init__()

        self.grid = QGridLayout()
        self.lcdwidget = LCDWidget(self, app)
        self.grid.addWidget(self.lcdwidget)
        self.setLayout(self.grid)

        self.qtcb_update_illuminance.connect(self.update_illuminance_data_slot)
        self.qtcb_update_air_pressure.connect(self.update_air_pressure_data_slot)
        self.qtcb_update_temperature.connect(self.update_temperature_data_slot)
        self.qtcb_update_humidity.connect(self.update_humidity_data_slot)

    def update_illuminance_data_slot(self, illuminance):

        text = 'Illuminanc %6.2f lx' % (illuminance/10.0)
        self.lcdwidget.write_line(0, 0, text)

    def update_illuminance(self, illuminance):
        self.qtcb_update_illuminance.emit(illuminance)

    def update_humidity_data_slot(self, humidity):
        text = 'Humidity   %6.2f %%' % (humidity/10.0)
        self.lcdwidget.write_line(1, 0, text)
    
    def update_humidity(self, humidity):
        self.qtcb_update_humidity.emit(humidity)

    def update_air_pressure_data_slot(self, air_pressure):

        text = 'Air Press %7.2f mb' % (air_pressure/1000.0)
        self.lcdwidget.write_line(2, 0, text)
    
    def update_air_pressure(self, air_pressure):
        self.qtcb_update_air_pressure.emit(air_pressure)

    def update_temperature_data_slot(self, temperature):

        # \xDF == ° on LCD 20x4 charset
        text = 'Temperature %5.2f \xDFC' % (temperature/100.0)

        self.lcdwidget.write_line(3, 0, text)

    def update_temperature(self, temperature):
        self.qtcb_update_temperature.emit(temperature)

