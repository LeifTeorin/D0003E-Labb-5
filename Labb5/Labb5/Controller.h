#ifndef _CONTROLLER_
#define _CONTROLLER_

#include "TinyTimber.h"

typedef struct{
	Object super;
	struct CarQueue *northbound;
	struct CarQueue *southbound;
	struct CarQueue *currentQ;
	int counter;
}Controller;

#define initController(north, south)\
	{initObject(), north, south, north}

void switchQueue(Controller *self, int num);
void emptyCurrent(Controller *self, int num);
void findNonEmpty(Controller *self, int num);

#endif