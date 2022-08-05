from django.core.management.base import BaseCommand, CommandError
from django.core.management import call_command
import socket
import os

class Command(BaseCommand):
    help = 'Sotom commands'

    def __init__(self, port=8333, host='localhost' , *args, **kwargs):
        super(Command, self).__init__(*args, **kwargs)
        self.port = port
        self.host = socket.gethostbyname(socket.gethostname())
        # self.host = 'localhost'
        if self.host == None or self.host == '0.0.0.0':
            self.host='localhost'

    def add_arguments(self, parser):
        parser.add_argument('--run', dest='run' ,action='store_true')

    def handle(self, *args, **options):
        cwd = os.getcwd()
        if options['run'] != None:
            self.stdout.write(self.style.NOTICE('Project Folder: {}'.format(cwd)))
            call_command('runserver',str(self.host)+':'+str(self.port))
            self.stdout.write(self.style.ERROR('Run command exit();'))
