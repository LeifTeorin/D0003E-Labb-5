 #include "TinyTimber.h"
 #include "PortWriter.h"

 #include <avr/io.h>
 #include <avr/portpins.h>
 #include <stdint.h>
 
 void writeToPort(PortWriter *self, int num){
	while ((UCSR0A & (1 << UDRE0)) == 0) {};
	UDR0 = num; 
 }
 //TODO: Skriv en funktion som kommunicerar med datorn
 
 