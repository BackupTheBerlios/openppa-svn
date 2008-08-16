//
// C++ Implementation: ppclspack
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ppclspack.h"

PPClsPack::PPClsPack()
 : PPVarPack()
{ size = 0; }

PPClsPack::PPClsPack(int iSize)
 : PPVarPack(iSize) {
	for(int i=0; i < iSize; i++)
		varPackArray[i] = 0;
}

PPClsPack::~PPClsPack()
{
}

int PPClsPack::getCompressSize() {
	int ttl = 3;

	for(int i=0; i < size; i++) {
		ttl += (*this)[i]->getCompressSize();
	}
	return ttl;
}

int PPClsPack::compress(char* dataPtr) {
	int ttl = 0;

	dataPtr[0] = (char)CLS_ID;
	cvtIntTo2B(&size,dataPtr + 1);

	dataPtr += 3;
	ttl += 3;

	if(size == 0)
		dbgPrint(2, "PPClsPack::compress size of class is 0");

	for(int i=0; i < size; i++) {
		int outSize = (*this)[i]->compress(dataPtr);
		dataPtr += outSize;
		ttl += outSize;
	}

	dbgPrint(0, "CLASS_COMPRESSION_COMPLETE");
	dumpCharArray(dataPtr - ttl);

	return ttl;
}

void PPClsPack::setSize(int iSize) {
	size = iSize;
	for(int i=0; i < iSize; i++)
		varPackArray[i] = 0;
}

// -- decompression --
int PPClsPack::decompress(int iSize, char* data) {
	int ttl;

	size = iSize;
	for(int i=0; i < iSize; i++) {
		int nextSize;
		
		(*this)[i] = mkVarPack(data, nextSize);

		data += nextSize;
		ttl += nextSize;
	}

	(*this).data = data;
	return ttl;
}

int PPClsPack::decompressInfo(int iSize, char* data) {
	dbgPrint(0, "PPClsPack array size=%d", iSize);
	int ttl=0;

	size = iSize;
	for(int i=0; i < iSize; i++) {
		int nextSize;
		
		(*this)[i] = mkVarPackInfo(data, nextSize);
		dumpCharArray(data);

		data += nextSize;
		ttl += nextSize;
	}

	(*this).data = data;
	return ttl;

}

PPVarPack* PPClsPack::clone() const {
	return new PPClsPack(*this);
}

void PPClsPack::getData(char*& dataPtr, int& dataLen) { dbgPrint(2,"PPClsPack::getData not implemented"); }
void PPClsPack::setData(char* dataPtr, int dataLen) { dbgPrint(2,"PPClsPack::setData not implemented"); }
