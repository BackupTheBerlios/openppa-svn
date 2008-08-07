// TODO: const char / char, get rid of typecasts
#include <stdio.h>
#include <expat.h>
#include <string>
#include "parser/parsemain.h"
#include "misc/dbgprint.h"


//#define BUFSIZ 8192
#define FNLISTSIZE 128
#define ARGSIZE 6

#ifdef XML_LARGE_SIZE
#if defined(XML_USE_MSC_EXTENSIONS) && _MSC_VER < 1400
#define XML_FMT_INT_MOD "I64"
#else
#define XML_FMT_INT_MOD "ll"
#endif
#else
#define XML_FMT_INT_MOD "l"
#endif


int Depth;

int main(int argv, char ** argc) {
  char buf[BUFSIZ];
  XML_Parser parser = XML_ParserCreate(NULL);
  int done;
  int depth = 0;

  XML_SetUserData(parser, &depth);
  XML_SetElementHandler(parser, start, end);

  do {
    int len = (int)fread(buf, 1, sizeof(buf), stdin);
    done = len < sizeof(buf);
    if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
      fprintf(stderr,
              "%s at line %" XML_FMT_INT_MOD "u\n",
              XML_ErrorString(XML_GetErrorCode(parser)),
              XML_GetCurrentLineNumber(parser));
      return 1;
    }
  } while (!done);
  XML_ParserFree(parser);

	PGMain pGen;
	char dat[128];
	//fseek(stdin,0,SEEK_END);
	//printf("input function:");
	//while(scanf("%s\n", dat) == 1);
	//printf("got=%s\n",dat);	
	//pGen.genCode(std::string(dat));
	pGen.genCode("dumpCharArray");

  return 0;
}

void start(void *data, const char *el, const char **attr) {
  int i;

//dbgPrint(0,"data=[%d,%d,%d,...]",((char*)data)[0],((char*)data)[1],((char*)data)[2]);

//  for (i = 0; i < Depth; i++)
//    printf("  ");

	// __builtin filter
	for (i = 0; attr[i]; i += 2) {
		if (!strcmp(attr[i],"name"))
			if(!strncmp(attr[i+1],"__builtin",9))
				return;
	}

	if(!strcmp(el,"Function") || !strcmp(el,"Method")) {
		if(bParInFunc)
			dbgPrint(2,"ParseMain::start spotted 'Function' when parsing body of 'Function'");

		bParInFunc = true;
		fnHandler(attr);
	}

	else if(!strcmp(el,"Argument")) {
		if(!bParInFunc){
			//dbgPrint(1,"ParseMain::start spotted 'Argument' not in body of 'Function', %s", attr[1]);
			return;}

		fnArgHandler(attr);
	}

	// if it is some kind of type specifier
	else if(!strcmp(el,"PointerType")     ||
		    !strcmp(el,"ReferenceType")   ||
	        !strcmp(el,"FundamentalType") ||
			!strcmp(el,"CvQualifiedType") ||
			!strcmp(el,"ArrayType")	) {
		argHandler(el, attr);
	}

//  Depth++;
}  /* End of start handler */

void end(void *data, const char *el) {
	if(bParInFunc)
		if(!strcmp(el,"Function") || !strcmp(el,"Method"))
			bParInFunc = false;

//	Depth--;
}  /* End of end handler */


void fnHandler(const char **attr) {
	const char* cFnName = NULL;
	const char* cRet = NULL;

	int i;

	for (i = 0; attr[i]; i += 2) {
		if(!strcmp(attr[i], "name"))
			cFnName = attr[i+1];
		
		else if(!strcmp(attr[i],"returns"))
			cRet = attr[i+1];
	}

	if(!cFnName || !cRet)
		dbgPrint(1,"ParseMain:fnHandler NULL function name or argument");

	PAFuncObject *fnObj = new PAFuncObject();
	fnCurrent = fnObj;

	fnObj->setFuncName((char*)cFnName);
	fnObj->setRet((char*)cRet);

	fnBlock.addFunc(fnObj);
}

void fnArgHandler(const char **attr) {
	const char* cArgName = NULL;
	const char* cArgType = NULL;
	int i;

	for (i = 0; attr[i]; i += 2) {
		if(!strcmp(attr[i], "name"))
			cArgName = attr[i+1];
		
		else if(!strcmp(attr[i],"type"))
			cArgType = attr[i+1];		// cArgType is abbreviated as cArg in other classes
	}

	if(!cArgName || !cArgType)
		dbgPrint(1,"ParseMain:fnArgHandler NULL parameter name or type");

	fnCurrent->setNextArg((char*)cArgName, (char*)cArgType);
}

void argHandler(const char *cSpec, const char **attr) {
	const char* cArgId = NULL;
	const char* cArgType = NULL;
	int i;

// search for "ID"
	for (i = 0; attr[i]; i += 2) {
		if(!strcmp(attr[i], "id")) {
			cArgId = attr[i+1];
			break; }
	}
	if(!attr[i])	// if "ID" not found
		dbgPrint(2,"ParseMain::argHandler: Id not found");


	// if this is _not_ FundamentalType 
	if(strcmp(cSpec,"FundamentalType")) {
		for (i=0; attr[i]; i += 2) {
			if(!strcmp(attr[i],"type")) {
				cArgType = attr[i+1];		// cArgType is abbreviated as cArg in other classes sometimes
				break; }
		}
	}	// if this is FundamentalType
	else {
		for (i=0; attr[i]; i+=2) {
			if(!strcmp(attr[i],"name")) {
				cArgType = attr[i+1];
				break; }
		}
	}

	if(!attr[i])	// type or name not found
		dbgPrint(2,"ParseMain::argHandler: Type not found");

	PAArgObject *argObj = new PAArgObject((char*)cSpec, (char*)cArgId, (char*)cArgType);

	fnBlock.addArg(argObj);
}
