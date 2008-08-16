//
// C++ Implementation: ppappreceiver
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ppappreceiver.h"
#include "misc/dbgprint.h"
#include "proxy/varpack/ppptrpack.h"	// others are included
#include "proxy/varpack/ppfunpack.h"

#include <string.h>

//void PPAppReceiver::switchFn() {
//}

PPClsPack* PPAppReceiver::proxy_hello(PPClsPack& args) {
	dbgPrint(0, "calling proxyhello");

 	int* iArg = (int*)args(0).getData();
	dbgPrint(0, "data=%d", *iArg);
	dumpCharArray((char*)iArg);
	dumpCharArray(args(0).getData());
	*iArg *= -1;

	PPClsPack argRet(1);
	argRet(0) <<= PPFunPack(4, (char*)iArg);

	sendFn(argRet);
	dbgPrint(0, "proxyhello ended");
}

PPAppReceiver::PPAppReceiver(PCPlugEntity* ent)
{
	coreEnt = ent;

	// switching loop
	char* fnName; PPClsPack* args;
	receiveFn(fnName, args);

	if(!strcmp("hello", fnName))
		proxy_hello(*args);

	else
		dbgPrint(2, "PPAppReceiver::PPAppReceiver unknown function");
}

void PPAppReceiver::receiveFn(char*& fnName, PPClsPack*& args) {
	char* data; int dataLen;
	opId = coreEnt->rreceive(data, dataLen);

	PPCallPack pak;
	pak.decompressInfo(data);

	fnName = pak.getFuncName();
	args = recvArgs = pak.getArgs();
}

void PPAppReceiver::sendFn(PPClsPack& args) {
	PPCallPack pak(&args, "");
	char* data; int dataLen;
	dataLen = pak.compress(data);

	coreEnt->rsend(data, dataLen, opId);
}


PPAppReceiver::~PPAppReceiver()
{
}

extern "C" void mainLoop(PCPlugEntity* ent) {
	dbgPrint(0,"hello from mainloop");
	PPAppReceiver recv(ent);
}
