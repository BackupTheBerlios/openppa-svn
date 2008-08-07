/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

#ifndef PCPACKETCENTER_H_
#define PCPACKETCENTER_H_

class PCPacketCenter{
public:
	static void createHeader(int packetType,
                             int opId,
                             int dataPartLen,
                             char*& headerData, 
                             int& headerLen);

	static void parseHeader(char* headerData,
                            int& packetType,
                            int& opId,
                            char*& dataPart,
                            int& dataPartLen);

	static int getHeaderLength() { return 10; }

	static void createPacket(int* iLen, const char* inData, char* outPacket);
	static void readPacket(int* iLen, const char* inPacket, char* outData); 

	static bool testPacket(const char* inPacket);
	static void shiftPacket(char* inOutPacket);

private:
};

#endif /* PCPACKETCENTER_H_ */
