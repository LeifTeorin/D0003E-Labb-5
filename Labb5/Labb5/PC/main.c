/*
 * PC_Code.c
 * Created: 2021-03-10 08:42:25
 * Author : Crill
 */ 

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
//POSIX
#include <pthread.h>


#define SERIAL_PORT "/dev/ttyS0"

#include <avr/io.h>

int yeet;
//Trafikljusen
int LightNorth; //0 = R�d; 1 = Gr�n.
int LightSouth; //0 = R�d; 1 = Gr�n.

void openPort()
	//Open serial port
	//yeet = open(SERIAL_PORT)
}

void readPort()
{
	//Read from serial port
	//F� LAMPSTATUS
}

void writePort(uint8_t data)
{
	//Write to serial port
	//GE SENSOR SIGNALS
	//1 om success, 0 om fail
	int yeet = printf(yeet, &data, sizeof(data));
	if(yeet < 0)
	{
		//Om det inte gick att printa datan blir det rip
		printf("rip");
	}
}

void Display()
{
	�//Displaya p� datorn vad som h�nder
}

void Input()
{
	�//Keyboard inputs
}

int main(void)
{
	
	uint8_t data = 1001;
    while (1) 
    {
    }
}

