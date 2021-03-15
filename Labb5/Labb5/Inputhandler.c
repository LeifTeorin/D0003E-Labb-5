#include "TinyTimber.h"
#include "Inputhandler.h"
#include "CarQueue.h"
#include "GUI.h"

#include <avr/io.h>
#include <avr/portpins.h>
#include <stdint.h>

void inputs(InputHandler *self){
	char recieved = UDR0;
	if(recieved&1 == 1){ // northbound car arrival
		ASYNC(self->nBound, carArrives, NULL);
	}
	if(recieved&2){ // northbound enters bridge
		ASYNC(self->nBound, carLeavesQueue, NULL);
	}
	if(recieved&4){
		ASYNC(self->sBound, carArrives, NULL);
	}
	if(recieved&8){
		ASYNC(self->sBound, carLeavesQueue, NULL);
	}
}

void testInputs(InputHandler *self){
	if((PINE&0x04)==0){
		//ASYNC(self->controller, addNorth, NULL);
		//ASYNC(self->nBound, greenLight, NULL);
		UDR0 = 9;
	}
	if((PINE&0x08)==0){
		//ASYNC(self->controller, addSouth, NULL);
		ASYNC(self->sBound, greenLight, NULL);
		UDR0 = 6;
	}
}