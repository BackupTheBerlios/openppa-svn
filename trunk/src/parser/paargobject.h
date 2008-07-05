//
// C++ Interface: paargobject
//
// Description: 
//
//
// Author: pista <pista@pista-desktop>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

// = variable types; refactor?

#ifndef PAARGOBJECT_H
#define PAARGOBJECT_H

#include <string>

/**
	@author pista <pista@pista-desktop>
*/
class PAArgObject {
public:
    PAArgObject(char* cArgSpec, char* cArgId, char* cArgRef);
    ~PAArgObject();

	void setSize(int iSize);
	void setAlign(int iAlign);
private:
	std::string* strArgSpec;	// Pointer, Reference, FundamentalType
	std::string* strArgId;		// Id of the type
	std::string* strArgRef;		// (pointer to other), int, ...
	//argRef is reffered to as 'arg', 'argType'

	int iSize;					// in bits  (-1 = unused)
	int iAlign;					// in bits  (-1 = unused)
};

#endif
