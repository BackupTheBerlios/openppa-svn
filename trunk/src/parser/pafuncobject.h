//
// C++ Interface: pafuncobject
//
// Description: 
//
//
// Author: pista <pista@pista-desktop>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PAFUNCOBJECT_H
#define PAFUNCOBJECT_H

#include <string>

#define ARGLIMIT 8

/**
	@author pista <pista@pista-desktop>
*/
class PAFuncObject{
public:
    PAFuncObject();
    ~PAFuncObject();

    void setFuncName(char* cFnName);
	const char* getFuncName();
	void listArgs();

    void beginSetArg(char* cArgName, char* cArg);	// DEPRECATED
    void setNextArg(char* cArgName, char* cArg);

    void setRet(char* cRet);

private:
	void cleanupArgs();

    std::string* strFuncName;

    std::string* strArgs[ARGLIMIT];
	std::string* strArgNames[ARGLIMIT];

    int iArgs;
    std::string* strRet;
};

#endif
