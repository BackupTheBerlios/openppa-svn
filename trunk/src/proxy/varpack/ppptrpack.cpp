//
// C++ Implementation: ppptrpack
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ppptrpack.h"

PPPtrPack::PPPtrPack()
 : PPVarPack()
{
	size = 0;
}

PPPtrPack::~PPPtrPack()
{
}

int PPPtrPack::getCompressSize() {
	return 3 + size * this[0];	// all submembers same size
}


int PPPtrPack::compress(char* dataPtr) {
	int ttl = 0;

	dataPtr[0] = PTR_ID;
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

void PPPtrPack::addNode(VarPack& vPack) {
	this[size++] = vPack;
}

VarPack& PPPtrPack::operator[] (const int nIndex) {		// + decompression
	return varPackArray[nIndex];
}

void PPPtrPack::setData(char* dataPtr, int dataLen) {
	dbgPrint(2,"PPPtrPack::setData not implemented");
}

	// -- decompression --
int PPPtrPack::decompress(int iSize, char* data) {
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

void PPPtrPack::getData(char*& data, int& dataLen) {
}