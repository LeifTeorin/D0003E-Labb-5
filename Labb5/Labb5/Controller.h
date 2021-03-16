#ifndef _CONTROLLER_
#define _CONTROLLER_

#include "TinyTimber.h"
#include "CarQueue.h"
#include "Bridge.h"
#include "PortWriter.h"

typedef struct{
	Object super;
	struct Bridge *bridge;
	struct CarQueue *northbound;
	struct CarQueue *southbound;
	struct CarQueue *currentQ;
	struct PortWriter *writer;
	int counter;
	int curr;
}Controller;

#define initController(bridge, northB, southB, writer)\
	{initObject(), bridge, northB, southB, northB, writer, 0, 0}

void switchQueue(Controller *self, int num);
void emptyCurrent(Controller *self, int num);
void switchLights(Controller *self, int origin);
void startEmptying(Controller *self, int origin);
void findNonEmpty(Controller *self, int num);
//void connectRoads(Controller *self, struct CarQueue *northB, struct CarQueue *southB);
//void startup(Controller *self, int num);
//void addNorth(Controller *self, int num);
//void addSouth(Controller *self, int num);

#endif