// PL->CL api functions
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "pcctrlnode.h"

extern char ** environ;
// TODO: ITOA 16char, file names
// TODO: pipe handling specifics, IO!!
// TODO: platform independence

//-----------------------
// PCCtrlNode: PL comunicates with CL through this, exec too
//-----------------------

// called when lib
PCCtrlNode::PCCtrlNode() {
	bProcessed = true;

	dbgPrint("initPPAExec called\n");
	createPipe();
	dbgPrint("<1>\n");
	forkExec();
	dbgPrint("<2>\n");

	connectPipe();
	dbgPrint("<3>\n");

	sendLoop();
}

void PCCtrlNode::sendLoop() {
	while(true){
		printf("enter your junk: ");
		scanf("%s", cOutBuffer);
		send(cOutBuffer,strlen(cOutBuffer) + 1);

		if(strlen(cOutBuffer) >= 2)
			if((cOutBuffer[0] == 'q') && (cOutBuffer[1] == 'u'))
				break;
	}			
}

void PCCtrlNode::execHandler() {
	while(true) {
		iInBuffer = thr_receive(cInBuffer);
		if(iInBuffer > 2)
			if((cInBuffer[0] == 'q') && (cInBuffer[1] == 'u'))
				break;
	}
}

// called when exec
PCCtrlNode::PCCtrlNode(char * input, char * output) {
	bProcessed = true;

	strcpy(cInPipe, input);
	strcpy(cOutPipe, output);

	dbgPrint("<2t>\n");
	connectPipe();
	
	close(pipeIn);
	close(pipeOut);

	unlink(cInPipe);
	unlink(cOutPipe);
}

PCCtrlNode::~PCCtrlNode() {
	close(pipeIn);
	close(pipeOut);
}

void PCCtrlNode::forkExec(){
	int ret = fork();
	if(!ret)// This is child
		// PPACore expects Input and Output in this order
		// it's vice-versa from PPACoreLib's point of view
		execl("./PPACoreExec", "PPACoreExec", cOutPipe, cInPipe, 0);

	else
		dbgPrint("PPACoreLib ended\n");
}

void PCCtrlNode::dbgPrint(char * stuff) {
	return;
	printf(stuff);
}


//-----------------------
// PPACore Pipe and IO section
//-----------------------


void PCCtrlNode::send(char * dat, int length) {
	write(pipeOut, dat, length);
}

int PCCtrlNode::thr_receive(char * dat) {
	int iBuffer;

	while(true) {
		usleep(10000);
		iBuffer = 128;
		iBuffer = read(pipeIn, dat, iBuffer);
		if(iBuffer == -1)
			continue;

		return iBuffer;
	}
}


void PCCtrlNode::createPipe() {
	// create communication channels
	sprintf(cInPipe,"%s%x","ppaI",getpid());
	sprintf(cOutPipe,"%s%x","ppaO",getpid());

	mkfifo(cInPipe,0600);
	mkfifo(cOutPipe,0600);
}

void PCCtrlNode::connectPipe() {
	pipeIn = open(cInPipe, O_RDONLY | O_NONBLOCK);
	pipeOut = open(cOutPipe, O_WRONLY | O_NONBLOCK);
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
if(	pthread_create(&thrId, NULL, thrRoutine, pcn)	) {
		printf("error!!!");
	}

	return 0;
}

void *thrRoutine(void* arg) {
	((PCCtrlNode*)arg)->execHandler();
	pthread_exit(NULL);
}
