#ifndef PPACORE_H_
#define PPACORE_H_

#include "core/pcpipectrl.h"
#include "core/pcprocctrl.h"

void *thrRoutine(void* arg);

class PCCtrlNode {
public:
	PCCtrlNode();
	PCCtrlNode(char * input, char * output);
	~PCCtrlNode();

	void execHandler();

private:
	PCPipeCtrl *pipeCtrl;
	PCProcCtrl *procCtrl;

	void forkExec();
	void dbgPrint(char * stuff);
	void sendLoop();


	int iInBuffer;
	char cInBuffer[128];
	bool bProcessed;
	bool bParent;

	int iOutBuffer;
	char cOutBuffer[128];
};

#endif /* PPACORE_H_ */
