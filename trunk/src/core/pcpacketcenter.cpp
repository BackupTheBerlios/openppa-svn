/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

#include "core/pcpacketcenter.h"
#include "misc/dbgprint.h"

#include <stdio.h>
#include <string.h>

/*!
 * 	\class PCPacketCenter
 * 	\brief Packet manipulation
 *
 * 	This class is used to manipulate with packets. Packets are sent through pipes
 * 	TODO: max. length issues.
 * 	Data packet format:
 * 	first 3 bytes:	ppa
 * 	second 2 bytes:	version
 * 	third 2 bytes:	length (max = 64kB)
 *
 * 	TODO: WRITE and USE this class!!
 */


// IN: dataPartLen, packetType: send=0/reply=1... OU headerData
// headerData will be allocated. Need to DELETE OUTSIDE!!
void PCPacketCenter::createHeader(int packetType,
                                  int opId,
                                  int dataPartLen,
                                  char*& headerData, 
                                  int& headerLen) {
	headerData = new char[10];
	headerLen = 10;

	headerData[0] = 'P'; headerData[1] = 'P'; headerData[2] = 'A';	// PPA id
	headerData[3] = (char)1; headerData[4] = (char)0;				// 1.0
	headerData[5] = packetType?(char)1:(char)0;
	cvtIntTo2B(&opId, headerData + 6);
	cvtIntTo2B(&dataPartLen, headerData + 8);
}

// IN: headerData. OU: packetType, opId, dataPart (headerData reference)
void PCPacketCenter::parseHeader(char* headerData,
                                 int& packetType,
                                 int& opId,
                                 char*& dataPart,
                                 int& dataPartLen) {
	if(headerData[0] != 'P' && headerData[1] != 'P' && headerData[2] != 'A') {
		dbgPrint(2, "PCPacketHeader::parseHeader PPA id mismatch");
	}

	if((int)headerData[3] != 1 && (int)headerData[4] != 0) {
		dbgPrint(2, "PCPacketHeader::parseHeader protocol version mismatch");
	}

	packetType = headerData[5]?1:0;
	cvtIntFrom2B(&opId, headerData + 6);
	dataPart = headerData + 10;
	cvtIntFrom2B(&dataPartLen, headerData + 8);
}


// obsolete
/*!
 * Create packet outData from inData. WARNING: Everything must be preallocated.
 * TODO: length issues!!
 */
void PCPacketCenter::createPacket(int *iLen, const char* inData, char* outPacket) {
	outPacket[0] = 'p'; outPacket[1] = 'c';
	memcpy(&outPacket[2],iLen,2);
	memcpy(&outPacket[4],inData,*iLen);
	*iLen += 4;
}

/*!
 *  outData from inData. WARNING: Everything must be preallocated.
 *  TODO: length issues!!!
 */
void PCPacketCenter::readPacket(int *iLen, const char* inPacket, char* outData) {
	if(!((inPacket[0] == 'p') && (inPacket[1] == 'c'))){
		dbgPrint(4,"ReadPacket:Incorrect packet!");
		return;
	}

	memcpy(iLen, &inPacket[2], 2);
	memcpy(outData, &inPacket[4], *iLen);
}

bool PCPacketCenter::testPacket(const char* inPacket) {
	if(!((inPacket[0] == 'p') && (inPacket[1] == 'c'))) {
		// is not packet
		return false;
	}
	else
		return true;
}

void PCPacketCenter::shiftPacket(char* inOutPacket) {
	if(!testPacket(inOutPacket)) {
		dbgPrint(3,"shiftPacket: invalid packet");
		return;
	}

	int* iLen = new int;

	memcpy(iLen, &inOutPacket[2], 2);
	memcpy(inOutPacket, &inOutPacket[*iLen+4], 128-*iLen);
}
