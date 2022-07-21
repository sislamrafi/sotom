import framework.management.commands.helpers.file_struct as F_STRUCT
import framework.management.commands.helpers.config as ConfigManager
import os
import shutil
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


def ignore_files(dir, files):
    print(dir)
    print(files)
    return [f for f in files if os.path.isfile(os.path.join(dir, f))]


def create_directory_tree_from_dict(dict_, _root):
    if isinstance(dict_, dict):
        for _folder in dict_.keys():
            create_directory_tree_from_dict(dict_[_folder], _root+'/'+_folder)
    else:
        if _root != None:
            os.makedirs(_root)


def create(project, chip, _self=None):

    # create project file
    project_folder = F_STRUCT.cwd+'/'+project
    isDir = os.path.isdir(project_folder)
    if isDir:
        printCL(_self, "Project with same name exists", "notice")
        return
    else:
        os.makedirs(project_folder)
        printCL(_self, "Creating {}...".format(project), "success")

    # find chip
    chip_folder_src = F_STRUCT.FRAMEWORK_CORE_FOLDER+'/'+chip.lower()
    isDir = os.path.isdir(chip_folder_src)
    if not isDir:
        printCL(_self, "No such chip named {} found".format(chip), "error")
        printCL(_self, "Abort operation".format(chip), "error")
        return
    else:
        printCL(_self, "{} chip found".format(chip), "success")

    # core install
    printCL(_self, "Installing core of {}".format(chip), "success")
    project_chip_folder = project_folder+'/'+'core'+'/'+chip.lower()
    shutil.copytree(chip_folder_src, project_chip_folder)

    # create app dir
    printCL(_self, "Creating app structure : {}".format(project), "success")
    project_app_folder = project_folder+'/'+'app'
    create_directory_tree_from_dict(
        F_STRUCT.app_directory_struct, project_app_folder)

    # change dir
    os.chdir(r"{}".format(project))
    F_STRUCT.refresh()
    ConfigManager.refresh()

    # init main file of project
    printCL(_self, "Initializing Main in {}".format(project), "success")
    core_temp_folder = F_STRUCT.FRAMEWORK_CORE_FOLDER+'/'+chip+'/'+'templates'
    temp_main_h_name = core_temp_folder+'/'+'main.h.tmp'
    shutil.copyfile(temp_main_h_name, F_STRUCT.MAIN_FOLDER+'/'+'main.h')
    temp_main_c_name = core_temp_folder+'/'+'main.c.tmp'
    shutil.copyfile(temp_main_c_name, F_STRUCT.MAIN_FOLDER+'/'+'main.c')
    temp_board_h_name = core_temp_folder+'/'+'board.h.tmp'
    shutil.copyfile(temp_board_h_name, F_STRUCT.MAIN_FOLDER+'/'+'board.h')

    # run init commands
    project_chip_folder = F_STRUCT.CORE_FOLDER+'/'+chip.lower()

    call_command('config', '--init')

    call_command('config', '--add-var', 'CORE_DEVICE_FOLDER',
                 project_chip_folder.replace(F_STRUCT.cwd+'/', ''))
    call_command('config', '--add-var', 'CHIP',chip.lower())

    for file in os.listdir(project_chip_folder+'/'+'startup'):
        if file.endswith(".c"):
            s = str(project_chip_folder+'/'+'startup' +'/'+ file)
            ConfigManager.cfg_addOrRemove_object(s.replace(F_STRUCT.cwd+'/', ''))

    call_command('install', '--lib', 'GPIO')
    call_command('install', '--lib', 'TIMER')
    call_command('install', '--lib', 'RCC')
    call_command('install', '--lib', 'DEBUG')

