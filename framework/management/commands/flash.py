from django.core.management.base import BaseCommand, CommandError
from django.core.management import call_command
import os
import logging
logging.basicConfig(level=logging.INFO)
import framework.management.commands.helpers.file_struct as F_STRUCT

class Command(BaseCommand):
    help = 'Install sotom packages'

    def __init__(self, *args, **kwargs):
        super(Command, self).__init__(*args, **kwargs)

    def add_arguments(self, parser):
        parser.add_argument('--flash', dest='flash', action="store_true")

    def handle(self, *args, **options):
        if options['flash'] != None:
            if not os.path.isfile(F_STRUCT.BUILD_ELF_FILE_PATH):
                self.stdout.write(self.style.ERROR('No Recent Build Found. Are you forgot to build project?\nUse command `sotom build`'))
                return
            os.system(r'pyocd flash "{}"'.format(F_STRUCT.cwd+'/'+'build'+'/'+'final.elf'))
            self.stdout.write(self.style.SUCCESS('Program flashed..'))