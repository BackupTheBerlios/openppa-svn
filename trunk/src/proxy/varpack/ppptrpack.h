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

#include "ppvarpack.h"

/**
	@author Štefan Sakalík <rabbit64@users.berlios.de>
*/
class PPPtrPack : public PPVarPack
{
public:
    PPPtrPack();
	PPPtrPack(int iSize);

    ~PPPtrPack();

	int getCompressSize();
	int compress(char* dataPtr);

	void setSize(int iSize);

	// -- decompression --
	int decompress(int iSize, char* data);
	int decompressInfo(int iSize, char* data);

	// FunPack: not implemented
	void getData(char*& data, int& dataLen);
	void setData(char* dataPtr, int dataLen);

	// convenience
	PPVarPack* clone() const;
};

#endif
