//
// C++ Implementation: pafuncobject
//
// Description: 
//
//
// Author: pista <pista@pista-desktop>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

using namespace std;

#include "parser/pafuncobject.h"
#include "misc/dbgprint.h"

PAFuncObject::PAFuncObject() {
	strFuncName = NULL;
	strRet = NULL;

	for(int i=0; i < ARGLIMIT; i++) {
		strArgs[i] = NULL;
		strArgNames[i] = NULL;
	}

	iArgs = 0;
}

void PAFuncObject::setFuncName(char* cFnName) {
	if(strFuncName != NULL) {
		dbgPrint(1, "PAFuncObject::setFuncName overwriting function name");
		delete strFuncName;
	}

	strFuncName = new string(cFnName);
}

const char* PAFuncObject::getFuncName() {
	return strFuncName->c_str();
}

void PAFuncObject::listArgs() {
	for(int i=0; i < iArgs; i++)
		dbgPrint(0," Arg: %s -> %s",strArgNames[i]->c_str(),strArgs[i]->c_str());
}

void PAFuncObject::beginSetArg(char* cArgName, char* cArg) {
	cleanupArgs();

    iArgs = 0;
    strArgs[0] = new string(cArg);
	strArgNames[0] = new string(cArgName);
    iArgs++;
}

void PAFuncObject::setNextArg(char* cArgName, char* cArg) {
    if(iArgs >= ARGLIMIT) {
		dbgPrint(2,"PARSER:setNextArg Too many arguments");
		return;
	}

	if(strArgs[iArgs]) {	// if arg[i] is not NULL
		dbgPrint(1,"PARSER:setNextArg: arg[i] is not null. probably no beginSetArg call");
		delete strArgs[iArgs]; }

	if(strArgNames[iArgs]) {	// if arg[i] is not NULL, name
		dbgPrint(1,"PARSER:setNextArg: argname[i] is not null. probably no beginSetArg call");
		delete strArgNames[iArgs]; }

	strArgs[iArgs] = new string(cArg);
	strArgNames[iArgs] = new string(cArgName);
	iArgs++;
}

void PAFuncObject::cleanupArgs() {
	for(int i=0; i < ARGLIMIT; i++) {
		if(strArgs[i] != NULL)
			delete strArgs[i];

		strArgs[i] = NULL;

		if(strArgNames[i] != NULL)
			delete strArgNames[i];

		strArgNames[i] = NULL;
	}

	iArgs = 0;
}
 
void PAFuncObject::setRet(char* cRet) {
	if(strRet != NULL)
		delete strRet;

	strRet = new string(cRet);
}

PAFuncObject::~PAFuncObject()
{
	if(strFuncName != NULL)
		delete strFuncName;

	if(strRet != NULL)
		delete strRet;

	cleanupArgs();
}
