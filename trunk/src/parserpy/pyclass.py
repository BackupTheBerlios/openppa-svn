from pygccxml import parser, declarations
import pyargs

def genClassCompress(pycls):
    """INPUT:  class
       OUTPUT: List of lines of code, class compressor """

    classFnList = [\
    'void pack_CLS_' + pycls.name + '(void* data, UniStruct* uniStr) {', \
    'ClsNode* clsArgTmp = new ClsNode;', \
    'uniStr->strPtr = clsArgTmp;']
    
    for var in pycls.variables().declarations:
        (codeList, genVar) = pyargs.argCompress(var)
        classFnList.extend(codeList)
        classFnList.append('clsArgTmp->strPtr = ' + genVar + ';')
        classFnList.append('clsArgTmp->next = new ClsNode;')
        classFnList.append('clsArgTmp = clsArgTmp->next;')
        classFnList.append('')
        
    del classFnList[-3:]
    classFnList.append('clsArgTmp->next = NULL;')
    classFnList.append('}')
    return classFnList