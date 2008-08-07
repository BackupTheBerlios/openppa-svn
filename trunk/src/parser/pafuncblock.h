//
// C++ Interface: pafuncblock
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PAFUNCBLOCK_H
#define PAFUNCBLOCK_H

#include "parser/pafuncobject.h"
#include "parser/paargobject.h"

#include <string>

/**
	@author Štefan Sakalík <rabbit64@users.berlios.de>
*/
class PAFuncBlock {
public:
    PAFuncBlock();
    ~PAFuncBlock();

	// Functios	(variable is _almost_ nullary function, no need for new class [or inherit?])
	void addFunc(PAFuncObject *func);
	void addArg(PAArgObject *arg);	// Arguments (types)

	PAArgObject* getArgById(std::string strId);
	PAFuncObject* getFuncByName(std::string strFnName);

	void listFns();

private:
	PAFuncObject** fnObjArray;	// array of pointers to FuncObject
	PAArgObject** argObjArray;	// array of pointers to ArgObject

	int iFunc, iArg;
};

#endif
