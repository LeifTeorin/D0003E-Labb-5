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
	struct PortWriter *writer;
	struct GUI *gui;
	int maxSends;
}CarQueue;

#define initCarQueue(direction, bridge, writer, gui)\
{initObject(), 0, 0, direction, 0, bridge, writer, gui, 0}
	
void setMax(CarQueue *self, int num);	
void resetCounter(CarQueue *self, int num);
void carLeavesQueue(CarQueue *self, int num);
void carArrives(CarQueue *self, int num);
//void emptyQueue(CarQueue *self, int num);
void greenLight(CarQueue *self, int num);
void redLight(CarQueue *self, int num);
//int isEmpty(CarQueue *self, int num);

#endif