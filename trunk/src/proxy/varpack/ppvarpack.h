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
	virtual int compress(char* dataPtr) =0;
	virtual void addNode(PPVarPack* vPack) =0;
	virtual PPVarPack*& operator[] (const int nIndex) =0;		// ++ decompression
	virtual void setData(char* dataPtr, int dataLen) =0;

	// -- decompression --
	virtual int decompress(int iSize, char* data) =0;
	virtual int decompressInfo(int iSize, char* data) =0;
	virtual void getData(char*& data, int& dataLen) =0;

protected:
	// variables
	PPVarPack* varPackArray[20];// more VarPacks
	char* data;    // or data. can use union
	int size;
};
PPVarPack* mkVarPack(char* dataPtr, int& size);
PPVarPack* mkVarPackInfo(char* dataPtr, int& size);

#endif
