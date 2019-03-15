# -*- coding: utf-8 -*-
"""
Starter Kit: Weather Station Demo
Copyright (C) 2013 Olaf Lüke <olaf@tinkerforge.com>
Copyright (C) 2015 Matthias Bolte <matthias@tinkerforge.com>

setup.py: Setuptools script for Starter Kit: Weather Station Demo

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
import glob
from setuptools import setup, find_packages

from starter_kit_weather_station_demo.config import DEMO_VERSION

UNDERSCORE_NAME = 'starter_kit_weather_station_demo'
CAMEL_CASE_NAME = 'Starter Kit Weather Station Demo'
DESCRIPTION = 'Demo for Starter Kit: Weather Station'

packages = find_packages(include=[UNDERSCORE_NAME, '{0}.*'.format(UNDERSCORE_NAME)])

package_data = {}
image_patterns = ['*.bmp', '*.png', '*.jpg']

for package in packages:
    package_path = os.path.join(*package.split('.'))
    images = []

    for pattern in image_patterns:
        images += glob.glob(os.path.join(package_path, pattern))

    if len(images) > 0:
        package_data[package] = [os.path.basename(image) for image in images]

setup_arguments = {
    'name':         UNDERSCORE_NAME,
    'version':      DEMO_VERSION,
    'author':       'Tinkerforge',
    'author_email': 'info@tinkerforge.com',
    'url':          'http://www.tinkerforge.com',
    'license':      'GPL v2',
    'description':  DESCRIPTION,
    'packages':     packages,
    'package_data': package_data,
    'scripts':      ['{0}/{0}'.format(UNDERSCORE_NAME)]
}

setup(**setup_arguments)
