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
	int x = 112;

	PPClsPack args(2);
	args(0) <<= PPPtrPack(1) << PPFunPack(4, (char*)&iData);
	args(1) <<= PPClsPack(1) <<= PPFunPack(4, (char*)&x);

	// MEMORY LEAKS (deep)!
	sendFn("hello", args);
	//args = receiveFn();
}

PPAppSender::PPAppSender()
{
	
}


PPAppSender::~PPAppSender()
{
}

void PPAppSender::sendFn(char* fName, PPClsPack& args) {
	PPCallPack callPack(&args, fName);

	char* dataSend;
	int dataLen = callPack.compress(dataSend);

	opId = coreEnt->send(dataSend, dataLen);
}

PPClsPack& PPAppSender::receiveFn() {
	char* data; int dataLen;
	coreEnt->receive(opId, data, dataLen);

	PPCallPack callPack;
	retArgs = dynamic_cast<PPClsPack*>(callPack.decompressInfo(data));

	return *retArgs;		// downcast
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
