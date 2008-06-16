#ifndef PPACORE_H_
#define PPACORE_H_

void *thrRoutine(void* arg);

class PCCtrlNode {
public:
	PCCtrlNode();
	PCCtrlNode(char * input, char * output);
	~PCCtrlNode();

	void send(char * dat, int length);
	int thr_receive(char * dat);
	void execHandler();

private:
	void createPipe();
	void connectPipe();
	void forkExec();
	void dbgPrint(char * stuff);
	void sendLoop();

	char cInPipe[20];
	char cOutPipe[20];

	int iInBuffer;
	char cInBuffer[128];
	bool bProcessed;

	int iOutBuffer;
	char cOutBuffer[128];

	int pipeIn, pipeOut;
};

#endif /* PPACORE_H_ */
