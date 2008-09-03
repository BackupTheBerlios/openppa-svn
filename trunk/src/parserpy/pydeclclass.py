""" A (singleton?) state class, that is used throughout the whole program as a flytrap.
This class is a data container for functions, it's types, results of parsing operations.
"""

import pygccxml.declarations as decls
import pymisc
import pyplugapi

class DeclCls:
    _types = {}          # 'type name'              : 'obj_ref'
    
    # API functions, used to generate idl and PPA::Plug::*
    _apiFuns = {}        # 'function name'          : 'obj_ref'  ; todo: namespaces / clashes
    
    # functions
    # add* functions todo: type clashes
    def addType(self, type):
        typeDec = decls.type_traits.decompose_type(type)
        self._types[typeDec[-1].decl_string] = typeDec[-1]
        
    # automatically add types
    def addAPIFn(self, func):
        self.addType(func.return_type)
        for arg in func.arguments:
            self.addType(arg.type)
        
        self._apiFuns[func.name] = func
        
    def genAPIFnIdl(self):
        for fn in self._apiFuns.values():
                print pyplugapi.genIdlFn(fn)
        
    # debugging
    def printAPIFns(self):
        pymisc.printFns(self._apiFuns.values())
    
    def printTypes(self):
        print self._types.keys()