""" Base class for namespace and class
Class vs Namespace:
Namespace: can be extended
           all members are static
           syntactic sugar
           can be nested
           can nest anything
           'using keyword'

Class:     can't be extended
           members may be static
           instantiation
           can nest only classes


All the indexing capabilities are in pygccxml,
this class, and others are used just for processing (translation),
and serialization of settings.

FreeFunctions: Namespace class for non-member functions,
typically Plugin API releated stuff.

Contain children classes
"""
#scopedef implements interface typeref

import pygccxml.declarations as decls
import pyplugapi

# normally call one setGlobalNS, many getGlobalNS
def getGlobalNS():
    try:
        return globalNS
    
    except NameError:
        raise Exception('globalNS not defined')
    
def setGlobalNS(ns):
    if vars().has_key('globalNS'):
        raise Exception('global NS already set')
    
    else:
        global globalNS
        globalNS = ns

class Scopedef():
    _pgxDecl = None
    _parent = None          # Namespace or Class (scopedef)
    
    def __init__(self, decl, parent):
        self._pgxDecl = decl
        self._parent = parent
        
    # try to find free functions in namespace
    # that are not '__builtin_*'
    
    # can be namespace's free functions or static class member functions
    def findFreeFns(self):
        raise NotImplementedError('Scopedef::findFreeFns')
    
    def getDecl(self):
        return self._pgxDecl
    
    def isEqualToType(self, type):
        return self._pgxDecl.decl_string == type._pgxDecl.decl_string
    

""" 
    container of everything
"""

class Namespace(Scopedef):
    _childNamespaces = None   # Array of namespaces
    _childClasses = None
    _freeFnsContainer = None  # (usually?) Plugin API (events)
    
    # init with declaration
    def __init__(self, decl, parent = None):
        print 'creating namespace==' + decl.decl_string
        
        Scopedef.__init__(self, decl, parent)
        
        self._childNamespaces = []
        self._childClasses = []
        self._freeFnsContainer = pyplugapi.FreeFnsContainer()

        if parent == None:  # global NS
            setGlobalNS(self)
            
        # namespace can have only NS parent
        else:
            parent._childNamespaces.append(self)
        
    def addFreeFns(self):
        pass
        
    def findFreeFns(self):
        fnList = self._pgxDecl.free_funs(recursive = False)     # we want to keep it in our namespaces
        freeFuns = []
        for fn in fnList:
            if not fn.name.startswith('__builtin_'):
                freeFuns.append(pyplugapi.FreeFunc(fn))     # if it's not builtin, then it's okay
                
        self._freeFnsContainer.addFuns(freeFuns)

    def printFreeFns(self):
        self._freeFnsContainer.printFnsNat()
        
    def childScope(self, childScope):
        for child in self._childNamespaces:
            if child.getDecl().decl_string == childName:
                return child
            
        # we didn't find the child namespace, soo make me a child
        # can be class or namespace (or something else TODO)
        if decls.type_traits.is_namespace(childScope):
            Namespace(childScope, self) # add namespace
            
        elif decls.type_traits.is_class(childScope):
            CClass(childScope, self)
            
    
    def getDeps(self):
        return [self._parent]       # namespace depends only on it's parent
    
    # Namespace declaration: i am only a namespace wtf
    def genDecls(self):
        return ['module ' + self._pgxDecl.decl_string ]
    
    
    
    """ There is only one global Namespace object per project (or computation)
    So resolveNS is like a special variable that can be set only once.
"""
#****************************************************************************************************
        
        
class CClass(Scopedef):             # avoid name clash
    _childClasses = None
    
    def __init__(self, decl, parent):
        Scopedef.__init__(self, decl, parent)
        self._childClasses = []
        
        # assign itself to global NS
        # TODO is this necessarry?
        if parent == None:
            self._parent = getGlobalNS()
        
        else:
            parentDecl = parent.getDecl()
            if decls.type_traits.is_class(parentDecl):
                parent._childClasses.append(self)
                
            elif decls.type_traits.is_namespace(parentDecl):
                parent._childNamespaces.append(self)
            
        
    def findFreeFns(self):
        raise NotImplementedError('CClass::findFreeFns')
        
    def getDeps(self):      # class depends on parent only?, and itself(definition)
        return [self._parent]
