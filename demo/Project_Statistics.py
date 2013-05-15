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

    UPDATE_TYPE_STANDARD = 0
    UPDATE_TYPE_GRAPH = 1
    UPDATE_TYPE_MIN_MAX_AVG = 2
    UPDATE_TYPE_TIME = 3

    LINE_LENGTH = 20
    CUSTOM_CHAR_START = 8
    CUSTOM_CHAR_END = 15
    BAR_HEIGHT = 24

    MODE_ILLUMINANCE = 0
    MODE_HUMIDITY = 1
    MODE_AIR_PRESSURE = 2
    MODE_TEMPERATURE = 3

    latestIlluminance = 0.0
    latestHumidity = 0.0
    latestAirPressure = 0.0
    latestTemperature = 0.0

    illuminanceQueue = []
    humidityQueue = []
    airPressureQueue = []
    temperatureQueue = []

    buttonPressed = 0
    buttonPressedCounter = [0, 0, 0, 0]



    qtcb_update_illuminance = pyqtSignal(float)
    qtcb_update_air_pressure = pyqtSignal(float)
    qtcb_update_temperature = pyqtSignal(float)
    qtcb_update_humidity = pyqtSignal(float)
    qtcb_button_pressed = pyqtSignal(int)

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
        self.latestIlluminance = illuminance/10.0

    def update_illuminance(self, illuminance):
        self.qtcb_update_illuminance.emit(illuminance)

    def update_humidity_data_slot(self, humidity):
        self.latestHumidity = humidity/10.0
    
    def update_humidity(self, humidity):
        self.qtcb_update_humidity.emit(humidity)

    def update_air_pressure_data_slot(self, air_pressure):
        self.latestAirPressure = air_pressure/1000.0
    
    def update_air_pressure(self, air_pressure):
        self.qtcb_update_air_pressure.emit(air_pressure)

    def update_temperature_data_slot(self, temperature):
        self.latesttemperature = temperature/100.0

    def update_temperature(self, temperature):
        self.qtcb_update_temperature.emit(temperature)

    def button_pressed_slot(self, button):
        if button == self.buttonPressed:
            self.buttonPressedCounter[button]++
        else:
            self.buttonPressed = button

        self.lcdWidget.clear()
        UpdateSwitch()

    def button_pressed(self, button)
        self.qtcb_button_pressed.emit(button)

    def TimeFromSeconds(self, s)
        string = "("
        m = s/60
        h = m/60

        if h > 0:
            string += h + "h"
        else:
            if m == 0:
                m = 1
            string += m + "m"

        if len(string) == 4:
            return " " + string

        return string

    def GetMinMaxAvg(self, q):
        if len(q) == 0:
            return [0.0, 0.0, 0.0]

        vmin = 10000.0
        vmax = -10000.0
        vavg = 0.0

        for v in q:
            if v < vmin:
                vmin = v
            if v > vmax
                vmax = v
            vavg += v

        vavg / len(q)

        return [vmin, vmax, vavg]

    def UpdateStandard(self):
        text = 'Illuminanc %6.2f lx' % (illuminance/10.0)
        self.lcdwidget.write_line(0, 0, text)
        text = 'Humidity   %6.2f %%' % (humidity/10.0)
        self.lcdwidget.write_line(1, 0, text)
        text = 'Air Press %7.2f mb' % (air_pressure/1000.0)
        self.lcdwidget.write_line(2, 0, text)
        text = 'Temperature %5.2f \xDFC' % (temperature/100.0)
        self.lcdwidget.write_line(3, 0, text)


    def UpdateGraph(self):
        barSumMin = 0.0
        barSumMax = 0.0

        if buttonPressedCounter[1] % 4 == MODE_ILLUMINANCE:
            [barSumMin, barSumMax] = self.UpdateGraphWriteBars(illuminanceQueue)
            self.self.UpdateGraphWriteTitle("I: ", barSumMin, barSumMax, len(illuminanceQueue)
        if buttonPressedCounter[1] % 4 == MODE_HUMIDITY:
            [barSumMin, barSumMax] = self.UpdateGraphWriteBars(humidityQueue)
            self.UpdateGraphWriteTitle("I: ", barSumMin, barSumMax, len(humidityQueue)
        if buttonPressedCounter[1] % 4 == MODE_AIR_PRESSURE:
            [barSumMin, barSumMax] = self.UpdateGraphWriteBars(airPressureQueue)
            self.UpdateGraphWriteTitle("I: ", barSumMin, barSumMax, len(airPressureQueue)
        if buttonPressedCounter[1] % 4 == MODE_TEMPERATURE:
            [barSumMin, barSumMax] = self.UpdateGraphWriteBars(temperatureQueue)
            self.UpdateGraphWriteTitle("I: ", barSumMin, barSumMax, len(temperatureQueue)

    def UpdateGraphWriteTitle(self, string, barSumMin, barSumMax, count):
        line0 = string + str(int(barSumMin)) + " - " + str(int(barSumMax+1.0))
        time = TimeFromSeconds(count)

        numSpaces = LINE_LENGTH - len(line0) - len(time)

        for i in range(numSpaces):
            line0 += " "

        self.lcdwidget.write_line(0,0, line0 + time)

    def UpdateGraphWriteBars(self, q):
        barSumMin = 10000.0
        barSumMax = -10000.0

        count = len(q)

        countBars = count/LINE_LENGTH
        if countBars == 0:
            countBards = 1

        barSum = [  0.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 0.0, 0.0 ]
        i = 0
        for v in q:
            barSum[i/countBars] += v
            i += 1
            if i == LINE_LENGTH*countBars
                break

        for i in range(len(barSum)):
            barSum[i] /= countBars
            if barSum[i] < barSumMin:
                barSumMin = barSum[i]
            if barSum[i] > barSumMax:
                barSumMax = barSum[i]

        scale = (BAR_HEIGHT-1)/(barSumMax - barSumMin)
        offset = barSumMin * scale - 1

        barHeight = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        for i in range(len(barSum)):
            barHeight[i] = int(math.round(barSum[i] * scale - offset, 0)

        lines = [[' ' for x in range(20)] for y in range(4)]

        for i in range(len(barHeight)):
            barLenght = barHeight[i]/8
            j = 0
            for j in range(barLenght):
                lines[j+q][i] = chr(CUSTOM_CHAR_END)
            if j < 3:
                lines[j+1][i] = chr((barHeight[i]-1)/3 + CUSTOM_CHAR_START)

        for line in range(4):
            string = ''
            for s in lines[line]:
                string += s
            self.lcdwidget.write_line(4-line, 0, string)

