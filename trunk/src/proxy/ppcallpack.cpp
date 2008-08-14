//
// C++ Implementation: ppcallpack
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "ppcallpack.h"
#include "proxy/varpack/ppvarpack.h"
#include "string.h"

PPCallPack::PPCallPack() {}

PPCallPack::PPCallPack(PPClsPack* args, char* cFuncName)
{
	funcName = cFuncName;
	this->args = args;
}

PPCallPack::~PPCallPack()
{
}

PPVarPack*& PPCallPack::operator[] (const int nIndex) {
	return (*args)[nIndex];
}

int PPCallPack::compress(char*& data) {
	int dataLen = strlen(funcName) + args->getCompressSize() + 1;
	char* compressData = new char[dataLen];

	strcpy(compressData, funcName);
	dbgPrint(0,"ppcallpack compress data len = %d", dataLen);
	args->compress(compressData + strlen(funcName) + 1);
	//!! COPY FUNCTION NAME TO DATA
	data = compressData;

	return dataLen;
}

PPVarPack* decompress(char* data, int dataLen) {
	// dataLen dummy!
	int fnNameLen = strlen(data) + 1;
	int dataSize;
 
	PPVarPack* argsPack = mkVarPack(data + fnNameLen, dataSize);
	return argsPack;
}

PPVarPack* PPCallPack::decompressInfo(char* data) {
	int fnNameLen = strlen(data)+1;
	int dataSize;		// return of mkVarPack

	dbgPrint(0, "PPCallPack function: %s", data);
	PPVarPack* argsPack = mkVarPackInfo(data + fnNameLen, dataSize);
	return argsPack;
}
