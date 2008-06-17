
class PCProcCtrl {
public:
	PCProcCtrl() {};
	~PCProcCtrl() {};

	void newProcess(char* cExecName, char* cInPipe, char* cOutPipe); 
	void waitChildEnd();
private:
	int iChildProc;
	void dbgPrint(char * stuff);
};
