#include "TinyTimber.h"
#include "CarQueue.h"
#include "PortWriter.h"
#include "GUI.h"
#include "Controller.h"

#include <avr/io.h>
#include <avr/portpins.h>
#include <stdint.h>

void setMax(CarQueue *self, int num){
	self->maxSends = num;
}

void resetCounter(CarQueue *self, int num){
	self->counter = 0;
}

void carLeavesQueue(CarQueue *self, int num){
	self->counter++;
	self->length--;
	ASYNC(self->bridge, carEnters, NULL);
	int args[2] = {(self->direction)*4, self->length};
	SYNC(self->gui, printAt, args);

	if(self->counter > (self->maxSends) || self->length == 0){
		ASYNC(self->writer, redred, NULL);
		self->counter = 0;
	}else{
		if(self->direction == 1){
			AFTER(MSEC(500), self->writer, redred, NULL);
			AFTER(SEC(1), self->writer, greenred, NULL);
		}else{
			AFTER(MSEC(500), self->writer, redred, NULL);
			AFTER(SEC(1), self->writer, redgreen, NULL);
		}
	}
	
}


void carArrives(CarQueue *self, int num){
	int test = self->length;
	int pos = self->direction;
	self->length++;
	int args[2] = {4*pos, self->length};
	SYNC(self->gui, printAt, args);
}

void greenLight(CarQueue *self, int num){
	self->light = 1;
}

void redLight(CarQueue *self, int num){
	self->light = 0;
}