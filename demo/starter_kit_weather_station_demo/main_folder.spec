# -*- mode: python -*-

import os
import sys
sys.path.append('..')
from starter_kit_weather_station_demo.pyinstaller_utils import *

a = Analysis(['main.py'], pathex=pathex, excludes=excludes, hiddenimports=hiddenimports)

patterns = ['qt5qml', 'qt5quick', 'libglesv2', 'libcrypto', 'qt5network', 'qt5dbus',
            'qt5svg', 'qt5websockets', 'd3dcompiler', 'libegl', 'opengl32sw', 'qwebp',
            'qjpeg', 'qwebgl', 'qminimal', 'qoffscreen', 'qwebgl']
binaries = strip_binaries(a.binaries, patterns)

pyz = PYZ(a.pure, a.zipped_data)

exe = EXE(pyz,
          a.scripts,
          [],
          exclude_binaries=True,
          name=UNDERSCORE_NAME + ('.exe' if windows else ''),
          debug=False,
          bootloader_ignore_signals=False,
          strip=False,
          upx=False,
          console=False,
          icon=icon)

coll = COLLECT(exe,
               binaries,
               a.zipfiles,
               a.datas + datas,
               strip=False,
               upx=False,
               name='')
if macos:
    app = BUNDLE(coll,
                    name=UNDERSCORE_NAME + '.app',
                    icon=icon)

post_generate()
