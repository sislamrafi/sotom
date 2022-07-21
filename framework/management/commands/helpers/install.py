import framework.management.commands.helpers.file_struct as F_STRUCT
from django.core.management import call_command
import os
import shutil

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

def install(_name,_pkg,_type=None,_self=None,_call_from=None):
    _name=_name.lower()
    _type_str = None
    if _type == None:
        _type_str = ''
    else:
        _type_str = '/'+_type

    pkg_dir_src = F_STRUCT.FRAMEWORK_APP_FOLDER+'/'+_pkg+_type_str+'/'+_name

    pkg_dir_dest = None
    if _call_from == None:
        pkg_dir_dest = F_STRUCT.APP_FOLDER+'/'+_pkg+_type_str+'/'+_name
    else:
        pkg_dir_dest = F_STRUCT.cwd+'/'+_call_from+'/'+'app'+_pkg+_type_str+'/'+_name

    isDir = os.path.isdir(pkg_dir_src)
    if isDir:
        printCL(_self, "Package {} found".format(_name), "success")
    else:
        printCL(_self, "Package {} not found".format(_name), "error")
        return

    #copy pkg 
    print(pkg_dir_src)
    print(pkg_dir_dest)
    if os.path.isdir(pkg_dir_dest):
        printCL(_self, "Package already installed: {}".format(_name), "error")
        return
    else:
        printCL(_self, "Installing package: {}".format(_name), "success")
        shutil.copytree(pkg_dir_src,pkg_dir_dest)

    if _pkg == 'devices':
        call_command('config','--add-device',_type.lower(),_name)
    if _pkg == 'protocols':
        call_command('config','--add-protocol',_name)
    if _pkg == 'libs':
        call_command('config','--add-lib',_name)
    


