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

	virtual void addItem(PPVarPack* vPack, int nIndex = -1) =0;
	//virtual PPVarPack& operator<<= (const PPVarPack& entry);	// 'deref' addItem(v alias (hardlink)

	virtual PPVarPack*& getItem(const int nIndex) =0;
	virtual PPVarPack*& operator[] (const int nIndex) =0;	// getItem alias, (softlink)
	virtual PPVarPack& operator() (const int nIndex) =0;	// 'deref' getItem alias   (hardlink), convenience

	virtual void setData(char* dataPtr, int dataLen) =0;

	// -- decompression --
	virtual int decompress(int iSize, char* data) =0;
	virtual int decompressInfo(int iSize, char* data) =0;
	virtual void getData(char*& data, int& dataLen) =0;

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
