#include "TinyTimber.h"
#include "Inputhandler.h"
#include "CarQueue.h"
#include "GUI.h"

#include <avr/io.h>
#include <avr/portpins.h>
#include <stdint.h>

void inputs(InputHandler *self){
	if(UDR0 == 0xff){
		int args[2] = {0, 69};
		SYNC(self->gui, printAt, args);
	}
}

void testInputs(InputHandler *self){
	if((PINE&0x04)==0){
		SYNC(self->nBound, carArrives, NULL);
	}
	if((PINE&0x08)==0){
		SYNC(self->sBound, carArrives, NULL);
	}
}