#include "TinyTimber.h"
#include "Inputhandler.h"

#include <avr/io.h>
#include <avr/portpins.h>
#include <stdint.h>

void inputs(InputHandler *self){
	if(UDR0&0x1){
		ASYNC(self->nBound, carArrives, NULL);
	}
	if((UDR0>>2)&0x1){
		ASYNC(self->sBound, carArrives, NULL);
	}
}