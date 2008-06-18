/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

#ifndef PCPACKETFACTORY_H_
#define PCPACKETFACTORY_H_

class PCPacketFactory{
public:
	PCPacketFactory() {};
	~PCPacketFactory() {};

	void createPacket(int iLen, const char* inData, char* outData);
	void readPacket(int* iLen, const char* inData, char* outData); 
private:
};

#endif /* PCPACKETFACTORY_H_ */
