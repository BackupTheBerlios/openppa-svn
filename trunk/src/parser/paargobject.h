//
// C++ Interface: paargobject
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

// = variable types; refactor?

#ifndef PAARGOBJECT_H
#define PAARGOBJECT_H

#include <string>

/**
	@author Štefan Sakalík <rabbit64@users.berlios.de>
*/
class PAArgObject {
public:
    PAArgObject(char* cArgSpec, char* cArgId, char* cArgRef);
    ~PAArgObject();

	void setSize(int iSize);
	void setAlign(int iAlign);

	std::string getArgSpec() { return *strArgSpec; }
	std::string getArgId() { return *strArgId; }
	std::string getArgRef() { return *strArgRef; }

private:
	std::string* strArgSpec;	// Pointer, Reference, FundamentalType
	std::string* strArgId;		// Id of the type
	std::string* strArgRef;		// (pointer to other), int, ...
	//argRef is reffered to as 'arg', 'argType'

	int iSize;					// in bits  (-1 = unused)
	int iAlign;					// in bits  (-1 = unused)
};

#endif
