MAP_FILE_LOCATION = None #"F:\\Projects\\MicroController\\STM32F446RE\\sotom_v1\\build\\final.map"
MAP_FILE = None
MAP_LINES = None

section = []
sub_section = []
variables = []
origins = []
filename = []
vartype = []
lengthVar = []

def init(file):
    global section
    global sub_section
    global variables
    global origins
    global filename
    global vartype
    global lengthVar

    section = []
    sub_section = []
    variables = []
    origins = []
    filename = []
    vartype = []
    lengthVar = []

    global MAP_FILE_LOCATION
    global MAP_FILE
    global MAP_LINES
    MAP_FILE_LOCATION = file
    MAP_FILE = open(MAP_FILE_LOCATION)
    MAP_LINES = MAP_FILE.readlines()

def is_opened():
    global MAP_FILE
    if MAP_FILE != None and not MAP_FILE.closed:
        return True
    return False

def close():
    global MAP_FILE
    MAP_FILE.close()

def isSection(line):
    if len(line) != 0 and line.startswith('.'):
        return True
    else:
        return False


def isSubSection(line):
    if len(line) != 0 and line.startswith(' ') and line.lstrip().startswith('.'):
        return True
    else:
        return False


def isValueSection(line):
    if len(line) != 0 and line.startswith('  ') and (not line.lstrip().startswith('.') or not line.lstrip().startswith('*')):
        return True
    else:
        return False


def parse():
    #all globals
    global section
    global sub_section
    global variables
    global origins
    global filename
    global vartype
    global lengthVar

    section = []
    sub_section = []
    variables = []
    origins = []
    filename = []
    vartype = []
    lengthVar = []

    #all locals
    MAP_LINE = ""
    SECTION = ''
    SUB_SECTION = ''
    FILE_NAME = ''
    VAR_TYPE = ''

    SIZE = 0
    SIZE_TAKER_FLAG = False
    PRV_ORIGIN = None

    for MAP_LINE in MAP_LINES:

        LINE_SPLIT = MAP_LINE.split()
        if len(LINE_SPLIT) == 0:
            continue

        if isSection(MAP_LINE):
            SECTION = LINE_SPLIT[0]
            SUB_SECTION = None
            FILE_NAME = None
            
            #ignore sections
            if SECTION == '.comment' or SECTION == '.ARM.attributes':
                continue

            #for size calculation
            if SIZE_TAKER_FLAG and len(LINE_SPLIT)>1:
                SIZE = int(LINE_SPLIT[1],16) - int(PRV_ORIGIN,16)
                lengthVar.append(SIZE)
            PRV_ORIGIN = LINE_SPLIT[1]
            SIZE_TAKER_FLAG = False

        if isSubSection(MAP_LINE):
            SUB_SECTION = LINE_SPLIT[0]
            FILE_NAME = "".join(map(str, LINE_SPLIT[3:]))

            #ignore sections
            if SUB_SECTION == '.comment' or SUB_SECTION == '.ARM.attributes':
                continue

            #for size calculation
            if SIZE_TAKER_FLAG:
                SIZE = int(LINE_SPLIT[1],16) - int(PRV_ORIGIN,16)
                lengthVar.append(SIZE)
            PRV_ORIGIN = LINE_SPLIT[1]
            SIZE_TAKER_FLAG = False

        if isValueSection(MAP_LINE):
            VAR_TYPE = 'address'
            if SECTION == '.comment' or SECTION == '.ARM.attributes':
                continue
            origins.append(int(LINE_SPLIT[0], 16))

            #for size calculation
            if SIZE_TAKER_FLAG:
                SIZE = int(LINE_SPLIT[0],16) - int(PRV_ORIGIN,16)
                lengthVar.append(SIZE)
            PRV_ORIGIN = LINE_SPLIT[0]
            SIZE_TAKER_FLAG = True


            if LINE_SPLIT[1] == '.':
                if len(LINE_SPLIT) >= 4:
                    variables.append('align:'+LINE_SPLIT[4])
                else:
                    variables.append('unknown')
                FILE_NAME = None
            # found end marker
            elif len(LINE_SPLIT) > 3 and LINE_SPLIT[3] == '.':
                variables.append(LINE_SPLIT[1])
                FILE_NAME = None
                VAR_TYPE = 'value'
            elif len(LINE_SPLIT) > 3 and LINE_SPLIT[3] == 'LOADADDR':
                variables.append(LINE_SPLIT[1])
                FILE_NAME = None
                VAR_TYPE = 'loadaddr:'+LINE_SPLIT[4]
            else:
                variables.append(LINE_SPLIT[1])

            # if PRV_ORIGIN != None:
            #     if len(filename) != 0 and filename[-1] == None:
            #         SIZE = 0
            #     lengthVar.append(SIZE)

            section.append(SECTION)
            sub_section.append(SUB_SECTION)
            filename.append(FILE_NAME)
            vartype.append(VAR_TYPE)
            #PRV_ORIGIN = int(LINE_SPLIT[0], 16)

def save_output(file):
    #all globals
    global section
    global sub_section
    global variables
    global origins
    global filename
    global vartype
    global lengthVar
    with open(file,'w') as f:
        lis=[section,sub_section,variables,origins,lengthVar,vartype,filename]
        f.write("section,subsection,variable,origin,size,type,filename\n")
        for x in zip(*lis):
            f.write("{0},{1},{2},{3},{4},{5},{6}\n".format(*x))

def get_symbol_details(name,_target=None,bit=32):
    global variables
    global vartype
    global origins
    global section
    global sub_section
    global filename
    global lengthVar

    output = {'name':None,'type':None,'address':None, 'value':None, 'section':None, 'sub_section':None, 'file':None, 'size':None}
    try:
        pos = variables.index(name)
        print(pos)
        output['name'] = name
        output['type'] = vartype[pos]
        output['section'] = section[pos]
        output['sub_section'] = sub_section[pos]
        output['file'] = filename[pos]
        output['size'] = lengthVar[pos]
        if output['type'] == 'address':
            output['address'] = origins[pos]
            if _target != None:
                value = None
                if output['size'] >= 4:
                    value = _target.read32(output['address'])
                elif output['size'] >= 2:
                    value = _target.read16(output['address'])
                else:
                    value = _target.read8(output['address'])
                output['value'] = value
        elif output['type'] == 'value':
            output['value'] = origins[pos]
        return output
    except ValueError:
        return output

if __name__ == "__main__":
   init("F:\\Projects\\MicroController\\STM32F446RE\\sotom_v1\\build\\final.map")
   parse()
   save_output('data_out.csv')
   print(get_symbol_details('CURRENT_MSP'))