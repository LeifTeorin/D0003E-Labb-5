#include "TinyTimber.h"
#include "CarQueue.h"
#include "PortWriter.h"
#include "GUI.h"

#include <avr/io.h>
#include <avr/portpins.h>
#include <stdint.h>

void carLeavesQueue(CarQueue *self, int num){
	self->length--;
//	self->light = 0;
	ASYNC(self->bridge, carEnters, NULL);
	int args[2] = {(self->direction)*4, self->length};
	ASYNC(self->gui, printAt, args);
}

void emptyQueue(CarQueue *self, int num){
	if(self->light){
		if(self->length>0){
			self->length--;
			ASYNC(self->bridge, carEnters, NULL);
			int args[2] = {(self->direction)*4, self->length};
			ASYNC(self->gui, printAt, args);
			(self->counter)++;
		}
	}else{
		self->counter = 0;
	}
	if(self->counter > 10){
		ASYNC(self->controller, lights, NULL);
		ASYNC(self, redLight, NULL);
		self->counter = 0;
	}
	AFTER(SEC(1), self, emptyQueue, NULL);
}

void carArrives(CarQueue *self, int num){
	if(self->length == 0){
		self->length++;
		// kolla med controllern, bron måste stått tom i minst 1 sek eller 5 beroende på
	}else{
		self->length++;
		int args[2] = {4*(self->direction), self->length};
		ASYNC(self->gui, printAt, args);
	}
	
}

void greenLight(CarQueue *self, int num){
	self->light = 1;
	ASYNC(self->writer, writeToPort, (1<<2*((self->direction))));
}

void redLight(CarQueue *self, int num){
	self->light = 0;
	ASYNC(self->writer, writeToPort, (2<<(2*(self->direction))));
}