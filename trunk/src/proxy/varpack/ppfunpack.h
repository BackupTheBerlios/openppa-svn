//
// C++ Interface: ppfunpack
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PPFUNPACK_H
#define PPFUNPACK_H

#include <ppvarpack.h>

/**
	@author Štefan Sakalík <rabbit64@users.berlios.de>
*/
class PPFunPack : public PPVarPack
{
public:
    PPFunPack();
    ~PPFunPack();

	int getCompressSize();
	int compress(char* dataPtr);
	void addNode(VarPack& vPack);
	VarPack& operator[] (const int nIndex);
	void setData(char* dataPtr, int dataLen);

	// -- decompression --
	int decompress(int iSize, char* data);
	void getData(OU: data);
};

#endif
