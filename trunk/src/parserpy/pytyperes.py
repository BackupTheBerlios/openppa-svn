""" Type resolution, dependencies
    TypeDeps is only concerned with TypeRes(olvable)s 
    TypeRes must implement these (interface definition):
        
        _typeType:
            string variable describing Type of TypeRes ('TypedefSeq', 'Namespace', ...)
        Bool isEqualToType(TypeRes):
            Determines whether type is equal to it's argument.
            It utilizes _typeType variable.
            There may be side effects. see TypedefSeq::isEqualToType for ex.
            
        TypeDeps getDeps():
            Returns depencies of itself and all it's children.
            Implementator is not concerned with parent object, it can't know whether
            It's inside or outside it's scope. However this information is included
            in it's definition? 'parent' variable (include formally?).
            **The difference between declaration and defintion** :specify
            
            namespace is parent of all it contains.
            class is parent of all it's members
            function is parent of it's return type and arguments
            
        TreeStrLs genDeclString():
            TreeStrLs :: [str | TreeStrLs]
            for ex.: ['module xyz', ['member1', 'member2'], ...]
            These extra levels in tree are used to indent in declarations generation functions
            Generate decls only for itself and it's children.

"""

import pyscopedef

""" Depency resolution, generation of declarations
    Lifecycle: I have type t, I want to get all depencies
    x = TypeDeps()
    x->addType(t)
    x->resolveDeps()
"""
class TypeDeps():
    # most important variable
    # [[Type, Status]]: Type = PPA type, FreeFnList, FreeFunc, CClass, ...
    #                   Status = int (0: ToProcess, 1: Processing, 2: Processed)
    _typeArray = None
    
    """ Create new TypeDeps object.
        Optional argument types: can be list of TypeRes-s or TypeRes
    """
    def __init__(self, typeInit = None):
        self._typeArray = []
        
        if typeInit == None:
            pass    # default
        
        if type(typeInit) == list:
            for typ in typeInit:
                self.addType(typ)
                
        else:   # we assume type == TypeRes
            self.addType(typeInit)
        
    """ TypeDeps can be viewed as list of typeRes-s
    
        --- internal ---
        There is no need for end-user to be concerned with Status of type.
        Either all are ToProcess or all are Processed.
    """
    def __getitem__(self, key):
        return self._typeArray[key][0]
        

    """ Add new TypeRes to resolution (for TypeDeps use mergeWith)
        Outside this class, don't use startIdx parameter!
        
        --- internal ---
        Function assumes there is no processing element in the list,
        before or at startIdx position. If this position is not specified,
        assumes there is no 'processing' element at all.
    """
    def addType(self, typeRes, startIdx = 0):
        #typeArray.append([type, 0])
        idx = startIdx
        while idx < len(self._typeArray):
            if self[idx].isEqualToType(typeRes):
                break
            idx += 1
            
        # if we're here, it means we should add dependency
        if idx >= len(self._typeArray):     # if the type is not in deparray
            self._typeArray.append([type,0])
            
            
    """ Resolve dependencies of everything in typeList.
        This dependency resolution can detect cycles
        
        --- offtopic ---
        and say cycles are not supported, because cycles are not supported yet.
        Some cycles might be supported. TODO. Some cycles are really useful.
        Hmm, will this be read by anyone besides me? Who knows.
        
        --- internal ---
        We assume there are no duplicate (equalTo) elements. see addType function.
        Take first element, flag it as processing, get all it's depencies, merge.
        Flag it as processed. Do this for each element in the array.
    """
    def resolveDeps(self):
        idx = 0
        
        while idx < len(self._typeArray):
            if self._typeArray[idx][1] == 0:      # if is undefined
                resType = self._typeArray[idx]
                resType[1] = 1                     # we've got unresolved at currIdx, processing
                resDeps = resType[0].getDeps() # we've got type depencies now
                self.mergeWith(resDeps, True)
                resType[1] = 2  #processed
                
                brk = True
            
            idx += 1
            
    """ Merge self(TypeDep) with another TypeDep. No type in depList is in 'processing' state!
        
    """
    def mergeWith(self, depList, processing = False):
        for depen in depList._typeArray:
            dep = depen[0]  # the actual type
            # todo: PROCESSED NON-PROCESS ?? merge what lists
            idx = 0
            
            if processing:       # definition can't be BEFORE depcy
                while self._typeArray[idx] != 1:
                    if self._typeArray[idx].isEqualToType(dep):
                        raise NotImplementedException('mergeWith. something is before dependable')
                    idx += 1
                
            # idx is at processing typeArray
            while idx < len(self._typeArray):
                if self._typeArray[idx][0].isEqualToType(dep):
                    break
                idx += 1
                
            # if we're here, it means we should add dependency
            if idx >= len(self._typeArray):     # if the type is not in deparray
                self._typeArray.append([dep,0])
            
    # -----------------------------------------------
        
    # genDeclStr for each type
    # 
    def genDecls(self):
        retArray = [typ[0].genDecl() for typ in self._typeArray]
        retArray.reverse()
        return retArray
    


# Take type, resolve namespaces, add resolved ns path to global namespace
# return the last namespace. I don't understand oO. TODO: explain
def resolveNS(type):
    globalNS = pyscopedef.getGlobalNS()
    scopeLs = []
    
    ltype = type
    while ltype:
        ltype = ltype.parent
        scopeLs.append(ltype)

    # go from global ns to our namespace
    # if there is not, generate new namespace
    # this functionality is in pyscopedef module
    scopeLs.reverse()       # from top to bottom
    currScope = globalNS
    
    # walk/generate namespaces
    for scope in scopeLs:
        currScope = globalNS.childScope(scope)
        
    return currScope

""" Generate idl declarations
"""
def printDecls(declList, indent = 0):
    for decl in declList:
        if type(decl) == str:
            print '{'
            print '    '*indent + decl
            print '};'
            
        elif type(decl ) == list:
            printDecls(decl, indent + 1)
            
        else:
            print '!!!error!!!'