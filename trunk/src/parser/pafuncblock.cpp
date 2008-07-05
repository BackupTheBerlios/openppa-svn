//
// C++ Implementation: pafuncblock
//
// Description: 
//
//
// Author: pista <pista@pista-desktop>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "parser/pafuncblock.h"
#include "misc/dbgprint.h"

#define FUNCARRLIMIT 128
#define ARGARRLIMIT 1024

PAFuncBlock::PAFuncBlock()
{
	fnObjArray = new PAFuncObject* [FUNCARRLIMIT];
	argObjArray = new PAArgObject* [ARGARRLIMIT];

	int i;
	for(i = 0; i < 128; i++)
		fnObjArray[i] = NULL;

	for(i = 0; i < 1024; i++)
		argObjArray[i] = NULL;

	iFunc = iArg = 0;
}

void PAFuncBlock::addFunc(char* cFuncName, char* cRet) {
	if(iFunc >= FUNCARRLIMIT) {
		dbgPrint(3,"PAFuncBlock too many functions");
		return;
	}

	PAFuncObject* fnObj = new PAFuncObject;
	fnObjArray[iFunc] = fnObj;

	fnObj->setFuncName(cFuncName);
	fnObj->setRet(cRet);

	iFunc++;
}

void PAFuncBlock::addFuncArg(char* cArgName, char* cArg) {
	fnObjArray[iFunc-1]->setNextArg(cArgName, cArg);
}

// Arguments
void PAFuncBlock::addArg(char* cArgSpec, char* cArgId, char* cArgRef) {
	if(iArg >= ARGARRLIMIT) {
		dbgPrint(3,"PAFuncBlock too many arguments");
		return;
	}

	argObjArray[iArg] = new PAArgObject(cArgSpec, cArgId, cArgRef);
	iArg++;
}

void PAFuncBlock::setArgSize(int iSize) {
	argObjArray[iArg-1]->setSize(iSize);
}

void PAFuncBlock::setArgAlign(int iAlign) {
	argObjArray[iArg-1]->setAlign(iAlign);
}

void PAFuncBlock::listFns() {
	for(int i=0; i < iFunc; i++) {
		dbgPrint(0,"Function: %s",fnObjArray[i]->getFuncName());
		fnObjArray[i]->listArgs();
	}
}

PAFuncBlock::~PAFuncBlock()
{
	int i;

	for(i=0; i < FUNCARRLIMIT; i++) {
		if(fnObjArray[i] == NULL)
			break;		// no more functions

		delete fnObjArray[i];
	}
	delete [] fnObjArray;

	for(i=0; i < ARGARRLIMIT; i++) {
		if(argObjArray[i] == NULL)
			break;		// no more args

		delete argObjArray[i];
	}
	delete [] argObjArray;
}
