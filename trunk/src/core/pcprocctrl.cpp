/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "core/pcprocctrl.h"

/*!
 * 	\class PCProcCtrl
 * 	\brief Process management
 *
 * 	TODO: platform dependencies
 */


//! Create child process, give it files
void PCProcCtrl::newProcess(char* cExecName, char* cInPipe, char* cOutPipe, char* cProxyPath, char* cPluginPath) {
	iChildProc = fork();
	if(!iChildProc) {// This is child
		// PPACore expects Input and Output in this order
		// it's vice-versa from PPACoreLib's point of view
		if(execl(cExecName, cExecName, cOutPipe, cInPipe, cProxyPath, cPluginPath,  0) == -1)
			dbgPrint(3,"Exec Fault: tried exec (%s, pipes: %s, %s)",cExecName,cInPipe,cOutPipe);

		else
			dbgPrint(0,"PPACoreLib ended\n");
	} else {	//this is parent
		usleep(100000);	
		dbgPrint(0, "parent cont.");
	}
}

//! TODO: Wait until child terminates (PARTIAL)
void PCProcCtrl::waitChildEnd() {
	waitpid(iChildProc, NULL, 0);
}
