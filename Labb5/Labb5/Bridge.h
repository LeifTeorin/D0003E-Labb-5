#ifndef _BRIDGE_
#define _BRIDGE_

#include "TinyTimber.h"
#include "GUI.h"

typedef struct{
	Object super;
	int carcount;
	int traveltime;
	struct GUI *gui;
}Bridge;

#define initBridge(traveltime, gui)\
	{initObject(), 0, traveltime, gui}

void carEnters(Bridge *self, int num);
void carLeaves(Bridge *self, int num);

#endif