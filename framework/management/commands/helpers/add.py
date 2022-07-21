import os
from datetime import datetime
from django.core.management import call_command
import framework.management.commands.helpers.file_struct as F_STRUCT
import framework.management.commands.helpers.config as ConfigManager

def fileGenerator(_instance,_directory,_fname,_ext,_type):
    isDir = os.path.isdir(_directory)

    if not isDir:
        _instance.stdout.write(_instance.style.NOTICE('Directiry not found.'))
        _instance.stdout.write(_instance.style.NOTICE('Creating Directory: {}'.format(_directory)))
        os.makedirs(_directory)
        _instance.stdout.write(_instance.style.SUCCESS('Directiry created: {}'.format(_fname)))

    if True:
        if os.path.isfile(_directory+'/'+_fname+_ext):
            _instance.stdout.write(_instance.style.NOTICE('{}{} already exists'.format(_fname,_ext)))
        else:
            #create new file here
            _fileow = open(_directory+'/'+_fname+_ext, 'w+')
            
            _fileo = None
            if _ext == '.h':
                _fileo = open(F_STRUCT.header,'r')
            elif _ext == '.c':
                _fileo = open(F_STRUCT.source,'r')

            _file_txt = _fileo.read()

            if _ext == '.h':
                _fileow.write(_file_txt.format(str(datetime.now()),_type.upper(),_fname.upper(),_type.upper(),_fname.upper()))
                _instance.stdout.write(_instance.style.SUCCESS('Successfully created {}{}'.format(_fname,_ext)))
            elif _ext == '.c':
                _fileow.write(_file_txt.format(str(datetime.now()),_fname.lower()))
                _instance.stdout.write(_instance.style.SUCCESS('Successfully created {}{}'.format(_fname,_ext)))

def addDevice(_instance,_type, _name):
    dirNameFull = F_STRUCT.DEVICE_FOLDER+'/'+_type+'/'+_name
    fileGenerator(_instance, dirNameFull, _name.lower(), '.h', 'DEVICE_'+_type)
    fileGenerator(_instance, dirNameFull, _name.lower(), '.c', 'DEVICE_'+_type)
    call_command('config','--add-device',_type.lower(),_name)

def addLibs(_instance, _name):
    dirNameFull = F_STRUCT.LIBS_FOLDER+'/'+_name
    fileGenerator(_instance, dirNameFull, _name.lower(), '.h', 'LIBS_')
    fileGenerator(_instance, dirNameFull, _name.lower(), '.c', 'LIBS_')
    call_command('config','--add-lib',_name)

def addProtocols(_instance, _name):
    dirNameFull = F_STRUCT.PROTOCOLS_FOLDER+'/'+_name
    fileGenerator(_instance, dirNameFull, _name.lower(), '.h', 'PROTOCOLS_')
    fileGenerator(_instance, dirNameFull, _name.lower(), '.c', 'PROTOCOLS_')
    call_command('config','--add-protocol',_name)

def addInterrupts(_instance, _name):
    dirNameFull = F_STRUCT.INTERRUPTS_FOLDER+'/'+_name
    fileGenerator(_instance, dirNameFull, _name.lower(), '.h', 'INTERRUPTS_')
    fileGenerator(_instance, dirNameFull, _name.lower(), '.c', 'INTERRUPTS_')
    call_command('config','--add-interrupt',_name)