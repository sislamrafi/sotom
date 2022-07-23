from django.core.management.base import BaseCommand, CommandError
from django.core.management import call_command

import framework.management.commands.helpers.create as CreateManager
import framework.management.commands.helpers.build as BuildManager

class Command(BaseCommand):
    help = 'Create new sotom project'

    def __init__(self, *args, **kwargs):
        super(Command, self).__init__(*args, **kwargs)

    def add_arguments(self, parser):
        parser.add_argument('--all', dest='all' ,action='store_true')
        parser.add_argument('-mc','--map-csv', dest='mcsv' ,action='store_true')

    def handle(self, *args, **options):
        if options['all']!=None:
            BuildManager.build(self,options['mcsv'])