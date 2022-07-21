import json
import os
from datetime import datetime
import framework.management.commands.helpers.file_struct as F_STRUCT

# Get the current working directory
cwd = os.getcwd()
config_filename = 'config.json'
config_filename_path = cwd+'/'+config_filename
cfg_file = None
config = None

def refresh():
    global cwd
    global config_filename
    global config_filename_path
    cwd = os.getcwd()
    config_filename = 'config.json'
    config_filename_path = cwd+'/'+config_filename


def add_cfg():
    if os.path.isfile(config_filename_path):
        print('config file exists')
    else:
        cfg_file = open(config_filename_path, 'w+')
        cfg_file.write('{}')
        print('config file created')
        cfg_file.close()
        cfg_add_main()


def cfg_load():
    global config
    cfg_file = open(config_filename_path)
    config = json.load(cfg_file)


def cfg_save():
    global config
    global config_filename_path
    cfg_file = open(config_filename_path, 'w+')
    cfg_file.write(json.dumps(config, indent=4, sort_keys=True))
    print('config file updated')
    cfg_file.close()


def cfg_default():
    cfg_load()
    global config

    if not 'CC' in config:
        config['CC'] = 'arm-none-eabi-gcc'
    if not 'MACH' in config:
        config['MACH'] = 'cortex-m4'
    if not 'OUTPUT_FOLDER' in config:
        config['OUTPUT_FOLDER'] = 'build'
    if not 'CFLAGS' in config:
        config['CFLAGS'] = '-c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0'
    if not 'LFLAGS' in config:
        config['LFLAGS'] = '-nostdlib -T $(CORE_DEVICE_FOLDER)/chip.ld -Wl,-Map=$(OUTPUT_FOLDER)/final.map'
    cfg_save()


def cfg_addOrRemove_object(_file, _add=True):
    global config
    cfg_load()
    if not 'dependency' in config.keys():
        config['dependency'] = []
    if _add and not _file in config['dependency']:
        config['dependency'].append(_file)
        cfg_save()
    if not _add and _file in config['dependency']:
        config['dependency'].remove(_file)
        cfg_save()

def cfg_addOrRemove_variable(_var,_val, _add=True):
    global config
    cfg_load()
    if _add and not _var in config.keys():
        config[_var]=_val
    if not _add and _var in config.keys():
        del config[_var]
    cfg_save()

def cfg_add_main(_name='main'):
    if not os.path.isfile(F_STRUCT.MAIN_FOLDER+'/'+_name.lower()+'.c'):
        print("File not exists")
        return
    cfg_addOrRemove_object(
        (F_STRUCT.MAIN_FOLDER+'/'+_name.lower()+'.c').replace(F_STRUCT.cwd+'/', ''))


def cfg_add_or_remove_package(package_folder, _name, _type=None, add=True):
    if _type == None:
        fullPath = package_folder+'/'+_name.upper()+'/'+_name.lower()+'.c'
    else:
        fullPath = package_folder+'/'+_type+'/'+_name.upper()+'/'+_name.lower()+'.c'
    if not os.path.isfile(fullPath):
        print("Package: {} doesn't exists".format(_name.upper()))
        return
    cfg_addOrRemove_object(fullPath.replace(F_STRUCT.cwd+'/', ''), add)
