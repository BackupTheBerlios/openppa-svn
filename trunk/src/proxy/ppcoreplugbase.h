//
// C++ Interface: ppcoreplugbase
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PPCOREPLUGBASE_H
#define PPCOREPLUGBASE_H

/**
	@author Štefan Sakalík <rabbit64@users.berlios.de>
*/
class PPCorePlugBase{
public:
    PPCorePlugBase() {};
    virtual ~PPCorePlugBase() {};

	virtual int send(char* data, int dataLen) =0;
	virtual void receive(int opId, char*& data, int& dataLen) =0;

	virtual int rreceive(char*& data, int& dataLen) =0;
	virtual void rsend(char* data , int dataLen, int opId) =0;
};

#endif
