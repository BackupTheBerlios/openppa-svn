//
// C++ Implementation: pcplugentity
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "misc/dbgprint.h"
#include "core/pcpacketcenter.h"
#include "pcplugentity.h"
#include <dlfcn.h>

PCPlugEntity::PCPlugEntity(char* execPath, char* proxyPath, char* pluginPath)
{
	opIdArrayLength = 0;
	bIsPlugCreator = true;

	pipeCtrl = new PCPipeCtrl();
	pipeCtrl -> setAutoDel(true);

	procCtrl = new PCProcCtrl();
	procCtrl->newProcess(execPath, pipeCtrl->getInPipe(), pipeCtrl->getOutPipe(), proxyPath, pluginPath);

dbgPrint(0,"hello");

	pipeCtrl->openWritePipe();

dbgPrint(0,"hello2");
}

PCPlugEntity::PCPlugEntity(char* pipeIn, char* pipeOut)
{
	bIsPlugCreator = false;

	pipeCtrl = new PCPipeCtrl(pipeIn, pipeOut);
	pipeCtrl->openWritePipe();
	
}

int PCPlugEntity::send(char* data, int dataLen) {
	char* headerData; int headerLen;
	int opId = genOpId();

	PCPacketCenter::createHeader(0, opId, dataLen, headerData, headerLen);
	pipeCtrl->send(headerData, headerLen);
	pipeCtrl->send(data, dataLen);

	delete [] headerData;

	return opId;
}

// todo buffering, opid handling
void PCPlugEntity::receive(int opId, char*& data, int& dataLen) {
	char* headerData = new char[PCPacketCenter::getHeaderLength()];
	pipeCtrl->receive(headerData, PCPacketCenter::getHeaderLength());

	int packetType, recvOpId;
	// WARNING: data points to UNUSABLE memory block
	PCPacketCenter::parseHeader(headerData, packetType, recvOpId, data, dataLen);

	if(opId != recvOpId) {	// todo
		dbgPrint(2,"IMPL ERROR: opId mismatch / receive");
	}
	delOpId(recvOpId);

	pipeCtrl->receive(data, dataLen);

	delete [] headerData;
}

int PCPlugEntity::rreceive(char*& data, int& dataLen) {
	char* headerData = new char[PCPacketCenter::getHeaderLength()];
	pipeCtrl->receive(headerData, PCPacketCenter::getHeaderLength());

	int packetType, opId;
	// WARNING: data points to UNUSABLE memory block
	PCPacketCenter::parseHeader(headerData, packetType, opId, data, dataLen);
	addOpId(opId);
	pipeCtrl->receive(data, dataLen);

	delete [] headerData;
}

void PCPlugEntity::rsend(char* data, int dataLen, int opId) {
	delOpId(opId);

	char* headerData; int headerLen;

	PCPacketCenter::createHeader(0, opId, dataLen, headerData, headerLen);
	pipeCtrl->send(headerData, headerLen);
	pipeCtrl->send(data, dataLen);

	delete [] headerData;
}

void PCPlugEntity::addOpId(int opId) {
	if(opIdArrayLength)
		dbgPrint(2,"IMPL ERROR: opId >0");

	opIdArrayLength = 1;
	opIdArray[0] = opId;
}

int PCPlugEntity::genOpId() {
	if(opIdArrayLength)
		dbgPrint(2,"IMPL ERROR: opId >0");

	opIdArrayLength = 1; 
	opIdArray[0] = 1;
	return 1;
}

void PCPlugEntity::delOpId(int opId) {
	if(opIdArrayLength > 1)
		dbgPrint(2,"IMPL ERROR: opId >1");

	if(opId != opIdArray[0])
		dbgPrint(2,"PCPlugEntity::delOpId opId not found");

	opIdArrayLength = 0;
}

PCPlugEntity::~PCPlugEntity()
{
	if(bIsPlugCreator) {
		pipeCtrl->setAutoDel(true);
		//send("qu(PCDELROUTINE)\n");
	}

	delete pipeCtrl;
}

#include <stdio.h>

extern "C" void PCPlugEntityRSend(PCPlugEntity* ent, char* data, int dataLen, int opId) {
	ent->rsend(data, dataLen, opId);
}

extern "C" int PCPlugEntityRReceive(PCPlugEntity* ent, char*& data, int& dataLen) {
	return ent->rreceive(data, dataLen);
}


extern "C" int PCPlugEntitySend(PCPlugEntity* ent, char* data, int dataLen) {
	return ent->send(data, dataLen);
}

extern "C" void PCPlugEntityReceive(PCPlugEntity* ent, int opId, char*& data, int& dataLen) {
	ent->receive(opId, data, dataLen);
}

extern "C" PCPlugEntity* createPlugin(char* proxyPath, char* plugPath) {
	char execPath[] = EXEC_PATH; 
	return new PCPlugEntity(execPath, proxyPath, plugPath);
}

int main(int argc, char ** argv)
{
	if(argc != 5) {
		printf("argc != 5, quitting \n");
		return 1;
	}
	else {
		dbgPrint(0,"Successful init: %s %s", argv[1],argv[2]);
	}
	
	PCPlugEntity ent(argv[1],argv[2]);

	void* proxyLibHandle = dlopen(argv[3], RTLD_NOW);
	if(!proxyLibHandle)
		dbgPrint(2,"core::main can't open proxy lib, %s", argv[3]);

	typedef void (*CoreEntRSend) (PCPlugEntity* ent, char* data, int dataLen, int opId);
	typedef int (*CoreEntRReceive) (PCPlugEntity* ent, char*& data, int& dataLen);
	typedef void (*MainLoopType) (PCPlugEntity* ent, CoreEntRSend rsendFn, CoreEntRReceive rreceiveFn);

	MainLoopType mainLoopPtr = (MainLoopType)dlsym(proxyLibHandle, "mainLoop");
	if(!mainLoopPtr)
		dbgPrint(2,"core::main can't find mainLoopPtr symbol");

	mainLoopPtr(&ent, PCPlugEntityRSend, PCPlugEntityRReceive);
	
/*
	pthread_t thrId;
	if(pthread_create(&thrId, NULL, thrRoutine, pcn)) {
		dbgPrint(4, "Thread error");
	}

	pthread_join(thrId, NULL);
*/
	return 0;
}
