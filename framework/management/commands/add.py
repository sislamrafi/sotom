from django.core.management.base import BaseCommand, CommandError
from django.core.management import call_command
import socket
import os

import framework.management.commands.helpers.add as AddCommandManager

class Command(BaseCommand):
    help = 'For adding devices and other libs'

    def __init__(self, *args, **kwargs):
        super(Command, self).__init__(*args, **kwargs)

    def add_arguments(self, parser):
        parser.add_argument('--device', dest='device' ,nargs=2, metavar=('device_type','device_name') ,type=str)
        parser.add_argument('--lib', dest='lib' ,nargs=1, metavar='library_name' ,type=str)
        parser.add_argument('--interrupt', dest='interrupt' ,nargs=1, metavar='interrupt_name' ,type=str)
        parser.add_argument('--protocol', dest='protocol' ,nargs=1, metavar='protocol_name' ,type=str)

    def handle(self, *args, **options):
        cwd = os.getcwd()
        if options['device']:
            AddCommandManager.addDevice(self,options['device'][0],options['device'][1].lower())
        if options['lib']:
            AddCommandManager.addLibs(self,options['lib'][0].lower())
        if options['interrupt']:
            AddCommandManager.addInterrupts(self,options['interrupt'][0].lower())
        if options['protocol']:
            AddCommandManager.addProtocols(self,options['protocol'][0].lower())
