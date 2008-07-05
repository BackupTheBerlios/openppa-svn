#ifndef PARSEMAIN_H_
#define PARSEMAIN_H_

#include "parser/pafuncblock.h"

struct FnObj {
	char* cFnName;
	char * cRet;
	char ** cArgs;
};

bool bParInFunc = false;

int main(int argv, char ** argc);
void start(void *data, const char *el, const char **attr);
void end(void *data, const char *el);
void fnHandler(const char **attr);
void fnArgHandler(const char **attr);
void argHandler(const char *cSpec, const char **attr);

PAFuncBlock fnBlock;

#endif /* PARSEMAIN_H_ */
