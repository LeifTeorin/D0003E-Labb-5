 #include "TinyTimber.h"
 #include "PortWriter.h"

 #include <avr/io.h>
 #include <avr/portpins.h>
 #include <stdint.h>
 
 void writeToPort(PortWriter *self, int num){
	while (!(UCSR0A & (1 << UDRE0))) {
		
	}
	volatile uint8_t testwrite = (self->northgreen << 0) | (self->northred << 1) |( self->southgreen << 2) | (self->southred << 3);
//	testwrite &= 0x0F;
//	UDR0 = (self->northgreen << 0) | (self->northred << 1) |( self->southgreen << 2) | (self->southred << 3);
	UDR0 = testwrite;
//	UDR0 = (self->northgreen << 0) | (self->northred << 1) |( self->southgreen << 2) | (self->southred << 3);
 }
 
 void updateBits(PortWriter *self, int num){
	 self->writebits &= num;
	 writeToPort(self, NULL);
 }
 
 void updateNorth(PortWriter *self, int num){
	 if(num == 2){
		 self->northgreen=0;
		 self->northred = 1;
	 }
	 if(num == 1){
		 self->northgreen= 1;
		 self->northred = 0;
	 }
	 ASYNC(self, writeToPort, NULL);
//	 writeToPort(self, NULL);
 }
 
 void updateSouth(PortWriter *self, int num){
	 if(num == 2){
		 self->southgreen=0;
		 self->southred = 1;
	 }
	 if(num == 1){
		 self->southgreen= 1;
		 self->southred = 0;
	 }
	 //writeToPort(self, NULL);
	 ASYNC(self, writeToPort, NULL);
 }
 
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
 
 