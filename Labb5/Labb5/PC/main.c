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

void openPort()
{
	//Simulera termios
	struct termios tty;
	if(tcgetattr(fd, &tty) < 0)
	{
		//Skriver ut felkoden fr�n tcgetattr
		printf(strerror(errno));
		return;
	}
	//Output speed
	cfsetospeed(&tty, (speed_t)speed);
	//Input speed
	cfsetispeed(&tty, (speed_t)speed);
	
	tty.c_cflag |= CLOCAL | CREAD;
	tty.c_cflag &= ~CSIZE;
	//Set 8bit
	tty.c_cflag |= CS8;
	//St�ng av parity bit
	tty.c_cflag &= ~PARENB;
	//Kr�v enbart en stop bit
	tty.c_cflag &= ~CSTOPB;
	//Ingen hardware flowcontrol
	tty.c_cflag &= ~CRTSCTS;  
	//St�ng av CANONICAL INPUT
	tty.c_lflag &= ~ICANON 
	//St�ng av Interrupts
	tty.c_lflag &= ~ISIG; 
	//St�ng av ECHO for shits and giggles
	tty.c_lflag &= ~(ECHO | ECHOE | ECHONL | IEXTEN);
	//Ignorera inte carriage return
	tty.c_iflag &= ~IGNCR; 
	//Disabla input parity
	tty.c_iflag &= ~INPCK;
	//Disable Translations och posix stuff
	tty.c_iflag &= ~(INLCR | ICRNL | IUCLC | IMAXBEL);
	//Disabla XON/XOFF flow control, restarta ej stoppad output
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);  
	//Disable output processing
	tty.c_oflag &= ~OPOST;
	
	///////////////////////
	//IDK OM DESSA BEH�VS//
	///////////////////////
	//S�tt ej end of line characters
	tty.c_cc[VEOL] = 0;
	//S�tt ej end of line characters 2.0 electric boogaloo
	tty.c_cc[VEOL2] = 0;
	//S�tt ej end of line characters the third.
	tty.c_cc[VEOF] = 0x04;
	
	//TCSANOW = immediate change
	if(tcsetattr(fd, TCSANOW, &tty) != 0)
	{
		//Printa attribute error
		printf(strerror(errno));
		return;
	}
	return;
}

void readPort(void *arg)
{
	//Read from serial port
	//F� LAMPSTATUS
	uint8_t data = read(1, &stdin, NULL, NULL, NULL, NULL);
	
	uint8_t signal = 0;
}

void writePort(uint8_t data, int speed)
{
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
	do
	{
	if(c == 'n')
	{
		arrivalSensor(1);
	}
	else if (c == 's')
	{
		arrivalSensor(0);
	}while((c=getchar()) != 'q')
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

