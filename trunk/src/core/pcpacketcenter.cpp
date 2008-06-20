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
