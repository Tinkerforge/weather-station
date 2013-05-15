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
from PyQt4.QtGui import QPushButton
from PyQt4.QtCore import Qt
from PyQt4.QtCore import QTimer

import math
import time


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

    latestIlluminance = None
    latestHumidity = None
    latestAirPressure = None
    latestTemperature = None

    illuminanceQueue = []
    humidityQueue = []
    airPressureQueue = []
    temperatureQueue = []

    buttonPressed = 0
    buttonPressedCounter = [0, 0, 0, 0]

    timer = None

    buttons = [None, None, None, None]

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

        self.buttons[0] = QPushButton(self)
        self.buttons[0].setFixedSize(100, 30)
        self.buttons[0].setText("BTN0")
        self.buttons[0].clicked.connect(lambda: self.button_pressed_slot(4))

        self.buttons[1] = QPushButton(self)
        self.buttons[1].setFixedSize(100, 30)
        self.buttons[1].setText("BTN1")
        self.buttons[1].clicked.connect(lambda: self.button_pressed_slot(5))

        self.buttons[2] = QPushButton(self)
        self.buttons[2].setFixedSize(100, 30)
        self.buttons[2].setText("BTN2")
        self.buttons[2].clicked.connect(lambda: self.button_pressed_slot(6))

        self.buttons[3] = QPushButton(self)
        self.buttons[3].setFixedSize(100, 30)
        self.buttons[3].setText("BTN3")
        self.buttons[3].clicked.connect(lambda: self.button_pressed_slot(7))

        self.grid.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self.grid.addWidget(self.lcdwidget,0,0,1,4, Qt.AlignHCenter | Qt.AlignVCenter)
        self.grid.addWidget(self.buttons[0],1,0, Qt.AlignHCenter | Qt.AlignVCenter)
        self.grid.addWidget(self.buttons[1],1,1, Qt.AlignHCenter | Qt.AlignVCenter)
        self.grid.addWidget(self.buttons[2],1,2, Qt.AlignHCenter | Qt.AlignVCenter)
        self.grid.addWidget(self.buttons[3],1,3, Qt.AlignHCenter | Qt.AlignVCenter)
        self.setLayout(self.grid)

        self.qtcb_update_illuminance.connect(self.update_illuminance_data_slot)
        self.qtcb_update_air_pressure.connect(self.update_air_pressure_data_slot)
        self.qtcb_update_temperature.connect(self.update_temperature_data_slot)
        self.qtcb_update_humidity.connect(self.update_humidity_data_slot)
        self.qtcb_button_pressed.connect(self.button_pressed_slot)

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.Update)
        self.timer.start(1000)

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
        self.latestTemperature = temperature/100.0

    def update_temperature(self, temperature):
        self.qtcb_update_temperature.emit(temperature)

    def button_pressed_slot(self, button):
        if button < 4:
            self.buttons[button % 4].animateClick()
        else:

            if button % 4 == self.buttonPressed:
                self.buttonPressedCounter[button % 4] += 1
            else:
                self.buttonPressed = button % 4

            self.lcdwidget.clear(self)
            self.UpdateSwitch()

    def button_pressed(self, button):
        self.qtcb_button_pressed.emit(button)

    def TimeFromSeconds(self, s):
        string = "("
        m = s/60
        h = m/60

        if h > 0:
            string += str(h) + "h)"
        else:
            if m == 0:
                m = 1
            string += str(m) + "m)"

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
            if v > vmax:
                vmax = v
            vavg += v

        vavg = vavg / len(q)

        return [vmin, vmax, vavg]

    def UpdateStandard(self):

        if self.latestIlluminance == None or \
           self.latestHumidity == None or \
           self.latestAirPressure == None or \
           self.latestTemperature == None:
            return

        text = 'Illuminanc %6.2f lx' % (self.latestIlluminance)
        self.lcdwidget.write_line(0, 0, text, self)
        text = 'Humidity   %6.2f %%' % (self.latestHumidity)
        self.lcdwidget.write_line(1, 0, text, self)
        text = 'Air Press %7.2f mb' % (self.latestAirPressure)
        self.lcdwidget.write_line(2, 0, text, self)
        text = 'Temperature %5.2f \xDFC' % (self.latestTemperature)
        self.lcdwidget.write_line(3, 0, text, self)


    def UpdateGraph(self):
        barSumMin = 0.0
        barSumMax = 0.0

        if self.buttonPressedCounter[1] % 4 == self.MODE_ILLUMINANCE:
            [barSumMin, barSumMax] = self.UpdateGraphWriteBars(self.illuminanceQueue)
            self.UpdateGraphWriteTitle("I: ", barSumMin, barSumMax, len(self.illuminanceQueue))

        if self.buttonPressedCounter[1] % 4 == self.MODE_HUMIDITY:
            [barSumMin, barSumMax] = self.UpdateGraphWriteBars(self.humidityQueue)
            self.UpdateGraphWriteTitle("H: ", barSumMin, barSumMax, len(self.humidityQueue))

        if self.buttonPressedCounter[1] % 4 == self.MODE_AIR_PRESSURE:
            [barSumMin, barSumMax] = self.UpdateGraphWriteBars(self.airPressureQueue)
            self.UpdateGraphWriteTitle("A: ", barSumMin, barSumMax, len(self.airPressureQueue))

        if self.buttonPressedCounter[1] % 4 == self.MODE_TEMPERATURE:
            [barSumMin, barSumMax] = self.UpdateGraphWriteBars(self.temperatureQueue)
            self.UpdateGraphWriteTitle("T: ", barSumMin, barSumMax, len(self.temperatureQueue))

    def UpdateGraphWriteTitle(self, string, barSumMin, barSumMax, count):
        line0 = string + str(int(barSumMin)) + " - " + str(int(barSumMax+1.0))
        time = self.TimeFromSeconds(count)

        numSpaces = self.LINE_LENGTH - len(line0) - len(time)

        for i in range(numSpaces):
            line0 += " "

        self.lcdwidget.write_line(0,0, line0 + time, self)

    def UpdateGraphWriteBars(self, q):
        barSumMin = 10000.0
        barSumMax = -10000.0

        count = len(q)

        countBars = count/self.LINE_LENGTH
        if countBars == 0:
            countBars = 1

        barSum = [  0.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 0.0, 0.0 ]
        i = 0
        for v in q:
            barSum[i/countBars] += v
            i += 1
            if i == self.LINE_LENGTH*countBars:
                break

        for i in range(len(barSum)):
            barSum[i] /= countBars
            if barSum[i] < barSumMin:
                barSumMin = barSum[i]
            if barSum[i] > barSumMax:
                barSumMax = barSum[i]

        scale = (self.BAR_HEIGHT-1)/(barSumMax - barSumMin)
        offset = barSumMin * scale - 1

        barHeight = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        for i in range(len(barSum)):
            barHeight[i] = int(round(barSum[i] * scale - offset, 0))

        lines = [[' ' for x in range(20)] for y in range(4)]

        for i in range(len(barHeight)):
            barLenght = barHeight[i]/8
            j = 0
            for j in range(barLenght):
                lines[j+1][i] = chr(self.CUSTOM_CHAR_END)
            if j < 3:
                lines[j+1][i] = chr((barHeight[i]-1)/3 + self.CUSTOM_CHAR_START)

        for line in range(4):
            string = ''
            for s in lines[line]:
                string += s
            self.lcdwidget.write_line(4-line, 0, string, self)

        return [barSumMin, barSumMax]


    def UpdateMinMaxAvg(self):
        if self.buttonPressedCounter[2] % 4 == self.MODE_ILLUMINANCE:
            self.UpdateMinMaxAvgWrite("Illuminance    " + self.TimeFromSeconds(len(self.illuminanceQueue)), "Lux", self.GetMinMaxAvg(self.illuminanceQueue))
        if self.buttonPressedCounter[2] % 4 == self.MODE_HUMIDITY:
            self.UpdateMinMaxAvgWrite("Humidity       " + self.TimeFromSeconds(len(self.humidityQueue)), "%RH", self.GetMinMaxAvg(self.humidityQueue))
        if self.buttonPressedCounter[2] % 4 == self.MODE_AIR_PRESSURE:
            self.UpdateMinMaxAvgWrite("Air Pressure   " + self.TimeFromSeconds(len(self.airPressureQueue)), "Lux", self.GetMinMaxAvg(self.illuminanceQueue))
        if self.buttonPressedCounter[2] % 4 == self.MODE_TEMPERATURE:
            self.UpdateMinMaxAvgWrite("Temperature    " + self.TimeFromSeconds(len(self.temperatureQueue)), "\xDFC", self.GetMinMaxAvg(self.temperatureQueue))

    def UpdateMinMaxAvgWrite(self, title, unit, values):
        vmin = "Min: %6.2f" % values[0] + unit 
        vavg = "Avg: %6.2f" % values[2] + unit 
        vmax = "Max: %6.2f" % values[1] + unit 

        self.lcdwidget.write_line(0, 0, title, self)
        self.lcdwidget.write_line(1, 0, vmin, self)
        self.lcdwidget.write_line(2, 0, vavg, self)
        self.lcdwidget.write_line(3, 0, vmax, self)
        
    
    def UpdateTime(self): 
        line0 = time.strftime("%H:%M:%S")
        line1 = time.strftime("%A")
        line2 = time.strftime("%d. %b %Y")

        self.lcdwidget.write_line(0, (self.LINE_LENGTH-len(line0))/2, line0, self)
        self.lcdwidget.write_line(1, (self.LINE_LENGTH-len(line1))/2, line1, self)
        self.lcdwidget.write_line(2, (self.LINE_LENGTH-len(line2))/2, line2, self)

    def UpdateSwitch(self):
        if self.buttonPressed == self.UPDATE_TYPE_STANDARD:
            self.UpdateStandard()
        if self.buttonPressed == self.UPDATE_TYPE_GRAPH:
            self.UpdateGraph()
        if self.buttonPressed == self.UPDATE_TYPE_MIN_MAX_AVG:
            self.UpdateMinMaxAvg()
        if self.buttonPressed == self.UPDATE_TYPE_TIME:
            self.UpdateTime()

    def Update(self):

        if self.latestIlluminance == None or \
           self.latestHumidity == None or \
           self.latestAirPressure == None or \
           self.latestTemperature == None:
            return

        self.illuminanceQueue.append(self.latestIlluminance)
        if len(self.illuminanceQueue) > 60*60*24:
            self.illuminanceQueue.pop()

        self.humidityQueue.append(self.latestHumidity)
        if len(self.humidityQueue) > 60*60*24:
            self.humidityQueue.pop()
        
        self.airPressureQueue.append(self.latestAirPressure)
        if len(self.airPressureQueue) > 60*60*24:
            self.airPressureQueue.pop()
        
        self.temperatureQueue.append(self.latestTemperature)
        if len(self.temperatureQueue) > 60*60*24:
            self.temperatureQueue.pop()

        self.UpdateSwitch()

