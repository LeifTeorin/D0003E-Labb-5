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
	/*CarQueue *currentQ;
	currentQ = self->currentQ;
	ASYNC(self->currentQ, carLeavesQueue, NULL);
	if(self->counter<10 && currentQ->length > 0){
		AFTER(SEC(1), self, emptyCurrent, NULL);
		self->counter += 1;
		//ASYNC(self->currentQ, redLight, NULL);
	}else if(1){
		ASYNC(self->currentQ, redLight, NULL);
		switchQueue(self, NULL);
		self->counter = 0;
		AFTER(SEC(5), self->currentQ, greenLight, NULL);
		AFTER(SEC(5), self, emptyCurrent, NULL);
	}else{
		ASYNC(self->currentQ, redLight, NULL);
		self->counter = 0;
	}*/
	CarQueue *currentQ;
	CarQueue *northQ;
	CarQueue *southQ;
	int bl;
	northQ = self->northbound;
	southQ = self->southbound;
	currentQ = self->currentQ;
	int northEmpty = SYNC(self->northbound, isEmpty, NULL);
	int southEmpty = SYNC(self->southbound, isEmpty, NULL);
	if(northEmpty == 0 || southEmpty == 0){
	if(currentQ->direction==1){
		self->currentQ = self->northbound;
		ASYNC(self->southbound, redLight, NULL);
		AFTER(SEC(5), self->northbound, greenLight, NULL);
	}else{
		self->currentQ = self->southbound;
		ASYNC(self->northbound, redLight, NULL);
		AFTER(SEC(5), self->southbound, greenLight, NULL);
	}
	currentQ = self->currentQ;
	if(currentQ->length>10){
		bl = 10;
	}else{
		bl = currentQ->length;
	}
	AFTER(SEC(5 + bl), self, emptyCurrent, NULL);
	}else if((northEmpty && !southEmpty) || (!northEmpty && southEmpty)){
		
	}else{
		
	}
	
}

void switchLights(Controller *self, int origin){
	if(origin == 1){
		CarQueue *nBound;
		nBound = self->northbound;
		if(nBound->length > 0){
			AFTER(SEC(5), self->northbound, greenLight, NULL);
			self->currentQ = self->northbound;
		}
	}
	if(origin == 0){
		CarQueue *sBound;
		sBound = self->southbound;
		if(sBound->length > 0){
			AFTER(SEC(5), self->southbound, greenLight, NULL);
			self->currentQ = self->southbound;
		}
	}
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
			int carcnt = bridge->carcount;
			while(carcnt > 0){
				
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