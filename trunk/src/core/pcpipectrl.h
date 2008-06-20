/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

#ifndef PCPIPECTRL_H_
#define PCPIPECTRL_H_

class PCPipeCtrl {
public:
	PCPipeCtrl();
	PCPipeCtrl(char * input, char * output);
	~PCPipeCtrl();

	void setAutoDel(bool del) { bDel = del;	}

	void send(char * dat, int length);
	int thr_receive(char * dat);

	char* getInPipe() { return cInPipe; }
	char* getOutPipe() { return cOutPipe; }
	void openWritePipe();

private:
	void createPipe();

	void openReadPipe();

	void setIOPipes(char * cPInp, char * cPOut);

	char cInPipe[20];
	char cOutPipe[20];

	int iInBuffer;
	char cInBuffer[128];

	int iOutBuffer;
	char cOutBuffer[128];

	bool bProcessed;
	bool bDel;

	int pipeIn, pipeOut;
};

#endif /* PCPIPECTRL_H_ */
