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

PPAppReceiver::PPAppReceiver(PCPlugEntity* ent)
{
	coreEnt = ent;

	char* data; int dataLen;
	ent->rreceive(data, dataLen);
	dbgPrint(0, "Data received: %s", data);
}


PPAppReceiver::~PPAppReceiver()
{
}

extern "C" void mainLoop(PCPlugEntity* ent) {
	dbgPrint(0,"hello from mainloop");
	PPAppReceiver recv(ent);
}
