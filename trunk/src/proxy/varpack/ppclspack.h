//
// C++ Interface: ppclspack
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PPCLSPACK_H
#define PPCLSPACK_H

#include <ppvarpack.h>

/**
	@author Štefan Sakalík <rabbit64@users.berlios.de>
*/
class PPClsPack : public PPVarPack
{
public:
    PPClsPack();
    ~PPClsPack();

	int getCompressSize();
	int compress(char* dataPtr);
	void addNode(VarPack& vPack);
	VarPack& operator[] (const int nIndex);		// ++ decompression
	void setData(char* dataPtr, int dataLen);

	// -- decompression --
	int decompress(int iSize, char* data);
	void getData(char*& data, int& dataLen);
};

#endif
