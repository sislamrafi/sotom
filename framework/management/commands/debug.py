from django.core.management.base import BaseCommand, CommandError
from django.core.management import call_command
import socket
import os
class Command(BaseCommand):
    help = 'Sotom commands'

    def __init__(self, port=8333, host='localhost' , *args, **kwargs):
        super(Command, self).__init__(*args, **kwargs)
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        try:
            self.socket.connect(("8.8.8.8", 80))
            self.host = self.socket.getsockname()[0]
        except:
            self.host = socket.gethostbyname(socket.gethostname())

        if self.host == None or self.host == '0.0.0.0':
            self.host='localhost'

    def __del__(self):
        self.socket.close()

    def add_arguments(self, parser):
        parser.add_argument('--run', dest='run' ,action='store_true')

    def handle(self, *args, **options):
        cwd = os.getcwd()
        if options['run'] != None:
            self.stdout.write(self.style.NOTICE('Project Folder: {}'.format(cwd)))
            call_command('runserver',str(self.host)+':'+str(self.port))
            self.stdout.write(self.style.ERROR('Run command exit();'))
