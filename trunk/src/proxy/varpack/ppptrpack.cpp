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
{ size = 1; }		// by default 1, when generating code (?)

PPPtrPack::PPPtrPack(int iSize)
 : PPVarPack(iSize)
{}

PPPtrPack::~PPPtrPack()
{
}

int PPPtrPack::getCompressSize() {
	int ttl = 3;

	for(int i=0; i < size; i++) {
		ttl += (*this)[i]->getCompressSize();
	}
	return ttl;
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
	if(iCurrent >= size)
		dbgPrint(2, "PPPtrPack::addNode size exceeded");

	(*this)[iCurrent] = vPack;
	iCurrent++;
}

PPPtrPack& PPPtrPack::operator<< (const PPVarPack& rightArg) {
	this->addNode(rightArg.clone());
	return *this;
}

PPVarPack*& PPPtrPack::getItem (const int nIndex) {		// + decompression
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

PPVarPack* PPPtrPack::clone() const{
	return new PPPtrPack(*this);
}
