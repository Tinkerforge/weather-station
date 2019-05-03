#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Starter Kit: Weather Station Demo
Copyright (C) 2013-2015 Matthias Bolte <matthias@tinkerforge.com>
Copyright (C) 2011 Olaf Lüke <olaf@tinkerforge.com>
Copyright (C) 2011 Bastian Nordmeyer <bastian@tinkerforge.com>

build_pkg.py: Package builder for Starter Kit: Weather Station Demo

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

# Windows:
#   dependencies:
#       python
#       pyinstaller
#       PyQt5
#       nsis
#       universal CRT

import sys
if (sys.hexversion & 0xFF000000) != 0x03000000:
    print('Python 3.x required')
    sys.exit(1)

import os
import shutil
import subprocess
from starter_kit_weather_station_demo.config import DEMO_VERSION

from build_pkg_utils import *

UNDERSCORE_NAME = 'starter_kit_weather_station_demo'
CAMEL_CASE_NAME = 'Starter Kit Weather Station Demo'

def build_linux_pkg():
    print('building demo Debian package')
    utils = BuildPkgUtils(UNDERSCORE_NAME, 'linux', DEMO_VERSION, '--internal' in sys.argv)

    utils.run_sdist()
    utils.copy_build_data()
    utils.unpack_sdist()

    utils.build_debian_pkg()

def build_pyinstaller_pkg():
    platform_dict = {'win32': 'windows', 'darwin': 'macos'}

    utils = BuildPkgUtils(UNDERSCORE_NAME, platform_dict[sys.platform], DEMO_VERSION, '--internal' in sys.argv)
    utils.exit_if_not_venv()

    utils.build_pyinstaller_pkg()

    utils.copy_build_artefact()

def main():
    if sys.platform != 'win32' and os.geteuid() == 0:
        print('error: must not be started as root, exiting')
        return 1

    if sys.platform.startswith('linux'):
        build_linux_pkg()
        return 0

    if sys.platform == 'win32' or sys.platform == 'darwin':
        build_pyinstaller_pkg()
        return 0

    print('error: unsupported platform: ' + sys.platform)
    return 1

# run 'python build_pkg.py' to build the windows/linux/macos package
if __name__ == '__main__':
    exit_code = main()
    if exit_code == 0:
        print('done')
    sys.exit(exit_code)
