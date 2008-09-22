""" A (singleton?) state class, that is used throughout the whole program as a flytrap.
This class is a data container for functions, it's types, results of parsing operations.
"""

import pygccxml.declarations as decls
import pymisc
import pyfreefns
import pytypes

class DeclCls:
    _types = None        # TypeTreeContainer
    _funs = None         # FreeFnsContainer
        
    def __init__(self):
        self._types = pytypes.TypeTreeContainer()
        self._funs = pyfreefns.FreeFnsContainer()
    
    # functions
    # add* functions todo: type clashes
    def addType(self, type):
        return self._types.addType(type)
        
    # automatically add types
    def addAPIFn(self, func):
        retFnIdl = self.addType(func.return_type)
        argsIdl = [self.addType(arg.type) for arg in func.arguments]        
        self._funs.addFunc(func,retFnIdl,argsIdl)
        
    # RETURN IDL GENERATED STUFF
    #def genIdl(self):
        
        
    def genAPIFnIdl(self):
        for fn in self._apiFuns.values():
            print pyfreefns.genIdlFn(fn)
        
    # debugging
    def printAPIFns(self):
        #pymisc.printFns(self._apiFuns.values())
        print '--as parsed--'
        self._funs.printFnsNat()
        print '--idl transform--'
        self._funs.printFnsIdl()
    
    # little debug
    def printTypes(self):
        self._types.printTypes()

    # total debug
    def printTypesAll(self):
        self._types.printTypesAll()