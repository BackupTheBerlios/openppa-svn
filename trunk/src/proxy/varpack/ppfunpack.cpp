//
// C++ Implementation: ppfunpack
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ppfunpack.h"
#include "misc/dbgprint.h"
#include <string.h>

PPFunPack::PPFunPack()
 : PPVarPack()
{ size = sizeof(int); }

PPFunPack::PPFunPack(int iSize)
 : PPVarPack(iSize)
{}

PPFunPack::PPFunPack(int iSize, char* dataPtr)
 : PPVarPack(iSize) { data = dataPtr; }

PPFunPack::~PPFunPack()
{
}

int PPFunPack::getCompressSize() {
	return 3 + size;
}

// assume preallocated
int PPFunPack::compress(char* dataPtr) {
	dataPtr[0] = FUN_ID;
	cvtIntTo2B(&size, dataPtr + 1);
	dbgPrint(0, "FUNPACK::MEMCPY:size %d", data, size);
	dumpCharArray(data);
	memcpy(dataPtr+3, data, size);

	return 3 + size;
}

int PPFunPack::decompress(int iSize, char* data) {
	size = iSize;
	this->data = data;
	return iSize;
}

int PPFunPack::decompressInfo(int iSize, char* data) {
	dbgPrint(0, "PPFunPack data size=%d", iSize);
	if(iSize == 0);
	else if( iSize == 1)
		dbgPrint(0, "PPFunPack data = [%d]", data[0]);

	else
		dbgPrint(0, "PPFunPack data = [%d, %d,...]", data[0], data[1]);

	size = iSize;
	this->data = data;
	return iSize;
}

void PPFunPack::getData(char*& data, int& dataLen) {
	data = this->data;
	dataLen = size;
}

void PPFunPack::setData(char* dataPtr, int dataLen) {
	data = dataPtr;
	size = dataLen;
}

PPVarPack* PPFunPack::clone() const {
	return new PPFunPack(*this);
}

// not imlemented / undefined
PPVarPack*& PPFunPack::getItemRef(const int nIndex) { dbgPrint(2,"PPFunPack::getItemRef not implemented"); }
PPVarPack*& PPFunPack::operator[] (const int nIndex) { dbgPrint(2,"PPFunPack::operator[] not implemented"); }

PPVarPack& PPFunPack::operator<<  (const PPVarPack& entry) { dbgPrint(2,"PPFunPack::operator<< not implemented"); }
PPVarPack& PPFunPack::operator<<= (const PPVarPack& entry) { dbgPrint(2,"PPFunPack::operator<<= not implemented"); }
PPVarPack& PPFunPack::operator() (const int nIndex) { dbgPrint(2,"PPFunPack::operator() not implemented"); }
