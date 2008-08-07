/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

#ifndef PCPROCCTRL_H_
#define PCPROCCTRL_H_

#include "misc/dbgprint.h"

class PCProcCtrl {
public:
	PCProcCtrl() {};
	~PCProcCtrl() {};

	void newProcess(char* cExecName, char* cInPipe, char* cOutPipe, char* cProxyPath, char* cPluginPath); 
	void waitChildEnd();
private:
	int iChildProc;
};

#endif /* PCPROCCTRL_H_ */
