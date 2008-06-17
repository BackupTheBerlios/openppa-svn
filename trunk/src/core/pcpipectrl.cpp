#include "core/pcpipectrl.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>

PCPipeCtrl::PCPipeCtrl() {
	bDel = false;

	createPipe();
	openReadPipe();
}

PCPipeCtrl::PCPipeCtrl(char * input, char * output) {
	bDel = false;

	setIOPipes(input, output);
	openReadPipe();
}

PCPipeCtrl::~PCPipeCtrl() {
	printf("PIPECTRL:TERMINATING\n");
	close(pipeIn);
	close(pipeOut);

	if(bDel){
		unlink(cInPipe);
		unlink(cOutPipe);
	}
}


void PCPipeCtrl::setIOPipes(char * cPInp, char * cPOut) {
	strcpy(cInPipe, cPInp);
	strcpy(cOutPipe, cPOut);
}

//-----------------------
// send / recv
//-----------------------

void PCPipeCtrl::send(char * dat, int length) {
	write(pipeOut, dat, length);
}

int PCPipeCtrl::thr_receive(char * dat) {
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


//-----------------------
// pipe creation / handling
//-----------------------

void PCPipeCtrl::createPipe() {
	// create communication channels
	sprintf(cInPipe,"%s%x","ppaI",getpid());
	sprintf(cOutPipe,"%s%x","ppaO",getpid());

	mkfifo(cInPipe,0600);
	mkfifo(cOutPipe,0600);
}

void PCPipeCtrl::openReadPipe() {
	pipeIn = open(cInPipe, O_RDONLY | O_NONBLOCK);
}

void PCPipeCtrl::openWritePipe() {
	pipeOut = open(cOutPipe, O_WRONLY);
}
