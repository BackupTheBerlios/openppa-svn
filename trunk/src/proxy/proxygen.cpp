#include "proxy/proxygen.h"

#include "proxy/ppfunccenter.h"
#include "proxy/ppfuncpacket.h"
#include "misc/dbgprint.h"

#include <string.h>

int printHello() {
	PPFuncPacket callPacket(0,"printHello");
	dbgPrint(0,"<printHello>");
	PPFuncCenter::sendFunc(callPacket);
	dbgPrint(0,"</printHello>");

	char cRetData[128];
	dbgPrint(0,"querying data");
	int iRetLen = PPFuncCenter::receiveData(cRetData);
	dbgPrint(0,"querying data...complete");
	PPFuncPacket recvPacket(cRetData);

	char* retDat;
	int* iRetFnCall;
	recvPacket.getParam(1,(void*&)retDat);

	iRetFnCall = (int*)retDat;

	dbgPrint(0,"CALL RETURN == %d", *iRetFnCall);
	

	PPFuncCenter::destroyPC();
        return 0;
}

//! Dummy function
void recvFnCall(char* data) {}
