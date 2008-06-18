/*!
 * 	Copyright (c) 2008, Štefan Sakalík.
 * 	All Rights Reserved.
 *
 * 	Licensed under the GNU GENERAL PUBLIC LICENSE (v3).
 */

#include "ppaproxy.h"
#include <unistd.h>

int main(int argc, char ** argv)
{
	PCCtrlNode* pcn = new PCCtrlNode("./PPACoreExec");
	pcn->send("Hello world\n");
	pcn->send("1whazup\n");
usleep(500000);
	pcn->send("2yaaa\n");
	pcn->send("3goohw\n");
	pcn->send("4wooclaw\n");
//	pcn->send("qu\n");
usleep(500000);
	delete pcn;
}
