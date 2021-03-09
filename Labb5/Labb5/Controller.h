#ifndef _CONTROLLER_
#define _CONTROLLER_

#include "TinyTimber.h"

typedef struct{
	Object super;
	struct CarQueue *northbound;
	struct CarQueue *southbound;
	struct CarQueue *currentQ;
	
}Controller;

#define initController(north, south)\
	{initObject(), north, south, north}

	}