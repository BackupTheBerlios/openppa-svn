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
{
	size = 0;
}

PPClsPack::~PPClsPack()
{
}

int PPClsPack::getCompressSize() {
	int ttl = 3;

	for(int i=0; i < size; i++) {
		ttl += this[i].getCompressSize;
	}
	return ttl;
}

int PPClsPack::compress(char* dataPtr) {
	int ttl = 0;

	dataPtr[0] = CLS_ID;
	cvtIntTo2B(size,dataPtr + 1);
	dataPtr += 3;
	ttl += 3;

	for(int i=0; i < size; i++) {
		int outSize = this[i].compress(dataPtr);
		dataPtr += outSize;
		ttl + = outSize;
	}

	return ttl;
}

void PPClsPack::addNode(VarPack& vPack) {
	dbgPrint(2, "PPClsPack::addNode not implemented");
}

VarPack& PPClsPack::operator[] (const int nIndex) {		// ++ decompression
	return varPackArray[nIndex];
}

void PPClsPack::setData(char* dataPtr, int dataLen) {
	dbgPrint(2, "PPClsPack::setData not implemented");
}

// -- decompression --
int PPClsPack::decompress(int iSize, char* data) {
	int ttl;

	size = iSize;
	for(int i=0; i < iSize; i++) {
		int nextSize;
		this[i] = &mkVarPack(data, nextSize);

		data += nextSize;
		ttl += nextSize;
	}

	this.data = data;
	return ttl;
}

void PPClsPack::getData(char*& data, int& dataLen) {
	dbgPrint(2, "PPClsPack::getData not implemented");
}