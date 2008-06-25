#include "proxy/proxygen.h"

#include "proxy/ppfunccenter.h"
#include "proxy/ppfuncpacket.h"
#include "misc/dbgprint.h"

#include <string.h>

int printHello() {
	PPFuncPacket callPacket(0,"printHello");
	PPFuncCenter::sendFunc(callPacket);

	char cRetData[128];
	int iRetLen = PPFuncCenter::receiveData(cRetData);
	PPFuncPacket recvPacket(cRetData);

	char* retDat;
	int* iRetFnCall;
	recvPacket.getParam(1,(void*&)retDat);

	iRetFnCall = (int*)retDat;

	dbgPrint(0,"First parameter(int cast) = %d", *iRetFnCall);	

	PPFuncCenter::destroyPC();
        return 0;
}

//! Dummy function
void recvFnCall(char* data) {}
