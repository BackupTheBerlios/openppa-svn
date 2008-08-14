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

void PPClsPack::addNode(PPVarPack* vPack) {
	dbgPrint(2, "PPClsPack::addNode not implemented");
}


PPVarPack*& PPClsPack::getItem (const int nIndex) {		// ++ decompression
	return varPackArray[nIndex];
}

void PPClsPack::setData(char* dataPtr, int dataLen) {
	dbgPrint(2, "PPClsPack::setData not implemented");
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

void PPClsPack::getData(char*& data, int& dataLen) {
	dbgPrint(2, "PPClsPack::getData not implemented");
}

PPVarPack* PPClsPack::clone() const {
	return new PPClsPack(*this);
}

PPClsPack& PPClsPack::operator() (int nIndex) {
	if(nIndex >= size) 
		dbgPrint(1, "PPClsPack::operator() index out of range");

	iCurrent = nIndex;
	return *this;
}

PPClsPack& PPClsPack::operator<<= (const PPVarPack& entry) {
	if(iCurrent >= size) 
		dbgPrint(1, "PPClsPack::operator<<= operating on out-of-range index");

	if(varPackArray[iCurrent])
		varPackArray[iCurrent]->addNode(entry.clone());

	else
		varPackArray[iCurrent] = entry.clone();

	return *this;
}
