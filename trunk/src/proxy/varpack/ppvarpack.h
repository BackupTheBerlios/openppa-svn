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
	~PPVarPack();

	virtual int getCompressSize() =0;
	virtual int compress(char* dataPtr);
	virtual void addNode(VarPack& vPack);
	virtual VarPack& operator[] (const int nIndex);		// ++ decompression
	virtual void setData(char* dataPtr, int dataLen);

	// -- decompression --
	virtual int decompress(int iSize, char* data);
	virtual void getData(char*& data, int& dataLen);

private:
	// variables
	VarPack varPackArray[20];// more VarPacks
	void* data;    // or data. can use union
	int size;
};

#endif
