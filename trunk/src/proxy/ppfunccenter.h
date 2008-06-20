#ifndef PPFUNCCENTER_H_
#define PPFUNCCENTER_H_

// a function call: You call f

#include "core/pcctrlnode.h"


class PPFuncCenter {
public:
	PPFuncCenter() {};
	~PPFuncCenter() {};
	
	static void sendFunc(char* data, int iLen);
	static void checkInitPC();
	static void destroyPC();

};

#endif /* PPFUNCCENTER_H_ */
