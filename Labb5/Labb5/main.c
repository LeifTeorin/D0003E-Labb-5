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

#define FOSC 8000000UL // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

GUI gui = initGUI();
PortWriter writer = initPortWriter();
Bridge bridge = initBridge(5, &gui);
Controller controller = initController(&bridge);
CarQueue northB = initCarQueue(0, &bridge, &controller, &writer, &gui);
CarQueue southB = initCarQueue(1, &bridge, &controller, &writer, &gui);
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
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01);
}

int main(void)
{
	init_program(&gui);
	init_usart(MYUBRR);
//	connectRoads(&controller, &northB, &southB);
//	sei();
	INSTALL(&inputs, inputs, IRQ_USART0_RX);
//	INSTALL(&inputs, testInputs, IRQ_PCINT0);
//	tinytimber(&controller, startup, NULL);
	tinytimber(&writer, writeToPort, 0);
//	UDR0 = 7;
}

