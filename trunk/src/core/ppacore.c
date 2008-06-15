// PL->CL api functions
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ppacore.h"

extern char ** environ;
// TODO: ITOA 16char, file names
char cPid[16];
char cFile[20];

//-----------------------
// Pipe management section
//-----------------------

void createPipe() {

	sprintf(cFile,"%s%x","ppa0",getpid());
//	itoa(getpid(),cPid, 16);
//	strcpy(cFile, "ppaO");
//	strcat(cFile,cPid);
	mkfifo(cFile,0600);
}

//-----------------------
// PPACore lib section
//-----------------------

int initPPAExec()
{
	printf("initPPAExec called\n");
	createPipe();

	int ret = fork();
	if(!ret)
		execl("./PPACoreExec", "PPACoreExec", "stuff", 0);
	else
		printf("exec ended\n");

	return 0;
}

//-----------------------
// PPACore exec section
//-----------------------

int main(int argc, char ** argv)
{
	if(argc != 2) {
		printf("argc != 2, quitting \n");
		return 1;
	}
	else {
		printf("Successful init: %s \n", argv[1]);
	}
return 0;
}
