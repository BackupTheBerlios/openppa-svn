//
// C++ Interface: ppappreceiver
//
// Description: 
//
//
// Author: Štefan Sakalík <rabbit64@users.berlios.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PPAPPRECEIVER_H
#define PPAPPRECEIVER_H
#include "core/pcplugentity.h"

/**
	@author  <rabbit64@users.berlios.de>
*/

class PPAppReceiver{
public:
    PPAppReceiver(PCPlugEntity* ent);
    ~PPAppReceiver();

private:
	PCPlugEntity* coreEnt;
};

#endif
