#ifndef _PORTWRITER_
#define _PORTWRITER_

#include "TinyTimber.h"

typedef struct{
	Object super;
}PortWriter;

#define initPortWriter()\
{initObject()}
	
void writeToPort(PortWriter *self, int num);

#endif