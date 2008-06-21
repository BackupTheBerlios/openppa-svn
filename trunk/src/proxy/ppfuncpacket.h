#ifndef PPFUNCPACKET_H_
#define PPFUNCPACKET_H_

class PPFuncPacket {
public:
	PPFuncPacket(char* data);
	PPFuncPacket(int iParams, char* cFuncName);
	~PPFuncPacket();

	void addParam(void* data, int iLen);
	int getParam(int iParamNum, void* &cParam);

	int getData(char* &data);
	char* cDataPack;

private:
	struct paramPack {
		int iLen;
		char* cData;
	};

	struct funcPack {
		int iParams;
		paramPack* params;
	};

	int iTtlBytes;

	char* cFuncName;
	int iCurParam;

	funcPack fPack;
};

#endif /* PPFUNCPACKET_H_ */
