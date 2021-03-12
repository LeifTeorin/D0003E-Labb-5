#ifndef _CARQUEUE_
#define _CARQUEUE_

#include "TinyTimber.h"
#include "Bridge.h"
#include "GUI.h"
#include "Controller.h"

typedef struct{
	Object super;
	int length;
	int light; // 1 är grönt och 0 är rött
	int direction; // 0 är northbound och 1 är southbound 
	int counter;
	struct Bridge *bridge;
	struct Controller *controller;
	struct PortWriter *writer;
	struct GUI *gui;
}CarQueue;

#define initCarQueue(bridge, controller, writer, gui)\
{initObject(), 0, 0, bridge, controller, writer, gui}
	
void carLeavesQueue(CarQueue *self, int num);
void carArrives(CarQueue *self, int num);
void emptyCurrent(Controller *self, int num);
void greenLight(CarQueue *self, int num);
void redLight(CarQueue *self, int num);

#endif