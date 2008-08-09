//
// C++ Implementation: ppappsender
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ppappsender.h"
#include <dlfcn.h>
#include "misc/dbgprint.h"
#include "proxy/ppcallpack.h"
#include "proxy/varpack/ppptrpack.h"	// others are included
#include "proxy/varpack/ppfunpack.h"

int PPAppSender::hello(int iData) {
	dbgPrint(0, "calling hello function");

	PPClsPack args;
	args.setSize(2);
	args[0] = new PPPtrPack;

	PPFunPack *fnPak = new PPFunPack;
	args[0]->addNode(fnPak);
	(*args[0])[0]->setData((char*)&iData, 4);
	args[1] = new PPFunPack;
	int x = 112;
	args[1]->setData((char*)&x, 4);
 
	PPCallPack callPack(&args, "hello");

	char* dataSend;
	int dataLen = callPack.compress(dataSend);

	int opId = coreEnt->send(dataSend, dataLen);
	// MEMORY LEAK!
	coreEnt->receive(opId, dataSend, dataLen);
	// process return
}

PPAppSender::PPAppSender()
{
}


PPAppSender::~PPAppSender()
{
}



void PPAppSender::loadLibSymbols(char* coreLibPath) {
	coreLibHandle = dlopen(coreLibPath, RTLD_NOW);

	if(!coreLibHandle){
		dbgPrint(2, "PPAppSender::loadLibSymbols can't find core library");
		return;
	}

	coreIfaceFn = (CoreIfaceFnType)(dlsym(coreLibHandle, "createPlugin"));
	
	if(!coreIfaceFn)
		dbgPrint(2, "PPAppSender::loadLibSymbols could not find createPlugin symbol");
}

void PPAppSender::loadPlugin(char* proxyPath, char* pluginPath) {
	coreEnt = coreIfaceFn(proxyPath, pluginPath);
}

void PPAppSender::closeLib() {
	dlclose(coreLibHandle);
}

void PPAppSender::sendSomeStuff() {
	hello(4);

	return;
	dbgPrint(0, "Sending some stuff...");
	char data[] = "Sam Staf";
	int dataLen = 9;
	int opId;

	opId = coreEnt->send(data, dataLen);
	coreEnt->receive(opId, (char*&)data, dataLen);

	dbgPrint(0, "Received reply: <%s>, <%d>", data, dataLen);
}

int main( int argc, const char* argv[] ) {
	PPAppSender proxyPlug;

	proxyPlug.loadLibSymbols("./libPPACoreLib.so");
	proxyPlug.loadPlugin("./libPPAProxyPlugLib.so", "something");

	proxyPlug.sendSomeStuff();
}
