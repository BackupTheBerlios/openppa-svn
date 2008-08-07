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

PPVarPack::PPVarPack()
{
}


PPVarPack::~PPVarPack()
{
}

VarPack& mkVarPack(char* dataPtr, int& size) {
	int iType = (int)data[0];
	int iSize;

	cvtIntFrom2B(&iSize, data + 1);
	
	switch(iType) {
		case PTR_ID:
			VarPack pak = PtrPack();
			size pak.decompress(iSize, data+3);
			
			return pak;
		
		case CLS_ID:
			VarPack pak = ClsPack();
			size = pak.decompress(iSize, data+3);
			
			return pak;
		
		case FUN_ID:
			VarPack pak = FunPack();
			size = pak.decompress(iSize, data+3);
			
			return pak;
	}
}


