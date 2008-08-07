#ifndef PARSEMAIN_H_
#define PARSEMAIN_H_

#include "parser/pafuncblock.h"
#include "parser/pafuncobject.h"
#include "parser/paargobject.h"

#include "proxygen/pgmain.h"

bool bParInFunc = false;

int main(int argv, char ** argc);
void start(void *data, const char *el, const char **attr);
void end(void *data, const char *el);
void fnHandler(const char **attr);
void fnArgHandler(const char **attr);
void argHandler(const char *cSpec, const char **attr);

PAFuncBlock fnBlock;
PAFuncObject *fnCurrent;

#endif /* PARSEMAIN_H_ */
