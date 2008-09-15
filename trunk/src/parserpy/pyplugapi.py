""" Plugin API functions (nonmember)
These may be located in different namespaces.
In every namespace is _PPAps interface (ppa plugin-static). *future PPAa[pp]s?

"""


import pymisc
import pytypes
import pytyperes

# FreeFnContainer is leaf (with is FreeFns), so it doesn't have internal dependencies
# Nothing can depend on it (and FreeFns)
class FreeFnsContainer(pytyperes.TypeRes):
    _freeFuns = None        # FreeFunctions
    
    def __init__(self):
        self.initTypeResToNull() # has no dependencies, may depend on something **relevance
        self._freeFuns = []
    
    def isEqualToType(self, type):
        for func in self._freeFuns:
            if func.isEqualToType(type):
                return True
            
        return False
        
    # DEPRECATED?
    def addFunc(self, freeFunc):
        self._freeFuns.append(freeFunc)
        
    def addFuns(self, freeFuns):
        self._freeFuns.extend(freeFuns)
        
    # Dependency list. Exclude itself
    # is not class/namespace (even if it's mapped to idl interface)
    # pass dependencies of it's fns to upper level
    def getDeps(self):
        deps = pytyperes.TypeDeps()
        for freeFunc in self._freeFuns:
            deps.mergeWith(freeFunc.getDeps())
            
        return deps
    
    def genDecls(self):
        return ["interface _PPAFreeFuns"] + [[f.genDecls() for f in self._freeFuns]]
        
    # print functions with types in natural form, as parsed    
    def printFnsNat(self):
        pymisc.printFns([free_func.getFuncDecl() for free_func in self._freeFuns])
        
    def printFnsIdl(self):
        for fn in self._freeFuncDict:
            print fn.getFnIdlStringDef()
        
    
# Implements isEqualToType
# TODO: is this typeres?
class FreeFunc:
    _pgxDecl = None
    _retType = None
    _argTypes = None
    
    # IN: function declaration, return Name, arg Names
    def __init__(self, func):
        self._pgxDecl = func
        self._retType = pytypes.TypedefSeq(func.return_type)
        
        argTypes = []
        for arg in func.arguments:
            argTypes.append(pytypes.TypedefSeq(arg.type))
            
        self._argTypes = argTypes
        
    # TypeDeps interface(1/2)
    def isEqualToType(self, type):
        # access private member TODO eliminate
        return type._pgxDecl.decl_string == self._pgxDecl.decl_string
        
    def getFuncDecl(self):
        return self._pgxDecl
    
    # TypeDeps interface(2/2)
    # this is always leaf (in ns/class tree). It's dependencies are all composite types, or typedef
    def getDeps(self):
        return pytyperes.TypeDeps([self._retType] + self._argTypes)
        
    # declaration is always absolute for it's argumentes (todo: future)
    def genDecls(self):
        return self.getFnIdlStringDef()
        
    # GENERATOR FUNCTIONS. Prior problem is categorization
    def getFnIdlStringDef(self):
        retTypeName = self._retType.getTypeName()
        fnName = self._pgxDecl.name
        argDefs = []
        
        idx=0
        while idx < len(self._argTypes):
            argDefs.append(self._argTypes[idx].getTypeName() + ' ' + \
                           self._pgxDecl.arguments[idx].name)
            idx += 1
            
            
        return (retTypeName + ' ' + fnName + '(' + ', '.join(argDefs) + ')')
    
def genIdlFn(func):
    idlFnString = pytypes.genTypeName(func.return_type)
    idlFnString += ' ' + func.name
    
    argString = ''
    for arg in func.arguments:
        argString += pytypes.genTypeName(arg.type)
        argString += ' ' + arg.name + ', '
        
    argString = argString.rstrip(', ')
    
    return idlFnString + '(' + argString + ');'