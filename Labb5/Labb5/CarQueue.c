#include "TinyTimber.h"
#include "CarQueue.h"

#include <avr/io.h>
#include <avr/portpins.h>
#include <stdint.h>

void carLeavesQueue(CarQueue *self, int num)
{
	self->length--;
	ASYNC(self->bridge, carEnters, NULL);
}

void carArrives(CarQueue *self, int num)
{
	self->length++;
}