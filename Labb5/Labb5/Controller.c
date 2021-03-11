#include <avr/io.h>
#include "Controller.h"
#include "CarQueue.h"
#include "TinyTimber.h"

void emptyCurrent(Controller *self, int num){
	/*if(self->currentQ->length==0){
//		ASYNC(self->currentQ, redLight, NULL);
		switchQueue(Controller *self, int num);
		AFTER(SEC(0.5), self, emptyCurrent, NULL);
		return;
		//anropa portwriter
	}*/
	ASYNC(self->currentQ, carLeavesQueue, NULL);
	if(self->counter<10 && self->currentQ->length > 0){
		AFTER(SEC(1), self, emptyCurrent, NULL);
		self->counter += 1;
		//ASYNC(self->currentQ, redLight, NULL);
	}else if(self->){
		ASYNC(self->currentQ, redLight, NULL);
		switchQueue(self, NULL);
		self->counter = 0;
		AFTER(SEC(5), self->currentQ, greenLight, NULL);
		AFTER(SEC(5), self, emptyCurrent, NULL);
	}else{
		ASYNC(self->currentQ, redLight, NULL);
		self->counter = 0;
	}
}

void lights(Controller *self, int num){
	
}


void switchQueue(Controller *self, int num){
	if(self->currentQ->direction == 0){
		self->currentQ = self->southbound;
		// anropa portwriter
	}else{
		self->currentQ = self->northbound;
		// anropa portwriter
	}
}

void findNonEmpty(Controller *self, int num){
	if(self->northbound->length > 0 && self->southbound->length == 0){
		if(self->southbound->light == 0){
			self->currentQ = self->northbound;
			ASYNC(self, emptyCurrent, NULL);
		}
	}else if(self->northbound->length == 0 && self->southbound->length > 0){
		if(self->northbound->light == 0){
			self->currentQ = self->southbound;
			ASYNC(self, emptyCurrent, NULL);
		}
	}
}