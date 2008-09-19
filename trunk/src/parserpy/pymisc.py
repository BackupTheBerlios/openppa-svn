from pygccxml import parser,declarations

""" IN: functions
    OUT: classes, datatypes of arguments + return values
"""

# static config container
try:
    cfgDict
except NameError:
    cfgDict = {}


def cfgGet(key):
    global cfgDict
    try:
        return cfgDict[key]
    except:
        return None
    
def cfgSet(key, value, overwrite = False):
    global cfgDict
    if not overwrite and cfgDict.has_key(key):
        raise Exception('cfgDict already has this key')
    
    else:
        cfgDict[key] = value
    
def cfgMerge(dict2):
    global cfgDict
    cfgDict.update(dict2)
    

def freeFnTypes(funs):
    typeList = {}
    for func in funs:
        typeList.update(argTypes(func.arguments))
        typeList[func.return_type.decl_string] = func.return_type
        
    return typeList
    
# from [args] query types
def argTypes(args):
    typeList = {}
    for arg in args:
        typeList[arg.type.decl_string] = arg.type
        
    return typeList

# debugging: print function declaration (fn_type fn_name(args...))
def printFn(fnDecl):
    retName = fnDecl.return_type.decl_string
    fName = fnDecl.name
    argNames = ''
    for arg in fnDecl.arguments:
        argNames += arg.type.decl_string
        argNames += ' '
        argNames += arg.name
        argNames += ', '

    argNames = argNames.rstrip(', ')
    print(retName + ' ' + fName + '(' + argNames + ')')

def printFns(fnDecls):
    i=0
    for fnDecl in fnDecls:
        print i,
        i += 1
        printFn(fnDecl)

def typeToStr(type):
    return type.decl_string

# IN: arg type class
# OUT: arg string idl def
def transArgTypeIdl(argType):
    transTable = { 'int'   : 'long' }
    pass

