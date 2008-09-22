from pygccxml import parser, declarations
import pymain

# TODO: classes dictionary overwrite (is rel. ?)
# Arguments and Functions packing

# Global: argClsList

argClsList = {}

def getDeclType(arg):
    """Determines arg type"""
    if    declarations.is_pointer(arg):                return "Pointer"
    elif  declarations.is_fundamental(arg):            return "Fundamental"
    elif  declarations.is_class(arg):                  return "Class"
    else:                                              return "Unknown"
    #for argDec in argDecom:
        

def argCompress(arg):
    """INPUT:   pygccxml Argument
    
       RETURN:  Tuple ([codeLst], procArg).
       codeList = generated code in list. call '\n',join()
       procArg = pointer to packed Argument struct (variable). Is meant to be used after
       codeList (in 1 function) to further processing"""
    global argDecs, argTypes, nameArr
    global argClsList
    
    argDecs  = declarations.decompose_type(arg.type)        # [char const *, char const, char]
    argTypes = map(lambda w:getDeclType(w), argDecs)        # [Pointer, Fundamental..]
    
    # code generation
    codeList = []
    nameArr = []
    argN = arg.name
    
    # from [decomposedTypes] -> [list structures], length is the same
    for argPtr in argDecs:
        if getDeclType(argPtr) == "Pointer":
            nameArr.append('P_' + argN)
            argN = nameArr[-1]
            codeList.append('PACK_NODE ' + nameArr[-1] + ' = ' + 'PACK_PTR();')
        
        elif getDeclType(argPtr) == "Fundamental":
            nameArr.append('F_' + argN);
            argN = nameArr[-1]  
            codeList.append('PACK_NODE %s = PACK_FUN(%d);' % (nameArr[-1], argPtr.byte_size))
            
        elif getDeclType(argPtr) == "Class":
            nameArr.append('C_' + argN);
            argN = nameArr[-1]  
            codeList.append('PACK_NODE %s = PACK_CLS(&pack_CLS_%s);' % (nameArr[-1], (argPtr.decl_string).lstrip('::')))
            # TODO identities
            argClsList[argPtr.declaration.name] = argPtr.declaration
            
        elif getDeclType(argPtr) == "Unknown":
            nameArr.append('U_' + argN);
            argN = nameArr[-1]
            codeList.append('PACK_NODE %s = PACK_UNK(%d);' % (nameArr[-1], argPtr.byte_size))
            
    if argTypes[0] == "Unknown":
        print "ERROR: pyargs(argcompress): Unknown Type"
        
    i = 0
    while i < len(argTypes) - 1:
        #print("TYPE[%d]=%s" % (i, argTypes[i]))
        if argTypes[i] == "Pointer":
            codeList.append('CONNECT(%s, %s);' % (nameArr[i], nameArr[i+1]))
            
        elif argTypes[i] == "Fundamental":
            codeList.append('CONNECT_ALIAS(%s, %s);' % (nameArr[i], nameArr[i+1]))
            
        elif argTypes[i] == "Class":
            codeList.append('CONNECT_CLASS(%s, %s);' % (nameArr[i], nameArr[i+1]))
            
        elif argTypes[i] == "Unknown":
            print "ERROR: pyargs(argcompress): Unknown Type"
            codeList.append('UNKNOWN;')
            
        i+=1
    
    codeList.append("PROCESS(%s,%s);" % (arg.name, nameArr[0]))
    
    #return ";\n".join(codeList)
    return (codeList, nameArr[0])

def fnCompress(func):
    """INPUT:  pygccxml function
    
       RETURN: []Code List"""
    
    fnPackList = ['BEGIN_PACK;']
    
    for arg in func.arguments:
        codeList, finalVar = argCompress(arg)
        fnPackList.extend(codeList)
        fnPackList.append("PACK(%s);" % finalVar)
        fnPackList.append('')
        
    fnPackList.append('END_PACK;')
    return fnPackList
    
#argCompress( pymain.ns.mem_funs()[50].arguments[0] )