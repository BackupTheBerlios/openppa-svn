/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

// PL->CL api functions
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "core/pcctrlnode.h"
#include "proxy/proxyexecgen.h"

extern char ** environ;		//?
// TODO: ITOA 16char, file names
// TODO: pipe handling specifics, IO!!
// TODO: platform independence

//-----------------------
// PCCtrlNode: PL comunicates with CL through this, exec too
//-----------------------

/*!
 * 	\class PCCtrlNode
 * 	\brief Main CL class, CL<->PL communication
 *
 * 	Only this class is used for communication with PL. 
 * 	TODO: Communication protocol (new class)
 */

/*!
 * 	\brief Called from PL
 * 	It automatically creates pipes, spawns new process and initiates communication with it.
 * 	TODO: Only parameter is executable name.
 * 	TODO: strExec, absolute/relative path?, name stripping
 */
PCCtrlNode::PCCtrlNode(char* strExec) {
	bProcessed = true;
	bParent = true;

	dbgPrint(0,"initPPAExec called");
	pipeCtrl = new PCPipeCtrl();
	pipeCtrl -> setAutoDel(true);

	procCtrl = new PCProcCtrl();
	procCtrl->newProcess(strExec, pipeCtrl->getInPipe(), pipeCtrl->getOutPipe());

	pipeCtrl->openWritePipe();

	//sendLoop();
}

/*!
 *	\brief Called from executable, communication for spawned process
 *	It takes parameters input and output pipe, initiates communication.
 */
PCCtrlNode::PCCtrlNode(char * input, char * output) {
	bProcessed = true;
	bParent = false;

	pipeCtrl = new PCPipeCtrl(input, output);
	pipeCtrl->openWritePipe();
}

/*!
 * 	\brief Destroy pipes, end communication
 * 	\TODO: Tell child to terminate.
 */
PCCtrlNode::~PCCtrlNode() {
	if(bParent) {
		pipeCtrl->setAutoDel(true);
		send("qu(PCDELROUTINE)\n");
		usleep(500000);
//		procCtrl->waitChildEnd();
	}

	delete pipeCtrl;
}

//!	Send null-terminated string
void PCCtrlNode::send(char* string) {
	send(string,strlen(string));
}

//!	Send data of length iLen 
void PCCtrlNode::send(char* string, int iLen) {
	dbgPrint(0,"send data: [%d][%d], len=%d", string[0], string[1], iLen);
	pipeCtrl->send(string,iLen);
}

//!	Receive null-terminated string, allocates the string
//	TODO: receive length handling
void PCCtrlNode::receive(char* string) {
	int length = pipeCtrl->thr_receive(string);
}


//-----------------------
//----- FOR TESTING -----
//-----------------------

/*!
 * 	\brief For testing
 * 	Request input from user and send it through pipe
 */
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

/*!
 * 	\brief For testing
 *	(spawned exec) Wait for input, when requested to end, terminate child process
 */
void PCCtrlNode::execHandler() {
	dbgPrint(0,"execHandl");
	while(true) {
		dbgPrint(0,"recv>=%d",iInBuffer);
		iInBuffer = pipeCtrl->thr_receive(cInBuffer);
		dbgPrint(0,"recv<=%d",iInBuffer);
		if(iInBuffer > 2) {
			//dbgPrint(0, "<THRINP:%s>", cInBuffer);
			dbgPrint(0, "<THRINP:[%d][%d],%s>", cInBuffer[0], cInBuffer[1], cInBuffer);

			if((cInBuffer[0] == 'q') && (cInBuffer[1] == 'u'))
				break;

			// TODO: protocol controller, funcCtrlHandler
			recvFnCall(cInBuffer);
		}
	}	
}

//-----------------------
// PPACore exec section, threading proxy
//-----------------------

//! This is main routine when executed. Pass control to PCCtrlNode immediatelly
int main(int argc, char ** argv)
{
	if(argc != 3) {
		printf("argc != 3, quitting \n");
		return 1;
	}
	else {
		dbgPrint(0,"Successful init: %s %s \n", argv[1],argv[2]);
	}
	
	PCCtrlNode * pcn = new PCCtrlNode(argv[1],argv[2]);

	pthread_t thrId;
	if(pthread_create(&thrId, NULL, thrRoutine, pcn)) {
		dbgPrint(4, "Thread error");
	}
	else
		dbgPrint(0,"pthread created");

	pthread_join(thrId, NULL);

	return 0;
}

//! Threading routine, pass control to PCCtrlNode
void *thrRoutine(void* arg) {
	((PCCtrlNode*)arg)->execHandler();
	delete ((PCCtrlNode*)arg);
	pthread_exit(NULL);
}
