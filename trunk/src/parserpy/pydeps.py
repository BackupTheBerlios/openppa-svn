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