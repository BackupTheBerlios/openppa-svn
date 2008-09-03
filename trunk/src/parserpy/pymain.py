import os
from pygccxml import parser,declarations
import pyheader
import pyargs
import pyclass
import pymisc  # debugging + designing

import pydeclclass

altHead1 = '/home/pista/usr/projects/3proxy-0.6-devel/src/proxy.h'
altConf1 = parser.config_t(define_symbols = ['NOODBC'])

""" Generate idls and PPA wrapper files.
Input: {PrjDir}
       List of definitions(headers) in {PrjDir}, called prjDefs
       Plugin API definitions(headers) plugDefs
       *plugDefs may reference prjDefs
       
Output: generated files in {PrjDir}/PPA/

This program examines definitions of classes (and non-class functions), 
plugin API functions and generates appropriate files.

Dataflow:
pyheader.parseHeaders: get declarations from header
 V
pyheader.parseheader: get classes & free functions
 V
1) Free functions:
  LogicalLoc:  PPA::Free_funs::{function}
  PhysicalLoc: {PrjDir}/PPA/{HeaderDir}/*
  idls, files generated from idl and PPA stuff are located in {PhysicalLoc}
2) Classes = similar
3) Common
  LogicalLoc:  PPA::Plug::*
  PhysicalLoc: {PrjDir}/PPA/Plug/*
  

** All PPA stuff is in PPA namespace and {$PrjDir}/PPA directory
"""

global prjDir
prjDir = os.getcwd().rsplit('/',1)[0]
            
# Parse, save file
def runParser():
    fnProc = pyheader.procFnList(headFnList)
    
    fnProc.insert(0,'UniStruct* tmpMacro;')
    fnProc.insert(0,'#include "proxygen/pgcommon.h"')

    pyheader.writeLDebugFile(fnProc)


# same as dbgRun, but uses the new infrastructure
def dbgRun2(hFile, config = None):
    global declCls
    global dbgx
    
    declCls = pydeclclass.DeclCls()
    decls = pyheader.parseHeaders([hFile], config)
    dbgx = pyheader.parseHeader(decls, hFile)
        
    for func in dbgx:
       declCls.addAPIFn(func)
       
       
       

def dbgRun(hFile, config = None):
    global decls
    global dbgx
    
    global dbgArg
    global dbgArgStr
        
    decls = pyheader.parseHeaders([hFile], config)
    dbgx = pyheader.parseHeader(decls, hFile)
    
    dbgArg = declarations.type_traits.decompose_type(dbgx[0].arguments[0].type)
    dbgArgStr = [x.decl_string for x in dbgArg]
    
if __name__ == '__main__':
    dbgRun2('/home/pista/usr/projects/openppaln/misc/dbgprint.h')
    pass
    #cfg = parser.config_t(working_directory = prjDir, include_paths = [prjDir])
    #getDefsFromHeader(prjDir + '/parser/parsemain.h', cfg)
    #getDefsFromHeader('/home/pista/usr/projects/3proxy-0.6-devel/src/proxy.h', None)
    #runParser()

def dbgRunAlt1():
    dbgRun(altHead1, altConf1)
    
def dbgRun2Alt1():
    dbgRun2(altHead1, altConf1)
