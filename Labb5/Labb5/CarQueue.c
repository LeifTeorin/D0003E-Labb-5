#include "TinyTimber.h"
#include "CarQueue.h"

#include <avr/io.h>
#include <avr/portpins.h>
#include <stdint.h>

void carLeavesQueue(CarQueue *self, int num)
{
	self->length--;
	self->light = 0;
	ASYNC(self->bridge, carEnters, NULL);
	int args[2] = {(self->direction)*4, self->length};
	ASYNC(self->gui, printAt, args);
}

void carArrives(CarQueue *self, int num)
{
	if(self->length == 0){
		self->length++;
		// kolla med controllern, bron måste stått tom i minst 1 sek eller 5 beroende på
	}else{
		self->length++;
	}
	
}

void greenLight(CarQueue *self, int num){
	self->light = 1;
}

void redLight(CarQueue *self, int num){
	self->light = 0;
}