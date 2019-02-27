#!/usr/bin/env python
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
#       python (3.2)
#       pyinstaller
#       PyQt5
#       pyserial
#       nsis
#       win redistributables vcredist under winxp

import os
import sys
import base64
import shutil
import struct
import subprocess
from starter_kit_weather_station_demo.config import DEMO_VERSION


UNDERSCORE_NAME = 'starter_kit_weather_station_demo'
CAMEL_CASE_NAME = 'Starter Kit Weather Station Demo'

def system(command):
    if subprocess.call(command) != 0:
        sys.exit(1)


def specialize_template(template_filename, destination_filename, replacements):
    template_file = open(template_filename, 'r')
    lines = []
    replaced = set()

    for line in template_file.readlines():
        for key in replacements:
            replaced_line = line.replace(key, replacements[key])

            if replaced_line != line:
                replaced.add(key)

            line = replaced_line

        lines.append(line)

    template_file.close()

    if replaced != set(replacements.keys()):
        raise Exception('Not all replacements for {0} have been applied'.format(template_filename))

    try:
        os.makedirs(os.path.dirname(destination_filename))
    except:
        pass
    destination_file = open(destination_filename, 'w+')
    destination_file.writelines(lines)
    destination_file.close()




def build_linux_pkg():
    print('building demo Debian package')
    root_path = os.getcwd()

    print('removing old build directories')
    dist_path = os.path.join(root_path, 'dist')
    egg_info_path = os.path.join(root_path, '{0}.egg-info'.format(UNDERSCORE_NAME))

    if os.path.exists(dist_path):
        shutil.rmtree(dist_path)

    print('calling setup.py sdist')
    system(['python3', 'setup.py', 'sdist'])

    if os.path.exists(egg_info_path):
        shutil.rmtree(egg_info_path)

    print('copying build data')
    build_data_path = os.path.join(root_path, 'build_data', 'linux', UNDERSCORE_NAME)
    linux_path = os.path.join(dist_path, 'linux')
    shutil.copytree(build_data_path, linux_path)

    print('unpacking sdist tar file')
    system(['tar', '-x', '-C', dist_path, '-f', '{0}/{1}-{2}.tar.gz'.format(dist_path, UNDERSCORE_NAME, DEMO_VERSION), '{0}-{1}/{0}'.format(UNDERSCORE_NAME, DEMO_VERSION)])

    print('copying unpacked demo source')
    unpacked_path = os.path.join(dist_path, '{0}-{1}'.format(UNDERSCORE_NAME, DEMO_VERSION), UNDERSCORE_NAME)
    linux_share_path = os.path.join(linux_path, 'usr', 'share', UNDERSCORE_NAME)
    shutil.copytree(unpacked_path, linux_share_path)

    print('creating DEBIAN/control from template')
    installed_size = int(subprocess.check_output(['du', '-s', '--exclude', 'dist/linux/DEBIAN', 'dist/linux']).split(b'\t')[0])
    control_path = os.path.join(linux_path, 'DEBIAN', 'control')
    specialize_template(control_path, control_path,
                        {'<<VERSION>>': DEMO_VERSION,
                         '<<INSTALLED_SIZE>>': str(installed_size)})

    print('changing directory modes to 0755')
    system(['find', 'dist/linux', '-type', 'd', '-exec', 'chmod', '0755', '{}', ';'])

    print('changing file modes')
    system(['find', 'dist/linux', '-type', 'f', '-perm', '664', '-exec', 'chmod', '0644', '{}', ';'])
    system(['find', 'dist/linux', '-type', 'f', '-perm', '775', '-exec', 'chmod', '0755', '{}', ';'])

    print('changing owner to root')
    stat = os.stat('dist/linux')
    user, group = stat.st_uid, stat.st_gid
    system(['sudo', 'chown', '-R', 'root:root', 'dist/linux'])

    print('building Debian package')
    system(['dpkg', '-b', 'dist/linux', '{0}-{1}_all.deb'.format(UNDERSCORE_NAME, DEMO_VERSION)])

    print('changing owner back to original user')
    system(['sudo', 'chown', '-R', '{}:{}'.format(user, group), 'dist/linux'])

    if os.path.exists('/usr/bin/lintian'):
        print('checking Debian package')
        system(['lintian', '--pedantic', '{0}-{1}_all.deb'.format(UNDERSCORE_NAME, DEMO_VERSION)])
    else:
        print('skipping lintian check')



# run 'python build_pkg.py' to build the windows/linux/macos package
if __name__ == '__main__':
    if sys.platform != 'win32' and os.geteuid() == 0:
        print('error: must not be started as root, exiting')
        sys.exit(1)

    if sys.platform.startswith('linux'):
        build_linux_pkg()
    elif sys.platform == 'win32' or sys.platform == 'darwin':
        in_virtualenv = hasattr(sys, 'real_prefix')
        in_pyvenv = hasattr(sys, 'base_prefix') and sys.base_prefix != sys.prefix

        if not in_virtualenv and not in_pyvenv:
            print('error: Please build Windows or macOS binaries in the correct virtualenv.')
            sys.exit(1)

        root_path = os.getcwd()
        os.chdir(os.path.join(root_path, UNDERSCORE_NAME))
        system(['pyinstaller', 'main_folder.spec'])
        os.chdir(root_path)
    else:
        print('error: unsupported platform: ' + sys.platform)
        sys.exit(1)

    print('done')
