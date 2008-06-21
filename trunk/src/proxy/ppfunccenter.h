#ifndef PPFUNCCENTER_H_
#define PPFUNCCENTER_H_

// a function call: You call f

#include "core/pcctrlnode.h"
#include "proxy/ppfuncpacket.h"

class PPFuncCenter {
public:
	PPFuncCenter() {};
	~PPFuncCenter() {};
	
	static void sendFunc(PPFuncPacket &fnPack);
	static int receiveData(char* data);
	static void sendPack(PPFuncPacket &fpac);

	static void checkInitPC();
	static void setPC(PCCtrlNode* controlNode);
	static void destroyPC();

};

#endif /* PPFUNCCENTER_H_ */
