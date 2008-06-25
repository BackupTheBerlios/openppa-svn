#include "proxy/proxyexecgen.h"
#include "proxy/ppfuncpacket.h"
#include "proxy/ppfunccenter.h"
#include "misc/dbgprint.h"
#include <string.h>

extern "C" int printHello();

void recvFnCall(char* data) {
	PPFuncPacket callPacket(data);

	char* cFuncName;
	callPacket.getParam(0, (void*&)cFuncName);

	if(!strcmp(cFuncName, "printHello")) {
		int iRet = printHello();
		PPFuncPacket callReturn(1,"printHello");
		callReturn.addParam(&iRet, sizeof(iRet));
		PPFuncCenter::sendFunc(callReturn);
	}
}
