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
import pyfreefns
import pyresolvable
import pydeps
import pymisc

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
class Scopedef(pyresolvable.TypeRes):
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
    
    """ Class + Namespace must implement this
    """
    def childScope(self):
        raise Exception('scope::childScope Not implemented')
""" 
    container of everything
"""
class Namespace(Scopedef):
    _childNamespaces = None   # Array of namespaces
    _childClasses = None      #       of classes
    _freeFnsContainer = None  # Usually Plugin API (events)
    _typeType = 'Namespace'
    
    # init with declaration
    def __init__(self, decl, parent = None):
        print 'creating namespace==' + decl.decl_string
        
        Scopedef.__init__(self, decl, parent)
        
        self._childNamespaces = []
        self._childClasses = []
        self._freeFnsContainer = pyfreefns.FreeFnsContainer()

        if parent == None:  # global NS
            setGlobalNS(self)
            
        # namespace can have only NS parent
        else:
            parent._childNamespaces.append(self)
            
        self._typeName = self._pgxDecl.decl_string
        self._pgxDecl.ALLOW_EMPTY_MDECL_WRAPPER = True
        
    # ---------------------------
    # --- Search declarations ---
    # ---------------------------
    
    """ Find free functions. Typically plugin API (events)
    """ 
    def findFreeFns(self):
        headerFile = pymisc.cfgGet('header', True)
        if headerFile:
            fnList = self._pgxDecl.free_funs(recursive = False, header_file = headerFile)     # we want to keep it in our namespaces
        else:
            fnList = self._pgxDecl.free_funs(recursive = False)     # we want to keep it in our namespaces
            
        freeFuns = []
        for fn in fnList:
            if not fn.name.startswith('__builtin_'):
                freeFuns.append(pyfreefns.FreeFunc(fn))     # if it's not builtin, then it's okay
                
        self._freeFnsContainer.addFuns(freeFuns)
        
    """ all classes in namespace
    """
    def findClasses(self):
        headerFile = pymisc.cfgGet('header', True)
        if headerFile:
            clsList = self._pgxDecl.classes(recursive = False, header_file = headerFile)
        else:
            clsList = self._pgxDecl.classes(recursive = False)
            
        self._childClasses = clsList
        
    """ Our namespace is ::a, class ::a::b. Class is in namespace. Simple.
    """
    def isChild(self, typeRes):
        raise Exception('isChild: not yet implemented')
        # self = ::a::b, typeRes = ::a::b::c
        if not self._typeName.startswith(typeRes._typeName):
            return False
                
        selfTypePath = self._typeName.split('::')
        childTypePath = typeRes.split('::')
        selfTypePath[-1]
    
    """ Find lowest common parent of x and y (x,y are TypeRes).
        If autoCreate is True(default), create this parent if necessary,
        otherwise throw exception
    """
    def findParent(self, x, y, autoCreate = True):
        pass
    
    """ Find child object,
        If autoCreate is True(default), create this child if necessary,
        otherwise throw exception
    """
    def findChild(self, typeRes, autoCreate = True):
        # 1) is it me?
        if self._typeName == typeRes._typeName:
            return self
        
        # 2) can it be mine? (assertion)
        if not (typeRes + '::').beginswith(self._typeName):
            raise Exception('findChild query in irrelevant namespace')
            
        # 3) we assume this could be our child (depends on pygccxml)
        (nextChildNode, dummy1, dummy2) = (typeRes._typeName
                                            .lstrip(self._typeName)) \
                                           .partition('::')
        
        for child in self._childNamespaces:
            if child.lstrip(self._typeName) == nextChildNode:
                print ('depency (' + typeRes._typeName + ') found in (' + self._typeName + ')')
                return child
            
            
        
        # we didn't find the child namespace, soo make me a child
        # can be class or namespace (or something else TODO)
        if decls.type_traits.is_namespace(childScope):
            Namespace(childScope, self) # add namespace
            
        elif decls.type_traits.is_class(childScope):
            CClass(childScope, self)

    
    # ------------------------------
    # --- TypeRes implementation ---
    # ------------------------------
    
    """ dependencies are all things inside the namespace
        deps = FreeFnLst + Classes (recurs.) + Namespace-s
        does internal dependency resolution
    """
    def getDeps(self):
        # FreeFunctions
        fnDeps = self._freeFnsContainer.getDeps()
        
        # Classes
        clsDeps = pydeps.TypeDeps(self._childClasses)
        
        # Namespaces
        nsDeps = pydeps.TypeDeps(self._childNamespaces)
        
        # All dependencies
        deps = pydeps.TypeDeps([clsDeps, nsDeps, fnDeps])
        
        # resolve internal dependencies
        children = filter(self.isChild, deps)
        self.addInternalDeps(self._freeFnsContainer, children)
        
    # Namespace declaration: i am only a namespace wtf
    def genDecls(self):
        return ['module ' + self._pgxDecl.decl_string, self._freeFnsContainer.genDecls()]
    
    # --------------
    # --- others ---
    # --------------
    def printFreeFns(self):
        self._freeFnsContainer.printFnsNat()    

class CClass(Scopedef):             # avoid name clash
    _childClasses = None
    _typeType = 'CClass'
    
    def __init__(self, decl, parent):
        Scopedef.__init__(self, decl, parent)
        self._childClasses = []
        self._typeName = decl.decl_string
        
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
        return []
        #return [self._parent]

    def genDecls(self):
        if not isinstance(self._pgxDecl, decls.class_t):
            return ['<Nonclass type ' + str(self._pgxDecl) + ' declaration>']
            
        self._pgxDecl.ALLOW_EMPTY_MDECL_WRAPPER = True
        clsDecl = '<Class ' + self._typeName + ' declaration>' # default undefined fallback (not used here)
        
        memList = [] #member variables
        memList = ['variables', [x.name for x in self._pgxDecl.get_members()]]
        
        fnList = []
        fnList = ['member functions', [x.name for x in self._pgxDecl.mem_funs()]]
        #for arg in self._pgxDecl.arguments:
        #    arg
        return ['module ' + self._pgxDecl.name, [memList + fnList]]