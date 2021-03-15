/*
 * Labb5.c
 *
 * Created: 2021-03-08 10:41:56
 * Author : hjall
 */ 

// shaved off är ground DONT FCKING FORGET

#include <avr/io.h>
#include "Bridge.h"
#include "CarQueue.h"
#include "GUI.h"
#include "Inputhandler.h"
#include "PortWriter.h"
#include "TinyTimber.h"
#include "Controller.h"

//#define FOSC 1843200
#define FOSC 8000000 
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
//#define MYUBRR (((FOSC / (BAUD * 16UL))) - 1)

GUI gui = initGUI();
Bridge bridge = initBridge(5, &gui);
PortWriter writer = initPortWriter();
CarQueue northB = initCarQueue(0, &bridge, &writer, &gui);
CarQueue southB = initCarQueue(1, &bridge, &writer, &gui);
Controller controller = initController(&bridge, &northB, &southB, &writer);
InputHandler inputhandl = initInputHandler(&northB, &southB, &gui, &controller);
//&(controller)->northbound = &northB; // fix this
//&(controller)->southbound = &southB; // fix this

void init_usart(unsigned int ubrr){
/*	//Interrupt request
	EIFR = 0xC0;
	
	//Cause an interrupt
	EIMSK = 0xC0;*/
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
//	UBRR0H = 0;
//	UBRR0L = 51;
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0);
	
	/* Set frame format: 8data, 1stop bit */
//	UCSR0C = (0<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

int main(void)
{
	init_program(&gui);
	init_usart(MYUBRR);
//	UDR0 = 6;
//	connectRoads(&controller, &northB, &southB);
//	sei();
	INSTALL(&inputhandl, inputs, IRQ_USART0_RX);
//	INSTALL(&inputhandl, testInputs, IRQ_PCINT0);
	tinytimber(&controller, switchLights, NULL);
	
//	tinytimber(&writer, writeToPort, 0);
//	UDR0 = 7;
}

