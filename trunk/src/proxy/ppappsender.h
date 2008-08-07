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

/**
	@author pista <pista@pista-desktop>
*/
class PPAppSender{
public:
    PPAppSender();
    ~PPAppSender();

	void loadLibSymbols(char* coreLibPath);
	void loadPlugin(char* proxyPath, char* pluginPath);
	void closeLib();

	void sendSomeStuff();
private:
	// symbols
	void* coreLibHandle;
	PCPlugEntity* coreEnt;
	//core ent
	//int (PCPlugEntity::*sendPtr)(char* data, int dataLen);
	//void (PCPlugEntity::*receivePtr)(int opId, char* data, int& dataLen);

	typedef PCPlugEntity* (*CoreIfaceFnType) (char* proxyPath, char* plugPath);
	typedef int (*CoreEntSend) (PCPlugEntity* ent, char* data, int dataLen);
	typedef void (*CoreEntReceive) (PCPlugEntity* ent, int opId, char* data, int& dataLen);

	CoreIfaceFnType coreIfaceFn;
	CoreEntSend coreEntSendFn;
	CoreEntReceive coreEntReceiveFn;
};

#endif
