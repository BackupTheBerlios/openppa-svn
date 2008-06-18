/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

#include "core/pcpacketfactory.h"

/*!
 * 	\class PCPacketFactory
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
void PCPacketFactory::createPacket(int iLen, const char* inData, char* outData) {
	memcpy(outData,iLen
}

/*!
 *  outData from inData. WARNING: Everything must be preallocated.
 *  TODO: length issues!!!
 */
void PCPacketFactory::readPacket(int *iLen, const char* inData, char* outData) {

}
