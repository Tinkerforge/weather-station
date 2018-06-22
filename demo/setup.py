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

import os
import sys
import glob
from setuptools import setup, find_packages
from starter_kit_weather_station_demo.config import DEMO_VERSION

UNDERSCORE_NAME = 'starter_kit_weather_station_demo'
CAMEL_CASE_NAME = 'Starter Kit Weather Station Demo'
DESCRIPTION = 'Demo for Starter Kit: Weather Station'

# Find demo packages
if sys.platform == 'darwin':
    packages = find_packages() # FIXME: setuptools on our macbook doesn't understand 'include'
else:
    packages = find_packages(include=[UNDERSCORE_NAME, '{0}.*'.format(UNDERSCORE_NAME)])

# Collect non-frozen package_data
package_data = {}

if sys.platform.startswith('linux'):
    image_patterns = ['*.bmp', '*.png', '*.jpg']

    for package in packages:
        package_path = os.path.join(*package.split('.'))
        images = []

        for pattern in image_patterns:
            images += glob.glob(os.path.join(package_path, pattern))

        if len(images) > 0:
            package_data[package] = [os.path.basename(d) for d in images]

    package_data[UNDERSCORE_NAME].append('{0}.desktop'.format(UNDERSCORE_NAME))

# Collect platform specific data_files
def collect_data_files(path, excludes=None):
    path = os.path.normcase(path)
    files = []

    for root, dirnames, names in os.walk(path):
        for name in names:
            if excludes != None and name in excludes:
                continue

            full_name = os.path.join(root, name)

            if os.path.isfile(full_name):
                files.append((os.path.join(root.replace(path, '')), [full_name]))

    return files

data_files = []

if sys.platform.startswith('linux'):
    data_files.append(('/usr/share/pixmaps/', ['{0}/{0}-icon.png'.format(UNDERSCORE_NAME)]))
    data_files.append(('/usr/share/applications/', ['{0}/{0}.desktop'.format(UNDERSCORE_NAME)]))
elif sys.platform == 'win32':
    data_files += collect_data_files('build_data/windows/', ['{0}-icon.ico'.format(UNDERSCORE_NAME)])
elif sys.platform == 'darwin':
    data_files += collect_data_files('build_data/macos/')

# Run setup
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
    'data_files':   data_files
}

if sys.platform.startswith('linux'):
    setup_arguments['scripts'] = ['{0}/{0}'.format(UNDERSCORE_NAME)]
elif sys.platform == 'win32':
    import py2exe

    options = {
        'py2exe' : {
            'dll_excludes': ['MSVCP90.dll'],
            'includes':     ['sip',
                             'PyQt4.QtCore',
                             'PyQt4.QtGui'],
            'excludes':     ['PyQt4.QtOpenGL',
                             'PyQt4.QtSvg',
                             'PyQt4.Qwt5',
                             'OpenGL.GL',
                             'config_linux',
                             'config_macos',
                             '_gtkagg',
                             '_tkagg',
                             'Tkconstants',
                             'Tkinter',
                             'tcl',
                             'pydoc',
                             'email',
                             'nose',
                             'pdb',
                             'inspect',
                             'doctest',
                             'difflib',
                             'numpy.numarray',
                             'numpy.oldnumeric',
                             'numpy.core._dotblas',
                             'numpy.random',
                             'ctypes.macholib',
                             'win32pdh',
                             'win32ui']
        }
    }

    windows = [{
        'script':        '{0}/main.py'.format(UNDERSCORE_NAME),
        'dest_base':     UNDERSCORE_NAME,
        'icon_resources': [(0, os.path.normcase('build_data/windows/{0}-icon.ico'.format(UNDERSCORE_NAME)))]
    }]

    setup_arguments['options'] = options
    setup_arguments['windows'] = windows
    setup_arguments['zipfile'] = None
elif sys.platform == 'darwin':
    options = {
        'py2app': {
            'argv_emulation': True,
            'iconfile':       'build_data/macos/{0}-icon.icns'.format(UNDERSCORE_NAME),
            'site_packages':  True,
            'includes':       ['atexit',
                               'sip',
                               'PyQt4.QtCore',
                               'PyQt4.QtGui'],
            'excludes':       ['scipy',
                               'numpy',
                               'distutils',
                               'setuptools',
                               'email',
                               'matplotlib',
                               'OpenGL',
                               'PyQt4.QtOpenGL',
                               'PyQt4.QtSvg',
                               'PyQt4.Qwt5',
                               'PyQt4.QtDeclarative',
                               'PyQt4.QtDesigner',
                               'PyQt4.QtHelp',
                               'PyQt4.QtMultimedia',
                               'PyQt4.QtNetwork',
                               'PyQt4.QtScript',
                               'PyQt4.QtScriptTools',
                               'PyQt4.QtSql',
                               'PyQt4.QtTest',
                               'PyQt4.QtWebKit',
                               'PyQt4.QtXml',
                               'PyQt4.QtXmlPatterns']
        }
    }

    app = [{
        'script': '{0}/main.py'.format(UNDERSCORE_NAME),
        'plist': {
            'CFBundleName':               CAMEL_CASE_NAME,
            'CFBundleVersion':            DEMO_VERSION,
            'CFBundleShortVersionString': DEMO_VERSION,
            'CFBundleGetInfoString':      DESCRIPTION,
            'CFBundleExecutable':         UNDERSCORE_NAME,
            'CFBundleIdentifier':         'com.tinkerforge.{0}'.format(UNDERSCORE_NAME),
            'CFBundleIconFile':           '{0}-icon.icns'.format(UNDERSCORE_NAME),
            'NSHumanReadableCopyright':   'Tinkerforge GmbH 2013-2015'
        }
    }]

    setup_arguments['options']   = options
    setup_arguments['scripts']   = ['{0}/main.py'.format(UNDERSCORE_NAME)]
    setup_arguments['app']       = app

setup(**setup_arguments)
