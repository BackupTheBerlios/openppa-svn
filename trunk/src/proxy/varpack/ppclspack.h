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

#include "ppvarpack.h"
#include "misc/dbgprint.h"

/**
	@author Štefan Sakalík <rabbit64@users.berlios.de>
*/
class PPClsPack : public PPVarPack
{
public:
    PPClsPack();
	PPClsPack(int iSize);
    ~PPClsPack();

	int getCompressSize();
	int compress(char* dataPtr);
	void addNode(PPVarPack* vPack);
	PPVarPack*& getItem (const int nIndex);		// ++ decompression

	void setData(char* dataPtr, int dataLen);
	void setSize(int iSize);

	// -- decompression --
	int decompress(int iSize, char* data);
	int decompressInfo(int iSize, char* data);
	void getData(char*& data, int& dataLen);

	// convenience
	PPVarPack* clone() const;

	// chain operators
	PPClsPack& operator() (int nIndex);
	PPClsPack& operator<<= (const PPVarPack& entry);
};

#endif
