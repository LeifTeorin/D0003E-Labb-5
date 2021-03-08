#ifndef _PORTWRITER_
#define _PORTWRITER_

#include "TinyTimber.h"

typedef struct{
	Object super;
}PortWriter;

#define initPortWriter()\
{initObject()}

#endif