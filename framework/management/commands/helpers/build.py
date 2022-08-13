
import framework.management.commands.helpers.file_struct as F_STRUCT
import framework.management.commands.helpers.config as ConfigManager
import ide.map_parser as MAP_PARSER
import os
import sys
import platform
import sotom.settings as Settings
from django.core.management import call_command

def printCL(_self, txt, _type=None):
    if _self == None:
        print(txt)
    else:
        if _type == 'success':
            _self.stdout.write(_self.style.SUCCESS(txt))
        if _type == 'error':
            _self.stdout.write(_self.style.ERROR(txt))
        if _type == 'notice':
            _self.stdout.write(_self.style.NOTICE(txt))

def build(_self=None,mcsv=False):
    makeFile = open(F_STRUCT.cwd+'/'+'makefile','w+')
    ConfigManager.cfg_load()
    data = ConfigManager.config

    #print makefile variables
    for key in data.keys():
        if isinstance(data[key], str):
            makeFile.write(key+' = '+data[key]+'\n')
    makeFile.write('\n\n')

    #print all
    if 'dependency' in data.keys() and isinstance(data['dependency'], list):
        makeFile.write('all:')
        for dep in data['dependency']:
            o_name = dep.replace('\\','_')
            o_name = o_name.replace('/','_')
            o_name = o_name.replace('..','_')
            o_name = o_name.replace('.c','.o')
            makeFile.write(o_name+' ')
    makeFile.write('final.elf')
    makeFile.write('\n\n')

    #print final.elf
    if 'dependency' in data.keys() and isinstance(data['dependency'], list):
        makeFile.write('final.elf:')
        for dep in data['dependency']:
            o_name = dep.replace('\\','_')
            o_name = o_name.replace('/','_')
            o_name = o_name.replace('..','_')
            o_name = o_name.replace('.c','.o')
            makeFile.write(o_name+' ')
    makeFile.write('\n\t'+"$(CC) $(LFLAGS) $(OUTPUT_FOLDER)/*.o  -o $(OUTPUT_FOLDER)/$@\n")
    makeFile.write('\n\n')

    #print dependency lists
    if 'dependency' in data.keys() and isinstance(data['dependency'], list):
        for dep in data['dependency']:
            o_name = dep.replace('\\','_')
            o_name = o_name.replace('/','_')
            o_name = o_name.replace('..','_')
            o_name = o_name.replace('.c','.o')
            makeFile.write(o_name+':'+dep+'\n')
            makeFile.write('\t'+"$(CC) $(CFLAGS) -o $(OUTPUT_FOLDER)/$@ $^\n")
            makeFile.write('\n')

    #print clean
    makeFile.write('\n\nclean-windows:\n')
    makeFile.write('\t@del /s build/*.o build/*.elf build/*.map')
    makeFile.write('\n\nclean-linux:\n')
    makeFile.write('\trm -f build/*.o build/*.elf build*.map')

    #print load
    makeFile.write('\n\nload:\n')
    makeFile.write('\topenocd -f board/stm32f4discovery.cfg')

    printCL(_self, "Make file created...",'success')
    makeFile.close()

    #build file
    if not os.path.isdir(F_STRUCT.cwd+'/'+'build'):
        os.mkdir(F_STRUCT.cwd+'/'+'build')

    if platform.system().lower() == 'windows':
        os.system('make clean-windows')
    if platform.system().lower() == 'linux':
        os.system('make clean-linux')

    if os.path.isdir(F_STRUCT.cwd+'/'+'build'):
        printCL(_self, "Running make command...",'success')
        os.system('make')

    if mcsv:
        MAP_PARSER.init(F_STRUCT.BUILD_MAP_FILE_PATH)
        MAP_PARSER.parse()
        MAP_PARSER.save_output(F_STRUCT.BUILD_PATH+'/'+'map.csv')
