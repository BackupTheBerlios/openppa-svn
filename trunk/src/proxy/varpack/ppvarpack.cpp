//
// C++ Implementation: ppvarpack
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ppvarpack.h"
#include "misc/dbgprint.h"

#include "proxy/varpack/ppclspack.h"
#include "proxy/varpack/ppptrpack.h"
#include "proxy/varpack/ppfunpack.h"

PPVarPack::PPVarPack()
{ iCurrent = 0; }

PPVarPack::PPVarPack(int iSize) 
{ iCurrent = 0; size = iSize; }

PPVarPack::~PPVarPack() {}


PPVarPack*& PPVarPack::getItemRef(const int nIndex) {
	if(nIndex == -1) {
		if(iCurrent >= size)
			dbgPrint(2, "PPVarPack::getItemRef current index out of range");

		return varPackArray[iCurrent++];

	} else {
		if(nIndex >= size)
			dbgPrint(2, "PPVarPack::getItemRef index out of range");

		return varPackArray[nIndex];
	}
}

PPVarPack*& PPVarPack::operator[] (const int nIndex) {
	return getItemRef(nIndex);
}

PPVarPack& PPVarPack::operator<<  (const PPVarPack& entry) {
	getItemRef() = entry.clone();
}

PPVarPack& PPVarPack::operator<<= (const PPVarPack& entry) {
	*this << entry;
}

PPVarPack& PPVarPack::operator() (const int nIndex) {
	iCurrent = nIndex;
	if(getItemRef(nIndex))
		return *getItemRef(nIndex);

	else
		return *this;
}

char* PPVarPack::getData() {
	char* retData; int dataLen;
	getData(retData, dataLen);
	return retData;
}

char* PPVarPack::getData(int& dataLen) {
	char* retData;
	getData(retData, dataLen);
	return retData;
}

// TODO: Memory Leaks
PPVarPack* mkVarPack(char* dataPtr, int& size) {
	int iType = 0;
	iType = (int)dataPtr[0];
	int iSize;
	PPVarPack* pak;

	cvtIntFrom2B(&iSize, dataPtr + 1);
	
	switch(iType) {
	case PTR_ID: {
		pak = new PPPtrPack;
		size = pak->decompress(iSize, dataPtr+3)	+3;
		
		return pak; }
		
	case CLS_ID: {
		pak = new PPClsPack;
		size = pak->decompress(iSize, dataPtr+3)	+3;
		
		return pak; }
		
	case FUN_ID:{
		pak = new PPFunPack;
		size = pak->decompress(iSize, dataPtr+3)	+3;
		
		return pak;}
	}
}

PPVarPack* mkVarPackInfo(char* dataPtr, int& size) {
	int iType = 0;
	iType = (int)dataPtr[0];
	int iSize = 0;
	PPVarPack* pak;

	cvtIntFrom2B(&iSize, dataPtr + 1);
	
	dumpCharArray(dataPtr);

	switch(iType) {
	case PTR_ID: {
		pak = new PPPtrPack;
		size = pak->decompressInfo(iSize, dataPtr+3);
		
		break; }
		
	case CLS_ID: {
		pak = new PPClsPack;
		size = pak->decompressInfo(iSize, dataPtr+3);
		
		break; }
		
	case FUN_ID:{
		pak = new PPFunPack;
		size = pak->decompressInfo(iSize, dataPtr+3);
		
		break;}

	default:
		dbgPrint(1, "mkVarPackInfo: unknown data type");
	}

	size += 3;
	return pak;
}
