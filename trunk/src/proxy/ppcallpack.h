//
// C++ Interface: ppcallpack
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PPCALLPACK_H
#define PPCALLPACK_H

#include "proxy/varpack/ppclspack.h"

/**
	@author pista <rabbit64@users.berlios.de>
*/
class PPCallPack {
public:
	PPCallPack();
    PPCallPack(PPClsPack* args, char* cFuncName);
    ~PPCallPack();

	PPVarPack*& operator[] (const int nIndex);
	int compress(char*& data);

	void decompressInfo(char* data);

private:
	char* funcName;
	PPClsPack* args;
};

#endif
