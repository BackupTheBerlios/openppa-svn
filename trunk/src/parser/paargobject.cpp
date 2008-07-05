//
// C++ Implementation: paargobject
//
// Description: 
//
//
// Author: pista <pista@pista-desktop>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "parser/paargobject.h"
#include "misc/dbgprint.h"

using namespace std;

PAArgObject::PAArgObject(char* cArgSpec, char* cArgId, char* cArgRef) {
	strArgSpec = new string(cArgSpec);
	strArgId = new string(cArgId);
	strArgRef = new string(cArgRef);

	iSize = iAlign = -1;
}

void PAArgObject::setSize(int iSize) {
	if(iSize >= 1024) 
		dbgPrint(1,"PAArgObject: too big argument size");

	this->iSize = iSize;
}

void PAArgObject::setAlign(int iAlign) {
	if(iAlign >= 1024)
		dbgPrint(1,"PAArgObject: too big align");

	this->iAlign = iAlign;
}

PAArgObject::~PAArgObject() {
}
