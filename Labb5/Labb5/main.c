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

#define BAUD 9600
#define FOSC 8000000UL

GUI gui = initGUI();
PortWriter writer = initPortWriter();
Bridge bridge = initBridge(5, &gui);
Controller controller = initController(&bridge);
CarQueue northB = initCarQueue(0, &bridge, &controller, &writer, &gui);
CarQueue southB = initCarQueue(1, &bridge, &controller, &writer, &gui);
InputHandler inputhandl = initInputHandler(&northB, &southB, &gui);
//&(controller)->northbound = &northB; // fix this
//&(controller)->southbound = &southB; // fix this

int main(void)
{
	//Interrupt request
	EIFR = 0xC0;
	
	//Cause an interrupt
	EIMSK = 0xC0;
	UBRR0H = (((FOSC / (BAUD * 16UL))) - 1) >> 8;
	UBRR0L = (((FOSC / (BAUD * 16UL))) - 1);
	
	//Activate the pin but only on receive.
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	//
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	connectRoads(&controller, &northB, &southB);
	init_program(&gui);
	INSTALL(&inputs, inputs, IRQ_USART0_RX);
	tinytimber(&controller, startup, NULL);
}

