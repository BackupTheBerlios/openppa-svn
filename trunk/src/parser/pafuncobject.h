//
// C++ Interface: pafuncobject
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PAFUNCOBJECT_H
#define PAFUNCOBJECT_H

#include <string>

#define ARGLIMIT 8

/**
	@author Štefan Sakalík <rabbit64@users.berlios.de>
*/
class PAFuncObject{
public:
    PAFuncObject();
    ~PAFuncObject();

    void setFuncName(char* cFnName);
	std::string getFuncName();
	void listArgs();

    void setNextArg(char* cArgName, char* cArg);
	void cleanupArgs();

	int getArgNum();
	void getArg(int iArgNum, const char *& cArgName, const char *& cArg);

    void setRet(char* cRet);
	std::string getRet();

private:
    std::string* strFuncName;

    std::string* strArgs[ARGLIMIT];
	std::string* strArgNames[ARGLIMIT];
    int iArgs;

    std::string* strRet;
};

#endif
