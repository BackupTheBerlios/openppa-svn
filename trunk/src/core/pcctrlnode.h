/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

#ifndef PCCTRLNODE_H_
#define PCCTRLNODE_H_

#include "core/pcpipectrl.h"
#include "core/pcprocctrl.h"

#include "misc/dbgprint.h"

void *thrRoutine(void* arg);

class PCCtrlNode {
public:
	PCCtrlNode(char* strExec);
	PCCtrlNode(char * input, char * output);
	~PCCtrlNode();

	void execHandler();
	void send(char* string);
	void receive(char* string);

private:
	PCPipeCtrl *pipeCtrl;
	PCProcCtrl *procCtrl;

	void forkExec();
	void sendLoop();


	int iInBuffer;
	char cInBuffer[128];
	bool bProcessed;
	bool bParent;

	int iOutBuffer;
	char cOutBuffer[128];
};

#endif /* PCCTRLNODE_H_ */
