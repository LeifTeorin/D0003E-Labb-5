#include <avr/io.h>
#include "Controller.h"
#include "CarQueue.h"
#include "TinyTimber.h"
#include "PortWriter.h"

void emptyCurrent(Controller *self, int num){
	CarQueue *currentQ;
	CarQueue *northQ;
	CarQueue *southQ;
	Bridge *bridge;
	int bl;
	northQ = self->northbound;
	southQ = self->southbound;
	currentQ = self->currentQ;
	bridge = self->bridge;
	ASYNC(self->northbound, resetCounter, NULL);
	ASYNC(self->southbound, resetCounter, NULL);
	ASYNC(self->northbound, setMax, 0);
	ASYNC(self->southbound, setMax, 0);
	if(northQ->length > 0 && southQ->length > 0){
		if(self->curr==1){
			self->currentQ = self->northbound;
			self->curr = 0;
			ASYNC(self->writer, redgreen, NULL);
			currentQ = self->currentQ;
			if(currentQ->length>10){
				bl = 10;
			}else{
				bl = currentQ->length;
			}
		}else{
			self->currentQ = self->southbound;
			self->curr = 1;
			ASYNC(self->writer, greenred, NULL);
			currentQ = self->currentQ;
			if(currentQ->length>10){
				bl = 10;
			}else{
				bl = currentQ->length;
			}
		}
		
		ASYNC(self->currentQ, setMax, bl-1);
		AFTER(SEC(bl), self->writer, redred, NULL);
		AFTER(SEC(5 + bl), self, emptyCurrent, NULL);
	}else if((northQ->length ==0 && southQ->length > 0) || (northQ->length > 0 && southQ->length == 0)){
		if(southQ->length == 0){
			if(self->curr == 1){
				if(bridge->carcount>0){
					ASYNC(self->writer, redred,  NULL);
					AFTER(MSEC(500), self, emptyCurrent, NULL);
					return;
				}else{
					self->curr = 0;
					self->currentQ = self->northbound;
					ASYNC(self->writer, redgreen, NULL);
				}
			}else{
				ASYNC(self->writer, redgreen, NULL);
			}
			currentQ = self->currentQ;
			if(currentQ->length>10){
				bl = 10;
			}else{
				bl = currentQ->length;
			}
			ASYNC(self->northbound, setMax, bl-1);
			AFTER(SEC(bl), self, emptyCurrent, NULL);
		}else if(northQ->length == 0){
			if(self->curr == 0){
				if(bridge->carcount>0){
					AFTER(MSEC(500), self, emptyCurrent, NULL);
					return;
				}else{
					self->curr = 1;
					self->currentQ = self->southbound;
					ASYNC(self->writer, greenred, NULL);
				}
			}else{
				ASYNC(self->writer, greenred, NULL);
			}
			currentQ = self->currentQ;
			if(currentQ->length>10){
				bl = 10;
			}else{
				bl = currentQ->length;
			}
			ASYNC(self->southbound, setMax, bl-1);
			AFTER(SEC(bl), self, emptyCurrent, NULL);
		}
	}else{
		AFTER(MSEC(100), self, emptyCurrent, NULL); 
	}
	
}

void switchLights(Controller *self, int origin){
	if(self->curr==1){
		self->curr = 0;
		ASYNC(self->writer, redred, NULL);
		AFTER(SEC(5), self->writer, redgreen, NULL);
	}else{
		self->curr = 1;
		ASYNC(self->writer, redred, NULL);
		AFTER(SEC(5), self->writer, greenred, NULL);
	}
	AFTER(SEC(9), self, switchLights, NULL);
}
