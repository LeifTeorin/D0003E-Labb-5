#ifndef _INPUTHANDLER_
#define _INPUTHANDLER_

#include "TinyTimber.h"
#include "CarQueue.h"

typedef struct{
	Object super;
	struct CarQueue *nBound;
	struct CarQueue *sBound;
}InputHandler;

#define initInputHandler(nBound, sBound)\
	{initObject(), nBound, sBound}
		
void inputs(InputHandler *self);
		
#endif