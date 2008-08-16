//
// C++ Interface: ppvarpack
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PPVARPACK_H
#define PPVARPACK_H

#define FUN_ID 0
#define PTR_ID 1
#define CLS_ID 2

/**
	@author Štefan Sakalík <rabbit64@users.berlios.de>
*/

class PPVarPack{
public:
	PPVarPack();
	PPVarPack(int iSize);

	~PPVarPack();

	virtual int getCompressSize() =0;
	virtual int compress(char* dataPtr) =0;

// <Functions common for PPVarPack and PPClsPack>
	//get item operators
	virtual PPVarPack*& getItemRef(const int nIndex = -1);		// get/set, because of reference
	virtual PPVarPack*& operator[] (const int nIndex);			// getItemRef alias, (softlink)

	// add(set) item operators
	virtual PPVarPack& operator<<  (const PPVarPack& entry);	// 'deref' getItem(x, -1) (hardlink)
	virtual PPVarPack& operator<<= (const PPVarPack& entry);	// exactly the same as <<, but right-to-left evaluation
	virtual PPVarPack& operator() (const int nIndex);			// if has node: return node; else setCurrentItem(nIndex), return this. Two purposes
// </Functions...>

	// only with FunPack
	char* getData();
	char* getData(int& dataLen);
	virtual void getData(char*& data, int& dataLen) =0;
	virtual void setData(char* dataPtr, int dataLen) =0;

	// -- decompression --
	virtual int decompress(int iSize, char* data) =0;
	virtual int decompressInfo(int iSize, char* data) =0;

	// convenience
	virtual PPVarPack* clone() const =0;

protected:
	// variables
	PPVarPack* varPackArray[20];// more VarPacks
	char* data;    // or data. can use union
	int size, iCurrent;
};
PPVarPack* mkVarPack(char* dataPtr, int& size);
PPVarPack* mkVarPackInfo(char* dataPtr, int& size);

#endif
