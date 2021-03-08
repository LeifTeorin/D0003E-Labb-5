#ifndef _GUI_
#define _GUI_

#include "TinyTimber.h"

typedef struct{
	Object super;
}GUI;

#define initGUI()\
	{initObject()}
		
void printAt(GUI *self, int args[]);
//void writeChar(GUI *self, char ch, int pos);
void init_program(GUI *self);
	
#endif