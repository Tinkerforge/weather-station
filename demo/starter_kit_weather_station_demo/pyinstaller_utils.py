import os
import shutil
import sys
import subprocess
import traceback
from starter_kit_weather_station_demo.config import DEMO_VERSION

def get_path_rel_to_root(path, root):
    return path.replace('\\', '/').replace(root.replace('\\', '/') + '/', '')

def collect_data(pred, root):
    print("Collecting data")
    result = []
    for dirpath, _directories, files in os.walk(root):
        for file in files:
            full_name = os.path.join(dirpath, file)

            if pred(full_name):
                path_rel_to_root = get_path_rel_to_root(full_name, root)
                result.append((path_rel_to_root, path_rel_to_root, 'DATA'))
    return result

def by_ext(exts):
    def fn(full_name):
        if os.path.isfile(full_name):
            _, ext = os.path.splitext(full_name)
            ext = ext[1:]

            return ext in exts
    return fn

def by_name(name):
    return lambda full_name: os.path.basename(full_name) == name

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

def system(command, stdout=None):
    if subprocess.call(command, stdout=stdout) != 0:
        sys.exit(1)

def win_sign(exe_path):
    system([
        "C:\\Program Files (x86)\\Windows Kits\\10\\bin\\x86\\signtool.exe", "sign",
        "/v",
        "/tr", "http://rfc3161timestamp.globalsign.com/advanced",
        "/td", "sha256",
        "/n", "Tinkerforge GmbH",
        exe_path])
    system(["C:\\Program Files (x86)\\Windows Kits\\10\\bin\\x86\\signtool.exe", "verify", "/v", "/pa", exe_path])


def win_build_installer():
    print("Hier")
    nsis_template_path = os.path.join(windows_build_data_path, 'nsis', UNDERSCORE_NAME + '_installer.nsi.template')
    nsis_path = os.path.join(dist_path, 'nsis', UNDERSCORE_NAME + '.nsi')
    specialize_template(nsis_template_path, nsis_path,
                        {'<<DEMO_DOT_VERSION>>': DEMO_VERSION,
                         '<<DEMO_UNDERSCORE_VERSION>>': DEMO_VERSION.replace('.', '_')})
    system('"C:\\Program Files (x86)\\NSIS\\makensis.exe" dist\\nsis\\{}.nsi'.format(UNDERSCORE_NAME))
    installer = '{}_windows_{}.exe'.format(UNDERSCORE_NAME, DEMO_VERSION.replace('.', '_'))

    if os.path.exists(installer):
        os.unlink(installer)

    shutil.move(os.path.join(dist_path, 'nsis', installer), root_path)
    return os.path.join(root_path, installer)


UNDERSCORE_NAME = 'starter_kit_weather_station_demo'
CAMEL_CASE_NAME = 'Starter Kit Weather Station Demo'

root_path = os.getcwd()
build_path = os.path.join(root_path, 'build')
dist_path = os.path.join(root_path, 'dist')
linux_build_data_path =   os.path.normpath(os.path.join(root_path, '..', 'build_data', 'linux'))
mac_build_data_path =     os.path.normpath(os.path.join(root_path, '..', 'build_data', 'macos'))
windows_build_data_path = os.path.normpath(os.path.join(root_path, '..', 'build_data', 'windows'))
windows = sys.platform == 'win32'
macos = sys.platform == 'darwin'
linux = not windows and not macos

print('removing old dist directory')
if os.path.exists(dist_path):
    shutil.rmtree(dist_path)

datas = []
datas += collect_data(by_ext(['bmp', 'jpg', 'png', 'svg']), root_path)
excludes = ['wx', 'gtk+', '_gtkagg', 'gtk', 'gdk', 'gtk2', 'gtk3', 'cairo', 'wayland', 'xinerama', 'share', 'icons', 'atk', 'pango', 'pil', 'PIL',
            '_tkagg',
            'Tkconstants',
            'Tkinter',
            'tcl',
            'pydoc',
            'email',
            'nose',
            #'inspect',
            'ctypes.macholib',
            'win32pdh',
            'win32ui',
            'PyOpenGL',
            'OpenGL',
            'PyQt5.Enginio',
            'PyQt5.QAxContainer',
            'PyQt5.Qt3DAnimation',
            'PyQt5.Qt3DCore',
            'PyQt5.Qt3DExtras',
            'PyQt5.Qt3DInput',
            'PyQt5.Qt3DLogic',
            'PyQt5.Qt3DRender',
            'PyQt5.QtAndroidExtras',
            'PyQt5.QtBluetooth',
            'PyQt5.QtChart',
            'PyQt5.QtDBus',
            'PyQt5.QtDataVisualization',
            'PyQt5.QtDesigner',
            'PyQt5.QtHelp',
            'PyQt5.QtLocation',
            'PyQt5.QtMacExtras',
            'PyQt5.QtMultimedia',
            'PyQt5.QtMultimediaWidgets',
            'PyQt5.QtNetwork',
            'PyQt5.QtNetworkAuth',
            'PyQt5.QtNfc',
            'PyQt5.QtOpenGL',
            'PyQt5.QtPositioning',
            'PyQt5.QtPrintSupport',
            'PyQt5.QtPurchasing',
            'PyQt5.QtQml',
            'PyQt5.QtQuick',
            'PyQt5.QtQuickWidgets',
            'PyQt5.QtSensors',
            'PyQt5.QtSerialPort',
            'PyQt5.QtSql',
            'PyQt5.QtSvg',
            'PyQt5.QtTest',
            'PyQt5.QtWebChannel',
            'PyQt5.QtWebEngine',
            'PyQt5.QtWebEngineCore',
            'PyQt5.QtWebEngineWidgets',
            'PyQt5.QtWebKit',
            'PyQt5.QtWebKitWidgets',
            'PyQt5.QtWebSockets',
            'PyQt5.QtWinExtras',
            'PyQt5.QtX11Extras',
            'PyQt5.QtXml',
            'PyQt5.QtXmlPatterns']
hiddenimports = []

win_dll_path = 'C:\\Program Files (x86)\\Windows Kits\\10\\Redist\\ucrt\\DLLs\\x86'

if windows:
    pathex = [root_path, win_dll_path]
else:
    pathex = [root_path]

if windows:
	icon = os.path.join(windows_build_data_path, UNDERSCORE_NAME+'-icon.ico')
elif linux:
	icon = UNDERSCORE_NAME+'-icon.png'
else:
	icon = os.path.join(mac_build_data_path, UNDERSCORE_NAME+'-icon.icns')

def strip_binaries(binaries, patterns):
    return [x for x in binaries if all([pattern not in x[0].lower() for pattern in patterns])]



def post_generate():
    if windows:
        post_generate_windows()
    elif macos:
        post_generate_macos()

def post_generate_windows():
    exe_path = os.path.join(root_path, 'dist', UNDERSCORE_NAME+'.exe')
    #win_sign(exe_path)
    installer_exe_path = win_build_installer()
    #win_sign(installer_exe_path)

def post_generate_macos():
    build_data = os.path.join(mac_build_data_path, '*')
    app_name = UNDERSCORE_NAME + '.app'
    resources_path = os.path.join(dist_path, app_name, 'Contents', 'Resources')
    system(['bash', '-c', 'cp -R {} {}'.format(build_data, resources_path)])

    system(['bash', '-c', 'security unlock-keychain /Users/$USER/Library/Keychains/login.keychain'])

    system(['bash', '-c', 'codesign --deep --force --verify --verbose=1 --sign "Developer ID Application: Tinkerforge GmbH (K39N76HTZ4)" ' + os.path.join(dist_path, app_name)])

    system(['codesign', '--verify', '--deep', '--verbose=1', os.path.join(dist_path, app_name)])

    print('building disk image')
    dmg_name = '{}_macos_{}.dmg'.format(UNDERSCORE_NAME, DEMO_VERSION.replace('.', '_'))

    if os.path.exists(dmg_name):
        os.remove(dmg_name)
    os.mkdir(os.path.join(dist_path, 'dmg'))
    shutil.move(os.path.join(dist_path, app_name), os.path.join(dist_path, 'dmg'))
    system(['hdiutil', 'create', '-fs', 'HFS+', '-volname', '{}-{}'.format(UNDERSCORE_NAME, DEMO_VERSION), '-srcfolder', os.path.join(dist_path, 'dmg'), dmg_name])
