 #include "TinyTimber.h"
 #include "PortWriter.h"

 #include <avr/io.h>
 #include <avr/portpins.h>
 #include <stdint.h>
 
 
 void redgreen(PortWriter *self, int num){
	while (!(UCSR0A & (1 << UDRE0))) {
		
	}
	UDR0 = 9;
 }
 
 void redred(PortWriter *self, int num){
	 while (!(UCSR0A & (1 << UDRE0))) {
		
	}
	UDR0 = 10;
 }
 
 void greenred(PortWriter *self, int num){
	while (!(UCSR0A & (1 << UDRE0))) {
		
	}
	UDR0 = 6;
 }
 //TODO: Skriv en funktion som kommunicerar med datorn
 
 