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

from pygccxml import parser, declarations
import pyheader
import pyargs
import pyclass
import pymisc  # debugging + designing

import pydeclclass
import pyscopedef
import pytyperes
import sys

global prjDir
prjDir = sys.path[0]
prjDir = prjDir.rstrip('/')     # if it's just '/'
prjDir += '/'


# test files
tests = [ \
        ['tests/dbgprint.h', None] \
        ]

# when pygccxml does not find anything of particular type
declarations.ALLOW_EMPTY_MDECL_WRAPPER = True

def main():
    runTest(0)

def dbgRun(hFiles, config = None):
    global decls        # directly from parser
    global dbgNs       # global ns
    global dbgPPANs    # 'my' global ns
    
    decls = parser.parse(hFiles, config)
    dbgNs = declarations.get_global_namespace(decls)             # loss of information decls ?-> dbgx
    
    # we've got our namespace
    dbgPPANs = pyscopedef.Namespace(dbgNs)
    dbgPPANs.findFreeFns() # find free functions
    
    print '--- Free functions of global namespace ---'
    dbgPPANs.printFreeFns()
    
    global dbgFnLst
    dbgFnLst = dbgPPANs._freeFnsContainer
    
    global dps
    dps = pytyperes.TypeDeps(dbgFnLst)
    dps.resolveDeps()
    dclst = dps.genDecls()
    
    pytyperes.printDecls(dclst)
    
    
def runTest(iTest):
    dbgRun([prjDir + tests[iTest][0]], tests[iTest][1])
    

altHead1 = '/home/pista/usr/projects/3proxy-0.6-devel/src/proxy.h'
altConf1 = parser.config_t(define_symbols = ['NOODBC'])

            
# Parse, save file
def runParser():
    fnProc = pyheader.procFnList(headFnList)
    
    fnProc.insert(0,'UniStruct* tmpMacro;')
    fnProc.insert(0,'#include "proxygen/pgcommon.h"')

    pyheader.writeLDebugFile(fnProc)


def dbgRun3(hFiles, config = None):
    global decls
    decls = parser.parse(hFiles, config)

# same as dbgRun, but uses the new infrastructure
def dbgRun2(hFile, config = None):
    global declCls
    global decls
    global dbgx
    
    declCls = pydeclclass.DeclCls()
    decls = pyheader.parseHeaders([hFile], config)
    dbgx = pyheader.parseHeader(decls, hFile)
        
    for func in dbgx:
       declCls.addAPIFn(func)
       
       

       


def dbgRunAlt1():
    dbgRun(altHead1, altConf1)
    
def dbgRun2Alt1():
    dbgRun2(altHead1, altConf1)

def dbgalt3():
    dbgRun3(['/home/pista/usr/tmp/pygccxml/someHeader.h'], None)
    
    
""" Locate and categorize definitions
"""

if __name__ == '__main__':
    main()
    
    #dbgRun(['./tests/dbgprint.h'])
    #declCls.printAPIFns()
    #cfg = parser.config_t(working_directory = prjDir, include_paths = [prjDir])
    #getDefsFromHeader(prjDir + '/parser/parsemain.h', cfg)
    #getDefsFromHeader('/home/pista/usr/projects/3proxy-0.6-devel/src/proxy.h', None)
    #runParser()
