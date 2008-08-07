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
