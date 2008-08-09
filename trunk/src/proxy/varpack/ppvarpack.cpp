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
{
}


PPVarPack::~PPVarPack()
{
}

// TODO: Memory Leaks
PPVarPack* mkVarPack(char* dataPtr, int& size) {
	int iType = (int)dataPtr[0];
	int iSize;

	cvtIntFrom2B(&iSize, dataPtr + 1);
	
	switch(iType) {
	case PTR_ID: {
		PPPtrPack* pak = new PPPtrPack;
		size = pak->decompress(iSize, dataPtr+3);
		
		return pak; }
		
	case CLS_ID: {
		PPClsPack* pak = new PPClsPack;
		size = pak->decompress(iSize, dataPtr+3);
		
		return pak; }
		
	case FUN_ID:{
		PPFunPack* pak = new PPFunPack;
		size = pak->decompress(iSize, dataPtr+3);
		
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
