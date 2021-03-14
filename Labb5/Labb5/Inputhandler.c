#include "TinyTimber.h"
#include "Inputhandler.h"
#include "CarQueue.h"
#include "GUI.h"

#include <avr/io.h>
#include <avr/portpins.h>
#include <stdint.h>

void inputs(InputHandler *self){
	char recieved = UDR0;
	int args[2] = {0, recieved+10};
//	ASYNC(self->gui, printAt, args);
	printAt(self->gui, args);
	while ((UCSR0A & (1 << UDRE0)) == 0) {};
	UDR0 = recieved + 100;
}

void testInputs(InputHandler *self){
	if((PINE&0x04)==0){
		ASYNC(self->controller, addNorth, NULL);
	}
	if((PINE&0x08)==0){
		ASYNC(self->controller, addSouth, NULL);
	}
}