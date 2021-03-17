#ifndef _PORTWRITER_
#define _PORTWRITER_

#include "TinyTimber.h"

typedef struct{
	Object super;
	int writebits;
	int northgreen;
	int northred;
	int southgreen;
	int southred;
}PortWriter;

#define initPortWriter()\
{initObject(), 0, 0, 1, 0, 1}
	
//void writeToPort(PortWriter *self, int num);
//void updateBits(PortWriter *self, int num);
//void updateNorth(PortWriter *self, int num);
//void updateSouth(PortWriter *self, int num);
void greenred(PortWriter *self, int num);
void redgreen(PortWriter *self, int num);
void redred(PortWriter *self, int num);

#endif