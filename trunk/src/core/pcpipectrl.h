
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
	bool bProcessed;
	bool bDel;

	int iOutBuffer;
	char cOutBuffer[128];

	int pipeIn, pipeOut;
};
