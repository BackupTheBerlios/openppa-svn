#ifndef DBGPRINT_H_
#define DBGPRINT_H_

void cvtIntTo2B(int* from, char* to);
void cvtIntFrom2B(int* to, char* from);
void dbgPrint(int lvl, const char* str, ...);
void dumpCharArray(char* cArray);

#endif /* DBGPRINT_H_ */
