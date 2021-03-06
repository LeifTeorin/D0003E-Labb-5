#include "TinyTimber.h"
#include "Bridge.h"
#include "GUI.h"

#include <avr/io.h>
#include <avr/portpins.h>
#include <stdint.h>


void carEnters(Bridge *self, int num){
	int test = self->carcount;
	self->carcount++;
	AFTER(SEC(5), self, carLeaves, NULL);
	int args[2] = {2, test + 1};
	ASYNC(self->gui, printAt, args);
}

void carLeaves(Bridge *self, int num){
	int test = self->carcount;
	self->carcount--;
	int args[2] = {2, self->carcount};
	ASYNC(self->gui, printAt, args);
}