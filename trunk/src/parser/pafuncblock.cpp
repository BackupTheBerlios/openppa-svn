//
// C++ Implementation: pafuncblock
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
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

void PAFuncBlock::addFunc(PAFuncObject *func) {
	if(iFunc >= FUNCARRLIMIT) {
		dbgPrint(3,"PAFuncBlock too many functions");
		return;
	}

	fnObjArray[iFunc++] = func;
}

// Arguments
void PAFuncBlock::addArg(PAArgObject *arg) {
	if(iArg >= ARGARRLIMIT) {
		dbgPrint(3,"PAFuncBlock too many arguments");
		return;
	}

	argObjArray[iArg++] = arg;
}

PAArgObject* PAFuncBlock::getArgById(std::string strId) {
	for(int i=0; i < iArg; i++) {
		//dbgPrint(0,"id=<%s> : <%s>",argObjArray[i]->getArgId().c_str(), strId.c_str());
		if(!argObjArray[i]->getArgId().compare(strId))
			return argObjArray[i];
	}

	dbgPrint(1,"PAFuncBlock::getArgById: Argument not found.");
	return NULL;	//nothing found
}

PAFuncObject* PAFuncBlock::getFuncByName(std::string strFnName) {
	for(int i=0; i < iArg; i++) {
		if(!fnObjArray[i]->getFuncName().compare(strFnName))
			return fnObjArray[i];
	}

	dbgPrint(1,"PAFuncBlock::getArgById: Argument not found.");
	return NULL;	//nothing found
}

void PAFuncBlock::listFns() {
	for(int i=0; i < iFunc; i++) {
		dbgPrint(0,"Function: %s",fnObjArray[i]->getFuncName().c_str());
		for(int j=0; j < fnObjArray[i]->getArgNum(); j++) {
			char* argName; char* arg;
			fnObjArray[i]->getArg(j, (const char*&)argName, (const char *&)arg);
			//fnObjArray[i]->getArg(0,NULL,NULL);
			std::string argStr(arg);

			PAArgObject* arg2 = getArgById(argStr);
			argStr;

			dbgPrint(0," arg = %s::%s -> %s", argName, arg, arg2->getArgSpec().c_str());
		}
		//fnObjArray[i]->listArgs();
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
