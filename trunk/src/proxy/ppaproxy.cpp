/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

#include "ppaproxy.h"
#include <unistd.h>

int xmain(int argc, char ** argv)
{
	PCCtrlNode* pcn = new PCCtrlNode("./PPACoreExec");
	delete pcn;
}
