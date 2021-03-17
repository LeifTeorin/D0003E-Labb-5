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


void init_usart(unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	UCSR0B = (1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

int main(void)
{
	init_program(&gui);
	init_usart(MYUBRR);
	INSTALL(&inputhandl, inputs, IRQ_USART0_RX);
	tinytimber(&controller, emptyCurrent, NULL);
}

