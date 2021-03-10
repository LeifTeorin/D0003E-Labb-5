#ifndef _CARQUEUE_
#define _CARQUEUE_

#include "TinyTimber.h"
#include "Bridge.h"

typedef struct{
	Object super;
	int length;
	int light; // 1 är grönt och 0 är rött
	int direction; // 1 är southbound och 0 är northbound
	struct Bridge *bridge;
	struct Controller *controller;
}CarQueue;

#define initCarQueue(bridge, controller)\
{initObject(), 0, 0, bridge, controller}
	
void carLeavesQueue(CarQueue *self, int num);
void carArrives(CarQueue *self, int num);
void greenLight(CarQueue *self, int num);
void redLight(CarQueue *self, int num);

#endif