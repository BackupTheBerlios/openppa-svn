# Definition of an arbitrary type (+ sequence)
#
# Rules: Pointer -> sequence
#        Array   -> sequence
#        Enum    -> enum
#        Class   -> interface
#        Reference, constness... todo
# TODO: shortcut char* -> not sequence<char>, rather string

import pygccxml.declarations as decls
import pyscope
import pyresolvable
import pydeps
import pymisc

""" IN:  pygccxml type
    OUT: corresponding PPA type
    
    This ppa type will be automaticaly put to the tree (it's TypeRes mechanism)
"""
def getTypePPA(ptype):
    if  decls.is_pointer(ptype):
        return TypedefSeq(ptype)
        
    # specify return value
    elif decls.is_fundamental(ptype):
        return []
    
    elif  decls.is_class(ptype):
        return pyscope.CClass(ptype, pyresolvable.resolveNS(ptype))
    
    # unknown / not yet implemented type.
    # This is treated as class => there may be some complications.
    else:
        print 'WARNING: unknown type:' + str(ptype)
        return pyscope.CClass(ptype, pyresolvable.resolveNS(ptype))
        #raise Exception('getTypePPA::unknown type (maybe enum or something)')
    #for argDec in argDecom
    
""" Sequence of types (or just container)
    type can be fundamental or composite
"""
class TypedefSeq(pyresolvable.TypeRes):
    # _typeName # init. in __init__
    _pgxDecl = None       # base declaration
    _typeType = 'Typedef' # TypedefSeq
    
    _level = 0            # 0 = Base Type (not allowed by assert)
    _levelOfVar = 0       # Highest indirection level. Can increase when merging
                          # levelOfVar >= level
    
    _baseName = ''        # base type name (equal to _typeName). TODO: remove?
    _baseTypeName = ''    # idl type name
    _deriveTypeName = ''  # name to derive sequences from. (typically similar to _baseTypeName)

    # that below may be not
    #_typeArrList = None  # [(typeName, typeDef decl.),...] -> primary index is level
    
    def setTreeNodeInfo(self, decl):
        decom = decls.decompose_type(decl)
        decom.reverse() # decom for int** is now [int, int*, int**]
        self._pgxDecl = base = decls.type_traits.remove_declarated(decom[0])
        
        # type base is either fundamental type or class/enum/else. TODO: void
        if decls.type_traits.is_fundamental(base):
            self._baseName = base.decl_string
            self._baseTypeName = funIdlMapping(base)
            self._deriveTypeName = funIdlMappingShort(base)
            
        # TODO: let classes/enums/... objects control it's name or control it from here?
        # TODO2: specify the _whole_ mechanism behind this class
        else:
            self._baseName = base.decl_string
            self._baseTypeName = self._deriveTypeName = base.decl_string.lstrip('::') ## TODO: maybe need to stripall a::b::[actual name]
        
        # array / ptr -> corba sequence
        for ptype in decom[1:]:
            if decls.type_traits.is_pointer(ptype) or decls.type_traits.is_array(ptype):
                self._incDimension()
                
        self._levelOfVar = self._level
        self._typeName = self.getTypeName
        
        if not isinstance(decl, decls.cpptypes.void_t):
            assert(self._level) # must not be null
    
    def __init__(self, decl):
        #everything's in setTreeNodeInfo
        pass
                
    """ Side effects: if compared to another Typedef of same type:
        increase _levelOfVar if necessary
        
        Testing equality is only used in type resolution (NOW).
        Increasing level of indirection can't worsen anything.
    """
    def isEqualToType(self, ptype):
        if hasattr(ptype,'_typeType'):                  # use isinstance plz
            if ptype._typeType == 'Typedef':            # it is TypedefSeq
                if(self._baseName == ptype._baseName):  # it's the same
                    if self._level < ptype._level:      #level correction
                        self._level = ptype._level
                    
                    return True
        
        return False
        
    # depencies are if only base is not fundamental (class or else)
    # TODO: FOR EACH!!!!11
    def getDeps(self):
        if decls.type_traits.is_fundamental(self._pgxDecl):
            return pydeps.TypeDeps()    # empty typedeps: fundamentals have no deps.
        
        # this case is more like assert()
        elif decls.type_traits.is_pointer(self._pgxDecl):
            raise Exception('TypedefSeq base can not be pointer!')
        
        else:
            depType =  getTypePPA(self._pgxDecl)
            return pydeps.TypeDeps(depType)
    
    """ TypeRes API """
    def genDecls(self):
        decls = []
        
        i=0
        while i < self._level:
            i += 1
            typeName = '_PPAt' + self._deriveTypeName + 'Seq'*i
            decls.append('typedef sequence<' + self._baseTypeName + '> ' + typeName)
        
        return decls
                
    """ increase dimension """
    def _incDimension(self):
        self._level += 1    

    # DEPRECATED stuff, scheduled for removal
    """ DEPRECATED """
    def isCompat(self, decl):
        decom = decls.decompose_type(decl)
        decom.reverse()
        return decom[0].decl_string == self._baseName

    """ DEPRECATED """
    def getTypeName(self):
        if self._levelOfVar == 0:
            return self._baseTypeName
        else:
            return '_PPAt' + self._deriveTypeName + 'Seq'*self._levelOfVar
    
    """ DEPRECATED """
    def _isCompatBaseDirect(self, base):
        return base.decl_string == self._isCompatBaseDirect
                
    """ DEPRECATED """
    # increase dimension if necessary
    def merge(self, decl):
        currLvl = 0
        decom = decls.decompose_type(decl)
        decom.reverse()
        if not self._isCompatBaseDirect(decom[0]):
            raise Exception('TypedefSeq::merge types not compatible')
        
        for ptype in typeDecom[1:]:
            if decls.type_traits.is_pointer(ptype) or decls.type_traits.is_array(ptype):
                currLvl += 1
                if currLvl > self._level:
                    self._incDimension()
                    
        if currLvl == 0:
            return self._baseTypeName
        else:
            return self._typeArrList[currLvl-1][0]    # return type name
    
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
    return funMapTable.get(funType.decl_string, 'UNDEFINED')
    
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
                    'void'                      : 'void',                   # we can map void->void
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
    return funMapTable.get(funType.decl_string, 'UNDEFINED(' + funType.decl_string + ')')

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
    #--             'void'                      : 'undefined',      #no short void mapping
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
    return funMapTable.get(funType.decl_string, 'UNDEFINED('+funType.decl_string+')')


# We've got a function, it has some types.
# We need to tell the parser about the type, but also query type name
# DEPRECAATED DEPRECATED!!! really retarded class
class TypeTreeContainer():
    _types = None   # list of all types
    _baseDict = {}  # dict of {baseName :: <BaseType class>}
    
    # return typeName
    def addType(self, ptype):
        # make REVERSED decl string -> standard procedure
        typeDecom = decls.type_traits.decompose_type(ptype)
        typeDecom.reverse()
        baseName = typeDecom[0].decl_string
        
        # We have already a type registered
        if self._baseDict.has_key(baseName):
            return self._baseDict[baseName].merge(typeDecom)     # merge this type into BaseType
        
        else:
            self._baseDict[baseName] = BaseType(typeDecom)
            return self._baseDict[baseName].getDimmestType()
            
    def printTypes(self):
        for baseName, baseCls in self._baseDict.items():
            print "Base: " + baseName
            
    def printTypesAll(self):
        for baseName, baseCls in self._baseDict.items():
            print "Base: " + baseName
            baseCls.printDebug(1)
            
    def getTypedefs(self):
        allTypeDefs = []
        for baseType in self._baseDict.values():
            allTypeDefs.extend(baseType.getTypedefs())
            
        return allTypeDefs



# DEPRECATED SHITS


# hmm something
# Base for all types? class - yes, namespace - no
# typedefs
class BaseType():

    # initialize: give me a decomttposed type list
    def __init__(self, typeDecom):
        self._typeArrList = []
        base = typeDecom[0]
        
        # type base is either fundamental type or class/enum/else
        if decls.type_traits.is_fundamental(base):
            self._baseName = base.decl_string
            self._baseTypeName = funIdlMapping(base)
            self._deriveTypeName = funIdlMappingShort(base)
            
        else:
            self._baseName = base.decl_string
            self._baseTypeName = self._deriveTypeName = base.decl_string.lstrip('::')
        
        # array / ptr -> corba sequence
        for ptype in typeDecom[1:]:
            if decls.type_traits.is_pointer(ptype) or decls.type_traits.is_array(ptype):
                self._incDimension()
                
                    
    def getBaseName(self):
        return self._baseName
    
    def getTypeName(self, level = 0):
        assert level <= self._level
        if level == 0:
            return self._baseTypeName
        
        else:
            return self._typeArrList[level-1][0]
        
    def getDimmestType(self):
        if self._level == 0:
            return self._baseTypeName
        else:
            return self.getTypeIdlDef(self._level-1)
        
    def getTypeIdlDef(self, level = 0):
        assert level <= self._level
        if level == 0:
# TODO!!: this shit
            return self._baseTypeName
        
        else:
            return self._typeArrList[level-1][1]
                        
    def printDebug(self, indent=0):
        print " "*4*indent + ('level=%d, base=%s(%s), baseDerive=%s' % (self._level, self._baseName, self._baseTypeName, self._deriveTypeName))
        for typeName, typeDef in self._typeArrList:
            print ' '*4*indent + ('TypeName=%s, Typedef=%s' % (typeName, typeDef))

    def getTypedefs(self):
        return [typeTuple[1] for typeTuple in self._typeArrList]

