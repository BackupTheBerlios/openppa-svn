""" Type resolution, dependenciesc ua  
    TypeDeps is only concerned with TypeRes(olvable)s 
    TypeRes must implement these (interface definition):
        
        _typeType:
            string variable describing Type of TypeRes ('TypedefSeq', 'Namespace', ...)
            is also needed for hashing
            
        _typeName:
            string variable containing definition. Not necessarily idl definition
            
        Bool isEqualToType(TypeRes):
            Determines whether type is equal to it's argument.
            It utilizes _typeType variable.
            There may be side effects. see TypedefSeq::isEqualToType for ex.
            
        TypeDeps getDeps():
            Returns depencies of itself and all it's children.
            Implementator is not concerned with parent object, it can't know whether
            it's inside or outside it's scope. However this information is included
            in it's definition? 'parent' variable (include formally?).
            **The difference between declaration and defintion** :specify
            
            namespace is parent of all it contains.
            class is parent of all it's members
            function is parent of it's return type and arguments
            
        TreeStrLs genDecls():
            TreeStrLs :: [str | TreeStrLs]
            for ex.: ['module xyz', ['member1', 'member2'], ...]
            These extra levels in tree are used to indent in declarations generation functions
            Generate decls only for itself and it's children.
            
    depency resolution:
        Every TypeRes must provide mechanism to resolve internal dependencies. See TypeRes for more information.
        This mechanism is implemented in 'TypeRes'

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

import pyscope
import pydeps
import pyresolvable
import pymisc

# interface TypeRes
""" stub interface methods (that must be overloaded
    implements internal dependency resolution
    
    Example
    x {
        // depList
        y {
            ...
        }
        
        z {
            ...
        }
    }, where y depends on z:
    x.getDeps = [y,z].getDeps()
    
"""
# use this as base class
class TypeRes():
    _typeType = ''          # main type string ('Namespace', 'Class'...)
    _typeName = '*UNKNOWN*' # declaration string. 'a::b'...
                            # type + subtype = unique type identifier
    
    _orderedDepList = None  # -> 0.. = top..bottom
    _depList = None         # {x : [y]} - where x depends on y-s # TODO SET
    
    # this is abstract interface, do not allow instantiation **
    def __init__(self):
        self._orderedDepList = []
        self._depList = []
        
    def __hash__(self):
        hash(self.getUniqueIdentStr())
    
    def __eq__(self, other):
        try:
            func = getattr(other, "getUniqueIndentStr")
            
        except AttributeError:
            return False
        
        else:
            return self.getUniqueIdentStr() == func()
            
    # __str__ = __repr__
    def __repr__(self):
        return self.getUniqueIdentStr()
    
    def getUniqueIdentStr(self):
        return '<' + self._typeType + ' ' + self._typeName + '>'
        
    def isEqualToType(self, otherType):
        raise Exception('can not call abstract interface methods')

    def getDeps(self):
        raise Exception('can not call abstract interface methods')

    def genDecls(self):
        raise Exception('can not call abstract interface methods')
    
    # internal depency resolution
    # use: self.addInternalDeps(TypeRes->getDeps())
    # requirement: no list(resolution in self)
    def addInternalDeps(self, resType, resTypeDepList):
        # if x is not in depList (and therefore not in orderedDepList)
        #    add x to both lists, then add dependencies
        if resType not in self._depList:
            self._depList[resType] = resTypeDepList
            self._orderedDepList.append(resType)
            
        # otherwise add it to the list. TODO: consider using sets
        else:
            for dep in self.resTypeDepList:
                if dep not in self._depList[resType]:
                    self._depList[resType].append(dep)
            
        # we assume resType is already in orderedDepList    
        for dep in resTypeDepList:
            self.addDepToOrdList(resType, dep)
                
    
    # x depends on y; y before x
    # x MUST be in both lists 
    def addDepToOrdList(self, x, y):
        # find x in list:
        idx = 0
        found = False
        for item in self._orderedDepList:
            if item.isEqualToType(x):
                found = True
                break
            idx += 1
            
        if not found:
            raise Exception('x is not in list')
        
        idx2 = idx + 1
        resolved = False

        # determine whether y is after x, 
        while idx2 < len(self._orderedDepList):
            if self._orderedDepList[idx2].isEqualToType():
                lift(idx2, idx)
                resolved = True
                break
            
            idx2 += 1
            
        # either there was an error, y was lifted before x, or y is not in list
        # add y bef. x
        idx2 = 0
        
        if not resolved:
            while idx2 < idx:
                if self._orderedDepList[idx2].isEqualToType(y):
                    resolved = True
                    break # do not use return
        
        if not resolved:
            # y is not in the list
            self._orderedDepList.insert(idx, y) 
        
    # lift item at itemIdx in _orderedDepList to whereIdx
    # similar to insert@insertSort
    def lift(self, itemIdx, whereIdx):
        assert itemIdx < len(self._orderedDepList), whereIdx < len(self._orderedDepList)
        assert itemIdx >= 0, whereIdx >= 0
        
        if not (itemIdx > whereIdx):
            raise Exception('reverse and nop lift is not allowed')
        
        idx = itemIdx
        # at the end, 
        while whereIdx + 1 > idx:
            self.liftSwap(itemIdx, itemIdx - 1)
        
        assert whereIdx + 1 == idx
        self.liftSwap(idx-1, idx)
        
    
    # swap idx, idx+1
    # ignoreDep: depency checking; if == True, do simple swap without any dependency checking
    def liftSwap(self, idx):
        assert idx + 1 < self._orderedDepList, idx >= 0
        s0 = self._orderedDepList[idx]
        s1 = self._orderedDepList[idx+1]
        
        # s1 must not depend on s0
        for dep in self._depList.get(s1,[]):
            if s0 == dep:
                raise Exception('liftSwap: cycle detected while swapping s0=%s with s1=%s \
                                s1 depends on s0' % (s0, s1))
            
        # finally swap
        self._orderedDepList[idx] = s1
        self._orderedDepList[idx+1]=s0
        
    # search for typeRes in depencyList
    # todo: don't access private var's of TypeDep
    def dependsOn(self, typeRes):
        for dep in self._dependencyList._typeArray:
            if dep.isEqualToType(typeRes):
                return True
            
        return False    # we expect this
                


""" Take namespace, resolve it's path to parent, add this path to global NS.
"""
def resolveNS(resType):
    globalNS = pyscope.getGlobalNS()
    scopeLs = []
    
    ltype = resType
    if ltype:
        while True:
            ltype = ltype.parent
            if not ltype: break
            scopeLs.append(ltype)

    # go from global ns to our namespace
    # if there is not, generate new namespace
    # this functionality is in pyscope module
    scopeLs.reverse()       # from top to bottom
    currScope = globalNS
    
    # walk/generate namespaces
    for scope in scopeLs:
        
        if scope == None:
            pymisc.cfgSet('scopeLs', scopeLs)
            raise Exception('none scope caught')
        
        
        currScope = globalNS.childScope(scope)
        
    return currScope

""" Print declarations from list declList
    indent = indentation level (tab = 4 spaces)
/f/
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
                raise Exception('pyresolvable::printDecls unrecognized type')
                
        elif type(declList[idx]) == list:
            printDecls(declList[idx], indent)
            
        elif type(declList[idx]) == type(None):
            print '***Warning: none in declList!!!***'
            pass
        
        else:
            print declList
            raise Exception('pyresolvable::printDecls unrecognized type')
        
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
