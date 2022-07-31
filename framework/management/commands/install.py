from django.core.management.base import BaseCommand, CommandError
from django.core.management import call_command
import framework.management.commands.helpers.install as InstallManager

class Command(BaseCommand):
    help = 'Install sotom packages'

    def __init__(self, *args, **kwargs):
        super(Command, self).__init__(*args, **kwargs)

    def add_arguments(self, parser):
        parser.add_argument('--device', dest='device' ,nargs=2, metavar=('device_type','device_name') ,type=str)
        parser.add_argument('--lib', dest='lib' ,nargs=1, metavar=('library_name') ,type=str)
        parser.add_argument('--protocol', dest='protocol' ,nargs=1, metavar=('protocol_name') ,type=str)

    def handle(self, *args, **options):
        if options['device']:
            InstallManager.install(options['device'][1].lower(), 'devices',_type=options['device'][0].lower(),_self=self)
        if options['lib']:
            InstallManager.install(options['lib'][0].lower(), 'libs',_self=self)
