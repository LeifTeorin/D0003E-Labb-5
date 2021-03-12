#ifndef _INPUTHANDLER_
#define _INPUTHANDLER_

#include "TinyTimber.h"
#include "CarQueue.h"
#include "GUI.h"

typedef struct{
	Object super;
	struct CarQueue *nBound;
	struct CarQueue *sBound;
	struct GUI *gui;
}InputHandler;

#define initInputHandler(nBound, sBound, gui)\
	{initObject(), nBound, sBound, gui}
		
void inputs(InputHandler *self);
void testInputs(InputHandler *self);
		
#endif