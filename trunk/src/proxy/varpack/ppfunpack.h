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

#include "ppvarpack.h"

/**
	@author Štefan Sakalík <rabbit64@users.berlios.de>
*/
class PPFunPack : public PPVarPack
{
public:
    PPFunPack();
	PPFunPack(int iSize);
	PPFunPack(int iSize, char* dataPtr);

    ~PPFunPack();

	int getCompressSize();
	int compress(char* dataPtr);

// <Functions common for PPVarPack and PPClsPack>, not-implemented errors
	//get item operators
	PPVarPack*& getItemRef(const int nIndex = -1);		// get/set, because of reference
	PPVarPack*& operator[] (const int nIndex);			// getItemRef alias, (softlink)

	// add(set) item operators
	PPVarPack& operator<<  (const PPVarPack& entry);	// 'deref' getItem(x, -1) (hardlink)
	PPVarPack& operator<<= (const PPVarPack& entry);	// exactly the same as <<, but right-to-left evaluation
	PPVarPack& operator() (const int nIndex);			// if has node: return node; else setCurrentItem(nIndex), return this. Two purposes
// </Functions...>

	// only with FunPack
	void getData(char*& data, int& dataLen);
	void setData(char* dataPtr, int dataLen);

	// -- decompression --
	int decompress(int iSize, char* data);
	int decompressInfo(int iSize, char* data);

	// convenience
	PPVarPack* clone() const;
};

#endif
