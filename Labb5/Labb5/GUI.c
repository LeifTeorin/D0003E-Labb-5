#include "TinyTimber.h"
#include "GUI.h"

#include <avr/io.h>
#include <avr/portpins.h>
#include <stdint.h>

int characters[10] = {
	0x1551,		// 0
	0x0118,		// 1
	0x1e11,		// 2
	0x1b11,		// 3
	0x0b50,		// 4
	0x1b41,		// 5
	0x1f41,		// 6
	0x4009,		// 7
	0x1f51,		// 8
	0x1b51		// 9
};

void writeChar(GUI *self, char ch, int pos){
	if((pos>5) | (pos<0)){
		return;
	}
	int shift;
	char mask_reg;
	char currbyte = 0x00;
	int character = 0;
	char *ptr;
	ptr  = &LCDDR0; // pekaren bï¿½rjar pï¿½ lcddr0
	
	if((int)ch > -1 && (int)ch < 10){
		character = characters[(int)ch];
	}
	
	
	if(pos & 0x01){ // om pos ï¿½r udda finns det en etta pï¿½ slutet, dï¿½ ska den skrivas pï¿½ de fyra bitarna till vï¿½nster pï¿½ registren
		mask_reg = 0x0f;
		shift = 4; // bitarna som ska skrivas behï¿½ver ï¿½ven flyttas fï¿½r att skrivas pï¿½ vï¿½nster sida
		}else{
		mask_reg = 0xf0;
		shift = 0;
	}
	ptr = ptr + (pos>>1); // pekaren hamnar pï¿½ lcddr0, 1 eller 2 beroende pï¿½ pos
	
	for(int i = 0; i < 4; i++){
		currbyte = (character & 0x0f); // vi tar de 4 bittarna lï¿½ngst till hï¿½ger
		currbyte = currbyte << shift; // bittarna shiftas antingen 0 eller 4 steg ï¿½t vï¿½nster
		*ptr = ((*ptr & mask_reg)|currbyte); // registret maskas och fylls sedan in med de bittarna vi hï¿½mtat
		character = (character>>4); // vi tar bort de 4 bittarna till hï¿½ger
		ptr += 5; // pekaren gï¿½r fem register fram fï¿½r nï¿½sta 4 bittar
	}
}

void printAt(GUI *self, int args[]) {
	int pp = args[0]; // index 0 är position
	int num = args[1]; // index 1 är numret
	//SYNC(self, writeChar, (((num % 100) / 10), pp));
	writeChar(self, ((num % 100) / 10), pp); // ï¿½ndra detta till sync sen, sï¿½ den blir lï¿½st
	pp++;
	//SYNC(self, writeChar, ((num % 10), pp));
	writeChar(self, (num % 10), pp); // ï¿½ndra detta till sync sen, sï¿½ den blir lï¿½st
}


void init_program(GUI *self){
	LCDCRA |= 0x80; // LCD enable
	//	LCDCRB = (1<<LCDMUX0)|(1<<LCDMUX1)|(1<<LCDPM2)|(1<<LCDPM1)|(1<<LCDPM0);
	LCDCRB = 0xb7; // 1/3 bias och 1/4 duty, asynk-klockan anvï¿½nds och 25 segment anvï¿½nds
	LCDCCR |= 15; // sätter kontrastkontrollen till 3,35 V
	LCDFRR = 7;	// sätter prescalern och ger framerate 32 Hz
	
	PORTB |= (1<<PB7)|(1<<PB6)|(1<<PB4);
	PORTE |= (1<<PE2)|(1<<PE3);
	DDRB = (1<<DDB5)|(1<<DDB3)|(1<<DDB2)|(1<<DDB1)|(1<<DDB0);
	DDRE = (1<<DDE6)|(1<<DDE4);
	
	EIMSK = (1<<PCIE1)|(1<<PCIE0);
	PCMSK0 = (1<<PCINT3)|(1<<PCINT2);
	PCMSK1 = (1<<PCINT15)|(1<<PCINT14)|(1<<PCINT12);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(0<<RXCIE0)|(0<<UDRIE0);
	//	PCMSK1 = 0x80;
	TCCR1A = 0xC0;
	TCCR1B = 0x18;
	PRR = 0x00;
	
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	
	//	SYNC(self, printRight, 0);
	//	SYNC(self, printLeft, 0);
	int argone[2] = {0, 0};
	int argtwo[2] = {4, 0};
	int argthree[2] = {2, 0};
	printAt(self, argone);
	printAt(self, argtwo);
	printAt(self, argthree);
	//	printLeft(self, 0);
	//	printRight(self, 0);
}