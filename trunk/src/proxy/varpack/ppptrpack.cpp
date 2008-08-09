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
#include "misc/dbgprint.h"

PPPtrPack::PPPtrPack()
 : PPVarPack()
{
	size = 0;
}

PPPtrPack::~PPPtrPack()
{
}

int PPPtrPack::getCompressSize() {
	return 3 + size * (*this)[0]->getCompressSize();	// all submembers same size
}


int PPPtrPack::compress(char* dataPtr) {
	int ttl = 0;

	dataPtr[0] = PTR_ID;
	cvtIntTo2B(&size,dataPtr + 1);
	dataPtr += 3;
	ttl += 3;

	for(int i=0; i < size; i++) {
		int outSize = (*this)[i]->compress(dataPtr);
		dataPtr += outSize;
		ttl += outSize;
	}

	return ttl;
}

void PPPtrPack::addNode(PPVarPack* vPack) {
	(*this)[size++] = vPack;
}

PPVarPack*& PPPtrPack::operator[] (const int nIndex) {		// + decompression
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
		(*this)[i] = mkVarPack(data, nextSize);

		data += nextSize;
		ttl += nextSize;
	}

	this->data = data;
	return ttl;
}

int PPPtrPack::decompressInfo(int iSize, char* data) {
	dbgPrint(0, "PPPtrPack array size=%d", iSize);
	int ttl = 0;

	size = iSize;
	for(int i=0; i < iSize; i++) {
		int nextSize;
		(*this)[i] = mkVarPackInfo(data, nextSize);

		data += nextSize;
		ttl += nextSize;
	}

	this->data = data;
	dbgPrint(0, "PtrPack, returning size=%d", ttl);
	return ttl;
}

void PPPtrPack::getData(char*& data, int& dataLen) {
}
