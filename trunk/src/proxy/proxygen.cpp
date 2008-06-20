#include "proxy/proxygen.h"

#include "proxy/ppfunccenter.h"
#include "proxy/ppfuncpacket.h"
#include "misc/dbgprint.h"

#include <string.h>

int printHello() {
	PPFuncPacket callPacket(0,"printHello");
	char* dat;
	int iDatLen;

	iDatLen = callPacket.getData(dat);

	dbgPrint(0,"RECONSTRUCT***:%s",&dat[4]);

	PPFuncCenter::sendFunc(dat, iDatLen);
	PPFuncCenter::destroyPC();

	callPacket.dummy();

        return 0;
}

//! Dummy function
void recvFnCall(char* data) {}
