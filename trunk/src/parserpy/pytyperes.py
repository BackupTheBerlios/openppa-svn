""" Type resolution, dependenciesc ua  
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

        *new
        // request-swap system
        // if X depends on Y, Y must be declared before first (move towards the end of the list)
        // each TypeRes implementer implements sequence of depencies + seq of elements that depend on this TypeRes implementer
        types: [Dependencies], [Dependenter]
        ex. [x,x,self,y ,x,x]
        self.dependsOn(y) : if self depends on y return True        # usually raises exception
        
        
        

Testing example:
		namespace x {
			typedef tDef int;
			namespace y {
				void k (tDef param);
				void z1 (t1 p1, t2 p2, ...);
				void z2...
			};
		};

~> res to: ['x', ['y', ['z1', 'z2']]]
resolveDeps context: In it's context resolve EXTERNAL dependencies, for example
	x.resolveDeps() -> [tDef]

	depency tree travel ->
	x.resolveDeps {
		seq1 = y.resolveDeps()
			...
		depSeq = depencies of all children
		***for dep in depSeq: filter dep if is in self

	return (filtered)
***multiple inheritance:
    namespace x {
        namespace y {
            typedef tDef int
        };
        
        namespace z {
            void k (tDef param);
        };
    }
    
    *** because of multiple inheritance we must modify first approach
    look for depencies in children too:
    if depency is in self -> request depency:
        *<connect> depency with requester*w
        if depency is not there, place it before requester
        if depency is there: check if it's before requester
            (if it's before, all's ok)
        DepSwp: <if depency is AFTER requester> cycle resol.
        [x,x,REQ,x0,x1,DEP,x,x,x]
        swp (x1) w. (DEP) -> if DEP depends on x1 -> raise Exception(Clash)
            # cycle detected -> todo allow cycles for interfaces
            
        swp (x0) w. (DEP)
            ...
        swp (REQ,DEP) -> 
=> internal/external dependency resolution
"""

import pyscopedef

# interface TypeRes
""" stub interface methods (that must be overloaded
    implements internal dependency resolution
"""
class TypeRes():
    _dependencyList = None     # TypeDeps // TypeRes's I depend on
    _dependerList = None    # TypeDeps // TypeRes's that depend on me
    
    # this is abstract interface, do not allow instantiation
    def __init__(self):
        raise Exception('can not instantate abstract interface class')

    def isEqualToType(self):
        raise Exception('can not call abstract interface methods')

    def getDeps(self):
        raise Exception('can not call abstract interface methods')

    def genDecls(self):
        raise Exception('can not call abstract interface methods')

    def initTypeResToNull(self):
        # nul
        self._dependencyList = TypeDeps()
        self._dependerList = TypeDeps()

    def addDependency(self, typeRes):
        self._dependencyList.addType(typeRes)
        
    def addDepender(self, typeRes):
        self._dependerList.addType(typeRes)
        
    # search for typeRes in depencyList
    # todo: don't access private var's of TypeDep
    def dependsOn(self, typeRes):
        for dep in self._dependencyList._typeArray:
            if dep.isEqualToType(typeRes):
                return True
            
        return False    # we expect this
        
    """ Move down in idl declaration; move towards beginning of _
    """
    def moveDown(self):
        pass

""" Depency resolution, generation of declarations
    Lifecycle: I have type (TypeRes) t, I want to get all depencies
    x = TypeDeps()
    x->addType(t)
    # alternatively x = TypeDeps(t)
    
    x->resolveDeps()
    
    and finally get declarations
    declLst = x->genDecls()
    TypeDeps can be viewed as reversed array. indexing is from bottom to top,
    so the last element is the one that should be declared first in idl.
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
        
        elif type(typeInit) == list:
            for typ in typeInit:
                self.addType(typ)
                
        else:   # we assume type == TypeRes
            self.addType(typeInit)
        
    """ TypeDeps can be viewed as list of TypeRes-s
    
        --- internal ---
        There is no need for end-user to be concerned with Status of particular TypeRes.
        Either all are ToProcess or all are Processed.
    """
    def __getitem__(self, key):
        return self._typeArray[key][0]
    
    def __len__(self):
        return len(self._typeArray)
        

    """ Add new TypeRes to resolution (for TypeDeps use mergeWith)
        Outside this class, don't use startIdx parameter!
        
        --- internal ---
        Function assumes there is no processing element in the list,
        before or at startIdx position. If this position is not specified,
        assumes there is no 'processing' element at all.
        
        We're moving this type from bottom to top. If there is clash...(todo)
    """
    def addType(self, typeRes, startIdx = 0):
        idx = startIdx
        while idx < len(self._typeArray):
            if self[idx].isEqualToType(typeRes):
                break
            idx += 1
            
        # if we're here, it means we should add dependency
        if idx >= len(self._typeArray):     # if the type is not in deparray
            self._typeArray.append([typeRes,0])
            
            
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
        
        Example: typeDeps = [x,x,x,x,x].
        Flag first as 'processing': [*,x,x,x,x]
        Get first item dependencies: [*,x,x,x,x]; * = [y,y]
        Then we merge it: [*,x,x,x,x].mergeWith([y,y])
        And we get result: [+,x,x,x,x,y]. One y is gone because it was same as one x.
        The other Y. Then proceed to the next x; then y, etc...
        
        In this case: [+, + = y, *, x, ...] -> where depency is already placed
        before 'resolving' element there is a problem; there may be cycle, or not.
        We must try to reloate that element (+=y) after *.
        hmm, todo or something?. Right now it displays warning message
        
        
        ***ALWAYS*** go this way. The resolution goes from bottom to top; the array
        can only look like this [+,+,*,x,x,x], NO [+,x,+,...]. So [resolved] + resolving + [to resolve]
        
    """
    def resolveDeps(self):
        idx = 0
        
        while idx < len(self):
            if self._typeArray[idx][1] == 0:      # if is ToProcess
                resType = self._typeArray[idx]
                resType[1] = 1                    # we've got unresolved at currIdx, processing
                resDeps = resType[0].getDeps()    # we've got type depencies now
                
                self.mergeWith(resDeps, True)
                resType[1] = 2  #processed
                
            idx += 1
            
    """ Merge self(TypeDep) with another TypeDep.
        Dont use processing argument!
        x->mergeWith(y) merges y into x. y can (don't have to) be deleted.
        
        --- internal ---
        case: x->mergeWith(y)
        y must not have 'processing' element. Handles Processed and ToProcess element
        the same way (TODO: specify for user).
        
        if processing == False:
            it's like calling addType for each element in the array
            
        elif processing == True (by example):
            list == [resolved] + resolving + [to resolve] <= invariantc
            [x,x,x,x,x,x] -> this is a list of depencies
    """
    def mergeWith(self, typeDeps, processing = False):
        for dep in typeDeps:
            # todo: PROCESSED NON-PROCESS ?? merge what lists
            idx = 0
            
            if processing:       # definition can't be BEFORE depcy
                while self._typeArray[idx][1] != 1:
                    if self[idx].isEqualToType(dep):
                        raise NotImplementedException('mergeWith. something is before dependable')
                    idx += 1
                
            # so add the dependency
            self.addType(dep, idx)

    # -----------------------------------------------
        
    # genDeclStr for each type
    # 
    def genDecls(self):
        retArray = [x.genDecls() for x in self]
        retArray.reverse()
        return retArray
    


""" Take namespace, resolve it's path to parent, add this path to global NS.
"""
def resolveNS(resType):
    globalNS = pyscopedef.getGlobalNS()
    scopeLs = []
    
    ltype = resType
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

""" Print declarations from list declList
    indent = indentation level (tab = 4 spaces)

    declList :: [str | declList]
    if declList in list is preceded by str, put that list in str-named scope,
    otherwise put it in unnamed scope
    
    example: ['x', ['y', 'z']] ->
    x {
        y;
        z;
    };
    
    example2: [['x','y']] ->
    x;
    y;
    
    empty sequence ~> nothing
"""
def printDecls(declList, indent = 0):
    idx = 0
    while idx < len(declList) - 1: # we use idx + 1 here
        
        # [..., 'string', ...]
        if type(declList[idx]) == str:
            
            # [..., 'string', 'string', ] -> sequence
            if type(declList[idx+1]) == str:
                print '    '*indent + declList[idx] + ';'
                
            # [..., 'string', [decls], ] -> sequence    
            elif type(declList[idx+1]) == list:
                print '    '*indent + declList[idx] + ' {'
                printDecls(declList[idx+1], indent+1)
                print '    '*indent + '};'
                idx += 1        # += 2 ttl
                
            else:
                raise Exception('pytyperes::printDecls unrecognized type')
                
        elif type(declList[idx]) == list:
            printDecls(declList[idx], indent)
            
        else:
            raise Exception('pytyperes::printDecls unrecognized type')
        
        idx += 1
            
    # last item
    if idx == len(declList) - 1:
        if type(declList[idx]) == str:
            print '    '*indent + declList[idx] + ';'
            
        elif type(declList[idx]) == list:
            printDecls(declList[idx], indent)
            
        idx += 1
        
    # at the end idx == len(declList)
    assert(idx == len(declList))
