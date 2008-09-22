from pygccxml import parser,declarations
import pyargs

parHFiles = []  # by parseHeaders, every call appends

""" headerList ex.:(["../header1.h", "../header2.h"])
config: common for all headers (working dir, include dir,...). see pygccxml.parser.parse
TODO: other arguments? help(parser.parse)

return [namespaces]
"""
def parseHeaders(hFiles, config = None):
    global parHFiles
    par = []
    par = parser.parse(hFiles, config)
    parHFiles.extend(par)
    return par


""" Take namespace, extract functions from namespace
TODO: not only from global namespace

return: [[functions], *TODO:[classes]]
"""
def parseHeader(decls, hFile):
    global freeFnList
    global clsList
    global ns

    ns = declarations.get_global_namespace( decls )
    ns.ALLOW_EMPTY_MDECL_WRAPPER = True

    # non-member functions  
    freeFnList = []
    for func in ns.free_funs(header_file = hFile):
        if func.name.startswith("__builtin"): continue      # filter builtin functions
        freeFnList.append(func)         

    return freeFnList
    # classes
"""
    for cls in ns.classes(header_file = header):
        print cls.name + "::"       
        for clfn in cls.mem_funs():
            
                        awdstrArgs = "("
            for arg in clfn.arguments:
                strArgs += "" + arg.type.decl_string + " " + arg.name + ", "
                
            strArgs = strArgs.rstrip(", ")
            strArgs += ")"
            
            print "    " + clfn.name + strArgs  
"""

# inner working

def genIdlFromFreeFuns(freeFnList):
    lvlIdent = 0
    strIdent = '  '     # two spaces for indentation
    
    idlStr = 'module PPA {\n'
    lvlIdent += 1
    
    idlStr += strIdent*lvlIdent + 'interface FreeFuns {\n'
    # now we've got the basic stub, now let's generate functions
    for fn in freeFnList:
        pass
    


# Pack Functions. Do not generate class packers, must be done separately
def procFnList(fnList):
    fnProcList = []
    for func in fnList:
        
        print "FN:",
        #common code snippet (put into proc?)
        strArgs = "("
        for arg in func.arguments:
            strArgs += "" + arg.type.decl_string.lstrip('::') + " " + arg.name + ", "
                
        strArgs = strArgs.rstrip(", ")
        strArgs += ")"
            
        print func.name + strArgs
        
        
        fnProcList.append(func.return_type.decl_string + ' ' + func.name + strArgs + '{')
        fnProcList.extend(pyargs.fnCompress(func))
        fnProcList.append('}')
        fnProcList.append('')   # append newline
    
    print 'UNRESOLVED: ',
    #print [cls.name for cls in pyargs.argClsList]
    print pyargs.argClsList.keys()
    return fnProcList
        
def writeDebugFile(strData):
    f = open('out.txt', 'w')
    f.write(strData)
    f.close()

def writeLDebugFile(lstData):
    f = open('out.txt', 'w')
    f.write('\n'.join(lstData))
    f.close()
