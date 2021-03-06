""" Plugin API functions (nonmember)
These may be located in different namespaces.
In every namespace is _PPAps interface (ppa plugin-static). *future PPAa[pp]s?

"""


import pymisc
import pytypes
import pyresolvable
import pydeps

""" FreeFnContainer is leaf (with its FreeFns) in dependency tree,
    so it doesn't have internal dependencies.
    This is in interface _PPAFreeFuns
"""
class FreeFnsContainer(pyresolvable.TypeRes):
    _freeFuns = None        # FreeFunctions
    
    def setTreeNodeInfo(self, decl):
        self._typeType = "FreeFnsContainer"
        self._pgxDecl = decl
        
        decStr = decl.decl_string
        decStr = decStr.rstrip('::')
        decStr += '::'
        self._typeName = decStr
        
    def __init__(self, decl):
        self._freeFuns = []
    
    def isEqualToType(self, type):
        for func in self._freeFuns:
            if func.isEqualToType(type):
                return True
            
        return False
        
    # DEPRECATED?
    def addFunc(self, freeFunc):
        self._freeFuns.append(freeFunc)
        
    """ add free functions to container
    """
    def addFuns(self, freeFuns):
        self._freeFuns.extend(freeFuns) # uh-oh extend TODO: eq check
        
    """ There are no internal depencies. Pass everything to caller.
        TypeRes API
    """
    def getDeps(self):
        deps = pydeps.TypeDeps()
        for freeFunc in self._freeFuns:
            deps.mergeWith(freeFunc.getDeps())
            
        return deps
    
    """ Like getDeps, it's very simple
        TypeRes API
    """
    def genDecls(self):
        return ["interface _PPAFreeFuns"] + [[f.genDecls() for f in self._freeFuns]]
        
    """ Debugging: print pygccxml declarations
    """
    def printFnsNat(self):
        pymisc.printFns([free_func.getFuncDecl() for free_func in self._freeFuns])
        
    """ Debugging: print function declarations converted to idl
    """
    def printFnsIdl(self):
        for fn in self._freeFuncDict:
            print fn.getFnIdlStringDef()
        
    
""" FreeFunc, can only be in FreeFnsContainer
    Although it's TypeRes, it implements: isEqualToType, getDeps, genDecls
"""
class FreeFunc():
    _pgxDecl = None
    _retType = None
    _argTypes = None
    
    """ IN: pygccxml free_function
    """
    def __init__(self, func):
        self._pgxDecl = func
        self._retType = pytypes.getTypePPA(func.return_type)
        
        argTypes = []
#        try:
        for arg in func.arguments:
            argTypes.append(pytypes.getTypePPA(arg.type))
 #       except:
        #raise Exception('nondecl in function:' + func.name)
                
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
        return pydeps.TypeDeps([self._retType] + self._argTypes)
        
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