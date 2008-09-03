# definition of type
#
# Most of the functions behave like this:
# IN: type (pygccxml class, pointerType..)
# OUT: declaration strubg
#
# Rules: Pointer -> sequence
#        Array   -> sequence
#        Enum    -> enum
#        Class   -> interface
#        Reference, constness... todo
# TODO: shortcut 4string

import pygccxml.declarations as decls

class TypeIdlGen():
    type = None
    def __init__(self, type):
        self.type = type
        
    def genDeclList(self):
        if decls.type_traits.is_pointer(type):
            return '_PPA_' + genTypeName(type.base, False) + 'Seq'
            
        elif decls.type_traits.is_void(type):
            return 'void'
    
        elif decls.type_traits.is_fundamental(type):
            if firstCall:
                return funCppMapping(type)
            
            else:
                return funIdlMappingShort(type)
            
        elif decls.type_traits.is_enum(type):
            return '_PPA_' + type.declaration.name + 'Enum'
        
        elif decls.type_traits.is_class(type):
            return '_PPA_' + type.declaration.name + 'Cls'
                    
        #elif decls.type_traits.is_pointer(type):
        #elif decls.type_traits.is_pointer(type):
        #elif decls.type_traits.is_pointer(type):
        #elif decls.type_traits.is_pointer(type):
        #elif decls.type_traits.is_pointer(type):
        #elif decls.type_traits.is_pointer(type):
        #elif decls.type_traits.is_pointer(type):
        #elif decls.type_traits.is_pointer(type):
        #elif decls.type_traits.is_pointer(type):
        #elif decls.type_traits.is_pointer(type):
        else:
            return 'UNKNOWN_PPA_' + type.decl_string


# FUNDAMENTAL TYPE MAPPING
# todo: check 32/64
# IN: C++ fundamental type
# OUT: C++ corba mapping string
# this is used only when there is pure fundamental type
def funCppMapping(funType):         
    funMapTable = { 'short unsigned int'        : 'CORBA::UShort',            # cp: 16    | co: 16
                    'int'                       : 'CORBA::Long',              # cp: 32/64 | co: 64
    #               'complex long double'       :
                    'float'                     : 'CORBA::Float',             # ?
                    'long unsigned int'         : 'CORBA::ULongLong',         # cp: 32/64 | co: 64
                    'long long unsigned int'    : 'CORBA::ULongLong',         # cp: 64    | co: 64
    #               '__java_long'               :
    #               '__java_int'                :
                    'signed int'                : 'CORBA::Long',              # cp: 32/64 | co: 64
    #               '__java_byte'               : 'a',
    #               'jchar'                     : 'a',
                    'short int'                 : 'CORBA::Short',             # cp: 16    | co: 32
                    'signed short int'          : 'CORBA::Short',             # cp: 16    | co: 32
                    'long double'               : 'CORBA::LongDouble',        # ?
    #               'jint'                      : 'a',
                    'bool'                      : 'CORBA::Boolean',           # boolean (1B?)
    #               '__java_boolean'            : 'a',
                    'unsigned char'             : 'CORBA::Char',              # 1B?
                    'long long int'             : 'CORBA::LongLong',          # cp: 64    | co: 128
    #               'complex float'             : 'a',
    #               'jdouble'                   : 'a',
    #               '__java_float'              : 'a',
    #--             'void'                      : 'undefined',
    #               '__java_double'             : 'a',
    #               'complex double'            : 'a',
    #               'jlong'                     : 'a',
                    'long int'                  : 'CORBA::Long',              # cp: 32/64 | co: 64
    #               'jfloat'                    : 'a',
                    'unsigned int'              : 'CORBA::ULongLong',         # cp: 32/64 | co: 64
                    'char'                      : 'CORBA::Char',              # 1B?
                    'double'                    : 'CORBA::Double',            # ?
    #               '__java_short'              : 'a',
    #               'jshort'                    : 'a',
    #               '__java_char'               : 'a',
                    'wchar_t'                   : 'CORBA::WChar',             # ??
                    'signed char'               : 'CORBA::Char',              # wtf?
    #               'jbyte'                     : 'a',
    #               'jboolean'                  : 'a',
                    }
    return funMapTable.get(funType.decl_string, 'undefined')
    
# keep in sync with funCppMapping
# todo: check 32/64
# IN: C++ fundamental type
# OUT: IDL corba mapping string
def funIdlMapping(funType):         
    funMapTable = { 'short unsigned int'        : 'unsigned short',           # cp: 16    | co: 16
                    'int'                       : 'long',                     # cp: 32/64 | co: 64
    #               'complex long double'       :
                    'float'                     : 'float',                    # ?
                    'long unsigned int'         : 'unsigned long long',       # cp: 32/64 | co: 64
                    'long long unsigned int'    : 'unsigned long long',       # cp: 64    | co: 64
    #               '__java_long'               :
    #               '__java_int'                :
                    'signed int'                : 'long',                     # cp: 32/64 | co: 64
    #               '__java_byte'               : 'a',
    #               'jchar'                     : 'a',
                    'short int'                 : 'short',                    # cp: 16    | co: 32
                    'signed short int'          : 'short',                    # cp: 16    | co: 32
                    'long double'               : 'long double',              # ?
    #               'jint'                      : 'a',
                    'bool'                      : 'boolean',                  # boolean (1B?)
    #               '__java_boolean'            : 'a',
                    'unsigned char'             : 'char',                     # 1B?
                    'long long int'             : 'long long',                # cp: 64    | co: 128
    #               'complex float'             : 'a',
    #               'jdouble'                   : 'a',
    #               '__java_float'              : 'a',
    #--             'void'                      : 'undefined',
    #               '__java_double'             : 'a',
    #               'complex double'            : 'a',
    #               'jlong'                     : 'a',
                    'long int'                  : 'long',                     # cp: 32/64 | co: 64
    #               'jfloat'                    : 'a',
                    'unsigned int'              : 'unsigned long long',       # cp: 32/64 | co: 64
                    'char'                      : 'char',                     # 1B?
                    'double'                    : 'double',                   # ?
    #               '__java_short'              : 'a',
    #               'jshort'                    : 'a',
    #               '__java_char'               : 'a',
                    'wchar_t'                   : 'wchar',                    # ??
                    'signed char'               : 'wchar',                    # wtf?
    #               'jbyte'                     : 'a',
    #               'jboolean'                  : 'a',
                    }
    return funMapTable.get(funType.decl_string, 'undefined')

# in sync with funIdlMapping
# todo: check 32/64
# IN: C++ fundamental type
# OUT: IDL corba mapping string
# generate shortcutted idl mapping for typedef names
def funIdlMappingShort(funType):         
    funMapTable = { 'short unsigned int'        : 'UnsignedShort',           # cp: 16    | co: 16
                    'int'                       : 'Long',                     # cp: 32/64 | co: 64
    #               'complex long double'       :
                    'float'                     : 'Float',                    # ?
                    'long unsigned int'         : 'UnsignedLongLong',       # cp: 32/64 | co: 64
                    'long long unsigned int'    : 'UnsignedLongLong',       # cp: 64    | co: 64
    #               '__java_long'               :
    #               '__java_int'                :
                    'signed int'                : 'Long',                     # cp: 32/64 | co: 64
    #               '__java_byte'               : 'a',
    #               'jchar'                     : 'a',
                    'short int'                 : 'Short',                    # cp: 16    | co: 32
                    'signed short int'          : 'Short',                    # cp: 16    | co: 32
                    'long double'               : 'LongDouble',              # ?
    #               'jint'                      : 'a',
                    'bool'                      : 'Boolean',                  # boolean (1B?)
    #               '__java_boolean'            : 'a',
                    'unsigned char'             : 'Char',                     # 1B?
                    'long long int'             : 'LongLong',                # cp: 64    | co: 128
    #               'complex float'             : 'a',
    #               'jdouble'                   : 'a',
    #               '__java_float'              : 'a',
    #--             'void'                      : 'undefined',
    #               '__java_double'             : 'a',
    #               'complex double'            : 'a',
    #               'jlong'                     : 'a',
                    'long int'                  : 'Long',                     # cp: 32/64 | co: 64
    #               'jfloat'                    : 'a',
                    'unsigned int'              : 'UnsignedLongLong',       # cp: 32/64 | co: 64
                    'char'                      : 'Char',                     # 1B?
                    'double'                    : 'Double',                   # ?
    #               '__java_short'              : 'a',
    #               'jshort'                    : 'a',
    #               '__java_char'               : 'a',
                    'wchar_t'                   : 'Wchar',                    # ??
                    'signed char'               : 'Wchar',                    # wtf?
    #               'jbyte'                     : 'a',
    #               'jboolean'                  : 'a',
                    }
    return funMapTable.get(funType.decl_string, 'undefined')