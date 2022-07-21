# api code goes here
import ide.map_parser as MapParser
from django.http import JsonResponse

import os
import json
import datetime

from pyocd.core.helpers import ConnectHelper
from pyocd.flash.file_programmer import FileProgrammer
from pyocd.debug.elf.symbols import ELFSymbolProvider

import logging
logging.basicConfig(level=logging.INFO)


session = ConnectHelper.session_with_chosen_probe(auto_open=False)
board = session.board
target = board.target
flash = target.memory_map.get_boot_memory()
session_start_time = None

ELF_FILE = os.getcwd()+"\\build\\final.elf"
MAP_FILE = os.getcwd()+"\\build\\final.map"

provider = None


def loadELF_MAP():
    global MAP_FILE
    check_map = checkFileOrError(MAP_FILE, 'ELF_MAP')
    if check_map != None:
        return JsonResponse(check_map)
    MapParser.init(MAP_FILE)
    print("Loading {} from {}".format('ELF_MAP', MAP_FILE))


def loadELF():
    global provider
    global ELF_FILE
    check_elf = checkFileOrError(ELF_FILE)
    if check_elf != None:
        return JsonResponse(check_elf)
    target.elf = None
    target.elf = ELF_FILE
    provider = ELFSymbolProvider(target.elf)
    print("Loading ELF from {}".format(ELF_FILE))


def checkSessionOrError():
    global session
    response = {}
    if not session.is_open:
        response['status'] = 'error'
        response['message'] = 'No session is running'
        return response
    return None


def checkFileOrError(filename, type='ELF'):
    response = {}
    if not os.path.exists(filename):
        response['status'] = 'error'
        response['message'] = '{} File not found'.format(type)
        return response
    return None


def get_device(request):
    check_session = checkSessionOrError()
    if check_session != None:
        return JsonResponse(check_session)

    response = {}
    response['status'] = 'ok'
    response['name'] = board.name
    response['target'] = board.target_type
    response['uinque_id'] = board.unique_id
    return JsonResponse(response)


def get_debuger_session(request):
    global session_start_time
    global session
    response = {}
    response['is_running'] = session.is_open

    if session.is_open:
        print("A session is running already")
    else:
        session.open()
        session_start_time = datetime.datetime.now()
        target.reset()
        loadELF_MAP()
        MapParser.parse()
        print("New session opened {}".format(session_start_time))

    response['session_start_time'] = session_start_time
    response['status'] = 'ok'
    return JsonResponse(response)


def command_device(request):
    check_session = checkSessionOrError()
    if check_session != None:
        return JsonResponse(check_session)

    command = request.GET.get('command', None)
    address = request.GET.get('address', None)
    value = request.GET.get('value', None)

    response = {}
    status = 'ok'
    print(command)
    if value != None:
        print(int(address))
        print(int(value))
    if command == None:
        response['message'] = "No command given"
    elif 'write' in command and (address == None or value == None):
        response['message'] = "No value or address given"
        status = 'error'
    elif command == 'reset':
        target.reset()
        response['message'] = "Reset Done"
    elif command == 'resume':
        target.resume()
        response['message'] = "Device Resumed"
    elif command == 'reset_and_halt':
        target.reset_and_halt()
        response['message'] = "Device Reset, then Halted"
    elif command == 'halt':
        target.halt()
        response['message'] = "Device Halted"
    elif command == 'flash':
        MapParser.close()
        target.reset_and_halt()
        FileProgrammer(session).program(ELF_FILE)
        target.reset()
        response['message'] = "100\\% Flashed"
        loadELF_MAP()
        MapParser.parse()
    elif command == 'write16':
        target.write16(int(address), int(value))
    elif command == 'write32':
        target.write32(int(address), int(value))

    response['device_state'] = target.get_state().value
    response['status'] = status

    return JsonResponse(response)


def get_memory_info(request):
    global target
    global ELF_FILE
    global provider

    response = {}

    check_session = checkSessionOrError()
    if check_session != None:
        return JsonResponse(check_session)

    if not MapParser.is_opened():
        response['message'] = "ELF_MAP File not opened"
        response['status'] = 'error'
        return JsonResponse(response)

    FlashTotal = 0
    SRamTotal = 0
    SRamEnd = 0
    for m in target.get_memory_map():
        if str(m.type) == 'MemoryType.FLASH':
            FlashTotal += m.length
        if str(m.type) == 'MemoryType.RAM':
            SRamTotal += m.length
            SRamEnd = m.end

    msp = MapParser.get_symbol_details("CURRENT_MSP", target)
    bssSize = MapParser.get_symbol_details("_bss_size", target)
    dataSize = MapParser.get_symbol_details("_data_size", target)
    textSize = MapParser.get_symbol_details("_text_size", target)
    debugButton = MapParser.get_symbol_details("DEBUG_BUTTON", target, bit=16)
    debugAnalogIo = MapParser.get_symbol_details(
        "DEBUG_ANALOG_IO", target, bit=16)
    clockSpeed = MapParser.get_symbol_details("SYS_CLOCK_SPEED", target)

    # print(msp) #return

    response['bss_size'] = bssSize['value']
    response['data_size'] = dataSize['value']
    response['stack_size'] = SRamEnd - msp['value']
    response['text_size'] = textSize['value']
    response['total_flash'] = FlashTotal
    response['total_sram'] = SRamTotal
    response['sys_clock_speed'] = clockSpeed['value']
    response['debug_button_addr'] = debugButton['address']
    response['debug_button'] = debugButton['value']
    response['debug_analog_io'] = debugAnalogIo['value']
    response['debug_analog_io_addr'] = debugAnalogIo['address']
    response['message'] = "Success"
    response['status'] = 'ok'

    return JsonResponse(response)


def searchMemory(request):
    global target
    check_session = checkSessionOrError()
    if check_session != None:
        return JsonResponse(check_session)

    value = request.GET.get('value', None)

    response = {}
    print("variable")
    print(value)

    if not MapParser.is_opened():
        response['message'] = "ELF_MAP File not opened"
        response['status'] = 'error'
        return JsonResponse(response)

    is_integer = False

    try:
        if value.startswith('0x'):
            print("hex Value")
            value = int(value,16)
        else:
            value = int(value,10)
        is_integer = True
    except:
        print('Variable is not integer')

    if is_integer:
        try:
            pos = MapParser.origins.index(value)
            value = MapParser.variables[pos]
            val = MapParser.get_symbol_details(value, target)
        except ValueError:
            print('address not found in ELF')
        
    val = MapParser.get_symbol_details(value, target)
    response = val

    if val['name'] == None and is_integer:
        response['address'] = value
        try:
            response['value'] = target.read32(value)
        except:
            try:
                print("read with 16 bit")
                response['value'] = target.read16(value)
            except:
                print("read with 8 bit")
                response['value'] = target.read8(value)
        response['message'] = 'Value read from device memory'


    response['status'] = 'ok'

    return JsonResponse(response)
