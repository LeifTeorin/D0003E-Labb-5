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
			ASYNC(self->writer, redred, NULL);
			
			AFTER(SEC(5), self->writer, redgreen, NULL);
		}else{
			self->currentQ = self->southbound;
			self->curr = 1;
			ASYNC(self->writer, redred, NULL);
			
			AFTER(SEC(5), self->writer, greenred, NULL);
		}
		currentQ = self->currentQ;
		if(currentQ->length>10){
			bl = 10;
		}else{
			bl = currentQ->length;
		}
		ASYNC(self->currentQ, setMax, bl-1);
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
		AFTER(MSEC(100), self, emptyCurrent, NULL); //kanske kör detta i don't know
	}
	
}

void switchLights(Controller *self, int origin){
	/*CarQueue *sBound;
	sBound = self->southbound;
	CarQueue *nBound;
	nBound = self->northbound;
	if(origin == 1){
		if(nBound->length > 0){
			AFTER(SEC(5), self->northbound, greenLight, NULL);
			self->currentQ = self->northbound;
			}else if(sBound->length>0){
			AFTER(SEC(1), self->southbound, greenLight, NULL);
			}else{
			
		}
	}
	if(origin == 0){
		
		if(sBound->length > 0){
			AFTER(SEC(5), self->southbound, greenLight, NULL);
			self->currentQ = self->southbound;
		}
	}*/
	
	if(self->curr==1){
//		self->currentQ = self->northbound;
		self->curr = 0;
	//	ASYNC(self->southbound, redLight, NULL);
		ASYNC(self->writer, redred, NULL);
		AFTER(SEC(5), self->writer, redgreen, NULL);
	//	AFTER(SEC(5), self->northbound, greenLight, NULL);
	}else{
//		self->currentQ = self->southbound;
		self->curr = 1;
	//	ASYNC(self->northbound, redLight, NULL);
		ASYNC(self->writer, redred, NULL);
		AFTER(SEC(5), self->writer, greenred, NULL);
	//	AFTER(SEC(5), self->southbound, greenLight, NULL);
	}
	AFTER(SEC(9), self, switchLights, NULL);
}

void startEmptying(Controller *self, int origin){
	CarQueue *nBound;
	CarQueue *sBound;
	CarQueue *cQueue;
	Bridge *bridge;
	nBound = self->northbound;
	sBound = self->southbound;
	cQueue = self->currentQ;
	bridge = self->bridge;
	if(origin == 1 && nBound->length == 0){
		int dir = cQueue->direction;
		if(dir == 1){
			ASYNC(self->currentQ, greenLight, NULL);
		}else{
			self->currentQ = self->southbound;
//			int carcnt = bridge->carcount;
			while(bridge->carcount > 0){
				
			}
			ASYNC(self->currentQ, greenLight, NULL);
		}
	}
	if(origin == 0 && sBound->length == 0){
		int dir = cQueue->direction;
		if(dir == 0){
			ASYNC(self->currentQ, greenLight, NULL);
		}else{
			int carcnt = bridge->carcount;
			self->currentQ = self->northbound;
			while(carcnt>0){
				
			}
			ASYNC(self->currentQ, greenLight, NULL);
		}
	}
}

void switchQueue(Controller *self, int num){
	CarQueue *cQ;
	cQ = self->currentQ;
	if(cQ->direction == 0){
		self->currentQ = self->southbound;
		// anropa portwriter
	}else{
		self->currentQ = self->northbound;
		// anropa portwriter
	}
}

void findNonEmpty(Controller *self, int num){
	CarQueue *nBound;
	CarQueue *sBound;
	sBound = self->southbound;
	nBound = self->northbound;
	if(nBound->length > 0 && sBound->length == 0){
		int light = sBound->light;
		if(light == 0){
			self->currentQ = self->northbound;
			ASYNC(self, emptyCurrent, NULL);
		}
	}else if(nBound->length == 0 && sBound->length > 0){
		if(nBound->light == 0){
			self->currentQ = self->southbound;
			ASYNC(self, emptyCurrent, NULL);
		}
	}
}

void connectRoads(Controller *self, struct CarQueue *northB, struct CarQueue *southB){
	self->northbound = northB;
	self->southbound = southB;
}

void startup(Controller *self, int num){
	ASYNC(self->northbound, emptyQueue, NULL);
	ASYNC(self->southbound, emptyQueue, NULL);
}

void addNorth(Controller *self, int num){
	CarQueue *northQ;
	CarQueue *southQ;
	CarQueue *cQueue;
	Bridge *bridge;
	bridge = self->bridge;
	northQ = self->northbound;
	southQ = self->southbound;
	cQueue = self->currentQ;
	int northlength = northQ->length;
	if(northlength == 0 && southQ->length == 0){
		ASYNC(self->northbound, carArrives, NULL);
		int dir = cQueue->direction;
		if(dir == 0){
			ASYNC(self->currentQ, greenLight, NULL);
		}else{
			self->currentQ = self->northbound;
//			int carcnt = bridge->carcount;
			while(bridge->carcount > 0){
				
			}
			ASYNC(self->currentQ, greenLight, NULL);
		}
	}else{
		ASYNC(self->northbound, carArrives, NULL);
	}
}

void addSouth(Controller *self, int num){
	CarQueue *northQ;
	CarQueue *southQ;
	CarQueue *cQueue;
	Bridge *bridge;
	bridge = self->bridge;
	northQ = self->northbound;
	southQ = self->southbound;
	cQueue = self->currentQ;
	int northlength = northQ->length;
	int southlength = southQ->length;
	if(southlength == 0 && northlength == 0){
		ASYNC(self->southbound, carArrives, NULL);
		int dir = cQueue->direction;
		if(dir == 1){
			ASYNC(self->currentQ, greenLight, NULL);
		}else{
			self->currentQ = self->southbound;
			int carcnt = bridge->carcount;
			while(carcnt > 0){
				
			}
			ASYNC(self->currentQ, greenLight, NULL);
		}
	}else{
		ASYNC(self->southbound, carArrives, NULL);
	}
}