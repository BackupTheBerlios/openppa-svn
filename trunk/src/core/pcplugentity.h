//
// C++ Interface: pcplugentity
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PCPLUGENTITY_H
#define PCPLUGENTITY_H

#include "core/pcpipectrl.h"
#include "core/pcprocctrl.h"
#include "proxy/ppcoreplugbase.h"

/**
	@author Štefan Sakalík <rabbit64@users.berlios.de>
*/

#define EXEC_PATH "./PPACoreExec"

class PCPlugEntity : PPCorePlugBase{
public:
    PCPlugEntity(char* execPath, char* proxyPath, char* pluginPath);	// PPAProxy-App calls this
	PCPlugEntity(char* pipeIn, char* pipeOut);	// main calls this
	~PCPlugEntity();

	int send(char* data, int dataLen);
	void receive(int opId, char*& data, int& dataLen);

	int rreceive(char*& data, int& dataLen);
	void rsend(char* data , int dataLen, int opId);

private:
	// todo: opId handling
	void addOpId(int opId);
	int genOpId();
	void delOpId(int opId);

	int opIdArray[128];	
	int opIdArrayLength;
	
	PCPipeCtrl *pipeCtrl;		// IO abstraction:
	PCProcCtrl *procCtrl;		// Processes abstraction

	bool bIsPlugCreator;
};

#endif
