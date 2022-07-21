import os
import sotom.settings as Settings

# template files
source = os.path.dirname(__file__)+'/templates/source.tmp'
header = os.path.dirname(__file__)+'/templates/header.tmp'


# Get the current working directory and folders
cwd = os.getcwd()
APP_FOLDER = cwd+"/app"
CORE_FOLDER = cwd+"/core"
DEVICE_FOLDER = cwd+"/app/devices"
LIBS_FOLDER = cwd+"/app/libs"
PROTOCOLS_FOLDER = cwd+"/app/protocols"
INTERRUPTS_FOLDER = cwd+"/app/interrupts"
MAIN_FOLDER = cwd+"/app/main"

# Get sotom frame work base
FRAMEWORK_CORE_FOLDER = str(Settings.BASE_DIR)+'/structure/core'
FRAMEWORK_APP_FOLDER = str(Settings.BASE_DIR)+'/structure/app'
FRAMEWORK_DEVICE_FOLDER = FRAMEWORK_APP_FOLDER+'/'+'devices'
FRAMEWORK_LIB_FOLDER = FRAMEWORK_APP_FOLDER+'/'+'libs'
FRAMEWORK_PROTOCOL_FOLDER = FRAMEWORK_APP_FOLDER+'/'+'protocols'

def refresh():
    # Get the current working directory and folders
    global cwd
    global APP_FOLDER
    global CORE_FOLDER
    global LIBS_FOLDER
    global DEVICE_FOLDER
    global PROTOCOLS_FOLDER
    global INTERRUPTS_FOLDER
    global MAIN_FOLDER
    cwd = os.getcwd()
    APP_FOLDER = cwd+"/app"
    CORE_FOLDER = cwd+"/core"
    DEVICE_FOLDER = cwd+"/app/devices"
    LIBS_FOLDER = cwd+"/app/libs"
    PROTOCOLS_FOLDER = cwd+"/app/protocols"
    INTERRUPTS_FOLDER = cwd+"/app/interrupts"
    MAIN_FOLDER = cwd+"/app/main"

app_directory_struct = {
    'devices': {
        'input': None,
        'output': None,
        'io': None
    },
    'protocols': None,
    'interrupts': None,
    'libs': None,
    'main': None
}

core_directory_struct = {
    '@chip':{
        'registers':None,
        'startup':None,
    }
}