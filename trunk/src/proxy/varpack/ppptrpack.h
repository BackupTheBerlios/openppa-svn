//
// C++ Interface: ppptrpack
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PPPTRPACK_H
#define PPPTRPACK_H

#include <ppvarpack.h>

/**
	@author Štefan Sakalík <rabbit64@users.berlios.de>
*/
class PPPtrPack : public PPVarPack
{
public:
    PPPtrPack();
    ~PPPtrPack();

	int getCompressSize();
	int compress(char* dataPtr);
	void addNode(VarPack& vPack);
	VarPack& operator[] (const int nIndex);		// + decompression
	void setData(char* dataPtr, int dataLen);

	// -- decompression --
	int decompress(int iSize, char* data);
	void getData(char*& data, int& dataLen);

private:
	
};

#endif
