#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Starter Kit: Weather Station Demo Application
Copyright (C) 2013 Bastian Nordmeyer <bastian@tinkerforge.com>

LCDWidget.py: LCD Display Widget Implementation which controls
also physical LCD20x4 Bricklet

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

from PyQt4.QtGui import QLabel
from PyQt4.QtGui import QPixmap
from PyQt4.QtGui import QPainter
from PyQt4.QtGui import QFont
from PyQt4.QtCore import Qt
from PyQt4.QtCore import QString
from PyQt4.QtCore import pyqtSignal, SIGNAL, SLOT
from PyQt4.QtGui import QGridLayout
from PyQt4.QtGui import QWidget
from tinkerforge.bricklet_lcd_20x4 import LCD20x4


import math


class LCDChar (QLabel):

    qtcb_set_char = pyqtSignal(str)
    CUSTOM_CHAR_START = 8
    CUSTOM_CHAR_END = 15

    def __init__(self, parent):
        super(QLabel, self).__init__(parent)
        
        font = QFont()
        font.setPixelSize(self.height())
        self.setFont(font)
        self.qtcb_set_char.connect(self.set_char_slot)

        self.setText(" ")
        self.setFixedSize(23, 45)


        self.setAutoFillBackground(True)
        palette = self.palette()
        palette.setColor(self.backgroundRole(), Qt.blue)
        palette.setColor(self.foregroundRole(), Qt.white)
        self.setPalette(palette)

    def set_char_slot(self, char):

        if char <= chr(self.CUSTOM_CHAR_END):
            c = ord(str(char)) - self.CUSTOM_CHAR_START + 1

            width = self.width()
            height = self.height()
        
            pixmap = QPixmap(width,height)
            painter = QPainter(pixmap)

            painter.fillRect(0, 0, width, height, Qt.blue)
            painter.fillRect(0, int(height*((8-c)/8.0)), width, height, Qt.white)

            painter.end() 

            self.setPixmap(pixmap)
            
        else:
            if char == '\xDF':
                char = '\xB0'

            self.setText(char)
            self.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)

    def set_char(self, char):
        self.qtcb_set_char.emit(char)


class LCDWidget (QWidget):

    qtcb_write_line = pyqtSignal(int, int, str)

    def __init__(self, parent, app):
        super(QWidget, self).__init__(parent)
        self.array = [[None for x in range(20)] for y in range(4)]
        self.grid = None

        self.app = app

        self.setFixedSize(550, 204)

        self.setAutoFillBackground(True)
        palette = self.palette()
        palette.setColor(self.backgroundRole(), Qt.black)
        palette.setColor(self.foregroundRole(), Qt.white)
        self.setPalette(palette)


        self.qtcb_write_line.connect(self.write_line_slot)

        self.grid = QGridLayout()
        self.grid.setSpacing(4)

        for y in range(len(self.array)):
            for x in range(len(self.array[0])):
                character = LCDChar(self)
                self.array[y][x] = character
                self.grid.addWidget(character,y,x)

        self.setLayout(self.grid)


    def write_line_slot(self, line, begin, text):

        if self.app.lcd is not None:
            self.app.lcd.write_line(line, begin, str(text.toAscii().data()))

        for i in range(begin, len(self.array[0])):
            try:
                self.array[line][i].set_char(text[i-begin])
            except Exception:
                break

    def write_line(self, line, begin, text, proj):
        if proj == self.app.active_project:
            self.qtcb_write_line.emit(line, begin, text)

    def clear(self, proj):
        if proj == self.app.active_project:
            self.write_line_slot(0,0, QString("                    "))
            self.write_line_slot(1,0, QString("                    "))
            self.write_line_slot(2,0, QString("                    "))
            self.write_line_slot(3,0, QString("                    "))
