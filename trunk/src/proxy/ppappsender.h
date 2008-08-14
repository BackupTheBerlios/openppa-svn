//
// C++ Interface: ppappsender
//
// Description: 
//
//
// Author: pista <pista@pista-desktop>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PPAPPSENDER_H
#define PPAPPSENDER_H
#include "core/pcplugentity.h"
#include "proxy/varpack/ppclspack.h"

/**
	@author pista <pista@pista-desktop>
*/
class PPAppSender{
public:
	// sample send fn
	int hello(int iData);

	// sender functions
    PPAppSender();
    ~PPAppSender();

	// convenient send/receive
	void sendFn(char* fName, PPClsPack& args);
	PPClsPack& receiveFn();

	// proxy fn: int function
	void loadLibSymbols(char* coreLibPath);
	void loadPlugin(char* proxyPath, char* pluginPath);
	void closeLib();

	void sendSomeStuff();
private:
	// state information
	int opId;
	PPClsPack* retArgs;

	// symbols
	void* coreLibHandle;
	PCPlugEntity* coreEnt;

	typedef PCPlugEntity* (*CoreIfaceFnType) (char* proxyPath, char* plugPath);
	CoreIfaceFnType coreIfaceFn;
};

#endif
