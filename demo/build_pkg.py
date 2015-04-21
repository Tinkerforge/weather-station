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
#       pythonxy (2.6)
#       py2exe
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
    if os.system(command) != 0:
        sys.exit(1)


def check_output(*args, **kwargs):
    if 'stdout' in kwargs:
        raise ValueError('stdout argument not allowed, it will be overridden')

    process = subprocess.Popen(stdout=subprocess.PIPE, *args, **kwargs)
    output, error = process.communicate()
    exit_code = process.poll()

    if exit_code != 0:
        command = kwargs.get('args')

        if command == None:
            command = args[0]

        raise subprocess.CalledProcessError(exit_code, command, output=output)

    return output


def specialize_template(template_filename, destination_filename, replacements):
    template_file = open(template_filename, 'rb')
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

    destination_file = open(destination_filename, 'wb')
    destination_file.writelines(lines)
    destination_file.close()


def freeze_images():
    directory = UNDERSCORE_NAME
    image_files = []

    for root, dirnames, names in os.walk(directory):
        for name in names:
            full_name = os.path.join(root, name)

            if os.path.isfile(full_name):
                _, ext = os.path.splitext(name)
                ext = ext[1:]

                if ext in ['bmp', 'png', 'jpg']:
                    image_files.append([full_name.replace('\\', '/').replace(directory + '/', ''), ext])

    images = open(os.path.join(directory, 'frozen_images.py'), 'wb')
    images.write('image_data = {\n'.encode('utf-8'))

    for image_file in image_files:
        image_data = base64.b64encode(file(os.path.join(directory, image_file[0]), 'rb').read())
        images.write("'{0}': ['{1}', '{2}'],\n".format(image_file[0], image_file[1], image_data).encode('utf-8'))

    images.write('}\n'.encode('utf-8'))
    images.close()


def build_macosx_pkg():
    print('building demo disk image')
    root_path = os.getcwd()

    print('removing old build directories')
    build_path = os.path.join(root_path, 'build')
    dist_path = os.path.join(root_path, 'dist')

    if os.path.exists(build_path):
        shutil.rmtree(build_path)

    if os.path.exists(dist_path):
        shutil.rmtree(dist_path)

    print('freezing images')
    freeze_images()

    print('calling setup.py py2app build')
    system('python setup.py py2app build')

    print('copying build data')
    build_data_path = os.path.join(root_path, 'build_data', 'macosx', '*')
    resources_path = os.path.join(dist_path, '{0}.app'.format(CAMEL_CASE_NAME), 'Contents', 'Resources')
    system('cp -R {0} "{1}"'.format(build_data_path, resources_path))

    print('patching __boot__.py')
    boot_path = os.path.join(resources_path, '__boot__.py')
    boot_prefix = 'import os\nimport sys\nos.environ["RESOURCEPATH"] = os.path.dirname(os.path.realpath(__file__))\n'

    with open(boot_path, 'rb') as f:
        boot = f.read()

    with open(boot_path, 'wb') as f:
        f.write(boot_prefix + boot)

    print('building disk image')
    dmg_name = '{0}_macos_{1}.dmg'.format(UNDERSCORE_NAME, DEMO_VERSION.replace('.', '_'))

    if os.path.exists(dmg_name):
        os.remove(dmg_name)

    system('hdiutil create -fs HFS+ -volname "{0}-{1}" -srcfolder dist {2}'
           .format(CAMEL_CASE_NAME.replace(' ', '-'), DEMO_VERSION, dmg_name))


# https://github.com/rfk/www.rfk.id.au/blob/master/content/blog/entry/code-signing-py2exe/index.html
def sign_py2exe(exepath):
    # First, sign a *copy* of the file so that we know its final size.
    execopy = os.path.join(os.path.dirname(exepath), 'temp-' + os.path.basename(exepath))
    shutil.copy2(exepath, execopy)
    system('X:\\sign.bat ' + execopy)

    # Figure out the size of the appended signature.
    comment_size = os.stat(execopy).st_size - os.stat(exepath).st_size
    os.unlink(execopy)

    # Write the correct comment size as the last two bytes of the file.
    with open(exepath, "r+") as f:
        f.seek(-2, os.SEEK_END)
        f.write(struct.pack("<H", comment_size))

    # Now we can sign the file for real.
    system('X:\\sign.bat ' + exepath)


def build_windows_pkg():
    print('building demo NSIS installer')
    root_path = os.getcwd()

    print('removing old build directories')
    build_path = os.path.join(root_path, 'build')
    dist_path = os.path.join(root_path, 'dist')

    if os.path.exists(build_path):
        shutil.rmtree(build_path)

    if os.path.exists(dist_path):
        shutil.rmtree(dist_path)

    print('freezing images')
    freeze_images()

    print('calling setup.py py2exe')
    system('python setup.py py2exe')

    # FIXME: doesn't work yet
    #if os.path.exists('X:\\sign.bat'):
    #    sign_py2exe('dist\\{0}.exe'.format(UNDERSCORE_NAME))

    print('creating NSIS script from template')
    nsis_template_path = os.path.join(root_path, 'build_data', 'windows', 'nsis', '{0}_installer.nsi.template'.format(UNDERSCORE_NAME))
    nsis_path = os.path.join(dist_path, 'nsis', '{0}_installer.nsi'.format(UNDERSCORE_NAME))
    specialize_template(nsis_template_path, nsis_path,
                        {'<<DEMO_DOT_VERSION>>': DEMO_VERSION,
                         '<<DEMO_UNDERSCORE_VERSION>>': DEMO_VERSION.replace('.', '_')})

    print('building NSIS installer')
    system('"C:\\Program Files\\NSIS\\makensis.exe" dist\\nsis\\{0}_installer.nsi'.format(UNDERSCORE_NAME))
    installer = '{0}_windows_{1}.exe'.format(UNDERSCORE_NAME, DEMO_VERSION.replace('.', '_'))

    if os.path.exists(installer):
        os.unlink(installer)

    shutil.move(os.path.join(dist_path, 'nsis', installer), root_path)

    if os.path.exists('X:\\sign.bat'):
        system('X:\\sign.bat ' + installer)


def build_linux_pkg():
    print('building demo Debian package')
    root_path = os.getcwd()

    print('removing old build directories')
    dist_path = os.path.join(root_path, 'dist')
    egg_info_path = os.path.join(root_path, '{0}.egg-info'.format(UNDERSCORE_NAME))

    if os.path.exists(dist_path):
        shutil.rmtree(dist_path)

    if os.path.exists(egg_info_path):
        shutil.rmtree(egg_info_path)

    print('calling setup.py sdist')
    system('python setup.py sdist')

    if os.path.exists(egg_info_path):
        shutil.rmtree(egg_info_path)

    print('copying build data')
    build_data_path = os.path.join(root_path, 'build_data', 'linux', UNDERSCORE_NAME)
    linux_path = os.path.join(dist_path, 'linux')
    shutil.copytree(build_data_path, linux_path)

    print('unpacking sdist tar file')
    system('tar -x -C {0} -f {0}/{1}-{2}.tar.gz {1}-{2}/{1}'.format(dist_path, UNDERSCORE_NAME, DEMO_VERSION))

    print('copying unpacked demo source')
    unpacked_path = os.path.join(dist_path, '{0}-{1}'.format(UNDERSCORE_NAME, DEMO_VERSION), UNDERSCORE_NAME)
    linux_share_path = os.path.join(linux_path, 'usr', 'share', UNDERSCORE_NAME)
    shutil.copytree(unpacked_path, linux_share_path)

    print('creating DEBIAN/control from template')
    installed_size = int(check_output(['du', '-s', '--exclude', 'dist/linux/DEBIAN', 'dist/linux']).split('\t')[0])
    control_path = os.path.join(linux_path, 'DEBIAN', 'control')
    specialize_template(control_path, control_path,
                        {'<<VERSION>>': DEMO_VERSION,
                         '<<INSTALLED_SIZE>>': str(installed_size)})

    print('changing directory modes to 0755')
    system('find dist/linux -type d -exec chmod 0755 {} \;')

    print('changing owner to root')
    system('sudo chown -R root:root dist/linux')

    print('building Debian package')
    system('dpkg -b dist/linux {0}-{1}_all.deb'.format(UNDERSCORE_NAME.replace('_', '-'), DEMO_VERSION))

    print('changing owner back to original user')
    system('sudo chown -R `logname`:`logname` dist/linux')

    #print('checking Debian package')
    #system('lintian --pedantic {0}-{1}_all.deb'.format(UNDERSCORE_NAME.replace('_', '-'), DEMO_VERSION))


# run 'python build_pkg.py' to build the windows/linux/macosx package
if __name__ == '__main__':
    if sys.platform != 'win32' and os.geteuid() == 0:
        print('error: must not be started as root, exiting')
        sys.exit(1)

    if sys.platform.startswith('linux'):
        build_linux_pkg()
    elif sys.platform == 'win32':
        build_windows_pkg()
    elif sys.platform == 'darwin':
        build_macosx_pkg()
    else:
        print('error: unsupported platform: ' + sys.platform)
        sys.exit(1)

    print('done')
