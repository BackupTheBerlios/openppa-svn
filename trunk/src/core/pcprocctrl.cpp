#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "core/pcprocctrl.h"

void PCProcCtrl::newProcess(char* cExecName, char* cInPipe, char* cOutPipe) {
	iChildProc = fork();
	if(!iChildProc)// This is child
		// PPACore expects Input and Output in this order
		// it's vice-versa from PPACoreLib's point of view
		execl(cExecName, cExecName, cOutPipe, cInPipe, 0);

	else
		dbgPrint("PPACoreLib ended\n");
}

void PCProcCtrl::waitChildEnd() {
	waitpid(iChildProc, NULL, 0);
}

//-----------------------
// debugging functions
//-----------------------

void PCProcCtrl::dbgPrint(char * stuff) {
	return;
	printf(stuff);
}
