#include "proxy/ppfuncpacket.h"
#include "misc/dbgprint.h"
#include <string.h>

PPFuncPacket::PPFuncPacket(char* data) {
	cDataPack = data;

	int dataPtr = 0;
	short int iTmp;

	memcpy(&iTmp, data, 2); dataPtr+=2;
	dbgPrint(0,"RECEIVED NUMBER,str:::%d,%s",iTmp,&data[4]);

	fPack.iParams = iTmp;
	dbgPrint(0,"recv.data.numargs %d", iTmp);

	fPack.params = new paramPack[iTmp];


	paramPack *pPack = fPack.params;

	for(int i=0; i < fPack.iParams; i++) {
		memcpy(&iTmp,&data[dataPtr],2); dataPtr += 2;
		pPack[i].iLen = iTmp;
		dbgPrint(0,"recv.data.numargs %d", iTmp);
		pPack[i].cData = new char[iTmp];

		memcpy(pPack[i].cData, &data[dataPtr], iTmp); dataPtr += iTmp;
	}

	dbgPrint(0,"0Param=%d,%s",pPack[0].iLen,pPack[0].cData);


	cDataPack = NULL;	// autodelete prevention
}

PPFuncPacket::PPFuncPacket(int iParams, char* cFuncName) {
	iParams++;	// add function name to param
	iCurParam = 0;	// we've got the function name
	iTtlBytes = 0;
	cDataPack = NULL;

	fPack.iParams = iParams;
	fPack.params = new paramPack[iParams];

	short int iFnNameLen = strlen(cFuncName);

	(fPack.params)[0].iLen = iFnNameLen;
	(fPack.params)[0].cData = new char[iFnNameLen];
	memcpy((fPack.params)[0].cData, cFuncName, iFnNameLen);

	iTtlBytes += 2+iFnNameLen;
}

PPFuncPacket::~PPFuncPacket() {
	if(cDataPack != NULL)
		delete [] cDataPack;

	int iNum = fPack.iParams;
	paramPack* pPac = fPack.params;

	for(int i=0; i < iNum; i++) {
		int datLen = pPac[i].iLen;

		delete [] (pPac[i].cData);
	}

	delete [] pPac;
}

void PPFuncPacket::addVar(void* data, int iLen) {
	iTtlBytes += iLen+2;
	iCurParam++;
	if(iCurParam >= fPack.iParams){
		dbgPrint(3,"Too Big Param PPFuncPacket::addVar");
		return;
	}

	(fPack.params[iCurParam]).iLen = iLen;
	(fPack.params[iCurParam]).cData = new char[iLen];

	memcpy((fPack.params[iCurParam]).cData, data, iLen);
}

int PPFuncPacket::getData(char* &data) {	
	if(cDataPack != NULL)
		delete [] cDataPack;

	dbgPrint(0,"iLENGTH=%d",iTtlBytes);

	cDataPack = new char[iTtlBytes];

	int dataPtr = 0;
	
	short int iNum = fPack.iParams;
	dbgPrint(0,"getData(num):%d",iNum);

	paramPack* pPac = fPack.params;

	memcpy(cDataPack, &iNum, 2); dataPtr+=2;
	dbgPrint(0,"getData(num):%d",iNum);


	for(int i=0; i < iNum; i++) {
		if(dataPtr >= 100) {
			dbgPrint(3,"PPFuncCenter:sendFunc: >= 100!");
		}

		short int datLen = pPac[i].iLen;
	
		memcpy(&cDataPack[dataPtr], &datLen, 2); dataPtr+=2;
		memcpy(&cDataPack[dataPtr], pPac[i].cData, datLen); dataPtr+=datLen;
	}
	dbgPrint(0,"**********dataptr=%d",dataPtr);
	data = cDataPack;

	return dataPtr;
}
/*
int PPFuncPacket::cpIntChar(int iLen, char* data) {
#	memcpy(
}*/
