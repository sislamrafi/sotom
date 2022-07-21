from django.core.management.base import BaseCommand, CommandError
from django.core.management import call_command

import framework.management.commands.helpers.create as CreateManager

class Command(BaseCommand):
    help = 'Create new sotom project'

    def __init__(self, *args, **kwargs):
        super(Command, self).__init__(*args, **kwargs)

    def add_arguments(self, parser):
        parser.add_argument('-n','--name', dest='name' ,nargs=1, metavar='project_name' ,type=str, required=True)
        parser.add_argument('--chip', dest='chip' ,nargs=1, metavar='chip_name' ,type=str, required=True)

    def handle(self, *args, **options):
        if options['name'] and options['chip']:
            CreateManager.create(options['name'][0], options['chip'][0],self)
