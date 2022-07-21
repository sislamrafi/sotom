from django.core.management.base import BaseCommand, CommandError
from django.core.management import call_command
import socket
import os

import framework.management.commands.helpers.config as ConfigManager
import framework.management.commands.helpers.file_struct as F_STRUCT


class Command(BaseCommand):
    help = 'For editing config'

    def __init__(self, *args, **kwargs):
        super(Command, self).__init__(*args, **kwargs)

    def add_arguments(self, parser):
        parser.add_argument('--init', dest='init_i', action='store_true')

        parser.add_argument('--add-device', dest='add_device',
                            metavar=('device_type', 'device_name'), nargs=2)
        parser.add_argument('--remove-device', dest='remove_device',
                            metavar=('device_type', 'device_name'), nargs=2)

        parser.add_argument('--add-lib', dest='add_lib',
                            metavar='library_name', nargs=1)
        parser.add_argument('--remove-lib', dest='remove_lib',
                            metavar='library_name', nargs=1)

        parser.add_argument('--add-protocol', dest='add_protocol',
                            metavar='protocol_name', nargs=1)
        parser.add_argument('--remove-protocol', dest='remove_protocol',
                            metavar='protocol_name', nargs=1)

        parser.add_argument('--add-interrupt', dest='add_interrupt',
                            metavar='interrupt_name', nargs=1)
        parser.add_argument('--remove-interrupt',
                            dest='remove_interrupt', metavar='interrupt_name', nargs=1)

        parser.add_argument('--add-var',
                            dest='add_var', metavar=('variable_name','variable_value'), nargs=2, type=str)
        parser.add_argument('--remove-var',
                            dest='remove_var', metavar=('variable_name'), nargs=1, type=str)

    def handle(self, *args, **options):
        if options['init_i']:
            ConfigManager.add_cfg()
            ConfigManager.cfg_default()
        elif options['add_device']:
            ConfigManager.cfg_add_or_remove_package(F_STRUCT.DEVICE_FOLDER,
                                                    options['add_device'][1], _type=options['add_device'][0])
        elif options['remove_device']:
            ConfigManager.cfg_add_or_remove_package(F_STRUCT.DEVICE_FOLDER,
                                                    options['remove_device'][1], _type=options['remove_device'][0], add=False)
        elif options['add_lib']:
            ConfigManager.cfg_add_or_remove_package(
                F_STRUCT.LIBS_FOLDER, options['add_lib'][0])
        elif options['remove_lib']:
            ConfigManager.cfg_add_or_remove_package(
                F_STRUCT.LIBS_FOLDER, options['remove_lib'][0], add=False)
        elif options['add_protocol']:
            ConfigManager.cfg_add_or_remove_package(
                F_STRUCT.PROTOCOLS_FOLDER, options['add_protocol'][0])
        elif options['remove_protocol']:
            ConfigManager.cfg_add_or_remove_package(
                F_STRUCT.PROTOCOLS_FOLDER, options['remove_protocol'][0], add=False)
        elif options['add_interrupt']:
            ConfigManager.cfg_add_or_remove_package(
                F_STRUCT.INTERRUPTS_FOLDER, options['add_interrupt'][0])
        elif options['remove_interrupt']:
            ConfigManager.cfg_add_or_remove_package(
                F_STRUCT.INTERRUPTS_FOLDER, options['remove_interrupt'][0], add=False)
        elif options['add_var']:
            ConfigManager.cfg_addOrRemove_variable(
                options['add_var'][0], options['add_var'][1], _add=True)
        elif options['remove_var']:
            ConfigManager.cfg_addOrRemove_variable(
                options['remove_var'][0], None,_add=False)

# print(os.getcwd())
# print(__file__ )
# print(os.path.realpath(__file__))
