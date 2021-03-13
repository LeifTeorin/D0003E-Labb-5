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
	struct Controller *controller;
}InputHandler;

#define initInputHandler(nBound, sBound, gui, controller)\
	{initObject(), nBound, sBound, gui, controller}
		
void inputs(InputHandler *self);
void testInputs(InputHandler *self);
		
#endif