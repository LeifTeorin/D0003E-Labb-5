#ifndef _CARQUEUE_
#define _CARQUEUE_

#include "TinyTimber.h"
#include "Bridge.h"

typedef struct{
	Object super;
	int length;
	int light; // 1 �r gr�nt och 0 �r r�tt
	struct Bridge *bridge;
}CarQueue;

#define initCarQueue(bridge)\
{initObject(), 0, 0, bridge}
	
void carLeavesQueue(CarQueue *self, int num);
void carArrives(CarQueue *self, int num);

#endif