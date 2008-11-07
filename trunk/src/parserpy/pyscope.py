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

    # common for namespace and class
    def setTreeNodeInfo(self, decl):
        self._pgxDecl = decl
	self._typeName = decl.decl_string
        if self._typeName == "::": setGlobalNS(self)

        self._pgxDecl.ALLOW_EMPTY_MDECL_WRAPPER = True
    
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
    def __init__(self, decl):
        print 'creating namespace==' + decl.decl_string
        
        self._childNamespaces = []
        self._childClasses = []
        self._freeFnsContainer = pyfreefns.FreeFnsContainer()
 
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
        
    """ Returns True if typeRes is child of self (not necessarily direct, not necessarily existent!)
        self is not child of self
    """
    def isChild(self, typeRes):
        # 1) special case: global namespasce
        if typeRes._typeName == '::':
	    return True if (not self._typeName == '::') else False

	typeResStrip = typeRes._typeName.lstrip(self._typeName)
        if typeResStrip.startsWith('::'):
            return True

	return False
    
    # --------------
    # Tree insertion
    # --------------
    """ Find child object,
        If child is in tree do nothing, ...
    """
    def findChild(self, typeRes, autoCreate = True):
        # 1) is it me?
        if self._typeName == typeRes._typeName:
            return self
        
        assert(self.isChild(typeRes))
            
        # 3) we assume this could be our child (depends on pygccxml)
	mePath = self._typeName.split('::')
        assert(mePath[0] == '')
	del mePath[0]

        childPath = typeRes._typeName.split('::')
	assert(childPath[0] == '')
	del childPath[0]

        for child in self._childNamespaces:
           if child == typeRes:
               return child
               
           if child.isChild(typeRes):
	       return child.isChild(typeRes)

	
        for child in self._childClasses:
           if child == typeRes:
               return child

           if child.isChild(typeRes):
	       return child.isChild(typeRes)
            
        
        # we didn't find the child name TODO
        if typeRes._typeType == "Namespace":
            self._childNamespaces.append(typeRes)
            
        elif typeRes._typeType == "Class":
            self._childClasses.append(typeRes)

        else:
            assert(0)

    
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
    
    def __init__(self, decl):
        self._childClasses = []
        
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
