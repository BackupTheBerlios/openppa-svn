#include "proxy/ppfunccenter.h"
#include "misc/dbgprint.h"

#include <string.h>

bool iEnter = false;
PCCtrlNode* pcn = NULL;

//! Send compressed arglist to PC

void PPFuncCenter::sendFunc(PPFuncPacket &fnPack) {
	char* dat;
	int iDatLen;

	iDatLen = fnPack.getData(dat);

	checkInitPC();
	pcn->send(dat,iDatLen);
}

int PPFuncCenter::receiveData(char* data) {
	return pcn->receive(data);
}

//! Check-fix if PC is initialized
void PPFuncCenter::checkInitPC() {
	if(iEnter)		// already entered
		return;

	iEnter = true;
	pcn = new PCCtrlNode("./PPAExec");
	
}

void PPFuncCenter::setPC(PCCtrlNode* controlNode) {
	if(pcn != NULL)
		delete pcn;

	pcn = controlNode;
	iEnter = true;
}

void PPFuncCenter::destroyPC() {
	delete pcn;
}
