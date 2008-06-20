
/*!
 * 	\brief print for debugging
 * 	Debugging print str
 * 	LVL = level
 * 	0: Informational
 * 	1: Warning
 * 	2: Error
 * 	3: Error (fatal)
 */

#include "misc/dbgprint.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void dbgPrint(int lvl, const char* str, ...) {
	char errStr[10];

	switch(lvl) {
		case 0:
			strcpy(errStr,"<info> ");
			break;

		case 1:
			strcpy(errStr,"<warn> ");
			break;

		case 2:
			strcpy(errStr,"<err> ");
			break;

		case 3:
		case 4:
			strcpy(errStr,"<***FATAL***> ");
		 	break;
	}

	printf(errStr);

	va_list ap;

	va_start(ap,str);
	vprintf(str,ap);
	va_end(ap);

	printf("\n");	
}
