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

import pygccxml.declarations as decls
import pyplugapi

""" There is one global namespace per project (task). getGlobalNS, setGlobalNS
    are wrappers around globalNS variable that can be set only once.
"""
def getGlobalNS():
    try:
        return globalNS
    
    except NameError:
        raise Exception('globalNS not defined')
    
""" See getGlobalNS
"""
def setGlobalNS(ns):
    if vars().has_key('globalNS'):
        raise Exception('global NS already set')
    
    else:
        global globalNS
        globalNS = ns

""" Scopedef has 2 children: class, namespace
    Implements TypeRes interface.
"""
class Scopedef():
    _pgxDecl = None
    _parent = None          # Namespace or Class (scopedef)
    _typeType = ''
    
    def __init__(self, decl, parent):
        self._pgxDecl = decl
        self._parent = parent
        
    # try to find free functions in namespace
    # that are not '__builtin_*'
    
    # can be namespace's free functions or static class member functions
    def findFreeFns(self):
        raise NotImplementedError('Scopedef::findFreeFns')
    
    # DEPRECATED. direct access?
    def getDecl(self):
        return self._pgxDecl
    
    def isEqualToType(self, otherType):
        ( otherType._typeType == self._typeType ) or \
        ( self._pgxDecl.decl_string == otherType._pgxDecl.decl_string )
    
    """ Does 
    """
    def doesContainType(self):
        pass
""" 
    container of everything
"""
class Namespace(Scopedef):
    _childNamespaces = None   # Array of namespaces
    _childClasses = None
    _freeFnsContainer = None  # (usually?) Plugin API (events)
    _typeType = 'Namespace'
    
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
      
    # ---------------------------
    # --- Search declarations ---
    # ---------------------------
    
    """ find free functions (typically api)
    """ 
    def findFreeFns(self):
        fnList = self._pgxDecl.free_funs(recursive = False)     # we want to keep it in our namespaces
        freeFuns = []
        for fn in fnList:
            if not fn.name.startswith('__builtin_'):
                freeFuns.append(pyplugapi.FreeFunc(fn))     # if it's not builtin, then it's okay
                
        self._freeFnsContainer.addFuns(freeFuns)
        
    """ all classes in namespace
    """
    def findClasses(self):
        clsList = self._pgxDecl.classes()
        
    # ------------------------------
    # --- TypeRes implementation ---
    # ------------------------------
    
    # dependencies are all things inside the namespace
    # deps = FreeFnLst + Classes (recurs.) + Namespace-s
    # dependency resolution
    def getDeps(self      ):
        return [self._freeFnsContainer]
    
    # Namespace declaration: i am only a namespace wtf
    def genDecls(self):
        return ['module ' + self._pgxDecl.decl_string, self._freeFnsContainer.genDecls()]
    
    # checks whether type is in this scope
    def isTypeInClass(self, typeRes):
        for ns in self._childNamespaces:
            if ns.isEqualToType(typeRes)
    
    # --------------
    # --- others ---
    # --------------
    def printFreeFns(self):
        self._freeFnsContainer.printFnsNat()
        
    """ return child by name
s        must be only that exists in source (pygccxml)
    """
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
    

class CClass(Scopedef):             # avoid name clash
    _childClasses = None
    _typeType = 'CClass'
    
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
                f
            elif decls.type_traits.is_namespace(parentDecl):
                parent._childNamespaces.append(self)
            
        
    def findFreeFns(self):
        raise NotImplementedError('CClass::findFreeFns')
        
    def getDeps(self):      # class depends on parent only?, and itself(definition)
        return [self._parent]
