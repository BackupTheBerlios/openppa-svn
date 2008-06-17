// PL->CL api functions
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "core/pcctrlnode.h"

extern char ** environ;		//?
// TODO: ITOA 16char, file names
// TODO: pipe handling specifics, IO!!
// TODO: platform independence

//-----------------------
// PCCtrlNode: PL comunicates with CL through this, exec too
//-----------------------

// called when lib
PCCtrlNode::PCCtrlNode() {
	bProcessed = true;
	bParent = true;

	dbgPrint("initPPAExec called\n");
	pipeCtrl = new PCPipeCtrl();
	pipeCtrl -> setAutoDel(true);

	procCtrl = new PCProcCtrl();
	procCtrl->newProcess("./PPACoreExec", pipeCtrl->getInPipe(), pipeCtrl->getOutPipe());

	pipeCtrl->openWritePipe();

	sendLoop();
}

// called when exec
PCCtrlNode::PCCtrlNode(char * input, char * output) {
	bProcessed = true;
	bParent = false;

	pipeCtrl = new PCPipeCtrl(input, output);
	pipeCtrl->openWritePipe();
}

PCCtrlNode::~PCCtrlNode() {
	if(bParent) {
		pipeCtrl->setAutoDel(true);
		procCtrl->waitChildEnd();
	}

	delete pipeCtrl;
}


// ---- FOR TESTING ----

//-----------------------
// Lib, Exec loop handlers
//-----------------------

void PCCtrlNode::sendLoop() {
	while(true){
		printf("\nenter your junk: ");
		gets(cOutBuffer);
		pipeCtrl->send(cOutBuffer,strlen(cOutBuffer) + 1);

		if(strlen(cOutBuffer) >= 2)
			if((cOutBuffer[0] == 'q') && (cOutBuffer[1] == 'u'))
				break;
	}			
}

void PCCtrlNode::execHandler() {
	while(true) {
		iInBuffer = pipeCtrl->thr_receive(cInBuffer);
		if(iInBuffer > 2) {
			printf("<THRINP:%s>\n", cInBuffer);

			if((cInBuffer[0] == 'q') && (cInBuffer[1] == 'u'))
				break;
		}
	}	
}


//-----------------------
// debugging functions
//-----------------------

void PCCtrlNode::dbgPrint(char * stuff) {
	return;
	printf(stuff);
}


//-----------------------
// PPACore exec section, threading proxy
//-----------------------

int main(int argc, char ** argv)
{
	if(argc != 3) {
		printf("argc != 3, quitting \n");
		return 1;
	}
	else {
		printf("Successful init: %s %s \n", argv[1],argv[2]);
	}
	
	PCCtrlNode * pcn = new PCCtrlNode(argv[1],argv[2]);

	pthread_t thrId;
	if(pthread_create(&thrId, NULL, thrRoutine, pcn)) {
		printf("error!!!\n");
	}
	else
		printf("pthread created\n");

	pthread_join(thrId, NULL);

	return 0;
}

void *thrRoutine(void* arg) {
	printf("hello me iz tread\ntreadyaay\nsumstuf\n");
	((PCCtrlNode*)arg)->execHandler();
	printf("gtthreadterm\n");
	delete ((PCCtrlNode*)arg);
	pthread_exit(NULL);
}
