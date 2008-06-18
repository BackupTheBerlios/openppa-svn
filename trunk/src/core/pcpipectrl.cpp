/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

#include "core/pcpipectrl.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
/*!
 * 	\class PCPipeCtrl
 * 	\brief Low-level pipe IO
 *
 * 	This class is used for creation/deletion and data transfer between processes.
 * 	TODO: Platform dependencies.
 */

//! Constructor: generate pipes, open for reading
PCPipeCtrl::PCPipeCtrl() {
	bDel = false;

	createPipe();
	openReadPipe();
}

//! Constructor: Open (yet generated) pipes for reading
PCPipeCtrl::PCPipeCtrl(char * input, char * output) {
	bDel = false;

	setIOPipes(input, output);
	openReadPipe();
}

//! Destroy pipes, (and delete them if bDel)
PCPipeCtrl::~PCPipeCtrl() {
	printf("PIPECTRL:TERMINATING\n");
	close(pipeIn);
	close(pipeOut);

	if(bDel){
		unlink(cInPipe);
		unlink(cOutPipe);
	}
}

//! Pipes were generated, set files
void PCPipeCtrl::setIOPipes(char * cPInp, char * cPOut) {
	strcpy(cInPipe, cPInp);
	strcpy(cOutPipe, cPOut);
}

//-----------------------
// send / recv
//-----------------------

//! Send data block
//! TODO: length of sent data
void PCPipeCtrl::send(char * dat, int length) {
	write(pipeOut, dat, length);
	//usleep(10000);
}

//! Receive, buffer = 128B, sleep = 10ms
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

//! Create pipe
void PCPipeCtrl::createPipe() {
	// create communication channels
	sprintf(cInPipe,"%s%x","ppaI",getpid());
	sprintf(cOutPipe,"%s%x","ppaO",getpid());

//	mkfifo(cInPipe,0600);
//	mkfifo(cOutPipe,0600);
	mknod(cInPipe,S_IFIFO | 0666,0);
	mknod(cOutPipe,S_IFIFO | 0666,0);
}

//! Open read pipe
void PCPipeCtrl::openReadPipe() {
	pipeIn = open(cInPipe, O_RDONLY | O_NONBLOCK);
}

//! Open write pipe
//! Write pipe can be opened only when the same pipe was already opened for reading.
void PCPipeCtrl::openWritePipe() {
	pipeOut = open(cOutPipe, O_WRONLY);
}
