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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Simulator.h"


#define SERIAL_PORT "/dev/ttyS0"

enum Z {
	//NONE = 0,
	RED = 0,
	GREEN = 1
};

int yeet; // yeet blir vår filedescriptor tror jag
//Trafikljusen
int LightNorth; //0 = Röd; 1 = Grön.
int LightSouth; //0 = Röd; 1 = Grön.
int writeBit[4] = {0, 0, 0, 0};
int southQ;
int northQ;
int bridgecnt;
int speed = 96000;
enum Z light = RED;
pthread_t readinputs;
pthread_t sim;
pthread_t readinputs;

/*void openPort()
{
	//Open serial port
	//yeet = open(SERIAL_PORT)
}
*/

/*
Bit 0: Northbound green light status
Bit 1: Northbound red light status
Bit 2: Southbound green light status
Bit 3: Southbound red light status
*/

int openPort()
{
	//Simulera termios

	/*int fd = open("dev/ttyS0", O_RDWR|O_NOCTTY|O_SYNC);
	struct termios tty;
	if(tcgetattr(fd, &tty) < 0)
	{
		//Skriver ut felkoden fr�n tcgetattr
		printf(strerror(errno));
		return -1;
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
	tty.c_lflag &= ~ICANON; 
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
		return -1;
	}
	return fd;
	*/
	int fd = open("/dev/ttyS0", O_RDWR);
	if (fd < 0) {
		printf("Can't open file! \n");
	}


	struct termios termios_p;
	if (tcgetattr(fd, &termios_p)) {
		printf("Couldn't get termios attributes! \n");
		return -1;
	}


	tcflush(fd, TCIFLUSH); //Flush COM1 recieved data that is not read
	//Baud rate(signal changes per second) = 9600 | Character size mask 8 | Set two stop bits rather than one | Enable receiver | Ignore modem control lines | Lower modem control lines after last process closes the device (hang up) | Enable input parity checking
	termios_p.c_cflag = B9600 | CS8 | CSTOPB | CREAD | CLOCAL | HUPCL | INPCK;
	// NOT (Echo input characters | echo the NL character even if ECHO is not set)
	termios_p.c_lflag &= ~(ECHO | ECHONL | ICANON);
	// Avoid inter-message overlap.
	termios_p.c_cc[VTIME] = 10;
	termios_p.c_cc[VMIN] = 1;
	cfsetispeed(&termios_p, B9600);
	cfsetospeed(&termios_p, B9600);


	if (tcsetattr(fd, TCSANOW, &termios_p)) {
		printf("Could not set termios attributes! \n");
		return -1;
	}
	return fd;
}

void *readPort(void *arg)
{
	//Read from serial port
	//F� LAMPSTATUS
	uint8_t c;
	while(1)
	{
		int readBytes = read(yeet, &c, sizeof(c));
		//	uint8_t signal = 0;
		if(readBytes > 0)
		{
			printf("%d", (int)c);
			printf("\n");
			break;
			//printf("%d", readBytes);
			//printf("\n");
		}
	}
}

void writePort(uint8_t data)
{
	int bytes_written = write(yeet, &data, 1);
//	int yeet = printf(yeet, &data, sizeof(data));
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
	// här ska vi köra select tror jag
	int retval = 1; // select(1, &stdin, yeet, NULL, NULL);
//	do
	if(retval){
		c = getchar();
		if(c == 'n'){
			printf("%c", c);
			writePort(0xFF);
		}
	}
	/*{
	if(c == 'n')
	{
		arrivalSensor(1);
		writePort();
	}
	else if (c == 's')
	{
		arrivalSensor(0);
		writePort();
	}while((c=getchar()) != 'q')*/
}

//Uppdatera bron och se om det finns n�got p� den
void enterBridge()
{
	bridgecnt++;
	// do some gui magic
	// skriv till avr
	sleep(5);
	bridgecnt--;
}

void GUI()
{
	while(1)
	{

		//Cleara sk�rmen?
		printf("\e[1;1H\e[2J");
		if(LightNorth)
		{
		printf("North: GREEN");
		}
		if(!LightNorth)
		{
		printf("North: RED");
		}
		printf("\n");
		printf("NorthQ: %d", NorthQ);
		printf("\n");
		if(LightSouth)
		{
		printf("South: GREEN");
		}
		if(!LightSouth)
		{
		printf("South: RED");
		}
		printf("\n");
		printf("SouthQ: %d", SouthQ);
		printf("\n");
		msleep(100);
	}
}


/*
Om något åker in i bron
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

void simulator(void){
	northQ = 0;
	southQ = 0;
	bridgecnt = 0;
	while(1){

		if(LightNorth && (northQ > 0)){
			northQ--;
			pthread_t cartobridge;
			pthread_create(&cartobridge, NULL, enterBridge, NULL);
			entrySensor(1);
//			antingen fixar denna gui eller så tar bron den
//			enterBridge();
			sleep(1);
		}

		if(LightSouth && (southQ > 0)){
			southQ--;
			pthread_t cartobridge;
			pthread_create(&cartobridge, NULL, enterBridge, NULL);
//			enterBridge();
			entrySensor(0):
			sleep(1);
		}

		if(LightNorth && LightSouth){
			printf("ohell, nu är båda gröna");
		}
	}
}

void* Simulator(void *arg)
{
	while(1)
	{
		//D� s�der ljus �r gr�n (1) och det finns bilar i k�n
		while( (SouthQ > 0) && LightSouth == 1 && LightNorth != 1)
		{
			SouthQ--;
			pthread_t drive;
			pthread_create(&drive, NULL, updateBridge, NULL);
			writePort(0x8);
			sleep(1);
		}
		while( (NorthQ > 0) && LightNorth == 1 && LightSouth != 1)
		{
			SouthQ--;
			pthread_t drive;
			pthread_create(&drive, NULL, updateBridge, NULL);
			writePort(0x2);
			sleep(1);
		}
		//0x8 ska vara south, 0x2 ska vara north.
		fflush(stdin);
	}
}

void* updateBridge(void *arg)
{
	bridgecnt++;
	sleep(5);
	bridgecnt--;
	pthread_exit(0);
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
		writeBit[2] = 1;
	}
}

int main(void)
{
	yeet = openPort();
	uint8_t z = 15;
//	pthread_create(&readz, NULL, readPort, &z);
	char f = getchar();
	int fz;
	switch((int)f)
	{
		case((int)'1'):
			fz = 1;
			break;
		case((int)'2'):
			fz = 2;
			break;
		case((int)'3'):
			fz = 3;
			break;
		case((int)'4'):
			fz = 4;
			break;
		case((int)'5'):
			fz = 5;
			break;
		case((int)'6'):
			fz = 6;
			break;
		case((int)'7'):
			fz = 7;
			break;
		case((int)'8'):
			fz = 8;
			break;
		case((int)'9'):
			fz = 9;
			break;
		default:
			fz = 0;
			break;
	}
	writePort(fz);
	readPort(&z);
	/*int x = write(yeet, &z, 1);
	printf("x = %d",x);
	printf("\n");
	if(x<0)
	{
		printf("oheeeeell\n");
	}
	*/
//	readPort(&z);
	
}

