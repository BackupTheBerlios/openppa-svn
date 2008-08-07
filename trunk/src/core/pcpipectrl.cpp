/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

#include "core/pcpipectrl.h"
#include "core/pcpacketcenter.h"
#include "misc/dbgprint.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
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

	int i=0;
	while(i < 128)
		cInBuffer[i++] = 0;
}

//! Constructor: Open (yet generated) pipes for reading
PCPipeCtrl::PCPipeCtrl(char * input, char * output) {
	bDel = false;

	setIOPipes(input, output);
	openReadPipe();
}

//! Destroy pipes, (and delete them if bDel)
PCPipeCtrl::~PCPipeCtrl() {
	dbgPrint(0,"PipeCtrl terminating");
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
	if(length > 128) {
		dbgPrint(4,"PipeCtrl:send -> too big packet");
		return;
	}

	if(!write(pipeOut, dat, length)) {
		dbgPrint(2,"PCPipeCtrl::send can't write to pipe");
	}

	dbgPrint(0,"<PipeCtrl>Sent %d bytes",length);
	dumpCharArray(dat);
}

void PCPipeCtrl::receive(char* data, int dataLen) {
	while(true) {
		usleep(10000);
		iInBuffer = read(pipeIn, data, dataLen);
		if(iInBuffer == -1)
			continue;

		dbgPrint(0,"<PipeCtrl>Received %d chars. Want %d",iInBuffer, dataLen);
		if(iInBuffer == 0) {
			pollfd fds;
			fds.fd=pipeIn;
			fds.events=POLLIN;

			poll(&fds, NULL, 1000);
			continue;
		}

		dumpCharArray(data);

		if(iInBuffer < dataLen) {		// continue read
			dataLen -= iInBuffer;		// "move tape forward"
			data += iInBuffer;
		} else return;					// all OK
	}
}

//! Receive, buffer = 128B, sleep = 10ms
int PCPipeCtrl::thr_receive(char * dat) {
	if(PCPacketCenter::testPacket(cInBuffer)){

		PCPacketCenter::readPacket(&iInBuffer, cInBuffer, dat);
		PCPacketCenter::shiftPacket(cInBuffer);

		dbgPrint(0,"<PipeCtrl>Received %d chars",iInBuffer);
		dumpCharArray(cInBuffer);
		return iInBuffer;
	}

	while(true) {
		usleep(10000);
		iInBuffer = 128;
		iInBuffer = read(pipeIn, cInBuffer, iInBuffer);
		if(iInBuffer == -1)
			continue;

		dbgPrint(0,"<PipeCtrl>Received %d chars",iInBuffer);
		dumpCharArray(cInBuffer);

		PCPacketCenter::readPacket(&iInBuffer, cInBuffer, dat);
		PCPacketCenter::shiftPacket(cInBuffer);

		return iInBuffer;
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

	mkfifo(cInPipe,0600);
	mkfifo(cOutPipe,0600);
}

//! Open read pipe
void PCPipeCtrl::openReadPipe() {
	dbgPrint(0, "Opening read pipe:%s", cInPipe);
	pipeIn = open(cInPipe, O_RDONLY | O_NONBLOCK);
	if(pipeIn == -1)
		dbgPrint(2, "ReadPipe creation error (%s)", cInPipe);
}

//! Open write pipe
//! Write pipe can be opened only when the same pipe was already opened for reading.
void PCPipeCtrl::openWritePipe() {
	usleep(100000);	
	dbgPrint(0, "Opening write pipe:%s", cOutPipe);
	pipeOut = open(cOutPipe, O_WRONLY | O_NONBLOCK);
	if(pipeOut == -1)
		dbgPrint(2, "WritePipe creation error (%s)", cOutPipe);
}