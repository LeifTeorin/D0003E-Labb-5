/*
 * PC_Code.c
 * Created: 2021-03-10 08:42:25
 * Author : Crill
 */ 


/*
Likewise, we will use the transmitter side of the USART for implementing the four output signals that control the status of each lamp of the traffic lights. The four signals will map onto the bits of the transmit data register as follows:


*/

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
//POSIX
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


#define SERIAL_PORT "/dev/ttyS0"

#include <avr/io.h>

enum Z {
	//NONE = 0,
	RED = 0,
	GREEN = 1
};

int yeet;
//Trafikljusen
int LightNorth; //0 = Röd; 1 = Grön.
int LightSouth; //0 = Röd; 1 = Grön.
int[] writeBit = [0, 0, 0, 0];
enum Z light = RED;

void openPort()
{
	//Open serial port
	//yeet = open(SERIAL_PORT)
}


/*
Bit 0: Northbound green light status
Bit 1: Northbound red light status
Bit 2: Southbound green light status
Bit 3: Southbound red light status
*/
void readPort(void *arg)
{
	//Read from serial port
	//F� LAMPSTATUS
	uint8_t data = read(1, &stdin, NULL, NULL, NULL, NULL);
	
	uint8_t signal = 0;
}

void writePort(uint8_t data)
{
	//Write to serial port
	//GE SENSOR SIGNALS
	//1 om success, 0 om fail
	int yeet = printf(yeet, &data, sizeof(data));
	if(yeet < 0)
	{
		//Om det inte gick att printa datan
		printf("OHELL");
	}
}

void Display()
{
	//Displaya p� datorn vad som h�nder
	printf("(1 = gr�n, 0 = r�d)\n");
	printf("North: ", LightNorth);
	printf("South: ", LightSouth);
	printf("Northbound cars");
	printf("Southbound cars");
}

void Input(void *arg)
{
	char c;
	//Keyboard inputs 
	while((c=getchar()) != 'q')
	{
		if(c == 'n')
		{
			arrivalSensor(1);
		}
		else if (c == 's')
		{
			arrivalSensor(0);
		}
	}
}

//Uppdatera bron och se om det finns n�got p� den
void updateBridge()
{
	
}

/*
Om n�got �ker in i bron
Direction 1 = north, 0 = south
Bit 1: Northbound bridge entry sensor activated
Bit 3: Southbound bridge entry sensor activated
*/
void entrySensor(int dir)
{
	if(dir == 1)
	{
		writeBit[1] = 1;
		writeBit[3] = 0;
	}
	else if (dir == 0)
	{
		writeBit[1] = 0;
		writeBit[3] = 1;
	}
}

/*
Om n�gon kommer till bron
Bit 0: Northbound car arrival sensor activated
Bit 2: Southbound car arrival sensor activated
*/
void arrivalSensor(int dir)
{
	if(dir == 1)
	{
		writeBit[0] = 1;
		writeBit[2] = 0;
	}
	else if (dir == 0)
	{
		writeBit[0] = 0;
		writebit[2] = 1;
	}
}

int main(void)
{
    while (1) 
    {
		
    }
}

