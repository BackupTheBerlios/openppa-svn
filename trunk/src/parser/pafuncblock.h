//
// C++ Interface: pafuncblock
//
// Description: 
//
//
// Author: pista <pista@pista-desktop>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PAFUNCBLOCK_H
#define PAFUNCBLOCK_H

#include "parser/pafuncobject.h"
#include "parser/paargobject.h"

/**
	@author pista <pista@pista-desktop>
*/
class PAFuncBlock {
public:
    PAFuncBlock();
    ~PAFuncBlock();

	// Functios	(variable is almost nullary function, no need for new class [or inherit?])
	void addFunc(char* cFuncName, char* cRet);
	void addFuncArg(char* cArgName, char* cArg);

	// Arguments
	void addArg(char* cArgSpec, char* cArgId, char* cArgRef);
	void setArgSize(int iSize);
	void setArgAlign(int iAlign);

	void listFns();

private:
	PAFuncObject** fnObjArray;	// array of pointers to FuncObject
	PAArgObject** argObjArray;	// array of pointers to ArgObject

	int iFunc, iArg;
};

#endif
