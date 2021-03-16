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
//	self->light = 0;
	ASYNC(self->bridge, carEnters, NULL);
	int args[2] = {(self->direction)*4, self->length};
	SYNC(self->gui, printAt, args);

	if(self->counter > (self->maxSends) || self->length == 0){
		ASYNC(self->writer, redred, NULL);
		self->counter = 0;
	}else{
		if(self->direction == 1){
			AFTER(MSEC(500), self->writer, redred, NULL);
//			ASYNC(self->writer, redred, NULL);
			AFTER(SEC(1), self->writer, greenred, NULL);
		}else{
			AFTER(MSEC(500), self->writer, redred, NULL);
			AFTER(SEC(1), self->writer, redgreen, NULL);
		}
	}
	
}

void emptyQueue(CarQueue *self, int num){
	int light = self->light;
	if(light){
		if(self->length>0){
			self->length--;
			ASYNC(self->bridge, carEnters, NULL);
			int args[2] = {(self->direction)*4, self->length};
			SYNC(self->gui, printAt, args);
			(self->counter)++;
		}else{
			self->light = 0;
		}
	}else{
		self->counter = 0;
	}
	/*if(self->counter > 10){
		ASYNC(self->controller, switchLights, self->direction);
		ASYNC(self, redLight, NULL);
		self->counter = 0;
	}*/
	AFTER(SEC(1), self, emptyQueue, NULL);
}

void carArrives(CarQueue *self, int num){
	/*int test = self->length;
	int pos = self->direction;
	if(test == 0){
		(self->length)++;
		ASYNC(self->controller, startEmptying, pos);
		int args[2] = {4*pos, test + 1};
		SYNC(self->gui, printAt, args);
		// kolla med controllern, bron måste stått tom i minst 1 sek eller 5 beroende på, andra kön MÅSTE stå tom
	}else if(test>0){
		self->length++;
		int args[2] = {4*pos, test + 1};
		SYNC(self->gui, printAt, args);
	}*/
	int test = self->length;
	int pos = self->direction;
	self->length++;
	int args[2] = {4*pos, self->length};
	SYNC(self->gui, printAt, args);
}

void greenLight(CarQueue *self, int num){
	self->light = 1;
//	int writebits = (1<<(2*(self->direction)));
/*
	if(self->direction==1){
		ASYNC(self->writer, updateSouth, 1);
	}else{
		ASYNC(self->writer, updateNorth, 1);
	}
*/
//	ASYNC(self->writer, updateBits, bits); // skickar 01 binärt
}

void redLight(CarQueue *self, int num){
	self->light = 0;
//	int writebits = (2<<(2*(self->direction)));
/*
	if(self->direction==1){
		ASYNC(self->writer, updateSouth, 2);
	}else{
		ASYNC(self->writer, updateNorth, 2);
	}
*/
//	ASYNC(self->writer, updateBits, writebits); //skickar 10 binärt 
}