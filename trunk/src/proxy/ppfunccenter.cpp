#include "proxy/ppfunccenter.h"
#include "misc/dbgprint.h"

#include <string.h>

bool iEnter = false;
PCCtrlNode* pcn;

//! Send compressed arglist to PC
void PPFuncCenter::sendFunc(char* data, int iLen) {
	checkInitPC();
	pcn->send(data,iLen);
}

//! Check-fix if PC is initialized
void PPFuncCenter::checkInitPC() {
	if(iEnter)		// already entered
		return;

	iEnter = true;
	pcn = new PCCtrlNode("./PPAExec");
	
}

void PPFuncCenter::destroyPC() {
	delete pcn;
}
